/*
 * Section descriptor functions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include "libewf_checksum.h"
#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_section_descriptor.h"

#include "ewf_file_header.h"
#include "ewf_section.h"

/* Creates a section descriptor
 * Make sure the value section_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_section_descriptor_initialize(
     libewf_section_descriptor_t **section_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_descriptor_initialize";

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
	if( *section_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid section descriptor value already set.",
		 function );

		return( -1 );
	}
	*section_descriptor = memory_allocate_structure(
	                       libewf_section_descriptor_t );

	if( *section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *section_descriptor,
	     0,
	     sizeof( libewf_section_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear section descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *section_descriptor != NULL )
	{
		memory_free(
		 *section_descriptor );

		*section_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a section descriptor
 * Returns 1 if successful or -1 on error
 */
int libewf_section_descriptor_free(
     libewf_section_descriptor_t **section_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_descriptor_free";

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
	if( *section_descriptor != NULL )
	{
		memory_free(
		 *section_descriptor );

		*section_descriptor = NULL;
	}
	return( 1 );
}

/* Clones the section descriptor
 * Returns 1 if successful or -1 on error
 */
int libewf_section_descriptor_clone(
     libewf_section_descriptor_t **destination_section_descriptor,
     libewf_section_descriptor_t *source_section_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_descriptor_clone";

	if( destination_section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination section descriptor.",
		 function );

		return( -1 );
	}
	if( *destination_section_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination section descriptor value already set.",
		 function );

		return( -1 );
	}
	if( source_section_descriptor == NULL )
	{
		*destination_section_descriptor = NULL;

		return( 1 );
	}
	*destination_section_descriptor = memory_allocate_structure(
	                                   libewf_section_descriptor_t );

	if( *destination_section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination section descriptor.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_section_descriptor,
	     source_section_descriptor,
	     sizeof( libewf_section_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination section descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_section_descriptor != NULL )
	{
		memory_free(
		 *destination_section_descriptor );

		*destination_section_descriptor = NULL;
	}
	return( -1 );
}

/* Sets the section values
 * Returns 1 if successful or -1 on error
 */
int libewf_section_descriptor_set(
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
	static char *function = "libewf_section_descriptor_set";

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
	if( section_offset < 0 )
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
int libewf_section_descriptor_read_data(
     libewf_section_descriptor_t *section_descriptor,
     const uint8_t *data,
     size_t data_size,
     off64_t file_offset,
     uint8_t format_version,
     libcerror_error_t **error )
{
	static char *function               = "libewf_section_descriptor_read_data";
	size_t section_descriptor_data_size = 0;
	uint64_t safe_end_offset            = 0;
	uint64_t safe_start_offset          = 0;
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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data.",
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
	if( data_size != section_descriptor_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: section descriptor data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		if( memory_copy(
		     section_descriptor->type_string,
		     ( (ewf_section_descriptor_v1_t *) data )->type_string,
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
		section_descriptor->type_string[ 16 ] = 0;

		section_descriptor->type_string_length = narrow_string_length(
		                                          (char *) section_descriptor->type_string );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) data )->size,
		 section_descriptor->size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) data )->next_offset,
		 safe_end_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v1_t *) data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->type,
		 section_descriptor->type );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->data_flags,
		 section_descriptor->data_flags );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->previous_offset,
		 safe_start_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->data_size,
		 section_descriptor->data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->padding_size,
		 section_descriptor->padding_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->descriptor_size,
		 section_descriptor_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->checksum,
		 stored_checksum );

		if( memory_copy(
		     section_descriptor->data_integrity_hash,
		     ( (ewf_section_descriptor_v2_t *) data )->data_integrity_hash,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set data integrity hash.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: type string\t\t\t: %s\n",
			 function,
			 (char *) section_descriptor->type_string );

			libcnotify_printf(
			 "%s: next offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 safe_end_offset );

			libcnotify_printf(
			 "%s: size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section_descriptor->size );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v1_t *) data )->padding,
			 40,
			 0 );
		}
		else if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: type\t\t\t\t: 0x%08" PRIx32 " (",
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
			 safe_start_offset );

			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section_descriptor->data_size );

			libcnotify_printf(
			 "%s: section descriptor size\t\t: %" PRIu32 "\n",
			 function,
			 section_descriptor_size );

			libcnotify_printf(
			 "%s: padding size\t\t\t: %" PRIu32 "\n",
			 function,
			 section_descriptor->padding_size );

			libcnotify_printf(
			 "%s: data integrity hash:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v2_t *) data )->data_integrity_hash,
			 16,
			 0 );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v2_t *) data )->padding,
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
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     data_size - 4,
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
		if( ( safe_end_offset < (uint64_t) file_offset )
		 || ( safe_end_offset > (uint64_t) INT64_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section next offset value out of bounds.",
			 function );

			return( -1 );
		}
		section_descriptor->end_offset = (off64_t) safe_end_offset;

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

			return( -1 );
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
		if( ( file_offset < 0 )
		 || ( file_offset > (off64_t) ( INT64_MAX - sizeof( ewf_section_descriptor_v2_t ) ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid file offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( safe_start_offset > (uint64_t) file_offset )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section previous offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( safe_start_offset == 0 )
		{
			section_descriptor->start_offset = sizeof( ewf_file_header_v2_t );
		}
		else
		{
			if( safe_start_offset < (uint64_t) sizeof( ewf_file_header_v2_t ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid section previous offset value out of bounds.",
				 function );

				return( -1 );
			}
			section_descriptor->start_offset = (off64_t) ( safe_start_offset + sizeof( ewf_section_descriptor_v2_t ) );
		}
		section_descriptor->end_offset = file_offset + (off64_t) sizeof( ewf_section_descriptor_v2_t );
		section_descriptor->size       = (size64_t) ( section_descriptor->end_offset - section_descriptor->start_offset );

		if( section_descriptor->data_size > section_descriptor->size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section data size value out of bounds.",
			 function );

			return( -1 );
		}
		if( section_descriptor->padding_size > section_descriptor->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section padding size value out of bounds.",
			 function );

			return( -1 );
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

					return( -1 );
				}
				if( section_descriptor->size != sizeof( ewf_section_descriptor_v1_t ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid section size value out of bounds.",
					 function );

					return( -1 );
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

					return( -1 );
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

					return( -1 );
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

					return( -1 );
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

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads a section descriptor
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_descriptor_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t file_offset,
         uint8_t format_version,
         libcerror_error_t **error )
{
	uint8_t *section_descriptor_data    = NULL;
	static char *function               = "libewf_section_descriptor_read_file_io_pool";
	size_t section_descriptor_data_size = 0;
	ssize_t read_count                  = 0;

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading section descriptor from file IO pool entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_io_pool_entry,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_pool_read_buffer_at_offset(
	              file_io_pool,
	              file_io_pool_entry,
	              section_descriptor_data,
	              section_descriptor_data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) section_descriptor_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section descriptor data at offset: %" PRIi64 " (0x%08" PRIx64 ") from file IO pool entry: %d.",
		 function,
		 file_offset,
		 file_offset,
		 file_io_pool_entry );

		goto on_error;
	}
	if( libewf_section_descriptor_read_data(
	     section_descriptor,
	     section_descriptor_data,
	     section_descriptor_data_size,
	     file_offset,
	     format_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section descriptor.",
		 function,
		 file_io_pool_entry );

		goto on_error;
	}
	memory_free(
	 section_descriptor_data );

	section_descriptor_data = NULL;

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
int libewf_section_descriptor_write_data(
     libewf_section_descriptor_t *section_descriptor,
     uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libcerror_error_t **error )
{
	static char *function               = "libewf_section_descriptor_write_data";
	size_t section_descriptor_data_size = 0;
	off64_t previous_offset             = 0;
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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data.",
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
	if( data_size != section_descriptor_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     data,
	     0,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		if( memory_copy(
		     ( (ewf_section_descriptor_v1_t *) data )->type_string,
		     section_descriptor->type_string,
		     section_descriptor->type_string_length ) == NULL )
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
		 ( (ewf_section_descriptor_v1_t *) data )->size,
		 section_descriptor->size );

		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_section_descriptor_v1_t *) data )->next_offset,
		 section_descriptor->end_offset );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->type,
		 section_descriptor->type );

		if( section_descriptor->start_offset > (off64_t) sizeof( ewf_section_descriptor_v2_t ) )
		{
			previous_offset = section_descriptor->start_offset - sizeof( ewf_section_descriptor_v2_t );
		}
		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->previous_offset,
		 previous_offset );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->data_flags,
		 section_descriptor->data_flags );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->data_size,
		 section_descriptor->data_size );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->descriptor_size,
		 (uint32_t) sizeof( ewf_section_descriptor_v2_t ) );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->padding_size,
		 section_descriptor->padding_size );

		if( memory_copy(
		     ( (ewf_section_descriptor_v2_t *) data )->data_integrity_hash,
		     section_descriptor->data_integrity_hash,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set data integrity hash.",
			 function );

			return( -1 );
		}
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     data_size - 4,
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
		 ( (ewf_section_descriptor_v1_t *) data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_section_descriptor_v2_t *) data )->checksum,
		 calculated_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: section descriptor data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: type string\t\t\t: %s\n",
			 function,
			 (char *) section_descriptor->type_string );

			libcnotify_printf(
			 "%s: next offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 section_descriptor->end_offset );

			libcnotify_printf(
			 "%s: size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 section_descriptor->size );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v1_t *) data )->padding,
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
			 data_size );

			libcnotify_printf(
			 "%s: padding size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 section_descriptor->padding_size );

			libcnotify_printf(
			 "%s: data integrity hash:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v2_t *) data )->data_integrity_hash,
			 16,
			 0 );

			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_section_descriptor_v2_t *) data )->padding,
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
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Writes a section descriptor
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_section_descriptor_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libcerror_error_t **error )
{
	uint8_t *section_descriptor_data    = NULL;
	static char *function               = "libewf_section_descriptor_write_file_io_pool";
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
	if( libewf_section_descriptor_write_data(
	     section_descriptor,
	     section_descriptor_data,
	     section_descriptor_data_size,
	     format_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write section descriptor data.",
		 function );

		goto on_error;
	}
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

