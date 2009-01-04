/*
 * EWF header section
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
#include "ewf_header.h"

/* Allocates memory for a new ewf header struct
 * Return a pointer to the new instance, NULL on error
 */
EWF_HEADER *ewf_header_alloc( uint32_t size )
{
	EWF_HEADER *header   = NULL;
	uint32_t header_size = 0;

	header_size = size * sizeof( EWF_HEADER );
	header      = (EWF_HEADER *) libewf_alloc_cleared( header_size, 0 );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_alloc: unable to allocate ewf_header.\n" );

		return( NULL );
	}
	return( header );
}

/* Frees memory of a header
 */
void ewf_header_free( EWF_HEADER *header )
{
	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_free: invalid header.\n" );

		return;
	}
	libewf_free( header );
}

/* Uncompresses the header
 * Return a pointer to the new instance, NULL on error
 */
EWF_HEADER *ewf_header_uncompress( EWF_HEADER *compressed_header, uint32_t *size )
{
	EWF_HEADER *uncompressed_header = NULL;
	uint32_t compressed_size        = 0;
	int8_t result                   = 0;

	if( compressed_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_uncompress: invalid compressed header.\n" );

		return( NULL );
	}
	if( size == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_uncompress: invalid size.\n" );

		return( NULL );
	}
	compressed_size = *size;

	/* Make sure the target buffer for the uncompressed data is large enough
	 */
	*size *= 16;

	uncompressed_header = ewf_header_alloc( *size );

	if( uncompressed_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_uncompress: unable to allocate uncompressed header.\n" );

		return( NULL );
	}
	result = ewf_uncompress( (uint8_t *) uncompressed_header, size, (uint8_t *) compressed_header, compressed_size );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_uncompress: unable to uncompress header.\n" );

		ewf_header_free( uncompressed_header );

		return( NULL );
	}
	return( uncompressed_header );
}

/* Compresses the header
 * Return a pointer to the new instance, NULL on error
 */
EWF_HEADER *ewf_header_compress( EWF_HEADER *uncompressed_header, uint32_t *size, int8_t compression_level )
{
	EWF_HEADER *compressed_header = NULL;
	uint32_t uncompressed_size    = 0;
	int8_t result                 = 0;

	if( uncompressed_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_compress: invalid uncompressed header.\n" );

		return( NULL );
	}
	if( size == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_compress: invalid size.\n" );

		return( NULL );
	}
	uncompressed_size = *size;

	/* Make sure the target buffer for the uncompressed data is large enough
	 */
	*size *= 16;

	compressed_header = ewf_header_alloc( *size );

	if( compressed_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_compress: unable to allocate compressed header.\n" );

		return( NULL );
	}
	result = ewf_compress( (uint8_t *) compressed_header, size, (uint8_t *) uncompressed_header, uncompressed_size, compression_level );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_compress: unable to compress header.\n" );

		ewf_header_free( compressed_header );

		return( NULL );
	}
	return( compressed_header );
}

/* Reads the header from a file descriptor
 * Return a pointer to the new instance, NULL on error
 */
EWF_HEADER *ewf_header_read( int file_descriptor, uint32_t *size )
{
	EWF_HEADER *uncompressed_header = NULL;
	EWF_HEADER *compressed_header   = NULL;
	int32_t count                   = 0;

	if( size == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_read: invalid size.\n" );

		return( NULL );
	}
	compressed_header = ewf_header_alloc( *size );

	if( compressed_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_read: unable to allocate compressed header.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, compressed_header, *size );

	if( count < *size )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_read: unable to read EWF header.\n" );

		ewf_header_free( compressed_header );

		return( NULL );
	}
	uncompressed_header = ewf_header_uncompress( compressed_header, size );

	if( uncompressed_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_read: unable to uncompress EWF header.\n" );

		ewf_header_free( compressed_header );

		return( NULL );
	}
	return( uncompressed_header );
}

/* Writes the header to a file descriptor
 * Returns the amount of bytes written on success, -1 on error
 */
int32_t ewf_header_write( EWF_HEADER *header, int file_descriptor, uint32_t size )
{
	int32_t count = 0;

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_write: invalid header.\n" );

		return( -1 );
	}
	count = libewf_write( file_descriptor, header, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_write: unable to write header.\n" );

		return( -1 );
	}
	return( count );
}

/* Print the header data to a stream
 */
void ewf_header_fprint( FILE *stream, EWF_HEADER *uncompressed_header )
{
	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_fprint: invalid stream.\n" );

		return;
	}
	if( uncompressed_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_header_fprint: invalid uncompressed header.\n" );

		return;
	}
	fprintf( stream, "%s", (char *) uncompressed_header );
}

