/*
 * Low level writing functions
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

#if !defined( _LIBEWF_WRITE_IO_HANDLE_H )
#define _LIBEWF_WRITE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libewf_chunk_data.h"
#include "libewf_chunk_group.h"
#include "libewf_chunk_table.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcdata.h"
#include "libewf_libfdata.h"
#include "libewf_libfvalue.h"
#include "libewf_io_handle.h"
#include "libewf_media_values.h"
#include "libewf_read_io_handle.h"
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
	/* Flags used for chunk packing
	 */
	uint8_t pack_flags;

	/* The size of a section descriptor
	 */
	size_t section_descriptor_size;

	/* The size of a (sector) table header
	 */
	size_t table_header_size;

	/* The size of a (sector) table entry
	 */
	size_t table_entry_size;

	/* The size to reserve for (the end of) an chunks section
	 */
	size_t chunks_section_reserved_size;

	/* The size to reserve for the table entries of a chunk
	 */
	size_t chunk_table_entries_reserved_size;

	/* The acquiry timestamp
	 */
	time_t timestamp;

	/* A cached version of the case data
	 */
	uint8_t *case_data;

	/* The size of the cached version of the case data
	 */
	size_t case_data_size;

	/* A cached version of the device information
	 */
	uint8_t *device_information;

	/* The size of the cached version of the device information
	 */
	size_t device_information_size;

	/* A cached version of the data section
	 */
	ewf_data_t *data_section;

	/* Cached version of the table section
	 */
	uint8_t *table_section_data;

	/* The size of the cached version of the table sectoin
	 */
	size_t table_section_data_size;

	/* Cached version of the table entries
	 */
	uint8_t *table_entries_data;

	/* The size of the cached version of the table entries
	 */
	size_t table_entries_data_size;

	/* The number of allocated table entries
	 */
	uint32_t number_of_table_entries;

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
	uint32_t maximum_number_of_segments;

	/* The group of chunks written to the current chunks section
	 */
	libewf_chunk_group_t *chunk_group;

	/* The array chunks written to the current chunks section
	 */
	libcdata_array_t *chunks_section;

	/* The number of bytes written to a section containing chunks
	 */
	ssize64_t chunks_section_write_count;

	/* The size of the padding of a section containing chunks
	 */
	uint32_t chunks_section_padding_size;

        /* The (total) number of chunks written
         */
        uint64_t number_of_chunks_written;

        /* The current segment file IO pool entry
         */
        int current_file_io_pool_entry;

        /* The current segment number
         */
        uint32_t current_segment_number;

        /* The current segment file
         */
        libewf_segment_file_t *current_segment_file;

        /* The number of chunks written of the current segment file
         */
        uint64_t number_of_chunks_written_to_segment_file;

        /* The number of chunks written of the current (chunks) section
         */
        uint32_t number_of_chunks_written_to_section;

        /* The determined (estimated) number of chunks per segment file
         */
        uint64_t chunks_per_segment_file;

        /* The determined (estimated) number of chunks per section
         */
        uint32_t chunks_per_section;

        /* The maximum number of chunks that can be written to a chunks section
         */
        uint32_t maximum_chunks_per_section;

	/* The offset of the chunks section within the current segment file
	 */
	off64_t chunks_section_offset;

	/* Value to indicate the write values were initialized
	 */
	uint8_t values_initialized;

	/* Value to indicate a new chunks section should be created
	 */
	uint8_t create_chunks_section;

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
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error );

int libewf_write_io_handle_free(
     libewf_write_io_handle_t **write_io_handle,
     libcerror_error_t **error );

int libewf_write_io_handle_clone(
     libewf_write_io_handle_t **destination_write_io_handle,
     libewf_write_io_handle_t *source_write_io_handle,
     libcerror_error_t **error );

int libewf_write_io_handle_initialize_values(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     libcerror_error_t **error );

int libewf_write_io_handle_initialize_resume(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     libewf_read_io_handle_t *read_io_handle,
     off64_t *current_offset,
     libcerror_error_t **error );

int libewf_write_io_handle_resize_table_entries(
     libewf_write_io_handle_t *write_io_handle,
     uint32_t number_of_entries,
     libcerror_error_t **error );

int libewf_write_io_handle_calculate_chunks_per_segment_file(
     libewf_write_io_handle_t *write_io_handle,
     libewf_media_values_t *media_values,
     uint8_t segment_file_type,
     uint8_t format,
     libcerror_error_t **error );

int libewf_write_io_handle_calculate_chunks_per_section(
     libewf_write_io_handle_t *write_io_handle,
     libcerror_error_t **error );

int libewf_write_io_handle_test_segment_file_full(
     libewf_write_io_handle_t *write_io_handle,
     libewf_media_values_t *media_values,
     uint8_t segment_file_type,
     uint8_t format,
     libcerror_error_t **error );

int libewf_write_io_handle_test_chunks_section_full(
     libewf_write_io_handle_t *write_io_handle,
     libewf_media_values_t *media_values,
     off64_t segment_file_offset,
     uint8_t segment_file_type,
     uint8_t format,
     libcerror_error_t **error );

int libewf_write_io_handle_create_segment_file(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     uint8_t segment_file_type,
     uint32_t segment_number,
     uint32_t maximum_number_of_segments,
     const uint8_t *set_identifier,
     int *file_io_pool_entry,
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error );

int libewf_write_io_handle_generate_table_entries_data(
     libewf_write_io_handle_t *write_io_handle,
     uint64_t chunk_index,
     uint8_t format_version,
     uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint32_t number_of_entries,
     off64_t base_offset,
     libcerror_error_t **error );

ssize_t libewf_write_io_handle_write_chunks_section_start(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_segment_file_t *segment_file,
         libcerror_error_t **error );

ssize_t libewf_write_io_handle_write_chunks_section_end(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_segment_file_t *segment_file,
         libcerror_error_t **error );

ssize_t libewf_write_io_handle_write_new_chunk_create_segment_file(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         libewf_segment_table_t *segment_table,
         libfvalue_table_t *header_values,
         uint32_t segment_number,
         int *file_io_pool_entry,
         libewf_segment_file_t **segment_file,
         libcerror_error_t **error );

ssize_t libewf_write_io_handle_write_new_chunk_create_chunks_section(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         int file_io_pool_entry,
         libewf_segment_file_t *segment_file,
         libcerror_error_t **error );

ssize_t libewf_write_io_handle_write_new_chunk_create_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_segment_file_t *segment_file,
         uint64_t chunk_index,
         libewf_chunk_data_t *chunk_data,
         size_t input_data_size,
         libcerror_error_t **error );

ssize_t libewf_write_io_handle_write_new_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         libewf_segment_table_t *segment_table,
         libfvalue_table_t *header_values,
         libfvalue_table_t *hash_values,
         libewf_hash_sections_t *hash_sections,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcdata_range_list_t *acquiry_errors,
         uint64_t chunk_index,
         libewf_chunk_data_t *chunk_data,
         size_t input_data_size,
         libcerror_error_t **error );

int libewf_write_io_handle_finalize_write_sections_corrections(
     libewf_write_io_handle_t *write_io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     libfvalue_table_t *header_values,
     libfvalue_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libcdata_array_t *sessions,
     libcdata_array_t *tracks,
     libcdata_range_list_t *acquiry_errors,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_WRITE_IO_HANDLE_H ) */

