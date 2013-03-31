/*
 * Low level reading functions
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
#include <types.h>

#include "libewf_chunk_data.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_media_values.h"
#include "libewf_read_io_handle.h"

/* Initialize the read IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_read_io_handle_initialize(
     libewf_read_io_handle_t **read_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_read_io_handle_initialize";

	if( read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read IO handle.",
		 function );

		return( -1 );
	}
	if( *read_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid read IO handle value already set.",
		 function );

		return( -1 );
	}
	*read_io_handle = memory_allocate_structure(
	                   libewf_read_io_handle_t );

	if( *read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create read IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *read_io_handle,
	     0,
	     sizeof( libewf_read_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear read IO handle.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_initialize(
	     &( ( *read_io_handle )->checksum_errors ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create checksum errors range list.",
		 function );

		goto on_error;
	}
	( *read_io_handle )->zero_on_error = 1;

	return( 1 );

on_error:
	if( *read_io_handle != NULL )
	{
		memory_free(
		 *read_io_handle );

		*read_io_handle = NULL;
	}
	return( -1 );
}

/* Frees the read IO handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_read_io_handle_free(
     libewf_read_io_handle_t **read_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_read_io_handle_free";
	int result            = 1;

	if( read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read IO handle.",
		 function );

		return( -1 );
	}
	if( *read_io_handle != NULL )
	{
		if( libcdata_range_list_free(
		     &( ( *read_io_handle )->checksum_errors ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free checksum errors range list.",
			 function );

			result = -1;
		}
		if( ( *read_io_handle )->case_data != NULL )
		{
			memory_free(
			 ( *read_io_handle )->case_data );
		}
		if( ( *read_io_handle )->device_information != NULL )
		{
			memory_free(
			 ( *read_io_handle )->device_information );
		}
		memory_free(
		 *read_io_handle );

		*read_io_handle = NULL;
	}
	return( result );
}

/* Clones the read IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_read_io_handle_clone(
     libewf_read_io_handle_t **destination_read_io_handle,
     libewf_read_io_handle_t *source_read_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_read_io_handle_clone";

	if( destination_read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination read IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_read_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination read IO handle value already set.",
		 function );

		return( -1 );
	}
	if( source_read_io_handle == NULL )
	{
		*destination_read_io_handle = NULL;

		return( 1 );
	}
	*destination_read_io_handle = memory_allocate_structure(
	                               libewf_read_io_handle_t );

	if( *destination_read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination read IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *destination_read_io_handle,
	     0,
	     sizeof( libewf_read_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear source to destination read IO handle.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_clone(
	     &( ( *destination_read_io_handle )->checksum_errors ),
	     source_read_io_handle->checksum_errors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination checksum errors range list.",
		 function );

		goto on_error;
	}
	( *destination_read_io_handle )->zero_on_error = source_read_io_handle->zero_on_error;

	return( 1 );

on_error:
	if( *destination_read_io_handle != NULL )
	{
		memory_free(
		 *destination_read_io_handle );

		*destination_read_io_handle = NULL;
	}
	return( -1 );
}

/* Reads a specific chunk of data
 * Adds a checksum error if the data is corrupted
 * Returns 1 if successful or -1 on error
 */
int libewf_read_io_handle_read_chunk_data(
     libewf_read_io_handle_t *read_io_handle,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libmfdata_list_t *chunk_table_list,
     libfcache_cache_t *chunk_table_cache,
     int chunk_index,
     off64_t chunk_offset,
     libewf_chunk_data_t **chunk_data,
     libcerror_error_t **error )
{
	static char *function      = "libewf_read_io_handle_read_chunk_data";
	size_t chunk_size          = 0;
	uint64_t start_sector      = 0;
	uint32_t number_of_sectors = 0;
	int result                 = 0;

	if( read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	/* This function will expand element groups
	 */
	result = libmfdata_list_get_element_value_by_index(
	          chunk_table_list,
	          file_io_pool,
	          chunk_table_cache,
	          chunk_index,
	          (intptr_t **) chunk_data,
	          0,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk data: %d.",
		 function,
		 chunk_index );

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

		chunk_size = media_values->chunk_size;

		if( (size64_t) ( chunk_offset + chunk_size ) > media_values->media_size )
		{
			chunk_size = (size_t) ( media_values->media_size - chunk_offset );
		}
		if( libewf_chunk_data_initialize(
		     chunk_data,
		     chunk_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create chunk data.",
			 function );

			return( -1 );
		}
		if( *chunk_data == NULL )
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
		( *chunk_data )->data_size    = chunk_size;
		( *chunk_data )->range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;

		if( memory_set(
		     ( *chunk_data )->data,
		     0,
		     ( *chunk_data )->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to zero chunk data.",
			 function );

			libewf_chunk_data_free(
			 chunk_data,
			 NULL );

			return( -1 );
		}
		if( libmfdata_list_set_element_by_index(
		     chunk_table_list,
		     chunk_index,
		     -1,
		     chunk_offset,
		     chunk_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to set chunk: %d in table.",
			 function,
			 chunk_index );

			libewf_chunk_data_free(
			 chunk_data,
			 NULL );

			return( -1 );
		}
		if( libmfdata_list_set_element_value_by_index(
		     chunk_table_list,
		     chunk_table_cache,
		     chunk_index,
		     (intptr_t *) *chunk_data,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_data_free,
		     LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to set chunk data: %d as element value.",
			 function,
			 chunk_index );

			libewf_chunk_data_free(
			 chunk_data,
			 NULL );

			return( -1 );
		}
	}
	else
	{
		if( *chunk_data == NULL )
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
		if( libewf_chunk_data_unpack(
		     *chunk_data,
		     media_values->chunk_size,
		     io_handle->compression_method,
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
		if( ( ( *chunk_data )->range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
		{
			if( read_io_handle->zero_on_error != 0 )
			{
				if( memory_set(
				     ( *chunk_data )->data,
				     0,
				     ( *chunk_data )->data_size ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to zero chunk data.",
					 function );

					return( -1 );
				}
			}
		}
	}
	if( ( ( *chunk_data )->range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
	{
		/* Add checksum error
		 */
		start_sector      = (uint64_t) chunk_index * (uint64_t) media_values->sectors_per_chunk;
		number_of_sectors = media_values->sectors_per_chunk;

		if( ( start_sector + number_of_sectors ) > (uint64_t) media_values->number_of_sectors )
		{
			number_of_sectors = (uint32_t) ( (uint64_t) media_values->number_of_sectors - start_sector );
		}
		if( libcdata_range_list_append_range(
		     read_io_handle->checksum_errors,
		     start_sector,
		     number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append checksum error to range list.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

