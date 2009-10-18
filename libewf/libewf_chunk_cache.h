/*
 * Chunk cache functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBEWF_CHUNK_CACHE_H )
#define _LIBEWF_CHUNK_CACHE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_chunk_cache libewf_chunk_cache_t;

struct libewf_chunk_cache
{
	/* The allocated size of the cached chunk
	 */
	size_t allocated_size;

	/* The identifier of the cached chunk
	 */
	uint32_t chunk;

	/* The amount of data in the chunk
	 */
	size_t amount;

	/* The offset for data in the chunk
	 */
	size_t offset;

	/* A value to indicate if the chunk is cached
	 */
	uint8_t cached;

	/* The compressed data buffer
	 */
	uint8_t *compressed;

	/* The data buffer
	 */
	uint8_t *data;

	/* The zero byte empty block compressed data buffer
	 */
	uint8_t *zero_byte_empty_block_compressed;

	/* The size of the compressed zero byte empty block data
	 */
	size_t zero_byte_empty_block_compressed_size;

	/* The size of the uncompress zero byte empty block data
	 */
	size_t zero_byte_empty_block_uncompressed_size;
};

int libewf_chunk_cache_initialize(
     libewf_chunk_cache_t **chunk_cache,
     size_t size,
     liberror_error_t **error );

int libewf_chunk_cache_free(
     libewf_chunk_cache_t **chunk_cache,
     liberror_error_t **error );

int libewf_chunk_cache_resize(
     libewf_chunk_cache_t *chunk_cache,
     size_t size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

