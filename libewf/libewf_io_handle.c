/*
 * IO handle functions
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

#include "libewf_codepage.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libcerror.h"

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_io_handle_initialize(
     libewf_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libewf_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libewf_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	( *io_handle )->segment_file_type  = LIBEWF_SEGMENT_FILE_TYPE_UNDEFINED;
	( *io_handle )->format             = LIBEWF_FORMAT_ENCASE6;
	( *io_handle )->major_version      = 1;
	( *io_handle )->minor_version      = 0;
	( *io_handle )->compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;
	( *io_handle )->compression_level  = LIBEWF_COMPRESSION_LEVEL_NONE;
	( *io_handle )->zero_on_error      = 1;
	( *io_handle )->header_codepage    = LIBEWF_CODEPAGE_ASCII;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_io_handle_free(
     libewf_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		if( libewf_io_handle_clear(
		     *io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_io_handle_clear(
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_io_handle_clear";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libewf_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	io_handle->segment_file_type  = LIBEWF_SEGMENT_FILE_TYPE_UNDEFINED;
	io_handle->format             = LIBEWF_FORMAT_ENCASE6;
	io_handle->major_version      = 1;
	io_handle->minor_version      = 0;
	io_handle->compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;
	io_handle->compression_level  = LIBEWF_COMPRESSION_LEVEL_NONE;
	io_handle->zero_on_error      = 1;
	io_handle->header_codepage    = LIBEWF_CODEPAGE_ASCII;

	return( 1 );
}

/* Clones the IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_io_handle_clone(
     libewf_io_handle_t **destination_io_handle,
     libewf_io_handle_t *source_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_io_handle_clone";

	if( destination_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination IO handle value already set.",
		 function );

		return( -1 );
	}
	if( source_io_handle == NULL )
	{
		*destination_io_handle = NULL;

		return( 1 );
	}
	*destination_io_handle = memory_allocate_structure(
		                  libewf_io_handle_t );

	if( *destination_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination IO handle.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_io_handle,
	     source_io_handle,
	     sizeof( libewf_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination IO handle.",
		 function );

		goto on_error;
	}
	( *destination_io_handle )->zero_on_error = source_io_handle->zero_on_error;

	return( 1 );

on_error:
	if( *destination_io_handle != NULL )
	{
		memory_free(
		 *destination_io_handle );

		*destination_io_handle = NULL;
	}
	return( -1 );
}

