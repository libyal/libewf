/*
 * Logical Evidence File (LEF) permission functions
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

#if !defined( _LIBEWF_LEF_PERMISSION_H )
#define _LIBEWF_LEF_PERMISSION_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_serialized_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_lef_permission libewf_lef_permission_t;

struct libewf_lef_permission
{
	/* The name string
	 */
	libewf_serialized_string_t *name;

	/* The identifier string
	 * Contains either a Windows NT Security Identifier (SID) or a POSIX user (uid) or group (gid) group identifier
	 */
	libewf_serialized_string_t *identifier;

	/* The property type
	 */
	uint32_t property_type;

	/* The access mask
	 */
	uint32_t access_mask;

	/* The Windows NT access control entry (ACE) flags
	 */
	uint32_t ace_flags;
};

int libewf_lef_permission_initialize(
     libewf_lef_permission_t **lef_permission,
     libcerror_error_t **error );

int libewf_lef_permission_free(
     libewf_lef_permission_t **lef_permission,
     libcerror_error_t **error );

int libewf_lef_permission_clone(
     libewf_lef_permission_t **destination_lef_permission,
     libewf_lef_permission_t *source_lef_permission,
     libcerror_error_t **error );

int libewf_lef_permission_read_data(
     libewf_lef_permission_t *lef_permission,
     libfvalue_split_utf8_string_t *types,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_lef_permission_get_utf8_name_size(
     libewf_lef_permission_t *lef_permission,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_permission_get_utf8_name(
     libewf_lef_permission_t *lef_permission,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_permission_get_utf16_name_size(
     libewf_lef_permission_t *lef_permission,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_permission_get_utf16_name(
     libewf_lef_permission_t *lef_permission,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_permission_get_utf8_identifier_size(
     libewf_lef_permission_t *lef_permission,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_permission_get_utf8_identifier(
     libewf_lef_permission_t *lef_permission,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_permission_get_utf16_identifier_size(
     libewf_lef_permission_t *lef_permission,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_permission_get_utf16_identifier(
     libewf_lef_permission_t *lef_permission,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_permission_get_property_type(
     libewf_lef_permission_t *lef_permission,
     uint32_t *property_type,
     libcerror_error_t **error );

int libewf_lef_permission_get_access_mask(
     libewf_lef_permission_t *lef_permission,
     uint32_t *access_mask,
     libcerror_error_t **error );

int libewf_lef_permission_get_ace_flags(
     libewf_lef_permission_t *lef_permission,
     uint32_t *ace_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_LEF_PERMISSION_H ) */

