/*
 * Bit-stream functions
 *
 * Copyright (C) 2006-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_BIT_STREAM_H )
#define _LIBEWF_BIT_STREAM_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The storage type
 */
enum LIBEWF_BIT_STREAM_STORAGE_TYPES
{
	LIBEWF_BIT_STREAM_STORAGE_TYPE_UNKNOWN		= 0x00,
	LIBEWF_BIT_STREAM_STORAGE_TYPE_BYTE_FRONT_TO_BACK	= 0x01,
	LIBEWF_BIT_STREAM_STORAGE_TYPE_BYTE_BACK_TO_FRONT	= 0x02
};

typedef struct libewf_bit_stream libewf_bit_stream_t;

struct libewf_bit_stream
{
	/* The byte stream
	 */
	const uint8_t *byte_stream;

	/* The byte stream size
	 */
	size_t byte_stream_size;

	/* The byte stream offset
	 */
	size_t byte_stream_offset;

	/* The storage type
	 */
	uint8_t storage_type;

	/* The bit buffer
	 */
	uint32_t bit_buffer;

	/* The number of bits remaining in the bit buffer
	 */
	uint8_t bit_buffer_size;
};

int libewf_bit_stream_initialize(
     libewf_bit_stream_t **bit_stream,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     size_t byte_stream_offset,
     uint8_t storage_type,
     libcerror_error_t **error );

int libewf_bit_stream_free(
     libewf_bit_stream_t **bit_stream,
     libcerror_error_t **error );

int libewf_bit_stream_get_value(
     libewf_bit_stream_t *bit_stream,
     uint8_t number_of_bits,
     uint32_t *value_32bit,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_BIT_STREAM_H ) */

