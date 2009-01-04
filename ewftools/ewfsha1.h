/*
 * SHA1 support for the ewftools
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

#if !defined( _EWFSHA1_H )
#define _EWFSHA1_H

#include <common.h>

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>
#else
#include "ewfdigest_context.h"
#endif

#if defined( __cplusplus )
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

#define EWFSHA1_CONTEXT	EWFDIGEST_CONTEXT

#define ewfsha1_initialize( context ) \
	ewfdigest_context_initialize( context, EWFDIGEST_CONTEXT_TYPE_SHA1 )

#define ewfsha1_update( context, buffer, size ) \
	ewfdigest_context_update( context, buffer, size )

#define ewfsha1_finalize( context, hash, size ) \
	ewfdigest_context_finalize( context, hash, size )

#endif

#if defined( __cplusplus )
}
#endif

#endif

