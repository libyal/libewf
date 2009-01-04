/*
 * libewf section
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
#include "libewf_compression.h"
#include "libewf_debug.h"
#include "libewf_endian.h"
#include "libewf_header_values.h"
#include "libewf_notify.h"
#include "libewf_section.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"

#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_error2.h"
#include "ewf_file_header.h"
#include "ewf_hash.h"
#include "ewf_ltree.h"
#include "ewf_session.h"
#include "ewf_volume.h"
#include "ewf_volume_smart.h"
#include "ewfx_delta_chunk.h"

/* Reads a section start from a segment file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_start_read( LIBEWF_SEGMENT_FILE *segment_file, EWF_SECTION *section, uint8_t error_tollerance )
{
	static char *function  = "libewf_section_start_read";
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;
	ssize_t read_count     = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              section,
	              EWF_SECTION_SIZE );

	if( read_count != EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read section start.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( section, ( EWF_SECTION_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, section->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	LIBEWF_VERBOSE_EXEC( libewf_debug_section_fprint( stderr, section ); );

#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( section->padding, 40 ); );
#endif
	return( read_count );
}

/* Writes a section start to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_start_write( LIBEWF_SEGMENT_FILE *segment_file, EWF_CHAR *section_type, size_t section_type_length, size_t section_data_size )
{
	EWF_SECTION section;

	static char *function   = "libewf_section_start_write";
	EWF_CRC calculated_crc  = 0;
	ssize_t write_count     = 0;
	uint64_t section_size   = 0;
	uint64_t section_offset = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section type.\n",
		 function );

		return( -1 );
	}
	if( section_type_length == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: section type is empty.\n",
		 function );

		return( -1 );
	}
	if( section_type_length >= 16 )
	{
		LIBEWF_WARNING_PRINT( "%s: section type is too long.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( &section, 0, EWF_SECTION_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear section.\n",
		 function );

		return( -1 );
	}
	/* Add one character for the end of string
	 */
	if( ewf_string_copy( section.type, section_type, ( section_type_length + 1 ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set section type.\n",
		 function );

		return( -1 );
	}
	section_size   = EWF_SECTION_SIZE + section_data_size;
	section_offset = segment_file->file_offset + section_size;

	if( libewf_endian_revert_64bit( section_size, section.size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert size value.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( section_offset, section.next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert next offset value.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &section, ( EWF_SECTION_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, section.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
	               segment_file,
	               &section,
	               EWF_SECTION_SIZE );

	if( write_count != EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Reads a compressed string section from a segment file and uncompresses it
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_compressed_string_read( LIBEWF_SEGMENT_FILE *segment_file, size_t compressed_string_size, EWF_CHAR **uncompressed_string, size_t *uncompressed_string_size )
{
	EWF_CHAR *compressed_string = NULL;
	EWF_CHAR *reallocation      = NULL;
	EWF_CHAR *uncompressed      = NULL;
	static char *function       = "libewf_section_compressed_string_read";
	ssize_t read_count          = 0;
	int result                  = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( ( uncompressed_string == NULL )
	 || ( *uncompressed_string != NULL ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid uncompressed string.\n",
		 function );

		return( -1 );
	}
	if( uncompressed_string_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid uncompressed string size.\n",
		 function );

		return( -1 );
	}
	if( compressed_string_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compressed string size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	compressed_string = (EWF_CHAR *) libewf_common_alloc( EWF_CHAR_SIZE * compressed_string_size );

	if( compressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create compressed string.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              compressed_string,
	              compressed_string_size );

	if( read_count != (ssize_t) compressed_string_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read compressed string.\n",
		 function );

		libewf_common_free( compressed_string );

		return( -1 );
	}
	/* On average the uncompressed string will be twice as large as the compressed string
	 */
	*uncompressed_string_size = 2 * compressed_string_size;

	/* Add a byte for the end of string
	 */
	uncompressed = (EWF_CHAR *) libewf_common_alloc( EWF_CHAR_SIZE * ( *uncompressed_string_size + 1 ) );

	if( uncompressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate uncompressed string.\n",
		 function );

		libewf_common_free( compressed_string );

		return( -1 );
	}
	result = libewf_uncompress(
	          (uint8_t *) uncompressed,
	          uncompressed_string_size,
	          (uint8_t *) compressed_string,
	          compressed_string_size );

	while( ( result == -1 )
	 && ( *uncompressed_string_size > 0 ) )
	{
		/* Add a byte for the end of string
		 */
		reallocation = (EWF_CHAR *) libewf_common_realloc( uncompressed, ( *uncompressed_string_size + 1 ) );

		if( reallocation == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate uncompressed string.\n",
			 function );

			libewf_common_free( compressed_string );
			libewf_common_free( uncompressed );

			return( -1 );
		}
		uncompressed = reallocation;

		result = libewf_uncompress(
		          (uint8_t *) uncompressed,
		          uncompressed_string_size,
		          (uint8_t *) compressed_string,
		          compressed_string_size );
	}
	libewf_common_free( compressed_string );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to uncompress string.\n",
		 function );

		libewf_common_free( uncompressed );

		return( -1 );
	}
	uncompressed[ *uncompressed_string_size ] = (EWF_CHAR) '\0';

	*uncompressed_string       = uncompressed;
	*uncompressed_string_size += 1;

	return( read_count );
}

/* Writes a compressed string section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_write_compressed_string( LIBEWF_SEGMENT_FILE *segment_file, EWF_CHAR *section_type, size_t section_type_length, EWF_CHAR *uncompressed_string, size_t uncompressed_string_size, int8_t compression_level )
{
	EWF_CHAR *compressed_string   = NULL;
	EWF_CHAR *reallocation        = NULL;
	static char *function         = "libewf_section_write_compressed_string";
	off64_t section_offset        = 0;
	size_t compressed_string_size = 0;
	ssize_t section_write_count   = 0;
	ssize_t write_count           = 0;
	int result                    = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section type.\n",
		 function );

		return( -1 );
	}
	if( uncompressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid uncompressed string.\n",
		 function );

		return( -1 );
	}
	section_offset         = segment_file->file_offset;
	compressed_string_size = uncompressed_string_size;
	compressed_string      = (EWF_CHAR *) libewf_common_alloc( EWF_CHAR_SIZE * compressed_string_size );

	if( compressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create compress string.\n",
		 function );

		return( -1 );
	}
	result = libewf_compress(
	          (uint8_t *) compressed_string,
	          &compressed_string_size,
	          (uint8_t *) uncompressed_string,
	          uncompressed_string_size,
	          compression_level );

	if( ( result == -1 )
	 && ( compressed_string_size > 0 ) )
	{
		reallocation = (EWF_CHAR *) libewf_common_realloc( compressed_string, compressed_string_size );

		if( reallocation == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate compressed string.\n",
			 function );

			libewf_common_free( compressed_string );

			return( -1 );
		}
		compressed_string = reallocation;

		result = libewf_compress(
		          (uint8_t *) compressed_string,
		          &compressed_string_size,
		          (uint8_t *) uncompressed_string,
		          uncompressed_string_size,
		          compression_level );
	}
	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to compress string.\n",
		 function );

		libewf_common_free( compressed_string );

		return( -1 );
	}
	section_write_count = libewf_section_start_write(
	                       segment_file,
	                       section_type,
	                       section_type_length,
	                       compressed_string_size );

	if( section_write_count != (ssize_t) EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( compressed_string );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
	               segment_file,
	               compressed_string,
	               compressed_string_size );

	libewf_common_free( compressed_string );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write string to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file->section_list,
	     section_type,
	     section_offset,
	     ( section_offset + section_write_count ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to append: %s section to section list.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a header section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_header_read( LIBEWF_SEGMENT_FILE *segment_file, size_t section_size, EWF_CHAR **cached_header, size_t *cached_header_size )
{
	EWF_CHAR *header      = NULL;
	static char *function = "libewf_section_header_read";
	ssize_t read_count    = 0;
	size_t header_size    = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( cached_header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid cached header.\n",
		 function );

		return( -1 );
	}
	if( cached_header_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid cached header size.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              segment_file,
	              section_size,
	              &header,
	              &header_size );

	if( read_count != (ssize_t) section_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read header.\n",
		 function );

		return( -1 );
	}
	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, header, header_size ); );

	if( *cached_header == NULL )
	{
		*cached_header = header;
		*cached_header_size = header_size;
	}
	else
	{
		libewf_common_free( header );
	}
	return( read_count );
}

/* Writes a header section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_header_write( LIBEWF_SEGMENT_FILE *segment_file, EWF_CHAR *header, size_t header_size, int8_t compression_level )
{
	static char *function       = "libewf_section_header_write";
	ssize_t section_write_count = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, header, header_size ); );

	section_write_count = libewf_section_write_compressed_string(
	                       segment_file,
	                       (EWF_CHAR *) "header",
	                       6,
	                       header,
	                       header_size,
	                       compression_level );

	/* refactor */
	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write header to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a header2 section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_header2_read( LIBEWF_SEGMENT_FILE *segment_file, size_t section_size, EWF_CHAR **cached_header2, size_t *cached_header2_size )
{
	EWF_CHAR *header2     = NULL;
	static char *function = "libewf_section_header2_read";
	ssize_t read_count    = 0;
	size_t header2_size   = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( cached_header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid cached header2.\n",
		 function );

		return( -1 );
	}
	if( cached_header2_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid cached header2 size.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              segment_file,
	              section_size,
	              &header2,
	              &header2_size );

	if( read_count != (ssize_t) section_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read header2.\n",
		 function );

		return( -1 );
	}
	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header2.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header2:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header2_fprint( stderr, header2, header2_size ); );

	if( *cached_header2 == NULL )
	{
		*cached_header2      = header2;
		*cached_header2_size = header2_size;
	}
	else
	{
		libewf_common_free( header2 );
	}
	return( read_count );
}

/* Writes a header2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_header2_write( LIBEWF_SEGMENT_FILE *segment_file, EWF_CHAR *header2, size_t header2_size, int8_t compression_level )
{
	static char *function       = "libewf_section_header2_write";
	ssize_t section_write_count = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header2:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header2_fprint( stderr, header2, header2_size ); );

	section_write_count = libewf_section_write_compressed_string(
	                       segment_file,
	                       (EWF_CHAR *) "header2",
	                       7,
	                       header2,
	                       header2_size,
	                       compression_level );

	/* refactor */
	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write header2 to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads an EWF-S01 (SMART) volume section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_s01_read( LIBEWF_SEGMENT_FILE *segment_file, uint32_t *amount_of_chunks, uint32_t *sectors_per_chunk, uint32_t *bytes_per_sector, uint32_t *amount_of_sectors, uint8_t *format, uint8_t error_tollerance )
{
	EWF_VOLUME_SMART *volume = NULL;
	static char *function    = "libewf_section_volume_s01_read";
	EWF_CRC calculated_crc   = 0;
	EWF_CRC stored_crc       = 0;
	ssize_t read_count       = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( amount_of_chunks == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of chunks.\n",
		 function );

		return( -1 );
	}
	if( sectors_per_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector.\n",
		 function );

		return( -1 );
	}
	if( amount_of_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of sectors.\n",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid format.\n",
		 function );

		return( -1 );
	}
	volume = (EWF_VOLUME_SMART *) libewf_common_alloc( EWF_VOLUME_SMART_SIZE );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate volume.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              volume,
	              EWF_VOLUME_SMART_SIZE );

	if( read_count != EWF_VOLUME_SMART_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( volume, ( EWF_VOLUME_SMART_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, volume->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown2, 20 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown3, 45 ); );
#endif

	if( libewf_endian_convert_32bit( amount_of_chunks, volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_common_memcmp( (void *) volume->signature, (void *) "SMART", 5 ) == 0 )
	{
		*format = LIBEWF_FORMAT_SMART;
	}
	else
	{
		*format = LIBEWF_FORMAT_EWF;
	}
	libewf_common_free( volume );

	return( read_count );
}

/* Writes an EWF-S01 (SMART) volume section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_volume_s01_write( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, uint8_t no_section_append )
{
	EWF_CHAR *section_type      = (EWF_CHAR *) "volume";
	EWF_VOLUME_SMART *volume    = NULL;
	static char *function       = "libewf_section_volume_s01_write";
	EWF_CRC calculated_crc      = 0;
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file->file_offset;

	volume = (EWF_VOLUME_SMART *) libewf_common_alloc( EWF_VOLUME_SMART_SIZE );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create volume.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( volume, 0, EWF_VOLUME_SMART_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear volume.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	volume->unknown1[ 0 ] = 1;

	if( libewf_endian_revert_32bit( internal_handle->media_values->amount_of_chunks, volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media_values->sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media_values->bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media_values->amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( internal_handle->format == LIBEWF_FORMAT_SMART )
	{
		volume->signature[ 0 ] = (uint8_t) 'S';
		volume->signature[ 1 ] = (uint8_t) 'M';
		volume->signature[ 2 ] = (uint8_t) 'A';
		volume->signature[ 3 ] = (uint8_t) 'R';
		volume->signature[ 4 ] = (uint8_t) 'T';
	}
	calculated_crc = ewf_crc_calculate( volume, ( EWF_VOLUME_SMART_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, volume->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: amount_of_chunks: %" PRIu32 ", sectors_per_chunk: %" PRIu32 ", bytes_per_sector: %" PRIu32 ", amount_of_sectors: %" PRIu32 ".\n",
	 function, internal_handle->media_values->amount_of_chunks, internal_handle->media_values->sectors_per_chunk,
	 internal_handle->media_values->bytes_per_sector, internal_handle->media_values->amount_of_sectors );

	section_write_count = libewf_section_start_write(
	                       segment_file,
	                       section_type,
	                       6,
	                       EWF_VOLUME_SMART_SIZE );

	if( section_write_count != (ssize_t) EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
	               segment_file,
	               volume,
	               EWF_VOLUME_SMART_SIZE );

	libewf_common_free( volume );

	if( write_count != (ssize_t) EWF_VOLUME_SMART_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write volume to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( no_section_append == 0 )
	{
		if( libewf_section_list_append(
		     segment_file->section_list,
		     section_type,
		     section_offset,
		     ( section_offset + section_write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to append: %s section to section list.\n",
			 function, (char *) section_type );

			return( -1 );
		}
	}
	return( section_write_count );
}

/* Reads an EWF-E01 (EnCase) volume section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_e01_read( LIBEWF_SEGMENT_FILE *segment_file, uint32_t *amount_of_chunks, uint32_t *sectors_per_chunk, uint32_t *bytes_per_sector, uint32_t *amount_of_sectors, uint32_t *error_granularity, uint8_t *media_type, uint8_t *media_flags, int8_t *compression_level, uint8_t *guid, uint8_t error_tollerance )
{
	EWF_VOLUME *volume     = NULL;
	static char *function  = "libewf_section_volume_e01_read";
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;
	ssize_t read_count     = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( amount_of_chunks == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of chunks.\n",
		 function );

		return( -1 );
	}
	if( sectors_per_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector.\n",
		 function );

		return( -1 );
	}
	if( amount_of_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of sectors.\n",
		 function );

		return( -1 );
	}
	if( error_granularity == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid error granularity.\n",
		 function );

		return( -1 );
	}
	if( media_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media type.\n",
		 function );

		return( -1 );
	}
	if( media_flags == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media flags.\n",
		 function );

		return( -1 );
	}
	if( compression_level == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compression level.\n",
		 function );

		return( -1 );
	}
	if( guid == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid GUID.\n",
		 function );

		return( -1 );
	}
	volume = (EWF_VOLUME *) libewf_common_alloc( EWF_VOLUME_SIZE );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              volume,
	              EWF_VOLUME_SIZE );

	if( read_count != EWF_VOLUME_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( volume, ( EWF_VOLUME_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, volume->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown1, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown2, 16 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown3, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown4, 12 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown5, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown6, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown7, 963 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->signature, 5 ); );
#endif

	if( libewf_endian_convert_32bit( amount_of_chunks, volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( error_granularity, volume->error_granularity ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert error granularity value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	*media_type        = volume->media_type;
	*media_flags       = volume->media_flags;
	*compression_level = (int8_t) volume->compression_level;

	if( libewf_common_memcpy( guid, volume->guid, 16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume );

			return( -1 );
		}
	}
	libewf_common_free( volume );

	return( read_count );
}

/* Writes an EWF-E01 (EnCase) volume section to file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_e01_write( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, uint8_t no_section_append )
{
	EWF_CHAR *section_type      = (EWF_CHAR *) "volume";
	EWF_VOLUME *volume          = NULL;
	static char *function       = "libewf_section_volume_e01_write";
	EWF_CRC calculated_crc      = 0;
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file->file_offset;

	volume = (EWF_VOLUME *) libewf_common_alloc( EWF_VOLUME_SIZE );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create volume.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( volume, 0, EWF_VOLUME_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear volume.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( internal_handle->format == LIBEWF_FORMAT_FTK )
	{
		volume->media_type = 0x01;
	}
	else
	{
		volume->media_type = internal_handle->media_values->media_type;
	}
	volume->media_flags = internal_handle->media_values->media_flags;

	if( libewf_endian_revert_32bit( internal_handle->media_values->amount_of_chunks, volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media_values->sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media_values->bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media_values->amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) )
	{
		volume->compression_level = (uint8_t) internal_handle->compression_level;

		if( libewf_common_memcpy( volume->guid, internal_handle->media_values->guid, 16 ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
			 function );

			libewf_common_free( volume );

			return( -1 );
		}
		if( libewf_endian_revert_32bit( internal_handle->media_values->error_granularity, volume->error_granularity ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert error granularity value.\n",
			 function );

			libewf_common_free( volume );

			return( -1 );
		}
	}
	calculated_crc = ewf_crc_calculate( volume, ( EWF_VOLUME_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, volume->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: amount_of_chunks: %" PRIu32 ", sectors_per_chunk: %" PRIu32 ", bytes_per_sector: %" PRIu32 ", amount_of_sectors: %" PRIu32 ".\n",
	 function, internal_handle->media_values->amount_of_chunks, internal_handle->media_values->sectors_per_chunk,
	 internal_handle->media_values->bytes_per_sector, internal_handle->media_values->amount_of_sectors );

	section_write_count = libewf_section_start_write(
	                       segment_file,
	                       section_type,
	                       6,
	                       EWF_VOLUME_SIZE );

	if( section_write_count != (ssize_t) EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
	               segment_file,
	               volume,
	               EWF_VOLUME_SIZE );

	libewf_common_free( volume );

	if( write_count != (ssize_t) EWF_VOLUME_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write volume to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( no_section_append == 0 )
	{
		if( libewf_section_list_append(
		     segment_file->section_list,
		     section_type,
		     section_offset,
		     ( section_offset + section_write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to append: %s section to section list.\n",
			 function, (char *) section_type );

			return( -1 );
		}
	}
	return( section_write_count );
}

/* Reads a volume section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_read( LIBEWF_SEGMENT_FILE *segment_file, size_t section_size, uint32_t *amount_of_chunks, uint32_t *sectors_per_chunk, uint32_t *bytes_per_sector, uint32_t *amount_of_sectors, uint32_t *chunk_size, uint32_t *error_granularity, uint8_t *media_type, uint8_t *media_flags, int8_t *compression_level, uint8_t *guid, uint8_t *format, uint8_t *ewf_format, uint8_t error_tollerance )
{
	static char *function    = "libewf_section_volume_read";
	ssize_t read_count       = 0;
	size64_t bytes_per_chunk = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( amount_of_chunks == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of chunks.\n",
		 function );

		return( -1 );
	}
	if( sectors_per_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector.\n",
		 function );

		return( -1 );
	}
	if( amount_of_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of sectors.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid format.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid ewf format.\n",
		 function );

		return( -1 );
	}
	if( section_size == EWF_VOLUME_SMART_SIZE )
	{
		*ewf_format = EWF_FORMAT_S01;
		read_count  = libewf_section_volume_s01_read(
		               segment_file,
		               amount_of_chunks,
		               sectors_per_chunk,
		               bytes_per_sector,
		               amount_of_sectors,
		               format,
		               error_tollerance );
	}
	else if( section_size == EWF_VOLUME_SIZE )
	{
		*ewf_format = EWF_FORMAT_E01;
		read_count  = libewf_section_volume_e01_read(
		               segment_file,
		               amount_of_chunks,
		               sectors_per_chunk,
		               bytes_per_sector,
		               amount_of_sectors,
		               error_granularity,
		               media_type,
		               media_flags,
		               compression_level,
		               guid,
		               error_tollerance );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: mismatch in section data size.\n",
		 function );

		return( -1 );
	}
	if( read_count != (ssize_t) section_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume section.\n",
		 function );

		return( -1 );
	}
	if( *sectors_per_chunk > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( *bytes_per_sector > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	bytes_per_chunk = (size64_t) *sectors_per_chunk * (size64_t) *bytes_per_sector;

	if( bytes_per_chunk > (size64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk size value exceeds maximum.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		*chunk_size = EWF_MINIMUM_CHUNK_SIZE;
	}
	else
	{
		*chunk_size = (uint32_t) bytes_per_chunk;
	}
	LIBEWF_VERBOSE_PRINT( "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes each.\n",
	 function, *amount_of_chunks, *chunk_size );

	LIBEWF_VERBOSE_PRINT( "%s: volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n",
	 function, *amount_of_sectors, *bytes_per_sector );

	if( *amount_of_chunks == 0 )
	{
		*ewf_format = EWF_FORMAT_L01;
	}
	return( read_count );
}

/* Reads an offset table from file
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_read( LIBEWF_SEGMENT_FILE *segment_file, LIBEWF_OFFSET_TABLE *offset_table, uint32_t *amount_of_chunks, off64_t section_offset, size_t section_size, uint8_t ewf_format, uint8_t error_tollerance )
{
	uint8_t stored_crc_buffer[ 4 ];

	EWF_TABLE *table          = NULL;
	EWF_TABLE_OFFSET *offsets = NULL;
	static char *function     = "libewf_offset_table_read";
	EWF_CRC calculated_crc    = 0;
	EWF_CRC stored_crc        = 0;
	size_t offsets_size       = 0;
	ssize_t read_count        = 0;
	uint64_t base_offset      = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( amount_of_chunks == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of chunks.\n",
		 function );

		return( -1 );
	}
	table = (EWF_TABLE *) libewf_common_alloc( EWF_TABLE_SIZE );

	if( table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate table.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              table,
	              EWF_TABLE_SIZE );
	
	if( read_count != (ssize_t) EWF_TABLE_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read table.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	if( libewf_endian_convert_64bit( &base_offset, table->base_offset ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert base offset value.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( table->padding1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( table->padding2, 4 ); );
#endif

	/* The table size contains the size of the CRC (4 bytes)
	 */
	calculated_crc = ewf_crc_calculate( table, ( EWF_TABLE_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, table->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( table );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( amount_of_chunks, table->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	libewf_common_free( table );

	LIBEWF_VERBOSE_PRINT( "%s: table is of size %" PRIu32 " chunks CRC %" PRIu32 " (%" PRIu32 ").\n",
	 function, *amount_of_chunks, stored_crc, calculated_crc );

	if( *amount_of_chunks == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: table contains no offsets.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	else
	{
		/* Check if the maximum amount of offsets is not exceeded
		 */
		if( *amount_of_chunks > EWF_MAXIMUM_OFFSETS_IN_TABLE )
		{
			LIBEWF_WARNING_PRINT( "%s: table contains more than %d offsets!.\n",
			 function, EWF_MAXIMUM_OFFSETS_IN_TABLE );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
		offsets_size = EWF_TABLE_OFFSET_SIZE * *amount_of_chunks;
		offsets      = (EWF_TABLE_OFFSET *) libewf_common_alloc( offsets_size );

		if( offsets == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocated table offsets.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_segment_file_read(
		              segment_file,
		              offsets,
		              offsets_size );
	
		if( read_count != (ssize_t) offsets_size )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read table offsets.\n",
			 function );

			libewf_common_free( offsets );

			return( -1 );
		}
		/* The EWF-S01 format does not contain a CRC after the offsets
		 */
		if( ewf_format != EWF_FORMAT_S01 )
		{
			/* Check if the offset table CRC matches
			 */
			calculated_crc = ewf_crc_calculate( offsets, offsets_size, 1 );

			read_count = libewf_segment_file_read(
			              segment_file,
			              stored_crc_buffer,
			              EWF_CRC_SIZE );

			if( read_count != (ssize_t) EWF_CRC_SIZE )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to read CRC from file descriptor.\n",
				 function );

				libewf_common_free( offsets );

				return( -1 );
			}
			if( libewf_endian_convert_32bit( &stored_crc, stored_crc_buffer ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to convert CRC value.\n",
				 function );

				libewf_common_free( offsets );

				return( -1 );
			}
			if( stored_crc != calculated_crc )
			{
				LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
				 function, stored_crc, calculated_crc );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					libewf_common_free( offsets );

					return( -1 );
				}
			}
		}
		if( libewf_offset_table_fill(
		     offset_table,
		     (off64_t) base_offset,
		     offsets,
		     *amount_of_chunks,
		     segment_file,
		     error_tollerance ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to fill offset table.\n",
			 function );

			libewf_common_free( offsets );

			return( -1 );
		}
		libewf_common_free( offsets );

		if( libewf_offset_table_calculate_last_offset(
		     offset_table,
		     segment_file->section_list,
		     error_tollerance ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to calculate last offset.\n",
			 function );

			return( -1 );
		}
	}
	/* Skip the chunk data within the section
	 * for chunks after the table section
	 */
	section_offset += section_size;

	if( section_offset != segment_file->file_offset )
	{
		LIBEWF_VERBOSE_PRINT( "%s: found data after table offsets.\n",
		 function );

		if( libewf_segment_file_seek_offset(
		     segment_file,
		     section_offset ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads a table section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_table_read( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, off64_t section_offset, size_t section_size )
{
	static char *function     = "libewf_section_table_read";
	uint32_t amount_of_chunks = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		internal_handle->offset_table = libewf_offset_table_alloc(
		                                 internal_handle->media_values->amount_of_chunks );

		if( internal_handle->offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create offset table.\n",
			 function );

			return( -1 );
		}
	}
	if( libewf_offset_table_read(
	     segment_file,
	     internal_handle->offset_table,
	     &amount_of_chunks,
	     section_offset,
	     section_size,
	     internal_handle->ewf_format,
	     internal_handle->error_tollerance ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read offset table.\n",
		 function );

		return( -1 );
	}
	segment_file->amount_of_chunks += amount_of_chunks;

	return( (ssize_t) section_size );
}

/* Writes a table or table2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_table_write( LIBEWF_SEGMENT_FILE *segment_file, off64_t base_offset, LIBEWF_OFFSET_TABLE *offset_table, uint32_t offset_table_index, uint32_t amount_of_offsets, EWF_CHAR *section_type, size_t section_type_length, size_t additional_size, uint8_t format, uint8_t ewf_format, uint8_t no_section_append )
{
	EWF_TABLE table;
	uint8_t calculated_crc_buffer[ 4 ];

	EWF_TABLE_OFFSET *offsets   = NULL;
	static char *function       = "libewf_section_table_write";
	EWF_CRC calculated_crc      = 0;
	off64_t section_offset      = 0;
	off64_t offset64_value      = 0;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;
	size_t section_size         = 0;
	size_t offsets_size         = 0;
	uint32_t offset32_value     = 0;
	uint32_t iterator           = 0;
	uint8_t overflow            = 0;
	uint8_t write_crc           = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file\n",
		 function );

		return( -1 );
	}
	if( base_offset <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid base offset.\n",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( offset_table->chunk_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing chunk offsets.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file->file_offset;
	offsets_size   = EWF_TABLE_OFFSET_SIZE * amount_of_offsets;
	section_size   = EWF_TABLE_SIZE + offsets_size + additional_size;

	if( ewf_format != EWF_FORMAT_S01 )
	{
		write_crc     = 1;
		section_size += EWF_CRC_SIZE;
	}
	if( libewf_common_memset( &table, 0, EWF_TABLE_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear table.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( amount_of_offsets, table.amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( base_offset, table.base_offset ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert base offset value.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &table, ( EWF_TABLE_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, table.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	offsets = (EWF_TABLE_OFFSET *) libewf_common_alloc( offsets_size );

	if( offsets == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create offsets.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_offsets; iterator++ )
	{
		offset64_value = offset_table->chunk_offset[ offset_table_index + iterator ].file_offset
		               - base_offset;

		if( ( overflow == 0 )
		 && ( ( offset64_value < 0 )
		  || ( offset64_value > (off64_t) INT32_MAX ) ) )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid chunk offset value.\n",
			 function );

			libewf_common_free( offsets );

			return( -1 );
		}
		offset32_value = (uint32_t) offset64_value;

		if( offset_table->chunk_offset[ offset_table_index + iterator ].compressed != 0 )
		{
			if( overflow == 0 )
			{
				offset32_value |= EWF_OFFSET_COMPRESSED_WRITE_MASK;
			}
			else
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write compressed chunks after chunk overflow.\n",
				 function );

				libewf_common_free( offsets );

				return( -1 );
			}
		}
		if( libewf_endian_revert_32bit( offset32_value, (uint8_t *) offsets[ iterator ].offset ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert start offset.\n",
			 function );

			libewf_common_free( offsets );

			return( -1 );
		}
		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6
		 */
		if( ( overflow == 0 )
		 && ( ( offset64_value + offset_table->chunk_offset[ offset_table_index + iterator ].size ) > (off64_t) INT32_MAX ) )
		{
			if( ( format == LIBEWF_FORMAT_ENCASE6 )
			 || ( format == LIBEWF_FORMAT_LINEN6 ) )
			{
				LIBEWF_VERBOSE_PRINT( "%s: chunk offset overflow at: %" PRIi64 ".\n",
				 function, offset64_value );

				overflow = 1;
			}
			else
			{
				LIBEWF_WARNING_PRINT( "%s: chunk offset overflow at: %" PRIi64 ".\n",
				 function, offset64_value );

				libewf_common_free( offsets );

				return( -1 );
			}
		}
	}
	if( write_crc != 0 )
	{
		calculated_crc = ewf_crc_calculate( offsets, offsets_size, 1 );
	}
	section_write_count = libewf_section_start_write(
	                       segment_file,
	                       section_type,
	                       section_type_length,
	                       section_size );

	if( section_write_count != (ssize_t) EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		libewf_common_free( offsets );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
	               segment_file,
	               &table,
	               EWF_TABLE_SIZE );

	if( write_count != (ssize_t) EWF_TABLE_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write table to file.\n",
		 function );

		libewf_common_free( offsets );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_segment_file_write(
	               segment_file,
	               offsets,
	               offsets_size );

	libewf_common_free( offsets );

	if( write_count != (ssize_t) offsets_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write table offsets to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( write_crc != 0 )
	{
		if( libewf_endian_revert_32bit( calculated_crc, calculated_crc_buffer ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
			 function );

			return( -1 );
		}
		write_count = libewf_segment_file_write(
		               segment_file,
	        	       calculated_crc_buffer,
	        	       EWF_CRC_SIZE );

		if( write_count != (ssize_t) EWF_CRC_SIZE )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write table offsets CRC to file.\n",
			 function );

			return( -1 );
		}
		section_write_count += write_count;
	}
	if( no_section_append == 0 )
	{
		if( libewf_section_list_append(
		     segment_file->section_list,
		     section_type,
		     section_offset,
		     ( section_offset + section_write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to append: %s section to section list.\n",
			 function, (char *) section_type );

			return( -1 );
		}
	}
	return( section_write_count );
}

/* Reads a table2 section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_table2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, off64_t section_offset, size_t section_size )
{
	static char *function     = "libewf_section_table2_read";
	uint32_t amount_of_chunks = 0;
	int result                = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->secondary_offset_table == NULL )
	{
		internal_handle->secondary_offset_table = libewf_offset_table_alloc( internal_handle->media_values->amount_of_chunks );

		if( internal_handle->secondary_offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create secondairy offset table.\n",
			 function );

			return( -1 );
		}
	}
	if( libewf_offset_table_read(
	     segment_file,
	     internal_handle->secondary_offset_table,
	     &amount_of_chunks,
	     section_offset,
	     section_size,
	     internal_handle->ewf_format,
	     internal_handle->error_tollerance ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read offset table.\n",
		 function );

		return( -1 );
	}
	result = libewf_offset_table_compare(
	          internal_handle->offset_table,
	          internal_handle->secondary_offset_table );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to compare table1 and table2.\n",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: table1 and table2 differ.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		/* TODO Try to correct the table
		 */
	}
	return( (ssize_t) section_size );
}

/* Reads a sectors section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize64_t libewf_section_sectors_read( LIBEWF_SEGMENT_FILE *segment_file, off64_t section_offset, size64_t section_size, uint8_t ewf_format, uint8_t error_tollerance )
{
	static char *function = "libewf_section_sectors_read";

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* In the EWF-E01 format the sectors section holds the actual data chunks
	 */
	if( ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain sectors section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	/* Skip the chunk data within the section
	 */
	if( libewf_segment_file_seek_offset(
	     segment_file,
	     ( section_offset + section_size ) ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
		 function );

		return( -1 );
	}
	return( (ssize64_t) section_size );
}

/* Reads a ltree section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_ltree_read( LIBEWF_SEGMENT_FILE *segment_file, size_t section_size, uint8_t *ewf_format, uint8_t error_tollerance )
{
	EWF_LTREE *ltree       = NULL;
	EWF_CHAR *ltree_data   = NULL;
	static char *function = "libewf_section_ltree_read";
	size_t ltree_data_size = 0;
	ssize_t read_count     = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid ewf format.\n",
		 function );

		return( -1 );
	}
	if( *ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain ltree section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	*ewf_format = EWF_FORMAT_L01;

	ltree = (EWF_LTREE *) libewf_common_alloc( EWF_LTREE_SIZE );

	if( ltree == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate ltree.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              ltree,
	              EWF_LTREE_SIZE );
	
	if( read_count != (ssize_t) EWF_LTREE_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read ltree.\n",
		 function );

		libewf_common_free( ltree );

		return( -1 );
	}
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown1, 16 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->tree_size, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown2, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown3, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown4, 20 ); );

	libewf_common_free( ltree );

	ltree_data_size = section_size - EWF_LTREE_SIZE;

	ltree_data = (EWF_CHAR *) libewf_common_alloc( EWF_CHAR_SIZE * ltree_data_size );

	if( ltree_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate ltree data.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              ltree_data,
	              ltree_data_size );

	if( read_count != (ssize_t) ltree_data_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read ltree data.\n",
		 function );

		libewf_common_free( ltree_data );

		return( -1 );
	}
	LIBEWF_VERBOSE_EXEC( libewf_debug_header2_fprint( stderr, ltree_data, ltree_data_size ); );

	libewf_common_free( ltree_data );

	return( (ssize_t) section_size );
}

/* Reads a session section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_session_read( LIBEWF_SEGMENT_FILE *segment_file, size_t size, uint8_t ewf_format, uint8_t error_tollerance )
{
	EWF_SESSION session;

	static char *function  = "libewf_section_session_read";
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;
	ssize_t read_count     = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( size != EWF_SESSION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: mismatch in section session size.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain session section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              &session,
	              EWF_SESSION_SIZE );

	if( read_count != (ssize_t) EWF_SESSION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read session.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &session, ( EWF_SESSION_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, session.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( session.unknown, 68 ); );
#endif

	return( (ssize_t) size );
}

/* Reads a data section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_data_read( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, size_t section_size, uint8_t ewf_format, uint8_t error_tollerance )
{
	EWF_DATA *data             = NULL;
	static char *function      = "libewf_section_data_read";
	EWF_CRC calculated_crc     = 0;
	EWF_CRC stored_crc         = 0;
	ssize_t read_count         = 0;
	uint32_t amount_of_chunks  = 0;
	uint32_t sectors_per_chunk = 0;
	uint32_t bytes_per_sector  = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t error_granularity = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	if( section_size != EWF_DATA_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: mismatch in section data size.\n",
		 function );

		return( -1 );
	}
	data = (EWF_DATA *) libewf_common_alloc( EWF_DATA_SIZE );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create data.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              data,
	              EWF_DATA_SIZE );
	
	if( read_count != (ssize_t) EWF_DATA_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read data.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( data, ( EWF_DATA_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, data->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown1, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown2, 16 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown3, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown4, 12 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown5, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown6, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown7, 963 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->signature, 5 ); );
#endif
	/* TODO add more checks
	 */
	if( internal_handle->media_values->media_type != data->media_type )
	{
		LIBEWF_WARNING_PRINT( "%s: media type does not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &amount_of_chunks, data->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media_values->amount_of_chunks != amount_of_chunks )
	{
		LIBEWF_WARNING_PRINT( "%s: amount of chunks does not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &sectors_per_chunk, data->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert sectors per chunk value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media_values->sectors_per_chunk != sectors_per_chunk )
	{
		LIBEWF_WARNING_PRINT( "%s: sectors per chunk does not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &bytes_per_sector, data->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert bytes per sector value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media_values->bytes_per_sector != bytes_per_sector )
	{
		LIBEWF_WARNING_PRINT( "%s: bytes per sector does not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &amount_of_sectors, data->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media_values->amount_of_sectors != amount_of_sectors )
	{
		LIBEWF_WARNING_PRINT( "%s: amount of sectors does not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &error_granularity, data->error_granularity ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert error granularity value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media_values->error_granularity != error_granularity )
	{
		LIBEWF_WARNING_PRINT( "%s: error granularity does not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( internal_handle->media_values->media_flags != data->media_flags )
	{
		LIBEWF_WARNING_PRINT( "%s: media flags do not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_common_memcmp( internal_handle->media_values->guid, data->guid, 16 ) != 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: GUID does not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	libewf_common_free( data );

	return( read_count );
}

/* Writes a data section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_data_write( LIBEWF_SEGMENT_FILE *segment_file, uint32_t amount_of_chunks, uint32_t sectors_per_chunk, uint32_t bytes_per_sector, uint32_t amount_of_sectors, uint32_t error_granularity, uint8_t media_type, uint8_t media_flags, int8_t compression_level, uint8_t *guid, uint8_t format, EWF_DATA **cached_data_section, uint8_t no_section_append )
{
	EWF_CHAR *section_type      = (EWF_CHAR *) "data";
	static char *function       = "libewf_section_data_write";
	EWF_CRC calculated_crc      = 0;
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( guid == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid GUID.\n",
		 function );

		return( -1 );
	}
	if( cached_data_section == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid caches data section.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file->file_offset;

	/* Check if the data section was already created
	 */
	if( *cached_data_section == NULL )
	{
		*cached_data_section = (EWF_DATA *) libewf_common_alloc( EWF_DATA_SIZE );

		if( *cached_data_section == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create data.\n",
			 function );

			return( -1 );
		}
		if( libewf_common_memset( *cached_data_section, 0, EWF_DATA_SIZE ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to clear data.\n",
			 function );

			return( -1 );
		}
		if( format == LIBEWF_FORMAT_FTK )
		{
			( *cached_data_section )->media_type = 0x01;
		}
		else
		{
			( *cached_data_section )->media_type = media_type;
		}
		( *cached_data_section )->media_flags = media_flags;

		if( libewf_endian_revert_32bit(
		     amount_of_chunks,
		     ( *cached_data_section )->amount_of_chunks ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
			 function );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     sectors_per_chunk,
		     ( *cached_data_section )->sectors_per_chunk ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert sectors per chunk value.\n",
			 function );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     bytes_per_sector,
		     ( *cached_data_section )->bytes_per_sector ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert bytes per sector value.\n",
			 function );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     amount_of_sectors,
		     ( *cached_data_section )->amount_of_sectors ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
			 function );

			return( -1 );
		}
		if( ( format == LIBEWF_FORMAT_ENCASE5 )
		 || ( format == LIBEWF_FORMAT_ENCASE6 )
		 || ( format == LIBEWF_FORMAT_LINEN5 )
		 || ( format == LIBEWF_FORMAT_LINEN6 )
		 || ( format == LIBEWF_FORMAT_EWFX ) )
		{
			if( libewf_endian_revert_32bit(
			     error_granularity,
			     ( *cached_data_section )->error_granularity ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to revert error granularity value.\n",
				 function );

				return( -1 );
			}
			( *cached_data_section )->compression_level = (uint8_t) compression_level;

			if( libewf_common_memcpy( ( *cached_data_section )->guid, guid, 16 ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
				 function );

				return( -1 );
			}
		}
		calculated_crc = ewf_crc_calculate( *cached_data_section, ( EWF_DATA_SIZE - EWF_CRC_SIZE ), 1 );

		if( libewf_endian_revert_32bit(
		     calculated_crc,
		     ( *cached_data_section )->crc ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
			 function );

			return( -1 );
		}
	}
	section_write_count = libewf_section_start_write(
	                       segment_file,
	                       section_type,
	                       4,
	                       EWF_DATA_SIZE );

	if( section_write_count != (ssize_t) EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
	               segment_file,
	               *cached_data_section,
	               EWF_DATA_SIZE );

	if( write_count != (ssize_t) EWF_DATA_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write data to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( no_section_append == 0 )
	{
		if( libewf_section_list_append(
		     segment_file->section_list,
		     section_type,
		     section_offset,
		     ( section_offset + section_write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to append: %s section to section list.\n",
			 function, (char *) section_type );

			return( -1 );
		}
	}
	return( section_write_count );
}

/* Reads a error2 section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_error2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, size_t section_size, uint8_t ewf_format, uint8_t error_tollerance )
{
	EWF_ERROR2 error2;
	uint8_t stored_crc_buffer[ 4 ];

	EWF_ERROR2_SECTOR *error2_sectors = NULL;
	static char *function             = "libewf_section_error2_read";
	EWF_CRC calculated_crc            = 0;
	EWF_CRC stored_crc                = 0;
	ssize_t read_count                = 0;
	size_t sectors_size               = 0;
	uint32_t amount_of_errors         = 0;
	uint32_t iterator                 = 0;
	uint32_t sector                   = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain error2 section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              &error2,
	              EWF_ERROR2_SIZE );
	
	if( read_count != (ssize_t) EWF_ERROR2_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read error2.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &error2, ( EWF_ERROR2_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, error2.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &amount_of_errors, error2.amount_of_errors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of errors value.\n",
		 function );

		return( -1 );
	}
	sectors_size = EWF_ERROR2_SECTOR_SIZE * amount_of_errors;

	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( error2.unknown, 200 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( (uint8_t *) error2_sectors, sectors_size ); );
#endif

	if( amount_of_errors == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: error2 contains no sectors!.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	else
	{
		error2_sectors = (EWF_ERROR2_SECTOR *) libewf_common_alloc( sectors_size );

		if( error2_sectors == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocate error2 sectors.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_segment_file_read(
		              segment_file,
		              error2_sectors,
		              sectors_size );
	
		if( read_count != (ssize_t) sectors_size )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read error2 sectors.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		calculated_crc = ewf_crc_calculate( error2_sectors, sectors_size, 1 );

		read_count = libewf_segment_file_read(
		              segment_file,
		              stored_crc_buffer,
		              EWF_CRC_SIZE );

		if( read_count != (ssize_t) EWF_CRC_SIZE )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read CRC from file descriptor.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( libewf_endian_convert_32bit( &stored_crc, stored_crc_buffer ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to convert CRC value.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( stored_crc != calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
			 function, stored_crc, calculated_crc );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				libewf_common_free( error2_sectors );

				return( -1 );
			}
		}
		if( internal_handle->acquiry_error_sectors != NULL )
		{
			LIBEWF_VERBOSE_PRINT( "%s: acquiry error sectors already set in handle - removing previous one.\n",
			 function );

			libewf_common_free( internal_handle->acquiry_error_sectors );
		}
		internal_handle->acquiry_error_sectors = (LIBEWF_ERROR_SECTOR *) libewf_common_alloc( LIBEWF_ERROR_SECTOR_SIZE * amount_of_errors );

		if( internal_handle->acquiry_error_sectors == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create acquiry error sectors.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		internal_handle->acquiry_amount_of_errors = amount_of_errors;

		for( iterator = 0; iterator < amount_of_errors; iterator++ )
		{
			if( libewf_endian_convert_32bit( &sector, error2_sectors[ iterator ].sector ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to convert sector value.\n",
				 function );

				libewf_common_free( error2_sectors );

				return( -1 );
			}
			internal_handle->acquiry_error_sectors[ iterator ].sector = (uint64_t) sector;

			if( libewf_endian_convert_32bit(
			     &internal_handle->acquiry_error_sectors[ iterator ].amount_of_sectors,
			     error2_sectors[ iterator ].amount_of_sectors ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
				 function );

				libewf_common_free( error2_sectors );

				return( -1 );
			}
		}
		libewf_common_free( error2_sectors );
	}
	return( (ssize_t) section_size );
}

/* Writes a error2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_error2_write( LIBEWF_SEGMENT_FILE *segment_file, LIBEWF_ERROR_SECTOR *sectors, uint32_t amount_of_errors )
{
	EWF_ERROR2 error2;
	uint8_t calculated_crc_buffer[ 4 ];

	EWF_ERROR2_SECTOR *error2_sectors = NULL;
	EWF_CHAR *section_type            = (EWF_CHAR *) "error2";
	static char *function             = "libewf_section_error2_write";
	EWF_CRC calculated_crc            = 0;
	off64_t section_offset            = 0;
	ssize_t section_write_count       = 0;
	ssize_t write_count               = 0;
	size_t section_size               = 0;
	size_t sectors_size               = 0;
	uint32_t iterator                 = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file\n",
		 function );

		return( -1 );
	}
	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid acquiry read error sectors.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file->file_offset;
	sectors_size   = EWF_ERROR2_SECTOR_SIZE * amount_of_errors;
	section_size   = EWF_ERROR2_SIZE + sectors_size + EWF_CRC_SIZE;

	if( libewf_common_memset( &error2, 0, EWF_ERROR2_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear error2.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( amount_of_errors, error2.amount_of_errors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of errors value.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &error2, ( EWF_ERROR2_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, error2.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	error2_sectors = (EWF_ERROR2_SECTOR *) libewf_common_alloc( sectors_size );

	if( error2_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create error2 sectors.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_errors; iterator++ )
	{
		if( libewf_endian_revert_32bit(
		     (uint32_t) sectors[ iterator ].sector,
		     error2_sectors[ iterator ].sector ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert sector value.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     sectors[ iterator ].amount_of_sectors,
		     error2_sectors[ iterator ].amount_of_sectors ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
	}
	calculated_crc = ewf_crc_calculate( error2_sectors, sectors_size, 1 );

	section_write_count = libewf_section_start_write(
	                       segment_file,
	                       section_type,
	                       6,
	                       section_size );

	if( section_write_count != (ssize_t) EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		libewf_common_free( error2_sectors );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
	               segment_file,
	               &error2,
	               EWF_ERROR2_SIZE );

	if( write_count != (ssize_t) EWF_ERROR2_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write error2 to file.\n",
		 function );

		libewf_common_free( error2_sectors );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_segment_file_write(
	               segment_file,
	               error2_sectors,
	               sectors_size );

	libewf_common_free( error2_sectors );

	if( write_count != (ssize_t) sectors_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write error2 sectors to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_endian_revert_32bit( calculated_crc, calculated_crc_buffer ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
 	               segment_file,
 	               calculated_crc_buffer,
 	               EWF_CRC_SIZE );

	if( write_count != (ssize_t) EWF_CRC_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write error2 sectors CRC to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file->section_list,
	     section_type,
	     section_offset,
	     ( section_offset + section_write_count ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to append %s section to section list.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a hash section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_hash_read( LIBEWF_SEGMENT_FILE *segment_file, EWF_DIGEST_HASH *md5_hash, uint8_t error_tollerance )
{
	EWF_HASH hash;

	static char *function  = "libewf_section_hash_read";
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;
	ssize_t read_count     = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid md5 hash.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_read(
	              segment_file,
	              &hash,
	              EWF_HASH_SIZE );

	if( read_count != (ssize_t) EWF_HASH_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read hash.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &hash, ( EWF_HASH_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, hash.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash->unknown1, 16 ); );
#endif

	if( libewf_common_memcpy( md5_hash, hash.md5_hash, EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set MD5 hash in handle.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	return( read_count );
}

/* Writes a hash section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_hash_write( LIBEWF_SEGMENT_FILE *segment_file, EWF_DIGEST_HASH *md5_hash )
{
	EWF_HASH hash;

	EWF_CHAR *section_type      = (EWF_CHAR *) "hash";
	static char *function       = "libewf_section_hash_write";
	EWF_CRC calculated_crc      = 0;
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file->file_offset;

	if( libewf_common_memset( &hash, 0, EWF_HASH_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear hash.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memcpy( hash.md5_hash, md5_hash, EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &hash, ( EWF_HASH_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, hash.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	section_write_count = libewf_section_start_write(
	                       segment_file,
	                       section_type,
	                       4,
	                       EWF_HASH_SIZE );

	if( section_write_count != (ssize_t) EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
	               segment_file,
	               &hash,
	               EWF_HASH_SIZE );

	if( write_count != (ssize_t) EWF_HASH_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write hash to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file->section_list,
	     section_type,
	     section_offset,
	     ( section_offset + section_write_count ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to append %s section to section list.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Writes the last section start to file
 * This is used for the next and done sections, these sections point back towards themselves
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_last_write( LIBEWF_SEGMENT_FILE *segment_file, EWF_CHAR *section_type, size_t section_type_length, uint8_t format, uint8_t ewf_format )
{
	EWF_SECTION section;

	static char *function       = "libewf_section_last_write";
	EWF_CRC calculated_crc      = 0;
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;
	size_t section_size         = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file\n",
		 function );

		return( -1 );
	}
	if( section_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section type.\n",
		 function );

		return( -1 );
	}
	if( section_type_length == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: section type is empty.\n",
		 function );

		return( -1 );
	}
	if( section_type_length >= 16 )
	{
		LIBEWF_WARNING_PRINT( "%s: section type is too long.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( &section, 0, EWF_SECTION_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear section.\n",
		 function );

		return( -1 );
	}
	/* The EnCase (EWF-E01) format leaves the size of this section empty
	 */
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( format == LIBEWF_FORMAT_FTK ) )
	{
		section_size = EWF_SECTION_SIZE;
	}
	section_offset = segment_file->file_offset;

	/* Add one character for the end of string
	 */
	if( ewf_string_copy( section.type, section_type, ( section_type_length + 1 ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set section type.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( section_size, section.size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert size value.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( section_offset, section.next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert next offset value.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &section, ( EWF_SECTION_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, section.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	section_write_count = libewf_segment_file_write(
	                       segment_file,
	                       &section,
	                       EWF_SECTION_SIZE );

	if( section_write_count != (ssize_t) EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	if( libewf_section_list_append(
	     segment_file->section_list,
	     section_type,
	     section_offset,
	     ( section_offset + section_write_count ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to append %s section to section list.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a xheader section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_xheader_read( LIBEWF_SEGMENT_FILE *segment_file, size_t section_size, EWF_CHAR **cached_xheader, size_t *cached_xheader_size )
{
	EWF_CHAR *xheader     = NULL;
	static char *function = "libewf_section_xheader_read";
	ssize_t read_count    = 0;
	size_t xheader_size   = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( cached_xheader == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid cached xheader.\n",
		 function );

		return( -1 );
	}
	if( cached_xheader_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid cached xheader size.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              segment_file,
	              section_size,
	              &xheader,
	              &xheader_size );

	if( read_count != (ssize_t) section_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read xheader.\n",
		 function );

		return( -1 );
	}
	if( xheader == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid xheader.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xheader, xheader_size ); );

	if( *cached_xheader == NULL )
	{
		*cached_xheader      = xheader;
		*cached_xheader_size = xheader_size;
	}
	else
	{
		libewf_common_free( xheader );
	}
	return( read_count );
}

/* Writes a xheader section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_xheader_write( LIBEWF_SEGMENT_FILE *segment_file, EWF_CHAR *xheader, size_t xheader_size, int8_t compression_level )
{
	static char *function       = "libewf_section_xheader_write";
	ssize_t section_write_count = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: XHeader:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xheader, xheader_size ); );

	section_write_count = libewf_section_write_compressed_string(
	                       segment_file,
	                       (EWF_CHAR *) "xheader",
	                       7,
	                       xheader,
	                       xheader_size,
	                       compression_level );

	/* refactor */
	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write xheader to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a xhash section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_xhash_read( LIBEWF_SEGMENT_FILE *segment_file, size_t section_size, EWF_CHAR **cached_xhash, size_t *cached_xhash_size )
{
	EWF_CHAR *xhash       = NULL;
	static char *function = "libewf_section_xhash_read";
	ssize_t read_count    = 0;
	size_t xhash_size     = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( cached_xhash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid cached xhash.\n",
		 function );

		return( -1 );
	}
	if( cached_xhash_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid cached xhash size.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              segment_file,
	              section_size,
	              &xhash,
	              &xhash_size );

	if( read_count != (ssize_t) section_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read xheader.\n",
		 function );

		return( -1 );
	}
	if( xhash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid xhash.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Hash:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xhash, xhash_size ); );

	if( *cached_xhash == NULL )
	{
		*cached_xhash      = xhash;
		*cached_xhash_size = xhash_size;
	}
	else
	{
		libewf_common_free( xhash );
	}
	return( read_count );
}

/* Writes a xhash section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_xhash_write( LIBEWF_SEGMENT_FILE *segment_file, EWF_CHAR *xhash, size_t xhash_size, int8_t compression_level )
{
	static char *function       = "libewf_section_xhash_write";
	ssize_t section_write_count = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: XHash:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xhash, xhash_size ); );

	section_write_count = libewf_section_write_compressed_string(
	                       segment_file,
	                       (EWF_CHAR *) "xhash",
	                       5,
	                       xhash,
	                       xhash_size,
	                       compression_level );

	/* refactor */
	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write xhash to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a delta chunk section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_delta_chunk_read( LIBEWF_SEGMENT_FILE *segment_file, off64_t section_offset, size_t section_size, LIBEWF_OFFSET_TABLE *offset_table, uint8_t error_tollerance )
{
	EWFX_DELTA_CHUNK_HEADER delta_chunk_header;

	static char *function  = "libewf_section_delta_chunk_read";
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;
	uint32_t chunk         = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( libewf_segment_file_read(
	     segment_file,
	     &delta_chunk_header,
	     EWFX_DELTA_CHUNK_HEADER_SIZE ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read delta chunk header.\n",
		 function );

		return( -1 );
	}
	/* The chunk value is stored + 1 count in the file
	 */
	if( libewf_endian_convert_32bit( &chunk, delta_chunk_header.chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert chunk value.\n",
		 function );

		return( -1 );
	}
	chunk -= 1;

	calculated_crc = ewf_crc_calculate( &delta_chunk_header, ( EWFX_DELTA_CHUNK_HEADER_SIZE - EWF_CRC_SIZE ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, delta_chunk_header.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	/* Skip the chunk data within the section
	 */
	if( libewf_segment_file_seek_offset(
	     segment_file,
	     ( section_offset + section_size ) ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
		 function );

		return( -1 );
	}
	if( chunk >= offset_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid delta chunk: %" PRIu32 " value outside offset table.\n",
		 function, chunk );

		return( -1 );
	}
	/* Update the chunk data in the offset table
	 */
	offset_table->chunk_offset[ chunk ].segment_file = segment_file;
	offset_table->chunk_offset[ chunk ].file_offset  = section_offset + EWFX_DELTA_CHUNK_HEADER_SIZE;
	offset_table->chunk_offset[ chunk ].size         = section_size - EWFX_DELTA_CHUNK_HEADER_SIZE;
	offset_table->chunk_offset[ chunk ].compressed   = 0;
	offset_table->chunk_offset[ chunk ].dirty        = 1;

	return( (ssize_t) section_size );
}

/* Writes a delta chunk section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_delta_chunk_write( LIBEWF_SEGMENT_FILE *segment_file, uint32_t chunk, EWF_CHAR *chunk_data, size_t chunk_size, EWF_CRC *chunk_crc, uint8_t write_crc, uint8_t no_section_append )
{
	EWFX_DELTA_CHUNK_HEADER delta_chunk_header;
	uint8_t calculated_crc_buffer[ 4 ];

	EWF_CHAR *section_type      = (EWF_CHAR *) "delta_chunk";
	static char *function       = "libewf_section_delta_chunk_write";
	EWF_CRC calculated_crc      = 0;
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;
	size_t section_size         = 0;

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( chunk_size > (size_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file->file_offset;

	if( libewf_common_memset( &delta_chunk_header, 0, EWFX_DELTA_CHUNK_HEADER_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear delta chunk header.\n",
		 function );

		return( -1 );
	}
	/* The chunk value is stored + 1 count in the file
	 */
	if( libewf_endian_revert_32bit( ( chunk + 1 ), delta_chunk_header.chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert chunk value.\n" );

		return( -1 );
	}
	delta_chunk_header.padding[ 0 ] = (uint8_t) 'D';
	delta_chunk_header.padding[ 1 ] = (uint8_t) 'E';
	delta_chunk_header.padding[ 2 ] = (uint8_t) 'L';
	delta_chunk_header.padding[ 3 ] = (uint8_t) 'T';
	delta_chunk_header.padding[ 4 ] = (uint8_t) 'A';

	calculated_crc = ewf_crc_calculate(
	                  &delta_chunk_header,
	                  ( EWFX_DELTA_CHUNK_HEADER_SIZE - EWF_CRC_SIZE ),
	                  1 );

	if( libewf_endian_revert_32bit( calculated_crc, delta_chunk_header.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	section_size = EWFX_DELTA_CHUNK_HEADER_SIZE + chunk_size;

	if( write_crc != 0 )
	{
		section_size += EWF_CRC_SIZE;
	}
	section_write_count = libewf_section_start_write(
	                       segment_file,
	                       section_type,
	                       11,
	                       section_size );

	if( section_write_count != (ssize_t) EWF_SECTION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function );

		return( -1 );
	}
	write_count = libewf_segment_file_write(
	               segment_file,
	               &delta_chunk_header,
	               EWFX_DELTA_CHUNK_HEADER_SIZE );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write chunk value to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_segment_file_write(
	               segment_file,
	               chunk_data,
	               chunk_size );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write chunk data to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( write_crc != 0 )
	{
		if( libewf_endian_revert_32bit( *chunk_crc, calculated_crc_buffer ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
			 function );

			return( -1 );
		}
		write_count = libewf_segment_file_write(
			       segment_file,
			       calculated_crc_buffer,
			       EWF_CRC_SIZE );

		if( write_count != (size_t) EWF_CRC_SIZE )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write CRC to file.\n",
			 function );

			return( -1 );
		}
		section_write_count += write_count;
	}
	if( no_section_append == 0 )
	{
		if( libewf_section_list_append(
		     segment_file->section_list,
		     section_type,
		     section_offset,
		     ( section_offset + section_write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to append: %s section to section list.\n",
			 function, (char *) section_type );

			return( -1 );
		}
	}
	return( section_write_count );
}

/* Reads and processes a section
 * The section start offset will be updated
 * Returns 1 if successful, -1 on error
 */
int libewf_section_read( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SEGMENT_FILE *segment_file, EWF_SECTION *section, off64_t *section_start_offset )
{
	static char *function      = "libewf_section_read";
	off64_t section_end_offset = 0;
	ssize64_t read_count       = 0;
	uint64_t size              = 0;
	uint64_t next_offset       = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section start.\n",
		 function );

		return( -1 );
	}
	if( section_start_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section start offset.\n",
		 function );

		return( -1 );
	}
	if( *section_start_offset > (off64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section start offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_section_start_read(
	     segment_file,
	     section,
	     internal_handle->error_tollerance ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read section start.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_convert_64bit( &size, section->size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert size value.\n",
		 function );

		return( -1 );
	}
	if( size > (uint64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_convert_64bit( &next_offset, section->next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert next offset value.\n",
		 function );

		return( -1 );
	}
	if( next_offset > (uint64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid next offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	section_end_offset = *section_start_offset + (off64_t) size;

	if( section_end_offset > (off64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section end offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_section_list_append(
	     segment_file->section_list,
	     section->type,
	     *section_start_offset,
	     section_end_offset ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to append value to section list.\n",
		 function );

		return( -1 );
	}
	*section_start_offset += EWF_SECTION_SIZE;

	/* No need to correct empty sections like done and next
	 */
	if( size > 0 )
	{
		size -= EWF_SECTION_SIZE;
	}
	if( size > (uint64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}

	/* Nothing to do for the next and done section
	 * The \0 byte is included in the compare
	 */
	if( ( ewf_string_compare( section->type, "next", 5 ) == 0 )
	 || ( ewf_string_compare( section->type, "done", 5 ) == 0 ) )
	{
		/* Determine the size of the largest segment file for read and write mode only
		 */
		if( ( internal_handle->write != NULL )
		 && ( (off64_t) internal_handle->write->segment_file_size < ( *section_start_offset + (off64_t) EWF_SECTION_SIZE ) ) )
		{
			internal_handle->write->segment_file_size = (size64_t) ( *section_start_offset + EWF_SECTION_SIZE );
		}
	}
	/* Read the header2 section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "header2", 8 ) == 0 )
	{
		read_count = libewf_section_header2_read(
		              segment_file,
		              (size_t) size,
		              &( internal_handle->header2 ),
		              &( internal_handle->header2_size ) );

		internal_handle->amount_of_header_sections++;
	}
	/* Read the header section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "header", 7 ) == 0 )
	{
		read_count = libewf_section_header_read(
		              segment_file,
		              (size_t) size,
		              &( internal_handle->header ),
		              &( internal_handle->header_size ) );

		internal_handle->amount_of_header_sections++;
	}
	/* Read the xheader section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "xheader", 8 ) == 0 )
	{
		read_count = libewf_section_xheader_read(
		              segment_file,
		              (size_t) size,
		              &( internal_handle->xheader ),
		              &( internal_handle->xheader_size ) );

		internal_handle->amount_of_header_sections++;
	}
	/* Read the volume or disk section
	 * The \0 byte is included in the compare
	 */
	else if( ( ewf_string_compare( section->type, "volume", 7 ) == 0 )
	 || ( ewf_string_compare( section->type, "disk", 5 ) == 0 ) )
	{
		read_count = libewf_section_volume_read(
		              segment_file,
		              (size_t) size,
		              &( internal_handle->media_values->amount_of_chunks ),
		              &( internal_handle->media_values->sectors_per_chunk ),
		              &( internal_handle->media_values->bytes_per_sector ),
		              &( internal_handle->media_values->amount_of_sectors ),
		              &( internal_handle->media_values->chunk_size ),
		              &( internal_handle->media_values->error_granularity ),
		              &( internal_handle->media_values->media_type ),
		              &( internal_handle->media_values->media_flags ),
		              &( internal_handle->compression_level ),
		              internal_handle->media_values->guid,
		              &( internal_handle->format ),
		              &( internal_handle->ewf_format ),
		              internal_handle->error_tollerance );
	}
	/* Read the table2 section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "table2", 7 ) == 0 )
	{
		read_count = libewf_section_table2_read(
		              internal_handle,
		              segment_file,
		              *section_start_offset,
		              (size_t) size );
	}
	/* Read the table section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "table", 6 ) == 0 )
	{
		read_count = libewf_section_table_read(
		              internal_handle,
		              segment_file,
		              *section_start_offset,
		              (size_t) size );
	}
	/* Read the sectors section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "sectors", 8 ) == 0 )
	{
		read_count = libewf_section_sectors_read(
		              segment_file,
 		              *section_start_offset,
 		              (size_t) size,
 		              internal_handle->ewf_format,
 		              internal_handle->error_tollerance );
	}
	/* Read the delta_chunk section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "delta_chunk", 12 ) == 0 )
	{
		read_count = libewf_section_delta_chunk_read(
 		              segment_file,
 		              *section_start_offset,
 		              (size_t) size,
 		              internal_handle->offset_table,
 		              internal_handle->error_tollerance );
	}
	/* Read the ltree section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "ltree", 6 ) == 0 )
	{
		read_count = libewf_section_ltree_read(
		              segment_file,
		              (size_t) size,
		              &( internal_handle->ewf_format ),
 		              internal_handle->error_tollerance );
	}
	/* Read the session section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "session", 8 ) == 0 )
	{
		read_count = libewf_section_session_read(
		              segment_file,
		              (size_t) size,
		              internal_handle->ewf_format,
 		              internal_handle->error_tollerance );
	}
	/* Read the data section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "data", 5 ) == 0 )
	{
		read_count = libewf_section_data_read(
		              internal_handle,
		              segment_file,
		              (size_t) size,
		              internal_handle->ewf_format,
 		              internal_handle->error_tollerance );
	}
	/* Read the hash section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "hash", 5 ) == 0 )
	{
		read_count = libewf_section_hash_read(
		              segment_file,
		              internal_handle->md5_hash,
 		              internal_handle->error_tollerance );

		internal_handle->md5_hash_set = 1;

	}
	/* Read the xhash section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "xhash", 6 ) == 0 )
	{
		read_count = libewf_section_xhash_read(
		              segment_file,
		              (size_t) size,
		              &( internal_handle->xhash ),
		              &( internal_handle->xhash_size ) );
	}
	/* Read the error2 section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "error2", 7 ) == 0 )
	{
		read_count = libewf_section_error2_read(
		              internal_handle,
		              segment_file,
		              (size_t) size,
		              internal_handle->ewf_format,
 		              internal_handle->error_tollerance );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported section type: %s.\n",
		 function, (char *) section->type );

#if defined( HAVE_DEBUG_OUTPUT )
		if( size > SSIZE_MAX )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
		LIBEWF_VERBOSE_EXEC( libewf_debug_read_section(
		                      internal_handle,
		                      segment_file->file_descriptor,
		                      (size_t) size ); );
#else
		/* Skip the data within the section
		 */
		if( libewf_common_lseek(
		     segment_file->file_descriptor,
		     size,
		     SEEK_CUR ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
#endif
		read_count = (ssize64_t) size;
	}
	if( read_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read section: %s.\n",
		 function, (char *) section->type );

		return( -1 );
	}
	*section_start_offset += (off64_t) read_count;

	if( read_count != (ssize64_t) size )
	{
		LIBEWF_WARNING_PRINT( "%s: section: %s was not entirely read.\n",
		 function, (char *) section->type );

		return( -1 );
	}
	return( 1 );
}

