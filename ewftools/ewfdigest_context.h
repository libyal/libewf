/*
 * Crypographic digest wrapper code for ewftools
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _EWFDIGEST_CONTEXT_H )
#define _EWFDIGEST_CONTEXT_H

#include <common.h>
#include <types.h>

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>
#elif defined( HAVE_WINCPRYPT_H )
#include <windows.h>
#include <wincrypt.h>
#endif

#include "ewfdigest_hash.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define EWFDIGEST_CONTEXT_TYPE_MD5		(uint8_t) 'm'
#define EWFDIGEST_CONTEXT_TYPE_SHA1		(uint8_t) 's'

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#define EWFDIGEST_CONTEXT EVP_MD_CTX

#else
#define EWFDIGEST_CONTEXT ewfdigest_context_t

#if defined( HAVE_WINCPRYPT_H )
typedef struct ewfdigest_context ewfdigest_context_t;

struct ewfdigest_context
{
	HCRYPTPROV crypt_provider;
	HCRYPTHASH hash;
};
#else
typedef int ewfdigest_context_t;
#endif
#endif

int ewfdigest_context_initialize(
     EWFDIGEST_CONTEXT *digest_context,
     uint8_t type );

int ewfdigest_context_update(
     EWFDIGEST_CONTEXT *digest_context,
     uint8_t *buffer,
     size_t size );

int ewfdigest_context_finalize(
     EWFDIGEST_CONTEXT *digest_context,
     ewfdigest_hash_t *digest_hash,
     size_t *size );

#if defined( __cplusplus )
}
#endif

#endif

