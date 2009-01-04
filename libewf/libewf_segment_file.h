/*
 * Segment file reading/writing functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#include "libewf_file_io_pool.h"
#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_section_list.h"
#include "libewf_sector_table.h"
#include "libewf_segment_file_handle.h"

#include "ewf_crc.h"
#include "ewf_data.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_segment_file_check_file_signature(
     int file_descriptor );

ssize_t libewf_segment_file_read_file_header(
         libewf_segment_file_handle_t *segment_file_handle,
         uint16_t *segment_number,
         libewf_file_io_pool_t *file_io_pool );

int libewf_segment_file_read_sections(
     libewf_segment_file_handle_t *segment_file_handle,
     libewf_file_io_pool_t *file_io_pool,
     int *last_segment_file,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     int8_t *compression_level,
     uint8_t *format,
     uint8_t *ewf_format,
     size64_t *segment_file_size,
     uint8_t error_tollerance );

ssize_t libewf_segment_file_write_headers(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_file_io_pool_t *file_io_pool,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         uint8_t format );

ssize_t libewf_segment_file_write_last_section(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_file_io_pool_t *file_io_pool,
         int last_segment_file,
         uint8_t format,
         uint8_t ewf_format );

ssize_t libewf_segment_file_write_start(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_file_io_pool_t *file_io_pool,
         uint16_t segment_number,
         uint8_t segment_file_type,
         libewf_media_values_t *media_values,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         uint8_t format,
         uint8_t ewf_format,
         ewf_data_t **cached_data_section );

ssize_t libewf_segment_file_write_chunks_section_start(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_file_io_pool_t *file_io_pool,
         libewf_offset_table_t *offset_table,
         ewf_table_offset_t *table_offsets,
         uint32_t amount_of_table_offsets,
         size32_t chunk_size,
         uint32_t total_chunk_amount,
         uint32_t segment_chunk_amount,
         uint8_t format,
         uint8_t ewf_format );

ssize_t libewf_segment_file_write_chunks_data(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_file_io_pool_t *file_io_pool,
         libewf_offset_table_t *offset_table,
         uint32_t chunk,
         uint8_t *chunk_data,
         size_t chunk_data_size,
         int8_t is_compressed,
         ewf_crc_t *chunk_crc,
         int8_t write_crc );

ssize_t libewf_segment_file_write_chunks_correction(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_file_io_pool_t *file_io_pool,
         libewf_offset_table_t *offset_table,
         ewf_table_offset_t *table_offsets,
         uint32_t amount_of_table_offsets,
         off64_t chunks_section_offset,
         size64_t chunks_section_size,
         uint32_t amount_of_chunks,
         uint32_t section_amount_of_chunks,
         uint8_t format,
         uint8_t ewf_format );

ssize_t libewf_segment_file_write_delta_chunk(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_file_io_pool_t *file_io_pool,
         libewf_offset_table_t *offset_table,
         uint32_t chunk,
         uint8_t *chunk_data,
         size_t chunk_size,
         ewf_crc_t *chunk_crc,
         uint8_t write_crc,
	 uint8_t no_section_append );

ssize_t libewf_segment_file_write_close(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_file_io_pool_t *file_io_pool,
         uint16_t segment_number,
         uint32_t segment_amount_of_chunks,
         int last_segment_file,
         libewf_hash_sections_t *hash_sections,
         libewf_values_table_t *hash_values,
         libewf_media_values_t *media_values,
         libewf_sector_table_t *sessions,
         libewf_sector_table_t *acquiry_errors,
         int8_t compression_level,
         uint8_t format,
         uint8_t ewf_format,
         ewf_data_t **cached_data_section );

#if defined( __cplusplus )
}
#endif

#endif

