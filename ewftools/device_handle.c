/* 
 * Device handle
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include "byte_size_string.h"
#include "device_handle.h"
#include "ewfinput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libcstring.h"
#include "ewftools_libcsystem.h"
#include "ewftools_libewf.h"
#include "ewftools_libodraw.h"
#include "ewftools_libsmdev.h"
#include "ewftools_libsmraw.h"
#include "storage_media_buffer.h"

#define DEVICE_HANDLE_INPUT_BUFFER_SIZE		64
#define DEVICE_HANDLE_STRING_SIZE		1024
#define DEVICE_HANDLE_VALUE_SIZE		512
#define DEVICE_HANDLE_NOTIFY_STREAM		stdout

/* Retrieves the track type
 * Returns a string represenation of the track type
 */
const char *device_handle_get_track_type(
             uint8_t track_type )
{
	switch( track_type )
	{
		case DEVICE_HANDLE_TRACK_TYPE_AUDIO:
			return( "audio" );

		case DEVICE_HANDLE_TRACK_TYPE_CDG:
			return( "CD+G" );

		case DEVICE_HANDLE_TRACK_TYPE_MODE1_2048:
			return( "mode1/2048" );

		case DEVICE_HANDLE_TRACK_TYPE_MODE1_2352:
			return( "mode1/2352" );

		case DEVICE_HANDLE_TRACK_TYPE_MODE2_2048:
			return( "mode2/2048" );

		case DEVICE_HANDLE_TRACK_TYPE_MODE2_2324:
			return( "mode2/2324" );

		case DEVICE_HANDLE_TRACK_TYPE_MODE2_2336:
			return( "mode2/2336" );

		case DEVICE_HANDLE_TRACK_TYPE_MODE2_2352:
			return( "mode2/2352" );

		case DEVICE_HANDLE_TRACK_TYPE_CDI_2336:
			return( "CDI/2336" );

		case DEVICE_HANDLE_TRACK_TYPE_CDI_2352:
			return( "CDI/2352" );

		default:
			break;
	}
	return( "UNKNOWN" );
}

/* Initializes the device handle
 * Returns 1 if successful or -1 on error
 */
int device_handle_initialize(
     device_handle_t **device_handle,
     libcerror_error_t **error )
{
	static char *function = "device_handle_initialize";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( *device_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid device handle value already set.",
		 function );

		return( -1 );
	}
	*device_handle = memory_allocate_structure(
	                  device_handle_t );

	if( *device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create device handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *device_handle,
	     0,
	     sizeof( device_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear device handle.",
		 function );

		memory_free(
		 *device_handle );

		*device_handle = NULL;

		return( -1 );
	}
	( *device_handle )->input_buffer = libcstring_system_string_allocate(
	                                    DEVICE_HANDLE_INPUT_BUFFER_SIZE );

	if( ( *device_handle )->input_buffer == NULL )
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
	     ( *device_handle )->input_buffer,
	     0,
	     sizeof( libcstring_system_character_t ) * DEVICE_HANDLE_INPUT_BUFFER_SIZE ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear device handle.",
		 function );

		goto on_error;
	}
	( *device_handle )->number_of_error_retries = 2;
	( *device_handle )->notify_stream           = DEVICE_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *device_handle != NULL )
	{
		if( ( *device_handle )->input_buffer != NULL )
		{
			memory_free(
			 ( *device_handle )->input_buffer );
		}
		memory_free(
		 *device_handle );

		*device_handle = NULL;
	}
	return( -1 );
}

/* Frees the device handle and its elements
 * Returns 1 if successful or -1 on error
 */
int device_handle_free(
     device_handle_t **device_handle,
     libcerror_error_t **error )
{
	static char *function = "device_handle_free";
	int result            = 1;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( *device_handle != NULL )
	{
		memory_free(
		 ( *device_handle )->input_buffer );

		if( ( *device_handle )->toc_filename != NULL )
		{
			memory_free(
			 ( *device_handle )->toc_filename );
		}
		if( ( *device_handle )->type == DEVICE_HANDLE_TYPE_DEVICE )
		{
			if( ( *device_handle )->smdev_input_handle != NULL )
			{
				if( libsmdev_handle_free(
				     &( ( *device_handle )->smdev_input_handle ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free device input handle.",
					 function );

					result = -1;
				}
			}
		}
		else if( ( *device_handle )->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
		{
			if( ( *device_handle )->odraw_input_handle != NULL )
			{
				if( libodraw_handle_free(
				     &( ( *device_handle )->odraw_input_handle ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free optical disc raw input handle.",
					 function );

					result = -1;
				}
			}
		}
		else if( ( *device_handle )->type == DEVICE_HANDLE_TYPE_FILE )
		{
			if( ( *device_handle )->smraw_input_handle != NULL )
			{
				if( libsmraw_handle_free(
				     &( ( *device_handle )->smraw_input_handle ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free raw input handle.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 *device_handle );

		*device_handle = NULL;
	}
	return( result );
}

/* Signals the device handle to abort
 * Returns 1 if successful or -1 on error
 */
int device_handle_signal_abort(
     device_handle_t *device_handle,
     libcerror_error_t **error )
{
	static char *function = "device_handle_signal_abort";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( device_handle->smdev_input_handle != NULL )
		{
			if( libsmdev_handle_signal_abort(
			     device_handle->smdev_input_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to signal device input handle to abort.",
				 function );

				return( -1 );
			}
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		if( device_handle->odraw_input_handle != NULL )
		{
			if( libodraw_handle_signal_abort(
			     device_handle->odraw_input_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to signal optical disc raw input handle to abort.",
				 function );

				return( -1 );
			}
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( device_handle->smraw_input_handle != NULL )
		{
			if( libsmraw_handle_signal_abort(
			     device_handle->smraw_input_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to signal raw input handle to abort.",
				 function );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Opens the input of the device handle
 * Returns 1 if successful or -1 on error
 */
int device_handle_open_input(
     device_handle_t *device_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	static char *function = "device_handle_open_input";
	int result            = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
	if( number_of_filenames == 1 )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libsmdev_check_device_wide(
		          filenames[ 0 ],
	                  error );
#else
		result = libsmdev_check_device(
		          filenames[ 0 ],
	                  error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if filename is a device.",
			 function );

			return( -1 );
		}
	}
	if( result != 0 )
	{
		device_handle->type = DEVICE_HANDLE_TYPE_DEVICE;
	}
	else if( device_handle->toc_filename != NULL )
	{
		device_handle->type = DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE;
	}
	else
	{
		device_handle->type = DEVICE_HANDLE_TYPE_FILE;
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( device_handle_open_smdev_input(
		     device_handle,
		     filenames,
		     number_of_filenames,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open device input.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		if( device_handle_open_odraw_input(
		     device_handle,
		     filenames,
		     number_of_filenames,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open optical disc raw input.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( device_handle_open_smraw_input(
		     device_handle,
		     filenames,
		     number_of_filenames,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open raw input.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Opens the device input of the device handle
 * Returns 1 if successful or -1 on error
 */
int device_handle_open_smdev_input(
     device_handle_t *device_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	static char *function = "device_handle_open_smdev_input";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
	if( device_handle->smdev_input_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid device handle - device input handle already set.",
		 function );

		return( -1 );
	}
	if( libsmdev_handle_initialize(
	     &( device_handle->smdev_input_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create device input handle.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmdev_handle_open_wide(
	     device_handle->smdev_input_handle,
	     (wchar_t * const *) filenames,
	     number_of_filenames,
	     LIBSMDEV_OPEN_READ,
	     error ) != 1 )
#else
	if( libsmdev_handle_open(
	     device_handle->smdev_input_handle,
	     (char * const *) filenames,
	     number_of_filenames,
	     LIBSMDEV_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open device input handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( device_handle->smdev_input_handle != NULL )
	{
		libsmdev_handle_free(
		 &( device_handle->smdev_input_handle ),
		 NULL );
	}
	return( -1 );
}

/* Opens the optical disc raw input of the device handle
 * Returns 1 if successful or -1 on error
 */
int device_handle_open_odraw_input(
     device_handle_t *device_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	libodraw_data_file_t *data_file = NULL;
	static char *function           = "device_handle_open_odraw_input";
	size_t filename_length          = 0;
	int data_file_index             = 0;
	int number_of_data_files        = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
	if( device_handle->odraw_input_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid device handle - optical disc raw input handle already set.",
		 function );

		return( -1 );
	}
	if( libodraw_handle_initialize(
	     &( device_handle->odraw_input_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create optical disc raw input handle.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libodraw_handle_open_wide(
	     device_handle->odraw_input_handle,
	     device_handle->toc_filename,
	     LIBODRAW_OPEN_READ,
	     error ) != 1 )
#else
	if( libodraw_handle_open(
	     device_handle->odraw_input_handle,
	     device_handle->toc_filename,
	     LIBODRAW_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open optical disc raw table of contents file.",
		 function );

		goto on_error;
	}
	if( libodraw_handle_get_number_of_data_files(
	     device_handle->odraw_input_handle,
	     &number_of_data_files,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of optical disc raw data files.",
		 function );

		goto on_error;
	}
	if( number_of_filenames != number_of_data_files )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of filenames does not match number of data files.",
		 function );

		goto on_error;
	}
	for( data_file_index = 0;
	     data_file_index < number_of_data_files;
	     data_file_index++ )
	{
		if( filenames[ data_file_index ] == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing filename: %d.",
			 function,
			 data_file_index );

			return( -1 );
		}
		filename_length = libcstring_system_string_length(
		                   filenames[ data_file_index ] );

		if( libodraw_handle_get_data_file(
		     device_handle->odraw_input_handle,
		     data_file_index,
		     &data_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve optical disc raw data file: %d.",
			 function,
			 data_file_index );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libodraw_data_file_set_filename_wide(
		     data_file,
		     filenames[ data_file_index ],
		     filename_length,
		     error ) != 1 )
#else
		if( libodraw_data_file_set_filename(
		     data_file,
		     filenames[ data_file_index ],
		     filename_length,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set filename in optical disc raw data file: %d.",
			 function,
			 data_file_index );

			goto on_error;
		}
		if( libodraw_data_file_free(
		     &data_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free optical disc raw data file: %d.",
			 function,
			 data_file_index );

			goto on_error;
		}
	}
	if( libodraw_handle_open_data_files(
	     device_handle->odraw_input_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open optical disc raw data files.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data_file != NULL )
	{
		libodraw_data_file_free(
		 &data_file,
		 error );
	}
	if( device_handle->odraw_input_handle != NULL )
	{
		libodraw_handle_free(
		 &( device_handle->odraw_input_handle ),
		 NULL );
	}
	return( -1 );
}

/* Opens the raw input of the device handle
 * Returns 1 if successful or -1 on error
 */
int device_handle_open_smraw_input(
     device_handle_t *device_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	static char *function = "device_handle_open_smraw_input";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
	if( device_handle->smraw_input_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid device handle - raw input handle already set.",
		 function );

		return( -1 );
	}
	if( libsmraw_handle_initialize(
	     &( device_handle->smraw_input_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create raw input handle.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmraw_handle_open_wide(
	     device_handle->smraw_input_handle,
	     (wchar_t * const *) filenames,
	     number_of_filenames,
	     LIBSMDEV_OPEN_READ,
	     error ) != 1 )
#else
	if( libsmraw_handle_open(
	     device_handle->smraw_input_handle,
	     (char * const *) filenames,
	     number_of_filenames,
	     LIBSMDEV_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open raw input handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( device_handle->smraw_input_handle != NULL )
	{
		libsmraw_handle_free(
		 &( device_handle->smraw_input_handle ),
		 NULL );
	}
	return( -1 );
}

/* Closes the device handle
 * Returns the 0 if succesful or -1 on error
 */
int device_handle_close(
     device_handle_t *device_handle,
     libcerror_error_t **error )
{
	static char *function = "device_handle_close";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_close(
		     device_handle->smdev_input_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close device input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		if( libodraw_handle_close(
		     device_handle->odraw_input_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close optical disc raw input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( libsmraw_handle_close(
		     device_handle->smraw_input_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close raw input handle.",
			 function );

			return( -1 );
		}
	}
	return( 0 );
}

/* Reads a buffer from the input of the device handle
 * Returns the number of bytes written or -1 on error
 */
ssize_t device_handle_read_buffer(
         device_handle_t *device_handle,
         uint8_t *buffer,
         size_t read_size,
         libcerror_error_t **error )
{
	static char *function = "device_handle_read_buffer";
	ssize_t read_count    = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		read_count = libsmdev_handle_read_buffer(
			      device_handle->smdev_input_handle,
			      buffer,
			      read_size,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer from device input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		read_count = libodraw_handle_read_buffer(
			      device_handle->odraw_input_handle,
			      buffer,
			      read_size,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer from optical disc raw input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		read_count = libsmraw_handle_read_buffer(
			      device_handle->smraw_input_handle,
			      buffer,
			      read_size,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer from raw input handle.",
			 function );

			return( -1 );
		}
	}
	return( read_count );
}

/* Seeks the offset in the input file
 * Returns the new offset if successful or -1 on error
 */
off64_t device_handle_seek_offset(
         device_handle_t *device_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "device_handle_seek_offset";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		offset = libsmdev_handle_seek_offset(
		          device_handle->smdev_input_handle,
		          offset,
		          whence,
		          error );

		if( offset < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in device input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		offset = libodraw_handle_seek_offset(
		          device_handle->odraw_input_handle,
		          offset,
		          whence,
		          error );

		if( offset < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in optical disc raw input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		offset = libsmraw_handle_seek_offset(
		          device_handle->smraw_input_handle,
		          offset,
		          whence,
		          error );

		if( offset < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in raw input handle.",
			 function );

			return( -1 );
		}
	}
	return( offset );
}

/* Prompts the user for a string
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int device_handle_prompt_for_string(
     device_handle_t *device_handle,
     const libcstring_system_character_t *request_string,
     libcstring_system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error )
{
	static char *function = "device_handle_prompt_for_string";
	int result            = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( internal_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal string.",
		 function );

		return( -1 );
	}
	if( internal_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal string size.",
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
	*internal_string_size = DEVICE_HANDLE_STRING_SIZE;

	*internal_string = libcstring_system_string_allocate(
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
	          device_handle->notify_stream,
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

/* Prompts the user for the number of error retries
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int device_handle_prompt_for_number_of_error_retries(
     device_handle_t *device_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error )
{
	static char *function  = "device_handle_prompt_for_number_of_error_retries";
	uint64_t size_variable = 0;
	int result             = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_size_variable(
	          device_handle->notify_stream,
	          device_handle->input_buffer,
	          DEVICE_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          0,
	          255,
	          device_handle->number_of_error_retries,
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
		device_handle->number_of_error_retries = (uint8_t) size_variable;
	}
	return( result );
}

/* Prompts the user for the zero buffer on error
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int device_handle_prompt_for_zero_buffer_on_error(
     device_handle_t *device_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error )
{
	libcstring_system_character_t *fixed_string_variable = NULL;
	static char *function                                = "device_handle_prompt_for_zero_buffer_on_error";
	int result                                           = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( ewfinput_get_fixed_string_variable(
	     device_handle->notify_stream,
	     device_handle->input_buffer,
	     DEVICE_HANDLE_INPUT_BUFFER_SIZE,
	     request_string,
	     ewfinput_yes_no,
	     2,
	     1,
	     &fixed_string_variable,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_yes_no(
	          fixed_string_variable,
	          &( device_handle->zero_buffer_on_error ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine zero buffer on error.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_type(
     device_handle_t *device_handle,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "device_handle_get_type";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = device_handle->type;

	return( 1 );
}

/* Retrieves the media size
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_media_size(
     device_handle_t *device_handle,
     size64_t *media_size,
     libcerror_error_t **error )
{
	static char *function = "device_handle_get_media_size";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_media_size(
		     device_handle->smdev_input_handle,
		     media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media size from device input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		if( libodraw_handle_get_media_size(
		     device_handle->odraw_input_handle,
		     media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media size from optical disc raw input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( libsmraw_handle_get_media_size(
		     device_handle->smraw_input_handle,
		     media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media size from raw input handle.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the media type
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_media_type(
     device_handle_t *device_handle,
     uint8_t *media_type,
     libcerror_error_t **error )
{
	static char *function    = "device_handle_get_media_type";
	uint8_t smdev_media_type = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( media_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media type.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_media_type(
		     device_handle->smdev_input_handle,
		     &smdev_media_type,
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
		switch( smdev_media_type )
		{
			case LIBSMDEV_MEDIA_TYPE_REMOVABLE:
				*media_type = DEVICE_HANDLE_MEDIA_TYPE_REMOVABLE;

				break;

			case LIBSMDEV_MEDIA_TYPE_FIXED:
				*media_type = DEVICE_HANDLE_MEDIA_TYPE_FIXED;

				break;

			case LIBSMDEV_MEDIA_TYPE_OPTICAL:
				*media_type = DEVICE_HANDLE_MEDIA_TYPE_OPTICAL;

				break;

			case LIBSMDEV_MEDIA_TYPE_MEMORY:
				*media_type = DEVICE_HANDLE_MEDIA_TYPE_MEMORY;

				break;
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		*media_type = DEVICE_HANDLE_MEDIA_TYPE_OPTICAL;
	}
	return( 1 );
}

/* Retrieves the number of bytes per sector
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_bytes_per_sector(
     device_handle_t *device_handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error )
{
	static char *function = "device_handle_get_bytes_per_sector";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_bytes_per_sector(
		     device_handle->smdev_input_handle,
		     bytes_per_sector,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve bytes per sector from device input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		if( libodraw_handle_get_bytes_per_sector(
		     device_handle->odraw_input_handle,
		     bytes_per_sector,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve bytes per sector from optical disc raw input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( libsmraw_handle_get_bytes_per_sector(
		     device_handle->smraw_input_handle,
		     bytes_per_sector,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve bytes per sector from raw input handle.",
			 function );

			return( -1 );
		}
		if( *bytes_per_sector == 0 )
		{
			*bytes_per_sector = 512;
		}
	}
	return( 1 );
}

/* Retrieves the information value by identifier
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int device_handle_get_information_value(
     device_handle_t *device_handle,
     const uint8_t *information_value_identifier,
     size_t information_value_identifier_length,
     libcstring_system_character_t *information_value,
     size_t information_value_size,
     libcerror_error_t **error )
{
	static char *function = "device_handle_get_information_value";
	int result            = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libsmdev_handle_get_utf16_information_value(
		          device_handle->smdev_input_handle,
		          information_value_identifier,
		          information_value_identifier_length,
		          (uint16_t *) information_value,
		          information_value_size,
		          error );
#else
		result = libsmdev_handle_get_utf8_information_value(
		          device_handle->smdev_input_handle,
		          information_value_identifier,
		          information_value_identifier_length,
		          (uint8_t *) information_value,
		          information_value_size,
		          error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: %s from device input handle.",
			 function,
			 (char *) information_value_identifier );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		result = 0;
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libsmraw_handle_get_utf16_information_value(
		          device_handle->smraw_input_handle,
		          information_value_identifier,
		          information_value_identifier_length,
		          (uint16_t *) information_value,
		          information_value_size,
		          error );
#else
		result = libsmraw_handle_get_utf8_information_value(
		          device_handle->smraw_input_handle,
		          information_value_identifier,
		          information_value_identifier_length,
		          (uint8_t *) information_value,
		          information_value_size,
		          error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve information value: %s from raw input handle.",
			 function,
			 (char *) information_value_identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the number of sessions
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_number_of_sessions(
     device_handle_t *device_handle,
     int *number_of_sessions,
     libcerror_error_t **error )
{
	static char *function = "device_handle_get_number_of_sessions";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_number_of_sessions(
		     device_handle->smdev_input_handle,
		     number_of_sessions,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sessions from device input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		if( libodraw_handle_get_number_of_sessions(
		     device_handle->odraw_input_handle,
		     number_of_sessions,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sessions from optical disc raw input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( number_of_sessions == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of sessions raw input handle.",
			 function );

			return( -1 );
		}
		*number_of_sessions = 0;
	}
	return( 1 );
}

/* Retrieves the information of a session
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_session(
     device_handle_t *device_handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	static char *function = "device_handle_get_session";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_session(
		     device_handle->smdev_input_handle,
		     index,
		     start_sector,
		     number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve session: %d from device input handle.",
			 function,
			 index );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		if( libodraw_handle_get_session(
		     device_handle->odraw_input_handle,
		     index,
		     start_sector,
		     number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve session: %d from optical disc raw input handle.",
			 function,
			 index );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index value out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of tracks
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_number_of_tracks(
     device_handle_t *device_handle,
     int *number_of_tracks,
     libcerror_error_t **error )
{
	static char *function = "device_handle_get_number_of_tracks";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_number_of_tracks(
		     device_handle->smdev_input_handle,
		     number_of_tracks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of tracks from device input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		if( libodraw_handle_get_number_of_tracks(
		     device_handle->odraw_input_handle,
		     number_of_tracks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of tracks from optical disc raw input handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( number_of_tracks == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of tracks raw input handle.",
			 function );

			return( -1 );
		}
		*number_of_tracks = 0;
	}
	return( 1 );
}

/* Retrieves the information of a track
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_track(
     device_handle_t *device_handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function           = "device_handle_get_track";
	uint64_t data_file_start_sector = 0;
	int data_file_index             = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_track(
		     device_handle->smdev_input_handle,
		     index,
		     start_sector,
		     number_of_sectors,
		     type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve track: %d from device input handle.",
			 function,
			 index );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	{
		if( libodraw_handle_get_track(
		     device_handle->odraw_input_handle,
		     index,
		     start_sector,
		     number_of_sectors,
		     type,
		     &data_file_index,
		     &data_file_start_sector,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve track: %d from optical disc raw input handle.",
			 function,
			 index );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index value out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets a string
 * Returns 1 if successful or -1 on error
 */
int device_handle_set_string(
     device_handle_t *device_handle,
     const libcstring_system_character_t *string,
     libcstring_system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error )
{
	static char *function = "device_handle_set_string";
	size_t string_length  = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
		 "%s: invalid internal string.",
		 function );

		return( -1 );
	}
	if( internal_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal string size.",
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
	string_length = libcstring_system_string_length(
	                 string );

	if( string_length > 0 )
	{
		*internal_string = libcstring_system_string_allocate(
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
		if( libcstring_system_string_copy(
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

/* Sets the number of error retries
 * Returns 1 if successful or -1 on error
 */
int device_handle_set_number_of_error_retries(
     device_handle_t *device_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "device_handle_set_number_of_error_retries";
	size_t string_length   = 0;
	uint64_t size_variable = 0;
	int result             = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	string_length = libcstring_system_string_length(
	                 string );

	if( libcsystem_string_decimal_copy_to_64_bit(
	     string,
	     string_length + 1,
	     &size_variable,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine number of error retries.",
		 function );

		return( -1 );
	}
	if( size_variable > (uint64_t) UINT8_MAX )
	{
		result = 0;
	}
	else
	{
		device_handle->number_of_error_retries = (uint8_t) size_variable;
	}
	return( result );
}

/* Sets the error values
 * Returns 1 if successful or -1 on error
 */
int device_handle_set_error_values(
     device_handle_t *device_handle,
     size_t error_granularity,
     libcerror_error_t **error )
{
	static char *function = "device_handle_set_error_values";
	uint8_t error_flags   = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_set_number_of_error_retries(
		     device_handle->smdev_input_handle,
		     device_handle->number_of_error_retries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set number of error retries in device input handle.",
			 function );

			return( -1 );
		}
		if( libsmdev_handle_set_error_granularity(
		     device_handle->smdev_input_handle,
		     error_granularity,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set error granularity in device input handle.",
			 function );

			return( -1 );
		}
		if( device_handle->zero_buffer_on_error != 0 )
		{
			error_flags = LIBSMDEV_ERROR_FLAG_ZERO_ON_ERROR;
		}
		if( libsmdev_handle_set_error_flags(
		     device_handle->smdev_input_handle,
		     error_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set error flags in device input handle.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the number of read errors
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_number_of_read_errors(
     device_handle_t *device_handle,
     int *number_of_read_errors,
     libcerror_error_t **error )
{
	static char *function = "device_handle_get_number_of_read_errors";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_number_of_errors(
		     device_handle->smdev_input_handle,
		     number_of_read_errors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of read errors.",
			 function );

			return( -1 );
		}
	}
	else if( ( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	      || ( device_handle->type == DEVICE_HANDLE_TYPE_FILE ) )
	{
		if( number_of_read_errors == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of read errors.",
			 function );

			return( -1 );
		}
		*number_of_read_errors = 0;
	}
	return( 1 );
}

/* Retrieves the information of a read error
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_read_error(
     device_handle_t *device_handle,
     int index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "device_handle_get_read_error";

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_error(
		     device_handle->smdev_input_handle,
		     index,
		     offset,
		     size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve read error: %d.",
			 function,
			 index );

			return( -1 );
		}
	}
	else if( ( device_handle->type == DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE )
	      || ( device_handle->type == DEVICE_HANDLE_TYPE_FILE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index value out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Print the media information to a stream
 * Returns 1 if successful or -1 on error
 */
int device_handle_media_information_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	uint8_t media_information_value[ 64 ];

        libcstring_system_character_t byte_size_string[ 16 ];

	static char *function     = "device_handle_media_information_fprint";
	size64_t media_size       = 0;
	uint32_t bytes_per_sector = 0;
	uint8_t bus_type          = 0;
	uint8_t media_type        = 0;
	int result                = 0;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		fprintf(
		 stream,
		 "Device information:\n" );

		if( libsmdev_handle_get_bus_type(
		     device_handle->smdev_input_handle,
		     &bus_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve bus type.",
			 function );

			return( -1 );
		}
		fprintf(
		 stream,
		 "Bus type:\t\t\t\t" );

		switch( bus_type )
		{
			case LIBSMDEV_BUS_TYPE_ATA:
				fprintf(
				 stream,
				 "ATA/ATAPI" );
				break;

			case LIBSMDEV_BUS_TYPE_FIREWIRE:
				fprintf(
				 stream,
				 "FireWire (IEEE1394)" );
				break;

			case LIBSMDEV_BUS_TYPE_SCSI:
				fprintf(
				 stream,
				 "SCSI" );
				break;

			case LIBSMDEV_BUS_TYPE_USB:
				fprintf(
				 stream,
				 "USB" );
				break;
		}
		fprintf(
		 stream,
		 "\n" );

		result = libsmdev_handle_get_utf8_information_value(
		          device_handle->smdev_input_handle,
		          (uint8_t *) "vendor",
		          6,
		          media_information_value,
		          64,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media information value: vendor.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			media_information_value[ 0 ] = 0;
		}
		fprintf(
		 stream,
		 "Vendor:\t\t\t\t\t%s\n",
		 (char *) media_information_value );

		result = libsmdev_handle_get_utf8_information_value(
		          device_handle->smdev_input_handle,
		          (uint8_t *) "model",
		          5,
		          media_information_value,
		          64,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media information value: model.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			media_information_value[ 0 ] = 0;
		}
		fprintf(
		 stream,
		 "Model:\t\t\t\t\t%s\n",
		 (char *) media_information_value );

		result = libsmdev_handle_get_utf8_information_value(
		          device_handle->smdev_input_handle,
		          (uint8_t *) "serial_number",
		          13,
		          media_information_value,
		          64,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media information value: serial_number.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			media_information_value[ 0 ] = 0;
		}
		fprintf(
		 stream,
		 "Serial:\t\t\t\t\t%s\n",
		 (char *) media_information_value );

		fprintf(
		 stream,
		 "\n" );
	}
	fprintf(
	 stream,
	 "Storage media information:\n" );

	fprintf(
	 stream,
	 "Type:\t\t\t\t\t" );

	switch( device_handle->type )
	{
		case DEVICE_HANDLE_TYPE_DEVICE:
			fprintf(
			 stream,
			 "Device" );
			break;

		case DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE:
			fprintf(
			 stream,
			 "Optical disc RAW image" );
			break;

		case DEVICE_HANDLE_TYPE_FILE:
			fprintf(
			 stream,
			 "RAW image" );
			break;

		default:
			fprintf(
			 stream,
			 "unknown" );
			break;
	}
	fprintf(
	 stream,
	 "\n" );

	if( device_handle_get_media_type(
	     device_handle,
	     &media_type,
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
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		fprintf(
		 stream,
		 "Media type:\t\t\t\t" );

		switch( media_type )
		{
			case DEVICE_HANDLE_MEDIA_TYPE_REMOVABLE:
				fprintf(
				 stream,
				 "Removable" );

				break;

			case DEVICE_HANDLE_MEDIA_TYPE_FIXED:
				fprintf(
				 stream,
				 "Fixed" );

				break;

			case DEVICE_HANDLE_MEDIA_TYPE_OPTICAL:
				fprintf(
				 stream,
				 "Optical" );

				break;

			case DEVICE_HANDLE_MEDIA_TYPE_MEMORY:
				fprintf(
				 stream,
				 "Memory" );

				break;

			default:
				fprintf(
				 stream,
				 "unknown" );

				break;
		}
		fprintf(
		 stream,
		 "\n" );
	}
	if( device_handle_get_media_size(
	     device_handle,
	     &media_size,
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
	result = byte_size_string_create(
		  byte_size_string,
		  16,
		  media_size,
		  BYTE_SIZE_STRING_UNIT_MEGABYTE,
		  NULL );

	if( result == 1 )
	{
		fprintf(
		 stream,
		 "Media size:\t\t\t\t%" PRIs_LIBCSTRING_SYSTEM " (%" PRIu64 " bytes)\n",
		 byte_size_string,
		 media_size );
	}
	else
	{
		fprintf(
		 stream,
		 "Media size:\t\t\t\t%" PRIu64 " bytes\n",
		 media_size );
	}
	if( device_handle_get_bytes_per_sector(
	     device_handle,
	     &bytes_per_sector,
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
	fprintf(
	 stream,
	 "Bytes per sector:\t\t\t%" PRIu32 "\n",
	 bytes_per_sector );

	if( media_type == DEVICE_HANDLE_MEDIA_TYPE_OPTICAL )
	{
		if( device_handle_sessions_fprint(
		     device_handle,
		     stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print sessions.",
			 function );

			return( -1 );
		}
		if( device_handle_tracks_fprint(
		     device_handle,
		     stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print tracks.",
			 function );

			return( -1 );
		}
	}
	else
	{
		fprintf(
		 stream,
		 "\n" );
	}
	return( 1 );
}

/* Print the read errors to a stream
 * Returns 1 if successful or -1 on error
 */
int device_handle_read_errors_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	static char *function     = "device_handle_read_errors_fprint";
	off64_t read_error_offset = 0;
	size64_t read_error_size  = 0;
	uint32_t bytes_per_sector = 0;
	int number_of_read_errors = 0;
	int read_error_index      = 0;
	int result                = 1;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
	if( device_handle_get_bytes_per_sector(
	     device_handle,
	     &bytes_per_sector,
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
	if( bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid bytes per sector returned.",
		 function );

		return( -1 );
	}
	if( device_handle_get_number_of_read_errors(
	     device_handle,
	     &number_of_read_errors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of read errors.",
		 function );

		return( -1 );
	}
	if( number_of_read_errors > 0 )
	{
		fprintf(
		 stream,
		 "Errors reading device:\n" );
		fprintf(
		 stream,
		 "\ttotal number: %d\n",
		 number_of_read_errors );
		
		for( read_error_index = 0;
		     read_error_index < number_of_read_errors;
		     read_error_index++ )
		{
			if( device_handle_get_read_error(
			     device_handle,
			     read_error_index,
			     &read_error_offset,
			     &read_error_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve read error: %d.",
				 function,
				 read_error_index );

				result = -1;
			}
			else
			{
				fprintf(
				 stream,
				 "\tat sector(s): %" PRIi64 " - %" PRIi64 " number: %" PRIu64 " (offset: 0x%08" PRIx64 " of size: %" PRIu64 ")\n",
				 read_error_offset / bytes_per_sector,
				 ( read_error_offset + read_error_size ) / bytes_per_sector,
				 read_error_size / bytes_per_sector,
				 read_error_offset,
				 read_error_size );
			}
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

/* Print the sessions to a stream
 * Returns 1 if successful or -1 on error
 */
int device_handle_sessions_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	static char *function      = "device_handle_sessions_fprint";
	uint64_t last_sector       = 0;
	uint64_t number_of_sectors = 0;
	uint64_t start_sector      = 0;
	int number_of_sessions     = 0;
	int session_index          = 0;
	int result                 = 1;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
	if( device_handle_get_number_of_sessions(
	     device_handle,
	     &number_of_sessions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sessions.",
		 function );

		return( -1 );
	}
	if( number_of_sessions > 0 )
	{
		fprintf(
		 stream,
		 "Sessions:\n" );
		
		fprintf(
		 stream,
		 "\ttotal number: %d\n",
		 number_of_sessions );

		for( session_index = 0;
		     session_index < number_of_sessions;
		     session_index++ )
		{
			if( device_handle_get_session(
			     device_handle,
			     session_index,
			     &start_sector,
			     &number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve session: %d.",
				 function,
				 session_index );

				start_sector      = 0;
				number_of_sectors = 0;

				result = -1;
			}
			last_sector = start_sector + number_of_sectors;

			if( number_of_sectors != 0 )
			{
				last_sector -= 1;
			}
			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " number: %" PRIu64 "\n",
			 start_sector,
			 last_sector,
			 number_of_sectors );
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

/* Print the tracks to a stream
 * Returns 1 if successful or -1 on error
 */
int device_handle_tracks_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	static char *function      = "device_handle_tracks_fprint";
	uint64_t last_sector       = 0;
	uint64_t number_of_sectors = 0;
	uint64_t start_sector      = 0;
	uint8_t type               = 0;
	int number_of_tracks       = 0;
	int track_index            = 0;
	int result                 = 1;

	if( device_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
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
	if( device_handle_get_number_of_tracks(
	     device_handle,
	     &number_of_tracks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of tracks.",
		 function );

		return( -1 );
	}
	if( number_of_tracks > 0 )
	{
		fprintf(
		 stream,
		 "Tracks:\n" );
		
		fprintf(
		 stream,
		 "\ttotal number: %d\n",
		 number_of_tracks );

		for( track_index = 0;
		     track_index < number_of_tracks;
		     track_index++ )
		{
			if( device_handle_get_track(
			     device_handle,
			     track_index,
			     &start_sector,
			     &number_of_sectors,
			     &type,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve track: %d.",
				 function,
				 track_index );

				start_sector      = 0;
				number_of_sectors = 0;
				type              = 0;

				result = -1;
			}
			fprintf(
			 stream,
			 "\t" );

			fprintf(
			 stream,
			 "type: %s",
			 device_handle_get_track_type(
			  type ) );

			last_sector = start_sector + number_of_sectors;

			if( number_of_sectors != 0 )
			{
				last_sector -= 1;
			}
			fprintf(
			 stream,
			 " at sector(s): %" PRIu64 " - %" PRIu64 " number: %" PRIu64 "\n",
			 start_sector,
			 last_sector,
			 number_of_sectors );
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

