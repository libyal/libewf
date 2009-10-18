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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libewf_chunk_cache.h"

/* Initialize the chunk cache
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_cache_initialize(
     libewf_chunk_cache_t **chunk_cache,
     size_t size,
     liberror_error_t **error )
{
	static char *function = "libewf_chunk_cache_initialize";

	if( chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk cache.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( *chunk_cache == NULL )
	{
		*chunk_cache = (libewf_chunk_cache_t *) memory_allocate(
		                                         sizeof( libewf_chunk_cache_t ) );

		if( *chunk_cache == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create chunk cache.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *chunk_cache,
		     0,
		     sizeof( libewf_chunk_cache_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear chunk cache.",
			 function );

			memory_free(
			 *chunk_cache );

			*chunk_cache = NULL;

			return( -1 );
		}
		( *chunk_cache )->compressed = (uint8_t *) memory_allocate(
		                                            sizeof( uint8_t ) * size );

		if( ( *chunk_cache )->compressed == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create chunk cache compressed.",
			 function );

			memory_free(
			 *chunk_cache );

			*chunk_cache = NULL;

			return( -1 );
		}
		( *chunk_cache )->data = (uint8_t *) memory_allocate(
		                                      sizeof( uint8_t ) * size );

		if( ( *chunk_cache )->data == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create chunk cache data.",
			 function );

			memory_free(
			 ( *chunk_cache )->compressed );
			memory_free(
			 *chunk_cache );

			*chunk_cache = NULL;

			return( -1 );
		}
		( *chunk_cache )->allocated_size = size;
	}
	return( 1 );
}

/* Frees the chunk cache including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_cache_free(
     libewf_chunk_cache_t **chunk_cache,
     liberror_error_t **error )
{
	static char *function = "libewf_chunk_cache_free";

	if( chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk cache.",
		 function );

		return( -1 );
	}
	if( *chunk_cache != NULL )
	{
		if( ( *chunk_cache )->compressed != NULL )
		{
			memory_free(
			 ( *chunk_cache )->compressed );
		}
		if( ( *chunk_cache )->data != NULL )
		{
			memory_free(
			 ( *chunk_cache )->data );
		}
		if( ( *chunk_cache )->zero_byte_empty_block_compressed != NULL )
		{
			memory_free(
			 ( *chunk_cache )->zero_byte_empty_block_compressed );
		}
		memory_free(
		 *chunk_cache );

		*chunk_cache = NULL;
	}
	return( 1 );
}

/* Resizes the chunk cache
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_cache_resize(
     libewf_chunk_cache_t *chunk_cache,
     size_t size,
     liberror_error_t **error )
{
	static char *function = "libewf_chunk_cache_resize";
	void *reallocation    = NULL;

	if( chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk cache.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( size > chunk_cache->allocated_size )
	{
		reallocation = memory_reallocate(
				chunk_cache->compressed,
				sizeof( uint8_t ) * size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize chunk cache compressed.",
			 function );

			return( -1 );
		}
		chunk_cache->compressed = (uint8_t *) reallocation;
		reallocation            = memory_reallocate(
					   chunk_cache->data,
					   sizeof( uint8_t ) * size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize chunk cache data.",
			 function );

			return( -1 );
		}
		chunk_cache->data           = (uint8_t *) reallocation;
		chunk_cache->allocated_size = size;
		chunk_cache->chunk          = 0;
		chunk_cache->amount         = 0;
		chunk_cache->offset         = 0;
		chunk_cache->cached         = 0;
	}
	return( 1 );
}

