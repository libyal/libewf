/*
 * libewf chunk_cache
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <stdlib.h>

#include "libewf_chunk_cache.h"
#include "libewf_common.h"
#include "libewf_notify.h"

/* Allocates memory for the chunk_cache cache
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHUNK_CACHE *libewf_chunk_cache_alloc( uint32_t size )
{
	LIBEWF_CHUNK_CACHE *chunk_cache = NULL;

	chunk_cache = (LIBEWF_CHUNK_CACHE *) libewf_alloc( LIBEWF_CHUNK_CACHE_SIZE );

	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_alloc: unable to allocate chunk cache.\n" );

		return( NULL );
	}
	chunk_cache->read = ewf_sectors_chunk_alloc( size );

	if( chunk_cache->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_alloc: unable to create sectors chunk (read).\n" );

		libewf_free( chunk_cache );

		return( NULL );
	}
	chunk_cache->data = ewf_sectors_chunk_alloc( size );

	if( chunk_cache->data == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_alloc: unable to create sectors chunk (data).\n" );

		ewf_sectors_chunk_free( chunk_cache->read );
		libewf_free( chunk_cache );

		return( NULL );
	}
	chunk_cache->amount         = 0;
	chunk_cache->identifier     = -1;
	chunk_cache->allocated_size = size;

	return( chunk_cache );
}

/* Reallocates and wipes memory for the chunk_cache cache
 * Returns a pointer to the instance, NULL on error
 */
LIBEWF_CHUNK_CACHE *libewf_chunk_cache_realloc( LIBEWF_CHUNK_CACHE *chunk_cache, uint32_t size )
{
	EWF_SECTORS_CHUNK *reallocation = NULL;

	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: invalid chunk cache.\n" );

		return( NULL );
	}
	reallocation = ewf_sectors_chunk_realloc( chunk_cache->read, chunk_cache->allocated_size, size );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: unable to realloc sectors chunk (read).\n" );

		return( NULL );
	}
	chunk_cache->read = reallocation;
	reallocation      = ewf_sectors_chunk_realloc( chunk_cache->data, chunk_cache->allocated_size, size );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: unable to realloc sectors chunk (data).\n" );

		return( NULL );
	}
	chunk_cache->data           = reallocation;
	chunk_cache->amount         = 0;
	chunk_cache->identifier     = -1;
	chunk_cache->allocated_size = size;

	return( chunk_cache );
}

/* Frees memory of a chunk_cache struct including elements
 */
void libewf_chunk_cache_free( LIBEWF_CHUNK_CACHE *chunk_cache )
{
	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_free: invalid chunk cache.\n" );

		return;
	}
	if( chunk_cache->read != NULL )
	{
		libewf_free( chunk_cache->read );
	}
	if( chunk_cache->data != NULL )
	{
		libewf_free( chunk_cache->data );
	}
	libewf_free( chunk_cache );
}

/* Wipes memory for the chunk_cache cache
 * Returns a pointer to the instance, NULL on error
 */
LIBEWF_CHUNK_CACHE *libewf_chunk_cache_wipe( LIBEWF_CHUNK_CACHE *chunk_cache )
{
	EWF_SECTORS_CHUNK *ewf_sectors = NULL;

	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_wipe: invalid chunk cache.\n" );

		return( NULL );
	}
	ewf_sectors = ewf_sectors_chunk_wipe( chunk_cache->read, chunk_cache->allocated_size );

	if( ewf_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: unable to wipe sectors chunk (read).\n" );

		return( NULL );
	}
	chunk_cache->read = ewf_sectors;
	ewf_sectors       = ewf_sectors_chunk_wipe( chunk_cache->data, chunk_cache->allocated_size );

	if( ewf_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_chunk_cache_realloc: unable to wipe sectors chunk (data).\n" );

		return( NULL );
	}
	chunk_cache->data       = ewf_sectors;
	chunk_cache->amount     = 0;
	chunk_cache->identifier = -1;

	return( chunk_cache );
}

