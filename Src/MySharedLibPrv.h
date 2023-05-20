/*
 * MySharedLibPrv.h
 *
 * Note: - Prv stands for PRIVATE
 *
 * 9/8/00 Duncan S Wong - Initial Modification
 *
 */

// *****
// * PROJECT:		MySharedLib (MSL)
// * FILENAME: 		MySharedLibPrv.h
// * AUTHOR:		Jeff Ishaq 05/21/99
// * 
// * DESCRIPTION:	Shared library functionality interface definition for PRIVATE
// *				functions.  These should be used to build a library, but should
// *				not be distributed with that library; instead, just distribute
// *				MySharedLib.h
// *
// * COPYRIGHT:		As long as this 'copyright' is intact, this code is freely modifiable
// *				and distributable.
// *****

#pragma once

// These are used by MSLGetVersion to determine the current version of this lib:
#define BUG_FIX				0
#define	BUILD_NUM			1
#define	DEV_STAGE			0	// Dev = 0, Alpha = 1, Beta = 2, Release = 3
#define	MAJOR_VERSION		1
#define	MINOR_VERSION		0
#define GENERATE_LIB_VER	sysMakeROMVersion(MAJOR_VERSION, MINOR_VERSION, BUG_FIX, DEV_STAGE, BUILD_NUM)


// This is the Globals struct that we use throughout our library.
typedef struct tagMSLGlobalsType
{
	Int		iOpenCount;				// Our internal open-count of the lib
	
	/////
	// Your globals go here...
	/////

} MSLGlobalsType;
typedef MSLGlobalsType*	MSLGlobalsTypePtr;

// These are some utility functions.  We don't actually use these in our dispatch
// table, so we don't need to define traps for them nor extern them.
MSLGlobalsTypePtr	MSLAllocGlobals	( UInt uRefNum );
MSLGlobalsTypePtr	MSLLockGlobals	( UInt uRefNum );
Err 				MSLFreeGlobals	( UInt uRefNum );
Err					MSLUnlockGlobals( MSLGlobalsTypePtr gP );
