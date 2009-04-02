/*
 * File writing functions
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libewf_definitions.h"
#include "libewf_chunk_cache.h"
#include "libewf_compression.h"
#include "libewf_handle.h"
#include "libewf_hash_values.h"
#include "libewf_header_values.h"
#include "libewf_libbfio.h"
#include "libewf_list_type.h"
#include "libewf_notify.h"
#include "libewf_offset_table.h"
#include "libewf_read_io_handle.h"
#include "libewf_section.h"
#include "libewf_section_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_file_handle.h"
#include "libewf_segment_table.h"
#include "libewf_write.h"
#include "libewf_write_io_handle.h"

#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_file_header.h"
#include "ewf_section.h"
#include "ewfx_delta_chunk.h"

/* Writes a new chunk of data in EWF format from a buffer at the current offset
 * The necessary settings of the write values must have been made
 * Returns the amount of data bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_chunk_data_new(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         int8_t force_write,
         liberror_error_t **error )
{
	uint8_t *chunk_data               = NULL;
	static char *function             = "libewf_write_chunk_data_new";
	ewf_crc_t chunk_crc               = 0;
	ssize_t chunk_data_size           = 0;
	ssize_t write_count               = 0;
	size_t write_size                 = 0;
	size_t read_size                  = 0;
	size_t remaining_chunk_size       = 0;
	size_t compressed_chunk_data_size = 0;
	int chunk_cache_data_used         = 0;
	int8_t is_compressed              = 0;
	int8_t write_crc                  = 0;

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
	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.",
		 function );

		return( -1 );
	}
	/* Check if the write was already finalized
	 */
	if( internal_handle->write_io_handle->write_finalized == 1 )
	{
		return( 0 );
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
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: writing buffer of size: %" PRIzu " with data of size: %" PRIzd ".\n",
	 function,
	 buffer_size,
	 data_size );
#endif

	/* Determine the size of data to read
	 */
	if( data_size < (size_t) internal_handle->media_values->chunk_size )
	{
		read_size = (size_t) data_size;
	}
	else
	{
		read_size = internal_handle->media_values->chunk_size;
	}
	if( read_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid read size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Directy write the buffer if
	 *  the chunk cache data is directly being passed (for finalize)
	 *  or no data was previously copied into the chunk cache
	 *   and the buffer contains the necessary amount of bytes to fill a chunk
	 */
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( ( internal_handle->chunk_cache->offset == 0 )
	 && ( data_size >= (size_t) internal_handle->media_values->chunk_size ) ) )
	{
		chunk_data = (uint8_t *) buffer;
		write_size = read_size;
	}
	else
	{
		/* Check if data is present in the chunk cache
		 * and calculate the amount of data to read from the buffer
		 */
		remaining_chunk_size = internal_handle->media_values->chunk_size
		                     - internal_handle->chunk_cache->offset;

		if( read_size > (size_t) remaining_chunk_size )
		{
			read_size = remaining_chunk_size;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: reading %" PRIzu " bytes from buffer.\n",
		 function,
		 read_size );
#endif

		if( memory_copy(
		     &internal_handle->chunk_cache->data[ internal_handle->chunk_cache->offset ],
		     buffer,
		     read_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to write data to chunk cache.",
			 function );

			return( -1 );
		}
		internal_handle->chunk_cache->chunk  = chunk;
		internal_handle->chunk_cache->amount = internal_handle->chunk_cache->offset + read_size;

		/* Adjust the chunk cache offset
		 */
		if( internal_handle->chunk_cache->amount == internal_handle->media_values->chunk_size )
		{
			internal_handle->chunk_cache->offset = 0;
		}
		else if( internal_handle->chunk_cache->amount < internal_handle->media_values->chunk_size )
		{
			internal_handle->chunk_cache->offset = internal_handle->chunk_cache->amount;
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid chunk cache amount value out of range.",
			 function );

			return( -1 );
		}
		chunk_data = internal_handle->chunk_cache->data;
		write_size = internal_handle->chunk_cache->amount;
	}
	if( ( write_size == internal_handle->media_values->chunk_size )
	 || ( ( internal_handle->media_values->media_size != 0 )
	  && ( ( internal_handle->write_io_handle->input_write_count + (ssize64_t) write_size ) == (ssize64_t) internal_handle->media_values->media_size ) )
	 || ( force_write != 0 ) )
	{
		chunk_cache_data_used = (int) ( chunk_data == internal_handle->chunk_cache->data );

		/* The compressed data size contains the maximum allowed buffer size
		 */
		compressed_chunk_data_size = internal_handle->chunk_cache->allocated_size;

		/* Compress the chunk if necessary and determine its CRC
		 */
		chunk_data_size = libewf_write_io_handle_process_chunk(
				   internal_handle->chunk_cache,
				   internal_handle->media_values,
				   internal_handle->io_handle->compression_level,
				   internal_handle->io_handle->compress_empty_block,
				   internal_handle->io_handle->ewf_format,
				   chunk_data,
				   write_size,
				   internal_handle->chunk_cache->compressed,
				   &compressed_chunk_data_size,
				   &is_compressed,
				   &chunk_crc,
				   &write_crc,
		                   error );

		if( chunk_data_size <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_OUTPUT_FAILED,
			 "%s: unable to process chunk data.",
			 function );

			return( -1 );
		}
		/* Make sure to update the chunk_data pointer if
		 * internal_handle->chunk_cache->data has been reallocated by
		 * libewf_write_io_handle_process_chunk()
		 */
		if( ( chunk_cache_data_used == 1 )
		 && ( chunk_data != internal_handle->chunk_cache->data ) )
		{
			chunk_data = internal_handle->chunk_cache->data;
		}
		if( is_compressed != 0 )
		{
			chunk_data = internal_handle->chunk_cache->compressed;
		}
		write_count = libewf_write_io_handle_write_new_chunk(
		               internal_handle->write_io_handle,
		               internal_handle->io_handle,
		               internal_handle->media_values,
		               internal_handle->offset_table,
		               internal_handle->segment_table,
		               &( internal_handle->header_values ),
		               internal_handle->hash_values,
		               internal_handle->header_sections,
		               internal_handle->hash_sections,
		               internal_handle->sessions,
		               internal_handle->acquiry_errors,
		               chunk,
		               chunk_data,
		               chunk_data_size,
		               write_size,
		               is_compressed,
		               chunk_crc,
		               write_crc,
		               error );

		if( write_count <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write chunk.",
			 function );

			return( -1 );
		}
	}
	/* Report the amount of chunk data written
	 */
	return( (ssize_t) read_size );
}

/* Writes an existing chunk of data in EWF format from a buffer at the current offset
 * The necessary settings of the write values must have been made
 * Returns the amount of data bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_chunk_data_existing(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint32_t chunk_offset,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         liberror_error_t **error )
{
	uint8_t *chunk_data         = NULL;
	static char *function       = "libewf_write_chunk_data_existing";
	ewf_crc_t chunk_crc         = 0;
	size_t remaining_chunk_size = 0;
	ssize_t read_count          = 0;
	ssize_t write_count         = 0;
	size_t write_size           = 0;

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
	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.",
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
	if( chunk >= internal_handle->offset_table->amount_of_chunk_offsets )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid chunk value out of range.",
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
	if( data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: writing buffer of size: %" PRIzu " with data of size: %" PRIzd ".\n",
	 function,
	 buffer_size,
	 data_size );
#endif

	/* Check if the data size exceeds the chunk size
	 */
	if( data_size > (size_t) internal_handle->media_values->chunk_size )
	{
		data_size = (size_t) internal_handle->media_values->chunk_size;
	}
	/* Check if the data in the buffer aligns with a chunk
	 */
	if( ( chunk_offset == 0 )
	 && ( data_size == internal_handle->media_values->chunk_size ) )
	{
		chunk_data = buffer;
		write_size = (size_t) internal_handle->media_values->chunk_size;
	}
	else
	{
		/* Read the chunk data into the chunk cache
		 */
		read_count = libewf_read_io_handle_read_chunk_data(
		              internal_handle->read_io_handle,
		              internal_handle->io_handle,
		              internal_handle->media_values,
		              internal_handle->offset_table,
		              internal_handle->chunk_cache,
		              chunk,
		              0,
		              internal_handle->chunk_cache->data,
		              internal_handle->chunk_cache->allocated_size,
		              error );

		if( read_count <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data from chunk.",
			 function );

			return( -1 );
		}
		internal_handle->io_handle->current_chunk_offset = chunk_offset;

		remaining_chunk_size = internal_handle->media_values->chunk_size
		                     - internal_handle->io_handle->current_chunk_offset;

		if( data_size > (size_t) remaining_chunk_size )
		{
			data_size = remaining_chunk_size;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: updating data in chunk: %" PRIu32 " at offset: %" PRIu32 " with size: %" PRIzd ".\n",
		 function,
		 ( chunk + 1 ),
		 chunk_offset,
		 data_size );
#endif

		/* Update the chunk data
		 */
		if( memory_copy(
		     &( internal_handle->chunk_cache->data[ chunk_offset ] ),
		     buffer,
		     data_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to update data in chunk: %" PRIu32 ".",
			 function,
			 (chunk + 1 ) );

			return( -1 );
		}
		chunk_data = internal_handle->chunk_cache->data;
		write_size = (size_t) read_count;
	}
	/* Calculate the new CRC
         */
        chunk_crc = ewf_crc_calculate(
	             chunk_data,
	             write_size,
	             1 );

	write_count = libewf_write_io_handle_write_existing_chunk(
	               internal_handle->write_io_handle,
	               internal_handle->io_handle,
	               internal_handle->media_values,
	               internal_handle->offset_table,
	               internal_handle->delta_segment_table,
	               internal_handle->header_sections,
	               chunk,
	               chunk_data,
	               write_size,
	               write_size,
	               0,
	               chunk_crc,
	               1,
	               error );

	if( write_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write delta chunk.",
		 function );

		return( -1 );
	}
	/* Report the amount of data written
	 */
	return( (ssize_t) data_size );
}

