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

#if defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_EXPORT
#endif

#include <libewf.h>

#include "digest_context.h"
#include "digest_hash.h"
#include "ewfcommon.h"
#include "file_io.h"
#include "imaging_handle.h"
#include "md5.h"
#include "sha1.h"
#include "storage_media_buffer.h"
#include "system_string.h"

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

/* Signals the imaging handle to abort
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_signal_abort(
     imaging_handle_t *imaging_handle,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_signal_abort";

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
#if defined( HAVE_V2_API )
	if( libewf_handle_signal_abort(
	     imaging_handle->output_handle,
	     error ) != 1 )
#else
	if( libewf_signal_abort(
	     imaging_handle->output_handle ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal output handle to abort.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the output of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_open_output(
     imaging_handle_t *imaging_handle,
     const system_character_t *filename,
     liberror_error_t **error )
{
	system_character_t *filenames[ 1 ] = { NULL };
	static char *function              = "imaging_handle_open_output";

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
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filenames[ 0 ] = (system_character_t *) filename;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#if defined( HAVE_V2_API )
	if( libewf_handle_open_wide(
	     imaging_handle->output_handle,
	     filenames,
	     1,
	     LIBEWF_OPEN_WRITE,
	     error ) != 1 )
#else
	imaging_handle->output_handle = libewf_open_wide(
	                                 filenames,
	                                 1,
	                                 LIBEWF_OPEN_WRITE );

	if( imaging_handle->output_handle == NULL )
#endif
#else
#if defined( HAVE_V2_API )
	if( libewf_handle_open(
	     imaging_handle->output_handle,
	     filenames,
	     1,
	     LIBEWF_OPEN_WRITE,
	     error ) != 1 )
#else
	imaging_handle->output_handle = libewf_open(
	                                 filenames,
	                                 1,
	                                 LIBEWF_OPEN_WRITE );

	if( imaging_handle->output_handle == NULL )
#endif
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( 1 );
	}
	return( 1 );
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
	ssize_t process_count = 0;

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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	storage_media_buffer->compression_buffer_amount = storage_media_buffer->compression_buffer_size;

#if defined( HAVE_V2_API )
	process_count = libewf_handle_prepare_write_chunk(
	                 imaging_handle->output_handle,
	                 storage_media_buffer->raw_buffer,
	                 storage_media_buffer->raw_buffer_amount,
	                 storage_media_buffer->compression_buffer,
	                 (size_t *) &( storage_media_buffer->compression_buffer_amount ),
	                 &( storage_media_buffer->is_compressed ),
	                 &( storage_media_buffer->crc ),
	                 &( storage_media_buffer->process_crc ),
	                 error );
#else
	process_count = libewf_raw_write_prepare_buffer(
	                 imaging_handle->output_handle,
	                 storage_media_buffer->raw_buffer,
	                 storage_media_buffer->raw_buffer_amount,
	                 storage_media_buffer->compression_buffer,
	                 (size_t *) &( storage_media_buffer->compression_buffer_amount ),
	                 &( storage_media_buffer->is_compressed ),
	                 &( storage_media_buffer->crc ),
	                 &( storage_media_buffer->process_crc ) );
#endif

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

/* Writes a buffer to the output of the imaging handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t imaging_handle_write_buffer(
         imaging_handle_t *imaging_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t write_size,
         liberror_error_t **error )
{
#if defined( HAVE_LOW_LEVEL_FUNCTIONS ) && defined( HAVE_V2_API )
	uint8_t crc_buffer[ 4 ];
#endif

	static char *function        = "imaging_handle_write_buffer";
	ssize_t write_count          = 0;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	uint8_t *raw_write_buffer    = NULL;
	size_t raw_write_buffer_size = 0;
#endif

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
		 "%s: mismatch in write size and amount of bytes in storage media buffer.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	write_count = libewf_handle_write_chunk(
	               imaging_handle->output_handle,
	               raw_write_buffer,
	               raw_write_buffer_size,
	               storage_media_buffer->raw_buffer_amount,
	               storage_media_buffer->is_compressed,
	               crc_buffer,
	               storage_media_buffer->crc,
	               storage_media_buffer->process_crc,
	               error );
#else
	write_count = libewf_raw_write_buffer(
	               imaging_handle->output_handle,
	               raw_write_buffer,
	               raw_write_buffer_size,
	               storage_media_buffer->raw_buffer_amount,
	               storage_media_buffer->is_compressed,
	               storage_media_buffer->crc,
	               storage_media_buffer->process_crc );
#endif
#else
#if defined( HAVE_V2_API )
	write_count = libewf_handle_write_buffer(
	               imaging_handle->output_handle,
	               storage_media_buffer->raw_buffer,
	               write_size,
	               error );
#else
	write_count = libewf_write_buffer(
	               imaging_handle->output_handle,
	               storage_media_buffer->raw_buffer,
	               write_size );
#endif
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
		 "%s: invalid read size value out of range.",
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
		 "%s: invalid read size value is odd.",
		 function );

		return( -1 );
	}
	if( read_size != (size_t) storage_media_buffer->raw_buffer_amount )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: mismatch in read size and buffer data amount.",
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
		 "%s: unable to retrieve storage media buffer data.",
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

		return( -1 );
	}
#if !defined( HAVE_V2_API )
	imaging_handle->output_handle = NULL;
#endif
	return( 0 );
}

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_get_chunk_size(
     imaging_handle_t *imaging_handle,
     size32_t *chunk_size,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_get_chunk_size";

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
	if( libewf_handle_get_chunk_size(
	     imaging_handle->output_handle,
	     chunk_size,
	     error ) != 1 )
#else
	if( libewf_get_chunk_size(
	     imaging_handle->output_handle,
	     chunk_size ) != 1 )
#endif
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

/* Sets the output values of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_output_values(
     imaging_handle_t *imaging_handle,
     system_character_t *case_number,
     system_character_t *description,
     system_character_t *evidence_number,
     system_character_t *examiner_name,
     system_character_t *notes,
     system_character_t *acquiry_operating_system,
     system_character_t *acquiry_software,
     system_character_t *acquiry_software_version,
     uint32_t bytes_per_sector,
     size64_t media_size,
     uint8_t media_type,
     uint8_t volume_type,
     int8_t compression_level,
     uint8_t compression_flags,
     uint8_t libewf_format,
     size64_t segment_file_size,
     uint32_t sectors_per_chunk,
     uint32_t sector_error_granularity,
     liberror_error_t **error )
{
#if defined( HAVE_UUID_UUID_H )
	uint8_t guid[ 16 ];
#endif

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
	/* Set case number
	 */
	if( imaging_handle_set_header_value(
	     imaging_handle,
	     "case_number",
	     11,
	     case_number,
	     system_string_length(
	      case_number ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: case number.",
		 function );

		return( -1 );
	}
	/* Set description
	 */
	if( imaging_handle_set_header_value(
	     imaging_handle,
	     "description",
	     11,
	     description,
	     system_string_length(
	      description ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: description.",
		 function );

		return( -1 );
	}
	/* Set evidence number
	 */
	if( imaging_handle_set_header_value(
	     imaging_handle,
	     "evidence_number",
	     15,
	     evidence_number,
	     system_string_length(
	      evidence_number ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: evidence number.",
		 function );

		return( -1 );
	}
	/* Set examiner name
	 */
	if( imaging_handle_set_header_value(
	     imaging_handle,
	     "examiner_name",
	     13,
	     examiner_name,
	     system_string_length(
	      examiner_name ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: examiner name.",
		 function );

		return( -1 );
	}
	/* Set notes
	 */
	if( imaging_handle_set_header_value(
	     imaging_handle,
	     "notes",
	     5,
	     notes,
	     system_string_length(
	      notes ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: notes.",
		 function );

		return( -1 );
	}
	/* Password is not used within libewf
	 */

	/* Acquiry date, system date and compression type will be generated automatically when set to NULL
	 */

	/* Set acquiry operating system, software and software version
	 */
	if( ( acquiry_operating_system != NULL )
	 && ( imaging_handle_set_header_value(
	       imaging_handle,
	       "acquiry_operating_system",
	       24,
	       acquiry_operating_system,
	       system_string_length(
	        acquiry_operating_system ),
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry operating system.",
		 function );

		return( -1 );
	}
	if( imaging_handle_set_header_value(
	     imaging_handle,
	     "acquiry_software",
	     16,
	     acquiry_software,
	     system_string_length(
	      acquiry_software ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry software.",
		 function );

		return( -1 );
	}
	if( imaging_handle_set_header_value(
	     imaging_handle,
	     "acquiry_software_version",
	     24,
	     acquiry_software_version,
	     system_string_length(
	      acquiry_software_version ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry software version.",
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
		 "%s: unable to set bytes per sector.",
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
		 "%s: unable to set media size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_media_type(
	     imaging_handle->output_handle,
	     media_type,
	     error ) != 1 )
#else
	if( libewf_set_media_type(
	     imaging_handle->output_handle,
	     media_type ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_volume_type(
	     imaging_handle->output_handle,
	     volume_type,
	     error ) != 1 )
#else
	if( libewf_set_volume_type(
	     imaging_handle->output_handle,
	     volume_type ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set volume type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_compression_values(
	     imaging_handle->output_handle,
	     compression_level,
	     compression_flags,
	     error ) != 1 )
#else
	if( libewf_set_compression_values(
	     imaging_handle->output_handle,
	     compression_level,
	     compression_flags ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set compression values.",
		 function );

		return( -1 );
	}
	/* Format needs to be set before segment file size
	 */
#if defined( HAVE_V2_API )
	if( libewf_handle_set_format(
	     imaging_handle->output_handle,
	     libewf_format,
	     error ) != 1 )
#else
	if( libewf_set_format(
	     imaging_handle->output_handle,
	     libewf_format ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set format.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_segment_file_size(
	     imaging_handle->output_handle,
	     segment_file_size,
	     error ) != 1 )
#else
	if( libewf_set_segment_file_size(
	     imaging_handle->output_handle,
	     segment_file_size ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment file size.",
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
	if( libewf_handle_set_error_granularity(
	     imaging_handle->output_handle,
	     sector_error_granularity,
	     error ) != 1 )
#else
	if( libewf_set_error_granularity(
	     imaging_handle->output_handle,
	     sector_error_granularity ) != 1 )
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
#if defined( HAVE_UUID_UUID_H )
	/* Add a system GUID if necessary
	 */
	if( ewfcommon_determine_guid(
	     guid,
	     libewf_format ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create GUID.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_guid(
	     imaging_handle->output_handle,
	     guid,
	     16,
	     error ) != 1 )
#else
	if( libewf_set_guid(
	     imaging_handle->output_handle,
	     guid,
	     16 ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set GUID.",
		 function );

		return( -1 );
	}
#endif
	/* TODO for now just fake one session
	 */
	if( media_type == LIBEWF_MEDIA_TYPE_OPTICAL )
	{
		if( media_size > (size_t) UINT32_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid media size value out of range to add session.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_add_session(
		     imaging_handle->output_handle,
		     1,
		     (uint32_t) media_size,
		     error ) != 1 )
#else
		if( libewf_add_session(
		     imaging_handle->output_handle,
		     1,
		     (uint32_t) media_size ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable add session.",
			 function );

			return( -1 );
		}
	}
	imaging_handle->bytes_per_sector = bytes_per_sector;

	return( 1 );
}

/* Sets the header value in the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_header_value(
     imaging_handle_t *imaging_handle,
     char *header_value_identifier,
     size_t header_value_identifier_length,
     system_character_t *header_value,
     size_t header_value_length,
     liberror_error_t **error )
{
	uint8_t *utf8_header_value    = NULL;
	static char *function         = "imaging_handle_set_header_value";
	size_t utf8_header_value_size = 0;

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
	if( utf8_string_size_from_system_string(
	     header_value,
	     header_value_length + 1,
	     &utf8_header_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 header value size.",
		 function );

		return( -1 );
	}
	utf8_header_value = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * utf8_header_value_size );

	if( utf8_header_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 header value.",
		 function );

		return( -1 );
	}
	if( utf8_string_copy_from_system_string(
	     utf8_header_value,
	     utf8_header_value_size,
	     header_value,
	     header_value_length + 1,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 header value.",
		 function );

		memory_free(
		 utf8_header_value );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_header_value(
	     imaging_handle->output_handle,
	     (uint8_t *) header_value_identifier,
	     header_value_identifier_length,
	     utf8_header_value,
	     utf8_header_value_size - 1,
	     error ) != 1 )
#else
	if( libewf_set_header_value(
	     imaging_handle->output_handle,
	     header_value_identifier,
	     (char *) utf8_header_value,
	     utf8_header_value_size - 1 ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: %s.",
		 function,
		 header_value_identifier );

		memory_free(
		 utf8_header_value );

		return( -1 );
	}
	memory_free(
	 utf8_header_value );

	return( 1 );
}

/* Sets the hash value in the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_hash_value(
     imaging_handle_t *imaging_handle,
     char *hash_value_identifier,
     size_t hash_value_identifier_length,
     system_character_t *hash_value,
     size_t hash_value_length,
     liberror_error_t **error )
{
	uint8_t *utf8_hash_value    = NULL;
	static char *function       = "imaging_handle_set_hash_value";
	size_t utf8_hash_value_size = 0;

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
	if( utf8_string_size_from_system_string(
	     hash_value,
	     hash_value_length + 1,
	     &utf8_hash_value_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 hash value size.",
		 function );

		return( -1 );
	}
	utf8_hash_value = (uint8_t *) memory_allocate(
	                               sizeof( uint8_t ) * utf8_hash_value_size );

	if( utf8_hash_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 hash value.",
		 function );

		return( -1 );
	}
	if( utf8_string_copy_from_system_string(
	     utf8_hash_value,
	     utf8_hash_value_size,
	     hash_value,
	     hash_value_length + 1,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 hash value.",
		 function );

		memory_free(
		 utf8_hash_value );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_hash_value(
	     imaging_handle->output_handle,
	     (uint8_t *) hash_value_identifier,
	     hash_value_identifier_length,
	     utf8_hash_value,
	     utf8_hash_value_size - 1,
	     error ) != 1 )
#else
	if( libewf_set_hash_value(
	     imaging_handle->output_handle,
	     hash_value_identifier,
	     (char *) utf8_hash_value,
	     utf8_hash_value_size - 1 ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set hash value: %s.",
		 function,
		 hash_value_identifier );

		memory_free(
		 utf8_hash_value );

		return( -1 );
	}
	memory_free(
	 utf8_hash_value );

	return( 1 );
}

/* Appends a read error to the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_add_read_error(
      imaging_handle_t *imaging_handle,
      off64_t start_offset,
      size_t amount_of_bytes,
      liberror_error_t **error )
{
	static char *function      = "imaging_handle_add_read_error";
	off64_t start_sector       = 0;
	uint32_t amount_of_sectors = 0;

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
	if( imaging_handle->bytes_per_sector == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid imaging handle - invalid bytes per sector value out of range.",
		 function );

		return( -1 );
	}
	start_sector      = start_offset / imaging_handle->bytes_per_sector;
	amount_of_sectors = (uint32_t) ( amount_of_bytes / imaging_handle->bytes_per_sector );

#if defined( HAVE_V2_API )
	if( libewf_handle_add_acquiry_error(
	     imaging_handle->output_handle,
	     start_sector,
	     amount_of_sectors,
	     error ) != 1 )
#else
	if( libewf_add_acquiry_error(
	     imaging_handle->output_handle,
	     start_sector,
	     amount_of_sectors ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to add acquiry errror.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}
 
/* Finalizes the imaging handle
 * Returns the amount of input bytes written or -1 on error
 */
ssize_t imaging_handle_finalize(
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
	ssize_t write_count              = 0;

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
		if( imaging_handle_set_hash_value(
		     imaging_handle,
		     "MD5",
		     3,
		     calculated_md5_hash_string,
		     calculated_md5_hash_string_size - 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: MD5.",
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
		if( imaging_handle_set_hash_value(
		     imaging_handle,
		     "SHA1",
		     5,
		     calculated_sha1_hash_string,
		     calculated_sha1_hash_string_size - 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: SHA1.",
			 function );

			return( -1 );
		}
	}
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
		 "%s: unable to finalize EWF file(s).",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Print the acquiry errors to a stream
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_acquiry_errors_fprint(
     imaging_handle_t *imaging_handle,
     FILE *stream,
     liberror_error_t **error )
{
	static char *function      = "imaging_handle_acquiry_errors_fprint";
	off64_t first_sector       = 0;
	uint32_t amount_of_errors  = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t error_iterator    = 0;
	int result                 = 1;

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
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_amount_of_acquiry_errors(
	     imaging_handle->output_handle,
	     &amount_of_errors,
	     error ) == -1 )
#else
	if( libewf_get_amount_of_acquiry_errors(
	     imaging_handle->output_handle,
	     &amount_of_errors ) == -1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of acquiry errors.",
		 function );

		return( -1 );
	}
	if( amount_of_errors > 0 )
	{
		fprintf(
		 stream,
		 "Read errors during acquiry:\n" );
		fprintf(
		 stream,
		 "\ttotal amount: %" PRIu32 "\n",
		 amount_of_errors );
		
		for( error_iterator = 0; error_iterator < amount_of_errors; error_iterator++ )
		{
#if defined( HAVE_V2_API )
			if( libewf_handle_get_acquiry_error(
			     imaging_handle->output_handle,
			     error_iterator,
			     &first_sector,
			     &amount_of_sectors,
			     error ) != 1 )
#else
			if( libewf_get_acquiry_error(
			     imaging_handle->output_handle,
			     error_iterator,
			     &first_sector,
			     &amount_of_sectors ) != 1 )
#endif
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the acquiry error: %" PRIu32 ".",
				 function,
				 error_iterator );

				first_sector      = 0;
				amount_of_sectors = 0;

				result = -1;
			}
			fprintf(
			 stream,
			 "\tin sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu32 "\n",
			 (uint64_t) first_sector,
			 (uint64_t) ( first_sector + amount_of_sectors ),
			 amount_of_sectors );
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

