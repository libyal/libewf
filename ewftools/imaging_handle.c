/* 
 * Imaging handle
 *
 * Copyright (C) 2007-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_EXPORT
#endif

#include <libewf.h>

#include "digest_context.h"
#include "digest_hash.h"
#include "ewflibewf.h"
#include "file_io.h"
#include "imaging_handle.h"
#include "md5.h"
#include "notify.h"
#include "sha1.h"
#include "storage_media_buffer.h"
#include "system_string.h"

/* TODO refactor */
#include "ewfcommon.h"

#if !defined( USE_LIBEWF_GET_HASH_VALUE_MD5 ) && !defined( USE_LIBEWF_GET_MD5_HASH )
#define USE_LIBEWF_GET_HASH_VALUE_MD5
#endif

/* Initializes the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_initialize(
     imaging_handle_t **imaging_handle,
     uint8_t calculate_md5,
     uint8_t calculate_sha1,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_initialize";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( *imaging_handle == NULL )
	{
		*imaging_handle = (imaging_handle_t *) memory_allocate(
		                                        sizeof( imaging_handle_t ) );

		if( *imaging_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create imaging handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *imaging_handle,
		     0,
		     sizeof( imaging_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear imaging handle.",
			 function );

			memory_free(
			 *imaging_handle );

			*imaging_handle = NULL;

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_initialize(
		     &( ( *imaging_handle )->output_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize output handle.",
			 function );

			memory_free(
			 *imaging_handle );

			*imaging_handle = NULL;

			return( -1 );
		}
#endif
		( *imaging_handle )->calculate_md5  = calculate_md5;
		( *imaging_handle )->calculate_sha1 = calculate_sha1;

		if( ( ( *imaging_handle )->calculate_md5 != 0 )
		 && ( md5_initialize(
		       &( ( *imaging_handle )->md5_context ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize MD5 context.",
			 function );

#if defined( HAVE_V2_API )
			libewf_handle_free(
			 &( ( *imaging_handle )->output_handle ),
			 NULL );
#endif
			memory_free(
			 *imaging_handle );

			*imaging_handle = NULL;

			return( -1 );
		}
		if( ( ( *imaging_handle )->calculate_sha1 != 0 )
		 && ( sha1_initialize(
		       &( ( *imaging_handle )->sha1_context ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize SHA1 context.",
			 function );

#if defined( HAVE_V2_API )
			libewf_handle_free(
			 &( ( *imaging_handle )->output_handle ),
			 NULL );
#endif
			memory_free(
			 *imaging_handle );

			*imaging_handle = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the imaging handle and its elements
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_free(
     imaging_handle_t **imaging_handle,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_free";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( *imaging_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( ( ( *imaging_handle )->output_handle != NULL )
		 && ( libewf_handle_free(
		       &( ( *imaging_handle )->output_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free output handle.",
			 function );
		}
#endif
		memory_free(
		 *imaging_handle );

		*imaging_handle = NULL;
	}
	return( 1 );
}

/* Opens the output of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_open_output(
     imaging_handle_t *imaging_handle,
     char * const * filenames,
     int amount_of_files,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_open_output";
	int result            = 1;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
#else
	if( imaging_handle->output_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid imaging handle - output handle already set.",
		 function );

		return( -1 );
	}
#endif
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
	if( amount_of_files <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of filenames.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_open(
	     imaging_handle->output_handle,
	     filenames,
	     amount_of_files,
	     LIBEWF_OPEN_WRITE,
	     error ) != 1 )
#else
	imaging_handle->output_handle = libewf_open(
	                                 filenames,
	                                 amount_of_files,
	                                 LIBEWF_OPEN_WRITE );

	if( imaging_handle->output_handle == NULL )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open files.",
		 function );

		return( 1 );
	}
	return( result );
}

/* TODO refactor input_file_descriptor out of function into handle */

/* Reads a buffer from the input of the imaging handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t imaging_handle_read_buffer(
         imaging_handle_t *imaging_handle,
         int input_file_descriptor,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_read_buffer";
	ssize_t read_count    = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
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
	/* TODO refactor */
	read_count = ewfcommon_read_input(
		      imaging_handle->output_handle,
		      input_file_descriptor,
		      storage_media_buffer->raw_buffer,
		      storage_media_buffer->raw_buffer_size,
		      imaging_handle->chunk_size,
		      imaging_handle->bytes_per_sector,
		      imaging_handle->input_offset,
		      imaging_handle->media_size,
		      imaging_handle->read_error_retry,
		      imaging_handle->error_granularity,
		      imaging_handle->wipe_chunk_on_error,
		      imaging_handle->seek_on_error );

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
	storage_media_buffer->offset            = imaging_handle->input_offset;
	storage_media_buffer->raw_buffer_amount = read_count;

	return( read_count );
}

/* Prepares a buffer before writing the output of the imaging handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t imaging_handle_write_prepare_buffer(
         imaging_handle_t *imaging_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_write_prepare_buffer";
	ssize_t write_count   = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
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
#if defined( HAVE_RAW_ACCESS )
	storage_media_buffer->raw_buffer_amount = storage_media_buffer->raw_buffer_size;

#if defined( HAVE_V2_API )
	write_count = libewf_handle_raw_write_prepare_buffer(
                       imaging_handle->output_handle,
                       storage_media_buffer->compression_buffer,
                       storage_media_buffer->compression_buffer_amount,
                       storage_media_buffer->raw_buffer,
                       (size_t *) &( storage_media_buffer->raw_buffer_amount ),
                       storage_media_buffer->is_compressed,
                       storage_media_buffer->crc,
                       storage_media_buffer->process_crc,
	               error );
#else
	write_count = libewf_raw_write_prepare_buffer(
                       imaging_handle->output_handle,
                       storage_media_buffer->compression_buffer,
                       storage_media_buffer->compression_buffer_amount,
                       storage_media_buffer->raw_buffer,
                       (size_t *) &( storage_media_buffer->raw_buffer_amount ),
                       storage_media_buffer->is_compressed,
                       storage_media_buffer->crc,
                       storage_media_buffer->process_crc );
#endif

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to prepare storage media buffer before writing.",
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
#else
	write_count = storage_media_buffer->raw_buffer_amount;
#endif
	storage_media_buffer->input_size  = write_count;
	imaging_handle->input_offset     += write_count;

	return( write_count );
}

/* Writes a buffer to the output of the imaging handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t imaging_handle_write_buffer(
         imaging_handle_t *imaging_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_read_buffer";
	ssize_t read_count    = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
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
#if defined( HAVE_RAW_ACCESS )
#if defined( HAVE_V2_API )
	read_count = libewf_raw_read_buffer(
                      imaging_handle->output_handle,
                      storage_media_buffer->compression_buffer,
                      storage_media_buffer->compression_buffer_size,
	              &( storage_media_buffer->is_compressed ),
	              &( storage_media_buffer->crc ),
	              &( storage_media_buffer->process_crc ),
	              error );
#else
	read_count = libewf_raw_read_buffer(
	              imaging_handle->output_handle,
	              storage_media_buffer->compression_buffer,
	              storage_media_buffer->compression_buffer_size,
	              &( storage_media_buffer->is_compressed ),
	              &( storage_media_buffer->crc ),
	              &( storage_media_buffer->process_crc ) );
#endif
#else
#if defined( HAVE_V2_API )
	read_count = libewf_read_buffer(
                      imaging_handle->output_handle,
                      storage_media_buffer->raw_buffer,
                      read_size,
	              error );
#else
	read_count = libewf_read_buffer(
                      imaging_handle->output_handle,
                      storage_media_buffer->raw_buffer,
                      read_size );
#endif
#endif
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
	storage_media_buffer->offset = imaging_handle->input_offset;

#if defined( HAVE_RAW_ACCESS )
	storage_media_buffer->compression_buffer_amount = read_count;
#else
	storage_media_buffer->raw_buffer_amount         = read_count;
#endif
	return( read_count );
}

/* TODO refactor ? */
/* Finalizes the output write
 * Returns 1 if successful or -1 on error
 */
ssize_t imaging_handle_write_finalize(
         imaging_handle_t *imaging_handle,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_write_finalize";
	ssize_t write_count   = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.\n",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
#if defined( LIBEWF_CD_SUPPORT )
	if( libewf_add_session(
	     imaging_handle->output_handle,
	     1,
	     write_size ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable add session.\n",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_V2_API )
	write_count = libewf_handle_write_finalize(
	               imaging_handle->output_handle,
	               error );
#else
	write_count = libewf_write_finalize(
	               imaging_handle->output_handle );
#endif

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to finalize EWF file(s).\n",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Swaps the byte order of byte pairs within a buffer of a certain size
 * Returns 1 if successful, -1 on error
 */
int imaging_handle_swap_byte_pairs(
     imaging_handle_t *imaging_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_swap_byte_pairs";
	size_t iterator       = 0;
	uint8_t byte          = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.\n",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.\n",
		 function );

		return( -1 );
	}
	if( ( read_size == 0 )
	 || ( read_size > (size_t) SSIZE_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid read size value out of range.\n",
		 function );

		return( -1 );
	}
	/* If the last bit is set the value is odd
	 */
	if( ( read_size & 0x01 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid read size value is odd.\n",
		 function );

		return( -1 );
	}
	if( read_size != (size_t) storage_media_buffer->raw_buffer_amount )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: mismatch in read size and buffer data amount.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < read_size; iterator += 2 )
	{
		byte                                             = storage_media_buffer->raw_buffer[ iterator ];
		storage_media_buffer->raw_buffer[ iterator ]     = storage_media_buffer->raw_buffer[ iterator + 1 ];
		storage_media_buffer->raw_buffer[ iterator + 1 ] = byte;
	}
	return( 1 );
}

/* Updates the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_update_integrity_hash(
     imaging_handle_t *imaging_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_update_integrity_hash";
	void *data            = NULL;
	size_t data_size      = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
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
	if( ( read_size == 0 )
	 || ( read_size > (size_t) SSIZE_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid size value out of range.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer_get_data(
	     storage_media_buffer,
	     &data,
	     &data_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine storage media buffer data.",
		 function );

		return( -1 );
	}
	if( imaging_handle->calculate_md5 != 0 )
	{
		/* TODO check for return value */
		md5_update(
		 &( imaging_handle->md5_context ),
		 data,
		 read_size,
		 error );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update MD5 digest hash.",
			 function );

			return( -1 );
		}
	}
	if( imaging_handle->calculate_sha1 != 0 )
	{
		/* TODO check for return value */
		sha1_update(
		 &( imaging_handle->sha1_context ),
		 data,
		 read_size,
		 error );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update SHA1 digest hash.",
			 function );

			return( -1 );
		}
	}
	imaging_handle->output_offset += read_size;

	return( 1 );
}

/* Closes the imaging handle
 * Returns the 0 if succesful or -1 on error
 */
int imaging_handle_close(
     imaging_handle_t *imaging_handle,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_close";
	int result            = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_close(
	     imaging_handle->output_handle,
	     error ) != 0 )
#else
	if( libewf_close(
	     imaging_handle->output_handle ) != 0 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close output handle.",
		 function );

		result = -1;
	}
	return( result );
}

/* Sets the input values of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_input_values(
     imaging_handle_t *imaging_handle,
     uint8_t read_error_retry,
     uint8_t wipe_chunk_on_error,
     uint8_t seek_on_error,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_set_input_values";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	imaging_handle->read_error_retry    = read_error_retry;
	imaging_handle->wipe_chunk_on_error = wipe_chunk_on_error;
	imaging_handle->seek_on_error       = seek_on_error;

	return( 1 );
}

/* Sets the output values of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_output_values(
     imaging_handle_t *imaging_handle,
     uint32_t sectors_per_chunk,
     uint32_t bytes_per_sector,
     size64_t media_size,
     uint32_t error_granularity,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_set_output_values";

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_sectors_per_chunk(
	     imaging_handle->output_handle,
	     sectors_per_chunk,
	     error ) != 1 )
#else
	if( libewf_set_sectors_per_chunk(
	     imaging_handle->output_handle,
	     sectors_per_chunk ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sectors per chunk in output handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_bytes_per_sector(
	     imaging_handle->output_handle,
	     bytes_per_sector,
	     error ) != 1 )
#else
	if( libewf_set_bytes_per_sector(
	     imaging_handle->output_handle,
	     bytes_per_sector ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set bytes per sector in output handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_media_size(
	     imaging_handle->output_handle,
	     media_size,
	     error ) != 1 )
#else
	if( libewf_set_media_size(
	     imaging_handle->output_handle,
	     media_size ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media size in output handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_error_granularity(
	     imaging_handle->output_handle,
	     error_granularity,
	     error ) != 1 )
#else
	if( libewf_set_error_granularity(
	     imaging_handle->output_handle,
	     error_granularity ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set error granularity in output handle.",
		 function );

		return( -1 );
	}
	/* TODO refactor */
	imaging_handle->bytes_per_sector    = bytes_per_sector;
	imaging_handle->media_size          = media_size;
	imaging_handle->error_granularity   = error_granularity;
	imaging_handle->chunk_size          = sectors_per_chunk * bytes_per_sector;

	return( 1 );
}

/* Finalizes the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_finalize(
     imaging_handle_t *imaging_handle,
     system_character_t *calculated_md5_hash_string,
     size_t calculated_md5_hash_string_size,
     system_character_t *calculated_sha1_hash_string,
     size_t calculated_sha1_hash_string_size,
     liberror_error_t **error )
{
#if defined( USE_LIBEWF_GET_MD5_HASH )
        digest_hash_t stored_md5_hash[ DIGEST_HASH_SIZE_MD5 ];
#endif

	digest_hash_t calculated_md5_hash[ DIGEST_HASH_SIZE_MD5 ];
	digest_hash_t calculated_sha1_hash[ DIGEST_HASH_SIZE_SHA1 ];

	static char *function            = "imaging_handle_finalize";
	size_t calculated_md5_hash_size  = DIGEST_HASH_SIZE_MD5;
	size_t calculated_sha1_hash_size = DIGEST_HASH_SIZE_SHA1;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing output handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->calculate_md5 != 0 )
	{
		/* Finalize the MD5 hash calculation
		 */
		if( md5_finalize(
		     &( imaging_handle->md5_context ),
		     calculated_md5_hash,
		     &calculated_md5_hash_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize MD5 hash.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_md5_hash,
		     calculated_md5_hash_size,
		     calculated_md5_hash_string,
		     calculated_md5_hash_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set calculated MD5 hash string.",
			 function );

			return( -1 );
		}
	}
	if( imaging_handle->calculate_sha1 != 0 )
	{
		/* Finalize the SHA1 hash calculation
		 */
		if( sha1_finalize(
		     &( imaging_handle->sha1_context ),
		     calculated_sha1_hash,
		     &calculated_sha1_hash_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize SHA1 hash.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_sha1_hash,
		     calculated_sha1_hash_size,
		     calculated_sha1_hash_string,
		     calculated_sha1_hash_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create calculated SHA1 hash string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

