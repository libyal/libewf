/*
 * Extent functions
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

#include "libewf_extent.h"
#include "libewf_lef_extent.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_types.h"

/* Creates an extent
 * Make sure the value extent is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_extent_initialize(
     libewf_extent_t **extent,
     libewf_lef_extent_t *lef_extent,
     libcerror_error_t **error )
{
	libewf_internal_extent_t *internal_extent = NULL;
	static char *function                     = "libewf_extent_initialize";

	if( extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent.",
		 function );

		return( -1 );
	}
	if( *extent != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent value already set.",
		 function );

		return( -1 );
	}
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
	internal_extent = memory_allocate_structure(
	                   libewf_internal_extent_t );

	if( internal_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extent.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_extent,
	     0,
	     sizeof( libewf_internal_extent_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extent.",
		 function );

		memory_free(
		 internal_extent );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_extent->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_extent->lef_extent = lef_extent;

	*extent = (libewf_extent_t *) internal_extent;

	return( 1 );

on_error:
	if( internal_extent != NULL )
	{
		memory_free(
		 internal_extent );
	}
	return( -1 );
}

/* Frees an extent
 * Returns 1 if successful or -1 on error
 */
int libewf_extent_free(
     libewf_extent_t **extent,
     libcerror_error_t **error )
{
	libewf_internal_extent_t *internal_extent = NULL;
	static char *function                     = "libewf_extent_free";
	int result                                = 1;

	if( extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent.",
		 function );

		return( -1 );
	}
	if( *extent != NULL )
	{
		internal_extent = (libewf_internal_extent_t *) *extent;
		*extent         = NULL;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_extent->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		memory_free(
		 internal_extent );
	}
	return( result );
}

