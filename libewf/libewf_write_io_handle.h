/*
 * Low level writing functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBEWF_WRITE_IO_HANDLE_H )
#define _LIBEWF_WRITE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_chunk_cache.h"
#include "libewf_libbfio.h"
#include "libewf_io_handle.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_read_io_handle.h"
#include "libewf_sector_table.h"
#include "libewf_segment_table.h"
#include "libewf_values_table.h"

#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_section.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_write_io_handle libewf_write_io_handle_t;

struct libewf_write_io_handle
{
	/* A cached version of the data section
	 */
	ewf_data_t *data_section;

	/* A cached version of the table offsets
	 */
	ewf_table_offset_t *table_offsets;

	/* The amount of allocated table offsets
	 */
	uint32_t amount_of_table_offsets;

	/* The amount of bytes of the input written
	 */
	ssize64_t input_write_count;

	/* The maximum segment file size
	 */
	size64_t maximum_segment_file_size;

	/* The remaining segment file size
	 */
	ssize64_t remaining_segment_file_size;

	/* The maximum amount of segments
	 */
	uint16_t maximum_amount_of_segments;

	/* The amount of bytes written to a section containing chunks
	 */
	ssize64_t chunks_section_write_count;

        /* The amount of chunks written
         */
        uint32_t amount_of_chunks;

        /* The determined (estimated) amount of chunks per segment
         */
        uint32_t chunks_per_segment;

        /* The determined (estimated) amount of chunks per chunks section
         */
        uint32_t chunks_per_chunks_section;

        /* The amount of chunks written of the current segment file
         */
        uint32_t segment_amount_of_chunks;

        /* The maximum amount of chunks that can be written to a chunks section
         */
        uint32_t maximum_section_amount_of_chunks;

        /* The amount of chunks written of the current chunks section
         */
        uint32_t section_amount_of_chunks;

	/* The offset of the chunks section within the current segment file
	 */
	off64_t chunks_section_offset;

	/* The current chunks section number
	 */
	uint8_t chunks_section_number;

	/* Value to indicate if the offset table should not be restricted
	 * to the maximum amount of offsets
	 */
	uint8_t unrestrict_offset_amount;

	/* Value to indicate the write values were initialized
	 */
	uint8_t values_initialized;

	/* Value to indicate a new chunks section should be created
	 */
	uint8_t create_chunks_section;

	/* The offset in the segment file from which to resume write
	 */
	off64_t resume_segment_file_offset;

	/* Value to indicate if the write has been finalized
	 */
	uint8_t write_finalized;
};

int libewf_write_io_handle_initialize(
     libewf_write_io_handle_t **write_io_handle,
     liberror_error_t **error );

int libewf_write_io_handle_free(
     libewf_write_io_handle_t **write_io_handle,
     liberror_error_t **error );

int libewf_write_io_handle_initialize_values(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     liberror_error_t **error );

int libewf_write_io_handle_initialize_resume(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_segment_table_t *segment_table,
     liberror_error_t **error );

int libewf_write_io_handle_test_empty_block(
     uint8_t *block_buffer,
     size_t size,
     liberror_error_t **error );

int libewf_write_io_handle_calculate_chunks_per_segment(
     uint32_t *chunks_per_segment,
     size64_t remaining_segment_file_size,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t segment_amount_of_chunks,
     uint32_t amount_of_chunks,
     libewf_media_values_t *media_values,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_amount,
     liberror_error_t **error );

int libewf_write_io_handle_calculate_chunks_per_chunks_section(
     uint32_t *chunks_per_chunks_section,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t chunks_per_segment,
     uint8_t chunks_section_number,
     uint8_t unrestrict_offset_amount,
     liberror_error_t **error );

int libewf_write_io_handle_test_segment_file_full(
     ssize64_t remaining_segment_file_size,
     uint32_t segment_amount_of_chunks,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     uint32_t chunks_per_segment,
     uint32_t current_amount_of_chunks,
     uint8_t format,
     uint8_t ewf_format,
     liberror_error_t **error );

int libewf_write_io_handle_test_chunks_section_full(
     off64_t chunks_section_offset,
     ssize64_t remaining_segment_file_size,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     off64_t segment_file_offset,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t section_amount_of_chunks,
     uint32_t current_amount_of_chunks,
     uint32_t chunks_per_chunks_section,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_amount,
     liberror_error_t **error );

ssize_t libewf_write_io_handle_process_chunk(
         libewf_chunk_cache_t *chunk_cache,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t compression_flags,
         uint8_t ewf_format,
         uint8_t *chunk_data,
         size_t chunk_data_size,
         uint8_t *compressed_chunk_data,
         size_t *compressed_chunk_data_size,
         int8_t *is_compressed,
         uint8_t chunk_exists,
         ewf_crc_t *chunk_crc,
         int8_t *write_crc,
         liberror_error_t **error );

ssize_t libewf_write_io_handle_write_new_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *segment_table,
         libewf_values_table_t **header_values,
         libewf_values_table_t *hash_values,
         libewf_header_sections_t *header_sections,
         libewf_hash_sections_t *hash_sections,
         libewf_sector_table_t *sessions,
         libewf_sector_table_t *acquiry_errors,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         uint8_t *crc_buffer,
         ewf_crc_t chunk_crc,
         int8_t write_crc,
         liberror_error_t **error );

ssize_t libewf_write_io_handle_write_existing_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *segment_table,
         libewf_header_sections_t *header_sections,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         uint8_t *crc_buffer,
         ewf_crc_t chunk_crc,
         int8_t write_crc,
         liberror_error_t **error );

ssize_t libewf_write_io_handle_write_new_chunk_data(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *segment_table,
         libewf_values_table_t **header_values,
         libewf_values_table_t *hash_values,
         libewf_header_sections_t *header_sections,
         libewf_hash_sections_t *hash_sections,
         libewf_sector_table_t *sessions,
         libewf_sector_table_t *acquiry_errors,
         libewf_chunk_cache_t *chunk_cache,
         uint32_t chunk,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         int8_t force_write,
         liberror_error_t **error );

ssize_t libewf_write_io_handle_write_existing_chunk_data(
         libewf_write_io_handle_t *write_io_handle,
         libewf_read_io_handle_t *read_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *delta_segment_table,
         libewf_header_sections_t *header_sections,
         libewf_chunk_cache_t *chunk_cache,
         uint32_t chunk,
         uint32_t chunk_offset,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         liberror_error_t **error );

ssize_t libewf_write_io_handle_finalize(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *segment_table,
         libewf_values_table_t **header_values,
         libewf_values_table_t *hash_values,
         libewf_header_sections_t *header_sections,
         libewf_hash_sections_t *hash_sections,
         libewf_sector_table_t *sessions,
         libewf_sector_table_t *acquiry_errors,
         libewf_chunk_cache_t *chunk_cache,
         liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

