/*
 * EWF sectors section
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

#include "ewf_compress.h"
#include "ewf_sectors.h"

/* Allocates memory for a new ewf sectors_chunk 
 * Returns a pointer to the new instance, NULL on error
 */
EWF_SECTORS_CHUNK *ewf_sectors_chunk_alloc( uint32_t size )
{
	EWF_SECTORS_CHUNK *sectors_chunk = NULL;
	uint32_t sectors_chunk_size      = 0;

	sectors_chunk_size = size * sizeof( EWF_SECTORS_CHUNK );
	sectors_chunk      = (EWF_SECTORS_CHUNK *) libewf_alloc_cleared( sectors_chunk_size, 0 );

	if( sectors_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_alloc: unable to allocate sectors chunk.\n" );

		return( NULL );
	}
	return( sectors_chunk );
}

/* Reallocates memory for a ewf sectors_chunk
 * Returns a pointer to the instance, NULL on error
 */
EWF_SECTORS_CHUNK *ewf_sectors_chunk_realloc( EWF_SECTORS_CHUNK *sectors_chunk, uint32_t previous_size, uint32_t new_size )
{
	uint32_t previous_sectors_chunk_size = sizeof( EWF_SECTORS_CHUNK ) * previous_size;
	uint32_t new_sectors_chunk_size      = sizeof( EWF_SECTORS_CHUNK ) * new_size;

	if( sectors_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_realloc: invalid sectors chunk.\n" );

		return( NULL );
	}
	if( new_sectors_chunk_size <= previous_sectors_chunk_size )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_realloc: new size must be greater than previous size.\n" );

		return( NULL );
	}
	sectors_chunk = (EWF_SECTORS_CHUNK *) libewf_realloc( (void *) sectors_chunk, new_sectors_chunk_size );

	if( sectors_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_realloc: unable to allocate sectors chunk.\n" );

		return( NULL );
	}
	return( sectors_chunk );
}

/* Wipes memory for a ewf sectors_chunk
 * Returns a pointer to the instance, NULL on error
 */
EWF_SECTORS_CHUNK *ewf_sectors_chunk_wipe( EWF_SECTORS_CHUNK *sectors_chunk, uint32_t size )
{
	EWF_SECTORS_CHUNK *cleared_chunk = NULL;
	uint32_t sectors_chunk_size      = size * sizeof( EWF_SECTORS_CHUNK );

	if( sectors_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_realloc: invalid sectors chunk.\n" );

		return( NULL );
	}
	cleared_chunk = libewf_memset( sectors_chunk, 0, sectors_chunk_size );

	if( cleared_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_realloc: unable to clear sectores chunk.\n" );

		ewf_sectors_chunk_free( sectors_chunk );

		return( NULL );
	}
	return( cleared_chunk );
}

/* Frees memory of a sectors_chunk
 */
void ewf_sectors_chunk_free( EWF_SECTORS_CHUNK *sectors_chunk )
{
	if( sectors_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_free: invalid sectors_chunk.\n" );

		return;
	}
	libewf_free( sectors_chunk );
}

/* Uncompresses the sectors_chunk
 */
int8_t ewf_sectors_chunk_uncompress( EWF_SECTORS_CHUNK *sectors_chunk, uint32_t *size, EWF_SECTORS_CHUNK *compressed_sectors_chunk, uint32_t compressed_size )
{
	int8_t result = ewf_uncompress( sectors_chunk, size, compressed_sectors_chunk, compressed_size );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_uncompress: unable to uncompress chunk.\n" );

		return( -1 );
	}
	return( result );
}

/* Compresses the sectors_chunk
 * Returns a 1 if compression is successful, -1 on error
 */
int8_t ewf_sectors_chunk_compress( EWF_SECTORS_CHUNK *compressed_sectors_chunk, uint32_t *compressed_size, EWF_SECTORS_CHUNK *sectors_chunk, uint32_t size, int8_t compression_level )
{
	int8_t result = ewf_compress( compressed_sectors_chunk, compressed_size, sectors_chunk, size, compression_level );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_compress: unable to uncompress chunk.\n" );

		return( -1 );
	}
	return( result );
}

/* Reads the sectors_chunk from a file descriptor into a buffer
 * Returns a -1 on error, the amount of bytes read on success
 */
int32_t ewf_sectors_chunk_read( EWF_SECTORS_CHUNK *sectors_chunk, int file_descriptor, off_t offset, uint32_t size )
{
	int32_t count = 0;

	if( sectors_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_read: invalid sectors_chunk.\n" );

		return( -1 );
	}
	if( lseek( file_descriptor, offset, SEEK_SET ) < 0 )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_read: cannot find offset: %ld.\n", offset );

		return( -1 );
	}
	count = libewf_read( file_descriptor, sectors_chunk, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_read: unable to read sectors_chunk.\n" );

		return( -1 );
	}
	return( count );
}

/* Writes the sectors_chunk to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_sectors_chunk_write( EWF_SECTORS_CHUNK *sectors_chunk, int file_descriptor, uint32_t size )
{
	int32_t count = 0;

	if( sectors_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_write: invalid sectors chunk.\n" );

		return( -1 );
	}
	count = libewf_write( file_descriptor, sectors_chunk, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_sectors_chunk_write: unable to write sectors chunk.\n" );

		return( -1 );
	}
	return( count );
}

/* Print the sectors_chunk data to a stream
 */
void ewf_sectors_chunk_fprint( FILE *stream, EWF_SECTORS_CHUNK *sectors_chunk )
{
	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_section_fprint: invalid stream.\n" );

		return;
	}
	if( sectors_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_section_fprint: invalid sectors chunk.\n" );

		return;
	}
	fprintf( stream, "%s", sectors_chunk );
}

