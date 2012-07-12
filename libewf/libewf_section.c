/*
 * Section reading/writing functions
 *
 * Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "libewf_definitions.h"
#include "libewf_compression.h"
#include "libewf_debug.h"
#include "libewf_hash_sections.h"
#include "libewf_header_values.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcstring.h"
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
#include "ewf_error.h"
#include "ewf_file_header.h"
#include "ewf_hash.h"
#include "ewf_ltree.h"
#include "ewf_session.h"
#include "ewf_table.h"
#include "ewf_volume.h"
#include "ewfx_delta_chunk.h"

/* Tests if a buffer entirely consists of zero values
 * Returns 1 if zero, 0 if not, or -1 on error
 */
int libewf_section_test_zero(
     uint8_t *buffer,
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

/* Creates a section
 * Returns 1 if successful or -1 on error
 */
int libewf_section_initialize(
     libewf_section_t **section,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_initialize";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( *section != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid csection value already set.",
		 function );

		return( -1 );
	}
	*section = memory_allocate_structure(
	            libewf_section_t );

	if( *section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *section,
	     0,
	     sizeof( libewf_section_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
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
     libcerror_error_t **error )
{
	static char *function = "libewf_section_free";

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
     libcerror_error_t **error )
{
	static char *function = "libewf_section_clone";

	if( destination_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination section.",
		 function );

		return( -1 );
	}
	if( *destination_section != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination section.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_section,
	     source_section,
	     sizeof( libewf_section_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
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

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
		     section->type_string,
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
		section->type_string[ type_string_length ] = 0;

		section->type_string_length = type_string_length;
	}
	else
	{
		if( memory_set(
		     section->type_string,
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
		section->type_string_length = 0;
	}
	section->type         = type;
	section->start_offset = section_offset;
	section->end_offset   = (off64_t) ( section_offset + section_size );
	section->size         = section_size;
	section->data_size    = data_size;
	section->padding_size = padding_size;

	return( 1 );
}

/* Reads a section descriptor
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_descriptor_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t file_offset,
         uint8_t format_version,
         libcerror_error_t **error )
{
	uint8_t section_descriptor_data[ 76 ];

	static char *function               = "libewf_section_descriptor_read";
	size_t section_descriptor_data_size = 0;
	ssize_t read_count                  = 0;
	uint32_t calculated_checksum        = 0;
	uint32_t section_descriptor_size    = 0;
	uint32_t stored_checksum            = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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

		return( -1 );
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

		return( -1 );
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
		     section->type_string,
		     ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->type_string,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set type string.",
			 function );

			return( -1 );
		}
		section->type_string[ 16 ] = 0;

		section->type_string_length = libcstring_narrow_string_length(
		                               (char *) section->type_string );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->size,
		 section->size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->next_offset,
		 section->end_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->type,
		 section->type );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_flags,
		 section->data_flags );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->previous_offset,
		 section->start_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_size,
		 section->data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->padding_size,
		 section->padding_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->descriptor_size,
		 section_descriptor_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: type string\t\t\t\t: %s\n",
			 function,
			 (char *) section->type_string );

			libcnotify_printf(
			 "%s: next offset\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 section->end_offset );

			libcnotify_printf(
			 "%s: size\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section->size );

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
			 section->type );
			libewf_debug_print_section_type(
			 section->type );
			libcnotify_printf(
			 ")\n" );

			libcnotify_printf(
			 "%s: data flags\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 section->data_flags );

			libcnotify_printf(
			 "%s: previous offset\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 section->start_offset );

			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section->data_size );

			libcnotify_printf(
			 "%s: section descriptor size\t\t\t: %" PRIu32 "\n",
			 function,
			 section_descriptor_size );

			libcnotify_printf(
			 "%s: padding size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 section->padding_size );

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
	calculated_checksum = ewf_checksum_calculate(
	                       section_descriptor_data,
	                       section_descriptor_data_size - 4,
	                       1 );

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

		return( -1 );
	}
	if( format_version == 1 )
	{
		section->start_offset = file_offset;
		section->data_size    = section->size - sizeof( ewf_section_descriptor_v1_t );
	}
	else if( format_version == 2 )
	{
/* TODO check sanity of start offset and data size, padding size */
		if( section->start_offset == 0 )
		{
			section->start_offset = sizeof( ewf_file_header_v2_t );
		}
		else
		{
			section->start_offset += sizeof( ewf_section_descriptor_v2_t );
		}
		section->end_offset = file_offset + sizeof( ewf_section_descriptor_v2_t );
		section->size       = (size64_t) ( section->end_offset - section->start_offset );
	}
	if( ( section->size != 0 )
	 && ( ( section->size < (size64_t) section_descriptor_data_size )
	  ||  ( section->size > (size64_t) INT64_MAX ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( section->end_offset < file_offset )
	 || ( section->end_offset > (off64_t) INT64_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section next offset value out of bounds.",
		 function );

	}
	if( format_version == 1 )
	{
		if( section->type_string_length == 4 )
		{
			if( memory_compare(
			     section->type_string,
			     "done",
			     4 ) == 0 )
			{
				section->type = LIBEWF_SECTION_TYPE_DONE;
			}
			else if( memory_compare(
			          section->type_string,
			          "hash",
			          4 ) == 0 )
			{
				section->type = LIBEWF_SECTION_TYPE_MD5_HASH;
			}
			else if( memory_compare(
			          section->type_string,
			          "next",
			          4 ) == 0 )
			{
				section->type = LIBEWF_SECTION_TYPE_NEXT;
			}
		}
		else if( section->type_string_length == 5 )
		{
			if( memory_compare(
			     section->type_string,
			     "ltree",
			     5 ) == 0 )
			{
				section->type = LIBEWF_SECTION_TYPE_SINGLE_FILES_DATA;
			}
			else if( memory_compare(
			          section->type_string,
			          "table",
			          5 ) == 0 )
			{
				section->type = LIBEWF_SECTION_TYPE_SECTOR_TABLE;
			}
		}
		else if( section->type_string_length == 6 )
		{
			if( memory_compare(
			     section->type_string,
			     "error2",
			     6 ) == 0 )
			{
				section->type = LIBEWF_SECTION_TYPE_ERROR_TABLE;
			}
		}
		else if( section->type_string_length == 7 )
		{
			if( memory_compare(
			     section->type_string,
			     "sectors",
			     7 ) == 0 )
			{
				section->type = LIBEWF_SECTION_TYPE_SECTOR_DATA;
			}
			else if( memory_compare(
			          section->type_string,
			          "session",
			          7 ) == 0 )
			{
				section->type = LIBEWF_SECTION_TYPE_SESSION_TABLE;
			}
		}
		/* Make sure to check if the section next value is sane
		 * the end offset of the next and done sections point back at themselves
		 */
		if( ( section->end_offset == section->start_offset )
		 && ( section->size == sizeof( ewf_section_descriptor_v1_t ) ) )
		{
			if( ( section->type != LIBEWF_SECTION_TYPE_DONE )
			 && ( section->type != LIBEWF_SECTION_TYPE_NEXT ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: mismatch in next section offset (stored: %" PRIi64 ", calculated: %" PRIi64 ").",
				 function,
				 section->end_offset,
				 file_offset );

				return( -1 );
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

			return( -1 );
		}
	}
	return( read_count );
}

/* Writes a section descriptor
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_descriptor_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libcerror_error_t **error )
{
	uint8_t section_descriptor_data[ 76 ];

	static char *function               = "libewf_section_descriptor_write";
	size_t section_descriptor_data_size = 0;
	ssize_t write_count                 = 0;
	uint32_t calculated_checksum        = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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

		return( -1 );
	}
	if( format_version == 1 )
	{
		if( memory_copy(
		     ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->type_string,
		     section->type_string,
		     section->type_string_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set type string.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->size,
		 section->size );

		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) section_descriptor_data )->next_offset,
		 section->end_offset );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->type,
		 section->type );

/* TODO set data flags
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_flags,
		 section->data_flags );
*/

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->data_size,
		 section->data_size );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->descriptor_size,
		 (uint32_t) section_descriptor_data_size );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) section_descriptor_data )->padding_size,
		 section->padding_size );

/* TODO set data_integrity_hash */
	}
	calculated_checksum = ewf_checksum_calculate(
	                       section_descriptor_data,
	                       section_descriptor_data_size - 4,
	                       1 );

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
			 (char *) section->type_string );

			libcnotify_printf(
			 "%s: next offset\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 section->end_offset );

			libcnotify_printf(
			 "%s: size\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section->size );

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
			 section->type );
			libewf_debug_print_section_type(
			 section->type );
			libcnotify_printf(
			 ")\n" );

/* TODO */
			libcnotify_printf(
			 "%s: data flags\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 0 );

			libcnotify_printf(
			 "%s: previous offset\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 section->start_offset );

			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section->data_size );

			libcnotify_printf(
			 "%s: section descriptor size\t\t\t: %" PRIzd "\n",
			 function,
			 section_descriptor_data_size );

			libcnotify_printf(
			 "%s: padding size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 section->padding_size );

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
         libcerror_error_t **error )
{
	uint8_t *data              = NULL;
	uint8_t *uncompressed_data = NULL;
	static char *function      = "libewf_section_debug_read";
	ssize_t read_count         = 0;
	size_t uncompressed_size   = 0;
	int result                 = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( section->size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.",
		 function );

		return( -1 );
	}
	uncompressed_size = (size_t) ( section->size * 2 );

	if( uncompressed_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: uncompressed size value exceeds maximum.",
		 function );

		return( -1 );
	}
	data = (uint8_t *) memory_allocate(
	                    (size_t) section->size );

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              data,
	              (size_t) section->size,
	              error );

	if( read_count != (ssize_t) section->size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	uncompressed_data = (uint8_t *) memory_allocate(
	                                 uncompressed_size );

	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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

/* Writes the last section descriptor
 * This is used for the next and done sections,
 * these sections point back towards themselves
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_last_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         uint32_t type,
         uint8_t segment_file_type,
         libcerror_error_t **error )
{
	uint8_t *type_string                = NULL;
	static char *function               = "libewf_section_last_write";
	size64_t section_size               = 0;
	size_t section_descriptor_data_size = 0;
	size_t type_string_length           = 0;
	ssize_t write_count                 = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( type == LIBEWF_SECTION_TYPE_NEXT )
	{
		type_string        = (uint8_t *) "next";
		type_string_length = 4;
	}
	else if( type == LIBEWF_SECTION_TYPE_DONE )
	{
		type_string        = (uint8_t *) "done";
		type_string_length = 4;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported type: 0x08%" PRIx32 ".",
		 function,
		 type );

		return( -1 );
	}
	/* The version 1 EWF-E01 and EWF-L01 formats leave the size of this section empty
	 */
	if( ( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 && ( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
	{
		section_size = section_descriptor_data_size;
	}
	if( libewf_section_set_values(
	     section,
	     type,
	     type_string,
	     type_string_length,
	     section_offset,
	     section_size,
	     0,
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
	               section,
	               file_io_pool,
	               file_io_pool_entry,
	               1,
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

/* Reads a compressed string section and decompresses it
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_compressed_string_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **uncompressed_string,
         size_t *uncompressed_string_size,
         libcerror_error_t **error )
{
	uint8_t *compressed_string = NULL;
	static char *function      = "libewf_section_compressed_string_read";
	void *reallocation         = NULL;
	ssize_t read_count         = 0;
	int result                 = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( section->data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section size value exceeds maximum.",
		 function );

		return( -1 );
	}
	compressed_string = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * (size_t) section->data_size );

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
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              compressed_string,
	              (size_t) section->data_size,
	              error );

	if( read_count != (ssize_t) section->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read compressed string.",
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
		 compressed_string,
		 (size_t) section->data_size,
		 0 );
	}
#endif
	/* On average the uncompressed string will be twice as large as the compressed string
	 */
	*uncompressed_string_size = 2 * (size_t) section->data_size;

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
	result = libewf_decompress(
	          *uncompressed_string,
	          uncompressed_string_size,
	          compressed_string,
	          (size_t) section->data_size,
	          error );

	while( ( result == -1 )
	    && ( *uncompressed_string_size > 0 ) )
	{
		libcerror_error_free(
		 error );

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

		result = libewf_decompress(
		          *uncompressed_string,
		          uncompressed_string_size,
		          compressed_string,
		          (size_t) section->data_size,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable to decompress string.",
		 function );

		goto on_error;
	}
	memory_free(
	 compressed_string );

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
         libcerror_error_t **error )
{
	uint8_t *compressed_string    = NULL;
	static char *function         = "libewf_section_write_compressed_string";
	void *reallocation            = NULL;
	size_t compressed_string_size = 0;
	ssize_t total_write_count     = 0;
	ssize_t write_count           = 0;
	int result                    = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	compressed_string_size = uncompressed_string_size;

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
		libcerror_error_free(
		 error );

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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
		 "%s: unable to compress string.",
		 function );

		goto on_error;
	}
/* TODO add support for EWF2 */
	if( libewf_section_set_values(
	     section,
	     0,
	     type_string,
	     type_string_length,
	     section_offset,
	     sizeof( ewf_section_descriptor_v1_t ) + compressed_string_size,
	     compressed_string_size,
	     0,
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
	write_count = libewf_section_descriptor_write(
	               section,
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
		 "%s: unable to write %s section descriptor.",
		 function,
		 (char *) type_string );

		goto on_error;
	}
	total_write_count += write_count;

	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               compressed_string,
	               compressed_string_size,
	               error );

	if( write_count != (ssize_t) compressed_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
         libcerror_error_t **error )
{
	ewf_data_t *data             = NULL;
	static char *function        = "libewf_section_data_read";
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

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( section->data_size != (size64_t) sizeof( ewf_data_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	data = memory_allocate_structure(
	        ewf_data_t );

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) data,
	              sizeof( ewf_data_t ),
	              error );
	
	if( read_count != (ssize_t) sizeof( ewf_data_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data.",
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
		 (uint8_t *) data,
		 sizeof( ewf_data_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
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
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: media type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 data->media_type );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 data->unknown1,
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
		 data->chs_cylinders,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of cylinders\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 data->chs_heads,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of heads\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 data->chs_sectors,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of sectors\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: media flags\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 data->media_flags );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 data->unknown2,
		 3,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 data->palm_volume_start_sector,
		 value_32bit );
		libcnotify_printf(
		 "%s: PALM volume start sector\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 data->unknown3,
		 4,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 data->smart_logs_start_sector,
		 value_32bit );
		libcnotify_printf(
		 "%s: SMART logs start sector\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: compression level\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 data->compression_level );

		libcnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libcnotify_print_data(
		 data->unknown4,
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
		 data->unknown5,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: set identifier:\n",
		 function );
		libcnotify_print_data(
		 data->set_identifier,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: unknown6:\n",
		 function );
		libcnotify_print_data(
		 data->unknown6,
		 963,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		libcnotify_printf(
		 "%s: signature:\n",
		 function );
		libcnotify_print_data(
		 data->signature,
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
	calculated_checksum = ewf_checksum_calculate(
	                       data,
	                       sizeof( ewf_data_t ) - 4,
	                       1 );

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
	if( ( data->media_type != 0 )
	 && ( data->media_type != media_values->media_type ) )
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
	if( ( data->media_flags != 0 )
	 && ( data->media_flags != media_values->media_flags ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: media flags do not match.",
		 function );

		goto on_error;
	}
	if( ( data->compression_level != 0 )
	 && ( data->compression_level != io_handle->compression_level ) )
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
	if( ( data->set_identifier[ 0 ] != 0 )
	 || ( data->set_identifier[ 1 ] != 0 )
	 || ( data->set_identifier[ 2 ] != 0 )
	 || ( data->set_identifier[ 3 ] != 0 )
	 || ( data->set_identifier[ 4 ] != 0 )
	 || ( data->set_identifier[ 5 ] != 0 )
	 || ( data->set_identifier[ 6 ] != 0 )
	 || ( data->set_identifier[ 7 ] != 0 )
	 || ( data->set_identifier[ 8 ] != 0 )
	 || ( data->set_identifier[ 9 ] != 0 )
	 || ( data->set_identifier[ 10 ] != 0 )
	 || ( data->set_identifier[ 11 ] != 0 )
	 || ( data->set_identifier[ 12 ] != 0 )
	 || ( data->set_identifier[ 13 ] != 0 )
	 || ( data->set_identifier[ 14 ] != 0 )
	 || ( data->set_identifier[ 15 ] != 0 ) )
	{
		if( memory_compare(
		     media_values->set_identifier,
		     data->set_identifier,
		     16 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: mismatch in set identifier.",
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
         libcerror_error_t **error )
{
	static char *function        = "libewf_section_data_write";
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	     section,
	     0,
	     (uint8_t *) "data",
	     4,
	     section_offset,
	     sizeof( ewf_section_descriptor_v1_t ) + sizeof( ewf_data_t ),
	     sizeof( ewf_data_t ),
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
	               section,
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
			 "%s: unable to create data.",
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

/* TODO check if media values -> number of chunks does not exceed the 32-bit maximum */
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
		calculated_checksum = ewf_checksum_calculate(
		                       *cached_data_section,
		                       sizeof( ewf_data_t ) - 4,
		                       1 );

		byte_stream_copy_from_uint32_little_endian(
		 ( *cached_data_section )->checksum,
		 calculated_checksum );
	}
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) *cached_data_section,
	               sizeof( ewf_data_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_data_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
         libcerror_error_t **error )
{
	ewf_digest_t digest;

	static char *function        = "libewf_section_digest_read";
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;
	int result                   = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( section->data_size != (size64_t) sizeof( ewf_digest_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &digest,
	              sizeof( ewf_digest_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_digest_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read digest.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: digest data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &digest,
		 sizeof( ewf_digest_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 digest.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libcnotify_print_data(
		 digest.md5_hash,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: SHA1 hash:\n",
		 function );
		libcnotify_print_data(
		 digest.sha1_hash,
		 20,
		 0 );

		libcnotify_printf(
		 "%s: padding:\n",
		 function );
		libcnotify_print_data(
		 digest.padding1,
		 40,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &digest,
	                       sizeof( ewf_digest_t ) - 4,
	                       1 );

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

		return( -1 );
	}
	result = libewf_section_test_zero(
		  digest.md5_hash,
		  16,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
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
         libcerror_error_t **error )
{
	ewf_digest_t digest;

	static char *function        = "libewf_section_digest_write";
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( libewf_section_set_values(
	     section,
	     0,
	     (uint8_t *) "digest",
	     6,
	     section_offset,
	     sizeof( ewf_section_descriptor_v1_t ) + sizeof( ewf_digest_t ),
	     sizeof( ewf_digest_t ),
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
	               section,
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

	if( memory_set(
	     &digest,
	     0,
	     sizeof( ewf_digest_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 hash.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libcnotify_print_data(
		 digest.md5_hash,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: SHA1 hash:\n",
		 function );
		libcnotify_print_data(
		 digest.sha1_hash,
		 20,
		 0 );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &digest,
	                       sizeof( ewf_digest_t ) - 4,
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 digest.checksum,
	 calculated_checksum );

	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &digest,
	               sizeof( ewf_digest_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_digest_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write digest.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	return( total_write_count );
}

/* Reads a version 1 error2 section or version 2 error table section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_error_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libewf_sector_list_t *acquiry_errors,
         libcerror_error_t **error )
{
	uint8_t error_header_data[ 520 ];
	uint8_t error_footer_data[ 16 ];

	uint8_t *error_entry_data      = NULL;
	uint8_t *error_entries_data    = NULL;
	static char *function          = "libewf_section_error_read";
	size64_t section_data_size     = 0;
	size_t error_entry_data_size   = 0;
	size_t error_entries_data_size = 0;
	size_t error_footer_data_size  = 0;
	size_t error_header_data_size  = 0;
	ssize_t read_count             = 0;
	ssize_t total_read_count       = 0;
	uint64_t first_sector          = 0;
	uint32_t calculated_checksum   = 0;
	uint32_t entry_index           = 0;
	uint32_t number_of_entries     = 0;
	uint32_t number_of_sectors     = 0;
	uint32_t stored_checksum       = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( acquiry_errors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid acquiry errors.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		error_header_data_size = sizeof( ewf_error_header_v1_t );
		error_entry_data_size  = sizeof( ewf_error_entry_v1_t );
		error_footer_data_size = 4;
	}
	else if( format_version == 2 )
	{
		error_header_data_size = sizeof( ewf_error_header_v2_t );
		error_entry_data_size  = sizeof( ewf_error_entry_v2_t );
		error_footer_data_size = 16;
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
	if( section->data_size < (size64_t) error_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	section_data_size = section->data_size;

	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              error_header_data,
	              error_header_data_size,
	              error );
	
	if( read_count != (ssize_t) error_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read error header data.",
		 function );

		goto on_error;
	}
	total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error header data:\n",
		 function );
		libcnotify_print_data(
		 error_header_data,
		 error_header_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_error_header_v1_t *) error_header_data )->number_of_entries,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_error_header_v1_t *) error_header_data )->checksum,
		 stored_checksum);
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_error_header_v2_t *) error_header_data )->number_of_entries,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_error_header_v2_t *) error_header_data )->checksum,
		 stored_checksum);
	}

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );

		if( format_version == 1 )
		{
			libcnotify_print_data(
			 ( (ewf_error_header_v1_t *) error_header_data )->unknown1,
			 200,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else if( format_version == 1 )
		{
			libcnotify_print_data(
			 ( (ewf_error_header_v2_t *) error_header_data )->unknown1,
			 12,
			 0 );
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
			 ( (ewf_error_header_v2_t *) error_header_data )->padding,
			 12,
			 0 );
		}
	}
#endif
	section_data_size -= error_header_data_size;

	if( format_version == 2 )
	{
		error_header_data_size -= 12;
	}
	calculated_checksum = ewf_checksum_calculate(
	                       error_header_data,
	                       error_header_data_size - 4,
	                       1 );

	if( stored_checksum!= calculated_checksum)
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
	if( number_of_entries > 0 )
	{
		error_entries_data_size = number_of_entries * error_entry_data_size;

		if( section_data_size < error_entries_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds.",
			 function );

			goto on_error;
		}
/* TODO check SSIZE_MAX bounds */
		error_entries_data = (uint8_t *) memory_allocate(
		                                  sizeof( uint8_t ) * error_entries_data_size );

		if( error_entries_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create error entries data.",
			 function );

			goto on_error;
		}
		read_count = libbfio_pool_read_buffer(
		              file_io_pool,
		              file_io_pool_entry,
		              error_entries_data,
		              error_entries_data_size,
	                      error );
	
		if( read_count != (ssize_t) error_entries_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read error entries data.",
			 function );

			goto on_error;
		}
		total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: error entries data:\n",
			 function );
			libcnotify_print_data(
			 error_entries_data,
			 error_entries_data_size,
			 0 );
		}
#endif
		read_count = libbfio_pool_read_buffer(
		              file_io_pool,
		              file_io_pool_entry,
		              error_footer_data,
		              error_footer_data_size,
	                      error );

		if( read_count != (ssize_t) error_footer_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read error footer data.",
			 function );

			goto on_error;
		}
		total_read_count += read_count;

		byte_stream_copy_to_uint32_little_endian(
		 error_footer_data,
		 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: error entries checksum\t\t\t: 0x%08" PRIx32 "\n",
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
				 &( error_footer_data[ 4 ] ),
				 12,
				 0 );
			}
		}
#endif
		calculated_checksum = ewf_checksum_calculate(
		                       error_entries_data,
		                       error_entries_data_size,
		                       1 );

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
		if( libewf_sector_list_empty(
		     acquiry_errors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty acquiry errors sector list.",
			 function );

			goto on_error;
		}
		error_entry_data = error_entries_data;

		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			if( format_version == 1 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_error_entry_v1_t *) error_entry_data )->first_sector,
				 first_sector );

				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_error_entry_v1_t *) error_entry_data )->number_of_sectors,
				 number_of_sectors );
			}
			else if( format_version == 2 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (ewf_error_entry_v2_t *) error_entry_data )->first_sector,
				 first_sector );

				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_error_entry_v2_t *) error_entry_data )->number_of_sectors,
				 number_of_sectors );
			}

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: error entry: %02" PRIu32 " first sector\t\t\t: %" PRIu64 "\n",
				 function,
				 entry_index,
				 first_sector );

				libcnotify_printf(
				 "%s: error entry: %02" PRIu32 " number of sectors\t\t: %" PRIu32 "\n",
				 function,
				 entry_index,
				 number_of_sectors );

				if( format_version == 2 )
				{
					libcnotify_printf(
					 "%s: error entry: %02" PRIu32 " padding:\n",
					 function,
					 entry_index );
					libcnotify_print_data(
					 ( (ewf_error_entry_v2_t *) error_entry_data )->padding,
					 12,
					 0 );
				}
			}
#endif
			if( libewf_sector_list_append_sector(
			     acquiry_errors,
			     first_sector,
			     (uint64_t) number_of_sectors,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append acquiry error to sector list.",
				 function );

				goto on_error;
			}
			error_entry_data += error_entry_data_size;
		}
		memory_free(
		 error_entries_data );

		error_entries_data = NULL;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error section contains no entries.\n",
		 function );
	}
#endif
	return( total_read_count );

on_error:
	if( error_entries_data != NULL )
	{
		memory_free(
		 error_entries_data );
	}
	return( -1 );
}

/* Writes a version 1 error2 section or version 2 error table section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_error_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libewf_sector_list_t *acquiry_errors,
         libcerror_error_t **error )
{
	uint8_t error_header_data[ 520 ];
	uint8_t error_footer_data[ 16 ];

	uint8_t *error_entry_data           = NULL;
	uint8_t *error_entries_data         = NULL;
	static char *function               = "libewf_section_error_write";
	size64_t section_data_size          = 0;
	size_t error_entry_data_size        = 0;
	size_t error_entries_data_size      = 0;
	size_t error_footer_data_size       = 0;
	size_t error_header_data_size       = 0;
	size_t section_descriptor_data_size = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	uint64_t first_sector               = 0;
	uint64_t number_of_sectors          = 0;
	uint32_t calculated_checksum        = 0;
	uint32_t section_padding_size       = 0;
	int entry_index                     = 0;
	int number_of_entries               = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
		error_header_data_size       = sizeof( ewf_error_header_v1_t );
		error_entry_data_size        = sizeof( ewf_error_entry_v1_t );
		error_footer_data_size       = 4;
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
		error_header_data_size       = sizeof( ewf_error_header_v2_t );
		error_entry_data_size        = sizeof( ewf_error_entry_v2_t );
		error_footer_data_size       = 16;
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
	if( libewf_sector_list_get_number_of_elements(
	     acquiry_errors,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from acquiry error sector list.",
		 function );

		goto on_error;
	}
	if( number_of_entries <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of errors value out of bounds.",
		 function );

		goto on_error;
	}
	error_entries_data_size = number_of_entries * error_entry_data_size;

	section_data_size = error_header_data_size
	                  + error_entries_data_size
	                  + error_footer_data_size;

	if( libewf_section_set_values(
	     section,
	     LIBEWF_SECTION_TYPE_ERROR_TABLE,
	     (uint8_t *) "error2",
	     6,
	     section_offset,
	     section_descriptor_data_size + section_data_size,
	     section_data_size,
	     section_padding_size,
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
			       section,
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

			goto on_error;
		}
		total_write_count += write_count;
	}
	if( memory_set(
	     error_header_data,
	     0,
	     error_header_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear error header data.",
		 function );

		goto on_error;
	}
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_error_header_v1_t *) error_header_data )->number_of_entries,
		 number_of_entries );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_error_header_v2_t *) error_header_data )->number_of_entries,
		 number_of_entries );

		error_header_data_size -= 12;
	}
	calculated_checksum = ewf_checksum_calculate(
	                       error_header_data,
	                       error_header_data_size - 4,
	                       1 );

	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_error_header_v1_t *) error_header_data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_error_header_v2_t *) error_header_data )->checksum,
		 calculated_checksum );

		error_header_data_size += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error header data:\n",
		 function );
		libcnotify_print_data(
		 error_header_data,
		 error_header_data_size,
		 0 );
	}
#endif
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               error_header_data,
	               error_header_data_size,
	               error );

	if( write_count != (ssize_t) error_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write error header.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

/* TODO EnCase compatible way to handle > 32-bit sector values */
	error_entries_data = (uint8_t *) memory_allocate(
	                                  sizeof( uint8_t ) * error_entries_data_size );

	if( error_entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create error entries data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     error_entries_data,
	     0,
	     sizeof( uint8_t ) * error_entries_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear error entries data.",
		 function );

		goto on_error;
	}
	error_entry_data = error_entries_data;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libewf_sector_list_get_sector(
		     acquiry_errors,
		     entry_index,
		     &first_sector,
		     &number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve acquiry error: %d from sector list.",
			 function,
			 entry_index );

			goto on_error;
		}
/* TODO check bounds fo first_sector and number_of_sectors and print warning if truncation happens */
		if( format_version == 1 )
		{
			byte_stream_copy_from_uint32_little_endian(
			 ( (ewf_error_entry_v1_t *) error_entry_data )->first_sector,
			 (uint32_t) first_sector );

			byte_stream_copy_from_uint32_little_endian(
			 ( (ewf_error_entry_v1_t *) error_entry_data )->number_of_sectors,
			 (uint32_t) number_of_sectors );
		}
		else if( format_version == 2 )
		{
			byte_stream_copy_from_uint64_little_endian(
			 ( (ewf_error_entry_v2_t *) error_entry_data )->first_sector,
			 first_sector );

			byte_stream_copy_from_uint32_little_endian(
			 ( (ewf_error_entry_v2_t *) error_entry_data )->number_of_sectors,
			 (uint32_t) number_of_sectors );
		}
		error_entry_data += error_entry_data_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error entries data:\n",
		 function );
		libcnotify_print_data(
		 error_entries_data,
		 error_entries_data_size,
		 0 );
	}
#endif
	write_count = libbfio_pool_write_buffer(
		       file_io_pool,
		       file_io_pool_entry,
		       error_entries_data,
		       error_entries_data_size,
		       error );

	if( write_count != (ssize_t) error_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write error entries data.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	calculated_checksum = ewf_checksum_calculate(
			       error_entries_data,
			       error_entries_data_size,
			       1 );

	memory_free(
	 error_entries_data );

	error_entries_data = NULL;

	if( memory_set(
	     error_footer_data,
	     0,
	     error_footer_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear error footer data.",
		 function );

		goto on_error;
	}
	byte_stream_copy_from_uint32_little_endian(
	 error_footer_data,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error footer data:\n",
		 function );
		libcnotify_print_data(
		 error_footer_data,
		 error_footer_data_size,
		 0 );
	}
#endif
	write_count = libbfio_pool_write_buffer(
		       file_io_pool,
		       file_io_pool_entry,
		       error_footer_data,
		       error_footer_data_size,
		       error );

	if( write_count != (ssize_t) error_footer_data_size  )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write error footer data.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	if( format_version == 2 )
	{
		write_count = libewf_section_descriptor_write(
			       section,
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

			goto on_error;
		}
		total_write_count += write_count;
	}
	return( total_write_count );

on_error:
	if( error_entries_data != NULL )
	{
		memory_free(
		 error_entries_data );
	}
	return( -1 );
}

/* Reads a version 1 hash section or a version 2 MD5 hash section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_md5_hash_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error )
{
	uint8_t md5_hash_data[ 36 ];

	static char *function        = "libewf_section_md5_hash_read";
	size_t md5_hash_data_size    = 0;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;
	int result                   = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		md5_hash_data_size = sizeof( ewf_hash_t );
	}
	else if( format_version == 2 )
	{
		md5_hash_data_size = sizeof( ewf_md5_hash_t );
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
	if( section->data_size != (size64_t) md5_hash_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              md5_hash_data,
	              md5_hash_data_size,
	              error );

	if( read_count != (ssize_t) md5_hash_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MD5 hash data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: MD5 hash data:\n",
		 function );
		libcnotify_print_data(
		 md5_hash_data,
		 md5_hash_data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_hash_t *) md5_hash_data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_md5_hash_t *) md5_hash_data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libcnotify_print_data(
		 md5_hash_data,
		 16,
		 0 );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_hash_t *) md5_hash_data )->unknown1,
			 16,
			 0 );
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
			 ( (ewf_md5_hash_t *) md5_hash_data )->padding,
			 12,
			 0 );
		}
	}
#endif
	if( format_version == 2 )
	{
		md5_hash_data_size -= 12;
	}
	calculated_checksum = ewf_checksum_calculate(
	                       md5_hash_data,
	                       md5_hash_data_size - 4,
	                       1 );

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

		return( -1 );
	}
	result = libewf_section_test_zero(
		  md5_hash_data,
		  16,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MD5 hash is empty.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( memory_copy(
		     hash_sections->md5_hash,
		     md5_hash_data,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
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

/* Writes a version 1 or 2 MD5 hash section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_md5_hash_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error )
{
	uint8_t md5_hash_data[ 36 ];

	static char *function               = "libewf_section_md5_hash_write";
	size_t md5_hash_data_size           = 0;
	size_t section_descriptor_data_size = 0;
	size_t section_padding_size         = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	uint32_t calculated_checksum        = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
		md5_hash_data_size           = sizeof( ewf_hash_t );
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
		md5_hash_data_size           = sizeof( ewf_md5_hash_t );
		section_padding_size         = 12;
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
	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( libewf_section_set_values(
	     section,
	     LIBEWF_SECTION_TYPE_MD5_HASH,
	     (uint8_t *) "hash",
	     4,
	     section_offset,
	     section_descriptor_data_size + md5_hash_data_size,
	     md5_hash_data_size,
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
			       section,
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
	if( memory_set(
	     md5_hash_data,
	     0,
	     md5_hash_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear MD5 hash data.",
		 function );

		return( -1 );
	}
	if( hash_sections->md5_hash_set != 0 )
	{
		if( memory_copy(
		     md5_hash_data,
		     hash_sections->md5_hash,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 hash.",
			 function );

			return( -1 );
		}
	}
	if( format_version == 2 )
	{
		md5_hash_data_size -= 12;
	}
	calculated_checksum = ewf_checksum_calculate(
	                       md5_hash_data,
	                       md5_hash_data_size - 4,
	                       1 );

	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_hash_t *) md5_hash_data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_md5_hash_t *) md5_hash_data )->checksum,
		 calculated_checksum );

		md5_hash_data_size += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MD5 hash data:\n",
		 function );
		libcnotify_print_data(
		 md5_hash_data,
		 md5_hash_data_size,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libcnotify_print_data(
		 md5_hash_data,
		 16,
		 0 );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_hash_t *) md5_hash_data )->unknown1,
			 16,
			 0 );
		}
		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 calculated_checksum );

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
			 ( (ewf_md5_hash_t *) md5_hash_data )->padding,
			 12,
			 0 );
		}
	}
#endif
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               md5_hash_data,
	               md5_hash_data_size,
	               error );

	if( write_count != (ssize_t) md5_hash_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write MD5 hash data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( format_version == 2 )
	{
		write_count = libewf_section_descriptor_write(
			       section,
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

/* Reads a version 2 SHA1 hash section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_sha1_hash_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error )
{
	uint8_t sha1_hash_data[ 32 ];

	static char *function        = "libewf_section_sha1_hash_read";
	size_t sha1_hash_data_size   = 0;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;
	int result                   = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	sha1_hash_data_size = sizeof( ewf_sha1_hash_t );

	if( section->data_size != (size64_t) sha1_hash_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              sha1_hash_data,
	              sha1_hash_data_size,
	              error );

	if( read_count != (ssize_t) sha1_hash_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read SHA1 hash data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: SHA1 hash data:\n",
		 function );
		libcnotify_print_data(
		 sha1_hash_data,
		 sha1_hash_data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_sha1_hash_t *) sha1_hash_data )->checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: SHA1 hash:\n",
		 function );
		libcnotify_print_data(
		 sha1_hash_data,
		 20,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "%s: padding:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_sha1_hash_t *) sha1_hash_data )->padding,
		 8,
		 0 );
	}
#endif
	sha1_hash_data_size -= 8;

	calculated_checksum = ewf_checksum_calculate(
	                       sha1_hash_data,
	                       sha1_hash_data_size - 4,
	                       1 );

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

		return( -1 );
	}
	result = libewf_section_test_zero(
		  sha1_hash_data,
		  16,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if SHA1 hash is empty.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( memory_copy(
		     hash_sections->sha1_hash,
		     sha1_hash_data,
		     20 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set SHA1 hash in hash sections.",
			 function );

			return( -1 );
		}
		hash_sections->sha1_hash_set = 1;
	}
	else
	{
		hash_sections->sha1_hash_set = 0;
	}
	return( read_count );
}

/* Reads a header section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_header_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         libcerror_error_t **error )
{
	uint8_t *header       = NULL;
	static char *function = "libewf_section_header_read";
	ssize_t read_count    = 0;
	size_t header_size    = 0;

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read header.",
		 function );

		goto on_error;
	}
	if( header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_byte_stream_print(
		     "Header",
		     header,
		     header_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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
         libcerror_error_t **error )
{
	static char *function = "libewf_section_header_write";
	ssize_t write_count   = 0;

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_byte_stream_print(
		     "Header",
		     header_sections->header,
		     header_sections->header_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
         libcerror_error_t **error )
{
	uint8_t *header2      = NULL;
	static char *function = "libewf_section_header2_read";
	ssize_t read_count    = 0;
	size_t header2_size   = 0;

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read header2.",
		 function );

		goto on_error;
	}
	if( header2 == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing header2.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_utf16_stream_print(
		     "Header2",
		     header2,
		     header2_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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
         libcerror_error_t **error )
{
	static char *function = "libewf_section_header2_write";
	ssize_t write_count   = 0;

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_utf16_stream_print(
		     "Header2",
		     header_sections->header2,
		     header_sections->header2_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
         uint8_t format_version,
         uint8_t **cached_ltree_data,
         size_t *cached_ltree_data_size,
         libcerror_error_t **error )
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( ( format_version != 1 )
	 && ( format_version != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( cached_ltree_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached ltree.",
		 function );

		return( -1 );
	}
	if( cached_ltree_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cached ltree data size.",
		 function );

		return( -1 );
	}
/* TODO check if section->data_size > SSIZE_MAX */
	if( format_version == 1 )
	{
		section_data_size = section->data_size;

		if( section_data_size < (size64_t) sizeof( ewf_ltree_header_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds.",
			 function );

			goto on_error;
		}
		section_data_size -= sizeof( ewf_ltree_header_t );

		ltree_header = memory_allocate_structure(
		                ewf_ltree_header_t );

		if( ltree_header == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create ltree header.",
			 function );

			goto on_error;
		}
		read_count = libbfio_pool_read_buffer(
			      file_io_pool,
			      file_io_pool_entry,
			      (uint8_t *) ltree_header,
			      sizeof( ewf_ltree_header_t ),
			      error );
		
		if( read_count != (ssize_t) sizeof( ewf_ltree_header_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read ltree header.",
			 function );

			goto on_error;
		}
		total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: ltree header data:\n",
			 function );
			libcnotify_print_data(
			 (uint8_t *) ltree_header,
			 sizeof( ewf_ltree_header_t ),
			 0 );
		}
#endif
		byte_stream_copy_to_uint32_little_endian(
		 ltree_header->tree_size,
		 ltree_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ltree_header->unknown1,
			 16,
			 0 );

			libcnotify_printf(
			 "%s: tree size\t\t\t\t\t: %" PRIu32 "\n",
			 function,
			 ltree_size );

			libcnotify_printf(
			 "%s: unknown2:\n",
			 function );
			libcnotify_print_data(
			 ltree_header->unknown2,
			 4,
			 0 );

			libcnotify_printf(
			 "%s: unknown3:\n",
			 function );
			libcnotify_print_data(
			 ltree_header->unknown3,
			 4,
			 0 );

			libcnotify_printf(
			 "%s: unknown4:\n",
			 function );
			libcnotify_print_data(
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds.",
			 function );

			goto on_error;
		}
		ltree_data_size = (size_t) ltree_size;
	}
	else if( format_version == 2 )
	{
		ltree_data_size = (size_t) section->data_size;
	}
	ltree_data = (uint8_t *) memory_allocate(
                                  sizeof( uint8_t ) * ltree_data_size );

	if( ltree_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create ltree data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              ltree_data,
	              ltree_data_size,
	              error );

	if( read_count != (ssize_t) ltree_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ltree data.",
		 function );

		goto on_error;
	}
	total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_utf16_stream_print(
		     "ltree data",
		     ltree_data,
		     ltree_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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
/* TODO print trailing version 1 data */
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
         uint8_t format_version,
         off64_t section_offset,
         size64_t chunks_data_size,
         libcerror_error_t **error )
{
	static char *function               = "libewf_section_sectors_write";
	size_t section_descriptor_data_size = 0;
	ssize_t write_count                 = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	     section,
	     LIBEWF_SECTION_TYPE_SECTOR_DATA,
	     (uint8_t *) "sectors",
	     7,
	     section_offset,
	     section_descriptor_data_size + chunks_data_size,
	     chunks_data_size,
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
	               section,
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

/* Reads a version 1 session section or version 2 session table section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_session_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libewf_media_values_t *media_values,
         libewf_sector_list_t *sessions,
         libewf_sector_list_t *tracks,
         libcerror_error_t **error )
{
	uint8_t session_header_data[ 36 ];
	uint8_t session_footer_data[ 16 ];

	uint8_t *session_entry_data      = NULL;
	uint8_t *session_entries_data    = NULL;
	static char *function            = "libewf_section_session_read";
	size64_t section_data_size       = 0;
	size_t session_entry_data_size   = 0;
	size_t session_entries_data_size = 0;
	size_t session_footer_data_size  = 0;
	size_t session_header_data_size  = 0;
	ssize_t read_count               = 0;
	ssize_t total_read_count         = 0;
	uint64_t first_sector            = 0;
	uint64_t previous_first_sector   = 0;
	uint64_t session_first_sector    = 0;
	uint64_t track_first_sector      = 0;
	uint32_t calculated_checksum     = 0;
	uint32_t entry_index             = 0;
	uint32_t flags                   = 0;
	uint32_t number_of_entries       = 0;
	uint32_t number_of_sectors       = 0;
	uint32_t previous_flags          = 0;
	uint32_t stored_checksum         = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		session_header_data_size = sizeof( ewf_session_header_v1_t );
		session_entry_data_size  = sizeof( ewf_session_entry_v1_t );
		session_footer_data_size = 4;
	}
	else if( format_version == 2 )
	{
		session_header_data_size = sizeof( ewf_session_header_v2_t );
		session_entry_data_size  = sizeof( ewf_session_entry_v2_t );
		session_footer_data_size = 16;
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
	if( sessions == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sessions.",
		 function );

		return( -1 );
	}
	if( section->data_size < (size64_t) session_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	section_data_size = section->data_size;

	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              session_header_data,
	              session_header_data_size,
	              error );

	if( read_count != (ssize_t) session_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read session header data.",
		 function );

		goto on_error;
	}
	total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session header data:\n",
		 function );
		libcnotify_print_data(
		 session_header_data,
		 session_header_data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_session_header_v1_t *) session_header_data )->number_of_entries,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_session_header_v1_t *) session_header_data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_session_header_v2_t *) session_header_data )->number_of_entries,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_session_header_v2_t *) session_header_data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );

		if( format_version == 1 )
		{
			libcnotify_print_data(
			 ( (ewf_session_header_v1_t *) session_header_data )->unknown1,
			 28,
			 0 );
		}
		else if( format_version == 2 )
		{
			libcnotify_print_data(
			 ( (ewf_session_header_v2_t *) session_header_data )->unknown1,
			 12,
			 0 );
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
			 ( (ewf_session_header_v2_t *) session_header_data )->padding,
			 12,
			 0 );
		}
	}
#endif
	section_data_size -= session_header_data_size;

	if( format_version == 2 )
	{
		session_header_data_size -= 12;
	}
	calculated_checksum = ewf_checksum_calculate(
	                       session_header_data,
	                       session_header_data_size - 4,
	                       1 );

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
	if( number_of_entries > 0 )
	{
		session_entries_data_size = number_of_entries * session_entry_data_size;

		if( section_data_size < (size64_t) session_entries_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds.",
			 function );

			goto on_error;
		}
/* TODO check SSIZE_MAX bounds */
		session_entries_data = (uint8_t *) memory_allocate(
		                                    sizeof( uint8_t ) * session_entries_data_size );

		if( session_entries_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create session entries data.",
			 function );

			goto on_error;
		}
		read_count = libbfio_pool_read_buffer(
		              file_io_pool,
		              file_io_pool_entry,
		              session_entries_data,
		              session_entries_data_size,
		              error );
	
		if( read_count != (ssize_t) session_entries_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read session entries data.",
			 function );

			goto on_error;
		}
		total_read_count += read_count;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: session entries data:\n",
			 function );
			libcnotify_print_data(
			 session_entries_data,
			 session_entries_data_size,
			 0 );
		}
#endif
		read_count = libbfio_pool_read_buffer(
		              file_io_pool,
		              file_io_pool_entry,
		              session_footer_data,
		              session_footer_data_size,
		              error );

		if( read_count != (ssize_t) session_footer_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read session footer data.",
			 function );

			goto on_error;
		}
		total_read_count += read_count;

		byte_stream_copy_to_uint32_little_endian(
		 session_footer_data,
		 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: session entries checksum\t\t\t: 0x%08" PRIx32 "\n",
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
				 &( session_footer_data[ 4 ] ),
				 12,
				 0 );
			}
		}
#endif
		calculated_checksum = ewf_checksum_calculate(
		                       session_entries_data,
		                       session_entries_data_size,
		                       1 );

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
		if( libewf_sector_list_empty(
		     sessions,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty sessions sector list.",
			 function );

			goto on_error;
		}
		session_entry_data = session_entries_data;

		if( format_version == 1 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_session_entry_v1_t *) session_entry_data )->flags,
			 previous_flags );

			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_session_entry_v1_t *) session_entry_data )->first_sector,
			 previous_first_sector );
		}
		else if( format_version == 2 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (ewf_session_entry_v2_t *) session_entry_data )->first_sector,
			 previous_first_sector );

			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_session_entry_v2_t *) session_entry_data )->flags,
			 previous_flags );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( format_version == 1 )
			{
				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " flags\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 previous_flags );

				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " first sector\t\t\t: 0 (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 previous_first_sector );
			}
			else if( format_version == 2 )
			{
				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " first sector\t\t\t: 0 (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 previous_first_sector );

				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " flags\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 previous_flags );
			}
			libcnotify_printf(
			 "%s: entry: %02" PRIu32 " unknown1:\n",
			 function,
			 entry_index );

			if( format_version == 1 )
			{
				libcnotify_print_data(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->unknown1,
				 24,
				 0 );
			}
			else if( format_version == 2 )
			{
				libcnotify_print_data(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->unknown1,
				 20,
				 0 );
			}
		}
#endif
		session_entry_data += session_entry_data_size;

		/* Note that EnCase says the first session starts at session 16
		 * This is either some EnCase specific behavior or the value is used for
		 * other purposes.
		 */
		session_first_sector = 0;
		track_first_sector   = 0;

		for( entry_index = 1;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			if( format_version == 1 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->flags,
				 flags );

				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->first_sector,
				 first_sector );
			}
			else if( format_version == 2 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->first_sector,
				 first_sector );

				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->flags,
				 flags );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( format_version == 1 )
				{
					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " flags\t\t\t\t: 0x%08" PRIx32 "\n",
					 function,
					 entry_index,
					 flags );

					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " first sector\t\t\t: %" PRIu64 "\n",
					 function,
					 entry_index,
					 first_sector );
				}
				else if( format_version == 2 )
				{
					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " first sector\t\t\t: %" PRIu64 "\n",
					 function,
					 entry_index,
					 first_sector );

					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " flags\t\t\t\t: 0x%08" PRIx32 "\n",
					 function,
					 entry_index,
					 flags );
				}
				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " unknown1:\n",
				 function,
				 entry_index );

				if( format_version == 1 )
				{
					libcnotify_print_data(
					 ( (ewf_session_entry_v1_t *) session_entry_data )->unknown1,
					 24,
					 0 );
				}
				else if( format_version == 2 )
				{
					libcnotify_print_data(
					 ( (ewf_session_entry_v2_t *) session_entry_data )->unknown1,
					 20,
					 0 );
				}
			}
#endif
			session_entry_data += session_entry_data_size;

			if( first_sector < previous_first_sector )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid first sector: %" PRIu32 " value out of bounds.",
				 function,
				 first_sector );

				goto on_error;
			}
			if( ( flags & 0x00000001UL ) == 0 )
			{
				number_of_sectors = first_sector - session_first_sector;

				if( libewf_sector_list_append_sector(
				     sessions,
				     session_first_sector,
				     (uint64_t) number_of_sectors,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append session to sector list.",
					 function );
	
					goto on_error;
				}
				session_first_sector = first_sector;
			}
			if( ( previous_flags & 0x00000001UL ) != 0 )
			{
				number_of_sectors = first_sector - track_first_sector;

				if( libewf_sector_list_append_sector(
				     tracks,
				     track_first_sector,
				     (uint64_t) number_of_sectors,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append track to sector list.",
					 function );
	
					goto on_error;
				}
				track_first_sector = first_sector;
			}
			previous_first_sector = first_sector;
			previous_flags        = flags;
		}
		memory_free(
		 session_entries_data );

		session_entries_data = NULL;

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
		     session_first_sector,
		     (uint64_t) number_of_sectors,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append session to sector list.",
			 function );

			goto on_error;
		}
		if( ( flags & 0x00000001UL ) != 0 )
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
			     track_first_sector,
			     (uint64_t) number_of_sectors,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append track to sector list.",
				 function );

				goto on_error;
			}
		}
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session section contains no entries.\n",
		 function );
	}
#endif
	return( total_read_count );

on_error:
	if( session_entries_data != NULL )
	{
		memory_free(
		 session_entries_data );
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
         uint8_t format_version,
         off64_t section_offset,
         libewf_sector_list_t *sessions,
         libewf_sector_list_t *tracks,
         libcerror_error_t **error )
{
	uint8_t session_header_data[ 36 ];
	uint8_t session_footer_data[ 16 ];

	uint8_t *session_entry_data         = NULL;
	uint8_t *session_entries_data       = NULL;
	static char *function               = "libewf_section_session_write";
	size64_t section_data_size          = 0;
	size_t section_descriptor_data_size = 0;
	size_t session_entry_data_size      = 0;
	size_t session_entries_data_size    = 0;
	size_t session_footer_data_size     = 0;
	size_t session_header_data_size     = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	uint64_t current_sector             = 0;
	uint64_t session_first_sector       = 0;
	uint64_t session_last_sector        = 0;
	uint64_t session_number_of_sectors  = 0;
	uint64_t track_first_sector         = 0;
	uint64_t track_last_sector          = 0;
	uint64_t track_number_of_sectors    = 0;
	uint32_t calculated_checksum        = 0;
	uint32_t entry_index                = 0;
	uint32_t section_padding_size       = 0;
	int number_of_sessions              = 0;
	int number_of_entries               = 0;
	int number_of_tracks                = 0;
	int session_index                   = 0;
	int track_index                     = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
		session_header_data_size     = sizeof( ewf_session_header_v1_t );
		session_entry_data_size      = sizeof( ewf_session_entry_v1_t );
		session_footer_data_size     = 4;
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
		session_header_data_size     = sizeof( ewf_session_header_v2_t );
		session_entry_data_size      = sizeof( ewf_session_entry_v2_t );
		session_footer_data_size     = 16;
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
	if( libewf_sector_list_get_number_of_elements(
	     sessions,
	     &number_of_sessions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from sessions sector list.",
		 function );

		goto on_error;
	}
	if( number_of_sessions < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of sessions value out of bounds.",
		 function );

		goto on_error;
	}
	if( libewf_sector_list_get_number_of_elements(
	     tracks,
	     &number_of_tracks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from tracks sector list.",
		 function );

		goto on_error;
	}
	if( number_of_tracks < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of tracks value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( number_of_sessions != 0 )
	 && ( number_of_tracks == 0 ) )
	{
		number_of_entries = number_of_sessions;
	}
	else if( ( number_of_sessions == 0 )
	      && ( number_of_tracks != 0 ) )
	{
		number_of_entries = number_of_tracks;
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
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
				number_of_entries++;

				current_sector = track_last_sector;
			}
			else if( ( number_of_sessions > 0 )
			      && ( current_sector >= session_first_sector )
			      && ( current_sector < session_last_sector ) )
			{
				if( ( track_last_sector == 0 )
				 || ( track_last_sector < session_first_sector ) ) 
				{
					number_of_entries++;
				}
				current_sector = session_last_sector;
			}
		}
		while( ( session_index < number_of_sessions )
		    || ( track_index < number_of_tracks ) );
	}
	if( number_of_entries == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of sessions entries value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "\n" );
	}
#endif
	session_entries_data_size = number_of_entries * session_entry_data_size;

	section_data_size = session_header_data_size
	                  + session_entries_data_size
	                  + session_footer_data_size;

	if( libewf_section_set_values(
	     section,
	     LIBEWF_SECTION_TYPE_SESSION_TABLE,
	     (uint8_t *) "session",
	     7,
	     section_offset,
	     section_descriptor_data_size + section_data_size,
	     section_data_size,
	     section_padding_size,
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
			       section,
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

			goto on_error;
		}
		total_write_count += write_count;
	}
	if( memory_set(
	     session_header_data,
	     0,
	     session_header_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear session header data.",
		 function );

		goto on_error;
	}
/* TODO check if number_of_entries is in bounds */
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_session_header_v1_t *) session_header_data )->number_of_entries,
		 (uint32_t) number_of_entries );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_session_header_v2_t *) session_header_data )->number_of_entries,
		 (uint32_t) number_of_entries );

		session_header_data_size -= 12;
	}
	calculated_checksum = ewf_checksum_calculate(
	                       session_header_data,
	                       session_header_data_size - 4,
	                       1 );

	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_session_header_v1_t *) session_header_data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_session_header_v2_t *) session_header_data )->checksum,
		 calculated_checksum );

		session_header_data_size += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session header data:\n",
		 function );
		libcnotify_print_data(
		 session_header_data,
		 session_header_data_size,
		 0 );
	}
#endif
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               session_header_data,
	               session_header_data_size,
	               error );

	if( write_count != (ssize_t) session_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write session header data.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	session_entries_data = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * session_entries_data_size );

	if( session_entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create session entries data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     session_entries_data,
	     0,
	     session_entries_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sessions entries data.",
		 function );

		goto on_error;
	}
	session_entry_data   = session_entries_data;
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " flags\t\t\t\t: 1\n",
				 function,
				 entry_index );

				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " first sector\t\t\t: %" PRIu32 "\n",
				 function,
				 entry_index,
				 track_first_sector );

				libcnotify_printf(
				 "%s: entry: %02" PRIu32 " last sector\t\t\t: %" PRIu32 "\n",
				 function,
				 entry_index,
				 track_last_sector );

				libcnotify_printf(
				 "\n" );
			}
#endif
			/* Note that EnCase says the first track starts at sector 16
			 * This is either some EnCase specific behavior or the value is used for
			 * other purposes.
			 */
			if( ( entry_index == 0 )
			 && ( track_first_sector == 0 ) )
			{
				track_first_sector = 16;
			}
			if( format_version == 1 )
			{
				byte_stream_copy_from_uint32_little_endian(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->flags,
				 1 );

				byte_stream_copy_from_uint32_little_endian(
				 ( (ewf_session_entry_v1_t *) session_entry_data )->first_sector,
				 (uint32_t) track_first_sector );
			}
			else if( format_version == 2 )
			{
				byte_stream_copy_from_uint64_little_endian(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->first_sector,
				 track_first_sector );

				byte_stream_copy_from_uint32_little_endian(
				 ( (ewf_session_entry_v2_t *) session_entry_data )->flags,
				 1 );
			}
			session_entry_data += session_entry_data_size;

			entry_index++;

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
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " flags\t\t\t\t: 0\n",
					 function,
					 entry_index );

					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " first sector\t\t\t: %" PRIu32 "\n",
					 function,
					 entry_index,
					 session_first_sector );

					libcnotify_printf(
					 "%s: entry: %02" PRIu32 " last sector\t\t\t: %" PRIu32 "\n",
					 function,
					 entry_index,
					 session_last_sector );

					libcnotify_printf(
					 "\n" );
				}
#endif
				/* Note that EnCase says the first session starts at sector 16
				 * This is either some EnCase specific behavior or the value is used for
				 * other purposes.
				 */
				if( ( entry_index == 0 )
				 && ( session_first_sector == 0 ) )
				{
					session_first_sector = 16;
				}
				if( format_version == 1 )
				{
					byte_stream_copy_from_uint32_little_endian(
					 ( (ewf_session_entry_v1_t *) session_entry_data )->first_sector,
					 (uint32_t) session_first_sector );
				}
				else if( format_version == 1 )
				{
					byte_stream_copy_from_uint64_little_endian(
					 ( (ewf_session_entry_v2_t *) session_entry_data )->first_sector,
					 session_first_sector );
				}
				session_entry_data += session_entry_data_size;

				entry_index++;
			}
			current_sector = session_last_sector;
		}
		if( entry_index >= (uint32_t) number_of_entries )
		{
			break;
		}
	}
	while( ( session_index < number_of_sessions )
	    || ( track_index < number_of_tracks ) );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session entries data:\n",
		 function );
		libcnotify_print_data(
		 session_entries_data,
		 session_entries_data_size,
		 0 );
	}
#endif
	write_count = libbfio_pool_write_buffer(
		       file_io_pool,
		       file_io_pool_entry,
		       session_entries_data,
		       session_entries_data_size,
		       error );

	if( write_count != (ssize_t) session_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write session entries data.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	calculated_checksum = ewf_checksum_calculate(
			       session_entries_data,
			       session_entries_data_size,
			       1 );

	memory_free(
	 session_entries_data );

	session_entries_data = NULL;

	if( memory_set(
	     session_footer_data,
	     0,
	     session_footer_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear session footer data.",
		 function );

		goto on_error;
	}
	byte_stream_copy_from_uint32_little_endian(
	 session_footer_data,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: session footer data:\n",
		 function );
		libcnotify_print_data(
		 session_footer_data,
		 session_footer_data_size,
		 0 );
	}
#endif
	write_count = libbfio_pool_write_buffer(
		       file_io_pool,
		       file_io_pool_entry,
		       session_footer_data,
		       session_footer_data_size,
		       error );

	if( write_count != (ssize_t) session_footer_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write session footer data.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	if( format_version == 2 )
	{
		write_count = libewf_section_descriptor_write(
			       section,
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

			goto on_error;
		}
		total_write_count += write_count;
	}
	return( total_write_count );

on_error:
	if( session_entries_data != NULL )
	{
		memory_free(
		 session_entries_data );
	}
	return( -1 );
}

/* Reads a version 1 table or table2 section header or version 2 sector table section header
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_table_header_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t format LIBEWF_ATTRIBUTE_UNUSED,
         uint32_t *number_of_entries,
         uint64_t *base_offset,
         libcerror_error_t **error )
{
	uint8_t table_header_data[ 32 ];

	static char *function         = "libewf_section_table_header_read";
	size_t table_header_data_size = 0;
	ssize_t read_count            = 0;
	uint32_t calculated_checksum  = 0;
	uint32_t stored_checksum      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit          = 0;
	uint32_t value_32bit          = 0;
#endif

	LIBEWF_UNREFERENCED_PARAMETER( format )

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		table_header_data_size = sizeof( ewf_table_header_v1_t );
	}
	else if( format_version == 2 )
	{
		table_header_data_size = sizeof( ewf_table_header_v2_t );
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
	if( section->data_size < (size64_t) table_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section data size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              table_header_data,
	              table_header_data_size,
	              error );
	
	if( read_count != (ssize_t) table_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table header data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: table header data:\n",
		 function );
		libcnotify_print_data(
		 table_header_data,
		 table_header_data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_header_data )->number_of_entries,
		 *number_of_entries );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_table_header_v1_t *) table_header_data )->base_offset,
		 *base_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_header_data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_header_data )->number_of_entries,
		 *number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_header_data )->checksum,
		 stored_checksum );

		/* The 12 byte alignment padding is not part of the table header
		 */
		table_header_data_size -= 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( format_version == 2 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (ewf_table_header_v2_t *) table_header_data )->unknown1,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );
		}
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 *number_of_entries );

		if( format_version == 1 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v1_t *) table_header_data )->padding1,
			 value_32bit );
			libcnotify_printf(
			 "%s: padding1\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: base offset\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 *base_offset );

			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v1_t *) table_header_data )->padding2,
			 value_32bit );
			libcnotify_printf(
			 "%s: padding2\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		else if( format_version == 2 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v2_t *) table_header_data )->unknown2,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
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
			 ( (ewf_table_header_v2_t *) table_header_data )->padding,
			 12,
			 0 );
		}
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       table_header_data,
	                       table_header_data_size - 4,
	                       1 );

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

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( *number_of_entries == 0 )
		{
			libcnotify_printf(
			 "%s: table contains no entries.\n",
			 function );
		}
		else if( ( ( format != LIBEWF_FORMAT_ENCASE6 )
		       &&  ( *number_of_entries > EWF_MAXIMUM_TABLE_ENTRIES ) )
		      || ( ( format == LIBEWF_FORMAT_ENCASE6 )
		       &&  ( *number_of_entries > EWF_MAXIMUM_TABLE_ENTRIES_ENCASE6 ) ) )
		{
			libcnotify_printf(
			 "%s: number of entries: %" PRIu32 " exceeds maximum.\n",
			 function,
			 number_of_entries );
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
         uint8_t format_version,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         off64_t base_offset,
         const uint8_t *table_entries_data,
         uint32_t number_of_entries,
         size64_t chunks_data_size,
         uint8_t segment_file_type,
         libcerror_error_t **error )
{
	uint8_t table_header_data[ 32 ];
	uint8_t table_footer_data[ 16 ];

	static char *function               = "libewf_section_table_write";
	size64_t section_data_size          = 0;
	size_t section_descriptor_data_size = 0;
	size_t table_entry_data_size        = 0;
	size_t table_entries_data_size      = 0;
	size_t table_header_data_size       = 0;
	size_t table_footer_data_size       = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	uint32_t calculated_checksum        = 0;
	uint32_t section_padding_size       = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( base_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid base offset value less than zero.",
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
	table_entries_data_size = number_of_entries * table_entry_data_size;

	section_data_size = table_header_data_size
	                  + table_entries_data_size
	                  + chunks_data_size;

	if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		section_data_size += table_footer_data_size;
	}
	if( libewf_section_set_values(
	     section,
	     LIBEWF_SECTION_TYPE_SECTOR_TABLE,
	     type_string,
	     type_string_length,
	     section_offset,
	     section_descriptor_data_size + section_data_size,
	     section_data_size,
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
			       section,
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
	if( memory_set(
	     table_header_data,
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
		 ( (ewf_table_header_v1_t *) table_header_data )->number_of_entries,
		 number_of_entries );

		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_table_header_v1_t *) table_header_data )->base_offset,
		 base_offset );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_header_data )->number_of_entries,
		 number_of_entries );

		table_header_data_size -= 12;
	}
	calculated_checksum = ewf_checksum_calculate(
	                       table_header_data,
	                       table_header_data_size - 4,
	                       1 );

	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_header_data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_header_data )->checksum,
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
		 table_header_data,
		 table_header_data_size,
		 0 );
	}
#endif
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               table_header_data,
	               table_header_data_size,
	               error );

	if( write_count != (ssize_t) table_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write table header data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table entries data:\n",
		 function );
		libcnotify_print_data(
		 table_entries_data,
		 table_entries_data_size,
		 0 );
	}
#endif
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               table_entries_data,
	               table_entries_data_size,
	               error );

	if( write_count != (ssize_t) table_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write table entries data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		calculated_checksum = ewf_checksum_calculate(
		                       table_entries_data,
		                       table_entries_data_size,
		                       1 );

		if( memory_set(
		     table_footer_data,
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
		 table_footer_data,
		 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table footer data:\n",
			 function );
			libcnotify_print_data(
			 table_footer_data,
			 table_footer_data_size,
			 0 );
		}
#endif
		write_count = libbfio_pool_write_buffer(
		               file_io_pool,
		               file_io_pool_entry,
	        	       table_footer_data,
	        	       table_footer_data_size,
		               error );

		if( write_count != (ssize_t) table_footer_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write table footer data.",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	if( format_version == 2 )
	{
		write_count = libewf_section_descriptor_write(
			       section,
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

/* Reads an EWF-E01 (EnCase) volume section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_volume_e01_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libcerror_error_t **error )
{
	ewf_volume_t *volume         = NULL;
	static char *function        = "libewf_io_handle_read_volume_e01_section";
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit         = 0;
#endif

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( section->data_size != (size64_t) sizeof( ewf_volume_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	volume = memory_allocate_structure(
	          ewf_volume_t );

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to read volume.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) volume,
	              sizeof( ewf_volume_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_volume_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: volume data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) volume,
		 sizeof( ewf_volume_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
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
	     media_values->set_identifier,
	     volume->set_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy set identifier.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 volume->checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: media type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_type );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 volume->unknown1,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: number of chunks\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_chunks );

		libcnotify_printf(
		 "%s: sectors per chunk\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libcnotify_printf(
		 "%s: number of sectors\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		byte_stream_copy_to_uint32_little_endian(
		 volume->chs_cylinders,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of cylinders\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 volume->chs_heads,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of heads\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 volume->chs_sectors,
		 value_32bit );
		libcnotify_printf(
		 "%s: CHS number of sectors\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: media flags\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_flags );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 volume->unknown2,
		 3,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 volume->palm_volume_start_sector,
		 value_32bit );
		libcnotify_printf(
		 "%s: PALM volume start sector\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 volume->unknown3,
		 4,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 volume->smart_logs_start_sector,
		 value_32bit );
		libcnotify_printf(
		 "%s: SMART logs start sector\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: compression level\t\t: 0x%02" PRIx8 "\n",
		 function,
		 io_handle->compression_level );

		libcnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libcnotify_print_data(
		 volume->unknown4,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: error granularity\t\t: %" PRIu32 "\n",
		 function,
		 media_values->error_granularity );

		libcnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libcnotify_print_data(
		 volume->unknown5,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: set identifier:\n",
		 function );
		libcnotify_print_data(
		 volume->set_identifier,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: unknown6:\n",
		 function );
		libcnotify_print_data(
		 volume->unknown6,
		 963,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		libcnotify_printf(
		 "%s: signature:\n",
		 function );
		libcnotify_print_data(
		 volume->signature,
		 5,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       volume,
	                       sizeof( ewf_volume_t ) - 4,
	                       1 );

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
	 volume );

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
         libcerror_error_t **error )
{
	ewf_volume_t *volume         = NULL;
	static char *function        = "libewf_section_volume_e01_write";
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( libewf_section_set_values(
	     section,
	     0,
	     (uint8_t *) "volume",
	     6,
	     section_offset,
	     sizeof( ewf_section_descriptor_v1_t ) + sizeof( ewf_volume_t ),
	     sizeof( ewf_volume_t ),
	     0,
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
	write_count = libewf_section_descriptor_write(
	               section,
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

		goto on_error;
	}
	total_write_count += write_count;

	volume = memory_allocate_structure(
	          ewf_volume_t );

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     volume,
	     0,
	     sizeof( ewf_volume_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: media type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_type );

		libcnotify_printf(
		 "%s: number of chunks\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_chunks );

		libcnotify_printf(
		 "%s: sectors per chunk\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libcnotify_printf(
		 "%s: number of sectors\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		libcnotify_printf(
		 "%s: media flags\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_flags );

		libcnotify_printf(
		 "%s: compression level\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 io_handle->compression_level );

		libcnotify_printf(
		 "%s: error granularity\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->error_granularity );

		libcnotify_printf(
		 "%s: set identifier:\n",
		 function );
		libcnotify_print_data(
		 media_values->set_identifier,
		 16,
		 0 );
	}
#endif
	volume->media_type = media_values->media_type;
	volume->media_flags = media_values->media_flags;

/* TODO check if media values -> number of chunks does not exceed the 32-bit maximum */
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
		     volume->set_identifier,
		     media_values->set_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy set identifier.",
			 function );

			goto on_error;
		}
		byte_stream_copy_from_uint32_little_endian(
		 volume->error_granularity,
		 media_values->error_granularity );
	}
	calculated_checksum = ewf_checksum_calculate(
	                       volume,
	                       sizeof( ewf_volume_t ) - 4,
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 volume->checksum,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume has %" PRIu64 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
		 function,
		 media_values->number_of_chunks,
		 media_values->chunk_size,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: volume has %" PRIu64 " sectors of %" PRIi32 " bytes each.\n",
		 function,
		 media_values->number_of_sectors,
		 media_values->bytes_per_sector );
	}
#endif
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) volume,
	               sizeof( ewf_volume_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_volume_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
         libcerror_error_t **error )
{
	ewf_volume_smart_t *volume   = NULL;
	static char *function        = "libewf_section_volume_s01_read";
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( section->data_size != (size64_t) sizeof( ewf_volume_smart_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		goto on_error;
	}
	volume = memory_allocate_structure(
	          ewf_volume_smart_t );

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) volume,
	              sizeof( ewf_volume_smart_t ),
	              error );

	if( read_count != (ssize_t) sizeof( ewf_volume_smart_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) volume,
		 sizeof( ewf_volume_smart_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
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
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 volume->unknown1,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: number of chunks\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_chunks );

		libcnotify_printf(
		 "%s: sectors per chunk\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libcnotify_printf(
		 "%s: number of sectors\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 volume->unknown2,
		 20,
		 0 );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 volume->unknown3,
		 45,
		 0 );

		libcnotify_printf(
		 "%s: signature:\n",
		 function );
		libcnotify_print_data(
		 volume->signature,
		 5,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
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
	                       sizeof( ewf_volume_smart_t ) - 4,
	                       1 );

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
	 volume );

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
         libcerror_error_t **error )
{
	ewf_volume_smart_t *volume   = NULL;
	static char *function        = "libewf_section_volume_s01_write";
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
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
	if( libewf_section_set_values(
	     section,
	     0,
	     (uint8_t *) "volume",
	     6,
	     section_offset,
	     sizeof( ewf_section_descriptor_v1_t ) + sizeof( ewf_volume_smart_t ),
	     sizeof( ewf_volume_smart_t ),
	     0,
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
	write_count = libewf_section_descriptor_write(
	               section,
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

		goto on_error;
	}
	total_write_count += write_count;

	volume = memory_allocate_structure(
	          ewf_volume_smart_t );

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     volume,
	     0,
	     sizeof( ewf_volume_smart_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: media type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 media_values->media_type );

		libcnotify_printf(
		 "%s: number of chunks\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_chunks );

		libcnotify_printf(
		 "%s: sectors per chunk\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu32 "\n",
		 function,
		 media_values->bytes_per_sector );

		libcnotify_printf(
		 "%s: number of sectors\t\t\t: %" PRIu64 "\n",
		 function,
		 media_values->number_of_sectors );

		libcnotify_printf(
		 "\n" );
	}
#endif
	volume->unknown1[ 0 ] = 1;

/* TODO check if media values -> number of chunks does not exceed the 32-bit maximum */
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
	                       sizeof( ewf_volume_smart_t ) - 4,
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 volume->checksum,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume has %" PRIu64 " chunks of %" PRIi32 " bytes (%" PRIi32 " sectors) each.\n",
		 function,
		 media_values->number_of_chunks,
		 media_values->chunk_size,
		 media_values->sectors_per_chunk );

		libcnotify_printf(
		 "%s: volume has %" PRIu64 " sectors of %" PRIi32 " bytes each.\n",
		 function,
		 media_values->number_of_sectors,
		 media_values->bytes_per_sector );
	}
#endif
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) volume,
	               sizeof( ewf_volume_smart_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewf_volume_smart_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
         libcerror_error_t **error )
{
	uint8_t *xhash        = NULL;
	static char *function = "libewf_section_xhash_read";
	ssize_t read_count    = 0;
	size_t xhash_size     = 0;

	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read xhash.",
		 function );

		goto on_error;
	}
	if( xhash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing xhash.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		 if( libewf_debug_utf8_stream_print(
		      "XHash",
		      xhash,
		      xhash_size,
		      error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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
         libcerror_error_t **error )
{
	static char *function = "libewf_section_xhash_write";
	ssize_t write_count   = 0;

	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_utf8_stream_print(
		     "XHash",
		     hash_sections->xhash,
		     hash_sections->xhash_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
         libcerror_error_t **error )
{
	uint8_t *xheader      = NULL;
	static char *function = "libewf_section_xheader_read";
	ssize_t read_count    = 0;
	size_t xheader_size   = 0;

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read xheader.",
		 function );

		goto on_error;
	}
	if( xheader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing xheader.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_utf8_stream_print(
		     "XHeader",
		     xheader,
		     xheader_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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
         libcerror_error_t **error )
{
	static char *function = "libewf_section_xheader_write";
	ssize_t write_count   = 0;

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_utf8_stream_print(
		     "XHeader",
		     header_sections->xheader,
		     header_sections->xheader_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
         libcerror_error_t **error )
{
	ewfx_delta_chunk_header_t delta_chunk_header;

	static char *function        = "libewf_section_delta_chunk_read";
	size64_t section_data_size   = 0;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( chunk_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk index.",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk size.",
		 function );

		return( -1 );
	}
	if( section->data_size < (size64_t) sizeof( ewfx_delta_chunk_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	section_data_size = section->data_size - sizeof( ewfx_delta_chunk_header_t );

	if( section_data_size > (size64_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              (uint8_t *) &delta_chunk_header,
	              sizeof( ewfx_delta_chunk_header_t ),
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read delta chunk header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: delta chunk header data:\n",
		 function );
		libcnotify_print_data(
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
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: chunk\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *chunk_index );

		libcnotify_printf(
	 	 "%s: chunk size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *chunk_size );

		libcnotify_printf(
	 	 "%s: padding:\n",
		 function );
		libcnotify_print_data(
		 delta_chunk_header.padding,
		 6,
		 0 );

		libcnotify_printf(
	 	 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
	 	 "\n" );
	}
#endif
	calculated_checksum = ewf_checksum_calculate(
	                       &delta_chunk_header,
	                       sizeof( ewfx_delta_chunk_header_t ) - 4,
	                       1 );

	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: %" PRIu32 ", calculated: %" PRIu32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
	if( *chunk_index == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_INVALID_DATA,
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
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
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
         uint32_t chunk_checksum,
         uint8_t write_checksum,
         libcerror_error_t **error )
{
	ewfx_delta_chunk_header_t delta_chunk_header;

	static char *function        = "libewf_section_delta_chunk_write";
	size64_t section_data_size   = 0;
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;
	uint32_t calculated_checksum = 0;
	uint32_t write_size          = 0;

	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section.",
		 function );

		return( -1 );
	}
	if( ( chunk_index + 1 ) > (uint32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk index value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( chunk_size > (uint32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
	write_size = chunk_size;

	if( write_checksum != 0 )
	{
		write_size += 4;
	}
	if( write_size > (uint32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid write size value out of bounds.",
		 function );

		return( -1 );
	}
	section_data_size = sizeof( ewfx_delta_chunk_header_t )
	                  + write_size;

	if( libewf_section_set_values(
	     section,
	     0,
	     (uint8_t *) "delta_chunk",
	     11,
	     section_offset,
	     sizeof( ewf_section_descriptor_v1_t ) + section_data_size,
	     section_data_size,
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
	               section,
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

	if( memory_set(
	     &delta_chunk_header,
	     0,
	     sizeof( ewfx_delta_chunk_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
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
	                       sizeof( ewfx_delta_chunk_header_t ) - 4,
	                       1 );

	byte_stream_copy_from_uint32_little_endian(
	 delta_chunk_header.checksum,
	 calculated_checksum );

	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               (uint8_t *) &delta_chunk_header,
	               sizeof( ewfx_delta_chunk_header_t ),
	               error );

	if( write_count != (ssize_t) sizeof( ewfx_delta_chunk_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid checksum buffer.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint32_little_endian(
		 checksum_buffer,
		 chunk_checksum );

		if( checksum_buffer == &( chunk_buffer[ chunk_size ] ) )
		{
			write_size += 4;

			write_checksum = 0;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: chunk: %" PRIu32 " file IO pool entry\t\t: %d\n",
		 function,
		 chunk_index - 1,
		 file_io_pool_entry );

		libcnotify_printf(
		 "%s: chunk: %" PRIu32 " offset\t\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 chunk_index - 1,
		 section_offset + total_write_count,
		 section_offset + total_write_count );

		libcnotify_printf(
		 "%s: chunk: %" PRIu32 " size\t\t\t\t: %" PRIzd "\n",
		 function,
		 chunk_index - 1,
		 chunk_size );

		if( ( write_checksum == 0 )
		 && ( chunk_size >= 4 ) )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( chunk_buffer[ chunk_size - 4 ] ),
			 chunk_checksum );
		}
		libcnotify_printf(
		 "%s: chunk: %" PRIu32 " checksum\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 chunk_index - 1,
		 chunk_checksum );

		libcnotify_printf(
		 "%s: chunk: %" PRIu32 " flags:\n",
		 function,
		 chunk_index - 1 );
		libcnotify_printf(
		 "Has checksum\n" );
		libcnotify_printf(
		 "Is delta\n" );

		libcnotify_printf(
		 "\n" );
	}
#endif
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               chunk_buffer,
	               (size_t) write_size,
	               error );

	if( write_count != (ssize_t) write_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( write_checksum != 0 )
	{
		write_count = libbfio_pool_write_buffer(
		               file_io_pool,
		               file_io_pool_entry,
			       checksum_buffer,
			       4,
		               error );

		if( write_count != (ssize_t) 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write checksum.",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}

