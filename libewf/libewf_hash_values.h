/*
 * Hash values functions
 *
 * Copyright (C) 2006-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEWF_HASH_VALUES_H )
#define _LIBEWF_HASH_VALUES_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_hash_values_initialize(
     libfvalue_table_t **hash_values,
     libcerror_error_t **error );

int libewf_hash_values_parse_md5_hash(
     libfvalue_table_t *hash_values,
     const uint8_t *md5_hash,
     size_t md5_hash_size,
     libcerror_error_t **error );

int libewf_hash_values_parse_sha1_hash(
     libfvalue_table_t *hash_values,
     const uint8_t *sha1_hash,
     size_t sha1_hash_size,
     libcerror_error_t **error );

int libewf_hash_values_parse_xhash(
     libfvalue_table_t *hash_values,
     const uint8_t *xhash,
     size_t xhash_size,
     libcerror_error_t **error );

int libewf_hash_values_generate_xhash(
     libfvalue_table_t *hash_values,
     uint8_t **xhash,
     size_t *xhash_size,
     libcerror_error_t **error );

int libewf_hash_values_generate_md5_hash(
     libfvalue_table_t *hash_values,
     uint8_t *md5_hash,
     size_t md5_hash_size,
     uint8_t *md5_hash_set,
     libcerror_error_t **error );

int libewf_hash_values_generate_sha1_hash(
     libfvalue_table_t *hash_values,
     uint8_t *sha1_hash,
     size_t sha1_hash_size,
     uint8_t *sha1_hash_set,
     libcerror_error_t **error );

int libewf_hash_values_get_identifier_size(
     libfvalue_table_t *hash_values,
     uint32_t index,
     size_t *identifier_size,
     libcerror_error_t **error );

int libewf_hash_values_get_identifier(
     libfvalue_table_t *hash_values,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

int libewf_hash_values_get_utf8_value_size(
     libfvalue_table_t *hash_values,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_hash_values_get_utf8_value(
     libfvalue_table_t *hash_values,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_hash_values_set_utf8_value(
     libfvalue_table_t *hash_values,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

int libewf_hash_values_get_utf16_value_size(
     libfvalue_table_t *hash_values,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_hash_values_get_utf16_value(
     libfvalue_table_t *hash_values,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_hash_values_set_utf16_value(
     libfvalue_table_t *hash_values,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_HASH_VALUES_H ) */

