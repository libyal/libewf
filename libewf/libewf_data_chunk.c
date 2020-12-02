/*
 * Data chunk functions
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
#include <types.h>

#include "libewf_data_chunk.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_types.h"
#include "libewf_write_io_handle.h"

/* Creates a data chunk
 * Make sure the value data_chunk is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_data_chunk_initialize(
     libewf_data_chunk_t **data_chunk,
     libewf_io_handle_t *io_handle,
     libewf_write_io_handle_t *write_io_handle,
     libcerror_error_t **error )
{
	libewf_internal_data_chunk_t *internal_data_chunk = NULL;
	static char *function                             = "libewf_data_chunk_initialize";

	if( data_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data chunk.",
		 function );

		return( -1 );
	}
	if( *data_chunk != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data chunk value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	internal_data_chunk = memory_allocate_structure(
	                       libewf_internal_data_chunk_t );

	if( internal_data_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data chunk.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_data_chunk,
	     0,
	     sizeof( libewf_internal_data_chunk_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data chunk.",
		 function );

		memory_free(
		 internal_data_chunk );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_data_chunk->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_data_chunk->io_handle       = io_handle;
	internal_data_chunk->write_io_handle = write_io_handle;

	*data_chunk = (libewf_data_chunk_t *) internal_data_chunk;

	return( 1 );

on_error:
	if( internal_data_chunk != NULL )
	{
		memory_free(
		 internal_data_chunk );
	}
	return( -1 );
}

/* Frees a data chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_data_chunk_free(
     libewf_data_chunk_t **data_chunk,
     libcerror_error_t **error )
{
	libewf_internal_data_chunk_t *internal_data_chunk = NULL;
	static char *function                             = "libewf_data_chunk_free";
	int result                                        = 1;

	if( data_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data chunk.",
		 function );

		return( -1 );
	}
	if( *data_chunk != NULL )
	{
		internal_data_chunk = (libewf_internal_data_chunk_t *) *data_chunk;
		*data_chunk         = NULL;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_data_chunk->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( libewf_chunk_data_free(
		     &( internal_data_chunk->chunk_data ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk data.",
			 function );

			result = -1;
		}
		/* The io_handle and write_io_handle references are freed elsewhere
		 */
		memory_free(
		 internal_data_chunk );
	}
	return( result );
}

/* Sets the chunk data in the data chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_data_chunk_set_chunk_data(
     libewf_internal_data_chunk_t *internal_data_chunk,
     libewf_chunk_data_t *chunk_data,
     libcerror_error_t **error )
{
	static char *function = "libewf_internal_data_chunk_set_chunk_data";
	int result            = 1;

	if( internal_data_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data chunk.",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_data_chunk->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_data_chunk->chunk_data != NULL )
	{
		if( libewf_chunk_data_free(
		     &( internal_data_chunk->chunk_data ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk data.",
			 function );

			result = -1;
		}
	}
	if( result != -1 )
	{
		internal_data_chunk->chunk_data = chunk_data;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_data_chunk->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if the data chunk is corrupted
 * Returns 1 if the data chunk is corrupted, 0 if not or -1 on error
 */
int libewf_data_chunk_is_corrupted(
     libewf_data_chunk_t *data_chunk,
     libcerror_error_t **error )
{
	libewf_internal_data_chunk_t *internal_data_chunk = NULL;
	static char *function                             = "libewf_data_chunk_is_corrupted";
	int result                                        = 0;

	if( data_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data chunk.",
		 function );

		return( -1 );
	}
	internal_data_chunk = (libewf_internal_data_chunk_t *) data_chunk;

	if( internal_data_chunk->chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data chunk - missing chunk data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_data_chunk->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_data_chunk->chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
	{
		result = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_data_chunk->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Reads a buffer from the data chunk
 * It applies decompression if necessary and validates the chunk checksum
 * This function should be used after libewf_handle_read_data_chunk
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 */
ssize_t libewf_data_chunk_read_buffer(
         libewf_data_chunk_t *data_chunk,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libewf_internal_data_chunk_t *internal_data_chunk = NULL;
	static char *function                             = "libewf_data_chunk_read_buffer";
	ssize_t read_count                                = 0;

	if( data_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data chunk.",
		 function );

		return( -1 );
	}
	internal_data_chunk = (libewf_internal_data_chunk_t *) data_chunk;

	if( internal_data_chunk->chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data chunk - missing chunk data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_data_chunk->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_data_chunk->chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_PACKED ) != 0 )
	{
/* TODO optimize to unpack directly to buffer */
		if( libewf_chunk_data_unpack(
		     internal_data_chunk->chunk_data,
		     internal_data_chunk->io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to unpack chunk: %" PRIu64 " data.",
			 function,
			 internal_data_chunk->chunk_data->chunk_index );

			read_count = -1;
		}
	}
	if( read_count != -1 )
	{
		read_count = libewf_chunk_data_read_buffer(
		              internal_data_chunk->chunk_data,
		              buffer,
		              buffer_size,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read chunk: %" PRIu64 " data.",
			 function,
			 internal_data_chunk->chunk_data->chunk_index );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_data_chunk->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Writes a buffer to the data chunk
 * It applies compression if necessary and calculated the chunk checksum
 * This function should be used before libewf_handle_write_data_chunk
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
ssize_t libewf_data_chunk_write_buffer(
         libewf_data_chunk_t *data_chunk,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libewf_internal_data_chunk_t *internal_data_chunk = NULL;
	static char *function                             = "libewf_data_chunk_write_buffer";
	ssize_t write_count                               = 0;

	if( data_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data chunk.",
		 function );

		return( -1 );
	}
	internal_data_chunk = (libewf_internal_data_chunk_t *) data_chunk;

	if( internal_data_chunk->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data chunk - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_data_chunk->write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data chunk - missing write IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_data_chunk->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_data_chunk->chunk_data != NULL )
	{
		if( libewf_chunk_data_free(
		     &( internal_data_chunk->chunk_data ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk data.",
			 function );

			goto on_error;
		}
	}
	if( libewf_chunk_data_initialize(
	     &( internal_data_chunk->chunk_data ),
	     internal_data_chunk->io_handle->chunk_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk data.",
		 function );

		goto on_error;
	}
	write_count = libewf_chunk_data_write_buffer(
	               internal_data_chunk->chunk_data,
	               buffer,
	               buffer_size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer to chunk data.",
		 function );

		goto on_error;
	}
	internal_data_chunk->data_size = buffer_size;

	if( libewf_chunk_data_pack(
	     internal_data_chunk->chunk_data,
	     internal_data_chunk->io_handle,
	     internal_data_chunk->write_io_handle->compressed_zero_byte_empty_block,
	     internal_data_chunk->write_io_handle->compressed_zero_byte_empty_block_size,
	     internal_data_chunk->write_io_handle->pack_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to pack chunk data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_data_chunk->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( write_count );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_data_chunk->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

