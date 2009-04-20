/*
 * IO handle functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#include <liberror.h>

#include "libewf_definitions.h"
#include "libewf_libbfio.h"
#include "libewf_io_handle.h"

#include "ewf_definitions.h"

/* Initialize the write io handle
 * Returns 1 if successful or -1 on error
 */
int libewf_io_handle_initialize(
     libewf_io_handle_t **io_handle,
     liberror_error_t **error )
{
	static char *function = "libewf_io_handle_initialize";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( -1 );
	}
	if( *io_handle == NULL )
	{
		*io_handle = (libewf_io_handle_t *) memory_allocate(
		                                     sizeof( libewf_io_handle_t ) );

		if( io_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create write io handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *io_handle,
		     0,
		     sizeof( libewf_io_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear write io handle.",
			 function );

			memory_free(
			 *io_handle );

			*io_handle = NULL;

			return( -1 );
		}
		( *io_handle )->format            = LIBEWF_FORMAT_UNKNOWN;
		( *io_handle )->ewf_format        = EWF_FORMAT_UNKNOWN;
		( *io_handle )->compression_level = EWF_COMPRESSION_UNKNOWN;
	}
	return( 1 );
}

/* Frees the write io handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_io_handle_free(
     libewf_io_handle_t **io_handle,
     liberror_error_t **error )
{
	static char *function = "libewf_io_handle_free";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( 1 );
	}
	if( *io_handle != NULL )
	{
		if( ( ( *io_handle )->pool_created_in_library != 0 )
		 && ( libbfio_pool_free(
		       &( ( *io_handle )->file_io_pool ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file io pool.",
			 function );
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( 1 );
}

