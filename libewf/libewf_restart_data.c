/*
 * Restart data functions
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
#include "libewf_libcnotify.h"
#include "libewf_libuna.h"
#include "libewf_restart_data.h"

/* Parses EWF version 2 restart data
 * Returns 1 if successful or -1 on error
 */
int libewf_restart_data_parse(
     const uint8_t *restart_data,
     size_t restart_data_size,
     libcerror_error_t **error )
{
	uint8_t *restart_data_string    = NULL;
	static char *function           = "libewf_restart_data_parse";
	size_t restart_data_string_size = 0;

	if( restart_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid restart data.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     restart_data,
	     restart_data_size,
	     0,
	     &restart_data_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine restart data string size.",
		 function );

		goto on_error;
	}
	if( ( restart_data_string_size == 0 )
	 || ( restart_data_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid restart data string size value out of bounds.",
		 function );

		goto on_error;
	}
	restart_data_string = (uint8_t *) memory_allocate(
	                                   sizeof( uint8_t ) * restart_data_string_size );

	if( restart_data_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create restart data string.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     restart_data_string,
	     restart_data_string_size,
	     restart_data,
	     restart_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to set restart data string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: restart data string:\n%s",
		 function,
		 restart_data_string );
	}
#endif
/* TODO
	if( libewf_restart_data_parse_string(
	     restart_data_string,
	     restart_data_string_size,
	     media_values,
	     header_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse restart data string.",
		 function );

		goto on_error;
	}
*/
	memory_free(
	 restart_data_string );

	return( 1 );

on_error:
	if( restart_data_string != NULL )
	{
		memory_free(
		 restart_data_string );
	}
	return( -1 );
}

