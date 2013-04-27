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

#if !defined( _LIBEWF_CHUNK_TABLE_NEW_H )
#define _LIBEWF_CHUNK_TABLE_NEW_H

#include <common.h>
#include <types.h>

#include "libewf_chunk_table.h"
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
     libcerror_error_t **error );

int libewf_chunk_table_chunk_exists_for_offset(
     libewf_chunk_table_t *chunk_table,
     uint64_t chunk_index,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     off64_t offset,
     libcerror_error_t **error );

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
     libcerror_error_t **error );

int libewf_chunk_table_set_chunk_data_by_offset(
     libewf_chunk_table_t *chunk_table,
     uint64_t chunk_index,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     libfcache_cache_t *chunks_list_cache,
     off64_t offset,
     libewf_chunk_data_t *chunk_data,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

