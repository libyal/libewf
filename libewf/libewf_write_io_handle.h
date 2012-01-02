/*
 * Low level writing functions
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

#if !defined( _LIBEWF_WRITE_IO_HANDLE_H )
#define _LIBEWF_WRITE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_chunk_table.h"
#include "libewf_libbfio.h"
#include "libewf_libfvalue.h"
#include "libewf_libmfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_io_handle.h"
#include "libewf_media_values.h"
#include "libewf_read_io_handle.h"
#include "libewf_sector_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"

#include "ewf_data.h"
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

	/* The number of allocated table offsets
	 */
	uint32_t number_of_table_offsets;

	/* The number of bytes of the input written
	 */
	ssize64_t input_write_count;

	/* The maximum segment file size
	 */
	size64_t maximum_segment_file_size;

	/* The remaining segment file size
	 */
	ssize64_t remaining_segment_file_size;

	/* The maximum number of segments
	 */
	uint16_t maximum_number_of_segments;

	/* The number of bytes written to a section containing chunks
	 */
	ssize64_t chunks_section_write_count;

        /* The (total) number of chunks written
         */
        uint32_t number_of_chunks_written;

        /* The number of chunks written of the current segment file
         */
        uint32_t number_of_chunks_written_to_segment;

        /* The number of chunks written of the current (chunks) section
         */
        uint32_t number_of_chunks_written_to_section;

        /* The determined (estimated) number of chunks per segment file
         */
        uint32_t chunks_per_segment_file;

        /* The determined (estimated) number of chunks per section
         */
        uint32_t chunks_per_section;

        /* The maximum number of chunks that can be written to a chunks section
         */
        uint32_t maximum_chunks_per_section;

	/* The offset of the chunks section within the current segment file
	 */
	off64_t chunks_section_offset;

	/* Value to indicate if the offset table should not be restricted
	 * to the maximum number of offsets
	 */
	uint8_t unrestrict_offset_table;

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

	/* The compressed zero byte empty block
	 */
	uint8_t *compressed_zero_byte_empty_block;

	/* The size of the compressed zero byte empty block
	 */
	size_t compressed_zero_byte_empty_block_size;
};

int libewf_write_io_handle_initialize(
     libewf_write_io_handle_t **write_io_handle,
     liberror_error_t **error );

int libewf_write_io_handle_free(
     libewf_write_io_handle_t **write_io_handle,
     liberror_error_t **error );

int libewf_write_io_handle_clone(
     libewf_write_io_handle_t **destination_write_io_handle,
     libewf_write_io_handle_t *source_write_io_handle,
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
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libmfdata_file_list_t *segment_files_list,
     libmfcache_cache_t *segment_files_cache,
     libmfdata_list_t *chunk_table_list,
     libewf_chunk_table_t *chunk_table,
     libewf_segment_table_t *segment_table,
     liberror_error_t **error );

int libewf_write_io_handle_calculate_chunks_per_segment_file(
     uint32_t *chunks_per_segment_file,
     size64_t remaining_segment_file_size,
     uint32_t maximum_chunks_per_section,
     uint32_t number_of_chunks_written_to_segment,
     uint32_t number_of_chunks_written,
     libewf_media_values_t *media_values,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_table,
     liberror_error_t **error );

int libewf_write_io_handle_calculate_chunks_per_section(
     uint32_t *chunks_per_section,
     uint32_t maximum_chunks_per_section,
     uint32_t number_of_chunks_written_to_segment,
     uint32_t chunks_per_segment_file,
     uint8_t unrestrict_offset_table,
     liberror_error_t **error );

int libewf_write_io_handle_test_segment_file_full(
     ssize64_t remaining_segment_file_size,
     uint32_t number_of_chunks_written_to_segment,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     uint32_t chunks_per_segment_file,
     uint32_t number_of_chunks_written,
     uint8_t format,
     uint8_t ewf_format,
     liberror_error_t **error );

int libewf_write_io_handle_test_chunks_section_full(
     off64_t chunks_section_offset,
     ssize64_t remaining_segment_file_size,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     off64_t segment_file_offset,
     uint32_t maximum_chunks_per_section,
     uint32_t number_of_chunks_written_to_section,
     uint32_t number_of_chunks_written,
     uint32_t chunks_per_section,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_table,
     liberror_error_t **error );

int libewf_write_io_handle_create_segment_file(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libmfdata_file_list_t *segment_files_list,
     libmfcache_cache_t *segment_files_cache,
     libewf_segment_table_t *segment_table,
     uint16_t segment_number,
     uint16_t maximum_number_of_segments,
     uint8_t segment_file_type,
     libewf_segment_file_t **segment_file,
     int *segment_files_list_index,
     int *file_io_pool_entry,
     liberror_error_t **error );

ssize_t libewf_write_io_handle_write_new_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         libmfdata_file_list_t *segment_files_list,
         libmfcache_cache_t *segment_files_cache,
         libewf_segment_table_t *segment_table,
         libmfdata_list_t *chunk_table_list,
         libfvalue_table_t *header_values,
         libfvalue_table_t *hash_values,
         libewf_header_sections_t *header_sections,
         libewf_hash_sections_t *hash_sections,
         libewf_sector_list_t *sessions,
         libewf_sector_list_t *tracks,
         libewf_sector_list_t *acquiry_errors,
         int chunk_index,
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         uint8_t *checksum_buffer,
         uint32_t chunk_checksum,
         int8_t write_checksum,
         liberror_error_t **error );

ssize_t libewf_write_io_handle_write_existing_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         libmfdata_file_list_t *delta_segment_files_list,
         libmfcache_cache_t *segment_files_cache,
         libewf_segment_table_t *delta_segment_table,
         libmfdata_list_t *chunk_table_list,
         libewf_header_sections_t *header_sections,
         int chunk_index,
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         uint8_t *checksum_buffer,
         uint32_t chunk_checksum,
         int8_t write_checksum,
         liberror_error_t **error );

int libewf_write_io_handle_finalize_write_sections_corrections(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     uint32_t number_of_chunks_written_to_last_segment,
     libewf_media_values_t *media_values,
     libmfdata_file_list_t *segment_files_list,
     libmfcache_cache_t *segment_files_cache,
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

