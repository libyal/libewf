/*
 * SHA1 support
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _SHA1_H )
#define _SHA1_H

#include <common.h>

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>
#else
#include "digest_context.h"
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )

typedef SHA_CTX sha1_context_t;

#define sha1_initialize( context, error ) \
	SHA1_Init( context )

#define sha1_update( context, buffer, size, error ) \
	SHA1_Update( context, buffer, size )

#define sha1_finalize( context, hash, size, error ) \
	SHA1_Final( hash, context )

#else

typedef digest_context_t sha1_context_t;

#define sha1_initialize( context, error ) \
	digest_context_initialize( context, DIGEST_CONTEXT_TYPE_SHA1, error )

#define sha1_update( context, buffer, size, error ) \
	digest_context_update( context, buffer, size, error )

#define sha1_finalize( context, hash, size, error ) \
	digest_context_finalize( context, hash, size, error )

#endif

#if defined( __cplusplus )
}
#endif

#endif

