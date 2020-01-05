/*
 * Low level reading functions
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

#include "libewf_chunk_data.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_media_values.h"
#include "libewf_read_io_handle.h"
#include "libewf_segment_file.h"

/* Creates a read IO handle
 * Make sure the value read_io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_read_io_handle_initialize(
     libewf_read_io_handle_t **read_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_read_io_handle_initialize";

	if( read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read IO handle.",
		 function );

		return( -1 );
	}
	if( *read_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid read IO handle value already set.",
		 function );

		return( -1 );
	}
	*read_io_handle = memory_allocate_structure(
	                   libewf_read_io_handle_t );

	if( *read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create read IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *read_io_handle,
	     0,
	     sizeof( libewf_read_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear read IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *read_io_handle != NULL )
	{
		memory_free(
		 *read_io_handle );

		*read_io_handle = NULL;
	}
	return( -1 );
}

/* Frees a read IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_read_io_handle_free(
     libewf_read_io_handle_t **read_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_read_io_handle_free";
	int result            = 1;

	if( read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read IO handle.",
		 function );

		return( -1 );
	}
	if( *read_io_handle != NULL )
	{
		if( ( *read_io_handle )->case_data != NULL )
		{
			memory_free(
			 ( *read_io_handle )->case_data );
		}
		if( ( *read_io_handle )->device_information != NULL )
		{
			memory_free(
			 ( *read_io_handle )->device_information );
		}
		memory_free(
		 *read_io_handle );

		*read_io_handle = NULL;
	}
	return( result );
}

/* Clones the read IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_read_io_handle_clone(
     libewf_read_io_handle_t **destination_read_io_handle,
     libewf_read_io_handle_t *source_read_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_read_io_handle_clone";

	if( destination_read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination read IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_read_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination read IO handle value already set.",
		 function );

		return( -1 );
	}
	if( source_read_io_handle == NULL )
	{
		*destination_read_io_handle = NULL;

		return( 1 );
	}
	*destination_read_io_handle = memory_allocate_structure(
	                               libewf_read_io_handle_t );

	if( *destination_read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination read IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *destination_read_io_handle,
	     0,
	     sizeof( libewf_read_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear source to destination read IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_read_io_handle != NULL )
	{
		memory_free(
		 *destination_read_io_handle );

		*destination_read_io_handle = NULL;
	}
	return( -1 );
}

