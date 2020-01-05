/*
 * Storage media buffer queue
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

#include "ewftools_libcerror.h"
#include "ewftools_libcthreads.h"
#include "ewftools_libewf.h"
#include "storage_media_buffer.h"
#include "storage_media_buffer_queue.h"

#if defined( HAVE_MULTI_THREAD_SUPPORT )

/* Creates a storage media buffer queue
 * Make sure the value queue is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_queue_initialize(
     libcthreads_queue_t **queue,
     libewf_handle_t *handle,
     int maximum_number_of_values,
     uint8_t storage_media_buffer_mode,
     size_t storage_media_buffer_size,
     libcerror_error_t **error )
{
	storage_media_buffer_t *buffer = NULL;
	static char *function          = "storage_media_buffer_queue_initialize";
	int value_index                = 0;

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	if( *queue != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid queue value already set.",
		 function );

		return( -1 );
	}
	if( ( maximum_number_of_values < 0 )
	 || ( maximum_number_of_values > (int) ( INT_MAX - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum number of values value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libcthreads_queue_initialize(
	     queue,
	     maximum_number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize storage media buffer queue.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < maximum_number_of_values;
	     value_index++ )
	{
		/* Add 1 to prevent the queue blocking if full
		 */
		if( storage_media_buffer_initialize(
		     &buffer,
		     handle,
		     storage_media_buffer_mode,
		     storage_media_buffer_size + 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create storage media buffer.",
			 function );

			goto on_error;
		}
		if( libcthreads_queue_push(
		     *queue,
		     (intptr_t *) buffer,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to push storage media buffer onto queue.",
			 function );

			goto on_error;
		}
		buffer = NULL;
	}
	return( 1 );

on_error:
	if( buffer != NULL )
	{
		storage_media_buffer_free(
		 &buffer,
		 NULL );
	}
	if( *queue != NULL )
	{
		storage_media_buffer_queue_free(
		 queue,
		 NULL );
	}
	return( -1 );
}

/* Frees a storage media buffer queue
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_queue_free(
     libcthreads_queue_t **queue,
     libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_queue_free";
	int result            = 1;

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	if( *queue != NULL )
	{
		if( libcthreads_queue_free(
		     queue,
		     (int (*)(intptr_t **, libcerror_error_t **)) &storage_media_buffer_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free storage media buffer queue.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Grabs a storage media buffer from the queue
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_queue_grab_buffer(
     libcthreads_queue_t *queue,
     storage_media_buffer_t **buffer,
     libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_queue_grab_buffer";

	if( libcthreads_queue_pop(
	     queue,
	     (intptr_t **) buffer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
		 "%s: unable to pop storage media buffer from queue.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Releases a storage media buffer onto the queue
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_queue_release_buffer(
     libcthreads_queue_t *queue,
     storage_media_buffer_t *buffer,
     libcerror_error_t **error )
{
	static char *function = "storage_media_buffer_queue_release_buffer";

	if( libcthreads_queue_push(
	     queue,
	     (intptr_t *) buffer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to push storage media buffer onto queue.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

