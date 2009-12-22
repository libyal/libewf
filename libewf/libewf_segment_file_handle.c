/*
 * Segment file handle
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
#include <types.h>

#include <liberror.h>

#include "libewf_filename.h"
#include "libewf_list_type.h"
#include "libewf_section_list.h"
#include "libewf_segment_file_handle.h"

/* Initialize the segment file handle
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_handle_initialize(
     libewf_segment_file_handle_t **segment_file_handle,
     int file_io_pool_entry,
     liberror_error_t **error )
{
	static char *function = "libewf_segment_file_handle_initialize";

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
	if( *segment_file_handle == NULL )
	{
		*segment_file_handle = (libewf_segment_file_handle_t *) memory_allocate(
		                                                         sizeof( libewf_segment_file_handle_t ) );

		if( *segment_file_handle == NULL )
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
		     *segment_file_handle,
		     0,
		     sizeof( libewf_segment_file_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear segment file handle.",
			 function );

			memory_free(
			 *segment_file_handle );

			*segment_file_handle = NULL;

			return( -1 );
		}
		( *segment_file_handle )->section_list = (libewf_list_t *) memory_allocate(
		                                                            sizeof( libewf_list_t ) );

		if( ( *segment_file_handle )->section_list == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create section list.",
			 function );

			memory_free(
			 *segment_file_handle );

			*segment_file_handle = NULL;

			return( -1 );
		}
		if( memory_set(
		     ( *segment_file_handle )->section_list,
		     0,
		     sizeof( libewf_list_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear section list.",
			 function );

			memory_free(
			 ( *segment_file_handle )->section_list );
			memory_free(
			 *segment_file_handle );

			*segment_file_handle = NULL;

			return( -1 );
		}
		( *segment_file_handle )->file_io_pool_entry = file_io_pool_entry;
	}
	return( 1 );
}

/* Frees the segment file handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_handle_free(
     libewf_segment_file_handle_t **segment_file_handle,
     liberror_error_t **error )
{
	static char *function = "libewf_segment_file_handle_free";
	int result            = 1;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
	if( *segment_file_handle != NULL )
	{
		if( ( *segment_file_handle )->section_list != NULL )
		{
			if( libewf_list_free(
			     &( ( *segment_file_handle )->section_list ),
			     &libewf_section_list_values_free,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free section list.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *segment_file_handle );

		*segment_file_handle = NULL;
	}
	return( result );
}

