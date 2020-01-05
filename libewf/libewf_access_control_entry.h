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

#if !defined( _LIBEWF_ACCESS_CONTROL_ENTRY_H )
#define _LIBEWF_ACCESS_CONTROL_ENTRY_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_lef_permission.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_access_control_entry libewf_internal_access_control_entry_t;

struct libewf_internal_access_control_entry
{
	/* The permission
	 */
	libewf_lef_permission_t *lef_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libewf_access_control_entry_initialize(
     libewf_access_control_entry_t **access_control_entry,
     libewf_lef_permission_t *lef_permission,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_free(
     libewf_access_control_entry_t **access_control_entry,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_type(
     libewf_access_control_entry_t *access_control_entry,
     uint32_t *type,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf8_identifier_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf8_identifier(
     libewf_access_control_entry_t *access_control_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf16_identifier_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf16_identifier(
     libewf_access_control_entry_t *access_control_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf8_name_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf8_name(
     libewf_access_control_entry_t *access_control_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf16_name_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf16_name(
     libewf_access_control_entry_t *access_control_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_access_mask(
     libewf_access_control_entry_t *access_control_entry,
     uint32_t *access_mask,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_access_control_entry_get_flags(
     libewf_access_control_entry_t *access_control_entry,
     uint32_t *flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_ACCESS_CONTROL_ENTRY_H ) */

