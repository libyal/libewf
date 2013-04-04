/*
 * Chunk group functions
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

#if !defined( _LIBEWF_CHUNK_GROUP_H )
#define _LIBEWF_CHUNK_GROUP_H

#include <common.h>
#include <types.h>

#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_section.h"

#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_chunk_group_fill_v1(
     libfdata_list_t *chunks_list,
     size32_t chunk_size,
     int file_io_pool_entry,
     libewf_section_t *table_section,
     off64_t base_offset,
     uint32_t number_of_offsets,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error );

int libewf_chunk_group_fill_v2(
     libfdata_list_t *chunks_list,
     size32_t chunk_size,
     int file_io_pool_entry,
     libewf_section_t *table_section,
     uint32_t number_of_offsets,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error );

int libewf_chunk_group_correct_v1(
     libfdata_list_t *chunks_list,
     size32_t chunk_size,
     int file_io_pool_entry,
     libewf_section_t *table_section,
     off64_t base_offset,
     uint32_t number_of_offsets,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error );

int libewf_chunk_group_generate_table_entries_data(
     libfdata_list_t *chunks_list,
     uint8_t format_version,
     uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint32_t number_of_entries,
     off64_t base_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

