/*
 * Serialized (file) object functions
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
#include <memory.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libuna.h"
#include "libewf_serialized_string.h"

/* Creates a serialized string
 * Make sure the value serialized_string is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_serialized_string_initialize(
     libewf_serialized_string_t **serialized_string,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_initialize";

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
		 function );

		return( -1 );
	}
	if( *serialized_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid serialized string value already set.",
		 function );

		return( -1 );
	}
	*serialized_string = memory_allocate_structure(
	                      libewf_serialized_string_t );

	if( *serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create serialized string.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *serialized_string,
	     0,
	     sizeof( libewf_serialized_string_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear serialized string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *serialized_string != NULL )
	{
		memory_free(
		 *serialized_string );

		*serialized_string = NULL;
	}
	return( -1 );
}

/* Frees a serialized string
 * Returns 1 if successful or -1 on error
 */
int libewf_serialized_string_free(
     libewf_serialized_string_t **serialized_string,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_free";

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
		 function );

		return( -1 );
	}
	if( *serialized_string != NULL )
	{
		if( ( *serialized_string )->data != NULL )
		{
			memory_free(
			 ( *serialized_string )->data );
		}
		memory_free(
		 *serialized_string );

		*serialized_string = NULL;
	}
	return( 1 );
}

/* Clones the serialized string
 * Returns 1 if successful or -1 on error
 */
int libewf_serialized_string_clone(
     libewf_serialized_string_t **destination_serialized_string,
     libewf_serialized_string_t *source_serialized_string,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_clone";

	if( destination_serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination serialized string.",
		 function );

		return( -1 );
	}
	if( *destination_serialized_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination serialized string value already set.",
		 function );

		return( -1 );
	}
	if( source_serialized_string == NULL )
	{
		*destination_serialized_string = NULL;

		return( 1 );
	}
	*destination_serialized_string = memory_allocate_structure(
	                                  libewf_serialized_string_t );

	if( *destination_serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination serialized string.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_serialized_string,
	     source_serialized_string,
	     sizeof( libewf_serialized_string_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination serialized string.",
		 function );

		memory_free(
		 *destination_serialized_string );

		*destination_serialized_string = NULL;

		return( -1 );
	}
	( *destination_serialized_string )->data = NULL;

	if( source_serialized_string->data != NULL )
	{
		( *destination_serialized_string )->data = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * source_serialized_string->data_size );

		if( ( *destination_serialized_string )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_serialized_string )->data,
		     source_serialized_string->data,
		     source_serialized_string->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination data.",
			 function );

			goto on_error;
		}
		( *destination_serialized_string )->data_size = source_serialized_string->data_size;
	}
	return( 1 );

on_error:
	if( *destination_serialized_string != NULL )
	{
		if( ( *destination_serialized_string )->data != NULL )
		{
			memory_free(
			 ( *destination_serialized_string )->data );
		}
		memory_free(
		 *destination_serialized_string );

		*destination_serialized_string = NULL;
	}
	return( -1 );
}

/* Reads a serialized string
 * Returns 1 if successful or -1 on error
 */
int libewf_serialized_string_read_data(
     libewf_serialized_string_t *serialized_string,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_read_data";

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
		 function );

		return( -1 );
	}
	if( serialized_string->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid serialized string - data value already set.",
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
	if( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( data_size >= 1 )
	 && ( data[ data_size - 1 ] == 0 ) )
	{
		data_size -= 1;
	}
	serialized_string->data = (uint8_t *) memory_allocate(
	                                       sizeof( uint8_t ) * ( data_size + 1 ) );

	if( serialized_string->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	serialized_string->data_size = data_size + 1;

	if( memory_copy(
	     serialized_string->data,
	     data,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data to string.",
		 function );

		goto on_error;
	}
	serialized_string->data[ data_size ] = 0;

	return( 1 );

on_error:
	if( serialized_string->data != NULL )
	{
		memory_free(
		 serialized_string->data );

		serialized_string->data = NULL;
	}
	serialized_string->data_size = 0;

	return( -1 );
}

/* Reads a hexadecimal serialized string
 * Returns 1 if successful or -1 on error
 */
int libewf_serialized_string_read_hexadecimal_data(
     libewf_serialized_string_t *serialized_string,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_read_hexadecimal_data";
	size_t data_offset    = 0;
	int zero_values_only  = 0;

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
		 function );

		return( -1 );
	}
	if( serialized_string->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid serialized string - data value already set.",
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
	if( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( data_size >= 1 )
	 && ( data[ data_size - 1 ] == 0 ) )
	{
		data_size -= 1;
	}
	serialized_string->data = (uint8_t *) memory_allocate(
	                                       sizeof( uint8_t ) * ( data_size + 1 ) );

	if( serialized_string->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	serialized_string->data_size = data_size + 1;

	zero_values_only = 1;

	for( data_offset = 0;
	     data_offset < data_size;
	     data_offset++ )
	{
		if( data[ data_offset ] != (uint8_t) '0' )
		{
			zero_values_only = 0;
		}
		if( ( data[ data_offset ] >= (uint8_t) '0' )
		 && ( data[ data_offset ] <= (uint8_t) '9' ) )
		{
			serialized_string->data[ data_offset ] = data[ data_offset ];
		}
		else if( ( data[ data_offset ] >= (uint8_t) 'A' )
		      && ( data[ data_offset ] <= (uint8_t) 'F' ) )
		{
			serialized_string->data[ data_offset ] = (uint8_t) ( 'a' - 'A' ) + data[ data_offset ];
		}
		else if( ( data[ data_offset ] >= (uint8_t) 'a' )
		      && ( data[ data_offset ] <= (uint8_t) 'f' ) )
		{
			serialized_string->data[ data_offset ] = data[ data_offset ];
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character in hexadecimal string.",
			 function );

			goto on_error;
		}
	}
	serialized_string->data[ data_offset ] = 0;

	if( zero_values_only != 0 )
	{
		memory_free(
		 serialized_string->data );

		serialized_string->data      = NULL;
		serialized_string->data_size = 0;
	}
	return( 1 );

on_error:
	if( serialized_string->data != NULL )
	{
		memory_free(
		 serialized_string->data );

		serialized_string->data = NULL;
	}
	serialized_string->data_size = 0;

	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if data is not set or -1 on error
 */
int libewf_serialized_string_get_utf8_string_size(
     libewf_serialized_string_t *serialized_string,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_get_utf8_string_size";

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
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
	if( ( serialized_string->data == NULL )
	 || ( serialized_string->data_size == 0 ) )
	{
		*utf8_string_size = 0;

		return( 0 );
	}
	*utf8_string_size = serialized_string->data_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded string value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if data is not set or -1 on error
 */
int libewf_serialized_string_get_utf8_string(
     libewf_serialized_string_t *serialized_string,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_get_utf8_string";

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
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
	if( ( serialized_string->data == NULL )
	 || ( serialized_string->data_size == 0 ) )
	{
		if( utf8_string_size < 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid UTF-8 string size value too small.",
			 function );

			return( -1 );
		}
		utf8_string[ 0 ] = 0;

		return( 0 );
	}
	if( utf8_string_size < serialized_string->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-8 string size value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     utf8_string,
	     serialized_string->data,
	     serialized_string->data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if data is not set or -1 on error
 */
int libewf_serialized_string_get_utf16_string_size(
     libewf_serialized_string_t *serialized_string,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_get_utf16_string_size";

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
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
	if( ( serialized_string->data == NULL )
	 || ( serialized_string->data_size == 0 ) )
	{
		*utf16_string_size = 0;

		return( 0 );
	}
	if( libuna_utf16_string_size_from_utf8(
	     serialized_string->data,
	     serialized_string->data_size,
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
	return( 1 );
}

/* Retrieves the UTF-16 encoded string value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if data is not set or -1 on error
 */
int libewf_serialized_string_get_utf16_string(
     libewf_serialized_string_t *serialized_string,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_get_utf16_string";

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
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
	if( ( serialized_string->data == NULL )
	 || ( serialized_string->data_size == 0 ) )
	{
		if( utf16_string_size < 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid UTF-16 string size value too small.",
			 function );

			return( -1 );
		}
		utf16_string[ 0 ] = 0;

		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8(
	     utf16_string,
	     utf16_string_size,
	     serialized_string->data,
	     serialized_string->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares an UTF-8 string with a serialized string
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libewf_serialized_string_compare_with_utf8_string(
     libewf_serialized_string_t *serialized_string,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_compare_with_utf8_string";
	int result            = 0;

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
		 function );

		return( -1 );
	}
	if( ( serialized_string->data == NULL )
	 || ( serialized_string->data_size == 0 ) )
	{
		result = LIBUNA_COMPARE_GREATER;
	}
	else
	{
		result = libuna_utf8_string_compare_with_utf8_stream(
			  utf8_string,
			  utf8_string_length,
			  serialized_string->data,
			  serialized_string->data_size,
			  error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare serialize string with UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Compares an UTF-16 string with a serialized string
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libewf_serialized_string_compare_with_utf16_string(
     libewf_serialized_string_t *serialized_string,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libewf_serialized_string_compare_with_utf16_string";
	int result            = 0;

	if( serialized_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serialized string.",
		 function );

		return( -1 );
	}
	if( ( serialized_string->data == NULL )
	 || ( serialized_string->data_size == 0 ) )
	{
		result = LIBUNA_COMPARE_GREATER;
	}
	else
	{
		result = libuna_utf16_string_compare_with_utf8_stream(
			  utf16_string,
			  utf16_string_length,
			  serialized_string->data,
			  serialized_string->data_size,
			  error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare serialize string with UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

