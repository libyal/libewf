/*
 * Hash sections functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#include "libewf_hash_sections.h"

/* Initialize the hash sections
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_sections_initialize(
     libewf_hash_sections_t **hash_sections,
     liberror_error_t **error )
{
	static char *function = "libewf_hash_sections_initialize";

	if( hash_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( *hash_sections == NULL )
	{
		*hash_sections = (libewf_hash_sections_t *) memory_allocate(
		                                             sizeof( libewf_hash_sections_t ) );

		if( *hash_sections == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create hash sections.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *hash_sections,
		     0,
		     sizeof( libewf_hash_sections_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear hash sections.",
			 function );

			memory_free(
			 *hash_sections );

			*hash_sections = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the hash sections including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_sections_free(
     libewf_hash_sections_t **hash_sections,
     liberror_error_t **error )
{
        static char *function = "libewf_hash_sections_free";

	if( hash_sections == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( *hash_sections != NULL )
	{
		if( ( *hash_sections )->xhash != NULL )
		{
			memory_free(
			 ( *hash_sections )->xhash );
		}
		memory_free(
		 *hash_sections );

		*hash_sections = NULL;
	}
	return( 1 );
}

