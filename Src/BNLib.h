/* BNLib.h - BigNum shared library header file
 *
 * 9/8/00 Duncan S Wong - Initial Port from pilotSSLeay-2.01 (not completed)
 */

// *****
// * PROJECT:		MySharedLib (MSL)
// * FILENAME: 		MySharedLib.h
// * AUTHOR:		Jeff Ishaq 05/21/99
// * 
// * DESCRIPTION:	Shared library functionality interface definition
// *
// * COPYRIGHT:		As long as this 'copyright' is intact, this code is freely modifiable
// *				and distributable.
// *****

#pragma once

// Use this for SysLibFind calls.  This is what we 'name' our dispatch table, too:
#define BN_LIB_NAME	"BNLib"
#define BN_LIB_CREATOR	'DWBN'								// Register this with Palm


// BNLib related settings
#include <Pilot.h>
#ifndef __palmos__
#define __palmos__
#endif
#include "bn.h"


// These are possible error types that MSL might return:
typedef enum tagBNErrEnum
{
	BNErrNone 			= 0,			
	BNErrParam			= -1,
	BNErrNoGlobals		= -2

	/////
	// Your custom return codes go here...
	/////
	
} BNErr;

// These are MSL's trap identifiers.  The PalmOS constant 'sysLibTrapCustom' is
// the first trap number we can use after open, close, sleep, and wake.
typedef enum tagBNTrapNumEnum
{
	/////
	// - Trap modification checklist -
	// 
	// If you add or remove or otherwise modify something here, be sure you've
	// also done all of the following steps!
	//
	// 0) All trap identifiers must always run sequentially; no gaps!
	// 1) Modify the BNTrapNumEnum in MySharedLib.h
	// 2) Modify the DC.W to MSL_DispatchTable() in MySharedLibDispatch.c (no gaps!)
	// 3) Modify the JMP in MSL_DispatchTable() in MySharedLibDispatch.c (no gaps!)
	// 4) ** Update NUMBER_OF_FUNCTIONS in MySharedLibDispatch.c ** (0-based)
	// 5) Add or remove an "extern MyFunc(...) SYS_TRAP(BNTrapMyFunc)" prototype somewhere
	//
	/////

	BNLibTrapBN_CTX_new        = sysLibTrapCustom,		// libDispatchEntry(4)
	BNLibTrapBN_CTX_free,
	BNLibTrapBN_rand,
	BNLibTrapBN_num_bits,
	BNLibTrapBN_new,
	BNLibTrapBN_clear_free,
	BNLibTrapBN_copy,
	BNLibTrapBN_bin2bn,
	BNLibTrapBN_bn2bin,
	BNLibTrapBN_sub,
	BNLibTrapbn_qsub,
	BNLibTrapbn_qadd,
	BNLibTrapBN_add,
	BNLibTrapBN_mod,
	BNLibTrapBN_div,
	BNLibTrapBN_mul,
	BNLibTrapBN_sqr,
	BNLibTrapBN_mod_word,
	BNLibTrapBN_div_word,
	BNLibTrapBN_add_word,
	BNLibTrapBN_set_word,
	BNLibTrapBN_get_word,
	BNLibTrapBN_cmp,
	BNLibTrapBN_free,
	BNLibTrapBN_is_bit_set,
	BNLibTrapBN_lshift,
	BNLibTrapBN_lshift1,
	BNLibTrapBN_mod_exp,
	BNLibTrapBN_mod_exp_recp,
	BNLibTrapBN_mod_exp_simple,
	BNLibTrapBN_mask_bits,
	BNLibTrapBN_mod_mul_reciprocal,
	BNLibTrapBN_mod_mul,
	BNLibTrapBN_reciprocal,
	BNLibTrapBN_rshift,
	BNLibTrapBN_rshift1,
	BNLibTrapBN_clear,
	BNLibTrapBN_dup,
	BNLibTrapBN_ucmp,
	BNLibTrapBN_set_bit,
	BNLibTrapBN_clear_bit,
	BNLibTrapBN_bn2ascii,
	BNLibTrapBN_ascii2bn,
	BNLibTrapBN_gcd,
	BNLibTrapBN_mod_inverse
} BNTrapNumEnum;


#ifdef __cplusplus
extern "C" {
#endif

// These are the four required entry points:
extern BNErr	BNLibOpen( UInt uRefNum )      SYS_TRAP ( sysLibTrapOpen	);
extern BNErr	BNLibClose( UInt uRefNum, DWordPtr dwRefCountP )	SYS_TRAP ( sysLibTrapClose	);
extern Err		BNLibSleep( UInt uRefNum )     SYS_TRAP ( sysLibTrapSleep	);
extern Err		BNLibWake( UInt uRefNum )      SYS_TRAP ( sysLibTrapWake	);

// Here are the actual functions we want the library to extend to callers.
extern BN_CTX *BNLibBN_CTX_new(UInt uRefNum)    SYS_TRAP ( BNLibTrapBN_CTX_new );
extern void	BNLibBN_CTX_free(UInt uRefNum, BN_CTX *c)    SYS_TRAP ( BNLibTrapBN_CTX_free );
extern int  BNLibBN_rand(UInt uRefNum, BIGNUM *rnd, int bits, int top,int bottom)    SYS_TRAP ( BNLibTrapBN_rand );
extern int	BNLibBN_num_bits(UInt uRefNum, BIGNUM *a)    SYS_TRAP ( BNLibTrapBN_num_bits );
extern BIGNUM *BNLibBN_new(UInt uRefNum)    SYS_TRAP ( BNLibTrapBN_new );
extern void	BNLibBN_clear_free(UInt uRefNum, BIGNUM *a)    SYS_TRAP ( BNLibTrapBN_clear_free );
extern BIGNUM *BNLibBN_copy(UInt uRefNum, BIGNUM *a, BIGNUM *b)    SYS_TRAP ( BNLibTrapBN_copy );
extern BIGNUM *BNLibBN_bin2bn(UInt uRefNum, unsigned char *s,int len,BIGNUM *ret)    SYS_TRAP ( BNLibTrapBN_bin2bn );
extern int	BNLibBN_bn2bin(UInt uRefNum, BIGNUM *a, unsigned char *to)    SYS_TRAP ( BNLibTrapBN_bn2bin );
extern int	BNLibBN_sub(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)    SYS_TRAP ( BNLibTrapBN_sub );
extern void	BNLibBN_qsub(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)    SYS_TRAP ( BNLibTrapbn_qsub );
extern void	BNLibBN_qadd(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)    SYS_TRAP ( BNLibTrapbn_qadd );
extern int	BNLibBN_add(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)    SYS_TRAP ( BNLibTrapBN_add );
extern int	BNLibBN_mod(UInt uRefNum, BIGNUM *rem, BIGNUM *m, BIGNUM *d, BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_mod );
extern int	BNLibBN_div(UInt uRefNum, BIGNUM *dv, BIGNUM *rem, BIGNUM *m, BIGNUM *d, BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_div );
extern int	BNLibBN_mul(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)    SYS_TRAP ( BNLibTrapBN_mul );
extern int	BNLibBN_sqr(UInt uRefNum, BIGNUM *r, BIGNUM *a,BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_sqr );
extern BN_ULONG BNLibBN_mod_word(UInt uRefNum, BIGNUM *a, unsigned long w)    SYS_TRAP ( BNLibTrapBN_mod_word );
extern BN_ULONG BNLibBN_div_word(UInt uRefNum, BIGNUM *a, unsigned long w)    SYS_TRAP ( BNLibTrapBN_div_word );
extern int	BNLibBN_add_word(UInt uRefNum, BIGNUM *a, unsigned long w)    SYS_TRAP ( BNLibTrapBN_add_word );
extern int	BNLibBN_set_word(UInt uRefNum, BIGNUM *a, unsigned long w)    SYS_TRAP ( BNLibTrapBN_set_word );
extern unsigned long BNLibBN_get_word(UInt uRefNum, BIGNUM *a)    SYS_TRAP ( BNLibTrapBN_get_word );
extern int	BNLibBN_cmp(UInt uRefNum, BIGNUM *a, BIGNUM *b)    SYS_TRAP ( BNLibTrapBN_cmp );
extern void	BNLibBN_free(UInt uRefNum, BIGNUM *a)    SYS_TRAP ( BNLibTrapBN_free );
extern int	BNLibBN_is_bit_set(UInt uRefNum, BIGNUM *a, int n)    SYS_TRAP ( BNLibTrapBN_is_bit_set );
extern int	BNLibBN_lshift(UInt uRefNum, BIGNUM *r, BIGNUM *a, int n)    SYS_TRAP ( BNLibTrapBN_lshift );
extern int	BNLibBN_lshift1(UInt uRefNum, BIGNUM *r, BIGNUM *a)    SYS_TRAP ( BNLibTrapBN_lshift1 );
extern int	BNLibBN_mod_exp(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_mod_exp );
extern int	BNLibBN_mod_exp_recp(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_mod_exp_recp );
extern int	BNLibBN_mod_exp_simple(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_mod_exp_simple );
extern int	BNLibBN_mask_bits(UInt uRefNum, BIGNUM *a,int n)    SYS_TRAP ( BNLibTrapBN_mask_bits );
extern int	BNLibBN_mod_mul_reciprocal(UInt uRefNum, BIGNUM *r, BIGNUM *x, BIGNUM *y, BIGNUM *m, BIGNUM *i, int nb, BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_mod_mul_reciprocal );
extern int	BNLibBN_mod_mul(UInt uRefNum, BIGNUM *ret, BIGNUM *a, BIGNUM *b, BIGNUM *m, BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_mod_mul );
extern int	BNLibBN_reciprocal(UInt uRefNum, BIGNUM *r, BIGNUM *m, BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_reciprocal );
extern int	BNLibBN_rshift(UInt uRefNum, BIGNUM *r, BIGNUM *a, int n)    SYS_TRAP ( BNLibTrapBN_rshift );
extern int	BNLibBN_rshift1(UInt uRefNum, BIGNUM *r, BIGNUM *a)    SYS_TRAP ( BNLibTrapBN_rshift1 );
extern void	BNLibBN_clear(UInt uRefNum, BIGNUM *a)    SYS_TRAP ( BNLibTrapBN_clear );
extern BIGNUM *BNLibBN_dup(UInt uRefNum, BIGNUM *a)    SYS_TRAP ( BNLibTrapBN_dup );
extern int	BNLibBN_ucmp(UInt uRefNum, BIGNUM *a, BIGNUM *b)    SYS_TRAP ( BNLibTrapBN_ucmp );
extern int	BNLibBN_set_bit(UInt uRefNum, BIGNUM *a, int n)    SYS_TRAP ( BNLibTrapBN_set_bit );
extern int	BNLibBN_clear_bit(UInt uRefNum, BIGNUM *a, int n)    SYS_TRAP ( BNLibTrapBN_clear_bit );
extern char *BNLibBN_bn2ascii(UInt uRefNum, BIGNUM *a)    SYS_TRAP ( BNLibTrapBN_bn2ascii );
extern int 	BNLibBN_ascii2bn(UInt uRefNum, BIGNUM **a,char *str)    SYS_TRAP ( BNLibTrapBN_gcd );
extern int	BNLibBN_gcd(UInt uRefNum, BIGNUM *r,BIGNUM *in_a,BIGNUM *in_b,BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_gcd );
extern BIGNUM *BNLibBN_mod_inverse(UInt uRefNum, BIGNUM *a, BIGNUM *n,BN_CTX *ctx)    SYS_TRAP ( BNLibTrapBN_mod_inverse );

#ifdef __cplusplus
}
#endif