/*
 * The value reader functions
 *
 * Copyright (C) 2006-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_VALUE_READER_H )
#define _LIBEWF_VALUE_READER_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_serialized_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_value_reader libewf_value_reader_t;

struct libewf_value_reader
{
	/* The line offset
	 */
	off64_t line_offset;

	/* The buffer
	 */
	const uint8_t *buffer;

	/* The buffer size
	 */
	size_t buffer_size;

	/* The buffer offset
	 */
	size_t buffer_offset;

	/* The value index
	 */
	int value_index;

/* TODO remove after refactor */
	/* The data type
	 */
	int data_type;

	/* The value data
	 */
	uint8_t *value_data;

	/* The value data size
	 */
	size_t value_data_size;
};

int libewf_value_reader_initialize(
     libewf_value_reader_t **value_reader,
     libcerror_error_t **error );

int libewf_value_reader_free(
     libewf_value_reader_t **value_reader,
     libcerror_error_t **error );

int libewf_value_reader_set_buffer(
     libewf_value_reader_t *value_reader,
     off64_t line_offset,
     const uint8_t *buffer,
     size_t buffer_size,
     int data_type,
     libcerror_error_t **error );

int libewf_value_reader_read_data(
     libewf_value_reader_t *value_reader,
     const uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

int libewf_value_reader_read_byte_stream_base16(
     libewf_value_reader_t *value_reader,
     uint8_t **byte_stream,
     size_t *byte_stream_size,
     libcerror_error_t **error );

int libewf_value_reader_read_integer_signed(
     libewf_value_reader_t *value_reader,
     int64_t *integer_value,
     libcerror_error_t **error );

int libewf_value_reader_read_integer_unsigned(
     libewf_value_reader_t *value_reader,
     uint64_t *integer_value,
     libcerror_error_t **error );

int libewf_value_reader_read_serialized_string(
     libewf_value_reader_t *value_reader,
     libewf_serialized_string_t *serialized_string,
     libcerror_error_t **error );

int libewf_value_reader_read_serialized_string_base16(
     libewf_value_reader_t *value_reader,
     libewf_serialized_string_t *serialized_string,
     libcerror_error_t **error );

int libewf_value_reader_read_utf8_string(
     libewf_value_reader_t *value_reader,
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_VALUE_READER_H ) */

