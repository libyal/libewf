/*
 * Chunk table functions
 *
 * Copyright (C) 2006-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libewf_chunk_data.h"
#include "libewf_chunk_group.h"
#include "libewf_chunk_table.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"

/* Creates a chunk table
 * Make sure the value chunk_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_initialize(
     libewf_chunk_table_t **chunk_table,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_initialize";

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( *chunk_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk table value already set.",
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
	*chunk_table = memory_allocate_structure(
	                libewf_chunk_table_t );

	if( *chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *chunk_table,
	     0,
	     sizeof( libewf_chunk_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunk table.",
		 function );

		memory_free(
		 *chunk_table );

		*chunk_table = NULL;

		return( -1 );
	}
	if( libfdata_list_initialize(
	     &( ( *chunk_table )->corrupted_chunks_list ),
	     NULL,
	     NULL,
	     NULL,
	     NULL,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create corrupted chunks list.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_initialize(
	     &( ( *chunk_table )->checksum_errors ),
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
	( *chunk_table )->io_handle = io_handle;

	return( 1 );

on_error:
	if( *chunk_table != NULL )
	{
		if( ( *chunk_table )->corrupted_chunks_list != NULL )
		{
			libfdata_list_free(
			 &( ( *chunk_table )->corrupted_chunks_list ),
			 NULL );
		}
		memory_free(
		 *chunk_table );

		*chunk_table = NULL;
	}
	return( -1 );
}

/* Frees a chunk table
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_free(
     libewf_chunk_table_t **chunk_table,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_free";
	int result            = 1;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( *chunk_table != NULL )
	{
		if( libfdata_list_free(
		     &( ( *chunk_table )->corrupted_chunks_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free corrupted chunks list.",
			 function );

			result = -1;
		}
		if( libcdata_range_list_free(
		     &( ( *chunk_table )->checksum_errors ),
		     NULL,
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
		memory_free(
		 *chunk_table );

		*chunk_table = NULL;
	}
	return( result );
}

/* Clones the chunk table
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_clone(
     libewf_chunk_table_t **destination_chunk_table,
     libewf_chunk_table_t *source_chunk_table,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_clone";

	if( destination_chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination chunk table.",
		 function );

		return( -1 );
	}
	if( *destination_chunk_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination chunk table value already set.",
		 function );

		return( -1 );
	}
	if( source_chunk_table == NULL )
	{
		*destination_chunk_table = NULL;

		return( 1 );
	}
	*destination_chunk_table = memory_allocate_structure(
	                            libewf_chunk_table_t );

	if( *destination_chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination chunk table.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_chunk_table,
	     source_chunk_table,
	     sizeof( libewf_chunk_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination chunk table.",
		 function );

		memory_free(
		 *destination_chunk_table );

		*destination_chunk_table = NULL;

		return( -1 );
	}
/* TODO: clonse corrupted_chunks_list */
	( *destination_chunk_table )->corrupted_chunks_list = NULL;
	( *destination_chunk_table )->checksum_errors       = NULL;

	if( libcdata_range_list_clone(
	     &( ( *destination_chunk_table )->checksum_errors ),
	     source_chunk_table->checksum_errors,
	     NULL,
	     NULL,
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
	return( 1 );

on_error:
	if( *destination_chunk_table != NULL )
	{
		memory_free(
		 *destination_chunk_table );

		*destination_chunk_table = NULL;
	}
	return( -1 );
}

/* Retrieves the number of checksum errors
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_get_number_of_checksum_errors(
     libewf_chunk_table_t *chunk_table,
     uint32_t *number_of_errors,
     libcerror_error_t **error )
{
	static char *function  = "libewf_chunk_table_get_number_of_checksum_errors";
	int number_of_elements = 0;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
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
	if( libcdata_range_list_get_number_of_elements(
	     chunk_table->checksum_errors,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from range list.",
		 function );

		return( -1 );
	}
	if( number_of_elements < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of elements value out of bounds.",
		 function );

		return( -1 );
	}
	*number_of_errors = (uint32_t) number_of_elements;

	return( 1 );
}


/* Retrieves a checksum error
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_get_checksum_error(
     libewf_chunk_table_t *chunk_table,
     uint32_t error_index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_get_checksum_error";
	intptr_t *value       = NULL;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( libcdata_range_list_get_range_by_index(
	     chunk_table->checksum_errors,
	     (int) error_index,
	     start_sector,
	     number_of_sectors,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve checksum error: %" PRIu32 " from range list.",
		 function,
		 error_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends a checksum error
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_append_checksum_error(
     libewf_chunk_table_t *chunk_table,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_append_checksum_error";

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( libcdata_range_list_insert_range(
	     chunk_table->checksum_errors,
	     start_sector,
	     number_of_sectors,
	     NULL,
	     NULL,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert checksum error in range list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the chunks group in a segment file at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_chunk_table_get_segment_file_chunk_group_by_offset(
     libewf_chunk_table_t *chunk_table,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     libfcache_cache_t *chunk_groups_cache,
     off64_t offset,
     uint32_t *segment_number,
     off64_t *segment_file_data_offset,
     libewf_segment_file_t **segment_file,
     int *chunk_groups_list_index,
     off64_t *chunk_group_data_offset,
     libewf_chunk_group_t **chunk_group,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_get_segment_file_chunk_group_by_offset";
	int result            = 0;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( segment_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment number.",
		 function );

		return( -1 );
	}
	if( segment_file_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file data offset.",
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
	if( chunk_groups_list_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk groups list index.",
		 function );

		return( -1 );
	}
	if( chunk_group_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group data offset.",
		 function );

		return( -1 );
	}
	if( chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk groups.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_segment_file_at_offset(
	          segment_table,
	          offset,
	          file_io_pool,
	          segment_number,
	          segment_file_data_offset,
	          segment_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file at 0x%08" PRIx64 " from segment files list.",
		 function,
		 offset );

		return( -1 );
	}
	if( result != 0 )
	{
		result = libewf_segment_file_get_chunk_group_by_offset(
			  *segment_file,
			  file_io_pool,
			  chunk_groups_cache,
			  *segment_file_data_offset,
			  chunk_groups_list_index,
			  chunk_group_data_offset,
			  chunk_group,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk group from segment file: %" PRIu32 " at 0x%08" PRIx64 ".",
			 function,
			 *segment_number,
			 *segment_file_data_offset );

			return( -1 );
		}
	}
	if( result != 0 )
	{
		if( *chunk_group == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk group: %d.",
			 function,
			 *chunk_groups_list_index );

			return( -1 );
		}
	}
	return( result );
}

/* Determines if the chunk exists
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_chunk_exists_for_offset(
     libewf_chunk_table_t *chunk_table,
     uint64_t chunk_index,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     libfcache_cache_t *chunk_groups_cache,
     off64_t offset,
     libcerror_error_t **error )
{
	libewf_chunk_group_t *chunk_group           = NULL;
	libewf_segment_file_t *segment_file         = NULL;
	libfdata_list_element_t *chunk_list_element = NULL;
	static char *function                       = "libewf_chunk_table_chunk_exists_for_offset";
	off64_t chunk_data_offset                   = 0;
	off64_t chunk_group_data_offset             = 0;
	off64_t segment_file_data_offset            = 0;
	uint32_t segment_number                     = 0;
	int chunk_groups_list_index                 = 0;
	int chunks_list_index                       = 0;
	int result                                  = 0;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	result = libewf_chunk_table_get_segment_file_chunk_group_by_offset(
	          chunk_table,
	          file_io_pool,
	          segment_table,
	          chunk_groups_cache,
	          offset,
	          &segment_number,
	          &segment_file_data_offset,
	          &segment_file,
		  &chunk_groups_list_index,
		  &chunk_group_data_offset,
		  &chunk_group,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file chunk group at 0x%08" PRIx64 ".",
		 function,
		 offset );

		return( -1 );
	}
	if( result != 0 )
	{
		if( chunk_group == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunks group: %d.",
			 function,
			 chunk_groups_list_index );

			return( -1 );
		}
		result = libfdata_list_get_list_element_at_offset(
			  chunk_group->chunks_list,
			  chunk_group_data_offset,
			  &chunks_list_index,
			  &chunk_data_offset,
			  &chunk_list_element,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk: %" PRIu64 " from chunk group: %d in segment file: %" PRIu32 " at 0x%08" PRIx64 ".",
			 function,
			 chunk_index,
			 chunk_groups_list_index,
			 segment_number,
			 segment_file_data_offset );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the chunk data of a chunk at a specific offset
 * Adds a checksum error if the data is corrupted
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_get_chunk_data_by_offset(
     libewf_chunk_table_t *chunk_table,
     uint64_t chunk_index,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     libfcache_cache_t *chunk_groups_cache,
     libfcache_cache_t *chunks_cache,
     off64_t offset,
     libewf_chunk_data_t **chunk_data,
     off64_t *chunk_data_offset,
     libcerror_error_t **error )
{
	libewf_chunk_data_t *corrupted_chunk_data = NULL;
	libewf_chunk_group_t *chunk_group         = NULL;
	libewf_segment_file_t *segment_file       = NULL;
	static char *function                     = "libewf_chunk_table_get_chunk_data_by_offset";
	off64_t chunk_offset                      = 0;
	off64_t chunk_group_data_offset           = 0;
	off64_t segment_file_data_offset          = 0;
	size_t chunk_data_size                    = 0;
	uint64_t start_sector                     = 0;
	uint64_t number_of_sectors                = 0;
	uint32_t segment_number                   = 0;
	int chunk_groups_list_index               = 0;
	int chunks_list_index                     = 0;
	int result                                = 0;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
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
	if( chunk_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data offset.",
		 function );

		return( -1 );
	}
	result = libewf_chunk_table_get_segment_file_chunk_group_by_offset(
		  chunk_table,
		  file_io_pool,
		  segment_table,
		  chunk_groups_cache,
		  offset,
		  &segment_number,
		  &segment_file_data_offset,
		  &segment_file,
		  &chunk_groups_list_index,
		  &chunk_group_data_offset,
		  &chunk_group,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file chunk group at 0x%08" PRIx64 ".",
		 function,
		 offset );

		goto on_error;
	}
	if( result != 0 )
	{
		if( chunk_group == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk group: %d.",
			 function,
			 chunk_groups_list_index );

			goto on_error;
		}
		result = libfdata_list_get_element_value_at_offset(
			  chunk_group->chunks_list,
			  (intptr_t *) file_io_pool,
			  chunks_cache,
			  chunk_group_data_offset,
			  &chunks_list_index,
			  chunk_data_offset,
			  (intptr_t **) chunk_data,
			  0,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk: %" PRIu64 " data from chunk group: %d in segment file: %" PRIu32 " at 0x%08" PRIx64 ".",
			 function,
			 chunk_index,
			 chunk_groups_list_index,
			 segment_number,
			 segment_file_data_offset );

			goto on_error;
		}
	}
	if( result != 0 )
	{
		if( *chunk_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk: %" PRIu64 " data.",
			 function,
			 chunk_index );

			goto on_error;
		}
		if( libewf_chunk_data_unpack(
		     *chunk_data,
		     io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to unpack chunk: %" PRIu64 " data.",
			 function,
			 chunk_index );

			goto on_error;
		}
		if( ( ( *chunk_data )->range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
		{
			chunk_offset = offset - *chunk_data_offset;
		}
	}
	else
	{
/* TODO get chunk from chunk_table->corrupted_chunks_list */

		chunk_offset    = (off64_t) chunk_index * media_values->chunk_size;
		chunk_data_size = media_values->chunk_size;

		if( (size64_t) ( chunk_offset + chunk_data_size ) > media_values->media_size )
		{
			chunk_data_size = (size_t) ( media_values->media_size - chunk_offset );
		}
		if( libewf_chunk_data_initialize(
		     &corrupted_chunk_data,
		     media_values->chunk_size,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create chunk: %" PRIu64 " data.",
			 function,
			 chunk_index );

			goto on_error;
		}
		if( corrupted_chunk_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk data: %" PRIu64 ".",
			 function,
			 chunk_index );

			goto on_error;
		}
		corrupted_chunk_data->data_size    = chunk_data_size;
		corrupted_chunk_data->range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;

		if( libfdata_list_cache_element_value(
		     chunk_table->corrupted_chunks_list,
		     chunks_cache,
		     (int) chunk_index,
		     (int) segment_number,
		     chunk_offset,
		     chunk_data_size,
		     corrupted_chunk_data->range_flags,
		     0,
		     (intptr_t *) corrupted_chunk_data,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_data_free,
		     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to cache chunk: %" PRIu64 " data in segment file: %" PRIu32 " at 0x%08" PRIx64 ".",
			 function,
			 chunk_index,
			 segment_number,
			 chunk_offset );

			return( -1 );
		}
		*chunk_data = corrupted_chunk_data;

		/* chunks_cache takes over management of chunk_data
		 */
		corrupted_chunk_data = NULL;
	}
	if( ( ( *chunk_data )->range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
	{
		/* Add checksum error
		 */
		start_sector      = chunk_offset / media_values->bytes_per_sector;
		number_of_sectors = media_values->sectors_per_chunk;

		if( ( start_sector + number_of_sectors ) > (uint64_t) media_values->number_of_sectors )
		{
			number_of_sectors = (uint64_t) media_values->number_of_sectors - start_sector;
		}
		if( libcdata_range_list_insert_range(
		     chunk_table->checksum_errors,
		     start_sector,
		     number_of_sectors,
		     NULL,
		     NULL,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert checksum error in range list.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( corrupted_chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 &corrupted_chunk_data,
		 NULL );
	}
	*chunk_data = NULL;

	return( -1 );
}

/* Sets the chunk data of a chunk at a specific offset
 * Adds a checksum error if the data is corrupted
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_set_chunk_data_by_offset(
     libewf_chunk_table_t *chunk_table,
     uint64_t chunk_index,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     libfcache_cache_t *chunk_groups_cache,
     libfcache_cache_t *chunks_cache,
     off64_t offset,
     libewf_chunk_data_t *chunk_data,
     libcerror_error_t **error )
{
	libewf_chunk_group_t *chunk_group   = NULL;
	libewf_segment_file_t *segment_file = NULL;
	static char *function               = "libewf_chunk_table_set_chunk_data_by_offset";
	off64_t chunk_group_data_offset     = 0;
	off64_t segment_file_data_offset    = 0;
	uint32_t segment_number             = 0;
	int chunk_groups_list_index         = 0;
	int result                          = 0;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
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
	result = libewf_chunk_table_get_segment_file_chunk_group_by_offset(
		  chunk_table,
		  file_io_pool,
		  segment_table,
		  chunk_groups_cache,
		  offset,
		  &segment_number,
		  &segment_file_data_offset,
		  &segment_file,
		  &chunk_groups_list_index,
		  &chunk_group_data_offset,
		  &chunk_group,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file chunk group at 0x%08" PRIx64 ".",
		 function,
		 offset );

		return( -1 );
	}
	if( chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk group: %d.",
		 function,
		 chunk_groups_list_index );

		return( -1 );
	}
	result = libfdata_list_set_element_value_at_offset(
		  chunk_group->chunks_list,
		  (intptr_t *) file_io_pool,
		  chunks_cache,
		  chunk_group_data_offset,
		  (intptr_t *) chunk_data,
		  (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_data_free,
		  LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk: %" PRIu64 " data in chunk group: %d in segment file: %" PRIu32 " at 0x%08" PRIx64 ".",
		 function,
		 chunk_index,
		 chunk_groups_list_index,
		 segment_number,
		 segment_file_data_offset );

		return( -1 );
	}
	return( 1 );
}

