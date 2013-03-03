/* 
 * Mount handle
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

#include "ewftools_libcerror.h"
#include "ewftools_libcstring.h"
#include "ewftools_libewf.h"
#include "mount_handle.h"

/* Initializes the mount handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_initialize(
     mount_handle_t **mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_initialize";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*mount_handle = memory_allocate_structure(
	                 mount_handle_t );

	if( *mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create mount handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mount_handle,
	     0,
	     sizeof( mount_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear mount handle.",
		 function );

		goto on_error;
	}
	if( libewf_handle_initialize(
	     &( ( *mount_handle )->input_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input handle.",
		 function );

		goto on_error;
	}
	( *mount_handle )->input_format = MOUNT_HANDLE_INPUT_FORMAT_RAW;

	return( 1 );

on_error:
	if( *mount_handle != NULL )
	{
		memory_free(
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( -1 );
}

/* Frees the mount handle and its elements
 * Returns 1 if successful or -1 on error
 */
int mount_handle_free(
     mount_handle_t **mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_free";
	int result            = 1;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle != NULL )
	{
		if( ( *mount_handle )->root_file_entry != NULL )
		{
			if( libewf_file_entry_free(
			     &( ( *mount_handle )->root_file_entry ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root file entry.",
				 function );

				result = -1;
			}
		}
		if( libewf_handle_free(
		     &( ( *mount_handle )->input_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( result );
}

/* Signals the mount handle to abort
 * Returns 1 if successful or -1 on error
 */
int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_signal_abort";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_handle->input_handle != NULL )
	{
		if( libewf_handle_signal_abort(
		     mount_handle->input_handle,
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
	return( 1 );
}

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_maximum_number_of_open_handles(
     mount_handle_t *mount_handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_maximum_number_of_open_handles";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_maximum_number_of_open_handles(
	     mount_handle->input_handle,
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

/* Sets the format
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int mount_handle_set_format(
     mount_handle_t *mount_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_format";
	size_t string_length  = 0;
	int result            = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	string_length = libcstring_system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "raw" ),
		     3 ) == 0 )
		{
			mount_handle->input_format = MOUNT_HANDLE_INPUT_FORMAT_RAW;
			result                     = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "files" ),
		     5 ) == 0 )
		{
			mount_handle->input_format = MOUNT_HANDLE_INPUT_FORMAT_FILES;
			result                     = 1;
		}
	}
	return( result );
}

/* Opens the input of the mount handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_open_input(
     mount_handle_t *mount_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	libcstring_system_character_t **libewf_filenames = NULL;
	static char *function                            = "mount_handle_open_input";
	size_t first_filename_length                     = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
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
		first_filename_length = libcstring_system_string_length(
		                         filenames[ 0 ] );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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

			return( -1 );
		}
		filenames = (libcstring_system_character_t * const *) libewf_filenames;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     mount_handle->input_handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     mount_handle->input_handle,
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
		 "%s: unable to open file(s).",
		 function );

		if( libewf_filenames != NULL )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
	if( libewf_filenames != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
	}
	if( mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		if( libewf_handle_get_root_file_entry(
		     mount_handle->input_handle,
		     &( mount_handle->root_file_entry ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root file entry.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Closes the mount handle
 * Returns the 0 if succesful or -1 on error
 */
int mount_handle_close(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_close";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_close(
	     mount_handle->input_handle,
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

/* Read a buffer from the input handle
 * Return the number of bytes read if successful or -1 on error
 */
ssize_t mount_handle_read_buffer(
         mount_handle_t *mount_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	static char *function = "mount_handle_read_buffer";
	ssize_t read_count    = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	read_count = libewf_handle_read_buffer(
	              mount_handle->input_handle,
	              buffer,
	              size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer from input handle.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a specific offset from the input handle
 * Return the offset if successful or -1 on error
 */
off64_t mount_handle_seek_offset(
         mount_handle_t *mount_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "mount_handle_seek_offset";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	offset = libewf_handle_seek_offset(
	          mount_handle->input_handle,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in input handle.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Retrieves the media size of the input handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_get_media_size(
     mount_handle_t *mount_handle,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_get_media_size";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_media_size(
	     mount_handle->input_handle,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size from input handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the file entry of a specific path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int mount_handle_get_file_entry_by_path(
     mount_handle_t *mount_handle,
     const libcstring_system_character_t *path,
     size_t path_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libcstring_system_character_t *ewf_path = NULL;
	static char *function                   = "mount_handle_get_file_entry_by_path";
	size_t path_index                       = 0;
	int result                              = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	if( path_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path length.",
		 function );

		return( -1 );
	}
	if( path[ 0 ] != (libcstring_system_character_t) '/' )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path.",
		 function );

		return( -1 );
	}
	ewf_path = libcstring_system_string_allocate(
	            path_length + 1 );

	if( ewf_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create REGF path.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     ewf_path,
	     path,
	     path_length + 1 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy REGF path.",
		 function );

		goto on_error;
	}
	for( path_index = 0;
	     path_index < path_length;
	     path_index++ )
	{
		if( ewf_path[ path_index ] == (libcstring_system_character_t) '/' )
		{
			ewf_path[ path_index ] = (libcstring_system_character_t) '\\';
		}
		else if( ewf_path[ path_index ] == (libcstring_system_character_t) '\\' )
		{
			ewf_path[ path_index ] = (libcstring_system_character_t) '/';
		}
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_file_get_file_entry_by_utf16_path(
		  mount_handle->input_handle,
		  (uint16_t *) ewf_path,
		  path_length,
		  file_entry,
		  error );
#else
	result = libewf_file_get_file_entry_by_utf8_path(
		  mount_handle->input_handle,
		  (uint8_t *) ewf_path,
		  path_length,
		  file_entry,
		  error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry.",
		 function );

		goto on_error;
	}
	memory_free(
	 ewf_path );

	return( result );

on_error:
	if( ewf_path != NULL )
	{
		memory_free(
		 ewf_path );
	}
	return( -1 );
}

