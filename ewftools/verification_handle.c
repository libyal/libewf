/*
 * Verification handle
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "byte_size_string.h"
#include "digest_hash.h"
#include "ewfcommon.h"
#include "ewfinput.h"
#include "ewftools_libcdata.h"
#include "ewftools_libcerror.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcpath.h"
#include "ewftools_libcsplit.h"
#include "ewftools_libcthreads.h"
#include "ewftools_libewf.h"
#include "ewftools_libhmac.h"
#include "ewftools_system_string.h"
#include "log_handle.h"
#include "process_status.h"
#include "storage_media_buffer.h"
#include "storage_media_buffer_queue.h"
#include "verification_handle.h"

#define VERIFICATION_HANDLE_VALUE_SIZE				64
#define VERIFICATION_HANDLE_VALUE_IDENTIFIER_SIZE		32
#define VERIFICATION_HANDLE_NOTIFY_STREAM			stdout
#define VERIFICATION_HANDLE_MAXIMUM_PROCESS_BUFFERS_SIZE	64 * 1024 * 1024

/* Creates a verification handle
 * Make sure the value verification_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int verification_handle_initialize(
     verification_handle_t **verification_handle,
     uint8_t calculate_md5,
     uint8_t use_data_chunk_functions,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_initialize";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( *verification_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid verification handle value already set.",
		 function );

		return( -1 );
	}
	*verification_handle = memory_allocate_structure(
	                        verification_handle_t );

	if( *verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create verification handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *verification_handle,
	     0,
	     sizeof( verification_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear verification handle.",
		 function );

		memory_free(
		 *verification_handle );

		*verification_handle = NULL;

		return( -1 );
	}
	if( libewf_handle_initialize(
	     &( ( *verification_handle )->input_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create input handle.",
		 function );

		goto on_error;
	}
#ifdef TODO
	/* TODO: have application determine limit value and set to value - 4 */
	if( libewf_handle_set_maximum_number_of_open_handles(
	     ( *verification_handle )->input_handle,
	     1000,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set maximum number of open handles.",
		 function );

		goto on_error;
	}
#endif
	if( calculate_md5 != 0 )
	{
		( *verification_handle )->calculated_md5_hash_string = system_string_allocate(
									33 );

		if( ( *verification_handle )->calculated_md5_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create calculated MD5 digest hash string.",
			 function );

			goto on_error;
		}
	}
	( *verification_handle )->stored_md5_hash_string = system_string_allocate(
							    33 );

	if( ( *verification_handle )->stored_md5_hash_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create stored MD5 digest hash string.",
		 function );

		goto on_error;
	}
	( *verification_handle )->stored_sha1_hash_string = system_string_allocate(
							     41 );

	if( ( *verification_handle )->stored_sha1_hash_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create stored SHA1 digest hash string.",
		 function );

		goto on_error;
	}
	( *verification_handle )->stored_sha256_hash_string = system_string_allocate(
							       65 );

	if( ( *verification_handle )->stored_sha256_hash_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create stored SHA256 digest hash string.",
		 function );

		goto on_error;
	}
	( *verification_handle )->input_format             = VERIFICATION_HANDLE_INPUT_FORMAT_RAW;
	( *verification_handle )->calculate_md5            = calculate_md5;
	( *verification_handle )->use_data_chunk_functions = use_data_chunk_functions;
	( *verification_handle )->header_codepage          = LIBEWF_CODEPAGE_ASCII;
	( *verification_handle )->process_buffer_size      = EWFCOMMON_PROCESS_BUFFER_SIZE;
	( *verification_handle )->notify_stream            = VERIFICATION_HANDLE_NOTIFY_STREAM;

#if defined( HAVE_MULTI_THREAD_SUPPORT )
	( *verification_handle )->number_of_threads        = 4;
#endif
	return( 1 );

on_error:
	if( *verification_handle != NULL )
	{
		if( ( *verification_handle )->stored_sha1_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->stored_sha1_hash_string );
		}
		if( ( *verification_handle )->calculated_sha1_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->calculated_sha1_hash_string );
		}
		if( ( *verification_handle )->stored_md5_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->stored_md5_hash_string );
		}
		if( ( *verification_handle )->calculated_md5_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->calculated_md5_hash_string );
		}
		if( ( *verification_handle )->input_handle != NULL )
		{
			libewf_handle_free(
			 &( ( *verification_handle )->input_handle ),
			 NULL );
		}
		memory_free(
		 *verification_handle );

		*verification_handle = NULL;
	}
	return( -1 );
}

/* Frees a verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_free(
     verification_handle_t **verification_handle,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_free";
	int result            = 1;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( *verification_handle != NULL )
	{
		if( ( ( *verification_handle )->input_handle != NULL )
		 && ( libewf_handle_free(
		       &( ( *verification_handle )->input_handle ),
		       error ) != 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input handle.",
			 function );

			result = -1;
		}
		if( ( *verification_handle )->md5_context != NULL )
		{
			if( libhmac_md5_free(
			     &( ( *verification_handle )->md5_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free MD5 context.",
				 function );

				result = -1;
			}
		}
		if( ( *verification_handle )->calculated_md5_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->calculated_md5_hash_string );
		}
		if( ( *verification_handle )->stored_md5_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->stored_md5_hash_string );
		}
		if( ( *verification_handle )->sha1_context != NULL )
		{
			if( libhmac_sha1_free(
			     &( ( *verification_handle )->sha1_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free SHA1 context.",
				 function );

				result = -1;
			}
		}
		if( ( *verification_handle )->calculated_sha1_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->calculated_sha1_hash_string );
		}
		if( ( *verification_handle )->stored_sha1_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->stored_sha1_hash_string );
		}
		if( ( *verification_handle )->sha256_context != NULL )
		{
			if( libhmac_sha256_free(
			     &( ( *verification_handle )->sha256_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free SHA256 context.",
				 function );

				result = -1;
			}
		}
		if( ( *verification_handle )->calculated_sha256_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->calculated_sha256_hash_string );
		}
		if( ( *verification_handle )->stored_sha256_hash_string != NULL )
		{
			memory_free(
			 ( *verification_handle )->stored_sha256_hash_string );
		}
		memory_free(
		 *verification_handle );

		*verification_handle = NULL;
	}
	return( result );
}

/* Signals the verification handle to abort
 * Returns 1 if successful or -1 on error
 */
int verification_handle_signal_abort(
     verification_handle_t *verification_handle,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_signal_abort";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->input_handle != NULL )
	{
		if( libewf_handle_signal_abort(
		     verification_handle->input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input handle to abort.",
			 function );

			return( -1 );
		}
	}
	verification_handle->abort = 1;

	return( 1 );
}

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int verification_handle_set_maximum_number_of_open_handles(
     verification_handle_t *verification_handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_set_maximum_number_of_open_handles";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_maximum_number_of_open_handles(
	     verification_handle->input_handle,
	     maximum_number_of_open_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set maximum number of open handles in input handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the input of the verification handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_open_input(
     verification_handle_t *verification_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	system_character_t **libewf_filenames = NULL;
	static char *function                 = "verification_handle_open_input";
	size_t first_filename_length          = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == 1 )
	{
		first_filename_length = system_string_length(
		                         filenames[ 0 ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_glob_wide(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &number_of_filenames,
		     error ) != 1 )
#else
		if( libewf_glob(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &number_of_filenames,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to resolve filename(s).",
			 function );

			goto on_error;
		}
		filenames = (system_character_t * const *) libewf_filenames;
	}
	if( verification_handle->header_codepage != LIBEWF_CODEPAGE_ASCII )
	{
		if( libewf_handle_set_header_codepage(
		     verification_handle->input_handle,
		     verification_handle->header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header codepage.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     verification_handle->input_handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     verification_handle->input_handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open files.",
		 function );

		goto on_error;
	}
	if( libewf_filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_glob_wide_free(
		     libewf_filenames,
		     number_of_filenames,
		     error ) != 1 )
#else
		if( libewf_glob_free(
		     libewf_filenames,
		     number_of_filenames,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free globbed filenames.",
			 function );

			return( -1 );
		}
		libewf_filenames = NULL;
	}
	if( libewf_handle_get_chunk_size(
	     verification_handle->input_handle,
	     &( verification_handle->chunk_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk size.",
		 function );

		goto on_error;
	}
	if( libewf_handle_get_bytes_per_sector(
	     verification_handle->input_handle,
	     &( verification_handle->bytes_per_sector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( libewf_filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		libewf_glob_wide_free(
		 libewf_filenames,
		 number_of_filenames,
		 NULL );
#else
		libewf_glob_free(
		 libewf_filenames,
		 number_of_filenames,
		 NULL );
#endif
		libewf_filenames = NULL;
	}
	return( -1 );
}

/* Closes the verification handle
 * Returns the 0 if succesful or -1 on error
 */
int verification_handle_close(
     verification_handle_t *verification_handle,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_close";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_close(
	     verification_handle->input_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Initializes the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int verification_handle_initialize_integrity_hash(
     verification_handle_t *verification_handle,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_initialize_integrity_hash";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->calculate_md5 != 0 )
	{
		if( libhmac_md5_initialize(
		     &( verification_handle->md5_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize MD5 context.",
			 function );

			goto on_error;
		}
		verification_handle->md5_context_initialized = 1;
	}
	if( verification_handle->calculate_sha1 != 0 )
	{
		if( libhmac_sha1_initialize(
		     &( verification_handle->sha1_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize SHA1 context.",
			 function );

			goto on_error;
		}
		verification_handle->sha1_context_initialized = 1;
	}
	if( verification_handle->calculate_sha256 != 0 )
	{
		if( libhmac_sha256_initialize(
		     &( verification_handle->sha256_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize SHA256 context.",
			 function );

			goto on_error;
		}
		verification_handle->sha256_context_initialized = 1;
	}
	return( 1 );

on_error:
	if( verification_handle->sha1_context != NULL )
	{
		libhmac_sha1_free(
		 &( verification_handle->sha1_context ),
		 NULL );
	}
	if( verification_handle->md5_context != NULL )
	{
		libhmac_md5_free(
		 &( verification_handle->md5_context ),
		 NULL );
	}
	return( -1 );
}

/* Updates the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int verification_handle_update_integrity_hash(
     verification_handle_t *verification_handle,
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_update_integrity_hash";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
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
	if( ( buffer_size == 0 )
	 || ( buffer_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid buffer size value out of bounds.",
		 function );

		return( -1 );
	}
	if( verification_handle->calculate_md5 != 0 )
	{
		if( libhmac_md5_update(
		     verification_handle->md5_context,
		     buffer,
		     buffer_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update MD5 digest hash.",
			 function );

			return( -1 );
		}
	}
	if( verification_handle->calculate_sha1 != 0 )
	{
		if( libhmac_sha1_update(
		     verification_handle->sha1_context,
		     buffer,
		     buffer_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update SHA1 digest hash.",
			 function );

			return( -1 );
		}
	}
	if( verification_handle->calculate_sha256 != 0 )
	{
		if( libhmac_sha256_update(
		     verification_handle->sha256_context,
		     buffer,
		     buffer_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update SHA256 digest hash.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Finalizes the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int verification_handle_finalize_integrity_hash(
     verification_handle_t *verification_handle,
     libcerror_error_t **error )
{
	uint8_t calculated_md5_hash[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t calculated_sha1_hash[ LIBHMAC_SHA1_HASH_SIZE ];
	uint8_t calculated_sha256_hash[ LIBHMAC_SHA256_HASH_SIZE ];

	static char *function = "verification_handle_finalize_integrity_hash";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->calculate_md5 != 0 )
	{
		if( verification_handle->calculated_md5_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid verification handle - missing calculated MD5 hash string.",
			 function );

			return( -1 );
		}
		if( libhmac_md5_finalize(
		     verification_handle->md5_context,
		     calculated_md5_hash,
		     LIBHMAC_MD5_HASH_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize MD5 hash.",
			 function );

			return( -1 );
		}
		if( libhmac_md5_free(
		     &( verification_handle->md5_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MD5 context.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_md5_hash,
		     LIBHMAC_MD5_HASH_SIZE,
		     verification_handle->calculated_md5_hash_string,
		     33,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set calculated MD5 hash string.",
			 function );

			return( -1 );
		}
	}
	if( verification_handle->calculate_sha1 != 0 )
	{
		if( verification_handle->calculated_sha1_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid verification handle - missing calculated SHA1 hash string.",
			 function );

			return( -1 );
		}
		if( libhmac_sha1_finalize(
		     verification_handle->sha1_context,
		     calculated_sha1_hash,
		     LIBHMAC_SHA1_HASH_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize SHA1 hash.",
			 function );

			return( -1 );
		}
		if( libhmac_sha1_free(
		     &( verification_handle->sha1_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free SHA1 context.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_sha1_hash,
		     LIBHMAC_SHA1_HASH_SIZE,
		     verification_handle->calculated_sha1_hash_string,
		     41,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create calculated SHA1 hash string.",
			 function );

			return( -1 );
		}
	}
	if( verification_handle->calculate_sha256 != 0 )
	{
		if( verification_handle->calculated_sha256_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid verification handle - missing calculated SHA256 hash string.",
			 function );

			return( -1 );
		}
		if( libhmac_sha256_finalize(
		     verification_handle->sha256_context,
		     calculated_sha256_hash,
		     LIBHMAC_SHA256_HASH_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize SHA256 hash.",
			 function );

			return( -1 );
		}
		if( libhmac_sha256_free(
		     &( verification_handle->sha256_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free SHA256 context.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_sha256_hash,
		     LIBHMAC_SHA256_HASH_SIZE,
		     verification_handle->calculated_sha256_hash_string,
		     65,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create calculated SHA256 hash string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

#if defined( HAVE_MULTI_THREAD_SUPPORT )

/* Prepares a storage media buffer for verification
 * Callback function for the process thread pool
 * Returns 1 if successful or -1 on error
 */
int verification_handle_process_storage_media_buffer_callback(
     storage_media_buffer_t *storage_media_buffer,
     verification_handle_t *verification_handle )
{
        libcerror_error_t *error = NULL;
        static char *function    = "verification_handle_process_storage_media_buffer_callback";
	ssize_t process_count    = 0;

	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		goto on_error;
	}
	if( verification_handle->abort != 0 )
	{
		return( 1 );
	}
	process_count = storage_media_buffer_read_process(
			 storage_media_buffer,
			 &error );

	if( process_count < 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 error );
		}
#endif
		libcerror_error_free(
		 &error );

		process_count = verification_handle->chunk_size;

		storage_media_buffer->is_corrupted = 1;
	}
	if( libcthreads_thread_pool_push(
	     verification_handle->output_thread_pool,
	     (intptr_t *) storage_media_buffer,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to push storage media buffer onto output thread pool queue.",
		 function );

		goto on_error;
	}
	storage_media_buffer = NULL;

	return( 1 );

on_error:
	if( storage_media_buffer != NULL )
	{
		if( storage_media_buffer_queue_release_buffer(
		     verification_handle->storage_media_buffer_queue,
		     storage_media_buffer,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to release storage media buffer onto queue.",
			 function );

			storage_media_buffer_free(
			 &storage_media_buffer,
			 NULL );
		}
	}
	if( error != NULL )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_print_error_backtrace(
			 error );
		}
#endif
		libcerror_error_free(
		 &error );
	}
	if( verification_handle->abort == 0 )
	{
		verification_handle_signal_abort(
		 verification_handle,
		 NULL );
	}
	return( -1 );
}

/* Prepares a storage media buffer for verification
 * Callback function for the process thread pool
 * Returns 1 if successful or -1 on error
 */
int verification_handle_output_storage_media_buffer_callback(
     storage_media_buffer_t *storage_media_buffer,
     verification_handle_t *verification_handle )
{
	libcdata_list_element_t *element      = NULL;
	libcdata_list_element_t *next_element = NULL;
        libcerror_error_t *error              = NULL;
	uint8_t *data                         = NULL;
        static char *function                 = "verification_handle_process_storage_media_buffer_callback";
	size_t data_size                      = 0;
	int result                            = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		goto on_error;
	}
	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		goto on_error;
	}
	if( verification_handle->abort != 0 )
	{
		return( 1 );
	}
	if( libcdata_list_insert_value(
	     verification_handle->output_list,
	     (intptr_t *) storage_media_buffer,
	     (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &storage_media_buffer_compare,
	     LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert storage media buffer into output list.",
		 function );

		goto on_error;
	}
	storage_media_buffer = NULL;

	if( libcdata_list_get_first_element(
	     verification_handle->output_list,
	     &element,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first element.",
		 function );

		goto on_error;
	}
	while( element != NULL )
	{
		if( verification_handle->abort != 0 )
		{
			break;
		}
		if( libcdata_list_element_get_value(
		     element,
		     (intptr_t **) &storage_media_buffer,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element.",
			 function );

			storage_media_buffer = NULL;

			goto on_error;
		}
		if( storage_media_buffer == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing storage media buffer.",
			 function );

			goto on_error;
		}
		if( storage_media_buffer->storage_media_offset != verification_handle->last_offset_hashed )
		{
			break;
		}
		result = storage_media_buffer_is_corrupted(
		          storage_media_buffer,
		          &error );

		if( result == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if storage media buffer is corrupted.",
			 function );

			storage_media_buffer = NULL;

			goto on_error;
		}
		else if( result != 0 )
		{
			/* Append a read error
			 */
			if( verification_handle_append_read_error(
			     verification_handle,
			     storage_media_buffer->storage_media_offset,
			     (size_t) verification_handle->chunk_size,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append read error.",
				 function );

				storage_media_buffer = NULL;

				goto on_error;
			}
		}
		if( storage_media_buffer_get_data(
		     storage_media_buffer,
		     &data,
		     &data_size,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine storage media buffer data.",
			 function );

			storage_media_buffer = NULL;

			goto on_error;
		}
		if( verification_handle_update_integrity_hash(
		     verification_handle,
		     data,
		     storage_media_buffer->processed_size,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to update integrity hash(es).",
			 function );

			storage_media_buffer = NULL;

			goto on_error;
		}
		verification_handle->last_offset_hashed = storage_media_buffer->storage_media_offset + storage_media_buffer->processed_size;

		if( libcdata_list_element_get_next_element(
		     element,
		     &next_element,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next list element.",
			 function );

			storage_media_buffer = NULL;

			goto on_error;
		}
		if( libcdata_list_remove_element(
		     verification_handle->output_list,
		     element,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to remove list element from output list.",
			 function );

			storage_media_buffer = NULL;

			goto on_error;
		}
		/* The output list no longer manages the list element and the storage media buffer it contains
		 */
		if( libcdata_list_element_free(
		     &element,
		     NULL,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free list element.",
			 function );

			goto on_error;
		}
		element = next_element;

		if( storage_media_buffer_queue_release_buffer(
		     verification_handle->storage_media_buffer_queue,
		     storage_media_buffer,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to release storage media buffer onto queue.",
			 function );

			goto on_error;
		}
		storage_media_buffer = NULL;

		if( process_status_update(
		     verification_handle->process_status,
		     verification_handle->last_offset_hashed,
		     verification_handle->media_size,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update process status.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( storage_media_buffer != NULL )
	{
		if( storage_media_buffer_queue_release_buffer(
		     verification_handle->storage_media_buffer_queue,
		     storage_media_buffer,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to release storage media buffer onto queue.",
			 function );

			storage_media_buffer_free(
			 &storage_media_buffer,
			 NULL );
		}
	}
	if( error != NULL )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_print_error_backtrace(
			 error );
		}
#endif
		libcerror_error_free(
		 &error );
	}
	if( verification_handle->abort == 0 )
	{
		verification_handle_signal_abort(
		 verification_handle,
		 NULL );
	}
	return( -1 );
}

/* Empties the output list
 * Returns 1 if successful or -1 on error
 */
int verification_handle_empty_output_list(
     verification_handle_t *verification_handle,
     libcerror_error_t **error )
{
	libcdata_list_element_t *element             = NULL;
	libcdata_list_element_t *next_element        = NULL;
	storage_media_buffer_t *storage_media_buffer = NULL;
        static char *function                        = "verification_handle_empty_output_list";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_first_element(
	     verification_handle->output_list,
	     &element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first element.",
		 function );

		return( -1 );
	}
	while( element != NULL )
	{
		if( libcdata_list_element_get_value(
		     element,
		     (intptr_t **) &storage_media_buffer,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element.",
			 function );

			return( -1 );
		}
		if( storage_media_buffer == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing storage media buffer.",
			 function );

			return( -1 );
		}
		if( storage_media_buffer_queue_release_buffer(
		     verification_handle->storage_media_buffer_queue,
		     storage_media_buffer,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to release storage media buffer onto queue.",
			 function );

			return( -1 );
		}
		storage_media_buffer = NULL;

		if( libcdata_list_element_get_next_element(
		     element,
		     &next_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next list element.",
			 function );

			return( -1 );
		}
		if( libcdata_list_remove_element(
		     verification_handle->output_list,
		     element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to remove list element from output list.",
			 function );

			return( -1 );
		}
		/* The output list no longer manages the list element and the storage media buffer it contains
		 */
		if( libcdata_list_element_free(
		     &element,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free list element.",
			 function );

			return( -1 );
		}
		element = next_element;
	}
	return( 1 );
}

#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

/* Verifies the input
 * Returns 1 if successful, 0 if not or -1 on error
 */
int verification_handle_verify_input(
     verification_handle_t *verification_handle,
     uint8_t print_status_information,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	storage_media_buffer_t *storage_media_buffer = NULL;
	uint8_t *data                                = NULL;
	static char *function                        = "verification_handle_verify_input";
	size64_t remaining_media_size                = 0;
	size_t data_size                             = 0;
	size_t process_buffer_size                   = 0;
	size_t read_size                             = 0;
	ssize_t process_count                        = 0;
	ssize_t read_count                           = 0;
	off64_t storage_media_offset                 = 0;
	uint32_t number_of_checksum_errors           = 0;
	uint8_t storage_media_buffer_mode            = 0;
	int is_corrupted                             = 0;
	int maximum_number_of_queued_items           = 0;
	int md5_hash_compare                         = 0;
	int result                                   = 0;
	int sha1_hash_compare                        = 0;
	int sha256_hash_compare                      = 0;
	int status                                   = PROCESS_STATUS_COMPLETED;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk size.",
		 function );

		return( -1 );
	}
	if( verification_handle->chunk_size > (size32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( verification_handle->process_buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid process buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if !defined( HAVE_MULTI_THREAD_SUPPORT )
	if( verification_handle->number_of_threads != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: multi-threading not supported.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_handle_get_media_size(
	     verification_handle->input_handle,
	     &( verification_handle->media_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		goto on_error;
	}
	if( verification_handle->use_data_chunk_functions != 0 )
	{
		process_buffer_size       = verification_handle->chunk_size;
		storage_media_buffer_mode = STORAGE_MEDIA_BUFFER_MODE_CHUNK_DATA;
	}
	else
	{
		if( verification_handle->process_buffer_size == 0 )
		{
			process_buffer_size = verification_handle->chunk_size;
		}
		else
		{
			process_buffer_size = verification_handle->process_buffer_size;
		}
		storage_media_buffer_mode = STORAGE_MEDIA_BUFFER_MODE_BUFFERED;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT )
	if( verification_handle->number_of_threads != 0 )
	{
		maximum_number_of_queued_items = 1 + (int) ( VERIFICATION_HANDLE_MAXIMUM_PROCESS_BUFFERS_SIZE / process_buffer_size );

		if( libcthreads_thread_pool_create(
		     &( verification_handle->process_thread_pool ),
		     NULL,
		     verification_handle->number_of_threads,
		     maximum_number_of_queued_items,
		     (int (*)(intptr_t *, void *)) &verification_handle_process_storage_media_buffer_callback,
		     (void *) verification_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize process thread pool.",
			 function );

			goto on_error;
		}
		if( libcthreads_thread_pool_create(
		     &( verification_handle->output_thread_pool ),
		     NULL,
		     1,
		     maximum_number_of_queued_items,
		     (int (*)(intptr_t *, void *)) &verification_handle_output_storage_media_buffer_callback,
		     (void *) verification_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize output thread pool.",
			 function );

			goto on_error;
		}
		if( libcdata_list_initialize(
		     &( verification_handle->output_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create output list.",
			 function );

			goto on_error;
		}
		if( storage_media_buffer_queue_initialize(
		     &( verification_handle->storage_media_buffer_queue ),
		     verification_handle->input_handle,
		     maximum_number_of_queued_items,
		     storage_media_buffer_mode,
		     process_buffer_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create storage media buffer queue.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

	if( verification_handle_initialize_integrity_hash(
	     verification_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize integrity hash(es).",
		 function );

		goto on_error;
	}
	if( process_status_initialize(
	     &( verification_handle->process_status ),
	     _SYSTEM_STRING( "Verify" ),
	     _SYSTEM_STRING( "verified" ),
	     _SYSTEM_STRING( "Read" ),
	     verification_handle->notify_stream,
	     print_status_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create process status.",
		 function );

		goto on_error;
	}
	if( process_status_start(
	     verification_handle->process_status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to start process status.",
		 function );

		goto on_error;
	}
	if( verification_handle->number_of_threads == 0 )
	{
		if( storage_media_buffer_initialize(
		     &storage_media_buffer,
		     verification_handle->input_handle,
		     storage_media_buffer_mode,
		     process_buffer_size,
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
	}
	remaining_media_size = verification_handle->media_size;

	while( remaining_media_size > 0 )
	{
		if( verification_handle->abort != 0 )
		{
			break;
		}
#if defined( HAVE_MULTI_THREAD_SUPPORT )
		if( verification_handle->number_of_threads != 0 )
		{
			if( storage_media_buffer_queue_grab_buffer(
			     verification_handle->storage_media_buffer_queue,
			     &storage_media_buffer,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to grab storage media buffer from queue.",
				 function );

				goto on_error;
			}
			if( storage_media_buffer == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing storage media buffer.",
				 function );

				goto on_error;
			}
		}
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

		read_size = process_buffer_size;

		if( remaining_media_size < read_size )
		{
			read_size = (size_t) remaining_media_size;
		}
		read_count = storage_media_buffer_read_from_handle(
		              storage_media_buffer,
		              verification_handle->input_handle,
		              read_size,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data.",
			 function );

			goto on_error;
		}
		if( read_count == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unexpected end of data.",
			 function );

			goto on_error;
		}
		storage_media_buffer->storage_media_offset = storage_media_offset;

		storage_media_offset += read_count;
		remaining_media_size -= read_count;

#if defined( HAVE_MULTI_THREAD_SUPPORT )
		if( verification_handle->number_of_threads != 0 )
		{
			if( libcthreads_thread_pool_push(
			     verification_handle->process_thread_pool,
			     (intptr_t *) storage_media_buffer,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to push storage media buffer onto process thread pool queue.",
				 function );

				goto on_error;
			}
			storage_media_buffer = NULL;
		}
		else
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */
		{
			process_count = storage_media_buffer_read_process(
			                 storage_media_buffer,
		        	         error );

			if( process_count < 0 )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( ( libcnotify_verbose != 0 )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
#endif
				libcerror_error_free(
				 error );

				process_count = verification_handle->chunk_size;

				storage_media_buffer->is_corrupted = 1;
			}
			result = storage_media_buffer_is_corrupted(
			          storage_media_buffer,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if storage media buffer is corrupted.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				/* Append a read error
				 */
				if( verification_handle_append_read_error(
				     verification_handle,
				     storage_media_buffer->storage_media_offset,
				     (size_t) verification_handle->chunk_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append read error.",
					 function );

					goto on_error;
				}
			}
			if( storage_media_buffer_get_data(
			     storage_media_buffer,
			     &data,
			     &data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine storage media buffer data.",
				 function );

				goto on_error;
			}
			if( verification_handle_update_integrity_hash(
			     verification_handle,
			     data,
			     storage_media_buffer->processed_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to update integrity hash(es).",
				 function );

				goto on_error;
			}
			verification_handle->last_offset_hashed += (off64_t) process_count;

			if( process_status_update(
			     verification_handle->process_status,
			     verification_handle->last_offset_hashed,
			     verification_handle->media_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to update process status.",
				 function );

				goto on_error;
			}
		}
  	}
	if( verification_handle->number_of_threads == 0 )
	{
		if( storage_media_buffer_free(
		     &storage_media_buffer,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free storage media buffer.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT )
	if( verification_handle->process_thread_pool != NULL )
	{
		if( libcthreads_thread_pool_join(
		     &( verification_handle->process_thread_pool ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to join process thread pool.",
			 function );

			goto on_error;
		}
	}
	if( verification_handle->output_thread_pool != NULL )
	{
		if( libcthreads_thread_pool_join(
		     &( verification_handle->output_thread_pool ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to join output thread pool.",
			 function );

			goto on_error;
		}
	}
	if( verification_handle->output_list != NULL )
	{
		if( verification_handle_empty_output_list(
		     verification_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty output list.",
			 function );

			goto on_error;
		}
		if( libcdata_list_free(
		     &( verification_handle->output_list ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free output list.",
			 function );

			goto on_error;
		}
	}
	if( verification_handle->storage_media_buffer_queue != NULL )
	{
		if( storage_media_buffer_queue_free(
		     &( verification_handle->storage_media_buffer_queue ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free storage media buffer queue.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

	if( verification_handle_finalize_integrity_hash(
	     verification_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize integrity hash(es).",
		 function );

		goto on_error;
	}
	if( verification_handle_get_integrity_hash_from_input(
	     verification_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to retrieve integrity hash(es) from input.",
		 function );

		goto on_error;
	}
	if( verification_handle->abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     verification_handle->process_status,
	     verification_handle->last_offset_hashed,
	     status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to stop process status.",
		 function );

		goto on_error;
	}
	if( process_status_free(
	     &( verification_handle->process_status ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free process status.",
		 function );

		goto on_error;
	}
	if( verification_handle->abort == 0 )
	{
		fprintf(
		 verification_handle->notify_stream,
		 "\n" );

		if( verification_handle_checksum_errors_fprint(
		     verification_handle,
		     verification_handle->notify_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print checksum errors.",
			 function );

			goto on_error;
		}
		if( verification_handle_hash_values_fprint(
		     verification_handle,
		     verification_handle->notify_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print hash values.",
			 function );

			goto on_error;
		}
		if( verification_handle_additional_hash_values_fprint(
		     verification_handle,
		     verification_handle->notify_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print additional hash values.",
			 function );

			goto on_error;
		}
		fprintf(
		 verification_handle->notify_stream,
		 "\n" );

		if( log_handle != NULL )
		{
			if( verification_handle_checksum_errors_fprint(
			     verification_handle,
			     log_handle->log_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print checksum errors in log handle.",
				 function );

				goto on_error;
			}
			if( verification_handle_hash_values_fprint(
			     verification_handle,
			     log_handle->log_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print hash values in log handle.",
				 function );

				goto on_error;
			}
			if( verification_handle_additional_hash_values_fprint(
			     verification_handle,
			     log_handle->log_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print additional hash values in log handle.",
				 function );

				goto on_error;
			}
		}
	}
	is_corrupted = libewf_handle_segment_files_corrupted(
	                verification_handle->input_handle,
	                error );

	if( is_corrupted == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if segment files are corrupted.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_number_of_checksum_errors(
	     verification_handle->input_handle,
	     &number_of_checksum_errors,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of checksum errors.",
		 function );

		return( -1 );
	}
	if( ( verification_handle->calculate_md5 != 0 )
	 && ( verification_handle->stored_md5_hash_available != 0 ) )
	{
		md5_hash_compare = system_string_compare(
		                    verification_handle->stored_md5_hash_string,
		                    verification_handle->calculated_md5_hash_string,
		                    33 );
	}
	if( ( verification_handle->calculate_sha1 != 0 )
	 && ( verification_handle->stored_sha1_hash_available != 0 ) )
	{
		sha1_hash_compare = system_string_compare(
		                     verification_handle->stored_sha1_hash_string,
		                     verification_handle->calculated_sha1_hash_string,
		                     41 );
	}
	if( ( verification_handle->calculate_sha256 != 0 )
	 && ( verification_handle->stored_sha256_hash_available != 0 ) )
	{
		sha256_hash_compare = system_string_compare(
		                       verification_handle->stored_sha256_hash_string,
		                       verification_handle->calculated_sha256_hash_string,
		                       65 );
	}
	/* Note that a set of EWF files can be verified without an integrity hash
	 */
	if( ( is_corrupted == 0 )
	 && ( number_of_checksum_errors == 0 )
	 && ( md5_hash_compare == 0 )
	 && ( sha1_hash_compare == 0 )
	 && ( sha256_hash_compare == 0 ) )
	{
		return( 1 );
	}
	return( 0 );

on_error:
	if( ( verification_handle->number_of_threads == 0 )
	 && ( storage_media_buffer != NULL ) )
	{
		storage_media_buffer_free(
		 &storage_media_buffer,
		 NULL );
	}
	if( verification_handle->process_status != NULL )
	{
		process_status_stop(
		 verification_handle->process_status,
		 verification_handle->last_offset_hashed,
		 PROCESS_STATUS_FAILED,
		 NULL );
		process_status_free(
		 &( verification_handle->process_status ),
		 NULL );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT )
	if( verification_handle->process_thread_pool != NULL )
	{
		libcthreads_thread_pool_join(
		 &( verification_handle->process_thread_pool ),
		 NULL );
	}
	if( verification_handle->output_thread_pool != NULL )
	{
		libcthreads_thread_pool_join(
		 &( verification_handle->output_thread_pool ),
		 NULL );
	}
	if( verification_handle->output_list != NULL )
	{
		verification_handle_empty_output_list(
		 verification_handle,
		 NULL );
		libcdata_list_free(
		 &( verification_handle->output_list ),
		 NULL,
		 NULL );
	}
	if( verification_handle->storage_media_buffer_queue != NULL )
	{
		storage_media_buffer_queue_free(
		 &( verification_handle->storage_media_buffer_queue ),
		 NULL );
	}
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

	return( -1 );
}

/* Verifies single files
 * Returns 1 if successful, 0 if not or -1 on error
 */
int verification_handle_verify_single_files(
     verification_handle_t *verification_handle,
     uint8_t print_status_information,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libewf_file_entry_t *file_entry    = NULL;
	static char *function              = "verification_handle_verify_single_files";
	uint32_t number_of_checksum_errors = 0;
	int result                         = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_root_file_entry(
	     verification_handle->input_handle,
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root file entry.",
		 function );

		goto on_error;
	}
	if( process_status_initialize(
	     &( verification_handle->process_status ),
	     _SYSTEM_STRING( "Verify" ),
	     _SYSTEM_STRING( "verified" ),
	     _SYSTEM_STRING( "Read" ),
	     verification_handle->notify_stream,
	     print_status_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create process status.",
		 function );

		goto on_error;
	}
	if( process_status_start(
	     verification_handle->process_status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to start process status.",
		 function );

		goto on_error;
	}
	result = verification_handle_verify_file_entry(
	          verification_handle,
	          file_entry,
	          _SYSTEM_STRING( "" ),
	          0,
	          log_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to verify root file entry.",
		 function );

		goto on_error;
	}
	if( process_status_stop(
	     verification_handle->process_status,
	     0,
	     PROCESS_STATUS_COMPLETED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to stop process status.",
		 function );

		goto on_error;
	}
	if( process_status_free(
	     &( verification_handle->process_status ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free process status.",
		 function );

		goto on_error;
	}
	if( libewf_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free root file entry.",
		 function );

		goto on_error;
	}
	if( libewf_handle_get_number_of_checksum_errors(
	     verification_handle->input_handle,
	     &number_of_checksum_errors,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of checksum errors.",
		 function );

		return( -1 );
	}
	if( ( result != 0 )
	 && ( number_of_checksum_errors == 0 ) )
	{
		return( 1 );
	}
	return( 0 );

on_error:
	if( verification_handle->process_status != NULL )
	{
		process_status_stop(
		 verification_handle->process_status,
		 0,
		 PROCESS_STATUS_FAILED,
		 NULL );
		process_status_free(
		 &( verification_handle->process_status ),
		 NULL );
	}
	if( file_entry != NULL )
	{
		libewf_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Verifies a (single) file entry
 * Returns 1 if successful, 0 if not or -1 on error
 */
int verification_handle_verify_file_entry(
     verification_handle_t *verification_handle,
     libewf_file_entry_t *file_entry,
     const system_character_t *file_entry_path,
     size_t file_entry_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t *name        = NULL;
	system_character_t *target_path = NULL;
	uint8_t *file_entry_data        = NULL;
	static char *function           = "verification_handle_verify_file_entry";
	size64_t file_entry_data_size   = 0;
	size_t name_size                = 0;
	size_t process_buffer_size      = 0;
	size_t read_size                = 0;
	size_t target_path_size         = 0;
	ssize_t read_count              = 0;
	uint8_t file_entry_type         = 0;
	int md5_hash_compare            = 0;
	int result                      = 0;
	int return_value                = 0;
	int sha1_hash_compare           = 0;
	int sha256_hash_compare         = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk size.",
		 function );

		return( -1 );
	}
	if( verification_handle->chunk_size > (size32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( verification_handle->process_buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid process buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_file_entry_get_utf16_name_size(
		  file_entry,
		  &name_size,
		  error );
#else
	result = libewf_file_entry_get_utf8_name_size(
		  file_entry,
		  &name_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the name.",
		 function );

		goto on_error;
	}
	if( name_size > 0 )
	{
		name = system_string_allocate(
			name_size );

		if( name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_file_entry_get_utf16_name(
			  file_entry,
			  (uint16_t *) name,
			  name_size,
			  error );
#else
		result = libewf_file_entry_get_utf8_name(
			  file_entry,
			  (uint8_t *) name,
			  name_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the name.",
			 function );

			memory_free(
			 name );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_path_join_wide(
		     &target_path,
		     &target_path_size,
		     file_entry_path,
		     file_entry_path_length,
		     name,
		     name_size - 1,
		     error ) != 1 )
#else
		if( libcpath_path_join(
		     &target_path,
		     &target_path_size,
		     file_entry_path,
		     file_entry_path_length,
		     name,
		     name_size - 1,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create target path.",
			 function );

			memory_free(
			 name );

			goto on_error;
		}
		memory_free(
		 name );
	}
	else
	{
		target_path      = (system_character_t *) file_entry_path;
		target_path_size = file_entry_path_length + 1;
	}
	if( libewf_file_entry_get_type(
	     file_entry,
	     &file_entry_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry type.",
		 function );

		goto on_error;
	}
	/* TODO what about NTFS streams ?
	 */
	if( file_entry_type == LIBEWF_FILE_ENTRY_TYPE_FILE )
	{
		fprintf(
		 verification_handle->notify_stream,
		 "Single file: %" PRIs_SYSTEM "\n",
		 target_path );

		if( log_handle != NULL )
		{
			log_handle_printf(
			 log_handle,
			 "Single file: %" PRIs_SYSTEM "\n",
			 target_path );
		}
		if( libewf_file_entry_get_size(
		     file_entry,
		     &file_entry_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry data size.",
			 function );

			goto on_error;
		}
		if( verification_handle_initialize_integrity_hash(
		     verification_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize integrity hash(es).",
			 function );

			goto on_error;
		}
		result = 1;

		if( file_entry_data_size > 0 )
		{
			if( verification_handle->process_buffer_size == 0 )
			{
				process_buffer_size = verification_handle->chunk_size;
			}
			else
			{
				process_buffer_size = verification_handle->process_buffer_size;
			}
			/* This function in not necessary for normal use
			 * but it was added for testing
			 */
			if( libewf_file_entry_seek_offset(
			     file_entry,
			     0,
			     SEEK_SET,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to seek the start of the file entry data.",
				 function );

				goto on_error;
			}
			file_entry_data = (uint8_t *) memory_allocate(
			                               sizeof( uint8_t ) * process_buffer_size );

			if( file_entry_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create file entry data.",
				 function );

				goto on_error;
			}
			while( file_entry_data_size > 0 )
			{
				if( file_entry_data_size >= process_buffer_size )
				{
					read_size = process_buffer_size;
				}
				else
				{
					read_size = (size_t) file_entry_data_size;
				}
				read_count = libewf_file_entry_read_buffer(
				              file_entry,
				              file_entry_data,
				              read_size,
				              error );

				if( read_count == (ssize_t) -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read file entry data.",
					 function );

					goto on_error;
				}
				else if( read_count != (ssize_t) read_size )
				{
					result = 0;

					break;
				}
				file_entry_data_size -= read_size;

				if( verification_handle_update_integrity_hash(
				     verification_handle,
				     file_entry_data,
				     read_count,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to update integrity hash(es).",
					 function );

					goto on_error;
				}
			}
			memory_free(
			 file_entry_data );

			file_entry_data = NULL;
		}
		if( verification_handle_finalize_integrity_hash(
		     verification_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize integrity hash(es).",
			 function );

			goto on_error;
		}
		if( result != 0 )
		{
			if( verification_handle_get_integrity_hash_from_file_entry(
			     verification_handle,
			     file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to retrieve integrity hash(es) from file entry.",
				 function );

				goto on_error;
			}
			if( verification_handle_hash_values_fprint(
			     verification_handle,
			     verification_handle->notify_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print hash values.",
				 function );

				goto on_error;
			}
			if( log_handle != NULL )
			{
				if( verification_handle_hash_values_fprint(
				     verification_handle,
				     log_handle->log_stream,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print hash values in log handle.",
					 function );

					goto on_error;
				}
			}
			if( ( verification_handle->calculate_md5 != 0 )
			 && ( verification_handle->stored_md5_hash_available != 0 ) )
			{
				md5_hash_compare = system_string_compare(
						    verification_handle->stored_md5_hash_string,
						    verification_handle->calculated_md5_hash_string,
						    33 );
			}
			if( ( verification_handle->calculate_sha1 != 0 )
			 && ( verification_handle->stored_sha1_hash_available != 0 ) )
			{
				sha1_hash_compare = system_string_compare(
						     verification_handle->stored_sha1_hash_string,
						     verification_handle->calculated_sha1_hash_string,
						     41 );
			}
			if( ( verification_handle->calculate_sha256 != 0 )
			 && ( verification_handle->stored_sha256_hash_available != 0 ) )
			{
				sha256_hash_compare = system_string_compare(
						       verification_handle->stored_sha256_hash_string,
						       verification_handle->calculated_sha256_hash_string,
						       65 );
			}
		}
		if( ( result != 0 )
		 && ( md5_hash_compare == 0 )
		 && ( sha1_hash_compare == 0 )
		 && ( sha256_hash_compare == 0 ) )
		{
			return_value = 1;
		}
		else
		{
			fprintf(
			 verification_handle->notify_stream,
			 "FAILED\n" );

			if( log_handle != NULL )
			{
				log_handle_printf(
				 log_handle,
				 "FAILED\n" );
			}
			return_value = 0;
		}
		fprintf(
		 verification_handle->notify_stream,
		 "\n" );
	}
	else if( file_entry_type == LIBEWF_FILE_ENTRY_TYPE_DIRECTORY )
	{
		return_value = verification_handle_verify_sub_file_entries(
		                verification_handle,
		                file_entry,
		                target_path,
		                target_path_size - 1,
		                log_handle,
		                error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to verify sub file entries.",
			 function );

			return( -1 );
		}
	}
	if( target_path != file_entry_path )
	{
		memory_free(
		 target_path );
	}
	return( return_value );

on_error:
	if( file_entry_data != NULL )
	{
		memory_free(
		 file_entry_data );
	}
	if( ( target_path != NULL )
	 && ( target_path != file_entry_path ) )
	{
		memory_free(
		 target_path );
	}
	return( -1 );
}

/* Verifies a (single) file entry sub file entries
 * Returns 1 if successful, 0 if not or -1 on error
 */
int verification_handle_verify_sub_file_entries(
     verification_handle_t *verification_handle,
     libewf_file_entry_t *file_entry,
     const system_character_t *file_entry_path,
     size_t file_entry_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libewf_file_entry_t *sub_file_entry = NULL;
	static char *function               = "verification_handle_verify_sub_file_entries";
	int number_of_sub_file_entries      = 0;
	int result                          = 1;
	int sub_file_entry_index            = 0;
	int sub_file_entry_result           = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( libewf_file_entry_get_number_of_sub_file_entries(
	     file_entry,
	     &number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		return( -1 );
	}
	for( sub_file_entry_index = 0;
	     sub_file_entry_index < number_of_sub_file_entries;
	     sub_file_entry_index++ )
	{
		if( libewf_file_entry_get_sub_file_entry(
		     file_entry,
		     sub_file_entry_index,
		     &sub_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to free retrieve sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
		sub_file_entry_result = verification_handle_verify_file_entry(
		                         verification_handle,
		                         sub_file_entry,
		                         file_entry_path,
		                         file_entry_path_length,
		                         log_handle,
		                         error );

		if( sub_file_entry_result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to verify sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
		if( sub_file_entry_result == 0 )
		{
			result = 0;
		}
		if( libewf_file_entry_free(
		     &sub_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( sub_file_entry != NULL )
	{
		libewf_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the integrity hash(es) from the input
 * Returns 1 if successful or -1 on error
 */
int verification_handle_get_integrity_hash_from_input(
     verification_handle_t *verification_handle,
     libcerror_error_t **error )
{
#if defined( USE_LIBEWF_GET_MD5_HASH )
        uint8_t stored_md5_hash[ DIGEST_HASH_SIZE_MD5 ];
#endif

	static char *function = "verification_handle_get_integrity_hash_from_input";
	int result            = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->stored_md5_hash_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing stored MD5 hash string.",
		 function );

		return( -1 );
	}
	if( verification_handle->stored_sha1_hash_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing stored SHA1 hash string.",
		 function );

		return( -1 );
	}
	if( verification_handle->stored_sha256_hash_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing stored SHA256 hash string.",
		 function );

		return( -1 );
	}
#if defined( USE_LIBEWF_GET_MD5_HASH )
	result = libewf_handle_get_md5_hash(
		  verification_handle->input_handle,
		  md5_hash,
		  DIGEST_HASH_SIZE_MD5,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine stored MD5 hash string.",
		 function );

		return( -1 );
	}
	verification_handle->stored_md5_hash_available = result;

	if( digest_hash_copy_to_string(
	     md5_hash,
	     DIGEST_HASH_SIZE_MD5,
	     verification_handle->stored_md5_hash_string,
	     33 ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set stored MD5 hash string.",
		 function );

		return( -1 );
	}
#else
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_utf16_hash_value(
		  verification_handle->input_handle,
		  (uint8_t *) "MD5",
		  3,
		  (uint16_t *) verification_handle->stored_md5_hash_string,
		  33,
		  error );
#else
	result = libewf_handle_get_utf8_hash_value(
		  verification_handle->input_handle,
		  (uint8_t *) "MD5",
		  3,
		  (uint8_t *) verification_handle->stored_md5_hash_string,
		  33,
		  error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine stored MD5 hash string.",
		 function );

		return( -1 );
	}
	verification_handle->stored_md5_hash_available = result;

#endif /* defined( USE_LIBEWF_GET_MD5_HASH ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_utf16_hash_value(
		  verification_handle->input_handle,
		  (uint8_t *) "SHA1",
		  4,
		  (uint16_t *) verification_handle->stored_sha1_hash_string,
		  41,
		  error );
#else
	result = libewf_handle_get_utf8_hash_value(
		  verification_handle->input_handle,
		  (uint8_t *) "SHA1",
		  4,
		  (uint8_t *) verification_handle->stored_sha1_hash_string,
		  41,
		  error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine stored SHA1 hash string.",
		 function );

		return( -1 );
	}
	verification_handle->stored_sha1_hash_available = result;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_utf16_hash_value(
		  verification_handle->input_handle,
		  (uint8_t *) "SHA256",
		  6,
		  (uint16_t *) verification_handle->stored_sha256_hash_string,
		  65,
		  error );
#else
	result = libewf_handle_get_utf8_hash_value(
		  verification_handle->input_handle,
		  (uint8_t *) "SHA256",
		  6,
		  (uint8_t *) verification_handle->stored_sha256_hash_string,
		  65,
		  error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine stored SHA256 hash string.",
		 function );

		return( -1 );
	}
	verification_handle->stored_sha256_hash_available = result;

	return( 1 );
}

/* Retrieves the integrity hash(es) from a (single) file entry
 * Returns 1 if successful or -1 on error
 */
int verification_handle_get_integrity_hash_from_file_entry(
     verification_handle_t *verification_handle,
     libewf_file_entry_t *file_entry,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_get_integrity_hash_from_file_entry";
	int result            = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->stored_md5_hash_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid verification handle - missing stored MD5 hash string.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_file_entry_get_utf16_hash_value_md5(
		  file_entry,
		  (uint16_t *) verification_handle->stored_md5_hash_string,
		  33,
		  error );
#else
	result = libewf_file_entry_get_utf8_hash_value_md5(
		  file_entry,
		  (uint8_t *) verification_handle->stored_md5_hash_string,
		  33,
		  error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine stored MD5 hash string.",
		 function );

		return( -1 );
	}
	verification_handle->stored_md5_hash_available = result;

	verification_handle->stored_sha1_hash_available = 0;

	verification_handle->stored_sha256_hash_available = 0;

	return( 1 );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int verification_handle_set_header_codepage(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_set_header_codepage";
	int result            = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_header_codepage(
	          string,
	          &( verification_handle->header_codepage ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine header codepage.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( verification_handle->input_handle != NULL )
		{
			if( libewf_handle_set_header_codepage(
			     verification_handle->input_handle,
			     verification_handle->header_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set header codepage.",
				 function );

				return( -1 );
			}
		}
	}
	return( result );
}

/* Sets the format
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int verification_handle_set_format(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_set_format";
	size_t string_length  = 0;
	int result            = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "raw" ),
		     3 ) == 0 )
		{
			verification_handle->input_format = VERIFICATION_HANDLE_INPUT_FORMAT_RAW;
			result                            = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "files" ),
		     5 ) == 0 )
		{
			verification_handle->input_format = VERIFICATION_HANDLE_INPUT_FORMAT_FILES;
			result                            = 1;
		}
	}
	return( result );
}

/* Sets the process buffer size
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int verification_handle_set_process_buffer_size(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "verification_handle_set_process_buffer_size";
	size_t string_length   = 0;
	uint64_t size_variable = 0;
	int result             = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	result = byte_size_string_convert(
	          string,
	          string_length,
	          &size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine process buffer size.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( size_variable > (uint64_t) SSIZE_MAX )
		{
			result = 0;
		}
		else
		{
			verification_handle->process_buffer_size = (size_t) size_variable;
		}
	}
	return( result );
}

/* Sets the number of threads
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int verification_handle_set_number_of_threads(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function      = "verification_handle_set_number_of_threads";
	size_t string_length       = 0;
	uint64_t number_of_threads = 0;
	int result                 = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string[ 0 ] != (system_character_t) '-' )
	{
		string_length = system_string_length(
				 string );

		if( ewftools_system_string_decimal_copy_to_64_bit(
		     string,
		     string_length + 1,
		     &number_of_threads,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine number of threads.",
			 function );

			return( -1 );
		}
		result = 1;

		if( number_of_threads > 32 )
		{
			result = 0;
		}
		else
		{
			verification_handle->number_of_threads = (int) number_of_threads;
		}
	}
	return( result );
}

/* Sets the additional digest types
 * Returns 1 if successful or -1 on error
 */
int verification_handle_set_additional_digest_types(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	system_character_t *string_segment               = NULL;
	static char *function                            = "verification_handle_set_additional_digest_types";
	size_t string_length                             = 0;
	size_t string_segment_size                       = 0;
	uint8_t calculate_sha1                           = 0;
	uint8_t calculate_sha256                         = 0;
	int number_of_segments                           = 0;
	int result                                       = 0;
	int segment_index                                = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	libcsplit_wide_split_string_t *string_elements   = NULL;
#else
	libcsplit_narrow_split_string_t *string_elements = NULL;
#endif

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_string_split(
	     string,
	     string_length + 1,
	     (wchar_t) ',',
	     &string_elements,
	     error ) != 1 )
#else
	if( libcsplit_narrow_string_split(
	     string,
	     string_length + 1,
	     (char) ',',
	     &string_elements,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_split_string_get_number_of_segments(
	     string_elements,
	     &number_of_segments,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_get_number_of_segments(
	     string_elements,
	     &number_of_segments,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments.",
		 function );

		goto on_error;
	}
	for( segment_index = 0;
	     segment_index < number_of_segments;
	     segment_index++ )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcsplit_wide_split_string_get_segment_by_index(
		     string_elements,
		     segment_index,
		     &string_segment,
		     &string_segment_size,
		     error ) != 1 )
#else
		if( libcsplit_narrow_split_string_get_segment_by_index(
		     string_elements,
		     segment_index,
		     &string_segment,
		     &string_segment_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string segment: %d.",
			 function,
			 segment_index );

			goto on_error;
		}
		if( string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing string segment: %d.",
			 function,
			 segment_index );

			goto on_error;
		}
		if( string_segment_size == 5 )
		{
			if( system_string_compare(
			     string_segment,
			     _SYSTEM_STRING( "sha1" ),
			     4 ) == 0 )
			{
				calculate_sha1 = 1;
			}
			else if( system_string_compare(
			          string_segment,
			          _SYSTEM_STRING( "SHA1" ),
			          4 ) == 0 )
			{
				calculate_sha1 = 1;
			}
		}
		else if( string_segment_size == 6 )
		{
			if( system_string_compare(
			     string_segment,
			     _SYSTEM_STRING( "sha-1" ),
			     5 ) == 0 )
			{
				calculate_sha1 = 1;
			}
			else if( system_string_compare(
			          string_segment,
			          _SYSTEM_STRING( "sha_1" ),
			          5 ) == 0 )
			{
				calculate_sha1 = 1;
			}
			else if( system_string_compare(
			          string_segment,
			          _SYSTEM_STRING( "SHA-1" ),
			          5 ) == 0 )
			{
				calculate_sha1 = 1;
			}
			else if( system_string_compare(
			          string_segment,
			          _SYSTEM_STRING( "SHA_1" ),
			          5 ) == 0 )
			{
				calculate_sha1 = 1;
			}
		}
		else if( string_segment_size == 7 )
		{
			if( system_string_compare(
			     string_segment,
			     _SYSTEM_STRING( "sha256" ),
			     6 ) == 0 )
			{
				calculate_sha256 = 1;
			}
			else if( system_string_compare(
			          string_segment,
			          _SYSTEM_STRING( "SHA256" ),
			          6 ) == 0 )
			{
				calculate_sha256 = 1;
			}
		}
		else if( string_segment_size == 8 )
		{
			if( system_string_compare(
			     string_segment,
			     _SYSTEM_STRING( "sha-256" ),
			     7 ) == 0 )
			{
				calculate_sha256 = 1;
			}
			else if( system_string_compare(
			          string_segment,
			          _SYSTEM_STRING( "sha_256" ),
			          7 ) == 0 )
			{
				calculate_sha256 = 1;
			}
			else if( system_string_compare(
			          string_segment,
			          _SYSTEM_STRING( "SHA-256" ),
			          7 ) == 0 )
			{
				calculate_sha256 = 1;
			}
			else if( system_string_compare(
			          string_segment,
			          _SYSTEM_STRING( "SHA_256" ),
			          7 ) == 0 )
			{
				calculate_sha256 = 1;
			}
		}
	}
	if( ( calculate_sha1 != 0 )
	 && ( verification_handle->calculate_sha1 == 0 ) )
	{
		verification_handle->calculated_sha1_hash_string = system_string_allocate(
		                                                    41 );

		if( verification_handle->calculated_sha1_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create calculated SHA1 digest hash string.",
			 function );

			goto on_error;
		}
		verification_handle->calculate_sha1 = 1;
	}
	if( ( calculate_sha256 != 0 )
	 && ( verification_handle->calculate_sha256 == 0 ) )
	{
		verification_handle->calculated_sha256_hash_string = system_string_allocate(
		                                                      65 );

		if( verification_handle->calculated_sha256_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create calculated SHA256 digest hash string.",
			 function );

			goto on_error;
		}
		verification_handle->calculate_sha256 = 1;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_split_string_free(
	     &string_elements,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_free(
	     &string_elements,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split string.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( string_elements != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		libcsplit_wide_split_string_free(
		 &string_elements,
		 NULL );
#else
		libcsplit_narrow_split_string_free(
		 &string_elements,
		 NULL );
#endif
	}
	return( -1 );
}

/* Sets the zero chunk on error
 * Returns 1 if successful or -1 on error
 */
int verification_handle_set_zero_chunk_on_error(
     verification_handle_t *verification_handle,
     uint8_t zero_chunk_on_error,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_set_zero_chunk_on_error";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_read_zero_chunk_on_error(
	     verification_handle->input_handle,
	     zero_chunk_on_error,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set zero chunk on error in input handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Appends a read error to the output handle
 * Returns 1 if successful or -1 on error
 */
int verification_handle_append_read_error(
      verification_handle_t *verification_handle,
      off64_t start_offset,
      size_t number_of_bytes,
      libcerror_error_t **error )
{
	static char *function      = "verification_handle_append_read_error";
	uint64_t start_sector      = 0;
	uint64_t number_of_sectors = 0;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( verification_handle->bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing bytes per sector.",
		 function );

		return( -1 );
	}
	if( verification_handle->use_data_chunk_functions != 0 )
	{
		start_sector      = start_offset / verification_handle->bytes_per_sector;
		number_of_sectors = number_of_bytes / verification_handle->bytes_per_sector;

		if( ( number_of_bytes % verification_handle->bytes_per_sector ) != 0 )
		{
			number_of_sectors += 1;
		}
		if( libewf_handle_append_checksum_error(
		     verification_handle->input_handle,
		     start_sector,
		     number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append checksum error.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Print the hash values to a stream
 * Returns 1 if successful or -1 on error
 */
int verification_handle_hash_values_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	static char *function = "verification_handle_hash_values_fprint";

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( verification_handle->calculate_md5 != 0 )
	{
		if( verification_handle->stored_md5_hash_available == 0 )
		{
			fprintf(
			 stream,
			 "MD5 hash stored in file:\t\tN/A\n" );
		}
		else
		{
			fprintf(
			 stream,
			 "MD5 hash stored in file:\t\t%" PRIs_SYSTEM "\n",
			 verification_handle->stored_md5_hash_string );
		}
		fprintf(
		 stream,
		 "MD5 hash calculated over data:\t\t%" PRIs_SYSTEM "\n",
		 verification_handle->calculated_md5_hash_string );
	}
	if( verification_handle->calculate_sha1 != 0 )
	{
		if( verification_handle->stored_sha1_hash_available == 0 )
		{
			fprintf(
			 stream,
			 "SHA1 hash stored in file:\t\tN/A\n" );
		}
		else
		{
			fprintf(
			 stream,
			 "SHA1 hash stored in file:\t\t%" PRIs_SYSTEM "\n",
			 verification_handle->stored_sha1_hash_string );
		}
		fprintf(
		 stream,
		 "SHA1 hash calculated over data:\t\t%" PRIs_SYSTEM "\n",
		 verification_handle->calculated_sha1_hash_string );
	}
	if( verification_handle->calculate_sha256 != 0 )
	{
		if( verification_handle->stored_sha256_hash_available == 0 )
		{
			fprintf(
			 stream,
			 "SHA256 hash stored in file:\t\tN/A\n" );
		}
		else
		{
			fprintf(
			 stream,
			 "SHA256 hash stored in file:\t\t%" PRIs_SYSTEM "\n",
			 verification_handle->stored_sha256_hash_string );
		}
		fprintf(
		 stream,
		 "SHA256 hash calculated over data:\t%" PRIs_SYSTEM "\n",
		 verification_handle->calculated_sha256_hash_string );
	}
	return( 1 );
}

/* Print the additional hash values to a stream
 * Returns 1 if successful or -1 on error
 */
int verification_handle_additional_hash_values_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	char hash_value_identifier[ VERIFICATION_HANDLE_VALUE_IDENTIFIER_SIZE ];
	system_character_t hash_value[ VERIFICATION_HANDLE_VALUE_SIZE ];

	static char *function             = "verification_handle_additional_hash_values_fprint";
	size_t hash_value_identifier_size = VERIFICATION_HANDLE_VALUE_IDENTIFIER_SIZE;
	size_t hash_value_size            = VERIFICATION_HANDLE_VALUE_SIZE;
	uint32_t number_of_values         = 0;
	uint32_t hash_value_iterator      = 0;
	uint8_t print_header              = 1;
	int result                        = 1;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_number_of_hash_values(
	     verification_handle->input_handle,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of hash values.",
		 function );

		return( -1 );
	}
	for( hash_value_iterator = 0;
	     hash_value_iterator < number_of_values;
	     hash_value_iterator++ )
	{
		if( libewf_handle_get_hash_value_identifier_size(
		     verification_handle->input_handle,
		     hash_value_iterator,
		     &hash_value_identifier_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the hash value identifier size for index: %" PRIu32 ".",
			 function,
			 hash_value_iterator );

			result = -1;

			continue;
		}
		if( hash_value_identifier_size > VERIFICATION_HANDLE_VALUE_IDENTIFIER_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: hash value identifier size value out of bounds for index: %" PRIu32 ".",
			 function,
			 hash_value_iterator );

			result = -1;

			continue;
		}
		if( libewf_handle_get_hash_value_identifier(
		     verification_handle->input_handle,
		     hash_value_iterator,
		     (uint8_t *) hash_value_identifier,
		     hash_value_identifier_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the hash identifier for index: %" PRIu32 ".",
			 function,
			 hash_value_iterator );

			result = -1;

			continue;
		}
		if( verification_handle->calculate_md5 != 0 )
		{
			if( hash_value_identifier_size == 4 )
			{
				if( narrow_string_compare(
				     hash_value_identifier,
				     "MD5",
				     3 ) == 0 )
				{
					continue;
				}
			}
		}
		if( verification_handle->calculate_sha1 != 0 )
		{
			if( hash_value_identifier_size == 5 )
			{
				if( narrow_string_compare(
				     hash_value_identifier,
				     "SHA1",
				     4 ) == 0 )
				{
					continue;
				}
			}
		}
		if( verification_handle->calculate_sha256 != 0 )
		{
			if( hash_value_identifier_size == 7 )
			{
				if( narrow_string_compare(
				     hash_value_identifier,
				     "SHA256",
				     6 ) == 0 )
				{
					continue;
				}
			}
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_handle_get_utf16_hash_value(
		     verification_handle->input_handle,
		     (uint8_t *) hash_value_identifier,
		     hash_value_identifier_size - 1,
		     (uint16_t *) hash_value,
		     hash_value_size,
		     error ) != 1 )
#else
		if( libewf_handle_get_utf8_hash_value(
		     verification_handle->input_handle,
		     (uint8_t *) hash_value_identifier,
		     hash_value_identifier_size - 1,
		     (uint8_t *) hash_value,
		     hash_value_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the hash value for identifier: %s.",
			 function,
			 hash_value_identifier );

			result = -1;
		}
		else
		{
			if( print_header != 0 )
			{
				fprintf(
				 stream,
				 "\nAdditional hash values:\n" );

				print_header = 0;
			}
			fprintf(
			 stream,
			 "%s:\t%" PRIs_SYSTEM "\n",
			 hash_value_identifier,
			 hash_value );
		}
	}
	return( result );
}

/* Print the checksum errors to a stream
 * Returns 1 if successful or -1 on error
 */
int verification_handle_checksum_errors_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	system_character_t *filename      = NULL;
	system_character_t *last_filename = NULL;
	static char *function             = "verification_handle_checksum_errors_fprint";
	size_t filename_size              = 0;
	size_t last_filename_size         = 0;
	uint64_t last_sector              = 0;
	uint64_t number_of_sectors        = 0;
	uint64_t start_sector             = 0;
	uint32_t error_index              = 0;
	uint32_t number_of_errors         = 0;
	int result                        = 0;
	int return_value                  = 1;

	if( verification_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_number_of_checksum_errors(
	     verification_handle->input_handle,
	     &number_of_errors,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of checksum errors.",
		 function );

		goto on_error;
	}
	if( number_of_errors > 0 )
	{
		fprintf(
		stream,
		 "Sector validation errors:\n" );
		fprintf(
		 stream,
		 "\ttotal number: %" PRIu32 "\n",
		 number_of_errors );

		for( error_index = 0;
		     error_index < number_of_errors;
		     error_index++ )
		{
			if( libewf_handle_get_checksum_error(
			     verification_handle->input_handle,
			     error_index,
			     &start_sector,
			     &number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the checksum error: %" PRIu32 ".",
				 function,
				 error_index );

				start_sector      = 0;
				number_of_sectors = 0;

				return_value = -1;

				continue;
			}
			last_sector = start_sector + number_of_sectors - 1;

			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " (number: %" PRIu64 ")",
			 start_sector,
			 last_sector,
			 number_of_sectors );

			fprintf(
			 stream,
			 " in segment file(s):" );

			start_sector *= verification_handle->bytes_per_sector;
			last_sector  *= verification_handle->bytes_per_sector;

			while( start_sector <= last_sector )
			{
				if( libewf_handle_seek_offset(
				     verification_handle->input_handle,
				     (off64_t) start_sector,
				     SEEK_SET,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to seek offset: %" PRIi64 ".",
					 function,
					 start_sector );

					goto on_error;
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result= libewf_handle_get_filename_size_wide(
				         verification_handle->input_handle,
				         &filename_size,
				         error );
#else
				result = libewf_handle_get_filename_size(
				          verification_handle->input_handle,
				          &filename_size,
				          error );
#endif
				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve filename size.",
					 function );

					goto on_error;
				}
				else if( result != 0 )
				{
					filename = system_string_allocate(
					            filename_size );

					if( filename == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create filename.",
						 function );

						goto on_error;
					}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					if( libewf_handle_get_filename_wide(
					     verification_handle->input_handle,
					     filename,
					     filename_size,
					     error ) != 1 )
#else
					if( libewf_handle_get_filename(
					     verification_handle->input_handle,
					     filename,
					     filename_size,
					     error ) != 1 )
#endif
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve filename.",
						 function );

						goto on_error;
					}
					if( last_filename == NULL )
					{
						fprintf(
						 stream,
						 " %" PRIs_SYSTEM "",
						 filename );

						last_filename      = filename;
						last_filename_size = filename_size;
					}
					else if( ( last_filename_size != filename_size )
					      || ( memory_compare(
						    last_filename,
						    filename,
						    filename_size ) != 0 ) )
					{
						fprintf(
						 stream,
						 ", %" PRIs_SYSTEM "",
						 filename );

						memory_free(
						 last_filename );

						last_filename      = filename;
						last_filename_size = filename_size;
					}
					else
					{
						memory_free(
						 filename );
					}
					filename      = NULL;
					filename_size = 0;
				}
				start_sector += verification_handle->chunk_size;

			}
			if( last_filename != NULL )
			{
				memory_free(
				 last_filename );

				last_filename      = NULL;
				last_filename_size = 0;
			}
			fprintf(
			 stream,
			 "\n" );
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( return_value );

on_error:
	if( last_filename != NULL )
	{
		memory_free(
		 last_filename );
	}
	if( filename != NULL )
	{
		memory_free(
		 filename );
	}
	return( -1 );
}

