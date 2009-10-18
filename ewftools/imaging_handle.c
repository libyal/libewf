/* 
 * Imaging handle
 *
 * Copyright (C) 2007-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if defined( WINAPI )
#include <rpcdce.h>

#elif defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include <libsystem.h>

#include "digest_context.h"
#include "digest_hash.h"
#include "ewfcommon.h"
#include "guid.h"
#include "imaging_handle.h"
#include "md5.h"
#include "sha1.h"
#include "storage_media_buffer.h"

#if !defined( USE_LIBEWF_GET_HASH_VALUE_MD5 ) && !defined( USE_LIBEWF_GET_MD5_HASH )
#define USE_LIBEWF_GET_HASH_VALUE_MD5
#endif

#define IMAGING_HANDLE_VALUE_SIZE		512

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

			result = -1;
		}
		if( ( ( *imaging_handle )->secondary_output_handle != NULL )
		 && ( libewf_handle_free(
		       &( ( *imaging_handle )->secondary_output_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free secondary output handle.",
			 function );

			result = -1;
		}
#endif
		memory_free(
		 *imaging_handle );

		*imaging_handle = NULL;
	}
	return( result );
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
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{	
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
			 "%s: unable to signal secondary output handle to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Opens the output of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_open_output(
     imaging_handle_t *imaging_handle,
     const libsystem_character_t *filename,
     uint8_t resume,
     liberror_error_t **error )
{
	libsystem_character_t **libewf_filenames = NULL;
	libsystem_character_t *filenames[ 1 ]    = { NULL };
	static char *function                    = "imaging_handle_open_output";
	size_t first_filename_length             = 0;
	int amount_of_filenames                  = 0;
	int result                               = 1;
	uint8_t flags                            = 0;

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
	filenames[ 0 ]      = (libsystem_character_t *) filename;
	amount_of_filenames = 1;

	if( resume != 0 )
	{
		first_filename_length = libsystem_string_length(
		                         filenames[ 0 ] );

#if defined( HAVE_V2_API )
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
#else
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
		amount_of_filenames = libewf_glob_wide(
		                       filenames[ 0 ],
		                       first_filename_length,
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &libewf_filenames );
#else
		amount_of_filenames = libewf_glob(
		                       filenames[ 0 ],
		                       first_filename_length,
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &libewf_filenames );

#endif
		if( amount_of_filenames <= 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to resolve filename(s).",
			 function );

			return( -1 );
		}
#endif
		flags = LIBEWF_OPEN_WRITE_RESUME;
	}
	else
	{
		libewf_filenames = filenames;
		flags            = LIBEWF_OPEN_WRITE;
	}
#if defined( HAVE_V2_API )
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libewf_handle_open_wide(
	     imaging_handle->output_handle,
	     libewf_filenames,
	     amount_of_filenames,
	     flags,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     imaging_handle->output_handle,
	     libewf_filenames,
	     amount_of_filenames,
	     flags,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		result = -1;
	}
#else
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	imaging_handle->output_handle = libewf_open_wide(
	                                 libewf_filenames,
	                                 amount_of_filenames,
	                                 (uint8_t) flags );
#else
	imaging_handle->output_handle = libewf_open(
	                                 libewf_filenames,
	                                 amount_of_filenames,
	                                 (uint8_t) flags );
#endif
	if( imaging_handle->output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		result = -1;
	}
#endif
	if( libewf_filenames != filenames )
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

/* Opens the secondary output of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_open_secondary_output(
     imaging_handle_t *imaging_handle,
     const libsystem_character_t *filename,
     uint8_t resume,
     liberror_error_t **error )
{
	libsystem_character_t **libewf_filenames = NULL;
	libsystem_character_t *filenames[ 1 ]    = { NULL };
	static char *function                    = "imaging_handle_open_secondary_output";
	size_t first_filename_length             = 0;
	int amount_of_filenames                  = 0;
	int result                               = 1;
	uint8_t flags                            = 0;

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
	if( imaging_handle->secondary_output_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid imaging handle - secondary output handle already set.",
		 function );

		return( -1 );
	}
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
	filenames[ 0 ]      = (libsystem_character_t *) filename;
	amount_of_filenames = 1;

	if( resume != 0 )
	{
		first_filename_length = libsystem_string_length(
		                         filenames[ 0 ] );

#if defined( HAVE_V2_API )
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
#else
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
		amount_of_filenames = libewf_glob_wide(
		                       filenames[ 0 ],
		                       first_filename_length,
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &libewf_filenames );
#else
		amount_of_filenames = libewf_glob(
		                       filenames[ 0 ],
		                       first_filename_length,
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &libewf_filenames );

#endif
		if( amount_of_filenames <= 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to resolve filename(s).",
			 function );

			return( -1 );
		}
#endif
		flags = LIBEWF_OPEN_WRITE_RESUME;
	}
	else
	{
		libewf_filenames = filenames;
		flags            = LIBEWF_OPEN_WRITE;
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_initialize(
	     &( imaging_handle->secondary_output_handle ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize secondary output handle.",
		 function );

		return( -1 );
	}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libewf_handle_open_wide(
	     imaging_handle->secondary_output_handle,
	     libewf_filenames,
	     amount_of_filenames,
	     flags,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     imaging_handle->secondary_output_handle,
	     libewf_filenames,
	     amount_of_filenames,
	     flags,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		result = -1;
	}
#else
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	imaging_handle->secondary_output_handle = libewf_open_wide(
	                                           libewf_filenames,
	                                           amount_of_filenames,
	                                           (uint8_t) flags );
#else
	imaging_handle->secondary_output_handle = libewf_open(
	                                           libewf_filenames,
	                                           amount_of_filenames,
	                                           (uint8_t) flags );
#endif
	if( imaging_handle->secondary_output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		result = -1;
	}
#endif
	if( libewf_filenames != filenames )
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

	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_close(
		     imaging_handle->secondary_output_handle,
		     error ) != 0 )
#else
		if( libewf_close(
		     imaging_handle->secondary_output_handle ) != 0 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close secondary output handle.",
			 function );

			return( -1 );
		}
#if !defined( HAVE_V2_API )
		imaging_handle->secondary_output_handle = NULL;
#endif
	}
	return( 0 );
}

/* Prepares a buffer after reading the input of the imaging handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t imaging_handle_prepare_read_buffer(
         imaging_handle_t *imaging_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_prepare_read_buffer";
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
	storage_media_buffer->raw_buffer_amount = storage_media_buffer->raw_buffer_size;

#if defined( HAVE_V2_API )
	process_count = libewf_handle_prepare_read_chunk(
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
	process_count = libewf_raw_read_prepare_buffer(
	                 imaging_handle->output_handle,
	                 storage_media_buffer->compression_buffer,
	                 storage_media_buffer->compression_buffer_amount,
	                 storage_media_buffer->raw_buffer,
	                 (size_t *) &( storage_media_buffer->raw_buffer_amount ),
	                 storage_media_buffer->is_compressed,
	                 storage_media_buffer->crc,
	                 storage_media_buffer->process_crc );
#endif

	if( process_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read storage media buffer.",
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
	process_count = storage_media_buffer->raw_buffer_amount;
#endif
	return( process_count );
}

/* Reads a buffer from the input of the imaging handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t imaging_handle_read_buffer(
         imaging_handle_t *imaging_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error )
{
	static char *function        = "imaging_handle_read_buffer";
	ssize_t read_count           = 0;
	ssize_t secondary_read_count = 0;

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
#if defined( HAVE_V2_API )
	read_count = libewf_handle_read_chunk(
                      imaging_handle->output_handle,
                      storage_media_buffer->compression_buffer,
                      storage_media_buffer->compression_buffer_size,
	              &( storage_media_buffer->is_compressed ),
	              &( storage_media_buffer->compression_buffer[ storage_media_buffer->raw_buffer_size ] ),
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
	read_count = libewf_handle_read_buffer(
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
	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
#if defined( HAVE_V2_API )
		secondary_read_count = libewf_handle_read_chunk(
			                imaging_handle->secondary_output_handle,
			                storage_media_buffer->compression_buffer,
			                storage_media_buffer->compression_buffer_size,
			                &( storage_media_buffer->is_compressed ),
			                &( storage_media_buffer->compression_buffer[ storage_media_buffer->raw_buffer_size ] ),
			                &( storage_media_buffer->crc ),
			                &( storage_media_buffer->process_crc ),
			                error );
#else
		secondary_read_count = libewf_raw_read_buffer(
		                        imaging_handle->secondary_output_handle,
		                        storage_media_buffer->compression_buffer,
		                        storage_media_buffer->compression_buffer_size,
		                        &( storage_media_buffer->is_compressed ),
		                        &( storage_media_buffer->crc ),
		                        &( storage_media_buffer->process_crc ) );
#endif
#else
#if defined( HAVE_V2_API )
		secondary_read_count = libewf_handle_read_buffer(
		                        imaging_handle->secondary_output_handle,
		                        storage_media_buffer->raw_buffer,
		                        read_size,
		                        error );
#else
		secondary_read_count = libewf_read_buffer(
		                        imaging_handle->secondary_output_handle,
		                        storage_media_buffer->raw_buffer,
		                        read_size );
#endif
#endif
		if( secondary_read_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read storage media buffer from secondary output handle.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	storage_media_buffer->compression_buffer_amount = read_count;
#else
	storage_media_buffer->raw_buffer_amount         = read_count;
#endif

	return( read_count );
}

/* Prepares a buffer before writing the output of the imaging handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t imaging_handle_prepare_write_buffer(
         imaging_handle_t *imaging_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "imaging_handle_prepare_write_buffer";
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
	static char *function         = "imaging_handle_write_buffer";
	ssize_t secondary_write_count = 0;
	ssize_t write_count           = 0;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	uint8_t *raw_write_buffer     = NULL;
	size_t raw_write_buffer_size  = 0;
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
		 "%s: mismatch in write size and amount of bytes in storage media buffer.",
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
	               storage_media_buffer->crc_buffer,
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
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write storage media buffer.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
#if defined( HAVE_V2_API )
		secondary_write_count = libewf_handle_write_chunk(
		                         imaging_handle->secondary_output_handle,
		                         raw_write_buffer,
		                         raw_write_buffer_size,
		                         storage_media_buffer->raw_buffer_amount,
		                         storage_media_buffer->is_compressed,
		                         storage_media_buffer->crc_buffer,
		                         storage_media_buffer->crc,
		                         storage_media_buffer->process_crc,
		                         error );
#else
		secondary_write_count = libewf_raw_write_buffer(
		                         imaging_handle->secondary_output_handle,
		                         raw_write_buffer,
		                         raw_write_buffer_size,
		                         storage_media_buffer->raw_buffer_amount,
		                         storage_media_buffer->is_compressed,
		                         storage_media_buffer->crc,
		                         storage_media_buffer->process_crc );
#endif
#else
#if defined( HAVE_V2_API )
		secondary_write_count = libewf_handle_write_buffer(
		                         imaging_handle->secondary_output_handle,
		                         storage_media_buffer->raw_buffer,
		                         write_size,
		                         error );
#else
		secondary_write_count = libewf_write_buffer(
		                         imaging_handle->secondary_output_handle,
		                         storage_media_buffer->raw_buffer,
		                         write_size );
#endif
#endif
		if( secondary_write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write storage media buffer to secondary output handle.",
			 function );

			return( -1 );
		}
	}
	return( write_count );
}

/* Seeks the offset in the input file
 * Returns the new offset if successful or -1 on error
 */
off64_t imaging_handle_seek_offset(
         imaging_handle_t *imaging_handle,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function    = "imaging_handle_seek_offset";
	off64_t secondary_offset = 0;

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
	offset = libewf_handle_seek_offset(
	          imaging_handle->output_handle,
	          offset,
	          SEEK_SET,
	          error );
#else
	offset = libewf_seek_offset(
	          imaging_handle->output_handle,
	          offset );
#endif

	if( offset == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in output handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		secondary_offset = libewf_handle_seek_offset(
		                    imaging_handle->secondary_output_handle,
		                    offset,
		                    SEEK_SET,
		                    error );
#else
		secondary_offset = libewf_seek_offset(
		                    imaging_handle->secondary_output_handle,
		                    offset );
#endif

		if( secondary_offset == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in secondary output handle.",
			 function );

			return( -1 );
		}
	}
	return( offset );
}

/* Retrieves the offset size
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_get_offset(
     imaging_handle_t *imaging_handle,
     off64_t *offset,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_get_offset";

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
	if( offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_offset(
	     imaging_handle->output_handle,
	     offset,
	     error ) != 1 )
#else
	*offset = libewf_get_offset(
	           imaging_handle->output_handle );

	if( *offset == -1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	uint8_t *data         = NULL;
	static char *function = "imaging_handle_swap_byte_pairs";
	size_t data_size      = 0;
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
	if( read_size != data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: mismatch in read size and data size.",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < read_size; iterator += 2 )
	{
		byte                 = data[ iterator ];
		data[ iterator ]     = data[ iterator + 1 ];
		data[ iterator + 1 ] = byte;
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
	uint8_t *data         = NULL;
	static char *function = "imaging_handle_update_integrity_hash";
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

/* Retrieves the output values of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_get_output_values(
     imaging_handle_t *imaging_handle,
     libsystem_character_t *case_number,
     size_t case_number_size,
     libsystem_character_t *description,
     size_t description_size,
     libsystem_character_t *evidence_number,
     size_t evidence_number_size,
     libsystem_character_t *examiner_name,
     size_t examiner_name_size,
     libsystem_character_t *notes,
     size_t notes_size,
     uint32_t *bytes_per_sector,
     size64_t *media_size,
     uint8_t *media_type,
     uint8_t *media_flags,
     int8_t *compression_level,
     uint8_t *compression_flags,
     uint8_t *libewf_format,
     size64_t *segment_file_size,
     uint32_t *sectors_per_chunk,
     uint32_t *sector_error_granularity,
     liberror_error_t **error )
{
	static char *function = "imaging_handle_get_output_values";

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
#if !defined( HAVE_V2_API )
	if( libewf_parse_header_values(
	     imaging_handle->output_handle,
	     LIBEWF_DATE_FORMAT_CTIME ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to parse header values.",
		 function );

		return( -1 );
	}
#endif
	/* Get case number
	 */
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     "case_number",
	     11,
	     case_number,
	     case_number_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: case number.",
		 function );

		return( -1 );
	}
	/* Get description
	 */
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     "description",
	     11,
	     description,
	     description_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: description.",
		 function );

		return( -1 );
	}
	/* Get evidence number
	 */
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     "evidence_number",
	     15,
	     evidence_number,
	     evidence_number_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: evidence number.",
		 function );

		return( -1 );
	}
	/* Get examiner name
	 */
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     "examiner_name",
	     13,
	     examiner_name,
	     examiner_name_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: examiner name.",
		 function );

		return( -1 );
	}
	/* Get notes
	 */
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     "notes",
	     5,
	     notes,
	     notes_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: notes.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_bytes_per_sector(
	     imaging_handle->output_handle,
	     bytes_per_sector,
	     error ) != 1 )
#else
	if( libewf_get_bytes_per_sector(
	     imaging_handle->output_handle,
	     bytes_per_sector ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_media_size(
	     imaging_handle->output_handle,
	     media_size,
	     error ) != 1 )
#else
	if( libewf_get_media_size(
	     imaging_handle->output_handle,
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
	if( libewf_handle_get_media_type(
	     imaging_handle->output_handle,
	     media_type,
	     error ) != 1 )
#else
	if( libewf_get_media_type(
	     imaging_handle->output_handle,
	     media_type ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_media_flags(
	     imaging_handle->output_handle,
	     media_flags,
	     error ) != 1 )
#else
	if( libewf_get_media_flags(
	     imaging_handle->output_handle,
	     media_flags ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media flags.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_compression_values(
	     imaging_handle->output_handle,
	     compression_level,
	     compression_flags,
	     error ) != 1 )
#else
	if( libewf_get_compression_values(
	     imaging_handle->output_handle,
	     compression_level,
	     compression_flags ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compression values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_format(
	     imaging_handle->output_handle,
	     libewf_format,
	     error ) != 1 )
#else
	if( libewf_get_format(
	     imaging_handle->output_handle,
	     libewf_format ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve format.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_segment_file_size(
	     imaging_handle->output_handle,
	     segment_file_size,
	     error ) != 1 )
#else
	if( libewf_get_segment_file_size(
	     imaging_handle->output_handle,
	     segment_file_size ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_sectors_per_chunk(
	     imaging_handle->output_handle,
	     sectors_per_chunk,
	     error ) != 1 )
#else
	if( libewf_get_sectors_per_chunk(
	     imaging_handle->output_handle,
	     sectors_per_chunk ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sectors per chunk.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_error_granularity(
	     imaging_handle->output_handle,
	     sector_error_granularity,
	     error ) != 1 )
#else
	if( libewf_get_error_granularity(
	     imaging_handle->output_handle,
	     sector_error_granularity ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve error granularity.",
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
     libsystem_character_t *case_number,
     size_t case_number_length,
     libsystem_character_t *description,
     size_t description_length,
     libsystem_character_t *evidence_number,
     size_t evidence_number_length,
     libsystem_character_t *examiner_name,
     size_t examiner_name_length,
     libsystem_character_t *notes,
     size_t notes_length,
     libsystem_character_t *acquiry_operating_system,
     size_t acquiry_operating_system_length,
     libsystem_character_t *acquiry_software,
     size_t acquiry_software_length,
     libsystem_character_t *acquiry_software_version,
     size_t acquiry_software_version_length,
     libsystem_character_t *model,
     size_t model_length,
     libsystem_character_t *serial_number,
     size_t serial_number_length,
     int header_codepage,
     uint32_t bytes_per_sector,
     size64_t media_size,
     uint8_t media_type,
     uint8_t media_flags,
     int8_t compression_level,
     uint8_t compression_flags,
     uint8_t libewf_format,
     size64_t segment_file_size,
     uint32_t sectors_per_chunk,
     uint32_t sector_error_granularity,
     liberror_error_t **error )
{
#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )
	uint8_t guid[ GUID_SIZE ];

	uint8_t guid_type     = 0;
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
	if( ( case_number != NULL )
	 && ( case_number_length > 0 ) )
	{
		/* Set case number
		 */
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     "case_number",
		     11,
		     case_number,
		     case_number_length,
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
	}
	if( ( description != NULL )
	 && ( description_length > 0 ) )
	{
		/* Set description
		 */
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     "description",
		     11,
		     description,
		     description_length,
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
	}
	if( ( evidence_number != NULL )
	 && ( evidence_number_length > 0 ) )
	{
		/* Set evidence number
		 */
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     "evidence_number",
		     15,
		     evidence_number,
		     evidence_number_length,
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
	}
	if( ( examiner_name != NULL )
	 && ( examiner_name_length > 0 ) )
	{
		/* Set examiner name
		 */
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     "examiner_name",
		     13,
		     examiner_name,
		     examiner_name_length,
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
	}
	if( ( notes != NULL )
	 && ( notes_length > 0 ) )
	{
		/* Set notes
		 */
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     "notes",
		     5,
		     notes,
		     notes_length,
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
	       acquiry_operating_system_length,
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
	     acquiry_software_length,
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
	     acquiry_software_version_length,
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
	if( ( model != NULL )
	 && ( imaging_handle_set_header_value(
	       imaging_handle,
	       "model",
	       5,
	       model,
	       model_length,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: model.",
		 function );

		return( -1 );
	}
	if( ( serial_number != NULL )
	 && ( imaging_handle_set_header_value(
	       imaging_handle,
	       "serial_number",
	       13,
	       serial_number,
	       serial_number_length,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: serial number.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_header_codepage(
	     imaging_handle->output_handle,
	     header_codepage,
	     error ) != 1 )
#else
	if( libewf_set_header_codepage(
	     imaging_handle->output_handle,
	     header_codepage ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header codepage.",
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
	if( libewf_handle_set_media_flags(
	     imaging_handle->output_handle,
	     media_flags,
	     error ) != 1 )
#else
	if( libewf_set_media_flags(
	     imaging_handle->output_handle,
	     media_flags ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media flags.",
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
		 "%s: unable to set sectors per chunk.",
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
		 "%s: unable to set error granularity.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_set_header_codepage(
		     imaging_handle->secondary_output_handle,
		     header_codepage,
		     error ) != 1 )
#else
		if( libewf_set_header_codepage(
		     imaging_handle->secondary_output_handle,
		     header_codepage ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header codepage in secondary output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_bytes_per_sector(
		     imaging_handle->secondary_output_handle,
		     bytes_per_sector,
		     error ) != 1 )
#else
		if( libewf_set_bytes_per_sector(
		     imaging_handle->secondary_output_handle,
		     bytes_per_sector ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set bytes per sector in secondary output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_media_size(
		     imaging_handle->secondary_output_handle,
		     media_size,
		     error ) != 1 )
#else
		if( libewf_set_media_size(
		     imaging_handle->secondary_output_handle,
		     media_size ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set media size in secondary output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_media_type(
		     imaging_handle->secondary_output_handle,
		     media_type,
		     error ) != 1 )
#else
		if( libewf_set_media_type(
		     imaging_handle->secondary_output_handle,
		     media_type ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set media type in secondary output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_media_flags(
		     imaging_handle->secondary_output_handle,
		     media_flags,
		     error ) != 1 )
#else
		if( libewf_set_media_flags(
		     imaging_handle->secondary_output_handle,
		     media_flags ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set media flags in secondary output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_compression_values(
		     imaging_handle->secondary_output_handle,
		     compression_level,
		     compression_flags,
		     error ) != 1 )
#else
		if( libewf_set_compression_values(
		     imaging_handle->secondary_output_handle,
		     compression_level,
		     compression_flags ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set compression values in secondary output handle.",
			 function );

			return( -1 );
		}
		/* Format needs to be set before segment file size
		 */
#if defined( HAVE_V2_API )
		if( libewf_handle_set_format(
		     imaging_handle->secondary_output_handle,
		     libewf_format,
		     error ) != 1 )
#else
		if( libewf_set_format(
		     imaging_handle->secondary_output_handle,
		     libewf_format ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set format in secondary output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_segment_file_size(
		     imaging_handle->secondary_output_handle,
		     segment_file_size,
		     error ) != 1 )
#else
		if( libewf_set_segment_file_size(
		     imaging_handle->secondary_output_handle,
		     segment_file_size ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set segment file size in secondary output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_sectors_per_chunk(
		     imaging_handle->secondary_output_handle,
		     sectors_per_chunk,
		     error ) != 1 )
#else
		if( libewf_set_sectors_per_chunk(
		     imaging_handle->secondary_output_handle,
		     sectors_per_chunk ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set sectors per chunk in secondary output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_error_granularity(
		     imaging_handle->secondary_output_handle,
		     sector_error_granularity,
		     error ) != 1 )
#else
		if( libewf_set_error_granularity(
		     imaging_handle->secondary_output_handle,
		     sector_error_granularity ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set error granularity in secondary output handle.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )
	if( ( libewf_format == LIBEWF_FORMAT_ENCASE5 )
	 || ( libewf_format == LIBEWF_FORMAT_ENCASE6 )
	 || ( libewf_format == LIBEWF_FORMAT_EWFX ) )
	{
		guid_type = GUID_TYPE_RANDOM;
        }
	else if( ( libewf_format == LIBEWF_FORMAT_LINEN5 )
	      || ( libewf_format == LIBEWF_FORMAT_LINEN6 ) )
	{
		guid_type = GUID_TYPE_TIME;
	}
	if( guid_type != 0 )
	{
		/* Add a GUID if necessary
		 */
		if( guid_generate(
		     guid,
		     GUID_SIZE,
		     guid_type,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to generate GUID.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_guid(
		     imaging_handle->output_handle,
		     guid,
		     GUID_SIZE,
		     error ) != 1 )
#else
		if( libewf_set_guid(
		     imaging_handle->output_handle,
		     guid,
		     GUID_SIZE ) != 1 )
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
		if( imaging_handle->secondary_output_handle != NULL )
		{
#if defined( HAVE_V2_API )
			if( libewf_handle_set_guid(
			     imaging_handle->secondary_output_handle,
			     guid,
			     GUID_SIZE,
			     error ) != 1 )
#else
			if( libewf_set_guid(
			     imaging_handle->secondary_output_handle,
			     guid,
			     GUID_SIZE ) != 1 )
#endif
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set GUID in secondary output handle.",
				 function );

				return( -1 );
			}
		}
	}
#endif
	imaging_handle->bytes_per_sector = bytes_per_sector;

	return( 1 );
}

/* Retrieves the header value from the output handle
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int imaging_handle_get_header_value(
     imaging_handle_t *imaging_handle,
     char *header_value_identifier,
     size_t header_value_identifier_length,
     libsystem_character_t *header_value,
     size_t header_value_size,
     liberror_error_t **error )
{
	uint8_t utf8_header_value[ IMAGING_HANDLE_VALUE_SIZE ];

	static char *function               = "imaging_handle_get_header_value";
	size_t calculated_header_value_size = 0;
	size_t utf8_header_value_size       = IMAGING_HANDLE_VALUE_SIZE;
	int result                          = 0;

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
	result = libewf_handle_get_header_value(
	          imaging_handle->output_handle,
	          (uint8_t *) header_value_identifier,
	          header_value_identifier_length,
	          utf8_header_value,
	          utf8_header_value_size,
	          error );
#else
	result = libewf_get_header_value(
	          imaging_handle->output_handle,
	          header_value_identifier,
	          (char *) utf8_header_value,
	          utf8_header_value_size );
#endif

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 header_value_identifier );

		return( -1 );
	}
	else if( result == 1 )
	{
		/* Determine the header value size
		 */
		utf8_header_value_size = 1 + narrow_string_length(
		                              (char *) utf8_header_value );

		if( libsystem_string_size_from_utf8_string(
		     utf8_header_value,
		     utf8_header_value_size,
		     &calculated_header_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to determine header value size.",
			 function );

			return( -1 );
		}
		if( header_value_size < calculated_header_value_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: header value too small.",
			 function );

			return( -1 );
		}
		if( libsystem_string_copy_from_utf8_string(
		     header_value,
		     header_value_size,
		     utf8_header_value,
		     utf8_header_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set header value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Sets the header value in the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_header_value(
     imaging_handle_t *imaging_handle,
     char *header_value_identifier,
     size_t header_value_identifier_length,
     libsystem_character_t *header_value,
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
	if( libsystem_string_size_to_utf8_string(
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
	if( libsystem_string_copy_to_utf8_string(
	     header_value,
	     header_value_length + 1,
	     utf8_header_value,
	     utf8_header_value_size,
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
	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_set_header_value(
		     imaging_handle->secondary_output_handle,
		     (uint8_t *) header_value_identifier,
		     header_value_identifier_length,
		     utf8_header_value,
		     utf8_header_value_size - 1,
		     error ) != 1 )
#else
		if( libewf_set_header_value(
		     imaging_handle->secondary_output_handle,
		     header_value_identifier,
		     (char *) utf8_header_value,
		     utf8_header_value_size - 1 ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: %s in secondary output handle.",
			 function,
			 header_value_identifier );

			memory_free(
			 utf8_header_value );

			return( -1 );
		}
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
     libsystem_character_t *hash_value,
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
	if( libsystem_string_size_to_utf8_string(
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
	if( libsystem_string_copy_to_utf8_string(
	     hash_value,
	     hash_value_length + 1,
	     utf8_hash_value,
	     utf8_hash_value_size,
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
	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_set_hash_value(
		     imaging_handle->secondary_output_handle,
		     (uint8_t *) hash_value_identifier,
		     hash_value_identifier_length,
		     utf8_hash_value,
		     utf8_hash_value_size - 1,
		     error ) != 1 )
#else
		if( libewf_set_hash_value(
		     imaging_handle->secondary_output_handle,
		     hash_value_identifier,
		     (char *) utf8_hash_value,
		     utf8_hash_value_size - 1 ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: %s in secondary output handle.",
			 function,
			 hash_value_identifier );

			memory_free(
			 utf8_hash_value );

			return( -1 );
		}
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
	uint64_t amount_of_sectors = 0;
	uint64_t start_sector      = 0;

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
	amount_of_sectors = amount_of_bytes / imaging_handle->bytes_per_sector;

#if defined( HAVE_V2_API )
	if( libewf_handle_add_acquiry_error(
	     imaging_handle->output_handle,
	     start_sector,
	     amount_of_sectors,
	     error ) != 1 )
#else
	if( libewf_add_acquiry_error(
	     imaging_handle->output_handle,
	     (off64_t) start_sector,
	     (uint32_t) amount_of_sectors ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to add acquiry errror.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_add_acquiry_error(
		     imaging_handle->secondary_output_handle,
		     start_sector,
		     amount_of_sectors,
		     error ) != 1 )
#else
		if( libewf_add_acquiry_error(
		     imaging_handle->secondary_output_handle,
		     (off64_t) start_sector,
		     (uint32_t) amount_of_sectors ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to add acquiry errror to secondary output handle.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}
 
/* Appends a session to the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_add_session(
      imaging_handle_t *imaging_handle,
      off64_t start_offset,
      size64_t amount_of_bytes,
      liberror_error_t **error )
{
	static char *function      = "imaging_handle_add_session";
	uint64_t amount_of_sectors = 0;
	uint64_t start_sector      = 0;

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
	amount_of_sectors = amount_of_bytes / imaging_handle->bytes_per_sector;

#if !defined( HAVE_V2_API )
	if( amount_of_sectors > (uint64_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid amount of sectors value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_V2_API )
	if( libewf_handle_add_session(
	     imaging_handle->output_handle,
	     start_sector,
	     amount_of_sectors,
	     error ) != 1 )
#else
	if( libewf_add_session(
	     imaging_handle->output_handle,
	     (off64_t) start_sector,
	     (uint32_t) amount_of_sectors ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to add session.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_add_session(
		     imaging_handle->secondary_output_handle,
		     start_sector,
		     amount_of_sectors,
		     error ) != 1 )
#else
		if( libewf_add_session(
		     imaging_handle->secondary_output_handle,
		     (off64_t) start_sector,
		     (uint32_t) amount_of_sectors ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to add session to secondary output handle.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}
 
/* Finalizes the imaging handle
 * Returns the amount of input bytes written or -1 on error
 */
ssize_t imaging_handle_finalize(
         imaging_handle_t *imaging_handle,
         libsystem_character_t *calculated_md5_hash_string,
         size_t calculated_md5_hash_string_size,
         libsystem_character_t *calculated_sha1_hash_string,
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
	ssize_t secondary_write_count    = 0;
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
		 "%s: unable to finalize output handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		secondary_write_count = libewf_handle_write_finalize(
		                         imaging_handle->secondary_output_handle,
		                         error );
#else
		secondary_write_count = libewf_write_finalize(
		                         imaging_handle->secondary_output_handle );
#endif

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to finalize secondary output handle.",
			 function );

			return( -1 );
		}
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
	uint64_t start_sector      = 0;
#if defined( HAVE_V2_API )
	uint64_t amount_of_sectors = 0;
#else
	uint32_t amount_of_sectors = 0;
#endif
	uint32_t amount_of_errors  = 0;
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
			     &start_sector,
			     &amount_of_sectors,
			     error ) != 1 )
#else
			if( libewf_get_acquiry_error(
			     imaging_handle->output_handle,
			     error_iterator,
			     (off64_t *) &start_sector,
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

				start_sector      = 0;
				amount_of_sectors = 0;

				result = -1;
			}
#if defined( HAVE_V2_API )
			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu64 "\n",
			 start_sector,
			 start_sector + amount_of_sectors,
			 amount_of_sectors );
#else
			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu32 "\n",
			 start_sector,
			 start_sector + amount_of_sectors,
			 amount_of_sectors );
#endif
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

