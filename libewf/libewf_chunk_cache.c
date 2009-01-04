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

#include <common.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#include "libewf_chunk_cache.h"

/* Allocates memory for the chunk cache cache
 * Returns a pointer to the new instance, NULL on error
 */
libewf_chunk_cache_t *libewf_chunk_cache_alloc(
                       size_t size )
{
	libewf_chunk_cache_t *chunk_cache = NULL;
	static char *function             = "libewf_chunk_cache_alloc";

	chunk_cache = (libewf_chunk_cache_t *) memory_allocate(
	                                        sizeof( libewf_chunk_cache_t ) );

	if( chunk_cache == NULL )
	{
		notify_warning_printf( "%s: unable to allocate chunk cache.\n",
		 function );

		return( NULL );
	}
	size *= sizeof( ewf_char_t );

	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	chunk_cache->compressed = (ewf_char_t *) memory_allocate(
	                                          size );

	if( chunk_cache->compressed == NULL )
	{
		notify_warning_printf( "%s: unable to create chunk cache compressed.\n",
		 function );

		memory_free(
		 chunk_cache );

		return( NULL );
	}
	chunk_cache->data = (ewf_char_t *) memory_allocate(
	                                    size );

	if( chunk_cache->data == NULL )
	{
		notify_warning_printf( "%s: unable to create chunk cache data.\n",
		 function );

		memory_free(
		 chunk_cache->compressed );
		memory_free(
		 chunk_cache );

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
int libewf_chunk_cache_realloc(
     libewf_chunk_cache_t *chunk_cache,
     size_t size )
{
	ewf_char_t *reallocation = NULL;
	static char *function    = "libewf_chunk_cache_realloc";

	if( chunk_cache == NULL )
	{
		notify_warning_printf( "%s: invalid chunk cache.\n",
		 function );

		return( -1 );
	}
	size *= sizeof( ewf_char_t );

	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( size <= chunk_cache->allocated_size )
	{
		notify_warning_printf( "%s: new size must be greater than previous size.\n",
		 function );

		return( -1 );
	}
	reallocation = (ewf_char_t *) memory_reallocate(
	                               (void *) chunk_cache->compressed,
	                                size );

	if( reallocation == NULL )
	{
		notify_warning_printf( "%s: unable to realloc chunk cache compressed.\n",
		 function );

		return( -1 );
	}
	chunk_cache->compressed = reallocation;
	reallocation            = (ewf_char_t *) memory_reallocate(
	                                          (void *) chunk_cache->data,
	                                          size );

	if( reallocation == NULL )
	{
		notify_warning_printf( "%s: unable to realloc chunk cache data.\n",
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
void libewf_chunk_cache_free(
      libewf_chunk_cache_t *chunk_cache )
{
	static char *function = "libewf_chunk_cache_free";

	if( chunk_cache == NULL )
	{
		notify_warning_printf( "%s: invalid chunk cache.\n",
		 function );

		return;
	}
	memory_free(
	 chunk_cache->compressed );
	memory_free(
	 chunk_cache->data );
	memory_free(
	 chunk_cache );
}

