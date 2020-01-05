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

#if !defined( _LIBEWF_PERMISSION_GROUP_H )
#define _LIBEWF_PERMISSION_GROUP_H

#include <common.h>
#include <types.h>

#include "libewf_lef_permission.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_permission_group libewf_permission_group_t;

struct libewf_permission_group
{
	/* The permissions array
	 */
	libcdata_array_t *permissions;
};

int libewf_permission_group_initialize(
     libewf_permission_group_t **permission_group,
     libcerror_error_t **error );

int libewf_permission_group_free(
     libewf_permission_group_t **permission_group,
     libcerror_error_t **error );

int libewf_permission_group_clone(
     libewf_permission_group_t **destination_permission_group,
     libewf_permission_group_t *source_permission_group,
     libcerror_error_t **error );

int libewf_permission_group_append_permission(
     libewf_permission_group_t *permission_group,
     libewf_lef_permission_t *lef_permission,
     libcerror_error_t **error );

int libewf_permission_group_get_number_of_entries(
     libewf_permission_group_t *permission_group,
     int *number_of_entries,
     libcerror_error_t **error );

int libewf_permission_group_get_entry_by_index(
     libewf_permission_group_t *permission_group,
     int entry_index,
     libewf_lef_permission_t **lef_permission,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_PERMISSION_GROUP_H ) */

