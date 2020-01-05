/*
 * Logical Evidence File (LEF) extended attribute functions
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

#if !defined( _LIBEWF_LEF_EXTENDED_ATTRIBUTE_H )
#define _LIBEWF_LEF_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_lef_extended_attribute libewf_lef_extended_attribute_t;

struct libewf_lef_extended_attribute
{
	/* The UTF-16 little-endian encoded name
	 */
	uint8_t *name;

	/* The size of the UTF-16 little-endian encoded name
	 */
	size_t name_size;

	/* The UTF-16 little-endian encoded value
	 */
	uint8_t *value;

	/* The size of the UTF-16 little-endian encoded value
	 */
	size_t value_size;

	/* Value to indicate if the extended attribute is a branch
	 */
	uint8_t is_branch;
};

int libewf_lef_extended_attribute_initialize(
     libewf_lef_extended_attribute_t **lef_extended_attribute,
     libcerror_error_t **error );

int libewf_lef_extended_attribute_free(
     libewf_lef_extended_attribute_t **lef_extended_attribute,
     libcerror_error_t **error );

int libewf_lef_extended_attribute_clone(
     libewf_lef_extended_attribute_t **destination_lef_extended_attribute,
     libewf_lef_extended_attribute_t *source_lef_extended_attribute,
     libcerror_error_t **error );

ssize_t libewf_lef_extended_attribute_read_data(
         libewf_lef_extended_attribute_t *lef_extended_attribute,
         const uint8_t *data,
         size_t data_size,
         libcerror_error_t **error );

int libewf_lef_extended_attribute_get_utf8_name_size(
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_extended_attribute_get_utf8_name(
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_extended_attribute_get_utf16_name_size(
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_extended_attribute_get_utf16_name(
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_extended_attribute_get_utf8_value_size(
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_extended_attribute_get_utf8_value(
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_extended_attribute_get_utf16_value_size(
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_extended_attribute_get_utf16_value(
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_LEF_EXTENDED_ATTRIBUTE_H ) */

