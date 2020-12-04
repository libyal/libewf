/*
 * Imaging handle
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

#if defined( WINAPI )
#include <rpcdce.h>

#elif defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

#include "byte_size_string.h"
#include "digest_hash.h"
#include "ewfcommon.h"
#include "ewfinput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libcfile.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcsplit.h"
#include "ewftools_libewf.h"
#include "ewftools_libhmac.h"
#include "ewftools_system_string.h"
#include "guid.h"
#include "imaging_handle.h"
#include "platform.h"
#include "process_status.h"
#include "storage_media_buffer.h"
#include "storage_media_buffer_queue.h"

#define IMAGING_HANDLE_INPUT_BUFFER_SIZE		64
#define IMAGING_HANDLE_STRING_SIZE			1024
#define IMAGING_HANDLE_NOTIFY_STREAM			stdout
#define IMAGING_HANDLE_MAXIMUM_PROCESS_BUFFERS_SIZE	64 * 1024 * 1024

/* Creates an imaging handle
 * Make sure the value imaging_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_initialize(
     imaging_handle_t **imaging_handle,
     uint8_t calculate_md5,
     uint8_t use_data_chunk_functions,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_initialize";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( *imaging_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid imaging handle value already set.",
		 function );

		return( -1 );
	}
	*imaging_handle = memory_allocate_structure(
	                   imaging_handle_t );

	if( *imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create imaging handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *imaging_handle,
	     0,
	     sizeof( imaging_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear imaging handle.",
		 function );

		memory_free(
		 *imaging_handle );

		*imaging_handle = NULL;

		return( -1 );
	}
	( *imaging_handle )->input_buffer = system_string_allocate(
	                                     IMAGING_HANDLE_INPUT_BUFFER_SIZE );

	if( ( *imaging_handle )->input_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create input buffer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *imaging_handle )->input_buffer,
	     0,
	     sizeof( system_character_t ) * IMAGING_HANDLE_INPUT_BUFFER_SIZE ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear imaging handle.",
		 function );

		goto on_error;
	}
	if( libewf_handle_initialize(
	     &( ( *imaging_handle )->output_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create output handle.",
		 function );

		goto on_error;
	}
	if( calculate_md5 != 0 )
	{
		( *imaging_handle )->calculated_md5_hash_string = system_string_allocate(
								   33 );

		if( ( *imaging_handle )->calculated_md5_hash_string == NULL )
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
	( *imaging_handle )->calculate_md5            = calculate_md5;
	( *imaging_handle )->use_data_chunk_functions = use_data_chunk_functions;
	( *imaging_handle )->compression_method       = LIBEWF_COMPRESSION_METHOD_DEFLATE;
	( *imaging_handle )->compression_level        = LIBEWF_COMPRESSION_LEVEL_NONE;
	( *imaging_handle )->ewf_format               = LIBEWF_FORMAT_ENCASE6;
	( *imaging_handle )->media_type               = LIBEWF_MEDIA_TYPE_FIXED;
	( *imaging_handle )->media_flags              = LIBEWF_MEDIA_FLAG_PHYSICAL;
	( *imaging_handle )->bytes_per_sector         = 512;
	( *imaging_handle )->sectors_per_chunk        = 64;
	( *imaging_handle )->sector_error_granularity = 64;
	( *imaging_handle )->maximum_segment_size     = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;
	( *imaging_handle )->header_codepage          = LIBEWF_CODEPAGE_ASCII;
	( *imaging_handle )->process_buffer_size      = EWFCOMMON_PROCESS_BUFFER_SIZE;
	( *imaging_handle )->notify_stream            = IMAGING_HANDLE_NOTIFY_STREAM;

#if defined( HAVE_MULTI_THREAD_SUPPORT )
	( *imaging_handle )->number_of_threads        = 4;
#endif
	return( 1 );

on_error:
	if( *imaging_handle != NULL )
	{
		if( ( *imaging_handle )->calculated_sha1_hash_string != NULL )
		{
			memory_free(
			 ( *imaging_handle )->calculated_sha1_hash_string );
		}
		if( ( *imaging_handle )->calculated_md5_hash_string != NULL )
		{
			memory_free(
			 ( *imaging_handle )->calculated_md5_hash_string );
		}
		if( ( *imaging_handle )->output_handle != NULL )
		{
			libewf_handle_free(
			 &( ( *imaging_handle )->output_handle ),
			 NULL );
		}
		if( ( *imaging_handle )->input_buffer != NULL )
		{
			memory_free(
			 ( *imaging_handle )->input_buffer );
		}
		memory_free(
		 *imaging_handle );

		*imaging_handle = NULL;
	}
	return( -1 );
}

/* Frees an imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_free(
     imaging_handle_t **imaging_handle,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_free";
	int result            = 1;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( *imaging_handle != NULL )
	{
		memory_free(
		 ( *imaging_handle )->input_buffer );

		if( ( *imaging_handle )->target_filename != NULL )
		{
			memory_free(
			 ( *imaging_handle )->target_filename );
		}
		if( ( *imaging_handle )->secondary_target_filename != NULL )
		{
			memory_free(
			 ( *imaging_handle )->secondary_target_filename );
		}
		if( ( *imaging_handle )->case_number != NULL )
		{
			memory_free(
			 ( *imaging_handle )->case_number );
		}
		if( ( *imaging_handle )->description != NULL )
		{
			memory_free(
			 ( *imaging_handle )->description );
		}
		if( ( *imaging_handle )->evidence_number != NULL )
		{
			memory_free(
			 ( *imaging_handle )->evidence_number );
		}
		if( ( *imaging_handle )->examiner_name != NULL )
		{
			memory_free(
			 ( *imaging_handle )->examiner_name );
		}
		if( ( *imaging_handle )->notes != NULL )
		{
			memory_free(
			 ( *imaging_handle )->notes );
		}
		if( ( *imaging_handle )->md5_context != NULL )
		{
			if( libhmac_md5_free(
			     &( ( *imaging_handle )->md5_context ),
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
		if( ( *imaging_handle )->calculated_md5_hash_string != NULL )
		{
			memory_free(
			 ( *imaging_handle )->calculated_md5_hash_string );
		}
		if( ( *imaging_handle )->sha1_context != NULL )
		{
			if( libhmac_sha1_free(
			     &( ( *imaging_handle )->sha1_context ),
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
		if( ( *imaging_handle )->calculated_sha1_hash_string != NULL )
		{
			memory_free(
			 ( *imaging_handle )->calculated_sha1_hash_string );
		}
		if( ( *imaging_handle )->sha256_context != NULL )
		{
			if( libhmac_sha256_free(
			     &( ( *imaging_handle )->sha256_context ),
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
		if( ( *imaging_handle )->calculated_sha256_hash_string != NULL )
		{
			memory_free(
			 ( *imaging_handle )->calculated_sha256_hash_string );
		}
		if( libewf_handle_free(
		     &( ( *imaging_handle )->output_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free output handle.",
			 function );

			result = -1;
		}
		if( ( *imaging_handle )->secondary_output_handle != NULL )
		{
			if( libewf_handle_free(
			     &( ( *imaging_handle )->secondary_output_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free secondary output handle.",
				 function );

				result = -1;
			}
		}
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
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_signal_abort";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_signal_abort(
	     imaging_handle->output_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal output handle to abort.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{	
		if( libewf_handle_signal_abort(
		     imaging_handle->output_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal secondary output handle to abort.",
			 function );

			return( -1 );
		}
	}
	imaging_handle->abort = 1;

	return( 1 );
}

/* Checks if a file can be written
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_check_write_access(
     imaging_handle_t *imaging_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	libcfile_file_t *target_file = NULL;
	static char *function        = "imaging_handle_check_write_access";
	int result                   = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( libcfile_file_initialize(
	     &target_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create target file.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcfile_file_open_wide(
		  target_file,
		  filename,
		  LIBCFILE_OPEN_WRITE,
		  error );
#else
	result = libcfile_file_open(
		  target_file,
		  filename,
		  LIBCFILE_OPEN_WRITE,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open target file.",
		 function );

		goto on_error;
	}
	if( libcfile_file_close(
	     target_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close target file.",
		 function );

		goto on_error;
	}
	if( libcfile_file_free(
	     &target_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free target file.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcfile_file_remove_wide(
		  filename,
		  error );
#else
	result = libcfile_file_remove(
		  filename,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_UNLINK_FAILED,
		 "%s: unable to remove target file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( target_file != NULL )
	{
		libcfile_file_free(
		 &target_file,
		 NULL );
	}
	return( -1 );
}

/* Opens the output of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_open_output(
     imaging_handle_t *imaging_handle,
     const system_character_t *filename,
     uint8_t resume,
     libcerror_error_t **error )
{
	system_character_t **libewf_filenames = NULL;
	system_character_t *filenames[ 1 ]    = { NULL };
	static char *function                 = "imaging_handle_open_output";
	size_t first_filename_length          = 0;
	int access_flags                      = 0;
	int number_of_filenames               = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filenames[ 0 ]      = (system_character_t *) filename;
	number_of_filenames = 1;

	if( resume != 0 )
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
		access_flags = LIBEWF_OPEN_WRITE_RESUME;
	}
	else
	{
		libewf_filenames = filenames;
		access_flags     = LIBEWF_OPEN_WRITE;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     imaging_handle->output_handle,
	     libewf_filenames,
	     number_of_filenames,
	     access_flags,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     imaging_handle->output_handle,
	     libewf_filenames,
	     number_of_filenames,
	     access_flags,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	if( libewf_filenames != filenames )
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

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( libewf_filenames != filenames )
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
	}
	return( -1 );
}

/* Opens the secondary output of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_open_secondary_output(
     imaging_handle_t *imaging_handle,
     const system_character_t *filename,
     uint8_t resume,
     libcerror_error_t **error )
{
	system_character_t **libewf_filenames = NULL;
	system_character_t *filenames[ 1 ]    = { NULL };
	static char *function                 = "imaging_handle_open_secondary_output";
	size_t first_filename_length          = 0;
	int access_flags                      = 0;
	int number_of_filenames               = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid imaging handle - secondary output handle already set.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filenames[ 0 ]      = (system_character_t *) filename;
	number_of_filenames = 1;

	if( resume != 0 )
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
		access_flags = LIBEWF_OPEN_WRITE_RESUME;
	}
	else
	{
		libewf_filenames = filenames;
		access_flags     = LIBEWF_OPEN_WRITE;
	}
	if( libewf_handle_initialize(
	     &( imaging_handle->secondary_output_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create secondary output handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     imaging_handle->secondary_output_handle,
	     libewf_filenames,
	     number_of_filenames,
	     access_flags,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     imaging_handle->secondary_output_handle,
	     libewf_filenames,
	     number_of_filenames,
	     access_flags,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	if( libewf_filenames != filenames )
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

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( imaging_handle->secondary_output_handle != NULL )
	{
		libewf_handle_free(
		 &( imaging_handle->secondary_output_handle ),
		 NULL );
	}
	if( libewf_filenames != filenames )
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
	}
	return( -1 );
}

/* Opens the output of the imaging handle for resume
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_open_output_resume(
     imaging_handle_t *imaging_handle,
     const system_character_t *filename,
     off64_t *resume_acquiry_offset,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_open_output_resume";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle_open_output(
	     imaging_handle,
	     filename,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	if( imaging_handle_get_output_values(
	     imaging_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine previous acquiry parameters.",
		 function );

		goto on_error;
	}
	if( imaging_handle_get_offset(
	     imaging_handle,
	     resume_acquiry_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine resume acquiry offset.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	imaging_handle_close(
	 imaging_handle,
	 NULL );

	return( -1 );
}

/* Closes the imaging handle
 * Returns the 0 if succesful or -1 on error
 */
int imaging_handle_close(
     imaging_handle_t *imaging_handle,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_close";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_close(
	     imaging_handle->output_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close output handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
		if( libewf_handle_close(
		     imaging_handle->secondary_output_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close secondary output handle.",
			 function );

			return( -1 );
		}
	}
	return( 0 );
}

/* Writes a storage media buffer to the output of the imaging handle
 * Returns the number of bytes written or -1 on error
 */
ssize_t imaging_handle_write_storage_media_buffer(
         imaging_handle_t *imaging_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t write_size,
         libcerror_error_t **error )
{
	static char *function         = "imaging_handle_write_buffer";
	ssize_t secondary_write_count = 0;
	ssize_t write_count           = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	write_count = storage_media_buffer_write_to_handle(
	               storage_media_buffer,
	               imaging_handle->output_handle,
	               write_size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write storage media buffer.",
		 function );

/* TODO ask for alternative segment file location and try again
		if( ( error != NULL )
		 && ( libcerror_error_matches(
		       *error,
		       LIBCERROR_ERROR_DOMAIN_OUTPUT,
		       LIBCERROR_OUTPUT_ERROR_INSUFFICIENT_SPACE ) != 0 ) )
		{
			return( -1 );
		}
*/
		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
		secondary_write_count = storage_media_buffer_write_to_handle(
		                         storage_media_buffer,
		                         imaging_handle->secondary_output_handle,
		                         write_size,
		                         error );

		if( secondary_write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write storage media buffer to secondary output handle.",
			 function );

			if( ( error != NULL )
			 && ( libcerror_error_matches(
			       *error,
			       LIBCERROR_ERROR_DOMAIN_OUTPUT,
			       LIBCERROR_OUTPUT_ERROR_INSUFFICIENT_SPACE ) == 0 ) )
			{
				return( -1 );
			}
/* TODO ask for alternative segment file location and try again */
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
         libcerror_error_t **error )
{
	static char *function    = "imaging_handle_seek_offset";
	off64_t secondary_offset = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	offset = libewf_handle_seek_offset(
	          imaging_handle->output_handle,
	          offset,
	          SEEK_SET,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in output handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
		secondary_offset = libewf_handle_seek_offset(
		                    imaging_handle->secondary_output_handle,
		                    offset,
		                    SEEK_SET,
		                    error );

		if( secondary_offset == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
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
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_get_offset";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_offset(
	     imaging_handle->output_handle,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
     uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_swap_byte_pairs";
	size_t buffer_offset  = 0;
	uint8_t byte_value    = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
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
		 "%s: invalid read size value out of bounds.",
		 function );

		return( -1 );
	}
	/* If the last bit is set the value is odd
	 */
	if( ( buffer_size & 0x01 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid read size value is odd.",
		 function );

		return( -1 );
	}
	for( buffer_offset = 0;
	     buffer_offset < buffer_size;
	     buffer_offset += 2 )
	{
		byte_value                  = buffer[ buffer_offset ];
		buffer[ buffer_offset ]     = buffer[ buffer_offset + 1 ];
		buffer[ buffer_offset + 1 ] = byte_value;
	}
	return( 1 );
}

/* Initializes the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_initialize_integrity_hash(
     imaging_handle_t *imaging_handle,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_initialize_integrity_hash";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->calculate_md5 != 0 )
	{
		if( libhmac_md5_initialize(
		     &( imaging_handle->md5_context ),
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
		imaging_handle->md5_context_initialized = 1;
	}
	if( imaging_handle->calculate_sha1 != 0 )
	{
		if( libhmac_sha1_initialize(
		     &( imaging_handle->sha1_context ),
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
		imaging_handle->sha1_context_initialized = 1;
	}
	if( imaging_handle->calculate_sha256 != 0 )
	{
		if( libhmac_sha256_initialize(
		     &( imaging_handle->sha256_context ),
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
		imaging_handle->sha256_context_initialized = 1;
	}
	return( 1 );

on_error:
	if( imaging_handle->sha1_context != NULL )
	{
		libhmac_sha1_free(
		 &( imaging_handle->sha1_context ),
		 NULL );
	}
	if( imaging_handle->md5_context != NULL )
	{
		libhmac_md5_free(
		 &( imaging_handle->md5_context ),
		 NULL );
	}
	return( -1 );
}

/* Updates the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_update_integrity_hash(
     imaging_handle_t *imaging_handle,
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_update_integrity_hash";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
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
	if( imaging_handle->calculate_md5 != 0 )
	{
		if( libhmac_md5_update(
		     imaging_handle->md5_context,
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
	if( imaging_handle->calculate_sha1 != 0 )
	{
		if( libhmac_sha1_update(
		     imaging_handle->sha1_context,
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
	if( imaging_handle->calculate_sha256 != 0 )
	{
		if( libhmac_sha256_update(
		     imaging_handle->sha256_context,
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
int imaging_handle_finalize_integrity_hash(
     imaging_handle_t *imaging_handle,
     libcerror_error_t **error )
{
	uint8_t calculated_md5_hash[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t calculated_sha1_hash[ LIBHMAC_SHA1_HASH_SIZE ];
	uint8_t calculated_sha256_hash[ LIBHMAC_SHA256_HASH_SIZE ];

	static char *function = "imaging_handle_finalize_integrity_hash";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->calculate_md5 != 0 )
	{
		if( imaging_handle->calculated_md5_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid imaging handle - missing calculated MD5 hash string.",
			 function );

			return( -1 );
		}
		if( libhmac_md5_finalize(
		     imaging_handle->md5_context,
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
		if( digest_hash_copy_to_string(
		     calculated_md5_hash,
		     LIBHMAC_MD5_HASH_SIZE,
		     imaging_handle->calculated_md5_hash_string,
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
	if( imaging_handle->calculate_sha1 != 0 )
	{
		if( imaging_handle->calculated_sha1_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid imaging handle - missing calculated SHA1 hash string.",
			 function );

			return( -1 );
		}
		if( libhmac_sha1_finalize(
		     imaging_handle->sha1_context,
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
		if( digest_hash_copy_to_string(
		     calculated_sha1_hash,
		     LIBHMAC_SHA1_HASH_SIZE,
		     imaging_handle->calculated_sha1_hash_string,
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
	if( imaging_handle->calculate_sha256 != 0 )
	{
		if( imaging_handle->calculated_sha256_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid imaging handle - missing calculated SHA256 hash string.",
			 function );

			return( -1 );
		}
		if( libhmac_sha256_finalize(
		     imaging_handle->sha256_context,
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
		if( digest_hash_copy_to_string(
		     calculated_sha256_hash,
		     LIBHMAC_SHA256_HASH_SIZE,
		     imaging_handle->calculated_sha256_hash_string,
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

/* Starts the threads
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_threads_start(
     imaging_handle_t *imaging_handle,
     size_t process_buffer_size,
     uint8_t storage_media_buffer_mode,
     libcerror_error_t **error )
{
	static char *function              = "imaging_handle_threads_start";
	int maximum_number_of_queued_items = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	maximum_number_of_queued_items = 1 + (int) ( IMAGING_HANDLE_MAXIMUM_PROCESS_BUFFERS_SIZE / process_buffer_size );

	if( libcthreads_thread_pool_create(
	     &( imaging_handle->process_thread_pool ),
	     NULL,
	     imaging_handle->number_of_threads,
	     maximum_number_of_queued_items,
	     (int (*)(intptr_t *, void *)) &imaging_handle_process_storage_media_buffer_callback,
	     (void *) imaging_handle,
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
	     &( imaging_handle->output_thread_pool ),
	     NULL,
	     1,
	     maximum_number_of_queued_items,
	     (int (*)(intptr_t *, void *)) &imaging_handle_output_storage_media_buffer_callback,
	     (void *) imaging_handle,
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
	     &( imaging_handle->output_list ),
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
	     &( imaging_handle->storage_media_buffer_queue ),
	     imaging_handle->output_handle,
	     maximum_number_of_queued_items,
	     storage_media_buffer_mode,
	     process_buffer_size,
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
	return( 1 );

on_error:
	if( imaging_handle->process_thread_pool != NULL )
	{
		libcthreads_thread_pool_join(
		 &( imaging_handle->process_thread_pool ),
		 NULL );
	}
	if( imaging_handle->output_thread_pool != NULL )
	{
		libcthreads_thread_pool_join(
		 &( imaging_handle->output_thread_pool ),
		 NULL );
	}
	if( imaging_handle->output_list != NULL )
	{
		imaging_handle_empty_output_list(
		 imaging_handle,
		 NULL );
		libcdata_list_free(
		 &( imaging_handle->output_list ),
		 NULL,
		 NULL );
	}
	if( imaging_handle->storage_media_buffer_queue != NULL )
	{
		storage_media_buffer_queue_free(
		 &( imaging_handle->storage_media_buffer_queue ),
		 NULL );
	}
	return( -1 );
}

/* Stops the threads
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_threads_stop(
     imaging_handle_t *imaging_handle,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_threads_stop";
	int result            = 1;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->process_thread_pool != NULL )
	{
		if( libcthreads_thread_pool_join(
		     &( imaging_handle->process_thread_pool ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to join process thread pool.",
			 function );

			result = -1;
		}
	}
	if( imaging_handle->output_thread_pool != NULL )
	{
		if( libcthreads_thread_pool_join(
		     &( imaging_handle->output_thread_pool ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to join output thread pool.",
			 function );

			result = -1;
		}
	}
	if( imaging_handle->output_list != NULL )
	{
		if( imaging_handle_empty_output_list(
		     imaging_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty output list.",
			 function );

			result = -1;
		}
		if( libcdata_list_free(
		     &( imaging_handle->output_list ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free output list.",
			 function );

			result = -1;
		}
	}
	if( imaging_handle->storage_media_buffer_queue != NULL )
	{
		if( storage_media_buffer_queue_free(
		     &( imaging_handle->storage_media_buffer_queue ),
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

/* Prepares a storage media buffer for imaging
 * Callback function for the process thread pool
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_process_storage_media_buffer_callback(
     storage_media_buffer_t *storage_media_buffer,
     imaging_handle_t *imaging_handle )
{
        libcerror_error_t *error = NULL;
        static char *function    = "imaging_handle_process_storage_media_buffer_callback";
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
	if( imaging_handle->abort != 0 )
	{
		return( 1 );
	}
	process_count = storage_media_buffer_write_process(
			 storage_media_buffer,
			 &error );

	if( process_count < 0 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to prepare storage media buffer before write.",
		 function );

		goto on_error;
	}
	if( libcthreads_thread_pool_push(
	     imaging_handle->output_thread_pool,
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
		     imaging_handle->storage_media_buffer_queue,
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
	if( imaging_handle->abort == 0 )
	{
		imaging_handle_signal_abort(
		 imaging_handle,
		 NULL );
	}
	return( -1 );
}

/* Prepares a storage media buffer for writing to image file
 * Callback function for the process thread pool
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_output_storage_media_buffer_callback(
     storage_media_buffer_t *storage_media_buffer,
     imaging_handle_t *imaging_handle )
{
	libcdata_list_element_t *element      = NULL;
	libcdata_list_element_t *next_element = NULL;
        libcerror_error_t *error              = NULL;
        static char *function                 = "imaging_handle_output_storage_media_buffer_callback";
	ssize_t write_count                   = 0;
	int result                            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
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
	if( imaging_handle->abort != 0 )
	{
		return( 1 );
	}
	if( libcdata_list_insert_value(
	     imaging_handle->output_list,
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
	     imaging_handle->output_list,
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
		if( imaging_handle->abort != 0 )
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

			return( -1 );
		}
		if( storage_media_buffer->storage_media_offset != imaging_handle->last_offset_written )
		{
			break;
		}
		write_count = imaging_handle_write_storage_media_buffer(
			       imaging_handle,
			       storage_media_buffer,
			       storage_media_buffer->processed_size,
			       &error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write storage media buffer.",
			 function );

			storage_media_buffer = NULL;

			goto on_error;
		}
		imaging_handle->last_offset_written = storage_media_buffer->storage_media_offset + storage_media_buffer->processed_size;

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
		     imaging_handle->output_list,
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
		     imaging_handle->storage_media_buffer_queue,
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

		if( imaging_handle->acquiry_size == 0 )
		{
			result = process_status_update_unknown_total(
			          imaging_handle->process_status,
			          imaging_handle->last_offset_written,
			          &error );
		}
		else
		{
			result = process_status_update(
			          imaging_handle->process_status,
			          imaging_handle->last_offset_written,
			          imaging_handle->acquiry_size,
			          &error );
		}
		if( result != 1 )
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
		     imaging_handle->storage_media_buffer_queue,
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
	if( imaging_handle->abort == 0 )
	{
		imaging_handle_signal_abort(
		 imaging_handle,
		 NULL );
	}
	return( -1 );
}

/* Empties the output list
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_empty_output_list(
     imaging_handle_t *imaging_handle,
     libcerror_error_t **error )
{
	libcdata_list_element_t *element             = NULL;
	libcdata_list_element_t *next_element        = NULL;
	storage_media_buffer_t *storage_media_buffer = NULL;
        static char *function                        = "imaging_handle_empty_output_list";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_first_element(
	     imaging_handle->output_list,
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
		     imaging_handle->storage_media_buffer_queue,
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
		     imaging_handle->output_list,
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

/* Retrieves the process buffer size
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_get_process_buffer_size(
     imaging_handle_t *imaging_handle,
     uint8_t use_data_chunk_functions,
     size_t *process_buffer_size,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_get_process_buffer_size";
	uint32_t chunk_size   = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( process_buffer_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process buffer size.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_chunk_size(
	     imaging_handle->output_handle,
	     &chunk_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve output chunk size.",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk size.",
		 function );

		return( -1 );
	}
	if( use_data_chunk_functions != 0 )
	{
		*process_buffer_size = (size_t) chunk_size;
	}
	else if( imaging_handle->process_buffer_size == 0 )
	{
		*process_buffer_size = (size_t) chunk_size;
	}
	else
	{
		*process_buffer_size = imaging_handle->process_buffer_size;
	}
	return( 1 );
}

/* Prompts the user for a string
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_string(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_prompt_for_string";
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( internal_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( internal_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	if( *internal_string != NULL )
	{
		memory_free(
		 *internal_string );

		*internal_string      = NULL;
		*internal_string_size = 0;
	}
	*internal_string_size = IMAGING_HANDLE_STRING_SIZE;

	*internal_string = system_string_allocate(
	                    *internal_string_size );

	if( *internal_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create internal string.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *internal_string,
	     0,
	     *internal_string_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal string.",
		 function );

		goto on_error;
	}
	result = ewfinput_get_string_variable(
	          imaging_handle->notify_stream,
	          request_string,
	          *internal_string,
	          *internal_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string variable.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( *internal_string != NULL )
	{
		memory_free(
		 *internal_string );

		*internal_string = NULL;
	}
	*internal_string_size = 0;

	return( -1 );
}

/* Prompts the user for the compression method
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_compression_method(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	system_character_t *fixed_string_variable = NULL;
	static char *function                     = "imaging_handle_prompt_for_compression_method";
	uint8_t compression_methods_amount        = 0;
	int result                                = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->ewf_format != LIBEWF_FORMAT_V2_ENCASE7 )
	{
		compression_methods_amount = 1;
	}
	else
	{
		compression_methods_amount = EWFINPUT_COMPRESSION_METHODS_AMOUNT;
	}
	result = ewfinput_get_fixed_string_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          ewfinput_compression_methods,
	          compression_methods_amount,
	          EWFINPUT_COMPRESSION_METHODS_DEFAULT,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = ewfinput_determine_compression_method(
			  fixed_string_variable,
			  &( imaging_handle->compression_method ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine compression method.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Prompts the user for the compression level
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_compression_level(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	system_character_t *fixed_string_variable = NULL;
	static char *function                     = "imaging_handle_prompt_for_compression_level";
	int result                                = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_fixed_string_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          ewfinput_compression_levels,
	          EWFINPUT_COMPRESSION_LEVELS_AMOUNT,
	          EWFINPUT_COMPRESSION_LEVELS_DEFAULT,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = ewfinput_determine_compression_values(
			  fixed_string_variable,
			  &( imaging_handle->compression_level ),
			  &( imaging_handle->compression_flags ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine compression values.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Prompts the user for the format
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_format(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	system_character_t *fixed_string_variable = NULL;
	static char *function                     = "imaging_handle_prompt_for_format";
	int result                                = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_fixed_string_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          ewfinput_format_types,
	          EWFINPUT_FORMAT_TYPES_AMOUNT,
	          EWFINPUT_FORMAT_TYPES_DEFAULT,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = ewfinput_determine_ewf_format(
			  fixed_string_variable,
			  &( imaging_handle->ewf_format ),
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine format.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Prompts the user for the media type
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_media_type(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	system_character_t *fixed_string_variable = NULL;
	static char *function                     = "imaging_handle_prompt_for_media_type";
	uint8_t default_value                     = 0;
	int result                                = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
	{
		default_value = 1;
	}
	else if( imaging_handle->media_type == LIBEWF_MEDIA_TYPE_OPTICAL )
	{
		default_value = 2;
	}
	else if( imaging_handle->media_type == LIBEWF_MEDIA_TYPE_MEMORY )
	{
		default_value = 3;
	}
	else
	{
		default_value = EWFINPUT_MEDIA_TYPES_DEFAULT;
	}
	result = ewfinput_get_fixed_string_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          ewfinput_media_types,
	          EWFINPUT_MEDIA_TYPES_AMOUNT,
	          default_value,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = ewfinput_determine_media_type(
			  fixed_string_variable,
			  &( imaging_handle->media_type ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media type.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Prompts the user for the media flags
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_media_flags(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	system_character_t *fixed_string_variable = NULL;
	static char *function                     = "imaging_handle_prompt_for_media_flags";
	uint8_t default_value                     = 0;
	int result                                = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( ( imaging_handle->media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
	 || ( imaging_handle->media_type == LIBEWF_MEDIA_TYPE_OPTICAL ) )
	{
		default_value = 0;
	}
	else
	{
		default_value = 1;
	}
	result = ewfinput_get_fixed_string_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          ewfinput_media_flags,
	          EWFINPUT_MEDIA_FLAGS_AMOUNT,
	          default_value,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = ewfinput_determine_media_flags(
			  fixed_string_variable,
			  &( imaging_handle->media_flags ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media flags.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Prompts the user for the bytes per sector
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_bytes_per_sector(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	static char *function  = "imaging_handle_prompt_for_bytes_per_sector";
	uint64_t size_variable = 0;
	int result             = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_size_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          1,
	          UINT32_MAX,
	          imaging_handle->bytes_per_sector,
	          &size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		imaging_handle->bytes_per_sector = (uint32_t) size_variable;
	}
	return( result );
}

/* Prompts the user for the number of sectors per chunk
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_sectors_per_chunk(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	system_character_t *fixed_string_variable = NULL;
	static char *function                     = "imaging_handle_prompt_for_sectors_per_chunk";
	int result                                = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_fixed_string_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          ewfinput_sector_per_block_sizes,
	          EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT,
	          EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = ewfinput_determine_sectors_per_chunk(
			  fixed_string_variable,
			  &( imaging_handle->sectors_per_chunk ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sectors per chunk.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Prompts the user for the sector error granularity
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_sector_error_granularity(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	static char *function  = "imaging_handle_prompt_for_sector_error_granularity";
	uint64_t size_variable = 0;
	int result             = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_size_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          1,
	          imaging_handle->sectors_per_chunk,
	          (uint64_t) imaging_handle->sectors_per_chunk,
	          &size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		imaging_handle->sector_error_granularity = (uint32_t) size_variable;
	}
	else
	{
		imaging_handle->sector_error_granularity = imaging_handle->sectors_per_chunk;
	}
	return( result );
}

/* Prompts the user for the maximum segment size
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_maximum_segment_size(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	static char *function  = "imaging_handle_prompt_for_maximum_segment_size";
	uint64_t default_size  = 0;
	uint64_t size_variable = 0;
	uint64_t maximum_size  = 0;
	int result             = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
/* TODO what about linen 7 */
	if( ( imaging_handle->ewf_format == LIBEWF_FORMAT_ENCASE6 )
	 || ( imaging_handle->ewf_format == LIBEWF_FORMAT_ENCASE7 )
	 || ( imaging_handle->ewf_format == LIBEWF_FORMAT_V2_ENCASE7 ) )
	{
		maximum_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT;
       	}
	else
       	{
		maximum_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT;
       	}
       	if( default_size == 0 )
       	{
		default_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;
       	}
	result = ewfinput_get_byte_size_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
	          maximum_size,
	          default_size,
	          &size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size variable.",
		 function );

		return( -1 );
	}
	imaging_handle->maximum_segment_size = size_variable;

	return( result );
}

/* Prompts the user for the acquiry offset
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_acquiry_offset(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	static char *function        = "imaging_handle_prompt_for_acquiry_offset";
	uint64_t input_size_variable = 0;
	int result                   = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_size_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          0,
	          imaging_handle->input_media_size,
	          imaging_handle->acquiry_offset,
	          &input_size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size variable.",
		 function );

		return( -1 );
	}
	imaging_handle->acquiry_offset = input_size_variable;

	return( result );
}

/* Prompts the user for the acquiry size
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int imaging_handle_prompt_for_acquiry_size(
     imaging_handle_t *imaging_handle,
     const system_character_t *request_string,
     libcerror_error_t **error )
{
	static char *function        = "imaging_handle_prompt_for_acquiry_size";
	uint64_t default_input_size  = 0;
	uint64_t input_size_variable = 0;
	uint64_t maximum_input_size  = 0;
	int result                   = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	maximum_input_size = imaging_handle->input_media_size
	                   - imaging_handle->acquiry_offset;

	if( ( imaging_handle->acquiry_size == 0 )
	 || ( imaging_handle->acquiry_size > maximum_input_size ) )
	{
		default_input_size = maximum_input_size;
	}
	else
	{
		default_input_size = imaging_handle->acquiry_size;
	}
	result = ewfinput_get_size_variable(
	          imaging_handle->notify_stream,
	          imaging_handle->input_buffer,
	          IMAGING_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          0,
	          maximum_input_size,
	          default_input_size,
	          &input_size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size variable.",
		 function );

		return( -1 );
	}
	imaging_handle->acquiry_size = input_size_variable;

	return( result );
}

/* Retrieves the output values of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_get_output_values(
     imaging_handle_t *imaging_handle,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_get_output_values";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     (uint8_t *) "case_number",
	     11,
	     &( imaging_handle->case_number ),
	     &( imaging_handle->case_number_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: case_number.",
		 function );

		return( -1 );
	}
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     (uint8_t *) "description",
	     11,
	     &( imaging_handle->description ),
	     &( imaging_handle->description_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: description.",
		 function );

		return( -1 );
	}
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     (uint8_t *) "evidence_number",
	     15,
	     &( imaging_handle->evidence_number ),
	     &( imaging_handle->evidence_number_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: evidence_number.",
		 function );

		return( -1 );
	}
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     (uint8_t *) "examiner_name",
	     13,
	     &( imaging_handle->examiner_name ),
	     &( imaging_handle->examiner_name_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: examiner_name.",
		 function );

		return( -1 );
	}
	if( imaging_handle_get_header_value(
	     imaging_handle,
	     (uint8_t *) "notes",
	     5,
	     &( imaging_handle->notes ),
	     &( imaging_handle->notes_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: notes.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_bytes_per_sector(
	     imaging_handle->output_handle,
	     &( imaging_handle->bytes_per_sector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_media_size(
	     imaging_handle->output_handle,
	     &( imaging_handle->acquiry_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_media_type(
	     imaging_handle->output_handle,
	     &( imaging_handle->media_type ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media type.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_media_flags(
	     imaging_handle->output_handle,
	     &( imaging_handle->media_flags ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media flags.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_compression_values(
	     imaging_handle->output_handle,
	     &( imaging_handle->compression_level ),
	     &( imaging_handle->compression_flags ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compression values.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_format(
	     imaging_handle->output_handle,
	     &( imaging_handle->ewf_format ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve format.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_maximum_segment_size(
	     imaging_handle->output_handle,
	     &( imaging_handle->maximum_segment_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve maximum segment size.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_sectors_per_chunk(
	     imaging_handle->output_handle,
	     &( imaging_handle->sectors_per_chunk ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sectors per chunk.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_error_granularity(
	     imaging_handle->output_handle,
	     &( imaging_handle->sector_error_granularity ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve error granularity.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets a string
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_string(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_set_string";
	size_t string_length  = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( internal_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( internal_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	if( *internal_string != NULL )
	{
		memory_free(
		 *internal_string );

		*internal_string      = NULL;
		*internal_string_size = 0;
	}
	string_length = system_string_length(
	                 string );

	if( string_length > 0 )
	{
		*internal_string = system_string_allocate(
		                    string_length + 1 );

		if( *internal_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create internal string.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     *internal_string,
		     string,
		     string_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string.",
			 function );

			goto on_error;
		}
		( *internal_string )[ string_length ] = 0;

		*internal_string_size = string_length + 1;
	}
	return( 1 );

on_error:
	if( *internal_string != NULL )
	{
		memory_free(
		 *internal_string );

		*internal_string = NULL;
	}
	*internal_string_size = 0;

	return( -1 );
}

/* Sets the compression values
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_compression_values(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	system_character_t *string_segment               = NULL;
	static char *function                            = "imaging_handle_set_compression_values";
	size_t string_length                             = 0;
	size_t string_segment_size                       = 0;
	int number_of_segments                           = 0;
	int result                                       = 0;
	int segment_index                                = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	libcsplit_wide_split_string_t *string_elements   = NULL;
#else
	libcsplit_narrow_split_string_t *string_elements = NULL;
#endif

	if( imaging_handle == NULL )
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

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_string_split(
	     string,
	     string_length + 1,
	     (wchar_t) ':',
	     &string_elements,
	     error ) != 1 )
#else
	if( libcsplit_narrow_string_split(
	     string,
	     string_length + 1,
	     (char) ':',
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

		return( -1 );
	}
	if( ( number_of_segments != 1 )
	 && ( number_of_segments != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of segments is out of bounds.",
		 function );

		return( -1 );
	}
	if( number_of_segments == 2 )
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
		result = ewfinput_determine_compression_method(
			  string_segment,
			  &( imaging_handle->compression_method ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine compression method.",
			 function );

			goto on_error;
		}
		if( imaging_handle->ewf_format != LIBEWF_FORMAT_V2_ENCASE7 )
		{
			if( imaging_handle->compression_method != LIBEWF_COMPRESSION_METHOD_DEFLATE )
			{
				imaging_handle->compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;

				result = 0;
			}
		}
		segment_index++;
	}
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
	result = ewfinput_determine_compression_values(
	          string_segment,
	          &( imaging_handle->compression_level ),
	          &( imaging_handle->compression_flags ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine compression values.",
		 function );

		goto on_error;
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

/* Sets the format
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_format(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_set_format";
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_ewf_format(
	          string,
	          &( imaging_handle->ewf_format ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine format.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the media type
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_media_type(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_set_media_type";
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_media_type(
	          string,
	          &( imaging_handle->media_type ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine media type.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the media flags
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_media_flags(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_set_media_flags";
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_media_flags(
	          string,
	          &( imaging_handle->media_flags ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine media flags.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the bytes per sector
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_bytes_per_sector(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "imaging_handle_set_bytes_per_sector";
	size_t string_length   = 0;
	uint64_t size_variable = 0;
	int result             = 0;

	if( imaging_handle == NULL )
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
		 "%s: unable to determine bytes per sector.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( size_variable > (uint64_t) UINT32_MAX )
		{
			result = 0;
		}
		else
		{
			imaging_handle->bytes_per_sector = (uint32_t) size_variable;
		}
	}
	return( result );
}

/* Sets the number of sectors per chunk
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_sectors_per_chunk(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_set_sectors_per_chunk";
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_sectors_per_chunk(
	          string,
	          &( imaging_handle->sectors_per_chunk ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine sectors per chunk.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the sector error granularity
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_sector_error_granularity(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "imaging_handle_set_sector_error_granularity";
	size_t string_length   = 0;
	uint64_t size_variable = 0;
	int result             = 0;

	if( imaging_handle == NULL )
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
		 "%s: unable to determine sector error granularity.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( ( size_variable > (uint64_t) UINT32_MAX )
		 || ( size_variable > imaging_handle->sectors_per_chunk ) )
		{
			size_variable = imaging_handle->sectors_per_chunk;

			result = 0;
		}
		imaging_handle->sector_error_granularity = (uint32_t) size_variable;
	}
	return( result );
}

/* Sets the maximum segment file size
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_maximum_segment_size(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_set_maximum_segment_size";
	size_t string_length  = 0;
	int result            = 0;

	if( imaging_handle == NULL )
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

	result = byte_size_string_convert(
	          string,
	          string_length,
	          &( imaging_handle->maximum_segment_size ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine maximum segment size.",
		 function );

		return( -1 );
	}
        else if( result != 0 )
	{
		if( imaging_handle->maximum_segment_size < EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE )
		{
			result = 0;
		}
/* TODO what about linen 7 */
		else if( ( imaging_handle->ewf_format == LIBEWF_FORMAT_ENCASE6 )
		      || ( imaging_handle->ewf_format == LIBEWF_FORMAT_ENCASE7 )
		      || ( imaging_handle->ewf_format == LIBEWF_FORMAT_V2_ENCASE7 ) )
		{
			if( imaging_handle->maximum_segment_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT )
			{
				result = 0;
			}
		}
		else
		{
			if( imaging_handle->maximum_segment_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT )
			{
				result = 0;
			}
		}
		if( result == 0 )
		{
			imaging_handle->maximum_segment_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;
		}
	}
	return( result );
}

/* Sets the acquiry offset
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_acquiry_offset(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_set_acquiry_offset";
	size_t string_length  = 0;
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string[ 0 ] != (system_character_t) '-' )
	{
		string_length = system_string_length(
				 string );

		if( ewftools_system_string_decimal_copy_to_64_bit(
		     string,
		     string_length + 1,
		     &( imaging_handle->acquiry_offset ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine acquiry offset.",
			 function );

			return( -1 );
		}
		result = 1;
	}
	return( result );
}

/* Sets the acquiry size
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_acquiry_size(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_set_acquiry_size";
	size_t string_length  = 0;
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string[ 0 ] != (system_character_t) '-' )
	{
		string_length = system_string_length(
				 string );

		if( ewftools_system_string_decimal_copy_to_64_bit(
		     string,
		     string_length + 1,
		     &( imaging_handle->acquiry_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine acquiry size.",
			 function );

			return( -1 );
		}
		result = 1;
	}
	return( result );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_header_codepage(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_set_header_codepage";
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_header_codepage(
	          string,
	          &imaging_handle->header_codepage,
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
	return( result );
}

/* Sets the process buffer size
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_process_buffer_size(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "imaging_handle_set_process_buffer_size";
	size_t string_length   = 0;
	uint64_t size_variable = 0;
	int result             = 0;

	if( imaging_handle == NULL )
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
			imaging_handle->process_buffer_size = 0;

			result = 0;
		}
		else
		{
			imaging_handle->process_buffer_size = (size_t) size_variable;
		}
	}
	return( result );
}

/* Sets the number of threads
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int imaging_handle_set_number_of_threads(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function      = "imaging_handle_set_number_of_threads";
	size_t string_length       = 0;
	uint64_t number_of_threads = 0;
	int result                 = 0;

	if( imaging_handle == NULL )
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
			imaging_handle->number_of_threads = (int) number_of_threads;
		}
	}
	return( result );
}

/* Sets the additional digest types
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_additional_digest_types(
     imaging_handle_t *imaging_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	system_character_t *string_segment               = NULL;
	static char *function                            = "imaging_handle_set_additional_digest_types";
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

	if( imaging_handle == NULL )
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
	 && ( imaging_handle->calculate_sha1 == 0 ) )
	{
		imaging_handle->calculated_sha1_hash_string = system_string_allocate(
		                                               41 );

		if( imaging_handle->calculated_sha1_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create calculated SHA1 digest hash string.",
			 function );

			goto on_error;
		}
		imaging_handle->calculate_sha1 = 1;
	}
	if( ( calculate_sha256 != 0 )
	 && ( imaging_handle->calculate_sha256 == 0 ) )
	{
		imaging_handle->calculated_sha256_hash_string = system_string_allocate(
		                                                 65 );

		if( imaging_handle->calculated_sha256_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create calculated SHA256 digest hash string.",
			 function );

			goto on_error;
		}
		imaging_handle->calculate_sha256 = 1;
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

/* TODO refactor into a function that sets all the output values
 * for a single output handle, repeat it for the secondary output handle if necessary
 * this requires to unfold imaging_handle_set_header_value
 */

/* Sets the output values of the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_output_values(
     imaging_handle_t *imaging_handle,
     system_character_t *acquiry_software,
     system_character_t *acquiry_software_version,
     system_character_t *model,
     system_character_t *serial_number,
     libcerror_error_t **error )
{
	system_character_t acquiry_operating_system[ 32 ];

#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )
	uint8_t guid[ GUID_SIZE ];

	uint8_t guid_type     = 0;
#endif

	static char *function = "imaging_handle_set_output_values";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->case_number != NULL )
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "case_number",
		     11,
		     imaging_handle->case_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: case_number.",
			 function );

			return( -1 );
		}
	}
	if( imaging_handle->description != NULL )
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "description",
		     11,
		     imaging_handle->description,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: description.",
			 function );

			return( -1 );
		}
	}
	if( imaging_handle->evidence_number != NULL )
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "evidence_number",
		     15,
		     imaging_handle->evidence_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: evidence_number.",
			 function );

			return( -1 );
		}
	}
	if( imaging_handle->examiner_name != NULL )
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "examiner_name",
		     13,
		     imaging_handle->examiner_name,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: examiner_name.",
			 function );

			return( -1 );
		}
	}
	if( imaging_handle->notes != NULL )
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "notes",
		     5,
		     imaging_handle->notes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: notes.",
			 function );

			return( -1 );
		}
	}
	/* Password is not used within libewf
	 */

	/* Acquiry date, system date and compression level will be generated automatically when set to NULL
	 */

	/* Set acquiry operating system, software and software version
	 */
	if( platform_get_operating_system(
	     acquiry_operating_system,
	     32,
	     error ) != 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to retrieve operating system.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );
	}
	else
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "acquiry_operating_system",
		     24,
		     acquiry_operating_system,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: acquiry_operating_system.",
			 function );

			return( -1 );
		}
	}
	if( acquiry_software != NULL )
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "acquiry_software",
		     16,
		     acquiry_software,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: acquiry_software.",
			 function );

			return( -1 );
		}
	}
	if( acquiry_software_version != NULL )
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "acquiry_software_version",
		     24,
		     acquiry_software_version,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: acquiry_software_version.",
			 function );

			return( -1 );
		}
	}
	if( model != NULL )
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "model",
		     5,
		     model,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: model.",
			 function );

			return( -1 );
		}
	}
	if( serial_number != NULL )
	{
		if( imaging_handle_set_header_value(
		     imaging_handle,
		     (uint8_t *) "serial_number",
		     13,
		     serial_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: serial_number.",
			 function );

			return( -1 );
		}
	}
	if( libewf_handle_set_header_codepage(
	     imaging_handle->output_handle,
	     imaging_handle->header_codepage,
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
	if( libewf_handle_set_bytes_per_sector(
	     imaging_handle->output_handle,
	     imaging_handle->bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set bytes per sector.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_media_size(
	     imaging_handle->output_handle,
	     imaging_handle->acquiry_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media size.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_media_type(
	     imaging_handle->output_handle,
	     imaging_handle->media_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media type.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_media_flags(
	     imaging_handle->output_handle,
	     imaging_handle->media_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media flags.",
		 function );

		return( -1 );
	}
	/* Format needs to be set before segment file size and compression values
	 */
	if( libewf_handle_set_format(
	     imaging_handle->output_handle,
	     imaging_handle->ewf_format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set format.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_compression_method(
	     imaging_handle->output_handle,
	     imaging_handle->compression_method,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set compression method.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_compression_values(
	     imaging_handle->output_handle,
	     imaging_handle->compression_level,
	     imaging_handle->compression_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set compression values.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_maximum_segment_size(
	     imaging_handle->output_handle,
	     imaging_handle->maximum_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set maximum segment size.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_sectors_per_chunk(
	     imaging_handle->output_handle,
	     imaging_handle->sectors_per_chunk,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sectors per chunk.",
		 function );

		return( -1 );
	}
	if( ( imaging_handle->sector_error_granularity == 0 )
	 || ( imaging_handle->sector_error_granularity >= imaging_handle->sectors_per_chunk ) )
	{
		imaging_handle->sector_error_granularity = imaging_handle->sectors_per_chunk;
	}
	if( libewf_handle_set_error_granularity(
	     imaging_handle->output_handle,
	     imaging_handle->sector_error_granularity,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set error granularity.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
		if( libewf_handle_set_header_codepage(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header codepage in secondary output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_bytes_per_sector(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->bytes_per_sector,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set bytes per sector in secondary output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_media_size(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->acquiry_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set media size in secondary output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_media_type(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->media_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set media type in secondary output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_media_flags(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->media_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set media flags in secondary output handle.",
			 function );

			return( -1 );
		}
		/* Format needs to be set before segment file size and compression values
		 */
		if( libewf_handle_set_format(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->ewf_format,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set format in secondary output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_compression_method(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->compression_method,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set compression method in secondary output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_compression_values(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->compression_level,
		     imaging_handle->compression_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set compression values in secondary output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_maximum_segment_size(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->maximum_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set maximum segment size in secondary output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_sectors_per_chunk(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->sectors_per_chunk,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set sectors per chunk in secondary output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_error_granularity(
		     imaging_handle->secondary_output_handle,
		     imaging_handle->sector_error_granularity,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set error granularity in secondary output handle.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )
	if( ( imaging_handle->ewf_format == LIBEWF_FORMAT_ENCASE5 )
	 || ( imaging_handle->ewf_format == LIBEWF_FORMAT_ENCASE6 )
	 || ( imaging_handle->ewf_format == LIBEWF_FORMAT_ENCASE7 )
	 || ( imaging_handle->ewf_format == LIBEWF_FORMAT_LINEN7 )
	 || ( imaging_handle->ewf_format == LIBEWF_FORMAT_V2_ENCASE7 )
	 || ( imaging_handle->ewf_format == LIBEWF_FORMAT_EWFX ) )
	{
		guid_type = GUID_TYPE_RANDOM;
        }
	else if( ( imaging_handle->ewf_format == LIBEWF_FORMAT_LINEN5 )
	      || ( imaging_handle->ewf_format == LIBEWF_FORMAT_LINEN6 ) )
	{
		guid_type = GUID_TYPE_TIME;
	}
	if( guid_type != 0 )
	{
		/* Set GUID if necessary
		 */
		if( guid_generate(
		     guid,
		     GUID_SIZE,
		     guid_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to generate GUID for set identifier.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_segment_file_set_identifier(
		     imaging_handle->output_handle,
		     guid,
		     GUID_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set segment file set identifier.",
			 function );

			return( -1 );
		}
		if( imaging_handle->secondary_output_handle != NULL )
		{
			if( libewf_handle_set_segment_file_set_identifier(
			     imaging_handle->secondary_output_handle,
			     guid,
			     GUID_SIZE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set segment file set identifier in secondary output handle.",
				 function );

				return( -1 );
			}
		}
	}
#endif
	return( 1 );
}

/* Retrieves the header value
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_get_header_value(
     imaging_handle_t *imaging_handle,
     const uint8_t *identifier,
     size_t identifier_size,
     system_character_t **header_value,
     size_t *header_value_size,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_get_header_value";
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( header_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header value.",
		 function );

		return( -1 );
	}
	if( header_value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header value size.",
		 function );

		return( -1 );
	}
	if( *header_value != NULL )
	{
		memory_free(
		 *header_value );

		*header_value      = NULL;
		*header_value_size = 0;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_utf16_header_value_size(
	          imaging_handle->output_handle,
	          identifier,
	          identifier_size,
	          header_value_size,
	          error );
#else
	result = libewf_handle_get_utf8_header_value_size(
	          imaging_handle->output_handle,
	          identifier,
	          identifier_size,
	          header_value_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	if( ( result != 0 )
	 && ( *header_value_size > 0 ) )
	{
		*header_value = system_string_allocate(
		                 *header_value_size );

		if( *header_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create header value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_handle_get_utf16_header_value(
			  imaging_handle->output_handle,
			  identifier,
			  identifier_size,
			  (uint16_t *) *header_value,
			  *header_value_size,
			  error );
#else
		result = libewf_handle_get_utf8_header_value(
			  imaging_handle->output_handle,
			  identifier,
			  identifier_size,
			  (uint8_t *) *header_value,
			  *header_value_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *header_value != NULL )
	{
		memory_free(
		 *header_value );

		*header_value = NULL;
	}
	*header_value_size = 0;

	return( -1 );
}

/* Sets the header value in the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_set_header_value(
     imaging_handle_t *imaging_handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const system_character_t *header_value,
     libcerror_error_t **error )
{
	static char *function      = "imaging_handle_set_header_value";
	size_t header_value_length = 0;
	int result                 = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( header_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header value.",
		 function );

		return( -1 );
	}
	header_value_length = system_string_length(
	                       header_value );

	if( header_value_length > 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_handle_set_utf16_header_value(
			  imaging_handle->output_handle,
			  identifier,
			  identifier_length,
			  (uint16_t *) header_value,
			  header_value_length,
			  error );
#else
		result = libewf_handle_set_utf8_header_value(
			  imaging_handle->output_handle,
			  identifier,
			  identifier_length,
			  (uint8_t *) header_value,
			  header_value_length,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: %s.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
		if( imaging_handle->secondary_output_handle != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libewf_handle_set_utf16_header_value(
				  imaging_handle->secondary_output_handle,
				  identifier,
				  identifier_length,
				  (uint16_t *) header_value,
				  header_value_length,
				  error );
#else
			result = libewf_handle_set_utf8_header_value(
				  imaging_handle->secondary_output_handle,
				  identifier,
				  identifier_length,
				  (uint8_t *) header_value,
				  header_value_length,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set header value: %s in secondary output handle.",
				 function,
				 (char *) identifier );

				return( -1 );
			}
		}
	}
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
     libcerror_error_t **error )
{
	uint8_t *utf8_hash_value    = NULL;
	static char *function       = "imaging_handle_set_hash_value";
	size_t utf8_hash_value_size = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( ewftools_string_size_to_utf8_string(
	     hash_value,
	     hash_value_length + 1,
	     &utf8_hash_value_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 hash value size.",
		 function );

		goto on_error;
	}
	utf8_hash_value = (uint8_t *) memory_allocate(
	                               sizeof( uint8_t ) * utf8_hash_value_size );

	if( utf8_hash_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 hash value.",
		 function );

		goto on_error;
	}
	if( ewftools_string_copy_to_utf8_string(
	     hash_value,
	     hash_value_length + 1,
	     utf8_hash_value,
	     utf8_hash_value_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 hash value.",
		 function );

		goto on_error;
	}
	if( libewf_handle_set_utf8_hash_value(
	     imaging_handle->output_handle,
	     (uint8_t *) hash_value_identifier,
	     hash_value_identifier_length,
	     utf8_hash_value,
	     utf8_hash_value_size - 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set hash value: %s.",
		 function,
		 hash_value_identifier );

		goto on_error;
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
		if( libewf_handle_set_utf8_hash_value(
		     imaging_handle->secondary_output_handle,
		     (uint8_t *) hash_value_identifier,
		     hash_value_identifier_length,
		     utf8_hash_value,
		     utf8_hash_value_size - 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: %s in secondary output handle.",
			 function,
			 hash_value_identifier );

			goto on_error;
		}
	}
	memory_free(
	 utf8_hash_value );

	return( 1 );

on_error:
	if( utf8_hash_value != NULL )
	{
		memory_free(
		 utf8_hash_value );
	}
	return( -1 );
}

/* Appends a read error to the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_append_read_error(
      imaging_handle_t *imaging_handle,
      off64_t start_offset,
      size64_t number_of_bytes,
      libcerror_error_t **error )
{
	static char *function      = "imaging_handle_append_read_error";
	uint64_t number_of_sectors = 0;
	uint64_t start_sector      = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid imaging handle - invalid bytes per sector value out of bounds.",
		 function );

		return( -1 );
	}
	start_sector      = start_offset / imaging_handle->bytes_per_sector;
	number_of_sectors = number_of_bytes / imaging_handle->bytes_per_sector;

	if( ( number_of_bytes % imaging_handle->bytes_per_sector ) != 0 )
	{
		number_of_sectors += 1;
	}
	if( libewf_handle_append_acquiry_error(
	     imaging_handle->output_handle,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append acquiry error.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
		if( libewf_handle_append_acquiry_error(
		     imaging_handle->secondary_output_handle,
		     start_sector,
		     number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append acquiry error to secondary output handle.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Appends a session to the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_append_session(
     imaging_handle_t *imaging_handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_append_session";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_append_session(
	     imaging_handle->output_handle,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append session.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
		if( libewf_handle_append_session(
		     imaging_handle->secondary_output_handle,
		     start_sector,
		     number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append session to secondary output handle.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Appends a track to the output handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_append_track(
     imaging_handle_t *imaging_handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_append_track";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_append_track(
	     imaging_handle->output_handle,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append track.",
		 function );

		return( -1 );
	}
	if( imaging_handle->secondary_output_handle != NULL )
	{
		if( libewf_handle_append_track(
		     imaging_handle->secondary_output_handle,
		     start_sector,
		     number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append track to secondary output handle.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Starts the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_start(
     imaging_handle_t *imaging_handle,
     uint8_t print_status_information,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_start";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle_initialize_integrity_hash(
	     imaging_handle,
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
	     &( imaging_handle->process_status ),
	     _SYSTEM_STRING( "Acquiry" ),
	     _SYSTEM_STRING( "acquired" ),
	     _SYSTEM_STRING( "Written" ),
	     stdout,
	     print_status_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create process status",
		 function );

		goto on_error;
	}
	if( process_status_start(
	     imaging_handle->process_status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to start process status",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( imaging_handle->sha1_context != NULL )
	{
		libhmac_sha1_free(
		 &( imaging_handle->sha1_context ),
		 NULL );
	}
	if( imaging_handle->md5_context != NULL )
	{
		libhmac_md5_free(
		 &( imaging_handle->md5_context ),
		 NULL );
	}
	if( imaging_handle->process_status != NULL )
	{
		process_status_free(
		 &( imaging_handle->process_status ),
		 NULL );
	}
	return( -1 );
}

/* Updates the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_update(
     imaging_handle_t *imaging_handle,
     storage_media_buffer_t *storage_media_buffer,
     ssize_t read_count,
     off64_t resume_acquiry_offset,
     uint8_t swap_byte_pairs,
     libcerror_error_t **error )
{
	uint8_t *data         = NULL;
	static char *function = "imaging_handle_update";
	size_t data_size      = 0;
	ssize_t process_count = 0;
	ssize_t write_count   = 0;
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
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
		 "%s: unable to retrieve storage media buffer data.",
		 function );

		return( -1 );
	}
	/* Swap byte pairs
	 */
	if( ( swap_byte_pairs == 1 )
	 && ( imaging_handle->last_offset_written >= resume_acquiry_offset ) )
	{
		if( imaging_handle_swap_byte_pairs(
		     imaging_handle,
		     data,
		     (size_t) read_count,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to swap byte pairs.",
			 function );

			return( -1 );
		}
	}
	/* Integrity (digest) hashes are calcultated after swap
	 */
	if( imaging_handle_update_integrity_hash(
	     imaging_handle,
	     data,
	     (size_t) read_count,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to update integrity hash(es).",
		 function );

		return( -1 );
	}
	if( imaging_handle->last_offset_written < resume_acquiry_offset )
	{
		imaging_handle->last_offset_written += (off64_t) read_count;
	}
	else if( imaging_handle->number_of_threads == 0 )
	{
		process_count = storage_media_buffer_write_process(
				 storage_media_buffer,
				 error );

		if( process_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to prepare storage media buffer before write.",
			 function );

			return( -1 );
		}
		write_count = imaging_handle_write_storage_media_buffer(
			       imaging_handle,
			       storage_media_buffer,
			       storage_media_buffer->processed_size,
			       error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write storage media buffer.",
			 function );

			return( -1 );
		}
		imaging_handle->last_offset_written += process_count;
	}
	if( ( imaging_handle->last_offset_written < resume_acquiry_offset )
	 || ( imaging_handle->number_of_threads == 0 ) )
	{
		if( imaging_handle->acquiry_size == 0 )
		{
			result = process_status_update_unknown_total(
			          imaging_handle->process_status,
			          imaging_handle->last_offset_written,
			          error );
		}
		else
		{
			result = process_status_update(
			          imaging_handle->process_status,
			          imaging_handle->last_offset_written,
			          imaging_handle->acquiry_size,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update process status.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Strops the imaging handle
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_stop(
     imaging_handle_t *imaging_handle,
     off64_t resume_acquiry_offset,
     int status,
     libcerror_error_t **error )
{
	static char *function         = "imaging_handle_stop";
	ssize_t secondary_write_count = 0;
	ssize_t write_count           = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle_finalize_integrity_hash(
	     imaging_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize integrity hash(es).",
		 function );

		return( -1 );
	}
	if( imaging_handle->last_offset_written >= resume_acquiry_offset )
	{
		if( imaging_handle->calculate_md5 != 0 )
		{
			if( imaging_handle_set_hash_value(
			     imaging_handle,
			     "MD5",
			     3,
			     imaging_handle->calculated_md5_hash_string,
			     32,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set hash value: MD5.",
				 function );

				return( -1 );
			}
		}
		if( imaging_handle->calculate_sha1 != 0 )
		{
			if( imaging_handle_set_hash_value(
			     imaging_handle,
			     "SHA1",
			     4,
			     imaging_handle->calculated_sha1_hash_string,
			     40,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set hash value: SHA1.",
				 function );

				return( -1 );
			}
		}
		if( imaging_handle->calculate_sha256 != 0 )
		{
			if( imaging_handle_set_hash_value(
			     imaging_handle,
			     "SHA256",
			     6,
			     imaging_handle->calculated_sha256_hash_string,
			     64,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set hash value: SHA256.",
				 function );

				return( -1 );
			}
		}
		write_count = libewf_handle_write_finalize(
		               imaging_handle->output_handle,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to finalize output handle.",
			 function );

			return( -1 );
		}
		if( imaging_handle->secondary_output_handle != NULL )
		{
			secondary_write_count = libewf_handle_write_finalize(
			                         imaging_handle->secondary_output_handle,
			                         error );

			if( secondary_write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to finalize secondary output handle.",
				 function );

				return( -1 );
			}
		}
		imaging_handle->last_offset_written += write_count;
	}
	if( imaging_handle->abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     imaging_handle->process_status,
	     imaging_handle->last_offset_written,
	     status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to stop process status",
		 function );

		return( -1 );
	}
	if( process_status_free(
	     &( imaging_handle->process_status ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free process status",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Prints an overview of the parameters
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_print_parameters(
     imaging_handle_t *imaging_handle,
     off64_t resume_acquiry_offset,
     uint8_t read_error_retries,
     uint8_t zero_block_on_read_error,
     uint8_t resume_acquiry,
     libcerror_error_t **error )
{
	system_character_t acquiry_size_string[ 16 ];
	system_character_t maximum_segment_size_string[ 16 ];

	static char *function = "imaging_handle_print_parameters";
	int result            = 0;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( imaging_handle->notify_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing notify stream.",
		 function );

		return( -1 );
	}
	if( imaging_handle->target_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid imaging handle - missing target filename.",
		 function );

		return( -1 );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "Image path and filename:\t\t%" PRIs_SYSTEM "",
	 imaging_handle->target_filename );

	if( resume_acquiry == 0 )
	{
		if( imaging_handle->ewf_format == LIBEWF_FORMAT_SMART )
		{
			fprintf(
			 imaging_handle->notify_stream,
			 ".s01" );
		}
		else if( ( imaging_handle->ewf_format == LIBEWF_FORMAT_EWF )
		      || ( imaging_handle->ewf_format == LIBEWF_FORMAT_EWFX ) )
		{
			fprintf(
			 imaging_handle->notify_stream,
			 ".e01" );
		}
		else if( imaging_handle->ewf_format == LIBEWF_FORMAT_V2_ENCASE7 )
		{
			fprintf(
			 imaging_handle->notify_stream,
			 ".Ex01" );
		}
		else if( ( imaging_handle->ewf_format == LIBEWF_FORMAT_LOGICAL_ENCASE5 )
		      || ( imaging_handle->ewf_format == LIBEWF_FORMAT_LOGICAL_ENCASE6 )
		      || ( imaging_handle->ewf_format == LIBEWF_FORMAT_LOGICAL_ENCASE7 ) )
		{
			fprintf(
			 imaging_handle->notify_stream,
			 ".L01" );
		}
		else if( imaging_handle->ewf_format == LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 )
		{
			fprintf(
			 imaging_handle->notify_stream,
			 ".Lx01" );
		}
		else
		{
			fprintf(
			 imaging_handle->notify_stream,
			 ".E01" );
		}
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	if( imaging_handle->secondary_target_filename != NULL )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "Secondary copy:\t\t\t\t%" PRIs_SYSTEM "",
		 imaging_handle->secondary_target_filename );

		if( resume_acquiry == 0 )
		{
			if( imaging_handle->ewf_format == LIBEWF_FORMAT_SMART )
			{
				fprintf(
				 imaging_handle->notify_stream,
				 ".s01" );
			}
			else if( ( imaging_handle->ewf_format == LIBEWF_FORMAT_EWF )
			      || ( imaging_handle->ewf_format == LIBEWF_FORMAT_EWFX ) )
			{
				fprintf(
				 imaging_handle->notify_stream,
				 ".e01" );
			}
			else if( imaging_handle->ewf_format == LIBEWF_FORMAT_V2_ENCASE7 )
			{
				fprintf(
				 imaging_handle->notify_stream,
				 ".Ex01" );
			}
			else if( ( imaging_handle->ewf_format == LIBEWF_FORMAT_LOGICAL_ENCASE5 )
			      || ( imaging_handle->ewf_format == LIBEWF_FORMAT_LOGICAL_ENCASE6 )
			      || ( imaging_handle->ewf_format == LIBEWF_FORMAT_LOGICAL_ENCASE7 ) )
			{
				fprintf(
				 imaging_handle->notify_stream,
				 ".L01" );
			}
			else if( imaging_handle->ewf_format == LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 )
			{
				fprintf(
				 imaging_handle->notify_stream,
				 ".Lx01" );
			}
			else
			{
				fprintf(
				 imaging_handle->notify_stream,
				 ".E01" );
			}
		}
		fprintf(
		 imaging_handle->notify_stream,
		 "\n" );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "Case number:\t\t\t\t" );

	if( imaging_handle->case_number != NULL )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 imaging_handle->case_number );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Description:\t\t\t\t" );

	if( imaging_handle->description != NULL )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 imaging_handle->description );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Evidence number:\t\t\t" );

	if( imaging_handle->evidence_number != NULL )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 imaging_handle->evidence_number );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Examiner name:\t\t\t\t" );

	if( imaging_handle->examiner_name != NULL )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 imaging_handle->examiner_name );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Notes:\t\t\t\t\t" );

	if( imaging_handle->notes != NULL )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 imaging_handle->notes );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Media type:\t\t\t\t" );

	if( imaging_handle->media_type == LIBEWF_MEDIA_TYPE_FIXED )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "fixed disk" );
	}
	else if( imaging_handle->media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "removable disk" );
	}
	else if( imaging_handle->media_type == LIBEWF_MEDIA_TYPE_OPTICAL )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "optical disk (CD/DVD/BD)" );
	}
	else if( imaging_handle->media_type == LIBEWF_MEDIA_TYPE_MEMORY )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "memory (RAM)" );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Is physical:\t\t\t\t" );

	if( ( imaging_handle->media_flags & LIBEWF_MEDIA_FLAG_PHYSICAL ) != 0 )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "yes" );
	}
	else
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "no" );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "EWF file format:\t\t\t" );

	switch( imaging_handle->ewf_format )
	{
		case LIBEWF_FORMAT_EWF:
			fprintf(
			 imaging_handle->notify_stream,
			 "original EWF (.e01)" );
			break;

		case LIBEWF_FORMAT_ENCASE1:
			fprintf(
			 imaging_handle->notify_stream,
			 "EnCase 1 (.E01)" );
			break;

		case LIBEWF_FORMAT_ENCASE2:
			fprintf(
			 imaging_handle->notify_stream,
			 "EnCase 2 (.E01)" );
			break;

		case LIBEWF_FORMAT_ENCASE3:
			fprintf(
			 imaging_handle->notify_stream,
			 "EnCase 3 (.E01)" );
			break;

		case LIBEWF_FORMAT_ENCASE4:
			fprintf(
			 imaging_handle->notify_stream,
			 "EnCase 4 (.E01)" );
			break;

		case LIBEWF_FORMAT_ENCASE5:
			fprintf(
			 imaging_handle->notify_stream,
			 "EnCase 5 (.E01)" );
			break;

		case LIBEWF_FORMAT_ENCASE6:
			fprintf(
			 imaging_handle->notify_stream,
			 "EnCase 6 (.E01)" );
			break;

		case LIBEWF_FORMAT_ENCASE7:
			fprintf(
			 imaging_handle->notify_stream,
			 "EnCase 7 (.E01)" );
			break;

		case LIBEWF_FORMAT_SMART:
			fprintf(
			 imaging_handle->notify_stream,
			 "SMART (.s01)" );
			break;

		case LIBEWF_FORMAT_FTK_IMAGER:
			fprintf(
			 imaging_handle->notify_stream,
			 "FTK Imager (.E01)" );
			break;

		case LIBEWF_FORMAT_LINEN5:
			fprintf(
			 imaging_handle->notify_stream,
			 "linen 5 (.E01)" );
			break;

		case LIBEWF_FORMAT_LINEN6:
			fprintf(
			 imaging_handle->notify_stream,
			 "linen 6 (.E01)" );
			break;

		case LIBEWF_FORMAT_LINEN7:
			fprintf(
			 imaging_handle->notify_stream,
			 "linen 7 (.E01)" );
			break;

		case LIBEWF_FORMAT_V2_ENCASE7:
			fprintf(
			 imaging_handle->notify_stream,
			 "EnCase 7 (.Ex01)" );
			break;

		case LIBEWF_FORMAT_EWFX:
			fprintf(
			 imaging_handle->notify_stream,
			 "extended EWF (ewfx) (.e01)" );
			break;
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Compression method:\t\t\t" );

	if( imaging_handle->compression_method == LIBEWF_COMPRESSION_METHOD_DEFLATE )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "deflate" );
	}
	else if( imaging_handle->compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "bzip2" );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Compression level:\t\t\t" );

	if( imaging_handle->compression_level == LIBEWF_COMPRESSION_LEVEL_FAST )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "fast" );
	}
	else if( imaging_handle->compression_level == LIBEWF_COMPRESSION_LEVEL_BEST )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "best" );
	}
	else if( imaging_handle->compression_level == LIBEWF_COMPRESSION_LEVEL_NONE )
	{
		if( ( imaging_handle->compression_flags & LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
		{
			fprintf(
			 imaging_handle->notify_stream,
			 "empty-block" );
		}
		else
		{
			fprintf(
			 imaging_handle->notify_stream,
			 "none" );
		}
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Acquiry start offset:\t\t\t%" PRIi64 "\n",
	 imaging_handle->acquiry_offset );

	if( resume_acquiry != 0 )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "Resuming acquiry at offset:\t\t%" PRIi64 "\n",
		 resume_acquiry_offset );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "Number of bytes to acquire:\t\t" );

	if( imaging_handle->acquiry_size == 0 )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "%" PRIu64 " (until end of input)",
		 imaging_handle->acquiry_size );
	}
	else
	{
		result = byte_size_string_create(
		          acquiry_size_string,
		          16,
		          imaging_handle->acquiry_size,
		          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );

		if( result == 1 )
		{
			fprintf(
			 imaging_handle->notify_stream,
			 "%" PRIs_SYSTEM " (%" PRIu64 " bytes)",
			 acquiry_size_string,
			 imaging_handle->acquiry_size );
		}
		else
		{
			fprintf(
			 imaging_handle->notify_stream,
			 "%" PRIu64 " bytes",
			 imaging_handle->acquiry_size );
		}
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	result = byte_size_string_create(
	          maximum_segment_size_string,
	          16,
	          imaging_handle->maximum_segment_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	fprintf(
	 imaging_handle->notify_stream,
	 "Evidence segment file size:\t\t" );

	if( result == 1 )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "%" PRIs_SYSTEM " (%" PRIu64 " bytes)",
		 maximum_segment_size_string,
		 imaging_handle->maximum_segment_size );
	}
	else
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "%" PRIu64 " bytes",
		 imaging_handle->maximum_segment_size );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "Bytes per sector:\t\t\t%" PRIu32 "\n",
	 imaging_handle->bytes_per_sector );

	fprintf(
	 imaging_handle->notify_stream,
	 "Block size:\t\t\t\t%" PRIu32 " sectors\n",
	 imaging_handle->sectors_per_chunk );

	fprintf(
	 imaging_handle->notify_stream,
	 "Error granularity:\t\t\t%" PRIu32 " sectors\n",
	 imaging_handle->sector_error_granularity );

	fprintf(
	 imaging_handle->notify_stream,
	 "Retries on read error:\t\t\t%" PRIu8 "\n",
	 read_error_retries );

	fprintf(
	 imaging_handle->notify_stream,
	 "Zero sectors on read error:\t\t" );

	if( zero_block_on_read_error == 0 )
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "no" );
	}
	else
	{
		fprintf(
		 imaging_handle->notify_stream,
		 "yes" );
	}
	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	fprintf(
	 imaging_handle->notify_stream,
	 "\n" );

	return( 1 );
}

/* Prints the hashes
 * Returns 1 if successful or -1 on error
 */
int imaging_handle_print_hashes(
     imaging_handle_t *imaging_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	static char *function = "imaging_handle_print_hashes";

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging stream.",
		 function );

		return( -1 );
	}
	if( imaging_handle->calculate_md5 != 0 )
	{
		fprintf(
		 stream,
		 "MD5 hash calculated over data:\t\t%" PRIs_SYSTEM "\n",
		 imaging_handle->calculated_md5_hash_string );
	}
	if( imaging_handle->calculate_sha1 != 0 )
	{
		fprintf(
		 stream,
		 "SHA1 hash calculated over data:\t\t%" PRIs_SYSTEM "\n",
		 imaging_handle->calculated_sha1_hash_string );
	}
	if( imaging_handle->calculate_sha256 != 0 )
	{
		fprintf(
		 stream,
		 "SHA256 hash calculated over data:\t%" PRIs_SYSTEM "\n",
		 imaging_handle->calculated_sha256_hash_string );
	}
	return( 1 );
}

