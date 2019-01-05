/*
 * Section reading/writing functions
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <types.h>

#include "libewf_checksum.h"
#include "libewf_compression.h"
#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_hash_sections.h"
#include "libewf_header_values.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcdata.h"
#include "libewf_libcnotify.h"
#include "libewf_libhmac.h"
#include "libewf_media_values.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"
#include "libewf_sector_range.h"
#include "libewf_single_files.h"
#include "libewf_unused.h"

#include "ewf_data.h"
#include "ewf_digest.h"
#include "ewf_file_header.h"
#include "ewf_hash.h"
#include "ewf_table.h"

/* Tests if a buffer entirely consists of zero values
 * Returns 1 if zero, 0 if not, or -1 on error
 */
int libewf_section_test_zero(
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_test_zero";
	size_t buffer_offset  = 0;

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
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

/* Retrieves the section data offset
 * Returns 1 if successful, 0 if the section contains no data or -1 on error
 */
int libewf_section_get_data_offset(
     libewf_section_descriptor_t *section_descriptor,
     uint8_t format_version,
     off64_t *data_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_get_data_offset";

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data offset.",
		 function );

		return( -1 );
	}
	if( section_descriptor->data_size == 0 )
	{
		return( 0 );
	}
	if( format_version == 1 )
	{
		*data_offset = section_descriptor->start_offset + sizeof( ewf_section_descriptor_v1_t );
	}
	else
	{
		*data_offset = section_descriptor->start_offset;
	}
	return( 1 );
}

/* Sets the section values
 * Returns 1 if successful or -1 on error
 */
int libewf_section_set_values(
     libewf_section_descriptor_t *section_descriptor,
     uint32_t type,
     const uint8_t *type_string,
     size_t type_string_length,
     off64_t section_offset,
     size64_t section_size,
     size64_t data_size,
     uint32_t padding_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_set_values";

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( ( section_offset < 0 )
	 || ( section_offset > (off64_t) INT64_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( data_size > (size64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( (size64_t) padding_size > data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid padding size value exceeds data size.",
		 function );

		return( -1 );
	}
	if( type_string != NULL )
	{
		if( ( type_string_length == 0 )
		 || ( type_string_length > 16 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid type string length value out of bounds.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     section_descriptor->type_string,
		     type_string,
		     type_string_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy type string.",
			 function );

			return( -1 );
		}
		section_descriptor->type_string[ type_string_length ] = 0;

		section_descriptor->type_string_length = type_string_length;
	}
	else
	{
		if( memory_set(
		     section_descriptor->type_string,
		     0,
		     17 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set type string.",
			 function );

			return( -1 );
		}
		section_descriptor->type_string_length = 0;
	}
	section_descriptor->type         = type;
	section_descriptor->start_offset = section_offset;
	section_descriptor->end_offset   = (off64_t) ( section_offset + section_size );
	section_descriptor->size         = section_size;
	section_descriptor->data_size    = data_size;
	section_descriptor->padding_size = padding_size;

	return( 1 );
}

/* Reads a section descriptor
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_descriptor_read(
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t file_offset,
         uint8_t format_version,
         libcerror_error_t **error )
{
	uint8_t *section_descriptor_data    = NULL;
	static char *function               = "libewf_section_descriptor_read";
	size_t section_descriptor_data_size = 0;
	ssize_t read_count                  = 0;
	uint32_t calculated_checksum        = 0;
	uint32_t section_descriptor_size    = 0;
	uint32_t stored_checksum            = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading section descriptor from file IO pool entry: %d at offset: 0x%08" PRIx64 "\n",
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek section descriptor offset: %" PRIi64 " in file IO pool entry: %d.",
		 function,
		 file_offset,
		 file_io_pool_entry );

		goto on_error;
	}
	section_descriptor_data = (uint8_t *) memory_allocate(
	                                       section_descriptor_data_size );

	if( section_descriptor_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section descriptor data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              section_descriptor_data,
	              section_descriptor_data_size,
	              error );

	if( read_count != (ssize_t) section_descriptor_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section descriptor from file IO pool entry: %d.",
		 function,
		 file_io_pool_entry );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: section descriptor data:\n",
		 function );
		libcnotify_print_data(
		 section_descriptor_data,
		 section_descriptor_data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		if( memory_copy(
		     section_descriptor->type_string,
		     ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->type_string,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set type string.",
			 function );

			goto on_error;
		}
		section_descriptor->type_string[ 16 ] = 0;

		section_descriptor->type_string_length = narrow_string_length(
		                                          (char *) section_descriptor->type_string );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->size,
		 section_descriptor->size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->next_offset,
		 section_descriptor->end_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->type,
		 section_descriptor->type );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_flags,
		 section_descriptor->data_flags );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->previous_offset,
		 section_descriptor->start_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_size,
		 section_descriptor->data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->padding_size,
		 section_descriptor->padding_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->descriptor_size,
		 section_descriptor_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->checksum,
		 stored_checksum );

		if( memory_copy(
		     section_descriptor->data_integrity_hash,
		     ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_integrity_hash,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set data integrity hash.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: type string\t\t\t\t: %s\n",
			 function,
			 (char *) section_descriptor->type_string );

			libcnotify_printf(
			 "%s: next offset\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 section_descriptor->end_offset );

			libcnotify_printf(
			 "%s: size\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section_descriptor->size );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->padding,
			 40,
			 0 );
		}
		else if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: type\t\t\t\t\t: 0x%08" PRIx32 " (",
			 function,
			 section_descriptor->type );
			libewf_debug_print_section_type(
			 section_descriptor->type );
			libcnotify_printf(
			 ")\n" );

			libcnotify_printf(
			 "%s: data flags\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 section_descriptor->data_flags );
			libewf_debug_print_section_data_flags(
			 section_descriptor->data_flags );
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "%s: previous offset\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 section_descriptor->start_offset );

			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section_descriptor->data_size );

			libcnotify_printf(
			 "%s: section descriptor size\t\t\t: %" PRIu32 "\n",
			 function,
			 section_descriptor_size );

			libcnotify_printf(
			 "%s: padding size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 section_descriptor->padding_size );

			libcnotify_printf(
			 "%s: data integrity hash:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_integrity_hash,
			 16,
			 0 );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->padding,
			 12,
			 0 );
		}
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     section_descriptor_data,
	     section_descriptor_data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate checksum.",
		 function );

		goto on_error;
	}
	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	memory_free(
	 section_descriptor_data );

	section_descriptor_data = NULL;

	if( format_version == 1 )
	{
		if( ( section_descriptor->end_offset < file_offset )
		 || ( section_descriptor->end_offset > (off64_t) INT64_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section next offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( section_descriptor->size != 0 )
		 && ( ( section_descriptor->size < (size64_t) sizeof( ewf_section_descriptor_v1_t ) )
		  ||  ( section_descriptor->size > (size64_t) INT64_MAX ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds.",
			 function );

			goto on_error;
		}
		section_descriptor->start_offset = file_offset;

		/* Some versions of EWF1 do not set the section size
		 * The next and done section_descriptor, which point back to themselves, are not corrected
		 */
		if( ( section_descriptor->size == 0 )
		 && ( section_descriptor->end_offset != section_descriptor->start_offset ) )
		{
			section_descriptor->size = section_descriptor->end_offset - section_descriptor->start_offset;
		}
		if( section_descriptor->size != 0 )
		{
			section_descriptor->data_size = section_descriptor->size - sizeof( ewf_section_descriptor_v1_t );
		}
	}
	else if( format_version == 2 )
	{
		if( ( section_descriptor->start_offset < 0 )
		 || ( ( section_descriptor->start_offset != 0 )
		  &&  ( section_descriptor->start_offset < (off64_t) sizeof( ewf_file_header_v2_t ) ) )
		 || ( section_descriptor->start_offset >= file_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section previous offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( section_descriptor->start_offset == 0 )
		{
			section_descriptor->start_offset = sizeof( ewf_file_header_v2_t );
		}
		else
		{
			section_descriptor->start_offset += sizeof( ewf_section_descriptor_v2_t );
		}
		section_descriptor->end_offset = file_offset + sizeof( ewf_section_descriptor_v2_t );
		section_descriptor->size       = (size64_t) ( section_descriptor->end_offset - section_descriptor->start_offset );

		if( section_descriptor->data_size > section_descriptor->size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( section_descriptor->padding_size > section_descriptor->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section padding size value out of bounds.",
			 function );

			goto on_error;
		}
	}
	if( format_version == 1 )
	{
		section_descriptor->type = 0;

		if( section_descriptor->type_string_length == 4 )
		{
			if( memory_compare(
			     section_descriptor->type_string,
			     "done",
			     4 ) == 0 )
			{
				section_descriptor->type = LIBEWF_SECTION_TYPE_DONE;
			}
			else if( memory_compare(
			          section_descriptor->type_string,
			          "hash",
			          4 ) == 0 )
			{
				section_descriptor->type = LIBEWF_SECTION_TYPE_MD5_HASH;
			}
			else if( memory_compare(
			          section_descriptor->type_string,
			          "next",
			          4 ) == 0 )
			{
				section_descriptor->type = LIBEWF_SECTION_TYPE_NEXT;
			}
		}
		else if( section_descriptor->type_string_length == 5 )
		{
			if( memory_compare(
			     section_descriptor->type_string,
			     "ltree",
			     5 ) == 0 )
			{
				section_descriptor->type = LIBEWF_SECTION_TYPE_SINGLE_FILES_DATA;
			}
			else if( memory_compare(
			          section_descriptor->type_string,
			          "table",
			          5 ) == 0 )
			{
				section_descriptor->type = LIBEWF_SECTION_TYPE_SECTOR_TABLE;
			}
		}
		else if( section_descriptor->type_string_length == 6 )
		{
			if( memory_compare(
			     section_descriptor->type_string,
			     "error2",
			     6 ) == 0 )
			{
				section_descriptor->type = LIBEWF_SECTION_TYPE_ERROR_TABLE;
			}
		}
		else if( section_descriptor->type_string_length == 7 )
		{
			if( memory_compare(
			     section_descriptor->type_string,
			     "sectors",
			     7 ) == 0 )
			{
				section_descriptor->type = LIBEWF_SECTION_TYPE_SECTOR_DATA;
			}
			else if( memory_compare(
			          section_descriptor->type_string,
			          "session",
			          7 ) == 0 )
			{
				section_descriptor->type = LIBEWF_SECTION_TYPE_SESSION_TABLE;
			}
		}
		if( section_descriptor->size != 0 )
		{
			/* Make sure to check if the section next value is sane
			 * the end offset of the next and done sections point back at themselves
			 */
			if( section_descriptor->end_offset == section_descriptor->start_offset )
			{
				if( ( section_descriptor->type != LIBEWF_SECTION_TYPE_DONE )
				 && ( section_descriptor->type != LIBEWF_SECTION_TYPE_NEXT ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: mismatch in next section offset (stored: %" PRIi64 ", calculated: %" PRIi64 ").",
					 function,
					 section_descriptor->end_offset,
					 section_descriptor->start_offset );

					goto on_error;
				}
				if( section_descriptor->size != sizeof( ewf_section_descriptor_v1_t ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid section size value out of bounds.",
					 function );

					goto on_error;
				}
			}
			else
			{
				file_offset += (off64_t) section_descriptor->size;

				if( section_descriptor->end_offset != file_offset )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: mismatch in next section offset (stored: %" PRIi64 ", calculated: %" PRIi64 ").",
					 function,
					 section_descriptor->end_offset,
					 file_offset );

					goto on_error;
				}
			}
		}
		else
		{
			/* Make sure to check if the section next value is sane
			 * the end offset of the next and done sections point back at themselves
			 */
			if( section_descriptor->end_offset == section_descriptor->start_offset )
			{
				if( ( section_descriptor->type != LIBEWF_SECTION_TYPE_DONE )
				 && ( section_descriptor->type != LIBEWF_SECTION_TYPE_NEXT ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
					 "%s: mismatch in next section offset (stored: %" PRIi64 ", calculated: %" PRIi64 ").",
					 function,
					 section_descriptor->end_offset,
					 section_descriptor->start_offset );

					goto on_error;
				}
				section_descriptor->size = (size64_t) sizeof( ewf_section_descriptor_v1_t );
			}
			else
			{
				file_offset += (off64_t) sizeof( ewf_section_descriptor_v1_t );

				if( section_descriptor->end_offset < file_offset )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid section next offset value out of bounds.",
					 function );

					goto on_error;
				}
				section_descriptor->size = (size64_t) ( section_descriptor->end_offset - section_descriptor->start_offset );
			}
		}
	}
	else if( format_version == 2 )
	{
		if( (size_t) section_descriptor_size != sizeof( ewf_section_descriptor_v2_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: mismatch in section descriptor size.",
			 function );

			goto on_error;
		}
	}
	return( read_count );

on_error:
	if( section_descriptor_data != NULL )
	{
		memory_free(
		 section_descriptor_data );
	}
	return( -1 );
}

/* Writes a section descriptor
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_descriptor_write(
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libcerror_error_t **error )
{
	uint8_t *section_descriptor_data    = NULL;
	static char *function               = "libewf_section_descriptor_write";
	off64_t previous_offset             = 0;
	size_t section_descriptor_data_size = 0;
	ssize_t write_count                 = 0;
	uint32_t calculated_checksum        = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	section_descriptor_data = (uint8_t *) memory_allocate(
	                                       section_descriptor_data_size );

	if( section_descriptor_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section descriptor data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     section_descriptor_data,
	     0,
	     section_descriptor_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear section descriptor data.",
		 function );

		goto on_error;
	}
	if( format_version == 1 )
	{
		if( memory_copy(
		     ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->type_string,
		     section_descriptor->type_string,
		     section_descriptor->type_string_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set type string.",
			 function );

			goto on_error;
		}
		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->size,
		 section_descriptor->size );

		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->next_offset,
		 section_descriptor->end_offset );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->type,
		 section_descriptor->type );

		if( section_descriptor->start_offset > (off64_t) sizeof( ewf_section_descriptor_v2_t ) )
		{
			previous_offset = section_descriptor->start_offset - sizeof( ewf_section_descriptor_v2_t );
		}
		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->previous_offset,
		 previous_offset );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_flags,
		 section_descriptor->data_flags );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_size,
		 section_descriptor->data_size );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->descriptor_size,
		 (uint32_t) section_descriptor_data_size );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->padding_size,
		 section_descriptor->padding_size );

		if( memory_copy(
		     ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_integrity_hash,
		     section_descriptor->data_integrity_hash,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set data integrity hash.",
			 function );

			goto on_error;
		}
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     section_descriptor_data,
	     section_descriptor_data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate checksum.",
		 function );

		goto on_error;
	}
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->checksum,
		 calculated_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: section descriptor data:\n",
		 function );
		libcnotify_print_data(
		 section_descriptor_data,
		 section_descriptor_data_size,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: type string\t\t\t\t: %s\n",
			 function,
			 (char *) section_descriptor->type_string );

			libcnotify_printf(
			 "%s: next offset\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 section_descriptor->end_offset );

			libcnotify_printf(
			 "%s: size\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section_descriptor->size );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->padding,
			 40,
			 0 );
		}
		else if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: type\t\t\t\t\t: 0x%08" PRIx32 " (",
			 function,
			 section_descriptor->type );
			libewf_debug_print_section_type(
			 section_descriptor->type );
			libcnotify_printf(
			 ")\n" );

			libcnotify_printf(
			 "%s: data flags\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 section_descriptor->data_flags );
			libewf_debug_print_section_data_flags(
			 section_descriptor->data_flags );
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "%s: previous offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 previous_offset );

			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section_descriptor->data_size );

			libcnotify_printf(
			 "%s: section descriptor size\t\t: %" PRIzd "\n",
			 function,
			 section_descriptor_data_size );

			libcnotify_printf(
			 "%s: padding size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 section_descriptor->padding_size );

			libcnotify_printf(
			 "%s: data integrity hash:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_integrity_hash,
			 16,
			 0 );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->padding,
			 12,
			 0 );
		}
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 calculated_checksum );

		libcnotify_printf(
		 "\n" );
	}
#endif
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               section_descriptor_data,
	               section_descriptor_data_size,
	               error );

	if( write_count != (ssize_t) section_descriptor_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section descriptor data.",
		 function );

		goto on_error;
	}
	memory_free(
	 section_descriptor_data );

	return( write_count );

on_error:
	if( section_descriptor_data != NULL )
	{
		memory_free(
		 section_descriptor_data );
	}
	return( -1 );
}

/* Reads the data of a section
 * The data is decrypted if necessary
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_read_data(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **section_data,
         size_t *section_data_size,
         libcerror_error_t **error )
{
	uint8_t calculated_md5_hash[ 16 ];

	static char *function = "libewf_section_read_data";
	ssize_t read_count    = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( section_descriptor->data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section data.",
		 function );

		return( -1 );
	}
	if( *section_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid section data value already set.",
		 function );

		return( -1 );
	}
	if( section_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section data size.",
		 function );

		return( -1 );
	}
	*section_data_size = (size_t) section_descriptor->data_size;

	*section_data = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * *section_data_size );

	if( *section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              *section_data,
	              *section_data_size,
	              error );

	if( read_count != (ssize_t) *section_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	if( ( section_descriptor->data_flags & LIBEWF_SECTION_DATA_FLAGS_HAS_INTEGRITY_HASH ) != 0 )
	{
		if( libhmac_md5_calculate(
		     *section_data,
		     *section_data_size,
		     calculated_md5_hash,
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate integrity hash.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: calculated MD5 hash:\n",
			 function );
			libcnotify_print_data(
			 calculated_md5_hash,
			 16,
			 0 );
		}
#endif
		if( memory_compare(
		     section_descriptor->data_integrity_hash,
		     calculated_md5_hash,
		     16 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: mismatch in integrity hash.",
			 function );

			goto on_error;
		}
	}
	if( ( section_descriptor->data_flags & LIBEWF_SECTION_DATA_FLAGS_IS_ENCRYPTED ) != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: encrypted data:\n",
			 function );
			libcnotify_print_data(
			 *section_data,
			 *section_data_size,
			 0 );
		}
#endif
/* TODO decrypt */
		memory_free(
		 *section_data );

		*section_data      = NULL;
		*section_data_size = 0;

		return( 0 );
	}
	return( read_count );

on_error:
	if( *section_data != NULL )
	{
		memory_free(
		 *section_data );

		*section_data = NULL;
	}
	*section_data_size = 0;

	return( -1 );
}

/* Writes the data of a section
 * The data is decrypted if necessary
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_write_data(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         const uint8_t *section_data,
         size_t section_data_size,
         libcerror_error_t **error )
{
	static char *function = "libewf_section_write_data";
	ssize_t write_count   = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( section_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section data size value exceeds maximum.",
		 function );

		return( -1 );
	}
/* TODO MD5 hash */
	if( ( section_descriptor->data_flags & LIBEWF_SECTION_DATA_FLAGS_IS_ENCRYPTED ) != 0 )
	{
/* TODO encrypt */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: encrypted data:\n",
			 function );
			libcnotify_print_data(
			 section_data,
			 section_data_size,
			 0 );
		}
#endif
		return( 0 );
	}
	if( ( section_descriptor->data_flags & LIBEWF_SECTION_DATA_FLAGS_HAS_INTEGRITY_HASH ) != 0 )
	{
		if( libhmac_md5_calculate(
		     section_data,
		     section_data_size,
		     section_descriptor->data_integrity_hash,
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate integrity hash.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: calculated MD5 hash:\n",
			 function );
			libcnotify_print_data(
			 section_descriptor->data_integrity_hash,
			 16,
			 0 );
		}
#endif
	}
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               section_data,
	               section_data_size,
	               error );

	if( write_count != (ssize_t) section_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section data.",
		 function );

		goto on_error;
	}
	return( write_count );

on_error:
/* TODO free encrypted data */
	return( -1 );
}

/* Reads a compressed string section and decompresses it
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_compressed_string_read(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint16_t compression_method,
         uint8_t **uncompressed_string,
         size_t *uncompressed_string_size,
         libcerror_error_t **error )
{
	uint8_t *section_data      = NULL;
	static char *function      = "libewf_section_compressed_string_read";
	void *reallocation         = NULL;
	size_t section_data_size   = 0;
	ssize_t read_count         = 0;
	uint8_t number_of_attempts = 0;
	int result                 = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( uncompressed_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed string.",
		 function );

		return( -1 );
	}
	if( *uncompressed_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid uncompressed string value already set.",
		 function );

		return( -1 );
	}
	if( uncompressed_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed string size.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_read_data(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              file_io_pool_entry,
	              &section_data,
	              &section_data_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	else if( read_count == 0 )
	{
		return( 0 );
	}
	if( section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing section data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: compressed string:\n",
		 function );
		libcnotify_print_data(
		 section_data,
		 section_data_size,
		 0 );
	}
#endif
	/* On average the uncompressed string will be more than twice as large as the compressed string
	 */
	*uncompressed_string_size = 4 * section_data_size;

	*uncompressed_string = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * *uncompressed_string_size );

	if( *uncompressed_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed string.",
		 function );

		goto on_error;
	}
	result = libewf_decompress_data(
	          section_data,
	          section_data_size,
	          compression_method,
	          *uncompressed_string,
	          uncompressed_string_size,
	          error );

	while( ( result == 0 )
	    && ( *uncompressed_string_size > 0 ) )
	{
		number_of_attempts++;

		reallocation = memory_reallocate(
		                *uncompressed_string,
		                sizeof( uint8_t ) * *uncompressed_string_size );

		if( reallocation == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize uncompressed string.",
			 function );

			goto on_error;
		}
		*uncompressed_string = (uint8_t *) reallocation;

		result = libewf_decompress_data(
		          section_data,
		          section_data_size,
		          compression_method,
		          *uncompressed_string,
		          uncompressed_string_size,
		          error );

		if( number_of_attempts >= 3 )
		{
			break;
		}
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable to decompress string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: uncompressed string:\n",
		 function );
		libcnotify_print_data(
		 *uncompressed_string,
		 *uncompressed_string_size,
		 0 );
	}
#endif
	memory_free(
	 section_data );

	return( read_count );

on_error:
	if( *uncompressed_string != NULL )
	{
		memory_free(
		 *uncompressed_string );

		*uncompressed_string = NULL;
	}
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}

/* Writes a compressed string section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_write_compressed_string(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint32_t type,
         const uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint16_t compression_method,
         int8_t compression_level,
         uint8_t *uncompressed_string,
         size_t uncompressed_string_size,
         size_t fill_size,
         libcerror_error_t **error )
{
	uint8_t *compressed_string          = NULL;
	static char *function               = "libewf_section_write_compressed_string";
	void *reallocation                  = NULL;
	size_t compressed_string_offset     = 0;
	size_t compressed_string_size       = 0;
	size_t padding_size                 = 0;
	size_t section_descriptor_data_size = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	int result                          = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( uncompressed_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed string.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: uncompressed string:\n",
		 function );
		libcnotify_print_data(
		 uncompressed_string,
		 uncompressed_string_size,
		 0 );
	}
#endif
	if( fill_size > uncompressed_string_size )
	{
		compressed_string_size = fill_size;
	}
	else
	{
		compressed_string_size = uncompressed_string_size;
	}
	if( format_version == 2 )
	{
		padding_size = compressed_string_size % 16;

		if( padding_size != 0 )
		{
			padding_size            = 16 - padding_size;
			compressed_string_size += padding_size;
		}
	}
	compressed_string = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * compressed_string_size );

	if( compressed_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed string.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     compressed_string,
	     0,
	     compressed_string_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear compressed string.",
		 function );

		goto on_error;
	}
	result = libewf_compress_data(
	          compressed_string,
	          &compressed_string_size,
	          compression_method,
	          compression_level,
	          uncompressed_string,
	          uncompressed_string_size,
	          error );

	if( result == 0 )
	{
		if( compressed_string_size <= uncompressed_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compressed string size value out of bounds.",
			 function );

			goto on_error;
		}
		if( format_version == 2 )
		{
			padding_size = compressed_string_size % 16;

			if( padding_size != 0 )
			{
				padding_size            = 16 - padding_size;
				compressed_string_size += padding_size;
			}
		}
		reallocation = memory_reallocate(
		                compressed_string,
		                sizeof( uint8_t ) * compressed_string_size );

		if( reallocation == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize compressed string.",
			 function );

			goto on_error;
		}
		compressed_string = (uint8_t *) reallocation;

		if( memory_set(
		     compressed_string,
		     0,
		     compressed_string_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear compressed string.",
			 function );

			goto on_error;
		}
		result = libewf_compress_data(
		          compressed_string,
		          &compressed_string_size,
		          compression_method,
		          compression_level,
		          uncompressed_string,
		          uncompressed_string_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
		 "%s: unable to compress string.",
		 function );

		goto on_error;
	}
/* TODO bzip2 support
	if( compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
	{
		compressed_string_offset = 4;
		compressed_string_size  -= 4;
	}
*/
	if( fill_size != 0 )
	{
		if( compressed_string_size > fill_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compressed string size value exceeds fill size.",
			 function );

			goto on_error;
		}
		padding_size           = fill_size - compressed_string_size;
		compressed_string_size = fill_size;
	}
	else if( format_version == 1 )
	{
		padding_size = 0;
	}
	else if( format_version == 2 )
	{
		padding_size = compressed_string_size % 16;

		if( padding_size != 0 )
		{
			padding_size            = 16 - padding_size;
			compressed_string_size += padding_size;
		}
	}
	if( libewf_section_set_values(
	     section_descriptor,
	     type,
	     type_string,
	     type_string_length,
	     section_offset,
	     (size64_t) ( section_descriptor_data_size + compressed_string_size ),
	     (size64_t) compressed_string_size,
	     (uint32_t) padding_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		goto on_error;
	}
	if( format_version == 1 )
	{
		write_count = libewf_section_descriptor_write(
			       section_descriptor,
			       file_io_pool,
			       file_io_pool_entry,
			       format_version,
			       error );

		if( write_count != (ssize_t) section_descriptor_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write section descriptor.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: compressed string:\n",
		 function );
		libcnotify_print_data(
		 &( compressed_string[ compressed_string_offset ] ),
		 compressed_string_size,
		 0 );
	}
#endif
	write_count = libewf_section_write_data(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               &( compressed_string[ compressed_string_offset ] ),
	               compressed_string_size,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section data.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	memory_free(
	 compressed_string );

	compressed_string = NULL;

	if( format_version == 2 )
	{
		write_count = libewf_section_descriptor_write(
			       section_descriptor,
			       file_io_pool,
			       file_io_pool_entry,
			       format_version,
			       error );

		if( write_count != (ssize_t) section_descriptor_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write section descriptor.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
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
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         int *set_identifier_change,
         libcerror_error_t **error )
{
	uint8_t *section_data        = NULL;
	static char *function        = "libewf_section_data_read";
	size_t section_data_size     = 0;
	ssize_t read_count           = 0;
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

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( set_identifier_change == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid set identifier change.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_read_data(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              file_io_pool_entry,
	              &section_data,
	              &section_data_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	else if( read_count == 0 )
	{
		return( 0 );
	}
	if( section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing section data.",
		 function );

		goto on_error;
	}
	if( section_data_size != (ssize_t) sizeof( ewf_data_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section data size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: data:\n",
		 function );
		libcnotify_print_data(
		 section_data,
		 section_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_data_t *) section_data )->checksum,
	 stored_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_data_t *) section_data )->number_of_chunks,
	 number_of_chunks );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_data_t *) section_data )->sectors_per_chunk,
	 sectors_per_chunk );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_data_t *) section_data )->bytes_per_sector,
	 bytes_per_sector );

	byte_stream_copy_to_uint64_little_endian(
	 ( (ewf_data_t *) section_data )->number_of_sectors,
	 number_of_sectors );

	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_data_t *) section_data )->error_granularity,
	 error_granularity );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: media type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (ewf_data_t *) section_data )->media_type );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_data_t *) section_data )->unknown1,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: number of chunks\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_chunks );

		libcnotify_printf(
		 "%s: sectors per chunk\t\t\t\t: %" PRIu32 "\n",
		 function,
		 sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t\t: %" PRIu32 "\n",
		 function,
		 bytes_per_sector );

		libcnotify_printf(
		 "%s: number of sectors\t\t\t\t: %" PRIu64 "\n",
		 function,
		 number_of_sectors );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_data_t *) section_data )->chs_cylinders,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of cylinders\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_data_t *) section_data )->chs_heads,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of heads\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_data_t *) section_data )->chs_sectors,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of sectors\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: media flags\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (ewf_data_t *) section_data )->media_flags );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_data_t *) section_data )->unknown2,
		 3,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_data_t *) section_data )->palm_volume_start_sector,
		 value_32bit );
		libcnotify_printf(
		 "%s: PALM volume start sector\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_data_t *) section_data )->unknown3,
		 4,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_data_t *) section_data )->smart_logs_start_sector,
		 value_32bit );
		libcnotify_printf(
		 "%s: SMART logs start sector\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: compression level\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (ewf_data_t *) section_data )->compression_level );

		libcnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_data_t *) section_data )->unknown4,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: error granularity\t\t\t\t: %" PRIu32 "\n",
		 function,
		 error_granularity );

		libcnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_data_t *) section_data )->unknown5,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: set identifier:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_data_t *) section_data )->set_identifier,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: unknown6:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_data_t *) section_data )->unknown6,
		 963,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		libcnotify_printf(
		 "%s: signature:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_data_t *) section_data )->signature,
		 5,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( stored_checksum != 0 )
	{
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     section_data,
		     section_data_size - 4,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate checksum.",
			 function );

			goto on_error;
		}
		if( stored_checksum != calculated_checksum )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
			 function,
			 stored_checksum,
			 calculated_checksum );

			goto on_error;
		}
	}
	*set_identifier_change = 0;

	if( ( ( (ewf_data_t *) section_data )->set_identifier[ 0 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 1 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 2 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 3 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 4 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 5 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 6 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 7 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 8 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 9 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 10 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 11 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 12 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 13 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 14 ] != 0 )
	 || ( ( (ewf_data_t *) section_data )->set_identifier[ 15 ] != 0 ) )
	{
		if( memory_compare(
		     media_values->set_identifier,
		     ( (ewf_data_t *) section_data )->set_identifier,
		     16 ) != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: set identifier change.",
				 function );
			}
#endif
			*set_identifier_change = 1;
		}
	}
	if( *set_identifier_change != 0 )
	{
/* TODO part of error tolerability changes
		media_values->media_type        = ( (ewf_data_t *) section_data )->media_type;
		media_values->number_of_chunks  = number_of_chunks;
		media_values->sectors_per_chunk = sectors_per_chunk;
		media_values->bytes_per_sector  = bytes_per_sector;
		media_values->number_of_sectors = number_of_sectors;
		media_values->media_flags       = ( (ewf_data_t *) section_data )->media_flags;
		io_handle->compression_level    = ( (ewf_data_t *) section_data )->compression_level;
		media_values->error_granularity = error_granularity;
*/
	}
	else
	{
		if( ( ( (ewf_data_t *) section_data )->media_type != 0 )
		 && ( ( (ewf_data_t *) section_data )->media_type != media_values->media_type ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: media type does not match.",
			 function );

			goto on_error;
		}
		if( ( number_of_chunks != 0 )
		 && ( (uint64_t) number_of_chunks != media_values->number_of_chunks ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: number of chunks does not match.",
			 function );

			goto on_error;
		}
		if( ( sectors_per_chunk != 0 )
		 && ( sectors_per_chunk != media_values->sectors_per_chunk ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: sectors per chunk does not match.",
			 function );

			goto on_error;
		}
		if( ( bytes_per_sector != 0 )
		 && ( bytes_per_sector != media_values->bytes_per_sector ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: bytes per sector does not match.",
			 function );

			goto on_error;
		}
		if( ( number_of_sectors != 0 )
		 && ( number_of_sectors != media_values->number_of_sectors ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: number of sectors does not match.",
			 function );

			goto on_error;
		}
		if( ( ( (ewf_data_t *) section_data )->media_flags != 0 )
		 && ( ( (ewf_data_t *) section_data )->media_flags != media_values->media_flags ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: media flags do not match.",
			 function );

			goto on_error;
		}
		if( ( ( (ewf_data_t *) section_data )->compression_level != 0 )
		 && ( ( (ewf_data_t *) section_data )->compression_level != io_handle->compression_level ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: compression level does not match.",
			 function );

			goto on_error;
		}
		if( ( error_granularity != 0 )
		 && ( error_granularity != media_values->error_granularity ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: error granularity does not match.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 section_data );

	return( read_count );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}

/* Writes a data section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_data_write(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         ewf_data_t **cached_data_section,
         libcerror_error_t **error )
{
	static char *function        = "libewf_section_data_write";
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( media_values->number_of_chunks > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid media values - number of chunks value out of bounds.",
		 function );

		return( -1 );
	}
	if( cached_data_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid caches data section.",
		 function );

		return( -1 );
	}
	if( libewf_section_set_values(
	     section_descriptor,
	     0,
	     (uint8_t *) "data",
	     4,
	     section_offset,
	     (size64_t) ( sizeof( ewf_section_descriptor_v1_t ) + sizeof( ewf_data_t ) ),
	     (size64_t) sizeof( ewf_data_t ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_descriptor_write(
	               section_descriptor,
	               file_io_pool,
	               file_io_pool_entry,
	               1,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_descriptor_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section descriptor.",
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create cached data section.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *cached_data_section,
		     0,
		     sizeof( ewf_data_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data.",
			 function );

			memory_free(
			 *cached_data_section );

			*cached_data_section = NULL;

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: media type\t\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 media_values->media_type );

			libcnotify_printf(
			 "%s: number of chunks\t\t\t\t: %" PRIu64 "\n",
			 function,
			 media_values->number_of_chunks );

			libcnotify_printf(
			 "%s: sectors per chunk\t\t\t\t: %" PRIu32 "\n",
			 function,
			 media_values->sectors_per_chunk );

			libcnotify_printf(
			 "%s: bytes per sector\t\t\t\t: %" PRIu32 "\n",
			 function,
			 media_values->bytes_per_sector );

			libcnotify_printf(
			 "%s: number of sectors\t\t\t\t: %" PRIu64 "\n",
			 function,
			 media_values->number_of_sectors );

			libcnotify_printf(
			 "%s: media flags\t\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 media_values->media_flags );

			libcnotify_printf(
			 "%s: compression level\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 io_handle->compression_level );

			libcnotify_printf(
			 "%s: error granularity\t\t\t\t: %" PRIu32 "\n",
			 function,
			 media_values->error_granularity );

			libcnotify_printf(
			 "%s: set identifier:\n",
			 function );
			libcnotify_print_data(
			 media_values->set_identifier,
			 16,
			 0 );

			libcnotify_printf(
			 "\n" );
		}
#endif
		( *cached_data_section )->media_type = media_values->media_type;
		( *cached_data_section )->media_flags = media_values->media_flags;

		byte_stream_copy_from_uint32_little_endian(
		 ( *cached_data_section )->number_of_chunks,
		 (uint32_t) media_values->number_of_chunks );

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
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN5 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN6 )
		 || ( io_handle->format == LIBEWF_FORMAT_LINEN7 )
		 || ( io_handle->format == LIBEWF_FORMAT_EWFX ) )
		{
			byte_stream_copy_from_uint32_little_endian(
			 ( *cached_data_section )->error_granularity,
			 media_values->error_granularity );

			( *cached_data_section )->compression_level = (uint8_t) io_handle->compression_level;

			if( memory_copy(
			     ( *cached_data_section )->set_identifier,
			     media_values->set_identifier,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy set identifier.",
				 function );

				memory_free(
				 *cached_data_section );

				*cached_data_section = NULL;

				return( -1 );
			}
		}
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     (uint8_t *) *cached_data_section,
		     sizeof( ewf_data_t ) - 4,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate checksum.",
			 function );

			memory_free(
			 *cached_data_section );

			*cached_data_section = NULL;

			return( -1 );
		}
		byte_stream_copy_from_uint32_little_endian(
		 ( *cached_data_section )->checksum,
		 calculated_checksum );
	}
	write_count = libewf_section_write_data(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) *cached_data_section,
	               sizeof( ewf_data_t ),
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	return( total_write_count );
}

/* Writes a sectors section
 * Does not write the actual data in the sectors section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_sectors_write(
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         size64_t chunks_data_size,
         uint32_t chunks_padding_size,
         libcerror_error_t **error )
{
	static char *function               = "libewf_section_sectors_write";
	size_t section_descriptor_data_size = 0;
	ssize_t write_count                 = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( libewf_section_set_values(
	     section_descriptor,
	     LIBEWF_SECTION_TYPE_SECTOR_DATA,
	     (uint8_t *) "sectors",
	     7,
	     section_offset,
	     section_descriptor_data_size + chunks_data_size,
	     chunks_data_size,
	     chunks_padding_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_descriptor_write(
	               section_descriptor,
	               file_io_pool,
	               file_io_pool_entry,
	               format_version,
	               error );

	if( write_count != (ssize_t) section_descriptor_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section descriptor data.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Reads a version 1 table or table2 section or version 2 sector table section
 * The section data will be set to a buffer containing the relevant (not necessarily full) section data
 * The table entries data will be set to a pointer within the section data
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_table_read(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t segment_file_type,
         uint8_t **section_data,
         size_t *section_data_size,
         uint64_t *first_chunk_index,
         uint64_t *base_offset,
         uint8_t **table_entries_data,
         size_t *table_entries_data_size,
         uint32_t *number_of_entries,
         uint8_t *entries_corrupted,	
         libcerror_error_t **error )
{
	uint8_t *table_data           = NULL;
	static char *function         = "libewf_section_table_read";
	size_t table_data_size        = 0;
	size_t table_entry_data_size  = 0;
	size_t table_header_data_size = 0;
	size_t table_footer_data_size = 0;
	ssize_t read_count            = 0;
	uint32_t calculated_checksum  = 0;
	uint32_t stored_checksum      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit          = 0;
#endif

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		table_header_data_size = sizeof( ewf_table_header_v1_t );
		table_entry_data_size  = sizeof( ewf_table_entry_v1_t );

		/* The original EWF and SMART (EWF-S01) formats do not contain a table footer
		 */
		if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
		{
			table_footer_data_size = 4;
		}
	}
	else if( format_version == 2 )
	{
		table_header_data_size = sizeof( ewf_table_header_v2_t );
		table_entry_data_size  = sizeof( ewf_table_entry_v2_t );
		table_footer_data_size = 16;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section data.",
		 function );

		return( -1 );
	}
	if( *section_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid section data value already set.",
		 function );

		return( -1 );
	}
	if( section_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section data size.",
		 function );

		return( -1 );
	}
	if( first_chunk_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first chunk index.",
		 function );

		return( -1 );
	}
	if( base_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base offset.",
		 function );

		return( -1 );
	}
	if( table_entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table entries data.",
		 function );

		return( -1 );
	}
	if( *table_entries_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table entries data value already set.",
		 function );

		return( -1 );
	}
	if( table_entries_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table entries data size.",
		 function );

		return( -1 );
	}
	if( number_of_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of entries.",
		 function );

		return( -1 );
	}
	if( entries_corrupted == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entries corrupted.",
		 function );

		return( -1 );
	}
	/* In original EWF, SMART (EWF-S01) and EnCase1 EWF-E01 the trailing data will be the chunk data
	 */
	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		*section_data_size = (size_t) table_header_data_size;
	}
	else
	{
		*section_data_size = (size_t) section_descriptor->data_size;
	}
/* TODO add support for table with chunk data */

	*section_data = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * *section_data_size );

	if( *section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              *section_data,
	              *section_data_size,
	              error );

	if( read_count != (ssize_t) *section_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	if( ( section_descriptor->data_flags & LIBEWF_SECTION_DATA_FLAGS_IS_ENCRYPTED ) != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: encrypted data:\n",
			 function );
			libcnotify_print_data(
			 *section_data,
			 *section_data_size,
			 0 );
		}
#endif
/* TODO decrypt */
		memory_free(
		 *section_data );

		*section_data      = NULL;
		*section_data_size = 0;

		return( 0 );
	}
	if( *section_data_size < table_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds - insufficient space for header.",
		 function );

		goto on_error;
	}
	table_data      = *section_data;
	table_data_size = *section_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: table header data:\n",
		 function );
		libcnotify_print_data(
		 table_data,
		 table_header_data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->number_of_entries,
		 *number_of_entries );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->base_offset,
		 *base_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->first_chunk_number,
		 *first_chunk_index );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->number_of_entries,
		 *number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: first chunk number\t\t\t\t: %" PRIu64 "\n",
			 function,
			 *first_chunk_index );
		}
		libcnotify_printf(
		 "%s: number of entries\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *number_of_entries );

		if( format_version == 1 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v1_t *) table_data )->padding1,
			 value_32bit );
			libcnotify_printf(
			 "%s: padding1\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: base offset\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 *base_offset );

			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v1_t *) table_data )->padding2,
			 value_32bit );
			libcnotify_printf(
			 "%s: padding2\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		else if( format_version == 2 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v2_t *) table_data )->unknown1,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "\n" );
		}
		else if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_table_header_v2_t *) table_data )->padding,
			 12,
			 0 );
		}
	}
#endif
	if( format_version == 2 )
	{
		table_header_data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     table_data,
	     table_header_data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate header checksum.",
		 function );

		goto on_error;
	}
	if( format_version == 2 )
	{
		table_header_data_size += 12;
	}
	table_data      += table_header_data_size;
	table_data_size -= table_header_data_size;

/* TODO flag that header is corrupted and continue */
	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: header checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	if( *number_of_entries > 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
/* TODO what about linen7 */
		if( format_version == 1 )
		{
			if( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
			 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 ) )
			{
				if( *number_of_entries > LIBEWF_MAXIMUM_TABLE_ENTRIES_ENCASE6 )
				{
					libcnotify_printf(
					 "%s: number of entries: %" PRIu32 " exceeds maximum: %" PRIu32 ".\n",
					 function,
					 *number_of_entries,
					 LIBEWF_MAXIMUM_TABLE_ENTRIES_ENCASE6 );
				}
			}
			else
			{
				if( *number_of_entries > LIBEWF_MAXIMUM_TABLE_ENTRIES )
				{
					libcnotify_printf(
					 "%s: number of entries: %" PRIu32 " exceeds maximum: %" PRIu32 ".\n",
					 function,
					 *number_of_entries,
					 LIBEWF_MAXIMUM_TABLE_ENTRIES );
				}
			}
		}
#endif
		*table_entries_data_size = *number_of_entries * table_entry_data_size;

		if( *table_entries_data_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid table entries data size value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
		{
			memory_free(
			 *section_data );

			*section_data_size += *table_entries_data_size + table_footer_data_size;

			*section_data = (uint8_t *) memory_allocate(
						     sizeof( uint8_t ) * *section_data_size );

			if( *section_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create section data.",
				 function );

				goto on_error;
			}
			read_count = libbfio_pool_read_buffer(
				      file_io_pool,
				      file_io_pool_entry,
				      *section_data,
				      *section_data_size,
				      error );

			if( read_count != (ssize_t) *section_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read section data.",
				 function );

				goto on_error;
			}
			table_data      = *section_data;
			table_data_size = *section_data_size;
		}
/* TODO flag that number of entries is corrupted and continue */
		if( table_data_size < *table_entries_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for entries.",
			 function );

			goto on_error;
		}
		*table_entries_data = table_data;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table entries data:\n",
			 function );
			libcnotify_print_data(
			 table_data,
			 *table_entries_data_size,
			 0 );
		}
#endif
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     table_data,
		     *table_entries_data_size,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate entries checksum.",
			 function );

			goto on_error;
		}
		table_data      += *table_entries_data_size;
		table_data_size -= *table_entries_data_size;

		if( table_footer_data_size > 0 )
		{
			if( table_data_size < table_footer_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid section size value out of bounds - insufficient space for footer.",
				 function );

				goto on_error;
			}
			byte_stream_copy_to_uint32_little_endian(
			 table_data,
			 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table entries checksum\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 stored_checksum );

				if( format_version == 1 )
				{
					libcnotify_printf(
					 "\n" );
				}
				else if( format_version == 2 )
				{
					libcnotify_printf(
					 "%s: padding:\n",
					 function );
					libcnotify_print_data(
					 &( table_data[ 4 ] ),
					 12,
					 0 );
				}
			}
#endif
			table_data      += table_footer_data_size;
			table_data_size -= table_footer_data_size;

			if( stored_checksum != calculated_checksum )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
	                        if( libcnotify_verbose != 0 )
        	                {
                	                libcnotify_printf(
					 "%s: entries checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").\n",
					 function,
					 stored_checksum,
					 calculated_checksum );
				}
#endif
				/* The table entries cannot be fully trusted therefore mark them as corrupted
				 */
				*entries_corrupted = 1;
			}
		}
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		if( *number_of_entries == 0 )
		{
			libcnotify_printf(
			 "%s: table section contains no entries.\n",
			 function );
		}
	}
#endif
#if defined( HAVE_VERBOSE_OUTPUT ) || defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( table_data_size > 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 table_data,
			 table_data_size,
			 0 );

#elif defined( HAVE_VERBOSE_OUTPUT )
			libcnotify_printf(
			 "%s: section has trailing data.\n",
			 function );
#endif
		}
	}
#endif
	return( read_count );

on_error:
	if( *section_data != NULL )
	{
		memory_free(
		 *section_data );

		*section_data = NULL;
	}
	*section_data_size       = 0;
	*table_entries_data      = NULL;
	*table_entries_data_size = 0;

	return( -1 );
}

/* Writes a version 1 table or table2 section or version 2 sector table section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_table_write(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t segment_file_type,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint8_t *section_data,
         size_t section_data_size,
         uint64_t first_chunk_index,
         uint64_t base_offset,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_entries,
         size64_t chunks_data_size,
         libcerror_error_t **error )
{
	uint8_t *table_data                 = NULL;
	static char *function               = "libewf_section_table_write";
	size_t required_section_data_size   = 0;
	size_t section_descriptor_data_size = 0;
	size_t table_entry_data_size        = 0;
	size_t table_header_data_size       = 0;
	size_t table_footer_data_size       = 0;
	size_t used_table_entries_data_size = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	uint32_t calculated_checksum        = 0;
	uint32_t section_padding_size       = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
		table_header_data_size       = sizeof( ewf_table_header_v1_t );
		table_entry_data_size        = sizeof( ewf_table_entry_v1_t );
		table_footer_data_size       = 4;
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
		table_header_data_size       = sizeof( ewf_table_header_v2_t );
		table_entry_data_size        = sizeof( ewf_table_entry_v2_t );
		table_footer_data_size       = 16;
		section_padding_size         = 24;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( type_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type string.",
		 function );

		return( -1 );
	}
	if( base_offset > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section data.",
		 function );

		return( -1 );
	}
	if( section_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table entries data.",
		 function );

		return( -1 );
	}
	if( table_entries_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid table entries data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	used_table_entries_data_size = number_of_entries * table_entry_data_size;

	if( used_table_entries_data_size > table_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data size value out of bounds.",
		 function );

		return( -1 );
	}
	required_section_data_size = table_header_data_size
	                           + used_table_entries_data_size;

	if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		required_section_data_size += table_footer_data_size;
	}
	if( required_section_data_size > section_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid section data value too small.",
		 function );

		return( -1 );
	}
	if( libewf_section_set_values(
	     section_descriptor,
	     LIBEWF_SECTION_TYPE_SECTOR_TABLE,
	     type_string,
	     type_string_length,
	     section_offset,
	     section_descriptor_data_size + required_section_data_size + chunks_data_size,
	     required_section_data_size + chunks_data_size,
	     section_padding_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section values.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		write_count = libewf_section_descriptor_write(
			       section_descriptor,
			       file_io_pool,
			       file_io_pool_entry,
			       format_version,
			       error );

		if( write_count != (ssize_t) section_descriptor_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write section descriptor data.",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	table_data = section_data;

	if( memory_set(
	     table_data,
	     0,
	     table_header_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table header data.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->number_of_entries,
		 number_of_entries );

		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->base_offset,
		 base_offset );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->first_chunk_number,
		 first_chunk_index );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->number_of_entries,
		 number_of_entries );

		table_header_data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     table_data,
	     table_header_data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate checksum.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->checksum,
		 calculated_checksum );

		table_header_data_size += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header data:\n",
		 function );
		libcnotify_print_data(
		 table_data,
		 table_header_data_size,
		 0 );
	}
#endif
	table_data += table_header_data_size;

	if( table_data != table_entries_data )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table entries data:\n",
		 function );
		libcnotify_print_data(
		 table_data,
		 used_table_entries_data_size,
		 0 );
	}
#endif
	if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     table_data,
		     used_table_entries_data_size,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate checksum.",
			 function );

			return( -1 );
		}
		table_data += used_table_entries_data_size;

		if( memory_set(
		     table_data,
		     0,
		     table_footer_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear table footer data.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint32_little_endian(
		 table_data,
		 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table footer data:\n",
			 function );
			libcnotify_print_data(
			 table_data,
			 table_footer_data_size,
			 0 );
		}
#endif
	}
	write_count = libewf_section_write_data(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               section_data,
	               required_section_data_size,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( format_version == 2 )
	{
		write_count = libewf_section_descriptor_write(
			       section_descriptor,
			       file_io_pool,
			       file_io_pool_entry,
			       format_version,
			       error );

		if( write_count != (ssize_t) section_descriptor_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write section descriptor data.",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}

