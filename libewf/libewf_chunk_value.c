/*
 * Chunk value functions
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

#include "libewf_chunk_value.h"

/* Initialize the segment file handle
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_value_initialize(
     libewf_chunk_value_t **chunk_value,
     liberror_error_t **error )
{
	static char *function = "libewf_chunk_value_initialize";

	if( chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
	if( *chunk_value == NULL )
	{
		*chunk_value = (libewf_chunk_value_t *) memory_allocate(
		                                         sizeof( libewf_chunk_value_t ) );

		if( *chunk_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment file handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *chunk_value,
		     0,
		     sizeof( libewf_chunk_value_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear segment file handle.",
			 function );

			memory_free(
			 *chunk_value );

			*chunk_value = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the segment file handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_value_free(
     intptr_t *chunk_value,
     liberror_error_t **error )
{
	static char *function = "libewf_chunk_value_free";

	if( chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
	memory_free(
	 chunk_value );

	return( 1 );
}

