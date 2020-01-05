/*
 * Access control entry functions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
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

/* Retrieves the (property) type
 * Returns 1 if successful or -1 on error
 */
int libewf_access_control_entry_get_type(
     libewf_access_control_entry_t *access_control_entry,
     uint32_t *type,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_type";
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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_lef_permission_get_property_type(
	     internal_access_control_entry->lef_permission,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve property type.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_access_control_entry_get_utf8_identifier_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_utf8_identifier_size";
	int result                                                            = 0;

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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_utf8_identifier_size(
	          internal_access_control_entry->lef_permission,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 identifier size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_access_control_entry_get_utf8_identifier(
     libewf_access_control_entry_t *access_control_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_utf8_identifier";
	int result                                                            = 0;

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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_utf8_identifier(
	          internal_access_control_entry->lef_permission,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 identifier size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_access_control_entry_get_utf16_identifier_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_utf16_identifier_size";
	int result                                                            = 0;

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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_utf16_identifier_size(
	          internal_access_control_entry->lef_permission,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 identifier size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_access_control_entry_get_utf16_identifier(
     libewf_access_control_entry_t *access_control_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_utf16_identifier";
	int result                                                            = 0;

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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_utf16_identifier(
	          internal_access_control_entry->lef_permission,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 identifier size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_access_control_entry_get_utf8_name_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_utf8_name_size";
	int result                                                            = 0;

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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_utf8_name_size(
	          internal_access_control_entry->lef_permission,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_access_control_entry_get_utf8_name(
     libewf_access_control_entry_t *access_control_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_utf8_name";
	int result                                                            = 0;

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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_utf8_name(
	          internal_access_control_entry->lef_permission,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_access_control_entry_get_utf16_name_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_utf16_name_size";
	int result                                                            = 0;

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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_utf16_name_size(
	          internal_access_control_entry->lef_permission,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_access_control_entry_get_utf16_name(
     libewf_access_control_entry_t *access_control_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_utf16_name";
	int result                                                            = 0;

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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_utf16_name(
	          internal_access_control_entry->lef_permission,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the access mask
 * Returns 1 if successful or -1 on error
 */
int libewf_access_control_entry_get_access_mask(
     libewf_access_control_entry_t *access_control_entry,
     uint32_t *access_mask,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_access_mask";
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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_access_mask(
	          internal_access_control_entry->lef_permission,
	          access_mask,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access mask.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the flags
 * Contains Windows NT access control entry (ACE) flags
 * Returns 1 if successful or -1 on error
 */
int libewf_access_control_entry_get_flags(
     libewf_access_control_entry_t *access_control_entry,
     uint32_t *flags,
     libcerror_error_t **error )
{
	libewf_internal_access_control_entry_t *internal_access_control_entry = NULL;
	static char *function                                                 = "libewf_access_control_entry_get_flags";
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
	internal_access_control_entry = (libewf_internal_access_control_entry_t *) access_control_entry;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_lef_permission_get_ace_flags(
	          internal_access_control_entry->lef_permission,
	          flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve ACE flags.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_access_control_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

