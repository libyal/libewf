/* 
 * Alteration handle
 *
 * Copyright (C) 2007-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include <libsystem.h>

#include "alteration_handle.h"
#include "storage_media_buffer.h"

/* Initializes the alteration handle
 * Returns 1 if successful or -1 on error
 */
int alteration_handle_initialize(
     alteration_handle_t **alteration_handle,
     liberror_error_t **error )
{
	static char *function = "alteration_handle_initialize";

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( *alteration_handle == NULL )
	{
		*alteration_handle = (alteration_handle_t *) memory_allocate(
		                                              sizeof( alteration_handle_t ) );

		if( *alteration_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create alteration handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *alteration_handle,
		     0,
		     sizeof( alteration_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear alteration handle.",
			 function );

			memory_free(
			 *alteration_handle );

			*alteration_handle = NULL;

			return( -1 );
		}
		if( libewf_handle_initialize(
		     &( ( *alteration_handle )->input_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input handle.",
			 function );

			memory_free(
			 *alteration_handle );

			*alteration_handle = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the alteration handle and its elements
 * Returns 1 if successful or -1 on error
 */
int alteration_handle_free(
     alteration_handle_t **alteration_handle,
     liberror_error_t **error )
{
	static char *function = "alteration_handle_free";
	int result            = 1;

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( *alteration_handle != NULL )
	{
		if( ( ( *alteration_handle )->input_handle != NULL )
		 && ( libewf_handle_free(
		       &( ( *alteration_handle )->input_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *alteration_handle );

		*alteration_handle = NULL;
	}
	return( result );
}

/* Signals the alteration handle to abort
 * Returns 1 if successful or -1 on error
 */
int alteration_handle_signal_abort(
     alteration_handle_t *alteration_handle,
     liberror_error_t **error )
{
	static char *function = "alteration_handle_signal_abort";

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle != NULL )
	{
		if( libewf_handle_signal_abort(
		     alteration_handle->input_handle,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input handle to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Opens the input of the alteration handle
 * Returns 1 if successful or -1 on error
 */
int alteration_handle_open_input(
     alteration_handle_t *alteration_handle,
     libsystem_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error )
{
	libsystem_character_t **libewf_filenames = NULL;
	static char *function                    = "alteration_handle_open_input";
	size_t first_filename_length             = 0;
	int result                               = 1;

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames == 1 )
	{
		first_filename_length = libsystem_string_length(
		                         filenames[ 0 ]);

#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
		if( libewf_glob_wide(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &amount_of_filenames,
		     error ) != 1 )
#else
		if( libewf_glob(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &amount_of_filenames,
		     error ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to resolve filename(s).",
			 function );

			return( -1 );
		}
		filenames = (libsystem_character_t * const *) libewf_filenames;
	}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libewf_handle_open_wide(
	     alteration_handle->input_handle,
	     filenames,
	     amount_of_filenames,
	     LIBEWF_OPEN_READ_WRITE,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     alteration_handle->input_handle,
	     filenames,
	     amount_of_filenames,
	     LIBEWF_OPEN_READ_WRITE,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open files.",
		 function );

		result = -1;
	}
	if( libewf_filenames != NULL )
	{
		for( ; amount_of_filenames > 0; amount_of_filenames-- )
		{
			memory_free(
			 libewf_filenames[ amount_of_filenames - 1 ] );
		}
		memory_free(
		 libewf_filenames );
	}
	return( result );
}

/* Closes the alteration handle
 * Returns the 0 if succesful or -1 on error
 */
int alteration_handle_close(
     alteration_handle_t *alteration_handle,
     liberror_error_t **error )
{
	static char *function = "alteration_handle_close";

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_close(
	     alteration_handle->input_handle,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )

/* Prepares a buffer after reading the input of the alteration handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t alteration_handle_prepare_read_buffer(
         alteration_handle_t *alteration_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "alteration_handle_prepare_read_buffer";
	ssize_t process_count = 0;

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	storage_media_buffer->raw_buffer_amount = storage_media_buffer->raw_buffer_size;

	process_count = libewf_handle_prepare_read_chunk(
	                 alteration_handle->input_handle,
	                 storage_media_buffer->compression_buffer,
	                 storage_media_buffer->compression_buffer_amount,
	                 storage_media_buffer->raw_buffer,
	                 (size_t *) &( storage_media_buffer->raw_buffer_amount ),
	                 storage_media_buffer->is_compressed,
	                 storage_media_buffer->crc,
	                 storage_media_buffer->process_crc,
	                 error );

	if( process_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to prepare storage media buffer after reading.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer->is_compressed == 0 )
	{
		storage_media_buffer->data_in_compression_buffer = 1;
	}
	else
	{
		storage_media_buffer->data_in_compression_buffer = 0;
	}
	return( process_count );
}

/* Reads a buffer from the input of the alteration handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t alteration_handle_read_buffer(
         alteration_handle_t *alteration_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error )
{
	static char *function = "alteration_handle_read_buffer";
	ssize_t read_count    = 0;

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	read_count = libewf_handle_read_chunk(
                      alteration_handle->input_handle,
                      storage_media_buffer->compression_buffer,
                      storage_media_buffer->compression_buffer_size,
	              &( storage_media_buffer->is_compressed ),
	              &( storage_media_buffer->compression_buffer[ storage_media_buffer->raw_buffer_size ] ),
	              &( storage_media_buffer->crc ),
	              &( storage_media_buffer->process_crc ),
	              error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read storage media buffer.",
		 function );

		return( -1 );
	}
	storage_media_buffer->compression_buffer_amount = read_count;

	return( read_count );
}

#endif

/* Prepares a buffer before writing the output of the alteration handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t alteration_handle_prepare_write_buffer(
         alteration_handle_t *alteration_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "alteration_handle_prepare_write_buffer";
	ssize_t process_count = 0;

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	storage_media_buffer->compression_buffer_amount = storage_media_buffer->compression_buffer_size;

	process_count = libewf_handle_prepare_write_chunk(
	                 alteration_handle->input_handle,
	                 storage_media_buffer->raw_buffer,
	                 storage_media_buffer->raw_buffer_amount,
	                 storage_media_buffer->compression_buffer,
	                 (size_t *) &( storage_media_buffer->compression_buffer_amount ),
	                 &( storage_media_buffer->is_compressed ),
	                 &( storage_media_buffer->crc ),
	                 &( storage_media_buffer->process_crc ),
	                 error );

	if( process_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to prepare storage media buffer before writing.",
		 function );

		return( -1 );
	}
#else
	process_count = storage_media_buffer->raw_buffer_amount;
#endif

	return( process_count );
}

/* Writes a buffer to the output of the alteration handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t alteration_handle_write_buffer(
         alteration_handle_t *alteration_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t write_size,
         liberror_error_t **error )
{
	static char *function        = "alteration_handle_write_buffer";
	ssize_t write_count          = 0;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	uint8_t *raw_write_buffer    = NULL;
	size_t raw_write_buffer_size = 0;
#endif

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	if( write_size == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( storage_media_buffer->is_compressed == 0 )
	{
		raw_write_buffer      = storage_media_buffer->raw_buffer;
		raw_write_buffer_size = storage_media_buffer->raw_buffer_amount;
	}
	else
	{
		raw_write_buffer      = storage_media_buffer->compression_buffer;
		raw_write_buffer_size = storage_media_buffer->compression_buffer_amount;
	}
	if( write_size != raw_write_buffer_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: mismatch in write size and amount of bytes in storage media buffer.",
		 function );

		return( -1 );
	}
	write_count = libewf_handle_write_chunk(
	               alteration_handle->input_handle,
	               raw_write_buffer,
	               raw_write_buffer_size,
	               storage_media_buffer->raw_buffer_amount,
	               storage_media_buffer->is_compressed,
	               storage_media_buffer->crc_buffer,
	               storage_media_buffer->crc,
	               storage_media_buffer->process_crc,
	               error );
#else
	write_count = libewf_handle_write_buffer(
	               alteration_handle->input_handle,
	               storage_media_buffer->raw_buffer,
	               write_size,
	               error );
#endif

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write storage media buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Seeks the offset in the input file
 * Returns the new offset if successful or -1 on error
 */
off64_t alteration_handle_seek_offset(
         alteration_handle_t *alteration_handle,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "alteration_handle_seek_offset";

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	offset = libewf_handle_seek_offset(
	          alteration_handle->input_handle,
	          offset,
	          SEEK_SET,
	          error );

	if( offset == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in input handle.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Retrieves the media size
 * Returns 1 if successful or -1 on error
 */
int alteration_handle_get_media_size(
     alteration_handle_t *alteration_handle,
     size64_t *media_size,
     liberror_error_t **error )
{
	static char *function = "alteration_handle_get_media_size";

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_media_size(
	     alteration_handle->input_handle,
	     media_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 */
int alteration_handle_get_chunk_size(
     alteration_handle_t *alteration_handle,
     size32_t *chunk_size,
     liberror_error_t **error )
{
	static char *function = "alteration_handle_get_chunk_size";

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk size.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_chunk_size(
	     alteration_handle->input_handle,
	     chunk_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int alteration_handle_set_header_codepage(
     alteration_handle_t *alteration_handle,
     int header_codepage,
     liberror_error_t **error )
{
	static char *function = "alteration_handle_set_header_codepage";

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_header_codepage(
	     alteration_handle->input_handle,
	     header_codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header codepage.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the output values of the alteration handle
 * Returns 1 if successful or -1 on error
 */
int alteration_handle_set_output_values(
     alteration_handle_t *alteration_handle,
     libsystem_character_t *delta_segment_filename,
     size_t delta_segment_filename_length,
     liberror_error_t **error )
{
	static char *function = "alteration_handle_set_output_values";

	if( alteration_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alteration handle.",
		 function );

		return( -1 );
	}
	if( alteration_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alteration handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libewf_handle_set_delta_segment_filename_wide(
	     alteration_handle->input_handle,
	     delta_segment_filename,
	     delta_segment_filename_length,
	     error ) != 1 )
#else
	if( libewf_handle_set_delta_segment_filename(
	     alteration_handle->input_handle,
	     delta_segment_filename,
	     delta_segment_filename_length,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set delta segment filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

