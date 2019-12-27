/*
 * Access control entry functions
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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

#include "libewf_access_control_entry.h"
#include "libewf_lef_permission.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_types.h"

/* Creates a access control entry
 * Make sure the value access_control_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_access_control_entry_initialize(
     libewf_access_control_entry_t **access_control_entry,
     libewf_lef_permission_t *lef_permission,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_initialize";

	if( access_control_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid access control entry.",
		 function );

		return( -1 );
	}
	if( *access_control_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid access control entry value already set.",
		 function );

		return( -1 );
	}
	if( lef_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission.",
		 function );

		return( -1 );
	}
	internal_access_control_entry = memory_allocate_structure(
	                                 libewf_internal_access_control_entry_t );

	if( internal_access_control_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create access control entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_access_control_entry,
	     0,
	     sizeof( libewf_internal_access_control_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear access control entry.",
		 function );

		memory_free(
		 internal_access_control_entry );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_access_control_entry->read_write_lock ),
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
	internal_access_control_entry->lef_permission = lef_permission;

	*access_control_entry = (libewf_access_control_entry_t *) internal_access_control_entry;

	return( 1 );

on_error:
	if( internal_access_control_entry != NULL )
	{
		memory_free(
		 internal_access_control_entry );
	}
	return( -1 );
}

/* Frees a access control entry
 * Returns 1 if successful or -1 on error
 */
int libewf_access_control_entry_free(
     libewf_access_control_entry_t **access_control_entry,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_free";
	int result                                                            = 1;

	if( access_control_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid access control entry.",
		 function );

		return( -1 );
	}
	if( *access_control_entry != NULL )
	{
		internal_access_control_entry = (libewf_internal_access_control_entry_t *) *access_control_entry;
		*access_control_entry         = NULL;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_access_control_entry->read_write_lock ),
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
		/* The lef_permission reference is freed elsewhere
		 */
		memory_free(
		 internal_access_control_entry );
	}
	return( result );
}

