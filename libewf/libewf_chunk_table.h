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

#if !defined( _LIBEWF_CHUNK_TABLE_H )
#define _LIBEWF_CHUNK_TABLE_H

#include <common.h>
#include <types.h>

#include "libewf_chunk_group.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_chunk_table libewf_chunk_table_t;

struct libewf_chunk_table
{
	/* The IO handle
	 */
	libewf_io_handle_t *io_handle;

	/* The current chunk group index
	 */
	int current_chunk_group_index;

	/* The current chunk group
	 */
	libewf_chunk_group_t *current_chunk_group;

	/* The current chunk data
	 */
	libewf_chunk_data_t *current_chunk_data;

	/* The chunk size
	 */
	uint32_t chunk_size;

	/* The format version
	 */
	uint8_t format_version;

	/* The sectors with checksum errors
	 */
	libcdata_range_list_t *checksum_errors;

	/* The chunk groups cache
	 */
	libfcache_cache_t *chunk_groups_cache;

	/* The chunk data cache
	 */
	libfcache_cache_t *chunk_data_cache;

	/* The single chunk data cache
	 */
	libfcache_cache_t *single_chunk_data_cache;
};

int libewf_chunk_table_initialize(
     libewf_chunk_table_t **chunk_table,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error );

int libewf_chunk_table_free(
     libewf_chunk_table_t **chunk_table,
     libcerror_error_t **error );

int libewf_chunk_table_clone(
     libewf_chunk_table_t **destination_chunk_table,
     libewf_chunk_table_t *source_chunk_table,
     libcerror_error_t **error );

int libewf_chunk_table_get_number_of_checksum_errors(
     libewf_chunk_table_t *chunk_table,
     uint32_t *number_of_errors,
     libcerror_error_t **error );

int libewf_chunk_table_get_checksum_error(
     libewf_chunk_table_t *chunk_table,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

int libewf_chunk_table_append_checksum_error(
     libewf_chunk_table_t *chunk_table,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error );

int libewf_chunk_table_get_segment_file_chunk_group_by_offset(
     libewf_chunk_table_t *chunk_table,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     off64_t offset,
     uint32_t *segment_number,
     int *chunk_groups_list_index,
     off64_t *chunk_group_data_offset,
     libewf_chunk_group_t **chunk_group,
     libcerror_error_t **error );

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
     libcerror_error_t **error );

int libewf_chunk_table_get_chunk_data_by_offset(
     libewf_chunk_table_t *chunk_table,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     off64_t offset,
     off64_t *chunk_data_offset,
     libewf_chunk_data_t **chunk_data,
     libcerror_error_t **error );

int libewf_chunk_table_get_chunk_data_by_offset_no_cache(
     libewf_chunk_table_t *chunk_table,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     off64_t offset,
     off64_t *chunk_data_offset,
     libewf_chunk_data_t **chunk_data,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_CHUNK_TABLE_H ) */

