/*
 * libewf MD5 support
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

#ifndef _LIBEWF_MD5_H
#define _LIBEWF_MD5_H

#include "libewf_includes.h"

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H )
#include <openssl/md5.h>
#else
#include "libewf_digest_context.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H )

#define LIBEWF_MD5_CONTEXT	MD5_CTX

#define libewf_md5_initialize( context ) \
	MD5_Init( context )

#define libewf_md5_update( context, buffer, size ) \
	MD5_Update( context, buffer, size )

#define libewf_md5_finalize( context, hash, size ) \
	MD5_Final( hash, context )

#else

#define LIBEWF_MD5_CONTEXT	LIBEWF_DIGEST_CONTEXT

#define libewf_md5_initialize( context ) \
	libewf_digest_context_initialize( context, LIBEWF_DIGEST_CONTEXT_TYPE_MD5 )

#define libewf_md5_update( context, buffer, size ) \
	libewf_digest_context_update( context, buffer, size )

#define libewf_md5_finalize( context, hash, size ) \
	libewf_digest_context_finalize( context, hash, size )

#endif

#ifdef __cplusplus
}
#endif

#endif

