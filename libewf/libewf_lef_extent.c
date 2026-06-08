/*
 * Logical Evidence File (LEF) extent functions
 *
 * Copyright (C) 2006-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include "libewf_lef_extent.h"
#include "libewf_libcerror.h"

/* Creates a extent
 * Make sure the value lef_extent is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_extent_initialize(
     libewf_lef_extent_t **lef_extent,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_extent_initialize";

	if( lef_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent.",
		 function );

		return( -1 );
	}
	if( *lef_extent != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent value already set.",
		 function );

		return( -1 );
	}
	*lef_extent = memory_allocate_structure(
	                libewf_lef_extent_t );

	if( *lef_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extent.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *lef_extent,
	     0,
	     sizeof( libewf_lef_extent_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extent.",
		 function );

		memory_free(
		 *lef_extent );

		*lef_extent = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *lef_extent != NULL )
	{
		memory_free(
		 *lef_extent );

		*lef_extent = NULL;
	}
	return( -1 );
}

/* Frees a extent
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_extent_free(
     libewf_lef_extent_t **lef_extent,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_extent_free";
	int result            = 1;

	if( lef_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent.",
		 function );

		return( -1 );
	}
	if( *lef_extent != NULL )
	{
		memory_free(
		 *lef_extent );

		*lef_extent = NULL;
	}
	return( result );
}

/* Clones the extent
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_extent_clone(
     libewf_lef_extent_t **destination_lef_extent,
     libewf_lef_extent_t *source_lef_extent,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_extent_clone";

	if( destination_lef_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination extent.",
		 function );

		return( -1 );
	}
	if( *destination_lef_extent != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination extent value already set.",
		 function );

		return( -1 );
	}
	if( source_lef_extent == NULL )
	{
		*destination_lef_extent = NULL;

		return( 1 );
	}
	*destination_lef_extent = memory_allocate_structure(
	                            libewf_lef_extent_t );

	if( *destination_lef_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination extent.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_lef_extent,
	     source_lef_extent,
	     sizeof( libewf_lef_extent_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination extent.",
		 function );

		memory_free(
		 *destination_lef_extent );

		*destination_lef_extent = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *destination_lef_extent != NULL )
	{
		libewf_lef_extent_free(
		 destination_lef_extent,
		 NULL );
	}
	return( -1 );
}

