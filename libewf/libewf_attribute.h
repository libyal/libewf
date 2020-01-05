/*
 * Attribute functions
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

#if !defined( _LIBEWF_ATTRIBUTE_H )
#define _LIBEWF_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_lef_extended_attribute.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_attribute libewf_internal_attribute_t;

struct libewf_internal_attribute
{
	/* The extended attribute
	 */
	libewf_lef_extended_attribute_t *lef_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libewf_attribute_initialize(
     libewf_attribute_t **attribute,
     libewf_lef_extended_attribute_t *lef_extended_attribute,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_attribute_free(
     libewf_attribute_t **attribute,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_attribute_get_utf8_name_size(
     libewf_attribute_t *attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_attribute_get_utf8_name(
     libewf_attribute_t *attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_attribute_get_utf16_name_size(
     libewf_attribute_t *attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_attribute_get_utf16_name(
     libewf_attribute_t *attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_attribute_get_utf8_value_size(
     libewf_attribute_t *attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_attribute_get_utf8_value(
     libewf_attribute_t *attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_attribute_get_utf16_value_size(
     libewf_attribute_t *attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_attribute_get_utf16_value(
     libewf_attribute_t *attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_ATTRIBUTE_H ) */

