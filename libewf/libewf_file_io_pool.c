/*
 * File IO pool
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#include "libewf_definitions.h"
#include "libewf_file_io_handle.h"
#include "libewf_file_io_pool.h"
#include "libewf_notify.h"

/* Initialize the file io pool
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_pool_initialize(
     libewf_file_io_pool_t **file_io_pool,
     int amount_of_files_io_handles,
     int maximum_amount_of_open_files,
     liberror_error_t **error )
{
	static char *function    = "libewf_file_io_pool_initialize";
	size_t file_io_pool_size = 0;

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( amount_of_files_io_handles < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid amount of file io handles value less than zero.",
		 function );

		return( -1 );
	}
	if( maximum_amount_of_open_files < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid maximum amount of open files value less than zero.",
		 function );

		return( -1 );
	}
	if( *file_io_pool == NULL )
	{
		file_io_pool_size = sizeof( libewf_file_io_handle_t ) * amount_of_files_io_handles;

		if( file_io_pool_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid file io pool size value exceeds maximum.",
			 function );

			return( -1 );
		}
		*file_io_pool = (libewf_file_io_pool_t *) memory_allocate(
		                                           sizeof( libewf_file_io_pool_t ) );

		if( *file_io_pool == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file io pool.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *file_io_pool,
		     0,
		     sizeof( libewf_file_io_pool_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear file io pool.",
			 function );

			memory_free(
			 *file_io_pool );

			*file_io_pool = NULL;

			return( -1 );
		}
		( *file_io_pool )->last_used_list = (libewf_list_t *) memory_allocate(
		                                                       sizeof( libewf_list_t ) );

		if( ( *file_io_pool )->last_used_list == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create last used list.",
			 function );

			memory_free(
			 *file_io_pool );

			*file_io_pool = NULL;

			return( -1 );
		}
		if( memory_set(
		     ( *file_io_pool )->last_used_list,
		     0,
		     sizeof( libewf_list_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear last used list.",
			 function );

			memory_free(
			 ( *file_io_pool )->last_used_list );
			memory_free(
			 *file_io_pool );

			*file_io_pool = NULL;

			return( -1 );
		}
		if( amount_of_files_io_handles > 0 )
		{
			( *file_io_pool )->file_io_handle = (libewf_file_io_handle_t *) memory_allocate(
											 file_io_pool_size );

			if( ( *file_io_pool )->file_io_handle == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create file io handles.",
				 function );

				memory_free(
				 ( *file_io_pool )->last_used_list );
				memory_free(
				 *file_io_pool );

				*file_io_pool = NULL;

				return( -1 );
			}
			if( memory_set(
			     ( *file_io_pool )->file_io_handle,
			     0,
			     file_io_pool_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear file io handles.",
				 function );

				memory_free(
				 ( *file_io_pool )->file_io_handle );
				memory_free(
				 ( *file_io_pool )->last_used_list );
				memory_free(
				 *file_io_pool );

				*file_io_pool = NULL;

				return( -1 );
			}
		}
		( *file_io_pool )->amount_of_files_io_handles   = amount_of_files_io_handles;
		( *file_io_pool )->maximum_amount_of_open_files = maximum_amount_of_open_files;
	}
	return( 1 );
}

/* Frees the file io pool including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_pool_free(
     libewf_file_io_pool_t **file_io_pool,
     liberror_error_t **error )
{
	static char *function = "libewf_file_io_pool_free";
	int iterator          = 0;

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( *file_io_pool != NULL )
	{
		for( iterator = 0; iterator < ( *file_io_pool )->amount_of_files_io_handles; iterator++ )
		{
			if( libewf_file_io_handle_close(
			     &( ( *file_io_pool )->file_io_handle[ iterator ] ),
			     error ) != 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close file io handle: %d.",
				 function,
				 iterator );
			}
			if( ( *file_io_pool )->file_io_handle[ iterator ].filename != NULL )
			{
				memory_free(
				 ( *file_io_pool )->file_io_handle[ iterator ].filename );
			}
		}
		if( ( ( *file_io_pool )->last_used_list != NULL )
		 && ( libewf_list_free(
		       ( *file_io_pool )->last_used_list,
		       NULL,
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free last used list.",
			 function );
		}
		if( ( *file_io_pool )->file_io_handle != NULL )
		{
			memory_free(
			 ( *file_io_pool )->file_io_handle );
		}
		memory_free(
		 *file_io_pool );

		*file_io_pool = NULL;
	}
	return( 1 );
}

/* Resizes the file io pool
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_pool_resize(
     libewf_file_io_pool_t *file_io_pool,
     int amount_of_files_io_handles,
     liberror_error_t **error )
{
	void *reallocation       = NULL;
	static char *function    = "libewf_file_io_pool_resize";
	size_t file_io_pool_size = 0;

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( amount_of_files_io_handles <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of file io handles value zero or less.",
		 function );

		return( -1 );
	}
	if( file_io_pool->amount_of_files_io_handles < amount_of_files_io_handles )
	{
		file_io_pool_size = sizeof( libewf_file_io_handle_t ) * amount_of_files_io_handles;

		if( file_io_pool_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid file io pool size value exceeds maximum.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
				file_io_pool->file_io_handle,
				file_io_pool_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize file io handles.",
			 function );

			return( -1 );
		}
		file_io_pool->file_io_handle = (libewf_file_io_handle_t *) reallocation;

		if( memory_set(
		     &( file_io_pool->file_io_handle[ file_io_pool->amount_of_files_io_handles ] ),
		     0,
		     sizeof( libewf_file_io_handle_t ) * ( amount_of_files_io_handles - file_io_pool->amount_of_files_io_handles ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear file io handles.",
			 function );

			return( -1 );
		}
		file_io_pool->amount_of_files_io_handles = amount_of_files_io_handles;
	}
	return( 1 );
}

/* Creates a new file io handle
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_pool_create_file_io_handle(
     libewf_file_io_pool_t *file_io_pool,
     libewf_file_io_handle_t **file_io_handle,
     int *entry,
     liberror_error_t **error )
{
	static char *function = "libewf_file_io_pool_create_file_io_handle";

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( entry == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry.",
		 function );

		return( -1 );
	}
	*entry = file_io_pool->amount_of_files;

	/* Resize the file io pool if necessary
	 */
	if( ( *entry + 1 ) >= file_io_pool->amount_of_files_io_handles )
	{
		if( libewf_file_io_pool_resize(
		     file_io_pool,
		     *entry + 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize file io pool.",
			 function );

			*entry          = -1;
			*file_io_handle = NULL;

			return( -1 );
		}
	}
	file_io_pool->amount_of_files++;

	file_io_pool->file_io_handle[ *entry ].file_descriptor = -1;

	/* Add the file io handle entry
	 */
	*file_io_handle = &( file_io_pool->file_io_handle[ *entry ] );

	return( 1 );
}

/* Opens the file io handle
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_pool_open_file_io_handle(
     libewf_file_io_pool_t *file_io_pool,
     libewf_file_io_handle_t *file_io_handle,
     int flags,
     liberror_error_t **error )
{
	libewf_list_element_t *last_used_list_element = NULL;
	static char *function                         = "libewf_file_io_pool_open_file_io_handle";

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle->file_descriptor != -1 )
	{
		return( 1 );
	}
	/* Check if there is room in the file io pool for another open file
	 */
	if( ( file_io_pool->maximum_amount_of_open_files != LIBEWF_FILE_IO_POOL_UNLIMITED_AMOUNT_OF_OPEN_FILES )
	 && ( ( file_io_pool->amount_of_open_files + 1 ) >= file_io_pool->maximum_amount_of_open_files ) )
	{
		if( file_io_pool->last_used_list == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid file io pool - missing last used list.",
			 function );

			return( -1 );
		}
		last_used_list_element = file_io_pool->last_used_list->last;

		if( last_used_list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid last used list element.",
			 function );

			return( -1 );
		}
		if( libewf_list_remove_element(
		     file_io_pool->last_used_list,
		     last_used_list_element,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to remove last used list element from list.",
			 function );

			return( -1 );
		}
		if( libewf_file_io_handle_close(
		     (libewf_file_io_handle_t *) last_used_list_element->value,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file io handle.",
			 function );

			memory_free(
			 last_used_list_element );

			return( -1 );
		}
		/* The last used list element is reused to contain the new last used entry
		 */
	}
	else
	{
		last_used_list_element = (libewf_list_element_t *) memory_allocate(
								    sizeof( libewf_list_element_t ) );

		if( last_used_list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create last used list element.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     last_used_list_element,
		     0,
		     sizeof( libewf_list_element_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear last used list element.",
			 function );

			memory_free(
			 last_used_list_element );

			return( -1 );
		}
	}
	if( libewf_file_io_handle_open(
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file io handle.",
		 function );

		memory_free(
		 last_used_list_element );

		return( -1 );
	}
	last_used_list_element->value = (intptr_t *) file_io_handle;

	if( libewf_list_prepend_element(
	     file_io_pool->last_used_list,
	     last_used_list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to prepend last used list element to list.",
		 function );

		memory_free(
		 last_used_list_element );

		return( -1 );
	}
	return( 1 );
}

/* Adds a file to the file io pool
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_pool_add_file(
     libewf_file_io_pool_t *file_io_pool,
     char *filename,
     int flags,
     int *entry,
     liberror_error_t **error )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_file_io_pool_add_file";
	size_t filename_size                    = 0;

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
	if( libewf_file_io_pool_create_file_io_handle(
	     file_io_pool,
	     &file_io_handle,
	     entry,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io pool entry.",
		 function );

		return( -1 );
	}
	filename_size = 1 + narrow_string_length(
			     filename );

	if( libewf_file_io_handle_set_filename(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file io handle.",
		 function );

		return( -1 );
	}
	file_io_handle->flags = flags;

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )
/* Adds a file to the file io pool
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_pool_add_file_wide(
     libewf_file_io_pool_t *file_io_pool,
     wchar_t *filename,
     int flags,
     int *entry,
     liberror_error_t **error )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_file_io_pool_add_file_wide";
	size_t filename_size                    = 0;

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
	if( libewf_file_io_pool_create_file_io_handle(
	     file_io_pool,
	     &file_io_handle,
	     entry,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io pool entry.",
		 function );

		return( -1 );
	}
	filename_size = 1 + wide_string_length(
			     filename );

	if( libewf_file_io_handle_set_filename_wide(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file io handle.",
		 function );

		return( -1 );
	}
	file_io_handle->flags = flags;

	return( 1 );
}
#endif

/* Opens a file in the file io pool
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_pool_open(
     libewf_file_io_pool_t *file_io_pool,
     int entry,
     int flags,
     liberror_error_t **error )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_file_io_pool_open";

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( file_io_pool->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file io pool - missing file io handles.",
		 function );

		return( -1 );
	}
	if( ( entry < 0 )
	 || ( entry >= file_io_pool->amount_of_files ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid entry value out of range.",
		 function );

		return( -1 );
	}
	file_io_handle = &( file_io_pool->file_io_handle[ entry ] );

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle for entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	if( file_io_handle->file_descriptor == -1 )
	{
		file_io_handle->flags = flags;

		if( libewf_file_io_pool_open_file_io_handle(
		     file_io_pool,
		     file_io_handle,
		     file_io_handle->flags,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open entry: %d.",
			 function,
			 entry );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reopens a file in the file io pool
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_pool_reopen(
     libewf_file_io_pool_t *file_io_pool,
     int entry,
     int flags,
     liberror_error_t **error )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_file_io_pool_reopen";

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( file_io_pool->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file io pool - missing file io handles.",
		 function );

		return( -1 );
	}
	if( ( entry < 0 )
	 || ( entry >= file_io_pool->amount_of_files ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid entry.",
		 function );

		return( -1 );
	}
	file_io_handle = &( file_io_pool->file_io_handle[ entry ] );

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle for entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	if( libewf_file_io_handle_reopen(
	     file_io_handle,
	     flags,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to reopen file io handle for entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	return( 1 );
}

/* Closes a file in the file io pool
 * Returns 0 if successful or -1 on error
 */
int libewf_file_io_pool_close(
     libewf_file_io_pool_t *file_io_pool,
     int entry,
     liberror_error_t **error )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_file_io_pool_close";

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( file_io_pool->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file io pool - missing file io handles.",
		 function );

		return( -1 );
	}
	if( ( entry < 0 )
	 || ( entry >= file_io_pool->amount_of_files ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid entry.",
		 function );

		return( -1 );
	}
	file_io_handle = &( file_io_pool->file_io_handle[ entry ] );

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle for entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	if( libewf_file_io_handle_close(
	     file_io_handle,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file io handle for entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	return( 0 );
}

/* Closes all the files in the file io pool
 * Returns 0 if successful or -1 on error
 */
int libewf_file_io_pool_close_all(
     libewf_file_io_pool_t *file_io_pool,
     liberror_error_t **error )
{
	static char *function = "libewf_file_io_pool_close_all";
	int iterator          = 0;
	int result            = 0;

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < file_io_pool->amount_of_files_io_handles; iterator++ )
	{
		if( libewf_file_io_handle_close(
		     &( file_io_pool->file_io_handle[ iterator ] ),
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file io handle: %d.",
			 function,
			 iterator );

			result = -1;
		}
	}
	return( result );
}

/* Read from a file in the file io pool
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_file_io_pool_read(
         libewf_file_io_pool_t *file_io_pool,
         int entry,
         uint8_t *buffer,
         size_t size,
         liberror_error_t **error )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_file_io_pool_read";
	ssize_t read_count                      = 0;

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( file_io_pool->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file io pool - missing file io handles.",
		 function );

		return( -1 );
	}
	if( ( entry < 0 )
	 || ( entry >= file_io_pool->amount_of_files ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid entry.",
		 function );

		return( -1 );
	}
	file_io_handle = &( file_io_pool->file_io_handle[ entry ] );

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle for entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	/* Make sure the file io handle is open
	 */
	if( ( file_io_handle->file_descriptor == -1 )
	 && ( libewf_file_io_pool_open_file_io_handle(
	       file_io_pool,
	       file_io_handle,
	       file_io_handle->flags,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	read_count = libewf_file_io_handle_read(
	              file_io_handle,
	              buffer,
	              size,
	              error );

	if( read_count < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	return( read_count );
}

/* Writes to a file in the file io pool
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_file_io_pool_write(
         libewf_file_io_pool_t *file_io_pool,
         int entry,
         uint8_t *buffer,
         size_t size,
         liberror_error_t **error )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_file_io_pool_write";
	ssize_t write_count                     = 0;

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( file_io_pool->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file io pool - missing file io handles.",
		 function );

		return( -1 );
	}
	if( ( entry < 0 )
	 || ( entry >= file_io_pool->amount_of_files ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid entry.",
		 function );

		return( -1 );
	}
	file_io_handle = &( file_io_pool->file_io_handle[ entry ] );

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle for entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	/* Make sure the file io handle is open
	 */
	if( ( file_io_handle->file_descriptor == -1 )
	 && ( libewf_file_io_pool_open_file_io_handle(
	       file_io_pool,
	       file_io_handle,
	       file_io_handle->flags,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	write_count = libewf_file_io_handle_write(
	               file_io_handle,
	               buffer,
	               size,
	               error );

	if( write_count < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write to entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	return( write_count );
}

/* Seeks an offset in a file in the file io pool
 * Returns the amount of bytes written or -1 on error
 */
off64_t libewf_file_io_pool_seek_offset(
         libewf_file_io_pool_t *file_io_pool,
         int entry,
         off64_t offset,
         int whence,
         liberror_error_t **error )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_file_io_pool_seek_offset";
	off64_t seek_offset                     = 0;

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( file_io_pool->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file io pool - missing file io handles.",
		 function );

		return( -1 );
	}
	if( ( entry < 0 )
	 || ( entry >= file_io_pool->amount_of_files ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid entry.",
		 function );

		return( -1 );
	}
	file_io_handle = &( file_io_pool->file_io_handle[ entry ] );

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle for entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	/* Make sure the file io handle is open
	 */
	if( ( file_io_handle->file_descriptor == -1 )
	 && ( libewf_file_io_pool_open_file_io_handle(
	       file_io_pool,
	       file_io_handle,
	       file_io_handle->flags,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	seek_offset = libewf_file_io_handle_seek_offset(
	               file_io_handle,
	               offset,
	               whence,
	               error );

	if( seek_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	return( seek_offset );
}

/* Retrieves the current offset in a file in the file io pool
 * Returns the amount of bytes written or -1 on error
 *  */
int libewf_file_io_pool_get_offset(
     libewf_file_io_pool_t *file_io_pool,
     int entry,
     off64_t *offset,
     liberror_error_t **error )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_file_io_pool_get_offset";

	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( file_io_pool->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file io pool - missing file io handles.",
		 function );

		return( -1 );
	}
	if( ( entry < 0 )
	 || ( entry >= file_io_pool->amount_of_files ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid entry.",
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
	file_io_handle = &( file_io_pool->file_io_handle[ entry ] );

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle for entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	/* Make sure the file io handle is open
	 */
	if( ( file_io_handle->file_descriptor == -1 )
	 && ( libewf_file_io_pool_open_file_io_handle(
	       file_io_pool,
	       file_io_handle,
	       file_io_handle->flags,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	*offset = file_io_handle->file_offset;

	return( 1 );
}

