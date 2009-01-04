/*
 * libewf file reading
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

#include "libewf_includes.h"

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_debug.h"
#include "libewf_endian.h"
#include "libewf_notify.h"
#include "libewf_string.h"

#include "ewf_compress.h"
#include "ewf_crc.h"

/* Prints a dump of data
 */
void libewf_debug_dump_data( uint8_t *data, size_t size )
{
	static char *function  = "libewf_debug_dump_data";
	EWF_CRC stored_crc     = 0;
	EWF_CRC calculated_crc = 0;

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return;
	}
	calculated_crc = ewf_crc_calculate( data, ( size - EWF_CRC_SIZE ), 1 );

	libewf_dump_data( data, size );

	if( libewf_common_memcpy( &stored_crc, &data[ size - EWF_CRC_SIZE ], EWF_CRC_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set CRC.\n",
		 function );
	}
	else
	{
		fprintf( stderr, "%s: possible CRC (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );
	}
}

/* Reads section data
 */
void libewf_debug_read_section( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size64_t size )
{
	static char *function      = "libewf_debug_read_section";
	uint8_t *data              = NULL;
	uint8_t *uncompressed_data = NULL;
	ssize_t read_count         = 0;
	size_t uncompressed_size   = 0;
	int result                 = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return;
	}
	size *= sizeof( uint8_t );

	if( size > (size64_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return;
	}
	data = (uint8_t *) libewf_common_alloc( (size_t) size );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate data.\n",
		 function );

		return;
	}
	read_count = libewf_common_read( file_descriptor, data, size );

	if( read_count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read section data.\n",
		 function );

		libewf_common_free( data );

		return;
	}
	uncompressed_size = (size_t) ( size + 1024 );

	if( uncompressed_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: uncompressed size value exceeds maximum.\n",
		 function );

		return;
	}
	uncompressed_data = (uint8_t *) libewf_common_alloc( uncompressed_size );

	if( uncompressed_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate uncompressed data.\n",
		 function );

		libewf_common_free( data );

		return;
	}
	result = ewf_uncompress( uncompressed_data, &uncompressed_size, data, (size_t) size );

	if( result == 0 )
	{
		fprintf( stderr, "%s: data is UNCOMPRESSED.\n",
		 function );

		libewf_debug_dump_data( data, (size_t) size );
	}
	else if( result == 1 )
	{
		fprintf( stderr, "%s: data is zlib COMPRESSED.\n",
		 function );

		libewf_debug_dump_data( uncompressed_data, uncompressed_size );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unable to uncompress data.\n",
		 function );

		libewf_common_free( data );
		libewf_common_free( uncompressed_data );

		return;
	}
	libewf_common_free( data );
	libewf_common_free( uncompressed_data );
}

/* Prints the section data to a stream
 */
void libewf_debug_section_fprint( FILE *stream, EWF_SECTION *section )
{
	static char *function  = "libewf_debug_section_fprint";
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;
	uint64_t next          = 0;
	uint64_t size          = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section.\n",
		 function );

		return;
	}
	calculated_crc = ewf_crc_calculate( section, ( EWF_SECTION_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, section->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		return;
	}
	if( libewf_endian_convert_64bit( &next, section->next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert next offset value.\n",
		 function );

		return;
	}
	if( libewf_endian_convert_64bit( &size, section->size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert size value.\n",
		 function );

		return;
	}
	fprintf( stream, "Section:\n" );
	fprintf( stream, "type: %s\n", (char *) section->type );
	fprintf( stream, "next: %" PRIu64 "\n", next );
	fprintf( stream, "size: %" PRIu64 "\n", size );
	fprintf( stream, "crc: %" PRIu32 " ( %" PRIu32 " )\n", stored_crc, calculated_crc );
	fprintf( stream, "\n" );
}

/* Prints a header string to a stream
 */
void libewf_debug_header_string_fprint( FILE *stream, LIBEWF_CHAR *header_string )
{
	static char *function = "libewf_debug_header_string_fprint";

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string.\n",
		 function );

		return;
	}
	fprintf( stream, "%" PRIs_EWF "", header_string );
}

/* Prints the header data to a stream
 */
void libewf_debug_header_fprint( FILE *stream, EWF_HEADER *header, size_t size )
{
	LIBEWF_CHAR *header_string = NULL;
	static char *function      = "libewf_debug_header_fprint";

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header.\n",
		 function );

		return;
	}
	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * size );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		return;
	}
	if( libewf_string_copy_from_header( header_string, size, header, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy header to header string.\n",
		 function );

		libewf_common_free( header_string );

		return;
	}
	libewf_debug_header_string_fprint( stream, header_string );

	libewf_common_free( header_string );
}

/* Prints the header2 data to a stream
 */
void libewf_debug_header2_fprint( FILE *stream, EWF_HEADER2 *header2, size_t size )
{
	LIBEWF_CHAR *header_string = NULL;
	static char *function      = "libewf_debug_header2_fprint";
	size_t header_size         = 0;

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header2.\n",
		 function );

		return;
	}
	header_size   = ( size - 1 ) / 2;
	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * header_size );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		return;
	}
	if( libewf_string_copy_from_header2( header_string, header_size, header2, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy header2 to header string.\n",
		 function );

		libewf_common_free( header_string );

		return;
	}
	libewf_debug_header_string_fprint( stream, header_string );

	libewf_common_free( header_string );
}

/* Print the chunk data to a stream
 */
void libewf_debug_chunk_fprint( FILE *stream, EWF_CHUNK *chunk )
{
	static char *function = "libewf_debug_chunk_fprint";

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk.\n",
		 function );

		return;
	}
	fprintf( stream, "%s", (char *) chunk );
}

