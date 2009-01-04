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
	EWF_CRC stored_crc     = 0;
	EWF_CRC calculated_crc = 0;

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_dump_data: invalid size value exceeds maximum.\n" );

		return;
	}
	if( ewf_crc_calculate( &calculated_crc, data, ( size - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_dump_data: unable to calculate CRC.\n" );

		return;
	}
	libewf_dump_data( data, size );

	if( libewf_common_memcpy( &stored_crc, &data[ size - EWF_CRC_SIZE ], EWF_CRC_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_dump_data: unable to set CRC.\n" );
	}
	else
	{
		fprintf( stderr, "libewf_debug_dump_data: possible CRC (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );
	}
}

/* Reads section data
 */
void libewf_debug_read_section( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	uint8_t *data              = NULL;
	uint8_t *uncompressed_data = NULL;
	ssize_t read_count         = 0;
	size_t uncompressed_size   = 0;
	int8_t result              = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: invalid handle.\n" );

		return;
	}
	size *= sizeof( uint8_t );

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: invalid size value exceeds maximum.\n" );

		return;
	}
	data = (uint8_t *) libewf_common_alloc( size );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: unable to allocate data.\n" );

		return;
	}
	read_count = libewf_common_read( file_descriptor, data, size );

	if( read_count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: unable to read section data.\n" );

		libewf_common_free( data );

		return;
	}
	uncompressed_size = size + 1024;

	if( uncompressed_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: uncompressed size value exceeds maximum.\n" );

		return;
	}
	uncompressed_data = (uint8_t *) libewf_common_alloc( uncompressed_size );

	if( uncompressed_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: unable to allocate uncompressed data.\n" );

		libewf_common_free( data );

		return;
	}
	result = ewf_uncompress( uncompressed_data, &uncompressed_size, data, size );

	if( result == 0 )
	{
		fprintf( stderr, "libewf_debug_read_section: data is UNCOMPRESSED.\n" );

		libewf_debug_dump_data( data, size );
	}
	else if( result == 1 )
	{
		fprintf( stderr, "libewf_debug_read_section: data is zlib COMPRESSED.\n" );

		libewf_debug_dump_data( uncompressed_data, uncompressed_size );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: unable to uncompress data.\n" );

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
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;
	uint64_t next          = 0;
	uint64_t size          = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_section_fprint: invalid stream.\n" );

		return;
	}
	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_section_fprint: invalid section.\n" );

		return;
	}
	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) section, ( EWF_SECTION_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_section_fprint: unable to calculate CRC.\n" );

		return;
	}
	if( libewf_endian_convert_32bit( &stored_crc, section->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_section_fprint: unable to convert stored CRC value.\n" );

		return;
	}
	if( libewf_endian_convert_64bit( &next, section->next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_section_fprint: unable to convert next offset value.\n" );

		return;
	}
	if( libewf_endian_convert_64bit( &size, section->size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_section_fprint: unable to convert size value.\n" );

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
	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_header_string_fprint: invalid stream.\n" );

		return;
	}
	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_header_string_fprint: invalid header string.\n" );

		return;
	}
	fprintf( stream, "%" PRIs_EWF "", header_string );
}

/* Prints the header data to a stream
 */
void libewf_debug_header_fprint( FILE *stream, EWF_HEADER *header, size_t size )
{
	LIBEWF_CHAR *header_string = NULL;

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_header_fprint: invalid header.\n" );

		return;
	}
	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * size );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_header_fprint: unable to create header string.\n" );

		return;
	}
	if( libewf_string_copy_from_header( header_string, size, header, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_header_fprint: unable to copy header to header string.\n" );

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
	size_t header_size         = 0;

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_header2_fprint: invalid header2.\n" );

		return;
	}
	header_size   = ( size - 1 ) / 2;
	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * header_size );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_header2_fprint: unable to create header string.\n" );

		return;
	}
	if( libewf_string_copy_from_header2( header_string, header_size, header2, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_header2_fprint: unable to copy header2 to header string.\n" );

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
	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_chunk_fprint: invalid stream.\n" );

		return;
	}
	if( chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_chunk_fprint: invalid chunk.\n" );

		return;
	}
	fprintf( stream, "%s", (char *) chunk );
}

