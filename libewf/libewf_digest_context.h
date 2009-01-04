/*
 * libewf crypographic digest wrapper code
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

#ifndef _LIBEWF_DIGEST_H
#define _LIBEWF_DIGEST_H

#include "libewf_includes.h"

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>
#elif defined( HAVE_WINCPRYPT_H )
#include <windows.h>
#include <wincrypt.h>
#else
#error Unsupported cryptographic library.
#endif

#include "ewf_digest_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIBEWF_DIGEST_CONTEXT_TYPE_MD5		(uint8_t) 'm'
#define LIBEWF_DIGEST_CONTEXT_TYPE_SHA1		(uint8_t) 's'

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )

#define LIBEWF_DIGEST_CONTEXT EVP_MD_CTX

#else

#define LIBEWF_DIGEST_CONTEXT libewf_digest_context_t

typedef struct libewf_digest_context libewf_digest_context_t;

struct libewf_digest_context
{
#if defined( HAVE_WINCPRYPT_H )
	HCRYPTPROV crypt_provider;
	HCRYPTHASH hash;
#else
#error Unsupported cryptographic library.
#endif
};

#endif

int8_t libewf_digest_context_initialize( LIBEWF_DIGEST_CONTEXT *digest_context, uint8_t type );
int8_t libewf_digest_context_update( LIBEWF_DIGEST_CONTEXT *digest_context, uint8_t *buffer, size_t size );
int8_t libewf_digest_context_finalize( LIBEWF_DIGEST_CONTEXT *digest_context, EWF_DIGEST_HASH *digest_hash, size_t *size );

#ifdef __cplusplus
}
#endif

#endif

