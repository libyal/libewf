/*
 * MD5 support for ewftools
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

#if !defined( _EWFMD5_H )
#define _EWFMD5_H

#include <common.h>

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H )
#include <openssl/md5.h>
#else
#include "ewfdigest_context.h"
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H )

#define EWFMD5_CONTEXT	MD5_CTX

#define ewfmd5_initialize( context ) \
	MD5_Init( context )

#define ewfmd5_update( context, buffer, size ) \
	MD5_Update( context, buffer, size )

#define ewfmd5_finalize( context, hash, size ) \
	MD5_Final( hash, context )

#else

#define EWFMD5_CONTEXT	EWFDIGEST_CONTEXT

#define ewfmd5_initialize( context ) \
	ewfdigest_context_initialize( context, EWFDIGEST_CONTEXT_TYPE_MD5 )

#define ewfmd5_update( context, buffer, size ) \
	ewfdigest_context_update( context, buffer, size )

#define ewfmd5_finalize( context, hash, size ) \
	ewfdigest_context_finalize( context, hash, size )

#endif

#if defined( __cplusplus )
}
#endif

#endif

