/*
 * Section reading/writing functions
 *
 * Copyright (c) 2006-2008,
 Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation,
 either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not,
 see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <endian.h>
#include <memory.h>
#include <narrow_string.h>
#include <notify.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_error.h"
#include "libewf_compression.h"
#include "libewf_debug.h"
#include "libewf_error.h"
#include "libewf_header_values.h"
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
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_start_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         off64_t section_start_offset,
         ewf_section_t *section,
         uint64_t *section_size,
         uint64_t *section_next,
         libewf_error_t **error )
{
	static char *function    = "libewf_section_start_read";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	ssize_t read_count       = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.\n",
		 function );

		return( -1 );
	}
	if( section_size == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section size.\n",
		 function );

		return( -1 );
	}
	if( section_next == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section next.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_file_io_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              (uint8_t *) section,
	              sizeof( ewf_section_t ),
	              error );

	if( read_count != sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read section start.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  section,
	                  sizeof( ewf_section_t ) - sizeof( ewf_crc_t ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 section->crc );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		/* TODO error_tollerance */

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_section_print(
	       section,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print section.\n",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 section->padding,
	 40 );
#endif

	endian_little_convert_64bit(
	 *section_size,
	 section->size );

	if( *section_size > (uint64_t) INT64_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	endian_little_convert_64bit(
	 *section_next,
	 section->next );

	if( *section_next > (uint64_t) INT64_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section next value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes a section start to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_start_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         off64_t segment_file_offset,
         uint8_t *section_type,
         size_t section_type_length,
         size64_t section_data_size,
         libewf_error_t **error )
{
	ewf_section_t section;

	static char *function    = "libewf_section_start_write";
	ewf_crc_t calculated_crc = 0;
	ssize_t write_count      = 0;
	uint64_t section_size    = 0;
	uint64_t section_offset  = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( segment_file_offset < 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid segment file offset value less than zero.\n",
		 function );

		return( -1 );
	}
	if( section_type == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section type.\n",
		 function );

		return( -1 );
	}
	if( ( section_type_length == 0 )
	 || ( section_type_length >= 16 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: section type is out of range.\n",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &section,
	     0,
	     sizeof( ewf_section_t ) ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear section.\n",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     section.type,
	     section_type,
	     section_type_length ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set section type.\n",
		 function );

		return( -1 );
	}
	section_size   = sizeof( ewf_section_t ) + section_data_size;
	section_offset = (uint64_t) segment_file_offset + section_size;

	if( section_size > (uint64_t) INT64_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( section_offset > (uint64_t) INT64_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	endian_little_revert_64bit(
	 section.size,
	 section_size );

	endian_little_revert_64bit(
	 section.next,
	 section_offset );

	calculated_crc = ewf_crc_calculate(
	                  &section,
	                  ( sizeof( ewf_section_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_revert_32bit(
	 section.crc,
	 calculated_crc );

#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: writing section start of type: %s with size: %" PRIu64 " and CRC: %" PRIu32 ".\n",
	 function,
	 (char *) section_type,
	 section_size,
	 calculated_crc );
#endif

	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) &section,
	               sizeof( ewf_section_t ),
	               error );

	if( write_count != sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section to file.\n",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Reads a compressed string section from a segment file and uncompresses it
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_compressed_string_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t compressed_string_size,
         uint8_t **uncompressed_string,
         size_t *uncompressed_string_size,
         libewf_error_t **error )
{
	uint8_t *compressed_string = NULL;
	uint8_t *uncompressed      = NULL;
	static char *function      = "libewf_section_compressed_string_read";
	void *reallocation         = NULL;
	ssize_t read_count         = 0;
	int result                 = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( ( uncompressed_string == NULL )
	 || ( *uncompressed_string != NULL ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed string.\n",
		 function );

		return( -1 );
	}
	if( uncompressed_string_size == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed string size.\n",
		 function );

		return( -1 );
	}
	if( compressed_string_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed string size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	compressed_string = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * compressed_string_size );

	if( compressed_string == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed string.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_file_io_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              compressed_string,
	              compressed_string_size,
	              error );

	if( read_count != (ssize_t) compressed_string_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read compressed string.\n",
		 function );

		memory_free(
		 compressed_string );

		return( -1 );
	}
	/* On average the uncompressed string will be twice as large as the compressed string
	 */
	*uncompressed_string_size = 2 * compressed_string_size;

	uncompressed = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * *uncompressed_string_size );

	if( uncompressed == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed string.\n",
		 function );

		memory_free(
		 compressed_string );

		return( -1 );
	}
	result = libewf_uncompress(
	          uncompressed,
	          uncompressed_string_size,
	          compressed_string,
	          compressed_string_size,
	          error );

	while( ( result == -1 )
	    && ( *uncompressed_string_size > 0 ) )
	{
		reallocation = memory_reallocate(
		                uncompressed,
		                sizeof( uint8_t ) * *uncompressed_string_size );

		if( reallocation == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize uncompressed string.\n",
			 function );

			memory_free(
			 compressed_string );
			memory_free(
			 uncompressed );

			return( -1 );
		}
		uncompressed = (uint8_t *) reallocation;

		result = libewf_uncompress(
		          uncompressed,
		          uncompressed_string_size,
		          compressed_string,
		          compressed_string_size,
		          error );
	}
	memory_free(
	 compressed_string );

	if( result == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_COMPRESSION,
		 LIBEWF_COMPRESSION_ERROR_UNCOMPRESS_FAILED,
		 "%s: unable to uncompress string.\n",
		 function );

		memory_free(
		 uncompressed );

		return( -1 );
	}
	*uncompressed_string = uncompressed;

	return( read_count );
}

/* Writes a compressed string section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_write_compressed_string(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         off64_t segment_file_offset,
         uint8_t *section_type,
         size_t section_type_length,
         uint8_t *uncompressed_string,
         size_t uncompressed_string_size,
         int8_t compression_level,
         libewf_error_t **error )
{
	uint8_t *compressed_string    = NULL;
	static char *function         = "libewf_section_write_compressed_string";
	void *reallocation            = NULL;
	size_t compressed_string_size = 0;
	ssize_t section_write_count   = 0;
	ssize_t write_count           = 0;
	int result                    = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( segment_file_offset < 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid segment file offset value less than zero.\n",
		 function );

		return( -1 );
	}
	if( section_type == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section type.\n",
		 function );

		return( -1 );
	}
	if( uncompressed_string == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed string.\n",
		 function );

		return( -1 );
	}
	compressed_string_size = uncompressed_string_size;
	compressed_string      = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * compressed_string_size );

	if( compressed_string == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compress string.\n",
		 function );

		return( -1 );
	}
	result = libewf_compress(
	          compressed_string,
	          &compressed_string_size,
	          uncompressed_string,
	          uncompressed_string_size,
	          compression_level,
	          error );

	if( ( result == -1 )
	 && ( compressed_string_size > 0 ) )
	{
		reallocation = memory_reallocate(
		                compressed_string,
		                ( sizeof( uint8_t ) * compressed_string_size ) );

		if( reallocation == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize compressed string.\n",
			 function );

			memory_free(
			 compressed_string );

			return( -1 );
		}
		compressed_string = (uint8_t *) reallocation;

		result = libewf_compress(
		          compressed_string,
		          &compressed_string_size,
		          uncompressed_string,
		          uncompressed_string_size,
		          compression_level,
		          error );
	}
	if( result == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_COMPRESSION,
		 LIBEWF_COMPRESSION_ERROR_COMPRESS_FAILED,
		 "%s: unable to compress string.\n",
		 function );

		memory_free(
		 compressed_string );

		return( -1 );
	}
	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       segment_file_offset,
	                       section_type,
	                       section_type_length,
	                       (size64_t) compressed_string_size,
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section to file.\n",
		 function );

		memory_free(
		 compressed_string );

		return( -1 );
	}
	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               compressed_string,
	               compressed_string_size,
	               error );

	memory_free(
	 compressed_string );

	if( write_count != (ssize_t) compressed_string_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section_type,
	     section_type_length,
	     segment_file_offset,
	     segment_file_offset + section_write_count,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append: %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a header section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_header_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint8_t **cached_header,
         size_t *cached_header_size,
         libewf_error_t **error )
{
	uint8_t *header       = NULL;
	static char *function = "libewf_section_header_read";
	ssize_t read_count    = 0;
	size_t header_size    = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( cached_header == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached header.\n",
		 function );

		return( -1 );
	}
	if( cached_header_size == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached header size.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              file_io_pool,
	              segment_file_handle,
	              section_size,
	              &header,
	              &header_size,
	              error );

	if( read_count != (ssize_t) section_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read header.\n",
		 function );

		return( -1 );
	}
	if( header == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_byte_stream_print(
	       _LIBEWF_STRING( "Header" ),
	       header,
	       header_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print header.\n",
		 function );

		memory_free(
		 header );

		return( -1 );
	}
#endif

	if( *cached_header == NULL )
	{
		*cached_header      = header;
		*cached_header_size = header_size;
	}
	else
	{
		memory_free(
		 header );
	}
	return( read_count );
}

/* Writes a header section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_header_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         uint8_t *header,
         size_t header_size,
         int8_t compression_level,
         libewf_error_t **error )
{
	static char *function       = "libewf_section_header_write";
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_byte_stream_print(
	       _LIBEWF_STRING( "Header" ),
	       header,
	       header_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print header.\n",
		 function );

		return( -1 );
	}
#endif

	section_write_count = libewf_section_write_compressed_string(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       (uint8_t *) "header",
	                       6,
	                       header,
	                       header_size,
	                       compression_level,
	                       error );

	if( section_write_count == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write header to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a header2 section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_header2_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint8_t **cached_header2,
         size_t *cached_header2_size,
         libewf_error_t **error )
{
	uint8_t *header2      = NULL;
	static char *function = "libewf_section_header2_read";
	ssize_t read_count    = 0;
	size_t header2_size   = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( cached_header2 == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached header2.\n",
		 function );

		return( -1 );
	}
	if( cached_header2_size == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached header2 size.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              file_io_pool,
	              segment_file_handle,
	              section_size,
	              &header2,
	              &header2_size,
	              error );

	if( read_count != (ssize_t) section_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read header2.\n",
		 function );

		return( -1 );
	}
	if( header2 == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header2.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_utf16_stream_print(
	       _LIBEWF_STRING( "Header2" ),
	       header2,
	       header2_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print header2.\n",
		 function );

		memory_free(
		 header2 );

		return( -1 );
	}
#endif
	if( *cached_header2 == NULL )
	{
		*cached_header2      = header2;
		*cached_header2_size = header2_size;
	}
	else
	{
		memory_free(
		 header2 );
	}
	return( read_count );
}

/* Writes a header2 section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_header2_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         uint8_t *header2,
         size_t header2_size,
         int8_t compression_level,
         libewf_error_t **error )
{
	static char *function       = "libewf_section_header2_write";
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_utf16_stream_print(
	       _LIBEWF_STRING( "Header2" ),
	       header2,
	       header2_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print header2.\n",
		 function );

		return( -1 );
	}
#endif
	section_write_count = libewf_section_write_compressed_string(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       (uint8_t *) "header2",
	                       7,
	                       header2,
	                       header2_size,
	                       compression_level,
	                       error );

	if( section_write_count == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write header2 to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads an EWF-S01 (SMART) volume section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_volume_s01_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         uint8_t *format,
         libewf_error_t **error )
{
	ewf_volume_smart_t *volume = NULL;
	static char *function      = "libewf_section_volume_s01_read";
	ewf_crc_t calculated_crc   = 0;
	ewf_crc_t stored_crc       = 0;
	ssize_t read_count         = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format.\n",
		 function );

		return( -1 );
	}
	volume = (ewf_volume_smart_t *) memory_allocate(
	                                 sizeof( ewf_volume_smart_t ) );

	if( volume == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_file_io_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              (uint8_t *) volume,
	              sizeof( ewf_volume_smart_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_volume_smart_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume.\n",
		 function );

		memory_free(
		 volume );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  volume,
	                  ( sizeof( ewf_volume_smart_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 volume->crc );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		/* TODO error_tollerance */

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 volume->unknown1,
	 4 );
	notify_dump_data(
	 volume->unknown2,
	 20 );
	notify_dump_data(
	 volume->unknown3,
	 45 );
#endif

	endian_little_convert_32bit(
	 media_values->amount_of_chunks,
	 volume->amount_of_chunks );

	endian_little_convert_32bit(
	 media_values->sectors_per_chunk,
	 volume->sectors_per_chunk );

	endian_little_convert_32bit(
	 media_values->bytes_per_sector,
	 volume->bytes_per_sector );

	endian_little_convert_32bit(
	 media_values->amount_of_sectors,
	 volume->amount_of_sectors );

	if( memory_compare(
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
	memory_free(
	 volume );

	return( read_count );
}

/* Writes an EWF-S01 (SMART) volume section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_volume_s01_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         uint8_t format,
         uint8_t no_section_append,
         libewf_error_t **error )
{
	uint8_t *section_type       = (uint8_t *) "volume";
	ewf_volume_smart_t *volume  = NULL;
	static char *function       = "libewf_section_volume_s01_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	size_t section_type_length  = 6;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	volume = (ewf_volume_smart_t *) memory_allocate(
	                                 sizeof( ewf_volume_smart_t ) );

	if( volume == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.\n",
		 function );

		return( -1 );
	}
	if( memory_set(
	     volume,
	     0,
	     sizeof( ewf_volume_smart_t ) ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.\n",
		 function );

		memory_free(
		 volume );

		return( -1 );
	}
	volume->unknown1[ 0 ] = 1;

	endian_little_revert_32bit(
	 volume->amount_of_chunks,
	 media_values->amount_of_chunks );

	endian_little_revert_32bit(
	 volume->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	endian_little_revert_32bit(
	 volume->bytes_per_sector,
	 media_values->bytes_per_sector );

	endian_little_revert_32bit(
	 volume->amount_of_sectors,
	 media_values->amount_of_sectors );

	if( format == LIBEWF_FORMAT_SMART )
	{
		volume->signature[ 0 ] = (uint8_t) 'S';
		volume->signature[ 1 ] = (uint8_t) 'M';
		volume->signature[ 2 ] = (uint8_t) 'A';
		volume->signature[ 3 ] = (uint8_t) 'R';
		volume->signature[ 4 ] = (uint8_t) 'T';
	}
	calculated_crc = ewf_crc_calculate(
	                  volume,
	                  ( sizeof( ewf_volume_smart_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_revert_32bit(
	 volume->crc,
	 calculated_crc );

#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
	 function,
	 media_values->amount_of_chunks,
	 media_values->chunk_size,
	 media_values->sectors_per_chunk );

	notify_verbose_printf(
	 "%s: volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n",
	 function,
	 media_values->amount_of_sectors,
	 media_values->bytes_per_sector );
#endif

	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       section_type,
	                       section_type_length,
	                       (size64_t) sizeof( ewf_volume_smart_t ),
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section to file.\n",
		 function );

		memory_free(
		 volume );

		return( -1 );
	}
	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) volume,
	               sizeof( ewf_volume_smart_t ),
	               error );

	memory_free(
	 volume );

	if( write_count != (ssize_t) sizeof( ewf_volume_smart_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write volume to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( ( no_section_append == 0 )
	 && ( libewf_section_list_append(
	       segment_file_handle->section_list,
	       section_type,
	       section_type_length,
	       section_offset,
	       section_offset + section_write_count,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append: %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads an EWF-E01 (EnCase) volume section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_volume_e01_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         int8_t *compression_level,
         libewf_error_t **error )
{
	ewf_volume_t *volume     = NULL;
	static char *function    = "libewf_section_volume_e01_read";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	ssize_t read_count       = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( compression_level == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression level.\n",
		 function );

		return( -1 );
	}
	volume = (ewf_volume_t *) memory_allocate(
	                           sizeof( ewf_volume_t ) );

	if( volume == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to read volume.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_file_io_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              (uint8_t *) volume,
	              sizeof( ewf_volume_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_volume_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume.\n",
		 function );

		memory_free(
		 volume );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  volume,
	                  ( sizeof( ewf_volume_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 volume->crc );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		memory_free(
		 volume );

		/* TODO error_tollerance */

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 volume->unknown1,
	 3 );
	notify_dump_data(
	 volume->unknown2,
	 16 );
	notify_dump_data(
	 volume->unknown3,
	 3 );
	notify_dump_data(
	 volume->unknown4,
	 12 );
	notify_dump_data(
	 volume->unknown5,
	 3 );
	notify_dump_data(
	 volume->unknown6,
	 4 );
	notify_dump_data(
	 volume->unknown7,
	 963 );
	notify_dump_data(
	 volume->signature,
	 5 );
#endif

	endian_little_convert_32bit(
	 media_values->amount_of_chunks,
	 volume->amount_of_chunks );

	endian_little_convert_32bit(
	 media_values->sectors_per_chunk,
	 volume->sectors_per_chunk );

	endian_little_convert_32bit(
	 media_values->bytes_per_sector,
	 volume->bytes_per_sector );

	endian_little_convert_32bit(
	 media_values->amount_of_sectors,
	 volume->amount_of_sectors );

	endian_little_convert_32bit(
	 media_values->error_granularity,
	 volume->error_granularity );

	media_values->media_type  = volume->media_type;
	media_values->media_flags = volume->media_flags;
	*compression_level        = (int8_t) volume->compression_level;

	if( memory_copy(
	     media_values->guid,
	     volume->guid,
	     16 ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set GUID.\n",
		 function );

		memory_free(
		 volume );

		return( -1 );
	}
	memory_free(
	 volume );

	return( read_count );
}

/* Writes an EWF-E01 (EnCase) volume section to file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_volume_e01_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t format,
         uint8_t no_section_append,
         libewf_error_t **error )
{
	uint8_t *section_type       = (uint8_t *) "volume";
	ewf_volume_t *volume        = NULL;
	static char *function       = "libewf_section_volume_e01_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	size_t section_type_length  = 6;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	volume = (ewf_volume_t *) memory_allocate(
	                           sizeof( ewf_volume_t ) );

	if( volume == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.\n",
		 function );

		return( -1 );
	}
	if( memory_set(
	     volume,
	     0,
	     sizeof( ewf_volume_t ) ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.\n",
		 function );

		memory_free(
		 volume );

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

	endian_little_revert_32bit(
	 volume->amount_of_chunks,
	 media_values->amount_of_chunks );

	endian_little_revert_32bit(
	 volume->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	endian_little_revert_32bit(
	 volume->bytes_per_sector,
	 media_values->bytes_per_sector );

	endian_little_revert_32bit(
	 volume->amount_of_sectors,
	 media_values->amount_of_sectors );

	if( ( format == LIBEWF_FORMAT_ENCASE5 )
	 || ( format == LIBEWF_FORMAT_ENCASE6 )
	 || ( format == LIBEWF_FORMAT_LINEN5 )
	 || ( format == LIBEWF_FORMAT_LINEN6 )
	 || ( format == LIBEWF_FORMAT_EWFX ) )
	{
		volume->compression_level = (uint8_t) compression_level;

		if( memory_copy(
		     volume->guid,
		     media_values->guid,
		     16 ) == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set GUID.\n",
			 function );

			memory_free(
			 volume );

			return( -1 );
		}
		endian_little_revert_32bit(
		 volume->error_granularity,
		 media_values->error_granularity );
	}
	calculated_crc = ewf_crc_calculate(
	                  volume,
	                  sizeof( ewf_volume_t ) - sizeof( ewf_crc_t ),
	                  1 );

	endian_little_revert_32bit(
	 volume->crc,
	 calculated_crc );

#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
	 function,
	 media_values->amount_of_chunks,
	 media_values->chunk_size,
	 media_values->sectors_per_chunk );

	notify_verbose_printf(
	 "%s: volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n",
	 function,
	 media_values->amount_of_sectors,
	 media_values->bytes_per_sector );
#endif

	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       section_type,
	                       section_type_length,
	                       (size64_t) sizeof( ewf_volume_t ),
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section to file.\n",
		 function );

		memory_free(
		 volume );

		return( -1 );
	}
	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) volume,
	               sizeof( ewf_volume_t ),
	               error );

	memory_free(
	 volume );

	if( write_count != (ssize_t) sizeof( ewf_volume_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write volume to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( ( no_section_append == 0 )
	 && ( libewf_section_list_append(
	       segment_file_handle->section_list,
	       section_type,
	       section_type_length,
	       section_offset,
	       section_offset + section_write_count,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append: %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a volume section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_volume_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         libewf_media_values_t *media_values,
         int8_t *compression_level,
         uint8_t *format,
         uint8_t *ewf_format,
         libewf_error_t **error )
{
	static char *function    = "libewf_section_volume_read";
	ssize_t read_count       = 0;
	size64_t bytes_per_chunk = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ewf format.\n",
		 function );

		return( -1 );
	}
	if( section_size == sizeof( ewf_volume_smart_t ) )
	{
		*ewf_format = EWF_FORMAT_S01;
		read_count  = libewf_section_volume_s01_read(
		               file_io_pool,
		               segment_file_handle,
		               media_values,
		               format,
		               error );
	}
	else if( section_size == sizeof( ewf_volume_t ) )
	{
		*ewf_format = EWF_FORMAT_E01;
		read_count  = libewf_section_volume_e01_read(
		               file_io_pool,
		               segment_file_handle,
		               media_values,
		               compression_level,
		               error );
	}
	else
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported section data size.\n",
		 function );

		return( -1 );
	}
	if( read_count != (ssize_t) section_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume section.\n",
		 function );

		return( -1 );
	}
	if( media_values->sectors_per_chunk > (uint32_t) INT32_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid sectors per chunk value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( media_values->bytes_per_sector > (uint32_t) INT32_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid bytes per sector value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	bytes_per_chunk = (size64_t) media_values->sectors_per_chunk
	                * (size64_t) media_values->bytes_per_sector;

	if( bytes_per_chunk > (size64_t) INT32_MAX )
	{
		notify_verbose_printf(
		 "%s: chunk size value exceeds maximum defaulting to: %d.n",
		 function,
		 EWF_MINIMUM_CHUNK_SIZE );

		media_values->chunk_size = EWF_MINIMUM_CHUNK_SIZE;
	}
	else
	{
		media_values->chunk_size = (uint32_t) bytes_per_chunk;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
	 function,
	 media_values->amount_of_chunks,
	 media_values->chunk_size,
	 media_values->sectors_per_chunk );

	notify_verbose_printf(
	 "%s: volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n",
	 function,
	 media_values->amount_of_sectors,
	 media_values->bytes_per_sector );
#endif

	if( media_values->amount_of_chunks == 0 )
	{
		*ewf_format = EWF_FORMAT_L01;
	}
	return( read_count );
}

/* Reads a table section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_table_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint32_t media_amount_of_chunks,
         libewf_offset_table_t *offset_table,
         uint8_t format,
         uint8_t ewf_format,
         libewf_error_t **error )
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
	uint8_t offsets_tainted     = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	/* Allocate the necessary amount of chunk offsets
	 * this reduces the amount of reallocations
	 */
	if( ( offset_table->amount_of_chunk_offsets < media_amount_of_chunks )
	 && ( libewf_offset_table_resize(
	       offset_table,
	       media_amount_of_chunks,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize offset table.\n",
		 function );

		return( -1 );
	}
	section_read_count = libewf_file_io_pool_read(
	                      file_io_pool,
	                      segment_file_handle->file_io_pool_entry,
	                      (uint8_t *) &table,
	                      sizeof( ewf_table_t ),
	                      error );
	
	if( section_read_count != (ssize_t) sizeof( ewf_table_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read table.\n",
		 function );

		return( -1 );
	}
	/* The table size contains the size of the CRC (4 bytes)
	 */
	calculated_crc = ewf_crc_calculate(
	                  &table,
	                  ( sizeof( ewf_table_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 table.crc );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		/* TODO error_tollerance */

		return( -1 );
	}
	endian_little_convert_32bit(
	 amount_of_chunks,
	 table.amount_of_chunks );

	endian_little_convert_64bit(
	 base_offset,
	 table.base_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 table.padding1,
	 4 );
	notify_dump_data(
	 table.padding2,
	 4 );
#endif
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: table is of size %" PRIu32 " chunks CRC %" PRIu32 " (%" PRIu32 ").\n",
	 function,
	 amount_of_chunks,
	 stored_crc,
	 calculated_crc );
#endif

	if( amount_of_chunks > 0 )
	{
		/* Check if the maximum amount of offsets is not exceeded
		 */
		if( amount_of_chunks > EWF_MAXIMUM_OFFSETS_IN_TABLE )
		{
			notify_verbose_printf(
			 "%s: table contains more offsets: %" PRIu32 " than the maximum amount: %d.\n",
			 function,
			 amount_of_chunks,
			 EWF_MAXIMUM_OFFSETS_IN_TABLE );
		}
		offsets_size = sizeof( ewf_table_offset_t ) * amount_of_chunks;

		if( offsets_size > (size_t) SSIZE_MAX )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid offsets size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		offsets = (ewf_table_offset_t *) memory_allocate(
		                                  offsets_size );

		if( offsets == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to allocated table offsets.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_file_io_pool_read(
		              file_io_pool,
		              segment_file_handle->file_io_pool_entry,
		              (uint8_t *) offsets,
		              offsets_size,
		              error );
	
		if( read_count != (ssize_t) offsets_size )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_READ_FAILED,
			 "%s: unable to read table offsets.\n",
			 function );

			memory_free(
			 offsets );

			return( -1 );
		}
		section_read_count += read_count;

		/* The EWF-S01 format does not contain a CRC after the offsets
		 */
		if( ewf_format != EWF_FORMAT_S01 )
		{
			/* Check if the offset table CRC matches
			 */
			calculated_crc = ewf_crc_calculate(
			                  offsets,
			                  offsets_size,
			                  1 );

			read_count = libewf_file_io_pool_read(
			              file_io_pool,
			              segment_file_handle->file_io_pool_entry,
			              stored_crc_buffer,
			              sizeof( ewf_crc_t ),
			              error );

			if( read_count != (ssize_t) sizeof( ewf_crc_t ) )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_IO,
				 LIBEWF_IO_ERROR_READ_FAILED,
				 "%s: unable to read CRC from file descriptor.\n",
				 function );

				memory_free(
				 offsets );

				return( -1 );
			}
			section_read_count += read_count;

			endian_little_convert_32bit(
			 stored_crc,
			 stored_crc_buffer );

			if( stored_crc != calculated_crc )
			{
				notify_verbose_printf(
				 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
				 function,
				 stored_crc,
				 calculated_crc );

				/* The offsets cannot be fully trusted therefore mark them as tainted during fill 
				 */
				offsets_tainted = 1;
			}
		}
		if( libewf_offset_table_fill(
		     offset_table,
		     (off64_t) base_offset,
		     offsets,
		     amount_of_chunks,
		     segment_file_handle,
		     offsets_tainted,
		     error ) != 1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to fill offset table.\n",
			 function );

			memory_free(
			 offsets );

			return( -1 );
		}
		memory_free(
		 offsets );

		if( libewf_offset_table_fill_last_offset(
		     offset_table,
		     segment_file_handle->section_list,
		     offsets_tainted,
		     error ) != 1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to fill last offset.\n",
			 function );

			return( -1 );
		}
	}
	else
	{
		notify_verbose_printf(
		 "%s: table contains no offsets.\n",
		 function );

		/* TODO error_tollerance
		 * mark all offset that should be in the table with an error flag?
		 * how do you now which offsets should be in the table?
		 */
	}
	if( section_size < (size_t) section_read_count )
	{
		notify_verbose_printf(
		 "%s: section size: %" PRIzd " smaller than section read count: %" PRIzd ".\n",
		 function,
		 section_size,
		 section_read_count );
	}
	/* Skip the chunk data within the section
	 * for chunks after the table section
	 */
	if( section_size != (size_t) section_read_count )
	{
		if( ( ewf_format != EWF_FORMAT_S01 )
		 && ( format != LIBEWF_FORMAT_ENCASE1 ) )
		{
			notify_verbose_printf(
			 "%s: unexpected data found after table offsets.\n",
			 function );
		}
		if( libewf_file_io_pool_seek_offset(
		     file_io_pool,
		     segment_file_handle->file_io_pool_entry,
		     section_size - section_read_count,
		     SEEK_CUR,
		     error ) == -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_SEEK_FAILED,
			 "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
		section_read_count = (ssize_t) section_size;
	}
	segment_file_handle->amount_of_chunks += amount_of_chunks;

	return( section_read_count );
}

/* Reads a table2 section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_table2_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint32_t media_amount_of_chunks,
         libewf_offset_table_t *offset_table,
         uint8_t format,
         uint8_t ewf_format,
         libewf_error_t **error )
{
	ewf_table_t table;
	uint8_t stored_crc_buffer[ 4 ];

	ewf_table_offset_t *offsets = NULL;
	static char *function       = "libewf_section_table2_read";
	ewf_crc_t calculated_crc    = 0;
	ewf_crc_t stored_crc        = 0;
	size_t offsets_size         = 0;
	ssize_t section_read_count  = 0;
	ssize_t read_count          = 0;
	uint64_t base_offset        = 0;
	uint32_t amount_of_chunks   = 0;
	uint8_t offsets_tainted     = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	/* Allocate the necessary amount of chunk offsets
	 * this reduces the amount of reallocations
	 */
	if( ( offset_table->amount_of_chunk_offsets < media_amount_of_chunks )
	 && ( libewf_offset_table_resize(
	       offset_table,
	       media_amount_of_chunks,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize offset table.\n",
		 function );

		return( -1 );
	}
	section_read_count = libewf_file_io_pool_read(
	                      file_io_pool,
	                      segment_file_handle->file_io_pool_entry,
	                      (uint8_t *) &table,
	                      sizeof( ewf_table_t ),
	                      error );
	
	if( section_read_count != (ssize_t) sizeof( ewf_table_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read table.\n",
		 function );

		return( -1 );
	}
	/* The table size contains the size of the CRC (4 bytes)
	 */
	calculated_crc = ewf_crc_calculate(
	                  &table,
	                  ( sizeof( ewf_table_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 table.crc );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		/* TODO error_tollerance */

		return( -1 );
	}
	endian_little_convert_32bit(
	 amount_of_chunks,
	 table.amount_of_chunks );

	endian_little_convert_64bit(
	 base_offset,
	 table.base_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 table.padding1,
	 4 );
	notify_dump_data(
	 table.padding2,
	 4 );
#endif
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: table is of size %" PRIu32 " chunks CRC %" PRIu32 " (%" PRIu32 ").\n",
	 function,
	 amount_of_chunks,
	 stored_crc,
	 calculated_crc );
#endif

	if( amount_of_chunks > 0 )
	{
		/* Check if the maximum amount of offsets is not exceeded
		 */
		if( amount_of_chunks > EWF_MAXIMUM_OFFSETS_IN_TABLE )
		{
			notify_verbose_printf(
			 "%s: table contains more offsets: %" PRIu32 " than the maximum amount: %d.\n",
			 function,
			 amount_of_chunks,
			 EWF_MAXIMUM_OFFSETS_IN_TABLE );
		}
		offsets_size = sizeof( ewf_table_offset_t ) * amount_of_chunks;

		if( offsets_size > (size_t) SSIZE_MAX )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid offsets size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		offsets = (ewf_table_offset_t *) memory_allocate(
		                                  offsets_size );

		if( offsets == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to allocated table offsets.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_file_io_pool_read(
		              file_io_pool,
		              segment_file_handle->file_io_pool_entry,
		              (uint8_t *) offsets,
		              offsets_size,
		              error );
	
		if( read_count != (ssize_t) offsets_size )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_READ_FAILED,
			 "%s: unable to read table offsets.\n",
			 function );

			memory_free(
			 offsets );

			return( -1 );
		}
		section_read_count += read_count;

		/* The EWF-S01 format does not contain a CRC after the offsets
		 */
		if( ewf_format != EWF_FORMAT_S01 )
		{
			/* Check if the offset table CRC matches
			 */
			calculated_crc = ewf_crc_calculate(
			                  offsets,
			                  offsets_size,
			                  1 );

			read_count = libewf_file_io_pool_read(
			              file_io_pool,
			              segment_file_handle->file_io_pool_entry,
			              stored_crc_buffer,
			              sizeof( ewf_crc_t ),
			              error );

			if( read_count != (ssize_t) sizeof( ewf_crc_t ) )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_IO,
				 LIBEWF_IO_ERROR_READ_FAILED,
				 "%s: unable to read CRC from file descriptor.\n",
				 function );

				memory_free(
				 offsets );

				return( -1 );
			}
			section_read_count += read_count;

			endian_little_convert_32bit(
			 stored_crc,
			 stored_crc_buffer );

			if( stored_crc != calculated_crc )
			{
				notify_verbose_printf(
				 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
				 function,
				 stored_crc,
				 calculated_crc );

				/* The offsets cannot be trusted therefore do not try to correct corrupted offsets during compare
				 */
				offsets_tainted = 1;
			}
		}
		if( libewf_offset_table_compare(
		     offset_table,
		     (off64_t) base_offset,
		     offsets,
		     amount_of_chunks,
		     segment_file_handle,
		     offsets_tainted,
		     error ) != 1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to compare offset table.\n",
			 function );

			memory_free(
			 offsets );

			return( -1 );
		}
		memory_free(
		 offsets );

		if( libewf_offset_table_compare_last_offset(
		     offset_table,
		     segment_file_handle->section_list,
		     offsets_tainted,
		     error ) != 1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to compare last offset.\n",
			 function );

			return( -1 );
		}
	}
	else
	{
		notify_verbose_printf(
		 "%s: table contains no offsets.\n",
		 function );
	}
	if( section_size < (size_t) section_read_count )
	{
		notify_verbose_printf(
		 "%s: section size: %" PRIzd " smaller than section read count: %" PRIzd ".\n",
		 function,
		 section_size,
		 section_read_count );
	}
	/* Skip the chunk data within the section
	 * for chunks after the table section
	 */
	else if( section_size != (size_t) section_read_count )
	{
		if( ( ewf_format != EWF_FORMAT_S01 )
		 && ( format != LIBEWF_FORMAT_ENCASE1 ) )
		{
			notify_verbose_printf(
			 "%s: unexpected data found after table offsets.\n",
			 function );
		}
		if( libewf_file_io_pool_seek_offset(
		     file_io_pool,
		     segment_file_handle->file_io_pool_entry,
		     section_size - section_read_count,
		     SEEK_CUR,
		     error ) == -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_SEEK_FAILED,
			 "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
		section_read_count = (ssize_t) section_size;
	}
	segment_file_handle->amount_of_chunks += amount_of_chunks;

	return( section_read_count );
}

/* Writes a table or table2 section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_table_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         off64_t base_offset,
         ewf_table_offset_t *offsets,
         uint32_t amount_of_offsets,
         uint8_t *section_type,
         size_t section_type_length,
         size_t additional_size,
         uint8_t format,
         uint8_t ewf_format,
         uint8_t no_section_append,
         libewf_error_t **error )
{
	ewf_table_t table;
	uint8_t calculated_crc_buffer[ 4 ];

	static char *function       = "libewf_section_table_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;
	size_t section_size         = 0;
	size_t offsets_size         = 0;
	uint8_t write_crc           = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file\n",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid base offset value less than zero.\n",
		 function );

		return( -1 );
	}
	if( offsets == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	offsets_size = sizeof( ewf_table_offset_t ) * amount_of_offsets;
	section_size = sizeof( ewf_table_t ) + offsets_size + additional_size;

	if( ewf_format != EWF_FORMAT_S01 )
	{
		write_crc     = 1;
		section_size += sizeof( ewf_crc_t );
	}
	if( memory_set(
	     &table,
	     0,
	     sizeof( ewf_table_t ) ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table.\n",
		 function );

		return( -1 );
	}
	endian_little_revert_32bit(
	 table.amount_of_chunks,
	 amount_of_offsets );

	endian_little_revert_64bit(
	 table.base_offset,
	 base_offset );

	calculated_crc = ewf_crc_calculate(
	                  &table,
	                  ( sizeof( ewf_table_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_revert_32bit(
	 table.crc,
	 calculated_crc );

	if( write_crc != 0 )
	{
		calculated_crc = ewf_crc_calculate(
		                  offsets,
		                  offsets_size,
		                  1 );
	}
	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       section_type,
	                       section_type_length,
	                       (size64_t) section_size,
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section: %s to file.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) &table,
	               sizeof( ewf_table_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_table_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write table to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) offsets,
	               offsets_size,
	               error );

	if( write_count != (ssize_t) offsets_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write table offsets to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( write_crc != 0 )
	{
		endian_little_revert_32bit(
		 calculated_crc_buffer,
		 calculated_crc );

		write_count = libewf_file_io_pool_write(
		               file_io_pool,
		               segment_file_handle->file_io_pool_entry,
	        	       calculated_crc_buffer,
	        	       sizeof( ewf_crc_t ),
		               error );

		if( write_count != (ssize_t) sizeof( ewf_crc_t ) )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write table offsets CRC to file.\n",
			 function );

			return( -1 );
		}
		section_write_count += write_count;
	}
	if( ( no_section_append == 0 )
	 && ( libewf_section_list_append(
	       segment_file_handle->section_list,
	       section_type,
	       section_type_length,
	       section_offset,
	       section_offset + section_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append: %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a sectors section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize64_t libewf_section_sectors_read(
           libewf_file_io_pool_t *file_io_pool,
           libewf_segment_file_handle_t *segment_file_handle,
           size64_t section_size,
           uint8_t ewf_format,
           libewf_error_t **error )
{
	static char *function = "libewf_section_sectors_read";

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size64_t) INT64_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* In the EWF-E01 format the sectors section holds the actual data chunks
	 */
	if( ewf_format == EWF_FORMAT_S01 )
	{
		notify_verbose_printf(
		 "%s: EWF-S01 format should not contain sectors section.\n",
		 function );
	}
	/* Skip the chunk data within the section
	 */
	if( libewf_file_io_pool_seek_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     section_size,
	     SEEK_CUR,
	     error ) == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_SEEK_FAILED,
		 "%s: unable to align with next section.\n",
		 function );

		return( -1 );
	}
	return( (ssize64_t) section_size );
}

/* Writes a sectors section to file
 * Does not write the actual data in the sectors section
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_sectors_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size64_t sectors_data_size,
         uint8_t no_section_append,
         libewf_error_t **error )
{
	uint8_t *section_type       = (uint8_t *) "sectors";
	static char *function       = "libewf_section_sectors_write";
	off64_t section_offset      = 0;
	size_t section_type_length  = 7;
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       section_type,
	                       section_type_length,
	                       sectors_data_size,
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section: %s to file.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	if( ( no_section_append == 0 )
	 && ( libewf_section_list_append(
	       segment_file_handle->section_list,
	       section_type,
	       section_type_length,
	       section_offset,
	       section_offset + sectors_data_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append: %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a ltree section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_ltree_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint8_t *ewf_format,
         libewf_error_t **error )
{
	ewf_ltree_t *ltree         = NULL;
	uint8_t *ltree_data        = NULL;
	static char *function      = "libewf_section_ltree_read";
	ssize_t section_read_count = 0;
	ssize_t read_count         = 0;
	size_t ltree_data_size     = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ewf format.\n",
		 function );

		return( -1 );
	}
	if( *ewf_format == EWF_FORMAT_S01 )
	{
		notify_verbose_printf(
		 "%s: EWF-S01 format should not contain ltree section.\n",
		 function );
	}
	*ewf_format = EWF_FORMAT_L01;

	ltree = (ewf_ltree_t *) memory_allocate(
	                         sizeof( ewf_ltree_t ) );

	if( ltree == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create ltree.\n",
		 function );

		return( -1 );
	}
	section_read_count = libewf_file_io_pool_read(
	                      file_io_pool,
	                      segment_file_handle->file_io_pool_entry,
	                      (uint8_t *) ltree,
	                      sizeof( ewf_ltree_t ),
	                      error );
	
	if( section_read_count != (ssize_t) sizeof( ewf_ltree_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read ltree.\n",
		 function );

		memory_free(
		 ltree );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 ltree->unknown1,
	 16 );
	notify_dump_data(
	 ltree->tree_size,
	 4 );
	notify_dump_data(
	 ltree->unknown2,
	 4 );
	notify_dump_data(
	 ltree->unknown3,
	 4 );
	notify_dump_data(
	 ltree->unknown4,
	 20 );
#endif

	memory_free(
	 ltree );

	ltree_data_size = section_size - sizeof( ewf_ltree_t );

	ltree_data = (uint8_t *) memory_allocate(
                                  sizeof( uint8_t ) * ltree_data_size );

	if( ltree_data == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create ltree data.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_file_io_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              ltree_data,
	              ltree_data_size,
	              error );

	if( read_count != (ssize_t) ltree_data_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read ltree data.\n",
		 function );

		memory_free(
		 ltree_data );

		return( -1 );
	}
	section_read_count += read_count;

#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_utf16_stream_print(
	       _LIBEWF_STRING( "ltree data" ),
	       ltree_data,
	       ltree_data_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print ltree data.\n",
		 function );

		memory_free(
		 ltree_data );

		return( -1 );
	}
#endif
	memory_free(
	 ltree_data );

	return( read_count );
}

/* Reads a session section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_session_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         libewf_sector_table_t *sessions,
         size_t section_size,
         uint8_t ewf_format,
         libewf_error_t **error )
{
	ewf_session_t ewf_session;
	uint8_t stored_crc_buffer[ 4 ];

	ewf_session_entry_t *ewf_sessions = NULL;
	static char *function             = "libewf_section_session_read";
	ewf_crc_t calculated_crc          = 0;
	ewf_crc_t stored_crc              = 0;
	ssize_t section_read_count        = 0;
	ssize_t read_count                = 0;
	size_t ewf_sessions_size          = 0;
	uint32_t amount_of_ewf_sessions   = 0;
	uint32_t iterator                 = 0;
	uint32_t first_sector             = 0;
	uint32_t last_first_sector        = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( sessions == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sessions.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == EWF_FORMAT_S01 )
	{
		notify_verbose_printf(
		 "%s: EWF-S01 format should not contain session section.\n",
		 function );
	}
	section_read_count = libewf_file_io_pool_read(
	                      file_io_pool,
	                      segment_file_handle->file_io_pool_entry,
	                      (uint8_t *) &ewf_session,
	                      sizeof( ewf_session_t ),
	                      error );

	if( section_read_count != (ssize_t) sizeof( ewf_session_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read session.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  &ewf_session,
	                  ( sizeof( ewf_session_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 ewf_session.crc );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		/* TODO error_tollerance */

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 ewf_session.unknown1,
	 28 );
#endif

	endian_little_convert_32bit(
	 amount_of_ewf_sessions,
	 ewf_session.amount_of_sessions );

	if( amount_of_ewf_sessions > 0 )
	{
		ewf_sessions_size = sizeof( ewf_session_entry_t ) * amount_of_ewf_sessions;

		ewf_sessions = (ewf_session_entry_t *) memory_allocate(
		                                        ewf_sessions_size );

		if( ewf_sessions == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create session data.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_file_io_pool_read(
		              file_io_pool,
		              segment_file_handle->file_io_pool_entry,
		              (uint8_t *) ewf_sessions,
		              ewf_sessions_size,
		              error );
	
		if( read_count != (ssize_t) ewf_sessions_size )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_READ_FAILED,
			 "%s: unable to read session data.\n",
			 function );

			memory_free(
			 ewf_sessions );

			return( -1 );
		}
		section_read_count += read_count;

		calculated_crc = ewf_crc_calculate(
		                  ewf_sessions,
		                  ewf_sessions_size,
		                  1 );

		read_count = libewf_file_io_pool_read(
		              file_io_pool,
		              segment_file_handle->file_io_pool_entry,
		              stored_crc_buffer,
		              sizeof( ewf_crc_t ),
		              error );

		if( read_count != (ssize_t) sizeof( ewf_crc_t ) )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_READ_FAILED,
			 "%s: unable to read CRC from file descriptor.\n",
			 function );

			memory_free(
			 ewf_sessions );

			return( -1 );
		}
		section_read_count += read_count;

		endian_little_convert_32bit(
		 stored_crc,
		 stored_crc_buffer );

		if( stored_crc != calculated_crc )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_INPUT,
			 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
			 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
			 function,
			 stored_crc,
			 calculated_crc );

			/* TODO error_tollerance */

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		notify_dump_data(
		 ewf_sessions,
		 ewf_sessions_size );
#endif
		if( sessions->sector != NULL )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf(
			 "%s: session entries already set in handle - removing previous one.\n",
			 function );
#endif

			memory_free(
			 sessions->sector );

			sessions->amount = 0;
		}
		sessions->sector = (libewf_sector_table_entry_t *) memory_allocate(
		                                                    sizeof( libewf_sector_table_entry_t ) * amount_of_ewf_sessions );

		if( sessions->sector == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create session entries.\n",
			 function );

			memory_free(
			 ewf_sessions );

			return( -1 );
		}
		sessions->amount = amount_of_ewf_sessions;

		for( iterator = 0; iterator < amount_of_ewf_sessions; iterator++ )
		{
			endian_little_convert_32bit(
			 first_sector,
			 ewf_sessions[ iterator ].first_sector );

			sessions->sector[ iterator ].first_sector = (uint64_t) first_sector;

			if( iterator > 0 )
			{
				sessions->sector[ iterator - 1 ].amount_of_sectors = first_sector - last_first_sector;
			}
			last_first_sector = first_sector;
		}
		sessions->sector[ iterator - 1 ].amount_of_sectors = media_values->amount_of_sectors - last_first_sector;

		memory_free(
		 ewf_sessions );
	}
	else
	{
		notify_verbose_printf(
		 "%s: session contains no session data.\n",
		 function );
	}
	return( section_read_count );
}

/* Writes a session section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_session_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_sector_table_t *sessions,
         libewf_error_t **error )
{
	ewf_session_t ewf_session;
	uint8_t calculated_crc_buffer[ 4 ];

	ewf_session_entry_t *ewf_sessions = NULL;
	uint8_t *section_type             = (uint8_t *) "session";
	static char *function             = "libewf_section_session_write";
	ewf_crc_t calculated_crc          = 0;
	off64_t section_offset            = 0;
	ssize_t section_write_count       = 0;
	ssize_t write_count               = 0;
	size_t section_type_length        = 7;
	size_t section_size               = 0;
	size_t ewf_sessions_size          = 0;
	uint32_t iterator                 = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file\n",
		 function );

		return( -1 );
	}
	if( sessions == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sessions.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	ewf_sessions_size = sizeof( ewf_session_entry_t ) * sessions->amount;
	section_size      = sizeof( ewf_session_t ) + ewf_sessions_size + sizeof( ewf_crc_t );

	if( memory_set(
	     &ewf_session,
	     0,
	     sizeof( ewf_session_t ) ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear session.\n",
		 function );

		return( -1 );
	}
	endian_little_revert_32bit(
	 ewf_session.amount_of_sessions,
	 sessions->amount );

	calculated_crc = ewf_crc_calculate(
	                  &ewf_session,
	                  ( sizeof( ewf_session_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_revert_32bit(
	 ewf_session.crc,
	 calculated_crc );

	ewf_sessions = (ewf_session_entry_t *) memory_allocate(
	                                        ewf_sessions_size );

	if( ewf_sessions == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create session entries.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < sessions->amount; iterator++ )
	{
		endian_little_revert_32bit(
		 ewf_sessions[ iterator ].first_sector,
		 (uint32_t) sessions->sector[ iterator ].first_sector );
	}
	calculated_crc = ewf_crc_calculate(
	                  ewf_sessions,
	                  ewf_sessions_size,
	                  1 );

	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       section_type,
	                       section_type_length,
	                       (size64_t) section_size,
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section: %s to file.\n",
		 function,
		 (char *) section_type );

		memory_free(
		 ewf_sessions );

		return( -1 );
	}
	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) &ewf_session,
	               sizeof( ewf_session_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_session_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write session to file.\n",
		 function );

		memory_free(
		 ewf_sessions );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) ewf_sessions,
	               ewf_sessions_size,
	               error );

	memory_free(
	 ewf_sessions );

	if( write_count != (ssize_t) ewf_sessions_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write session entries to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	endian_little_revert_32bit(
	 calculated_crc_buffer,
	 calculated_crc );

	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
 	               calculated_crc_buffer,
 	               sizeof( ewf_crc_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_crc_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write session entries CRC to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section_type,
	     section_type_length,
	     section_offset,
	     section_offset + section_write_count,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a data section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_data_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         libewf_media_values_t *media_values,
         uint8_t ewf_format,
         libewf_error_t **error )
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
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == EWF_FORMAT_S01 )
	{
		notify_verbose_printf(
		 "%s: EWF-S01 format should not contain data section.\n",
		 function );
	}
	if( section_size != sizeof( ewf_data_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in section data size.\n",
		 function );

		return( -1 );
	}
	data = (ewf_data_t *) memory_allocate(
	                       sizeof( ewf_data_t ) );

	if( data == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_file_io_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              (uint8_t *) data,
	              sizeof( ewf_data_t ),
	              error );
	
	if( read_count != (ssize_t) sizeof( ewf_data_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read data.\n",
		 function );

		memory_free(
		 data );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  data,
	                  ( sizeof( ewf_data_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 data->crc );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		/* TODO error_tollerance */

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 data->unknown1,
	 3 );
	notify_dump_data(
	 data->unknown2,
	 16 );
	notify_dump_data(
	 data->unknown3,
	 3 );
	notify_dump_data(
	 data->unknown4,
	 12 );
	notify_dump_data(
	 data->unknown5,
	 3 );
	notify_dump_data(
	 data->unknown6,
	 4 );
	notify_dump_data(
	 data->unknown7,
	 963 );
	notify_dump_data(
	 data->signature,
	 5 );
#endif
	/* TODO add more checks
	 */
	if( media_values->media_type != data->media_type )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: media type does not match in data section.\n",
		 function );

		memory_free(
		 data );

		/* TODO error_tollerance */

		return( -1 );
	}
	endian_little_convert_32bit(
	 amount_of_chunks,
	 data->amount_of_chunks );

	if( media_values->amount_of_chunks != amount_of_chunks )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: amount of chunks: %" PRIu32 " does not match amount in data section: %" PRIu32 ".\n",
		 function,
		 media_values->amount_of_chunks,
		 amount_of_chunks );

		memory_free(
		 data );

		/* TODO error_tollerance */

		return( -1 );
	}
	endian_little_convert_32bit(
	 sectors_per_chunk,
	 data->sectors_per_chunk );

	if( media_values->sectors_per_chunk != sectors_per_chunk )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: sectors per chunk does not match in data section.\n",
		 function );

		memory_free(
		 data );

		/* TODO error_tollerance */

		return( -1 );
	}
	endian_little_convert_32bit(
	 bytes_per_sector,
	 data->bytes_per_sector );

	if( media_values->bytes_per_sector != bytes_per_sector )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: bytes per sector does not match in data section.\n",
		 function );

		memory_free(
		 data );

		/* TODO error_tollerance */

		return( -1 );
	}
	endian_little_convert_32bit(
	 amount_of_sectors,
	 data->amount_of_sectors );

	if( media_values->amount_of_sectors != amount_of_sectors )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: amount of sectors does not match in data section.\n",
		 function );

		memory_free(
		 data );

		/* TODO error_tollerance */

		return( -1 );
	}
	endian_little_convert_32bit(
	 error_granularity,
	 data->error_granularity );

	if( media_values->error_granularity != error_granularity )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: error granularity does not match in data section.\n",
		 function );

		memory_free(
		 data );

		/* TODO error_tollerance */

		return( -1 );
	}
	if( media_values->media_flags != data->media_flags )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: media flags do not match in data section.\n",
		 function );

		memory_free(
		 data );

		/* TODO error_tollerance */

		return( -1 );
	}
	if( memory_compare(
	     media_values->guid,
	     data->guid,
	     16 ) != 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: GUID does not match in data section.\n",
		 function );

		memory_free(
		 data );

		/* TODO error_tollerance */

		return( -1 );
	}
	memory_free(
	 data );

	return( read_count );
}

/* Writes a data section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_data_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t format,
         ewf_data_t **cached_data_section,
         uint8_t no_section_append,
         libewf_error_t **error )
{
	uint8_t *section_type       = (uint8_t *) "data";
	static char *function       = "libewf_section_data_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	size_t section_type_length  = 4;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( cached_data_section == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid caches data section.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	/* Check if the data section was already created
	 */
	if( *cached_data_section == NULL )
	{
		*cached_data_section = (ewf_data_t *) memory_allocate(
		                                       sizeof( ewf_data_t ) );

		if( *cached_data_section == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *cached_data_section,
		     0,
		     sizeof( ewf_data_t ) ) == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data.\n",
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

		endian_little_revert_32bit(
		 ( *cached_data_section )->amount_of_chunks,
		 media_values->amount_of_chunks );

		endian_little_revert_32bit(
		 ( *cached_data_section )->sectors_per_chunk,
		 media_values->sectors_per_chunk );

		endian_little_revert_32bit(
		 ( *cached_data_section )->bytes_per_sector,
		 media_values->bytes_per_sector );

		endian_little_revert_32bit(
		 ( *cached_data_section )->amount_of_sectors,
		 media_values->amount_of_sectors );

		if( ( format == LIBEWF_FORMAT_ENCASE5 )
		 || ( format == LIBEWF_FORMAT_ENCASE6 )
		 || ( format == LIBEWF_FORMAT_LINEN5 )
		 || ( format == LIBEWF_FORMAT_LINEN6 )
		 || ( format == LIBEWF_FORMAT_EWFX ) )
		{
			endian_little_revert_32bit(
			 ( *cached_data_section )->error_granularity,
			 media_values->error_granularity );

			( *cached_data_section )->compression_level = (uint8_t) compression_level;

			if( memory_copy(
			     ( *cached_data_section )->guid,
			     media_values->guid,
			     16 ) == NULL )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_MEMORY,
				 LIBEWF_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to set GUID.\n",
				 function );

				return( -1 );
			}
		}
		calculated_crc = ewf_crc_calculate(
		                  *cached_data_section,
		                  ( sizeof( ewf_data_t ) - sizeof( ewf_crc_t ) ),
		                  1 );

		endian_little_revert_32bit(
		 ( *cached_data_section )->crc,
		 calculated_crc );
	}
	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       section_type,
	                       section_type_length,
	                       (size64_t) sizeof( ewf_data_t ),
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section: %s to file.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) *cached_data_section,
	               sizeof( ewf_data_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_data_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write data to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( ( no_section_append == 0 )
	 && ( libewf_section_list_append(
	       segment_file_handle->section_list,
	       section_type,
	       section_type_length,
	       section_offset,
	       section_offset + section_write_count,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append: %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a error2 section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_error2_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_sector_table_t *acquiry_errors,
         size_t section_size,
         uint8_t ewf_format,
         libewf_error_t **error )
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
	uint32_t first_sector               = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid acquiry errors.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == EWF_FORMAT_S01 )
	{
		notify_verbose_printf(
		 "%s: EWF-S01 format should not contain error2 section.\n",
		 function );
	}
	section_read_count = libewf_file_io_pool_read(
	                      file_io_pool,
	                      segment_file_handle->file_io_pool_entry,
	                      (uint8_t *) &error2,
	                      sizeof( ewf_error2_t ),
	                      error );
	
	if( section_read_count != (ssize_t) sizeof( ewf_error2_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read error2.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  &error2,
	                  ( sizeof( ewf_error2_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 error2.crc );

	endian_little_convert_32bit(
	 amount_of_errors,
	 error2.amount_of_errors );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		/* TODO error_tollerance */

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 error2.unknown,
	 200 );
#endif

	if( amount_of_errors > 0 )
	{
		sectors_size = sizeof( ewf_error2_sector_t ) * amount_of_errors;

		error2_sectors = (ewf_error2_sector_t *) memory_allocate(
		                                          sectors_size );

		if( error2_sectors == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create error2 sectors.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_file_io_pool_read(
		              file_io_pool,
		              segment_file_handle->file_io_pool_entry,
		              (uint8_t *) error2_sectors,
		              sectors_size,
	                      error );
	
		if( read_count != (ssize_t) sectors_size )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_READ_FAILED,
			 "%s: unable to read error2 sectors.\n",
			 function );

			memory_free(
			 error2_sectors );

			return( -1 );
		}
		section_read_count += read_count;

		calculated_crc = ewf_crc_calculate(
		                  error2_sectors,
		                  sectors_size,
		                  1 );

		read_count = libewf_file_io_pool_read(
		              file_io_pool,
		              segment_file_handle->file_io_pool_entry,
		              stored_crc_buffer,
		              sizeof( ewf_crc_t ),
	                      error );

		if( read_count != (ssize_t) sizeof( ewf_crc_t ) )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_READ_FAILED,
			 "%s: unable to read CRC from file descriptor.\n",
			 function );

			memory_free(
			 error2_sectors );

			return( -1 );
		}
		section_read_count += read_count;

		endian_little_convert_32bit(
		 stored_crc,
		 stored_crc_buffer );

		if( stored_crc != calculated_crc )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_INPUT,
			 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
			 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
			 function,
			 stored_crc,
			 calculated_crc );

			memory_free(
			 error2_sectors );

			/* TODO error_tollerance */

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		notify_dump_data(
		 error2_sectors,
		 sectors_size );
#endif
		if( acquiry_errors->sector != NULL )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf(
			 "%s: acquiry error sectors already set in handle - removing previous one.\n",
			 function );
#endif

			memory_free(
			 acquiry_errors->sector );

			acquiry_errors->amount = 0;
		}
		acquiry_errors->sector = (libewf_sector_table_entry_t *) memory_allocate(
		                                                          sizeof( libewf_sector_table_entry_t ) * amount_of_errors );

		if( acquiry_errors->sector == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create acquiry error sectors.\n",
			 function );

			memory_free(
			 error2_sectors );

			return( -1 );
		}
		acquiry_errors->amount = amount_of_errors;

		for( iterator = 0; iterator < amount_of_errors; iterator++ )
		{
			endian_little_convert_32bit(
			 first_sector,
			 error2_sectors[ iterator ].first_sector );

			acquiry_errors->sector[ iterator ].first_sector = (uint64_t) first_sector;

			endian_little_convert_32bit(
			 acquiry_errors->sector[ iterator ].amount_of_sectors,
			 error2_sectors[ iterator ].amount_of_sectors );
		}
		memory_free(
		 error2_sectors );
	}
	else
	{
		notify_verbose_printf(
		 "%s: error2 contains no sectors!.\n",
		 function );
	}
	return( section_read_count );
}

/* Writes a error2 section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_error2_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_sector_table_t *acquiry_errors,
         libewf_error_t **error )
{
	ewf_error2_t error2;
	uint8_t calculated_crc_buffer[ 4 ];

	ewf_error2_sector_t *error2_sectors = NULL;
	uint8_t *section_type               = (uint8_t *) "error2";
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
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file\n",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid acquiry errors.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	sectors_size = sizeof( ewf_error2_sector_t ) * acquiry_errors->amount;
	section_size = sizeof( ewf_error2_t ) + sectors_size + sizeof( ewf_crc_t );

	if( memory_set(
	     &error2,
	     0,
	     sizeof( ewf_error2_t ) ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear error2.\n",
		 function );

		return( -1 );
	}
	endian_little_revert_32bit(
	 error2.amount_of_errors,
	 acquiry_errors->amount );

	calculated_crc = ewf_crc_calculate(
	                  &error2,
	                  ( sizeof( ewf_error2_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_revert_32bit(
	 error2.crc,
	 calculated_crc );

	error2_sectors = (ewf_error2_sector_t *) memory_allocate(
	                                          sectors_size );

	if( error2_sectors == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create error2 sectors.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < acquiry_errors->amount; iterator++ )
	{
		endian_little_revert_32bit(
		 error2_sectors[ iterator ].first_sector,
		 (uint32_t) acquiry_errors->sector[ iterator ].first_sector );

		endian_little_revert_32bit(
		 error2_sectors[ iterator ].amount_of_sectors,
		 acquiry_errors->sector[ iterator ].amount_of_sectors );
	}
	calculated_crc = ewf_crc_calculate(
	                  error2_sectors,
	                  sectors_size,
	                  1 );

	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       section_type,
	                       section_type_length,
	                       (size64_t) section_size,
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section: %s to file.\n",
		 function,
		 (char *) section_type );

		memory_free(
		 error2_sectors );

		return( -1 );
	}
	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) &error2,
	               sizeof( ewf_error2_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_error2_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write error2 to file.\n",
		 function );

		memory_free(
		 error2_sectors );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) error2_sectors,
	               sectors_size,
	               error );

	memory_free(
	 error2_sectors );

	if( write_count != (ssize_t) sectors_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write error2 sectors to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	endian_little_revert_32bit(
	 calculated_crc_buffer,
	 calculated_crc );

	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
 	               calculated_crc_buffer,
 	               sizeof( ewf_crc_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_crc_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write error2 sectors CRC to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section_type,
	     section_type_length,
	     section_offset,
	     section_offset + section_write_count,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a hash section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_hash_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_digest_hash_t *md5_hash,
         libewf_error_t **error )
{
	ewf_hash_t hash;

	static char *function    = "libewf_section_hash_read";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	ssize_t read_count       = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid md5 hash.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_file_io_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              (uint8_t *) &hash,
	              sizeof( ewf_hash_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_hash_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read hash.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  &hash,
	                  ( sizeof( ewf_hash_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 hash.crc );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		/* TODO error_tollerance */

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	notify_dump_data(
	 hash.unknown1,
	 16 );
#endif

	if( memory_copy(
	     md5_hash,
	     hash.md5_hash,
	     EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set MD5 hash in handle.\n",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes a hash section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_hash_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_digest_hash_t *md5_hash,
         libewf_error_t **error )
{
	ewf_hash_t hash;

	uint8_t *section_type       = (uint8_t *) "hash";
	static char *function       = "libewf_section_hash_write";
	ewf_crc_t calculated_crc    = 0;
	off64_t section_offset      = 0;
	size_t section_type_length  = 4;
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &hash,
	     0,
	     sizeof( ewf_hash_t ) ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear hash.\n",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     hash.md5_hash,
	     md5_hash,
	     EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set hash.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  &hash,
	                  ( sizeof( ewf_hash_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_revert_32bit(
	 hash.crc,
	 calculated_crc );

	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       section_type,
	                       section_type_length,
	                       (size64_t) sizeof( ewf_hash_t ),
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section: %s to file.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) &hash,
	               sizeof( ewf_hash_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_hash_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write hash to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section_type,
	     section_type_length,
	     section_offset,
	     section_offset + section_write_count,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Writes the last section start to file
 * This is used for the next and done sections,
 these sections point back towards themselves
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_last_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         uint8_t *section_type,
         size_t section_type_length,
         uint8_t format,
         uint8_t ewf_format,
         libewf_error_t **error )
{
	ewf_section_t section;

	static char *function       = "libewf_section_last_write";
	ewf_crc_t calculated_crc    = 0;
	ssize_t section_write_count = 0;
	uint64_t section_size       = 0;
	uint64_t section_offset     = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file\n",
		 function );

		return( -1 );
	}
	if( section_type == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section type.\n",
		 function );

		return( -1 );
	}
	if( ( section_type_length == 0 )
	 || ( section_type_length >= 16 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: section type length value out of range.\n",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &section,
	     0,
	     sizeof( ewf_section_t ) ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear section.\n",
		 function );

		return( -1 );
	}
	/* The EnCase (EWF-E01) format leaves the size of this section empty
	 */
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( format == LIBEWF_FORMAT_FTK ) )
	{
		section_size = (uint64_t) sizeof( ewf_section_t );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     (off64_t *) &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     section.type,
	     section_type,
	     section_type_length ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set section type.\n",
		 function );

		return( -1 );
	}
	endian_little_revert_64bit(
	 section.size,
	 section_size );

	endian_little_revert_64bit(
	 section.next,
	 section_offset );

	calculated_crc = ewf_crc_calculate(
	                  &section,
	                  ( sizeof( ewf_section_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_revert_32bit(	
	 section.crc,
	 calculated_crc );

	section_write_count = libewf_file_io_pool_write(
	                       file_io_pool,
	                       segment_file_handle->file_io_pool_entry,
	                       (uint8_t *) &section,
	                       sizeof( ewf_section_t ),
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section: %s to file.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section_type,
	     section_type_length,
	     section_offset,
	     section_offset + section_write_count,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a xheader section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_xheader_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint8_t **cached_xheader,
         size_t *cached_xheader_size,
         libewf_error_t **error )
{
	uint8_t *xheader      = NULL;
	static char *function = "libewf_section_xheader_read";
	ssize_t read_count    = 0;
	size_t xheader_size   = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( cached_xheader == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached xheader.\n",
		 function );

		return( -1 );
	}
	if( cached_xheader_size == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached xheader size.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              file_io_pool,
	              segment_file_handle,
	              section_size,
	              &xheader,
	              &xheader_size,
	              error );

	if( read_count != (ssize_t) section_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read xheader.\n",
		 function );

		return( -1 );
	}
	if( xheader == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid xheader.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_utf8_stream_print(
	       _LIBEWF_STRING( "XHeader" ),
	       xheader,
	       xheader_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print xheader.\n",
		 function );

		memory_free(
		 xheader );

		return( -1 );
	}
#endif
	if( *cached_xheader == NULL )
	{
		*cached_xheader      = xheader;
		*cached_xheader_size = xheader_size;
	}
	else
	{
		memory_free(
		 xheader );
	}
	return( read_count );
}

/* Writes a xheader section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_xheader_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         uint8_t *xheader,
         size_t xheader_size,
         int8_t compression_level,
         libewf_error_t **error )
{
	static char *function       = "libewf_section_xheader_write";
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_utf8_stream_print(
	       _LIBEWF_STRING( "XHeader" ),
	       xheader,
	       xheader_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print xheader.\n",
		 function );

		return( -1 );
	}
#endif
	section_write_count = libewf_section_write_compressed_string(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       (uint8_t *) "xheader",
	                       7,
	                       xheader,
	                       xheader_size,
	                       compression_level,
	                       error );

	if( section_write_count == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write xheader to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a xhash section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_xhash_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint8_t **cached_xhash,
         size_t *cached_xhash_size,
         libewf_error_t **error )
{
	uint8_t *xhash        = NULL;
	static char *function = "libewf_section_xhash_read";
	ssize_t read_count    = 0;
	size_t xhash_size     = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( cached_xhash == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached xhash.\n",
		 function );

		return( -1 );
	}
	if( cached_xhash_size == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached xhash size.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              file_io_pool,
	              segment_file_handle,
	              section_size,
	              &xhash,
	              &xhash_size,
	              error );

	if( read_count != (ssize_t) section_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read xheader.\n",
		 function );

		return( -1 );
	}
	if( xhash == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid xhash.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_utf8_stream_print(
	       _LIBEWF_STRING( "XHash" ),
	       xhash,
	       xhash_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print xhash.\n",
		 function );

		memory_free(
		 xhash );

		return( -1 );
	}
#endif
	if( *cached_xhash == NULL )
	{
		*cached_xhash      = xhash;
		*cached_xhash_size = xhash_size;
	}
	else
	{
		memory_free(
		 xhash );
	}
	return( read_count );
}

/* Writes a xhash section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_xhash_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         uint8_t *xhash,
         size_t xhash_size,
         int8_t compression_level,
         libewf_error_t **error )
{
	static char *function       = "libewf_section_xhash_write";
	off64_t section_offset      = 0;
	ssize_t section_write_count = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( libewf_notify_verbose != 0 )
	 && ( libewf_debug_utf8_stream_print(
	       _LIBEWF_STRING( "XHash" ),
	       xhash,
	       xhash_size,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print xhash.\n",
		 function );

		return( -1 );
	}
#endif
	section_write_count = libewf_section_write_compressed_string(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       (uint8_t *) "xhash",
	                       5,
	                       xhash,
	                       xhash_size,
	                       compression_level,
	                       error );

	if( section_write_count == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write xhash to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count );
}

/* Reads a delta chunk section from file
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_delta_chunk_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         libewf_offset_table_t *offset_table,
         libewf_error_t **error )
{
	ewfx_delta_chunk_header_t delta_chunk_header;

	static char *function    = "libewf_section_delta_chunk_read";
	ewf_crc_t calculated_crc = 0;
	ewf_crc_t stored_crc     = 0;
	ssize_t read_count       = 0;
	uint32_t chunk           = 0;
	uint32_t chunk_size      = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size_t) INT32_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_file_io_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              (uint8_t *) &delta_chunk_header,
	              sizeof( ewfx_delta_chunk_header_t ),
	              error );

	if( read_count == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read delta chunk header.\n",
		 function );

		return( -1 );
	}
	calculated_crc = ewf_crc_calculate(
	                  &delta_chunk_header,
	                  ( sizeof( ewfx_delta_chunk_header_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_convert_32bit(
	 stored_crc,
	 delta_chunk_header.crc );

	if( stored_crc != calculated_crc )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_INPUT,
		 LIBEWF_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function,
		 stored_crc,
		 calculated_crc );

		/* TODO error_tollerance */

		return( -1 );
	}
	/* The chunk value is stored + 1 count in the file
	 */
	endian_little_convert_32bit(
	 chunk,
	 delta_chunk_header.chunk );

	chunk -= 1;

	if( chunk >= offset_table->amount_of_chunk_offsets )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid delta chunk: %" PRIu32 " value out of range.\n",
		 function,
		 chunk );

		return( -1 );
	}
	endian_little_convert_32bit(
	 chunk_size,
	 delta_chunk_header.chunk_size );

	if( chunk_size != ( section_size - sizeof( ewfx_delta_chunk_header_t ) ) )
	{
		notify_verbose_printf(
		 "%s: chunk size: %" PRIu32 " does not match size of data in section correcting in: %" PRIzd ".\n",
		 function,
		 chunk_size,
		 section_size - sizeof( ewfx_delta_chunk_header_t ) );

		chunk_size = (uint32_t) ( section_size - sizeof( ewfx_delta_chunk_header_t ) );
	}
	/* Update the chunk data in the offset table
	 */
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &( offset_table->chunk_offset[ chunk ].file_offset ),
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	offset_table->chunk_offset[ chunk ].segment_file_handle = segment_file_handle;
	offset_table->chunk_offset[ chunk ].size                = chunk_size;
	offset_table->chunk_offset[ chunk ].flags               = 0;

	/* Skip the chunk data within the section
	 */
	if( libewf_file_io_pool_seek_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     section_size - read_count,
	     SEEK_CUR,
	     error ) == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_SEEK_FAILED,
		 "%s: unable to align with next section.\n",
		 function );

		return( -1 );
	}
	return( (ssize_t) section_size );
}

/* Writes a delta chunk section to file
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_section_delta_chunk_write(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         uint32_t chunk,
         uint8_t *chunk_data,
         size_t chunk_size,
         ewf_crc_t *chunk_crc,
         uint8_t write_crc,
         uint8_t no_section_append,
         libewf_error_t **error )
{
	ewfx_delta_chunk_header_t delta_chunk_header;
	uint8_t calculated_crc_buffer[ 4 ];

	uint8_t *section_type       = (uint8_t *) "delta_chunk";
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
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( chunk_size > (size_t) INT32_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_get_offset(
	     file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &section_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &delta_chunk_header,
	     0,
	     sizeof( ewfx_delta_chunk_header_t ) ) == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear delta chunk header.\n",
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
	endian_little_revert_32bit(
	 delta_chunk_header.chunk,
	 ( chunk + 1 ) );

	endian_little_revert_32bit(
	 delta_chunk_header.chunk_size,
	 (uint32_t) chunk_data_size );

	delta_chunk_header.padding[ 0 ] = (uint8_t) 'D';
	delta_chunk_header.padding[ 1 ] = (uint8_t) 'E';
	delta_chunk_header.padding[ 2 ] = (uint8_t) 'L';
	delta_chunk_header.padding[ 3 ] = (uint8_t) 'T';
	delta_chunk_header.padding[ 4 ] = (uint8_t) 'A';

	calculated_crc = ewf_crc_calculate(
	                  &delta_chunk_header,
	                  ( sizeof( ewfx_delta_chunk_header_t ) - sizeof( ewf_crc_t ) ),
	                  1 );

	endian_little_revert_32bit(
	 delta_chunk_header.crc,
	 calculated_crc );

	section_size = sizeof( ewfx_delta_chunk_header_t ) + chunk_data_size;

	section_write_count = libewf_section_start_write(
	                       file_io_pool,
	                       segment_file_handle,
	                       section_offset,
	                       section_type,
	                       section_type_length,
	                       (size64_t) section_size,
	                       error );

	if( section_write_count != (ssize_t) sizeof( ewf_section_t ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section: %s to file.\n",
		 function );

		return( -1 );
	}
	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               (uint8_t *) &delta_chunk_header,
	               sizeof( ewfx_delta_chunk_header_t ),
	               error );

	if( write_count == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk value to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = libewf_file_io_pool_write(
	               file_io_pool,
	               segment_file_handle->file_io_pool_entry,
	               chunk_data,
	               chunk_size,
	               error );

	if( write_count == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	if( write_crc != 0 )
	{
		endian_little_revert_32bit(
		 calculated_crc_buffer,
		 *chunk_crc );

		write_count = libewf_file_io_pool_write(
		               file_io_pool,
		               segment_file_handle->file_io_pool_entry,
			       calculated_crc_buffer,
			       sizeof( ewf_crc_t ),
		               error );

		if( write_count != (size_t) sizeof( ewf_crc_t ) )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write CRC to file.\n",
			 function );

			return( -1 );
		}
		section_write_count += write_count;
	}
	if( ( no_section_append == 0 )
	 && ( libewf_section_list_append(
	       segment_file_handle->section_list,
	       section_type,
	       section_type_length,
	       section_offset,
	       section_offset + section_write_count,
	       error ) != 1 ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append: %s section to section list.\n",
		 function,
		 (char *) section_type );

		return( -1 );
	}
	return( section_write_count );
}

#if defined( HAVE_DEBUG_OUTPUT )

/* Reads a section from file for debugging purposes
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_section_debug_read(
         libewf_file_io_pool_t *file_io_pool,
         libewf_segment_file_handle_t *segment_file_handle,
         size64_t section_size,
         libewf_error_t **error )
{
	uint8_t *data              = NULL;
	uint8_t *uncompressed_data = NULL;
	static char *function      = "libewf_section_debug_read";
	ssize_t read_count         = 0;
	size_t uncompressed_size   = 0;
	int result                 = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( section_size > (size64_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	uncompressed_size = (size_t) ( section_size * 2 );

	if( uncompressed_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: uncompressed size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	data = (uint8_t *) memory_allocate(
	                    (size_t) section_size );

	if( data == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_file_io_pool_read(
	              file_io_pool,
	              segment_file_handle->file_io_pool_entry,
	              data,
	              section_size,
	              error );

	if( read_count != (ssize_t) section_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.\n",
		 function );

		memory_free(
		 data );

		return( -1 );
	}
	uncompressed_data = (uint8_t *) memory_allocate(
	                                 uncompressed_size );

	if( uncompressed_data == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed data.\n",
		 function );

		memory_free(
		 data );

		return( -1 );
	}
	result = libewf_uncompress(
	          uncompressed_data,
	          &uncompressed_size,
	          data,
	          (size_t) section_size,
	          error );

	if( result == 0 )
	{
		result = libewf_debug_dump_data(
		          _LIBEWF_STRING( "COMPRESSED data" ),
		          data,
		          (size_t) section_size,
	                  error );
	}
	else if( result == 1 )
	{
		result = libewf_debug_dump_data(
		          _LIBEWF_STRING( "UNCOMPRESSED data" ),
		          uncompressed_data,
		          uncompressed_size,
	                  error );
	}
	memory_free(
	 data );
	memory_free(
	 uncompressed_data );

	if( result == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to debug print data.\n",
		 function );

		return( -1 );
	}
	return( read_count );
}

#endif

/* Reads and processes a section
 * The section start offset will be updated
 * Returns 1 if successful or -1 on error
 */
int libewf_section_read(
     libewf_file_io_pool_t *file_io_pool,
     libewf_segment_file_handle_t *segment_file_handle,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     int8_t *compression_level,
     uint8_t *format,
     uint8_t *ewf_format,
     size64_t *segment_file_size,
     ewf_section_t *section,
     off64_t *section_start_offset,
     libewf_error_t **error )
{
	static char *function      = "libewf_section_read";
	off64_t section_end_offset = 0;
	ssize64_t read_count       = 0;
	uint64_t section_size      = 0;
	uint64_t section_next      = 0;
	size_t section_type_length = 0;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.\n",
		 function );

		return( -1 );
	}
	if( header_sections == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.\n",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid acquiry errors.\n",
		 function );

		return( -1 );
	}
	if( section == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section start.\n",
		 function );

		return( -1 );
	}
	if( section_start_offset == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section start offset.\n",
		 function );

		return( -1 );
	}
	if( compression_level == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression level.\n",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ewf format.\n",
		 function );

		return( -1 );
	}
	if( *section_start_offset > (off64_t) INT64_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section start offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_section_start_read(
	     file_io_pool,
	     segment_file_handle,
	     *section_start_offset,
	     section,
	     &section_size,
	     &section_next,
	     error ) == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read section start.\n",
		 function );

		return( -1 );
	}
	section_end_offset = *section_start_offset + (off64_t) section_size;

	if( section_end_offset > (off64_t) INT64_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section end offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	section_type_length = narrow_string_length(
	                       (char *) section->type );

	if( libewf_section_list_append(
	     segment_file_handle->section_list,
	     section->type,
	     section_type_length,
	     *section_start_offset,
	     section_end_offset,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append value to section list.\n",
		 function );

		return( -1 );
	}
	*section_start_offset += sizeof( ewf_section_t );

	/* No need to correct empty sections like done and next
	 */
	if( section_size > 0 )
	{
		section_size -= sizeof( ewf_section_t );
	}
	if( section_size > (uint64_t) INT64_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.\n",
		 function );

		return( -1 );
	}

	/* Nothing to do for the next and done section
	 * The \0 byte is included in the compare
	 */
	if( ( memory_compare(
	       section->type,
	       "next",
	       5 ) == 0 )
	 || ( memory_compare(
	       section->type,
	       "done",
	       5 ) == 0 ) )
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
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "header2",
	          8 ) == 0 )
	{
		read_count = libewf_section_header2_read(
		              file_io_pool,
		              segment_file_handle,
		              (size_t) section_size,
		              &( header_sections->header2 ),
		              &( header_sections->header2_size ),
		              error );

		header_sections->amount_of_header_sections++;
	}
	/* Read the header section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "header",
	          7 ) == 0 )
	{
		read_count = libewf_section_header_read(
		              file_io_pool,
		              segment_file_handle,
		              (size_t) section_size,
		              &( header_sections->header ),
		              &( header_sections->header_size ),
		              error );

		header_sections->amount_of_header_sections++;
	}
	/* Read the xheader section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "xheader",
	          8 ) == 0 )
	{
		read_count = libewf_section_xheader_read(
		              file_io_pool,
		              segment_file_handle,
		              (size_t) section_size,
		              &( header_sections->xheader ),
		              &( header_sections->xheader_size ),
		              error );

		header_sections->amount_of_header_sections++;
	}
	/* Read the volume or disk section
	 * The \0 byte is included in the compare
	 */
	else if( ( memory_compare(
	            (void *) section->type,
	            (void *) "volume",
	            7 ) == 0 )
	      || ( memory_compare(
	            (void *) section->type,
	            (void *) "disk",
	            5 ) == 0 ) )
	{
		read_count = libewf_section_volume_read(
		              file_io_pool,
		              segment_file_handle,
		              (size_t) section_size,
		              media_values,
		              compression_level,
		              format,
		              ewf_format,
		              error );

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
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "table2",
	          7 ) == 0 )
	{
		read_count = libewf_section_table2_read(
		              file_io_pool,
		              segment_file_handle,
		              (size_t) section_size,
		              media_values->amount_of_chunks,
		              offset_table,
		              *format,
		              *ewf_format,
		              error );
	}
	/* Read the table section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "table",
	          6 ) == 0 )
	{
		read_count = libewf_section_table_read(
		              file_io_pool,
		              segment_file_handle,
		              (size_t) section_size,
		              media_values->amount_of_chunks,
		              offset_table,
		              *format,
		              *ewf_format,
		              error );
	}
	/* Read the sectors section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "sectors",
	          8 ) == 0 )
	{
		read_count = libewf_section_sectors_read(
		              file_io_pool,
		              segment_file_handle,
 		              (size64_t) section_size,
 		              *ewf_format,
 		              error );
	}
	/* Read the delta_chunk section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "delta_chunk",
	          12 ) == 0 )
	{
		read_count = libewf_section_delta_chunk_read(
 		              file_io_pool,
 		              segment_file_handle,
 		              (size_t) section_size,
 		              offset_table,
 		              error );
	}
	/* Read the ltree section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "ltree",
	          6 ) == 0 )
	{
		read_count = libewf_section_ltree_read(
		              file_io_pool,
		              segment_file_handle,
 		              (size_t) section_size,
		              ewf_format,
 		              error );
	}
	/* Read the session section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "session",
	          8 ) == 0 )
	{
		read_count = libewf_section_session_read(
		              file_io_pool,
		              segment_file_handle,
		              media_values,
		              sessions,
		              (size_t) section_size,
		              *ewf_format,
 		              error );
	}
	/* Read the data section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "data",
	          5 ) == 0 )
	{
		read_count = libewf_section_data_read(
		              file_io_pool,
		              segment_file_handle,
		              (size_t) section_size,
		              media_values,
		              *ewf_format,
 		              error );
	}
	/* Read the hash section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "hash",
	          5 ) == 0 )
	{
		read_count = libewf_section_hash_read(
		              file_io_pool,
		              segment_file_handle,
		              hash_sections->md5_hash,
 		              error );

		hash_sections->md5_hash_set = 1;
	}
	/* Read the xhash section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "xhash",
	          6 ) == 0 )
	{
		read_count = libewf_section_xhash_read(
		              file_io_pool,
		              segment_file_handle,
		              (size_t) section_size,
		              &( hash_sections->xhash ),
		              &( hash_sections->xhash_size ),
		              error );
	}
	/* Read the error2 section
	 * The \0 byte is included in the compare
	 */
	else if( memory_compare(
	          (void *) section->type,
	          (void *) "error2",
	          7 ) == 0 )
	{
		read_count = libewf_section_error2_read(
		              file_io_pool,
		              segment_file_handle,
		              acquiry_errors,
		              (size_t) section_size,
		              *ewf_format,
 		              error );
	}
	else
	{
		notify_verbose_printf(
		 "%s: unsupported section type: %s.\n",
		 function,
		 (char *) section->type );

#if defined( HAVE_DEBUG_OUTPUT )
		if( section_size > SSIZE_MAX )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: section size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		read_count = libewf_section_debug_read(
		              file_io_pool,
		              segment_file_handle,
		              (size_t) section_size,
		              error );
#else
		/* Skip the data within the section
		 */
		if( libewf_file_io_pool_seek_offset(
		     file_io_pool,
		     segment_file_handle->file_io_pool_entry,
		     section_end_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_SEEK_FAILED,
			 "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
		read_count = (ssize64_t) section_size;
#endif
	}
	if( read_count != (ssize64_t) section_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read section: %s.\n",
		 function,
		 (char *) section->type );

		return( -1 );
	}
	*section_start_offset += (off64_t) read_count;

	return( 1 );
}

