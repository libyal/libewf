/*
 * Value table functions
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
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_value_table.h"

/* Retrieves the UTF-8 encoded string length of a table value
 * Returns 1 if successful or -1 on error
 */
int libewf_value_table_get_value_utf8_string_length(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_length,
     libcerror_error_t **error )
{
	libfvalue_value_t *value = NULL;
	static char *function    = "libewf_value_table_get_value_utf8_string_length";
	size_t utf8_string_size  = 0;
	int result               = 0;

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
	if( utf8_string_length == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string length.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          value_table,
	          identifier,
	          identifier_length,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table value: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = libfvalue_value_get_utf8_string_size(
		          value,
		          0,
		          &utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size of table value: %s.",
			 function,
			 identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( utf8_string_size > 0 )
			{
				utf8_string_size -= 1;
			}
		}
	}
	*utf8_string_length = utf8_string_size;

	return( 1 );
}

/* Copies a table value to an UTF-8 encoded string
 * Returns 1 if successful or -1 on error
 */
int libewf_value_table_get_value_copy_to_utf8_string_with_index(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     libcerror_error_t **error )
{
	libfvalue_value_t *value      = NULL;
	static char *function         = "libewf_value_table_get_value_copy_to_utf8_string_with_index";
	size_t safe_utf8_string_index = 0;
	int result                    = 0;

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
	if( utf8_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string index.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          value_table,
	          identifier,
	          identifier_length,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table value: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		safe_utf8_string_index = *utf8_string_index;

		result = libfvalue_value_copy_to_utf8_string_with_index(
		          value,
		          0,
		          utf8_string,
		          utf8_string_size,
		          &safe_utf8_string_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy table value: %s to UTF-8 string.",
			 function,
			 identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			*utf8_string_index = safe_utf8_string_index - 1;
		}
	}
	return( 1 );
}

/* Sets a specific value in the table
 * Returns 1 if successful or -1 on error
 */
int libewf_value_table_set_value_by_index(
     libfvalue_table_t *value_table,
     int value_index,
     const uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *value = NULL;
	static char *function    = "libewf_value_table_set_value_by_index";

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
	if( libfvalue_value_type_initialize(
	     &value,
	     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_identifier(
	     value,
	     identifier,
	     identifier_size,
	     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value: %s identifier.",
		 function,
		 (char *) identifier );

		goto on_error;
	}
	if( libfvalue_table_set_value_by_index(
	     value_table,
	     value_index,
	     value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value: %d in table.",
		 function,
		 value_index );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libfvalue_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Sets a value in the table specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_value_table_set_value_by_identifier(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_size,
     const uint8_t *value_string,
     size_t value_string_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *value = NULL;
	static char *function    = "libewf_value_table_set_value_by_identifier";

	if( libfvalue_value_type_initialize(
	     &value,
	     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_identifier(
	     value,
	     identifier,
	     identifier_size,
	     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value: %s identifier.",
		 function,
		 (char *) identifier );

		goto on_error;
	}
	if( libfvalue_value_set_data(
	     value,
	     value_string,
	     value_string_size,
	     LIBFVALUE_CODEPAGE_UTF8,
	     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value: %s data.",
		 function,
		 (char *) identifier );

		goto on_error;
	}
	if( libfvalue_table_set_value(
	     value_table,
	     value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value: %s in table.",
		 function,
		 (char *) identifier );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libfvalue_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_value_table_get_utf8_value_size(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *value = NULL;
	static char *function    = "libewf_value_table_get_utf8_value_size";
	int result               = 0;

	if( value_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          value_table,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = libfvalue_value_get_utf8_string_size(
		          value,
		          0,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size of value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 encoded value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_value_table_get_utf8_value(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *value = NULL;
	static char *function    = "libewf_value_table_get_utf8_value";
	int result               = 0;

	if( value_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          value_table,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = libfvalue_value_copy_to_utf8_string(
		          value,
		          0,
		          utf8_string,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Copies a value specified by the identifier from an UTF-8 encoded string
 * Returns 1 if successful or -1 on error
 */
int libewf_value_table_copy_value_from_utf8_string(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfvalue_value_t *safe_value = NULL;
	libfvalue_value_t *value      = NULL;
	static char *function         = "libewf_value_table_copy_value_from_utf8_string";
	int result                    = 0;

	if( value_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values.",
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
	if( identifier_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          value_table,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s.",
		 function,
		 (char *) identifier );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &safe_value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_identifier(
		     safe_value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s identifier.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		if( libfvalue_table_set_value(
		     value_table,
		     safe_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in table.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		value      = safe_value;
		safe_value = NULL;
	}
	if( libfvalue_value_copy_from_utf8_string(
	     value,
	     0,
	     utf8_string,
	     utf8_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value from UTF-8 string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( safe_value != NULL )
	{
		libfvalue_value_free(
		 &safe_value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size of the UTF-16 encoded value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_value_table_get_utf16_value_size(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *value = NULL;
	static char *function    = "libewf_value_table_get_utf16_value_size";
	int result               = 0;

	if( value_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          value_table,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = libfvalue_value_get_utf16_string_size(
		          value,
		          0,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size of value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 encoded value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_value_table_get_utf16_value(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *value = NULL;
	static char *function    = "libewf_value_table_get_utf16_value";
	int result               = 0;

	if( value_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value table.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          value_table,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = libfvalue_value_copy_to_utf16_string(
		          value,
		          0,
		          utf16_string,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Copies a value specified by the identifier from an UTF-16 encoded string
 * Returns 1 if successful or -1 on error
 */
int libewf_value_table_copy_value_from_utf16_string(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfvalue_value_t *safe_value = NULL;
	libfvalue_value_t *value      = NULL;
	static char *function         = "libewf_value_table_copy_value_from_utf16_string";
	int result                    = 0;

	if( value_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values.",
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
	if( identifier_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          value_table,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s.",
		 function,
		 (char *) identifier );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &safe_value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_identifier(
		     safe_value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s identifier.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		if( libfvalue_table_set_value(
		     value_table,
		     safe_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in table.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		value      = safe_value;
		safe_value = NULL;
	}
	if( libfvalue_value_copy_from_utf16_string(
	     value,
	     0,
	     utf16_string,
	     utf16_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value from UTF-16 string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( safe_value != NULL )
	{
		libfvalue_value_free(
		 &safe_value,
		 NULL );
	}
	return( -1 );
}
