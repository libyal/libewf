/*
 * ewfsha1
 * SHA1 support for the ewftools
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _EWFSHA1_H
#define _EWFSHA1_H

#include "../libewf/libewf_includes.h"

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>
#else
#include "../libewf/libewf_digest_context.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )

#define EWFSHA1_CONTEXT	SHA_CTX

#define ewfsha1_initialize( context ) \
	SHA1_Init( context )

#define ewfsha1_update( context, buffer, size ) \
	SHA1_Update( context, buffer, size )

#define ewfsha1_finalize( context, hash, size ) \
	SHA1_Final( hash, context )

#else

#define EWFSHA1_CONTEXT	LIBEWF_DIGEST_CONTEXT

#define ewfsha1_initialize( context ) \
	libewf_digest_context_initialize( context, LIBEWF_DIGEST_CONTEXT_TYPE_SHA1 )

#define ewfsha1_update( context, buffer, size ) \
	libewf_digest_context_update( context, buffer, size )

#define ewfsha1_finalize( context, hash, size ) \
	libewf_digest_context_finalize( context, hash, size )

#endif

#ifdef __cplusplus
}
#endif

#endif

