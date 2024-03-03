/*
 * Logical Evidence File (LEF) file entry functions
 *
 * Copyright (C) 2006-2024, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_lef_extended_attribute.h"
#include "libewf_lef_file_entry.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfguid.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_serialized_string.h"
#include "libewf_value_reader.h"

const uint8_t libewf_lef_extended_attributes_header[ 37 ] = {
	0x00, 0x00, 0x00, 0x00, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x41, 0x00, 0x74,
	0x00, 0x74, 0x00, 0x72, 0x00, 0x69, 0x00, 0x62, 0x00, 0x75, 0x00, 0x74, 0x00, 0x65, 0x00, 0x73,
	0x00, 0x00, 0x00, 0x00, 0x00 };

/* Creates a file entry
 * Make sure the value lef_file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_initialize(
     libewf_lef_file_entry_t **lef_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_initialize";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *lef_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
	*lef_file_entry = memory_allocate_structure(
	                   libewf_lef_file_entry_t );

	if( *lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *lef_file_entry,
	     0,
	     sizeof( libewf_lef_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file entry.",
		 function );

		memory_free(
		 *lef_file_entry );

		*lef_file_entry = NULL;

		return( -1 );
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_file_entry )->guid ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_file_entry )->short_name ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create short name string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_file_entry )->md5_hash ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MD5 hash string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_initialize(
	     &( ( *lef_file_entry )->sha1_hash ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create SHA1 hash string.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *lef_file_entry )->extended_attributes ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extended attributes array.",
		 function );

		goto on_error;
	}
	( *lef_file_entry )->data_offset            = -1;
	( *lef_file_entry )->duplicate_data_offset  = -1;
	( *lef_file_entry )->permission_group_index = 0;

	return( 1 );

on_error:
	if( *lef_file_entry != NULL )
	{
		if( ( *lef_file_entry )->sha1_hash != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_file_entry )->sha1_hash ),
			 NULL );
		}
		if( ( *lef_file_entry )->md5_hash != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_file_entry )->md5_hash ),
			 NULL );
		}
		if( ( *lef_file_entry )->short_name != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_file_entry )->short_name ),
			 NULL );
		}
		if( ( *lef_file_entry )->guid != NULL )
		{
			libewf_serialized_string_free(
			 &( ( *lef_file_entry )->guid ),
			 NULL );
		}
		memory_free(
		 *lef_file_entry );

		*lef_file_entry = NULL;
	}
	return( -1 );
}

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_free(
     libewf_lef_file_entry_t **lef_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_free";
	int result            = 1;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *lef_file_entry != NULL )
	{
		if( ( *lef_file_entry )->guid != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_file_entry )->guid ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free GUID string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_file_entry )->name_data != NULL )
		{
			memory_free(
			 ( *lef_file_entry )->name_data );
		}
		if( ( *lef_file_entry )->short_name != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_file_entry )->short_name ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free short name string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_file_entry )->md5_hash != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_file_entry )->md5_hash ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free MD5 hash string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_file_entry )->sha1_hash != NULL )
		{
			if( libewf_serialized_string_free(
			     &( ( *lef_file_entry )->sha1_hash ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free SHA1 hash string.",
				 function );

				result = -1;
			}
		}
		if( ( *lef_file_entry )->extended_attributes != NULL )
		{
			if( libcdata_array_free(
			     &( ( *lef_file_entry )->extended_attributes ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_extended_attribute_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free extended attributes array.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *lef_file_entry );

		*lef_file_entry = NULL;
	}
	return( result );
}

/* Clones the file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_clone(
     libewf_lef_file_entry_t **destination_lef_file_entry,
     libewf_lef_file_entry_t *source_lef_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_clone";

	if( destination_lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination file entry.",
		 function );

		return( -1 );
	}
	if( *destination_lef_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination file entry value already set.",
		 function );

		return( -1 );
	}
	if( source_lef_file_entry == NULL )
	{
		*destination_lef_file_entry = NULL;

		return( 1 );
	}
	*destination_lef_file_entry = memory_allocate_structure(
			               libewf_lef_file_entry_t );

	if( *destination_lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination file entry.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_lef_file_entry,
	     source_lef_file_entry,
	     sizeof( libewf_lef_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination file entry.",
		 function );

		memory_free(
		 *destination_lef_file_entry );

		*destination_lef_file_entry = NULL;

		return( -1 );
	}
	( *destination_lef_file_entry )->guid                = NULL;
	( *destination_lef_file_entry )->name_data           = NULL;
	( *destination_lef_file_entry )->short_name          = NULL;
	( *destination_lef_file_entry )->md5_hash            = NULL;
	( *destination_lef_file_entry )->sha1_hash           = NULL;
	( *destination_lef_file_entry )->extended_attributes = NULL;

	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_file_entry )->guid ),
	     source_lef_file_entry->guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination GUID string.",
		 function );

		goto on_error;
	}
	( *destination_lef_file_entry )->name_data = (uint8_t *) memory_allocate(
	                                                          source_lef_file_entry->name_data_size );

	if( ( *destination_lef_file_entry )->name_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination name data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     ( *destination_lef_file_entry )->name_data,
	     source_lef_file_entry->name_data,
	     source_lef_file_entry->name_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination name data.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_file_entry )->short_name ),
	     source_lef_file_entry->short_name,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination short name string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_file_entry )->md5_hash ),
	     source_lef_file_entry->md5_hash,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination MD5 hash string.",
		 function );

		goto on_error;
	}
	if( libewf_serialized_string_clone(
	     &( ( *destination_lef_file_entry )->sha1_hash ),
	     source_lef_file_entry->sha1_hash,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone destination SHA1 hash string.",
		 function );

		goto on_error;
	}
	if( libcdata_array_clone(
	     &( ( *destination_lef_file_entry )->extended_attributes ),
	     source_lef_file_entry->extended_attributes,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_extended_attribute_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_lef_extended_attribute_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination extended attributes array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 destination_lef_file_entry,
		 NULL );
	}
	return( -1 );
}

/* Reads a single file binary extents
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_read_binary_extents(
     libewf_lef_file_entry_t *lef_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_lef_file_entry_read_binary_extents";
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int number_of_values                  = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     data,
	     data_size,
	     (uint8_t) ' ',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string into values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     values,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values",
		 function );

		goto on_error;
	}
	if( ( number_of_values != 1 )
	 && ( number_of_values != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of values.",
		 function );

		goto on_error;
	}
	if( number_of_values == 3 )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     values,
		     1,
		     &value_string,
		     &value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value string: 1.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_copy_to_integer(
		     value_string,
		     value_string_size,
		     &value_64bit,
		     64,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set data offset.",
			 function );

			goto on_error;
		}
		lef_file_entry->data_offset = (off64_t) value_64bit;

		if( libfvalue_split_utf8_string_get_segment_by_index(
		     values,
		     2,
		     &value_string,
		     &value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value string: 2.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_copy_to_integer(
		     value_string,
		     value_string_size,
		     &value_64bit,
		     64,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set data size.",
			 function );

			goto on_error;
		}
		lef_file_entry->data_size = (size64_t) value_64bit;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data offset\t\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 lef_file_entry->data_offset,
			 lef_file_entry->data_offset );

			libcnotify_printf(
			 "%s: data size\t\t\t: %" PRIu64 "\n",
			 function,
			 lef_file_entry->size );

			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	if( libfvalue_split_utf8_string_free(
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &values,
		 NULL );
	}
	return( -1 );
}

/* Reads a single file extended attributes
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_read_extended_attributes(
     libewf_lef_file_entry_t *lef_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libewf_lef_extended_attribute_t *lef_extended_attribute = NULL;
	static char *function                                   = "libewf_lef_file_entry_read_extended_attributes";
	size_t data_offset                                      = 0;
	ssize_t read_count                                      = 0;
	int entry_index                                         = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 37 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: extended attributes data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( memory_compare(
	     libewf_lef_extended_attributes_header,
	     data,
	     37 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported extended attributes header.",
		 function );

		goto on_error;
	}
	while( data_offset < data_size )
	{
		if( libewf_lef_extended_attribute_initialize(
		     &lef_extended_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extended attribute.",
			 function );

			goto on_error;
		}
		read_count = libewf_lef_extended_attribute_read_data(
		              lef_extended_attribute,
		              &( data[ data_offset ] ),
		              data_size - data_offset,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read extended attribute.",
			 function );

			goto on_error;
		}
		data_offset += read_count;

		if( lef_extended_attribute->is_branch == 0 )
		{
			if( libcdata_array_append_entry(
			     lef_file_entry->extended_attributes,
			     &entry_index,
			     (intptr_t *) lef_extended_attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append entry to extended attributes array.",
				 function );

				goto on_error;
			}
			lef_extended_attribute = NULL;
		}
		else
		{
			if( libewf_lef_extended_attribute_free(
			     &lef_extended_attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free extended attribute.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( lef_extended_attribute != NULL )
	{
		libewf_lef_extended_attribute_free(
		 &lef_extended_attribute,
		 NULL );
	}
	return( -1 );
}

/* Reads a file entry short name
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_read_short_name(
     libewf_lef_file_entry_t *lef_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_lef_file_entry_read_short_name";
	size_t value_string_size              = 0;
	uint64_t short_name_size              = 0;
	int number_of_values                  = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     data,
	     data_size,
	     (uint8_t) ' ',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string into values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     values,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values",
		 function );

		goto on_error;
	}
	if( number_of_values != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     values,
	     0,
	     &value_string,
	     &value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string: 0.",
		 function );

		goto on_error;
	}
	if( libfvalue_utf8_string_copy_to_integer(
	     value_string,
	     value_string_size,
	     &short_name_size,
	     64,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set short name size.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     values,
	     1,
	     &value_string,
	     &value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string: 1.",
		 function );

		goto on_error;
	}
	if( ( short_name_size > (uint64_t) SSIZE_MAX )
	 || ( (size_t) short_name_size != value_string_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid short name size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( value_string != NULL )
	 && ( value_string_size > 0 ) )
	{
		if( libewf_serialized_string_read_data(
		     lef_file_entry->short_name,
		     value_string,
		     value_string_size - 1,
		     LIBEWF_VALUE_DATA_TYPE_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read short name string.",
			 function );

			goto on_error;
		}
	}
	if( libfvalue_split_utf8_string_free(
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &values,
		 NULL );
	}
	return( -1 );
}

/* Reads a file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_read_data(
     libewf_lef_file_entry_t *lef_file_entry,
     libfvalue_split_utf8_string_t *types,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libewf_value_reader_t *value_reader = NULL;
	uint8_t *byte_stream                = NULL;
	uint8_t *type_string                = NULL;
	uint8_t *utf8_string                = NULL;
	const uint8_t *value_data           = NULL;
	static char *function               = "libewf_lef_file_entry_read_data";
	size_t byte_stream_size             = 0;
	size_t type_string_size             = 0;
	size_t utf8_string_size             = 0;
	size_t value_data_size              = 0;
	uint64_t value_64bit_unsigned       = 0;
	int64_t value_64bit_signed          = 0;
	int known_value                     = 0;
	int number_of_types                 = 0;
	int result                          = 0;
	int value_index                     = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( lef_file_entry->name_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - name data value already set.",
		 function );

		return( -1 );
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     types,
	     &number_of_types,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of types",
		 function );

		goto on_error;
	}
	if( libewf_value_reader_initialize(
	     &value_reader,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value reader.",
		 function );

		goto on_error;
	}
	if( libewf_value_reader_set_buffer(
	     value_reader,
	     0,
	     data,
	     data_size,
	     LIBEWF_VALUE_DATA_TYPE_UTF16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set buffer in value reader.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_types;
	     value_index++ )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     types,
		     value_index,
		     &type_string,
		     &type_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve type string: %d.",
			 function,
			 value_index );

			goto on_error;
		}
		if( ( type_string == NULL )
		 || ( type_string_size < 2 )
		 || ( type_string[ 0 ] == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing type string: %d.",
			 function,
			 value_index );

			goto on_error;
		}
		known_value = 0;

		if( type_string_size == 4 )
		{
			if( ( type_string[ 0 ] == (uint8_t) 'c' )
			 && ( type_string[ 1 ] == (uint8_t) 'i' )
			 && ( type_string[ 2 ] == (uint8_t) 'd' ) )
			{
				result = libewf_value_reader_read_integer_unsigned(
				          value_reader,
				          &value_64bit_unsigned,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read record type value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIu64 "",
						 value_64bit_unsigned );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				if( result != 0 )
				{
					if( value_64bit_unsigned > (uint64_t) UINT32_MAX )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid record type value out of bounds.",
						 function );

						goto on_error;
					}
					lef_file_entry->record_type = (uint32_t) value_64bit_unsigned;
				}
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'm' )
			      && ( type_string[ 1 ] == (uint8_t) 'i' )
			      && ( type_string[ 2 ] == (uint8_t) 'd' ) )
			{
				result = libewf_value_reader_read_serialized_string_base16(
				          value_reader,
				          lef_file_entry->guid,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read GUID value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value:\n",
					 function,
					 (char *) type_string );
					libcnotify_print_data(
					 lef_file_entry->guid->data,
					 lef_file_entry->guid->data_size,
					 0 );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'o' )
			      && ( type_string[ 1 ] == (uint8_t) 'p' )
			      && ( type_string[ 2 ] == (uint8_t) 'r' ) )
			{
				result = libewf_value_reader_read_integer_unsigned(
				          value_reader,
				          &value_64bit_unsigned,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read entry flags value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIu64 "",
						 value_64bit_unsigned );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				if( result != 0 )
				{
					if( value_64bit_unsigned > (uint64_t) UINT32_MAX )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid entry flags value out of bounds.",
						 function );

						goto on_error;
					}
					lef_file_entry->flags = (uint32_t) value_64bit_unsigned;
				}
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'r' )
			      && ( type_string[ 2 ] == (uint8_t) 'c' ) )
			{
				result = libewf_value_reader_read_integer_unsigned(
				          value_reader,
				          &value_64bit_unsigned,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read source identifier value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIu64 "",
						 value_64bit_unsigned );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				if( result != 0 )
				{
					if( ( (int64_t) value_64bit_unsigned < (int64_t) 0 )
					 || ( (int64_t) value_64bit_unsigned > (int64_t) INT_MAX ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid source identifier value out of bounds.",
						 function );

						goto on_error;
					}
					lef_file_entry->source_identifier = (int) value_64bit_unsigned;
				}
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'u' )
			      && ( type_string[ 2 ] == (uint8_t) 'b' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &value_64bit_signed,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read subject identifier value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 value_64bit_signed );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				if( result != 0 )
				{
					if( value_64bit_signed > (int64_t) UINT32_MAX )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid subject identifier value out of bounds.",
						 function );

						goto on_error;
					}
					lef_file_entry->subject_identifier = (uint32_t) value_64bit_signed;
				}
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'h' )
			      && ( type_string[ 2 ] == (uint8_t) 'a' ) )
			{
				result = libewf_value_reader_read_serialized_string_base16(
				          value_reader,
				          lef_file_entry->sha1_hash,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read SHA1 hash value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value:\n",
					 function,
					 (char *) type_string );
					libcnotify_print_data(
					 lef_file_entry->sha1_hash->data,
					 lef_file_entry->sha1_hash->data_size,
					 0 );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'n' )
			      && ( type_string[ 2 ] == (uint8_t) 'h' ) )
			{
				if( libewf_value_reader_read_utf8_string(
				     value_reader,
				     &utf8_string,
				     &utf8_string_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read read short name value data.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value:\n",
					 function,
					 (char *) type_string );
					libcnotify_print_data(
					 utf8_string,
					 utf8_string_size,
					 0 );
				}
#endif
				if( ( utf8_string != NULL )
				 && ( utf8_string_size > 0 ) )
				{
/* Note that libfvalue_utf8_string_split expects the value string size including the end-of-string character */
					if( libewf_lef_file_entry_read_short_name(
					     lef_file_entry,
					     utf8_string,
					     utf8_string_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read short name.",
						 function );

						goto on_error;
					}
				}
				known_value = 1;
			}
		}
		else if( type_string_size == 3 )
		{
			/* Access time
			 */
			if( ( type_string[ 0 ] == (uint8_t) 'a' )
			 && ( type_string[ 1 ] == (uint8_t) 'c' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &( lef_file_entry->access_time ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read access time value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 lef_file_entry->access_time );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'a' )
			      && ( type_string[ 1 ] == (uint8_t) 'q' ) )
			{
/* TODO implement */
			}
			/* Data offset
			 * consist of: unknown, offset and size
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'b' )
			      && ( type_string[ 1 ] == (uint8_t) 'e' ) )
			{
				if( libewf_value_reader_read_utf8_string(
				     value_reader,
				     &utf8_string,
				     &utf8_string_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read read binary extents value data.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value:\n",
					 function,
					 (char *) type_string );
					libcnotify_print_data(
					 utf8_string,
					 utf8_string_size,
					 0 );
				}
#endif
				if( ( utf8_string != NULL )
				 && ( utf8_string_size > 0 ) )
				{
/* Note that libfvalue_utf8_string_split expects the value string size including the end-of-string character */
					if( libewf_lef_file_entry_read_binary_extents(
					     lef_file_entry,
					     utf8_string,
					     utf8_string_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read binary extents.",
						 function );

						goto on_error;
					}
				}
				known_value = 1;
			}
			/* Creation time
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'c' )
			      && ( type_string[ 1 ] == (uint8_t) 'r' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &( lef_file_entry->creation_time ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read creation time value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 lef_file_entry->creation_time );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'd' )
			      && ( type_string[ 1 ] == (uint8_t) 'l' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &( lef_file_entry->deletion_time ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read deletion time value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 lef_file_entry->deletion_time );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'd' )
			      && ( type_string[ 1 ] == (uint8_t) 'u' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &( lef_file_entry->duplicate_data_offset ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read duplicate data offset value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 lef_file_entry->duplicate_data_offset );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'e' )
			      && ( type_string[ 1 ] == (uint8_t) 'a' ) )
			{
				if( libewf_value_reader_read_byte_stream_base16(
				     value_reader,
				     &byte_stream,
				     &byte_stream_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read read extended attributes value data.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value:\n",
					 function,
					 (char *) type_string );
					libcnotify_print_data(
					 byte_stream,
					 byte_stream_size,
					 0 );
				}
#endif
				if( ( byte_stream != NULL )
				 && ( byte_stream_size > 0 ) )
				{
					if( libewf_lef_file_entry_read_extended_attributes(
					     lef_file_entry,
					     byte_stream,
					     byte_stream_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read extended attributes.",
						 function );

						goto on_error;
					}
				}
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'h' )
			      && ( type_string[ 1 ] == (uint8_t) 'a' ) )
			{
				result = libewf_value_reader_read_serialized_string_base16(
				          value_reader,
				          lef_file_entry->md5_hash,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read MD5 hash value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value:\n",
					 function,
					 (char *) type_string );
					libcnotify_print_data(
					 lef_file_entry->md5_hash->data,
					 lef_file_entry->md5_hash->data_size,
					 0 );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'i' )
			      && ( type_string[ 1 ] == (uint8_t) 'd' ) )
			{
				result = libewf_value_reader_read_integer_unsigned(
				          value_reader,
				          &( lef_file_entry->identifier ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read identifier value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIu64 "",
						 lef_file_entry->identifier );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'j' )
			      && ( type_string[ 1 ] == (uint8_t) 'q' ) )
			{
/* TODO implement */
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'l' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &( lef_file_entry->logical_offset ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read logical offset value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 lef_file_entry->logical_offset );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'l' )
			      && ( type_string[ 1 ] == (uint8_t) 's' ) )
			{
				result = libewf_value_reader_read_integer_unsigned(
				          value_reader,
				          (uint64_t *) &( lef_file_entry->size ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read size value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIu64 "",
						 lef_file_entry->size );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'm' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &( lef_file_entry->entry_modification_time ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read modification time value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 lef_file_entry->entry_modification_time );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'p' )
			      && ( type_string[ 1 ] == (uint8_t) 'm' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &value_64bit_signed,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read permission group index value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 value_64bit_signed );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				if( result != 0 )
				{
					if( ( value_64bit_signed < (int64_t) -1 )
					 || ( value_64bit_signed > (int64_t) INT_MAX ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid permission group index value out of bounds.",
						 function );

						goto on_error;
					}
					lef_file_entry->permission_group_index = (int) value_64bit_signed;
				}
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'p' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &( lef_file_entry->physical_offset ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read physical offset value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 lef_file_entry->physical_offset );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'w' )
			      && ( type_string[ 1 ] == (uint8_t) 'r' ) )
			{
				result = libewf_value_reader_read_integer_signed(
				          value_reader,
				          &( lef_file_entry->modification_time ),
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read modification time value.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value\t\t\t:",
					 function,
					 (char *) type_string );

					if( result != 0 )
					{
						libcnotify_printf(
						 " %" PRIi64 "",
						 lef_file_entry->modification_time );
					}
					libcnotify_printf(
					 "\n" );
				}
#endif
				known_value = 1;
			}
		}
		else if( type_string_size == 2 )
		{
			if( type_string[ 0 ] == (uint8_t) 'n' )
			{
				if( libewf_value_reader_read_data(
				     value_reader,
				     &value_data,
				     &value_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read read name value data.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value:\n",
					 function,
					 (char *) type_string );
					libcnotify_print_data(
					 value_data,
					 value_data_size,
					 0 );
				}
#endif
				if( ( value_data != NULL )
				 && ( value_data_size > 0 ) )
				{
					lef_file_entry->name_data = (uint8_t *) memory_allocate(
					                                         value_data_size );

					if( lef_file_entry->name_data == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create name data.",
						 function );

						goto on_error;
					}
					lef_file_entry->name_data_size = value_data_size;

					if( memory_copy(
					     lef_file_entry->name_data,
					     value_data,
					     value_data_size ) == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
						 "%s: unable to copy name data.",
						 function );

						goto on_error;
					}
				}
				known_value = 1;
			}
			else if( type_string[ 0 ] == (uint8_t) 'p' )
			{
				if( libewf_value_reader_read_data(
				     value_reader,
				     &value_data,
				     &value_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read read entry type value data.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type: %3s with value:\n",
					 function,
					 (char *) type_string );
					libcnotify_print_data(
					 value_data,
					 value_data_size,
					 0 );
				}
#endif
				/* p = 1 if directory
				 * p = empty if file
				 */
				if( value_data == NULL )
				{
					lef_file_entry->type = LIBEWF_FILE_ENTRY_TYPE_FILE;
				}
				else if( ( value_data_size == 2 )
				      && ( value_data[ 0 ] == (uint8_t) '1' )
				      && ( value_data[ 1 ] == 0 ) )
				{
					lef_file_entry->type = LIBEWF_FILE_ENTRY_TYPE_DIRECTORY;
				}
				known_value = 1;
			}
		}
		if( known_value == 0 )
		{
			if( libewf_value_reader_read_data(
			     value_reader,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read read %s value data.",
				 function,
				 (char *) type_string );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: type: %3s with value:\n",
				 function,
				 (char *) type_string );
				libcnotify_print_data(
				 value_data,
				 value_data_size,
				 0 );
			}
#endif
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( ( number_of_types != ( value_reader->value_index + 1 ) )
	 || ( value_reader->buffer_offset < value_reader->buffer_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in number of types and values.",
		 function );

		goto on_error;
	}
	if( libewf_value_reader_free(
	     &value_reader,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free value reader.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( lef_file_entry->name_data != NULL )
	{
		memory_free(
		 lef_file_entry->name_data );

		lef_file_entry->name_data = NULL;
	}
	lef_file_entry->name_data_size = 0;

	if( value_reader != NULL )
	{
		libewf_value_reader_free(
		 &value_reader,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_identifier(
     libewf_lef_file_entry_t *lef_file_entry,
     uint64_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_identifier";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = lef_file_entry->identifier;

	return( 1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_type(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_type";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = lef_file_entry->type;

	return( 1 );
}

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_flags(
     libewf_lef_file_entry_t *lef_file_entry,
     uint32_t *flags,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_flags";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid flags.",
		 function );

		return( -1 );
	}
	*flags = lef_file_entry->flags;

	return( 1 );
}

/* Retrieves the data offset
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_data_offset(
     libewf_lef_file_entry_t *lef_file_entry,
     off64_t *data_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_data_offset";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
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
	*data_offset = lef_file_entry->data_offset;

	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_data_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_data_size";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data_size = lef_file_entry->data_size;

	return( 1 );
}

/* Retrieves the logical offset
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_logical_offset(
     libewf_lef_file_entry_t *lef_file_entry,
     off64_t *logical_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_logical_offset";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( logical_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical offset.",
		 function );

		return( -1 );
	}
	*logical_offset = lef_file_entry->logical_offset;

	return( 1 );
}

/* Retrieves the physical offset
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_physical_offset(
     libewf_lef_file_entry_t *lef_file_entry,
     off64_t *physical_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_physical_offset";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( physical_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid physical offset.",
		 function );

		return( -1 );
	}
	*physical_offset = lef_file_entry->physical_offset;

	return( 1 );
}

/* Retrieves the duplicate data offset
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_duplicate_data_offset(
     libewf_lef_file_entry_t *lef_file_entry,
     off64_t *duplicate_data_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_duplicate_data_offset";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( duplicate_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid duplicate data offset.",
		 function );

		return( -1 );
	}
	*duplicate_data_offset = lef_file_entry->duplicate_data_offset;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded GUID
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf8_guid_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf8_guid_size";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_file_entry->guid,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve GUID UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded GUID value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf8_guid(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf8_guid";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_file_entry->guid,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded GUID
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf16_guid_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf16_guid_size";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_file_entry->guid,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve GUID UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded GUID value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf16_guid(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf16_guid";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_file_entry->guid,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf8_name_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function        = "libewf_lef_file_entry_get_utf8_name_size";
	size_t safe_utf8_string_size = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	if( ( lef_file_entry->name_data != NULL )
	 && ( lef_file_entry->name_data_size > 0 ) )
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     lef_file_entry->name_data,
		     lef_file_entry->name_data_size,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     &safe_utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	*utf8_string_size = safe_utf8_string_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf8_name(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf8_name";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     lef_file_entry->name_data,
	     lef_file_entry->name_data_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares an UTF-8 string with a file entry name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libewf_lef_file_entry_compare_name_with_utf8_string(
     libewf_lef_file_entry_t *lef_file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_compare_name_with_utf8_string";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libuna_utf8_string_compare_with_utf16_stream(
		  utf8_string,
		  utf8_string_length,
	          lef_file_entry->name_data,
	          lef_file_entry->name_data_size,
	          LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare name with UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf16_name_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function         = "libewf_lef_file_entry_get_utf16_name_size";
	size_t safe_utf16_string_size = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string size.",
		 function );

		return( -1 );
	}
	if( ( lef_file_entry->name_data != NULL )
	 && ( lef_file_entry->name_data_size > 0 ) )
	{
		if( libuna_utf16_string_size_from_utf16_stream(
		     lef_file_entry->name_data,
		     lef_file_entry->name_data_size,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     &safe_utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	*utf16_string_size = safe_utf16_string_size;

	return( 1 );
}

/* Retrieves the UTF-16 encoded name value
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf16_name(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf16_name";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     lef_file_entry->name_data,
	     lef_file_entry->name_data_size,
	     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares an UTF-16 string with a file entry name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libewf_lef_file_entry_compare_name_with_utf16_string(
     libewf_lef_file_entry_t *lef_file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_compare_name_with_utf16_string";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libuna_utf16_string_compare_with_utf16_stream(
		  utf16_string,
		  utf16_string_length,
	          lef_file_entry->name_data,
	          lef_file_entry->name_data_size,
	          LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare name with UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded short name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf8_short_name_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf8_short_name_size";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string_size(
	          lef_file_entry->short_name,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve short name UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded short name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf8_short_name(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf8_short_name";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_file_entry->short_name,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy short name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded short name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf16_short_name_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf16_short_name_size";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string_size(
	          lef_file_entry->short_name,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve short name UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded short name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_utf16_short_name(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf16_short_name";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_file_entry->short_name,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy short name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_size";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	*size = lef_file_entry->size;

	return( 1 );
}

/* Retrieves the source identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_source_identifier(
     libewf_lef_file_entry_t *lef_file_entry,
     int *source_identifier,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_source_identifier";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( source_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source identifier.",
		 function );

		return( -1 );
	}
	*source_identifier = lef_file_entry->source_identifier;

	return( 1 );
}

/* Retrieves the permission group index
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_permission_group_index(
     libewf_lef_file_entry_t *lef_file_entry,
     int *permission_group_index,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_permission_group_index";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( permission_group_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission group index.",
		 function );

		return( -1 );
	}
	*permission_group_index = lef_file_entry->permission_group_index;

	return( 1 );
}

/* Retrieves the record type
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_record_type(
     libewf_lef_file_entry_t *lef_file_entry,
     uint32_t *record_type,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_record_type";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( record_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record type.",
		 function );

		return( -1 );
	}
	*record_type = lef_file_entry->record_type;

	return( 1 );
}

/* Retrieves the creation date and time
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_file_entry_get_creation_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_creation_time";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = lef_file_entry->creation_time;

	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_file_entry_get_modification_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_modification_time";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = lef_file_entry->modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_file_entry_get_access_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_access_time";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = lef_file_entry->access_time;

	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_file_entry_get_entry_modification_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_entry_modification_time";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = lef_file_entry->entry_modification_time;

	return( 1 );
}

/* Retrieves the deletion date and time
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_file_entry_get_deletion_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_deletion_time";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = lef_file_entry->deletion_time;

	return( 1 );
}

/* Retrieves the UTF-8 encoded MD5 hash value
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_file_entry_get_utf8_hash_value_md5(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf8_hash_value_md5";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_file_entry->md5_hash,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy MD5 hash to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded MD5 hash value
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_file_entry_get_utf16_hash_value_md5(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf16_hash_value_md5";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_file_entry->md5_hash,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy MD5 hash to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded SHA1 hash value
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_file_entry_get_utf8_hash_value_sha1(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf8_hash_value_sha1";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf8_string(
	          lef_file_entry->sha1_hash,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy SHA1 hash to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded SHA1 hash value
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_lef_file_entry_get_utf16_hash_value_sha1(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_utf16_hash_value_sha1";
	int result            = 0;

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	result = libewf_serialized_string_get_utf16_string(
	          lef_file_entry->sha1_hash,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy SHA1 hash to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the number of extended attributes
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_number_of_extended_attributes(
     libewf_lef_file_entry_t *lef_file_entry,
     int *number_of_extended_attributes,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_number_of_extended_attributes";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     lef_file_entry->extended_attributes,
	     number_of_extended_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from extended attributes array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific extended attribute from the group
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_file_entry_get_extended_attribute_by_index(
     libewf_lef_file_entry_t *lef_file_entry,
     int extended_attribute_index,
     libewf_lef_extended_attribute_t **lef_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_file_entry_get_extended_attribute_by_index";

	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     lef_file_entry->extended_attributes,
	     extended_attribute_index,
	     (intptr_t **) lef_extended_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from extended attributes array.",
		 function,
		 extended_attribute_index );

		return( -1 );
	}
	return( 1 );
}

