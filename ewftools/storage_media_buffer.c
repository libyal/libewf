/*
 * Storage media buffer
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>

#include "ewftools_libcdata.h"
#include "ewftools_libcerror.h"
#include "ewftools_libcthreads.h"
#include "ewftools_libewf.h"
#include "storage_media_buffer.h"

/* Creates a storage media buffer
 * Make sure the value buffer is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_initialize(
     storage_media_buffer_t **buffer,
     libewf_handle_t *handle,
     uint8_t mode,
     size_t size,
     libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_initialize";

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( *buffer != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid buffer value already set.",
		 function );

		return( -1 );
	}
	if( ( mode != STORAGE_MEDIA_BUFFER_MODE_BUFFERED )
	 && ( mode != STORAGE_MEDIA_BUFFER_MODE_CHUNK_DATA ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unusupported mode.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	*buffer = memory_allocate_structure(
	           storage_media_buffer_t );

	if( *buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create buffer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *buffer,
	     0,
	     sizeof( storage_media_buffer_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear buffer.",
		 function );

		memory_free(
		 *buffer );

		*buffer = NULL;

		return( -1 );
	}
	if( size > 0 )
	{
/* TODO can low level functions and direct IO be combined ? */
#if defined( memory_allocate_aligned )
		if( memory_allocate_aligned(
		     (void **) &( ( *buffer )->raw_buffer ),
		     size,
		     512 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create aligned raw buffer.",
			 function );

			goto on_error;
		}
#else
		( *buffer )->raw_buffer = (uint8_t *) memory_allocate(
		                                       sizeof( uint8_t ) * size );
			
		if( ( *buffer )->raw_buffer == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create raw buffer.",
			 function );

			goto on_error;
		}
#endif /* defined( memory_allocate_aligned ) */

		( *buffer )->raw_buffer_size = size;
	}
	if( ( mode == STORAGE_MEDIA_BUFFER_MODE_CHUNK_DATA )
	 && ( handle != NULL ) )
	{
		if( libewf_handle_get_data_chunk(
		     handle,
		     &( ( *buffer )->data_chunk ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data chunk.",
			 function );

			goto on_error;
		}
	}
	( *buffer )->mode = mode;

	return( 1 );

on_error:
	if( *buffer != NULL )
	{
		if( ( *buffer )->data_chunk != NULL )
		{
			libewf_data_chunk_free(
			 &( ( *buffer )->data_chunk ),
			 NULL );
		}
		if( ( *buffer )->raw_buffer != NULL )
		{
			memory_free(
			 ( *buffer )->raw_buffer );
		}
		memory_free(
		 *buffer );

		*buffer = NULL;
	}
	return( -1 );
}

/* Frees a storage media buffer
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_free(
     storage_media_buffer_t **buffer,
     libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_free";
	int result            = 1;

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( *buffer != NULL )
	{
		if( ( *buffer )->raw_buffer != NULL )
		{
			memory_free(
			 ( *buffer )->raw_buffer );
		}
		if( ( *buffer )->data_chunk != NULL )
		{
			if( libewf_data_chunk_free(
			     &( ( *buffer )->data_chunk ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data chunk.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *buffer );

		*buffer = NULL;
	}
	return( result );
}

/* Retrieves the reference to the actual data and its size
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_get_data(
     storage_media_buffer_t *buffer,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_get_data";

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data      = buffer->raw_buffer;
	*data_size = buffer->raw_buffer_data_size;

	return( 1 );
}

/* Compares two storage media buffers
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER
 * if successful or -1 on error
 */
int storage_media_buffer_compare(
     storage_media_buffer_t *first_buffer,
     storage_media_buffer_t *second_buffer,
     libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_compare";

	if( first_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first buffer.",
		 function );

		return( -1 );
	}
	if( second_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second buffer.",
		 function );

		return( -1 );
	}
	if( first_buffer->storage_media_offset < second_buffer->storage_media_offset )
	{
		return( LIBCDATA_COMPARE_LESS ); 
	}
	else if( first_buffer->storage_media_offset > second_buffer->storage_media_offset )
	{
		return( LIBCDATA_COMPARE_GREATER ); 
	}
	return( LIBCDATA_COMPARE_EQUAL ); 
}

/* Determines if the storage media buffer is corrupted
 * Returns 1 if the storage media buffer is corrupted, 0 if not or -1 on error
 */
int storage_media_buffer_is_corrupted(
     storage_media_buffer_t *storage_media_buffer,
     libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_is_corrupted";

	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	return( (int) storage_media_buffer->is_corrupted );
}

/* Reads a storage media buffer from the input handle
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 */
ssize_t storage_media_buffer_read_from_handle(
         storage_media_buffer_t *storage_media_buffer,
         libewf_handle_t *handle,
         size_t read_size,
         libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_read_from_handle";
	ssize_t read_count    = 0;

	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	if( read_size == 0 )
	{
		return( 0 );
	}
	if( storage_media_buffer->mode == STORAGE_MEDIA_BUFFER_MODE_CHUNK_DATA )
	{
		read_count = libewf_handle_read_data_chunk(
	                      handle,
	                      storage_media_buffer->data_chunk,
		              error );
	}
	else
	{
		read_count = libewf_handle_read_buffer(
	                      handle,
	                      storage_media_buffer->raw_buffer,
	                      read_size,
		              error );
	}
	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read storage media buffer.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer->mode == STORAGE_MEDIA_BUFFER_MODE_BUFFERED )
	{
		storage_media_buffer->raw_buffer_data_size = (size_t) read_count;
	}
	storage_media_buffer->requested_size = read_size;

	return( read_count );
}

/* Processes a storage media buffer after read
 * Returns the resulting buffer size or -1 on error
 */
ssize_t storage_media_buffer_read_process(
         storage_media_buffer_t *storage_media_buffer,
         libcerror_error_t **error )
{
        static char *function = "storage_media_buffer_read_process";
	ssize_t process_count = 0;
	int result            = 0;

	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer->mode == STORAGE_MEDIA_BUFFER_MODE_CHUNK_DATA )
	{
		process_count = libewf_data_chunk_read_buffer(
		                 storage_media_buffer->data_chunk,
		                 storage_media_buffer->raw_buffer,
		                 storage_media_buffer->raw_buffer_size,
		                 error );

		if( process_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from data chunk.",
			 function );

			return( -1 );
		}
		if( process_count > (ssize_t) storage_media_buffer->requested_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: more bytes read than requested.",
			 function );

			return( -1 );
		}
		storage_media_buffer->raw_buffer_data_size = (size_t) process_count;

		result = libewf_data_chunk_is_corrupted(
		          storage_media_buffer->data_chunk,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if data chunk is corrupted.",
			 function );

			return( -1 );
		}
		storage_media_buffer->is_corrupted = (uint8_t) result;
	}
	else
	{
		process_count = (ssize_t) storage_media_buffer->raw_buffer_data_size;
	}
	storage_media_buffer->processed_size = (size_t) process_count;

	return( process_count );
}

/* Processes a storage media buffer after read
 * Returns the resulting buffer size or -1 on error
 */
ssize_t storage_media_buffer_write_process(
         storage_media_buffer_t *storage_media_buffer,
         libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_write_process";
	ssize_t process_count = 0;

	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer->mode == STORAGE_MEDIA_BUFFER_MODE_CHUNK_DATA )
	{
		process_count = libewf_data_chunk_write_buffer(
				 storage_media_buffer->data_chunk,
				 storage_media_buffer->raw_buffer,
				 storage_media_buffer->raw_buffer_data_size,
				 error );

		if( process_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to write to data chunk.",
			 function );

			return( -1 );
		}
	}
	else
	{
		process_count = (ssize_t) storage_media_buffer->raw_buffer_data_size;
	}
	storage_media_buffer->processed_size = (size_t) process_count;

	return( process_count );
}

/* Writes a storage media buffer to the input handle
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
ssize_t storage_media_buffer_write_to_handle(
         storage_media_buffer_t *storage_media_buffer,
         libewf_handle_t *handle,
         size_t write_size,
         libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_write_to_handle";
	ssize_t write_count   = 0;

	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	if( write_size == 0 )
	{
		return( 0 );
	}
	if( storage_media_buffer->mode == STORAGE_MEDIA_BUFFER_MODE_CHUNK_DATA )
	{
		write_count = libewf_handle_write_data_chunk(
		               handle,
		               storage_media_buffer->data_chunk,
		               error );
	}
	else
	{
		write_count = libewf_handle_write_buffer(
		               handle,
		               storage_media_buffer->raw_buffer,
		               write_size,
		               error );
	}
	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write storage media buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

