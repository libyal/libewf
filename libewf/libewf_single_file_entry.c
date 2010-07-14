/*
 * Low level reading functions
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

#include <libcstring.h>
#include <liberror.h>

#include "libewf_single_file_entry.h"

/* Initialize the single file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_initialize(
     libewf_single_file_entry_t **single_file_entry,
     liberror_error_t **error )
{
	static char *function = "libewf_single_file_entry_initialize";

	if( single_file_entry == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( -1 );
	}
	if( *single_file_entry == NULL )
	{
		*single_file_entry = (libewf_single_file_entry_t *) memory_allocate(
		                                                     sizeof( libewf_single_file_entry_t ) );

		if( *single_file_entry == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create single file entry.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *single_file_entry,
		     0,
		     sizeof( libewf_single_file_entry_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear single file entry.",
			 function );

			memory_free(
			 *single_file_entry );

			*single_file_entry = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the single file entry including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_free(
     intptr_t *single_file_entry,
     liberror_error_t **error )
{
	static char *function = "libewf_single_file_entry_free";

	if( single_file_entry == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file entry.",
		 function );

		return( 1 );
	}
	if( ( (libewf_single_file_entry_t *) single_file_entry )->name != NULL )
	{
		memory_free(
		 ( (libewf_single_file_entry_t *) single_file_entry )->name );
	}
	if( ( (libewf_single_file_entry_t *) single_file_entry )->md5_hash != NULL )
	{
		memory_free(
		 ( (libewf_single_file_entry_t *) single_file_entry )->md5_hash );
	}
	memory_free(
	 single_file_entry );

	return( 1 );
}

/* Clones the existing single file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_entry_clone(
     intptr_t **destination,
     intptr_t *source,
     liberror_error_t **error )
{
	/* TODO */
	return( -1 );
}

