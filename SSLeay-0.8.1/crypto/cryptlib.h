/* crypto/cryptlib.h */
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

#ifndef HEADER_CRYPTLIB_H
#define HEADER_CRYPTLIB_H

//#include <stdlib.h>
//#include <string.h>

#ifndef __palmos__
#define __palmos__
#endif

//#include "../e_os.h"
#include "crypto.h"
//#include "buffer/buffer.h"
//#include "bio/bio.h"
//#include "err/err.h"

/* We don't use the supplied error lib, as that would require lhash, etc.,
   so we just do something simple */
#define ERR_R_FATAL            32      
#define ERR_R_MALLOC_FAILURE                    (1|ERR_R_FATAL)
#define ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED       (2|ERR_R_FATAL)
#define ERR_R_PASSED_NULL_PARAMETER             (3|ERR_R_FATAL)
#define ERR_LIB_BN             3
#define ERR_R_BN_LIB   ERR_LIB_BN
#define BNerr(f,r) ErrFatalDisplayIf(1, \
    r==ERR_R_MALLOC_FAILURE ? "malloc failure" : \
    r==BN_R_BAD_RECIPROCAL ? "bad reciprocal" : \
    r==BN_R_CALLED_WITH_EVEN_MODULUS ? "called with even modulus" : \
    r==BN_R_DIV_BY_ZERO ? "div by zero" : \
    r==BN_R_NO_INVERSE ? "no inverse" : \
    "unknown error");
#define RSAerr(f,r) ErrFatalDisplayIf(1, \
    r==ERR_R_MALLOC_FAILURE ? "malloc failure" : \
    r==ERR_LIB_BN ? "bn lib error" : \
    r==RSA_R_ALGORITHM_MISMATCH ? "algorithm mismatch" : \
    r==RSA_R_BAD_E_VALUE ? "bad e value" : \
    r==RSA_R_BAD_FIXED_HEADER_DECRYPT ? "bad fixed header decrypt" : \
    r==RSA_R_BAD_PAD_BYTE_COUNT ? "bad pad byte count" : \
    r==RSA_R_BAD_SIGNATURE ? "bad signature" : \
    r==RSA_R_BLOCK_TYPE_IS_NOT_01 ? "block type is not 01" : \
    r==RSA_R_BLOCK_TYPE_IS_NOT_02 ? "block type is not 02" : \
    r==RSA_R_DATA_GREATER_THAN_MOD_LEN ? "data greater than mod len" : \
    r==RSA_R_DATA_TOO_LARGE_FOR_KEY_SIZE ? "data too large for key size" : \
    r==RSA_R_DIGEST_TOO_BIG_FOR_RSA_KEY ? "digest too big for rsa key" : \
    r==RSA_R_NULL_BEFORE_BLOCK_MISSING ? "null before block missing" : \
    r==RSA_R_SSLV3_ROLLBACK_ATTACK ? "sslv3 rollback attack" : \
    r==RSA_R_THE_ASN1_OBJECT_IDENTIFIER_IS_NOT_KNOWN_FOR_THIS_MD ? "the asn1 object identifier is not known for this md" : \
    r==RSA_R_UNKNOWN_ALGORITHM_TYPE ? "unknown algorithm type" : \
    r==RSA_R_UNKNOWN_PADDING_TYPE ? "unknown padding type" : \
    r==RSA_R_WRONG_SIGNATURE_LENGTH ? "wrong signature length" : \
    "unknown error");
#define DHerr(f,r) ErrFatalDisplayIf(1, \
    r==ERR_R_MALLOC_FAILURE ? "malloc failure" : \
    r==ERR_LIB_BN ? "bn lib error" : \
    r==DH_R_NO_PRIVATE_VALUE ? "no private value" : \
    "unknown error");

#endif
