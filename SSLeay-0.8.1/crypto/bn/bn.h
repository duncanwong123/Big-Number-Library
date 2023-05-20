/* crypto/bn/bn.org */
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

/* WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING 
 *
 * Always modify bn.org since bn.h is automatically generated from
 * it during SSLeay configuration.
 *
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 */

#ifndef HEADER_BN_H
#define HEADER_BN_H

#define BN_LLONG

#define RECP_MUL_MOD
#define MONT_MUL_MOD

/* Only one for the following should be defined */
/* The prime number generation stuff may not work when
 * EIGHT_BIT but I don't care since I've only used this mode
 * for debuging the bignum libraries */
//#define SIXTY_FOUR_BIT
//#define THIRTY_TWO_BIT
//#define SIXTEEN_BIT
#define EIGHT_BIT

#ifdef THIRTY_TWO_BIT
#define BN_ULLONG	unsigned long long
#define BN_ULONG	unsigned long
#define BN_LONG		long
#define BN_BITS		64
#define BN_BYTES	4
#define BN_BITS2	32
#define BN_BITS4	16
#define BN_MASK2	(0xffffffffL)
#define BN_MASK2l	(0xffff)
#define BN_MASK2h1	(0xffff8000L)
#define BN_MASK2h	(0xffff0000L)
#define BN_TBIT		(0x80000000L)
#endif

#ifdef SIXTEEN_BIT
#ifndef BN_DIV2W
#define BN_DIV2W
#endif
#define BN_ULLONG	unsigned long
#define BN_ULONG	unsigned short
#define BN_LONG		short
#define BN_BITS		32
#define BN_BYTES	2
#define BN_BITS2	16
#define BN_BITS4	8
#define BN_MASK2	(0xffff)
#define BN_MASK2l	(0xff)
#define BN_MASK2h1	(0xff80)
#define BN_MASK2h	(0xff00)
#define BN_TBIT		(0x8000)
#endif

#ifdef EIGHT_BIT
#ifndef BN_DIV2W
#define BN_DIV2W
#endif
#define BN_ULLONG	unsigned short
#define BN_ULONG	unsigned char
#define BN_LONG		char
#define BN_BITS		16
#define BN_BYTES	1
#define BN_BITS2	8
#define BN_BITS4	4
#define BN_MASK2	(0xff)
#define BN_MASK2l	(0xf)
#define BN_MASK2h1	(0xf8)
#define BN_MASK2h	(0xf0)
#define BN_TBIT		(0x80)
#endif

#define BN_DEFAULT_BITS	1280

#ifdef BIGNUM
#undef BIGNUM
#endif

typedef struct bignum_st
	{
	BN_ULONG *d;	/* Pointer to an array of 'BN_BITS2' bit chunks. */
	int top;	/* Index of last used d +1. */
	/* The next are internal book keeping for bn_expand. */
	int max;	/* Size of the d array. */
	int neg;	/* one if the number is negative */
	} BIGNUM;

/* Used for temp variables */
#define BN_CTX_NUM	12
typedef struct bignum_ctx
	{
	int tos;
	BIGNUM *bn[BN_CTX_NUM+1];
	} BN_CTX;

/* Used for montgomery multiplication */
typedef struct bn_mont_ctx_st
        {
        int ri;         /* number of bits in R */
        BIGNUM *RR;     /* used to convert to montgomery form */
        BIGNUM *N;      /* The modulus */
        BIGNUM *Ni;     /* The inverse of N */
	BN_ULONG n0;	/* word form of inverse, normally only one of
			 * Ni or n0 is defined */
        } BN_MONT_CTX;

#define BN_to_montgomery(r,a,mont,ctx)	BN_mod_mul_montgomery(\
	r,a,(mont)->RR,(mont),ctx)

#define BN_prime_checks		(5)

#define BN_num_bytes(a)	((BN_num_bits(a)+7)/8)
#define BN_is_word(a,w)	(((a)->top == 1) && ((a)->d[0] == (BN_ULONG)(w)))
#define BN_is_zero(a)	(((a)->top <= 1) && ((a)->d[0] == (BN_ULONG)0))
#define BN_is_one(a)	(BN_is_word((a),1))
#define BN_is_odd(a)	((a)->d[0] & 1)
#define BN_one(a)	(BN_set_word((a),1))
#define BN_zero(a)	(BN_set_word((a),0))

#define bn_fix_top(a) \
	{ \
	BN_ULONG *fix_top_l; \
	for (fix_top_l= &((a)->d[(a)->top-1]); (a)->top > 0; (a)->top--) \
		if (*(fix_top_l--)) break; \
	}

#define bn_expand(n,b) ((((b)/BN_BITS2) <= (n)->max)?(n):bn_expand2((n),(b)))


BIGNUM *BN_value_one(void);
char *BN_options(void);
BN_CTX *BN_CTX_new(void);
void	BN_CTX_free(BN_CTX *c);
int   BN_rand(BIGNUM *rnd, int bits, int top,int bottom);
int	BN_num_bits(BIGNUM *a);
int	BN_num_bits_word(BN_ULONG l);
BIGNUM *BN_new(void);
void	BN_clear_free(BIGNUM *a);
BIGNUM *BN_copy(BIGNUM *a, BIGNUM *b);
BIGNUM *BN_bin2bn(unsigned char *s,int len,BIGNUM *ret);
int	BN_bn2bin(BIGNUM *a, unsigned char *to);
int	BN_sub(BIGNUM *r, BIGNUM *a, BIGNUM *b);
void	bn_qsub(BIGNUM *r, BIGNUM *a, BIGNUM *b);
void	bn_qadd(BIGNUM *r, BIGNUM *a, BIGNUM *b);
int	BN_add(BIGNUM *r, BIGNUM *a, BIGNUM *b);
int	BN_mod(BIGNUM *rem, BIGNUM *m, BIGNUM *d, BN_CTX *ctx);
int	BN_div(BIGNUM *dv, BIGNUM *rem, BIGNUM *m, BIGNUM *d, BN_CTX *ctx);
int	BN_mul(BIGNUM *r, BIGNUM *a, BIGNUM *b);
int	BN_sqr(BIGNUM *r, BIGNUM *a,BN_CTX *ctx);
BN_ULONG BN_mod_word(BIGNUM *a, BN_ULONG w);
BN_ULONG BN_div_word(BIGNUM *a, BN_ULONG w);
int	BN_add_word(BIGNUM *a, BN_ULONG w);
int	BN_set_word(BIGNUM *a, unsigned long w);
unsigned long BN_get_word(BIGNUM *a);
int	BN_cmp(BIGNUM *a, BIGNUM *b);
void	BN_free(BIGNUM *a);
int	BN_is_bit_set(BIGNUM *a, int n);
int	BN_lshift(BIGNUM *r, BIGNUM *a, int n);
int	BN_lshift1(BIGNUM *r, BIGNUM *a);
int	BN_mod_exp(BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx);
int	BN_mod_exp_mont(BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx);
int	BN_mod_exp_recp(BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx);
int	BN_mod_exp_simple(BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx);
int	BN_mask_bits(BIGNUM *a,int n);
int	BN_mod_mul_reciprocal(BIGNUM *r, BIGNUM *x, BIGNUM *y, BIGNUM *m, BIGNUM *i, int nb, BN_CTX *ctx);
int	BN_mod_mul(BIGNUM *ret, BIGNUM *a, BIGNUM *b, BIGNUM *m, BN_CTX *ctx);
int	BN_reciprocal(BIGNUM *r, BIGNUM *m, BN_CTX *ctx);
int	BN_rshift(BIGNUM *r, BIGNUM *a, int n);
int	BN_rshift1(BIGNUM *r, BIGNUM *a);
void	BN_clear(BIGNUM *a);
BIGNUM *bn_expand2(BIGNUM *b, int bits);
BIGNUM *BN_dup(BIGNUM *a);
int	BN_ucmp(BIGNUM *a, BIGNUM *b);
int	BN_set_bit(BIGNUM *a, int n);
int	BN_clear_bit(BIGNUM *a, int n);
char *	BN_bn2ascii(BIGNUM *a);
int 	BN_ascii2bn(BIGNUM **a,char *str);
int	BN_gcd(BIGNUM *r,BIGNUM *in_a,BIGNUM *in_b,BN_CTX *ctx);
BIGNUM *BN_mod_inverse(BIGNUM *a, BIGNUM *n,BN_CTX *ctx);
BIGNUM *BN_generate_prime(int bits,int strong,BIGNUM *add, BIGNUM *rem,void (*callback)(int,int));
int	BN_is_prime(BIGNUM *p,int nchecks,void (*callback)(int,int),BN_CTX *ctx);
BN_ULONG bn_mul_add_word(BN_ULONG *rp, BN_ULONG *ap, int num, BN_ULONG w);
BN_ULONG bn_mul_word(BN_ULONG *rp, BN_ULONG *ap, int num, BN_ULONG w);
void     bn_sqr_words(BN_ULONG *rp, BN_ULONG *ap, int num);
BN_ULONG bn_div64(BN_ULONG h, BN_ULONG l, BN_ULONG d);
BN_MONT_CTX *BN_MONT_CTX_new(void );
int BN_mod_mul_montgomery(BIGNUM *r,BIGNUM *a,BIGNUM *b,BN_MONT_CTX *mont,BN_CTX *ctx);
int BN_from_montgomery(BIGNUM *r,BIGNUM *a,BN_MONT_CTX *mont,BN_CTX *ctx);
void BN_MONT_CTX_free(BN_MONT_CTX *mont);
int BN_MONT_CTX_set(BN_MONT_CTX *mont,BIGNUM *modulus,BN_CTX *ctx);

/* BEGIN ERROR CODES */
/* Error codes for the BN functions. */

/* Function codes. */
#define BN_F_BN_BL_CTX_INIT				 100
#define BN_F_BN_BL_CTX_NEW				 101
#define BN_F_BN_BN2ASCII				 102
#define BN_F_BN_CTX_NEW					 103
#define BN_F_BN_DIV					 104
#define BN_F_BN_EXPAND2					 105
#define BN_F_BN_MOD_EXP_MONT				 106
#define BN_F_BN_MOD_INVERSE				 107
#define BN_F_BN_MOD_MUL_RECIPROCAL			 108
#define BN_F_BN_NEW					 109
#define BN_F_BN_RAND					 110

/* Reason codes. */
#define BN_R_BAD_RECIPROCAL				 100
#define BN_R_CALLED_WITH_EVEN_MODULUS			 101
#define BN_R_DIV_BY_ZERO				 102
#define BN_R_NO_INVERSE					 103
 
#endif
