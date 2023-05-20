/*
 * BNLibDispatch.c
 *
 * 9/8/00 Duncan S Wong - Initial Implementation
 */

// *****
// * PROJECT:		MySharedLib (MSL)
// * FILENAME: 		MySharedLibDispatch.c
// * AUTHOR:		Jeff Ishaq 05/21/99
// * 
// * DESCRIPTION:	Installation routine for shared library's dispatch table.
// *
// * COPYRIGHT:		As long as this 'copyright' is intact, this code is freely modifiable
// *				and distributable.
// *****

// Because we play with #defines that you're not normally expected to play with,
// we tend to run into cryptic link errors by including precompiled headers:
#ifndef PILOT_PRECOMPILED_HEADERS_OFF
	#define	PILOT_PRECOMPILED_HEADERS_OFF
#endif

#define EMULATION_LEVEL		EMULATION_NONE		// Force this to no emulation:

#undef 		__PALMOS_TRAPS__					// To prevent a redeclaration error
#define 	__PALMOS_TRAPS__ 	0				// Now, define this ourselves
#define		USE_TRAPS 			0				// To _make_ traps, we need to turn this off

#include <Pilot.h>
#include "BNLib.h"


Err __Startup__( UInt uRefNum, SysLibTblEntryPtr entryP );
static Ptr	asm MSL_DispatchTable(void);

// *****
// * FUNCTION: 		__Startup__
// * 
// * DESCRIPTION:	Called to install the library by SysLibLoad().  You mustn't change
// *				the function's signature.
// *
// * PARAMETERS:	uRefNum		-		Lib refnum
// *				entryP		-		Ptr to our entry in the OS's lib table
// *
// * RETURNS:		Always 0.  I don't know why uses this return value, or if it's needed.
// *			
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/19/99	Initial Implementation
// *****
Err __Startup__( UInt uRefNum, SysLibTblEntryPtr entryP )
{
	// Stash our dispatch table's address into the OS's shared libaray
	// table ptr slot that corresponds to this uRefNum:
	entryP->dispatchTblP = (Ptr*) MSL_DispatchTable();
	
	// Zero the globals ptr so that our call to MSLAllocGlobals() does the
	// right thing:
	entryP->globalsP = 0;

	return 0;
}

#define prvJmpSize	4				// How many bytes a JMP instruction occupies
#define NUMBER_OF_FUNCTIONS	49		// Don't forget to update this if necessary!!

#define TABLE_OFFSET 			2 * (NUMBER_OF_FUNCTIONS + 1)

#define MSL_DISPATCH_SLOT(i)	(TABLE_OFFSET + ( (i) * prvJmpSize))

// *****
// * FUNCTION: 		MSL_DispatchTable
// * 
// * DESCRIPTION:	The actual dispatch table.  The linker will run through here and
// *				put the addresses of the JMP'd-to functions.
// *
// * RETURNS:		(Ptr)This
// *			
// * REVISION HISTORY:
// *	NAME	DATE		DESCRIPTION
// *    -------------------------------------------------------------------------------
// *	JeffI	05/19/99	Initial Implementation
// *****
static Ptr	asm	MSL_DispatchTable( void )
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

	LEA	@TableStart, A0
	RTS
	
@TableStart:
	DC.W		@LibName
	
	DC.W		MSL_DISPATCH_SLOT(0)			// BNLibOpen()
	DC.W		MSL_DISPATCH_SLOT(1)			// BNLibClose()
	DC.W		MSL_DISPATCH_SLOT(2)			// BNLibSleep()
	DC.W		MSL_DISPATCH_SLOT(3)			// BNLibWake()
	DC.W		MSL_DISPATCH_SLOT(4)
	DC.W		MSL_DISPATCH_SLOT(5)
	DC.W		MSL_DISPATCH_SLOT(6)
	DC.W		MSL_DISPATCH_SLOT(7)
	DC.W		MSL_DISPATCH_SLOT(8)
	DC.W		MSL_DISPATCH_SLOT(9)
	DC.W		MSL_DISPATCH_SLOT(10)
	DC.W		MSL_DISPATCH_SLOT(11)
	DC.W		MSL_DISPATCH_SLOT(12)
	DC.W		MSL_DISPATCH_SLOT(13)
	DC.W		MSL_DISPATCH_SLOT(14)
	DC.W		MSL_DISPATCH_SLOT(15)
	DC.W		MSL_DISPATCH_SLOT(16)
	DC.W		MSL_DISPATCH_SLOT(17)
	DC.W		MSL_DISPATCH_SLOT(18)
	DC.W		MSL_DISPATCH_SLOT(19)
	DC.W		MSL_DISPATCH_SLOT(20)
	DC.W		MSL_DISPATCH_SLOT(21)
	DC.W		MSL_DISPATCH_SLOT(22)
	DC.W		MSL_DISPATCH_SLOT(23)
	DC.W		MSL_DISPATCH_SLOT(24)
	DC.W		MSL_DISPATCH_SLOT(25)
	DC.W		MSL_DISPATCH_SLOT(26)
	DC.W		MSL_DISPATCH_SLOT(27)
	DC.W		MSL_DISPATCH_SLOT(28)
	DC.W		MSL_DISPATCH_SLOT(29)
	DC.W		MSL_DISPATCH_SLOT(30)
	DC.W		MSL_DISPATCH_SLOT(31)
	DC.W		MSL_DISPATCH_SLOT(32)
	DC.W		MSL_DISPATCH_SLOT(33)
	DC.W		MSL_DISPATCH_SLOT(34)
	DC.W		MSL_DISPATCH_SLOT(35)
	DC.W		MSL_DISPATCH_SLOT(36)
	DC.W		MSL_DISPATCH_SLOT(37)
	DC.W		MSL_DISPATCH_SLOT(38)
	DC.W		MSL_DISPATCH_SLOT(39)
	DC.W		MSL_DISPATCH_SLOT(40)
	DC.W		MSL_DISPATCH_SLOT(41)
	DC.W		MSL_DISPATCH_SLOT(42)
	DC.W		MSL_DISPATCH_SLOT(43)
	DC.W		MSL_DISPATCH_SLOT(44)
	DC.W		MSL_DISPATCH_SLOT(45)
	DC.W		MSL_DISPATCH_SLOT(46)
	DC.W		MSL_DISPATCH_SLOT(47)
	DC.W		MSL_DISPATCH_SLOT(48)

	JMP			BNLibOpen								// 0
	JMP			BNLibClose							// 1
	JMP 		BNLibSleep							// 2
	JMP			BNLibWake								// 3
	JMP			BNLibBN_CTX_new
	JMP			BNLibBN_CTX_free
	JMP			BNLibBN_rand
	JMP			BNLibBN_num_bits
	JMP			BNLibBN_new
	JMP			BNLibBN_clear_free
	JMP			BNLibBN_copy
	JMP			BNLibBN_bin2bn
	JMP			BNLibBN_bn2bin
	JMP			BNLibBN_sub
	JMP			BNLibBN_qsub
	JMP			BNLibBN_qadd
	JMP			BNLibBN_add
	JMP			BNLibBN_mod
	JMP			BNLibBN_div
	JMP			BNLibBN_mul
	JMP			BNLibBN_sqr
	JMP			BNLibBN_mod_word
	JMP			BNLibBN_div_word
	JMP			BNLibBN_add_word
	JMP			BNLibBN_set_word
	JMP			BNLibBN_get_word
	JMP			BNLibBN_cmp
	JMP			BNLibBN_free
	JMP			BNLibBN_is_bit_set
	JMP			BNLibBN_lshift
	JMP			BNLibBN_lshift1
	JMP			BNLibBN_mod_exp
	JMP			BNLibBN_mod_exp_recp
	JMP			BNLibBN_mod_exp_simple
	JMP			BNLibBN_mask_bits
	JMP			BNLibBN_mod_mul_reciprocal
	JMP			BNLibBN_mod_mul
	JMP			BNLibBN_reciprocal
	JMP			BNLibBN_rshift
	JMP			BNLibBN_rshift1
	JMP			BNLibBN_clear
	JMP			BNLibBN_dup
	JMP			BNLibBN_ucmp
	JMP			BNLibBN_set_bit
	JMP			BNLibBN_clear_bit
	JMP			BNLibBN_bn2ascii
	JMP			BNLibBN_ascii2bn
	JMP			BNLibBN_gcd
	JMP			BNLibBN_mod_inverse
	
@LibName:
	DC.B		BN_LIB_NAME							// SysLibFind()'s name key
}