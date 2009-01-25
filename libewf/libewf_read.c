/*
 * File reading functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <endian.h>
#include <types.h>

#include <liberror.h>

#include "libewf_definitions.h"
#include "libewf_compression.h"
#include "libewf_file.h"
#include "libewf_filename.h"
#include "libewf_file_io_pool.h"
#include "libewf_interface.h"
#include "libewf_notify.h"
#include "libewf_read.h"
#include "libewf_segment_file_handle.h"

#include "ewf_crc.h"
#include "ewf_file_header.h"

/* Processes the chunk data, applies decompression if necessary and validates the CRC
 * Sets the crc_mismatch value to 1 if the chunk CRC did not match the calculated CRC
 * Returns the amount of bytes of the processed chunk data or -1 on error
 */
ssize_t libewf_read_process_chunk_data(
         libewf_internal_handle_t *internal_handle,
         uint8_t *chunk_data,
         size_t chunk_data_size,
         uint8_t *uncompressed_chunk_data,
         size_t *uncompressed_chunk_data_size,
         int8_t is_compressed,
         ewf_crc_t chunk_crc,
         int8_t read_crc,
         uint8_t *crc_mismatch,
         liberror_error_t **error )
{
	uint8_t *chunk_data_crc  = NULL;
	static char *function    = "libewf_read_process_chunk_data";
	ewf_crc_t calculated_crc = 0;

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( crc_mismatch == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid CRC mismatch.",
		 function );

		return( -1 );
	}
	*crc_mismatch = 0;

	if( is_compressed == 0 )
	{
		if( read_crc == 0 )
		{
			chunk_data_size -= sizeof( ewf_crc_t );
			chunk_data_crc   = &( chunk_data[ chunk_data_size ] );

			endian_little_convert_32bit(
			 chunk_crc,
			 chunk_data_crc );
		}
		calculated_crc = ewf_crc_calculate(
		                  chunk_data,
		                  chunk_data_size,
		                  1 );

		if( chunk_crc != calculated_crc )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
			 function,
			 chunk_crc,
			 calculated_crc );
#endif

			*crc_mismatch = 1;
		}
		*uncompressed_chunk_data_size = chunk_data_size;
	}
	else
	{
		if( uncompressed_chunk_data == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid uncompressed chunk data.",
			 function );

			return( -1 );
		}
		if( chunk_data == uncompressed_chunk_data )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid uncompressed chunk data is the same as chunk data.",
			 function );

			return( -1 );
		}
		if( *uncompressed_chunk_data_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid uncompressed chunk data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( libewf_uncompress(
		     uncompressed_chunk_data,
		     uncompressed_chunk_data_size,
		     chunk_data,
		     chunk_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBERROR_COMPRESSION_ERROR_UNCOMPRESS_FAILED,
			 "%s: unable to uncompressed chunk data.",
			 function );

			return( -1 );
		}
	}
	return( (ssize_t) *uncompressed_chunk_data_size );
}

/* Reads a certain chunk of data from the segment file(s) to a buffer
 * Will read until the requested size is filled or the entire chunk is read
 * read_crc is set if the crc has been read into chunk_crc instead of the
 * 4 last bytes of the buffer, used for uncompressed chunks only
 * Returns the amount of bytes read, 0 if no bytes can be read or -1 on error
 */
ssize_t libewf_raw_read_chunk(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         int8_t *is_compressed,
         ewf_crc_t *chunk_crc,
         int8_t *read_crc,
         liberror_error_t **error )
{
	uint8_t stored_crc_buffer[ 4 ];

	libewf_segment_file_handle_t *segment_file_handle = NULL;
#if defined( HAVE_VERBOSE_OUTPUT )
        char *chunk_type                                  = NULL;
#endif
	static char *function                             = "libewf_raw_read_chunk";
	ssize_t chunk_read_count                          = 0;
	ssize_t crc_read_count                            = 0;
	size_t chunk_data_size                            = 0;

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->chunk_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid offset table - missing chunk offsets.",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid segment table - missing segment file handles.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( chunk_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid chunk size value is zero.",
		 function );

		return( -1 );
	}
	if( chunk_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( is_compressed == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid is compressed.",
		 function );

		return( -1 );
	}
	if( chunk_crc == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk crc.",
		 function );

		return( -1 );
	}
	if( read_crc == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read crc.",
		 function );

		return( -1 );
	}
	/* Check if the chunk is available
	 */
	if( chunk >= internal_handle->offset_table->amount_of_chunk_offsets )
	{
		return( 0 );
	}
	*chunk_crc = 0;
	*read_crc  = 0;

	/* Determine the size of the chunk including the CRC
	 */
	chunk_data_size = internal_handle->offset_table->chunk_offset[ chunk ].size;

	/* Determine if the chunk is compressed or not
	 */
	if( ( internal_handle->offset_table->chunk_offset[ chunk ].flags & LIBEWF_CHUNK_OFFSET_FLAGS_COMPRESSED ) == 0 )
	{
		if( chunk_size < chunk_data_size )
		{
			chunk_data_size -= sizeof( ewf_crc_t );
			*read_crc        = 1;
		}
		*is_compressed = 0;
	}
	else
	{
		*is_compressed = 1;
	}
	/* Determine if the chunk data fits in the buffer
	 */
	if( chunk_size < chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid chunk size too small (%" PRIzd " < %" PRIzd ").",
		 function,
		 chunk_size,
		 chunk_data_size );

		return( -1 );
	}
	segment_file_handle = internal_handle->offset_table->chunk_offset[ chunk ].segment_file_handle;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	/* Make sure the segment file offset is in the right place
	 */
	if( libewf_file_io_pool_seek_offset(
	     internal_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     internal_handle->offset_table->chunk_offset[ chunk ].file_offset,
	     SEEK_SET,
	     error ) <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek chunk in segment file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: reading chunk at offset: %" PRIjd ".\n",
	 function,
	 internal_handle->offset_table->chunk_offset[ chunk ].file_offset );
#endif

	/* Read the chunk data
	 */
	chunk_read_count = libewf_file_io_pool_read(
			    internal_handle->file_io_pool,
			    segment_file_handle->file_io_pool_entry,
			    chunk_buffer,
			    chunk_data_size,
	                    error );

	if( chunk_read_count != (ssize_t) chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk in segment file.",
		 function );

		return( -1 );
	}
	/* Determine if the CRC should be read seperately
	 */
	if( *read_crc != 0 )
	{
		crc_read_count = libewf_file_io_pool_read(
		                  internal_handle->file_io_pool,
		                  segment_file_handle->file_io_pool_entry,
				  stored_crc_buffer,
				  sizeof( ewf_crc_t ),
		                  error );

		if( crc_read_count != (ssize_t) sizeof( ewf_crc_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: error reading CRC of chunk: %" PRIu32 " from segment file.",
			 function,
			 chunk );

			return( -1 );
		}
		endian_little_convert_32bit(
		 *chunk_crc,
		 stored_crc_buffer );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( *is_compressed == 0 )
	{
		chunk_type = "UNCOMPRESSED";
	}
	else
	{
		chunk_type = "COMPRESSED";
	}
	libewf_notify_verbose_printf(
	 "%s: chunk %" PRIu32 " of %" PRIu32 " is %s and has size: %" PRIzd ".\n",
	 function,
	 chunk + 1,
	 internal_handle->offset_table->amount_of_chunk_offsets,
	 chunk_type,
	 internal_handle->offset_table->chunk_offset[ chunk ].size );
#endif
	return( chunk_read_count );
}

/* Reads a certain chunk of data from the segment file(s)
 * Will read until the requested size is filled or the entire chunk is read
 * Returns the amount of bytes read, 0 if no bytes can be read or -1 on error
 */
ssize_t libewf_read_chunk_data(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint32_t chunk_offset,
         uint8_t *buffer,
         size_t size,
         liberror_error_t **error )
{
	uint8_t *chunk_data        = NULL;
	uint8_t *chunk_read        = NULL;
	static char *function      = "libewf_read_chunk_data";
	ewf_crc_t chunk_crc        = 0;
	off64_t sector             = 0;
	ssize_t chunk_read_count   = 0;
	size_t chunk_data_size     = 0;
	size_t bytes_available     = 0;
	uint32_t amount_of_sectors = 0;
	int chunk_cache_data_used  = 0;
	uint8_t crc_mismatch       = 0;
	int8_t is_compressed       = 0;
	int8_t read_crc            = 0;

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->read == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle read.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->chunk_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid offset table - missing chunk offsets.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer == internal_handle->chunk_cache->compressed )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache compressed.",
		 function );

		return( -1 );
	}
	/* Check if the chunk is not cached
	 */
	if( ( internal_handle->chunk_cache->chunk != chunk )
	 || ( internal_handle->chunk_cache->cached == 0 ) )
	{
		/* Determine the size of the chunk including the CRC
		 */
		chunk_data_size = internal_handle->offset_table->chunk_offset[ chunk ].size;

		/* Make sure the chunk cache is large enough
		 */
		chunk_cache_data_used = (int) ( buffer == internal_handle->chunk_cache->data );

		if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: reallocating chunk data size: %" PRIzu ".\n",
			 function,
			 chunk_data_size );
#endif

			if( libewf_chunk_cache_resize(
			     internal_handle->chunk_cache,
			     chunk_data_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize chunk cache.",
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
		if( ( internal_handle->offset_table->chunk_offset[ chunk ].flags & LIBEWF_CHUNK_OFFSET_FLAGS_COMPRESSED ) == 0 )
		{
			is_compressed = 0;
		}
		else
		{
			is_compressed = 1;
		}
		chunk_data = internal_handle->chunk_cache->data;

		/* Directly read to the buffer if
		 *  the buffer isn't the chunk cache
		 *  and no data was previously copied into the chunk cache
		 *  and the buffer contains the necessary amount of bytes to fill a chunk
		 *  and the buffer is not compressed
		 */
		if( ( buffer != internal_handle->chunk_cache->data )
		 && ( chunk_offset == 0 )
		 && ( size >= (size_t) internal_handle->media_values->chunk_size )
		 && ( is_compressed == 0 ) )
		{
			chunk_data = buffer;

			/* The CRC is read seperately for uncompressed chunks
			 */
			chunk_data_size -= sizeof( ewf_crc_t );
		}
		/* Determine if the chunk data should be directly read into chunk data buffer
		 * or to use the intermediate storage for a compressed chunk
		 */
		if( is_compressed == 1 )
		{
			chunk_read = internal_handle->chunk_cache->compressed;
		}
		else
		{
			chunk_read = chunk_data;
		}
		/* Read the chunk
		 */
		chunk_read_count = libewf_raw_read_chunk(
		                    internal_handle,
		                    chunk,
		                    chunk_read,
		                    chunk_data_size,
		                    &is_compressed,
		                    &chunk_crc,
		                    &read_crc,
		                    error );

		if( chunk_read_count <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read chunk.",
			 function );

			return( -1 );
		}
		if( is_compressed != 0 )
		{
			chunk_data_size = internal_handle->media_values->chunk_size
			                + sizeof( ewf_crc_t );
		}
		if( libewf_read_process_chunk_data(
		     internal_handle,
		     chunk_read,
		     chunk_read_count,
		     chunk_data,
		     &chunk_data_size,
		     is_compressed,
		     chunk_crc,
		     read_crc,
		     &crc_mismatch,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to process chunk data.",
			 function );

			return( -1 );
		}
		if( crc_mismatch != 0 )
		{
			/* Wipe the chunk if nescessary
			 */
			if( ( internal_handle->read->wipe_on_error != 0 )
			 && ( memory_set(
			       chunk_read,
			       0,
			       size ) == NULL ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to wipe chunk data.",
				 function );

				return( -1 );
			}
			/* Add CRC error
			 */
			sector            = (off64_t) chunk * (off64_t) internal_handle->media_values->sectors_per_chunk;
			amount_of_sectors = internal_handle->media_values->sectors_per_chunk;

			if( ( sector + amount_of_sectors ) > internal_handle->media_values->amount_of_sectors )
			{
				amount_of_sectors = (uint32_t) ( (off64_t) internal_handle->media_values->amount_of_sectors - sector );
			}
			if( libewf_add_crc_error(
			     (libewf_handle_t *) internal_handle,
			     sector,
			     amount_of_sectors ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set CRC error.",
				 function );

				return( -1 );
			}
			chunk_data_size = amount_of_sectors * internal_handle->media_values->bytes_per_sector;
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
	else
	{
		chunk_data      = internal_handle->chunk_cache->data;
		chunk_data_size = internal_handle->chunk_cache->amount;
	}
	/* Determine the available amount of data within the cached chunk
	 */
	if( chunk_offset > chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: chunk offset exceeds amount of bytes available in chunk.",
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid available amount of bytes value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* If the data was read into the chunk cache copy it to the buffer
	 *  and the buffer is not the chunk cache itself
	 */
	if( ( chunk_data == internal_handle->chunk_cache->data )
	 && ( buffer != internal_handle->chunk_cache->data ) )
	{
		/* Copy the relevant data to buffer
		 */
		if( ( bytes_available > 0 )
		 && ( memory_copy(
		       buffer,
		       &( chunk_data[ chunk_offset ] ),
		       bytes_available ) == NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set chunk data in buffer.",
			 function );

			return( -1 );
		}
	}
	return( (ssize_t) bytes_available );
}

/* Prepares a buffer with chunk data after reading it according to the handle settings
 * intended for raw read
 * The buffer size cannot be larger than the chunk size
 * Returns the resulting chunk size or -1 on error
 */
#if defined( HAVE_V2_API )
ssize_t libewf_raw_read_prepare_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         void *uncompressed_buffer,
         size_t *uncompressed_buffer_size,
         int8_t is_compressed,
         uint32_t chunk_crc,
         int8_t read_crc,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_raw_read_prepare_buffer";
	ssize_t chunk_data_size                   = 0;
	uint8_t crc_mismatch                      = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( ( uncompressed_buffer == internal_handle->chunk_cache->data )
	 || ( uncompressed_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	chunk_data_size = libewf_read_process_chunk_data(
	                   internal_handle,
	                   (uint8_t *) buffer,
	                   buffer_size,
	                   (uint8_t *) uncompressed_buffer,
	                   uncompressed_buffer_size,
	                   is_compressed,
	                   (ewf_crc_t) chunk_crc,
	                   read_crc,
	                   &crc_mismatch,
	                   error );

	if( chunk_data_size <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_INPUT_FAILED,
		 "%s: unable to prepare chunk data.",
		 function );

		return( -1 );
	}
	if( crc_mismatch != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC mismatch for chunk data.",
		 function );

		return( -1 );
	}
	return( chunk_data_size );
}
#else
ssize_t libewf_raw_read_prepare_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         void *uncompressed_buffer,
         size_t *uncompressed_buffer_size,
         int8_t is_compressed,
         uint32_t chunk_crc,
         int8_t read_crc )
{
	liberror_error_t *error                     = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_raw_read_prepare_buffer";
	ssize_t chunk_data_size                   = 0;
	uint8_t crc_mismatch                      = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	if( ( uncompressed_buffer == internal_handle->chunk_cache->data )
	 || ( uncompressed_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed buffer - same as chunk cache.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	chunk_data_size = libewf_read_process_chunk_data(
	                   internal_handle,
	                   (uint8_t *) buffer,
	                   buffer_size,
	                   (uint8_t *) uncompressed_buffer,
	                   uncompressed_buffer_size,
	                   is_compressed,
	                   (ewf_crc_t) chunk_crc,
	                   read_crc,
	                   &crc_mismatch,
	                   &error );

	if( chunk_data_size <= -1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_INPUT_FAILED,
		 "%s: unable to prepare chunk data.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	if( crc_mismatch != 0 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC mismatch for chunk data.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	return( chunk_data_size );
}
#endif

/* Reads 'raw' data from the curent offset into a buffer
 * size contains the size of the buffer
 * The function sets the chunk crc, is compressed and read crc values
 * Returns the amount of bytes read or -1 on error
 */
#if defined( HAVE_V2_API )
ssize_t libewf_raw_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         int8_t *is_compressed,
         uint32_t *chunk_crc,
         int8_t *read_crc,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_raw_read_buffer";
	ssize_t read_count                        = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	read_count = libewf_raw_read_chunk(
	              internal_handle,
	              internal_handle->current_chunk,
	              buffer,
	              size,
	              is_compressed,
	              (ewf_crc_t *) chunk_crc,
	              read_crc,
	              error );

	if( read_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk data.",
		 function );
	}
	/* TODO check if this is needed on error */
	internal_handle->current_chunk += 1;

	return( read_count );
}
#else
ssize_t libewf_raw_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         int8_t *is_compressed,
         uint32_t *chunk_crc,
         int8_t *read_crc )
{
	liberror_error_t *error                     = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_raw_read_buffer";
	ssize_t read_count                        = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	read_count = libewf_raw_read_chunk(
	              internal_handle,
	              internal_handle->current_chunk,
	              buffer,
	              size,
	              is_compressed,
	              (ewf_crc_t *) chunk_crc,
	              read_crc,
	              &error );

	if( read_count <= -1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk data.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	/* TODO check if this is needed on error */
	internal_handle->current_chunk += 1;

	return( read_count );
}
#endif

/* Reads media data from the last current into a buffer
 * Returns the amount of bytes read or -1 on error
 */
#if defined( HAVE_V2_API )
ssize_t libewf_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_read_buffer";
	ssize_t chunk_read_count                  = 0;
	ssize_t total_read_count                  = 0;
	size_t chunk_data_size                    = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
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
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: reading size: %" PRIzu ".\n",
	 function,
	 size );
#endif

	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents some reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media_values->chunk_size + sizeof( ewf_crc_t );

	if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: reallocating chunk data size: %" PRIzu ".\n",
		 function,
		 chunk_data_size );
#endif

		if( libewf_chunk_cache_resize(
		     internal_handle->chunk_cache,
		     chunk_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk cache.",
			 function );

			return( -1 );
		}
	}
	while( size > 0 )
	{
		chunk_read_count = libewf_read_chunk_data(
		                    internal_handle,
		                    internal_handle->current_chunk,
		                    internal_handle->current_chunk_offset,
		                    (uint8_t *) &( (uint8_t *) buffer )[ total_read_count ],
		                    size,
		                    error );

		if( chunk_read_count <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data from chunk.",
			 function );

			return( -1 );
		}
		else if( chunk_read_count == 0 )
		{
			break;
		}
		size             -= chunk_read_count;
		total_read_count += chunk_read_count;

		internal_handle->current_chunk_offset += (uint32_t) chunk_read_count;

		if( internal_handle->current_chunk_offset == internal_handle->media_values->chunk_size )
		{
			internal_handle->current_chunk_offset = 0;
			internal_handle->current_chunk       += 1;
		}
		else if( internal_handle->current_chunk_offset > internal_handle->media_values->chunk_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid current chunk offset.",
			 function );

			return( -1 );
		}
		if( internal_handle->abort == 1 )
		{
			break;
		}
	}
	return( total_read_count );
}
#else
ssize_t libewf_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t size )
{
	liberror_error_t *error                     = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_read_buffer";
	ssize_t chunk_read_count                  = 0;
	ssize_t total_read_count                  = 0;
	size_t chunk_data_size                    = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: reading size: %" PRIzu ".\n",
	 function,
	 size );
#endif

	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents some reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media_values->chunk_size + sizeof( ewf_crc_t );

	if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: reallocating chunk data size: %" PRIzu ".\n",
		 function,
		 chunk_data_size );
#endif

		if( libewf_chunk_cache_resize(
		     internal_handle->chunk_cache,
		     chunk_data_size,
		     &error ) != 1 )
		{
			liberror_error_set(
			 &error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk cache.",
			 function );

			libewf_notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			return( -1 );
		}
	}
	while( size > 0 )
	{
		chunk_read_count = libewf_read_chunk_data(
		                    internal_handle,
		                    internal_handle->current_chunk,
		                    internal_handle->current_chunk_offset,
		                    (uint8_t *) &( (uint8_t *) buffer )[ total_read_count ],
		                    size,
		                    &error );

		if( chunk_read_count <= -1 )
		{
			liberror_error_set(
			 &error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data from chunk.",
			 function );

			libewf_notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			return( -1 );
		}
		else if( chunk_read_count == 0 )
		{
			break;
		}
		size             -= chunk_read_count;
		total_read_count += chunk_read_count;

		internal_handle->current_chunk_offset += (uint32_t) chunk_read_count;

		if( internal_handle->current_chunk_offset == internal_handle->media_values->chunk_size )
		{
			internal_handle->current_chunk_offset = 0;
			internal_handle->current_chunk       += 1;
		}
		else if( internal_handle->current_chunk_offset > internal_handle->media_values->chunk_size )
		{
			liberror_error_set(
			 &error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid current chunk offset.",
			 function );

			libewf_notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			return( -1 );
		}
		if( internal_handle->abort == 1 )
		{
			break;
		}
	}
	return( total_read_count );
}
#endif

/* Reads media data from an offset into a buffer
 * Returns the amount of bytes read or -1 on error
 */
#if defined( HAVE_V2_API )
ssize_t libewf_read_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "libewf_read_random";
	ssize_t read_count    = 0;

	if( libewf_seek_offset(
	     handle,
	     offset,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libewf_read_buffer(
	              handle,
	              buffer,
	              size,
	              error );

	if( read_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}
#else
ssize_t libewf_read_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         off64_t offset )
{
	liberror_error_t *error = NULL;
	static char *function = "libewf_read_random";
	ssize_t read_count    = 0;

	if( libewf_seek_offset(
	     handle,
	     offset ) == -1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	read_count = libewf_read_buffer(
	              handle,
	              buffer,
	              size );

	if( read_count <= -1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		libewf_notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( -1 );
	}
	return( read_count );
}
#endif

