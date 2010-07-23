/*
 * Low level reading functions
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <byte_stream.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libewf_chunk_value.h"
#include "libewf_definitions.h"
#include "libewf_compression.h"
#include "libewf_chunk_cache.h"
#include "libewf_libbfio.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_read_io_handle.h"
#include "libewf_sector_table.h"
#include "libewf_segment_file_handle.h"

#include "ewf_checksum.h"
#include "ewf_file_header.h"

/* Initialize the read IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_read_io_handle_initialize(
     libewf_read_io_handle_t **read_io_handle,
     liberror_error_t **error )
{
	static char *function = "libewf_read_io_handle_initialize";

	if( read_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read IO handle.",
		 function );

		return( -1 );
	}
	if( *read_io_handle == NULL )
	{
		*read_io_handle = (libewf_read_io_handle_t *) memory_allocate(
		                                               sizeof( libewf_read_io_handle_t ) );

		if( *read_io_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create read IO handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *read_io_handle,
		     0,
		     sizeof( libewf_read_io_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear read IO handle.",
			 function );

			memory_free(
			 *read_io_handle );

			*read_io_handle = NULL;

			return( -1 );
		}
		if( libewf_sector_table_initialize(
		     &( ( *read_io_handle )->checksum_errors ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create checksum errors.",
			 function );

			memory_free(
			 *read_io_handle );

			*read_io_handle = NULL;

			return( -1 );
		}
		( *read_io_handle )->wipe_on_error = 1;
	}
	return( 1 );
}

/* Frees the read IO handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_read_io_handle_free(
     libewf_read_io_handle_t **read_io_handle,
     liberror_error_t **error )
{
	static char *function = "libewf_read_io_handle_free";
	int result            = 1;

	if( read_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read IO handle.",
		 function );

		return( 1 );
	}
	if( *read_io_handle != NULL )
	{
		if( libewf_sector_table_free(
		     &( ( *read_io_handle )->checksum_errors ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free checksum errors.",
			 function );

			result = -1;
		}
		memory_free(
		 *read_io_handle );

		*read_io_handle = NULL;
	}
	return( result );
}

/* Processes the chunk data, applies decompression if necessary and validates the checksum
 * Sets the checksum_mismatch value to 1 if the chunk checksum did not match the calculated checksum
 * Returns the number of bytes of the processed chunk data or -1 on error
 */
ssize_t libewf_read_io_handle_process_chunk(
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         uint8_t *uncompressed_buffer,
         size_t *uncompressed_buffer_size,
         int8_t is_compressed,
         uint32_t chunk_checksum,
         int8_t read_checksum,
         uint8_t *checksum_mismatch,
         liberror_error_t **error )
{
	uint8_t *checksum_buffer     = NULL;
	static char *function        = "libewf_read_io_handle_process_chunk";
	uint32_t calculated_checksum = 0;

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
	if( chunk_buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( checksum_mismatch == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checksum mismatch.",
		 function );

		return( -1 );
	}
	*checksum_mismatch = 0;

	/* Do not bother with an empry chunk
	 */
	if( chunk_buffer_size == 0 )
	{
		return( 0 );
	}
	if( is_compressed == 0 )
	{
		if( read_checksum == 0 )
		{
			chunk_buffer_size -= sizeof( uint32_t );
			checksum_buffer    = &( chunk_buffer[ chunk_buffer_size ] );

			byte_stream_copy_to_uint32_little_endian(
			 checksum_buffer,
			 chunk_checksum );
		}
		calculated_checksum = ewf_checksum_calculate(
		                       chunk_buffer,
		                       chunk_buffer_size,
		                       1 );

		if( chunk_checksum != calculated_checksum )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: checksum does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
				 function,
				 chunk_checksum,
				 calculated_checksum );
			}
#endif

			*checksum_mismatch = 1;
		}
		*uncompressed_buffer_size = chunk_buffer_size;
	}
	else
	{
		if( uncompressed_buffer == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid uncompressed buffer.",
			 function );

			return( -1 );
		}
		if( chunk_buffer== uncompressed_buffer )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid uncompressed buffer is the same as chunk buffer.",
			 function );

			return( -1 );
		}
		if( *uncompressed_buffer_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid uncompressed buffer size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( libewf_decompress(
		     uncompressed_buffer,
		     uncompressed_buffer_size,
		     chunk_buffer,
		     chunk_buffer_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBERROR_COMPRESSION_ERROR_UNCOMPRESS_FAILED,
			 "%s: unable to decompress buffer.",
			 function );

			return( -1 );
		}
	}
	return( (ssize_t) *uncompressed_buffer_size );
}

/* Reads a certain chunk of data into the chunk buffer
 * Will read until the requested size is filled or the entire chunk is read
 * read_checksum is set if the checksum has been read into checksum_buffer
 * read_checksum is used for uncompressed chunks only
 * chunk_checksum is set to a runtime version of the value in the checksum_buffer
 * Returns the number of bytes read, 0 if no bytes can be read or -1 on error
 */
ssize_t libewf_read_io_handle_read_chunk(
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_offset_table_t *offset_table,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         int8_t *is_compressed,
         uint8_t *checksum_buffer,
         uint32_t *chunk_checksum,
         int8_t *read_checksum,
         liberror_error_t **error )
{
	libewf_chunk_value_t *chunk_value = NULL;
	static char *function             = "libewf_read_io_handle_read_chunk";
	ssize_t read_count                = 0;
	ssize_t total_read_count          = 0;
	size_t chunk_size                 = 0;
	uint32_t number_of_chunk_values   = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
        char *chunk_type                  = NULL;
#endif
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
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
	if( chunk_buffer_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid chunk buffer size value is zero.",
		 function );

		return( -1 );
	}
	if( chunk_buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk buffer size value exceeds maximum.",
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
	if( chunk_checksum == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk checksum.",
		 function );

		return( -1 );
	}
	if( read_checksum == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read checksum.",
		 function );

		return( -1 );
	}
	if( libewf_offset_table_get_number_of_chunk_values(
	     offset_table,
	     &number_of_chunk_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of chunk values in the offset table.",
		 function );

		return( -1 );
	}
	/* Check if the chunk is available
	 */
	if( chunk >= number_of_chunk_values )
	{
		return( 0 );
	}
	*chunk_checksum = 0;
	*read_checksum  = 0;
	*is_compressed  = 0;

	if( libewf_offset_table_get_chunk_value(
	     offset_table,
	     chunk,
	     &chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk value: %" PRIu32 ".",
		 function,
		 chunk );

		return( -1 );
	}
	if( chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk value: %" PRIu32 ".",
		 function,
		 chunk );

		return( -1 );
	}
	/* Determine the size of the chunk including the checksum
	 */
	chunk_size = chunk_value->size;

	/* Determine if the chunk is compressed or not
	 */
	if( ( chunk_value->flags & LIBEWF_CHUNK_VALUE_FLAGS_COMPRESSED ) != 0 )
	{
		*is_compressed = 1;
	}
	else if( chunk_buffer_size < chunk_size )
	{
		chunk_size    -= sizeof( uint32_t );
		*read_checksum = 1;
	}
	if( chunk_value->segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk value - missing segment file handle.",
		 function );

		return( -1 );
	}
	/* Make sure the segment file offset is in the right place
	 */
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     chunk_value->segment_file_handle->file_io_pool_entry,
	     chunk_value->file_offset,
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
	if( libnotify_verbose != 0 )
	{
		if( ( chunk_value->flags & LIBEWF_CHUNK_VALUE_FLAGS_DELTA_CHUNK ) != 0 )
		{
			chunk_type = "uncompressed delta";
		}
		else if( *is_compressed == 0 )
		{
			chunk_type = "uncompressed";
		}
		else
		{
			chunk_type = "compressed";
		}
		libnotify_printf(
		 "%s: reading %s chunk %" PRIu32 " of %" PRIu32 " at offset: %" PRIi64 " with size: %" PRIzd ".\n",
		 function,
		 chunk_type,
		 chunk,
		 number_of_chunk_values,
		 chunk_value->file_offset,
		 chunk_value->size );
	}
#endif
	/* Check if the chunk and checksum buffers are aligned
	 * if so read the chunk and checksum at the same time
	 */
	if( ( *is_compressed == 0 )
	 && ( *read_checksum != 0 )
	 && ( &( chunk_buffer[ chunk_size ] ) == checksum_buffer ) )
	{
		chunk_size += sizeof( uint32_t );
	}
	/* Read the chunk data
	 */
	read_count = libbfio_pool_read(
	              file_io_pool,
	              chunk_value->segment_file_handle->file_io_pool_entry,
	              chunk_buffer,
	              chunk_size,
	              error );

	if( read_count != (ssize_t) chunk_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk in segment file.",
		 function );

		return( -1 );
	}
	total_read_count += read_count;

	/* Determine if the checksum should be read seperately
	 */
	if( *read_checksum != 0 )
	{
		/* Check if the chunk and checksum buffers are aligned
		 * if not the chunk and checksum need to be read separately
		 */
		if( &( chunk_buffer[ chunk_size ] ) != checksum_buffer )
		{
			if( checksum_buffer == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
				 "%s: invalid checksum buffer.",
				 function );

				return( -1 );
			}
			read_count = libbfio_pool_read(
			              file_io_pool,
			              chunk_value->segment_file_handle->file_io_pool_entry,
			              checksum_buffer,
			              sizeof( uint32_t ),
			              error );

			if( read_count != (ssize_t) sizeof( uint32_t ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: error reading checksum of chunk: %" PRIu32 " from segment file.",
				 function,
				 chunk );

				return( -1 );
			}
			total_read_count += read_count;
		}
		byte_stream_copy_to_uint32_little_endian(
		 checksum_buffer,
		 *chunk_checksum );
	}
	return( total_read_count );
}

/* Reads a certain chunk of data from the segment file(s)
 * Will read until the requested size is filled or the entire chunk is read
 * Returns the number of bytes read, 0 if no bytes can be read or -1 on error
 */
ssize_t libewf_read_io_handle_read_chunk_data(
         libewf_read_io_handle_t *read_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_chunk_cache_t *chunk_cache,
         uint32_t chunk,
         uint32_t chunk_offset,
         uint8_t *buffer,
         size_t size,
         liberror_error_t **error )
{
	uint8_t stored_checksum_buffer[ 4 ];

	libewf_chunk_value_t *chunk_value = NULL;
	uint8_t *chunk_buffer             = NULL;
	uint8_t *chunk_read_buffer        = NULL;
	uint8_t *checksum_read_buffer     = NULL;
	static char *function             = "libewf_read_io_handle_read_chunk_data";
	size_t chunk_data_size            = 0;
	size_t chunk_size                 = 0;
	size_t bytes_available            = 0;
	ssize_t read_count                = 0;
	int64_t sector                    = 0;
	uint32_t chunk_checksum           = 0;
	uint32_t number_of_sectors        = 0;
	uint8_t checksum_mismatch         = 0;
	int8_t is_compressed              = 0;
	int8_t read_checksum              = 0;
	int chunk_cache_data_used         = 0;

	if( read_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
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
	if( buffer == chunk_cache->compressed )
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
	if( ( chunk_cache->chunk != chunk )
	 || ( chunk_cache->cached == 0 ) )
	{
		if( libewf_offset_table_get_chunk_value(
		     offset_table,
		     chunk,
		     &chunk_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk value: %" PRIu32 ".",
			 function,
			 chunk );

			return( -1 );
		}
		if( chunk_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk value: %" PRIu32 ".",
			 function,
			 chunk );

			return( -1 );
		}
		/* Determine the size of the chunk including the checksum
		 */
		chunk_size = chunk_value->size;

		/* Make sure the chunk cache is large enough
		 */
		chunk_cache_data_used = (int) ( buffer == chunk_cache->data );

		if( chunk_size > chunk_cache->size )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: reallocating chunk size: %" PRIzu ".\n",
				 function,
				 chunk_size );
			}
#endif

			if( libewf_chunk_cache_resize(
			     chunk_cache,
			     chunk_size,
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
			 && ( buffer != chunk_cache->data ) )
			{
				buffer = chunk_cache->data;
			}
		}
		if( ( chunk_value->flags & LIBEWF_CHUNK_VALUE_FLAGS_COMPRESSED ) == 0 )
		{
			is_compressed = 0;
		}
		else
		{
			is_compressed = 1;
		}
		chunk_buffer = chunk_cache->data;

		/* Directly read to the buffer if
		 *  the buffer isn't the chunk cache
		 *  and no data was previously copied into the chunk cache
		 *  and the buffer contains the necessary number of bytes to fill a chunk
		 *  and the buffer is not compressed
		 */
		if( ( buffer != chunk_cache->data )
		 && ( chunk_offset == 0 )
		 && ( size >= (size_t) media_values->chunk_size )
		 && ( is_compressed == 0 ) )
		{
			chunk_buffer = buffer;

			/* The checksum is read seperately for uncompressed chunks
			 */
			chunk_size -= sizeof( uint32_t );
		}
		/* Determine if the chunk data should be directly read into chunk data buffer
		 * or to use the intermediate storage for a compressed chunk
		 */
		if( is_compressed == 1 )
		{
			chunk_read_buffer = chunk_cache->compressed;
		}
		else
		{
			chunk_read_buffer = chunk_buffer;
		}
		/* Use chunk and checksum buffer alignment when the chunk cache data is directly being passed
		 */
		if( chunk_read_buffer == chunk_cache->data )
		{
			checksum_read_buffer = &( chunk_read_buffer[ media_values->chunk_size ] );
		}
		else
		{
			checksum_read_buffer = stored_checksum_buffer;
		}
		/* Read the chunk
		 */
		read_count = libewf_read_io_handle_read_chunk(
		              io_handle,
		              file_io_pool,
		              offset_table,
		              chunk,
		              chunk_read_buffer,
		              chunk_size,
		              &is_compressed,
		              checksum_read_buffer,
		              &chunk_checksum,
		              &read_checksum,
		              error );

		if( read_count <= -1 )
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
			chunk_data_size = media_values->chunk_size
			                + sizeof( uint32_t );
		}
		else
		{
			chunk_data_size = chunk_size;
		}
		if( libewf_read_io_handle_process_chunk(
		     chunk_read_buffer,
		     chunk_size,
		     chunk_buffer,
		     &chunk_data_size,
		     is_compressed,
		     chunk_checksum,
		     read_checksum,
		     &checksum_mismatch,
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
		if( checksum_mismatch != 0 )
		{
			/* Wipe the chunk if nescessary
			 */
			if( ( read_io_handle->wipe_on_error != 0 )
			 && ( memory_set(
			       chunk_read_buffer,
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
			/* Add checksum error
			 */
			sector            = (int64_t) chunk * (int64_t) media_values->sectors_per_chunk;
			number_of_sectors = media_values->sectors_per_chunk;

			if( ( sector + number_of_sectors ) > (int64_t) media_values->number_of_sectors )
			{
				number_of_sectors = (uint32_t) ( (int64_t) media_values->number_of_sectors - sector );
			}
			if( libewf_sector_table_append_sector(
			     read_io_handle->checksum_errors,
			     sector,
			     number_of_sectors,
			     1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append checksum error.",
				 function );

				return( -1 );
			}
			chunk_data_size = number_of_sectors * media_values->bytes_per_sector;
		}
		/* Flag that the chunk was cached
		 */
		if( chunk_buffer == chunk_cache->data )
		{
			chunk_cache->chunk       = chunk;
			chunk_cache->data_size   = chunk_data_size;
			chunk_cache->data_offset = 0;
			chunk_cache->cached      = 1;
		}
	}
	else
	{
		chunk_buffer    = chunk_cache->data;
		chunk_data_size = chunk_cache->data_size;
	}
	/* Determine the available number of data within the cached chunk
	 */
	if( chunk_offset > chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: chunk offset exceeds number of bytes available in chunk.",
		 function );

		return( -1 );
	}
	bytes_available = chunk_data_size - chunk_offset;

	/* Correct the available number of bytes is larger than the requested number of bytes
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
		 "%s: invalid available number of bytes value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* If the data was read into the chunk cache copy it to the buffer
	 *  and the buffer is not the chunk cache itself
	 */
	if( ( chunk_buffer == chunk_cache->data )
	 && ( buffer != chunk_cache->data ) )
	{
		/* Copy the relevant data to buffer
		 */
		if( ( bytes_available > 0 )
		 && ( memory_copy(
		       buffer,
		       &( chunk_buffer[ chunk_offset ] ),
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

