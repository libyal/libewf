/*
 * libewf section
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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
ssize_t libewf_section_start_read(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_section_t *section,
         uint8_t error_tollerance )
{
	static char *function    = "libewf_section_start_read";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	ssize_t read_count       = 0;

	if( segment_file_handle == NULL )
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
	read_count = libewf_segment_file_handle_read(
	              segment_file_handle,
	              section,
	              sizeof( ewf_section_t ) );

	if( read_count != sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read section start.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( section, ( sizeof( ewf_section_t ) - sizeof( ewf_crc_t ) ), 1 );

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
ssize_t libewf_section_start_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *section_type,
         size_t section_type_length,
         size64_t section_data_size )
{
	ewf_section_t section;

	static char *function    = "libewf_section_start_write";
	ewf_crc_t calculated_crc = 0;
	ssize_t write_count      = 0;
	uint64_t section_size    = 0;
	uint64_t section_offset  = 0;

	if( segment_file_handle == NULL )
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
	if( libewf_common_memset(
	     &section,
	     0,
	     sizeof( ewf_section_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear section.\n",
		 function );

		return( -1 );
	}
	if( ewf_string_copy(
	     section.type,
	     section_type,
	     section_type_length ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set section type.\n",
		 function );

		return( -1 );
	}
	section_size   = sizeof( ewf_section_t ) + section_data_size;
	section_offset = segment_file_handle->file_offset + section_size;

	if( section_size > (uint64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_64bit(
	     section_size,
	     section.size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert size value.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_64bit(
	     section_offset,
	     section.next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert next offset value.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  &section,
	                  ( sizeof( ewf_section_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	if( libewf_endian_revert_32bit(
	     calculated_crc,
	     section.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               &section,
	               sizeof( ewf_section_t ) );

	if( write_count != sizeof( ewf_section_t ) )
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
ssize_t libewf_section_compressed_string_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t compressed_string_size,
         ewf_char_t **uncompressed_string,
         size_t *uncompressed_string_size )
{
	ewf_char_t *compressed_string = NULL;
	ewf_char_t *reallocation      = NULL;
	ewf_char_t *uncompressed      = NULL;
	static char *function         = "libewf_section_compressed_string_read";
	ssize_t read_count            = 0;
	int result                    = 0;

	if( segment_file_handle == NULL )
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
	compressed_string = (ewf_char_t *) libewf_common_alloc(
	                                    sizeof( ewf_char_t ) * compressed_string_size );

	if( compressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create compressed string.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_handle_read(
	              segment_file_handle,
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
	uncompressed = (ewf_char_t *) libewf_common_alloc(
	                               sizeof( ewf_char_t ) * ( *uncompressed_string_size + 1 ) );

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
		reallocation = (ewf_char_t *) libewf_common_realloc(
		                               uncompressed,
		                               ( sizeof( ewf_char_t ) * ( *uncompressed_string_size + 1 ) ) );

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
	uncompressed[ *uncompressed_string_size ] = (ewf_char_t) '\0';

	*uncompressed_string       = uncompressed;
	*uncompressed_string_size += 1;

	return( read_count );
}

/* Writes a compressed string section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_write_compressed_string(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *section_type,
         size_t section_type_length,
         ewf_char_t *uncompressed_string,
         size_t uncompressed_string_size,
         int8_t compression_level )
{
	ewf_char_t *compressed_string = NULL;
	ewf_char_t *reallocation      = NULL;
	static char *function         = "libewf_section_write_compressed_string";
	off64_t section_offset        = 0;
	size_t compressed_string_size = 0;
	ssize_t section_write_count   = 0;
	ssize_t write_count           = 0;
	int result                    = 0;

	if( segment_file_handle == NULL )
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
	section_offset         = segment_file_handle->file_offset;
	compressed_string_size = uncompressed_string_size;
	compressed_string      = (ewf_char_t *) libewf_common_alloc(
	                                         sizeof( ewf_char_t ) * compressed_string_size );

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
		reallocation = (ewf_char_t *) libewf_common_realloc(
		                               compressed_string,
		                               ( sizeof( ewf_char_t ) * compressed_string_size ) );

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
	                       segment_file_handle,
	                       section_type,
	                       section_type_length,
	                       (size64_t) compressed_string_size );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( compressed_string );

		return( -1 );
	}
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               compressed_string,
	               compressed_string_size );

	libewf_common_free( compressed_string );

	if( write_count != (ssize_t) compressed_string_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write string to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section_type,
	     section_type_length,
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
ssize_t libewf_section_header_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         ewf_char_t **cached_header,
         size_t *cached_header_size )
{
	ewf_char_t *header    = NULL;
	static char *function = "libewf_section_header_read";
	ssize_t read_count    = 0;
	size_t header_size    = 0;

	if( segment_file_handle == NULL )
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
	              segment_file_handle,
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
ssize_t libewf_section_header_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *header,
         size_t header_size,
         int8_t compression_level )
{
	static char *function       = "libewf_section_header_write";
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, header, header_size ); );

	section_write_count = libewf_section_write_compressed_string(
	                       segment_file_handle,
	                       (ewf_char_t *) "header",
	                       6,
	                       header,
	                       header_size,
	                       compression_level );

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
ssize_t libewf_section_header2_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         ewf_char_t **cached_header2,
         size_t *cached_header2_size )
{
	ewf_char_t *header2   = NULL;
	static char *function = "libewf_section_header2_read";
	ssize_t read_count    = 0;
	size_t header2_size   = 0;

	if( segment_file_handle == NULL )
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
	              segment_file_handle,
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
ssize_t libewf_section_header2_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *header2,
         size_t header2_size,
         int8_t compression_level )
{
	static char *function       = "libewf_section_header2_write";
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header2:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header2_fprint( stderr, header2, header2_size ); );

	section_write_count = libewf_section_write_compressed_string(
	                       segment_file_handle,
	                       (ewf_char_t *) "header2",
	                       7,
	                       header2,
	                       header2_size,
	                       compression_level );

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
ssize_t libewf_section_volume_s01_read(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         uint8_t *format,
         uint8_t error_tollerance )
{
	ewf_volume_smart_t *volume = NULL;
	static char *function      = "libewf_section_volume_s01_read";
	ewf_crc_t calculated_crc   = 0;
	ewf_crc_t stored_crc       = 0;
	ssize_t read_count         = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid format.\n",
		 function );

		return( -1 );
	}
	volume = (ewf_volume_smart_t *) libewf_common_alloc(
	                                 sizeof( ewf_volume_smart_t ) );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate volume.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_handle_read(
	              segment_file_handle,
	              volume,
	              sizeof( ewf_volume_smart_t ) );

	if( read_count != (ssize_t) sizeof( ewf_volume_smart_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( volume, ( sizeof( ewf_volume_smart_t ) - sizeof( ewf_crc_t ) ), 1 );

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

	if( libewf_endian_convert_32bit(
	     &( media_values->amount_of_chunks ),
	     volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit(
	     &( media_values->sectors_per_chunk ),
	     volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit(
	     &( media_values->bytes_per_sector ),
	     volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit(
	     &( media_values->amount_of_sectors ),
	     volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_common_memcmp(
	     (void *) volume->signature,
	     (void *) "SMART",
	     5 ) == 0 )
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
ssize_t libewf_section_volume_s01_write(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         uint8_t format,
         uint8_t no_section_append )
{
	ewf_char_t *section_type    = (ewf_char_t *) "volume";
	ewf_volume_smart_t *volume  = NULL;
	static char *function       = "libewf_section_volume_s01_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	size_t section_type_length  = 6;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file_handle->file_offset;

	volume = (ewf_volume_smart_t *) libewf_common_alloc(
	                                 sizeof( ewf_volume_smart_t ) );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create volume.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( volume, 0, sizeof( ewf_volume_smart_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear volume.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	volume->unknown1[ 0 ] = 1;

	if( libewf_endian_revert_32bit(
	     media_values->amount_of_chunks,
	     volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit(
	     media_values->sectors_per_chunk,
	     volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit(
	     media_values->bytes_per_sector,
	     volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit(
	     media_values->amount_of_sectors,
	     volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_SMART )
	{
		volume->signature[ 0 ] = (uint8_t) 'S';
		volume->signature[ 1 ] = (uint8_t) 'M';
		volume->signature[ 2 ] = (uint8_t) 'A';
		volume->signature[ 3 ] = (uint8_t) 'R';
		volume->signature[ 4 ] = (uint8_t) 'T';
	}
	calculated_crc = ewf_crc_calculate( volume, ( sizeof( ewf_volume_smart_t ) - sizeof( ewf_crc_t ) ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, volume->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
	 function, media_values->amount_of_chunks, media_values->chunk_size, media_values->sectors_per_chunk );

	LIBEWF_VERBOSE_PRINT( "%s: volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n",
	 function, media_values->amount_of_sectors, media_values->bytes_per_sector );

	section_write_count = libewf_section_start_write(
	                       segment_file_handle,
	                       section_type,
	                       section_type_length,
	                       (size64_t) sizeof( ewf_volume_smart_t ) );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               volume,
	               sizeof( ewf_volume_smart_t ) );

	libewf_common_free( volume );

	if( write_count != (ssize_t) sizeof( ewf_volume_smart_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write volume to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( no_section_append == 0 )
	{
		if( libewf_section_list_append(
		     segment_file_handle->section_list,
		     section_type,
		     section_type_length,
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
ssize_t libewf_section_volume_e01_read(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         int8_t *compression_level,
         uint8_t error_tollerance )
{
	ewf_volume_t *volume     = NULL;
	static char *function    = "libewf_section_volume_e01_read";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	ssize_t read_count       = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( compression_level == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compression level.\n",
		 function );

		return( -1 );
	}
	volume = (ewf_volume_t *) libewf_common_alloc(
	                           sizeof( ewf_volume_t ) );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_handle_read(
	              segment_file_handle,
	              volume,
	              sizeof( ewf_volume_t ) );

	if( read_count != (ssize_t) sizeof( ewf_volume_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( volume, ( sizeof( ewf_volume_t ) - sizeof( ewf_crc_t ) ), 1 );

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

	if( libewf_endian_convert_32bit(
	     &( media_values->amount_of_chunks ),
	     volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit(
	     &( media_values->sectors_per_chunk ),
	     volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit(
	     &( media_values->bytes_per_sector ),
	     volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit(
	     &( media_values->amount_of_sectors ),
	     volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit(
	     &( media_values->error_granularity ),
	     volume->error_granularity ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert error granularity value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	media_values->media_type  = volume->media_type;
	media_values->media_flags = volume->media_flags;
	*compression_level        = (int8_t) volume->compression_level;

	if( libewf_common_memcpy(
	     media_values->guid,
	     volume->guid,
	     16 ) == NULL )
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
ssize_t libewf_section_volume_e01_write(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t format,
         uint8_t no_section_append )
{
	ewf_char_t *section_type    = (ewf_char_t *) "volume";
	ewf_volume_t *volume        = NULL;
	static char *function       = "libewf_section_volume_e01_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	size_t section_type_length  = 6;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file_handle->file_offset;

	volume = (ewf_volume_t *) libewf_common_alloc(
	                           sizeof( ewf_volume_t ) );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create volume.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( volume, 0, sizeof( ewf_volume_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear volume.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_FTK )
	{
		volume->media_type = 0x01;
	}
	else
	{
		volume->media_type = media_values->media_type;
	}
	volume->media_flags = media_values->media_flags;

	if( libewf_endian_revert_32bit(
	     media_values->amount_of_chunks,
	     volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit(
	     media_values->sectors_per_chunk,
	     volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit(
	     media_values->bytes_per_sector,
	     volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit(
	     media_values->amount_of_sectors,
	     volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( ( format == LIBEWF_FORMAT_ENCASE5 )
	 || ( format == LIBEWF_FORMAT_ENCASE6 )
	 || ( format == LIBEWF_FORMAT_LINEN5 )
	 || ( format == LIBEWF_FORMAT_LINEN6 )
	 || ( format == LIBEWF_FORMAT_EWFX ) )
	{
		volume->compression_level = (uint8_t) compression_level;

		if( libewf_common_memcpy(
		     volume->guid,
		     media_values->guid,
		     16 ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
			 function );

			libewf_common_free( volume );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     media_values->error_granularity,
		     volume->error_granularity ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert error granularity value.\n",
			 function );

			libewf_common_free( volume );

			return( -1 );
		}
	}
	calculated_crc = ewf_crc_calculate( volume, ( sizeof( ewf_volume_t ) - sizeof( ewf_crc_t ) ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, volume->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
	 function, media_values->amount_of_chunks, media_values->chunk_size, media_values->sectors_per_chunk );

	LIBEWF_VERBOSE_PRINT( "%s: volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n",
	 function, media_values->amount_of_sectors, media_values->bytes_per_sector );

	section_write_count = libewf_section_start_write(
	                       segment_file_handle,
	                       section_type,
	                       section_type_length,
	                       (size64_t) sizeof( ewf_volume_t ) );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               volume,
	               sizeof( ewf_volume_t ) );

	libewf_common_free( volume );

	if( write_count != (ssize_t) sizeof( ewf_volume_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write volume to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( no_section_append == 0 )
	{
		if( libewf_section_list_append(
		     segment_file_handle->section_list,
		     section_type,
		     section_type_length,
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
ssize_t libewf_section_volume_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         libewf_media_values_t *media_values,
         int8_t *compression_level,
         uint8_t *format,
         uint8_t *ewf_format,
         uint8_t error_tollerance )
{
	static char *function    = "libewf_section_volume_read";
	ssize_t read_count       = 0;
	size64_t bytes_per_chunk = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media values.\n",
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
	if( section_size == sizeof( ewf_volume_smart_t ) )
	{
		*ewf_format = EWF_FORMAT_S01;
		read_count  = libewf_section_volume_s01_read(
		               segment_file_handle,
		               media_values,
		               format,
		               error_tollerance );
	}
	else if( section_size == sizeof( ewf_volume_t ) )
	{
		*ewf_format = EWF_FORMAT_E01;
		read_count  = libewf_section_volume_e01_read(
		               segment_file_handle,
		               media_values,
		               compression_level,
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
	if( media_values->sectors_per_chunk > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( media_values->bytes_per_sector > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	bytes_per_chunk = (size64_t) media_values->sectors_per_chunk
	                * (size64_t) media_values->bytes_per_sector;

	if( bytes_per_chunk > (size64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk size value exceeds maximum.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		media_values->chunk_size = EWF_MINIMUM_CHUNK_SIZE;
	}
	else
	{
		media_values->chunk_size = (uint32_t) bytes_per_chunk;
	}
	LIBEWF_VERBOSE_PRINT( "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
	 function, media_values->amount_of_chunks, media_values->chunk_size, media_values->sectors_per_chunk );

	LIBEWF_VERBOSE_PRINT( "%s: volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n",
	 function, media_values->amount_of_sectors, media_values->bytes_per_sector );

	if( media_values->amount_of_chunks == 0 )
	{
		*ewf_format = EWF_FORMAT_L01;
	}
	return( read_count );
}

/* Reads a table section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_table_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint32_t media_amount_of_chunks,
         libewf_offset_table_t *offset_table,
         uint8_t format,
         uint8_t ewf_format,
         uint8_t error_tollerance )
{
	ewf_table_t table;
	uint8_t stored_crc_buffer[ 4 ];

	ewf_table_offset_t *offsets = NULL;
	static char *function       = "libewf_section_table_read";
	ewf_crc_t calculated_crc    = 0;
	ewf_crc_t stored_crc        = 0;
	size_t offsets_size         = 0;
	ssize_t section_read_count  = 0;
	ssize_t read_count          = 0;
	uint64_t base_offset        = 0;
	uint32_t amount_of_chunks   = 0;

	if( segment_file_handle == NULL )
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
	/* Allocate the necessary amount of chunk offsets
	 * this reduces the amount of reallocations
	 */
	if( offset_table->amount < media_amount_of_chunks )
	{
		if( libewf_offset_table_realloc(
		     offset_table,
		     media_amount_of_chunks ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate offset table.\n",
			 function );

			return( -1 );
		}
	}
	section_read_count = libewf_segment_file_handle_read(
	                      segment_file_handle,
	                      &table,
	                      sizeof( ewf_table_t ) );
	
	if( section_read_count != (ssize_t) sizeof( ewf_table_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read table.\n",
		 function );

		return( -1 );
	}
	/* The table size contains the size of the CRC (4 bytes)
	 */
	calculated_crc = ewf_crc_calculate( &table, ( sizeof( ewf_table_t ) - sizeof( ewf_crc_t ) ), 1 );

	if( libewf_endian_convert_32bit( &stored_crc, table.crc ) != 1 )
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
	if( libewf_endian_convert_32bit( &amount_of_chunks, table.amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_convert_64bit( &base_offset, table.base_offset ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert base offset value.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( table.padding1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( table.padding2, 4 ); );
#endif

	LIBEWF_VERBOSE_PRINT( "%s: table is of size %" PRIu32 " chunks CRC %" PRIu32 " (%" PRIu32 ").\n",
	 function, amount_of_chunks, stored_crc, calculated_crc );

	if( amount_of_chunks == 0 )
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
		if( amount_of_chunks > EWF_MAXIMUM_OFFSETS_IN_TABLE )
		{
			LIBEWF_WARNING_PRINT( "%s: table contains more than %d offsets!.\n",
			 function, EWF_MAXIMUM_OFFSETS_IN_TABLE );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
		offsets_size = sizeof( ewf_table_offset_t ) * amount_of_chunks;
		offsets      = (ewf_table_offset_t *) libewf_common_alloc( offsets_size );

		if( offsets == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocated table offsets.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_segment_file_handle_read(
		              segment_file_handle,
		              offsets,
		              offsets_size );
	
		if( read_count != (ssize_t) offsets_size )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read table offsets.\n",
			 function );

			libewf_common_free( offsets );

			return( -1 );
		}
		section_read_count += read_count;

		/* The EWF-S01 format does not contain a CRC after the offsets
		 */
		if( ewf_format != EWF_FORMAT_S01 )
		{
			/* Check if the offset table CRC matches
			 */
			calculated_crc = ewf_crc_calculate( offsets, offsets_size, 1 );

			read_count = libewf_segment_file_handle_read(
			              segment_file_handle,
			              stored_crc_buffer,
			              sizeof( ewf_crc_t ) );

			if( read_count != (ssize_t) sizeof( ewf_crc_t ) )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to read CRC from file descriptor.\n",
				 function );

				libewf_common_free( offsets );

				return( -1 );
			}
			section_read_count += read_count;

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
		     amount_of_chunks,
		     segment_file_handle,
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
		     segment_file_handle->section_list,
		     error_tollerance ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to calculate last offset.\n",
			 function );

			return( -1 );
		}
	}
	if( section_size < (size_t) section_read_count )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value too small.\n",
		 function );
	}
	/* Skip the chunk data within the section
	 * for chunks after the table section
	 */
	else if( section_size != (size_t) section_read_count )
	{
		if( ( ewf_format != EWF_FORMAT_S01 )
		 && ( format != LIBEWF_FORMAT_ENCASE1 ) )
		{
			LIBEWF_VERBOSE_PRINT( "%s: unexpected data found after table offsets.\n",
			 function );
		}
		if( libewf_segment_file_handle_seek_offset(
		     segment_file_handle,
		     ( segment_file_handle->file_offset + section_size - section_read_count ) ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
		section_read_count = (ssize_t) section_size;
	}
	segment_file_handle->amount_of_chunks += amount_of_chunks;

	return( section_read_count );
}

/* Writes a table or table2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_table_write(
         libewf_segment_file_handle_t *segment_file_handle,
         off64_t base_offset,
         libewf_offset_table_t *offset_table,
         uint32_t offset_table_index,
         uint32_t amount_of_offsets,
         ewf_char_t *section_type,
         size_t section_type_length,
         size_t additional_size,
         uint8_t format,
         uint8_t ewf_format,
         uint8_t no_section_append )
{
	ewf_table_t table;
	uint8_t calculated_crc_buffer[ 4 ];

	ewf_table_offset_t *offsets = NULL;
	static char *function       = "libewf_section_table_write";
	ewf_crc_t calculated_crc    = 0;
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

	if( segment_file_handle == NULL )
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
	section_offset = segment_file_handle->file_offset;
	offsets_size   = sizeof( ewf_table_offset_t ) * amount_of_offsets;
	section_size   = sizeof( ewf_table_t ) + offsets_size + additional_size;

	if( ewf_format != EWF_FORMAT_S01 )
	{
		write_crc     = 1;
		section_size += sizeof( ewf_crc_t );
	}
	if( libewf_common_memset( &table, 0, sizeof( ewf_table_t ) ) == NULL )
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
	calculated_crc = ewf_crc_calculate( &table, ( sizeof( ewf_table_t ) - sizeof( ewf_crc_t ) ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, table.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	offsets = (ewf_table_offset_t *) libewf_common_alloc( offsets_size );

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
	                       segment_file_handle,
	                       section_type,
	                       section_type_length,
	                       (size64_t) section_size );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		libewf_common_free( offsets );

		return( -1 );
	}
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               &table,
	               sizeof( ewf_table_t ) );

	if( write_count != (ssize_t) sizeof( ewf_table_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write table to file.\n",
		 function );

		libewf_common_free( offsets );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
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
		write_count = libewf_segment_file_handle_write(
		               segment_file_handle,
	        	       calculated_crc_buffer,
	        	       sizeof( ewf_crc_t ) );

		if( write_count != (ssize_t) sizeof( ewf_crc_t ) )
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
		     segment_file_handle->section_list,
		     section_type,
		     section_type_length,
		     section_offset,
		     ( section_offset + section_size ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to append: %s section to section list.\n",
			 function, (char *) section_type );

			return( -1 );
		}
	}
	return( section_write_count );
}

/* Reads a sectors section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize64_t libewf_section_sectors_read(
           libewf_segment_file_handle_t *segment_file_handle,
           size64_t section_size,
           uint8_t ewf_format,
           uint8_t error_tollerance )
{
	static char *function = "libewf_section_sectors_read";

	if( segment_file_handle == NULL )
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
	if( libewf_segment_file_handle_seek_offset(
	     segment_file_handle,
	     ( segment_file_handle->file_offset + section_size ) ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
		 function );

		return( -1 );
	}
	return( (ssize64_t) section_size );
}

/* Writes a sectors section to file
 * Does not write the actual data in the sectors section
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_sectors_write(
         libewf_segment_file_handle_t *segment_file_handle,
         size64_t sectors_data_size,
         uint8_t no_section_append )
{
	ewf_char_t *section_type    = (ewf_char_t *) "sectors";
	static char *function       = "libewf_section_sectors_write";
	off64_t section_offset      = 0;
	size_t section_type_length  = 7;
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file_handle->file_offset;

	section_write_count = libewf_section_start_write(
	                       segment_file_handle,
	                       section_type,
	                       section_type_length,
	                       sectors_data_size );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	if( no_section_append == 0 )
	{
		if( libewf_section_list_append(
		     segment_file_handle->section_list,
		     section_type,
		     section_type_length,
		     section_offset,
		     ( section_offset + sectors_data_size ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to append: %s section to section list.\n",
			 function, (char *) section_type );

			return( -1 );
		}
	}
	return( section_write_count );
}

/* Reads a ltree section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_ltree_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint8_t *ewf_format,
         uint8_t error_tollerance )
{
	ewf_ltree_t *ltree         = NULL;
	ewf_char_t *ltree_data     = NULL;
	static char *function      = "libewf_section_ltree_read";
	ssize_t section_read_count = 0;
	ssize_t read_count         = 0;
	size_t ltree_data_size     = 0;

	if( segment_file_handle == NULL )
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

	ltree = (ewf_ltree_t *) libewf_common_alloc(
	                         sizeof( ewf_ltree_t ) );

	if( ltree == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate ltree.\n",
		 function );

		return( -1 );
	}
	section_read_count = libewf_segment_file_handle_read(
	                      segment_file_handle,
	                      ltree,
	                      sizeof( ewf_ltree_t ) );
	
	if( section_read_count != (ssize_t) sizeof( ewf_ltree_t ) )
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

	ltree_data_size = section_size - sizeof( ewf_ltree_t );

	ltree_data = (ewf_char_t *) libewf_common_alloc(
                                     sizeof( ewf_char_t ) * ltree_data_size );

	if( ltree_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate ltree data.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_handle_read(
	              segment_file_handle,
	              ltree_data,
	              ltree_data_size );

	if( read_count != (ssize_t) ltree_data_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read ltree data.\n",
		 function );

		libewf_common_free( ltree_data );

		return( -1 );
	}
	section_read_count += read_count;

	LIBEWF_VERBOSE_EXEC( libewf_debug_header2_fprint( stderr, ltree_data, ltree_data_size ); );

	libewf_common_free( ltree_data );

	return( read_count );
}

/* Reads a session section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_session_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint8_t ewf_format,
         uint8_t error_tollerance )
{
	ewf_session_t session;
	uint8_t stored_crc_buffer[ 4 ];

	ewf_session_entry_t *sessions = NULL;
	static char *function         = "libewf_section_session_read";
	ewf_crc_t calculated_crc      = 0;
	ewf_crc_t stored_crc          = 0;
	ssize_t section_read_count    = 0;
	ssize_t read_count            = 0;
	size_t sessions_size          = 0;
	uint32_t amount_of_sessions   = 0;

	if( segment_file_handle == NULL )
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
	if( ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain session section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	section_read_count = libewf_segment_file_handle_read(
	                      segment_file_handle,
	                      &session,
	                      sizeof( ewf_session_t ) );

	if( section_read_count != (ssize_t) sizeof( ewf_session_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read session.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  &session,
	                  ( sizeof( ewf_session_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	if( libewf_endian_convert_32bit( &stored_crc, session.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert CRC value.\n",
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
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( session.unknown1, 28 ); );
#endif

	if( libewf_endian_convert_32bit( &amount_of_sessions, session.amount_of_sessions ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sessions value.\n",
		 function );

		return( -1 );
	}
	if( amount_of_sessions == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: session contains no session data!.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	else
	{
		sessions_size = sizeof( ewf_session_entry_t ) * amount_of_sessions;
		sessions      = (ewf_session_entry_t *) libewf_common_alloc( sessions_size );

		if( sessions == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocate session data.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_segment_file_handle_read(
		              segment_file_handle,
		              sessions,
		              sessions_size );
	
		if( read_count != (ssize_t) sessions_size )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read session data.\n",
			 function );

			libewf_common_free( sessions );

			return( -1 );
		}
		section_read_count += read_count;

		calculated_crc = ewf_crc_calculate( sessions, sessions_size, 1 );

		read_count = libewf_segment_file_handle_read(
		              segment_file_handle,
		              stored_crc_buffer,
		              sizeof( ewf_crc_t ) );

		if( read_count != (ssize_t) sizeof( ewf_crc_t ) )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read CRC from file descriptor.\n",
			 function );

			libewf_common_free( sessions );

			return( -1 );
		}
		section_read_count += read_count;

		if( libewf_endian_convert_32bit( &stored_crc, stored_crc_buffer ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to convert CRC value.\n",
			 function );

			libewf_common_free( sessions );

			return( -1 );
		}
		if( stored_crc != calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
			 function, stored_crc, calculated_crc );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				libewf_common_free( sessions );

				return( -1 );
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( (uint8_t *) sessions, sessions_size ); );
#endif

		libewf_common_free( sessions );
	}
	return( section_read_count );
}

/* Reads a data section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_data_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         libewf_media_values_t *media_values,
         uint8_t ewf_format,
         uint8_t error_tollerance )
{
	ewf_data_t *data           = NULL;
	static char *function      = "libewf_section_data_read";
	ewf_crc_t calculated_crc   = 0;
	ewf_crc_t stored_crc       = 0;
	ssize_t read_count         = 0;
	uint32_t amount_of_chunks  = 0;
	uint32_t sectors_per_chunk = 0;
	uint32_t bytes_per_sector  = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t error_granularity = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media values.\n",
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
	if( section_size != sizeof( ewf_data_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: mismatch in section data size.\n",
		 function );

		return( -1 );
	}
	data = (ewf_data_t *) libewf_common_alloc( sizeof( ewf_data_t ) );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create data.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_handle_read(
	              segment_file_handle,
	              data,
	              sizeof( ewf_data_t ) );
	
	if( read_count != (ssize_t) sizeof( ewf_data_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read data.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( data, ( sizeof( ewf_data_t ) - sizeof( ewf_crc_t ) ), 1 );

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
	if( media_values->media_type != data->media_type )
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
	if( media_values->amount_of_chunks != amount_of_chunks )
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
	if( media_values->sectors_per_chunk != sectors_per_chunk )
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
	if( media_values->bytes_per_sector != bytes_per_sector )
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
	if( media_values->amount_of_sectors != amount_of_sectors )
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
	if( media_values->error_granularity != error_granularity )
	{
		LIBEWF_WARNING_PRINT( "%s: error granularity does not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( media_values->media_flags != data->media_flags )
	{
		LIBEWF_WARNING_PRINT( "%s: media flags do not match in data section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_common_memcmp( media_values->guid, data->guid, 16 ) != 0 )
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
ssize_t libewf_section_data_write(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t format,
         ewf_data_t **cached_data_section,
         uint8_t no_section_append )
{
	ewf_char_t *section_type    = (ewf_char_t *) "data";
	static char *function       = "libewf_section_data_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	size_t section_type_length  = 4;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( cached_data_section == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid caches data section.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file_handle->file_offset;

	/* Check if the data section was already created
	 */
	if( *cached_data_section == NULL )
	{
		*cached_data_section = (ewf_data_t *) libewf_common_alloc( sizeof( ewf_data_t ) );

		if( *cached_data_section == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create data.\n",
			 function );

			return( -1 );
		}
		if( libewf_common_memset( *cached_data_section, 0, sizeof( ewf_data_t ) ) == NULL )
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
			( *cached_data_section )->media_type = media_values->media_type;
		}
		( *cached_data_section )->media_flags = media_values->media_flags;

		if( libewf_endian_revert_32bit(
		     media_values->amount_of_chunks,
		     ( *cached_data_section )->amount_of_chunks ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
			 function );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     media_values->sectors_per_chunk,
		     ( *cached_data_section )->sectors_per_chunk ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert sectors per chunk value.\n",
			 function );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     media_values->bytes_per_sector,
		     ( *cached_data_section )->bytes_per_sector ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert bytes per sector value.\n",
			 function );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     media_values->amount_of_sectors,
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
			     media_values->error_granularity,
			     ( *cached_data_section )->error_granularity ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to revert error granularity value.\n",
				 function );

				return( -1 );
			}
			( *cached_data_section )->compression_level = (uint8_t) compression_level;

			if( libewf_common_memcpy(
			     ( *cached_data_section )->guid,
			     media_values->guid,
			     16 ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
				 function );

				return( -1 );
			}
		}
		calculated_crc = ewf_crc_calculate(
		                  *cached_data_section,
		                  ( sizeof( ewf_data_t ) - sizeof( ewf_crc_t ) ),
		                  1 );

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
	                       segment_file_handle,
	                       section_type,
	                       section_type_length,
	                       (size64_t) sizeof( ewf_data_t ) );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               *cached_data_section,
	               sizeof( ewf_data_t ) );

	if( write_count != (ssize_t) sizeof( ewf_data_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write data to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( no_section_append == 0 )
	{
		if( libewf_section_list_append(
		     segment_file_handle->section_list,
		     section_type,
		     section_type_length,
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
ssize_t libewf_section_error2_read(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_sector_table_t *acquiry_errors,
         uint8_t ewf_format,
         uint8_t error_tollerance )
{
	ewf_error2_t error2;
	uint8_t stored_crc_buffer[ 4 ];

	ewf_error2_sector_t *error2_sectors = NULL;
	static char *function               = "libewf_section_error2_read";
	ewf_crc_t calculated_crc            = 0;
	ewf_crc_t stored_crc                = 0;
	ssize_t section_read_count          = 0;
	ssize_t read_count                  = 0;
	size_t sectors_size                 = 0;
	uint32_t amount_of_errors           = 0;
	uint32_t iterator                   = 0;
	uint32_t sector                     = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid acquiry errors.\n",
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
	section_read_count = libewf_segment_file_handle_read(
	                      segment_file_handle,
	                      &error2,
	                      sizeof( ewf_error2_t ) );
	
	if( section_read_count != (ssize_t) sizeof( ewf_error2_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read error2.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &error2, ( sizeof( ewf_error2_t ) - sizeof( ewf_crc_t ) ), 1 );

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
	sectors_size = sizeof( ewf_error2_sector_t ) * amount_of_errors;

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
		error2_sectors = (ewf_error2_sector_t *) libewf_common_alloc( sectors_size );

		if( error2_sectors == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocate error2 sectors.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_segment_file_handle_read(
		              segment_file_handle,
		              error2_sectors,
		              sectors_size );
	
		if( read_count != (ssize_t) sectors_size )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read error2 sectors.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		section_read_count += read_count;

		calculated_crc = ewf_crc_calculate( error2_sectors, sectors_size, 1 );

		read_count = libewf_segment_file_handle_read(
		              segment_file_handle,
		              stored_crc_buffer,
		              sizeof( ewf_crc_t ) );

		if( read_count != (ssize_t) sizeof( ewf_crc_t ) )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read CRC from file descriptor.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		section_read_count += read_count;

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
#if defined( HAVE_DEBUG_OUTPUT )
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( (uint8_t *) error2_sectors, sectors_size ); );
#endif
		if( acquiry_errors->error_sector != NULL )
		{
			LIBEWF_VERBOSE_PRINT( "%s: acquiry error sectors already set in handle - removing previous one.\n",
			 function );

			libewf_common_free( acquiry_errors->error_sector );

			acquiry_errors->amount = 0;
		}
		acquiry_errors->error_sector = (libewf_error_sector_t *) libewf_common_alloc(
		                                                          sizeof( libewf_error_sector_t ) * amount_of_errors );

		if( acquiry_errors->error_sector == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create acquiry error sectors.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		acquiry_errors->amount = amount_of_errors;

		for( iterator = 0; iterator < amount_of_errors; iterator++ )
		{
			if( libewf_endian_convert_32bit( &sector, error2_sectors[ iterator ].sector ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to convert sector value.\n",
				 function );

				libewf_common_free( error2_sectors );

				return( -1 );
			}
			acquiry_errors->error_sector[ iterator ].sector = (uint64_t) sector;

			if( libewf_endian_convert_32bit(
			     &( acquiry_errors->error_sector[ iterator ].amount_of_sectors ),
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
	return( section_read_count );
}

/* Writes a error2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_error2_write(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_sector_table_t *acquiry_errors )
{
	ewf_error2_t error2;
	uint8_t calculated_crc_buffer[ 4 ];

	ewf_error2_sector_t *error2_sectors = NULL;
	ewf_char_t *section_type            = (ewf_char_t *) "error2";
	static char *function               = "libewf_section_error2_write";
	ewf_crc_t calculated_crc            = 0;
	off64_t section_offset              = 0;
	ssize_t section_write_count         = 0;
	ssize_t write_count                 = 0;
	size_t section_type_length          = 6;
	size_t section_size                 = 0;
	size_t sectors_size                 = 0;
	uint32_t iterator                   = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file\n",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid acquiry errors.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file_handle->file_offset;
	sectors_size   = sizeof( ewf_error2_sector_t ) * acquiry_errors->amount;
	section_size   = sizeof( ewf_error2_t ) + sectors_size + sizeof( ewf_crc_t );

	if( libewf_common_memset( &error2, 0, sizeof( ewf_error2_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear error2.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( acquiry_errors->amount, error2.amount_of_errors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of errors value.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &error2, ( sizeof( ewf_error2_t ) - sizeof( ewf_crc_t ) ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, error2.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	error2_sectors = (ewf_error2_sector_t *) libewf_common_alloc( sectors_size );

	if( error2_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create error2 sectors.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < acquiry_errors->amount; iterator++ )
	{
		if( libewf_endian_revert_32bit(
		     (uint32_t) acquiry_errors->error_sector[ iterator ].sector,
		     error2_sectors[ iterator ].sector ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert sector value.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     acquiry_errors->error_sector[ iterator ].amount_of_sectors,
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
	                       segment_file_handle,
	                       section_type,
	                       section_type_length,
	                       (size64_t) section_size );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		libewf_common_free( error2_sectors );

		return( -1 );
	}
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               &error2,
	               sizeof( ewf_error2_t ) );

	if( write_count != (ssize_t) sizeof( ewf_error2_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write error2 to file.\n",
		 function );

		libewf_common_free( error2_sectors );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
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
	write_count = libewf_segment_file_handle_write(
 	               segment_file_handle,
 	               calculated_crc_buffer,
 	               sizeof( ewf_crc_t ) );

	if( write_count != (ssize_t) sizeof( ewf_crc_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write error2 sectors CRC to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section_type,
	     section_type_length,
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
ssize_t libewf_section_hash_read(
         libewf_segment_file_handle_t *segment_file_handle,
         EWF_DIGEST_HASH *md5_hash,
         uint8_t error_tollerance )
{
	ewf_hash_t hash;

	static char *function    = "libewf_section_hash_read";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	ssize_t read_count       = 0;

	if( segment_file_handle == NULL )
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
	read_count = libewf_segment_file_handle_read(
	              segment_file_handle,
	              &hash,
	              sizeof( ewf_hash_t ) );

	if( read_count != (ssize_t) sizeof( ewf_hash_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read hash.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate( &hash, ( sizeof( ewf_hash_t ) - sizeof( ewf_crc_t ) ), 1 );

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
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash.unknown1, 16 ); );
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
ssize_t libewf_section_hash_write(
         libewf_segment_file_handle_t *segment_file_handle,
         EWF_DIGEST_HASH *md5_hash )
{
	ewf_hash_t hash;

	ewf_char_t *section_type    = (ewf_char_t *) "hash";
	static char *function       = "libewf_section_hash_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	size_t section_type_length  = 4;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	section_offset = segment_file_handle->file_offset;

	if( libewf_common_memset(
	     &hash,
	     0,
	     sizeof( ewf_hash_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear hash.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memcpy(
	     hash.md5_hash,
	     md5_hash,
	     EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  &hash,
	                  ( sizeof( ewf_hash_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	if( libewf_endian_revert_32bit(
	     calculated_crc,
	     hash.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	section_write_count = libewf_section_start_write(
	                       segment_file_handle,
	                       section_type,
	                       section_type_length,
	                       (size64_t) sizeof( ewf_hash_t ) );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               &hash,
	               sizeof( ewf_hash_t ) );

	if( write_count != (ssize_t) sizeof( ewf_hash_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write hash to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section_type,
	     section_type_length,
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
ssize_t libewf_section_last_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *section_type,
         size_t section_type_length,
         uint8_t format,
         uint8_t ewf_format )
{
	ewf_section_t section;

	static char *function       = "libewf_section_last_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;
	size_t section_size         = 0;

	if( segment_file_handle == NULL )
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
		LIBEWF_WARNING_PRINT( "%s: section type length value cannot be zero.\n",
		 function );

		return( -1 );
	}
	if( section_type_length >= 16 )
	{
		LIBEWF_WARNING_PRINT( "%s: section type length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( &section, 0, sizeof( ewf_section_t ) ) == NULL )
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
		section_size = sizeof( ewf_section_t );
	}
	section_offset = segment_file_handle->file_offset;

	if( ewf_string_copy(
	     section.type,
	     section_type,
	     section_type_length ) == NULL )
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
	calculated_crc = ewf_crc_calculate( &section, ( sizeof( ewf_section_t ) - sizeof( ewf_crc_t ) ), 1 );

	if( libewf_endian_revert_32bit( calculated_crc, section.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	section_write_count = libewf_segment_file_handle_write(
	                       segment_file_handle,
	                       &section,
	                       sizeof( ewf_section_t ) );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function, (char *) section_type );

		return( -1 );
	}
	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section_type,
	     section_type_length,
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
ssize_t libewf_section_xheader_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         ewf_char_t **cached_xheader,
         size_t *cached_xheader_size )
{
	ewf_char_t *xheader   = NULL;
	static char *function = "libewf_section_xheader_read";
	ssize_t read_count    = 0;
	size_t xheader_size   = 0;

	if( segment_file_handle == NULL )
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
	              segment_file_handle,
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
ssize_t libewf_section_xheader_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *xheader,
         size_t xheader_size,
         int8_t compression_level )
{
	static char *function       = "libewf_section_xheader_write";
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: XHeader:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xheader, xheader_size ); );

	section_write_count = libewf_section_write_compressed_string(
	                       segment_file_handle,
	                       (ewf_char_t *) "xheader",
	                       7,
	                       xheader,
	                       xheader_size,
	                       compression_level );

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
ssize_t libewf_section_xhash_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         ewf_char_t **cached_xhash,
         size_t *cached_xhash_size )
{
	ewf_char_t *xhash     = NULL;
	static char *function = "libewf_section_xhash_read";
	ssize_t read_count    = 0;
	size_t xhash_size     = 0;

	if( segment_file_handle == NULL )
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
	              segment_file_handle,
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
ssize_t libewf_section_xhash_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *xhash,
         size_t xhash_size,
         int8_t compression_level )
{
	static char *function       = "libewf_section_xhash_write";
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: XHash:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xhash, xhash_size ); );

	section_write_count = libewf_section_write_compressed_string(
	                       segment_file_handle,
	                       (ewf_char_t *) "xhash",
	                       5,
	                       xhash,
	                       xhash_size,
	                       compression_level );

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
ssize_t libewf_section_delta_chunk_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         libewf_offset_table_t *offset_table,
         libewf_offset_table_t *secondary_offset_table,
         uint8_t error_tollerance )
{
	EWFX_DELTA_CHUNK_HEADER delta_chunk_header;

	static char *function    = "libewf_section_delta_chunk_read";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	uint32_t chunk           = 0;
	uint32_t chunk_size      = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) INT32_MAX )
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
	if( libewf_segment_file_handle_read(
	     segment_file_handle,
	     &delta_chunk_header,
	     EWFX_DELTA_CHUNK_HEADER_SIZE ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read delta chunk header.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  &delta_chunk_header,
	                  ( EWFX_DELTA_CHUNK_HEADER_SIZE - sizeof( ewf_crc_t ) ),
	                  1 );

	if( libewf_endian_convert_32bit(
	     &stored_crc,
	     delta_chunk_header.crc ) != 1 )
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
	/* The chunk value is stored + 1 count in the file
	 */
	if( libewf_endian_convert_32bit(
	     &chunk,
	     delta_chunk_header.chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert chunk value.\n",
		 function );

		return( -1 );
	}
	chunk -= 1;

	if( chunk >= offset_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid delta chunk: %" PRIu32 " value outside offset table.\n",
		 function, chunk );

		return( -1 );
	}
	if( libewf_endian_convert_32bit(
	     &chunk_size,
	     delta_chunk_header.chunk_size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert chunk size value.\n",
		 function );

		return( -1 );
	}
	if( chunk_size != ( section_size - EWFX_DELTA_CHUNK_HEADER_SIZE ) )
	{
		LIBEWF_WARNING_PRINT( "%s: chunk size does not match size of data in section.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		chunk_size = (uint32_t) ( section_size - EWFX_DELTA_CHUNK_HEADER_SIZE );
	}
	/* Update the chunk data in the offset table
	 */
	offset_table->chunk_offset[ chunk ].segment_file_handle = segment_file_handle;
	offset_table->chunk_offset[ chunk ].file_offset         = segment_file_handle->file_offset;
	offset_table->chunk_offset[ chunk ].size                = chunk_size;
	offset_table->chunk_offset[ chunk ].compressed          = 0;
	offset_table->chunk_offset[ chunk ].dirty               = 1;

	/* Update the chunk data in the secondary offset table
	 * if necessary
	 */
	if( secondary_offset_table != NULL )
	{
		secondary_offset_table->chunk_offset[ chunk ].segment_file_handle = segment_file_handle;
		secondary_offset_table->chunk_offset[ chunk ].file_offset         = segment_file_handle->file_offset;
		secondary_offset_table->chunk_offset[ chunk ].size                = chunk_size;
		secondary_offset_table->chunk_offset[ chunk ].compressed          = 0;
		secondary_offset_table->chunk_offset[ chunk ].dirty               = 1;
	}

	/* Skip the chunk data within the section
	 */
	if( libewf_segment_file_handle_seek_offset(
	     segment_file_handle,
	     ( segment_file_handle->file_offset + chunk_size ) ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
		 function );

		return( -1 );
	}
	return( (ssize_t) section_size );
}

/* Writes a delta chunk section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_delta_chunk_write(
         libewf_segment_file_handle_t *segment_file_handle,
         uint32_t chunk,
         ewf_char_t *chunk_data,
         size_t chunk_size,
         ewf_crc_t *chunk_crc,
         uint8_t write_crc,
         uint8_t no_section_append )
{
	EWFX_DELTA_CHUNK_HEADER delta_chunk_header;
	uint8_t calculated_crc_buffer[ 4 ];

	ewf_char_t *section_type    = (ewf_char_t *) "delta_chunk";
	static char *function       = "libewf_section_delta_chunk_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;
	size_t section_type_length  = 11;
	size_t section_size         = 0;
	size_t chunk_data_size      = 0;

	if( segment_file_handle == NULL )
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
	section_offset = segment_file_handle->file_offset;

	if( libewf_common_memset( &delta_chunk_header, 0, EWFX_DELTA_CHUNK_HEADER_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear delta chunk header.\n",
		 function );

		return( -1 );
	}
	chunk_data_size = chunk_size;

	if( write_crc != 0 )
	{
		chunk_data_size += sizeof( ewf_crc_t );
	}
	/* The chunk value is stored + 1 count in the file
	 */
	if( libewf_endian_revert_32bit(
	     ( chunk + 1 ),
	     delta_chunk_header.chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert chunk value.\n" );

		return( -1 );
	}
	if( libewf_endian_revert_32bit(
	     (uint32_t) chunk_data_size,
	     delta_chunk_header.chunk_size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert chunk size value.\n" );

		return( -1 );
	}
	delta_chunk_header.padding[ 0 ] = (uint8_t) 'D';
	delta_chunk_header.padding[ 1 ] = (uint8_t) 'E';
	delta_chunk_header.padding[ 2 ] = (uint8_t) 'L';
	delta_chunk_header.padding[ 3 ] = (uint8_t) 'T';
	delta_chunk_header.padding[ 4 ] = (uint8_t) 'A';

	calculated_crc = ewf_crc_calculate(
	                  &delta_chunk_header,
	                  ( EWFX_DELTA_CHUNK_HEADER_SIZE - sizeof( ewf_crc_t ) ),
	                  1 );

	if( libewf_endian_revert_32bit( calculated_crc, delta_chunk_header.crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
		 function );

		return( -1 );
	}
	section_size = EWFX_DELTA_CHUNK_HEADER_SIZE + chunk_data_size;

	section_write_count = libewf_section_start_write(
	                       segment_file_handle,
	                       section_type,
	                       section_type_length,
	                       (size64_t) section_size );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section: %s to file.\n",
		 function );

		return( -1 );
	}
	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
	               &delta_chunk_header,
	               EWFX_DELTA_CHUNK_HEADER_SIZE );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write chunk value to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_segment_file_handle_write(
	               segment_file_handle,
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
		write_count = libewf_segment_file_handle_write(
			       segment_file_handle,
			       calculated_crc_buffer,
			       sizeof( ewf_crc_t ) );

		if( write_count != (size_t) sizeof( ewf_crc_t ) )
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
		     segment_file_handle->section_list,
		     section_type,
		     section_type_length,
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

#if defined( HAVE_DEBUG_OUTPUT )

/* Reads a section from file for debugging purposes
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_debug_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size64_t section_size )
{
	uint8_t *data              = NULL;
	uint8_t *uncompressed_data = NULL;
	static char *function      = "libewf_debug_read_section";
	ssize_t read_count         = 0;
	size_t uncompressed_size   = 0;
	int result                 = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size64_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	uncompressed_size = (size_t) ( section_size * 2 );

	if( uncompressed_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: uncompressed size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	data = (uint8_t *) libewf_common_alloc( (size_t) section_size );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate data.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_segment_file_handle_read(
	              segment_file_handle,
	              data,
	              section_size );

	if( read_count != (ssize_t) section_size )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read section data.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	uncompressed_data = (uint8_t *) libewf_common_alloc( uncompressed_size );

	if( uncompressed_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate uncompressed data.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	result = libewf_uncompress(
	          uncompressed_data,
	          &uncompressed_size,
	          data,
	          (size_t) section_size );

	if( result == 0 )
	{
		fprintf( stderr, "%s: data is UNCOMPRESSED.\n",
		 function );

		libewf_debug_dump_data(
		 data,
		 (size_t) section_size );
	}
	else if( result == 1 )
	{
		fprintf( stderr, "%s: data is zlib COMPRESSED.\n",
		 function );

		libewf_debug_dump_data(
		 uncompressed_data,
		 uncompressed_size );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unable to uncompress data.\n",
		 function );

		libewf_common_free( data );
		libewf_common_free( uncompressed_data );

		return( -1 );
	}
	libewf_common_free( data );
	libewf_common_free( uncompressed_data );

	return( read_count );
}

#endif

/* Reads and processes a section
 * The section start offset will be updated
 * Returns 1 if successful, -1 on error
 */
int libewf_section_read(
     libewf_segment_file_handle_t *segment_file_handle,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_offset_table_t *secondary_offset_table,
     libewf_sector_table_t *acquiry_errors,
     int8_t *compression_level,
     uint8_t *format,
     uint8_t *ewf_format,
     size64_t *segment_file_size,
     ewf_section_t *section,
     off64_t *section_start_offset,
     uint8_t error_tollerance )
{
	static char *function      = "libewf_section_read";
	off64_t section_end_offset = 0;
	ssize64_t read_count       = 0;
	uint64_t size              = 0;
	uint64_t next_offset       = 0;
	size_t section_type_length = 0;

	if( segment_file_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( header_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header sections.\n",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash sections.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid acquiry errors.\n",
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
	if( compression_level == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compression level.\n",
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
	if( *section_start_offset > (off64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section start offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_section_start_read(
	     segment_file_handle,
	     section,
	     error_tollerance ) <= -1 )
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
	section_type_length = ewf_string_length( section->type );

	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section->type,
	     section_type_length,
	     *section_start_offset,
	     section_end_offset ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to append value to section list.\n",
		 function );

		return( -1 );
	}
	*section_start_offset += sizeof( ewf_section_t );

	/* No need to correct empty sections like done and next
	 */
	if( size > 0 )
	{
		size -= sizeof( ewf_section_t );
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
		if( ( segment_file_size != NULL )
		 && ( (off64_t) *segment_file_size < ( *section_start_offset + (off64_t) sizeof( ewf_section_t ) ) ) )
		{
			*segment_file_size = (size64_t) ( *section_start_offset + sizeof( ewf_section_t ) );
		}
	}
	/* Read the header2 section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "header2", 8 ) == 0 )
	{
		read_count = libewf_section_header2_read(
		              segment_file_handle,
		              (size_t) size,
		              &( header_sections->header2 ),
		              &( header_sections->header2_size ) );

		header_sections->amount_of_header_sections++;
	}
	/* Read the header section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "header", 7 ) == 0 )
	{
		read_count = libewf_section_header_read(
		              segment_file_handle,
		              (size_t) size,
		              &( header_sections->header ),
		              &( header_sections->header_size ) );

		header_sections->amount_of_header_sections++;
	}
	/* Read the xheader section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "xheader", 8 ) == 0 )
	{
		read_count = libewf_section_xheader_read(
		              segment_file_handle,
		              (size_t) size,
		              &( header_sections->xheader ),
		              &( header_sections->xheader_size ) );

		header_sections->amount_of_header_sections++;
	}
	/* Read the volume or disk section
	 * The \0 byte is included in the compare
	 */
	else if( ( ewf_string_compare( section->type, "volume", 7 ) == 0 )
	 || ( ewf_string_compare( section->type, "disk", 5 ) == 0 ) )
	{
		read_count = libewf_section_volume_read(
		              segment_file_handle,
		              (size_t) size,
		              media_values,
		              compression_level,
		              format,
		              ewf_format,
		              error_tollerance );

		/* Check if the EWF file format is that of EnCase1
		 * this allows the table read function to reduce verbose
		 * output of unexpected additional data in table section
		 */
		if( ( *ewf_format == EWF_FORMAT_E01 )
		 && ( header_sections->amount_of_header_sections == 1 ) )
		{
			*format = LIBEWF_FORMAT_ENCASE1;
		}
	}
	/* Read the table2 section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "table2", 7 ) == 0 )
	{
		read_count = libewf_section_table_read(
		              segment_file_handle,
		              (size_t) size,
		              media_values->amount_of_chunks,
		              secondary_offset_table,
		              *format,
		              *ewf_format,
		              error_tollerance );
	}
	/* Read the table section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "table", 6 ) == 0 )
	{
		read_count = libewf_section_table_read(
		              segment_file_handle,
		              (size_t) size,
		              media_values->amount_of_chunks,
		              offset_table,
		              *format,
		              *ewf_format,
		              error_tollerance );
	}
	/* Read the sectors section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "sectors", 8 ) == 0 )
	{
		read_count = libewf_section_sectors_read(
		              segment_file_handle,
 		              (size64_t) size,
 		              *ewf_format,
 		              error_tollerance );
	}
	/* Read the delta_chunk section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "delta_chunk", 12 ) == 0 )
	{
		read_count = libewf_section_delta_chunk_read(
 		              segment_file_handle,
 		              (size_t) size,
 		              offset_table,
 		              secondary_offset_table,
 		              error_tollerance );
	}
	/* Read the ltree section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "ltree", 6 ) == 0 )
	{
		read_count = libewf_section_ltree_read(
		              segment_file_handle,
 		              (size_t) size,
		              ewf_format,
 		              error_tollerance );
	}
	/* Read the session section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "session", 8 ) == 0 )
	{
		read_count = libewf_section_session_read(
		              segment_file_handle,
		              (size_t) size,
		              *ewf_format,
 		              error_tollerance );
	}
	/* Read the data section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "data", 5 ) == 0 )
	{
		read_count = libewf_section_data_read(
		              segment_file_handle,
		              (size_t) size,
		              media_values,
		              *ewf_format,
 		              error_tollerance );
	}
	/* Read the hash section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "hash", 5 ) == 0 )
	{
		read_count = libewf_section_hash_read(
		              segment_file_handle,
		              hash_sections->md5_hash,
 		              error_tollerance );

		hash_sections->md5_hash_set = 1;

	}
	/* Read the xhash section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "xhash", 6 ) == 0 )
	{
		read_count = libewf_section_xhash_read(
		              segment_file_handle,
		              (size_t) size,
		              &( hash_sections->xhash ),
		              &( hash_sections->xhash_size ) );
	}
	/* Read the error2 section
	 * The \0 byte is included in the compare
	 */
	else if( ewf_string_compare( section->type, "error2", 7 ) == 0 )
	{
		read_count = libewf_section_error2_read(
		              segment_file_handle,
		              acquiry_errors,
		              *ewf_format,
 		              error_tollerance );
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
		LIBEWF_VERBOSE_EXEC( read_count = libewf_section_debug_read(
		                                   segment_file_handle,
		                                   (size_t) size ); );
#else
		/* Skip the data within the section
		 */
		if( libewf_segment_file_handle_seek_offset(
		     segment_file_handle,
		     section_end_offset ) == -1 )
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

