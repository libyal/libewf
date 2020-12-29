/*
 * Handle functions
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
#include <types.h>
#include <wide_string.h>

#include "libewf_analytical_data.h"
#include "libewf_case_data.h"
#include "libewf_case_data_section.h"
#include "libewf_chunk_data.h"
#include "libewf_chunk_table.h"
#include "libewf_codepage.h"
#include "libewf_compression.h"
#include "libewf_data_chunk.h"
#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_device_information.h"
#include "libewf_device_information_section.h"
#include "libewf_digest_section.h"
#include "libewf_error2_section.h"
#include "libewf_file_entry.h"
#include "libewf_handle.h"
#include "libewf_hash_sections.h"
#include "libewf_hash_values.h"
#include "libewf_header_sections.h"
#include "libewf_header_values.h"
#include "libewf_io_handle.h"
#include "libewf_lef_file_entry.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcthreads.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_libfvalue.h"
#include "libewf_ltree_section.h"
#include "libewf_md5_hash_section.h"
#include "libewf_restart_data.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"
#include "libewf_sector_range.h"
#include "libewf_sector_range_list.h"
#include "libewf_segment_file.h"
#include "libewf_session_section.h"
#include "libewf_sha1_hash_section.h"
#include "libewf_single_file_tree.h"
#include "libewf_single_files.h"
#include "libewf_types.h"
#include "libewf_unused.h"
#include "libewf_value_table.h"
#include "libewf_write_io_handle.h"

#include "ewf_data.h"
#include "ewf_file_header.h"

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_initialize(
     libewf_handle_t **handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_initialize";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle value already set.",
		 function );

		return( -1 );
	}
	internal_handle = memory_allocate_structure(
	                   libewf_internal_handle_t );

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_handle,
	     0,
	     sizeof( libewf_internal_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear handle.",
		 function );

		memory_free(
		 internal_handle );

		return( -1 );
	}
	if( libewf_io_handle_initialize(
	     &( internal_handle->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( libewf_media_values_initialize(
	     &( internal_handle->media_values ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create media values.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( internal_handle->sessions ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sessions array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( internal_handle->tracks ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create tracks array.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_initialize(
	     &( internal_handle->acquiry_errors ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create acquiry errors range list.",
		 function );

		goto on_error;
	}
	if( libewf_segment_table_initialize(
	     &( internal_handle->segment_table ),
	     internal_handle->io_handle,
	     LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment table.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_handle->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_handle->date_format                    = LIBEWF_DATE_FORMAT_CTIME;
	internal_handle->maximum_number_of_open_handles = LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES;

	*handle = (libewf_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		if( internal_handle->acquiry_errors != NULL )
		{
			libcdata_range_list_free(
			 &( internal_handle->acquiry_errors ),
			 NULL,
			 NULL );
		}
		if( internal_handle->tracks != NULL )
		{
			libcdata_array_free(
			 &( internal_handle->tracks ),
			 NULL,
			 NULL );
		}
		if( internal_handle->sessions != NULL )
		{
			libcdata_array_free(
			 &( internal_handle->sessions ),
			 NULL,
			 NULL );
		}
		if( internal_handle->media_values != NULL )
		{
			libewf_media_values_free(
			 &( internal_handle->media_values ),
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

/* Frees a handle
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_free(
     libewf_handle_t **handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_internal_handle_free";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close handle.",
				 function );

				result = -1;
			}
		}
		*handle = NULL;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_handle->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( libewf_segment_table_free(
		     &( internal_handle->segment_table ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment table.",
			 function );

			result = -1;
		}
		if( libcdata_range_list_free(
		     &( internal_handle->acquiry_errors ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free acquiry errors range list.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( internal_handle->tracks ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free tracks array.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( internal_handle->sessions ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sessions array.",
			 function );

			result = -1;
		}
		if( libewf_media_values_free(
		     &( internal_handle->media_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free media values.",
			 function );

			result = -1;
		}
		if( libewf_io_handle_free(
		     &( internal_handle->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_destination_handle = NULL;
	libewf_internal_handle_t *internal_source_handle      = NULL;
	static char *function                                 = "libewf_handle_clone";

	if( destination_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination handle.",
		 function );

		return( -1 );
	}
	if( *destination_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination handle value already set.",
		 function );

		return( -1 );
	}
	if( source_handle == NULL )
	{
		*destination_handle = NULL;

		return( 1 );
	}
	internal_source_handle = (libewf_internal_handle_t *) source_handle;

	if( internal_source_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( internal_source_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	internal_destination_handle = memory_allocate_structure(
			               libewf_internal_handle_t );

	if( internal_destination_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_destination_handle,
	     0,
	     sizeof( libewf_internal_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear destination handle.",
		 function );

		goto on_error;
	}
	if( libewf_io_handle_clone(
	     &( internal_destination_handle->io_handle ),
	     internal_source_handle->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination IO handle.",
		 function );

		goto on_error;
	}
	if( libewf_media_values_clone(
	     &( internal_destination_handle->media_values ),
	     internal_source_handle->media_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination media values.",
		 function );

		goto on_error;
	}
	if( libcdata_array_clone(
	     &( internal_destination_handle->sessions ),
	     internal_source_handle->sessions,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_sector_range_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination sessions array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_clone(
	     &( internal_destination_handle->tracks ),
	     internal_source_handle->tracks,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_sector_range_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination tracks array.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_clone(
	     &( internal_destination_handle->acquiry_errors ),
	     internal_source_handle->acquiry_errors,
	     NULL,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination acquiry errors range list.",
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination write IO handle.",
			 function );

			goto on_error;
		}
	}
	if( libewf_segment_table_clone(
	     &( internal_destination_handle->segment_table ),
	     internal_source_handle->segment_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination segment table.",
		 function );

		goto on_error;
	}
	if( internal_source_handle->chunk_table != NULL )
	{
		if( libewf_chunk_table_clone(
		     &( internal_destination_handle->chunk_table ),
		     internal_source_handle->chunk_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination chunk table.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->hash_sections != NULL )
	{
		if( libewf_hash_sections_clone(
		     &( internal_destination_handle->hash_sections ),
		     internal_source_handle->hash_sections,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination hash sections.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->header_values != NULL )
	{
		if( libfvalue_table_clone(
		     &( internal_destination_handle->header_values ),
		     internal_source_handle->header_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination header values.",
			 function );

			goto on_error;
		}
		internal_destination_handle->header_values_parsed = internal_source_handle->header_values_parsed;
	}
	if( internal_source_handle->hash_values != NULL )
	{
		if( libfvalue_table_clone(
		     &( internal_destination_handle->hash_values ),
		     internal_source_handle->hash_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination hash values.",
			 function );

			goto on_error;
		}
		internal_destination_handle->hash_values_parsed = internal_source_handle->hash_values_parsed;
	}
	internal_destination_handle->maximum_number_of_open_handles = internal_source_handle->maximum_number_of_open_handles;
	internal_destination_handle->date_format                    = internal_source_handle->date_format;

	*destination_handle = (libewf_handle_t *) internal_destination_handle;

	return( 1 );

on_error:
	if( internal_destination_handle != NULL )
	{
		if( internal_destination_handle->hash_values != NULL )
		{
			libfvalue_table_free(
			 &( internal_destination_handle->hash_values ),
			 NULL );
		}
		if( internal_destination_handle->header_values != NULL )
		{
			libfvalue_table_free(
			 &( internal_destination_handle->header_values ),
			 NULL );
		}
		if( internal_destination_handle->hash_sections != NULL )
		{
			libewf_hash_sections_free(
			 &( internal_destination_handle->hash_sections ),
			 NULL );
		}
		if( internal_destination_handle->chunk_table != NULL )
		{
			libewf_chunk_table_free(
			 &( internal_destination_handle->chunk_table ),
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
			libcdata_range_list_free(
			 &( internal_destination_handle->acquiry_errors ),
			 NULL,
			 NULL );
		}
		if( internal_destination_handle->tracks != NULL )
		{
			libcdata_array_free(
			 &( internal_destination_handle->tracks ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
			 NULL );
		}
		if( internal_destination_handle->sessions != NULL )
		{
			libcdata_array_free(
			 &( internal_destination_handle->sessions ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
			 NULL );
		}
		if( internal_destination_handle->media_values != NULL )
		{
			libewf_media_values_free(
			 &( internal_destination_handle->media_values ),
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_signal_abort";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libbfio_pool_t *file_io_pool              = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	char *first_segment_filename              = NULL;
	static char *function                     = "libewf_handle_open";
	size_t filename_length                    = 0;
	int file_io_pool_entry                    = 0;
	int filename_index                        = 0;
	int maximum_number_of_open_handles        = 0;
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

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
		 "%s: invalid number of files zero or less.",
		 function );

		return( -1 );
	}
#if !defined( HAVE_WRITE_SUPPORT )
	if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported - compiled without zlib.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	maximum_number_of_open_handles = internal_handle->maximum_number_of_open_handles;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     maximum_number_of_open_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO pool.",
		 function );

		goto on_error;
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 || ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			filename_length = narrow_string_length(
					   filenames[ filename_index ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %s is too small.",
				 function,
				 filenames[ filename_index ] );

				goto on_error;
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                libcerror_error_set(
		                 error,
		                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle.",
		                 function );

				goto on_error;
			}
#endif
			if( libbfio_file_set_name(
			     file_io_handle,
			     filenames[ filename_index ],
			     filename_length,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				goto on_error;
			}
			if( libbfio_pool_append_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append file IO handle to pool.",
				 function );

				goto on_error;
			}
			file_io_handle = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: added file IO pool entry: %d with filename: %s.\n",
				 function,
				 file_io_pool_entry,
				 filenames[ filename_index ] );
			}
#endif
			if( ( filenames[ filename_index ][ filename_length - 3 ] == 'e' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 'E' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 'l' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 'L' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 's' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 'S' ) )
			{
				if( ( filenames[ filename_index ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_index ][ filename_length - 1 ] == '1' ) )
				{
					first_segment_filename = filenames[ filename_index ];
				}
			}
		}
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 || ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = narrow_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				goto on_error;
			}
		}
	}
	else if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	{
		/* Get the basename and store it in the segment tables
		 */
		filename_length = narrow_string_length(
				   filenames[ 0 ] );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename(
		     internal_handle->segment_table,
		     filenames[ 0 ],
		     filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	result = libewf_internal_handle_open_file_io_pool(
	          internal_handle,
	          file_io_pool,
	          access_flags,
	          internal_handle->segment_table,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO pool.",
		 function );

		libbfio_pool_free(
		 &file_io_pool,
		 NULL );

		result = -1;
	}
	else
	{
		internal_handle->file_io_pool                    = file_io_pool;
		internal_handle->file_io_pool_created_in_library = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	result = libcthreads_read_write_lock_release_for_write(
	          internal_handle->read_write_lock,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		result = -1;
	}
#endif
	if( result != 1 )
	{
		goto on_error;
	}
	return( 1 );

on_error:
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
	internal_handle->file_io_pool                    = NULL;
	internal_handle->file_io_pool_created_in_library = 0;

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
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libbfio_pool_t *file_io_pool              = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	wchar_t *first_segment_filename           = NULL;
	static char *function                     = "libewf_handle_open_wide";
	size_t filename_length                    = 0;
	int file_io_pool_entry                    = 0;
	int filename_index                        = 0;
	int maximum_number_of_open_handles        = 0;
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

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
		 "%s: invalid number of files zero or less.",
		 function );

		return( -1 );
	}
#if !defined( HAVE_WRITE_SUPPORT )
	if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported - compiled without zlib.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	maximum_number_of_open_handles = internal_handle->maximum_number_of_open_handles;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     maximum_number_of_open_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO pool.",
		 function );

		goto on_error;
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 || ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			filename_length = wide_string_length(
					   filenames[ filename_index ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %ls is too small.",
				 function,
				 filenames[ filename_index ] );

				goto on_error;
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                libcerror_error_set(
		                 error,
		                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle.",
		                 function );

				goto on_error;
			}
#endif
			if( libbfio_file_set_name_wide(
			     file_io_handle,
			     filenames[ filename_index ],
			     filename_length,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				goto on_error;
			}
			if( libbfio_pool_append_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append file IO handle to pool.",
				 function );

				goto on_error;
			}
			file_io_handle = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: added file IO pool entry: %d with filename: %ls.\n",
				 function,
				 file_io_pool_entry,
				 filenames[ filename_index ] );
			}
#endif
			if( ( filenames[ filename_index ][ filename_length - 3 ] == 'e' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 'E' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 'l' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 'L' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 's' )
			 || ( filenames[ filename_index ][ filename_length - 3 ] == 'S' ) )
			{
				if( ( filenames[ filename_index ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_index ][ filename_length - 1 ] == '1' ) )
				{
					first_segment_filename = filenames[ filename_index ];
				}
			}
		}
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 || ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = wide_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename_wide(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				goto on_error;
			}
		}
	}
	else if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	{
		/* Get the basename and store it in the segment tables
		 */
		filename_length = wide_string_length(
				   filenames[ 0 ] );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename_wide(
		     internal_handle->segment_table,
		     filenames[ 0 ],
		     filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	result = libewf_internal_handle_open_file_io_pool(
	          internal_handle,
	          file_io_pool,
	          access_flags,
	          internal_handle->segment_table,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO pool.",
		 function );

		result = -1;
	}
	else
	{
		internal_handle->file_io_pool                    = file_io_pool;
		internal_handle->file_io_pool_created_in_library = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	result = libcthreads_read_write_lock_release_for_write(
	          internal_handle->read_write_lock,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		result = -1;
	}
#endif
	if( result != 1 )
	{
		goto on_error;
	}
	return( 1 );

on_error:
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
	internal_handle->file_io_pool                    = NULL;
	internal_handle->file_io_pool_created_in_library = 0;

	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Reads the section data from a segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_open_read_segment_file_section_data(
     libewf_internal_handle_t *internal_handle,
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	libewf_header_sections_t *header_sections = NULL;
	libewf_section_descriptor_t *section      = NULL;
	libfcache_cache_t *sections_cache         = NULL;
	uint8_t *single_files_data                = NULL;
	uint8_t *single_files_section_data        = NULL;
	uint8_t *string_data                      = NULL;
	static char *function                     = "libewf_internal_handle_open_read_segment_file_section_data";
	size_t single_files_data_size             = 0;
	size_t single_files_section_data_size     = 0;
	size_t string_data_size                   = 0;
	ssize_t read_count                        = 0;
	off64_t section_data_offset               = 0;
	uint8_t header_section_found              = 0;
	uint8_t initialize_chunk_values           = 0;
	int number_of_sections                    = 0;
	int read_table_sections                   = 0;
	int result                                = 0;
	int section_index                         = 0;
	int set_identifier_change                 = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	int known_section                         = 0;
#endif

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing read IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->single_files != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - single files value already set.",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( libfcache_cache_initialize(
	     &sections_cache,
	     LIBEWF_MAXIMUM_CACHE_ENTRIES_SECTIONS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sections cache.",
		 function );

		goto on_error;
	}
	if( libewf_header_sections_initialize(
	     &header_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header sections.",
		 function );

		goto on_error;
	}
	if( libfdata_list_get_number_of_elements(
	     segment_file->sections_list,
	     &number_of_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of sections in the sections list.",
		 function );

		goto on_error;
	}
	for( section_index = 0;
	     section_index < number_of_sections;
	     section_index++ )
	{
		if( libfdata_list_get_element_value_by_index(
		     segment_file->sections_list,
		     (intptr_t *) file_io_pool,
		     (libfdata_cache_t *) sections_cache,
		     section_index,
		     (intptr_t **) &section,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve section: %d from sections list.",
			 function,
			 section_index );

			goto on_error;
		}
		result = libewf_section_get_data_offset(
		          section,
		          segment_file->major_version,
		          &section_data_offset,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve section: %d data offset.",
			 function,
			 section_index );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libewf_segment_file_seek_offset(
			     segment_file,
			     file_io_pool,
			     file_io_pool_entry,
			     section_data_offset,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to seek section: %d data offset: %" PRIu64 ".",
				 function,
				 section_index,
				 section_data_offset );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( segment_file->major_version == 1 )
				{
					libcnotify_printf(
					 "%s: reading %s section data from file IO pool entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
					 function,
					 (char *) section->type_string,
					 file_io_pool_entry,
					 segment_file->current_offset,
					 segment_file->current_offset );
				}
				else if( segment_file->major_version == 2 )
				{
					libcnotify_printf(
					 "%s: reading 0x%08" PRIx32 " section data from file IO pool entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
					 function,
					 section->type,
					 file_io_pool_entry,
					 segment_file->current_offset,
					 segment_file->current_offset );
				}
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
		}
		if( section->type != 0 )
		{
			switch( section->type )
			{
				case LIBEWF_SECTION_TYPE_DEVICE_INFORMATION:
					read_count = libewf_device_information_section_read_file_io_pool(
						      section,
						      internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
						      internal_handle->read_io_handle,
						      internal_handle->media_values,
						      internal_handle->header_values,
						      error );
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_CASE_DATA:
					if( internal_handle->read_io_handle->case_data == NULL )
					{
						initialize_chunk_values = 1;
					}
					read_count = libewf_case_data_section_read_file_io_pool(
						      section,
						      internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
						      internal_handle->read_io_handle,
						      internal_handle->media_values,
						      internal_handle->header_values,
						      error );

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_SECTOR_DATA:
#if defined( HAVE_VERBOSE_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
						{
							libcnotify_printf(
							 "%s: found sectors section in EWF-S01 format.\n",
							 function );
						}
					}
#endif
					/* Nothing to do for the sectors section
					 */
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_SECTOR_TABLE:
					/* If the chunk_size was unknown when the segment file was opened we
					 * have to read the chunk groups here
					 */
					if( segment_file->number_of_chunks == 0 )
					{
						read_table_sections = 1;
					}
					if( read_table_sections != 0 )
					{
						read_count = libewf_segment_file_read_table_section(
							      segment_file,
							      section,
							      file_io_pool,
							      file_io_pool_entry,
							      internal_handle->media_values->chunk_size,
							      error );
					}
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_ERROR_TABLE:
#if defined( HAVE_VERBOSE_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
						{
							libcnotify_printf(
							 "%s: found error section in EWF-S01 format.\n",
							 function );
						}
					}
#endif
					read_count = libewf_error2_section_read_file_io_pool(
						      section,
						      internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
						      segment_file->major_version,
						      internal_handle->acquiry_errors,
						      error );

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_SESSION_TABLE:
#if defined( HAVE_VERBOSE_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
						{
							libcnotify_printf(
							 "%s: found session section in EWF-S01 format.\n",
							 function );
						}
					}
#endif
					read_count = libewf_session_section_read_file_io_pool(
						      section,
						      internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
						      segment_file->major_version,
						      internal_handle->media_values,
						      internal_handle->sessions,
						      internal_handle->tracks,
						      error );

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_INCREMENT_DATA:
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_MD5_HASH:
					read_count = libewf_md5_hash_section_read_file_io_pool(
						      section,
						      internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
						      segment_file->major_version,
						      internal_handle->hash_sections,
						      error );

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_SHA1_HASH:
					read_count = libewf_sha1_hash_section_read_file_io_pool(
						      section,
						      internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
						      internal_handle->hash_sections,
						      error );

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_RESTART_DATA:
					read_count = libewf_section_compressed_string_read(
						      section,
					              internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
					              internal_handle->io_handle->compression_method,
						      &string_data,
						      &string_data_size,
						      error );

					if( read_count == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read restart data file object string.",
						 function );

						goto on_error;
					}
					if( libewf_restart_data_parse(
					     string_data,
					     string_data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to parse restart data.",
						 function );

						goto on_error;
					}
					memory_free(
					 string_data );

					string_data = NULL;

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_ENCRYPTION_KEYS:
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_MEMORY_EXTENTS_TABLE:
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_NEXT:
					/* Nothing to do for the next section
					 */
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_FINAL_INFORMATION:
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_DONE:
					/* Nothing to do for the done section
					 */
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_ANALYTICAL_DATA:
					read_count = libewf_section_compressed_string_read(
						      section,
					              internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
					              internal_handle->io_handle->compression_method,
						      &string_data,
						      &string_data_size,
						      error );

					if( read_count == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read analytical data file object string.",
						 function );

						goto on_error;
					}
					else if( read_count != 0 )
					{
						if( libewf_analytical_data_parse(
						     string_data,
						     string_data_size,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to parse analytical data.",
							 function );

							goto on_error;
						}
						memory_free(
						 string_data );

						string_data = NULL;
					}
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_SINGLE_FILES_DATA:
#if defined( HAVE_VERBOSE_OUTPUT )
					if( ( internal_handle->io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
					 && ( internal_handle->io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
					{
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: found single files data section in none logical evidence format.\n",
							 function );
						}
					}
#endif
					read_count = libewf_ltree_section_read_file_io_pool(
						      section,
						      internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
						      segment_file->major_version,
						      &single_files_section_data,
						      &single_files_section_data_size,
						      &single_files_data,
						      &single_files_data_size,
						      error );

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;
			}
		}
		else if( section->type_string_length == 4 )
		{
			if( memory_compare(
			     (void *) section->type_string,
			     (void *) "data",
			     4 ) == 0 )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
					{
						libcnotify_printf(
						 "%s: found data section in EWF-S01 format.\n",
						 function );
					}
				}
#endif
				read_count = libewf_section_data_read(
					      section,
					      internal_handle->io_handle,
					      file_io_pool,
					      file_io_pool_entry,
					      internal_handle->media_values,
				              &set_identifier_change,
				              error );

				if( set_identifier_change != 0 )
				{
#if defined( HAVE_VERBOSE_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: set identifier does not match.",
						 function );
					}
#endif
					segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_IS_CORRUPTED;
				}
#if defined( HAVE_VERBOSE_OUTPUT )
				known_section = 1;
#endif
			}
			else if( memory_compare(
				  (void *) section->type_string,
				  (void *) "disk",
				  4 ) == 0 )
			{
				read_count = libewf_segment_file_read_volume_section(
					      segment_file,
					      section,
					      file_io_pool,
					      file_io_pool_entry,
					      internal_handle->media_values,
					      error );

				initialize_chunk_values = 1;

#if defined( HAVE_VERBOSE_OUTPUT )
				known_section = 1;
#endif
			}
		}
		else if( section->type_string_length == 5 )
		{
			if( memory_compare(
			     (void *) section->type_string,
			     (void *) "xhash",
			     5 ) == 0 )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
					{
						libcnotify_printf(
						 "%s: found xhash section in EWF-S01 format.\n",
						 function );
					}
				}
#endif
				read_count = libewf_section_compressed_string_read(
					      section,
				              internal_handle->io_handle,
					      file_io_pool,
					      file_io_pool_entry,
					      internal_handle->io_handle->compression_method,
					      &string_data,
					      &string_data_size,
					      error );

				if( read_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read xhash string.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					 if( libewf_debug_utf8_stream_print(
					      "XHash",
					      string_data,
					      string_data_size,
					      error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print xhash.",
						 function );

						goto on_error;
					}
				}
#endif
				if( internal_handle->hash_sections->xhash == NULL )
				{
					internal_handle->hash_sections->xhash      = string_data;
					internal_handle->hash_sections->xhash_size = string_data_size;
				}
				else
				{
					memory_free(
					 string_data );
				}
				string_data = NULL;
#if defined( HAVE_VERBOSE_OUTPUT )
				known_section = 1;
#endif
			}
		}
		else if( section->type_string_length == 6 )
		{
			if( memory_compare(
			     (void *) section->type_string,
			     (void *) "digest",
			     6 ) == 0 )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
					{
						libcnotify_printf(
						 "%s: found digest section in EWF-S01 format.\n",
						 function );
					}
				}
#endif
				read_count = libewf_digest_section_read_file_io_pool(
					      section,
				              internal_handle->io_handle,
					      file_io_pool,
					      file_io_pool_entry,
					      internal_handle->hash_sections,
					      error );

#if defined( HAVE_VERBOSE_OUTPUT )
				known_section = 1;
#endif
			}
			else if( memory_compare(
				  (void *) section->type_string,
				  (void *) "header",
				  6 ) == 0 )
			{
				read_count = libewf_section_compressed_string_read(
					      section,
				              internal_handle->io_handle,
					      file_io_pool,
					      file_io_pool_entry,
				              internal_handle->io_handle->compression_method,
					      &string_data,
					      &string_data_size,
					      error );

				if( read_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read header file object string.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( libewf_debug_byte_stream_print(
					     "Header",
					     string_data,
					     string_data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print header.",
						 function );

						goto on_error;
					}
				}
#endif
				if( header_sections->header == NULL )
				{
					header_sections->header      = string_data;
					header_sections->header_size = string_data_size;
				}
				else
				{
					memory_free(
					 string_data );
				}
				string_data = NULL;

				header_sections->number_of_header_sections += 1;

				header_section_found = 1;

#if defined( HAVE_VERBOSE_OUTPUT )
				known_section = 1;
#endif
			}
			else if( memory_compare(
				  (void *) section->type_string,
				  (void *) "table2",
				  6 ) == 0 )
			{
				if( read_table_sections != 0 )
				{
					read_count = libewf_segment_file_read_table2_section(
						      segment_file,
						      section,
						      file_io_pool,
						      file_io_pool_entry,
						      error );
				}
#if defined( HAVE_VERBOSE_OUTPUT )
				known_section = 1;
#endif
			}
			else if( memory_compare(
				  (void *) section->type_string,
				  (void *) "volume",
				  6 ) == 0 )
			{
				read_count = libewf_segment_file_read_volume_section(
					      segment_file,
					      section,
					      file_io_pool,
					      file_io_pool_entry,
					      internal_handle->media_values,
					      error );

				initialize_chunk_values = 1;

#if defined( HAVE_VERBOSE_OUTPUT )
				known_section = 1;
#endif
			}
		}
		else if( section->type_string_length == 7 )
		{
			if( memory_compare(
			     (void *) section->type_string,
			     (void *) "header2",
			     7 ) == 0 )
			{
				read_count = libewf_section_compressed_string_read(
					      section,
				              internal_handle->io_handle,
					      file_io_pool,
					      file_io_pool_entry,
				              internal_handle->io_handle->compression_method,
					      &string_data,
					      &string_data_size,
					      error );

				if( read_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read header2 file object string.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( libewf_debug_utf16_stream_print(
					     "Header2",
					     string_data,
					     string_data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print header2.",
						 function );

						goto on_error;
					}
				}
#endif
				if( header_sections->header2 == NULL )
				{
					header_sections->header2      = string_data;
					header_sections->header2_size = string_data_size;
				}
				else
				{
					memory_free(
					 string_data );
				}
				string_data = NULL;

				header_sections->number_of_header_sections += 1;

				header_section_found = 1;

#if defined( HAVE_VERBOSE_OUTPUT )
				known_section = 1;
#endif
			}
			else if( memory_compare(
				  (void *) section->type_string,
				  (void *) "xheader",
				  7 ) == 0 )
			{
				read_count = libewf_section_compressed_string_read(
					      section,
				              internal_handle->io_handle,
					      file_io_pool,
					      file_io_pool_entry,
				              internal_handle->io_handle->compression_method,
					      &string_data,
					      &string_data_size,
					      error );

				if( read_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read xheader string.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( libewf_debug_utf8_stream_print(
					     "XHeader",
					     string_data,
					     string_data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print xheader.",
						 function );

						goto on_error;
					}
				}
#endif
				if( header_sections->xheader == NULL )
				{
					header_sections->xheader      = string_data;
					header_sections->xheader_size = string_data_size;
				}
				else
				{
					memory_free(
					 string_data );
				}
				string_data = NULL;

				header_sections->number_of_header_sections += 1;

				header_section_found = 1;

#if defined( HAVE_VERBOSE_OUTPUT )
				known_section = 1;
#endif
			}
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( known_section == 0 )
			{
				if( segment_file->major_version == 1 )
				{
					libcnotify_printf(
					 "%s: unsupported section type: %s.\n",
					 function,
					 (char *) section->type_string );
				}
				else if( segment_file->major_version == 2 )
				{
					libcnotify_printf(
					 "%s: unsupported section type: 0x%08" PRIx32 ".\n",
					 function,
					 section->type );
				}
			}
		}
#endif
		if( read_count == -1 )
		{
			if( section->type_string_length > 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read section: %s.",
				 function,
				 (char *) section->type_string );
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read section: 0x%08" PRIx32 ".",
				 function,
				 section->type );
			}
			goto on_error;
		}
		if( initialize_chunk_values != 0 )
		{
			if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
			{
				segment_file->io_handle->segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
				segment_file->io_handle->format            = LIBEWF_FORMAT_SMART;
			}
			if( libewf_media_values_calculate_chunk_size(
			     internal_handle->media_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to calculate chunk size.",
				 function );

				goto on_error;
			}
/* TODO refactor */
			internal_handle->io_handle->chunk_size = internal_handle->media_values->chunk_size;

			/* Do a preliminary dection of the EWF format for reading the sector table section
			 */
			if( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
			{
				if( header_sections->number_of_header_sections == 1 )
				{
					internal_handle->io_handle->format = LIBEWF_FORMAT_ENCASE1;
				}
				else if( internal_handle->media_values->error_granularity == 0 )
				{
					internal_handle->io_handle->format = LIBEWF_FORMAT_ENCASE2;
				}
			}
			initialize_chunk_values = 0;
		}
	}
	if( header_section_found != 0 )
	{
		if( libewf_header_sections_parse(
		     header_sections,
		     internal_handle->io_handle,
		     internal_handle->header_values,
		     &( internal_handle->io_handle->format ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse header sections.",
			 function );

			goto on_error;
		}
	}
	if( single_files_data != NULL )
	{
		if( libewf_single_files_initialize(
		     &( internal_handle->single_files ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create single files.",
			 function );

			goto on_error;
		}
		if( libewf_single_files_read_data(
		     internal_handle->single_files,
		     single_files_data,
		     single_files_data_size,
		     &( internal_handle->media_values->media_size ),
		     &( internal_handle->io_handle->format ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse single files.",
			 function );

			goto on_error;
		}
		memory_free(
		 single_files_section_data );

		single_files_section_data = NULL;

		if( internal_handle->io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL )
		{
			if( internal_handle->io_handle->format == LIBEWF_FORMAT_LOGICAL_ENCASE7 )
			{
				internal_handle->io_handle->format = LIBEWF_FORMAT_V2_LOGICAL_ENCASE7;
			}
		}
		internal_handle->media_values->number_of_sectors = internal_handle->media_values->media_size
		                                                 / internal_handle->media_values->bytes_per_sector;

		if( ( internal_handle->media_values->media_size % internal_handle->media_values->bytes_per_sector ) != 0 )
		{
			internal_handle->media_values->number_of_sectors += 1;
		}
	}
	if( libewf_header_sections_free(
	     &header_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free header sections.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_free(
	     &sections_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sections cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_handle->single_files != NULL )
	{
		libewf_single_files_free(
		 &( internal_handle->single_files ),
		 NULL );
	}
	if( single_files_section_data != NULL )
	{
		memory_free(
		 single_files_section_data );
	}
	if( string_data != NULL )
	{
		memory_free(
		 string_data );
	}
	if( header_sections != NULL )
	{
		libewf_header_sections_free(
		 &header_sections,
		 NULL );
	}
	if( sections_cache != NULL )
	{
		libfcache_cache_free(
		 &sections_cache,
		 NULL );
	}
	return( -1 );
}

/* Opens the segment files for reading
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_open_read_segment_files(
     libewf_internal_handle_t *internal_handle,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     libcerror_error_t **error )
{
	libewf_segment_file_t *segment_file = NULL;
	static char *function               = "libewf_internal_handle_open_read_segment_files";
	size64_t maximum_segment_size       = 0;
	size64_t segment_file_size          = 0;
	uint32_t number_of_segments         = 0;
	uint32_t segment_number             = 0;
	int file_io_pool_entry              = 0;
	int last_segment_file               = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - missing read IO handle.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_get_number_of_segments(
	     segment_table,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments from segment table.",
		 function );

		return( -1 );
	}
	/* Make sure to read the device information section first so we
	 * have the correct chunk size when reading Lx01 files.
	 */
	if( libewf_internal_handle_open_read_device_information(
	     internal_handle,
	     file_io_pool,
	     segment_table,
	     number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read device information.",
		 function );

		return( -1 );
	}
	for( segment_number = 0;
	     segment_number < number_of_segments;
	     segment_number++ )
	{
		if( libewf_segment_table_get_segment_by_index(
		     segment_table,
		     segment_number,
		     &file_io_pool_entry,
		     &segment_file_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %" PRIu32 " from segment table.",
			 function,
			 segment_number );

			return( -1 );
		}
		if( ( segment_number == 0 )
		 && ( number_of_segments > 1 ) )
		{
			/* Round the maximum segment size to nearest number of KiB
			 */
			maximum_segment_size = ( segment_file_size >> 10 ) << 10;

			if( libewf_segment_table_set_maximum_segment_size(
			     segment_table,
			     maximum_segment_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set maximum segment size in segment table.",
				 function );

				return( -1 );
			}
		}
		if( libewf_segment_table_get_segment_file_by_index(
		     segment_table,
		     segment_number,
		     file_io_pool,
		     &segment_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %" PRIu32 " from segment table.",
			 function,
			 segment_number );

			return( -1 );
		}
		if( segment_file == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing segment file: %" PRIu32 ".",
			 function,
			 segment_number );

			return( -1 );
		}
		if( segment_file->segment_number != ( segment_number + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: segment number mismatch ( stored: %" PRIu32 ", expected: %" PRIu32 " ).",
			 function,
			 segment_file->segment_number,
			 segment_number + 1 );

			return( -1 );
		}
		if( segment_file->segment_number == 1 )
		{
			internal_handle->io_handle->segment_file_type  = segment_file->type;
			internal_handle->io_handle->major_version      = segment_file->major_version;
			internal_handle->io_handle->minor_version      = segment_file->minor_version;
			internal_handle->io_handle->compression_method = segment_file->compression_method;

			if( segment_file->major_version == 2 )
			{
				if( memory_copy(
				     internal_handle->media_values->set_identifier,
				     segment_file->set_identifier,
				     16 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy segment file set identifier to media values.",
					 function );

					return( -1 );
				}
			}
		}
		else
		{
			if( ( segment_file->major_version != internal_handle->io_handle->major_version )
			 || ( segment_file->minor_version != internal_handle->io_handle->minor_version ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_INPUT,
				 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
				 "%s: segment file format version value mismatch.",
				 function );

				return( -1 );
			}
			if( internal_handle->io_handle->major_version == 2 )
			{
				if( segment_file->compression_method != internal_handle->io_handle->compression_method )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_INPUT,
					 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
					 "%s: segment file compression method value mismatch.",
					 function );

					return( -1 );
				}
				if( memory_compare(
				     internal_handle->media_values->set_identifier,
				     segment_file->set_identifier,
				     16 ) != 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_INPUT,
					 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
					 "%s: segment file set identifier value mismatch.",
					 function );

					return( -1 );
				}
			}
		}
		if( ( segment_file->flags & LIBEWF_SEGMENT_FILE_FLAG_IS_LAST ) != 0 )
		{
			last_segment_file = 1;
		}
		if( ( segment_file->flags & LIBEWF_SEGMENT_FILE_FLAG_IS_ENCRYPTED ) != 0 )
		{
/* TODO get key info */
			internal_handle->io_handle->format       = LIBEWF_FORMAT_V2_ENCASE7;
			internal_handle->io_handle->is_encrypted = 1;
		}
		if( libewf_internal_handle_open_read_segment_file_section_data(
		     internal_handle,
		     segment_file,
		     file_io_pool,
		     file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section data from segment file: %" PRIu32 ".",
			 function,
			 segment_number );

			return( -1 );
		}
		if( ( segment_file->flags & LIBEWF_SEGMENT_FILE_FLAG_IS_CORRUPTED ) != 0 )
		{
			segment_table->flags |= LIBEWF_SEGMENT_TABLE_FLAG_IS_CORRUPTED;
		}
		if( libewf_segment_table_set_segment_storage_media_size_by_index(
		     segment_table,
		     segment_number,
		     segment_file->storage_media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to set mapped range of element: %" PRIu32 " in segment table.",
			 function,
			 segment_number );

			return( -1 );
		}
		internal_handle->read_io_handle->storage_media_size_read += segment_file->storage_media_size;
		internal_handle->read_io_handle->number_of_chunks_read   += segment_file->number_of_chunks;
	}
	if( last_segment_file == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing done section.",
		 function );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
		}
#endif
		libcerror_error_free(
		 error );

		segment_table->flags |= LIBEWF_SEGMENT_TABLE_FLAG_IS_CORRUPTED;
	}
	return( 1 );
}

/* Reads the device information from the segment files
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_open_read_device_information(
     libewf_internal_handle_t *internal_handle,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     uint32_t number_of_segments,
     libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	libewf_segment_file_t *segment_file             = NULL;
	libfcache_cache_t *sections_cache               = NULL;
	static char *function                           = "libewf_internal_handle_open_read_device_information";
	size64_t segment_file_size                      = 0;
	ssize_t read_count                              = 0;
	uint32_t segment_number                         = 0;
	int file_io_pool_entry                          = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( number_of_segments == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of segments value out of bounds.",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_get_segment_file_by_index(
	     segment_table,
	     segment_number,
	     file_io_pool,
	     &segment_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file: %" PRIu32 " from segment table.",
		 function,
		 segment_number );

		goto on_error;
	}
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment file: %" PRIu32 ".",
		 function,
		 segment_number );

		goto on_error;
	}
	if( segment_file->major_version != 2 )
	{
		return( 1 );
	}
	if( segment_file->device_information_section_index == -1 )
	{
		/* Lx01 stores the device information in the last segment file
		 */
		segment_number = number_of_segments - 1;

		if( libewf_segment_table_get_segment_file_by_index(
		     segment_table,
		     segment_number,
		     file_io_pool,
		     &segment_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %" PRIu32 " from segment table.",
			 function,
			 segment_number );

			goto on_error;
		}
		if( segment_file == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing segment file: %" PRIu32 ".",
			 function,
			 segment_number );

			goto on_error;
		}
	}
	if( segment_file->device_information_section_index == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to locate a device information section.",
		 function );

		goto on_error;
	}
	if( libewf_segment_table_get_segment_by_index(
	     segment_table,
	     segment_number,
	     &file_io_pool_entry,
	     &segment_file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment: %" PRIu32 " from segment table.",
		 function,
		 segment_number );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &sections_cache,
	     LIBEWF_MAXIMUM_CACHE_ENTRIES_SECTIONS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sections cache.",
		 function );

		goto on_error;
	}
	if( libfdata_list_get_element_value_by_index(
	     segment_file->sections_list,
	     (intptr_t *) file_io_pool,
	     (libfdata_cache_t *) sections_cache,
	     segment_file->device_information_section_index,
	     (intptr_t **) &section_descriptor,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve section: %d from sections list.",
		 function,
		 segment_file->device_information_section_index );

		goto on_error;
	}
	if( libewf_segment_file_seek_offset(
	     segment_file,
	     file_io_pool,
	     file_io_pool_entry,
	     section_descriptor->start_offset,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to seek section: %d data offset: %" PRIu64 " (0x%08%" PRIx64 ").",
		 function,
		 segment_file->device_information_section_index,
		 section_descriptor->start_offset );

		goto on_error;
	}
	read_count = libewf_device_information_section_read_file_io_pool(
		      section_descriptor,
		      internal_handle->io_handle,
		      file_io_pool,
		      file_io_pool_entry,
		      internal_handle->read_io_handle,
		      internal_handle->media_values,
		      internal_handle->header_values,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read device information section: %d.",
		 function,
		 segment_file->device_information_section_index );

		goto on_error;
	}
	if( libfcache_cache_free(
	     &sections_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sections cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sections_cache != NULL )
	{
		libfcache_cache_free(
		 &sections_cache,
		 NULL );
	}
	return( -1 );
}

/* Opens a set of EWF file(s) using a Basic File IO (bfio) pool
 * This function is not multi-thread safe acquire write lock before call
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_open_file_io_pool(
     libewf_internal_handle_t *internal_handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libewf_segment_table_t *segment_table,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle    = NULL;
	libewf_segment_file_t *segment_file = NULL;
	static char *function               = "libewf_internal_handle_open_file_io_pool";
	size64_t segment_file_size          = 0;
	ssize_t read_count                  = 0;
	int file_io_pool_entry              = 0;
	int number_of_file_io_handles       = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - file IO pool value already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - chunk table value already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_sections != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - hash sections value already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->single_files != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - single files value already set.",
		 function );

		return( -1 );
	}
	if( file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & ~( LIBEWF_ACCESS_FLAG_READ | LIBEWF_ACCESS_FLAG_WRITE | LIBEWF_ACCESS_FLAG_RESUME ) ) != 0 )
	 || ( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	  &&  ( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create read IO handle.",
			 function );

			goto on_error;
		}
	}
	if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	{
		if( libewf_write_io_handle_initialize(
		     &( internal_handle->write_io_handle ),
		     internal_handle->io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create write IO handle.",
			 function );

			goto on_error;
		}
	}
	if( libewf_chunk_table_initialize(
	     &( internal_handle->chunk_table ),
	     internal_handle->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk table.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_initialize(
	     &( internal_handle->header_values ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header values.",
		 function );

		goto on_error;
	}
	if( libewf_hash_sections_initialize(
	     &( internal_handle->hash_sections ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash sections.",
		 function );

		goto on_error;
	}
	if( libcdata_array_empty(
	     internal_handle->sessions,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty sessions array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_empty(
	     internal_handle->tracks,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty tracks array.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_empty(
	     internal_handle->acquiry_errors,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty acquiry errors range list.",
		 function );

		goto on_error;
	}
	if( ( ( access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 || ( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) != 0 ) )
	{
		if( libbfio_pool_get_number_of_handles(
		     file_io_pool,
		     &number_of_file_io_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the number of handles in the pool.",
			 function );

			goto on_error;
		}
		if( libewf_segment_file_initialize(
		     &segment_file,
		     internal_handle->io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create segment file.",
			 function );

			goto on_error;
		}
		for( file_io_pool_entry = 0;
		     file_io_pool_entry < number_of_file_io_handles;
		     file_io_pool_entry++ )
		{
			if( libbfio_pool_get_size(
			     file_io_pool,
			     file_io_pool_entry,
			     &segment_file_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve size of file IO pool entry: %d.",
				 function,
				 file_io_pool_entry );

				goto on_error;
			}
			if( libbfio_pool_get_handle(
			     file_io_pool,
			     file_io_pool_entry,
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to retrieve file IO handle: %d from pool.",
				 function,
				 file_io_pool_entry );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: processing file IO pool entry: %d.\n",
				 function,
				 file_io_pool_entry );
			}
#endif
			read_count = libewf_segment_file_read_file_header_file_io_pool(
				      segment_file,
				      file_io_pool,
				      file_io_pool_entry,
				      error );

			if( read_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read segment file header.",
				 function );

				goto on_error;
			}
			if( segment_file->segment_number == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_INPUT,
				 LIBCERROR_INPUT_ERROR_INVALID_DATA,
				 "%s: invalid segment number: 0 in file IO pool entry: %d.",
				 function,
				 file_io_pool_entry );

				goto on_error;
			}
			if( libewf_segment_table_append_segment_by_segment_file(
			     segment_table,
			     segment_file,
			     file_io_pool_entry,
			     segment_file_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append segment: %" PRIu32 " to segment table.",
				 function,
				 segment_file->segment_number );

				goto on_error;
			}
		}
		if( libewf_segment_file_free(
		     &segment_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment file.",
			 function );

			goto on_error;
		}
		if( libewf_internal_handle_open_read_segment_files(
		     internal_handle,
		     file_io_pool,
		     segment_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read segment files.",
			 function );

			if( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 )
			{
				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
		if( internal_handle->media_values == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing media values.",
			 function );

			goto on_error;
		}
/* TODO refactor */
		if( internal_handle->single_files == NULL )
		{
			if( libewf_internal_handle_get_media_values(
			     internal_handle,
			     &( internal_handle->media_values->media_size ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine media values.",
				 function );

				goto on_error;
			}
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
			     segment_table,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize write IO handle values.",
				 function );

				goto on_error;
			}
		}
		if( libewf_write_io_handle_initialize_resume(
		     internal_handle->write_io_handle,
		     internal_handle->io_handle,
		     file_io_pool,
		     internal_handle->media_values,
		     segment_table,
		     internal_handle->read_io_handle,
		     &( internal_handle->current_offset ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write IO handle to resume.",
			 function );

			goto on_error;
		}
	}
	internal_handle->io_handle->chunk_size   = internal_handle->media_values->chunk_size;
	internal_handle->io_handle->access_flags = access_flags;

	return( 1 );

on_error:
	if( segment_file != NULL )
	{
		libewf_segment_file_free(
		 &segment_file,
		 NULL );
	}
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
	if( internal_handle->header_values != NULL )
	{
		libfvalue_table_free(
		 &( internal_handle->header_values ),
		 NULL );
	}
	if( internal_handle->chunk_table != NULL )
	{
		libewf_chunk_table_free(
		 &( internal_handle->chunk_table ),
		 NULL );
	}
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

/* Opens a set of EWF file(s) using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_open_file_io_pool";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - file IO pool value already set.",
		 function );

		return( -1 );
	}
#if !defined( HAVE_WRITE_SUPPORT )
	if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported - compiled without zlib.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_internal_handle_open_file_io_pool(
	          internal_handle,
	          file_io_pool,
	          access_flags,
	          internal_handle->segment_table,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO pool.",
		 function );
	}
	else
	{
		internal_handle->file_io_pool = file_io_pool;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	result = libcthreads_read_write_lock_release_for_write(
	          internal_handle->read_write_lock,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );
	}
#endif
	if( result != 1 )
	{
		goto on_error;
	}
	return( 1 );

on_error:
	internal_handle->file_io_pool = NULL;

	return( -1 );
}

/* Closes the EWF handle
 * Returns 0 if successful or -1 on error
 */
int libewf_handle_close(
     libewf_handle_t *handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_close";
	ssize_t write_count                       = 0;
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->write_io_handle != NULL )
	 && ( internal_handle->write_io_handle->write_finalized == 0 ) )
	{
		write_count = libewf_internal_handle_write_finalize_file_io_pool(
		               internal_handle,
		               internal_handle->file_io_pool,
		               error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to finalize write.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->file_io_pool_created_in_library != 0 )
	{
		if( libbfio_pool_close_all(
		     internal_handle->file_io_pool,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close all file IO pool handles.",
			 function );

			result = -1;
		}
		if( libbfio_pool_free(
		     &( internal_handle->file_io_pool ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO pool.",
			 function );

			result = -1;
		}
		internal_handle->file_io_pool_created_in_library = 0;
	}
	internal_handle->file_io_pool = NULL;

	if( libewf_io_handle_clear(
	     internal_handle->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( libewf_media_values_clear(
	     internal_handle->media_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear media values.",
		 function );

		result = -1;
	}
	if( internal_handle->read_io_handle != NULL )
	{
		if( libewf_read_io_handle_free(
		     &( internal_handle->read_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free write IO handle.",
			 function );

			result = -1;
		}
	}
	/* Free the chunk data if it could not be passed to libfcache_cache_set_value_by_index
	 */
	if( internal_handle->chunk_data != NULL )
	{
		if( libewf_chunk_data_free(
		     &( internal_handle->chunk_data ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk data.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->chunk_table != NULL )
	{
		if( libewf_chunk_table_free(
		     &( internal_handle->chunk_table ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk table.",
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free hash sections.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->header_values != NULL )
	{
		if( libfvalue_table_free(
		     &( internal_handle->header_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free header values.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->hash_values != NULL )
	{
		if( libfvalue_table_free(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free single files.",
			 function );

			result = -1;
		}
	}
	if( libcdata_array_empty(
	     internal_handle->sessions,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty sessions array.",
		 function );

		result = -1;
	}
	if( libcdata_array_empty(
	     internal_handle->tracks,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty tracks array.",
		 function );

		result = -1;
	}
	if( libcdata_range_list_empty(
	     internal_handle->acquiry_errors,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty acquiry errors range list.",
		 function );

		result = -1;
	}
	if( libewf_segment_table_clear(
	     internal_handle->segment_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear segment table.",
		 function );

		result = -1;
	}
	if( libewf_io_handle_clear(
	     internal_handle->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		result = -1;
	}
#endif
	return( result );
}

/* Reads (media) data from the last current into a buffer using a Basic File IO (bfio) pool
 * This function is not multi-thread safe acquire write lock before call
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 */
ssize_t libewf_internal_handle_read_buffer_from_file_io_pool(
         libewf_internal_handle_t *internal_handle,
         libbfio_pool_t *file_io_pool,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libewf_chunk_data_t *chunk_data = NULL;
	static char *function           = "libewf_internal_handle_read_buffer_from_file_io_pool";
	off64_t chunk_data_offset       = 0;
	size_t buffer_offset            = 0;
	size_t read_size                = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - chunk data set.",
		 function );

		return( -1 );
	}
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid handle - invalid IO handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid media values - missing chunk size.",
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
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size64_t) internal_handle->current_offset >= internal_handle->media_values->media_size )
	{
		return( 0 );
	}
	internal_handle->io_handle->abort = 0;

	if( (size64_t) ( internal_handle->current_offset + buffer_size ) >= internal_handle->media_values->media_size )
	{
		buffer_size = (size_t) ( internal_handle->media_values->media_size - internal_handle->current_offset );
	}
	while( buffer_size > 0 )
	{
		if( libewf_chunk_table_get_chunk_data_by_offset(
		     internal_handle->chunk_table,
		     internal_handle->io_handle,
		     file_io_pool,
		     internal_handle->media_values,
		     internal_handle->segment_table,
		     internal_handle->current_offset,
		     &chunk_data_offset,
		     &chunk_data,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk data for offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 internal_handle->current_offset,
			 internal_handle->current_offset );

			return( -1 );
		}
		if( chunk_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk data for offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 internal_handle->current_offset,
			 internal_handle->current_offset );

			return( -1 );
		}
		if( (off64_t) chunk_data_offset > (off64_t) chunk_data->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: chunk: %" PRIu64 " offset exceeds data size.",
			 function,
			 chunk_data->chunk_index );

			return( -1 );
		}
		read_size = (size_t) ( chunk_data->data_size - chunk_data_offset );

		if( read_size > buffer_size )
		{
			read_size = buffer_size;
		}
		if( read_size == 0 )
		{
			break;
		}
		if( memory_copy(
		     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
		     &( ( chunk_data->data )[ chunk_data_offset ] ),
		     read_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy chunk: %" PRIu64 " data to buffer.",
			 function,
			 chunk_data->chunk_index );

			return( -1 );
		}
		buffer_offset += read_size;
		buffer_size   -= read_size;

		internal_handle->current_offset += (off64_t) read_size;

		if( (size64_t) internal_handle->current_offset >= internal_handle->media_values->media_size )
		{
			break;
		}
		if( internal_handle->io_handle->abort != 0 )
		{
			break;
		}
		chunk_data        = NULL;
		chunk_data_offset = 0;
	}
	internal_handle->io_handle->abort = 0;

	return( (ssize_t) buffer_offset );
}

/* Reads (media) data at the current offset into a buffer
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 */
ssize_t libewf_handle_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_read_buffer";
	ssize_t read_count                        = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libewf_internal_handle_read_buffer_from_file_io_pool(
		      internal_handle,
		      internal_handle->file_io_pool,
		      buffer,
		      buffer_size,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads (media) data at a specific offset
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 */
ssize_t libewf_handle_read_buffer_at_offset(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_read_buffer_at_offset";
	ssize_t read_count                        = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_internal_handle_seek_offset(
	     internal_handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		read_count = -1;
	}
	if( read_count != -1 )
	{
		read_count = libewf_internal_handle_read_buffer_from_file_io_pool(
		              internal_handle,
		              internal_handle->file_io_pool,
		              buffer,
		              buffer_size,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer.",
			 function );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Writes (media) data at the current offset from a buffer using a Basic File IO (bfio) pool
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * This function is not multi-thread safe acquire write lock before call
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
ssize_t libewf_internal_handle_write_buffer_to_file_io_pool(
         libewf_internal_handle_t *internal_handle,
         libbfio_pool_t *file_io_pool,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	static char *function     = "libewf_internal_handle_write_buffer_to_file_io_pool";
	size_t buffer_offset      = 0;
	size_t input_data_size    = 0;
	size_t write_size         = 0;
	ssize_t write_count       = 0;
	off64_t chunk_data_offset = 0;
	uint64_t chunk_index      = 0;
	int write_chunk           = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid handle - invalid IO handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		if( internal_handle->chunk_data != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid handle - chunk data set.",
			 function );

			return( -1 );
		}
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid media values - missing chunk size.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.",
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
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->media_values->media_size != 0 )
	 && ( (size64_t) internal_handle->current_offset >= internal_handle->media_values->media_size ) )
	{
		return( 0 );
	}
	internal_handle->io_handle->abort = 0;

	if( ( internal_handle->media_values->media_size != 0 )
	 && ( (size64_t) ( internal_handle->current_offset + buffer_size ) >= internal_handle->media_values->media_size ) )
	{
		buffer_size = (size_t) ( internal_handle->media_values->media_size - internal_handle->current_offset );
	}
	chunk_index = internal_handle->current_offset / internal_handle->media_values->chunk_size;

	if( chunk_index >= (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk index value exceeds maximum.",
		 function );

		return( -1 );
	}
	chunk_data_offset = chunk_index * internal_handle->media_values->chunk_size;
	chunk_data_offset = internal_handle->current_offset - chunk_data_offset;

	if( chunk_data_offset >= (off64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk data offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	while( buffer_size > 0 )
	{
		if( chunk_index < internal_handle->write_io_handle->number_of_chunks_written )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: chunk: %" PRIu64 " already exists.",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( internal_handle->write_io_handle->write_finalized != 0 )
		{
			break;
		}
		if( internal_handle->chunk_data == NULL )
		{
			if( libewf_chunk_data_initialize(
			     &( internal_handle->chunk_data ),
			     internal_handle->media_values->chunk_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create chunk: %" PRIu64 " data.",
				 function,
				 chunk_index );

				return( -1 );
			}
		}
		if( internal_handle->chunk_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: internal handle - missing chunk: %" PRIu64 " data.",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( chunk_data_offset > internal_handle->media_values->chunk_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: chunk offset exceeds chunk data size.",
			 function );

			return( -1 );
		}
		write_size = (size_t) ( internal_handle->media_values->chunk_size - chunk_data_offset );

		if( write_size > buffer_size )
		{
			write_size = buffer_size;
		}
		if( write_size == 0 )
		{
			break;
		}
		if( memory_copy(
		     &( ( internal_handle->chunk_data->data )[ chunk_data_offset ] ),
		     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
		     write_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy buffer to chunk data.",
			 function );

			return( -1 );
		}
		internal_handle->chunk_data->data_size = (size_t) ( chunk_data_offset + write_size );

		buffer_offset += write_size;
		buffer_size   -= write_size;

		if( internal_handle->chunk_data->data_size == internal_handle->media_values->chunk_size )
		{
			write_chunk = 1;
		}
		else if( ( internal_handle->media_values->media_size != 0 )
		      && ( ( (size64_t) internal_handle->current_offset + write_size ) == internal_handle->media_values->media_size ) )
		{
			write_chunk = 1;
		}
		else
		{
			write_chunk = 0;
		}
		if( write_chunk != 0 )
		{
			input_data_size = internal_handle->chunk_data->data_size;

			if( libewf_chunk_data_pack(
			     internal_handle->chunk_data,
			     internal_handle->io_handle,
			     internal_handle->write_io_handle->compressed_zero_byte_empty_block,
			     internal_handle->write_io_handle->compressed_zero_byte_empty_block_size,
			     internal_handle->write_io_handle->pack_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to pack chunk: %" PRIu64 " data.",
				 function,
				 chunk_index );

				return( -1 );
			}
			write_count = libewf_write_io_handle_write_new_chunk(
			               internal_handle->write_io_handle,
			               internal_handle->io_handle,
			               file_io_pool,
			               internal_handle->media_values,
			               internal_handle->segment_table,
			               internal_handle->header_values,
			               internal_handle->hash_values,
			               internal_handle->hash_sections,
			               internal_handle->sessions,
			               internal_handle->tracks,
			               internal_handle->acquiry_errors,
			               chunk_index,
			               internal_handle->chunk_data,
			               input_data_size,
			               error );

			if( write_count <= 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write new chunk.",
				 function );

				return( -1 );
			}
			if( libewf_chunk_data_free(
			     &( internal_handle->chunk_data ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free chunk data.",
				 function );

				return( -1 );
			}
		}
		chunk_index      += 1;
		chunk_data_offset = 0;

		internal_handle->current_offset += (off64_t) write_size;

		if( ( internal_handle->media_values->media_size != 0 )
		 && ( (size64_t) internal_handle->current_offset >= internal_handle->media_values->media_size ) )
		{
			break;
		}
		if( internal_handle->io_handle->abort != 0 )
		{
			break;
		}
	}
	internal_handle->io_handle->abort = 0;

	return( (ssize_t) buffer_offset );
}

/* Writes (media) data at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
ssize_t libewf_handle_write_buffer(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_buffer";
	ssize_t write_count                       = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->write_io_handle->values_initialized == 0 )
	{
		if( libewf_write_io_handle_initialize_values(
		     internal_handle->write_io_handle,
		     internal_handle->io_handle,
		     internal_handle->media_values,
		     internal_handle->segment_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write IO handle values.",
			 function );

			write_count = -1;
		}
	}
	if( write_count != -1 )
	{
		write_count = libewf_internal_handle_write_buffer_to_file_io_pool(
		               internal_handle,
		               internal_handle->file_io_pool,
		               buffer,
		               buffer_size,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to write buffer.",
			 function );

			write_count = -1;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( write_count );
}

/* Writes (media) data at a specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
ssize_t libewf_handle_write_buffer_at_offset(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_buffer_at_offset";
	ssize_t write_count                       = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->write_io_handle->values_initialized == 0 )
	{
		if( libewf_write_io_handle_initialize_values(
		     internal_handle->write_io_handle,
		     internal_handle->io_handle,
		     internal_handle->media_values,
		     internal_handle->segment_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write IO handle values.",
			 function );

			write_count = -1;
		}
	}
	if( write_count != -1 )
	{
		if( libewf_internal_handle_seek_offset(
		     internal_handle,
		     offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset.",
			 function );

			write_count = -1;
		}
	}
	if( write_count != -1 )
	{
		write_count = libewf_internal_handle_write_buffer_to_file_io_pool(
		               internal_handle,
		               internal_handle->file_io_pool,
		               buffer,
		               buffer_size,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to write buffer.",
			 function );

			write_count = -1;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( write_count );
}

/* Retrieves a (media) data chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_data_chunk(
     libewf_handle_t *handle,
     libewf_data_chunk_t **data_chunk,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_data_chunk";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( data_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data chunk.",
		 function );

		return( -1 );
	}
	if( *data_chunk != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: data chunk value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->write_io_handle != NULL )
	 && ( internal_handle->write_io_handle->values_initialized == 0 ) )
	{
		if( libewf_write_io_handle_initialize_values(
		     internal_handle->write_io_handle,
		     internal_handle->io_handle,
		     internal_handle->media_values,
		     internal_handle->segment_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write IO handle values.",
			 function );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libewf_data_chunk_initialize(
		     data_chunk,
		     internal_handle->io_handle,
		     internal_handle->write_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data chunk.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Reads a (media) data chunk at the current offset
 * This function is not multi-thread safe acquire write lock before call
 * Returns the number of bytes in the data chunk, 0 when no longer data can be read or -1 on error
 */
ssize_t libewf_internal_handle_read_data_chunk_from_file_io_pool(
         libewf_internal_handle_t *internal_handle,
         libbfio_pool_t *file_io_pool,
         libewf_internal_data_chunk_t *internal_data_chunk,
         libcerror_error_t **error )
{
	libewf_chunk_data_t *chunk_data = NULL;
	static char *function           = "libewf_internal_handle_read_data_chunk_from_file_io_pool";
	off64_t chunk_data_offset       = 0;
	size_t read_count               = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - chunk data set.",
		 function );

		return( -1 );
	}
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid handle - invalid IO handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( (size64_t) internal_handle->current_offset >= internal_handle->media_values->media_size )
	{
		return( 0 );
	}
	if( libewf_chunk_table_get_chunk_data_by_offset_no_cache(
	     internal_handle->chunk_table,
	     internal_handle->io_handle,
	     file_io_pool,
	     internal_handle->media_values,
	     internal_handle->segment_table,
	     internal_handle->current_offset,
	     &chunk_data_offset,
	     &chunk_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk data for offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 internal_handle->current_offset,
		 internal_handle->current_offset );

		goto on_error;
	}
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk data for offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 internal_handle->current_offset,
		 internal_handle->current_offset );

		goto on_error;
	}
	/* data_chunk takes over management of chunk_data
	 */
	if( libewf_internal_data_chunk_set_chunk_data(
	     internal_data_chunk,
	     chunk_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk: %" PRIu64 " data in data chunk.",
		 function,
		 chunk_data->chunk_index );

		goto on_error;
	}
	internal_handle->current_offset = chunk_data->range_end_offset;

	read_count = (ssize_t) ( chunk_data->range_end_offset - chunk_data->range_start_offset );

	return( read_count );

on_error:
	if( chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 &chunk_data,
		 NULL );
	}
	return( -1 );
}

/* Reads a (media) data chunk at the current offset
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 * Returns the number of bytes in the data chunk, 0 when no longer data can be read or -1 on error
 */
ssize_t libewf_handle_read_data_chunk(
         libewf_handle_t *handle,
         libewf_data_chunk_t *data_chunk,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_read_data_chunk";
	ssize_t read_count                        = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libewf_internal_handle_read_data_chunk_from_file_io_pool(
	              internal_handle,
	              internal_handle->file_io_pool,
	              (libewf_internal_data_chunk_t *) data_chunk,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data chunk.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Writes a (media) data chunk at the current offset
 * This function is not multi-thread safe acquire write lock before call
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
ssize_t libewf_internal_handle_write_data_chunk_to_file_io_pool(
         libewf_internal_handle_t *internal_handle,
         libbfio_pool_t *file_io_pool,
         libewf_internal_data_chunk_t *internal_data_chunk,
         libcerror_error_t **error )
{
	static char *function        = "libewf_internal_handle_write_data_chunk_to_file_io_pool";
	size_t data_size             = 0;
	ssize_t write_count          = 0;
	uint64_t current_chunk_index = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - chunk data set.",
		 function );

		return( -1 );
	}
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid handle - invalid IO handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid media values - missing chunk size.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.",
		 function );

		return( -1 );
	}
	if( internal_data_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data chunk.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->media_values->media_size != 0 )
	 && ( (size64_t) internal_handle->current_offset >= internal_handle->media_values->media_size ) )
	{
		return( 0 );
	}
	data_size = internal_data_chunk->data_size;

	if( internal_handle->media_values->media_size != 0 )
	{
		if( (size64_t) ( internal_handle->current_offset + data_size ) > internal_handle->media_values->media_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: last data chunk size value out of bounds.",
			 function );

			return( -1 );
		}
	}
/* TODO remove need to calculate */
	current_chunk_index = internal_handle->current_offset / internal_handle->media_values->chunk_size;

	internal_handle->current_offset = (off64_t) current_chunk_index * (off64_t) internal_handle->media_values->chunk_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: writing chunk: %" PRIu64 " of size: %" PRIzd ".\n",
		 function,
		 current_chunk_index,
		 data_size );
	}
#endif
	if( current_chunk_index < internal_handle->write_io_handle->number_of_chunks_written )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: chunk: %" PRIu64 " already exists.",
		 function,
		 current_chunk_index );

		return( -1 );
	}
	write_count = libewf_write_io_handle_write_new_chunk(
	               internal_handle->write_io_handle,
	               internal_handle->io_handle,
	               file_io_pool,
	               internal_handle->media_values,
	               internal_handle->segment_table,
	               internal_handle->header_values,
	               internal_handle->hash_values,
	               internal_handle->hash_sections,
	               internal_handle->sessions,
	               internal_handle->tracks,
	               internal_handle->acquiry_errors,
	               current_chunk_index,
	               internal_data_chunk->chunk_data,
	               data_size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data.",
		 function );

		return( -1 );
	}
	internal_handle->current_offset += (off64_t) data_size;

	return( write_count );
}

/* Writes a (media) data chunk at the current offset
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
ssize_t libewf_handle_write_data_chunk(
         libewf_handle_t *handle,
         libewf_data_chunk_t *data_chunk,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_data_chunk";
	ssize_t write_count                       = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	write_count = libewf_internal_handle_write_data_chunk_to_file_io_pool(
	               internal_handle,
	               internal_handle->file_io_pool,
	               (libewf_internal_data_chunk_t *) data_chunk,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write data chunk.",
		 function );

		write_count = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( write_count );
}

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after write from stream
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_internal_handle_write_finalize_file_io_pool(
         libewf_internal_handle_t *internal_handle,
         libbfio_pool_t *file_io_pool,
         libcerror_error_t **error )
{
	libewf_segment_file_t *segment_file = NULL;
	static char *function               = "libewf_internal_handle_write_finalize_file_io_pool";
	size64_t segment_file_size          = 0;
	size_t input_data_size              = 0;
	ssize_t write_count                 = 0;
	ssize_t write_finalize_count        = 0;
	uint64_t chunk_index                = 0;
	uint32_t number_of_segments         = 0;
	int file_io_pool_entry              = -1;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid handle - invalid IO handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		if( internal_handle->chunk_data != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid handle - chunk data set.",
			 function );

			return( -1 );
		}
	}
	if( internal_handle->write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing write IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid media values - missing chunk size.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_io_handle->write_finalized != 0 )
	{
		return( 0 );
	}
	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		return( 0 );
	}
	if( internal_handle->chunk_data != NULL )
	{
		chunk_index = internal_handle->current_offset / internal_handle->media_values->chunk_size;

		if( chunk_index < internal_handle->write_io_handle->number_of_chunks_written )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: chunk: %" PRIu64 " already exists.",
			 function,
			 chunk_index );

			return( -1 );
		}
		input_data_size = internal_handle->chunk_data->data_size;

		if( libewf_chunk_data_pack(
		     internal_handle->chunk_data,
		     internal_handle->io_handle,
		     internal_handle->write_io_handle->compressed_zero_byte_empty_block,
		     internal_handle->write_io_handle->compressed_zero_byte_empty_block_size,
		     internal_handle->write_io_handle->pack_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to pack chunk: %" PRIu64 " data.",
			 function,
			 chunk_index );

			return( -1 );
		}
		write_count = libewf_write_io_handle_write_new_chunk(
			       internal_handle->write_io_handle,
			       internal_handle->io_handle,
			       file_io_pool,
			       internal_handle->media_values,
			       internal_handle->segment_table,
			       internal_handle->header_values,
			       internal_handle->hash_values,
			       internal_handle->hash_sections,
			       internal_handle->sessions,
			       internal_handle->tracks,
			       internal_handle->acquiry_errors,
			       chunk_index,
			       internal_handle->chunk_data,
			       input_data_size,
			       error );

		if( write_count <= 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write new chunk.",
			 function );

			return( -1 );
		}
		write_finalize_count += write_count;

		if( libewf_chunk_data_free(
		     &( internal_handle->chunk_data ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk data.",
			 function );

			return( -1 );
		}
	}
	/* Check if all media data has been written
	 */
	if( ( internal_handle->media_values->media_size != 0 )
	 && ( internal_handle->write_io_handle->input_write_count < (ssize64_t) internal_handle->media_values->media_size ) )
	{
		return( write_finalize_count );
	}
	if( libewf_segment_table_get_number_of_segments(
	     internal_handle->segment_table,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments.",
		 function );

		return( -1 );
	}
	if( number_of_segments == 0 )
	{
		/* No segment file needs to be created
		 */
/* TODO clean up this logic: when is this triggered? */
		if( internal_handle->media_values->media_size != 0 )
		{
			return( write_finalize_count );
		}
		/* Create the segment file if required
		 */
		if( libewf_write_io_handle_create_segment_file(
		     internal_handle->io_handle,
		     file_io_pool,
		     internal_handle->segment_table,
		     internal_handle->io_handle->segment_file_type,
		     0,
		     internal_handle->write_io_handle->maximum_number_of_segments,
		     internal_handle->media_values->set_identifier,
		     &file_io_pool_entry,
		     &segment_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to create segment file: 0.",
			 function );

			return( -1 );
		}
		write_count = libewf_segment_file_write_start(
		               segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               &( internal_handle->write_io_handle->case_data ),
		               &( internal_handle->write_io_handle->case_data_size ),
		               &( internal_handle->write_io_handle->device_information ),
		               &( internal_handle->write_io_handle->device_information_size ),
		               &( internal_handle->write_io_handle->data_section ),
		               internal_handle->media_values,
		               internal_handle->header_values,
		               internal_handle->write_io_handle->timestamp,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write segment file: 0 start.",
			 function );

			return( -1 );
		}
		write_finalize_count += write_count;
	}
	/* Check if the last segment file is still open for writing
	 */
	else if( internal_handle->write_io_handle->current_segment_file != NULL )
	{
		if( libewf_segment_table_get_segment_by_index(
		     internal_handle->segment_table,
		     internal_handle->write_io_handle->current_segment_number,
		     &file_io_pool_entry,
		     &segment_file_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %" PRIu32 " from segment table.",
			 function,
			 internal_handle->write_io_handle->current_segment_number );

			return( -1 );
		}
		/* Check if chunks section needs to be corrected
		 */
		if( internal_handle->write_io_handle->chunks_section_offset != 0 )
		{
			write_count = libewf_write_io_handle_write_chunks_section_end(
			               internal_handle->write_io_handle,
			               internal_handle->io_handle,
				       file_io_pool,
				       file_io_pool_entry,
			               internal_handle->write_io_handle->current_segment_file,
			               error );

			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write chunks section end: %" PRIu32 ".",
				 function,
				 internal_handle->write_io_handle->current_segment_number );

				return( -1 );
			}
			write_finalize_count += write_count;
		}
		/* Close the segment file
		 */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: closing last segment file: %" PRIu32 ".\n",
			 function,
			 internal_handle->write_io_handle->current_segment_number );
		}
#endif
		write_count = libewf_segment_file_write_close(
		               internal_handle->write_io_handle->current_segment_file,
		               file_io_pool,
		               file_io_pool_entry,
		               internal_handle->write_io_handle->number_of_chunks_written_to_segment_file,
		               1,
		               internal_handle->hash_sections,
		               internal_handle->hash_values,
		               internal_handle->media_values,
		               internal_handle->sessions,
		               internal_handle->tracks,
		               internal_handle->acquiry_errors,
		               &( internal_handle->write_io_handle->data_section ),
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to close segment file: %" PRIu32 ".",
			 function,
			 internal_handle->write_io_handle->current_segment_number );

			return( -1 );
		}
		write_finalize_count += write_count;

		internal_handle->write_io_handle->current_segment_file = NULL;
	}
	/* Correct the media values if streamed write was used
	 */
	if( internal_handle->media_values->media_size == 0 )
	{
		/* Determine the media values
		 */
		internal_handle->media_values->number_of_chunks  = internal_handle->write_io_handle->number_of_chunks_written;
		internal_handle->media_values->number_of_sectors = (uint64_t) ( internal_handle->write_io_handle->input_write_count / internal_handle->media_values->bytes_per_sector );
		internal_handle->media_values->media_size        = (size64_t) internal_handle->write_io_handle->input_write_count;

		/* Flush the section write caches
		 */
		if( internal_handle->write_io_handle->case_data != NULL )
		{
			memory_free(
			 internal_handle->write_io_handle->case_data );

			internal_handle->write_io_handle->case_data      = NULL;
			internal_handle->write_io_handle->case_data_size = 0;
		}
		if( internal_handle->write_io_handle->device_information != NULL )
		{
			memory_free(
			 internal_handle->write_io_handle->device_information );

			internal_handle->write_io_handle->device_information      = NULL;
			internal_handle->write_io_handle->device_information_size = 0;
		}
		if( internal_handle->write_io_handle->data_section != NULL )
		{
			memory_free(
			 internal_handle->write_io_handle->data_section );

			internal_handle->write_io_handle->data_section = NULL;
		}
		/* Correct the sections in the segment files
		 */
		if( libewf_write_io_handle_finalize_write_sections_corrections(
		     internal_handle->write_io_handle,
		     file_io_pool,
		     internal_handle->media_values,
		     internal_handle->segment_table,
		     internal_handle->header_values,
		     internal_handle->hash_values,
		     internal_handle->hash_sections,
		     internal_handle->sessions,
		     internal_handle->tracks,
		     internal_handle->acquiry_errors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sections corrections to segment files.",
			 function );

			return( -1 );
		}
	}
	internal_handle->write_io_handle->write_finalized = 1;

	return( write_finalize_count );
}

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after write from stream
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_handle_write_finalize(
         libewf_handle_t *handle,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_finalize";
	ssize_t write_count                       = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	write_count = libewf_internal_handle_write_finalize_file_io_pool(
	               internal_handle,
	               internal_handle->file_io_pool,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write finalize.",
		 function );

		write_count = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( write_count );
}

/* Seeks a certain offset of the (media) data
 * This function is not multi-thread safe acquire write lock before call
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_internal_handle_seek_offset(
         libewf_internal_handle_t *internal_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libewf_internal_handle_seek_offset";

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - chunk data set.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid media values - missing chunk size.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{	
		offset += internal_handle->current_offset;
	}
	else if( whence == SEEK_END )
	{	
		offset += (off64_t) internal_handle->media_values->media_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: seeking media data offset: %" PRIi64 ".\n",
		 function,
		 offset );
	}
#endif
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	internal_handle->current_offset = offset;

	return( offset );
}

/* Seeks a certain offset of the (media) data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_handle_seek_offset(
         libewf_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_seek_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	offset = libewf_internal_handle_seek_offset(
	          internal_handle,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Retrieves the current offset of the (media) data
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_offset(
     libewf_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

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
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*offset = internal_handle->current_offset;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_maximum_number_of_open_handles(
     libewf_handle_t *handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_maximum_number_of_open_handles";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->file_io_pool != NULL )
	{
		result = libbfio_pool_set_maximum_number_of_open_handles(
		          internal_handle->file_io_pool,
		          maximum_number_of_open_handles,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set maximum number of open handles in file IO pool.",
			 function );
		}
	}
	if( result == 1 )
	{
		internal_handle->maximum_number_of_open_handles = maximum_number_of_open_handles;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determine if the segment files are corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libewf_handle_segment_files_corrupted(
     libewf_handle_t *handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_segment_files_corrupted";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = ( internal_handle->segment_table->flags & LIBEWF_SEGMENT_TABLE_FLAG_IS_CORRUPTED ) != 0;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determine if the segment files are encrypted
 * Returns 1 if encrypted, 0 if not or -1 on error
 */
int libewf_handle_segment_files_encrypted(
     libewf_handle_t *handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_segment_files_encrypted";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = (int) internal_handle->io_handle->is_encrypted;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the segment filename size
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_segment_table_get_basename_size(
	          internal_handle->segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_segment_table_get_basename(
	          internal_handle->segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_filename";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: segment filename cannot be changed.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_segment_table_set_basename(
	          internal_handle->segment_table,
	          filename,
	          filename_length,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename_size_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_segment_table_get_basename_size_wide(
	          internal_handle->segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_segment_table_get_basename_wide(
	          internal_handle->segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_filename_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: segment filename cannot be changed.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_segment_table_set_basename_wide(
	          internal_handle->segment_table,
	          filename,
	          filename_length,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Retrieves the maximum segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t *maximum_segment_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_maximum_segment_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( maximum_segment_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum segment size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*maximum_segment_size = internal_handle->segment_table->maximum_segment_size;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the maximum segment file size
 * A maximum segment file size of 0 represents the maximum possible size for the format
 * If the maximum segment file size is smaller than the size needed to store a single chunk
 * the size off the latter is enforced and not the maximum segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t maximum_segment_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_maximum_segment_size";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( maximum_segment_size > (size64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum segment size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: maximum segment size cannot be changed.",
		 function );

		result = -1;
	}
	else if( maximum_segment_size > internal_handle->write_io_handle->maximum_segment_file_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment file size value out of bounds.",
		 function );

		result = -1;
	}
	else
	{
		internal_handle->segment_table->maximum_segment_size = maximum_segment_size;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the filename size of the segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
int libewf_handle_get_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libbfio_handle_t *file_io_handle          = NULL;
	static char *function                     = "libewf_handle_get_filename_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_internal_handle_get_file_io_handle(
	          internal_handle,
	          &file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current chunk.",
		 function );
	}
	else if( result != 0 )
	{
		result = libbfio_file_get_name_size(
		          file_io_handle,
		          filename_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filename size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the filename of the segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
int libewf_handle_get_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libbfio_handle_t *file_io_handle          = NULL;
	static char *function                     = "libewf_handle_get_filename";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_internal_handle_get_file_io_handle(
	          internal_handle,
	          &file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current chunk.",
		 function );
	}
	else if( result != 0 )
	{
		result = libbfio_file_get_name(
		          file_io_handle,
		          filename,
		          filename_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filename.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the segment file of the current chunk
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
int libewf_handle_get_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libbfio_handle_t *file_io_handle          = NULL;
	static char *function                     = "libewf_handle_get_filename_size_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_internal_handle_get_file_io_handle(
	          internal_handle,
	          &file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current chunk.",
		 function );
	}
	else if( result != 0 )
	{
		result = libbfio_file_get_name_size_wide(
		          file_io_handle,
		          filename_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filename size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the filename of the segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
int libewf_handle_get_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libbfio_handle_t *file_io_handle          = NULL;
	static char *function                     = "libewf_handle_get_filename_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_internal_handle_get_file_io_handle(
	          internal_handle,
	          &file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current chunk.",
		 function );
	}
	else if( result != 0 )
	{
		result = libbfio_file_get_name_wide(
		          file_io_handle,
		          filename,
		          filename_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filename.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

#endif

/* Retrieves the file IO handle of the segment file of the current chunk
 * This function is not multi-thread safe acquire write lock before call
 * Returns 1 if successful, 0 if no such file IO handle or -1 on error
 */
int libewf_internal_handle_get_file_io_handle(
     libewf_internal_handle_t *internal_handle,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	static char *function      = "libewf_internal_handle_get_file_io_handle";
	size64_t segment_file_size = 0;
	uint64_t chunk_index       = 0;
	int file_io_pool_entry     = 0;
	int result                 = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid handle - invalid IO handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid media values - missing chunk size.",
		 function );

		return( -1 );
	}
	chunk_index = internal_handle->current_offset / internal_handle->media_values->chunk_size;

	result = libewf_segment_table_get_segment_at_offset(
	          internal_handle->segment_table,
	          internal_handle->current_offset,
	          &file_io_pool_entry,
	          &segment_file_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment at offset: %" PRIi64 " (0x%08" PRIx64 ") from segment table.",
		 function,
		 internal_handle->current_offset,
		 internal_handle->current_offset );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( file_io_pool_entry == -1 )
		{
			return( 0 );
		}
		if( libbfio_pool_get_handle(
		     internal_handle->file_io_pool,
		     file_io_pool_entry,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file IO handle: %d from pool (chunk: %" PRIu64 ").",
			 function,
			 file_io_pool_entry,
			 chunk_index );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the file IO handle of the segment file of the current chunk
 * Returns 1 if successful, 0 if no such file IO handle or -1 on error
 */
int libewf_handle_get_file_io_handle(
     libewf_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_file_io_handle";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_internal_handle_get_file_io_handle(
	          internal_handle,
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current chunk.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the media values
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_get_media_values(
     libewf_internal_handle_t *internal_handle,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libewf_chunk_data_t *chunk_data = NULL;
	static char *function           = "libewf_internal_handle_get_media_values";
	size64_t chunks_data_size       = 0;
	size64_t sector_data_size       = 0;
	size32_t chunk_size             = 0;
	off64_t chunk_data_offset       = 0;
	uint64_t chunk_index            = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
	sector_data_size = (size64_t) internal_handle->media_values->number_of_sectors * internal_handle->media_values->bytes_per_sector;

	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		chunk_size = (size32_t) internal_handle->media_values->sectors_per_chunk * internal_handle->media_values->bytes_per_sector;

		if( internal_handle->media_values->number_of_chunks > 0 )
		{
			internal_handle->current_offset = ( (off64_t) internal_handle->media_values->number_of_chunks - 1 ) * chunk_size;

			if( libewf_chunk_table_get_chunk_data_by_offset(
			     internal_handle->chunk_table,
			     internal_handle->io_handle,
			     internal_handle->file_io_pool,
			     internal_handle->media_values,
			     internal_handle->segment_table,
			     internal_handle->current_offset,
			     &chunk_data_offset,
			     &chunk_data,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve chunk data for offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 internal_handle->current_offset,
				 internal_handle->current_offset );

				return( -1 );
			}
			if( chunk_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing chunk data for offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 internal_handle->current_offset,
				 internal_handle->current_offset );

				return( -1 );
			}
			/* The only way to determine the size of the last compressed chunk is to unpack it
			 */
			if( libewf_chunk_data_unpack(
			     chunk_data,
			     internal_handle->io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to unpack chunk: %" PRIu64 " data.",
				 function,
				 chunk_index );

				return( -1 );
			}
			chunks_data_size = chunk_index * chunk_size;

			if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) == 0 )
			{
				chunks_data_size += chunk_data->data_size;
			}
			if( sector_data_size != chunks_data_size )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: mismatch of media data size in volume: %" PRIu64 " and chunk table: %" PRIu64 "\n",
					 function,
					 sector_data_size,
					 chunks_data_size );
				}
#endif
				if( sector_data_size < chunks_data_size )
				{
					sector_data_size = chunks_data_size;
				}
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
     libcerror_error_t **error )
{
	static char *function            = "libewf_internal_handle_set_media_values";
	size32_t chunk_size              = 0;
	size64_t maximum_input_file_size = 0;
	uint64_t number_of_chunks        = 0;
	uint64_t number_of_sectors       = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( ( sectors_per_chunk == 0 )
	 || ( sectors_per_chunk > (uint32_t) INT32_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sectors per chunk.",
		 function );

		return( -1 );
	}
	if( ( bytes_per_sector == 0 )
	 || ( bytes_per_sector > (uint32_t) INT32_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid number of chunks value exceeds maximum.",
			 function );

			return( -1 );
		}
		internal_handle->media_values->number_of_chunks = (uint64_t) number_of_chunks;

		/* Determine the number of sectors to write
		 */
		number_of_sectors = (uint64_t) media_size / (uint64_t) bytes_per_sector;

		if( number_of_sectors > (uint64_t) INT64_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid number of sectors value exceeds maximum.",
			 function );

			return( -1 );
		}
		internal_handle->media_values->number_of_sectors = number_of_sectors;
	}
	return( 1 );
}

/* Retrieves the root (single) file entry
 * Returns 1 if successful, 0 if no file entries are present or -1 on error
 */
int libewf_handle_get_root_file_entry(
     libewf_handle_t *handle,
     libewf_file_entry_t **root_file_entry,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *root_node           = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_root_file_entry";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing single files.",
		 function );

		return( -1 );
	}
	if( root_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root file entry.",
		 function );

		return( -1 );
	}
	if( *root_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: root file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_single_files_get_file_entry_tree_root_node(
	     internal_handle->single_files,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry tree root node.",
		 function );

		result = -1;
	}
	else if( root_node != NULL )
	{
		result = libewf_file_entry_initialize(
		          root_file_entry,
		          handle,
		          internal_handle->single_files,
		          root_node,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create root file entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the (single) file entry for the specific UTF-8 encoded path
 * The path separator is the \ character
 * This function is not multi-thread safe acquire write lock before call
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libewf_internal_handle_get_file_entry_by_utf8_path(
     libewf_internal_handle_t *internal_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *node                  = NULL;
	libcdata_tree_node_t *root_node             = NULL;
	libcdata_tree_node_t *sub_node              = NULL;
	libewf_lef_file_entry_t *lef_file_entry     = NULL;
	libewf_lef_file_entry_t *sub_lef_file_entry = NULL;
	uint8_t *utf8_string_segment                = NULL;
	static char *function                       = "libewf_handle_get_file_entry_by_utf8_path";
	size_t utf8_string_index                    = 0;
	size_t utf8_string_segment_length           = 0;
	int result                                  = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing single files.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: file entry value already set.",
		 function );

		return( -1 );
	}
	if( libewf_single_files_get_file_entry_tree_root_node(
	     internal_handle->single_files,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry tree root node.",
		 function );

		result = -1;
	}
	else if( root_node == NULL )
	{
		return( 0 );
	}
	if( libcdata_tree_node_get_value(
	     root_node,
	     (intptr_t **) &lef_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from root file entry node.",
		 function );

		return( -1 );
	}
	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing root file entry values.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf8_string[ utf8_string_index ] == (uint8_t) LIBEWF_SEPARATOR )
		{
			utf8_string_index++;
		}
	}
	node = root_node;

	if( ( utf8_string_length == 0 )
	 || ( utf8_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf8_string_index < utf8_string_length )
	{
		utf8_string_segment        = (uint8_t *) &( utf8_string[ utf8_string_index ] );
		utf8_string_segment_length = 0;

		while( utf8_string_index < utf8_string_length )
		{
			if( ( utf8_string[ utf8_string_index ] == (uint8_t) LIBEWF_SEPARATOR )
			 || ( utf8_string[ utf8_string_index ] == (uint8_t) 0 ) )
			{
				utf8_string_index++;

				break;
			}
			utf8_string_index++;

			utf8_string_segment_length++;
		}
		if( utf8_string_segment_length == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub file entry name.",
			 function );

			return( -1 );
		}
		result = libewf_single_file_tree_get_sub_node_by_utf8_name(
			  node,
			  utf8_string_segment,
			  utf8_string_segment_length,
			  &sub_node,
			  &sub_lef_file_entry,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry sub node by name.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			break;
		}
		node = sub_node;
	}
	if( result != 0 )
	{
		if( libewf_file_entry_initialize(
		     file_entry,
		     (libewf_handle_t *) internal_handle,
		     internal_handle->single_files,
		     node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the (single) file entry for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libewf_handle_get_file_entry_by_utf8_path(
     libewf_handle_t *handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_file_entry_by_utf8_path";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_internal_handle_get_file_entry_by_utf8_path(
	          internal_handle,
	          utf8_string,
	          utf8_string_length,
	          file_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry by UTF-8 path.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the (single) file entry for the specific UTF-16 encoded path
 * The path separator is the \ character
 * This function is not multi-thread safe acquire write lock before call
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libewf_internal_handle_get_file_entry_by_utf16_path(
     libewf_internal_handle_t *internal_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *node                  = NULL;
	libcdata_tree_node_t *root_node             = NULL;
	libcdata_tree_node_t *sub_node              = NULL;
	libewf_lef_file_entry_t *lef_file_entry     = NULL;
	libewf_lef_file_entry_t *sub_lef_file_entry = NULL;
	uint16_t *utf16_string_segment              = NULL;
	static char *function                       = "libewf_handle_get_file_entry_by_utf16_path";
	size_t utf16_string_index                   = 0;
	size_t utf16_string_segment_length          = 0;
	int result                                  = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->single_files == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing single files.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: file entry value already set.",
		 function );

		return( -1 );
	}
	if( libewf_single_files_get_file_entry_tree_root_node(
	     internal_handle->single_files,
	     &root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry tree root node.",
		 function );

		result = -1;
	}
	else if( root_node == NULL )
	{
		return( 0 );
	}
	if( libcdata_tree_node_get_value(
	     root_node,
	     (intptr_t **) &lef_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from root file entry node.",
		 function );

		return( -1 );
	}
	if( lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing root file entry values.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf16_string[ utf16_string_index ] == (uint16_t) LIBEWF_SEPARATOR )
		{
			utf16_string_index++;
		}
	}
	node = root_node;

	if( ( utf16_string_length == 0 )
	 || ( utf16_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf16_string_index < utf16_string_length )
	{
		utf16_string_segment        = (uint16_t *) &( utf16_string[ utf16_string_index ] );
		utf16_string_segment_length = 0;

		while( utf16_string_index < utf16_string_length )
		{
			if( ( utf16_string[ utf16_string_index ] == (uint16_t) LIBEWF_SEPARATOR )
			 || ( utf16_string[ utf16_string_index ] == (uint16_t) 0 ) )
			{
				utf16_string_index++;

				break;
			}
			utf16_string_index++;

			utf16_string_segment_length++;
		}
		if( utf16_string_segment_length == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub file entry name.",
			 function );

			return( -1 );
		}
		result = libewf_single_file_tree_get_sub_node_by_utf16_name(
			  node,
			  utf16_string_segment,
			  utf16_string_segment_length,
			  &sub_node,
			  &sub_lef_file_entry,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry sub node by name.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			break;
		}
		node = sub_node;
	}
	if( result != 0 )
	{
		if( libewf_file_entry_initialize(
		     file_entry,
		     (libewf_handle_t *) internal_handle,
		     internal_handle->single_files,
		     node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the (single) file entry for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libewf_handle_get_file_entry_by_utf16_path(
     libewf_handle_t *handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_file_entry_by_utf16_path";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_internal_handle_get_file_entry_by_utf16_path(
	          internal_handle,
	          utf16_string,
	          utf16_string_length,
	          file_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry by UTF-16 path.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t *sectors_per_chunk,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_sectors_per_chunk";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( sectors_per_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sectors per chunk.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->sectors_per_chunk > (uint32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid sectors per chunk value exceeds maximum.",
		 function );

		goto on_error;
	}
	*sectors_per_chunk = internal_handle->media_values->sectors_per_chunk;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the number of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t sectors_per_chunk,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_sectors_per_chunk";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: sectors per chunk cannot be changed.",
		 function );

		goto on_error;
	}
	if( libewf_internal_handle_set_media_values(
	     internal_handle,
	     sectors_per_chunk,
	     internal_handle->media_values->bytes_per_sector,
	     internal_handle->media_values->media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media values.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the number of bytes per sector
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_bytes_per_sector";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->bytes_per_sector > (uint32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid bytes per sector value exceeds maximum.",
		 function );

		goto on_error;
	}
	*bytes_per_sector = internal_handle->media_values->bytes_per_sector;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the number of bytes per sector
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t bytes_per_sector,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_bytes_per_sector";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: bytes per sector cannot be changed.",
		 function );

		goto on_error;
	}
	if( libewf_internal_handle_set_media_values(
	     internal_handle,
	     internal_handle->media_values->sectors_per_chunk,
	     bytes_per_sector,
	     internal_handle->media_values->media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media values.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the number of sectors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_sectors(
     libewf_handle_t *handle,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_sectors";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( number_of_sectors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->number_of_sectors > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of sectors value exceeds maximum.",
		 function );

		goto on_error;
	}
	*number_of_sectors = internal_handle->media_values->number_of_sectors;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_chunk_size(
     libewf_handle_t *handle,
     size32_t *chunk_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_chunk_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->chunk_size > (size32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		goto on_error;
	}
	*chunk_size = internal_handle->media_values->chunk_size;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the error granularity
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_error_granularity(
     libewf_handle_t *handle,
     uint32_t *error_granularity,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_error_granularity";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( error_granularity == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid error granularity.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->error_granularity > (uint32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid error granularity value exceeds maximum.",
		 function );

		goto on_error;
	}
	*error_granularity = internal_handle->media_values->error_granularity;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the error granularity
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_error_granularity(
     libewf_handle_t *handle,
     uint32_t error_granularity,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_error_granularity";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: error granularity cannot be changed.",
		 function );

		goto on_error;
	}
	internal_handle->media_values->error_granularity = error_granularity;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the compression method
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_compression_method(
     libewf_handle_t *handle,
     uint16_t *compression_method,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_compression_method";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( compression_method == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression method.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*compression_method = internal_handle->io_handle->compression_method;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the compression method
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_compression_method(
     libewf_handle_t *handle,
     uint16_t compression_method,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_compression_method";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( compression_method != LIBEWF_COMPRESSION_METHOD_DEFLATE )
	 && ( compression_method != LIBEWF_COMPRESSION_METHOD_BZIP2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression method.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: compression values cannot be changed.",
		 function );

		goto on_error;
	}
	if( ( compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
	 && ( internal_handle->io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 && ( internal_handle->io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: compression method not supported by format.",
		 function );

		goto on_error;
	}
	internal_handle->io_handle->compression_method = compression_method;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the compression values
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_compression_values(
     libewf_handle_t *handle,
     int8_t *compression_level,
     uint8_t *compression_flags,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_compression_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( compression_level == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression level.",
		 function );

		return( -1 );
	}
	if( compression_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression flags.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*compression_level = internal_handle->io_handle->compression_level;
	*compression_flags = internal_handle->io_handle->compression_flags;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the compression values
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_compression_values(
     libewf_handle_t *handle,
     int8_t compression_level,
     uint8_t compression_flags,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_compression_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( compression_level != LIBEWF_COMPRESSION_LEVEL_NONE )
	 && ( compression_level != LIBEWF_COMPRESSION_LEVEL_FAST )
	 && ( compression_level != LIBEWF_COMPRESSION_LEVEL_BEST ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression level.",
		 function );

		return( -1 );
	}
	if( ( compression_flags & ~( LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression flags.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: compression values cannot be changed.",
		 function );

		goto on_error;
	}
	if( ( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 && ( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		compression_flags |= LIBEWF_COMPRESS_FLAG_USE_PATTERN_FILL_COMPRESSION;
	}
	internal_handle->io_handle->compression_level = compression_level;
	internal_handle->io_handle->compression_flags = compression_flags;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the contained media data
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_media_size(
     libewf_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_media_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*media_size = internal_handle->media_values->media_size;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the media size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_media_size(
     libewf_handle_t *handle,
     size64_t media_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_media_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		goto on_error;
	}
	if( libewf_internal_handle_set_media_values(
	     internal_handle,
	     internal_handle->media_values->sectors_per_chunk,
	     internal_handle->media_values->bytes_per_sector,
	     media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set media values.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the media type value
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_media_type(
     libewf_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_media_type";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->media_type > (uint8_t) INT8_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid media type value exceeds maximum.",
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
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*media_type = internal_handle->media_values->media_type;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the media type
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_media_type(
     libewf_handle_t *handle,
     uint8_t media_type,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_media_type";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media type cannot be changed.",
		 function );

		goto on_error;
	}
	internal_handle->media_values->media_type = media_type;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the media flags
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_media_flags(
     libewf_handle_t *handle,
     uint8_t *media_flags,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_media_flags";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( media_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media flags.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->media_values->media_flags > (uint8_t) INT8_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid media flags value exceeds maximum.",
		 function );

		goto on_error;
	}
	*media_flags = internal_handle->media_values->media_flags;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the media flags
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_media_flags(
     libewf_handle_t *handle,
     uint8_t media_flags,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_media_flags";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media flags cannot be changed.",
		 function );

		goto on_error;
	}
	internal_handle->media_values->media_flags = media_flags;

	/* Make sure the lowest bit is always set
	 */
	internal_handle->media_values->media_flags |= 0x01;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the format type value
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_format(
     libewf_handle_t *handle,
     uint8_t *format,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_format";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->io_handle->format > (uint8_t) INT8_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid format value exceeds maximum.",
		 function );

		result = -1;
	}
	else
	{
		*format = internal_handle->io_handle->format;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the output format
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_format(
     libewf_handle_t *handle,
     uint8_t format,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_format";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
	 && ( format != LIBEWF_FORMAT_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK_IMAGER )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_LINEN7 )
	 && ( format != LIBEWF_FORMAT_V2_ENCASE7 )
/* TODO add support for: L01, Lx01:
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 )
*/
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format: %d.",
		 function,
	         format );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: format cannot be changed.",
		 function );

		goto on_error;
	}
/* TODO refactor into separate function */
	internal_handle->io_handle->format = format;

	if( format == LIBEWF_FORMAT_V2_ENCASE7 )
	{
		internal_handle->io_handle->major_version = 2;
		internal_handle->io_handle->minor_version = 1;
	}
	else
	{
		internal_handle->io_handle->major_version = 1;
		internal_handle->io_handle->minor_version = 0;
	}
	if( ( format == LIBEWF_FORMAT_EWF )
	 || ( format == LIBEWF_FORMAT_SMART ) )
	{
		/* Wraps .s01 to .s99 and then to .saa up to .zzz
		 * ( ( ( 's' to 'z' = 8 ) * 26 * 26 ) + 99 ) = 5507
		 */
		internal_handle->write_io_handle->maximum_number_of_segments = (uint32_t) 5507;
		internal_handle->io_handle->segment_file_type                = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
	}
	else if( format == LIBEWF_FORMAT_V2_ENCASE7 )
	{
		/* Wraps .Ex01 to .Ex99 and then to .ExAA up to .EzZZ
		 * ( ( ( 'x' to 'z' = 3 ) * 26 * 26 ) + 99 ) = 2127
		 */
		internal_handle->write_io_handle->maximum_number_of_segments = (uint32_t) 2127;
		internal_handle->io_handle->segment_file_type                = LIBEWF_SEGMENT_FILE_TYPE_EWF2;
	}
	else
	{
		/* Wraps .E01 to .E99 and then to .EAA up to .ZZZ
		 * ( ( ( 'E' to 'Z' or 'e' to 'z' = 22 ) * 26 * 26 ) + 99 ) = 14971
		 */
		internal_handle->write_io_handle->maximum_number_of_segments = (uint32_t) 14971;
		internal_handle->io_handle->segment_file_type                = LIBEWF_SEGMENT_FILE_TYPE_EWF1;
	}
	/* Determine the maximum number of table entries
	 */
	if( ( format == LIBEWF_FORMAT_ENCASE6 )
	 || ( format == LIBEWF_FORMAT_ENCASE7 ) )
	{
		internal_handle->write_io_handle->maximum_segment_file_size  = INT64_MAX;
		internal_handle->write_io_handle->maximum_chunks_per_section = LIBEWF_MAXIMUM_TABLE_ENTRIES_ENCASE6;
	}
	else if( format == LIBEWF_FORMAT_V2_ENCASE7 )
	{
		internal_handle->write_io_handle->maximum_segment_file_size  = INT64_MAX;
		internal_handle->write_io_handle->maximum_chunks_per_section = LIBEWF_MAXIMUM_TABLE_ENTRIES;
	}
	else if( format == LIBEWF_FORMAT_EWFX )
	{
		internal_handle->write_io_handle->maximum_segment_file_size  = INT32_MAX;
		internal_handle->write_io_handle->maximum_chunks_per_section = LIBEWF_MAXIMUM_TABLE_ENTRIES;
	}
	else
	{
		internal_handle->write_io_handle->maximum_segment_file_size  = INT32_MAX;
		internal_handle->write_io_handle->maximum_chunks_per_section = LIBEWF_MAXIMUM_TABLE_ENTRIES_EWF;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the segment file version
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_segment_file_version(
     libewf_handle_t *handle,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_file_version";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( major_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid major version.",
		 function );

		return( -1 );
	}
	if( minor_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid minor version.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*major_version = internal_handle->io_handle->major_version;
	*minor_version = internal_handle->io_handle->minor_version;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the segment file set identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_segment_file_set_identifier(
     libewf_handle_t *handle,
     uint8_t *set_identifier,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_file_set_identifier";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( set_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid set identifier.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: set identifier too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( memory_copy(
	     set_identifier,
	     internal_handle->media_values->set_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set set identifier.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the segment file set identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_file_set_identifier(
     libewf_handle_t *handle,
     const uint8_t *set_identifier,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_file_set_identifier";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( set_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid set identifier.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: set identifier too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: set identifier cannot be changed.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_handle->media_values->set_identifier,
	     set_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy set indentifier.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the MD5 hash
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_md5_hash(
     libewf_handle_t *handle,
     uint8_t *md5_hash,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_md5_hash";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: MD5 hash too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->hash_sections->md5_hash_set == 0 )
	 && ( internal_handle->hash_values != NULL ) )
	{
		if( libewf_hash_values_generate_md5_hash(
		     internal_handle->hash_values,
		     internal_handle->hash_sections->md5_hash,
		     16,
		     &( internal_handle->hash_sections->md5_hash_set ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse MD5 hash value for its value.",
			 function );

			goto on_error;
		}
	}
	if( internal_handle->hash_sections->md5_digest_set != 0 )
	{
		if( memory_copy(
		     md5_hash,
		     internal_handle->hash_sections->md5_digest,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy MD5 digest.",
			 function );

			goto on_error;
		}
		result = 1;
	}
	else if( internal_handle->hash_sections->md5_hash_set != 0 )
	{
		if( memory_copy(
		     md5_hash,
		     internal_handle->hash_sections->md5_hash,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy MD5 hash.",
			 function );

			goto on_error;
		}
		result = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the MD5 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_md5_hash(
     libewf_handle_t *handle,
     const uint8_t *md5_hash,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_md5_hash";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: MD5 hash too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->hash_sections->md5_hash_set != 0 )
	 || ( internal_handle->hash_sections->md5_digest_set != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: MD5 hash cannot be changed.",
		 function );

		goto on_error;
	}
	if( internal_handle->hash_values == NULL )
	{
		if( libewf_hash_values_initialize(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash values.",
			 function );

			goto on_error;
		}
		internal_handle->hash_values_parsed = 1;
	}
	if( memory_copy(
	     internal_handle->hash_sections->md5_hash,
	     md5_hash,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set MD5 hash.",
		 function );

		goto on_error;
	}
	internal_handle->hash_sections->md5_hash_set = 1;

	if( memory_copy(
	     internal_handle->hash_sections->md5_digest,
	     md5_hash,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set MD5 hash.",
		 function );

		goto on_error;
	}
	internal_handle->hash_sections->md5_digest_set = 1;

	if( libewf_hash_values_parse_md5_hash(
	     internal_handle->hash_values,
	     md5_hash,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse MD5 hash for its value.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the SHA1 hash
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_sha1_hash(
     libewf_handle_t *handle,
     uint8_t *sha1_hash,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_sha1_hash";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( sha1_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash.",
		 function );

		return( -1 );
	}
	if( size < 20 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: SHA1 hash too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->hash_sections->sha1_digest_set == 0 )
	 && ( internal_handle->hash_values != NULL ) )
	{
		if( libewf_hash_values_generate_sha1_hash(
		     internal_handle->hash_values,
		     internal_handle->hash_sections->sha1_digest,
		     20,
		     &( internal_handle->hash_sections->sha1_digest_set ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse MD5 hash value for its value.",
			 function );

			goto on_error;
		}
	}
	if( internal_handle->hash_sections->sha1_digest_set != 0 )
	{
		if( memory_copy(
		     sha1_hash,
		     internal_handle->hash_sections->sha1_digest,
		     20 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy SHA1 digest.",
			 function );

			goto on_error;
		}
		result = 1;
	}
	else if( internal_handle->hash_sections->sha1_hash_set != 0 )
	{
		if( memory_copy(
		     sha1_hash,
		     internal_handle->hash_sections->sha1_hash,
		     20 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy SHA1 hash.",
			 function );

			goto on_error;
		}
		result = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the SHA1 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_sha1_hash(
     libewf_handle_t *handle,
     const uint8_t *sha1_hash,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_sha1_hash";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( sha1_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash.",
		 function );

		return( -1 );
	}
	if( size < 20 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: SHA1 hash too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->hash_sections->sha1_hash_set != 0 )
	 || ( internal_handle->hash_sections->sha1_digest_set != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: SHA1 hash cannot be changed.",
		 function );

		goto on_error;
	}
	if( internal_handle->hash_values == NULL )
	{
		if( libewf_hash_values_initialize(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash values.",
			 function );

			goto on_error;
		}
		internal_handle->hash_values_parsed = 1;
	}
	if( memory_copy(
	     internal_handle->hash_sections->sha1_hash,
	     sha1_hash,
	     20 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set SHA1 hash.",
		 function );

		goto on_error;
	}
	internal_handle->hash_sections->sha1_hash_set = 1;

	if( memory_copy(
	     internal_handle->hash_sections->sha1_digest,
	     sha1_hash,
	     20 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set SHA1 hash.",
		 function );

		goto on_error;
	}
	internal_handle->hash_sections->sha1_digest_set = 1;

	if( libewf_hash_values_parse_sha1_hash(
	     internal_handle->hash_values,
	     sha1_hash,
	     20,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse SHA1 hash for its value.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets the read zero chunk on error
 * The chunk is not zeroed if read raw is used
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_read_zero_chunk_on_error(
     libewf_handle_t *handle,
     uint8_t zero_on_error,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_read_zero_chunk_on_error";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_handle->io_handle->zero_on_error = zero_on_error;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Copies the media values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_copy_media_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_destination_handle = NULL;
	libewf_internal_handle_t *internal_source_handle      = NULL;
	static char *function                                 = "libewf_handle_copy_media_values";

	if( destination_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination handle.",
		 function );

		return( -1 );
	}
	if( source_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source handle.",
		 function );

		return( -1 );
	}
	internal_destination_handle = (libewf_internal_handle_t *) destination_handle;
	internal_source_handle      = (libewf_internal_handle_t *) source_handle;

	if( internal_source_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_destination_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid destination handle - missing media values.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     internal_destination_handle->media_values,
	     internal_source_handle->media_values,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy media values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of acquiry errors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_acquiry_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_acquiry_errors";
	int number_of_elements                    = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_errors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of errors.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->acquiry_errors != NULL )
	{
		if( libcdata_range_list_get_number_of_elements(
		     internal_handle->acquiry_errors,
		     &number_of_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of elements from acquiry errors range list.",
			 function );

			goto on_error;
		}
	}
	if( number_of_elements < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of elements value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_errors = (uint32_t) number_of_elements;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves an acquiry error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_acquiry_error(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_acquiry_error";
	intptr_t *value                           = NULL;
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libcdata_range_list_get_range_by_index(
	          internal_handle->acquiry_errors,
	          (int) index,
	          start_sector,
	          number_of_sectors,
	          &value,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve acquiry error: %" PRIu32 ".",
		 function,
		 index );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Append an acquiry error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_append_acquiry_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_append_acquiry_error";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libcdata_range_list_insert_range(
	          internal_handle->acquiry_errors,
	          start_sector,
	          number_of_sectors,
	          NULL,
	          NULL,
	          NULL,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert acquiry error in range list.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of checksum errors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_checksum_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_checksum_errors";
	uint32_t number_of_elements               = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_errors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of errors.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->chunk_table != NULL )
	{
		if( libewf_chunk_table_get_number_of_checksum_errors(
		     internal_handle->chunk_table,
		     &number_of_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of checksum errors.",
			 function );

			goto on_error;
		}
	}
	*number_of_errors = number_of_elements;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a checksum error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_checksum_error(
     libewf_handle_t *handle,
     uint32_t error_index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_checksum_error";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk table.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_chunk_table_get_checksum_error(
	          internal_handle->chunk_table,
	          error_index,
	          start_sector,
	          number_of_sectors,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve checksum error: %" PRIu32 ".",
		 function,
		 error_index );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Appends a checksum error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_append_checksum_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_append_checksum_error";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libewf_chunk_table_append_checksum_error(
	          internal_handle->chunk_table,
	          start_sector,
	          number_of_sectors,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append checksum error.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of sessions
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_sessions(
     libewf_handle_t *handle,
     uint32_t *number_of_sessions,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_sessions";
	int number_of_entries                     = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_sessions == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sessions.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->sessions != NULL )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_handle->sessions,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from sessions array.",
			 function );

			goto on_error;
		}
	}
	if( number_of_entries < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_sessions = (uint32_t) number_of_entries;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a session
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_session(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_session";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_sector_range_list_get_range(
	     internal_handle->sessions,
	     index,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve session: %" PRIu32 " sector range.",
		 function,
		 index );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Appends a session
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_append_session(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_append_session";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_sector_range_list_append_range(
	     internal_handle->sessions,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append session sector range.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of tracks
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_tracks(
     libewf_handle_t *handle,
     uint32_t *number_of_tracks,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_tracks";
	int number_of_entries                     = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_tracks == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of tracks.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->tracks != NULL )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_handle->tracks,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from tracks array.",
			 function );

			goto on_error;
		}
	}
	if( number_of_entries < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_tracks = (uint32_t) number_of_entries;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves a track
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_track(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_track";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_sector_range_list_get_range(
	     internal_handle->tracks,
	     index,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve track: %" PRIu32 " sector range.",
		 function,
		 index );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Appends a track
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_append_track(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_append_track";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_sector_range_list_append_range(
	     internal_handle->tracks,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append track sector range.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the header codepage
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_header_codepage(
     libewf_handle_t *handle,
     int *header_codepage,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_header_codepage";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( header_codepage == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header codepage.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*header_codepage = internal_handle->io_handle->header_codepage;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_header_codepage(
     libewf_handle_t *handle,
     int header_codepage,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_header_codepage";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( header_codepage != LIBEWF_CODEPAGE_ASCII )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_874 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_932 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_936 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1250 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1251 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1252 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1253 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1254 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1255 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1256 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1257 )
	 && ( header_codepage != LIBEWF_CODEPAGE_WINDOWS_1258 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header codepage.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_handle->io_handle->header_codepage = header_codepage;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the header value date format
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_header_values_date_format(
     libewf_handle_t *handle,
     int *date_format,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_header_values_date_format";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( date_format == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date format.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*date_format = internal_handle->date_format;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the header values date format
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_header_values_date_format(
     libewf_handle_t *handle,
     int date_format,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_header_values_date_format";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date format.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_handle->date_format = date_format;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the number of header values
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_header_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_header_values";
	int number_of_header_values               = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		if( libfvalue_table_get_number_of_values(
		     internal_handle->header_values,
		     &number_of_header_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of header values.",
			 function );

			goto on_error;
		}
	}
	if( number_of_header_values < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of header values value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_values = (uint32_t) number_of_header_values;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the value identifier of a specific index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_handle_get_header_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_header_value_identifier_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_identifier_size(
		          internal_handle->header_values,
		          index,
		          identifier_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %" PRIu32 " identifier size.",
			 function,
			 index );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the header value identifier of a specific index
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_handle_get_header_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_header_value_identifier";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_identifier(
		          internal_handle->header_values,
		          index,
		          identifier,
		          identifier_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %" PRIu32 " identifier.",
			 function,
			 index );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_utf8_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_header_value_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_utf8_value_size(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          internal_handle->date_format,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_header_value";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_utf8_value(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          internal_handle->date_format,
		          utf8_string,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the UTF-8 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_utf8_header_value";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: header value cannot be changed.",
		 function );

		result = -1;
	}
	else if( internal_handle->header_values == NULL )
	{
		result = libewf_header_values_initialize(
		          &( internal_handle->header_values ),
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header values.",
			 function );
		}
	}
	if( result == 1 )
	{
		result = libewf_value_table_copy_value_from_utf8_string(
			  internal_handle->header_values,
			  identifier,
			  identifier_length,
			  utf8_string,
			  utf8_string_length,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_utf16_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_header_value_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( utf16_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_utf16_value_size(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          internal_handle->date_format,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf16_header_value";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->header_values != NULL )
	{
		result = libewf_header_values_get_utf16_value(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          internal_handle->date_format,
		          utf16_string,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the UTF-16 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_utf16_header_value";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: header value cannot be changed.",
		 function );

		result = -1;
	}
	else if( internal_handle->header_values == NULL )
	{
		result = libewf_header_values_initialize(
		          &( internal_handle->header_values ),
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header values.",
			 function );
		}
	}
	if( result == 1 )
	{
		result = libewf_value_table_copy_value_from_utf16_string(
		          internal_handle->header_values,
		          identifier,
		          identifier_length,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Copies the header values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_copy_header_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_destination_handle = NULL;
	libewf_internal_handle_t *internal_source_handle      = NULL;
	static char *function                                 = "libewf_handle_copy_header_values";
	int result                                            = 1;

	if( destination_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination handle.",
		 function );

		return( -1 );
	}
	if( source_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source handle.",
		 function );

		return( -1 );
	}
	internal_destination_handle = (libewf_internal_handle_t *) destination_handle;
	internal_source_handle      = (libewf_internal_handle_t *) source_handle;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_source_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_source_handle->header_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source handle - missing header values.",
		 function );

		result = -1;
	}
	else
	{
/* TODO add destination handle write lock */
		if( internal_destination_handle->header_values == NULL )
		{
			if( libewf_header_values_initialize(
			     &( internal_destination_handle->header_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create header values.",
				 function );

				result = -1;
			}
		}
		if( result == 1 )
		{
			result = libewf_header_values_copy(
			          internal_destination_handle->header_values,
			          internal_source_handle->header_values,
			          error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy header values.",
				 function );
			}
			else
			{
				internal_destination_handle->header_values_parsed = 1;
			}
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_source_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Parses the hash values from the hash, digest and/or xhash section
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_parse_hash_values(
     libewf_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_internal_handle_parse_hash_values";
	int result            = 1;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing hash sections.",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - hash sections already set.",
		 function );

		return( -1 );
	}
	if( libewf_hash_values_initialize(
	     &( internal_handle->hash_values ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash values.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->hash_sections->md5_hash_set != 0 )
	 && ( libewf_hash_values_parse_md5_hash(
	       internal_handle->hash_values,
	       internal_handle->hash_sections->md5_hash,
	       16,
	       error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse MD5 hash for its value.",
		 function );

		result = -1;
	}
	else if( ( internal_handle->hash_sections->md5_digest_set != 0 )
	      && ( libewf_hash_values_parse_md5_hash(
	            internal_handle->hash_values,
	            internal_handle->hash_sections->md5_digest,
	            16,
	            error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse MD5 digest for its value.",
		 function );

		result = -1;
	}
	if( ( internal_handle->hash_sections->sha1_hash_set != 0 )
	 && ( libewf_hash_values_parse_sha1_hash(
	       internal_handle->hash_values,
	       internal_handle->hash_sections->sha1_hash,
	       20,
	       error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse SHA1 hash for its value.",
		 function );

		result = -1;
	}
	else if( ( internal_handle->hash_sections->sha1_digest_set != 0 )
	      && ( libewf_hash_values_parse_sha1_hash(
	            internal_handle->hash_values,
	            internal_handle->hash_sections->sha1_digest,
	            20,
	            error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse SHA1 digest for its value.",
		 function );

		result = -1;
	}
	if( ( internal_handle->hash_sections->xhash != NULL )
	 && ( libewf_hash_values_parse_xhash(
	       internal_handle->hash_values,
	       internal_handle->hash_sections->xhash,
	       internal_handle->hash_sections->xhash_size,
	       error ) != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse xhash for values.",
		 function );

		result = -1;
	}
	return( result );
}

/* Retrieves the number of hash values
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_hash_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_hash_values";
	int number_of_hash_values                 = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( number_of_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			goto on_error;
		}
		internal_handle->hash_values_parsed = 1;
	}
	if( internal_handle->hash_values != NULL )
	{
		if( libfvalue_table_get_number_of_values(
		     internal_handle->hash_values,
		     &number_of_hash_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of hash values.",
			 function );

			goto on_error;
		}
	}
	if( number_of_hash_values < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of hash values value out of bounds.",
		 function );

		goto on_error;
	}
	*number_of_values = (uint32_t) number_of_hash_values;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_handle->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the hash value identifier of a specific index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if if no hash values are present or -1 on error
 */
int libewf_handle_get_hash_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_hash_value_identifier_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_hash_values_get_identifier_size(
		          internal_handle->hash_values,
		          index,
		          identifier_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value: %" PRIu32 " identifier size.",
			 function,
			 index );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the hash value identifier of a specific index
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 */
int libewf_handle_get_hash_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_hash_value_identifier";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_hash_values_get_identifier(
		          internal_handle->hash_values,
		          index,
		          identifier,
		          identifier_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value: %" PRIu32 " identifier.",
			 function,
			 index );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded hash value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_utf8_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_hash_value_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_value_table_get_utf8_value_size(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded hash value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf8_hash_value";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_value_table_get_utf8_value(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf8_string,
		          utf8_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the UTF-8 encoded hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_utf8_hash_value";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: hash value cannot be changed.",
		 function );

		result = -1;
	}
	else if( internal_handle->hash_values == NULL )
	{
		if( libewf_hash_values_initialize(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash values.",
			 function );

			result = -1;
		}
		else
		{
			internal_handle->hash_values_parsed = 1;
		}
	}
	if( result == 1 )
	{
		result = libewf_value_table_copy_value_from_utf8_string(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf8_string,
		          utf8_string_length,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value.",
			 function );
		}
		else if( internal_handle->hash_sections != NULL )
		{
			result = libewf_hash_sections_set_digest_from_hash_values(
				  internal_handle->hash_sections,
				  identifier,
				  identifier_length,
				  internal_handle->hash_values,
				  error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set digest in hash sections.",
				 function );
			}
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded hash value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_utf16_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf16_hash_value_size";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_value_table_get_utf16_value_size(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value size.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded hash value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_handle_get_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_utf16_hash_value";
	int result                                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

/* TODO add write lock */
	if( internal_handle->hash_values_parsed == 0 )
	{
		if( libewf_internal_handle_parse_hash_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse hash values.",
			 function );

			return( -1 );
		}
		internal_handle->hash_values_parsed = 1;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->hash_values != NULL )
	{
		result = libewf_value_table_get_utf16_value(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf16_string,
		          utf16_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value.",
			 function );
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets the UTF-16 encoded hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_utf16_hash_value";
	int result                                = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: hash value cannot be changed.",
		 function );

		result = -1;
	}
	else if( internal_handle->hash_values == NULL )
	{
		if( libewf_hash_values_initialize(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash values.",
			 function );

			result = -1;
		}
		else
		{
			internal_handle->hash_values_parsed = 1;
		}
	}
	if( result == 1 )
	{
		result = libewf_value_table_copy_value_from_utf16_string(
		          internal_handle->hash_values,
		          identifier,
		          identifier_length,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value.",
			 function );
		}
		else if( internal_handle->hash_sections != NULL )
		{
			result = libewf_hash_sections_set_digest_from_hash_values(
				  internal_handle->hash_sections,
				  identifier,
				  identifier_length,
				  internal_handle->hash_values,
				  error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set digest in hash sections.",
				 function );
			}
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

