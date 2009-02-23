/* 
 * Verification handle
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
#include "md5.h"
#include "notify.h"
#include "sha1.h"
#include "storage_media_buffer.h"
#include "system_string.h"
#include "verification_handle.h"

#if !defined( USE_LIBEWF_GET_HASH_VALUE_MD5 ) && !defined( USE_LIBEWF_GET_MD5_HASH )
#define USE_LIBEWF_GET_HASH_VALUE_MD5
#endif

/* Initializes the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_initialize(
     verification_handle_t **verification_handle,
     uint8_t calculate_md5,
     uint8_t calculate_sha1,
     liberror_error_t **error )
{
	static char *function = "verification_handle_initialize";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
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
			 "%s: unable to create verification handle.",
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
			 "%s: unable to clear verification handle.",
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
			 "%s: unable to initialize input handle.",
			 function );

			memory_free(
			 *verification_handle );

			*verification_handle = NULL;

			return( -1 );
		}
#endif
		( *verification_handle )->calculate_md5  = calculate_md5;
		( *verification_handle )->calculate_sha1 = calculate_sha1;

		if( ( ( *verification_handle )->calculate_md5 != 0 )
		 && ( md5_initialize(
		       &( ( *verification_handle )->md5_context ),
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
			 &( ( *verification_handle )->input_handle ),
			 NULL );
#endif
			memory_free(
			 *verification_handle );

			*verification_handle = NULL;

			return( -1 );
		}
		if( ( ( *verification_handle )->calculate_sha1 != 0 )
		 && ( sha1_initialize(
		       &( ( *verification_handle )->sha1_context ),
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
			 &( ( *verification_handle )->input_handle ),
			 NULL );
#endif
			memory_free(
			 *verification_handle );

			*verification_handle = NULL;

			return( -1 );
		}
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
		 "%s: invalid verification handle.",
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
			 "%s: unable to free input handle.",
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
     char * const * filenames,
     int amount_of_files,
     liberror_error_t **error )
{
	static char *function = "verification_handle_open_input";
	int result            = 1;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
#else
	if( verification_handle->input_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid verification handle - input handle already set.",
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
	     verification_handle->input_handle,
	     filenames,
	     amount_of_files,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	verification_handle->input_handle = libewf_open(
	                                     filenames,
	                                     amount_of_files,
	                                     LIBEWF_OPEN_READ );

	if( verification_handle->input_handle == NULL )
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
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
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
	read_count = libewf_handle_raw_read_prepare_buffer(
                      verification_handle->input_handle,
                      storage_media_buffer->compression_buffer,
                      storage_media_buffer->compression_buffer_amount,
                      storage_media_buffer->raw_buffer,
                      (size_t *) &( storage_media_buffer->raw_buffer_amount ),
                      storage_media_buffer->is_compressed,
                      storage_media_buffer->crc,
                      storage_media_buffer->process_crc,
	              error );
#else
	read_count = libewf_raw_read_prepare_buffer(
                      verification_handle->input_handle,
                      storage_media_buffer->compression_buffer,
                      storage_media_buffer->compression_buffer_amount,
                      storage_media_buffer->raw_buffer,
                      (size_t *) &( storage_media_buffer->raw_buffer_amount ),
                      storage_media_buffer->is_compressed,
                      storage_media_buffer->crc,
                      storage_media_buffer->process_crc );
#endif

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
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
#else
	read_count = storage_media_buffer->raw_buffer_amount;
#endif
	storage_media_buffer->input_size   = read_count;
	verification_handle->input_offset += read_count;

	return( read_count );
}

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
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
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
	read_count = libewf_handle_raw_read_buffer(
                      verification_handle->input_handle,
                      storage_media_buffer->compression_buffer,
                      storage_media_buffer->compression_buffer_size,
	              &( storage_media_buffer->is_compressed ),
	              &( storage_media_buffer->crc ),
	              &( storage_media_buffer->process_crc ),
	              error );
#else
	read_count = libewf_raw_read_buffer(
	              verification_handle->input_handle,
	              storage_media_buffer->compression_buffer,
	              storage_media_buffer->compression_buffer_size,
	              &( storage_media_buffer->is_compressed ),
	              &( storage_media_buffer->crc ),
	              &( storage_media_buffer->process_crc ) );
#endif
#else
#if defined( HAVE_V2_API )
	read_count = libewf_handle_read_buffer(
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
		 "%s: unable to read storage media buffer.",
		 function );

		return( -1 );
	}
	storage_media_buffer->offset = verification_handle->input_offset;

#if defined( HAVE_RAW_ACCESS )
	storage_media_buffer->compression_buffer_amount = read_count;
#else
	storage_media_buffer->raw_buffer_amount         = read_count;
#endif
	return( read_count );
}

/* Updates the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int verification_handle_update_integrity_hash(
     verification_handle_t *verification_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error )
{
	static char *function = "verification_handle_update_integrity_hash";
	void *data            = NULL;
	size_t data_size      = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
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
	if( verification_handle->calculate_md5 != 0 )
	{
		/* TODO check for return value */
		md5_update(
		 &( verification_handle->md5_context ),
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
	if( verification_handle->calculate_sha1 != 0 )
	{
		/* TODO check for return value */
		sha1_update(
		 &( verification_handle->sha1_context ),
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
	verification_handle->output_offset += read_size;

	return( 1 );
}

/* Closes the verification handle
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
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_close(
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
		 "%s: unable to close input handle.",
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
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
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
#if defined( HAVE_V2_API )
	if( libewf_handle_get_media_size(
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
		 "%s: unable to retrieve media size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_chunk_size(
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
     int wipe_chunk_on_error,
     liberror_error_t **error )
{
	static char *function = "verification_handle_set_input_values";

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_read_wipe_chunk_on_error(
	     verification_handle->input_handle,
	     wipe_chunk_on_error,
	     error ) != 1 )
#else
	if( libewf_set_read_wipe_chunk_on_error(
	     verification_handle->input_handle,
	     wipe_chunk_on_error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set wipe chunk on error in input handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Finalizes the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_finalize(
     verification_handle_t *verification_handle,
     system_character_t *calculated_md5_hash_string,
     size_t calculated_md5_hash_string_size,
     system_character_t *stored_md5_hash_string,
     size_t stored_md5_hash_string_size,
     int *stored_md5_hash_available,
     system_character_t *calculated_sha1_hash_string,
     size_t calculated_sha1_hash_string_size,
     system_character_t *stored_sha1_hash_string,
     size_t stored_sha1_hash_string_size,
     int *stored_sha1_hash_available,
     liberror_error_t **error )
{
#if defined( USE_LIBEWF_GET_MD5_HASH )
        digest_hash_t stored_md5_hash[ DIGEST_HASH_SIZE_MD5 ];
#endif

	digest_hash_t calculated_md5_hash[ DIGEST_HASH_SIZE_MD5 ];
	digest_hash_t calculated_sha1_hash[ DIGEST_HASH_SIZE_SHA1 ];

	static char *function            = "verification_handle_finalize";
	size_t calculated_md5_hash_size  = DIGEST_HASH_SIZE_MD5;
	size_t calculated_sha1_hash_size = DIGEST_HASH_SIZE_SHA1;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( USE_LIBEWF_GET_MD5_HASH )
	if( verification_handle->calculate_sha1 != 0 )
	{
		if( libewf_parse_hash_values(
		     verification_handle->input_handle ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
	}
#elif defined( USE_LIBEWF_GET_HASH_VALUE_MD5 )
	if( libewf_parse_hash_values(
	     verification_handle->input_handle ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to parse hash values.",
		 function );

		return( -1 );
	}
#endif
	if( verification_handle->calculate_md5 != 0 )
	{
		/* Finalize the MD5 hash calculation
		 */
		if( md5_finalize(
		     &( verification_handle->md5_context ),
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
		if( stored_md5_hash_available == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid stored MD5 hash available value.",
			 function );

			return( -1 );
		}
#if defined( USE_LIBEWF_GET_MD5_HASH )
#if defined( HAVE_V2_API )
		*stored_md5_hash_available = libewf_handle_get_md5_hash(
					      verification_handle->input_handle,
					      md5_hash,
					      DIGEST_HASH_SIZE_MD5,
					      error );
#else
		*stored_md5_hash_available = libewf_get_md5_hash(
					      verification_handle->input_handle,
					      md5_hash,
					      DIGEST_HASH_SIZE_MD5 );
#endif

		if( *stored_md5_hash_available == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine stored MD5 hash string.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     md5_hash,
		     DIGEST_HASH_SIZE_MD5,
		     stored_md5_hash_string,
		     stored_md5_hash_string_size ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set stored MD5 hash string.",
			 function );

			return( -1 );
		}
#elif defined( USE_LIBEWF_GET_HASH_VALUE_MD5 )
		*stored_md5_hash_available = ewflibewf_get_hash_value(
		                              verification_handle->input_handle,
		                              "MD5",
		                              3,
		                              stored_md5_hash_string,
		                              stored_md5_hash_string_size,
		                              error );

		if( *stored_md5_hash_available == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine stored MD5 hash string.",
			 function );

			return( -1 );
		}
#endif
	}
	if( verification_handle->calculate_sha1 != 0 )
	{
		/* Finalize the SHA1 hash calculation
		 */
		if( sha1_finalize(
		     &( verification_handle->sha1_context ),
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
		if( stored_sha1_hash_available == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid stored SHA1 hash available value.",
			 function );

			return( -1 );
		}
		*stored_sha1_hash_available = ewflibewf_get_hash_value(
		                               verification_handle->input_handle,
		                               "SHA1",
		                               4,
		                               stored_sha1_hash_string,
		                               stored_sha1_hash_string_size,
		                               error );

		if( *stored_sha1_hash_available == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine stored SHA1 hash string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

