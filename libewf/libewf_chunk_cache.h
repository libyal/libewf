/*
 * Chunk cache functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#include "ewf_char.h"

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
	ewf_char_t *compressed;

	/* The data buffer
	 */
	ewf_char_t *data;
};

libewf_chunk_cache_t *libewf_chunk_cache_alloc( size_t size );
int libewf_chunk_cache_realloc( libewf_chunk_cache_t *chunk_cache, size_t size );
void libewf_chunk_cache_free( libewf_chunk_cache_t *chunk_cache );

#if defined( __cplusplus )
}
#endif

#endif

