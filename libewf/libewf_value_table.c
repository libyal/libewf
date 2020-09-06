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

