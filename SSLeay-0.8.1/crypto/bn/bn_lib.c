/* crypto/bn/bn_lib.c */
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

#include <Pilot.h>
#include "cryptlib.h"
#include "bn_lcl.h"

char *BN_version="Big Number part of SSLeay 0.8.1 19-Jul-1997";

// support overlapping buffers
void *memcpy(void *dst, void *src, int len)
{
int i;
unsigned char *tmp, *P1, *P2;

	if(len) {
		tmp = (unsigned char *) MemPtrNew(len);
		P1 = (unsigned char *) tmp;
		P2 = (unsigned char *) src;

		for(i=0; i<len; i++) {
			*P1 = *P2;
			P1++;
			P2++;
		}

		P1 = (unsigned char *) dst;
		P2 = tmp;
		for(i=0; i<len; i++) {
			*P1 = *P2;
			P1++;
			P2++;
		}

		MemPtrFree(tmp);

		return dst;
	}

	return NULL;
}

void *memset(void *b, int c, int len)
{
	MemSet(b, len, c);
	return b;
}

/* PalmOS doesn't provide a good equivalent to realloc, so we roll our own. */
void *PalmOS_realloc(void *ptr, unsigned long size)
{
Err err;
unsigned long oldsize, copysize;
void *newptr;

  /* Remember the special cases */
	if (!size) {
		MemPtrFree(ptr);
		return NULL;
	}
    if (!ptr) {
	return MemPtrNew(size);
    }

    /* First try a simple Resize.  This will only work if the ptr doesn't
       have to move. */
    err = MemPtrResize(ptr, size);
    if (!err) return ptr;

    /* Darn.  Let's play a game. */
    oldsize = MemPtrSize(ptr);
    copysize = (oldsize < size) ? oldsize : size;
    newptr = MemPtrNew(size);
    if (!newptr) return newptr;  /* Note that ptr is still valid */

    MemMove(newptr, ptr, copysize);
    MemPtrFree(ptr);
    return newptr;
}


/*
 * 9/8/00 Duncan S Wong - The following code is more robust than the original one
 */
BIGNUM *BN_value_one()
	{
BN_ULONG data_one=1;
BIGNUM *const_one;

	const_one = (BIGNUM *) Malloc(sizeof(BIGNUM));

	const_one->d = &data_one;
	const_one->top = 1;
	const_one->max = 1;
	const_one->neg = 0;

	return const_one;
	}

char *BN_options()
	{
	static int init=0;
	static char data[16];

	if (!init)
		{
		init++;
#ifdef BN_LLONG
		sprintf(data,"bn(%d,%d)",(int)sizeof(BN_ULLONG)*8,
			(int)sizeof(BN_ULONG)*8);
#else
		sprintf(data,"bn(%d,%d)",(int)sizeof(BN_ULONG)*8,
			(int)sizeof(BN_ULONG)*8);
#endif
		}
	return(data);
	}

int BN_num_bits_word(BN_ULONG l)
	{
	char bits[256]={
		0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		};

#ifdef SIXTY_FOUR_BIT_LONG
	if (l & 0xffffffff00000000L)
		{
		if (l & 0xffff000000000000L)
			{
			if (l & 0xff00000000000000L)
				{
				return(bits[l>>56]+56);
				}
			else	return(bits[l>>48]+48);
			}
		else
			{
			if (l & 0x0000ff0000000000L)
				{
				return(bits[l>>40]+40);
				}
			else	return(bits[l>>32]+32);
			}
		}
	else
#else
#ifdef SIXTY_FOUR_BIT
	if (l & 0xffffffff00000000LL)
		{
		if (l & 0xffff000000000000LL)
			{
			if (l & 0xff00000000000000LL)
				{
				return(bits[l>>56]+56);
				}
			else	return(bits[l>>48]+48);
			}
		else
			{
			if (l & 0x0000ff0000000000LL)
				{
				return(bits[l>>40]+40);
				}
			else	return(bits[l>>32]+32);
			}
		}
	else
#endif
#endif
		{
#if defined(THIRTY_TWO_BIT) || defined(SIXTY_FOUR_BIT) || defined(SIXTY_FOUR_BIT_LONG)
		if (l & 0xffff0000L)
			{
			if (l & 0xff000000L)
				return(bits[l>>24L]+24);
			else	return(bits[l>>16L]+16);
			}
		else
#endif
			{
#if defined(SIXTEEN_BIT) || defined(THIRTY_TWO_BIT) || defined(SIXTY_FOUR_BIT) || defined(SIXTY_FOUR_BIT_LONG)
			if (l & 0xff00L)
				return(bits[l>>8]+8);
			else	
#endif
				return(bits[l   ]  );
			}
		}
	}

int BN_num_bits(BIGNUM *a)
	{
	BN_ULONG l;
	int i;

	if (a->top == 0) return(0);
	l=a->d[a->top-1];
	i=(a->top-1)*BN_BITS2;
	if (l == 0)
		{
		ErrFatalDisplayIf(1, "BAD TOP VALUE");
		}
	return(i+BN_num_bits_word(l));
	}

void BN_clear_free(BIGNUM *a)
	{
	if (a == NULL) return;
	if (a->d != NULL)
		{
		memset(a->d,0,a->max*sizeof(a->d[0]));
		Free(a->d);
		}
	memset(a,0,sizeof(BIGNUM));
	Free(a);
	}

void BN_free(BIGNUM *a)
	{
	if (a == NULL) return;
	if (a->d != NULL) Free(a->d);
	Free(a);
	}

BIGNUM *BN_new()
	{
	BIGNUM *ret;
	BN_ULONG *p;

	ret=(BIGNUM *)Malloc(sizeof(BIGNUM));
	if (ret == NULL) goto err;
	ret->top=0;
	ret->neg=0;
	ret->max=(BN_DEFAULT_BITS/BN_BITS2);
	p=(BN_ULONG *)Malloc(sizeof(BN_ULONG)*(ret->max+1));
	if (p == NULL) goto err;
	ret->d=p;

	memset(p,0,(ret->max+1)*sizeof(p[0]));
	return(ret);
err:
	BNerr(BN_F_BN_NEW,ERR_R_MALLOC_FAILURE);
	return(NULL);
	}

BN_CTX *BN_CTX_new()
	{
	BN_CTX *ret;
	BIGNUM *n;
	int i,j;

	ret=(BN_CTX *)Malloc(sizeof(BN_CTX));
	if (ret == NULL) goto err2;

	for (i=0; i<BN_CTX_NUM; i++)
		{
		n=BN_new();
		if (n == NULL) goto err;
		ret->bn[i]=n;
		}

	/* There is actually an extra one, this is for debugging my
	 * stuff */
	ret->bn[BN_CTX_NUM]=NULL;

	ret->tos=0;
	return(ret);
err:
	for (j=0; j<i; j++)
		BN_free(ret->bn[j]);
	Free(ret);
err2:
	BNerr(BN_F_BN_CTX_NEW,ERR_R_MALLOC_FAILURE);
	return(NULL);
	}

void BN_CTX_free(BN_CTX *c)
	{
	int i;

	for (i=0; i<BN_CTX_NUM; i++)
		BN_clear_free(c->bn[i]);
	Free(c);
	}

BIGNUM *bn_expand2(BIGNUM *b, int bits)
	{
	BN_ULONG *p;
	register int n;

	while (bits > b->max*BN_BITS2)
		{
		n=((bits+BN_BITS2-1)/BN_BITS2)*2;
		p=b->d=(BN_ULONG *)Realloc(b->d,sizeof(BN_ULONG)*(n+1));
		if (p == NULL)
			{
			BNerr(BN_F_BN_EXPAND2,ERR_R_MALLOC_FAILURE);
			return(NULL);
			}
		memset(&(p[b->max]),0,((n+1)-b->max)*sizeof(BN_ULONG));
		b->max=n;
		}
	return(b);
	}

BIGNUM *BN_dup(BIGNUM *a)
	{
	BIGNUM *r;

	r=BN_new();
	if (r == NULL) return(NULL);
	return((BIGNUM *)BN_copy(r,a));
	}

BIGNUM *BN_copy(BIGNUM *a, BIGNUM *b)
	{
	if (bn_expand(a,b->top*BN_BITS2) == NULL) return(NULL);
	memcpy(a->d,b->d,sizeof(b->d[0])*b->top);
/*	memset(&(a->d[b->top]),0,sizeof(a->d[0])*(a->max-b->top));*/
	a->top=b->top;
	a->neg=b->neg;
	return(a);
	}

void BN_clear(BIGNUM *a)
	{
	memset(a->d,0,a->max*sizeof(a->d[0]));
	a->top=0;
	a->neg=0;
	}

unsigned long BN_get_word(BIGNUM *a)
	{
	int i,n;
	unsigned long ret=0;

	n=BN_num_bytes(a);
	if (n > sizeof(unsigned long))
#ifdef SIXTY_FOUR_BIT_LONG
		return(BN_MASK2);
#else
		return(0xFFFFFFFFL);
#endif
	for (i=a->top-1; i>=0; i--)
		{
#ifndef SIXTY_FOUR_BIT /* the data item > unsigned long */
		ret<<=BN_BITS4; /* stops the compiler complaining */
		ret<<=BN_BITS4;
#endif
		ret|=a->d[i];
		}
	return(ret);
	}

int BN_set_word(BIGNUM *a, unsigned long w)
	{
	int i,n;
	if (bn_expand(a,sizeof(unsigned long)*8) == NULL) return(0);

	n=sizeof(unsigned long)/BN_BYTES;
	a->neg=0;
	a->top=0;
	a->d[0]=(BN_ULONG)w&BN_MASK2;
	if (a->d[0] != 0) a->top=1;
	for (i=1; i<n; i++)
		{
		/* the following is done instead of
		 * w>>=BN_BITS2 so compilers don't complain
		 * on builds where sizeof(long) == BN_TYPES */
#ifndef SIXTY_FOUR_BIT /* the data item > unsigned long */
		w>>=BN_BITS4;
		w>>=BN_BITS4;
#endif
		a->d[i]=(BN_ULONG)w&BN_MASK2;
		if (a->d[i] != 0) a->top=i+1;
		}
	return(1);
	}

/* ignore negative */
BIGNUM *BN_bin2bn(unsigned char *s, int len, BIGNUM *ret)
	{
	unsigned int i,m;
	unsigned int n;
	BN_ULONG l;

	if (ret == NULL) ret=BN_new();
	if (ret == NULL) return(NULL);
	l=0;
	n=len;
	if (n == 0)
		{
		ret->top=0;
		return(ret);
		}
	if (bn_expand(ret,(int)(n+2)*8) == NULL)
		return(NULL);
	i=((n-1)/BN_BYTES)+1;
	m=((n-1)%(BN_BYTES));
	ret->top=i;
	while (n-- > 0)
		{
		l=(l<<8L)| *(s++);
		if (m-- == 0)
			{
			ret->d[--i]=l;
			l=0;
			m=BN_BYTES-1;
			}
		}
	/* need to call this due to clear byte at top if avoiding
	 * having the top bit set (-ve number) */
	bn_fix_top(ret);
	return(ret);
	}

/* ignore negative */
int BN_bn2bin(BIGNUM *a, unsigned char *to)
	{
	int n,i;
	BN_ULONG l;
	unsigned char *tmpP;

	//CharPtr tmp2P;        // testing

	tmpP = to;
	n=i=BN_num_bytes(a);
	while (i-- > 0)
		{
		l=a->d[i/BN_BYTES];

		/* testing
		tmp2P = MemPtrNew(5);
		StrIToA(tmp2P, l);
		ErrDisplay(tmp2P);
		MemPtrFree(tmp2P);
		*/

		*(tmpP++)=(unsigned char)(l>>(8*(i%BN_BYTES)))&0xff;
		}
	return(n);
	}

int BN_ucmp(BIGNUM *a, BIGNUM *b)
	{
	int i;
	BN_ULONG t1,t2,*ap,*bp;

	i=a->top-b->top;
	if (i != 0) return(i);
	ap=a->d;
	bp=b->d;
	for (i=a->top-1; i>=0; i--)
		{
		t1= ap[i];
		t2= bp[i];
		if (t1 != t2)
			return(t1 > t2?1:-1);
		}
	return(0);
	}

int BN_cmp(BIGNUM *a, BIGNUM *b)
	{
	int i;
	int gt,lt;
	BN_ULONG t1,t2;

	if ((a == NULL) || (b == NULL))
		{
		if (a != NULL)
			return(-1);
		else if (b != NULL)
			return(1);
		else
			return(0);
		}
	if (a->neg != b->neg)
		{
		if (a->neg)
			return(-1);
		else	return(1);
		}
	if (a->neg == 0)
		{ gt=1; lt= -1; }
	else	{ gt= -1; lt=1; }

	if (a->top > b->top) return(gt);
	if (a->top < b->top) return(lt);
	for (i=a->top-1; i>=0; i--)
		{
		t1=a->d[i];
		t2=b->d[i];
		if (t1 > t2) return(gt);
		if (t1 < t2) return(lt);
		}
	return(0);
	}

int BN_set_bit(BIGNUM *a, int n)
	{
	int i,j;

	i=n/BN_BITS2;
	j=n%BN_BITS2;
	if (a->top <= i) return(0);

	a->d[i]|=(1L<<j);
	return(1);
	}

int BN_clear_bit(BIGNUM *a, int n)
	{
	int i,j;

	i=n/BN_BITS2;
	j=n%BN_BITS2;
	if (a->top <= i) return(0);

	a->d[i]&=(~(1L<<j));
	return(1);
	}

int BN_is_bit_set(BIGNUM *a, int n)
	{
	int i,j;

	if (n < 0) return(0);
	i=n/BN_BITS2;
	j=n%BN_BITS2;
	if (a->top <= i) return(0);
	return((a->d[i]&(((BN_ULONG)1)<<j))?1:0);
	}

int BN_mask_bits(BIGNUM *a, int n)
	{
	int b,w;

	w=n/BN_BITS2;
	b=n%BN_BITS2;
	if (w >= a->top) return(0);
	if (b == 0)
		a->top=w;
	else
		{
		a->top=w+1;
		a->d[w]&= ~(BN_MASK2<<b);
		while ((w >= 0) && (a->d[w] == 0))
			{
			a->top--;
			w--;
			}
		}
	return(1);
	}
