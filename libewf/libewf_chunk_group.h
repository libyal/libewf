/*
 * Chunk group functions
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

#if !defined( _LIBEWF_CHUNK_GROUP_H )
#define _LIBEWF_CHUNK_GROUP_H

#include <common.h>
#include <types.h>

#include "libewf_io_handle.h"
#include "libewf_libcerror.h"
#include "libewf_libfdata.h"
#include "libewf_section_descriptor.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_chunk_group libewf_chunk_group_t;

struct libewf_chunk_group
{
	/* The chunks list
	 */
	libfdata_list_t *chunks_list;

	/* The range start offset
	 */
	off64_t range_start_offset;

	/* The range end offset
	 */
	off64_t range_end_offset;
};

int libewf_chunk_group_initialize(
     libewf_chunk_group_t **chunk_group,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error );

int libewf_chunk_group_free(
     libewf_chunk_group_t **chunk_group,
     libcerror_error_t **error );

int libewf_chunk_group_clone(
     libewf_chunk_group_t **destination_chunk_group,
     libewf_chunk_group_t *source_chunk_group,
     libcerror_error_t **error );

int libewf_chunk_group_empty(
     libewf_chunk_group_t *chunk_group,
     libcerror_error_t **error );

int libewf_chunk_group_fill_v1(
     libewf_chunk_group_t *chunk_group,
     uint64_t chunk_index,
     size32_t chunk_size,
     int file_io_pool_entry,
     libewf_section_descriptor_t *table_section,
     off64_t base_offset,
     uint32_t number_of_entries,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error );

int libewf_chunk_group_fill_v2(
     libewf_chunk_group_t *chunk_group,
     uint64_t chunk_index,
     size32_t chunk_size,
     int file_io_pool_entry,
     libewf_section_descriptor_t *table_section,
     uint32_t number_of_entries,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error );

int libewf_chunk_group_correct_v1(
     libewf_chunk_group_t *chunk_group,
     uint64_t chunk_index,
     size32_t chunk_size,
     int file_io_pool_entry,
     libewf_section_descriptor_t *table_section,
     off64_t base_offset,
     uint32_t number_of_entries,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_CHUNK_GROUP_H ) */

