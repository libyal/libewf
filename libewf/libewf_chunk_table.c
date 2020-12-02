/*
 * Chunk table functions
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
	if( libfcache_cache_initialize(
	     &( ( *chunk_table )->chunk_data_cache ),
	     LIBEWF_MAXIMUM_CACHE_ENTRIES_CHUNKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk data cache.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *chunk_table )->single_chunk_data_cache ),
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create single chunk data cache.",
		 function );

		goto on_error;
	}
	( *chunk_table )->io_handle = io_handle;

	return( 1 );

on_error:
	if( *chunk_table != NULL )
	{
		if( ( *chunk_table )->chunk_data_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *chunk_table )->chunk_data_cache ),
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
		if( libfcache_cache_free(
		     &( ( *chunk_table )->chunk_data_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk data cache.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *chunk_table )->single_chunk_data_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free single chunk data cache.",
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
	( *destination_chunk_table )->current_chunk_group     = NULL;
	( *destination_chunk_table )->current_chunk_data      = NULL;
	( *destination_chunk_table )->checksum_errors         = NULL;
	( *destination_chunk_table )->chunk_data_cache        = NULL;
	( *destination_chunk_table )->single_chunk_data_cache = NULL;

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
	if( libfcache_cache_clone(
	     &( ( *destination_chunk_table )->chunk_data_cache ),
	     source_chunk_table->chunk_data_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination chunk data cache.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_clone(
	     &( ( *destination_chunk_table )->single_chunk_data_cache ),
	     source_chunk_table->single_chunk_data_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination single chunk data cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_chunk_table != NULL )
	{
		if( ( *destination_chunk_table )->chunk_data_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *destination_chunk_table )->chunk_data_cache ),
			 NULL );
		}
		if( ( *destination_chunk_table )->checksum_errors != NULL )
		{
			libcdata_range_list_free(
			 &( ( *destination_chunk_table )->checksum_errors ),
			 NULL,
			 NULL );
		}
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
	result = libcdata_range_list_insert_range(
	          chunk_table->checksum_errors,
	          start_sector,
	          number_of_sectors,
	          NULL,
	          NULL,
	          NULL,
	          error );

	if( result == -1 )
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

/* Retrieves the chunk group in a segment file at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_chunk_table_get_segment_file_chunk_group_by_offset(
     libewf_chunk_table_t *chunk_table,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     off64_t offset,
     uint32_t *segment_number,
     int *chunk_groups_list_index,
     off64_t *chunk_group_data_offset,
     libewf_chunk_group_t **chunk_group,
     libcerror_error_t **error )
{
	libewf_segment_file_t *segment_file  = NULL;
	static char *function                = "libewf_chunk_table_get_segment_file_chunk_group_by_offset";
	off64_t safe_chunk_group_data_offset = 0;
	off64_t segment_file_data_offset     = 0;
	uint32_t safe_segment_number         = 0;
	int result                           = 0;

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
	if( ( chunk_table->current_chunk_group != NULL )
	 && ( chunk_table->current_chunk_group->range_end_offset > 0 ) )
	{
		if( ( offset >= chunk_table->current_chunk_group->range_start_offset )
		 && ( offset < chunk_table->current_chunk_group->range_end_offset ) )
		{
			safe_chunk_group_data_offset = offset - chunk_table->current_chunk_group->range_start_offset;

			result = 1;
		}
	}
	if( result == 0 )
	{
		result = libewf_segment_table_get_segment_file_at_offset(
		          segment_table,
		          offset,
		          file_io_pool,
		          &safe_segment_number,
		          &segment_file_data_offset,
		          &segment_file,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file for offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 offset,
			 offset );

			return( -1 );
		}
		else if( result == 0 )
		{
/* TODO handle missing segment file */
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: missing segment file for offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 offset,
			 offset );

			return( -1 );
		}
		else
		{
			result = libewf_segment_file_get_chunk_group_by_offset(
				  segment_file,
				  file_io_pool,
				  segment_file_data_offset,
				  &( chunk_table->current_chunk_group_index ),
				  &safe_chunk_group_data_offset,
				  &( chunk_table->current_chunk_group ),
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve chunk group from segment file: %" PRIu32 " for offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 safe_segment_number,
				 offset,
				 offset );

				return( -1 );
			}
		}
	}
	if( result != 0 )
	{
		*segment_number          = safe_segment_number;
		*chunk_groups_list_index = chunk_table->current_chunk_group_index;
		*chunk_group_data_offset = safe_chunk_group_data_offset;
		*chunk_group             = chunk_table->current_chunk_group;
	}
	return( result );
}

/* Retrieves the chunk data of a chunk at a specific offset
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_get_segment_file_chunk_data_by_offset(
     libewf_chunk_table_t *chunk_table,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfcache_cache_t *chunk_data_cache,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     off64_t offset,
     off64_t *chunk_data_offset,
     libewf_chunk_data_t **chunk_data,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libewf_chunk_group_t *chunk_group = NULL;
	static char *function             = "libewf_chunk_table_get_segment_file_chunk_data_by_offset";
	off64_t chunk_group_data_offset   = 0;
	off64_t safe_chunk_data_offset    = 0;
	uint64_t chunk_index              = 0;
	uint32_t segment_number           = 0;
	int chunk_groups_list_index       = 0;
	int chunks_list_index             = 0;
	int result                        = 0;

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
	if( media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid media values - chunk size value out of bounds.",
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
	if( ( chunk_table->current_chunk_data != NULL )
	 && ( chunk_table->current_chunk_data->range_end_offset > 0 ) )
	{
		if( ( offset >= chunk_table->current_chunk_data->range_start_offset )
		 && ( offset < chunk_table->current_chunk_data->range_end_offset ) )
		{
			safe_chunk_data_offset = offset - chunk_table->current_chunk_data->range_start_offset;

			result = 1;
		}
	}
	if( result == 0 )
	{
		result = libewf_chunk_table_get_segment_file_chunk_group_by_offset(
			  chunk_table,
			  file_io_pool,
			  segment_table,
			  offset,
			  &segment_number,
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
			 "%s: unable to retrieve segment file chunk group for offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 offset,
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
				 "%s: missing chunk group: %d.",
				 function,
				 chunk_groups_list_index );

				return( -1 );
			}
			chunk_index            = (uint64_t) offset / media_values->chunk_size;
			chunks_list_index      = (int) ( chunk_group_data_offset / media_values->chunk_size );
			safe_chunk_data_offset = chunk_group_data_offset - ( (off64_t) chunks_list_index * media_values->chunk_size );

			if( libfdata_list_get_element_value_by_index(
			     chunk_group->chunks_list,
			     (intptr_t *) file_io_pool,
			     (libfdata_cache_t *) chunk_data_cache,
			     chunks_list_index,
			     (intptr_t **) &( chunk_table->current_chunk_data ),
			     read_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve chunk: %" PRIu64 " data from chunk group: %d in segment file: %" PRIu32 " for offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 chunk_index,
				 chunk_groups_list_index,
				 segment_number,
				 offset,
				 offset );

				return( -1 );
			}
			if( chunk_table->current_chunk_data == NULL )
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
			chunk_table->current_chunk_data->chunk_index = chunk_index;

			if( libfdata_list_get_element_mapped_range(
			     chunk_group->chunks_list,
			     chunks_list_index,
			     &( chunk_table->current_chunk_data->range_start_offset ),
			     (size64_t *) &( chunk_table->current_chunk_data->range_end_offset ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve list element: %d mapped range for chunk group: %d in segment file: %" PRIu32 ".",
				 function,
				 chunks_list_index,
				 chunk_groups_list_index,
				 segment_number );

				return( -1 );
			}
			chunk_table->current_chunk_data->range_start_offset += chunk_table->current_chunk_group->range_start_offset;
			chunk_table->current_chunk_data->range_end_offset   += chunk_table->current_chunk_data->range_start_offset;

			if( (size64_t) chunk_table->current_chunk_data->range_end_offset > media_values->media_size )
			{
				chunk_table->current_chunk_data->range_end_offset = (off64_t) media_values->media_size;
			}
		}
	}
	if( result != 0 )
	{
		*chunk_data_offset = safe_chunk_data_offset;
		*chunk_data        = chunk_table->current_chunk_data;
	}
	return( result );
}

/* Retrieves the chunk data of a chunk at a specific offset
 * Adds a checksum error if the data is corrupted
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_get_chunk_data_by_offset(
     libewf_chunk_table_t *chunk_table,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     off64_t offset,
     off64_t *chunk_data_offset,
     libewf_chunk_data_t **chunk_data,
     libcerror_error_t **error )
{
	libewf_chunk_data_t *safe_chunk_data = NULL;
	static char *function                = "libewf_chunk_table_get_chunk_data_by_offset";
	off64_t safe_chunk_data_offset       = 0;
	uint64_t chunk_index                 = 0;
	uint64_t number_of_sectors           = 0;
	uint64_t start_sector                = 0;
	int result                           = 0;

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
	if( media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid media values - chunk size value out of bounds.",
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
	result = libewf_chunk_table_get_segment_file_chunk_data_by_offset(
		  chunk_table,
		  io_handle,
		  file_io_pool,
	          chunk_table->chunk_data_cache,
		  media_values,
		  segment_table,
		  offset,
		  &safe_chunk_data_offset,
		  &safe_chunk_data,
		  0,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file chunk data for offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 offset,
		 offset );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libewf_chunk_data_unpack(
		     safe_chunk_data,
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

			return( -1 );
		}
		if( ( safe_chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
		{
			/* Add checksum error
			 */
			start_sector      = safe_chunk_data->range_start_offset / media_values->bytes_per_sector;
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
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert checksum error in range list.",
				 function );

				return( -1 );
			}
		}
	}
	if( result != 0 )
	{
		*chunk_data_offset = safe_chunk_data_offset;
		*chunk_data        = safe_chunk_data;
	}
	return( result );
}

/* Retrieves the chunk data of a chunk at a specific offset
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_get_chunk_data_by_offset_no_cache(
     libewf_chunk_table_t *chunk_table,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     off64_t offset,
     off64_t *chunk_data_offset,
     libewf_chunk_data_t **chunk_data,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_get_chunk_data_by_offset_no_cache";
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
	result = libewf_chunk_table_get_segment_file_chunk_data_by_offset(
		  chunk_table,
		  io_handle,
		  file_io_pool,
	          chunk_table->single_chunk_data_cache,
		  media_values,
		  segment_table,
		  offset,
		  chunk_data_offset,
		  chunk_data,
		  LIBFDATA_READ_FLAG_IGNORE_CACHE,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file chunk data for offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 offset,
		 offset );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfcache_cache_clear_value_by_index(
		     chunk_table->single_chunk_data_cache,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to clear single chunk data cache entry: 0.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

