/*
 * libewf chunk cache
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined( _LIBEWF_CHUNK_CACHE_H )
#define _LIBEWF_CHUNK_CACHE_H

#include "libewf_includes.h"

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

