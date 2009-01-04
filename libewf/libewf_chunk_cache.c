/*
 * libewf chunk_cache
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

#include "libewf_includes.h"
#include "libewf_chunk_cache.h"
#include "libewf_common.h"
#include "libewf_notify.h"

/* Allocates memory for the chunk cache cache
 * Returns a pointer to the new instance, NULL on error
 */
libewf_chunk_cache_t *libewf_chunk_cache_alloc( size_t size )
{
	libewf_chunk_cache_t *chunk_cache = NULL;
	static char *function             = "libewf_chunk_cache_alloc";

	chunk_cache = (libewf_chunk_cache_t *) libewf_common_alloc(
	                                        sizeof( libewf_chunk_cache_t ) );

	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate chunk cache.\n",
		 function );

		return( NULL );
	}
	size *= sizeof( ewf_char_t );

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	chunk_cache->compressed = (ewf_char_t *) libewf_common_alloc( size );

	if( chunk_cache->compressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create chunk cache compressed.\n",
		 function );

		libewf_common_free( chunk_cache );

		return( NULL );
	}
	chunk_cache->data = (ewf_char_t *) libewf_common_alloc( size );

	if( chunk_cache->data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create chunk cache data.\n",
		 function );

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
 * Returns 1 if successful, or -1 on error
 */
int libewf_chunk_cache_realloc( libewf_chunk_cache_t *chunk_cache, size_t size )
{
	ewf_char_t *reallocation = NULL;
	static char *function    = "libewf_chunk_cache_realloc";

	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk cache.\n",
		 function );

		return( -1 );
	}
	size *= sizeof( ewf_char_t );

	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( size <= chunk_cache->allocated_size )
	{
		LIBEWF_WARNING_PRINT( "%s: new size must be greater than previous size.\n",
		 function );

		return( -1 );
	}
	reallocation = (ewf_char_t *) libewf_common_realloc( (void *) chunk_cache->compressed, size );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to realloc chunk cache compressed.\n",
		 function );

		return( -1 );
	}
	chunk_cache->compressed = reallocation;
	reallocation            = (ewf_char_t *) libewf_common_realloc( (void *) chunk_cache->data, size );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to realloc chunk cache data.\n",
		 function );

		return( -1 );
	}
	chunk_cache->data           = reallocation;
	chunk_cache->allocated_size = size;
	chunk_cache->chunk          = 0;
	chunk_cache->amount         = 0;
	chunk_cache->offset         = 0;
	chunk_cache->cached         = 0;

	return( 1 );
}

/* Frees memory of a chunk cache struct including elements
 */
void libewf_chunk_cache_free( libewf_chunk_cache_t *chunk_cache )
{
	static char *function = "libewf_chunk_cache_free";

	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk cache.\n",
		 function );

		return;
	}
	libewf_common_free( chunk_cache->compressed );
	libewf_common_free( chunk_cache->data );
	libewf_common_free( chunk_cache );
}

