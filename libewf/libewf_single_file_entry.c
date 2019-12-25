/*
 * Logical Evidence File (LEF) file entry functions
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <types.h>

#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_lef_extended_attribute.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfguid.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_single_file_entry.h"

/* Creates a single file entry
 * Make sure the value single_file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_initialize(
     libewf_single_file_entry_t **single_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_initialize";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( *single_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single file entry value already set.",
		 function );

		return( -1 );
	}
	*single_file_entry = memory_allocate_structure(
	                      libewf_single_file_entry_t );

	if( *single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create single file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *single_file_entry,
	     0,
	     sizeof( libewf_single_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear single file entry.",
		 function );

		goto on_error;
	}
	( *single_file_entry )->data_offset           = -1;
	( *single_file_entry )->duplicate_data_offset = -1;

	return( 1 );

on_error:
	if( *single_file_entry != NULL )
	{
		memory_free(
		 *single_file_entry );

		*single_file_entry = NULL;
	}
	return( -1 );
}

/* Frees a single file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_free(
     libewf_single_file_entry_t **single_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_free";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( *single_file_entry != NULL )
	{
		if( ( *single_file_entry )->name != NULL )
		{
			memory_free(
			 ( *single_file_entry )->name );
		}
		if( ( *single_file_entry )->md5_hash != NULL )
		{
			memory_free(
			 ( *single_file_entry )->md5_hash );
		}
		if( ( *single_file_entry )->sha1_hash != NULL )
		{
			memory_free(
			 ( *single_file_entry )->sha1_hash );
		}
		memory_free(
		 *single_file_entry );

		*single_file_entry = NULL;
	}
	return( 1 );
}

/* Clones the single file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_clone(
     libewf_single_file_entry_t **destination_single_file_entry,
     libewf_single_file_entry_t *source_single_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_clone";

	if( destination_single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination single file entry.",
		 function );

		return( -1 );
	}
	if( *destination_single_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination single file entry value already set.",
		 function );

		return( -1 );
	}
	if( source_single_file_entry == NULL )
	{
		*destination_single_file_entry = NULL;

		return( 1 );
	}
	*destination_single_file_entry = memory_allocate_structure(
			                  libewf_single_file_entry_t );

	if( *destination_single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination single file entry.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_single_file_entry,
	     source_single_file_entry,
	     sizeof( libewf_single_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination single file entry.",
		 function );

		memory_free(
		 *destination_single_file_entry );

		*destination_single_file_entry = NULL;

		return( -1 );
	}
	( *destination_single_file_entry )->name      = NULL;
	( *destination_single_file_entry )->md5_hash  = NULL;
	( *destination_single_file_entry )->sha1_hash = NULL;

	if( source_single_file_entry->name != NULL )
	{
		( *destination_single_file_entry )->name = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * source_single_file_entry->name_size );

		if( ( *destination_single_file_entry )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_entry )->name,
		     source_single_file_entry->name,
		     source_single_file_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination name.",
			 function );

			goto on_error;
		}
		( *destination_single_file_entry )->name_size = source_single_file_entry->name_size;
	}
	if( source_single_file_entry->md5_hash != NULL )
	{
		( *destination_single_file_entry )->md5_hash = (uint8_t *) memory_allocate(
		                                                            sizeof( uint8_t ) * source_single_file_entry->md5_hash_size );

		if( ( *destination_single_file_entry )->md5_hash == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination MD5 hash.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_entry )->md5_hash,
		     source_single_file_entry->md5_hash,
		     source_single_file_entry->md5_hash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination MD5 hash.",
			 function );

			goto on_error;
		}
		( *destination_single_file_entry )->md5_hash_size = source_single_file_entry->md5_hash_size;
	}
	if( source_single_file_entry->sha1_hash != NULL )
	{
		( *destination_single_file_entry )->sha1_hash = (uint8_t *) memory_allocate(
		                                                             sizeof( uint8_t ) * source_single_file_entry->sha1_hash_size );

		if( ( *destination_single_file_entry )->sha1_hash == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination SHA1 hash.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_entry )->sha1_hash,
		     source_single_file_entry->sha1_hash,
		     source_single_file_entry->sha1_hash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination SHA1 hash.",
			 function );

			goto on_error;
		}
		( *destination_single_file_entry )->sha1_hash_size = source_single_file_entry->sha1_hash_size;
	}
	return( 1 );

on_error:
	if( *destination_single_file_entry != NULL )
	{
		if( ( *destination_single_file_entry )->sha1_hash != NULL )
		{
			memory_free(
			 ( *destination_single_file_entry )->sha1_hash );
		}
		if( ( *destination_single_file_entry )->md5_hash != NULL )
		{
			memory_free(
			 ( *destination_single_file_entry )->md5_hash );
		}
		if( ( *destination_single_file_entry )->name != NULL )
		{
			memory_free(
			 ( *destination_single_file_entry )->name );
		}
		memory_free(
		 *destination_single_file_entry );

		*destination_single_file_entry = NULL;
	}
	return( -1 );
}

/* Reads a single file binary extents
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_read_binary_extents(
     libewf_single_file_entry_t *single_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *offset_values  = NULL;
	uint8_t *offset_value_string                  = NULL;
	static char *function                         = "libewf_single_file_entry_read_binary_extents";
	size_t offset_value_string_size               = 0;
	uint64_t value_64bit                          = 0;
	int number_of_offset_values                   = 0;

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     data,
	     data_size,
	     (uint8_t) ' ',
	     &offset_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string into offset values.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     offset_values,
	     &number_of_offset_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offset values",
		 function );

		goto on_error;
	}
	if( ( number_of_offset_values != 1 )
	 && ( number_of_offset_values != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of offset values.",
		 function );

		goto on_error;
	}
	if( number_of_offset_values == 3 )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     offset_values,
		     1,
		     &offset_value_string,
		     &offset_value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset value string: 1.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_copy_to_integer(
		     offset_value_string,
		     offset_value_string_size,
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
		single_file_entry->data_offset = (off64_t) value_64bit;

		if( libfvalue_split_utf8_string_get_segment_by_index(
		     offset_values,
		     2,
		     &offset_value_string,
		     &offset_value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset value string: 2.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_copy_to_integer(
		     offset_value_string,
		     offset_value_string_size,
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
		single_file_entry->data_size = (size64_t) value_64bit;
	}
	if( libfvalue_split_utf8_string_free(
	     &offset_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split offset values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( offset_values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &offset_values,
		 NULL );
	}
	return( -1 );
}

/* Reads a single file extended attributes
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_read_extended_attributes(
     libewf_single_file_entry_t *single_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libewf_lef_extended_attribute_t *extended_attribute = NULL;
	uint8_t *byte_stream                                = NULL;
	static char *function                               = "libewf_single_file_entry_read_extended_attributes";
	size_t byte_stream_offset                           = 0;
	size_t byte_stream_size                             = 0;
	ssize_t read_count                                  = 0;

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( data_size >= 1 )
	 && ( data[ data_size - 1 ] == 0 ) )
	{
		data_size--;
	}
	if( libuna_base16_stream_size_to_byte_stream(
	     data,
	     data_size,
	     &byte_stream_size,
	     LIBUNA_BASE16_VARIANT_RFC4648,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine byte stream size of base16 encoded data.",
		 function );

		goto on_error;
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid byte stream size value out of bounds.",
		 function );

		goto on_error;
	}
	byte_stream = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * byte_stream_size );

	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create byte stream.",
		 function );

		goto on_error;
	}
	if( libuna_base16_stream_copy_to_byte_stream(
	     data,
	     data_size,
	     byte_stream,
	     byte_stream_size,
	     LIBUNA_BASE16_VARIANT_RFC4648,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to copy base16 encoded data to byte stream.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: extended attributes data:\n",
		 function );
		libcnotify_print_data(
		 byte_stream,
		 byte_stream_size,
		 0 );
	}
#endif
	while( byte_stream_offset < byte_stream_size )
	{
		if( libewf_lef_extended_attribute_initialize(
		     &extended_attribute,
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
		              extended_attribute,
		              &( byte_stream[ byte_stream_offset ] ),
		              byte_stream_size - byte_stream_offset,
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
		byte_stream_offset += read_count;
/* TODO implement */

		if( libewf_lef_extended_attribute_free(
		     &extended_attribute,
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
	memory_free(
	 byte_stream );

	return( 1 );

on_error:
	if( extended_attribute != NULL )
	{
		libewf_lef_extended_attribute_free(
		 &extended_attribute,
		 NULL );
	}
	if( byte_stream != NULL )
	{
		memory_free(
		 byte_stream );
	}
	return( -1 );
}

/* Reads a single file entry hexadecimal string
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_read_hexadecimal_string(
     libewf_single_file_entry_t *single_file_entry,
     const uint8_t *data,
     size_t data_size,
     uint8_t *string,
     size_t string_size,
     int *zero_values_only,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_read_hexadecimal_string";
	size_t data_offset    = 0;
	size_t string_index   = 0;

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( zero_values_only == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid zero values only.",
		 function );

		return( -1 );
	}
	*zero_values_only = 1;

	for( data_offset = 0;
	     data_offset < data_size - 1;
	     data_offset++ )
	{
		if( string_index >= string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid string value too small.",
			 function );

			return( -1 );
		}
		if( data[ data_offset ] != (uint8_t) '0' )
		{
			*zero_values_only = 0;
		}
		if( ( data[ data_offset ] >= (uint8_t) '0' )
		 && ( data[ data_offset ] <= (uint8_t) '9' ) )
		{
			string[ string_index ] = data[ data_offset ];
		}
		else if( ( data[ data_offset ] >= (uint8_t) 'A' )
		      && ( data[ data_offset ] <= (uint8_t) 'F' ) )
		{
			string[ string_index ] = (uint8_t) ( 'a' - 'A' ) + data[ data_offset ];
		}
		else if( ( data[ data_offset ] >= (uint8_t) 'a' )
		      && ( data[ data_offset ] <= (uint8_t) 'f' ) )
		{
			string[ string_index ] = data[ data_offset ];
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character in hexadecimal string.",
			 function );

			return( -1 );
		}
		string_index++;
	}
	if( string_index >= string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid string value too small.",
		 function );

		return( -1 );
	}
	string[ string_index ] = 0;

	return( 1 );
}

/* Reads a single file entry short name
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_read_short_name(
     libewf_single_file_entry_t *single_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_read_short_name";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
/* TODO implement */
	return( 1 );
}

/* Reads a single file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_read_data(
     libewf_single_file_entry_t *single_file_entry,
     libfvalue_split_utf8_string_t *types,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *type_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_single_file_entry_read_data";
	size_t type_string_size               = 0;
	size_t value_string_index             = 0;
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int number_of_types                   = 0;
	int number_of_values                  = 0;
	int value_index                       = 0;
	int zero_values_only                  = 0;

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( single_file_entry->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single file entry - name value already set.",
		 function );

		return( -1 );
	}
	if( single_file_entry->md5_hash != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single file entry - MD5 hash value already set.",
		 function );

		return( -1 );
	}
	if( single_file_entry->sha1_hash != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single file entry - SHA1 hash value already set.",
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
	if( libfvalue_utf8_string_split(
	     data,
	     data_size,
	     (uint8_t) '\t',
	     &values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split data into string values.",
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
	if( number_of_types != number_of_values )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in number of types and values.",
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
		/* Remove trailing carriage return
		 */
		else if( type_string[ type_string_size - 2 ] == (uint8_t) '\r' )
		{
			type_string[ type_string_size - 2 ] = 0;

			type_string_size -= 1;
		}
		if( value_index < number_of_values )
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
			     values,
			     value_index,
			     &value_string,
			     &value_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( ( value_string == NULL )
			 || ( value_string_size < 2 )
			 || ( value_string[ 0 ] == 0 ) )
			{
				value_string      = NULL;
				value_string_size = 0;
			}
			/* Remove trailing carriage return
			 */
			else if( value_string[ value_string_size - 2 ] == (uint8_t) '\r' )
			{
				value_string[ value_string_size - 2 ] = 0;

				value_string_size -= 1;
			}
		}
		else
		{
			value_string      = NULL;
			value_string_size = 0;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: type: %s with value: %s\n",
			 function,
			 (char *) type_string,
			 (char *) value_string );
		}
#endif
		if( value_string == NULL )
		{
			/* Ignore empty values
			 */
		}
		else if( type_string_size == 4 )
		{
			if( ( type_string[ 0 ] == (uint8_t) 'c' )
			 && ( type_string[ 1 ] == (uint8_t) 'i' )
			 && ( type_string[ 2 ] == (uint8_t) 'd' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set record type.",
					 function );

					goto on_error;
				}
				if( value_64bit > (uint64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid record type value out of bounds.",
					 function );

					goto on_error;
				}
				single_file_entry->record_type = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'm' )
			      && ( type_string[ 1 ] == (uint8_t) 'i' )
			      && ( type_string[ 2 ] == (uint8_t) 'd' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'o' )
			      && ( type_string[ 1 ] == (uint8_t) 'p' )
			      && ( type_string[ 2 ] == (uint8_t) 'r' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set entry flags.",
					 function );

					goto on_error;
				}
				if( value_64bit > (uint64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid entry flags value out of bounds.",
					 function );

					goto on_error;
				}
				single_file_entry->flags = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'r' )
			      && ( type_string[ 2 ] == (uint8_t) 'c' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set source identifier.",
					 function );

					goto on_error;
				}
				if( value_64bit > (uint64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid source identifier value out of bounds.",
					 function );

					goto on_error;
				}
				single_file_entry->source_identifier = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'u' )
			      && ( type_string[ 2 ] == (uint8_t) 'b' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set subject identifier.",
					 function );

					goto on_error;
				}
				if( (int64_t) value_64bit > (int64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid subject identifier value out of bounds.",
					 function );

					goto on_error;
				}
				single_file_entry->subject_identifier = (uint32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'h' )
			      && ( type_string[ 2 ] == (uint8_t) 'a' ) )
			{
				single_file_entry->sha1_hash = (uint8_t *) memory_allocate(
				                                            sizeof( uint8_t ) * value_string_size );

				if( single_file_entry->sha1_hash == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create SHA1 hash.",
					 function );

					goto on_error;
				}
				if( libewf_single_file_entry_read_hexadecimal_string(
				     single_file_entry,
				     value_string,
				     value_string_size,
				     single_file_entry->sha1_hash,
				     value_string_size,
				     &zero_values_only,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read SHA1 hash.",
					 function );

					goto on_error;
				}
				if( zero_values_only == 0 )
				{
					single_file_entry->sha1_hash_size = value_string_size;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 's' )
			      && ( type_string[ 1 ] == (uint8_t) 'n' )
			      && ( type_string[ 2 ] == (uint8_t) 'h' ) )
			{
				if( libewf_single_file_entry_read_short_name(
				     single_file_entry,
				     value_string,
				     value_string_size,
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
		}
		else if( type_string_size == 3 )
		{
			/* Access time
			 */
			if( ( type_string[ 0 ] == (uint8_t) 'a' )
			 && ( type_string[ 1 ] == (uint8_t) 'c' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     32,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set access time.",
					 function );

					goto on_error;
				}
				single_file_entry->access_time = (int32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'a' )
			      && ( type_string[ 1 ] == (uint8_t) 'q' ) )
			{
			}
			/* Data offset
			 * consist of: unknown, offset and size
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'b' )
			      && ( type_string[ 1 ] == (uint8_t) 'e' ) )
			{
				if( libewf_single_file_entry_read_binary_extents(
				     single_file_entry,
				     value_string,
				     value_string_size,
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
			/* Creation time
			 */
			else if( ( type_string[ 0 ] == (uint8_t) 'c' )
			      && ( type_string[ 1 ] == (uint8_t) 'r' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     32,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set creation time.",
					 function );

					goto on_error;
				}
				single_file_entry->creation_time = (int32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'd' )
			      && ( type_string[ 1 ] == (uint8_t) 'l' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     32,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set deletion time.",
					 function );

					goto on_error;
				}
				single_file_entry->deletion_time = (int32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'd' )
			      && ( type_string[ 1 ] == (uint8_t) 'u' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set duplicate data offset.",
					 function );

					goto on_error;
				}
				single_file_entry->duplicate_data_offset = (off64_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'e' )
			      && ( type_string[ 1 ] == (uint8_t) 'a' ) )
			{
				if( libewf_single_file_entry_read_extended_attributes(
				     single_file_entry,
				     value_string,
				     value_string_size,
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
			else if( ( type_string[ 0 ] == (uint8_t) 'h' )
			      && ( type_string[ 1 ] == (uint8_t) 'a' ) )
			{
				single_file_entry->md5_hash = (uint8_t *) memory_allocate(
				                                           sizeof( uint8_t ) * value_string_size );

				if( single_file_entry->md5_hash == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create MD5 hash.",
					 function );

					goto on_error;
				}
				if( libewf_single_file_entry_read_hexadecimal_string(
				     single_file_entry,
				     value_string,
				     value_string_size,
				     single_file_entry->md5_hash,
				     value_string_size,
				     &zero_values_only,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read MD5 hash.",
					 function );

					goto on_error;
				}
				if( zero_values_only == 0 )
				{
					single_file_entry->md5_hash_size = value_string_size;
				}
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'i' )
			      && ( type_string[ 1 ] == (uint8_t) 'd' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'j' )
			      && ( type_string[ 1 ] == (uint8_t) 'q' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'l' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'l' )
			      && ( type_string[ 1 ] == (uint8_t) 's' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set size.",
					 function );

					goto on_error;
				}
				single_file_entry->size = (size64_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'm' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     32,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set entry modification time.",
					 function );

					goto on_error;
				}
				single_file_entry->entry_modification_time = (int32_t) value_64bit;
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'p' )
			      && ( type_string[ 1 ] == (uint8_t) 'm' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set permissions identifier.",
					 function );

					goto on_error;
				}
/* TODO fix check
				if( ( (int64_t) value_64bit < (int64_t) -1 )
				 || ( (int64_t) value_64bit > (int64_t) UINT32_MAX ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid permissions identifier value out of bounds.",
					 function );

					goto on_error;
				}
				if( (int64_t) value_64bit >= 0 )
				{
					single_file_entry->permissions_identifier = (uint32_t) value_64bit;
				}
*/
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'p' )
			      && ( type_string[ 1 ] == (uint8_t) 'o' ) )
			{
			}
			else if( ( type_string[ 0 ] == (uint8_t) 'w' )
			      && ( type_string[ 1 ] == (uint8_t) 'r' ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     value_string,
				     value_string_size,
				     &value_64bit,
				     32,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set modification time.",
					 function );

					goto on_error;
				}
				single_file_entry->modification_time = (int32_t) value_64bit;
			}
		}
		else if( type_string_size == 2 )
		{
			if( type_string[ 0 ] == (uint8_t) 'n' )
			{
				single_file_entry->name = (uint8_t *) memory_allocate(
								       sizeof( uint8_t ) * value_string_size );

				if( single_file_entry->name == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create name.",
					 function );

					goto on_error;
				}
				if( narrow_string_copy(
				     single_file_entry->name,
				     value_string,
				     value_string_size - 1 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set name.",
					 function );

					goto on_error;
				}
				single_file_entry->name[ value_string_size - 1 ] = 0;

				single_file_entry->name_size = value_string_size;
			}
		}
		if( type_string_size == 2 )
		{
			if( type_string[ 0 ] == (uint8_t) 'p' )
			{
				/* p = 1 if directory
				 * p = empty if file
				 */
				if( value_string == NULL )
				{
					single_file_entry->type = LIBEWF_FILE_ENTRY_TYPE_FILE;
				}
				else if( ( value_string_size == 2 )
				      && ( value_string[ 0 ] == (uint8_t) '1' ) )
				{
					single_file_entry->type = LIBEWF_FILE_ENTRY_TYPE_DIRECTORY;
				}
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
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

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_type(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_type";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*type = single_file_entry->type;

	return( 1 );
}

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_flags(
     libewf_single_file_entry_t *single_file_entry,
     uint32_t *flags,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_flags";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*flags = single_file_entry->flags;

	return( 1 );
}

/* Retrieves the data offset
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_data_offset(
     libewf_single_file_entry_t *single_file_entry,
     off64_t *data_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_data_offset";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*data_offset = single_file_entry->data_offset;

	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_data_size(
     libewf_single_file_entry_t *single_file_entry,
     size64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_data_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*data_size = single_file_entry->data_size;

	return( 1 );
}

/* Retrieves the duplicate data offset
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_duplicate_data_offset(
     libewf_single_file_entry_t *single_file_entry,
     off64_t *duplicate_data_offset,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_duplicate_data_offset";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*duplicate_data_offset = single_file_entry->duplicate_data_offset;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf8_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_name_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*utf8_string_size = single_file_entry->name_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf8_name(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_name";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( ( utf8_string_size == 0 )
	 || ( utf8_string_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( single_file_entry->name == NULL )
	 || ( single_file_entry->name_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_entry->name_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid UTF-8 string size value too small.",
			 function );

			return( -1 );
		}
		if( narrow_string_copy(
		     (char *) utf8_string,
		     (char *) single_file_entry->name,
		     single_file_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_entry->name_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf16_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_name_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( ( single_file_entry->name == NULL )
	 || ( single_file_entry->name_size == 0 ) )
	{
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
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf8(
		     single_file_entry->name,
		     single_file_entry->name_size,
		     utf16_string_size,
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
	return( 1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf16_name(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_name";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( ( utf16_string_size == 0 )
	 || ( utf16_string_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 string size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( single_file_entry->name == NULL )
	 || ( single_file_entry->name_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_entry->name,
		     single_file_entry->name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy name to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded short name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf8_short_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_short_name_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*utf8_string_size = single_file_entry->short_name_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded short name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf8_short_name(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_short_name";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( ( utf8_string_size == 0 )
	 || ( utf8_string_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( single_file_entry->short_name == NULL )
	 || ( single_file_entry->short_name_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_entry->short_name_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid UTF-8 string size value too small.",
			 function );

			return( -1 );
		}
		if( narrow_string_copy(
		     (char *) utf8_string,
		     (char *) single_file_entry->short_name,
		     single_file_entry->short_name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_entry->short_name_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded short name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf16_short_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_short_name_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( ( single_file_entry->short_name == NULL )
	 || ( single_file_entry->short_name_size == 0 ) )
	{
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
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf8(
		     single_file_entry->short_name,
		     single_file_entry->short_name_size,
		     utf16_string_size,
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
	return( 1 );
}

/* Retrieves the UTF-16 encoded short name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_utf16_short_name(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_short_name";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( ( utf16_string_size == 0 )
	 || ( utf16_string_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 string size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( single_file_entry->short_name == NULL )
	 || ( single_file_entry->short_name_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_entry->short_name,
		     single_file_entry->short_name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy short name to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_size(
     libewf_single_file_entry_t *single_file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_size";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*size = single_file_entry->size;

	return( 1 );
}

/* Retrieves the record type
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_record_type(
     libewf_single_file_entry_t *single_file_entry,
     uint32_t *record_type,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_record_type";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
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
	*record_type = single_file_entry->record_type;

	return( 1 );
}

/* Retrieves the creation date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_creation_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *creation_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_creation_time";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( creation_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid creation time.",
		 function );

		return( -1 );
	}
	*creation_time = single_file_entry->creation_time;

	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_modification_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *modification_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_modification_time";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( modification_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid modification time.",
		 function );

		return( -1 );
	}
	*modification_time = single_file_entry->modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_access_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *access_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_access_time";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( access_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid access time.",
		 function );

		return( -1 );
	}
	*access_time = single_file_entry->access_time;

	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_entry_modification_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *entry_modification_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_entry_modification_time";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( entry_modification_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry modification time.",
		 function );

		return( -1 );
	}
	*entry_modification_time = single_file_entry->entry_modification_time;

	return( 1 );
}

/* Retrieves the deletion date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_get_deletion_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *deletion_time,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_deletion_time";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( deletion_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid deletion time.",
		 function );

		return( -1 );
	}
	*deletion_time = single_file_entry->deletion_time;

	return( 1 );
}

/* Retrieves the UTF-8 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_entry_get_utf8_hash_value_md5(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_hash_value_md5";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < single_file_entry->md5_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_entry->md5_hash_size == 0 )
	{
		return( 0 );
	}
	if( memory_copy(
	     utf8_string,
	     single_file_entry->md5_hash,
	     single_file_entry->md5_hash_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy MD5 hash to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_entry_get_utf16_hash_value_md5(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_hash_value_md5";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size < single_file_entry->md5_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_entry->md5_hash_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8(
	     utf16_string,
	     utf16_string_size,
	     single_file_entry->md5_hash,
	     single_file_entry->md5_hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy MD5 hash to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded SHA1 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_entry_get_utf8_hash_value_sha1(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf8_hash_value_sha1";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < single_file_entry->sha1_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_entry->sha1_hash_size == 0 )
	{
		return( 0 );
	}
	if( memory_copy(
	     utf8_string,
	     single_file_entry->sha1_hash,
	     single_file_entry->sha1_hash_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy SHA1 hash to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded SHA1 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_single_file_entry_get_utf16_hash_value_sha1(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_entry_get_utf16_hash_value_sha1";

	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size < single_file_entry->sha1_hash_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( single_file_entry->sha1_hash_size == 0 )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8(
	     utf16_string,
	     utf16_string_size,
	     single_file_entry->sha1_hash,
	     single_file_entry->sha1_hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy SHA1 hash to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

