/*
 * MD5 support
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

#if !defined( _MD5_H )
#define _MD5_H

#include <common.h>

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H )
#include <openssl/md5.h>
#else
#include "digest_context.h"
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H )

typedef MD5_CTX md5_context_t;

#define md5_initialize( context, error ) \
	MD5_Init( context )

#define md5_update( context, buffer, size, error ) \
	MD5_Update( context, buffer, size )

#define md5_finalize( context, hash, size, error ) \
	MD5_Final( hash, context )

#else

typedef digest_context_t md5_context_t;

#define md5_initialize( context, error ) \
	digest_context_initialize( context, DIGEST_CONTEXT_TYPE_MD5, error )

#define md5_update( context, buffer, size, error ) \
	digest_context_update( context, buffer, size, error )

#define md5_finalize( context, hash, size, error ) \
	digest_context_finalize( context, hash, size, error )

#endif

#if defined( __cplusplus )
}
#endif

#endif

