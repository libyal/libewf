/*
 * Segment file reading/writing functions
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBEWF_SEGMENT_FILE_H )
#define _LIBEWF_SEGMENT_FILE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_chunk_table.h"
#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libmfdata.h"
#include "libewf_list_type.h"
#include "libewf_media_values.h"
#include "libewf_section.h"
#include "libewf_sector_list.h"
#include "libewf_single_files.h"

#include "ewf_checksum.h"
#include "ewf_data.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t dvf_file_signature[ 8 ];
extern const uint8_t evf_file_signature[ 8 ];
extern const uint8_t lvf_file_signature[ 8 ];

typedef struct libewf_segment_file libewf_segment_file_t;

struct libewf_segment_file
{
	/* The segment file type
	 */
	uint8_t type;

	/* The segment number
	 */
	uint16_t segment_number;

        /* The list of all the sections
         */
        libewf_list_t *section_list;

	/* The number of chunks
	 */
	uint32_t number_of_chunks;

	/* Value to indicate that the segment file is open for writing
	 */
	uint8_t write_open;
};

int libewf_segment_file_initialize(
     libewf_segment_file_t **segment_file,
     liberror_error_t **error );

int libewf_segment_file_free(
     libewf_segment_file_t *segment_file,
     liberror_error_t **error );

int libewf_segment_file_clone(
     intptr_t **destination_segment_file,
     intptr_t *source_segment_file,
     liberror_error_t **error );

ssize_t libewf_segment_file_read_file_header(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_file_header(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         liberror_error_t **error );

int libewf_segment_file_read(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libmfdata_file_t *file,
     libmfdata_cache_t *cache,
     uint8_t read_flags,
     liberror_error_t **error );

ssize_t libewf_segment_file_read_table_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libewf_chunk_table_t *chunk_table,
         libmfdata_list_t *chunk_table_list,
         liberror_error_t **error );

ssize_t libewf_segment_file_read_table2_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_chunk_table_t *chunk_table,
         libmfdata_list_t *chunk_table_list,
         liberror_error_t **error );

ssize_t libewf_segment_file_read_volume_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libmfdata_list_t *chunk_table_list,
         liberror_error_t **error );

ssize_t libewf_segment_file_read_delta_chunk_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libmfdata_list_t *chunk_table_list,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_headers(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_last_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         int last_segment_file,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_start(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libewf_header_sections_t *header_sections,
         ewf_data_t **cached_data_section,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_chunks_section_start(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         ewf_table_offset_t *table_offsets,
         uint32_t number_of_table_offsets,
         uint32_t number_of_chunks_written,
         uint32_t chunks_per_section,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_chunks_section_correction(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         ewf_table_offset_t *table_offsets,
         uint32_t number_of_table_offsets,
         off64_t chunks_section_offset,
         size64_t chunks_section_size,
         uint32_t number_of_chunks,
         uint32_t section_number_of_chunks,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_chunk(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libmfdata_list_t *chunk_table_list,
         int chunk_index,
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         uint8_t *checksum_buffer,
         uint32_t *chunk_checksum,
         int8_t write_checksum,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_delta_chunk(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         int chunk_index,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         uint8_t *checksum_buffer,
         uint32_t *chunk_checksum,
         uint8_t write_checksum,
	 uint8_t no_section_append,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_close(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint32_t number_of_chunks_written_to_segment,
         int last_segment_file,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libewf_media_values_t *media_values,
         libewf_sector_list_t *sessions,
         libewf_sector_list_t *tracks,
         libewf_sector_list_t *acquiry_errors,
         ewf_data_t **cached_data_section,
         liberror_error_t **error );

int libewf_segment_file_write_sections_correction(
     libewf_segment_file_t *segment_file,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     uint32_t number_of_chunks_written_to_segment,
     int last_segment_file,
     libewf_media_values_t *media_values,
     libfvalue_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libewf_sector_list_t *sessions,
     libewf_sector_list_t *tracks,
     libewf_sector_list_t *acquiry_errors,
     ewf_data_t **cached_data_section,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

