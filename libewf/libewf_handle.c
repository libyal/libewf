/*
 * Handle functions
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libewf_definitions.h"
#include "libewf_handle.h"
#include "libewf_header_values.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libfvalue.h"
#include "libewf_sector_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_file_handle.h"
#include "libewf_single_files.h"
#include "libewf_types.h"
#include "libewf_write_io_handle.h"

#include "ewf_checksum.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_file_header.h"

/* Initialize the handle
 * The handle must point to a NULL pointer to be allocated
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_initialize(
     libewf_handle_t **handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_initialize";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle == NULL )
	{
		internal_handle = memory_allocate_structure(
		                   libewf_internal_handle_t );

		if( internal_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create handle.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     internal_handle,
		     0,
		     sizeof( libewf_internal_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear handle.",
			 function );

			goto on_error;
		}
		if( libewf_io_handle_initialize(
		     &( internal_handle->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create IO handle.",
			 function );

			goto on_error;
		}
		if( libewf_chunk_cache_initialize(
		     &( internal_handle->chunk_cache ),
		     EWF_MINIMUM_CHUNK_SIZE + sizeof( uint32_t ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create chunk cache.",
			 function );

			goto on_error;
		}
		if( libewf_media_values_initialize(
		     &( internal_handle->media_values ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create media values.",
			 function );

			goto on_error;
		}
		if( libewf_sector_list_initialize(
		     &( internal_handle->sessions ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sessions sector list.",
			 function );

			goto on_error;
		}
		if( libewf_sector_list_initialize(
		     &( internal_handle->acquiry_errors ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create acquiry errors sector list.",
			 function );

			goto on_error;
		}
		internal_handle->date_format                    = LIBEWF_DATE_FORMAT_CTIME;
		internal_handle->maximum_number_of_open_handles = LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES;

		*handle = (libewf_handle_t *) internal_handle;
	}
	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		if( internal_handle->sessions != NULL )
		{
			libewf_sector_list_free(
			 &( internal_handle->sessions ),
			 NULL );
		}
		if( internal_handle->media_values != NULL )
		{
			libewf_media_values_free(
			 &( internal_handle->media_values ),
			 NULL );
		}
		if( internal_handle->chunk_cache != NULL )
		{
			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ),
			 NULL );
		}
		if( internal_handle->io_handle != NULL )
		{
			libewf_io_handle_free(
			 &( internal_handle->io_handle ),
			 NULL );
		}
		memory_free(
		 internal_handle );
	}
	return( -1 );
}

/* Frees the handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_free(
     libewf_handle_t **handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_internal_handle_free";
	int result                                = 1;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		internal_handle = (libewf_internal_handle_t *) *handle;

		if( internal_handle->file_io_pool != NULL )
		{
			if( libewf_handle_close(
			     *handle,
			     error ) != 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close handle.",
				 function );

				result = -1;
			}
		}
		*handle = NULL;

		if( libewf_io_handle_free(
		     &( internal_handle->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		if( libewf_chunk_cache_free(
		     &( internal_handle->chunk_cache ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk cache.",
			 function );

			result = -1;
		}
		if( libewf_media_values_free(
		     &( internal_handle->media_values ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free media values.",
			 function );

			result = -1;
		}
		if( libewf_sector_list_free(
		     &( internal_handle->sessions ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sessions sector list.",
			 function );

			result = -1;
		}
		if( libewf_sector_list_free(
		     &( internal_handle->acquiry_errors ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free acquiry errors sector list.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_handle );
	}
	return( result );
}

/* Clones the handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_clone(
     libewf_handle_t **destination_handle,
     libewf_handle_t *source_handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_destination_handle = NULL;
	libewf_internal_handle_t *internal_source_handle      = NULL;
	static char *function                                 = "libewf_handle_clone";

	if( destination_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination handle.",
		 function );

		return( -1 );
	}
	if( *destination_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination handle value already set.",
		 function );

		return( -1 );
	}
	if( source_handle == NULL )
	{
		*destination_handle = NULL;

		return( 1 );
	}
	internal_source_handle = (libewf_internal_handle_t *) *source_handle;

	if( internal_source_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( internal_source_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	internal_destination_handle = memory_allocate_structure(
			               libewf_internal_handle_t );

	if( internal_destination_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_destination_handle,
	     0,
	     sizeof( libewf_internal_handle_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear destination handle.",
		 function );

		goto on_error;
	}
	if( libewf_io_handle_clone(
	     &( internal_destination_handle->io_handle ),
	     internal_source_handle->io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination IO handle.",
		 function );

		goto on_error;
	}
	if( libewf_chunk_cache_initialize(
	     &( internal_destination_handle->chunk_cache ),
	     EWF_MINIMUM_CHUNK_SIZE + sizeof( uint32_t ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk cache.",
		 function );

		goto on_error;
	}
	if( libewf_media_values_clone(
	     &( internal_destination_handle->media_values ),
	     internal_source_handle->media_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination media values.",
		 function );

		goto on_error;
	}
	if( libewf_sector_list_clone(
	     &( internal_destination_handle->sessions ),
	     internal_source_handle->sessions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination sessions.",
		 function );

		goto on_error;
	}
	if( libewf_sector_list_clone(
	     &( internal_destination_handle->acquiry_errors ),
	     internal_source_handle->acquiry_errors,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination acquiry errors.",
		 function );

		goto on_error;
	}
	if( internal_source_handle->file_io_pool != NULL )
	{
		if( libbfio_pool_clone(
		     &( internal_destination_handle->file_io_pool ),
		     internal_source_handle->file_io_pool,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination file IO pool.",
			 function );

			goto on_error;
		}
		internal_destination_handle->file_io_pool_created_in_library = 1;
	}
	if( internal_source_handle->read_io_handle != NULL )
	{
		if( libewf_read_io_handle_clone(
		     &( internal_destination_handle->read_io_handle ),
		     internal_source_handle->read_io_handle,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination read IO handle.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->write_io_handle != NULL )
	{
		if( libewf_write_io_handle_clone(
		     &( internal_destination_handle->write_io_handle ),
		     internal_source_handle->write_io_handle,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination write IO handle.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->segment_table != NULL )
	{
		if( libewf_segment_table_clone(
		     &( internal_destination_handle->segment_table ),
		     internal_source_handle->segment_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination segment table.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->delta_segment_table != NULL )
	{
		if( libewf_segment_table_clone(
		     &( internal_destination_handle->delta_segment_table ),
		     internal_source_handle->delta_segment_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination delta segment table.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->offset_table != NULL )
	{
		if( libewf_offset_table_clone(
		     &( internal_destination_handle->offset_table ),
		     internal_source_handle->offset_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination offset table.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->header_sections != NULL )
	{
/* TODO */
	}
	if( internal_source_handle->hash_sections != NULL )
	{
/* TODO */
	}
	if( internal_source_handle->header_values != NULL )
	{
/* TODO */
		internal_destination_handle->header_values_parsed = internal_source_handle->header_values_parsed;
	}
	if( internal_source_handle->hash_values != NULL )
	{
/* TODO */
		internal_destination_handle->hash_values_parsed = internal_source_handle->hash_values_parsed;
	}
	internal_destination_handle->maximum_number_of_open_handles = internal_source_handle->maximum_number_of_open_handles;
	internal_destination_handle->date_format                    = internal_source_handle->date_format;

	return( 1 );

on_error:
	if( internal_destination_handle != NULL )
	{
/* TODO */
		if( internal_destination_handle->offset_table != NULL )
		{
			libewf_offset_table_free(
			 &( internal_destination_handle->offset_table ),
			 NULL );
		}
		if( internal_destination_handle->delta_segment_table != NULL )
		{
			libewf_segment_table_free(
			 &( internal_destination_handle->delta_segment_table ),
			 NULL );
		}
		if( internal_destination_handle->segment_table != NULL )
		{
			libewf_segment_table_free(
			 &( internal_destination_handle->segment_table ),
			 NULL );
		}
		if( internal_destination_handle->write_io_handle != NULL )
		{
			libewf_write_io_handle_free(
			 &( internal_destination_handle->write_io_handle ),
			 NULL );
		}
		if( internal_destination_handle->read_io_handle != NULL )
		{
			libewf_read_io_handle_free(
			 &( internal_destination_handle->read_io_handle ),
			 NULL );
		}
		if( internal_destination_handle->file_io_pool != NULL )
		{
			libbfio_pool_free(
			 &( internal_destination_handle->file_io_pool ),
			 NULL );
		}
		if( internal_destination_handle->acquiry_errors != NULL )
		{
			libewf_sector_list_free(
			 &( internal_destination_handle->acquiry_errors ),
			 NULL );
		}
		if( internal_destination_handle->sessions != NULL )
		{
			libewf_sector_list_free(
			 &( internal_destination_handle->sessions ),
			 NULL );
		}
		if( internal_destination_handle->media_values != NULL )
		{
			libewf_media_values_free(
			 &( internal_destination_handle->media_values ),
			 NULL );
		}
		if( internal_destination_handle->chunk_cache != NULL )
		{
			libewf_chunk_cache_free(
			 &( internal_destination_handle->chunk_cache ),
			 NULL );
		}
		if( internal_destination_handle->io_handle != NULL )
		{
			libewf_io_handle_free(
			 &( internal_destination_handle->io_handle ),
			 NULL );
		}
		memory_free(
		 internal_destination_handle );
	}
	return( -1 );
}

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_signal_abort(
     libewf_handle_t *handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_signal_abort";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_handle->io_handle->abort = 1;

	return( 1 );
}

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open(
     libewf_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     uint8_t access_flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libbfio_pool_t *file_io_pool              = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	char *first_delta_segment_filename        = NULL;
	char *first_segment_filename              = NULL;
	static char *function                     = "libewf_handle_open";
	size_t filename_length                    = 0;
	int file_io_pool_entry                    = 0;
	int filename_iterator                     = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - segment table already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - delta segment table already set.",
		 function );

		return( -1 );
	}
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
	if( number_of_filenames <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of files zero or less.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     internal_handle->maximum_number_of_open_handles,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO pool.",
		 function );

		goto libewf_handle_open_on_error;
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 || ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		for( filename_iterator = 0;
		     filename_iterator < number_of_filenames;
		     filename_iterator++ )
		{
			filename_length = libcstring_narrow_string_length(
					   filenames[ filename_iterator ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %s is too small.",
				 function,
				 filenames[ filename_iterator ] );

				goto libewf_handle_open_on_error;
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle.",
				 function );

				goto libewf_handle_open_on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                liberror_error_set(
		                 error,
		                 LIBERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle.",
		                 function );

				goto libewf_handle_open_on_error;
			}
#endif
			if( libbfio_file_set_name(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				goto libewf_handle_open_on_error;
			}
			if( libbfio_pool_append_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append file IO handle to pool.",
				 function );

				goto libewf_handle_open_on_error;
			}
			file_io_handle = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: added pool entry: %d with filename: %s.\n",
				 function,
				 file_io_pool_entry,
				 filenames[ filename_iterator ] );
			}
#endif
			if( ( filenames[ filename_iterator ][ filename_length - 3 ] == 'e' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'E' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'l' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'L' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 's' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'S' ) )
			{
				if( ( filenames[ filename_iterator ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_iterator ][ filename_length - 1 ] == '1' ) )
				{
					first_segment_filename = filenames[ filename_iterator ];

					if( first_delta_segment_filename == NULL )
					{
						first_delta_segment_filename = filenames[ filename_iterator ];
					}
				}
			}
			else if( ( filenames[ filename_iterator ][ filename_length - 3 ] == 'd' )
			      || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'D' ) )
			{
				if( ( filenames[ filename_iterator ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_iterator ][ filename_length - 1 ] == '1' ) )
				{
					first_delta_segment_filename = filenames[ filename_iterator ];
				}
			}
		}
	}
	/* The segment table is initially filled with a single entry
	 */
	if( libewf_segment_table_initialize(
	     &( internal_handle->segment_table ),
	     1,
	     LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment table.",
		 function );

		goto libewf_handle_open_on_error;
	}
	/* The delta segment table is initially filled with a single entry
	 */
	if( libewf_segment_table_initialize(
	     &( internal_handle->delta_segment_table ),
	     1,
	     INT64_MAX,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create delta segment table.",
		 function );

		goto libewf_handle_open_on_error;
	}
	if( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = libcstring_narrow_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				goto libewf_handle_open_on_error;
			}
		}
		/* Get the basename of the first delta segment file
		 */
		if( first_delta_segment_filename != NULL )
		{
			filename_length = libcstring_narrow_string_length(
					   first_delta_segment_filename );

			/* Set delta segment table basename
			 */
			if( libewf_segment_table_set_basename(
			     internal_handle->delta_segment_table,
			     first_delta_segment_filename,
			     filename_length - 4,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in delta segment table.",
				 function );

				goto libewf_handle_open_on_error;
			}
		}
	}
	else if( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = libcstring_narrow_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				goto libewf_handle_open_on_error;
			}
		}
	}
	else if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	{
		/* Get the basename and store it in the segment tables
		 */
		filename_length = libcstring_narrow_string_length(
				   filenames[ 0 ] );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename(
		     internal_handle->segment_table,
		     filenames[ 0 ],
		     filename_length,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.",
			 function );

			goto libewf_handle_open_on_error;
		}
	}
	if( libewf_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO pool.",
		 function );

		goto libewf_handle_open_on_error;
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );

libewf_handle_open_on_error:
	if( internal_handle->delta_segment_table != NULL )
	{
		libewf_segment_table_free(
		 &( internal_handle->delta_segment_table ),
		 NULL );
	}
	if( internal_handle->segment_table != NULL )
	{
		libewf_segment_table_free(
		 &( internal_handle->segment_table ),
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open_wide(
     libewf_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     uint8_t access_flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libbfio_pool_t *file_io_pool              = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	wchar_t *first_delta_segment_filename     = NULL;
	wchar_t *first_segment_filename           = NULL;
	static char *function                     = "libewf_handle_open_wide";
	size_t filename_length                    = 0;
	int file_io_pool_entry                    = 0;
	int filename_iterator                     = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - segment table already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - delta segment table already set.",
		 function );

		return( -1 );
	}
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
	if( number_of_filenames <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of files zero or less.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     internal_handle->maximum_number_of_open_handles,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO pool.",
		 function );

		goto libewf_handle_open_wide_on_error;
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 || ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		for( filename_iterator = 0;
		     filename_iterator < number_of_filenames;
		     filename_iterator++ )
		{
			filename_length = libcstring_wide_string_length(
					   filenames[ filename_iterator ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %ls is too small.",
				 function,
				 filenames[ filename_iterator ] );

				goto libewf_handle_open_wide_on_error;
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle.",
				 function );

				goto libewf_handle_open_wide_on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                liberror_error_set(
		                 error,
		                 LIBERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle.",
		                 function );

				goto libewf_handle_open_wide_on_error;
			}
#endif
			if( libbfio_file_set_name_wide(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				goto libewf_handle_open_wide_on_error;
			}
			if( libbfio_pool_append_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append file IO handle to pool.",
				 function );

				goto libewf_handle_open_wide_on_error;
			}
			file_io_handle = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: added pool entry: %d with filename: %ls.\n",
				 function,
				 file_io_pool_entry,
				 filenames[ filename_iterator ] );
			}
#endif
			if( ( filenames[ filename_iterator ][ filename_length - 3 ] == 'e' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'E' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'l' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'L' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 's' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'S' ) )
			{
				if( ( filenames[ filename_iterator ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_iterator ][ filename_length - 1 ] == '1' ) )
				{
					first_segment_filename = filenames[ filename_iterator ];

					if( first_delta_segment_filename == NULL )
					{
						first_delta_segment_filename = filenames[ filename_iterator ];
					}
				}
			}
			else if( ( filenames[ filename_iterator ][ filename_length - 3 ] == 'd' )
			      || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'D' ) )
			{
				if( ( filenames[ filename_iterator ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_iterator ][ filename_length - 1 ] == '1' ) )
				{
					first_delta_segment_filename = filenames[ filename_iterator ];
				}
			}
		}
	}
	/* The segment table is initially filled with a single entry
	 */
	if( libewf_segment_table_initialize(
	     &( internal_handle->segment_table ),
	     1,
	     LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment table.",
		 function );

		goto libewf_handle_open_wide_on_error;
	}
	/* The delta segment table is initially filled with a single entry
	 */
	if( libewf_segment_table_initialize(
	     &( internal_handle->delta_segment_table ),
	     1,
	     INT64_MAX,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create delta segment table.",
		 function );

		goto libewf_handle_open_wide_on_error;
	}
	if( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = libcstring_wide_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename_wide(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				goto libewf_handle_open_wide_on_error;
			}
		}
		/* Get the basename of the first delta segment file
		 */
		if( first_delta_segment_filename != NULL )
		{
			filename_length = libcstring_wide_string_length(
					   first_delta_segment_filename );

			/* Set delta segment table basename
			 */
			if( libewf_segment_table_set_basename_wide(
			     internal_handle->delta_segment_table,
			     first_delta_segment_filename,
			     filename_length - 4,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in delta segment table.",
				 function );

				goto libewf_handle_open_wide_on_error;
			}
		}
	}
	else if( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = libcstring_wide_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename_wide(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				goto libewf_handle_open_wide_on_error;
			}
		}
	}
	else if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	{
		/* Get the basename and store it in the segment tables
		 */
		filename_length = libcstring_wide_string_length(
				   filenames[ 0 ] );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename_wide(
		     internal_handle->segment_table,
		     filenames[ 0 ],
		     filename_length,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.",
			 function );

			goto libewf_handle_open_wide_on_error;
		}
	}
	if( libewf_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO pool.",
		 function );

		goto libewf_handle_open_wide_on_error;
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );

libewf_handle_open_wide_on_error:
	if( internal_handle->delta_segment_table != NULL )
	{
		libewf_segment_table_free(
		 &( internal_handle->delta_segment_table ),
		 NULL );
	}
	if( internal_handle->segment_table != NULL )
	{
		libewf_segment_table_free(
		 &( internal_handle->segment_table ),
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	return( -1 );
}
#endif

/* Opens a set of EWF file(s) using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     uint8_t access_flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle                  = NULL;
	libewf_internal_handle_t *internal_handle         = NULL;
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_handle_open_file_io_pool";
	uint16_t segment_number                           = 0;
	int number_of_file_io_handles                     = 0;
	int number_of_segment_file_handles                = 0;
	int file_io_handle_iterator                       = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->file_io_pool != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - file IO pool already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - offset table already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->header_sections != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - header sections already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_sections != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - hash sections already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->single_files != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - single files already set.",
		 function );

		return( -1 );
	}
	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_number_of_handles(
	     file_io_pool,
	     &number_of_file_io_handles,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of handles in the file IO pool.",
		 function );

		return( -1 );
	}
	if( ( access_flags & ~( LIBEWF_ACCESS_FLAG_READ | LIBEWF_ACCESS_FLAG_WRITE | LIBEWF_ACCESS_FLAG_RESUME ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 || ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		if( libewf_read_io_handle_initialize(
		     &( internal_handle->read_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create read IO handle.",
			 function );

			goto libewf_handle_open_file_io_pool_on_error;
		}
	}
	if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	{
		if( libewf_write_io_handle_initialize(
		     &( internal_handle->write_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create write IO handle.",
			 function );

			goto libewf_handle_open_file_io_pool_on_error;
		}
	}
	internal_handle->io_handle->access_flags = access_flags;
	internal_handle->file_io_pool            = file_io_pool;

	if( libewf_offset_table_initialize(
	     &( internal_handle->offset_table ),
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create offset table.",
		 function );

		goto libewf_handle_open_file_io_pool_on_error;
	}
	if( libewf_header_sections_initialize(
	     &( internal_handle->header_sections ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header sections.",
		 function );

		goto libewf_handle_open_file_io_pool_on_error;
	}
	if( libewf_hash_sections_initialize(
	     &( internal_handle->hash_sections ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash sections.",
		 function );

		goto libewf_handle_open_file_io_pool_on_error;
	}
	if( libewf_single_files_initialize(
	     &( internal_handle->single_files ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create single files.",
		 function );

		goto libewf_handle_open_file_io_pool_on_error;
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 || ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		for( file_io_handle_iterator = 0;
		     file_io_handle_iterator < number_of_file_io_handles;
		     file_io_handle_iterator++ )
		{
			file_io_handle = NULL;

			if( libbfio_pool_get_handle(
			     file_io_pool,
			     file_io_handle_iterator,
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to retrieve file IO handle from pool entry: %d.",
				 function,
				 file_io_handle_iterator );

				goto libewf_handle_open_file_io_pool_on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: processing pool entry: %d.\n",
				 function,
				 file_io_handle_iterator );
			}
#endif
			if( libewf_segment_file_handle_initialize(
			     &segment_file_handle,
			     file_io_handle_iterator,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create segment file handle.",
				 function );

				goto libewf_handle_open_file_io_pool_on_error;
			}
			if( libewf_segment_file_read_file_header(
			     segment_file_handle,
			     &segment_number,
			     file_io_pool,
			     error ) <= -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read segment file header.",
				 function );

				libewf_segment_file_handle_free(
				 (intptr_t *) segment_file_handle,
				 NULL );

				goto libewf_handle_open_file_io_pool_on_error;
			}
			if( segment_number == 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_INPUT,
				 LIBERROR_INPUT_ERROR_INVALID_DATA,
				 "%s: invalid segment number: 0.",
				 function );

				libewf_segment_file_handle_free(
				 (intptr_t *) segment_file_handle,
				 NULL );

				goto libewf_handle_open_file_io_pool_on_error;
			}
			if( ( segment_file_handle->file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
			 || ( segment_file_handle->file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF ) )
			{
				if( libewf_segment_table_set_handle(
				     internal_handle->segment_table,
				     (int) segment_number,
				     segment_file_handle,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set handle: %" PRIu16 " in segment table.",
					 function,
					 segment_number );

					libewf_segment_file_handle_free(
					 (intptr_t *) segment_file_handle,
					 NULL );

					goto libewf_handle_open_file_io_pool_on_error;
				}
				segment_file_handle = NULL;
			}
			else if( segment_file_handle->file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
			{
				if( libewf_segment_table_set_handle(
				     internal_handle->delta_segment_table,
				     (int) segment_number,
				     segment_file_handle,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set handle: %" PRIu16 " in delta segment table.",
					 function,
					 segment_number );

					libewf_segment_file_handle_free(
					 (intptr_t *) segment_file_handle,
					 NULL );

					goto libewf_handle_open_file_io_pool_on_error;
				}
				segment_file_handle = NULL;

				/* Re-open the delta segment file with write access
				 */
				if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
				{
					if( libbfio_pool_reopen(
					     internal_handle->file_io_pool,
					     segment_file_handle->file_io_pool_entry,
					     LIBBFIO_OPEN_READ_WRITE,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_OPEN_FAILED,
						 "%s: unable to reopen delta segment file: %" PRIu16 ".",
						 function,
						 segment_number );

						goto libewf_handle_open_file_io_pool_on_error;
					}
				}
			}
			else
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported segment file type.",
				 function );

				libewf_segment_file_handle_free(
				 (intptr_t *) segment_file_handle,
				 NULL );

				goto libewf_handle_open_file_io_pool_on_error;
			}
			if( (int) segment_number > number_of_file_io_handles )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_INPUT,
				 LIBERROR_INPUT_ERROR_INVALID_DATA,
				 "%s: invalid segment number: %" PRIu16 " value out of bounds or missing segment files.",
				 function,
				 segment_number );

				goto libewf_handle_open_file_io_pool_on_error;
			}
		}
		if( libewf_handle_open_read(
		     internal_handle,
		     internal_handle->segment_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read segment table.",
			 function );

			if( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 )
			{
				goto libewf_handle_open_file_io_pool_on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			libnotify_print_error_backtrace(
			 *error );
#endif
			liberror_error_free(
			 error );
		}
		if( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 )
		{
			if( libewf_segment_table_get_number_of_handles(
			     internal_handle->delta_segment_table,
			     &number_of_segment_file_handles,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of delta segment file handles.",
				 function );

				goto libewf_handle_open_file_io_pool_on_error;
			}
			if( number_of_segment_file_handles > 1 )
			{
				if( libewf_handle_open_read(
				     internal_handle,
				     internal_handle->delta_segment_table,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read delta segment table.",
					 function );

					goto libewf_handle_open_file_io_pool_on_error;
				}
			}
		}
		/* Determine the EWF format
		 */
		if( libewf_header_sections_determine_format(
		     internal_handle->header_sections,
		     internal_handle->io_handle->ewf_format,
		     &( internal_handle->io_handle->format ),
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine format.",
			 function );

			goto libewf_handle_open_file_io_pool_on_error;
		}
		if( internal_handle->media_values == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing media values.",
			 function );

			goto libewf_handle_open_file_io_pool_on_error;
		}
		if( internal_handle->single_files->ltree_data != NULL )
		{
			if( libewf_single_files_parse(
			     internal_handle->single_files,
			     &( internal_handle->media_values->media_size ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to parse single files.",
				 function );

				goto libewf_handle_open_file_io_pool_on_error;
			}
		}
		else
		{
			if( libewf_internal_handle_get_media_values(
			     internal_handle,
			     &( internal_handle->media_values->media_size ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine media values.",
				 function );

				goto libewf_handle_open_file_io_pool_on_error;
			}
		}
	}
	/* Make sure format specific values are set
	 */
	if( internal_handle->io_handle->format != 0 )
	{
		if( libewf_internal_handle_set_format(
		     internal_handle,
		     internal_handle->io_handle->format,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set format.",
			 function );

			goto libewf_handle_open_file_io_pool_on_error;
		}
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	 && ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		if( internal_handle->write_io_handle->values_initialized == 0 )
		{
			if( libewf_write_io_handle_initialize_values(
			     internal_handle->write_io_handle,
			     internal_handle->io_handle,
			     internal_handle->media_values,
			     internal_handle->segment_table,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize write IO handle values.",
				 function );

				goto libewf_handle_open_file_io_pool_on_error;
			}
		}
		if( libewf_write_io_handle_initialize_resume(
		     internal_handle->write_io_handle,
		     internal_handle->io_handle,
		     internal_handle->file_io_pool,
		     internal_handle->media_values,
		     internal_handle->offset_table,
		     internal_handle->segment_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write IO handle to resume.",
			 function );

			goto libewf_handle_open_file_io_pool_on_error;
		}
	}
	return( 1 );

libewf_handle_open_file_io_pool_on_error:
	if( internal_handle->single_files != NULL )
	{
		libewf_single_files_free(
		 &( internal_handle->single_files ),
		 NULL );
	}
	if( internal_handle->hash_sections != NULL )
	{
		libewf_hash_sections_free(
		 &( internal_handle->hash_sections ),
		 NULL );
	}
	if( internal_handle->header_sections != NULL )
	{
		libewf_header_sections_free(
		 &( internal_handle->header_sections ),
		 NULL );
	}
	if( internal_handle->offset_table != NULL )
	{
		libewf_offset_table_free(
		 &( internal_handle->offset_table ),
		 NULL );
	}
	internal_handle->file_io_pool = NULL;

	if( internal_handle->write_io_handle != NULL )
	{
		libewf_write_io_handle_free(
		 &( internal_handle->write_io_handle ),
		 error );
	}
	if( internal_handle->read_io_handle != NULL )
	{
		libewf_read_io_handle_free(
		 &( internal_handle->read_io_handle ),
		 error );
	}
	return( -1 );
}

/* Closes the EWF handle
 * Returns 0 if successful or -1 on error
 */
int libewf_handle_close(
     libewf_handle_t *handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_close";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->write_io_handle != NULL )
	 && ( internal_handle->write_io_handle->write_finalized == 0 ) )
	{
		if( libewf_write_io_handle_finalize(
		     internal_handle->write_io_handle,
		     internal_handle->io_handle,
		     internal_handle->file_io_pool,
		     internal_handle->media_values,
		     internal_handle->offset_table,
		     internal_handle->segment_table,
		     &( internal_handle->header_values ),
		     internal_handle->hash_values,
		     internal_handle->header_sections,
		     internal_handle->hash_sections,
		     internal_handle->sessions,
		     internal_handle->acquiry_errors,
		     internal_handle->chunk_cache,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to finalize write.",
			 function );

			return( -1 );
		}
	}
	if( internal_handle->file_io_pool_created_in_library != 0 )
	{
		if( libbfio_pool_close_all(
		     internal_handle->file_io_pool,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close all file IO pool handles.",
			 function );

			result = -1;
		}
		if( libbfio_pool_free(
		     &( internal_handle->file_io_pool ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO pool.",
			 function );

			result = -1;
		}
	}
	internal_handle->file_io_pool                    = NULL;
	internal_handle->file_io_pool_created_in_library = 0;

	if( internal_handle->read_io_handle != NULL )
	{
		if( libewf_read_io_handle_free(
		     &( internal_handle->read_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read IO handle.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->write_io_handle != NULL )
	{
		if( libewf_write_io_handle_free(
		     &( internal_handle->write_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free write IO handle.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->segment_table != NULL )
	{
		if( libewf_segment_table_free(
		     &( internal_handle->segment_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment table.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->delta_segment_table != NULL )
	{
		if( libewf_segment_table_free(
		     &( internal_handle->delta_segment_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free delta segment table.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->offset_table != NULL )
	{
		if( libewf_offset_table_free(
		     &( internal_handle->offset_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free offset table.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->header_sections != NULL )
	{
		if( libewf_header_sections_free(
		     &( internal_handle->header_sections ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free header sections.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->hash_sections != NULL )
	{
		if( libewf_hash_sections_free(
		     &( internal_handle->hash_sections ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free hash sections.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->header_values != NULL )
	{
		if( libfvalue_table_free(
		     (intptr_t *) internal_handle->header_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free header values.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->hash_values != NULL )
	{
		if( libfvalue_table_free(
		     (intptr_t *) internal_handle->hash_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free hash values.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->single_files != NULL )
	{
		if( libewf_single_files_free(
		     &( internal_handle->single_files ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free single files.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->read_io_handle != NULL )
	{
		if( libewf_sector_list_empty(
		     internal_handle->read_io_handle->checksum_errors,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty checksum errors sector list.",
			 function );

			result = -1;
		}
	}
	if( libewf_sector_list_empty(
	     internal_handle->sessions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty sessions sector list.",
		 function );

		result = -1;
	}
	if( libewf_sector_list_empty(
	     internal_handle->acquiry_errors,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty acquiry errors sector list.",
		 function );

		result = -1;
	}
	return( result );
}

/* Opens the EWF handle for reading
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open_read(
     libewf_internal_handle_t *internal_handle,
     libewf_segment_table_t *segment_table,
     liberror_error_t **error )
{
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_handle_open_read";
	int number_of_segment_file_handles                = 0;
	int last_segment_file                             = 0;
	int result                                        = 0;
	int segment_number                                = 0;

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->read_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing read IO handle.",
		 function );

		return( -1 );
	}
	if( libewf_sector_list_empty(
	     internal_handle->read_io_handle->checksum_errors,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty checksum errors sector list.",
		 function );

		return( -1 );
	}
	if( libewf_sector_list_empty(
	     internal_handle->sessions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty sessions sector list.",
		 function );

		return( -1 );
	}
	if( libewf_sector_list_empty(
	     internal_handle->acquiry_errors,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty acquiry errors sector list.",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_get_number_of_handles(
	     segment_table,
	     &number_of_segment_file_handles,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segment file handles.",
		 function );

		return( -1 );
	}
	/* If there is more than one segment file, use the size of the first as the maximum segment size
	 */
	if( number_of_segment_file_handles > 2 )
	{
		if( libewf_segment_table_get_handle(
		     segment_table,
		     1,
		     &segment_file_handle,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve handle: 1 from segment table.",
			 function );

			return( -1 );
		}
		if( segment_file_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid segment file handle - value missing.",
			 function );

			return( -1 );
		}
		if( libbfio_pool_get_size(
		     internal_handle->file_io_pool,
		     segment_file_handle->file_io_pool_entry,
		     &( segment_table->maximum_segment_size ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file size.",
			 function );

			return( -1 );
		}
		/* Round the maximum segment size to nearest number of KiB
		 */
		if( libewf_segment_table_set_maximum_segment_size(
		     segment_table,
		     ( segment_table->maximum_segment_size / 1024 ) * 1024,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set maximum segment size.",
			 function );

			return( -1 );
		}
	}
	/* Read the segment and offset table from the segment file(s)
	 */
	for( segment_number = 1;
	     segment_number < number_of_segment_file_handles;
	     segment_number++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: reading section list for segment number: %d.\n",
			 function,
			 segment_number );
		}
#endif
		segment_file_handle = NULL;

		if( libewf_segment_table_get_handle(
		     segment_table,
		     segment_number,
		     &segment_file_handle,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve handle: %d from segment table.",
			 function,
			 segment_number );

			return( -1 );
		}
		result = libewf_segment_file_read_sections(
		          segment_file_handle,
		          internal_handle->io_handle,
		          internal_handle->file_io_pool,
		          &last_segment_file,
		          internal_handle->header_sections,
		          internal_handle->hash_sections,
		          internal_handle->media_values,
		          internal_handle->offset_table,
		          internal_handle->sessions,
		          internal_handle->acquiry_errors,
		          internal_handle->single_files,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sections.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing next or done section.",
			 function );

			return( 0 );
		}
		if( internal_handle->io_handle->abort == 1 )
		{
			return( -1 );
		}
	}
	/* Check to see if the done section has been found in the last segment file
	 */
	if( last_segment_file != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to find the last segment file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Prepares chunk of (media) data after reading it according to the handle settings
 * This function should be used after libewf_handle_read_chunk
 * The chunk_buffer_size should contain the actual chunk size
 * Returns the resulting chunk size or -1 on error
 */
ssize_t libewf_handle_prepare_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         void *uncompressed_buffer,
         size_t *uncompressed_buffer_size,
         int8_t is_compressed,
         uint32_t chunk_checksum,
         int8_t read_checksum,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_prepare_read_chunk";
	ssize_t chunk_data_size                   = 0;
	uint8_t checksum_mismatch                 = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer == internal_handle->chunk_cache->data )
	 || ( chunk_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( ( uncompressed_buffer == internal_handle->chunk_cache->data )
	 || ( uncompressed_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( chunk_buffer_size == 0 )
	{
		return( 0 );
	}
	chunk_data_size = libewf_read_io_handle_process_chunk(
	                   (uint8_t *) chunk_buffer,
	                   chunk_buffer_size,
	                   (uint8_t *) uncompressed_buffer,
	                   uncompressed_buffer_size,
	                   is_compressed,
	                   chunk_checksum,
	                   read_checksum,
	                   &checksum_mismatch,
	                   error );

	if( chunk_data_size <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_INPUT_FAILED,
		 "%s: unable to prepare chunk data.",
		 function );

		return( -1 );
	}
	if( checksum_mismatch != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum mismatch for chunk data.",
		 function );

		return( -1 );
	}
	return( chunk_data_size );
}

/* Reads a chunk of (media) data from the current offset into a buffer
 * size contains the size of the chunk buffer
 * The function sets the chunk checksum, is compressed and read checksum values
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_handle_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         int8_t *is_compressed,
         void *checksum_buffer,
         uint32_t *chunk_checksum,
         int8_t *read_checksum,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_read_chunk";
	ssize_t read_count                        = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer == internal_handle->chunk_cache->data )
	 || ( chunk_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( chunk_buffer_size == 0 )
	{
		return( 0 );
	}
	read_count = libewf_read_io_handle_read_chunk(
	              internal_handle->io_handle,
	              internal_handle->file_io_pool,
	              internal_handle->offset_table,
	              internal_handle->io_handle->current_chunk,
	              (uint8_t *) chunk_buffer,
	              chunk_buffer_size,
	              is_compressed,
	              (uint8_t *) checksum_buffer,
	              chunk_checksum,
	              read_checksum,
	              error );

	if( read_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk.",
		 function );

		return( -1 );
	}
	else if( read_count > 0 )
	{
		internal_handle->io_handle->current_offset += (off64_t) read_count;
		internal_handle->io_handle->current_chunk  += 1;
	}
	return( read_count );
}

/* Reads (media) data from the last current into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_handle_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_read_buffer";
	ssize_t chunk_read_count                  = 0;
	ssize_t total_read_count                  = 0;
	size_t chunk_data_size                    = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->current_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: reading size: %" PRIzu ".\n",
		 function,
		 buffer_size );
	}
#endif
	if( buffer_size == 0 )
	{
		return( 0 );
	}
	/* Do not read beyond the media size
	 */
	if( ( internal_handle->media_values->media_size > 0 )
	 && ( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->media_values->media_size ) )
	{
		return( 0 );
	}
	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents some reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media_values->chunk_size + sizeof( uint32_t );

	if( chunk_data_size > internal_handle->chunk_cache->size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: reallocating chunk data size: %" PRIzu ".\n",
			 function,
			 chunk_data_size );
		}
#endif
		if( libewf_chunk_cache_resize(
		     internal_handle->chunk_cache,
		     chunk_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk cache.",
			 function );

			return( -1 );
		}
	}
	while( buffer_size > 0 )
	{
		chunk_read_count = libewf_read_io_handle_read_chunk_data(
		                    internal_handle->read_io_handle,
		                    internal_handle->io_handle,
		                    internal_handle->file_io_pool,
		                    internal_handle->media_values,
		                    internal_handle->offset_table,
		                    internal_handle->chunk_cache,
		                    internal_handle->io_handle->current_chunk,
		                    internal_handle->io_handle->current_chunk_offset,
		                    (uint8_t *) &( (uint8_t *) buffer )[ total_read_count ],
		                    buffer_size,
		                    error );

		if( chunk_read_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data from chunk.",
			 function );

			return( -1 );
		}
		else if( chunk_read_count == 0 )
		{
			break;
		}
		buffer_size      -= chunk_read_count;
		total_read_count += chunk_read_count;

		internal_handle->io_handle->current_offset       += (off64_t) chunk_read_count;
		internal_handle->io_handle->current_chunk_offset += (uint32_t) chunk_read_count;

		if( internal_handle->io_handle->current_chunk_offset == internal_handle->media_values->chunk_size )
		{
			internal_handle->io_handle->current_chunk_offset = 0;
			internal_handle->io_handle->current_chunk       += 1;
		}
		else if( internal_handle->io_handle->current_chunk_offset > internal_handle->media_values->chunk_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid current chunk offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( internal_handle->io_handle->abort == 1 )
		{
			break;
		}
	}
	return( total_read_count );
}

/* Reads (media) data at the current offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_handle_read_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "libewf_handle_read_random";
	ssize_t read_count    = 0;

	if( libewf_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libewf_handle_read_buffer(
	              handle,
	              buffer,
	              buffer_size,
	              error );

	if( read_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Prepares a chunk of (media) data before writing according to the handle settings
 * This function should be used before libewf_handle_write_chunk
 * The chunk_buffer_size should contain the actual chunk size
 * The function sets the chunk checksum, is compressed and write checksum values
 * Returns the resulting chunk size or -1 on error
 */
ssize_t libewf_handle_prepare_write_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         void *compressed_buffer,
         size_t *compressed_buffer_size,
         int8_t *is_compressed,
         uint32_t *chunk_checksum,
         int8_t *write_checksum,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_prepare_write_chunk";
	ssize_t chunk_data_size                   = 0;
	int chunk_exists                          = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer == internal_handle->chunk_cache->data )
	 || ( chunk_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( ( compressed_buffer == internal_handle->chunk_cache->data )
	 || ( compressed_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( chunk_buffer_size == 0 )
	{
		return( 0 );
	}
	if( internal_handle->read_io_handle != NULL )
	{
		/* Check if chunk has already been created within a segment file
		 */
		chunk_exists = libewf_offset_table_chunk_exists(
		                internal_handle->offset_table,
		                internal_handle->io_handle->current_chunk,
		                error );

		if( chunk_exists == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if the chunk: %" PRIu32 " exists in the offset table.",
			 function,
			 internal_handle->io_handle->current_chunk );

			return( -1 );
		}
		else if( chunk_exists == 0 )
		{
			if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
			 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: chunk: %" PRIu32 " does not exist.",
				 function,
				 internal_handle->io_handle->current_chunk );

				return( -1 );
			}
		}
	}
	chunk_data_size = libewf_write_io_handle_process_chunk(
	                   internal_handle->chunk_cache,
	                   internal_handle->media_values,
	                   internal_handle->io_handle->compression_level,
	                   internal_handle->io_handle->compression_flags,
	                   internal_handle->io_handle->ewf_format,
	                   (uint8_t *) chunk_buffer,
	                   chunk_buffer_size,
	                   (uint8_t *) compressed_buffer,
	                   compressed_buffer_size,
	                   is_compressed,
	                   (uint8_t) chunk_exists,
	                   chunk_checksum,
	                   write_checksum,
	                   error );

	if( chunk_data_size <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_OUTPUT_FAILED,
		 "%s: unable to prepare chunk.",
		 function );

		return( -1 );
	}
	return( chunk_data_size );
}

/* Writes a chunk of (media) data in EWF format at the current offset
 * the necessary settings of the write values must have been made
 * size contains the size of the data within the buffer while
 * data size contains the size of the actual input data
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_handle_write_chunk(
         libewf_handle_t *handle,
         const void *chunk_buffer,
         size_t chunk_buffer_size,
         size_t data_size,
         int8_t is_compressed,
         void *checksum_buffer,
         uint32_t chunk_checksum,
         int8_t write_checksum,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_chunk";
	ssize_t write_count                       = 0;
	int chunk_exists                          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t number_of_chunk_values           = 0;
#endif

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_io_handle->values_initialized == 0 )
	{
		if( libewf_write_io_handle_initialize_values(
		     internal_handle->write_io_handle,
		     internal_handle->io_handle,
		     internal_handle->media_values,
		     internal_handle->segment_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write IO handle values.",
			 function );

			return( -1 );
		}
	}
	if( chunk_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer == internal_handle->chunk_cache->data )
	 || ( chunk_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( data_size > internal_handle->media_values->chunk_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: data size cannot be larger than maximum chunk size.",
		 function );

		return( -1 );
	}
	if( chunk_buffer_size == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libewf_offset_table_get_number_of_chunk_values(
		     internal_handle->offset_table,
		     &number_of_chunk_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the number of chunk values in the offset table.",
			 function );

			return( -1 );
		}
		libnotify_printf(
		 "%s: writing chunk: %" PRIu32 " of total: %" PRIu32 ".\n",
		 function,
		 internal_handle->io_handle->current_chunk,
		 number_of_chunk_values );
		libnotify_printf(
		 "%s: writing chunk buffer of size: %" PRIzd " with data of size: %" PRIzd ".\n",
		 function,
		 chunk_buffer_size,
		 data_size );
	}
#endif
	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		/* Check if chunk has already been created within a segment file
		 */
		chunk_exists = libewf_offset_table_chunk_exists(
		                internal_handle->offset_table,
		                internal_handle->io_handle->current_chunk,
		                error );

		if( chunk_exists == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if the chunk: %" PRIu32 " exists in the offset table.",
			 function,
			 internal_handle->io_handle->current_chunk );

			return( -1 );
		}
		else if( chunk_exists == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: chunk: %" PRIu32 " does not exist.",
			 function,
			 internal_handle->io_handle->current_chunk );

			return( -1 );
		}
		write_count = libewf_write_io_handle_write_existing_chunk(
		               internal_handle->write_io_handle,
		               internal_handle->io_handle,
		               internal_handle->file_io_pool,
		               internal_handle->media_values,
		               internal_handle->offset_table,
		               internal_handle->delta_segment_table,
		               internal_handle->header_sections,
		               internal_handle->io_handle->current_chunk,
		               (uint8_t *) chunk_buffer,
		               chunk_buffer_size,
		               data_size,
		               is_compressed,
		               (uint8_t *) checksum_buffer,
		               chunk_checksum,
		               write_checksum,
		               error );
	}
	else
	{
		write_count = libewf_write_io_handle_write_new_chunk(
		               internal_handle->write_io_handle,
		               internal_handle->io_handle,
		               internal_handle->file_io_pool,
		               internal_handle->media_values,
		               internal_handle->offset_table,
		               internal_handle->segment_table,
		               &( internal_handle->header_values ),
		               internal_handle->hash_values,
		               internal_handle->header_sections,
		               internal_handle->hash_sections,
		               internal_handle->sessions,
		               internal_handle->acquiry_errors,
		               internal_handle->io_handle->current_chunk,
		               (uint8_t *) chunk_buffer,
		               chunk_buffer_size,
		               data_size,
		               is_compressed,
		               (uint8_t *) checksum_buffer,
		               chunk_checksum,
		               write_checksum,
		               error );
	}
	if( write_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write raw chunk data.",
		 function );

		return( -1 );
	}
	else if( write_count > 0 )
	{
		internal_handle->io_handle->current_offset += (off64_t) chunk_buffer_size;
		internal_handle->io_handle->current_chunk  += 1;
	}
	return( (ssize_t) chunk_buffer_size );
}

/* Writes (media) data in EWF format at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_handle_write_buffer(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_buffer";
	ssize_t total_write_count                 = 0;
	ssize_t write_count                       = 0;
	size_t chunk_data_size                    = 0;
	int chunk_exists                          = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write_io_handle->values_initialized == 0 )
	 && ( libewf_write_io_handle_initialize_values(
	       internal_handle->write_io_handle,
	       internal_handle->io_handle,
	       internal_handle->media_values,
	       internal_handle->segment_table,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize write IO handle values.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->current_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( buffer_size == 0 )
	{
		return( 0 );
	}
	/* Do not write beyond the media size
	 */
	if( ( internal_handle->media_values->media_size > 0 )
	 && ( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->media_values->media_size ) )
	{
		return( 0 );
	}
	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents multiple reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media_values->chunk_size + sizeof( uint32_t );

	if( buffer == internal_handle->chunk_cache->compressed )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: chunk cache compressed cannot be used as buffer.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > internal_handle->chunk_cache->size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: reallocating chunk data size: %" PRIzu ".\n",
			 function,
			 chunk_data_size );
		}
#endif
		if( libewf_chunk_cache_resize(
		     internal_handle->chunk_cache,
		     chunk_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk cache.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_handle->media_values->media_size > 0 )
	 && ( (size64_t) ( internal_handle->io_handle->current_offset + buffer_size ) >= internal_handle->media_values->media_size ) )
	{
		buffer_size = (size_t) ( internal_handle->media_values->media_size - internal_handle->io_handle->current_offset );
	}
	while( buffer_size > 0 )
	{
		if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
		 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
		{
			/* Check if chunk has already been created within a segment file
			 */
			chunk_exists = libewf_offset_table_chunk_exists(
					internal_handle->offset_table,
					internal_handle->io_handle->current_chunk,
					error );

			if( chunk_exists == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if the chunk: %" PRIu32 " exists in the offset table.",
				 function,
				 internal_handle->io_handle->current_chunk );

				return( -1 );
			}
			else if( chunk_exists == 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: chunk: %" PRIu32 " does not exist.",
				 function,
				 internal_handle->io_handle->current_chunk );

				return( -1 );
			}
			write_count = libewf_write_io_handle_write_existing_chunk_data(
			               internal_handle->write_io_handle,
			               internal_handle->read_io_handle,
			               internal_handle->io_handle,
			               internal_handle->file_io_pool,
			               internal_handle->media_values,
			               internal_handle->offset_table,
			               internal_handle->delta_segment_table,
			               internal_handle->header_sections,
			               internal_handle->chunk_cache,
			               internal_handle->io_handle->current_chunk,
			               internal_handle->io_handle->current_chunk_offset,
			               (void *) &( (uint8_t *) buffer )[ total_write_count ],
			               buffer_size,
			               buffer_size,
			               error );
		}
		else
		{
			write_count = libewf_write_io_handle_write_new_chunk_data(
			               internal_handle->write_io_handle,
			               internal_handle->io_handle,
			               internal_handle->file_io_pool,
			               internal_handle->media_values,
			               internal_handle->offset_table,
			               internal_handle->segment_table,
			               &( internal_handle->header_values ),
			               internal_handle->hash_values,
			               internal_handle->header_sections,
			               internal_handle->hash_sections,
			               internal_handle->sessions,
			               internal_handle->acquiry_errors,
			               internal_handle->chunk_cache,
			               internal_handle->io_handle->current_chunk,
			               (void *) &( (uint8_t *) buffer )[ total_write_count ],
			               buffer_size,
			               buffer_size,
			               0,
			               error );
		}
		if( write_count <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write data from buffer.",
			 function );

			return( -1 );
		}
		else if( write_count == 0 )
		{
			break;
		}
		total_write_count += write_count;
		buffer_size       -= write_count;

		internal_handle->io_handle->current_offset       += (off64_t) write_count;
		internal_handle->io_handle->current_chunk_offset += (uint32_t) write_count;

		if( internal_handle->io_handle->current_chunk_offset == internal_handle->media_values->chunk_size )
		{
			internal_handle->io_handle->current_chunk_offset = 0;
			internal_handle->io_handle->current_chunk       += 1;
		}
		else if( internal_handle->io_handle->current_chunk_offset > internal_handle->media_values->chunk_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid current chunk offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( internal_handle->io_handle->abort == 1 )
		{
			break;
		}
	}
	return( total_write_count );
}

/* Writes (media) data in EWF format at a specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_handle_write_random(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "libewf_handle_write_random";
	ssize_t write_count   = 0;

	if( libewf_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	write_count = libewf_handle_write_buffer(
	               handle,
	               buffer,
	               buffer_size,
	               error );

	if( write_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after write from stream
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_handle_write_finalize(
         libewf_handle_t *handle,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_finalize";
	ssize_t write_count                       = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	write_count = libewf_write_io_handle_finalize(
	               internal_handle->write_io_handle,
	               internal_handle->io_handle,
	               internal_handle->file_io_pool,
	               internal_handle->media_values,
	               internal_handle->offset_table,
	               internal_handle->segment_table,
	               &( internal_handle->header_values ),
	               internal_handle->hash_values,
	               internal_handle->header_sections,
	               internal_handle->hash_sections,
	               internal_handle->sessions,
	               internal_handle->acquiry_errors,
	               internal_handle->chunk_cache,
	               error );

	if( write_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write finalize.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Seeks a certain offset of the (media) data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_handle_seek_offset(
         libewf_handle_t *handle,
         off64_t offset,
         int whence,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_seek_offset";
	uint64_t chunk                            = 0;
	uint64_t chunk_offset                     = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{	
		offset += internal_handle->io_handle->current_offset;
	}
	else if( whence == SEEK_END )
	{	
		offset += (off64_t) internal_handle->media_values->media_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: seeking offset: %" PRIi64 ".\n",
		 function,
		 offset );
	}
#endif
	if( offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	/* Determine the chunk that is requested
	 */
	chunk = offset / internal_handle->media_values->chunk_size;

	if( chunk >= (uint64_t) INT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Determine the offset within the decompressed chunk that is requested
	 */
	chunk_offset = offset % internal_handle->media_values->chunk_size;

	if( chunk_offset >= (uint64_t) INT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( offset < (off64_t) internal_handle->media_values->media_size )
	{
		if( libewf_offset_table_seek_chunk_offset(
		     internal_handle->offset_table,
		     (uint32_t) chunk,
		     internal_handle->file_io_pool,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek chunk offset.",
			 function );

			return( -1 );
		}
	}
	internal_handle->io_handle->current_offset       = offset;
	internal_handle->io_handle->current_chunk        = (uint32_t) chunk;
	internal_handle->io_handle->current_chunk_offset = (uint32_t) chunk_offset;

	return( offset );
}

/* Retrieves the current offset of the (media) data
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_offset(
     libewf_handle_t *handle,
     off64_t *offset,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_offset";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
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
	*offset = internal_handle->io_handle->current_offset;

	return( 1 );
}

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_maximum_number_of_open_handles(
     libewf_handle_t *handle,
     int maximum_number_of_open_handles,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_maximum_number_of_open_handles";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool != NULL )
	{
		if( libbfio_pool_set_maximum_number_of_open_handles(
		     internal_handle->file_io_pool,
		     maximum_number_of_open_handles,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set maximum number of open handles in file IO handle.",
			 function );

			return( -1 );
		}
	}
	internal_handle->maximum_number_of_open_handles = maximum_number_of_open_handles;

	return( 1 );
}

/* Retrieves the segment filename size
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename_size";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_size(
	          internal_handle->segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename(
	          internal_handle->segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_filename";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_set_basename(
	     internal_handle->segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename_size_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_size_wide(
	          internal_handle->segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_wide(
	          internal_handle->segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_filename_wide";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_set_basename_wide(
	     internal_handle->segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the maximum segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t *maximum_segment_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_maximum_segment_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( maximum_segment_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum segment size.",
		 function );

		return( -1 );
	}
	*maximum_segment_size = internal_handle->segment_table->maximum_segment_size;

	return( 1 );
}

/* Sets the maximum segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t maximum_segment_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_maximum_segment_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: maximum segment size cannot be changed.",
		 function );

		return( -1 );
	}
	if( maximum_segment_size > (size64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum segment size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( maximum_segment_size == 0 )
	 || ( maximum_segment_size > internal_handle->write_io_handle->maximum_segment_file_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment file size value out of bounds.",
		 function );

		return( -1 );
	}
	internal_handle->segment_table->maximum_segment_size = maximum_segment_size;

	return( 1 );
}

/* Retrieves the delta segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_delta_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename_size";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_size(
	          internal_handle->delta_segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_delta_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename(
	          internal_handle->delta_segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the delta segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_delta_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_delta_segment_filename";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: delta segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_set_basename(
	     internal_handle->delta_segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the delta segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_delta_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename_size_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_size_wide(
	          internal_handle->delta_segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_delta_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_wide(
	          internal_handle->delta_segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the delta segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_delta_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_delta_segment_filename_wide";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: delta segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_set_basename_wide(
	     internal_handle->delta_segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the maximum delta segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_maximum_delta_segment_size(
     libewf_handle_t *handle,
     size64_t *maximum_delta_segment_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_maximum_delta_segment_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( maximum_delta_segment_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum delta segment size.",
		 function );

		return( -1 );
	}
	*maximum_delta_segment_size = internal_handle->delta_segment_table->maximum_segment_size;

	return( 1 );
}

/* Sets the maximum delta segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_maximum_delta_segment_size(
     libewf_handle_t *handle,
     size64_t maximum_delta_segment_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_maximum_delta_segment_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: maximum delta segment size cannot be changed.",
		 function );

		return( -1 );
	}
	if( maximum_delta_segment_size > (size64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum delta segment size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( maximum_delta_segment_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid maximum delta segment size value out of bounds.",
		 function );

		return( -1 );
	}
	internal_handle->delta_segment_table->maximum_segment_size = maximum_delta_segment_size;

	return( 1 );
}

/* Retrieves the filename size of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current chunk.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_size(
	     file_io_handle,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current chunk.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the (delta) segment file of the current chunk
 * The filename size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename_size_wide";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current chunk.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_size_wide(
	     file_io_handle,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename_wide";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current chunk.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_wide(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the file IO handle of the (delta) segment file of the current chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_file_io_handle(
     libewf_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle         = NULL;
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_handle_get_file_io_handle";
	int file_io_pool_entry                            = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libewf_offset_table_get_segment_file_handle(
	     internal_handle->offset_table,
	     internal_handle->io_handle->current_chunk,
	     &segment_file_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file handle of chunk: %" PRIu32 ".",
		 function,
		 internal_handle->io_handle->current_chunk );

		return( -1 );
	}
	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment file handle of chunk: %" PRIu32 ".",
		 function,
		 internal_handle->io_handle->current_chunk );

		return( -1 );
	}
	file_io_pool_entry = segment_file_handle->file_io_pool_entry;

	if( libbfio_pool_get_handle(
	     internal_handle->file_io_pool,
	     file_io_pool_entry,
	     file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for pool entry: %d (chunk: %" PRIu32 ").",
		 function,
		 file_io_pool_entry,
		 internal_handle->io_handle->current_chunk );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the maximum number of supported segment files to write
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_get_write_maximum_number_of_segments(
     uint8_t ewf_format,
     uint16_t *maximum_number_of_segments,
     liberror_error_t **error )
{
	static char *function = "libewf_internal_handle_get_write_maximum_number_of_segments";

	if( maximum_number_of_segments == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum number of segments.",
		 function );

		return( -1 );
	}
	if( ewf_format == EWF_FORMAT_S01 )
	{
		/* ( ( ( 'z' - 's' ) * 26 * 26 ) + 99 ) = 4831
		 */
		*maximum_number_of_segments = (uint16_t) 4831;
	}
	else if( ewf_format == EWF_FORMAT_E01 )
	{
		/* ( ( ( 'Z' - 'E' ) * 26 * 26 ) + 99 ) = 14295
		 */
		*maximum_number_of_segments = (uint16_t) 14295;
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported EWF format.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media values
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_get_media_values(
     libewf_internal_handle_t *internal_handle,
     size64_t *media_size,
     liberror_error_t **error )
{
	libewf_chunk_value_t *chunk_value = NULL;
	static char *function             = "libewf_internal_handle_get_media_values";
	size64_t chunk_data_size          = 0;
	size64_t sector_data_size         = 0;
	size_t chunk_size                 = 0;
	ssize_t read_count                = 0;
	uint32_t number_of_chunk_values   = 0;

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
	sector_data_size  = internal_handle->media_values->number_of_sectors;
	sector_data_size *= internal_handle->media_values->bytes_per_sector;

	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		if( libewf_offset_table_get_number_of_chunk_values(
		     internal_handle->offset_table,
		     &number_of_chunk_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of chunk values in offset table.",
			 function );

			return( -1 );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			if( internal_handle->media_values->number_of_chunks != number_of_chunk_values )
			{
				libnotify_printf(
				 "%s: mismatch of number of chunks in volume: %" PRIu32 " and offset table(s): %" PRIu32 "\n",
				 function,
				 internal_handle->media_values->number_of_chunks,
				 number_of_chunk_values );
			}
		}
#endif
		if( number_of_chunk_values > 0 )
		{
			if( libewf_offset_table_get_chunk_value(
			     internal_handle->offset_table,
			     number_of_chunk_values - 1,
			     &chunk_value,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve chunk value: %" PRIu32 " from offset table.",
				 function,
				 number_of_chunk_values - 1 );

				return( -1 );
			}
			if( chunk_value == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing chunk value.",
				 function );

				return( -1 );
			}
			chunk_data_size  = number_of_chunk_values - 1;
			chunk_data_size *= internal_handle->media_values->sectors_per_chunk;
			chunk_data_size *= internal_handle->media_values->bytes_per_sector;

			if( ( chunk_value->flags & LIBEWF_CHUNK_VALUE_FLAG_COMPRESSED ) == 0 )
			{
				chunk_data_size += chunk_value->size - 4;
			}
			else
			{
				/* Reallocate the chunk cache if the chunk size is not the default chunk size
				 * this prevents some reallocations of the chunk cache
				 */
				chunk_size = internal_handle->media_values->chunk_size + sizeof( uint32_t );

				if( chunk_size > internal_handle->chunk_cache->size )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libnotify_verbose != 0 )
					{
						libnotify_printf(
						 "%s: reallocating chunk data size: %" PRIzu ".\n",
						 function,
						 chunk_size );
					}
#endif
					if( libewf_chunk_cache_resize(
					     internal_handle->chunk_cache,
					     chunk_size,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
						 "%s: unable to resize chunk cache.",
						 function );

						return( -1 );
					}
				}
				/* The only way to determine the size of the last compressed chunk
				 * is to read and decompress it
				 */
				read_count = libewf_read_io_handle_read_chunk_data(
				              internal_handle->read_io_handle,
				              internal_handle->io_handle,
				              internal_handle->file_io_pool,
				              internal_handle->media_values,
				              internal_handle->offset_table,
				              internal_handle->chunk_cache,
				              number_of_chunk_values - 1,
				              0,
				              internal_handle->chunk_cache->data,
				              internal_handle->chunk_cache->size,
				              error );

				if( read_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read data from chunk.",
					 function );

					return( -1 );
				}
				chunk_data_size += internal_handle->chunk_cache->data_size;
			}
			if( sector_data_size != chunk_data_size )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: mismatch of media data size in volume: %" PRIu64 " and offset table(s): %" PRIu64 "\n",
					 function,
					 sector_data_size,
					 chunk_data_size );
				}
#endif
				sector_data_size = chunk_data_size;
			}
		}
	}
	*media_size = sector_data_size;

	return( 1 );
}

/* Sets the media values
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_set_media_values(
     libewf_internal_handle_t *internal_handle,
     uint32_t sectors_per_chunk,
     uint32_t bytes_per_sector,
     size64_t media_size,
     liberror_error_t **error )
{
	static char *function            = "libewf_internal_handle_set_media_values";
	size32_t chunk_size              = 0;
	size64_t maximum_input_file_size = 0;
	uint64_t number_of_chunks        = 0;
	uint64_t number_of_sectors       = 0;

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( ( sectors_per_chunk == 0 )
	 || ( sectors_per_chunk > (uint32_t) INT32_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sectors per chunk.",
		 function );

		return( -1 );
	}
	if( ( bytes_per_sector == 0 )
	 || ( bytes_per_sector > (uint32_t) INT32_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid media size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Determine the chunk size
	 */
	chunk_size = sectors_per_chunk * bytes_per_sector;

	if( ( chunk_size == 0 )
	 || ( chunk_size > (size32_t) INT32_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk size.",
		 function );

		return( -1 );
	}
	/* Check if the input file size does not exceed the maximum possible input file size
	 * for the chunk size
	 */
	maximum_input_file_size = (size64_t) chunk_size * (size64_t) UINT32_MAX;

	if( media_size > maximum_input_file_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: media size cannot be larger than size: %" PRIu64 " with a chunk size of: %" PRIu32 ".",
		 function,
		 maximum_input_file_size,
		 chunk_size );

		return( -1 );
	}
	internal_handle->media_values->sectors_per_chunk = sectors_per_chunk;
	internal_handle->media_values->bytes_per_sector  = bytes_per_sector;
	internal_handle->media_values->chunk_size        = chunk_size;
	internal_handle->media_values->media_size        = media_size;

	/* If a media size was provided
	 */
	if( media_size > 0 )
	{
		/* Determine the number of chunks to write
		 */
		number_of_chunks = (uint64_t) media_size / (uint64_t) chunk_size;

		if( ( (uint64_t) media_size % (uint64_t) chunk_size ) != 0 )
		{
			number_of_chunks += 1;
		}
		if( number_of_chunks > (uint64_t) UINT32_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid number of chunks value exceeds maximum.",
			 function );

			return( -1 );
		}
		internal_handle->media_values->number_of_chunks = (uint32_t) number_of_chunks;

		/* Determine the number of sectors to write
		 */
		number_of_sectors = (uint64_t) media_size / (uint64_t) bytes_per_sector;

		if( number_of_sectors > (uint64_t) INT64_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid number of sectors value exceeds maximum.",
			 function );

			return( -1 );
		}
		internal_handle->media_values->number_of_sectors = number_of_sectors;
	}
	return( 1 );
}

/* Sets internal values based on the EWF file format
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_set_format(
     libewf_internal_handle_t *internal_handle,
     uint8_t format,
     liberror_error_t **error )
{
	static char *function = "libewf_internal_handle_set_format";

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK )
	 && ( format != LIBEWF_FORMAT_LVF )
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format: %d.",
		 function,
	         format );

		return( -1 );
	}
	internal_handle->io_handle->format = format;

	if( ( format == LIBEWF_FORMAT_EWF )
	 || ( format == LIBEWF_FORMAT_SMART ) )
	{
		internal_handle->io_handle->ewf_format = EWF_FORMAT_S01;
	}
	else if( format == LIBEWF_FORMAT_LVF )
	{
		internal_handle->io_handle->ewf_format = EWF_FORMAT_L01;
	}
	else
	{
		internal_handle->io_handle->ewf_format = EWF_FORMAT_E01;
	}
	if( internal_handle->write_io_handle != NULL )
	{
		if( format == LIBEWF_FORMAT_ENCASE6 )
		{
			internal_handle->write_io_handle->maximum_segment_file_size  = INT64_MAX;
			internal_handle->write_io_handle->maximum_chunks_per_section = EWF_MAXIMUM_OFFSETS_IN_TABLE_ENCASE6;
		}
		else if( format == LIBEWF_FORMAT_EWFX )
		{
			internal_handle->write_io_handle->unrestrict_offset_table    = 1;
			internal_handle->write_io_handle->maximum_segment_file_size  = INT32_MAX;
			internal_handle->write_io_handle->maximum_chunks_per_section = INT32_MAX;
		}
		else
		{
			internal_handle->write_io_handle->maximum_segment_file_size  = INT32_MAX;
			internal_handle->write_io_handle->maximum_chunks_per_section = EWF_MAXIMUM_OFFSETS_IN_TABLE;
		}
		/* Determine the maximum number of segments allowed to write
		 */
		if( libewf_internal_handle_get_write_maximum_number_of_segments(
		     internal_handle->io_handle->ewf_format,
		     &( internal_handle->write_io_handle->maximum_number_of_segments ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the maximum number of allowed segment files.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

