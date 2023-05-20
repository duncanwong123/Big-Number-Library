/*
 * SHALib.c
 *
 * 9/8/00 Duncan S Wong - Initial Porting
 */

// *****
// * PROJECT:		MySharedLib (MSL)
// * FILENAME: 		MySharedLib.c
// * AUTHOR:		Jeff Ishaq 05/21/99
// * 
// * DESCRIPTION:	Shared library functionality implementation.  This code is influenced
// *				by the design outlined in Palm's "Shared Libraries and Other Advanced 
// *				Project Types" white paper, article #1143.  This can be found on Palm's
// *				knowledge base.
// *
// * COPYRIGHT:		As long as this 'copyright' is intact, this code is freely modifiable
// *				and distributable.
// *****

// Because we play with #defines that you're not normally expected to play with,
// we tend to run into cryptic link errors by including precompiled headers:
#ifndef PILOT_PRECOMPILED_HEADERS_OFF
	#define	PILOT_PRECOMPILED_HEADERS_OFF
#endif

#include <Pilot.h>						// Standard Palm stuff
#include "BNLib.h"						// Our interface definition
#include "MySharedLibPrv.h"		// Private routiens (globals stuff)



#pragma mark - 
// Utility functions for globals structure access

// *****
// * FUNCTION: 		MSLAllocGlobals
// * 
// * DESCRIPTION:	Allocate AND LOCK library globals ptr for a given RefNum
// *
// * PARAMETERS:	uRefNum		-		Lib refnum whose globals we'll create
// *			
// * RETURNED:		Ptr to new globals		-	success
// *				NULL					-	failure, MemHandleNew failed (!)
// *
// * POSTCONDITION:	Since this routine locks the handle it returns if successful, the
// *				caller is responsible for calling MSLUnlockGlobals() when s/he is done.
// *				Remember, the sooner you do this, the more you prevent heap fragmentation.
// *
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/18/99	Initial Implementation
// *****
MSLGlobalsTypePtr MSLAllocGlobals( UInt uRefNum )
{

	MSLGlobalsTypePtr		gP = NULL;
	SysLibTblEntryPtr		sysLibEntryP;
	VoidHand				gH = NULL;
	
	ErrFatalDisplayIf(sysInvalidRefNum == uRefNum, "Invalid refnum.");

	// Fetch a ptr to our lib's table entry in the OS's array (refnum is index)
	sysLibEntryP = SysLibTblEntry( uRefNum );
	ErrFatalDisplayIf( NULL == sysLibEntryP, "Invalid refnum.");
	ErrFatalDisplayIf( sysLibEntryP->globalsP, "Lib globals ptr already exists.");
	
	gH = MemHandleNew( sizeof(MSLGlobalsType) );				// Alloc mem for globals here
	if ( !gH )
		return ( NULL );
		
	sysLibEntryP->globalsP = (void*)gH;							// Store handle in lib entry
	
	gP = (MSLGlobalsTypePtr) MSLLockGlobals( uRefNum );			
	ErrFatalDisplayIf( !gP, "Unable to lock lib globals ptr.");

	// We need to set the owner of this chunk to 'system'.  If we don't do this, then
	// the memory manager will automatically free this when the first application to
	// call MSLOpen() exits.  Since we intend this library (and its globals) to hang around
	// regardless of which app begins and exits, we need to do this:
	MemPtrSetOwner( gP, 0 );									// 0 == OS
	
	MemSet( gP, sizeof(MSLGlobalsType), 0 );					// Clean it out

	// Globals should be initialized in your lib's Open entry point... see MSLOpen()
	return ( gP );
}


// *****
// * FUNCTION: 		MSLFreeGlobals
// * 
// * DESCRIPTION:	Deallocate a lib's globals ptr, given its RefNum.
// *
// * PARAMETERS:	uRefNum		-		Lib refnum whose globals we'll deallocate
// *			
// * RETURNED:		ErrNone					-	MemHandleNew success
// *				!0						-	failure, MemHandleNew failed 
// *
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/18/99	Initial Implementation
// *****
Err MSLFreeGlobals( UInt uRefNum )
{
	SysLibTblEntryPtr		sysLibEntryP;
	VoidHand				gH = NULL;
	
	ErrFatalDisplayIf( sysInvalidRefNum == uRefNum, "Invalid refnum.");
	
	sysLibEntryP = SysLibTblEntry( uRefNum );
	ErrFatalDisplayIf( NULL == sysLibEntryP, "Invalid refnum.");
	
	gH = (VoidHand) (sysLibEntryP->globalsP);					// Get our globals handle
	ErrFatalDisplayIf(!gH, "Lib globals ptr does not exist.");

	sysLibEntryP->globalsP = NULL;
	return( MemHandleFree(gH) );
}

// *****
// * FUNCTION: 		MSLLockGlobals
// * 
// * DESCRIPTION:	Return a ptr to a particular lib's MSLGlobalsType structure
// *
// * PARAMETERS:	uRefNum		-		Lib refnum whose globals we'll lock
// *			
// * RETURNED:		0			-		Caller needs to allocate them first with MSLAllocGlobals()!
// *				Valid ptr	-		success
// *
// * POSTCONDITION:	If I return 0, the caller needs to MSLAllocGlobals().
// *
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/18/99	Initial Implementation
// *****
MSLGlobalsTypePtr MSLLockGlobals( UInt uRefNum )
{
	MSLGlobalsTypePtr		gP 				= NULL;						// Necessary!
	SysLibTblEntryPtr		sysLibEntryP 	= NULL;
	VoidHand				gH				= NULL;
	
	ErrFatalDisplayIf( sysInvalidRefNum == uRefNum, "Invalid refnum.");
	
	sysLibEntryP = SysLibTblEntry( uRefNum );
	ErrFatalDisplayIf( NULL == sysLibEntryP, "Invalid refnum.");
	
	gH = (VoidHand) (sysLibEntryP->globalsP);

	// We don't ErrFatalDisplay here if !gH.  This is so the caller can check the return
	// value and if it's null, the caller knows s/he needs to MSLAllocGlobals(), similar
	// to the behavior of SysLibFind() and SysLibLoad()ing something.
	if (gH)
	{
		gP = (MSLGlobalsTypePtr)MemHandleLock( gH );
	}
	
	// Notice we want to return NULL if this handle hasn't yet been allocated!
	
	return gP;
}

// *****
// * FUNCTION: 		MSLUnlockGlobals
// * 
// * DESCRIPTION:	Unlock a ptr to a MSLGlobalsType structure
// *
// * PRECONDITION:	gP has been locked down by a call to MSLLoclGlobals.
// *
// * PARAMETERS:	gP			-		Locked ptr to structure
// *			
// * RETURNED:		!0			-		MemPtrUnlock failure (!)
// *				ErrNone		-		MemPtrUnlock success
// *
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/18/99	Initial Implementation
// *****
Err MSLUnlockGlobals( MSLGlobalsTypePtr gP )
{
	return( MemPtrUnlock(gP) );											// No magic here..
}



#pragma mark -
// 9/8/00 Duncan S Wong - Initial Modification
// OS-Required entry point implementations:

// *****
// * FUNCTION: 		MSLOpen
// * 
// * DESCRIPTION:	Open MSL; alloc globals if necessary
// *
// * PRECONDITION:	Caller has already done a SysLibFind and SysLibLoad to get refnum
// *
// * PARAMETERS:	uRefNum		-		Lib refnum 
// *			
// * RETURNED:		BNErrNone			-	success
// *				BNErrNoGlobals		-   unable to allocate globals
// *
// * POSTCONDITION:	Caller should MSLClose() this lib as soon as s/he is done using it.
// *				Multiple MSLOpens() are ok, but each one should always have a 
// *				correspoding MSLClose() to balance it out.
// *
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/19/99	Initial Implementation
// *****
BNErr BNLibOpen( UInt uRefNum )
{
	Err						err;
	MSLGlobalsTypePtr		gP = NULL;
	
	// Allocate globals
	ErrFatalDisplayIf( sysInvalidRefNum == uRefNum, "Invalid refnum.");
	
	gP = MSLLockGlobals( uRefNum );
	
	// If this returns NULL, that means we need to allocate the globals.  This also
	// implies that this is the first time we've opened this shared library.  
	if ( !gP )
	{
		gP = MSLAllocGlobals( uRefNum );
		if ( !gP )
			return BNErrNoGlobals;
			
		// Initialize globals here:
		gP->iOpenCount = 1;

		// Dump diagnostic info, i.e. "MSL ref# %d initially opened; globals initialized.\n", uRefNum
	}
	else
		gP->iOpenCount++;
		
	err = MSLUnlockGlobals( gP );
	ErrFatalDisplayIf( err, "Unable to unlock lib globals.");			
	
	return BNErrNone;
}


// 9/8/00 Duncan S Wong - Initial Modification
// *****
// * FUNCTION: 		MSLClose
// * 
// * DESCRIPTION:	Close MSL; free globals if necessary
// *
// * PARAMETERS:	uRefNum			-				Lib refnum 
// *				dwRefCountP		- (Modified)	DWord into which we put the open count
// *			
// * RETURNED:		BNErrNone		-	success
// *				BNErrNoGlobals	-	Unable to lock down the globals, this is bad
// *
// * POSTCONDITION:	Caller should ALWAYS check dwRefCount upon successful return.  If it's
// *				zero, caller should SysLibRemove() this library as it's no longer in use.
// *
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/19/99	Initial Implementation
// *****
BNErr BNLibClose( UInt uRefNum, DWordPtr dwRefCountP )
{
	Err						err;
	MSLGlobalsTypePtr		gP = NULL;
	
	ErrFatalDisplayIf( sysInvalidRefNum == uRefNum, "Invalid refnum.");
	
	if ( !dwRefCountP )														// Validate param
		return BNErrParam;
		
	gP = MSLLockGlobals ( uRefNum );
	if ( !gP )
		return BNErrNoGlobals;
	
	gP->iOpenCount--;
	ErrNonFatalDisplayIf( gP->iOpenCount < 0, "Library globals underlock." );

	*dwRefCountP = gP->iOpenCount;
		
	MSLUnlockGlobals( gP );

	if ( *dwRefCountP <= 0 )		// Use this instead of gP->iOpenCount, since we just
	{								// unlocked gp!
		// Dump diagnostic info i.e.  "MSL ref# %d closed; globals freed.", uRefNum		
		err = MSLFreeGlobals( uRefNum );								
		ErrFatalDisplayIf( err, "Unable to free lib globals.");
	}
	
	return BNErrNone;
}


// 9/8/00 Duncan S Wong - Initial Modification
// *****
// * FUNCTION: 		MSLSleep
// * 
// * DESCRIPTION:	Called when device goes to sleep.  Since this routine can sometimes be
// *				called from an interrupt handler, you can never spend a lot of time in 
// *				this routine or else you'll make the system unstable and probably cause
// *				mysterious crashes.  In addition, this routine is called as a result of
// *				a battery pull situation; in that case, the Palm is running off of its
// *				super cap, which means there's about 1 ms of processor time remaining
// *				before there is no power.  To avoid catastrophic failure, you and any
// *				other installed shared libraries had better not take up too many cycles
// *				in their respective Sleep function!!
// *
// * PARAMETERS:	uRefNum		-		Lib refnum
// *			
// * RETURNS:		Always 0.  I don't know who uses this return value, or if it's needed.
// *
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/19/99	Initial Implementation
// *****
Err BNLibSleep( UInt uRefNum )
{
	// If you were implementing custom hardware, you'd do something like
	// this to put it to sleep to conserve power, and to prevent it from
	// sapping the super cap in the event of a battery pull:

	#ifdef MY_HARDWARE_INSTALLED	// ... fictitious example #define ...
		// Tell MyHardware to power down, and then return ASAP!
		MyHardwareBaseAddr->pwrCtlReg |= SLEEP_MODE;
	#endif
	
	return 0;
}


// 9/8/00 Duncan S Wong - Initial Modification
// *****
// * FUNCTION: 		MSLWake
// * 
// * DESCRIPTION:	Called when device wakes up from sleep.  Since this routine is sometimes
// *				called from an interrupt handler, you can never spend a lot of time in 
// *				this routine or else you'll make the system unstable and probably cause
// *				mysterious crashes.  If you have a time-consuming chore to do, consider
// *				using an interrupt-safe routine like EvtEnqueueKey() to set a flag.  In
// *				an EvtGetEvent hook, you can see this flag come through.  Since you're no
// *				longer in an intrreupt handler, you can do your time-consuming chore at 
// *				that time.
// *
// * PARAMETERS:	uRefNum		-		Lib refnum
// *
// * RETURNS:		Always 0.  I don't know why uses this return value, or if it's needed.
// *			
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/19/99	Initial Implementation
// *****
Err BNLibWake( UInt uRefNum )
{
	// If you were implementing custom hardware, you'd do something like
	// this to wake your hardware back up:

	#ifdef MY_HARDWARE_INSTALLED	// ... fictitious example #define ...
		// Tell MyHardware to wake up from sleep mode
		MyHardwareBaseAddr->pwrCtlReg &= ~SLEEP_MODE;
	#endif
	
	return 0;
}


// Here are those 46 functions!
BN_CTX *BNLibBN_CTX_new(UInt uRefNum)
{
	return BN_CTX_new();
}

void	BNLibBN_CTX_free(UInt uRefNum, BN_CTX *c)
{
	BN_CTX_free(c);
}

int  BNLibBN_rand(UInt uRefNum, BIGNUM *rnd, int bits, int top,int bottom)
{
	return BN_rand(rnd, bits, top, bottom);
}

int	BNLibBN_num_bits(UInt uRefNum, BIGNUM *a)
{
	return BN_num_bits(a);
}

BIGNUM *BNLibBN_new(UInt uRefNum)
{
	return BN_new();
}

void	BNLibBN_clear_free(UInt uRefNum, BIGNUM *a)
{
	BN_clear_free(a);
}

BIGNUM *BNLibBN_copy(UInt uRefNum, BIGNUM *a, BIGNUM *b)
{
	return BN_copy(a, b);
}

BIGNUM *BNLibBN_bin2bn(UInt uRefNum, unsigned char *s,int len,BIGNUM *ret)
{
	return BN_bin2bn(s, len, ret);
}

int	BNLibBN_bn2bin(UInt uRefNum, BIGNUM *a, unsigned char *to)
{
	return BN_bn2bin(a, to);
}

int	BNLibBN_sub(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)
{
	return BN_sub(r, a, b);
}

void	BNLibBN_qsub(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)
{
	bn_qsub(r, a, b);
}

void	BNLibBN_qadd(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)
{
	bn_qadd(r, a, b);
}

int	BNLibBN_add(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)
{
	return BN_add(r, a, b);
}

int	BNLibBN_mod(UInt uRefNum, BIGNUM *rem, BIGNUM *m, BIGNUM *d, BN_CTX *ctx)
{
	return BN_mod(rem, m, d, ctx);
}

int	BNLibBN_div(UInt uRefNum, BIGNUM *dv, BIGNUM *rem, BIGNUM *m, BIGNUM *d, BN_CTX *ctx)
{
	return BN_div(dv, rem, m, d, ctx);
}

int	BNLibBN_mul(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *b)
{
	return BN_mul(r, a, b);
}

int	BNLibBN_sqr(UInt uRefNum, BIGNUM *r, BIGNUM *a,BN_CTX *ctx)
{
	return BN_sqr(r, a, ctx);
}

BN_ULONG BNLibBN_mod_word(UInt uRefNum, BIGNUM *a, unsigned long w)
{
	return BN_mod_word(a, w);
}

BN_ULONG BNLibBN_div_word(UInt uRefNum, BIGNUM *a, unsigned long w)
{
	return BN_div_word(a, w);
}

int	BNLibBN_add_word(UInt uRefNum, BIGNUM *a, unsigned long w)
{
	return BN_add_word(a, w);
}

int	BNLibBN_set_word(UInt uRefNum, BIGNUM *a, unsigned long w)
{
	return BN_set_word(a, w);
}

unsigned long BNLibBN_get_word(UInt uRefNum, BIGNUM *a)
{
	return BN_get_word(a);
}

int	BNLibBN_cmp(UInt uRefNum, BIGNUM *a, BIGNUM *b)
{
	return BN_cmp(a, b);
}

void BNLibBN_free(UInt uRefNum, BIGNUM *a)
{
	BN_free(a);
}

int	BNLibBN_is_bit_set(UInt uRefNum, BIGNUM *a, int n)
{
	return BN_is_bit_set(a, n);
}

int	BNLibBN_lshift(UInt uRefNum, BIGNUM *r, BIGNUM *a, int n)
{
	return BN_lshift(r, a, n);
}

int	BNLibBN_lshift1(UInt uRefNum, BIGNUM *r, BIGNUM *a)
{
	return BN_lshift1(r, a);
}

int	BNLibBN_mod_exp(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx)
{
	return BN_mod_exp(r, a, p, m, ctx);
}

int	BNLibBN_mod_exp_recp(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx)
{
	return BN_mod_exp_recp(r, a, p, m, ctx);
}

int	BNLibBN_mod_exp_simple(UInt uRefNum, BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx)
{
	return BN_mod_exp_simple(r, a, p, m, ctx);
}

int	BNLibBN_mask_bits(UInt uRefNum, BIGNUM *a,int n)
{
	return BN_mask_bits(a, n);
}

int	BNLibBN_mod_mul_reciprocal(UInt uRefNum, BIGNUM *r, BIGNUM *x, BIGNUM *y, BIGNUM *m, BIGNUM *i, int nb, BN_CTX *ctx)
{
	return BN_mod_mul_reciprocal(r, x, y, m, i, nb, ctx);
}

int	BNLibBN_mod_mul(UInt uRefNum, BIGNUM *ret, BIGNUM *a, BIGNUM *b, BIGNUM *m, BN_CTX *ctx)
{
	return BN_mod_mul(ret, a, b, m, ctx);
}

int	BNLibBN_reciprocal(UInt uRefNum, BIGNUM *r, BIGNUM *m, BN_CTX *ctx)
{
	return BN_reciprocal(r, m, ctx);
}

int	BNLibBN_rshift(UInt uRefNum, BIGNUM *r, BIGNUM *a, int n)
{
	return BN_rshift(r, a, n);
}

int	BNLibBN_rshift1(UInt uRefNum, BIGNUM *r, BIGNUM *a)
{
	return BN_rshift1(r, a);
}

void	BNLibBN_clear(UInt uRefNum, BIGNUM *a)
{
	BN_clear(a);
}

BIGNUM *BNLibBN_dup(UInt uRefNum, BIGNUM *a)
{
	return BN_dup(a);
}

int	BNLibBN_ucmp(UInt uRefNum, BIGNUM *a, BIGNUM *b)
{
	return BN_ucmp(a, b);
}

int	BNLibBN_set_bit(UInt uRefNum, BIGNUM *a, int n)
{
	return BN_set_bit(a, n);
}

int	BNLibBN_clear_bit(UInt uRefNum, BIGNUM *a, int n)
{
	return BN_clear_bit(a, n);
}

char *BNLibBN_bn2ascii(UInt uRefNum, BIGNUM *a)
{
	return BN_bn2ascii(a);
}

int 	BNLibBN_ascii2bn(UInt uRefNum, BIGNUM **a,char *str)
{
	return BN_ascii2bn(a, str);
}

int	BNLibBN_gcd(UInt uRefNum, BIGNUM *r,BIGNUM *in_a,BIGNUM *in_b,BN_CTX *ctx)
{
	return BN_gcd(r, in_a, in_b, ctx);
}

BIGNUM *BNLibBN_mod_inverse(UInt uRefNum, BIGNUM *a, BIGNUM *n,BN_CTX *ctx)
{
	return BN_mod_inverse(a, n, ctx);
}