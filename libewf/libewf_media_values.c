/*
 * Media values functions
 *
 * Copyright (c) 2006-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <liberror.h>

#include "libewf_media_values.h"

#include "ewf_definitions.h"

/* Initialize the media values
 * Returns 1 if successful or -1 on error
 */
int libewf_media_values_initialize(
     libewf_media_values_t **media_values,
     liberror_error_t **error )
{
	static char *function = "libewf_media_values_initialize";

	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( *media_values != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid media values value already set.",
		 function );

		return( -1 );
	}
	*media_values = memory_allocate_structure(
	                 libewf_media_values_t );

	if( *media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create media values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *media_values,
	     0,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear media values.",
		 function );

		goto on_error;
	}
	( *media_values )->chunk_size        = EWF_MINIMUM_CHUNK_SIZE;
	( *media_values )->sectors_per_chunk = 64;
	( *media_values )->bytes_per_sector  = 512;
	( *media_values )->media_flags       = 0x01;

	return( 1 );

on_error:
	if( *media_values != NULL )
	{
		memory_free(
		 *media_values );

		*media_values = NULL;
	}
	return( -1 );
}

/* Frees the media values including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_media_values_free(
     libewf_media_values_t **media_values,
     liberror_error_t **error )
{
        static char *function = "libewf_media_values_free";

	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( *media_values != NULL )
	{
		memory_free(
		 *media_values );

		*media_values = NULL;
	}
	return( 1 );
}

/* Clones the media values
 * Returns 1 if successful or -1 on error
 */
int libewf_media_values_clone(
     libewf_media_values_t **destination_media_values,
     libewf_media_values_t *source_media_values,
     liberror_error_t **error )
{
	static char *function = "libewf_media_values_clone";

	if( destination_media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination media values.",
		 function );

		return( -1 );
	}
	if( *destination_media_values != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination media values already set.",
		 function );

		return( -1 );
	}
	if( source_media_values == NULL )
	{
		*destination_media_values = NULL;

		return( 1 );
	}
	*destination_media_values = memory_allocate_structure(
		                     libewf_media_values_t );

	if( *destination_media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination media values.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_media_values,
	     source_media_values,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination media values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_media_values != NULL )
	{
		memory_free(
		 *destination_media_values );

		*destination_media_values = NULL;
	}
	return( -1 );
}

