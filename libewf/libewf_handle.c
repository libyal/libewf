/*
 * Handle functions
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

#include "libewf_analytical_data.h"
#include "libewf_case_data.h"
#include "libewf_chunk_data.h"
#include "libewf_chunk_table.h"
#include "libewf_compression.h"
#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_device_information.h"
#include "libewf_file_entry.h"
#include "libewf_hash_sections.h"
#include "libewf_handle.h"
#include "libewf_header_sections.h"
#include "libewf_header_values.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcstring.h"
#include "libewf_libfvalue.h"
#include "libewf_libfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_metadata.h"
#include "libewf_restart_data.h"
#include "libewf_sector_range.h"
#include "libewf_segment_file.h"
#include "libewf_single_file_entry.h"
#include "libewf_single_file_tree.h"
#include "libewf_single_files.h"
#include "libewf_types.h"
#include "libewf_unused.h"
#include "libewf_write_io_handle.h"

#include "ewf_data.h"
#include "ewf_file_header.h"

/* Initialize the handle
 * The handle must point to a NULL pointer to be allocated
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
	internal_handle->date_format                    = LIBEWF_DATE_FORMAT_CTIME;
	internal_handle->maximum_number_of_open_handles = LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES;

	*handle = (libewf_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
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

/* Frees the handle including elements
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
		if( libcdata_range_list_free(
		     &( internal_handle->acquiry_errors ),
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
	internal_source_handle = (libewf_internal_handle_t *) *source_handle;

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
	if( internal_source_handle->segment_files_list != NULL )
	{
		if( libmfdata_file_list_clone(
		     &( internal_destination_handle->segment_files_list ),
		     internal_source_handle->segment_files_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination segment files list.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->delta_segment_files_list != NULL )
	{
		if( libmfdata_file_list_clone(
		     &( internal_destination_handle->delta_segment_files_list ),
		     internal_source_handle->delta_segment_files_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination delta segment files list.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->segment_files_cache != NULL )
	{
		if( libfcache_cache_clone(
		     &( internal_destination_handle->segment_files_cache ),
		     internal_source_handle->segment_files_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination segment files cache.",
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination delta segment table.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->chunk_table_list != NULL )
	{
		if( libmfdata_list_clone(
		     &( internal_destination_handle->chunk_table_list ),
		     internal_source_handle->chunk_table_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination chunk table list.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_handle->chunk_table_cache != NULL )
	{
		if( libfcache_cache_clone(
		     &( internal_destination_handle->chunk_table_cache ),
		     internal_source_handle->chunk_table_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination chunk table cache.",
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
		if( internal_destination_handle->chunk_table_list != NULL )
		{
			libmfdata_list_free(
			 &( internal_destination_handle->chunk_table_list ),
			 NULL );
		}
		if( internal_destination_handle->chunk_table_cache != NULL )
		{
			libfcache_cache_free(
			 &( internal_destination_handle->chunk_table_cache ),
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
		if( internal_destination_handle->segment_files_cache != NULL )
		{
			libfcache_cache_free(
			 &( internal_destination_handle->segment_files_cache ),
			 NULL );
		}
		if( internal_destination_handle->delta_segment_files_list != NULL )
		{
			libmfdata_file_list_free(
			 &( internal_destination_handle->delta_segment_files_list ),
			 NULL );
		}
		if( internal_destination_handle->segment_files_list != NULL )
		{
			libmfdata_file_list_free(
			 &( internal_destination_handle->segment_files_list ),
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
	char *first_delta_segment_filename        = NULL;
	char *first_segment_filename              = NULL;
	static char *function                     = "libewf_handle_open";
	size_t filename_length                    = 0;
	int file_io_pool_entry                    = 0;
	int filename_index                        = 0;

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

	if( internal_handle->segment_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - segment table already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - delta segment table already set.",
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
			filename_length = libcstring_narrow_string_length(
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

					if( first_delta_segment_filename == NULL )
					{
						first_delta_segment_filename = filenames[ filename_index ];
					}
				}
			}
			else if( ( filenames[ filename_index ][ filename_length - 3 ] == 'd' )
			      || ( filenames[ filename_index ][ filename_length - 3 ] == 'D' ) )
			{
				if( ( filenames[ filename_index ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_index ][ filename_length - 1 ] == '1' ) )
				{
					first_delta_segment_filename = filenames[ filename_index ];
				}
			}
		}
	}
	if( libewf_segment_table_initialize(
	     &( internal_handle->segment_table ),
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
	if( libewf_segment_table_initialize(
	     &( internal_handle->delta_segment_table ),
	     INT64_MAX,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create delta segment table.",
		 function );

		goto on_error;
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				goto on_error;
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in delta segment table.",
				 function );

				goto on_error;
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.",
			 function );

			goto on_error;
		}
	}
	if( libewf_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO pool.",
		 function );

		goto on_error;
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );

on_error:
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
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libbfio_pool_t *file_io_pool              = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	wchar_t *first_delta_segment_filename     = NULL;
	wchar_t *first_segment_filename           = NULL;
	static char *function                     = "libewf_handle_open_wide";
	size_t filename_length                    = 0;
	int file_io_pool_entry                    = 0;
	int filename_index                        = 0;

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

	if( internal_handle->segment_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - segment table already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - delta segment table already set.",
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
			filename_length = libcstring_wide_string_length(
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

					if( first_delta_segment_filename == NULL )
					{
						first_delta_segment_filename = filenames[ filename_index ];
					}
				}
			}
			else if( ( filenames[ filename_index ][ filename_length - 3 ] == 'd' )
			      || ( filenames[ filename_index ][ filename_length - 3 ] == 'D' ) )
			{
				if( ( filenames[ filename_index ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_index ][ filename_length - 1 ] == '1' ) )
				{
					first_delta_segment_filename = filenames[ filename_index ];
				}
			}
		}
	}
	if( libewf_segment_table_initialize(
	     &( internal_handle->segment_table ),
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
	if( libewf_segment_table_initialize(
	     &( internal_handle->delta_segment_table ),
	     INT64_MAX,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create delta segment table.",
		 function );

		goto on_error;
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				goto on_error;
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in delta segment table.",
				 function );

				goto on_error;
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.",
			 function );

			goto on_error;
		}
	}
	if( libewf_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file IO pool.",
		 function );

		goto on_error;
	}
	internal_handle->file_io_pool_created_in_library = 1;

	return( 1 );

on_error:
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
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libewf_chunk_table_t *chunk_table         = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	libewf_segment_file_t *segment_file       = NULL;
	static char *function                     = "libewf_handle_open_file_io_pool";
	ssize_t read_count                        = 0;
	uint32_t maximum_delta_segment_number     = 0;
	uint32_t maximum_segment_number           = 0;
	int file_io_pool_entry                    = 0;
	int number_of_file_io_handles             = 0;

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
	if( internal_handle->file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - file IO pool already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_files_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - segment files list already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_files_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - delta segment files list already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_table_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - chunk table list already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_table_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - chunk table cache already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_sections != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - hash sections already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->single_files != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - single files already set.",
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
	if( ( access_flags & ~( LIBEWF_ACCESS_FLAG_READ | LIBEWF_ACCESS_FLAG_WRITE | LIBEWF_ACCESS_FLAG_RESUME ) ) != 0 )
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
	if( libmfdata_file_list_initialize(
	     &( internal_handle->segment_files_list ),
	     NULL,
	     NULL,
	     NULL,
	     &libewf_segment_file_read,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment files list.",
		 function );

		goto on_error;
	}
	if( libmfdata_file_list_initialize(
	     &( internal_handle->delta_segment_files_list ),
	     NULL,
	     NULL,
	     NULL,
	     &libewf_segment_file_read,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create delta segment files list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_handle->segment_files_cache ),
	     8,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to segment files table cache.",
		 function );

		goto on_error;
	}
	if( libewf_chunk_table_initialize(
	     &chunk_table,
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
	if( libmfdata_list_initialize(
	     &( internal_handle->chunk_table_list ),
	     (intptr_t *) chunk_table,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_table_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_chunk_table_clone,
	     &libewf_chunk_table_read_chunk,
	     &libewf_chunk_table_read_offsets,
	     LIBMFDATA_FLAG_IO_HANDLE_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk table list.",
		 function );

		libewf_chunk_table_free(
		 &chunk_table,
		 NULL );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_handle->chunk_table_cache ),
	     8,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk table cache.",
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
	if( internal_handle->read_io_handle != NULL )
	{
		if( libcdata_range_list_empty(
		     internal_handle->read_io_handle->checksum_errors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty checksum errors range list.",
			 function );

			goto on_error;
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

				libewf_segment_file_free(
				 &segment_file,
				 NULL );

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
			read_count = libewf_segment_file_read_file_header(
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

				libewf_segment_file_free(
				 &segment_file,
				 NULL );

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

				libewf_segment_file_free(
				 &segment_file,
				 NULL );

				goto on_error;
			}
			if( ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
			 || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
			 || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
			 || ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
			{
				if( segment_file->segment_number > maximum_segment_number )
				{
					if( libmfdata_file_list_resize(
					     internal_handle->segment_files_list,
					     (int) segment_file->segment_number,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to resize data file list.",
						 function );

						libewf_segment_file_free(
						 &segment_file,
						 NULL );

						goto on_error;
					}
					maximum_segment_number = segment_file->segment_number;
				}
				if( libmfdata_file_list_set_file_by_index(
				     internal_handle->segment_files_list,
				     (int) ( segment_file->segment_number - 1 ),
				     file_io_pool_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set segment file: %" PRIu16 " in list.",
					 function,
					 segment_file->segment_number );

					libewf_segment_file_free(
					 &segment_file,
					 NULL );

					goto on_error;
				}
			}
			else if( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_DELTA )
			{
				if( segment_file->segment_number > maximum_delta_segment_number )
				{
					if( libmfdata_file_list_resize(
					     internal_handle->delta_segment_files_list,
					     (int) segment_file->segment_number,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to resize delta segment files list.",
						 function );

						libewf_segment_file_free(
						 &segment_file,
						 NULL );

						goto on_error;
					}
					maximum_delta_segment_number = segment_file->segment_number;
				}
				if( libmfdata_file_list_set_file_by_index(
				     internal_handle->delta_segment_files_list,
				     (int) ( segment_file->segment_number - 1 ),
				     file_io_pool_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set delta segment file: %" PRIu16 " in list.",
					 function,
					 segment_file->segment_number );

					libewf_segment_file_free(
					 &segment_file,
					 NULL );

					goto on_error;
				}
				/* Re-open the delta segment file with write access
				 */
				if( ( access_flags & LIBEWF_ACCESS_FLAG_WRITE ) != 0 )
				{
					if( libbfio_pool_reopen(
					     file_io_pool,
					     file_io_pool_entry,
					     LIBBFIO_OPEN_READ_WRITE,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_OPEN_FAILED,
						 "%s: unable to reopen file IO pool entry: %d.",
						 function,
						 file_io_pool_entry );

						libewf_segment_file_free(
						 &segment_file,
						 NULL );

						goto on_error;
					}
				}
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
		if( libewf_handle_open_read_segment_files(
		     internal_handle,
		     file_io_pool,
		     chunk_table,
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
		if( ( access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 )
		{
			if( libewf_handle_open_read_delta_segment_files(
			     internal_handle,
			     file_io_pool,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read delta segment files.",
				 function );

				goto on_error;
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

			goto on_error;
		}
/* TODO refactor */
		if( internal_handle->single_files->ltree_data == NULL )
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
			     internal_handle->segment_table,
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
		     internal_handle->segment_files_list,
		     internal_handle->segment_files_cache,
		     internal_handle->chunk_table_list,
		     chunk_table,
		     internal_handle->segment_table,
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
	internal_handle->io_handle->access_flags = access_flags;
	internal_handle->file_io_pool            = file_io_pool;

	return( 1 );

on_error:
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
	if( internal_handle->chunk_table_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_handle->chunk_table_cache ),
		 NULL );
	}
	if( internal_handle->chunk_table_list != NULL )
	{
		libmfdata_list_free(
		 &( internal_handle->chunk_table_list ),
		 NULL );
	}
	if( internal_handle->segment_files_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_handle->segment_files_cache ),
		 NULL );
	}
	if( internal_handle->segment_files_list != NULL )
	{
		libmfdata_file_list_free(
		 &( internal_handle->segment_files_list ),
		 NULL );
	}
	if( internal_handle->delta_segment_files_list != NULL )
	{
		libmfdata_file_list_free(
		 &( internal_handle->delta_segment_files_list ),
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

/* Opens a segment file for reading
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libewf_handle_open_read_segment_file(
         libewf_internal_handle_t *internal_handle,
         uint32_t segment_number,
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error )
{
	static char *function = "libewf_handle_open_read_segment_file";
	ssize_t read_count    = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal handle.",
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
	read_count = libewf_segment_file_read_file_header(
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

		return( -1 );
	}
	if( ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 && ( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported segment file type.",
		 function );

		return( -1 );
	}
	if( segment_file->segment_number != segment_number )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: segment number mismatch.",
		 function );

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
			if( ( segment_file->compression_method != LIBEWF_COMPRESSION_METHOD_DEFLATE )
			 && ( segment_file->compression_method != LIBEWF_COMPRESSION_METHOD_BZIP2 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported compression method.",
				 function );

				return( -1 );
			}
		}
	}
	else
	{
		if( ( internal_handle->io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
		 && ( segment_file->type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 ) )
		{
			segment_file->type = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
		}
		else if( segment_file->type != internal_handle->io_handle->segment_file_type )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: segment file type value mismatch.",
			 function );

			return( -1 );
		}
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
	return( read_count );
}

/* Reads the section data from a segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open_read_section_data(
     libewf_internal_handle_t *internal_handle,
     libewf_chunk_table_t *chunk_table,
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	libewf_header_sections_t *header_sections     = NULL;
	libcdata_list_element_t *section_list_element = NULL;
	libewf_section_t *section                     = NULL;
	uint8_t *string_data                          = NULL;
	static char *function                         = "libewf_handle_open_read_section_data";
	off64_t section_data_offset                   = 0;
	size_t string_data_size                       = 0;
	ssize_t read_count                            = 0;
	int initialize_chunk_table                    = 0;
	int header_section_found                      = 0;
	int set_identifier_change                     = 0;
	int single_files_section_found                = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	int known_section                             = 0;
#endif

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal handle.",
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
	if( libcdata_list_get_element_by_index(
	     segment_file->section_list,
	     0,
	     &section_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first element from section list.",
		 function );

		goto on_error;
	}
	if( section_list_element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing first section list element.",
		 function );

		 goto on_error;
	}
	while( section_list_element != NULL )
	{
		if( libcdata_list_element_get_value(
		     section_list_element,
		     (intptr_t **) &section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from section list element.",
			 function );

			goto on_error;
		}
		if( section == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing section.",
			 function );

			goto on_error;
		}
		if( section->data_size != 0 )
		{
			section_data_offset = section->start_offset;

			if( segment_file->major_version == 1 )
			{
				section_data_offset += sizeof( ewf_section_descriptor_v1_t );
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
					 section_data_offset,
					 section_data_offset );
				}
				else if( segment_file->major_version == 2 )
				{
					libcnotify_printf(
					 "%s: reading 0x%08" PRIx32 " section data from file IO pool entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
					 function,
					 section->type,
					 file_io_pool_entry,
					 section_data_offset,
					 section_data_offset );
				}
			}
#endif
			if( libbfio_pool_seek_offset(
			     file_io_pool,
			     file_io_pool_entry,
			     section_data_offset,
			     SEEK_SET,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to seek section data offset: %" PRIi64 ".",
				 function,
				 section_data_offset );

				goto on_error;
			}
		}
		if( section->type != 0 )
		{
			switch( section->type )
			{
				case LIBEWF_SECTION_TYPE_DEVICE_INFORMATION:
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
						 "%s: unable to read device information file object string.",
						 function );

						goto on_error;
					}
					else if( read_count != 0 )
					{
						if( internal_handle->read_io_handle->device_information == NULL )
						{
							if( libewf_device_information_parse(
							     string_data,
							     string_data_size,
							     internal_handle->media_values,
							     internal_handle->header_values,
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
								 "%s: unable to parse device information.",
								 function );

								goto on_error;
							}
							internal_handle->read_io_handle->device_information      = string_data;
							internal_handle->read_io_handle->device_information_size = string_data_size;

							if( internal_handle->read_io_handle->case_data != NULL )
							{
								initialize_chunk_table = 1;
							}
						}
						else
						{
							if( ( internal_handle->read_io_handle->device_information_size != string_data_size )
							 || ( memory_compare(
							       internal_handle->read_io_handle->device_information,
							       string_data,
							       16 ) != 0 ) )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_INPUT,
								 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
								 "%s: device information value mismatch.",
								 function );

								goto on_error;
							}
							memory_free(
							 string_data );
						}
						string_data = NULL;
					}
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_CASE_DATA:
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
						 "%s: unable to read case data file object string.",
						 function );

						goto on_error;
					}
					else if( read_count != 0 )
					{
						if( internal_handle->read_io_handle->case_data == NULL )
						{
							if( libewf_case_data_parse(
							     string_data,
							     string_data_size,
							     internal_handle->media_values,
							     internal_handle->header_values,
							     &( internal_handle->io_handle->format ),
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
								 "%s: unable to parse case data.",
								 function );

								goto on_error;
							}
							internal_handle->read_io_handle->case_data      = string_data;
							internal_handle->read_io_handle->case_data_size = string_data_size;

							if( internal_handle->read_io_handle->device_information != NULL )
							{
								initialize_chunk_table = 1;
							}
						}
						else
						{
							if( ( internal_handle->read_io_handle->case_data_size != string_data_size )
							 || ( memory_compare(
							       internal_handle->read_io_handle->case_data,
							       string_data,
							       16 ) != 0 ) )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_INPUT,
								 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
								 "%s: case data value mismatch.",
								 function );

								goto on_error;
							}
							memory_free(
							 string_data );
						}
						string_data = NULL;
					}
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
					read_count = libewf_segment_file_read_table_section(
						      segment_file,
						      section,
						      internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
						      internal_handle->media_values,
						      chunk_table,
						      internal_handle->chunk_table_list,
						      error );

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
					read_count = libewf_section_error_read(
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
					read_count = libewf_section_session_read(
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
/* TODO */
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_MD5_HASH:
					read_count = libewf_section_md5_hash_read(
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
					read_count = libewf_section_sha1_hash_read(
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
/* TODO */
#if defined( HAVE_VERBOSE_OUTPUT )
					known_section = 1;
#endif
					break;

				case LIBEWF_SECTION_TYPE_MEMORY_EXTENTS_TABLE:
/* TODO */
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
/* TODO */
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
					read_count = libewf_section_ltree_read(
						      section,
						      internal_handle->io_handle,
						      file_io_pool,
						      file_io_pool_entry,
						      segment_file->major_version,
						      &( internal_handle->single_files->section_data ),
						      &( internal_handle->single_files->section_data_size ),
						      &( internal_handle->single_files->ltree_data ),
						      &( internal_handle->single_files->ltree_data_size ),
						      error );

					single_files_section_found = 1;

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
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_INPUT,
					 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
					 "%s: set identifier does not match.",
					 function );

					goto on_error;

/* TODO part of error tolerability changes
					chunk_table->previous_last_chunk_filled = 0;
					chunk_table->last_chunk_filled          = 0;
					chunk_table->last_chunk_compared        = 0;
*/
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
					      internal_handle->io_handle,
					      file_io_pool,
					      file_io_pool_entry,
					      internal_handle->media_values,
					      error );

				initialize_chunk_table = 1;

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
				read_count = libewf_section_digest_read(
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
				read_count = libewf_segment_file_read_table2_section(
					      segment_file,
					      section,
					      internal_handle->io_handle,
					      file_io_pool,
					      file_io_pool_entry,
					      chunk_table,
					      internal_handle->chunk_table_list,
					      error );

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
					      internal_handle->io_handle,
					      file_io_pool,
					      file_io_pool_entry,
					      internal_handle->media_values,
					      error );

				initialize_chunk_table = 1;

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
		if( initialize_chunk_table != 0 )
		{
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
			if( internal_handle->media_values->number_of_chunks > 0 )
			{
				if( libmfdata_list_resize(
				     internal_handle->chunk_table_list,
				     (int) internal_handle->media_values->number_of_chunks,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
					 "%s: unable to resize chunk table list.",
					 function );

					goto on_error;
				}
			}
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
			initialize_chunk_table = 0;
		}
		if( libcdata_list_element_get_next_element(
		     section_list_element,
		     &section_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from section list element.",
			 function );

			goto on_error;
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
	if( single_files_section_found != 0 )
	{
		if( libewf_single_files_parse(
		     internal_handle->single_files,
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
		if( internal_handle->io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL )
		{
			if( internal_handle->io_handle->format == LIBEWF_FORMAT_LOGICAL_ENCASE7 )
			{
				internal_handle->io_handle->format = LIBEWF_FORMAT_V2_LOGICAL_ENCASE7;
			}
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
	return( 1 );

on_error:
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
	return( -1 );
}

/* Opens the segment files for reading
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open_read_segment_files(
     libewf_internal_handle_t *internal_handle,
     libbfio_pool_t *file_io_pool,
     libewf_chunk_table_t *chunk_table,
     libcerror_error_t **error )
{
	libewf_section_t *section           = NULL;
	libewf_segment_file_t *segment_file = NULL;
	static char *function               = "libewf_handle_open_read_segment_files";
	off64_t section_offset              = 0;
	size64_t maximum_segment_size       = 0;
	size64_t segment_file_size          = 0;
	ssize_t read_count                  = 0;
	int file_io_pool_entry              = 0;
	int number_of_segment_files         = 0;
	int last_section                    = 0;
	int last_segment_file               = 0;
	int segment_files_list_index        = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal handle.",
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
	if( libmfdata_file_list_get_number_of_files(
	     internal_handle->segment_files_list,
	     &number_of_segment_files,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segment files in list.",
		 function );

		return( -1 );
	}
	if( number_of_segment_files <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of segment files value out of bounds.",
		 function );

		return( -1 );
	}
	for( segment_files_list_index = 0;
	     segment_files_list_index < number_of_segment_files;
	     segment_files_list_index++ )
	{
		if( libmfdata_file_list_get_file_by_index(
		     internal_handle->segment_files_list,
		     segment_files_list_index,
		     &file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %d from list.",
			 function,
			 segment_files_list_index );

			return( -1 );
		}
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

			return( -1 );
		}
		if( ( segment_files_list_index == 0 )
		 && ( number_of_segment_files > 1 ) )
		{
			/* Round the maximum segment size to nearest number of KiB
			 */
			maximum_segment_size = ( segment_file_size >> 10 ) << 10;

			if( libewf_segment_table_set_maximum_segment_size(
			     internal_handle->segment_table,
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
		if( libewf_segment_file_initialize(
		     &segment_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create segment file: %d.",
			 function,
			 segment_files_list_index );

			return( -1 );
		}
		read_count = libewf_handle_open_read_segment_file(
		              internal_handle,
		              (uint32_t) ( segment_files_list_index + 1 ),
		              segment_file,
		              file_io_pool,
		              file_io_pool_entry,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read segment file: %d.",
			 function,
			 segment_files_list_index );

			libewf_segment_file_free(
			 &segment_file,
			 NULL );

			return( -1 );
		}
		/* The segment file is cached here in case of resume
		 */
		if( libmfdata_file_list_set_file_value_by_index(
		     internal_handle->segment_files_list,
		     internal_handle->segment_files_cache,
		     segment_files_list_index,
		     (intptr_t *) segment_file,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_segment_file_free,
		     LIBMFDATA_FILE_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set segment file: %d in list.",
			 function,
			 segment_files_list_index );

			libewf_segment_file_free(
			 &segment_file,
			 NULL );

			return( -1 );
		}
		/* The segment file is now handled by the segment files list
		 * and no longer needs to be explicitly freed
		 */

/* TODO refactor to function read section descriptors */
		/* Read the section descriptors:
		 * EWF version 1 read from front to back
		 * EWF version 2 read from back to front
		 */
		if( segment_file->major_version == 1 )
		{
			section_offset = (off64_t) read_count;
		}
		else if( segment_file->major_version == 2 )
		{
			section_offset = (off64_t) segment_file_size - sizeof( ewf_section_descriptor_v2_t );
		}
		last_section = 0;

		while( ( section_offset > 0 )
		    && ( (size64_t) section_offset < segment_file_size ) )
		{
			if( libewf_section_initialize(
			     &section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create section.",
				 function );

				return( -1 );
			}
			read_count = libewf_section_descriptor_read(
			              section,
			              file_io_pool,
			              file_io_pool_entry,
			              section_offset,
			              segment_file->major_version,
			              error );

			if( read_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read section descriptor.",
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

				libewf_section_free(
				 &section,
				 NULL );

				segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_CORRUPTED;

				internal_handle->segment_table->flags |= LIBEWF_SEGMENT_TABLE_FLAG_CORRUPTED;

				break;
			}
			if( segment_file->major_version == 1 )
			{
				if( section->type == LIBEWF_SECTION_TYPE_NEXT )
				{
					last_section = 1;
				}
				else if( section->type == LIBEWF_SECTION_TYPE_DONE )
				{
					last_section      = 1;
					last_segment_file = 1;
				}
				segment_file->last_section_offset = section_offset;

				section_offset += section->size;

/* TODO move into section descriptor read ? */
				if( ( last_section != 0 )
				 && ( section->size == 0 ) )
				{
					section_offset += sizeof( ewf_section_descriptor_v1_t );
				}
				if( libcdata_list_append_value(
				     segment_file->section_list,
				     (intptr_t *) section,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append section to list.",
					 function );

					libewf_section_free(
					 &section,
					 NULL );

					return( -1 );
				}
			}
			else if( segment_file->major_version == 2 )
			{
				if( section->type == LIBEWF_SECTION_TYPE_ENCRYPTION_KEYS )
				{
/* TODO get key info */
					internal_handle->io_handle->format       = LIBEWF_FORMAT_V2_ENCASE7;
					internal_handle->io_handle->is_encrypted = 1;
				}
				if( segment_file->last_section_offset == 0 )
				{
					if( section->type == LIBEWF_SECTION_TYPE_NEXT )
					{
						last_section = 1;
					}
					else if( section->type == LIBEWF_SECTION_TYPE_DONE )
					{
						last_section      = 1;
						last_segment_file = 1;
					}
					segment_file->last_section_offset = section_offset;
				}
				section_offset -= section->size;

				if( libcdata_list_prepend_value(
				     segment_file->section_list,
				     (intptr_t *) section,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to prepend section to list.",
					 function );

					libewf_section_free(
					 &section,
					 NULL );

					return( -1 );
				}
			}
			section = NULL;

			if( ( segment_file->major_version == 1 )
			 && ( last_section != 0 ) )
			{
				break;
			}
		}
		if( ( segment_file->flags & LIBEWF_SEGMENT_FILE_FLAG_CORRUPTED ) == 0 )
		{
			if( last_section == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing next or done section.",
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

				segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_CORRUPTED;

				internal_handle->segment_table->flags |= LIBEWF_SEGMENT_TABLE_FLAG_CORRUPTED;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			else if( libcnotify_verbose != 0 )
			{
				if( (size64_t) section_offset < segment_file_size )
				{
					libcnotify_printf(
					 "%s: trailing data in segment file: %" PRIu16 "\n",
					 function,
					 segment_files_list_index );
				}
			}
#endif
		}
		if( libewf_handle_open_read_section_data(
		     internal_handle,
		     chunk_table,
		     segment_file,
		     file_io_pool,
		     file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section data from segment file: %d.",
			 function,
			 segment_files_list_index );

			return( -1 );
		}
		segment_file = NULL;
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

		internal_handle->segment_table->flags |= LIBEWF_SEGMENT_TABLE_FLAG_CORRUPTED;
	}
	return( 1 );
}

/* Opens the delta segment files for reading
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open_read_delta_segment_files(
     libewf_internal_handle_t *internal_handle,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error )
{
	libewf_section_t *section           = NULL;
	libewf_segment_file_t *segment_file = NULL;
	static char *function               = "libewf_handle_open_read_delta_segment_files";
	off64_t section_offset              = 0;
	size64_t maximum_segment_size       = 0;
	size64_t segment_file_size          = 0;
	ssize_t read_count                  = 0;
	int file_io_pool_entry              = 0;
	int number_of_segment_files         = 0;
	int last_section                    = 0;
	int last_segment_file               = 0;
	int segment_files_list_index        = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	int known_section                   = 0;
#endif

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( libmfdata_file_list_get_number_of_files(
	     internal_handle->delta_segment_files_list,
	     &number_of_segment_files,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of delta segment files in list.",
		 function );

		goto on_error;
	}
	if( number_of_segment_files <= 1 )
	{
		return( 1 );
	}
	for( segment_files_list_index = 0;
	     segment_files_list_index < number_of_segment_files;
	     segment_files_list_index++ )
	{
		if( libmfdata_file_list_get_file_by_index(
		     internal_handle->segment_files_list,
		     segment_files_list_index,
		     &file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data file: %d from segment files list.",
			 function,
			 segment_files_list_index );

			goto on_error;
		}
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
		if( ( segment_files_list_index == 0 )
		 && ( number_of_segment_files > 1 ) )
		{
			/* Round the maximum segment size to nearest number of KiB
			 */
			maximum_segment_size = ( segment_file_size >> 10 ) << 10;

			if( libewf_segment_table_set_maximum_segment_size(
			     internal_handle->delta_segment_table,
			     maximum_segment_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set maximum segment size in delta segment table.",
				 function );

				goto on_error;
			}
		}
		if( libewf_segment_file_initialize(
		     &segment_file,
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
		read_count = libewf_segment_file_read_file_header(
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

			libewf_segment_file_free(
			 &segment_file,
			 NULL );

			goto on_error;
		}
		if( segment_file->type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_DELTA )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported segment file type.",
			 function );

			libewf_segment_file_free(
			 &segment_file,
			 NULL );

			goto on_error;
		}
		if( segment_file->segment_number != (uint32_t) ( segment_files_list_index + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: segment number mismatch.",
			 function );

			libewf_segment_file_free(
			 &segment_file,
			 NULL );

			goto on_error;
		}
		if( libmfdata_file_list_set_file_value_by_index(
		     internal_handle->segment_files_list,
		     internal_handle->segment_files_cache,
		     segment_files_list_index,
		     (intptr_t *) segment_file,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_segment_file_free,
		     LIBMFDATA_FILE_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set segment file: %d in segment files list.",
			 function,
			 segment_files_list_index );

			goto on_error;
		}
		section_offset = (off64_t) read_count;
		last_section   = 0;

		while( (size64_t) section_offset < segment_file_size )
		{
			if( libewf_section_initialize(
			     &section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create section.",
				 function );

				goto on_error;
			}
			read_count = libewf_section_descriptor_read(
			              section,
			              file_io_pool,
			              file_io_pool_entry,
			              section_offset,
			              segment_file->major_version,
			              error );

			if( read_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read section descriptor.",
				 function );

				goto on_error;
			}
			if( section->type_string_length == 4 )
			{
				if( memory_compare(
				     (void *) section->type_string,
				     (void *) "done",
				     4 ) == 0 )
				{
					if( ( segment_files_list_index + 1 ) != number_of_segment_files )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_INPUT,
						 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
						 "%s: last segment number mismatch.",
						 function );

						goto on_error;
					}
					/* Nothing to do for the done section
					 */
					last_section      = 1;
					last_segment_file = 1;

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section     = 1;
#endif
				}
				else if( memory_compare(
				          (void *) section->type_string,
				          (void *) "next",
				          4 ) == 0 )
				{
					/* Nothing to do for the next section
					 */
					last_section  = 1;

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section     = 1;
#endif
				}
			}
			else if( section->type_string_length == 11 )
			{
				if( memory_compare(
				     (void *) section->type_string,
				     (void *) "delta_chunk",
				     11 ) == 0 )
				{
					read_count = libewf_segment_file_read_delta_chunk_section(
						      segment_file,
						      section,
						      file_io_pool,
						      file_io_pool_entry,
						      internal_handle->chunk_table_list,
						      error );

#if defined( HAVE_VERBOSE_OUTPUT )
					known_section     = 1;
#endif
				}
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( known_section == 0 )
				{
					if( section->type_string_length == 0 )
					{
						libcnotify_printf(
						 "%s: unsupported section type: 0x%08" PRIx32 ".\n",
						 function,
						 section->type_string );
					}
					else
					{
						libcnotify_printf(
						 "%s: unsupported section type: %s.\n",
						 function,
						 (char *) section->type_string );
					}
				}
			}
#endif
			if( read_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read section: %s.",
				 function,
				 (char *) section->type_string );

				goto on_error;
			}
			section_offset += section->size;

			if( ( last_section != 0 )
			 && ( section->size == 0 ) )
			{
				section_offset += sizeof( ewf_section_descriptor_v1_t );
			}
			if( libcdata_list_append_value(
			     segment_file->section_list,
			     (intptr_t *) section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append section to list.",
				 function );

				goto on_error;
			}
			section = NULL;

			if( last_section != 0 )
			{
				break;
			}
		}
		if( last_section == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing next or done section.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( (size64_t) section_offset < segment_file_size )
			{
				libcnotify_printf(
				 "%s: trailing data in segment file: %" PRIu16 "\n",
				 function,
				 segment_files_list_index );
			}
		}
#endif
		segment_file = NULL;
	}
	if( last_segment_file == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing done section.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
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

	if( ( internal_handle->write_io_handle != NULL )
	 && ( internal_handle->write_io_handle->write_finalized == 0 ) )
	{
		write_count = libewf_handle_write_finalize(
		               handle,
		               error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
	}
	internal_handle->file_io_pool                    = NULL;
	internal_handle->file_io_pool_created_in_library = 0;

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
	if( internal_handle->segment_files_list != NULL )
	{
		if( libmfdata_file_list_free(
		     &( internal_handle->segment_files_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment files list.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->delta_segment_files_list != NULL )
	{
		if( libmfdata_file_list_free(
		     &( internal_handle->delta_segment_files_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free delta segment files list.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->segment_files_cache != NULL )
	{
		if( libfcache_cache_free(
		     &( internal_handle->segment_files_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment files cache.",
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free delta segment table.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->chunk_table_list != NULL )
	{
		if( libmfdata_list_free(
		     &( internal_handle->chunk_table_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk table list.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->chunk_table_cache != NULL )
	{
		if( libfcache_cache_free(
		     &( internal_handle->chunk_table_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk table cache.",
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
	if( internal_handle->read_io_handle != NULL )
	{
		if( libcdata_range_list_empty(
		     internal_handle->read_io_handle->checksum_errors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty checksum errors range list.",
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
	return( result );
}

/* Prepares chunk of (media) data after reading it according to the handle settings
 * It applies decompression if necessary and validates the chunk checksum
 * This function should be used after libewf_handle_read_chunk
 * The value chunk_buffer_size contains the size of the chunk
 * Returns the resulting chunk size or -1 on error
 */
ssize_t libewf_handle_prepare_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         void *uncompressed_chunk_buffer,
         size_t *uncompressed_chunk_buffer_size,
         int8_t is_compressed,
         uint32_t chunk_checksum,
         int8_t chunk_io_flags,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_prepare_read_chunk";
	uint32_t chunk_range_flags                = 0;

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
	if( chunk_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( uncompressed_chunk_buffer_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed chunk buffer size.",
		 function );

		return( -1 );
	}
	if( ( chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_IS_PACKED ) != 0 )
	{
		if( is_compressed != 0 )
		{
			chunk_range_flags = LIBEWF_RANGE_FLAG_IS_COMPRESSED;
		}
		else
		{
			chunk_range_flags = LIBEWF_RANGE_FLAG_HAS_CHECKSUM;

			uncompressed_chunk_buffer       = chunk_buffer;
			*uncompressed_chunk_buffer_size = chunk_buffer_size;
		}
		if( libewf_chunk_data_unpack_buffer(
		     uncompressed_chunk_buffer,
		     uncompressed_chunk_buffer_size,
		     chunk_buffer,
		     chunk_buffer_size,
		     internal_handle->media_values->chunk_size,
		     internal_handle->io_handle->compression_method,
		     chunk_range_flags,
		     chunk_checksum,
		     chunk_io_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to unpack chunk buffer.",
			 function );

			return( -1 );
		}
		chunk_io_flags &= ~( LIBEWF_CHUNK_IO_FLAG_IS_PACKED );
	}
	else
	{
		*uncompressed_chunk_buffer_size = chunk_buffer_size;
	}
	return( (ssize_t) *uncompressed_chunk_buffer_size );
}

/* Reads a chunk of (media) data from the current offset into a buffer
 * Will read until the requested size is filled or the entire chunk is read
 * The value chunk_checksum is set to a runtime version of the value in the checksum_buffer
 * and is used for uncompressed chunks only
 * The LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET chunk_io_flags is set if the checksum was read into checksum_buffer
 * otherwise the checksum is considered part of the data in the chunk buffer
 * The value chunk_buffer_size contains the size of the chunk buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_handle_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         int8_t *is_compressed,
         void *checksum_buffer,
         uint32_t *chunk_checksum,
         int8_t *chunk_io_flags,
         libcerror_error_t **error )
{
	libewf_chunk_data_t *chunk_data           = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	uint8_t *chunk_data_buffer                = NULL;
	static char *function                     = "libewf_handle_read_chunk";
	size_t data_size                          = 0;
	size_t read_size                          = 0;
	uint64_t chunk_index                      = 0;

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
	if( internal_handle->io_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal handle - invalid IO handle - current offset value out of bounds.",
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
	if( chunk_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer_size == 0 )
	 || ( chunk_buffer_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk buffer size value out of bounds.",
		 function );

		return( -1 );
	}
	if( is_compressed == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid is compressed.",
		 function );

		return( -1 );
	}
	if( checksum_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checksum buffer.",
		 function );

		return( -1 );
	}
	if( chunk_checksum == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk checksum.",
		 function );

		return( -1 );
	}
	if( chunk_io_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk IO flags.",
		 function );

		return( -1 );
	}
	if( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->media_values->media_size )
	{
		return( 0 );
	}
	chunk_index = internal_handle->io_handle->current_offset
	            / internal_handle->media_values->chunk_size;

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
	/* This function will expand element groups
	 */
	if( libmfdata_list_get_element_value_by_index(
	     internal_handle->chunk_table_list,
	     internal_handle->file_io_pool,
	     internal_handle->chunk_table_cache,
	     (int) chunk_index,
	     (intptr_t **) &chunk_data,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk data: %" PRIu64 ".",
		 function,
		 chunk_index );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk data: %" PRIu64 ".",
		 function,
		 chunk_index );

		return( -1 );
	}
	*is_compressed = 0;

	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_PACKED ) != 0 )
	{
		chunk_data_buffer = chunk_data->data;
		read_size         = chunk_data->data_size;
		*chunk_io_flags   = LIBEWF_CHUNK_IO_FLAG_IS_PACKED;

		if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			*is_compressed = 1;
		}
		/* If the chunk data has a checksum and if the checksum buffer is not aligned
		 * with the chunk buffer the chunk and checksum are stored separately
		 * otherwise the checksum will be read at the same time as the chunk buffer
		 */
		else if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
		{
			if( read_size < 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid chunk data size value out of bounds.",
				 function );

				return( -1 );
			}
			if( &( ( (uint8_t *) chunk_buffer )[ read_size - 4 ] ) != checksum_buffer )
			{
				if( memory_copy(
				     checksum_buffer,
				     &( chunk_data_buffer[ read_size - 4 ] ),
				     4 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy chunk checksum data in chunk buffer.",
					 function );

					return( -1 );
				}
				byte_stream_copy_to_uint32_little_endian(
				 (uint8_t *) checksum_buffer,
				 *chunk_checksum );

				*chunk_io_flags |= LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET;
			}
		}
	}
	else
	{
		chunk_data_buffer = chunk_data->data;
		read_size         = chunk_data->data_size;
		*chunk_io_flags   = 0;
	}
	if( read_size >= chunk_buffer_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid chunk buffer size value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     chunk_buffer,
	     chunk_data_buffer,
	     read_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy chunk data to chunk buffer.",
		 function );

		return( -1 );
	}
	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_PACKED ) != 0 )
	{
		data_size = (size_t) internal_handle->media_values->chunk_size;
	}
	else
	{
		data_size = chunk_data->data_size;
	}
	if( (size64_t) ( internal_handle->io_handle->current_offset + data_size ) >= internal_handle->media_values->media_size )
	{
		data_size = (size_t) ( internal_handle->media_values->media_size - internal_handle->io_handle->current_offset );
	}
	internal_handle->io_handle->current_offset += (off64_t) data_size;

	return( (ssize_t) read_size );
}

/* Reads (media) data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_handle_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libewf_chunk_data_t *chunk_data           = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_read_buffer";
	off64_t chunk_offset                      = 0;
	size_t buffer_offset                      = 0;
	size_t read_size                          = 0;
	ssize_t total_read_count                  = 0;
	uint64_t chunk_index                      = 0;
	uint64_t chunk_data_offset                = 0;

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
	if( internal_handle->io_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal handle - invalid IO handle - current offset value out of bounds.",
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
	if( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->media_values->media_size )
	{
		return( 0 );
	}
	if( (size64_t) ( internal_handle->io_handle->current_offset + buffer_size ) >= internal_handle->media_values->media_size )
	{
		buffer_size = (size_t) ( internal_handle->media_values->media_size - internal_handle->io_handle->current_offset );
	}
	chunk_index = internal_handle->io_handle->current_offset / internal_handle->media_values->chunk_size;

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
	chunk_offset = chunk_index * internal_handle->media_values->chunk_size;

	chunk_data_offset = internal_handle->io_handle->current_offset - chunk_offset;

	if( chunk_data_offset >= (uint64_t) SSIZE_MAX )
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
		if( libewf_read_io_handle_read_chunk_data(
		     internal_handle->read_io_handle,
		     internal_handle->io_handle,
		     internal_handle->file_io_pool,
		     internal_handle->media_values,
		     internal_handle->chunk_table_list,
		     internal_handle->chunk_table_cache,
		     (int) chunk_index,
		     chunk_offset,
		     &chunk_data,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read chunk data: %" PRIu64 ".",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( chunk_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk data: %" PRIu64 ".",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( chunk_data_offset > chunk_data->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: chunk offset exceeds chunk data size.",
			 function );

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
			 "%s: unable to copy chunk data to buffer.",
			 function );

			return( -1 );
		}
		buffer_offset    += read_size;
		buffer_size      -= read_size;
		total_read_count += (ssize_t) read_size;
		chunk_index      += 1;
		chunk_offset     += internal_handle->media_values->chunk_size;
		chunk_data        = NULL;
		chunk_data_offset = 0;

		internal_handle->io_handle->current_offset += (off64_t) read_size;

		if( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->media_values->media_size )
		{
			break;
		}
		if( internal_handle->io_handle->abort != 0 )
		{
			break;
		}
	}
	return( total_read_count );
}

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_handle_read_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libewf_handle_read_random";
	ssize_t read_count    = 0;

	if( libewf_handle_seek_offset(
	     handle,
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

		return( -1 );
	}
	read_count = libewf_handle_read_buffer(
	              handle,
	              buffer,
	              buffer_size,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Prepares a chunk of (media) data before writing according to the handle settings
 * This function should be used before libewf_handle_write_chunk
 * The chunk_buffer_size should contain the actual chunk size
 * The function sets the is_compressed, chunk_checksum and chunk_io_flags values
 * The LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET chunk_io_flags is set if the checksum was set in checksum_buffer
 * and needs to be written separately from the chunk data, in case of an uncompressed chunk
 * Returns the resulting chunk size or -1 on error
 */
ssize_t libewf_handle_prepare_write_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         void *compressed_chunk_buffer,
         size_t *compressed_chunk_buffer_size,
         int8_t *is_compressed,
         uint32_t *chunk_checksum,
         int8_t *chunk_io_flags,
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_prepare_write_chunk";
	size_t chunk_padding_size                 = 0;
	uint64_t chunk_index                      = 0;
	uint64_t fill_pattern                     = 0;
	uint32_t chunk_range_flags                = 0;
	uint8_t compression_flags                 = 0;
	uint8_t pack_flags                        = 0;
	int8_t compression_level                  = LIBEWF_COMPRESSION_NONE;
	int chunk_exists                          = 0;
	int result                                = 0;

/* TODO chunk data rewrite */
	size_t compressed_chunk_buffer_offset     = 0;

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
	if( internal_handle->io_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal handle - invalid IO handle - current offset value out of bounds.",
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
	if( chunk_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( chunk_buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_buffer_size > (size_t) internal_handle->media_values->chunk_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
		 "%s: invalid chunk buffer size: %" PRIzd " value exceeds media values chunk size: %" PRIu32 ".",
		 function,
		 chunk_buffer_size,
		 internal_handle->media_values->chunk_size );

		return( -1 );
	}
	if( compressed_chunk_buffer_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed chunk buffer size.",
		 function );

		return( -1 );
	}
	if( is_compressed == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid is compressed.",
		 function );

		return( -1 );
	}
	if( chunk_buffer_size == 0 )
	{
		return( 0 );
	}
	if( internal_handle->read_io_handle != NULL )
	{
		chunk_index = internal_handle->io_handle->current_offset
		            / internal_handle->media_values->chunk_size;

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
		chunk_exists = libmfdata_list_is_set(
		                internal_handle->chunk_table_list,
		                (int) chunk_index,
		                error );

		if( chunk_exists == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if the chunk: %" PRIu64 " exists in the chunk table list.",
			 function,
			 chunk_index );

			return( -1 );
		}
		else if( chunk_exists == 0 )
		{
			if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
			 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing chunk: %" PRIu64 ".",
				 function,
				 chunk_index );

				return( -1 );
			}
		}
	}
	pack_flags = internal_handle->write_io_handle->pack_flags;

	if( chunk_exists == 0 )
	{
		compression_level = internal_handle->io_handle->compression_level;
		compression_flags = internal_handle->io_handle->compression_flags;

		if( ( ( compression_flags & LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
		 || ( ( compression_flags & LIBEWF_COMPRESS_FLAG_USE_PATTERN_FILL_COMPRESSION ) != 0 ) )
		{
			if( ( chunk_buffer_size % 8 ) == 0 )
			{
				result = libewf_chunk_data_check_for_64_bit_pattern_fill(
				          chunk_buffer,
				          chunk_buffer_size,
					  &fill_pattern,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine if chunk data contains a fill pattern.",
					 function );

					return( -1 );
				}
				else if( result != 0 )
				{
					if( ( compression_flags & LIBEWF_COMPRESS_FLAG_USE_PATTERN_FILL_COMPRESSION ) != 0 )
					{
						pack_flags &= ~( LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM );
						pack_flags |= LIBEWF_PACK_FLAG_FORCE_COMPRESSION;
						pack_flags |= LIBEWF_PACK_FLAG_USE_PATTERN_FILL_COMPRESSION;
					}
					else if( fill_pattern == 0 )
					{
						pack_flags &= ~( LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM );
						pack_flags |= LIBEWF_PACK_FLAG_FORCE_COMPRESSION;
						pack_flags |= LIBEWF_PACK_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
					}
				}
			}
			else if( ( compression_flags & LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
			{
				result = libewf_chunk_data_check_for_empty_block(
				          chunk_buffer,
				          chunk_buffer_size,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine if chunk data is an empty block.",
					 function );

					return( -1 );
				}
				else if( result != 0 )
				{
					if( *( (uint8_t *) chunk_buffer ) == 0 )
					{
						pack_flags &= ~( LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM );
						pack_flags |= LIBEWF_PACK_FLAG_FORCE_COMPRESSION;
						pack_flags |= LIBEWF_PACK_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
					}
				}
			}
		}
	}
	if( libewf_chunk_data_pack_buffer(
	     chunk_buffer,
	     chunk_buffer_size,
	     compressed_chunk_buffer,
	     &compressed_chunk_buffer_offset,
	     compressed_chunk_buffer_size,
	     internal_handle->media_values->chunk_size,
	     chunk_buffer_size,
	     &chunk_padding_size,
	     internal_handle->io_handle->compression_method,
	     compression_level,
	     &chunk_range_flags,
	     chunk_checksum,
	     chunk_io_flags,
	     internal_handle->write_io_handle->compressed_zero_byte_empty_block,
	     internal_handle->write_io_handle->compressed_zero_byte_empty_block_size,
	     pack_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to pack chunk buffer.",
		 function );

		return( -1 );
	}
	if( ( chunk_range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		*is_compressed = 1;
	}
	else
	{
		*is_compressed = 0;
	}
	return( (ssize_t) *compressed_chunk_buffer_size );
}

/* Writes a chunk of (media) data in EWF format at the current offset
 * the necessary settings of the write values must have been made
 * chunk_buffer_size contains the size of the data within the buffer while
 * data_size contains the size of the actual input data
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
         int8_t chunk_io_flags,
         libcerror_error_t **error )
{
	libewf_chunk_data_t *chunk_data           = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_chunk";
	ssize_t write_count                       = 0;
	uint64_t chunk_index                      = 0;
	int chunk_exists                          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	int number_of_chunks                      = 0;
#endif

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
	if( internal_handle->io_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal handle - invalid IO handle - current offset value out of bounds.",
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

			goto on_error;
		}
	}
	if( chunk_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		goto on_error;
	}
#if SIZE_OF_SIZE_T > 4
	if( chunk_buffer_size > (size_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk buffer size value exceeds maximum.",
		 function );

		goto on_error;
	}
#endif
	if( data_size > internal_handle->media_values->chunk_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: data size cannot be larger than maximum chunk size.",
		 function );

		goto on_error;
	}
	if( ( internal_handle->media_values->media_size != 0 )
	 && ( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->media_values->media_size ) )
	{
		return( 0 );
	}
	if( chunk_buffer_size == 0 )
	{
		return( 0 );
	}
	chunk_index = internal_handle->io_handle->current_offset
	            / internal_handle->media_values->chunk_size;

	if( chunk_index >= (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk index value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( ( internal_handle->media_values->media_size != 0 )
	 && ( (size64_t) ( internal_handle->io_handle->current_offset + data_size ) >= internal_handle->media_values->media_size ) )
	{
		data_size = (size_t) ( internal_handle->media_values->media_size - internal_handle->io_handle->current_offset );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libmfdata_list_get_number_of_elements(
		     internal_handle->chunk_table_list,
		     &number_of_chunks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the number of chunks in the chunk table list.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: writing chunk: %" PRIu64 " of total: %d.\n",
		 function,
		 chunk_index,
		 number_of_chunks );

		libcnotify_printf(
		 "%s: writing chunk buffer of size: %" PRIzd " with data of size: %" PRIzd ".\n",
		 function,
		 chunk_buffer_size,
		 data_size );
	}
#endif
	if( libewf_chunk_data_initialize(
	     &chunk_data,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk data: %" PRIu64 ".",
		 function,
		 chunk_index );

		goto on_error;
	}
	if( is_compressed != 0 )
	{
		chunk_data->range_flags = LIBEWF_CHUNK_DATA_FLAG_IS_COMPRESSED;
	}
	else if( ( chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET ) != 0 )
	{
		/* Check if the chunk and checksum buffers are aligned
		 * if so write the chunk and checksum at the same time
		 */
		if( checksum_buffer == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid checksum buffer.",
			 function );

			goto on_error;
		}
		byte_stream_copy_from_uint32_little_endian(
		 (uint8_t *) checksum_buffer,
		 chunk_checksum );

		if( checksum_buffer == &( ( (uint8_t *) chunk_buffer )[ chunk_buffer_size ] ) )
		{
			chunk_buffer_size += 4;

			chunk_io_flags &= ~( LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET );
		}
		else
		{
			chunk_data->checksum_buffer = checksum_buffer;
		}
		chunk_data->range_flags = LIBEWF_RANGE_FLAG_HAS_CHECKSUM;
	}
	chunk_data->data                = (uint8_t *) chunk_buffer;
	chunk_data->allocated_data_size = chunk_buffer_size;
	chunk_data->flags               = LIBEWF_CHUNK_DATA_ITEM_FLAG_NON_MANAGED_DATA;
	chunk_data->chunk_io_flags      = chunk_io_flags;

	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		chunk_exists = libmfdata_list_is_set(
		                internal_handle->chunk_table_list,
		                (int) chunk_index,
		                error );

		if( chunk_exists == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if the chunk: %" PRIu64 " exists in the chunk table list.",
			 function,
			 chunk_index );

			goto on_error;
		}
		else if( chunk_exists == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk: %" PRIu64 ".",
			 function,
			 chunk_index );

			goto on_error;
		}
		chunk_data->data_size = data_size;

		chunk_data->range_flags |= LIBEWF_RANGE_FLAG_IS_DELTA;

		write_count = libewf_write_io_handle_write_existing_chunk(
		               internal_handle->write_io_handle,
		               internal_handle->io_handle,
		               internal_handle->file_io_pool,
		               internal_handle->media_values,
		               internal_handle->delta_segment_files_list,
		               internal_handle->segment_files_cache,
		               internal_handle->delta_segment_table,
		               internal_handle->chunk_table_list,
		               (int) chunk_index,
		               chunk_data,
		               chunk_buffer_size,
		               error );
	}
	else
	{
		chunk_data->data_size = chunk_buffer_size;

		write_count = libewf_write_io_handle_write_new_chunk(
		               internal_handle->write_io_handle,
		               internal_handle->io_handle,
		               internal_handle->file_io_pool,
		               internal_handle->media_values,
		               internal_handle->segment_files_list,
		               internal_handle->segment_files_cache,
		               internal_handle->segment_table,
		               internal_handle->chunk_table_list,
		               internal_handle->header_values,
		               internal_handle->hash_values,
		               internal_handle->hash_sections,
		               internal_handle->sessions,
		               internal_handle->tracks,
		               internal_handle->acquiry_errors,
		               (int) chunk_index,
		               chunk_data,
		               data_size,
		               error );
	}
	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write raw chunk data.",
		 function );

		goto on_error;
	}
	internal_handle->io_handle->current_offset += (off64_t) data_size;

	if( libewf_chunk_data_free(
	     &chunk_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free chunk data.",
		 function );

		goto on_error;
	}
	return( (ssize_t) chunk_buffer_size );

on_error:
	if( chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 &chunk_data,
		 NULL );
	}
	return( -1 );
}

/* Writes (media) data at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_handle_write_buffer(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libewf_chunk_data_t *chunk_data           = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_buffer";
	off64_t chunk_offset                      = 0;
	size_t buffer_offset                      = 0;
	size_t input_data_size                    = 0;
	size_t write_size                         = 0;
	ssize_t write_count                       = 0;
	uint64_t chunk_index                      = 0;
	uint64_t chunk_data_offset                = 0;
	int chunk_exists                          = 0;
	int write_chunk                           = 0;

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
	if( internal_handle->io_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal handle - invalid IO handle - current offset value out of bounds.",
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

			return( -1 );
		}
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
	if( internal_handle->io_handle->current_offset < 0 )
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
	 && ( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->media_values->media_size ) )
	{
		return( 0 );
	}
	if( ( internal_handle->media_values->media_size != 0 )
	 && ( (size64_t) ( internal_handle->io_handle->current_offset + buffer_size ) >= internal_handle->media_values->media_size ) )
	{
		buffer_size = (size_t) ( internal_handle->media_values->media_size - internal_handle->io_handle->current_offset );
	}
	chunk_index = internal_handle->io_handle->current_offset / internal_handle->media_values->chunk_size;

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
	chunk_offset = chunk_index * internal_handle->media_values->chunk_size;

	chunk_data_offset = internal_handle->io_handle->current_offset - chunk_offset;

	if( chunk_data_offset >= (uint64_t) SSIZE_MAX )
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
		chunk_exists = libmfdata_list_is_set(
				internal_handle->chunk_table_list,
				(int) chunk_index,
				error );

		if( chunk_exists == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if the chunk: %" PRIu64 " exists in the chunk table list.",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
		 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
		{
			if( chunk_exists == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing chunk: %" PRIu64 ".",
				 function,
				 chunk_index );

				return( -1 );
			}
			if( ( chunk_data_offset != 0 )
			 || ( buffer_size < internal_handle->media_values->chunk_size ) )
			{
				if( libewf_read_io_handle_read_chunk_data(
				     internal_handle->read_io_handle,
				     internal_handle->io_handle,
				     internal_handle->file_io_pool,
				     internal_handle->media_values,
				     internal_handle->chunk_table_list,
				     internal_handle->chunk_table_cache,
				     (int) chunk_index,
				     chunk_offset,
				     &chunk_data,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read chunk data: %" PRIu64 ".",
					 function,
					 chunk_index );

					return( -1 );
				}
				if( chunk_data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing chunk data: %" PRIu64 ".",
					 function,
					 chunk_index );

					return( -1 );
				}
				if( chunk_data_offset > chunk_data->data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: chunk offset exceeds chunk data size.",
					 function );

					return( -1 );
				}
				write_size = (size_t) ( chunk_data->data_size - chunk_data_offset );

				if( write_size > buffer_size )
				{
					write_size = buffer_size;
				}
				if( write_size == 0 )
				{
					break;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: updating chunk data: %" PRIu64 " at offset: %" PRIu64 " with size: %" PRIzd ".\n",
					 function,
					 chunk_index,
					 chunk_data_offset,
					 write_size );
				}
#endif
			}
			else
			{
				/* Reserve 4 bytes for the chunk checksum
				 */
				if( libewf_chunk_data_initialize(
				     &chunk_data,
				     (size_t) internal_handle->media_values->chunk_size + 4,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create chunk data: %" PRIu64 ".",
					 function,
					 chunk_index );

					return( -1 );
				}
				if( chunk_data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing chunk data: %" PRIu64 ".",
					 function,
					 chunk_index );

					return( -1 );
				}
				if( libmfdata_list_set_element_value_by_index(
				     internal_handle->chunk_table_list,
				     internal_handle->chunk_table_cache,
				     (int) chunk_index,
				     (intptr_t *) chunk_data,
				     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_data_free,
				     LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set chunk data: %" PRIu64 ".",
					 function,
					 chunk_index );

					libewf_chunk_data_free(
					 &chunk_data,
					 NULL );

					return( -1 );
				}
				chunk_data->data_size = internal_handle->media_values->chunk_size;

				write_size = chunk_data->data_size;
			}
			if( memory_copy(
			     &( ( chunk_data->data )[ chunk_data_offset ] ),
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
			buffer_offset += write_size;
			buffer_size   -= write_size;

			input_data_size = chunk_data->data_size;

			/* For now ignore the default pack flags when creating a delta chunk
			 */
			if( libewf_chunk_data_pack(
			     chunk_data,
			     internal_handle->media_values->chunk_size,
			     LIBEWF_COMPRESSION_METHOD_NONE,
			     LIBEWF_COMPRESSION_NONE,
			     0,
			     internal_handle->write_io_handle->compressed_zero_byte_empty_block,
			     internal_handle->write_io_handle->compressed_zero_byte_empty_block_size,
			     LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to pack chunk data: %" PRIu64 ".",
				 function,
				 chunk_index );

				return( -1 );
			}
			write_count = libewf_write_io_handle_write_existing_chunk(
				       internal_handle->write_io_handle,
				       internal_handle->io_handle,
				       internal_handle->file_io_pool,
				       internal_handle->media_values,
				       internal_handle->delta_segment_files_list,
				       internal_handle->segment_files_cache,
				       internal_handle->delta_segment_table,
				       internal_handle->chunk_table_list,
				       (int) chunk_index,
				       chunk_data,
				       input_data_size,
				       error );

			if( write_count <= 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write existing chunk.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( internal_handle->write_io_handle->write_finalized != 0 )
			{
				break;
			}
			if( chunk_exists != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
				 "%s: chunk: %" PRIu64 " already set.",
				 function,
				 chunk_index );

				return( -1 );
			}
			if( internal_handle->chunk_data == NULL )
			{
				/* Reserve 4 bytes for the chunk checksum
				 */
				if( libewf_chunk_data_initialize(
				     &( internal_handle->chunk_data ),
				     (size_t) internal_handle->media_values->chunk_size + 4,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create chunk data: %" PRIu64 ".",
					 function,
					 chunk_index );

					return( -1 );
				}
				if( internal_handle->chunk_data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing chunk data: %" PRIu64 ".",
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
				 "%s: internal handle - missing chunk data: %" PRIu64 ".",
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
			      && ( ( (size64_t) internal_handle->io_handle->current_offset + write_size ) == internal_handle->media_values->media_size ) )
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
				     internal_handle->media_values->chunk_size,
				     internal_handle->io_handle->compression_method,
				     internal_handle->io_handle->compression_level,
				     internal_handle->io_handle->compression_flags,
				     internal_handle->write_io_handle->compressed_zero_byte_empty_block,
				     internal_handle->write_io_handle->compressed_zero_byte_empty_block_size,
				     internal_handle->write_io_handle->pack_flags,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to pack chunk data: %" PRIu64 ".",
					 function,
					 chunk_index );

					return( -1 );
				}
				write_count = libewf_write_io_handle_write_new_chunk(
					       internal_handle->write_io_handle,
					       internal_handle->io_handle,
					       internal_handle->file_io_pool,
					       internal_handle->media_values,
					       internal_handle->segment_files_list,
					       internal_handle->segment_files_cache,
					       internal_handle->segment_table,
					       internal_handle->chunk_table_list,
					       internal_handle->header_values,
					       internal_handle->hash_values,
					       internal_handle->hash_sections,
					       internal_handle->sessions,
					       internal_handle->tracks,
					       internal_handle->acquiry_errors,
					       (int) chunk_index,
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
				if( libmfdata_list_set_element_value_by_index(
				     internal_handle->chunk_table_list,
				     internal_handle->chunk_table_cache,
				     (int) chunk_index,
				     (intptr_t *) internal_handle->chunk_data,
				     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_data_free,
				     LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set chunk data: %" PRIu64 ".",
					 function,
					 chunk_index );

					return( -1 );
				}
				internal_handle->chunk_data = NULL;
			}
		}
		chunk_index      += 1;
		chunk_offset     += internal_handle->media_values->chunk_size;
		chunk_data_offset = 0;

		internal_handle->io_handle->current_offset += (off64_t) write_size;

		if( ( internal_handle->media_values->media_size != 0 )
		 && ( (size64_t) internal_handle->io_handle->current_offset >= internal_handle->media_values->media_size ) )
		{
			break;
		}
		if( internal_handle->io_handle->abort != 0 )
		{
			break;
		}
	}
	return( (ssize_t) buffer_offset );
}

/* Writes (media) data at a specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_handle_write_random(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libewf_handle_write_random";
	ssize_t write_count   = 0;

	if( libewf_handle_seek_offset(
	     handle,
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

		return( -1 );
	}
	write_count = libewf_handle_write_buffer(
	               handle,
	               buffer,
	               buffer_size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
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
         libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libewf_segment_file_t *segment_file       = NULL;
	static char *function                     = "libewf_handle_write_finalize";
	off64_t segment_file_offset               = 0;
	size_t input_data_size                    = 0;
	ssize_t write_count                       = 0;
	ssize_t write_finalize_count              = 0;
	uint64_t chunk_index                      = 0;
	int file_io_pool_entry                    = -1;
	int number_of_segment_files               = 0;
	int segment_files_list_index              = 0;

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
	if( internal_handle->io_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal handle - invalid IO handle - current offset value out of bounds.",
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
	chunk_index = internal_handle->io_handle->current_offset
	            / internal_handle->media_values->chunk_size;

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
	if( internal_handle->chunk_data != NULL )
	{
		input_data_size = internal_handle->chunk_data->data_size;

		if( libewf_chunk_data_pack(
		     internal_handle->chunk_data,
		     internal_handle->media_values->chunk_size,
		     internal_handle->io_handle->compression_method,
		     internal_handle->io_handle->compression_level,
		     internal_handle->io_handle->compression_flags,
		     internal_handle->write_io_handle->compressed_zero_byte_empty_block,
		     internal_handle->write_io_handle->compressed_zero_byte_empty_block_size,
		     internal_handle->write_io_handle->pack_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to pack chunk data: %" PRIu64 ".",
			 function,
			 chunk_index );

			return( -1 );
		}
		write_count = libewf_write_io_handle_write_new_chunk(
			       internal_handle->write_io_handle,
			       internal_handle->io_handle,
			       internal_handle->file_io_pool,
			       internal_handle->media_values,
			       internal_handle->segment_files_list,
			       internal_handle->segment_files_cache,
			       internal_handle->segment_table,
			       internal_handle->chunk_table_list,
			       internal_handle->header_values,
			       internal_handle->hash_values,
			       internal_handle->hash_sections,
			       internal_handle->sessions,
			       internal_handle->tracks,
			       internal_handle->acquiry_errors,
			       (int) chunk_index,
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

		if( libmfdata_list_set_element_value_by_index(
		     internal_handle->chunk_table_list,
		     internal_handle->chunk_table_cache,
		     (int) chunk_index,
		     (intptr_t *) internal_handle->chunk_data,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_data_free,
		     LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set chunk data: %" PRIu64 ".",
			 function,
			 chunk_index );

			return( -1 );
		}
		internal_handle->chunk_data = NULL;
	}
	/* Check if all media data has been written
	 */
	if( ( internal_handle->media_values->media_size != 0 )
	 && ( internal_handle->write_io_handle->input_write_count < (ssize64_t) internal_handle->media_values->media_size ) )
	{
		return( write_finalize_count );
	}
	if( libmfdata_file_list_get_number_of_files(
	     internal_handle->segment_files_list,
	     &number_of_segment_files,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segment files.",
		 function );

		return( -1 );
	}
	if( ( number_of_segment_files < 0 )
	 || ( number_of_segment_files > (int) UINT16_MAX ) )
	{
		libcerror_error_set(
		 error,
	 	 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of segment files value out of bounds.",
		 function );

		return( -1 );
	}
	if( number_of_segment_files == 0 )
	{
		/* No segment file need to be created
		 */
		if( internal_handle->media_values->media_size != 0 )
		{
			return( write_finalize_count );
		}
		/* Create the segment file if required
		 */
		if( libewf_write_io_handle_create_segment_file(
		     internal_handle->io_handle,
		     internal_handle->file_io_pool,
		     internal_handle->segment_files_list,
		     internal_handle->segment_files_cache,
		     internal_handle->segment_table,
		     internal_handle->io_handle->segment_file_type,
		     1,
		     internal_handle->write_io_handle->maximum_number_of_segments,
		     internal_handle->media_values->set_identifier,
		     &segment_file,
		     &segment_files_list_index,
		     &file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to create segment file: 1.",
			 function );

			return( -1 );
		}
		write_count = libewf_segment_file_write_start(
		               segment_file,
		               internal_handle->io_handle,
		               internal_handle->file_io_pool,
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
			 "%s: unable to write segment file start.",
			 function );

			return( -1 );
		}
		write_finalize_count += write_count;
	}
	else
	{
		segment_files_list_index = number_of_segment_files - 1;

		if( libmfdata_file_list_get_file_by_index(
		     internal_handle->segment_files_list,
		     segment_files_list_index,
		     &file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %d from list.",
			 function,
			 segment_files_list_index );

			return( -1 );
		}
		if( libmfdata_file_list_get_file_value_by_index(
		     internal_handle->segment_files_list,
		     internal_handle->file_io_pool,
		     internal_handle->segment_files_cache,
		     segment_files_list_index,
		     (intptr_t **) &segment_file,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %d value from list.",
			 function,
			 segment_files_list_index );

			return( -1 );
		}
		if( segment_file == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing segment file: %d.",
			 function,
			 segment_files_list_index );

			return( -1 );
		}
	}
	/* Set segment file to the correct offset if write is resumed
	 */
	if( internal_handle->write_io_handle->resume_segment_file_offset > 0 )
	{
		if( libbfio_pool_seek_offset(
		     internal_handle->file_io_pool,
		     file_io_pool_entry,
		     internal_handle->write_io_handle->resume_segment_file_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to seek resume segment file offset: %" PRIi64 " in segment file: %d.",
			 function,
			 internal_handle->write_io_handle->resume_segment_file_offset,
			 segment_files_list_index );

			return( -1 );
		}
		internal_handle->write_io_handle->resume_segment_file_offset = 0;
	}
	/* Check if the last segment file is still open for writing
	 */
	if( ( segment_file->flags & LIBEWF_SEGMENT_FILE_FLAG_WRITE_OPEN ) != 0 )
	{
		if( libbfio_pool_get_offset(
		     internal_handle->file_io_pool,
		     file_io_pool_entry,
		     &segment_file_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve current offset in segment file.",
			 function );

			return( -1 );
		}
		/* Check if chunks section needs to be corrected
		 */
		if( internal_handle->write_io_handle->chunks_section_offset != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: closing chunks section.\n",
				 function );
			}
#endif
			if( internal_handle->write_io_handle->number_of_table_entries < internal_handle->write_io_handle->number_of_chunks_written_to_section )
			{
				if( libewf_write_io_handle_resize_table_entries(
				     internal_handle->write_io_handle,
				     internal_handle->write_io_handle->number_of_chunks_written_to_section,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to resize table entries.",
					 function );

					return( -1 );
				}
			}
			write_count = libewf_segment_file_write_chunks_section_final(
				       segment_file,
				       internal_handle->io_handle,
				       internal_handle->file_io_pool,
				       file_io_pool_entry,
				       segment_file_offset,
				       internal_handle->chunk_table_list,
			               internal_handle->write_io_handle->table_section_data,
			               internal_handle->write_io_handle->table_section_data_size,
			               internal_handle->write_io_handle->table_entries_data,
			               internal_handle->write_io_handle->table_entries_data_size,
			               internal_handle->write_io_handle->number_of_table_entries,
				       internal_handle->write_io_handle->chunks_section_offset,
				       (size64_t) internal_handle->write_io_handle->chunks_section_write_count,
				       internal_handle->write_io_handle->chunks_section_padding_size,
				       internal_handle->write_io_handle->number_of_chunks_written,
				       internal_handle->write_io_handle->number_of_chunks_written_to_section,
			               error );

			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to correct chunks section.",
				 function );

				return( -1 );
			}
			segment_file_offset  += write_count;
			write_finalize_count += write_count;
		}
		/* Close the segment file
		 */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: closing last segment file.\n",
			 function );
		}
#endif
		write_count = libewf_segment_file_write_close(
		               segment_file,
		               internal_handle->io_handle,
		               internal_handle->file_io_pool,
		               file_io_pool_entry,
		               segment_file_offset,
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
			 "%s: unable to close segment file.",
			 function );

			return( -1 );
		}
		write_finalize_count += write_count;
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
		     internal_handle->io_handle,
		     internal_handle->file_io_pool,
		     internal_handle->media_values,
		     internal_handle->segment_files_list,
		     internal_handle->segment_files_cache,
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
		offset += internal_handle->io_handle->current_offset;
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
	internal_handle->io_handle->current_offset = offset;

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
	*offset = internal_handle->io_handle->current_offset;

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
		if( libbfio_pool_set_maximum_number_of_open_handles(
		     internal_handle->file_io_pool,
		     maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set maximum number of open handles in file IO handle.",
			 function );

			return( -1 );
		}
	}
	internal_handle->maximum_number_of_open_handles = maximum_number_of_open_handles;

	return( 1 );
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
	if( ( internal_handle->segment_table->flags & LIBEWF_SEGMENT_TABLE_FLAG_CORRUPTED ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
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
	return( (int) internal_handle->io_handle->is_encrypted );
}

/* Retrieves the segment filename size
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_filename";

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
	if( libewf_segment_table_set_basename(
	     internal_handle->segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_filename_wide";

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
	if( libewf_segment_table_set_basename_wide(
	     internal_handle->segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
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
	*maximum_segment_size = internal_handle->segment_table->maximum_segment_size;

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
	if( maximum_segment_size > internal_handle->write_io_handle->maximum_segment_file_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename_size";
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

	if( internal_handle->delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename";
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

	if( internal_handle->delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_delta_segment_filename";

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
		 "%s: delta segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename_size_wide";
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

	if( internal_handle->delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename_wide";
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

	if( internal_handle->delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_delta_segment_filename_wide";

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
		 "%s: delta segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_maximum_delta_segment_size";

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

	if( internal_handle->delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( maximum_delta_segment_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_maximum_delta_segment_size";

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

	if( internal_handle->delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: maximum delta segment size cannot be changed.",
		 function );

		return( -1 );
	}
	if( maximum_delta_segment_size > (size64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum delta segment size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( maximum_delta_segment_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid maximum delta segment size value out of bounds.",
		 function );

		return( -1 );
	}
	internal_handle->delta_segment_table->maximum_segment_size = maximum_delta_segment_size;

	return( 1 );
}

/* Retrieves the filename size of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
int libewf_handle_get_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename_size";
	int result                       = 0;

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
	result = libewf_handle_get_file_io_handle(
	          handle,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libbfio_file_get_name_size(
		     file_io_handle,
		     filename_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filename size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the filename of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
int libewf_handle_get_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename";
	int result                       = 0;

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
	result = libewf_handle_get_file_io_handle(
	          handle,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libbfio_file_get_name(
		     file_io_handle,
		     filename,
		     filename_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filename.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the (delta) segment file of the current chunk
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
int libewf_handle_get_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename_size_wide";
	int result                       = 0;

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
	result = libewf_handle_get_file_io_handle(
	          handle,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libbfio_file_get_name_size_wide(
		     file_io_handle,
		     filename_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filename size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the filename of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
int libewf_handle_get_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename_wide";
	int result                       = 0;

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
	result = libewf_handle_get_file_io_handle(
	          handle,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libbfio_file_get_name_wide(
		     file_io_handle,
		     filename,
		     filename_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filename.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

#endif

/* Retrieves the file IO handle of the (delta) segment file of the current chunk
 * Returns 1 if successful, 0 if no such file IO handle or -1 on error
 */
int libewf_handle_get_file_io_handle(
     libewf_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_file_io_handle";
	off64_t chunk_offset                      = 0;
	size64_t chunk_size                       = 0;
	uint64_t chunk_index                      = 0;
	uint32_t range_flags                      = 0;
	int file_io_pool_entry                    = 0;

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
	if( internal_handle->io_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal handle - invalid IO handle - current offset value out of bounds.",
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
	chunk_index = internal_handle->io_handle->current_offset / internal_handle->media_values->chunk_size;

	if( chunk_index >= (uint64_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk index value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_get_data_range_by_index(
	     internal_handle->chunk_table_list,
	     (int) chunk_index,
	     &file_io_pool_entry,
	     &chunk_offset,
	     &chunk_size,
	     &range_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range of chunk: %" PRIu64 ".",
		 function,
		 chunk_index );

		return( -1 );
	}
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
	return( 1 );
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
	int chunk_index                 = 0;
	int number_of_chunks            = 0;

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
	sector_data_size  = internal_handle->media_values->number_of_sectors;
	sector_data_size *= internal_handle->media_values->bytes_per_sector;

	if( ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_READ ) != 0 )
	 && ( ( internal_handle->io_handle->access_flags & LIBEWF_ACCESS_FLAG_RESUME ) == 0 ) )
	{
		if( libmfdata_list_get_number_of_elements(
		     internal_handle->chunk_table_list,
		     &number_of_chunks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of chunks in chunk table list.",
			 function );

			return( -1 );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( ( internal_handle->media_values->number_of_chunks != 0 )
			 && ( internal_handle->media_values->number_of_chunks != (uint64_t) number_of_chunks ) )
			{
				libcnotify_printf(
				 "%s: mismatch of number of chunks in volume: %" PRIu64 " and chunk table: %d\n",
				 function,
				 internal_handle->media_values->number_of_chunks,
				 number_of_chunks );
			}
		}
#endif
		if( number_of_chunks > 0 )
		{
			chunk_index = number_of_chunks - 1;

			/* This function will expand element groups
			 */
			if( libmfdata_list_get_element_value_by_index(
			     internal_handle->chunk_table_list,
			     internal_handle->file_io_pool,
			     internal_handle->chunk_table_cache,
			     chunk_index,
			     (intptr_t **) &chunk_data,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve chunk data: %d.",
				 function,
				 chunk_index );

				return( -1 );
			}
			if( chunk_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing chunk data: %d.",
				 function,
				 chunk_index );

				return( -1 );
			}
			chunks_data_size = chunk_index
			                 * internal_handle->media_values->sectors_per_chunk
			                 * internal_handle->media_values->bytes_per_sector;

			/* The only way to determine the size of the last compressed chunk
			 * is to decompress it
			 */
			if( libewf_chunk_data_unpack(
			     chunk_data,
			     internal_handle->media_values->chunk_size,
			     internal_handle->io_handle->compression_method,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to unpack chunk data: %d.",
				 function,
				 chunk_index );

				return( -1 );
			}
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
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_root_file_entry";

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
		 "%s: root file entry already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->single_files->root_file_entry_node == NULL )
	{
		return( 0 );
	}
	if( libewf_file_entry_initialize(
	     root_file_entry,
	     internal_handle,
	     internal_handle->single_files->root_file_entry_node,
	     LIBEWF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create root file entry.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the (single) file entry for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libewf_file_get_file_entry_by_utf8_path(
     libewf_handle_t *handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle         = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	libewf_single_file_entry_t *sub_single_file_entry = NULL;
	libcdata_tree_node_t *node                        = NULL;
	libcdata_tree_node_t *sub_node                    = NULL;
	uint8_t *utf8_string_segment                      = NULL;
	static char *function                             = "libewf_file_get_file_entry_by_utf8_path";
	size_t utf8_string_index                          = 0;
	size_t utf8_string_segment_length                 = 0;
	int result                                        = 0;

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
		 "%s: file entry already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->single_files->root_file_entry_node == NULL )
	{
		return( 0 );
	}
	if( libcdata_tree_node_get_value(
	     internal_handle->single_files->root_file_entry_node,
	     (intptr_t **) &single_file_entry,
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
	if( single_file_entry == NULL )
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
	node = internal_handle->single_files->root_file_entry_node;

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
			  &sub_single_file_entry,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve single file entry sub node by name.",
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
		     internal_handle,
		     node,
		     LIBEWF_ITEM_FLAGS_DEFAULT,
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
int libewf_file_get_file_entry_by_utf16_path(
     libewf_handle_t *handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle         = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	libewf_single_file_entry_t *sub_single_file_entry = NULL;
	libcdata_tree_node_t *node                        = NULL;
	libcdata_tree_node_t *sub_node                    = NULL;
	uint16_t *utf16_string_segment                    = NULL;
	static char *function                             = "libewf_file_get_file_entry_by_utf16_path";
	size_t utf16_string_index                         = 0;
	size_t utf16_string_segment_length                = 0;
	int result                                        = 0;

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
		 "%s: file entry already set.",
		 function );

		return( -1 );
	}
	if( internal_handle->single_files->root_file_entry_node == NULL )
	{
		return( 0 );
	}
	if( libcdata_tree_node_get_value(
	     internal_handle->single_files->root_file_entry_node,
	     (intptr_t **) &single_file_entry,
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
	if( single_file_entry == NULL )
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
	node = internal_handle->single_files->root_file_entry_node;

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
			  &sub_single_file_entry,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve single file entry sub node by name.",
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
		     internal_handle,
		     node,
		     LIBEWF_ITEM_FLAGS_DEFAULT,
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

