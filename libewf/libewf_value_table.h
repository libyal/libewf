/*
 * Value table functions
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

#if !defined( _LIBEWF_VALUE_TABLE_H )
#define _LIBEWF_VALUE_TABLE_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_value_table_get_value_utf8_string_length(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_length,
     libcerror_error_t **error );

int libewf_value_table_get_value_copy_to_utf8_string_with_index(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     libcerror_error_t **error );

int libewf_value_table_set_value_by_index(
     libfvalue_table_t *value_table,
     int value_index,
     const uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

int libewf_value_table_set_value_by_identifier(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_size,
     const uint8_t *value_string,
     size_t value_string_size,
     libcerror_error_t **error );

int libewf_value_table_get_utf8_value_size(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_value_table_get_utf8_value(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_value_table_copy_value_from_utf8_string(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

int libewf_value_table_get_utf16_value_size(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_value_table_get_utf16_value(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_value_table_copy_value_from_utf16_string(
     libfvalue_table_t *value_table,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_VALUE_TABLE_H ) */

