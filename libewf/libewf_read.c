/*
 * libewf file reading
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

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"
#include "libewf_file.h"
#include "libewf_read.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"

#include "ewf_crc.h"
#include "ewf_file_header.h"

/* Processes the chunk data, applies decompression if necessary and validates the CRC
 * Returns the amount of bytes of the processed chunk data, or -1 on error
 */
ssize_t libewf_read_process_chunk_data( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_CHUNK *chunk_data, size_t chunk_data_size, EWF_CHUNK *uncompressed_chunk_data, size_t *uncompressed_chunk_data_size, int8_t is_compressed )
{
	static char *function  = "libewf_read_process_chunk_data";
	EWF_CRC calculated_crc = 0;
	size_t buffer_size     = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk data.\n",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk data size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( is_compressed == 0 )
	{
		buffer_size = chunk_data_size;

		if( ewf_crc_calculate( &calculated_crc, (uint8_t *) chunk_data, buffer_size, 1 ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( uncompressed_chunk_data == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid uncompressed chunk data.\n",
			 function );

			return( -1 );
		}
		if( chunk_data == uncompressed_chunk_data )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid uncompressed chunk data is the same as chunk data.\n",
			 function );

			return( -1 );
		}
		if( *uncompressed_chunk_data_size > (size_t) SSIZE_MAX )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid uncompressed chunk data size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		buffer_size = *uncompressed_chunk_data_size;

		if( ewf_chunk_uncompress( uncompressed_chunk_data, uncompressed_chunk_data_size, chunk_data, chunk_data_size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to uncompressed chunk data.\n",
			 function );

			return( -1 );
		}
	}
	return( (ssize_t) buffer_size );
}

/* Reads a certain chunk of data from the segment file(s)
 * Will read until the requested size is filled or the entire chunk is read
 * Returns the amount of bytes read, 0 if no bytes can be read, or -1 on error
 */
ssize_t libewf_read_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t raw_access, uint32_t chunk, uint32_t chunk_offset, void *buffer, size_t size, int8_t *is_compressed, EWF_CRC *chunk_crc, int8_t *read_crc )
{
	EWF_CHUNK *chunk_data     = NULL;
	EWF_CHUNK *chunk_read     = NULL;
#if defined( HAVE_VERBOSE_OUTPUT )
        char *chunk_type          = NULL;
#endif
	static char *function     = "libewf_read_chunk";
	ssize_t chunk_read_count  = 0;
	ssize_t crc_read_count    = 0;
	size_t chunk_data_size    = 0;
	size_t bytes_available    = 0;
	uint16_t segment_number   = 0;
	int chunk_cache_data_used = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle read.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing filenames.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->file_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing file offsets.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( buffer == internal_handle->chunk_cache->compressed )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer - same as chunk cache compressed.\n",
		 function );

		return( -1 );
	}
	if( is_compressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid is compressed.\n",
		 function );

		return( -1 );
	}
	if( chunk_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk crc.\n",
		 function );

		return( -1 );
	}
	if( read_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid read crc.\n",
		 function );

		return( -1 );
	}
	/* Check if the chunk is available
	 */
	if( chunk >= internal_handle->offset_table->amount )
	{
		return( 0 );
	}
	/* Check if the chunk is cached
	 */
	if( ( internal_handle->chunk_cache->chunk != chunk )
	 || ( internal_handle->chunk_cache->cached == 0 ) )
	{
		segment_number = internal_handle->offset_table->segment_number[ chunk ];

		/* Determine the size of the chunk including the CRC
		 */
		chunk_data_size = internal_handle->offset_table->size[ chunk ];

		chunk_cache_data_used = (int) ( buffer == internal_handle->chunk_cache->data );

		/* Make sure the chunk cache is large enough
		 */
		if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
		{
			LIBEWF_VERBOSE_PRINT( "%s: reallocating chunk data size: %zu.\n",
			 function, chunk_data_size );

			if( libewf_chunk_cache_realloc( internal_handle->chunk_cache, chunk_data_size ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to reallocate chunk cache.\n",
				 function );

				return( -1 );
			}
			/* Adjust chunk data buffer if necessary
			 */
			if( ( chunk_cache_data_used == 1 )
			 && ( buffer != internal_handle->chunk_cache->data ) )
			{
				buffer = internal_handle->chunk_cache->data;
			}
		}
		chunk_data = internal_handle->chunk_cache->data;

#if defined( HAVE_BUFFER_PASSTHROUGH )
		/* Determine if the chunk data should be put directly in the buffer
		 */
		if( ( buffer != internal_handle->chunk_cache->data )
		 && ( chunk_offset == 0 )
		 && ( size >= (size_t) internal_handle->media->chunk_size ) )
		{
			chunk_data = (EWF_CHUNK *) buffer;
		}
#endif
		/* Determine if the chunk data should be directly read into chunk data buffer
		 * or to use the intermediate storage for a compressed chunk
		 */
		if( internal_handle->offset_table->compressed[ chunk ] == 1 )
		{
			chunk_read = internal_handle->chunk_cache->compressed;
		}
		else
		{
			chunk_read = chunk_data;
		}
		/* If buffer passthrough is used the CRC is read seperately
		 */
		if( ( chunk_read != internal_handle->chunk_cache->compressed )
		 && ( chunk_read != internal_handle->chunk_cache->data ) )
		{
			chunk_data_size -= EWF_CRC_SIZE;
		}
		/* Make sure the file offset is in the right place
		 */
		if( libewf_segment_file_seek_chunk_offset( internal_handle, chunk ) <= -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to seek chunk.\n",
			 function );

			return( -1 );
		}
		/* Read the chunk data
		 */
		chunk_read_count = ewf_chunk_read(
		                    chunk_read,
		                    internal_handle->offset_table->file_descriptor[ chunk ],
		                    chunk_data_size );

		if( chunk_read_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read chunk.\n",
			 function );

			return( -1 );
		}
		internal_handle->segment_table->file_offset[ segment_number ] += (off64_t) chunk_read_count;

		/* Determine if the chunk is not compressed
		 */
		if( internal_handle->offset_table->compressed[ chunk ] == 0 )
		{
			/* If buffer passthrough is used the CRC needs to be read seperately
			 */
			if( ( chunk_read != internal_handle->chunk_cache->compressed )
			 && ( chunk_read != internal_handle->chunk_cache->data ) )
			{
				crc_read_count = ewf_crc_read(
				                  chunk_crc,
				                  internal_handle->offset_table->file_descriptor[ chunk ] );

				if( crc_read_count != (ssize_t) EWF_CRC_SIZE )
				{
					if( internal_handle->segment_table->filename[ segment_number ] == NULL )
					{
						LIBEWF_WARNING_PRINT( "%s: error reading CRC of chunk: %" PRIu32 " from segment file: %" PRIu16 ".\n",
						 function, chunk, segment_number );
					}
					else
					{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
						LIBEWF_WARNING_PRINT( "%s: error reading CRC of chunk: %" PRIu32 " from segment file: %" PRIu16 " (%ls).\n",
						 function, chunk, segment_number, internal_handle->segment_table->filename[ segment_number ] );
#else
						LIBEWF_WARNING_PRINT( "%s: error reading CRC of chunk: %" PRIu32 " from segment file: %" PRIu16 " (%s).\n",
						 function, chunk, segment_number, internal_handle->segment_table->filename[ segment_number ] );
#endif
					}
					return( -1 );
				}
				internal_handle->segment_table->file_offset[ segment_number ] += (off64_t) crc_read_count;
			}
			/* Otherwise convert the last 4 bytes of the chunk cache
			 */
			else
			{
				if( libewf_endian_convert_32bit( chunk_crc, &chunk_data[ chunk_data_size - EWF_CRC_SIZE ] ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
					 function );

					return( -1 );
				}
				chunk_data_size -= (uint32_t) EWF_CRC_SIZE;
			}
			chunk_type     = "UNCOMPRESSED";
			*is_compressed = 0;
			*read_crc      = 1;
		}
		/* Determine if the chunk is compressed
		 */
		else if( internal_handle->offset_table->compressed[ chunk ] == 1 )
		{
			chunk_data_size = internal_handle->media->chunk_size + EWF_CRC_SIZE;

			chunk_type     = "COMPRESSED";
			*is_compressed = 1;
			*read_crc      = 0;
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: unsupported compressed chunk value.\n",
			 function );

			return( -1 );
		}
		LIBEWF_VERBOSE_PRINT( "%s: chunk %" PRIu32 " of %" PRIu32 " is %s.\n",
		 function, ( chunk + 1 ), internal_handle->offset_table->amount, chunk_type );

		if( raw_access == 0 )
		{
			if( libewf_read_process_chunk_data(
			     internal_handle,
			     chunk_read,
			     chunk_read_count,
			     chunk_data,
			     &chunk_data_size,
			     internal_handle->offset_table->compressed[ chunk ] ) == -1 )
			{
#if defined( WIPE_ON_ERROR )
				/* The chunk data is wiped
				 */
				if( libewf_common_memset( chunk_read, 0, internal_handle->media->chunk_size ) == NULL )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to wipe chunk data.\n",
					 function );

					return( -1 );
				}
#endif
				if( libewf_internal_handle_add_crc_error_chunk( internal_handle, chunk ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to set CRC error chunk.\n",
					 function );

					return( -1 );
				}
				if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
		}
		/* Flag that the chunk was cached
		 */
		if( chunk_data == internal_handle->chunk_cache->data )
		{
			internal_handle->chunk_cache->chunk  = chunk;
			internal_handle->chunk_cache->amount = chunk_data_size;
			internal_handle->chunk_cache->offset = 0;
			internal_handle->chunk_cache->cached = 1;
		}
	}
	else if( ( raw_access != 0 )
	 && ( internal_handle->offset_table->compressed[ chunk ] == 1 ) )
	{
		chunk_data      = internal_handle->chunk_cache->compressed;
		chunk_data_size = internal_handle->offset_table->size[ chunk ];
		*is_compressed  = 1;
		*chunk_crc      = 0;
		*read_crc       = 0;
	}
	else
	{
		chunk_data      = internal_handle->chunk_cache->data;
		chunk_data_size = internal_handle->chunk_cache->amount;
		*is_compressed  = 0;
		*read_crc       = 1;

		if( libewf_endian_convert_32bit( chunk_crc, &chunk_data[ chunk_data_size ] ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
			 function );

			return( -1 );
		}
	}
	/* Determine the available amount of data within the cached chunk
	 */
	if( chunk_data_size < chunk_offset )
	{
		LIBEWF_WARNING_PRINT( "%s: chunk offset exceeds amount of bytes available in chunk.\n",
		 function );

		return( -1 );
	}
	bytes_available = chunk_data_size - chunk_offset;

	/* Correct the available amount of bytes is larger than the requested amount of bytes
	 */
	if( bytes_available > size )
	{
		bytes_available = size;
	}
	if( bytes_available > (size_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid available amount of bytes value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* If the data was read into the chunk cache copy it to the buffer
	 */
	if( ( chunk_data == internal_handle->chunk_cache->compressed )
	 || ( chunk_data == internal_handle->chunk_cache->data ) )
	{
		/* Copy the relevant data to buffer
		 */
		if( ( bytes_available > 0 )
		 && ( libewf_common_memcpy( buffer, &chunk_data[ chunk_offset ], bytes_available ) == NULL ) )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set chunk data in buffer.\n",
			 function );

			return( -1 );
		}
	}
	return( (ssize_t) bytes_available );
}

/* Reads certain chunk data from the segment file(s)
 * Will read until the requested size is filled or the entire chunk is read
 * Returns the amount of bytes read, 0 if no bytes can be read, or -1 on error
 */
ssize_t libewf_read_chunk_data( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t raw_access, void *buffer, size_t size, int8_t *is_compressed, uint32_t *chunk_crc, int8_t *read_crc )
{
	static char *function    = "libewf_read_chunk_data";
	ssize_t chunk_read_count = 0;
	ssize_t total_read_count = 0;
	size_t chunk_data_size   = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer - same as chunk cache.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( is_compressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid is compressed.\n",
		 function );

		return( -1 );
	}
	if( chunk_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk crc.\n",
		 function );

		return( -1 );
	}
	if( read_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid read crc.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: reading size: %zu.\n",
	 function, size );

	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents multiple reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media->chunk_size + EWF_CRC_SIZE;

	if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
	{
		LIBEWF_VERBOSE_PRINT( "%s: reallocating chunk data size: %zu.\n",
		 function, chunk_data_size );

		if( libewf_chunk_cache_realloc( internal_handle->chunk_cache, chunk_data_size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate chunk cache.\n",
			 function );

			return( -1 );
		}
	}
	while( size > 0 )
	{
		chunk_read_count = libewf_read_chunk(
		                    internal_handle,
		                    raw_access,
		                    internal_handle->current_chunk,
		                    internal_handle->current_chunk_offset,
		                    (void *) &( (uint8_t *) buffer )[ total_read_count ],
		                    size,
		                    is_compressed,
		                    chunk_crc,
		                    read_crc );

		if( chunk_read_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read data from chunk.\n",
			 function );

			return( -1 );
		}
		else if( chunk_read_count == 0 )
		{
			break;
		}
		size                                  -= chunk_read_count;
		total_read_count                      += chunk_read_count;
		internal_handle->current_chunk_offset += (uint32_t) chunk_read_count;

		if( internal_handle->current_chunk_offset == internal_handle->media->chunk_size )
		{
			internal_handle->current_chunk_offset = 0;
			internal_handle->current_chunk       += 1;
		}
		else if( internal_handle->current_chunk_offset > internal_handle->media->chunk_size )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid current chunk offset.\n",
			 function );

			return( -1 );
		}
	}
	return( total_read_count );
}

/* Prepares a buffer with chunk data after reading it according to the handle settings
 * intended for raw read
 * The buffer size cannot be larger than the chunk size
 * Returns the resulting chunk size, or -1 on error
 */
ssize_t libewf_raw_read_prepare_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t buffer_size, void *uncompressed_buffer, size_t *uncompressed_buffer_size, int8_t is_compressed )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_raw_read_prepare_buffer";
	ssize_t chunk_data_size                 = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer - same as chunk cache.\n",
		 function );

		return( -1 );
	}
	if( ( uncompressed_buffer == internal_handle->chunk_cache->data )
	 || ( uncompressed_buffer == internal_handle->chunk_cache->compressed ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid uncompressed buffer - same as chunk cache.\n",
		 function );

		return( -1 );
	}
	chunk_data_size = libewf_read_process_chunk_data(
	                   internal_handle,
	                   (EWF_CHUNK *) buffer,
	                   buffer_size,
	                   (EWF_CHUNK *) uncompressed_buffer,
	                   uncompressed_buffer_size,
	                   is_compressed );

	if( chunk_data_size <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to prepare chunk data.\n",
		 function );

		return( -1 );
	}
	return( chunk_data_size );
}

/* Reads 'raw' data from the curent offset into a buffer
 * size contains the size of the buffer
 * The function sets the chunk crc, is compressed and read crc values
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_raw_read_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size, int8_t *is_compressed, uint32_t *chunk_crc, int8_t *read_crc )
{
	static char *function = "libewf_raw_read_buffer";
	ssize_t read_count    = 0;

	read_count = libewf_read_chunk_data(
	              (LIBEWF_INTERNAL_HANDLE *) handle,
	              1,
	              buffer,
	              size,
	              is_compressed,
	              (EWF_CRC *) chunk_crc,
	              read_crc );

	if( read_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read chunk data.\n",
		 function );
	}
	return( read_count );
}

/* Reads media data from the last current into a buffer
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_read_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size )
{
	static char *function = "libewf_read_buffer";
	EWF_CRC chunk_crc     = 0;
	ssize_t read_count    = 0;
	int8_t is_compressed  = 0;
	int8_t read_crc       = 0;

	read_count = libewf_read_chunk_data(
	              (LIBEWF_INTERNAL_HANDLE *) handle,
	              0,
	              buffer,
	              size,
	              &is_compressed,
	              &chunk_crc,
	              &read_crc );

	if( read_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read chunk data.\n",
		 function );
	}
	return( read_count );
}

/* Reads media data from an offset into a buffer
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_read_random( LIBEWF_HANDLE *handle, void *buffer, size_t size, off64_t offset )
{
	static char *function = "libewf_read_random";
	ssize_t read_count    = 0;

	if( libewf_seek_offset( handle, offset ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to seek offset.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_read_buffer( handle, buffer, size );

	if( read_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read buffer.\n",
		 function );

		return( -1 );
	}
	return( read_count );
}

