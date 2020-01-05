/*
 * Permission group functions
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

#include "libewf_lef_permission.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_permission_group.h"

/* Creates a permission group
 * Make sure the value permission_group is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_permission_group_initialize(
     libewf_permission_group_t **permission_group,
     libcerror_error_t **error )
{
	static char *function = "libewf_permission_group_initialize";

	if( permission_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission group.",
		 function );

		return( -1 );
	}
	if( *permission_group != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid permission group value already set.",
		 function );

		return( -1 );
	}
	*permission_group = memory_allocate_structure(
	                     libewf_permission_group_t );

	if( *permission_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create permission group.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *permission_group,
	     0,
	     sizeof( libewf_permission_group_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear permission group.",
		 function );

		memory_free(
		 *permission_group );

		*permission_group = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *permission_group )->permissions ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create permissions array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *permission_group != NULL )
	{
		memory_free(
		 *permission_group );

		*permission_group = NULL;
	}
	return( -1 );
}

/* Frees a permission group
 * Returns 1 if successful or -1 on error
 */
int libewf_permission_group_free(
     libewf_permission_group_t **permission_group,
     libcerror_error_t **error )
{
	static char *function = "libewf_permission_group_free";
	int result            = 1;

	if( permission_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission group.",
		 function );

		return( -1 );
	}
	if( *permission_group != NULL )
	{
		if( libcdata_array_free(
		     &( ( *permission_group )->permissions ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_permission_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free permissions array.",
			 function );

			result = -1;
		}
		memory_free(
		 *permission_group );

		*permission_group = NULL;
	}
	return( result );
}

/* Clones the permission group
 * Returns 1 if successful or -1 on error
 */
int libewf_permission_group_clone(
     libewf_permission_group_t **destination_permission_group,
     libewf_permission_group_t *source_permission_group,
     libcerror_error_t **error )
{
	static char *function = "libewf_permission_group_clone";

	if( destination_permission_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination permission group.",
		 function );

		return( -1 );
	}
	if( *destination_permission_group != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination permission group value already set.",
		 function );

		return( -1 );
	}
	if( source_permission_group == NULL )
	{
		*destination_permission_group = NULL;

		return( 1 );
	}
	*destination_permission_group = memory_allocate_structure(
	                                 libewf_permission_group_t );

	if( *destination_permission_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination permission group.",
		 function );

		goto on_error;
	}
	( *destination_permission_group )->permissions = NULL;

	if( libcdata_array_clone(
	     &( ( *destination_permission_group )->permissions ),
	     source_permission_group->permissions,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_lef_permission_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_lef_permission_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination permissions array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_permission_group != NULL )
	{
		memory_free(
		 *destination_permission_group );

		*destination_permission_group = NULL;
	}
	return( -1 );
}

/* Appends a permission to the group
 * Returns 1 if successful or -1 on error
 */
int libewf_permission_group_append_permission(
     libewf_permission_group_t *permission_group,
     libewf_lef_permission_t *lef_permission,
     libcerror_error_t **error )
{
	static char *function = "libewf_permission_group_append_permission";
	int entry_index       = 0;

	if( permission_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission group.",
		 function );

		return( -1 );
	}
	if( libcdata_array_append_entry(
	     permission_group->permissions,
	     &entry_index,
	     (intptr_t *) lef_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append entry to permissions array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of permissions
 * Returns 1 if successful or -1 on error
 */
int libewf_permission_group_get_number_of_entries(
     libewf_permission_group_t *permission_group,
     int *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libewf_permission_group_get_number_of_entries";

	if( permission_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission group.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     permission_group->permissions,
	     number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from permissions array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific permission from the group
 * Returns 1 if successful or -1 on error
 */
int libewf_permission_group_get_entry_by_index(
     libewf_permission_group_t *permission_group,
     int entry_index,
     libewf_lef_permission_t **lef_permission,
     libcerror_error_t **error )
{
	static char *function = "libewf_permission_group_get_entry_by_index";

	if( permission_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid permission group.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     permission_group->permissions,
	     entry_index,
	     (intptr_t **) lef_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from permissions array.",
		 function,
		 entry_index );

		return( -1 );
	}
	return( 1 );
}

