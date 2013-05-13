/*
 * Chunk table functions
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

#include "libewf_chunk_data.h"
#include "libewf_chunk_table.h"
#include "libewf_chunk_table2.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"

/* Retrieves the chunks group in a segment file at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_chunk_table_get_segment_file_chunk_group_by_offset(
     libewf_chunk_table_t *chunk_table,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     off64_t offset,
     uint32_t *segment_number,
     off64_t *segment_file_data_offset,
     libewf_segment_file_t **segment_file,
     int *chunk_groups_list_index,
     off64_t *chunk_group_data_offset,
     libfdata_list_t **chunks_list,
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
	if( chunks_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunks list.",
		 function );

		return( -1 );
	}
/* TODO check delta segment files list first add an overlay bitmap ? bit per chunk ?
 */
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
		 "%s: unable to retrieve segment file at offset: %" PRIi64 " from segment files list.",
		 function,
		 offset );

		return( -1 );
	}
	if( result != 0 )
	{
		result = libewf_segment_file_get_chunk_group_by_offset(
			  *segment_file,
			  file_io_pool,
			  *segment_file_data_offset,
			  chunk_groups_list_index,
			  chunk_group_data_offset,
			  chunks_list,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk group from segment file: %" PRIu32 " at offset: %" PRIi64 ".",
			 function,
			 *segment_number,
			 *segment_file_data_offset );

			return( -1 );
		}
	}
	if( result != 0 )
	{
		if( *chunks_list == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunks list: %d.",
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
     off64_t offset,
     libcerror_error_t **error )
{
	libewf_segment_file_t *segment_file         = NULL;
	libfdata_list_t *chunks_list                = NULL;
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
	          offset,
	          &segment_number,
	          &segment_file_data_offset,
	          &segment_file,
		  &chunk_groups_list_index,
		  &chunk_group_data_offset,
		  &chunks_list,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file chunk group at offset: %" PRIi64 ".",
		 function,
		 offset );

		return( -1 );
	}
	if( result != 0 )
	{
		if( chunks_list == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunks list: %d.",
			 function,
			 chunk_groups_list_index );

			return( -1 );
		}
		result = libfdata_list_get_list_element_at_offset(
			  chunks_list,
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
			 "%s: unable to retrieve chunk: %" PRIu64 " from chunk group: %d in segment file: %" PRIu32 " at offset: %" PRIi64 ".",
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
     libfcache_cache_t *chunks_list_cache,
     off64_t offset,
     libewf_chunk_data_t **chunk_data,
     off64_t *chunk_data_offset,
     libcerror_error_t **error )
{
	libewf_segment_file_t *segment_file = NULL;
	libfdata_list_t *chunks_list        = NULL;
	static char *function               = "libewf_chunk_table_get_chunk_data_by_offset";
	off64_t chunk_offset                = 0;
	off64_t chunk_group_data_offset     = 0;
	off64_t segment_file_data_offset    = 0;
	size_t chunk_data_size              = 0;
	uint64_t start_sector               = 0;
	uint64_t number_of_sectors          = 0;
	uint32_t segment_number             = 0;
	int chunk_groups_list_index         = 0;
	int chunks_list_index               = 0;
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
	          offset,
	          &segment_number,
	          &segment_file_data_offset,
	          &segment_file,
		  &chunk_groups_list_index,
		  &chunk_group_data_offset,
		  &chunks_list,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file chunk group at offset: %" PRIi64 ".",
		 function,
		 offset );

		return( -1 );
	}
	if( result != 0 )
	{
		if( chunks_list == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunks list: %d.",
			 function,
			 chunk_groups_list_index );

			return( -1 );
		}
		result = libfdata_list_get_element_value_at_offset(
			  chunks_list,
			  (intptr_t *) file_io_pool,
			  chunks_list_cache,
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
			 "%s: unable to retrieve chunk: %" PRIu64 " data from chunk group: %d in segment file: %" PRIu32 " at offset: %" PRIi64 ".",
			 function,
			 chunk_index,
			 chunk_groups_list_index,
			 segment_number,
			 segment_file_data_offset );

			return( -1 );
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
			 "%s: unable to unpack chunk: %" PRIu64 " data.",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( ( ( *chunk_data )->range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
		{
			if( io_handle->zero_on_error != 0 )
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
					 "%s: unable to zero chunk: %" PRIu64 " data.",
					 function,
					 chunk_index );

					return( -1 );
				}
			}
			chunk_offset = offset - *chunk_data_offset;
		}
	}
	else
	{
		chunk_offset    = (off64_t) chunk_index * media_values->chunk_size;
		chunk_data_size = media_values->chunk_size;

		if( (size64_t) ( chunk_offset + chunk_data_size ) > media_values->media_size )
		{
			chunk_data_size = (size_t) ( media_values->media_size - chunk_offset );
		}
		if( libewf_chunk_data_initialize_clear_data(
		     chunk_data,
		     chunk_data_size,
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
		if( *chunk_data == NULL )
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
		( *chunk_data )->data_size    = chunk_data_size;
		( *chunk_data )->range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;
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

			return( -1 );
		}
	}
	return( 1 );
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
     libfcache_cache_t *chunks_list_cache,
     off64_t offset,
     libewf_chunk_data_t *chunk_data,
     libcerror_error_t **error )
{
	libewf_segment_file_t *segment_file = NULL;
	libfdata_list_t *chunks_list        = NULL;
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
	result = libewf_chunk_table_get_segment_file_chunk_group_by_offset(
	          chunk_table,
	          file_io_pool,
	          segment_table,
	          offset,
	          &segment_number,
	          &segment_file_data_offset,
	          &segment_file,
		  &chunk_groups_list_index,
		  &chunk_group_data_offset,
		  &chunks_list,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file chunk group at offset: %" PRIi64 ".",
		 function,
		 offset );

		return( -1 );
	}
	if( result != 0 )
	{
		if( chunks_list == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunks list: %d.",
			 function,
			 chunk_groups_list_index );

			return( -1 );
		}
		result = libfdata_list_set_element_value_at_offset(
			  chunks_list,
			  chunks_list_cache,
			  chunk_group_data_offset,
			  (intptr_t *) chunk_data,
		          (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_data_free,
		          LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set chunk: %" PRIu64 " data in chunk group: %d in segment file: %" PRIu32 " at offset: %" PRIi64 ".",
			 function,
			 chunk_index,
			 chunk_groups_list_index,
			 segment_number,
			 segment_file_data_offset );

			return( -1 );
		}
	}
	return( 1 );
}

