/*
 * Storage media buffer
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <liberror.h>

#include "storage_media_buffer.h"

/* Initialize a buffer
 * Make sure the value buffer is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_initialize(
     storage_media_buffer_t **buffer,
     size_t size,
     liberror_error_t **error )
{
	static char *function = "storage_media_buffer_initialize";

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
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( *buffer == NULL )
	{
		*buffer = (storage_media_buffer_t *) memory_allocate(
		                                      sizeof( storage_media_buffer_t ) );

		if( *buffer == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create buffer.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *buffer,
		     0,
		     sizeof( storage_media_buffer_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear buffer.",
			 function );

			memory_free(
			 *buffer );

			*buffer = NULL;

			return( -1 );
		}
		if( size > 0 )
		{
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			/* Add 4 bytes to allow for write CRC buffer alignment
			 */
			( *buffer )->raw_buffer = (uint8_t *) memory_allocate(
			                                       sizeof( uint8_t ) * ( size + 4 ) );
#else
			( *buffer )->raw_buffer = (uint8_t *) memory_allocate(
			                                       sizeof( uint8_t ) * size );
#endif
			
			if( ( *buffer )->raw_buffer == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create raw buffer.",
				 function );

				memory_free(
				 *buffer );

				*buffer = NULL;

				return( -1 );
			}
			( *buffer )->raw_buffer_size = size;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			( *buffer )->crc_buffer = &( ( ( *buffer )->raw_buffer )[ size ] );

			( *buffer )->compression_buffer = (uint8_t *) memory_allocate(
			                                               sizeof( uint8_t ) * ( size * 2 ) );
			
			if( ( *buffer )->compression_buffer == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create compression buffer.",
				 function );

				memory_free(
				 ( *buffer )->raw_buffer );
				memory_free(
				 *buffer );

				*buffer = NULL;

				return( -1 );
			}
			( *buffer )->compression_buffer_size = size * 2;

#endif
		}
	}
	return( 1 );
}

/* Frees an exisisting buffer
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_free(
     storage_media_buffer_t **buffer,
     liberror_error_t **error )
{
	static char *function = "storage_media_buffer_free";

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
	if( *buffer != NULL )
	{
		if( ( *buffer )->raw_buffer != NULL )
		{
			memory_free(
			 ( *buffer )->raw_buffer );
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		if( ( *buffer )->compression_buffer != NULL )
		{
			memory_free(
			 ( *buffer )->compression_buffer );
		}
#endif
		memory_free(
		 *buffer );

		*buffer = NULL;
	}
	return( 1 );
}

/* Resizes an exisisting buffer
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_resize(
     storage_media_buffer_t *buffer,
     size_t size,
     liberror_error_t **error )
{
	void *reallocation    = NULL;
	static char *function = "storage_media_buffer_resize";

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
	if( size > buffer->raw_buffer_size )
	{
		reallocation = memory_reallocate(
				buffer->raw_buffer,
				sizeof( uint8_t ) * size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to reallocate read buffer.",
			 function );

			return( -1 );
		}
		buffer->raw_buffer        = (uint8_t *) reallocation;
		buffer->raw_buffer_size   = size;
		buffer->raw_buffer_amount = 0;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		/* The compression buffer is cleared
		 */
		if( buffer->compression_buffer != NULL )
		{
			memory_free(
			 buffer->compression_buffer );

			buffer->compression_buffer        = NULL;
			buffer->compression_buffer_size   = 0;
			buffer->compression_buffer_amount = 0;
		}
#endif
	}
	return( 1 );
}

/* Retrieves the reference to the actual data and its size
 * Returns 1 if successful or -1 on error
 */
int storage_media_buffer_get_data(
     storage_media_buffer_t *buffer,
     uint8_t **data,
     size_t *size,
     liberror_error_t **error )
{
	static char *function = "storage_media_buffer_get_data";

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
	if( data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( buffer->data_in_compression_buffer == 0 )
	{
		*data = buffer->raw_buffer;
		*size = (size_t) buffer->raw_buffer_amount;
	}
	else
	{
		*data = (void *) buffer->compression_buffer;
		*size = (size_t) buffer->compression_buffer_amount;
	}
#else
	*data = (void *) buffer->raw_buffer;
	*size = (size_t) buffer->raw_buffer_amount;
#endif
	return( 1 );
}

