/*
 * libewf chunk_cache
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#include "libewf_includes.h"
#include "libewf_chunk_cache.h"
#include "libewf_common.h"
#include "libewf_notify.h"

/* Allocates memory for the chunk cache cache
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHUNK_CACHE *libewf_chunk_cache_alloc( size_t size )
{
	LIBEWF_CHUNK_CACHE *chunk_cache = NULL;

	chunk_cache = (LIBEWF_CHUNK_CACHE *) libewf_common_alloc( LIBEWF_CHUNK_CACHE_SIZE );

	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_alloc: unable to allocate chunk cache.\n" );

		return( NULL );
	}
	size *= EWF_CHUNK_SIZE;

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_alloc: invalid size value exceeds maximum.\n" );

		return( NULL );
	}
	chunk_cache->compressed = (EWF_CHUNK *) libewf_common_alloc( size );

	if( chunk_cache->compressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_alloc: unable to create chunk (compressed).\n" );

		libewf_common_free( chunk_cache );

		return( NULL );
	}
	chunk_cache->data = (EWF_CHUNK *) libewf_common_alloc( size );

	if( chunk_cache->data == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_alloc: unable to create chunk (data).\n" );

		libewf_common_free( chunk_cache->compressed );
		libewf_common_free( chunk_cache );

		return( NULL );
	}
	chunk_cache->allocated_size = size;
	chunk_cache->chunk          = 0;
	chunk_cache->amount         = 0;
	chunk_cache->offset         = 0;
	chunk_cache->cached         = 0;

	return( chunk_cache );
}

/* Reallocates and wipes memory for the chunk cache cache
 * Returns a pointer to the instance, NULL on error
 */
LIBEWF_CHUNK_CACHE *libewf_chunk_cache_realloc( LIBEWF_CHUNK_CACHE *chunk_cache, size_t size )
{
	EWF_CHUNK *reallocation = NULL;

	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: invalid chunk cache.\n" );

		return( NULL );
	}
	size *= EWF_CHUNK_SIZE;

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: invalid size value exceeds maximum.\n" );

		return( NULL );
	}
	if( size <= chunk_cache->allocated_size )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: new size must be greater than previous size.\n" );

		return( NULL );
	}
	reallocation = (EWF_CHAR *) libewf_common_realloc( (void *) chunk_cache->compressed, size );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: unable to realloc chunk (compressed).\n" );

		return( NULL );
	}
	chunk_cache->compressed = reallocation;
	reallocation            = (EWF_CHAR *) libewf_common_realloc( (void *) chunk_cache->data, size );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: unable to realloc chunk (data).\n" );

		return( NULL );
	}
	chunk_cache->data           = reallocation;
	chunk_cache->allocated_size = size;
	chunk_cache->chunk          = 0;
	chunk_cache->amount         = 0;
	chunk_cache->offset         = 0;
	chunk_cache->cached         = 0;

	return( chunk_cache );
}

/* Frees memory of a chunk cache struct including elements
 */
void libewf_chunk_cache_free( LIBEWF_CHUNK_CACHE *chunk_cache )
{
	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_free: invalid chunk cache.\n" );

		return;
	}
	libewf_common_free( chunk_cache->compressed );
	libewf_common_free( chunk_cache->data );
	libewf_common_free( chunk_cache );
}

