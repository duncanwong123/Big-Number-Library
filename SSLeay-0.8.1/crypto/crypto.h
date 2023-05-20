/* crypto/crypto.h */
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

#ifndef HEADER_CRYPTO_H
#define HEADER_CRYPTO_H


/* This is more to be used to check the correct DLL is being used
 * in the MS world. */
//#define SSLEAY_VERSION_NUMBER	0x0800	/* Version 0.5.1c would be 0513 */
//#define SSLEAY_VERSION		0
//#define SSLEAY_CFLAGS		2
//#define SSLEAY_BUILT_ON		3

/* When changing the CRYPTO_LOCK_* list, be sure to maintin the text lock
 * names in cryptlib.c
 */
/*
#define	CRYPTO_LOCK_ERR			1
#define	CRYPTO_LOCK_ERR_HASH		2
#define CRYPTO_LOCK_DSA			8
#define CRYPTO_LOCK_RSA			9
#define CRYPTO_LOCK_EVP_PKEY		10
#define	CRYPTO_LOCK_RAND		16
#define	CRYPTO_LOCK_MALLOC		17
#define	CRYPTO_LOCK_BIO			18
#define	CRYPTO_LOCK_BIO_GETHOSTBYNAME	19
#define	CRYPTO_NUM_LOCKS		20

#define CRYPTO_LOCK		1
#define CRYPTO_UNLOCK		2
#define CRYPTO_READ		4
#define CRYPTO_WRITE		8
*/

#define Malloc         MemPtrNew
#define Realloc(a,n)   PalmOS_realloc(a,n)     // in bn_lib.c
#define FreeFunc       MemPtrFree
#define Free(addr)     MemPtrFree(addr)

#define sprintf StrPrintF

void *memset(void *b, int c, int len);         // in bn_lib.c
void *memcpy(void *dst, void *src, int len);   // in bn_lib.c
void *PalmOS_realloc(void *ptr, unsigned long size); // in bn_lib.c

/*
char *SSLeay_version(int type);
unsigned long SSLeay(void);

int CRYPTO_mem_ctrl(int mode);
void CRYPTO_lock(int mode, int type,char *file,int line);
void CRYPTO_set_locking_callback(void (*func)(int mode,int type,char *file,
		int line));
void (*CRYPTO_get_locking_callback(void))(int mode,int type,char *file,
		int line);
void CRYPTO_set_add_lock_callback(int (*func)(int *num,int mount,
		int type,char *file, int line));
int (*CRYPTO_get_add_lock_callback(void))(int *num,int mount,
		int type,char *file,int line);
void CRYPTO_set_id_callback(unsigned long (*func)(void));
unsigned long (*CRYPTO_get_id_callback(void))(void);
unsigned long CRYPTO_thread_id(void);
char *CRYPTO_get_lock_name(int type);
int CRYPTO_add_lock(int *pointer,int amount,int type, char *file,int line);

void CRYPTO_set_mem_functions(char *(*m)(),char *(*r)(), void (*free_func)());
void CRYPTO_get_mem_functions(char *(**m)(),char *(**r)(), void (**f)());

char *CRYPTO_malloc(int num);
char *CRYPTO_realloc(char *addr,int num);
void CRYPTO_free(char *);
char *CRYPTO_remalloc(char *addr,int num);

char *CRYPTO_dbg_malloc(int num,char *file,int line);
char *CRYPTO_dbg_realloc(char *addr,int num,char *file,int line);
void CRYPTO_dbg_free(char *);
char *CRYPTO_dbg_remalloc(char *addr,int num,char *file,int line);
*/

#endif
