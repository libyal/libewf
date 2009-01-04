/* 
 * Verification handle
 *
 * Copyright (C) 2007-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include "file_io.h"
#include "storage_media_buffer.h"
#include "md5.h"
#include "notify.h"
#include "sha1.h"
#include "verification_handle.h"

/* Initializes the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_initialize(
     verification_handle_t **verification_handle,
     liberror_error_t **error )
{
	static char *function = "verification_handle_allocate";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
		 function );

		return( -1 );
	}
	if( *verification_handle == NULL )
	{
		*verification_handle = (verification_handle_t *) memory_allocate(
								  sizeof( verification_handle_t ) );

		if( *verification_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create verification handle.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *verification_handle,
		     0,
		     sizeof( verification_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear verification handle.\n",
			 function );

			memory_free(
			 *verification_handle );

			*verification_handle = NULL;

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_initialize(
		     &( ( *verification_handle )->input_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input handle.\n",
			 function );

			memory_free(
			 *verification_handle );

			*verification_handle = NULL;

			return( -1 );
		}
#endif
		if( md5_initialize(
		     &( ( *verification_handle )->md5_context ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize MD5 context.\n",
			 function );

#if defined( HAVE_V2_API )
			libewf_handle_free(
			 &( ( *verification_handle )->input_handle ),
			 NULL );
#endif
			memory_free(
			 *verification_handle );

			*verification_handle = NULL;

			return( -1 );
		}
		( *verification_handle )->input_offset  = 0;
		( *verification_handle )->output_offset = 0;
	}
	return( 1 );
}

/* Frees the verification handle and its elements
 * Returns 1 if successful or -1 on error
 */
int verification_handle_free(
     verification_handle_t **verification_handle,
     liberror_error_t **error )
{
	static char *function = "verification_handle_free";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
		 function );

		return( -1 );
	}
	if( *verification_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( ( ( *verification_handle )->input_handle != NULL )
		 && ( libewf_handle_free(
		       &( ( *verification_handle )->input_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input handle.\n",
			 function );
		}
#endif
		memory_free(
		 *verification_handle );

		*verification_handle = NULL;
	}
	return( 1 );
}

/* Opens the input of the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_open_input(
     verification_handle_t *verification_handle,
     const char *filename,
     size_t length_filename,
     liberror_error_t **error )
{
	char **filenames      = NULL;
	static char *function = "verification_handle_open_input";
	int amount_of_files   = 0;
	int result            = 1;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	amount_of_files = libewf_glob(
	                   filename,
	                   length_filename,
	                   LIBEWF_FORMAT_UNKNOWN,
	                   &filenames,
	                   error );
#else
	amount_of_files = libewf_glob(
	                   filename,
	                   length_filename,
	                   LIBEWF_FORMAT_UNKNOWN,
	                   &filenames );
#endif

	if( amount_of_files < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to glob filenames.\n",
		 function );

		memory_free(
		 filenames );

		return( -1 );
	}
	else if( amount_of_files == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: no files found.\n",
		 function );

		memory_free(
		 filenames );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_open(
	     verification_handle->input_handle,
	     (char * const *) filenames,
	     amount_of_files,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else

	verification_handle->input_handle = libewf_open(
	                                     (char * const *) filenames,
	                                     amount_of_files,
	                                     LIBEWF_OPEN_READ );

	if( verification_handle->input_handle == NULL )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open files.\n",
		 function );

		result = -1;
	}
	for( ; amount_of_files > 0; amount_of_files-- )
	{
		if( filenames[ amount_of_files - 1 ] != NULL )
		{
			memory_free(
			 filenames[ amount_of_files - 1 ] );
		}
	}
	memory_free(
	 filenames );

	return( result );
}

#if defined( HAVE_RAW_ACCESS )

/* Prepares a buffer after reading the input of the verification handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t verification_handle_read_prepare_buffer(
         verification_handle_t *verification_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "verification_handle_read_prepare_buffer";
	ssize_t read_count    = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.\n",
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
#if defined( HAVE_V2_API )
	read_count = libewf_raw_read_prepare_buffer(
                      verification_handle->input_handle,
                      storage_media_buffer->raw_buffer,
                      storage_media_buffer->raw_buffer_amount,
                      storage_media_buffer->compressed_buffer,
                      &( storage_media_buffer->compressed_buffer_amount ),
                      storage_media_buffer->is_compressed,
                      storage_media_buffer->process_crc,
	              error );
#else
	read_count = libewf_raw_read_prepare_buffer(
                      verification_handle->input_handle,
                      storage_media_buffer->raw_buffer,
                      storage_media_buffer->raw_buffer_amount,
                      storage_media_buffer->compressed_buffer,
                      &( storage_media_buffer->compressed_buffer_amount ),
                      storage_media_buffer->is_compressed,
                      storage_media_buffer->process_crc );
#endif

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to prepare storage media buffer after reading.\n",
		 function );

		return( -1 );
	}
	return( read_count );
}

#endif

/* Reads a buffer from the input of the verification handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t verification_handle_read_buffer(
         verification_handle_t *verification_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error )
{
	static char *function = "verification_handle_read_buffer";
	ssize_t read_count    = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.\n",
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
#if defined( HAVE_RAW_ACCESS )
#if defined( HAVE_V2_API )
	read_count = libewf_raw_read_buffer(
                      verification_handle->input_handle,
                      storage_media_buffer->raw_buffer,
                      read_size,
	              error );
#else
	read_count = libewf_raw_read_buffer(
                      verification_handle->input_handle,
                      storage_media_buffer->raw_buffer,
                      read_size );
#endif
#else
#if defined( HAVE_V2_API )
	read_count = libewf_read_buffer(
                      verification_handle->input_handle,
                      storage_media_buffer->raw_buffer,
                      read_size,
	              error );
#else
	read_count = libewf_read_buffer(
                      verification_handle->input_handle,
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
		 "%s: unable to read storage media buffer.\n",
		 function );

		return( -1 );
	}
	storage_media_buffer->raw_buffer_amount = read_count;
	storage_media_buffer->input_size        = read_count;
	storage_media_buffer->offset            = verification_handle->input_offset;
	verification_handle->input_offset      += read_count;

	return( read_count );
}

/* Updates the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int verification_handle_update_integrity_hash(
     verification_handle_t *verification_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t size,
     liberror_error_t **error )
{
	static char *function = "verification_handle_update_integrity_hash";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
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
	if( ( size == 0 )
	 || ( size > (size_t) SSIZE_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid size value out of range.\n",
		 function );

		return( -1 );
	}
	/* TODO check for return value */
	md5_update(
	 &( verification_handle->md5_context ),
	 storage_media_buffer->raw_buffer,
	 size,
	 error );

	if( ( error != NULL )
	 && ( *error != NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update MD5.\n",
		 function );

		return( -1 );
	}
	verification_handle->output_offset += size;

	return( 1 );
}

/* Closes both the input of the verification handle
 * Returns the 0 if succesful or -1 on error
 */
int verification_handle_close(
     verification_handle_t *verification_handle,
     liberror_error_t **error )
{
	static char *function = "verification_handle_close";
	int result            = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_close(
	     verification_handle->input_handle,
	     error ) != 0 )
#else
	if( libewf_close(
	     verification_handle->input_handle ) != 0 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.\n",
		 function );

		result = -1;
	}
	return( result );
}

/* Retrieves several verification values
 * The chunk size is set to 0 if not available
 * Returns 1 if successful or -1 on error
 */
int verification_handle_get_values(
     verification_handle_t *verification_handle,
     size64_t *media_size,
     uint32_t *chunk_size,
     liberror_error_t **error )
{
	static char *function = "verification_handle_get_values";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_get_media_size(
	     verification_handle->input_handle,
	     media_size,
	     error ) != 1 )
#else
	if( libewf_get_media_size(
	     verification_handle->input_handle,
	     media_size ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_get_chunk_size(
	     verification_handle->input_handle,
	     chunk_size,
	     error ) != 1 )
#else
	if( libewf_get_chunk_size(
	     verification_handle->input_handle,
	     chunk_size ) != 1 )
#endif
	{
		liberror_error_free(
		 error );

		*chunk_size = 0;
	}
	return( 1 );
}

/* Sets the input values of the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_set_input_values(
     verification_handle_t *verification_handle,
     int format,
     liberror_error_t **error )
{
	static char *function = "verification_handle_set_input_values";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_set_format(
	     verification_handle->input_handle,
	     format,
	     error ) != 1 )
#else
	if( libewf_set_format(
	     verification_handle->input_handle,
	     format ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set format in input handle.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Finalizes the verification handle
 * Returns 1 if the verification was successful, 0 if not or -1 on error
 */
int verification_handle_finalize(
     verification_handle_t *verification_handle,
     liberror_error_t **error )
{
	digest_hash_t calculated_md5_hash[ DIGEST_HASH_SIZE_MD5 ];
	char calculated_md5_hash_string[ 33 ];
	uint8_t stored_md5_hash_string[ 33 ];

	static char *function         = "verification_handle_finalize";
	size_t md5_hash_size          = DIGEST_HASH_SIZE_MD5;
	size_t md5_hash_string_length = 33;
	int result                    = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.\n",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.\n",
		 function );

		return( -1 );
	}
	/* Finalize the MD5 hash calculation
	 */
	if( md5_finalize(
	     &( verification_handle->md5_context ),
             calculated_md5_hash,
             &md5_hash_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize MD5 hash.\n",
		 function );

		return( -1 );
	}
	if( digest_hash_copy_to_string(
	     calculated_md5_hash,
	     md5_hash_size,
	     calculated_md5_hash_string,
	     md5_hash_string_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create calculated MD5 hash string.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	result = libewf_get_hash_value(
	          verification_handle->input_handle,
	          (uint8_t *) "MD5",
	          3,
	          stored_md5_hash_string,
	          md5_hash_string_length,
	          error );
#else
	result = libewf_get_hash_value(
	          verification_handle->input_handle,
	          "MD5",
	          (char *) stored_md5_hash_string,
	          md5_hash_string_length );
#endif
	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to retrieve stored MD5 hash string.\n",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( memory_compare(
	     calculated_md5_hash_string,
	     stored_md5_hash_string,
	     md5_hash_string_length ) != 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: calculated: %s and stored: %s MD5 hash string do not match.\n",
		 function,
	         calculated_md5_hash_string,
		 (char *) stored_md5_hash_string );
#endif

		return( 0 );
	}
	return( 1 );
}

