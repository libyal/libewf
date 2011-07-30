/*
 * Chunk data functions
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBEWF_CHUNK_DATA_H )
#define _LIBEWF_CHUNK_DATA_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_chunk_data libewf_chunk_data_t;

struct libewf_chunk_data
{
	/* The allocated data size
	 */
	size_t allocated_data_size;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The compressed data
	 */
	uint8_t *compressed_data;

	/* The compressed data size
	 */
	size_t compressed_data_size;

	/* Value to indicate the chunk is compressed
	 */
	uint8_t is_compressed;

	/* Value to indicate the chunk is packed
	 */
	uint8_t is_packed;

	/* Value to indicate the chunk is corrupt
	 */
	uint8_t is_corrupt;
};

int libewf_chunk_data_initialize(
     libewf_chunk_data_t **chunk_data,
     size_t data_size,
     liberror_error_t **error );

int libewf_chunk_data_free(
     intptr_t *chunk_data,
     liberror_error_t **error );

int libewf_chunk_data_pack(
     libewf_chunk_data_t *chunk_data,
     int8_t compression_level,
     uint8_t compression_flags,
     uint8_t ewf_format,
     size32_t chunk_size,
     const uint8_t *compressed_zero_byte_empty_block,
     size_t compressed_zero_byte_empty_block_size,
     liberror_error_t **error );

int libewf_chunk_data_unpack(
     libewf_chunk_data_t *chunk_data,
     size_t chunk_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

