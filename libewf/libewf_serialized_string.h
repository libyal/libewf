/*
 * Serialized (file object) string functions
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

#if !defined( _LIBEWF_SERIALIZED_STRING_H )
#define _LIBEWF_SERIALIZED_STRING_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_serialized_string libewf_serialized_string_t;

struct libewf_serialized_string
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;
};

int libewf_serialized_string_initialize(
     libewf_serialized_string_t **serialized_string,
     libcerror_error_t **error );

int libewf_serialized_string_free(
     libewf_serialized_string_t **serialized_string,
     libcerror_error_t **error );

int libewf_serialized_string_clone(
     libewf_serialized_string_t **destination_serialized_string,
     libewf_serialized_string_t *source_serialized_string,
     libcerror_error_t **error );

int libewf_serialized_string_read_data(
     libewf_serialized_string_t *serialized_string,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_serialized_string_read_hexadecimal_data(
     libewf_serialized_string_t *serialized_string,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_serialized_string_get_utf8_string_size(
     libewf_serialized_string_t *serialized_string,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_serialized_string_get_utf8_string(
     libewf_serialized_string_t *serialized_string,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_serialized_string_get_utf16_string_size(
     libewf_serialized_string_t *serialized_string,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_serialized_string_get_utf16_string(
     libewf_serialized_string_t *serialized_string,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_serialized_string_compare_with_utf8_string(
     libewf_serialized_string_t *serialized_string,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

int libewf_serialized_string_compare_with_utf16_string(
     libewf_serialized_string_t *serialized_string,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SERIALIZED_STRING_H ) */

