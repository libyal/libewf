/*
 * Segment file reading/writing functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_section_list.h"
#include "libewf_sector_table.h"
#include "libewf_segment_file_handle.h"
#include "libewf_system_string.h"

#include "ewf_crc.h"
#include "ewf_data.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t dvf_file_signature[ 8 ];
extern const uint8_t evf_file_signature[ 8 ];
extern const uint8_t lvf_file_signature[ 8 ];

ssize_t libewf_segment_file_read_file_header(
         libewf_segment_file_handle_t *segment_file_handle,
         uint16_t *segment_number,
         libbfio_pool_t *file_io_pool,
         liberror_error_t **error );

int libewf_segment_file_read_sections(
     libewf_segment_file_handle_t *segment_file_handle,
     libewf_io_handle_t *io_handle,
     int *last_segment_file,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     liberror_error_t **error );

ssize_t libewf_segment_file_write_headers(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_last_section(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         int last_segment_file,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_start(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         uint16_t segment_number,
         uint8_t segment_file_type,
         libewf_media_values_t *media_values,
         libewf_header_sections_t *header_sections,
         ewf_data_t **cached_data_section,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_chunks_section_start(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_offset_table_t *offset_table,
         ewf_table_offset_t *table_offsets,
         uint32_t amount_of_table_offsets,
         size32_t chunk_size,
         uint32_t total_chunk_amount,
         uint32_t segment_chunk_amount,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_chunks_section_correction(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_offset_table_t *offset_table,
         ewf_table_offset_t *table_offsets,
         uint32_t amount_of_table_offsets,
         off64_t chunks_section_offset,
         size64_t chunks_section_size,
         uint32_t amount_of_chunks,
         uint32_t section_amount_of_chunks,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_chunk(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_offset_table_t *offset_table,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         int8_t is_compressed,
         uint8_t *crc_buffer,
         ewf_crc_t *chunk_crc,
         int8_t write_crc,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_delta_chunk(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         libewf_offset_table_t *offset_table,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         uint8_t *crc_buffer,
         ewf_crc_t *chunk_crc,
         uint8_t write_crc,
	 uint8_t no_section_append,
         liberror_error_t **error );

ssize_t libewf_segment_file_write_close(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_io_handle_t *io_handle,
         uint16_t segment_number,
         uint32_t segment_amount_of_chunks,
         int last_segment_file,
         libewf_hash_sections_t *hash_sections,
         libewf_values_table_t *hash_values,
         libewf_media_values_t *media_values,
         libewf_sector_table_t *sessions,
         libewf_sector_table_t *acquiry_errors,
         ewf_data_t **cached_data_section,
         liberror_error_t **error );

int libewf_segment_file_write_sections_correction(
     libewf_segment_file_handle_t *segment_file_handle,
     libewf_io_handle_t *io_handle,
     uint16_t segment_number,
     uint32_t segment_amount_of_chunks,
     int last_segment_file,
     libewf_media_values_t *media_values,
     libewf_values_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     ewf_data_t **cached_data_section,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

