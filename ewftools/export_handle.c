/* 
 * Export handle
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
#include "export_handle.h"
#include "guid.h"
#include "md5.h"
#include "sha1.h"

#if !defined( USE_LIBEWF_GET_HASH_VALUE_MD5 ) && !defined( USE_LIBEWF_GET_MD5_HASH )
#define USE_LIBEWF_GET_HASH_VALUE_MD5
#endif

/* Initializes the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_initialize(
     export_handle_t **export_handle,
     uint8_t calculate_md5,
     uint8_t calculate_sha1,
     liberror_error_t **error )
{
	static char *function = "export_handle_initialize";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle == NULL )
	{
		*export_handle = (export_handle_t *) memory_allocate(
								  sizeof( export_handle_t ) );

		if( *export_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create export handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *export_handle,
		     0,
		     sizeof( export_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear export handle.",
			 function );

			memory_free(
			 *export_handle );

			*export_handle = NULL;

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_initialize(
		     &( ( *export_handle )->input_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input handle.",
			 function );

			memory_free(
			 *export_handle );

			*export_handle = NULL;

			return( -1 );
		}
#endif
		( *export_handle )->calculate_md5              = calculate_md5;
		( *export_handle )->calculate_sha1             = calculate_sha1;
		( *export_handle )->raw_output_file_descriptor = -1;

		if( ( ( *export_handle )->calculate_md5 != 0 )
		 && ( md5_initialize(
		       &( ( *export_handle )->md5_context ),
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
			 &( ( *export_handle )->ewf_output_handle ),
			 NULL );
#endif
			memory_free(
			 *export_handle );

			*export_handle = NULL;

			return( -1 );
		}
		if( ( ( *export_handle )->calculate_sha1 != 0 )
		 && ( sha1_initialize(
		       &( ( *export_handle )->sha1_context ),
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
			 &( ( *export_handle )->ewf_output_handle ),
			 NULL );
#endif
			memory_free(
			 *export_handle );

			*export_handle = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the export handle and its elements
 * Returns 1 if successful or -1 on error
 */
int export_handle_free(
     export_handle_t **export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_free";
	int result            = 1;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( ( ( *export_handle )->input_handle != NULL )
		 && ( libewf_handle_free(
		       &( ( *export_handle )->input_handle ),
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
		if( ( ( *export_handle )->ewf_output_handle != NULL )
		 && ( libewf_handle_free(
		       &( ( *export_handle )->ewf_output_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free ewf output handle.",
			 function );

			result = -1;
		}
#endif
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( result );
}

/* Signals the export handle to abort
 * Returns 1 if successful or -1 on error
 */
int export_handle_signal_abort(
     export_handle_t *export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_signal_abort";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_signal_abort(
		     export_handle->input_handle,
		     error ) != 1 )
#else
		if( libewf_signal_abort(
		     export_handle->input_handle ) != 1 )
#endif
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
	if( export_handle->ewf_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_signal_abort(
		     export_handle->ewf_output_handle,
		     error ) != 1 )
#else
		if( libewf_signal_abort(
		     export_handle->ewf_output_handle ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal ewf output handle to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Opens the input of the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_input(
     export_handle_t *export_handle,
     libsystem_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error )
{
	libsystem_character_t **libewf_filenames = NULL;
	static char *function                    = "export_handle_open_input";
	size_t first_filename_length             = 0;
	int result                               = 1;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
		 function );

		return( -1 );
	}
#else
	if( export_handle->input_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid export handle - input handle already set.",
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
		                         filenames[ 0 ] );

#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
#if defined( HAVE_V2_API )
		if( libewf_glob_wide(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &amount_of_filenames,
		     error ) != 1 )
#else
		amount_of_filenames = libewf_glob_wide(
		                       filenames[ 0 ],
		                       first_filename_length,
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &libewf_filenames );

		if( amount_of_filenames <= 0 )
#endif
#else
#if defined( HAVE_V2_API )
		if( libewf_glob(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &amount_of_filenames,
		     error ) != 1 )
#else
		amount_of_filenames = libewf_glob(
		                       filenames[ 0 ],
		                       first_filename_length,
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &libewf_filenames );

		if( amount_of_filenames <= 0 )
#endif
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
#if defined( HAVE_V2_API )
	if( libewf_handle_open_wide(
	     export_handle->input_handle,
	     filenames,
	     amount_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	export_handle->input_handle = libewf_open_wide(
	                               filenames,
	                               amount_of_filenames,
	                               LIBEWF_OPEN_READ );

	if( export_handle->input_handle == NULL )
#endif
#else
#if defined( HAVE_V2_API )
	if( libewf_handle_open(
	     export_handle->input_handle,
	     filenames,
	     amount_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	export_handle->input_handle = libewf_open(
	                               filenames,
	                               amount_of_filenames,
	                               LIBEWF_OPEN_READ );

	if( export_handle->input_handle == NULL )
#endif
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file(s).",
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
#if defined( HAVE_V2_API )
	if( libewf_handle_get_chunk_size(
	     export_handle->input_handle,
	     &( export_handle->input_chunk_size ),
	     error ) != 1 )
#else
	if( libewf_get_chunk_size(
	     export_handle->input_handle,
	     &( export_handle->input_chunk_size ) ) != 1 )
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
	return( result );
}

/* Opens the output of the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_output(
     export_handle_t *export_handle,
     uint8_t output_format,
     const libsystem_character_t *filename,
     liberror_error_t **error )
{
	libsystem_character_t *filenames[ 1 ] = { NULL };
	static char *function                 = "export_handle_open_output";
	size_t filename_length                = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( ( output_format != EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	 && ( output_format != EXPORT_HANDLE_OUTPUT_FORMAT_RAW ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported output format.",
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
	export_handle->output_format = output_format;

	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid export handle - ewf output handle already set.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_initialize(
		     &( export_handle->ewf_output_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize ewf output handle.",
			 function );

			return( -1 );
		}
#endif
		filenames[ 0 ] = (libsystem_character_t *) filename;

#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
#if defined( HAVE_V2_API )
		if( libewf_handle_open_wide(
		     export_handle->ewf_output_handle,
		     filenames,
		     1,
		     LIBEWF_OPEN_WRITE,
		     error ) != 1 )
#else
		export_handle->ewf_output_handle = libewf_open_wide(
		                                     filenames,
		                                     1,
		                                     LIBEWF_OPEN_WRITE );

		if( export_handle->ewf_output_handle == NULL )
#endif
#else
#if defined( HAVE_V2_API )
		if( libewf_handle_open(
		     export_handle->ewf_output_handle,
		     filenames,
		     1,
		     LIBEWF_OPEN_WRITE,
		     error ) != 1 )
#else
		export_handle->ewf_output_handle = libewf_open(
		                                     filenames,
		                                     1,
		                                     LIBEWF_OPEN_WRITE );

		if( export_handle->ewf_output_handle == NULL )
#endif
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file: %" PRIs_LIBSYSTEM ".",
			 function,
			 filename );

			return( 1 );
		}
	}
	else if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	{
		filename_length = libsystem_string_length(
		                   filename );

		if( ( filename_length == 1 )
		 && ( libsystem_string_compare(
		       filename,
		       _LIBSYSTEM_CHARACTER_T_STRING( "-" ),
		       1 ) == 0 ) )
		{
			export_handle->raw_output_file_descriptor = 1;
		}
		else
		{
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
			export_handle->raw_output_file_descriptor = libsystem_file_io_wopen(
			                                             filename,
			                                             LIBSYSTEM_FILE_IO_O_CREAT | LIBSYSTEM_FILE_IO_O_WRONLY | LIBSYSTEM_FILE_IO_O_TRUNC );
#else
			export_handle->raw_output_file_descriptor = libsystem_file_io_open(
			                                             filename,
			                                             LIBSYSTEM_FILE_IO_O_CREAT | LIBSYSTEM_FILE_IO_O_WRONLY | LIBSYSTEM_FILE_IO_O_TRUNC );
#endif

			if( export_handle->raw_output_file_descriptor == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open file: %" PRIs_LIBSYSTEM ".",
				 function,
				 filename );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Closes the export handle
 * Returns the 0 if succesful or -1 on error
 */
int export_handle_close(
     export_handle_t *export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_close";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_close(
	     export_handle->input_handle,
	     error ) != 0 )
#else
	if( libewf_close(
	     export_handle->input_handle ) != 0 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
#if !defined( HAVE_V2_API )
	export_handle->input_handle = NULL;
#endif

	if( export_handle->ewf_output_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_close(
		     export_handle->ewf_output_handle,
		     error ) != 0 )
#else
		if( libewf_close(
		     export_handle->ewf_output_handle ) != 0 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close ewf output handle.",
			 function );

			return( -1 );
		}
#if !defined( HAVE_V2_API )
		export_handle->ewf_output_handle = NULL;
#endif
	}

	if( export_handle->raw_output_file_descriptor != -1 )
	{
		if( libsystem_file_io_close(
		     export_handle->raw_output_file_descriptor ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close raw output file descriptor.",
			 function );

			return( 1 );
		}
		export_handle->raw_output_file_descriptor = -1;
	}
	return( 0 );
}

/* Prepares a buffer after reading the input of the export handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t export_handle_prepare_read_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "export_handle_prepare_read_buffer";
	ssize_t process_count = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
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
	                 export_handle->input_handle,
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
	                 export_handle->input_handle,
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
		liberror_error_free(
		 error );

		/* Wipe the chunk if nescessary
		 */
		if( export_handle->wipe_chunk_on_error != 0 )
		{
			if( ( storage_media_buffer->is_compressed != 0 )
			 && ( memory_set(
			       storage_media_buffer->compression_buffer,
			       0,
			       storage_media_buffer->compression_buffer_size ) == NULL ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to wipe compression buffer.",
				 function );

				return( -1 );
			}
			if( memory_set(
			     storage_media_buffer->raw_buffer,
			     0,
			     storage_media_buffer->raw_buffer_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to wipe raw buffer.",
				 function );

				return( -1 );
			}
		}
		process_count = export_handle->input_chunk_size;

		/* Add a read error
		 */
		if( export_handle_add_read_error(
		     export_handle,
		     export_handle->input_offset,
		     process_count,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to add read error.",
			 function );

			return( -1 );
		}
	}
	if( storage_media_buffer->is_compressed == 0 )
	{
		storage_media_buffer->data_in_compression_buffer = 1;
	}
	else
	{
		storage_media_buffer->data_in_compression_buffer = 0;
	}
	export_handle->input_offset += process_count;
#else
	process_count = storage_media_buffer->raw_buffer_amount;
#endif
	return( process_count );
}

/* Reads a buffer from the input of the export handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t export_handle_read_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error )
{
	static char *function = "export_handle_read_buffer";
	ssize_t read_count    = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
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
                      export_handle->input_handle,
                      storage_media_buffer->compression_buffer,
                      storage_media_buffer->compression_buffer_size,
	              &( storage_media_buffer->is_compressed ),
	              &( storage_media_buffer->compression_buffer[ storage_media_buffer->raw_buffer_size ] ),
	              &( storage_media_buffer->crc ),
	              &( storage_media_buffer->process_crc ),
	              error );
#else
	read_count = libewf_raw_read_buffer(
	              export_handle->input_handle,
	              storage_media_buffer->compression_buffer,
	              storage_media_buffer->compression_buffer_size,
	              &( storage_media_buffer->is_compressed ),
	              &( storage_media_buffer->crc ),
	              &( storage_media_buffer->process_crc ) );
#endif
#else
#if defined( HAVE_V2_API )
	read_count = libewf_handle_read_buffer(
                      export_handle->input_handle,
                      storage_media_buffer->raw_buffer,
                      read_size,
	              error );
#else
	read_count = libewf_read_buffer(
                      export_handle->input_handle,
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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	storage_media_buffer->compression_buffer_amount = read_count;
#else
	storage_media_buffer->raw_buffer_amount         = read_count;
#endif

	return( read_count );
}

/* Prepares a buffer before writing the output of the export handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t export_handle_prepare_write_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error )
{
	static char *function = "export_handle_prepare_write_buffer";
	ssize_t process_count = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
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
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		storage_media_buffer->compression_buffer_amount = storage_media_buffer->compression_buffer_size;

#if defined( HAVE_V2_API )
		process_count = libewf_handle_prepare_write_chunk(
				 export_handle->ewf_output_handle,
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
				 export_handle->ewf_output_handle,
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
	}
	else if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	{
		process_count = storage_media_buffer->raw_buffer_amount;
	}
	return( process_count );
}

/* Writes a buffer to the output of the export handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t export_handle_write_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t write_size,
         liberror_error_t **error )
{
	static char *function        = "export_handle_write_buffer";
	ssize_t write_count          = 0;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	uint8_t *raw_write_buffer    = NULL;
	size_t raw_write_buffer_size = 0;
#endif

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
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
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
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
			       export_handle->ewf_output_handle,
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
			       export_handle->ewf_output_handle,
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
			       export_handle->ewf_output_handle,
			       storage_media_buffer->raw_buffer,
			       write_size,
			       error );
#else
		write_count = libewf_write_buffer(
			       export_handle->ewf_output_handle,
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
	}
	else if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	{
		if( export_handle->raw_output_file_descriptor == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - invalid raw output file descriptor.",
			 function );

			return( -1 );
		}
		write_count = libsystem_file_io_write(
		               export_handle->raw_output_file_descriptor,
			       storage_media_buffer->raw_buffer,
			       write_size );

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
	}
	return( write_count );
}

/* Seeks the offset
 * Returns the resulting offset or -1 on error
 */
off64_t export_handle_seek_offset(
         export_handle_t *export_handle,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "export_handle_seek_offset";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_seek_offset(
	     export_handle->input_handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
#else
	if( libewf_seek_offset(
	     export_handle->input_handle,
	     offset ) == -1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Swaps the byte order of byte pairs within a buffer of a certain size
 * Returns 1 if successful, -1 on error
 */
int export_handle_swap_byte_pairs(
     export_handle_t *export_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error )
{
	uint8_t *data         = NULL;
	static char *function = "export_handle_swap_byte_pairs";
	size_t data_size      = 0;
	size_t iterator       = 0;
	uint8_t byte          = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
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
int export_handle_update_integrity_hash(
     export_handle_t *export_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error )
{
	uint8_t *data         = NULL;
	static char *function = "export_handle_update_integrity_hash";
	size_t data_size      = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
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
	if( export_handle->calculate_md5 != 0 )
	{
		/* TODO check for return value */
		md5_update(
		 &( export_handle->md5_context ),
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
	if( export_handle->calculate_sha1 != 0 )
	{
		/* TODO check for return value */
		sha1_update(
		 &( export_handle->sha1_context ),
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

/* Retrieves the input media size
 * Returns the 1 if succesful or -1 on error
 */
int export_handle_get_input_media_size(
     export_handle_t *export_handle,
     size64_t *media_size,
     liberror_error_t **error )
{
	static char *function = "export_handle_get_input_media_size";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_media_size(
	     export_handle->input_handle,
	     media_size,
	     error ) != 1 )
#else
	if( libewf_get_media_size(
	     export_handle->input_handle,
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
	return( 1 );
}

/* Retrieves the input chunk size
 * Returns 1 if successful or -1 on error
 */
int export_handle_get_input_chunk_size(
     export_handle_t *export_handle,
     size32_t *chunk_size,
     liberror_error_t **error )
{
	static char *function = "export_handle_get_input_chunk_size";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
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
	*chunk_size = export_handle->input_chunk_size;

	return( 1 );
}

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 */
int export_handle_get_output_chunk_size(
     export_handle_t *export_handle,
     size32_t *chunk_size,
     liberror_error_t **error )
{
	static char *function = "export_handle_get_output_chunk_size";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
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
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_get_chunk_size(
		     export_handle->ewf_output_handle,
		     chunk_size,
		     error ) != 1 )
#else
		if( libewf_get_chunk_size(
		     export_handle->ewf_output_handle,
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
	}
	else if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	{
		*chunk_size = export_handle->input_chunk_size;
	}
	return( 1 );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_header_codepage(
     export_handle_t *export_handle,
     int header_codepage,
     liberror_error_t **error )
{
	static char *function = "export_handle_set_header_codepage";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_header_codepage(
	     export_handle->input_handle,
	     header_codepage,
	     error ) != 1 )
#else
	if( libewf_set_header_codepage(
	     export_handle->input_handle,
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
	return( 1 );
}

/* Sets the output values of the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_output_values(
     export_handle_t *export_handle,
     libsystem_character_t *acquiry_operating_system,
     libsystem_character_t *acquiry_software,
     libsystem_character_t *acquiry_software_version,
     int header_codepage,
     size64_t media_size,
     int8_t compression_level,
     uint8_t compression_flags,
     uint8_t libewf_format,
     size64_t segment_file_size,
     uint32_t sectors_per_chunk,
     uint8_t wipe_chunk_on_error,
     liberror_error_t **error )
{
#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )
	uint8_t guid[ GUID_SIZE ];

	uint8_t guid_type     = 0;
#endif

	static char *function = "export_handle_set_output_values";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_bytes_per_sector(
	     export_handle->input_handle,
	     &( export_handle->bytes_per_sector ),
	     error ) != 1 )
#else
	if( libewf_get_bytes_per_sector(
	     export_handle->input_handle,
	     &( export_handle->bytes_per_sector ) ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector from input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_read_wipe_chunk_on_error(
	     export_handle->input_handle,
	     wipe_chunk_on_error,
	     error ) != 1 )
#else
	if( libewf_set_read_wipe_chunk_on_error(
	     export_handle->input_handle,
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
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
		}
#if !defined( HAVE_V2_API )
		if( libewf_parse_header_values(
		     export_handle->input_handle,
		     LIBEWF_DATE_FORMAT_ISO8601 ) != 1 )
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
#if defined( HAVE_V2_API )
		if( libewf_handle_copy_header_values(
		     export_handle->ewf_output_handle,
		     export_handle->input_handle,
		     error ) != 1 )
#else
		if( libewf_copy_header_values(
		     export_handle->ewf_output_handle,
		     export_handle->input_handle ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header values.",
			 function );

			return( -1 );
		}
		/* Set acquiry operating system, software and software version
		 */
		if( ( acquiry_operating_system != NULL )
		 && ( export_handle_set_header_value(
		       export_handle,
		       "acquiry_operating_system",
		       24,
		       acquiry_operating_system,
		       libsystem_string_length(
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
		if( export_handle_set_header_value(
		     export_handle,
		     "acquiry_software",
		     16,
		     acquiry_software,
		     libsystem_string_length(
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
		if( export_handle_set_header_value(
		     export_handle,
		     "acquiry_software_version",
		     24,
		     acquiry_software_version,
		     libsystem_string_length(
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
		if( libewf_handle_set_header_codepage(
		     export_handle->ewf_output_handle,
		     header_codepage,
		     error ) != 1 )
#else
		if( libewf_set_header_codepage(
		     export_handle->ewf_output_handle,
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
		if( libewf_handle_copy_media_values(
		     export_handle->ewf_output_handle,
		     export_handle->input_handle,
		     error ) != 1 )
#else
		if( libewf_copy_media_values(
		     export_handle->ewf_output_handle,
		     export_handle->input_handle ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy media values.",
			 function );

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_set_media_size(
		     export_handle->ewf_output_handle,
		     media_size,
		     error ) != 1 )
#else
		if( libewf_set_media_size(
		     export_handle->ewf_output_handle,
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
		if( libewf_handle_set_compression_values(
		     export_handle->ewf_output_handle,
		     compression_level,
		     compression_flags,
		     error ) != 1 )
#else
		if( libewf_set_compression_values(
		     export_handle->ewf_output_handle,
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
		if( ( compression_level != LIBEWF_COMPRESSION_NONE )
		 || ( ( compression_flags & LIBEWF_FLAG_COMPRESS_EMPTY_BLOCK ) == LIBEWF_FLAG_COMPRESS_EMPTY_BLOCK ) )
		{
			export_handle->write_compressed = 1;
		}
		else
		{
			export_handle->write_compressed = 0;
		}

		/* Format needs to be set before segment file size
		 */
#if defined( HAVE_V2_API )
		if( libewf_handle_set_format(
		     export_handle->ewf_output_handle,
		     libewf_format,
		     error ) != 1 )
#else
		if( libewf_set_format(
		     export_handle->ewf_output_handle,
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
		     export_handle->ewf_output_handle,
		     segment_file_size,
		     error ) != 1 )
#else
		if( libewf_set_segment_file_size(
		     export_handle->ewf_output_handle,
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
		     export_handle->ewf_output_handle,
		     sectors_per_chunk,
		     error ) != 1 )
#else
		if( libewf_set_sectors_per_chunk(
		     export_handle->ewf_output_handle,
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
			     export_handle->ewf_output_handle,
			     guid,
			     16,
			     error ) != 1 )
#else
			if( libewf_set_guid(
			     export_handle->ewf_output_handle,
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
		}
#endif
	}
	return( 1 );
}

/* Sets the header value in the output handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_header_value(
     export_handle_t *export_handle,
     char *header_value_identifier,
     size_t header_value_identifier_length,
     libsystem_character_t *header_value,
     size_t header_value_length,
     liberror_error_t **error )
{
	uint8_t *utf8_header_value    = NULL;
	static char *function         = "export_handle_set_header_value";
	size_t utf8_header_value_size = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->ewf_output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing ewf output handle.",
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
	     export_handle->ewf_output_handle,
	     (uint8_t *) header_value_identifier,
	     header_value_identifier_length,
	     utf8_header_value,
	     utf8_header_value_size - 1,
	     error ) != 1 )
#else
	if( libewf_set_header_value(
	     export_handle->ewf_output_handle,
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
int export_handle_set_hash_value(
     export_handle_t *export_handle,
     char *hash_value_identifier,
     size_t hash_value_identifier_length,
     libsystem_character_t *hash_value,
     size_t hash_value_length,
     liberror_error_t **error )
{
	uint8_t *utf8_hash_value    = NULL;
	static char *function       = "export_handle_set_hash_value";
	size_t utf8_hash_value_size = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->ewf_output_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing ewf output handle.",
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
	     export_handle->ewf_output_handle,
	     (uint8_t *) hash_value_identifier,
	     hash_value_identifier_length,
	     utf8_hash_value,
	     utf8_hash_value_size - 1,
	     error ) != 1 )
#else
	if( libewf_set_hash_value(
	     export_handle->ewf_output_handle,
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
int export_handle_add_read_error(
      export_handle_t *export_handle,
      off64_t start_offset,
      size_t amount_of_bytes,
      liberror_error_t **error )
{
	static char *function      = "export_handle_add_read_error";
	uint64_t start_sector      = 0;
	uint64_t amount_of_sectors = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
		 function );

		return( -1 );
	}
#endif
	if( export_handle->bytes_per_sector == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid export handle - invalid bytes per sector value out of range.",
		 function );

		return( -1 );
	}
	start_sector      = start_offset / export_handle->bytes_per_sector;
	amount_of_sectors = amount_of_bytes / export_handle->bytes_per_sector;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
#if defined( HAVE_V2_API )
	if( libewf_handle_add_crc_error(
	     export_handle->input_handle,
	     start_sector,
	     amount_of_sectors,
	     error ) != 1 )
#else
	if( libewf_add_crc_error(
	     export_handle->input_handle,
	     (off64_t) start_sector,
	     (uint32_t) amount_of_sectors ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to add CRC errror.",
		 function );

		return( -1 );
	}
#endif
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
		}

#if defined( HAVE_V2_API )
		if( libewf_handle_add_acquiry_error(
		     export_handle->ewf_output_handle,
		     start_sector,
		     amount_of_sectors,
		     error ) != 1 )
#else
		if( libewf_add_acquiry_error(
		     export_handle->ewf_output_handle,
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
	}
	return( 1 );
}
 
/* Finalizes the export handle
 * Returns the amount of input bytes written or -1 on error
 */
ssize_t export_handle_finalize(
         export_handle_t *export_handle,
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

	static char *function            = "export_handle_finalize";
	size_t calculated_md5_hash_size  = DIGEST_HASH_SIZE_MD5;
	size_t calculated_sha1_hash_size = DIGEST_HASH_SIZE_SHA1;
	ssize_t write_count              = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( ( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	 && ( export_handle->ewf_output_handle == NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing ewf output handle.",
		 function );

		return( -1 );
	}
	if( export_handle->calculate_md5 != 0 )
	{
		/* Finalize the MD5 hash calculation
		 */
		if( md5_finalize(
		     &( export_handle->md5_context ),
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
		if( ( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
		 && ( export_handle_set_hash_value(
		       export_handle,
		       "MD5",
		       3,
		       calculated_md5_hash_string,
		       calculated_md5_hash_string_size - 1,
		       error ) != 1 ) )
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
	if( export_handle->calculate_sha1 != 0 )
	{
		/* Finalize the SHA1 hash calculation
		 */
		if( sha1_finalize(
		     &( export_handle->sha1_context ),
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
		if( ( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
		 && ( export_handle_set_hash_value(
		       export_handle,
		       "SHA1",
		       4,
		       calculated_sha1_hash_string,
		       calculated_sha1_hash_string_size - 1,
		       error ) != 1 ) )
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
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
#if defined( HAVE_V2_API )
		write_count = libewf_handle_write_finalize(
		               export_handle->ewf_output_handle,
	        	       error );
#else
		write_count = libewf_write_finalize(
		               export_handle->ewf_output_handle );
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
	}
	return( write_count );
}

/* Print the CRC errors to a stream
 * Returns 1 if successful or -1 on error
 */
int export_handle_crc_errors_fprint(
     export_handle_t *export_handle,
     FILE *stream,
     liberror_error_t **error )
{
	static char *function                = "export_handle_crc_errors_fprint";
	uint64_t start_sector                = 0;
	uint64_t last_sector                 = 0;
#if defined( HAVE_V2_API )
	uint64_t amount_of_sectors           = 0;
#else
	uint32_t amount_of_sectors           = 0;
#endif
	uint32_t amount_of_errors            = 0;
	uint32_t error_iterator              = 0;
	int result                           = 1;

#if defined( HAVE_V2_API )
	libsystem_character_t *filename      = NULL;
	libsystem_character_t *last_filename = NULL;
	size_t filename_size                 = 0;
	size_t last_filename_size            = 0;
#endif

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
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
	if( libewf_handle_get_amount_of_crc_errors(
	     export_handle->input_handle,
	     &amount_of_errors,
	     error ) == -1 )
#else
	if( libewf_get_amount_of_crc_errors(
	     export_handle->input_handle,
	     &amount_of_errors ) == -1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of crc errors.",
		 function );

		return( -1 );
	}
	if( amount_of_errors > 0 )
	{
		fprintf(
		 stream,
		 "Read errors during export:\n" );
		fprintf(
		 stream,
		 "\ttotal amount: %" PRIu32 "\n",
		 amount_of_errors );
		
		for( error_iterator = 0; error_iterator < amount_of_errors; error_iterator++ )
		{
#if defined( HAVE_V2_API )
			if( libewf_handle_get_crc_error(
			     export_handle->input_handle,
			     error_iterator,
			     &start_sector,
			     &amount_of_sectors,
			     error ) != 1 )
#else
			if( libewf_get_crc_error(
			     export_handle->input_handle,
			     error_iterator,
			     (off64_t *) &start_sector,
			     &amount_of_sectors ) != 1 )
#endif
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the crc error: %" PRIu32 ".",
				 function,
				 error_iterator );

				start_sector      = 0;
				amount_of_sectors = 0;

				result = -1;
			}
			last_sector = start_sector + amount_of_sectors;

#if defined( HAVE_V2_API )
			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " (amount: %" PRIu64 ")",
			 start_sector,
			 last_sector,
			 amount_of_sectors );
#else
			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " (amount: %" PRIu32 ")",
			 start_sector,
			 last_sector,
			 amount_of_sectors );
#endif

#if defined( HAVE_V2_API )
			fprintf(
			 stream,
			 " in segment file(s):" );

			start_sector *= export_handle->bytes_per_sector;
			last_sector  *= export_handle->bytes_per_sector;

			while( start_sector < last_sector )
			{
				if( libewf_handle_seek_offset(
				     export_handle->input_handle,
				     (off64_t) start_sector,
				     SEEK_SET,
				     error ) == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to seek offset: %" PRIu64 ".",
					 function,
					 start_sector );

					if( last_filename != NULL )
					{
						memory_free(
						 last_filename );
					}
					return( -1 );
				}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
				if( libewf_handle_get_filename_size_wide(
				     export_handle->input_handle,
				     &filename_size,
				     error ) != 1 )
#else
				if( libewf_handle_get_filename_size(
				     export_handle->input_handle,
				     &filename_size,
				     error ) != 1 )
#endif
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve filename size.",
					 function );

					if( last_filename != NULL )
					{
						memory_free(
						 last_filename );
					}
					return( -1 );
				}
				filename = (libsystem_character_t *) memory_allocate(
				                                      sizeof( libsystem_character_t ) * filename_size ); 


				if( filename == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create filename.",
					 function );

					if( last_filename != NULL )
					{
						memory_free(
						 last_filename );
					}
					return( -1 );
				}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
				if( libewf_handle_get_filename_wide(
				     export_handle->input_handle,
				     filename,
				     256,
				     error ) != 1 )
#else
				if( libewf_handle_get_filename(
				     export_handle->input_handle,
				     filename,
				     256,
				     error ) != 1 )
#endif
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve filename.",
					 function );

					if( last_filename != NULL )
					{
						memory_free(
						 last_filename );
					}
					memory_free(
					 filename );

					return( -1 );
				}
				if( last_filename == NULL )
				{
					fprintf(
					 stream,
					 " %s",
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
					 ", %s",
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
				start_sector += export_handle->input_chunk_size;
			}
			memory_free(
			 last_filename );

			last_filename      = NULL;
			last_filename_size = 0;
#endif
			fprintf(
			 stream,
			 "\n" );
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

