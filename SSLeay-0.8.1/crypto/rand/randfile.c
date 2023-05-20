/* crypto/rand/randfile.c */
/* Copyright (C) 1995-1997 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

#ifdef __palmos__
#include "cryptlib.h"
#define RAND_DATA 1024 /* The amount of data to save, in bytes */

struct dbinfo {
    UInt cardNo;
    LocalID dbID;
    Char name[32];
    UInt attributes;
    UInt version;
    ULong crDate;
    ULong modDate;
    ULong bckUpDate;
    ULong modNum;
    LocalID appInfoID;
    LocalID sortInfoID;
    ULong type;
    ULong creator;
};

/* Use the contents of a database to seed the PRNG */
int RAND_load_db(LocalID lid, long bytes)
{
    long ret = 0;
    struct dbinfo di;
    Err err;
    UInt index, numrecs;
    DmOpenRef db;
    VoidHand rhand;
    unsigned char *rptr;
    ULong rsize;
    long len;

    /* Do a "stat" */
    di.cardNo = 0;
    di.dbID = lid;
    DmDatabaseInfo(di.cardNo, di.dbID, di.name, &di.attributes, &di.version,
		    &di.crDate, &di.modDate, &di.bckUpDate, &di.modNum,
		    &di.appInfoID, &di.sortInfoID, &di.type, &di.creator);
    len = bytes - ret;
    if (len > sizeof(di)) len = sizeof(di);
    RAND_seed((unsigned char *)&di, len);
    ret += len;

    if (di.attributes & dmHdrAttrResDB) {
	/* If it's a resource database, read the resources */
	db = DmOpenDatabase(0, lid, dmModeReadOnly);
	if (db) {
	    numrecs = DmNumResources(db);
	    for(index=0; index < numrecs && ret < bytes; ++index) {
		ULong type;
		Int id;
		err = DmResourceInfo(db, index, &type, &id, NULL);
		if (!err) {
		    rhand = DmGet1Resource(type, id);
		    if (rhand) {
			rptr = MemHandleLock(rhand);
			rsize = MemHandleSize(rhand);
			len = bytes - ret;
			if (len > rsize) len = rsize;
			RAND_seed(rptr, len);
			MemHandleUnlock(rhand);
			ret += len;
		    }
		    DmReleaseResource(rhand);
		}
	    }
	    DmCloseDatabase(db);
	}
    } else {
	/* If it's not a resource database, read the records */
	db = DmOpenDatabase(0, lid, dmModeReadOnly);
	if (db) {
	    numrecs = DmNumRecords(db);
	    for(index=0; index < numrecs && ret < bytes; ++index) {
		rhand = DmQueryRecord(db, index);
		if (rhand) {
		    rptr = MemHandleLock(rhand);
		    rsize = MemHandleSize(rhand);
		    len = bytes - ret;
		    if (len > rsize) len = rsize;
		    RAND_seed(rptr, len);
		    MemHandleUnlock(rhand);
		    ret += len;
		}
	    }
	    DmCloseDatabase(db);
	}
    }
    return (int)ret;
}

int RAND_load_file(char *file, long bytes)
{
    int ret = 0;
    LocalID lid = DmFindDatabase(0, file);
    if (lid) {
	ret = RAND_load_db(lid, bytes);
    }
    return ret;
}

int RAND_write_file(char *file)
{
    MS_STATIC unsigned char buf[32];
    DmOpenRef db;
    ULong numrec;
    Err err;
    VoidHand rhand;
    unsigned char *rptr;
    int ret;
    LocalID lid = DmFindDatabase(0, file);
    if (!lid) {
	/* Create it then */
	err = DmCreateDatabase(0, file, 'CrRN', 'RAND', false);
	if (!err) {
	    lid = DmFindDatabase(0, file);
	}
    }
    ErrFatalDisplayIf(!lid, "Cannot save RAND state: no database");
    db = DmOpenDatabase(0, lid, dmModeReadWrite);
    ErrFatalDisplayIf(!db, "Cannot save RAND state: cannot write");
    err = DmDatabaseSize(0, lid, &numrec, NULL, NULL);
    ErrFatalDisplayIf(err, "Cannot save RAND state: cannot get database size");
    if (numrec == 0) {
	/* Create a new record */
	UInt at = 0;
	rhand = DmNewRecord(db, &at, RAND_DATA);
    } else {
	/* Resize the existing first record */
	rhand = DmResizeRecord(db, 0, RAND_DATA);
    }
    ErrFatalDisplayIf(!rhand, "Cannot save RAND state: cannot write record");
    rptr = MemHandleLock(rhand);
    ret = 0;
    while(ret < RAND_DATA) {
	int bleft = RAND_DATA - ret;
	if (bleft > 32) bleft = 32;
	RAND_bytes(buf, bleft);
	DmWrite(rptr, ret, buf, bleft);
	ret += bleft;
    }
    MemHandleUnlock(rhand);
    DmReleaseRecord(db, 0, true);
    DmCloseDatabase(db);
    return ret;
}

char *RAND_file_name(char *buf,int size)
{
    static const char *defname = "RAND Library saved state";
    if (buf && size) {
	if (size < StrLen(defname)+1) {
	    MemMove(buf, (char *)defname, size-1);
	    buf[size] = '\0';
	} else {
	    StrCopy(buf, defname);
	}
    }
    return buf ? buf : (char *)defname;
}
#else
#include <stdio.h>
#include "cryptlib.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "rand.h"

#undef BUFSIZE
#define BUFSIZE	1024
#define RAND_DATA 1024

/* #define RFILE ".rand" - defined in ../../e_os.h */

int RAND_load_file(file,bytes)
char *file;
long bytes;
	{
	MS_STATIC unsigned char buf[BUFSIZE];
	struct stat sb;
	int i,ret=0,n;
	FILE *in;

	if (file == NULL) return(0);

	i=stat(file,&sb);
	/* If the state fails, put some crap in anyway */
	RAND_seed((unsigned char *)&sb,sizeof(sb));
	ret+=sizeof(sb);
	if (i < 0) return(0);
	if (bytes <= 0) return(ret);

	in=fopen(file,"r");
	if (in == NULL) goto err;
	for (;;)
		{
		n=(bytes < BUFSIZE)?(int)bytes:BUFSIZE;
		i=fread(buf,1,n,in);
		if (i <= 0) break;
		/* even if n != i, use the full array */
		RAND_seed(buf,n);
		ret+=i;
		bytes-=n;
		if (bytes <= 0) break;
		}
	fclose(in);
	memset(buf,0,BUFSIZE);
err:
	return(ret);
	}

int RAND_write_file(file)
char *file;
	{
	unsigned char buf[BUFSIZE];
	int i,ret=0;
	FILE *out;
	int n;

	out=fopen(file,"w");
	if (out == NULL) goto err;
	chmod(file,0600);
	n=RAND_DATA;
	for (;;)
		{
		i=(n > BUFSIZE)?BUFSIZE:n;
		n-=BUFSIZE;
		RAND_bytes(buf,i);
		i=fwrite(buf,1,i,out);
		if (i <= 0)
			{
			ret=0;
			break;
			}
		ret+=i;
		if (n <= 0) break;
		}
	fclose(out);
	memset(buf,0,BUFSIZE);
err:
	return(ret);
	}

char *RAND_file_name(buf,size)
char *buf;
int size;
	{
	char *s;
	char *ret=NULL;

	s=getenv("RANDFILE");
	if (s != NULL)
		{
		strncpy(buf,s,size-1);
		buf[size-1]='\0';
		ret=buf;
		}
	else
		{
		s=getenv("HOME");
		if (s == NULL) return(RFILE);
		if (((int)(strlen(s)+strlen(RFILE)+2)) > size)
			return(RFILE);
		strcpy(buf,s);
		strcat(buf,"/");
		strcat(buf,RFILE);
		ret=buf;
		}
	return(ret);
	}
#endif
