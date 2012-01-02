/*
 * Chunk table functions
 *
 * Copyright (c) 2006-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBEWF_CHUNK_TABLE_H )
#define _LIBEWF_CHUNK_TABLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libmfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_section.h"

#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_chunk_table libewf_chunk_table_t;

struct libewf_chunk_table
{
	/* The IO handle
	 */
	libewf_io_handle_t *io_handle;

	/* The chunk size
	 */
	uint32_t chunk_size;

	/* The previous last chunk that was filled
	 */
	int previous_last_chunk_filled;

	/* The last chunk that was filled
	 */
	int last_chunk_filled;

	/* The last chunk that was compared
	 */
	int last_chunk_compared;
};

int libewf_chunk_table_initialize(
     libewf_chunk_table_t **chunk_table,
     libewf_io_handle_t *io_handle,
     liberror_error_t **error );

int libewf_chunk_table_free(
     libewf_chunk_table_t **chunk_table,
     liberror_error_t **error );

int libewf_chunk_table_clone(
     intptr_t **destination_chunk_table,
     intptr_t *source_chunk_table,
     liberror_error_t **error );

int libewf_chunk_table_read_chunk(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libmfdata_list_element_t *list_element,
     libmfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_range_flags,
     uint8_t read_flags,
     liberror_error_t **error );

int libewf_chunk_table_read_offsets(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libmfdata_list_t *chunk_table_list,
     int element_index,
     int number_of_elements,
     libmfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_group_offset,
     size64_t element_group_size,
     uint8_t read_flags,
     liberror_error_t **error );

int libewf_chunk_table_fill(
     libewf_chunk_table_t *chunk_table,
     libmfdata_list_t *chunk_table_list,
     int chunk_index,
     int file_io_pool_entry,
     libewf_section_t *table_section,
     off64_t base_chunk,
     ewf_table_offset_t *table_offsets,
     uint32_t number_of_offsets,
     uint8_t tainted,
     liberror_error_t **error );

int libewf_chunk_table_correct(
     libewf_chunk_table_t *chunk_table,
     libmfdata_list_t *chunk_table_list,
     int chunk_index,
     int file_io_pool_entry,
     libewf_section_t *table_section,
     off64_t base_chunk,
     ewf_table_offset_t *table_offsets,
     uint32_t number_of_offsets,
     uint8_t tainted,
     liberror_error_t **error );

int libewf_chunk_table_fill_offsets(
     libmfdata_list_t *chunk_table_list,
     int chunk_index,
     off64_t base_offset,
     ewf_table_offset_t *table_offsets,
     uint32_t number_of_offsets,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

