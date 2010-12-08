/*
 * IO handle functions
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include "libewf_codepage.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"

#include "ewf_definitions.h"

/* Initialize the write IO handle
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
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle == NULL )
	{
		*io_handle = memory_allocate_structure(
		              libewf_io_handle_t );

		if( io_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create write IO handle.",
			 function );

			goto on_error;
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
			 "%s: unable to clear write IO handle.",
			 function );

			goto on_error;
		}
		( *io_handle )->format            = LIBEWF_FORMAT_ENCASE5;
		( *io_handle )->ewf_format        = EWF_FORMAT_E01;
		( *io_handle )->compression_level = EWF_COMPRESSION_NONE;
		( *io_handle )->header_codepage   = LIBEWF_CODEPAGE_ASCII;
	}
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

/* Frees the write IO handle including elements
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
		 "%s: invalid write IO handle.",
		 function );

		return( 1 );
	}
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( 1 );
}

/* Retrieves the current chunk and chunk offset
 * Returns 1 if the succesful or -1 on error
 */
int libewf_io_handle_get_current_chunk(
     libewf_io_handle_t *io_handle,
     uint32_t *current_chunk,
     size_t *current_chunk_offset,
     liberror_error_t **error )
{
	static char *function = "libewf_io_handle_get_current_chunk";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( current_chunk == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid current chunk.",
		 function );

		return( -1 );
	}
	if( current_chunk_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid current chunk offset.",
		 function );

		return( -1 );
	}
	*current_chunk        = io_handle->current_chunk;
	*current_chunk_offset = io_handle->current_chunk_offset;

	return( 1 );
}

/* Sets the current chunk and chunk offset
 * Returns 1 if the succesful or -1 on error
 */
int libewf_io_handle_set_current_chunk(
     libewf_io_handle_t *io_handle,
     uint32_t current_chunk,
     size_t current_chunk_offset,
     liberror_error_t **error )
{
	static char *function = "libewf_io_handle_set_current_chunk";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	io_handle->current_chunk        = current_chunk;
	io_handle->current_chunk_offset = current_chunk_offset;

	return( 1 );
}

