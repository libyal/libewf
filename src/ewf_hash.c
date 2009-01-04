/*
 * EWF hash section
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
#include "libewf_endian.h"
#include "libewf_notify.h"

#include "ewf_crc.h"
#include "ewf_file_header.h"
#include "ewf_hash.h"

/* Allocates memory for a new ewf hash struct
 * Return a pointer to the new instance, NULL on error
 */
EWF_HASH *ewf_hash_alloc( void )
{
	EWF_HASH *hash     = NULL;
	EWF_HASH *data_set = NULL;

	hash = (EWF_HASH *) libewf_alloc_cleared( EWF_HASH_SIZE, 0 );

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_alloc: unable to allocate ewf_hash.\n" );

		return( NULL );
	}
	data_set = (void *) libewf_memcpy( (uint8_t *) hash->signature, (uint8_t *) evf_file_signature, 8 );

	if( data_set == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_alloc: unable to set data.\n" );

		libewf_free( hash );

		return( NULL );
	}
	return( hash );
}

/* Frees memory of a hash
 */
void ewf_hash_free( EWF_HASH *hash )
{
	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_free: invalid hash.\n" );

		return;
	}
	libewf_free( hash );
}

/* Reads the hash from a file descriptor
 * Return a pointer to the new instance, NULL on error
 */
EWF_HASH *ewf_hash_read( int file_descriptor )
{
	EWF_HASH *hash = NULL;
	uint32_t size  = EWF_HASH_SIZE;
	int32_t count  = 0;

	hash = ewf_hash_alloc();

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_read: unable to create hash.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, hash, size );

	if( count < size )
	{
		ewf_hash_free( hash );

		LIBEWF_WARNING_PRINT( "ewf_hash_read: unable to read hash.\n" );

		return( NULL );
	}
	return( hash );
}

/* Writes the hash to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_hash_write( EWF_HASH *hash, int file_descriptor )
{
	EWF_CRC *crc  = NULL;
	uint32_t size = EWF_HASH_SIZE;
	int32_t count = 0;

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_write: invalid hash.\n" );

		return( -1 );
	}
	crc = ewf_crc_calculate( (void *) hash, ( size - EWF_CRC_SIZE ), 1 );

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	revert_32bit( *crc, hash->crc );

	ewf_crc_free( crc );

	count = libewf_write( file_descriptor, hash, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_hash_write: unable to write hash.\n" );

		return( -1 );
	}
	return( count );
}

