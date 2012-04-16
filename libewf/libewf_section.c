/*
 * Section reading/writing functions
 *
 * Copyright (c) 2006-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libewf_definitions.h"
#include "libewf_compression.h"
#include "libewf_debug.h"
#include "libewf_hash_sections.h"
#include "libewf_header_values.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libmfdata.h"
#include "libewf_media_values.h"
#include "libewf_section.h"
#include "libewf_sector_list.h"
#include "libewf_single_files.h"
#include "libewf_unused.h"

#include "ewf_checksum.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_digest.h"
#include "ewf_error2.h"
#include "ewf_file_header.h"
#include "ewf_hash.h"
#include "ewf_ltree.h"
#include "ewf_session.h"
#include "ewf_table.h"
#include "ewf_volume.h"
#include "ewf_volume_smart.h"
#include "ewfx_delta_chunk.h"

/* Tests if a buffer entirely consists of zero values
 * Returns 1 if zero, 0 if not, or -1 on error
 */
int libewf_section_test_zero(
     uint8_t *buffer,
     size_t buffer_size,
     liberror_error_t **error )
{
	static char *function = "libewf_section_test_zero";
	size_t buffer_offset  = 0;

	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	for( buffer_offset = 0;
	     buffer_offset < buffer_size;
	     buffer_offset++ )
	{
		if( buffer[ buffer_offset ] != 0 )
		{
			return( 0 );
		}
	}
	return( 1 );
}

/* Creates a section
 * Returns 1 if successful or -1 on error
 */
int libewf_section_initialize(
     libewf_section_t **section,
     liberror_error_t **error )
{
	static char *function = "libewf_section_initialize";

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( *section != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid csection value already set.",
		 function );

		return( -1 );
	}
	*section = memory_allocate_structure(
	            libewf_section_t );

	if( *section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *section,
	     0,
	     sizeof( libewf_section_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear section.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *section != NULL )
	{
		memory_free(
		 *section );

		*section = NULL;
	}
	return( -1 );
}

/* Frees the section
 * Returns 1 if successful or -1 on error
 */
int libewf_section_free(
     libewf_section_t **section,
     liberror_error_t **error )
{
	static char *function = "libewf_section_free";

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( *section != NULL )
	{
		memory_free(
		 *section );

		*section = NULL;
	}
	return( 1 );
}

/* Clones the section
 * Returns 1 if successful or -1 on error
 */
int libewf_section_clone(
     libewf_section_t **destination_section,
     libewf_section_t *source_section,
     liberror_error_t **error )
{
	static char *function = "libewf_section_clone";

	if( destination_section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination section.",
		 function );

		return( -1 );
	}
	if( *destination_section != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination section value already set.",
		 function );

		return( -1 );
	}
	if( source_section == NULL )
	{
		*destination_section = NULL;

		return( 1 );
	}
	*destination_section = memory_allocate_structure(
	                        libewf_section_t );

	if( *destination_section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination section.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_section,
	     source_section,
	     sizeof( libewf_section_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination section.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_section != NULL )
	{
		memory_free(
		 *destination_section );

		*destination_section = NULL;
	}
	return( -1 );
}

/* Sets the section values
 * Returns 1 if successful or -1 on error
 */
int libewf_section_set_values(
     libewf_section_t *section,
     uint8_t *type,
     size_t type_length,
     off64_t offset,
     size64_t size,
     liberror_error_t **error )
{
	static char *function = "libewf_section_set_values";

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	if( ( type_length == 0 )
	 || ( type_length > 16 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid type length value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( offset < 0 )
	 || ( offset > (off64_t) INT64_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( size > (size64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     section->type,
	     type,
	     type_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set type string.",
		 function );

		return( -1 );
	}
	section->type[ type_length ] = 0;

	section->type_length  = type_length;
	section->start_offset = offset;
	section->end_offset   = (off64_t) ( offset + size );
	section->size         = size;

	return( 1 );
}

/* Reads a section start
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_start_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t file_offset,
         liberror_error_t **error )
{
	ewf_section_start_t section_start;

	static char *function        = "libewf_section_start_read";
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: reading section start from file IO pool entry: %d at offset: %" PRIi64 "\n",
		 function,
		 file_io_pool_entry,
		 file_offset );
	}
#endif
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek section start offset: %" PRIi64 " in file IO pool entry: %d.",
		 function,
		 file_offset,
		 file_io_pool_entry );

		return( -1 );
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &section_start,
	              sizeof( ewf_section_start_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section start from file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: section start:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) &section_start,
		 sizeof( ewf_section_start_t ),
		 0 );
	}
#endif
	if( memory_copy(
	     section->type,
	     section_start.type,
	     16 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set type string.",
		 function );

		return( -1 );
	}
	section->type[ 16 ] = 0;

	byte_stream_copy_to_uint64_little_endian(
	 section_start.size,
	 section->size );

	byte_stream_copy_to_uint64_little_endian(
	 section_start.next_offset,
	 section->end_offset );

	byte_stream_copy_to_uint32_little_endian(
	 section_start.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: type\t\t\t\t\t\t: %s\n",
		 function,
		 (char *) section->type );

		libnotify_printf(
		 "%s: next offset\t\t\t\t\t: %" PRIu64 "\n",
		 function,
		 section->end_offset );

		libnotify_printf(
		 "%s: size\t\t\t\t\t\t: %" PRIu64 "\n",
		 function,
		 section->size );

		libnotify_printf(
	 	 "%s: padding:\n",
		 function );
		libnotify_print_data(
		 section_start.padding,
		 40,
		 0 );

		libnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
		 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       (uint8_t *) &section_start,
	                       sizeof( ewf_section_start_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
	section->type_length = libcstring_narrow_string_length(
	                        (char *) section->type );

	if( ( section->size != 0 )
	 && ( ( section->size < (size64_t) sizeof( ewf_section_start_t ) )
	  ||  ( section->size > (size64_t) INT64_MAX ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( section->end_offset < file_offset )
	 || ( section->end_offset > (off64_t) INT64_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section next offset value out of bounds.",
		 function );

	}
	section->start_offset = file_offset;

	/* Make sure to check if the section next value is sane
	 * the end offset of the next and done sections point back at themselves
	 */
	if( ( section->end_offset == section->start_offset )
	 && ( section->size == sizeof( ewf_section_start_t ) ) )
	{
		if( ( section->type_length != 4 )
		 || ( ( memory_compare(
		         section->type,
		         "next",
		         4 ) != 0 )
		  &&  ( memory_compare(
		         section->type,
		         "done",
		         4 ) != 0 ) ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: mismatch in next section offset (stored: %" PRIi64 ", calculated: %" PRIi64 ").",
			 function,
			 section->end_offset,
			 section->start_offset );

			return( -1 );
		}
	}
	else
	{
		file_offset += (off64_t) section->size;

		if( section->end_offset != file_offset )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: mismatch in next section offset (stored: %" PRIi64 ", calculated: %" PRIi64 ").",
			 function,
			 section->end_offset,
			 file_offset );

			return( -1 );
		}
	}
	return( read_count );
}

/* Writes a section start
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_start_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         liberror_error_t **error )
{
	ewf_section_start_t section_start;

	static char *function        = "libewf_section_start_write";
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &section_start,
	     0,
	     sizeof( ewf_section_start_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear section start.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     section_start.type,
	     section->type,
	     section->type_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set type string.",
		 function );

		return( -1 );
	}
	byte_stream_copy_from_uint64_little_endian(
	 section_start.size,
	 section->size );

	byte_stream_copy_from_uint64_little_endian(
	 section_start.next_offset,
	 section->end_offset );

	calculated_checksum = ewf_checksum_calculate(
	                       &section_start,
	                       sizeof( ewf_section_start_t ) - sizeof( uint32_t ),
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 section_start.checksum,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: writing section start: %s of size: %" PRIu64 " and checksum: 0x%08" PRIx32 ".\n",
		 function,
		 (char *) section->type,
		 section->size,
		 calculated_checksum );
	}
#endif
	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &section_start,
	               sizeof( ewf_section_start_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		return( -1 );
	}
	return( write_count );
}

#if defined( HAVE_DEBUG_OUTPUT )

/* Reads a section for debugging purposes
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_debug_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         liberror_error_t **error )
{
	uint8_t *data              = NULL;
	uint8_t *uncompressed_data = NULL;
	static char *function      = "libewf_section_debug_read";
	ssize_t read_count         = 0;
	size_t uncompressed_size   = 0;
	int result                 = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( section->size > (size64_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.",
		 function );

		return( -1 );
	}
	uncompressed_size = (size_t) ( section->size * 2 );

	if( uncompressed_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: uncompressed size value exceeds maximum.",
		 function );

		return( -1 );
	}
	data = (uint8_t *) memory_allocate(
	                    (size_t) section->size );

	if( data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              data,
	              (size_t) section->size,
	              error );

	if( read_count != (ssize_t) section->size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	uncompressed_data = (uint8_t *) memory_allocate(
	                                 uncompressed_size );

	if( uncompressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed data.",
		 function );

		goto on_error;
	}
	result = libewf_decompress(
	          uncompressed_data,
	          &uncompressed_size,
	          data,
	          (size_t) section->size,
	          error );

	if( result == 0 )
	{
		result = libewf_debug_dump_data(
		          "UNCOMPRESSED data",
		          data,
		          (size_t) section->size,
	                  error );
	}
	else if( result == 1 )
	{
		result = libewf_debug_dump_data(
		          "COMPRESSED data",
		          uncompressed_data,
		          uncompressed_size,
	                  error );
	}
	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print data.",
		 function );

		goto on_error;
	}
	memory_free(
	 uncompressed_data );
	memory_free(
	 data );

	return( read_count );

on_error:
	if( uncompressed_data != NULL )
	{
		memory_free(
		 uncompressed_data );
	}
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	return( -1 );
}

#endif

/* Writes the last section start
 * This is used for the next and done sections,
 * these sections point back towards themselves
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_last_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint8_t ewf_format,
         liberror_error_t **error )
{
	static char *function = "libewf_section_last_write";
	size64_t section_size = 0;
	ssize_t write_count   = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	/* The EnCase (EWF-E01) format leaves the size of this section empty
	 */
	if( ewf_format == EWF_FORMAT_S01 )
	{
		section_size = (uint64_t) sizeof( ewf_section_start_t );
	}
	if( libewf_section_set_values(
	     section,
	     type_string,
	     type_string_length,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start: %s.",
		 function,
		 (char *) type_string );

		return( -1 );
	}
	return( write_count );
}

/* Reads a compressed string section and decompresses it
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_compressed_string_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **uncompressed_string,
         size_t *uncompressed_string_size,
         liberror_error_t **error )
{
	uint8_t *compressed_string = NULL;
	static char *function      = "libewf_section_compressed_string_read";
	void *reallocation         = NULL;
	size64_t section_data_size = 0;
	ssize_t read_count         = 0;
	int result                 = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( uncompressed_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed string.",
		 function );

		return( -1 );
	}
	if( *uncompressed_string != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid uncompressed string value already set.",
		 function );

		return( -1 );
	}
	if( uncompressed_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed string size.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size > (size64_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.",
		 function );

		return( -1 );
	}
	compressed_string = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * (size_t) section_data_size );

	if( compressed_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed string.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              compressed_string,
	              (size_t) section_data_size,
	              error );

	if( read_count != (ssize_t) section_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read compressed string.",
		 function );

		goto on_error;
	}
	/* On average the uncompressed string will be twice as large as the compressed string
	 */
	*uncompressed_string_size = 2 * (size_t) section_data_size;

	*uncompressed_string = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * *uncompressed_string_size );

	if( *uncompressed_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed string.",
		 function );

		goto on_error;
	}
	result = libewf_decompress(
	          *uncompressed_string,
	          uncompressed_string_size,
	          compressed_string,
	          (size_t) section_data_size,
	          error );

	while( ( result == -1 )
	    && ( *uncompressed_string_size > 0 ) )
	{
		liberror_error_free(
		 error );

		reallocation = memory_reallocate(
		                *uncompressed_string,
		                sizeof( uint8_t ) * *uncompressed_string_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize uncompressed string.",
			 function );

			goto on_error;
		}
		*uncompressed_string = (uint8_t *) reallocation;

		result = libewf_decompress(
		          *uncompressed_string,
		          uncompressed_string_size,
		          compressed_string,
		          (size_t) section_data_size,
		          error );
	}
	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable to decompress string.",
		 function );

		goto on_error;
	}
	memory_free(
	 compressed_string );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: uncompressed string:\n",
		 function );
		libnotify_print_data(
		 *uncompressed_string,
		 *uncompressed_string_size,
		 0 );
	}
#endif
	return( read_count );

on_error:
	if( *uncompressed_string != NULL )
	{
		memory_free(
		 *uncompressed_string );

		*uncompressed_string = NULL;
	}
	if( compressed_string != NULL )
	{
		memory_free(
		 compressed_string );
	}
	return( -1 );
}

/* Writes a compressed string section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_write_compressed_string(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint8_t *uncompressed_string,
         size_t uncompressed_string_size,
         int8_t compression_level,
         liberror_error_t **error )
{
	uint8_t *compressed_string    = NULL;
	static char *function         = "libewf_section_write_compressed_string";
	void *reallocation            = NULL;
	size64_t section_size         = 0;
	size_t compressed_string_size = 0;
	ssize_t total_write_count     = 0;
	ssize_t write_count           = 0;
	int result                    = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( type_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type string.",
		 function );

		return( -1 );
	}
	if( uncompressed_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed string.",
		 function );

		return( -1 );
	}
	compressed_string_size = uncompressed_string_size;

	compressed_string = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * compressed_string_size );

	if( compressed_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed string.",
		 function );

		goto on_error;
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
		liberror_error_free(
		 error );

		reallocation = memory_reallocate(
		                compressed_string,
		                sizeof( uint8_t ) * compressed_string_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize compressed string.",
			 function );

			goto on_error;
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
		 "%s: unable to compress string.",
		 function );

		goto on_error;
	}
	section_size = sizeof( ewf_section_start_t )
	             + compressed_string_size;

	if( libewf_section_set_values(
	     section,
	     type_string,
	     type_string_length,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start: %s.",
		 function,
		 (char *) type_string );

		goto on_error;
	}
	total_write_count += write_count;

	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               compressed_string,
	               compressed_string_size,
	               error );

	if( write_count != (ssize_t) compressed_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write compressed string.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	memory_free(
	 compressed_string );

	return( total_write_count );

on_error:
	if( compressed_string != NULL )
	{
		memory_free(
		 compressed_string );
	}
	return( -1 );
}

/* Reads a data section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_data_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         liberror_error_t **error )
{
	ewf_data_t *data             = NULL;
	static char *function        = "libewf_section_data_read";
	ssize_t read_count           = 0;
	size64_t section_data_size   = 0;
	uint64_t number_of_sectors   = 0;
	uint32_t bytes_per_sector    = 0;
	uint32_t calculated_checksum = 0;
	uint32_t error_granularity   = 0;
	uint32_t number_of_chunks    = 0;
	uint32_t sectors_per_chunk   = 0;
	uint32_t stored_checksum     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit         = 0;
#endif

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size != (size64_t) sizeof( ewf_data_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	data = memory_allocate_structure(
	        ewf_data_t );

	if( data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) data,
	              sizeof( ewf_data_t ),
	              error );
	
	if( read_count != (ssize_t) sizeof( ewf_data_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: data:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) data,
		 sizeof( ewf_data_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 data->checksum,
	 stored_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 data->number_of_chunks,
	 number_of_chunks );

	byte_stream_copy_to_uint32_little_endian(
	 data->sectors_per_chunk,
	 sectors_per_chunk );

	byte_stream_copy_to_uint32_little_endian(
	 data->bytes_per_sector,
	 bytes_per_sector );

	byte_stream_copy_to_uint64_little_endian(
	 data->number_of_sectors,
	 number_of_sectors );

	byte_stream_copy_to_uint32_little_endian(
	 data->error_granularity,
	 error_granularity );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: media type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 data->media_type );

		libnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libnotify_print_data(
		 data->unknown1,
		 3,
		 0 );

		libnotify_printf(
		 "%s: number of chunks\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_chunks );

		libnotify_printf(
		 "%s: sectors per chunk\t\t\t\t: %" PRIu32 "\n",
		 function,
		 sectors_per_chunk );

		libnotify_printf(
		 "%s: bytes per sector\t\t\t\t: %" PRIu32 "\n",
		 function,
		 bytes_per_sector );

		libnotify_printf(
		 "%s: number of sectors\t\t\t\t: %" PRIu64 "\n",
		 function,
		 number_of_sectors );

		byte_stream_copy_to_uint32_little_endian(
		 data->chs_cylinders,
		 value_32bit );
		libnotify_printf(
		 "%s: CHS number of cylinders\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 data->chs_heads,
		 value_32bit );
		libnotify_printf(
		 "%s: CHS number of heads\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 data->chs_sectors,
		 value_32bit );
		libnotify_printf(
		 "%s: CHS number of sectors\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: media flags\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 data->media_flags );

		libnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libnotify_print_data(
		 data->unknown2,
		 3,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 data->palm_volume_start_sector,
		 value_32bit );
		libnotify_printf(
		 "%s: PALM volume start sector\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libnotify_print_data(
		 data->unknown3,
		 4,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 data->smart_logs_start_sector,
		 value_32bit );
		libnotify_printf(
		 "%s: SMART logs start sector\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: compression level\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 data->compression_level );

		libnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libnotify_print_data(
		 data->unknown4,
		 3,
		 0 );

		libnotify_printf(
		 "%s: error granularity\t\t\t\t: %" PRIu32 "\n",
		 function,
		 error_granularity );

		libnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libnotify_print_data(
		 data->unknown5,
		 4,
		 0 );

		libnotify_printf(
		 "%s: GUID:\n",
		 function );
		libnotify_print_data(
		 data->guid,
		 16,
		 0 );

		libnotify_printf(
		 "%s: unknown6:\n",
		 function );
		libnotify_print_data(
		 data->unknown6,
		 963,
		 0 );

		libnotify_printf(
		 "%s: signature:\n",
		 function );
		libnotify_print_data(
		 data->signature,
		 5,
		 0 );

		libnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
		 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       data,
	                       sizeof( ewf_data_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	if( ( data->media_type != 0 )
	 && ( data->media_type != media_values->media_type ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: media type does not match.",
		 function );

		goto on_error;
	}
	if( ( number_of_chunks != 0 )
	 && ( number_of_chunks != media_values->number_of_chunks ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: number of chunks does not match.",
		 function );

		goto on_error;
	}
	if( ( sectors_per_chunk != 0 )
	 && ( sectors_per_chunk != media_values->sectors_per_chunk ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: sectors per chunk does not match.",
		 function );

		goto on_error;
	}
	if( ( bytes_per_sector != 0 )
	 && ( bytes_per_sector != media_values->bytes_per_sector ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: bytes per sector does not match.",
		 function );

		goto on_error;
	}
	if( ( number_of_sectors != 0 )
	 && ( number_of_sectors != media_values->number_of_sectors ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: number of sectors does not match.",
		 function );

		goto on_error;
	}
	if( ( data->media_flags != 0 )
	 && ( data->media_flags != media_values->media_flags ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: media flags do not match.",
		 function );

		goto on_error;
	}
	if( ( data->compression_level != 0 )
	 && ( data->compression_level != io_handle->compression_level ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: compression level does not match.",
		 function );

		goto on_error;
	}
	if( ( error_granularity != 0 )
	 && ( error_granularity != media_values->error_granularity ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: error granularity does not match.",
		 function );

		goto on_error;
	}
	if( ( data->guid[ 0 ] != 0 )
	 || ( data->guid[ 1 ] != 0 )
	 || ( data->guid[ 2 ] != 0 )
	 || ( data->guid[ 3 ] != 0 )
	 || ( data->guid[ 4 ] != 0 )
	 || ( data->guid[ 5 ] != 0 )
	 || ( data->guid[ 6 ] != 0 )
	 || ( data->guid[ 7 ] != 0 )
	 || ( data->guid[ 8 ] != 0 )
	 || ( data->guid[ 9 ] != 0 )
	 || ( data->guid[ 10 ] != 0 )
	 || ( data->guid[ 11 ] != 0 )
	 || ( data->guid[ 12 ] != 0 )
	 || ( data->guid[ 13 ] != 0 )
	 || ( data->guid[ 14 ] != 0 )
	 || ( data->guid[ 15 ] != 0 ) )
	{
		if( memory_compare(
		     media_values->guid,
		     data->guid,
		     16 ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_INPUT,
			 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: GUID does not match.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 data );

	return( read_count );

on_error:
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	return( -1 );
}

/* Writes a data section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_data_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         ewf_data_t **cached_data_section,
         liberror_error_t **error )
{
	static char *function        = "libewf_section_data_write";
	size64_t section_size        = 0;
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( cached_data_section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid caches data section.",
		 function );

		return( -1 );
	}
	section_size = sizeof( ewf_section_start_t )
	             + sizeof( ewf_data_t );

	if( libewf_section_set_values(
	     section,
	     (uint8_t *) "data",
	     4,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( *cached_data_section == NULL )
	{
		*cached_data_section = memory_allocate_structure(
		                        ewf_data_t );

		if( *cached_data_section == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *cached_data_section,
		     0,
		     sizeof( ewf_data_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data.",
			 function );

			memory_free(
			 *cached_data_section );

			*cached_data_section = NULL;

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: media type\t\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 media_values->media_type );

			libnotify_printf(
			 "%s: number of chunks\t\t\t\t: %" PRIu32 "\n",
			 function,
			 media_values->number_of_chunks );

			libnotify_printf(
			 "%s: sectors per chunk\t\t\t\t: %" PRIu32 "\n",
			 function,
			 media_values->sectors_per_chunk );

			libnotify_printf(
			 "%s: bytes per sector\t\t\t\t: %" PRIu32 "\n",
			 function,
			 media_values->bytes_per_sector );

			libnotify_printf(
			 "%s: number of sectors\t\t\t\t: %" PRIu64 "\n",
			 function,
			 media_values->number_of_sectors );

			libnotify_printf(
			 "%s: media flags\t\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 media_values->media_flags );

			libnotify_printf(
			 "%s: compression level\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 io_handle->compression_level );

			libnotify_printf(
			 "%s: error granularity\t\t\t\t: %" PRIu32 "\n",
			 function,
			 media_values->error_granularity );

			libnotify_printf(
			 "%s: GUID:\n",
			 function );
			libnotify_print_data(
			 media_values->guid,
			 16,
			 0 );

			libnotify_printf(
			 "\n" );
		}
#endif
		( *cached_data_section )->media_type = media_values->media_type;
		( *cached_data_section )->media_flags = media_values->media_flags;

		byte_stream_copy_from_uint32_little_endian(
		 ( *cached_data_section )->number_of_chunks,
		 media_values->number_of_chunks );

		byte_stream_copy_from_uint32_little_endian(
		 ( *cached_data_section )->sectors_per_chunk,
		 media_values->sectors_per_chunk );

		byte_stream_copy_from_uint32_little_endian(
		 ( *cached_data_section )->bytes_per_sector,
		 media_values->bytes_per_sector );

		byte_stream_copy_from_uint64_little_endian(
		 ( *cached_data_section )->number_of_sectors,
		 media_values->number_of_sectors );

		if( ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
		 || ( io_handle->format == LIBEWF_FORMAT_EWFX ) )
		{
			byte_stream_copy_from_uint32_little_endian(
			 ( *cached_data_section )->error_granularity,
			 media_values->error_granularity );

			( *cached_data_section )->compression_level = (uint8_t) io_handle->compression_level;

			if( memory_copy(
			     ( *cached_data_section )->guid,
			     media_values->guid,
			     16 ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to set GUID.",
				 function );

				memory_free(
				 *cached_data_section );

				*cached_data_section = NULL;

				return( -1 );
			}
		}
		calculated_checksum = ewf_checksum_calculate(
		                       *cached_data_section,
		                       sizeof( ewf_data_t ) - sizeof( uint32_t ),
		                       1 );

		byte_stream_copy_from_uint32_little_endian(
		 ( *cached_data_section )->checksum,
		 calculated_checksum );
	}
	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) *cached_data_section,
	               sizeof( ewf_data_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_data_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	return( total_write_count );
}

/* Reads a digest section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_digest_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error )
{
	ewf_digest_t digest;

	static char *function        = "libewf_section_digest_read";
	size64_t section_data_size   = 0;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;
	int result                   = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size != (size64_t) sizeof( ewf_digest_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &digest,
	              sizeof( ewf_digest_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_digest_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read digest.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: digest data:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) &digest,
		 sizeof( ewf_digest_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 digest.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libnotify_print_data(
		 digest.md5_hash,
		 16,
		 0 );

		libnotify_printf(
		 "%s: SHA1 hash:\n",
		 function );
		libnotify_print_data(
		 digest.sha1_hash,
		 20,
		 0 );

		libnotify_printf(
		 "%s: padding:\n",
		 function );
		libnotify_print_data(
		 digest.padding1,
		 40,
		 0 );

		libnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
		 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &digest,
	                       sizeof( ewf_digest_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
	result = libewf_section_test_zero(
		  digest.md5_hash,
		  16,
		  error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MD5 hash is empty.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( memory_copy(
		     hash_sections->md5_digest,
		     digest.md5_hash,
		     16 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 digest in hash sections.",
			 function );

			return( -1 );
		}
		hash_sections->md5_digest_set = 1;
	}
	else
	{
		hash_sections->md5_digest_set = 0;
	}
	result = libewf_section_test_zero(
		  digest.sha1_hash,
		  20,
		  error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if SHA1 hash is empty.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( memory_copy(
		     hash_sections->sha1_digest,
		     digest.sha1_hash,
		     20 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set SHA1 digest in hash sections.",
			 function );

			return( -1 );
		}
		hash_sections->sha1_digest_set = 1;
	}
	else
	{
		hash_sections->sha1_digest_set = 0;
	}
	return( read_count );
}

/* Writes a digest section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_digest_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error )
{
	ewf_digest_t digest;

	static char *function        = "libewf_section_digest_write";
	size64_t section_size        = 0;
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	section_size = sizeof( ewf_section_start_t )
	             + sizeof( ewf_digest_t );

	if( libewf_section_set_values(
	     section,
	     (uint8_t *) "digest",
	     6,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( memory_set(
	     &digest,
	     0,
	     sizeof( ewf_digest_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear digest.",
		 function );

		return( -1 );
	}
	if( hash_sections->md5_digest_set != 0 )
	{
		if( memory_copy(
		     digest.md5_hash,
		     hash_sections->md5_digest,
		     16 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 hash.",
			 function );

			return( -1 );
		}
	}
	if( hash_sections->sha1_digest_set != 0 )
	{
		if( memory_copy(
		     digest.sha1_hash,
		     hash_sections->sha1_digest,
		     20 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 hash.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libnotify_print_data(
		 digest.md5_hash,
		 16,
		 0 );

		libnotify_printf(
		 "%s: SHA1 hash:\n",
		 function );
		libnotify_print_data(
		 digest.sha1_hash,
		 20,
		 0 );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &digest,
	                       sizeof( ewf_digest_t ) - sizeof( uint32_t ),
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 digest.checksum,
	 calculated_checksum );

	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &digest,
	               sizeof( ewf_digest_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_digest_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write digest.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	return( total_write_count );
}

/* Reads an error2 section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_error2_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_sector_list_t *acquiry_errors,
         liberror_error_t **error )
{
	ewf_error2_header_t error2_header;
	uint8_t stored_checksum_buffer[ 4 ];

	ewf_error2_sector_t *error2_sectors = NULL;
	static char *function               = "libewf_section_error2_read";
	size64_t section_data_size          = 0;
	size_t error2_sectors_size          = 0;
	ssize_t read_count                  = 0;
	ssize_t total_read_count            = 0;
	uint32_t calculated_checksum        = 0;
	uint32_t error_index                = 0;
	uint32_t first_sector               = 0;
	uint32_t number_of_errors           = 0;
	uint32_t number_of_sectors          = 0;
	uint32_t stored_checksum            = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid acquiry errors.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size < (size64_t) sizeof( ewf_error2_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	section_data_size -= sizeof( ewf_error2_header_t );

	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &error2_header,
	              sizeof( ewf_error2_header_t ),
	              error );
	
	if( read_count != (ssize_t) sizeof( ewf_error2_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read error2 header.",
		 function );

		goto on_error;
	}
	total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: error2 header data:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) &error2_header,
		 sizeof( ewf_error2_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 error2_header.number_of_errors,
	 number_of_errors );

	byte_stream_copy_to_uint32_little_endian(
	 error2_header.checksum,
	 stored_checksum);

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: number of errors\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_errors );

		libnotify_printf(
		 "%s: unknown:\n",
		 function );
		libnotify_print_data(
		 error2_header.unknown,
		 200,
		 0 );

		libnotify_printf(
	 	 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
	 	 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &error2_header,
	                       sizeof( ewf_error2_header_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum!= calculated_checksum)
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	if( number_of_errors > 0 )
	{
		error2_sectors_size = sizeof( ewf_error2_sector_t ) * number_of_errors;

		if( section_data_size < error2_sectors_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds.",
			 function );

			goto on_error;
		}
		error2_sectors = (ewf_error2_sector_t *) memory_allocate(
		                                          error2_sectors_size );

		if( error2_sectors == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create error2 sectors.",
			 function );

			goto on_error;
		}
		read_count = libbfio_pool_read(
		              file_io_pool,
		              file_io_pool_entry,
		              (uint8_t *) error2_sectors,
		              error2_sectors_size,
	                      error );
	
		if( read_count != (ssize_t) error2_sectors_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read error2 sectors.",
			 function );

			goto on_error;
		}
		total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: error2 sectors data:\n",
			 function );
			libnotify_print_data(
			 (uint8_t *) error2_sectors,
			 error2_sectors_size,
			 0 );
		}
#endif
		read_count = libbfio_pool_read(
		              file_io_pool,
		              file_io_pool_entry,
		              stored_checksum_buffer,
		              sizeof( uint32_t ),
	                      error );

		if( read_count != (ssize_t) sizeof( uint32_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read checksum.",
			 function );

			goto on_error;
		}
		total_read_count += read_count;

		byte_stream_copy_to_uint32_little_endian(
		 stored_checksum_buffer,
		 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: error2 sectors checksum\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 stored_checksum );

			libnotify_printf(
			 "\n" );
		}
#endif
		calculated_checksum = ewf_checksum_calculate(
		                       error2_sectors,
		                       error2_sectors_size,
		                       1 );

		if( stored_checksum != calculated_checksum )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_INPUT,
			 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
			 function,
			 stored_checksum,
			 calculated_checksum );

			goto on_error;
		}
		if( libewf_sector_list_empty(
		     acquiry_errors,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty acquiry errors sector list.",
			 function );

			goto on_error;
		}
		for( error_index = 0;
		     error_index < number_of_errors;
		     error_index++ )
		{
			byte_stream_copy_to_uint32_little_endian(
			 error2_sectors[ error_index ].first_sector,
			 first_sector );

			byte_stream_copy_to_uint32_little_endian(
			 error2_sectors[ error_index ].number_of_sectors,
			 number_of_sectors );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: error2: %02" PRIu32 " first sector\t\t\t: %" PRIu32 "\n",
				 function,
				 error_index,
				 first_sector );

				libnotify_printf(
				 "%s: error2: %02" PRIu32 " number of sectors\t\t: %" PRIu32 "\n",
				 function,
				 error_index,
				 number_of_sectors );
			}
#endif
			if( libewf_sector_list_append_sector(
			     acquiry_errors,
			     (uint64_t) first_sector,
			     (uint64_t) number_of_sectors,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append acquiry error to sector list.",
				 function );

				goto on_error;
			}
		}
		memory_free(
		 error2_sectors );

		error2_sectors = NULL;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: error2 section contains no error sectors.\n",
		 function );
	}
#endif
	return( total_read_count );

on_error:
	if( error2_sectors != NULL )
	{
		memory_free(
		 error2_sectors );
	}
	return( -1 );
}

/* Writes an error2 section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_error2_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_sector_list_t *acquiry_errors,
         liberror_error_t **error )
{
	ewf_error2_header_t error2_header;
	uint8_t calculated_checksum_buffer[ 4 ];

	ewf_error2_sector_t *error2_sectors = NULL;
	static char *function               = "libewf_section_error2_write";
	size64_t section_size               = 0;
	size_t error2_sectors_size          = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	uint64_t first_sector               = 0;
	uint64_t number_of_sectors          = 0;
	uint32_t calculated_checksum        = 0;
	int error_index                     = 0;
	int number_of_errors                = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( libewf_sector_list_get_number_of_elements(
	     acquiry_errors,
	     &number_of_errors,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from acquiry error sector list.",
		 function );

		goto on_error;
	}
	if( number_of_errors <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of errors value out of bounds.",
		 function );

		goto on_error;
	}
	error2_sectors_size = sizeof( ewf_error2_sector_t ) * number_of_errors;

	section_size = sizeof( ewf_section_start_t )
	             + sizeof( ewf_error2_header_t )
	             + error2_sectors_size
	             + sizeof( uint32_t );

	if( libewf_section_set_values(
	     section,
	     (uint8_t *) "error2",
	     6,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	if( memory_set(
	     &error2_header,
	     0,
	     sizeof( ewf_error2_header_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear error2 header.",
		 function );

		goto on_error;
	}
	byte_stream_copy_from_uint32_little_endian(
	 error2_header.number_of_errors,
	 number_of_errors );

	calculated_checksum = ewf_checksum_calculate(
	                       &error2_header,
	                       sizeof( ewf_error2_header_t ) - sizeof( uint32_t ),
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 error2_header.checksum,
	 calculated_checksum );

	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &error2_header,
	               sizeof( ewf_error2_header_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_error2_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write error2 header.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

/* TODO EnCase compatible way to handle > 32-bit sector values */
	error2_sectors = (ewf_error2_sector_t *) memory_allocate(
						  error2_sectors_size );

	if( error2_sectors == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create error2 sectors.",
		 function );

		goto on_error;
	}
	for( error_index = 0;
	     error_index < number_of_errors;
	     error_index++ )
	{
		if( libewf_sector_list_get_sector(
		     acquiry_errors,
		     error_index,
		     &first_sector,
		     &number_of_sectors,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve acquiry error: %d from sector list.",
			 function,
			 error_index );

			goto on_error;
		}
		byte_stream_copy_from_uint32_little_endian(
		 error2_sectors[ error_index ].first_sector,
		 (uint32_t) first_sector );

		byte_stream_copy_from_uint32_little_endian(
		 error2_sectors[ error_index ].number_of_sectors,
		 (uint32_t) number_of_sectors );
	}
	write_count = libbfio_pool_write(
		       file_io_pool,
		       file_io_pool_entry,
		       (uint8_t *) error2_sectors,
		       error2_sectors_size,
		       error );

	if( write_count != (ssize_t) error2_sectors_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write error2 sectors.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	calculated_checksum = ewf_checksum_calculate(
			       error2_sectors,
			       error2_sectors_size,
			       1 );

	byte_stream_copy_from_uint32_little_endian(
	 calculated_checksum_buffer,
	 calculated_checksum );

	write_count = libbfio_pool_write(
		       file_io_pool,
		       file_io_pool_entry,
		       calculated_checksum_buffer,
		       sizeof( uint32_t ),
		       error );

	if( write_count != (ssize_t) sizeof( uint32_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write error2 sectors checksum.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	memory_free(
	 error2_sectors );

	return( total_write_count );

on_error:
	if( error2_sectors != NULL )
	{
		memory_free(
		 error2_sectors );
	}
	return( -1 );
}

/* Reads a hash section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_hash_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error )
{
	ewf_hash_t hash;

	static char *function        = "libewf_section_hash_read";
	size64_t section_data_size   = 0;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;
	int result                   = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size != (size64_t) sizeof( ewf_hash_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &hash,
	              sizeof( ewf_hash_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_hash_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read hash.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: hash data:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) &hash,
		 sizeof( ewf_hash_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 hash.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libnotify_print_data(
		 hash.md5_hash,
		 16,
		 0 );

		libnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libnotify_print_data(
		 hash.unknown1,
		 16,
		 0 );

		libnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
		 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &hash,
	                       sizeof( ewf_hash_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
	result = libewf_section_test_zero(
		  hash.md5_hash,
		  16,
		  error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MD5 hash is empty.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( memory_copy(
		     hash_sections->md5_hash,
		     hash.md5_hash,
		     16 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 hash in hash sections.",
			 function );

			return( -1 );
		}
		hash_sections->md5_hash_set = 1;
	}
	else
	{
		hash_sections->md5_hash_set = 0;
	}
	return( read_count );
}

/* Writes a hash section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_hash_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error )
{
	ewf_hash_t hash;

	static char *function        = "libewf_section_hash_write";
	size64_t section_size        = 0;
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	section_size = sizeof( ewf_section_start_t )
	             + sizeof( ewf_hash_t );

	if( libewf_section_set_values(
	     section,
	     (uint8_t *) "hash",
	     4,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( memory_set(
	     &hash,
	     0,
	     sizeof( ewf_hash_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear hash.",
		 function );

		return( -1 );
	}
	if( hash_sections->md5_hash_set != 0 )
	{
		if( memory_copy(
		     hash.md5_hash,
		     hash_sections->md5_hash,
		     16 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 hash.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libnotify_print_data(
		 hash.md5_hash,
		 16,
		 0 );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &hash,
	                       sizeof( ewf_hash_t ) - sizeof( uint32_t ),
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 hash.checksum,
	 calculated_checksum );

	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &hash,
	               sizeof( ewf_hash_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_hash_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write hash.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	return( total_write_count );
}

/* Reads a header section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_header_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error )
{
	uint8_t *header       = NULL;
	static char *function = "libewf_section_header_read";
	ssize_t read_count    = 0;
	size_t header_size    = 0;

	if( header_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              section,
	              file_io_pool,
	              file_io_pool_entry,
	              &header,
	              &header_size,
	              error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read header.",
		 function );

		goto on_error;
	}
	if( header == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libewf_debug_byte_stream_print(
		     "Header",
		     header,
		     header_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print header.",
			 function );

			goto on_error;
		}
	}
#endif
	if( header_sections->header == NULL )
	{
		header_sections->header      = header;
		header_sections->header_size = header_size;
	}
	else
	{
		memory_free(
		 header );
	}
	header_sections->number_of_header_sections += 1;

	return( read_count );

on_error:
	if( header != NULL )
	{
		memory_free(
		 header );
	}
	return( -1 );
}

/* Writes a header section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_header_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         liberror_error_t **error )
{
	static char *function = "libewf_section_header_write";
	ssize_t write_count   = 0;

	if( header_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libewf_debug_byte_stream_print(
		     "Header",
		     header_sections->header,
		     header_sections->header_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print header.",
			 function );

			return( -1 );
		}
	}
#endif
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) "header",
	               6,
	               section_offset,
	               header_sections->header,
	               header_sections->header_size - 1,
	               compression_level,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write compressed string.",
		 function );

		return( -1 );
	}
	header_sections->number_of_header_sections += 1;

	return( write_count );
}

/* Reads a header2 section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_header2_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error )
{
	uint8_t *header2      = NULL;
	static char *function = "libewf_section_header2_read";
	ssize_t read_count    = 0;
	size_t header2_size   = 0;

	if( header_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              section,
	              file_io_pool,
	              file_io_pool_entry,
	              &header2,
	              &header2_size,
	              error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read header2.",
		 function );

		goto on_error;
	}
	if( header2 == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing header2.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libewf_debug_utf16_stream_print(
		     "Header2",
		     header2,
		     header2_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print header2.",
			 function );

			goto on_error;
		}
	}
#endif
	if( header_sections->header2 == NULL )
	{
		header_sections->header2      = header2;
		header_sections->header2_size = header2_size;
	}
	else
	{
		memory_free(
		 header2 );
	}
	header_sections->number_of_header_sections += 1;

	return( read_count );

on_error:
	if( header2 != NULL )
	{
		memory_free(
		 header2 );
	}
	return( -1 );
}

/* Writes a header2 section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_header2_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         liberror_error_t **error )
{
	static char *function = "libewf_section_header2_write";
	ssize_t write_count   = 0;

	if( header_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libewf_debug_utf16_stream_print(
		     "Header2",
		     header_sections->header2,
		     header_sections->header2_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print header2.",
			 function );

			return( -1 );
		}
	}
#endif
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) "header2",
	               7,
	               section_offset,
	               header_sections->header2,
	               header_sections->header2_size - 2,
	               compression_level,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write compressed string.",
		 function );

		return( -1 );
	}
	header_sections->number_of_header_sections += 1;

	return( write_count );
}

/* Reads a ltree section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_ltree_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **cached_ltree_data,
         size_t *cached_ltree_data_size,
         liberror_error_t **error )
{
	ewf_ltree_header_t *ltree_header = NULL;
	uint8_t *ltree_data              = NULL;
	static char *function            = "libewf_section_ltree_read";
	size64_t section_data_size       = 0;
	size_t ltree_data_size           = 0;
	ssize_t read_count               = 0;
	ssize_t total_read_count         = 0;
	uint32_t ltree_size              = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( cached_ltree_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached ltree.",
		 function );

		return( -1 );
	}
	if( cached_ltree_data_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached ltree data size.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size < (size64_t) sizeof( ewf_ltree_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	section_data_size -= sizeof( ewf_ltree_header_t );

	ltree_header = memory_allocate_structure(
	                ewf_ltree_header_t );

	if( ltree_header == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create ltree header.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) ltree_header,
	              sizeof( ewf_ltree_header_t ),
	              error );
	
	if( read_count != (ssize_t) sizeof( ewf_ltree_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ltree header.",
		 function );

		goto on_error;
	}
	total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: ltree header data:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) ltree_header,
		 sizeof( ewf_ltree_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ltree_header->tree_size,
	 ltree_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libnotify_print_data(
		 ltree_header->unknown1,
		 16,
		 0 );

		libnotify_printf(
		 "%s: tree size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 ltree_size );

		libnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libnotify_print_data(
		 ltree_header->unknown2,
		 4,
		 0 );

		libnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libnotify_print_data(
		 ltree_header->unknown3,
		 4,
		 0 );

		libnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libnotify_print_data(
		 ltree_header->unknown4,
		 20,
		 0 );
	}
#endif
	memory_free(
	 ltree_header );

	ltree_header = NULL;

	if( section_data_size < (size64_t) ltree_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	ltree_data_size = (size_t) ltree_size;

	ltree_data = (uint8_t *) memory_allocate(
                                  ltree_data_size );

	if( ltree_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create ltree data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              ltree_data,
	              ltree_data_size,
	              error );

	if( read_count != (ssize_t) ltree_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ltree data.",
		 function );

		goto on_error;
	}
	total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libewf_debug_utf16_stream_print(
		     "ltree data",
		     ltree_data,
		     ltree_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print ltree data.",
			 function );

			goto on_error;
		}
	}
#endif
	if( *cached_ltree_data == NULL )
	{
		*cached_ltree_data      = ltree_data;
		*cached_ltree_data_size = ltree_data_size;
	}
	else
	{
		memory_free(
		 ltree_data );
	}
	return( total_read_count );

on_error:
	if( ltree_data != NULL )
	{
		memory_free(
		 ltree_data );
	}
	if( ltree_header != NULL )
	{
		memory_free(
		 ltree_header );
	}
	return( -1 );
}

/* Writes a sectors section
 * Does not write the actual data in the sectors section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_sectors_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         size64_t chunks_data_size,
         liberror_error_t **error )
{
	static char *function = "libewf_section_sectors_write";
	size64_t section_size = 0;
	ssize_t write_count   = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	section_size = sizeof( ewf_section_start_t )
	             + chunks_data_size;

	if( libewf_section_set_values(
	     section,
	     (uint8_t *) "sectors",
	     7,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Reads a session section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_session_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libewf_sector_list_t *sessions,
         libewf_sector_list_t *tracks,
         liberror_error_t **error )
{
	ewf_session_header_t session_header;
	uint8_t stored_checksum_buffer[ 4 ];

	ewf_session_entry_t *session_entries = NULL;
	static char *function                = "libewf_section_session_read";
	size64_t section_data_size           = 0;
	size_t session_entries_size          = 0;
	ssize_t read_count                   = 0;
	ssize_t total_read_count             = 0;
	uint32_t calculated_checksum         = 0;
	uint32_t first_sector                = 0;
	uint32_t previous_first_sector       = 0;
	uint32_t previous_type               = 0;
	uint32_t number_of_sectors           = 0;
	uint32_t number_of_sessions_entries  = 0;
	uint32_t session_first_sector        = 0;
	uint32_t sessions_entry_index        = 0;
	uint32_t stored_checksum             = 0;
	uint32_t track_first_sector          = 0;
	uint32_t type                        = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( sessions == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sessions.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size < (size64_t) sizeof( ewf_session_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	section_data_size -= sizeof( ewf_session_header_t );

	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &session_header,
	              sizeof( ewf_session_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_session_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read session.",
		 function );

		goto on_error;
	}
	total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: session header data:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) &session_header,
		 sizeof( ewf_session_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 session_header.number_of_sessions,
	 number_of_sessions_entries );

	byte_stream_copy_to_uint32_little_endian(
	 session_header.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: number of sessions entries\t\t: %" PRIu32 "\n",
		 function,
		 number_of_sessions_entries );

		libnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libnotify_print_data(
		 session_header.unknown1,
		 28,
		 0 );

		libnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
		 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &session_header,
	                       sizeof( ewf_session_header_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	if( number_of_sessions_entries > 0 )
	{
		session_entries_size = sizeof( ewf_session_entry_t ) * number_of_sessions_entries;

		if( section_data_size < (size64_t) session_entries_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds.",
			 function );

			goto on_error;
		}
		session_entries = (ewf_session_entry_t *) memory_allocate(
		                                           session_entries_size );

		if( session_entries == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create session entries.",
			 function );

			goto on_error;
		}
		read_count = libbfio_pool_read(
		              file_io_pool,
		              file_io_pool_entry,
		              (uint8_t *) session_entries,
		              session_entries_size,
		              error );
	
		if( read_count != (ssize_t) session_entries_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read session entries.",
			 function );

			goto on_error;
		}
		total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: session entries data:\n",
			 function );
			libnotify_print_data(
			 (uint8_t *) session_entries,
			 session_entries_size,
			 0 );
		}
#endif
		read_count = libbfio_pool_read(
		              file_io_pool,
		              file_io_pool_entry,
		              stored_checksum_buffer,
		              sizeof( uint32_t ),
		              error );

		if( read_count != (ssize_t) sizeof( uint32_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read checksum.",
			 function );

			goto on_error;
		}
		total_read_count += read_count;

		byte_stream_copy_to_uint32_little_endian(
		 stored_checksum_buffer,
		 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: session entries checksum\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 stored_checksum );

			libnotify_printf(
			 "\n" );
		}
#endif
		calculated_checksum = ewf_checksum_calculate(
		                       session_entries,
		                       session_entries_size,
		                       1 );

		if( stored_checksum != calculated_checksum )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_INPUT,
			 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
			 function,
			 stored_checksum,
			 calculated_checksum );

			goto on_error;
		}
		if( libewf_sector_list_empty(
		     sessions,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty sessions sector list.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( session_entries[ sessions_entry_index ] ).type,
		 previous_type );

		byte_stream_copy_to_uint32_little_endian(
		 ( session_entries[ sessions_entry_index ] ).first_sector,
		 previous_first_sector );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: entry: %02" PRIu32 " type\t\t\t\t: %" PRIu32 "\n",
			 function,
			 sessions_entry_index,
			 previous_type );

			libnotify_printf(
			 "%s: entry: %02" PRIu32 " first sector\t\t\t: 0 (%" PRIu32 ")\n",
			 function,
			 sessions_entry_index,
			 previous_first_sector );

			libnotify_printf(
			 "%s: entry: %02" PRIu32 " unknown2:\n",
			 function,
			 sessions_entry_index );
			libnotify_print_data(
			 ( session_entries[ sessions_entry_index ] ).unknown2,
			 24,
			 0 );
		}
#endif
		/* Note that EnCase says the first session starts at session 16
		 * This is either some EnCase specific behavior or the value is used for
		 * other purposes.
		 */
		session_first_sector = 0;
		track_first_sector   = 0;

		for( sessions_entry_index = 1;
		     sessions_entry_index < number_of_sessions_entries;
		     sessions_entry_index++ )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( session_entries[ sessions_entry_index ] ).type,
			 type );

			byte_stream_copy_to_uint32_little_endian(
			 session_entries[ sessions_entry_index ].first_sector,
			 first_sector );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: entry: %02" PRIu32 " type\t\t\t\t: %" PRIu32 "\n",
				 function,
				 sessions_entry_index,
				 type );

				libnotify_printf(
				 "%s: entry: %02" PRIu32 " first sector\t\t\t: %" PRIu32 "\n",
				 function,
				 sessions_entry_index,
				 first_sector );

				libnotify_printf(
				 "%s: entry: %02" PRIu32 " unknown2:\n",
				 function,
				 sessions_entry_index );
				libnotify_print_data(
				 ( session_entries[ sessions_entry_index ] ).unknown2,
				 24,
				 0 );
			}
#endif
			if( first_sector < previous_first_sector )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid first sector: %" PRIu32 " value out of bounds.",
				 function,
				 first_sector );

				goto on_error;
			}
			if( type == 0 )
			{
				number_of_sectors = first_sector - session_first_sector;

				if( libewf_sector_list_append_sector(
				     sessions,
				     (uint64_t) session_first_sector,
				     (uint64_t) number_of_sectors,
				     0,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append session to sector list.",
					 function );
	
					goto on_error;
				}
				session_first_sector = first_sector;
			}
			if( previous_type == 1 )
			{
				number_of_sectors = first_sector - track_first_sector;

				if( libewf_sector_list_append_sector(
				     tracks,
				     (uint64_t) track_first_sector,
				     (uint64_t) number_of_sectors,
				     0,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append track to sector list.",
					 function );
	
					goto on_error;
				}
				track_first_sector = first_sector;
			}
			previous_type         = type;
			previous_first_sector = first_sector;
		}
		memory_free(
		 session_entries );

		session_entries = NULL;

		if( media_values->number_of_sectors > session_first_sector )
		{
			number_of_sectors = (uint32_t) ( media_values->number_of_sectors - session_first_sector );
		}
		else
		{
			number_of_sectors = 0;
		}
		if( libewf_sector_list_append_sector(
		     sessions,
		     (uint64_t) session_first_sector,
		     (uint64_t) number_of_sectors,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append session to sector list.",
			 function );

			goto on_error;
		}
		if( type == 1 )
		{
			if( media_values->number_of_sectors > track_first_sector )
			{
				number_of_sectors = (uint32_t) ( media_values->number_of_sectors - track_first_sector );
			}
			else
			{
				number_of_sectors = 0;
			}
			if( libewf_sector_list_append_sector(
			     tracks,
			     (uint64_t) track_first_sector,
			     (uint64_t) number_of_sectors,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append track to sector list.",
				 function );

				goto on_error;
			}
		}
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: session section contains no entries.\n",
		 function );
	}
#endif
	return( total_read_count );

on_error:
	if( session_entries != NULL )
	{
		memory_free(
		 session_entries );
	}
	return( -1 );
}

/* Writes a session section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_session_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_sector_list_t *sessions,
         libewf_sector_list_t *tracks,
         liberror_error_t **error )
{
	ewf_session_header_t session_header;
	uint8_t calculated_checksum_buffer[ 4 ];

	ewf_session_entry_t *session_entries = NULL;
	static char *function                = "libewf_section_session_write";
	size64_t section_size                = 0;
	size_t session_entries_size          = 0;
	ssize_t total_write_count            = 0;
	ssize_t write_count                  = 0;
	uint64_t current_sector              = 0;
	uint64_t session_first_sector        = 0;
	uint64_t session_last_sector         = 0;
	uint64_t session_number_of_sectors   = 0;
	uint64_t track_first_sector          = 0;
	uint64_t track_last_sector           = 0;
	uint64_t track_number_of_sectors     = 0;
	uint32_t calculated_checksum         = 0;
	int number_of_sessions               = 0;
	int number_of_sessions_entries       = 0;
	int number_of_tracks                 = 0;
	int session_index                    = 0;
	int sessions_entry_index             = 0;
	int track_index                      = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( libewf_sector_list_get_number_of_elements(
	     sessions,
	     &number_of_sessions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from sessions sector list.",
		 function );

		goto on_error;
	}
	if( number_of_sessions < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of sessions value out of bounds.",
		 function );

		goto on_error;
	}
	if( libewf_sector_list_get_number_of_elements(
	     tracks,
	     &number_of_tracks,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from tracks sector list.",
		 function );

		goto on_error;
	}
	if( number_of_tracks < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of tracks value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( number_of_sessions != 0 )
	 && ( number_of_tracks == 0 ) )
	{
		number_of_sessions_entries = number_of_sessions;
	}
	else if( ( number_of_sessions == 0 )
	      && ( number_of_tracks != 0 ) )
	{
		number_of_sessions_entries = number_of_tracks;
	}
	else if( ( number_of_sessions != 0 )
	      && ( number_of_tracks != 0 ) )
	{
		/* Encase does not store sessions containing tracks
		 * therefore the number of sessions entries needs to be determined
		 * from the run-time tracks and session information
		 */
		do
		{
			if( ( session_index < number_of_sessions )
			 && ( current_sector >= session_last_sector ) )
			{
				if( libewf_sector_list_get_sector(
				     sessions,
				     session_index,
				     &session_first_sector,
				     &session_number_of_sectors,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve session: %d from sector list.",
					 function,
					 session_index );

					goto on_error;
				}
				session_last_sector = session_first_sector
						    + session_number_of_sectors;

				session_index++;
			}
			if( ( track_index < number_of_tracks )
			 && ( current_sector >= track_last_sector ) )
			{
				if( libewf_sector_list_get_sector(
				     tracks,
				     track_index,
				     &track_first_sector,
				     &track_number_of_sectors,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve track: %d from sector list.",
					 function,
					 track_index );

					goto on_error;
				}
				track_last_sector = track_first_sector
						  + track_number_of_sectors;

				track_index++;
			}
			if( ( number_of_tracks > 0 )
			 && ( current_sector >= track_first_sector )
			 && ( current_sector < track_last_sector ) )
			{
				number_of_sessions_entries++;

				current_sector = track_last_sector;
			}
			else if( ( number_of_sessions > 0 )
			      && ( current_sector >= session_first_sector )
			      && ( current_sector < session_last_sector ) )
			{
				if( ( track_last_sector == 0 )
				 || ( track_last_sector < session_first_sector ) ) 
				{
					number_of_sessions_entries++;
				}
				current_sector = session_last_sector;
			}
		}
		while( ( session_index < number_of_sessions )
		    || ( track_index < number_of_tracks ) );
	}
	if( number_of_sessions_entries == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of sessions entries value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: number of sessions entries\t\t: %" PRIu32 "\n",
		 function,
		 number_of_sessions_entries );

		libnotify_printf(
		 "\n" );
	}
#endif
	session_entries_size = sizeof( ewf_session_entry_t )
	                     * number_of_sessions_entries;

	section_size = sizeof( ewf_section_start_t )
	             + sizeof( ewf_session_header_t )
	             + session_entries_size
	             + sizeof( uint32_t );

	if( libewf_section_set_values(
	     section,
	     (uint8_t *) "session",
	     7,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	if( memory_set(
	     &session_header,
	     0,
	     sizeof( ewf_session_header_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear session header.",
		 function );

		goto on_error;
	}
	byte_stream_copy_from_uint32_little_endian(
	 session_header.number_of_sessions,
	 (uint32_t) number_of_sessions_entries );

	calculated_checksum = ewf_checksum_calculate(
	                       &session_header,
	                       sizeof( ewf_session_header_t ) - sizeof( uint32_t ),
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 session_header.checksum,
	 calculated_checksum );

	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &session_header,
	               sizeof( ewf_session_header_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_session_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write session header.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	session_entries = (ewf_session_entry_t *) memory_allocate(
	                                           session_entries_size );

	if( session_entries == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create session entries.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     session_entries,
	     0,
	     session_entries_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sessions entries.",
		 function );

		goto on_error;
	}
	current_sector       = 0;
	session_first_sector = 0;
	session_index        = 0;
	session_last_sector  = 0;
	track_first_sector   = 0;
	track_index          = 0;
	track_last_sector    = 0;

	do
	{
		if( ( session_index < number_of_sessions )
		 && ( current_sector >= session_last_sector ) )
		{
			if( libewf_sector_list_get_sector(
			     sessions,
			     session_index,
			     &session_first_sector,
			     &session_number_of_sectors,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve session: %d from sector list.",
				 function,
				 session_index );

				goto on_error;
			}
			session_last_sector = session_first_sector
					    + session_number_of_sectors;

			session_index++;
		}
		if( ( track_index < number_of_tracks )
		 && ( current_sector >= track_last_sector ) )
		{
			if( libewf_sector_list_get_sector(
			     tracks,
			     track_index,
			     &track_first_sector,
			     &track_number_of_sectors,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve track: %d from sector list.",
				 function,
				 track_index );

				goto on_error;
			}
			track_last_sector = track_first_sector
					  + track_number_of_sectors;

			track_index++;
		}
		if( ( number_of_tracks > 0 )
		 && ( current_sector >= track_first_sector )
		 && ( current_sector < track_last_sector ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: entry: %02" PRIu32 " type\t\t\t\t: 1\n",
				 function,
				 sessions_entry_index );

				libnotify_printf(
				 "%s: entry: %02" PRIu32 " first sector\t\t\t: %" PRIu32 "\n",
				 function,
				 sessions_entry_index,
				 track_first_sector );

				libnotify_printf(
				 "%s: entry: %02" PRIu32 " last sector\t\t\t: %" PRIu32 "\n",
				 function,
				 sessions_entry_index,
				 track_last_sector );

				libnotify_printf(
				 "\n" );
			}
#endif
			/* Note that EnCase says the first track starts at sector 16
			 * This is either some EnCase specific behavior or the value is used for
			 * other purposes.
			 */
			if( ( sessions_entry_index == 0 )
			 && ( track_first_sector == 0 ) )
			{
				track_first_sector = 16;
			}
			byte_stream_copy_from_uint32_little_endian(
			 ( session_entries[ sessions_entry_index ] ).type,
			 1 );
			byte_stream_copy_from_uint32_little_endian(
			 ( session_entries[ sessions_entry_index ] ).first_sector,
			 (uint32_t) track_first_sector );

			sessions_entry_index++;

			current_sector = track_last_sector;
		}
		else if( ( number_of_sessions > 0 )
		      && ( current_sector >= session_first_sector )
		      && ( current_sector < session_last_sector ) )
		{
			if( ( track_last_sector == 0 )
			 || ( track_last_sector < session_first_sector ) ) 
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: entry: %02" PRIu32 " type\t\t\t\t: 0\n",
					 function,
					 sessions_entry_index );

					libnotify_printf(
					 "%s: entry: %02" PRIu32 " first sector\t\t\t: %" PRIu32 "\n",
					 function,
					 sessions_entry_index,
					 session_first_sector );

					libnotify_printf(
					 "%s: entry: %02" PRIu32 " last sector\t\t\t: %" PRIu32 "\n",
					 function,
					 sessions_entry_index,
					 session_last_sector );

					libnotify_printf(
					 "\n" );
				}
#endif
				/* Note that EnCase says the first session starts at sector 16
				 * This is either some EnCase specific behavior or the value is used for
				 * other purposes.
				 */
				if( ( sessions_entry_index == 0 )
				 && ( session_first_sector == 0 ) )
				{
					session_first_sector = 16;
				}
				byte_stream_copy_from_uint32_little_endian(
				 ( session_entries[ sessions_entry_index ] ).first_sector,
				 (uint32_t) session_first_sector );

				sessions_entry_index++;
			}
			current_sector = session_last_sector;
		}
		if( sessions_entry_index >= number_of_sessions_entries )
		{
			break;
		}
	}
	while( ( session_index < number_of_sessions )
	    || ( track_index < number_of_tracks ) );

	write_count = libbfio_pool_write(
		       file_io_pool,
		       file_io_pool_entry,
		       (uint8_t *) session_entries,
		       session_entries_size,
		       error );

	if( write_count != (ssize_t) session_entries_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write session entries.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	calculated_checksum = ewf_checksum_calculate(
			       session_entries,
			       session_entries_size,
			       1 );

	byte_stream_copy_from_uint32_little_endian(
	 calculated_checksum_buffer,
	 calculated_checksum );

	write_count = libbfio_pool_write(
		       file_io_pool,
		       file_io_pool_entry,
		       calculated_checksum_buffer,
		       sizeof( uint32_t ),
		       error );

	if( write_count != (ssize_t) sizeof( uint32_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write session entries checksum.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	memory_free(
	 session_entries );

	return( total_write_count );

on_error:
	if( session_entries != NULL )
	{
		memory_free(
		 session_entries );
	}
	return( -1 );
}

/* Reads a table section header
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_table_header_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format LIBEWF_ATTRIBUTE_UNUSED,
         uint32_t *number_of_offsets,
         uint64_t *base_offset,
         liberror_error_t **error )
{
	ewf_table_header_t table_header;

	static char *function        = "libewf_section_table_header_read";
	size64_t section_data_size   = 0;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

	LIBEWF_UNREFERENCED_PARAMETER( format )

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( number_of_offsets == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of offsets.",
		 function );

		return( -1 );
	}
	if( base_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base offset.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size < (size64_t) sizeof( ewf_table_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &table_header,
	              sizeof( ewf_table_header_t ),
	              error );
	
	if( read_count != (ssize_t) sizeof( ewf_table_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: table header:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) &table_header,
		 sizeof( ewf_table_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 table_header.number_of_offsets,
	 *number_of_offsets );

	byte_stream_copy_to_uint64_little_endian(
	 table_header.base_offset,
	 *base_offset );

	byte_stream_copy_to_uint32_little_endian(
	 table_header.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: number of offsets\t\t\t: %" PRIu32 "\n",
		 function,
		 *number_of_offsets );

		libnotify_printf(
		 "%s: padding1:\n",
		 function );
		libnotify_print_data(
		 table_header.padding1,
		 4,
		 0 );

		libnotify_printf(
		 "%s: base offset\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 *base_offset );

		libnotify_printf(
		 "%s: padding2:\n",
		 function );
		libnotify_print_data(
		 table_header.padding2,
		 4,
		 0 );

		libnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
		 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &table_header,
	                       sizeof( ewf_table_header_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( *number_of_offsets == 0 )
		{
			libnotify_printf(
			 "%s: table contains no offsets.\n",
			 function );
		}
		else if( ( ( format != LIBEWF_FORMAT_ENCASE6 )
		       &&  ( *number_of_offsets > EWF_MAXIMUM_OFFSETS_IN_TABLE ) )
		      || ( ( format == LIBEWF_FORMAT_ENCASE6 )
		       &&  ( *number_of_offsets > EWF_MAXIMUM_OFFSETS_IN_TABLE_ENCASE6 ) ) )
		{
			libnotify_printf(
			 "%s: number of offsets: %" PRIu32 " exceeds maximum: %d.\n",
			 function,
			 number_of_offsets,
			 EWF_MAXIMUM_OFFSETS_IN_TABLE );
		}
	}
#endif
	return( read_count );
}

/* Writes a table or table2 section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_table_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         off64_t base_offset,
         ewf_table_offset_t *table_offsets,
         uint32_t number_of_offsets,
         size64_t chunks_data_size,
         uint8_t ewf_format,
         liberror_error_t **error )
{
	ewf_table_header_t table_header;
	uint8_t calculated_checksum_buffer[ 4 ];

	static char *function        = "libewf_section_table_write";
	size64_t section_size        = 0;
	size_t table_offsets_size    = 0;
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;
	uint8_t write_checksum       = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( type_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type string.",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid base offset value less than zero.",
		 function );

		return( -1 );
	}
	if( table_offsets == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table offsets.",
		 function );

		return( -1 );
	}
	table_offsets_size = sizeof( ewf_table_offset_t ) * number_of_offsets;

	section_size = sizeof( ewf_section_start_t )
	             + sizeof( ewf_table_header_t )
	             + table_offsets_size
	             + chunks_data_size;

	if( ewf_format != EWF_FORMAT_S01 )
	{
		write_checksum = 1;

		section_size += sizeof( uint32_t );
	}
	if( libewf_section_set_values(
	     section,
	     type_string,
	     type_string_length,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start: %s.",
		 function,
		 (char *) type_string );

		return( -1 );
	}
	total_write_count += write_count;

	if( memory_set(
	     &table_header,
	     0,
	     sizeof( ewf_table_header_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table header.",
		 function );

		return( -1 );
	}
	byte_stream_copy_from_uint32_little_endian(
	 table_header.number_of_offsets,
	 number_of_offsets );

	byte_stream_copy_from_uint64_little_endian(
	 table_header.base_offset,
	 base_offset );

	calculated_checksum = ewf_checksum_calculate(
	                       &table_header,
	                       sizeof( ewf_table_header_t ) - sizeof( uint32_t ),
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 table_header.checksum,
	 calculated_checksum );

	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &table_header,
	               sizeof( ewf_table_header_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_table_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write table header.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) table_offsets,
	               table_offsets_size,
	               error );

	if( write_count != (ssize_t) table_offsets_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write table offsets.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( write_checksum != 0 )
	{
		calculated_checksum = ewf_checksum_calculate(
		                       table_offsets,
		                       table_offsets_size,
		                       1 );

		byte_stream_copy_from_uint32_little_endian(
		 calculated_checksum_buffer,
		 calculated_checksum );

		write_count = libbfio_pool_write(
		               file_io_pool,
		               file_io_pool_entry,
	        	       calculated_checksum_buffer,
	        	       sizeof( uint32_t ),
		               error );

		if( write_count != (ssize_t) sizeof( uint32_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write table offsets checksum.",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Reads a volume section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_volume_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         liberror_error_t **error )
{
	static char *function      = "libewf_section_volume_read";
	ssize_t read_count         = 0;
	size64_t bytes_per_chunk   = 0;
	size64_t section_data_size = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( ( section_data_size != (size64_t) sizeof( ewf_volume_t ) )
	 && ( section_data_size != (size64_t) sizeof( ewf_volume_smart_t ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	if( section_data_size == (size64_t) sizeof( ewf_volume_t ) )
	{
		read_count = libewf_section_volume_e01_read(
		              section,
		              io_handle,
		              file_io_pool,
		              file_io_pool_entry,
		              media_values,
		              error );

		if( read_count != (ssize_t) section_data_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read EWF-E01 volume section.",
			 function );

			return( -1 );
		}
	}
	else if( section_data_size == (size64_t) sizeof( ewf_volume_smart_t ) )
	{
		read_count = libewf_section_volume_s01_read(
		              section,
		              io_handle,
		              file_io_pool,
		              file_io_pool_entry,
		              media_values,
		              error );

		if( read_count != (ssize_t) section_data_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read EWF-S01 volume section.",
			 function );

			return( -1 );
		}
	}
	if( media_values->number_of_chunks > (uint32_t) INT_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of chunks value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( media_values->sectors_per_chunk > (uint32_t) INT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid sectors per chunk value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( media_values->bytes_per_sector > (uint32_t) INT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid bytes per sector value exceeds maximum.",
		 function );

		return( -1 );
	}
	bytes_per_chunk = (size64_t) media_values->sectors_per_chunk
	                * (size64_t) media_values->bytes_per_sector;

	if( bytes_per_chunk > (size64_t) INT32_MAX )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: chunk size value exceeds maximum defaulting to: %d.\n",
			 function,
			 EWF_MINIMUM_CHUNK_SIZE );
		}
#endif
		bytes_per_chunk = (size64_t) EWF_MINIMUM_CHUNK_SIZE;
	}
	media_values->chunk_size = (uint32_t) bytes_per_chunk;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
		 function,
		 media_values->number_of_chunks,
		 media_values->chunk_size,
		 media_values->sectors_per_chunk );

		libnotify_printf(
		 "%s: volume has %" PRIu64 " sectors of %" PRIi32 " bytes each.\n",
		 function,
		 media_values->number_of_sectors,
		 media_values->bytes_per_sector );
	}
#endif
	return( read_count );
}

/* Reads an EWF-E01 (EnCase) volume section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_volume_e01_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         liberror_error_t **error )
{
	ewf_volume_t *volume         = NULL;
	static char *function        = "libewf_io_handle_read_volume_e01_section";
	size64_t section_data_size   = 0;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit         = 0;
#endif

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size != (size64_t) sizeof( ewf_volume_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	volume = memory_allocate_structure(
	          ewf_volume_t );

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to read volume.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) volume,
	              sizeof( ewf_volume_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_volume_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: volume data:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) volume,
		 sizeof( ewf_volume_t ),
		 0 );
	}
#endif
	media_values->media_type = volume->media_type;

	byte_stream_copy_to_uint32_little_endian(
	 volume->number_of_chunks,
	 media_values->number_of_chunks );

	byte_stream_copy_to_uint32_little_endian(
	 volume->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	byte_stream_copy_to_uint32_little_endian(
	 volume->bytes_per_sector,
	 media_values->bytes_per_sector );

	byte_stream_copy_to_uint64_little_endian(
	 volume->number_of_sectors,
	 media_values->number_of_sectors );

	media_values->media_flags = volume->media_flags;

	io_handle->compression_level = (int8_t) volume->compression_level;

	byte_stream_copy_to_uint32_little_endian(
	 volume->error_granularity,
	 media_values->error_granularity );

	if( memory_copy(
	     media_values->guid,
	     volume->guid,
	     16 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set GUID.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 volume->checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: media type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_type );

		libnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libnotify_print_data(
		 volume->unknown1,
		 3,
		 0 );

		libnotify_printf(
		 "%s: number of chunks\t\t: %" PRIu32 "\n",
		 function,
		 media_values->number_of_chunks );

		libnotify_printf(
		 "%s: sectors per chunk\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libnotify_printf(
		 "%s: bytes per sector\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libnotify_printf(
		 "%s: number of sectors\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		byte_stream_copy_to_uint32_little_endian(
		 volume->chs_cylinders,
		 value_32bit );
		libnotify_printf(
		 "%s: CHS number of cylinders\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 volume->chs_heads,
		 value_32bit );
		libnotify_printf(
		 "%s: CHS number of heads\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 volume->chs_sectors,
		 value_32bit );
		libnotify_printf(
		 "%s: CHS number of sectors\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: media flags\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_flags );

		libnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libnotify_print_data(
		 volume->unknown2,
		 3,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 volume->palm_volume_start_sector,
		 value_32bit );
		libnotify_printf(
		 "%s: PALM volume start sector\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libnotify_print_data(
		 volume->unknown3,
		 4,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 volume->smart_logs_start_sector,
		 value_32bit );
		libnotify_printf(
		 "%s: SMART logs start sector\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: compression level\t\t: 0x%02" PRIx8 "\n",
		 function,
		 io_handle->compression_level );

		libnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libnotify_print_data(
		 volume->unknown4,
		 3,
		 0 );

		libnotify_printf(
		 "%s: error granularity\t\t: %" PRIu32 "\n",
		 function,
		 media_values->error_granularity );

		libnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libnotify_print_data(
		 volume->unknown5,
		 4,
		 0 );

		libnotify_printf(
		 "%s: GUID:\n",
		 function );
		libnotify_print_data(
		 volume->guid,
		 16,
		 0 );

		libnotify_printf(
		 "%s: unknown6:\n",
		 function );
		libnotify_print_data(
		 volume->unknown6,
		 963,
		 0 );

		libnotify_printf(
		 "%s: signature:\n",
		 function );
		libnotify_print_data(
		 volume->signature,
		 5,
		 0 );

		libnotify_printf(
		 "%s: checksum\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
		 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       volume,
	                       sizeof( ewf_volume_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	memory_free(
	 volume );

	if( media_values->media_type == LIBEWF_MEDIA_TYPE_SINGLE_FILES )
	{
		io_handle->ewf_format = EWF_FORMAT_L01;
	}
	else
	{
		io_handle->ewf_format = EWF_FORMAT_E01;
	}
	return( read_count );

on_error:
	if( volume != NULL )
	{
		memory_free(
		 volume );
	}
	return( -1 );
}

/* Writes an EWF-E01 (EnCase) volume section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_volume_e01_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         liberror_error_t **error )
{
	ewf_volume_t *volume         = NULL;
	static char *function        = "libewf_section_volume_e01_write";
	size64_t section_size        = 0;
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	section_size = sizeof( ewf_section_start_t )
	             + sizeof( ewf_volume_t );

	if( libewf_section_set_values(
	     section,
	     (uint8_t *) "volume",
	     6,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	volume = memory_allocate_structure(
	          ewf_volume_t );

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     volume,
	     0,
	     sizeof( ewf_volume_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: media type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_type );

		libnotify_printf(
		 "%s: number of chunks\t\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->number_of_chunks );

		libnotify_printf(
		 "%s: sectors per chunk\t\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libnotify_printf(
		 "%s: bytes per sector\t\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libnotify_printf(
		 "%s: number of sectors\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		libnotify_printf(
		 "%s: media flags\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_flags );

		libnotify_printf(
		 "%s: compression level\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 io_handle->compression_level );

		libnotify_printf(
		 "%s: error granularity\t\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->error_granularity );

		libnotify_printf(
		 "%s: GUID:\n",
		 function );
		libnotify_print_data(
		 media_values->guid,
		 16,
		 0 );

		libnotify_printf(
		 "\n" );
	}
#endif
	volume->media_type = media_values->media_type;
	volume->media_flags = media_values->media_flags;

	byte_stream_copy_from_uint32_little_endian(
	 volume->number_of_chunks,
	 media_values->number_of_chunks );

	byte_stream_copy_from_uint32_little_endian(
	 volume->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	byte_stream_copy_from_uint32_little_endian(
	 volume->bytes_per_sector,
	 media_values->bytes_per_sector );

	byte_stream_copy_from_uint64_little_endian(
	 volume->number_of_sectors,
	 media_values->number_of_sectors );

	if( ( io_handle->format == LIBEWF_FORMAT_ENCASE5 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
	 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( io_handle->format == LIBEWF_FORMAT_EWFX ) )
	{
		volume->compression_level = (uint8_t) io_handle->compression_level;

		if( memory_copy(
		     volume->guid,
		     media_values->guid,
		     16 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set GUID.",
			 function );

			goto on_error;
		}
		byte_stream_copy_from_uint32_little_endian(
		 volume->error_granularity,
		 media_values->error_granularity );
	}
	calculated_checksum = ewf_checksum_calculate(
	                       volume,
	                       sizeof( ewf_volume_t ) - sizeof( uint32_t ),
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 volume->checksum,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
		 function,
		 media_values->number_of_chunks,
		 media_values->chunk_size,
		 media_values->sectors_per_chunk );

		libnotify_printf(
		 "%s: volume has %" PRIu64 " sectors of %" PRIi32 " bytes each.\n",
		 function,
		 media_values->number_of_sectors,
		 media_values->bytes_per_sector );
	}
#endif
	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) volume,
	               sizeof( ewf_volume_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_volume_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write volume.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	memory_free(
	 volume );

	return( total_write_count );

on_error:
	if( volume != NULL )
	{
		memory_free(
		 volume );
	}
	return( -1 );
}

/* Reads an EWF-S01 (SMART) volume section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_volume_s01_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         liberror_error_t **error )
{
	ewf_volume_smart_t *volume   = NULL;
	static char *function        = "libewf_io_handle_read_volume_s01_section";
	size64_t section_data_size   = 0;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size != (size64_t) sizeof( ewf_volume_smart_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	volume = memory_allocate_structure(
	          ewf_volume_smart_t );

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) volume,
	              sizeof( ewf_volume_smart_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_volume_smart_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: volume data:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) volume,
		 sizeof( ewf_volume_smart_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 volume->number_of_chunks,
	 media_values->number_of_chunks );

	byte_stream_copy_to_uint32_little_endian(
	 volume->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	byte_stream_copy_to_uint32_little_endian(
	 volume->bytes_per_sector,
	 media_values->bytes_per_sector );

	byte_stream_copy_to_uint32_little_endian(
	 volume->number_of_sectors,
	 media_values->number_of_sectors );

	byte_stream_copy_to_uint32_little_endian(
	 volume->checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libnotify_print_data(
		 volume->unknown1,
		 4,
		 0 );

		libnotify_printf(
		 "%s: number of chunks\t: %" PRIu32 "\n",
		 function,
		 media_values->number_of_chunks );

		libnotify_printf(
		 "%s: sectors per chunk\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libnotify_printf(
		 "%s: bytes per sector\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libnotify_printf(
		 "%s: number of sectors\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		libnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libnotify_print_data(
		 volume->unknown2,
		 20,
		 0 );

		libnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libnotify_print_data(
		 volume->unknown3,
		 45,
		 0 );

		libnotify_printf(
		 "%s: signature:\n",
		 function );
		libnotify_print_data(
		 volume->signature,
		 5,
		 0 );

		libnotify_printf(
		 "%s: checksum\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
		 "\n" );
	}
#endif
	if( memory_compare(
	     (void *) volume->signature,
	     (void *) "SMART",
	     5 ) == 0 )
	{
		io_handle->format = LIBEWF_FORMAT_SMART;
	}
	else
	{
		io_handle->format = LIBEWF_FORMAT_EWF;
	}
	calculated_checksum = ewf_checksum_calculate(
	                       volume,
	                       sizeof( ewf_volume_smart_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	memory_free(
	 volume );

	io_handle->ewf_format = EWF_FORMAT_S01;

	return( read_count );

on_error:
	if( volume != NULL )
	{
		memory_free(
		 volume );
	}
	return( -1 );
}

/* Writes an EWF-S01 (SMART) volume section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_volume_s01_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         liberror_error_t **error )
{
	ewf_volume_smart_t *volume   = NULL;
	static char *function        = "libewf_section_volume_s01_write";
	size64_t section_size        = 0;
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	section_size = sizeof( ewf_section_start_t )
	             + sizeof( ewf_volume_smart_t );

	if( libewf_section_set_values(
	     section,
	     (uint8_t *) "volume",
	     6,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	volume = memory_allocate_structure(
	          ewf_volume_smart_t );

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     volume,
	     0,
	     sizeof( ewf_volume_smart_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: media type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_type );

		libnotify_printf(
		 "%s: number of chunks\t\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->number_of_chunks );

		libnotify_printf(
		 "%s: sectors per chunk\t\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libnotify_printf(
		 "%s: bytes per sector\t\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libnotify_printf(
		 "%s: number of sectors\t\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		libnotify_printf(
		 "\n" );
	}
#endif
	volume->unknown1[ 0 ] = 1;

	byte_stream_copy_from_uint32_little_endian(
	 volume->number_of_chunks,
	 media_values->number_of_chunks );

	byte_stream_copy_from_uint32_little_endian(
	 volume->sectors_per_chunk,
	 media_values->sectors_per_chunk );

	byte_stream_copy_from_uint32_little_endian(
	 volume->bytes_per_sector,
	 media_values->bytes_per_sector );

	byte_stream_copy_from_uint32_little_endian(
	 volume->number_of_sectors,
	 media_values->number_of_sectors );

	if( io_handle->format == LIBEWF_FORMAT_SMART )
	{
		volume->signature[ 0 ] = (uint8_t) 'S';
		volume->signature[ 1 ] = (uint8_t) 'M';
		volume->signature[ 2 ] = (uint8_t) 'A';
		volume->signature[ 3 ] = (uint8_t) 'R';
		volume->signature[ 4 ] = (uint8_t) 'T';
	}
	calculated_checksum = ewf_checksum_calculate(
	                       volume,
	                       sizeof( ewf_volume_smart_t ) - sizeof( uint32_t ),
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 volume->checksum,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: volume has %" PRIu32 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
		 function,
		 media_values->number_of_chunks,
		 media_values->chunk_size,
		 media_values->sectors_per_chunk );

		libnotify_printf(
		 "%s: volume has %" PRIu64 " sectors of %" PRIi32 " bytes each.\n",
		 function,
		 media_values->number_of_sectors,
		 media_values->bytes_per_sector );
	}
#endif
	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) volume,
	               sizeof( ewf_volume_smart_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_volume_smart_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write volume.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	memory_free(
	 volume );

	return( total_write_count );

on_error:
	if( volume != NULL )
	{
		memory_free(
		 volume );
	}
	return( -1 );
}

/* Reads a xhash section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_xhash_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error )
{
	uint8_t *xhash        = NULL;
	static char *function = "libewf_section_xhash_read";
	ssize_t read_count    = 0;
	size_t xhash_size     = 0;

	if( hash_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              section,
	              file_io_pool,
	              file_io_pool_entry,
	              &xhash,
	              &xhash_size,
	              error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read xhash.",
		 function );

		goto on_error;
	}
	if( xhash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing xhash.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		 if( libewf_debug_utf8_stream_print(
		      "XHash",
		      xhash,
		      xhash_size,
		      error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print xhash.",
			 function );

			goto on_error;
		}
	}
#endif
	if( hash_sections->xhash == NULL )
	{
		hash_sections->xhash      = xhash;
		hash_sections->xhash_size = xhash_size;
	}
	else
	{
		memory_free(
		 xhash );
	}
	return( read_count );

on_error:
	if( xhash != NULL )
	{
		memory_free(
		 xhash );
	}
	return( -1 );
}

/* Writes a xhash section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_xhash_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         int8_t compression_level,
         liberror_error_t **error )
{
	static char *function = "libewf_section_xhash_write";
	ssize_t write_count   = 0;

	if( hash_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libewf_debug_utf8_stream_print(
		     "XHash",
		     hash_sections->xhash,
		     hash_sections->xhash_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print xhash.",
			 function );

			return( -1 );
		}
	}
#endif
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) "xhash",
	               5,
	               section_offset,
	               hash_sections->xhash,
	               hash_sections->xhash_size - 1,
	               compression_level,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write compressed string.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Reads a xheader section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_xheader_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error )
{
	uint8_t *xheader      = NULL;
	static char *function = "libewf_section_xheader_read";
	ssize_t read_count    = 0;
	size_t xheader_size   = 0;

	if( header_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
	              section,
	              file_io_pool,
	              file_io_pool_entry,
	              &xheader,
	              &xheader_size,
	              error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read xheader.",
		 function );

		goto on_error;
	}
	if( xheader == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing xheader.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libewf_debug_utf8_stream_print(
		     "XHeader",
		     xheader,
		     xheader_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print xheader.",
			 function );

			goto on_error;
		}
	}
#endif
	if( header_sections->xheader == NULL )
	{
		header_sections->xheader      = xheader;
		header_sections->xheader_size = xheader_size;
	}
	else
	{
		memory_free(
		 xheader );
	}
	header_sections->number_of_header_sections += 1;

	return( read_count );

on_error:
	if( xheader != NULL )
	{
		memory_free(
		 xheader );
	}
	return( -1 );
}

/* Writes a xheader section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_xheader_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         liberror_error_t **error )
{
	static char *function = "libewf_section_xheader_write";
	ssize_t write_count   = 0;

	if( header_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libewf_debug_utf8_stream_print(
		     "XHeader",
		     header_sections->xheader,
		     header_sections->xheader_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print xheader.",
			 function );

			return( -1 );
		}
	}
#endif
	/* Do not include the end of string character in the compressed data
	 */
	write_count = libewf_section_write_compressed_string(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) "xheader",
	               7,
	               section_offset,
	               header_sections->xheader,
	               header_sections->xheader_size - 1,
	               compression_level,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write compressed string.",
		 function );

		return( -1 );
	}
	header_sections->number_of_header_sections += 1;

	return( write_count );
}

/* Reads a delta chunk section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_delta_chunk_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint32_t *chunk_index,
         uint32_t *chunk_size,
         liberror_error_t **error )
{
	ewfx_delta_chunk_header_t delta_chunk_header;

	static char *function        = "libewf_section_delta_chunk_read";
	size64_t section_data_size   = 0;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( chunk_index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk index.",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk size.",
		 function );

		return( -1 );
	}
	section_data_size = section->size - sizeof( ewf_section_start_t );

	if( section_data_size < (size64_t) sizeof( ewfx_delta_chunk_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	section_data_size -= sizeof( ewfx_delta_chunk_header_t );

	if( section_data_size > (size64_t) INT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &delta_chunk_header,
	              sizeof( ewfx_delta_chunk_header_t ),
	              error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read delta chunk header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: delta chunk header data:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) &delta_chunk_header,
		 sizeof( ewfx_delta_chunk_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 delta_chunk_header.chunk,
	 *chunk_index );

	byte_stream_copy_to_uint32_little_endian(
	 delta_chunk_header.chunk_size,
	 *chunk_size );

	byte_stream_copy_to_uint32_little_endian(
	 delta_chunk_header.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: chunk\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *chunk_index );

		libnotify_printf(
	 	 "%s: chunk size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *chunk_size );

		libnotify_printf(
	 	 "%s: padding:\n",
		 function );
		libnotify_print_data(
		 delta_chunk_header.padding,
		 6,
		 0 );

		libnotify_printf(
	 	 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
	 	 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &delta_chunk_header,
	                       sizeof( ewfx_delta_chunk_header_t ) - sizeof( uint32_t ),
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: %" PRIu32 " calculated: %" PRIu32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
	if( *chunk_index == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_INVALID_DATA,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	/* The chunk value is stored as + 1 in the file
	 */
	*chunk_index -= 1;

	if( (size64_t) *chunk_size != section_data_size )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: chunk size: %" PRIu32 " does not match size of data in section correcting in: %" PRIu64 ".\n",
			 function,
			 *chunk_size,
			 section_data_size );
		}
#endif
		*chunk_size = (uint32_t) section_data_size;
	}
	return( read_count );
}

/* Writes a delta chunk section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_delta_chunk_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint32_t chunk_index,
         uint8_t *chunk_buffer,
         uint32_t chunk_size,
         uint8_t *checksum_buffer,
         uint32_t *chunk_checksum,
         uint8_t write_checksum,
         liberror_error_t **error )
{
	ewfx_delta_chunk_header_t delta_chunk_header;

	static char *function        = "libewf_section_delta_chunk_write";
	size64_t section_size        = 0;
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;
	uint32_t write_size          = 0;

	if( section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( ( chunk_index + 1 ) > (uint32_t) INT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk index value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( chunk_size > (uint32_t) INT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
	write_size = chunk_size;

	if( write_checksum != 0 )
	{
		write_size += sizeof( uint32_t );
	}
	if( write_size > (uint32_t) INT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid write size value out of bounds.",
		 function );

		return( -1 );
	}
	section_size = sizeof( ewf_section_start_t )
	             + sizeof( ewfx_delta_chunk_header_t )
	             + write_size;

	if( libewf_section_set_values(
	     section,
	     (uint8_t *) "delta_chunk",
	     11,
	     section_offset,
	     section_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_start_write(
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_start_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( memory_set(
	     &delta_chunk_header,
	     0,
	     sizeof( ewfx_delta_chunk_header_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear delta chunk header.",
		 function );

		return( -1 );
	}
	/* The chunk number is stored as + 1 in the file
	 */
	chunk_index += 1;

	byte_stream_copy_from_uint32_little_endian(
	 delta_chunk_header.chunk,
	 chunk_index );

	byte_stream_copy_from_uint32_little_endian(
	 delta_chunk_header.chunk_size,
	 write_size );

	delta_chunk_header.padding[ 0 ] = (uint8_t) 'D';
	delta_chunk_header.padding[ 1 ] = (uint8_t) 'E';
	delta_chunk_header.padding[ 2 ] = (uint8_t) 'L';
	delta_chunk_header.padding[ 3 ] = (uint8_t) 'T';
	delta_chunk_header.padding[ 4 ] = (uint8_t) 'A';

	calculated_checksum = ewf_checksum_calculate(
	                       &delta_chunk_header,
	                       sizeof( ewfx_delta_chunk_header_t ) - sizeof( uint32_t ),
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 delta_chunk_header.checksum,
	 calculated_checksum );

	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &delta_chunk_header,
	               sizeof( ewfx_delta_chunk_header_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewfx_delta_chunk_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk value.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	write_size = chunk_size;

	if( write_checksum != 0 )
	{
		if( checksum_buffer == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid checksum buffer.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint32_little_endian(
		 checksum_buffer,
		 *chunk_checksum );

		if( checksum_buffer == &( chunk_buffer[ chunk_size ] ) )
		{
			write_size += sizeof( uint32_t );

			write_checksum = 0;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( write_checksum == 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( chunk_buffer[ chunk_size - sizeof( uint32_t ) ] ),
			 calculated_checksum );
		}
		else
		{
			calculated_checksum = *chunk_checksum;
		}
		libnotify_printf(
		 "%s: writing uncompressed delta chunk: %" PRIu32 " at offset: %" PRIi64 " with size: %" PRIzu ", with checksum: 0x%08" PRIx32 ".\n",
		 function,
		 chunk_index - 1,
		 section_offset + total_write_count,
		 chunk_size,
		 calculated_checksum );
	}
#endif
	write_count = libbfio_pool_write(
	               file_io_pool,
	               file_io_pool_entry,
	               chunk_buffer,
	               (size_t) write_size,
	               error );

	if( write_count != (ssize_t) write_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( write_checksum != 0 )
	{
		write_count = libbfio_pool_write(
		               file_io_pool,
		               file_io_pool_entry,
			       checksum_buffer,
			       sizeof( uint32_t ),
		               error );

		if( write_count != (ssize_t) sizeof( uint32_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write checksum.",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}

