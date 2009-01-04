/*
 * EWF MD5 hash
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <stdlib.h>

#include "libewf_common.h"
#include "libewf_notify.h"

#include "ewf_md5hash.h"

/* Allocates memory for a new ewf md5 hash
 * Return a pointer to the new instance, NULL on error
 */
EWF_MD5HASH *ewf_md5hash_alloc( void )
{
	EWF_MD5HASH *md5hash = NULL;

	md5hash = (EWF_MD5HASH *) libewf_alloc_cleared( EWF_MD5HASH_SIZE, 0 );

	if( md5hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_md5hash_alloc: unable to allocate ewf_md5hash.\n" );

		return( NULL );
	}
	return( md5hash );
}

/* Frees memory of a md5 hash
 */
void ewf_md5hash_free( EWF_MD5HASH *md5hash )
{
	if( md5hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_md5hash_free: invalid md5hash.\n" );

		return;
	}
	libewf_free( md5hash );
}

/* Writes the MD5 hash to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_md5hash_write( EWF_MD5HASH *md5hash, int file_descriptor )
{
	uint32_t size = EWF_MD5HASH_SIZE;
	int32_t count = 0;

	if( md5hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_md5hash_write: invalid md5hash.\n" );

		return( -1 );
	}
	count = libewf_write( file_descriptor, md5hash, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_md5hash_write: unable to write md5hash.\n" );

		return( -1 );
	}
	return( count );
}

/* Converts the md5 hash to a printable string
 * Return a pointer to the new instance, NULL on error
 */
char *ewf_md5hash_to_string( EWF_MD5HASH *md5hash )
{
	/* An md5 hash consists of 32 characters + 1 end of string
	 */
	char *string          = NULL;
	unsigned char md5char = 0;
	uint64_t iterator     = 0;

	string = (char *) libewf_alloc( 33 * sizeof( char ) );

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_md5hash_to_string: unable to allocate string.\n" );

		return( NULL );
	}
	for( iterator = 0; iterator < 16; iterator++ )
	{
		md5char = *( md5hash + iterator );

		if( snprintf( &string[ iterator * 2 ], 3, "%02x", md5char ) <= -1 )
		{
			LIBEWF_WARNING_PRINT( "ewf_md5hash_to_string: unable to fill string.\n" );

			libewf_free( string );

			return( NULL );
		}
	}
	return( string );
}

