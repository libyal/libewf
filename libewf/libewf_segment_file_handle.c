/*
 * Segment file handle functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_segment_file_handle.h"

/* Initialize the segment file handle
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_handle_initialize(
     libewf_segment_file_handle_t **segment_file_handle,
     int segment_file_index,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_handle_initialize";

	if( segment_file_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
	if( *segment_file_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment file handle value already set.",
		 function );

		return( -1 );
	}
	if( segment_file_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid segment file index value less than zero.",
		 function );

		return( -1 );
	}
	*segment_file_handle = memory_allocate_structure(
	                        libewf_segment_file_handle_t );

	if( *segment_file_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment file handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *segment_file_handle,
	     0,
	     sizeof( libewf_segment_file_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear segment file handle.",
		 function );

		goto on_error;
	}
	( *segment_file_handle )->segment_file_index = segment_file_index;

	return( 1 );

on_error:
	if( *segment_file_handle != NULL )
	{
		memory_free(
		 *segment_file_handle );

		*segment_file_handle = NULL;
	}
	return( -1 );
}

/* Frees the segment file handle
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_handle_free(
     libewf_segment_file_handle_t **segment_file_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_handle_free";

	if( segment_file_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
	if( *segment_file_handle != NULL )
	{
		memory_free(
		 *segment_file_handle );

		*segment_file_handle = NULL;
	}
	return( 1 );
}

/* Clones the segment file handle
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_handle_clone(
     libewf_segment_file_handle_t **destination_segment_file_handle,
     libewf_segment_file_handle_t *source_segment_file_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_segment_file_handle_clone";

	if( destination_segment_file_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination segment file handle.",
		 function );

		return( -1 );
	}
	if( *destination_segment_file_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination segment file handle value already set.",
		 function );

		return( -1 );
	}
	if( source_segment_file_handle == NULL )
	{
		*destination_segment_file_handle = NULL;

		return( 1 );
	}
	*destination_segment_file_handle = memory_allocate_structure(
	                                    libewf_segment_file_handle_t );

	if( *destination_segment_file_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination segment file handle.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_segment_file_handle,
	     source_segment_file_handle,
	     sizeof( libewf_segment_file_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination segment file handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_segment_file_handle != NULL )
	{
		memory_free(
		 *destination_segment_file_handle );

		*destination_segment_file_handle = NULL;
	}
	return( -1 );
}

