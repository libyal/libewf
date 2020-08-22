/*
 * Analytical data functions
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

#include "libewf_analytical_data.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libuna.h"

/* Parses EWF version 2 analytical data
 * Returns 1 if successful or -1 on error
 */
int libewf_analytical_data_parse(
     const uint8_t *analytical_data,
     size_t analytical_data_size,
     libcerror_error_t **error )
{
	uint8_t *analytical_data_string    = NULL;
	static char *function              = "libewf_analytical_data_parse";
	size_t analytical_data_string_size = 0;

	if( analytical_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid analytical data.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     analytical_data,
	     analytical_data_size,
	     0,
	     &analytical_data_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine analytical data string size.",
		 function );

		goto on_error;
	}
	if( ( analytical_data_string_size == 0 )
	 || ( analytical_data_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid analytical data string size value out of bounds.",
		 function );

		goto on_error;
	}
	analytical_data_string = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * analytical_data_string_size );

	if( analytical_data_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create analytical data string.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     analytical_data_string,
	     analytical_data_string_size,
	     analytical_data,
	     analytical_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to set analytical data string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: analytical data string:\n%s",
		 function,
		 analytical_data_string );
	}
#endif
/* TODO add support for analytical data
	if( libewf_analytical_data_parse_string(
	     analytical_data_string,
	     analytical_data_string_size,
	     media_values,
	     header_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse analytical data string.",
		 function );

		goto on_error;
	}
*/
	memory_free(
	 analytical_data_string );

	return( 1 );

on_error:
	if( analytical_data_string != NULL )
	{
		memory_free(
		 analytical_data_string );
	}
	return( -1 );
}

