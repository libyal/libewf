/*
 * Segment file reading/writing functions
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

#if !defined( _LIBEWF_SEGMENT_FILE_H )
#define _LIBEWF_SEGMENT_FILE_H

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
#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_libfvalue.h"
#include "libewf_media_values.h"
#include "libewf_section_descriptor.h"
#include "libewf_single_files.h"

#include "ewf_data.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t ewf1_dvf_file_signature[ 8 ];
extern const uint8_t ewf1_evf_file_signature[ 8 ];
extern const uint8_t ewf1_lvf_file_signature[ 8 ];
extern const uint8_t ewf2_evf_file_signature[ 8 ];
extern const uint8_t ewf2_lef_file_signature[ 8 ];

typedef struct libewf_segment_file libewf_segment_file_t;

struct libewf_segment_file
{
	/* The IO handle
	 */
	libewf_io_handle_t *io_handle;

	/* The segment file type
	 */
	uint8_t type;

	/* The major version number
	 */
	uint8_t major_version;

	/* The minor version number
	 */
	uint8_t minor_version;

	/* The segment number
	 */
	uint32_t segment_number;

	/* The set identifier
	 */
	uint8_t set_identifier[ 16 ];

	/* The compression method
	 */
	uint16_t compression_method;

	/* The current offset
	 */
	off64_t current_offset;

	/* The last section offset
	 */
	off64_t last_section_offset;

        /* The the sections list
         */
        libfdata_list_t *sections_list;

	/* The device information section index
	 */
	int device_information_section_index;

	/* The chunk groups list
	 */
	libfdata_list_t *chunk_groups_list;

	/* The current chunk groups index
	 */
	int current_chunk_group_index;

	/* The storage media size (in the segment file)
	 */
	size64_t storage_media_size;

	/* The number of chunks (in the segment file)
	 */
	uint64_t number_of_chunks;

	/* The previous last chunk that was filled
	 */
	int64_t previous_last_chunk_filled;

	/* The last chunk that was filled
	 */
	int64_t last_chunk_filled;

	/* The last chunk that was compared
	 */
	int64_t last_chunk_compared;

	/* Flags
	 */
	uint8_t flags;

	/* The range start offset
	 */
	off64_t range_start_offset;

	/* The range end offset
	 */
	off64_t range_end_offset;

	/* The chunk groups cache
	 */
	libfcache_cache_t *chunk_groups_cache;
};

int libewf_segment_file_initialize(
     libewf_segment_file_t **segment_file,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error );

int libewf_segment_file_free(
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error );

int libewf_segment_file_clone(
     libewf_segment_file_t **destination_segment_file,
     libewf_segment_file_t *source_segment_file,
     libcerror_error_t **error );

int libewf_segment_file_get_number_of_sections(
     libewf_segment_file_t *segment_file,
     int *number_of_sections,
     libcerror_error_t **error );

int libewf_segment_file_get_section_by_index(
     libewf_segment_file_t *segment_file,
     int section_index,
     libbfio_pool_t *file_io_pool,
     libfcache_cache_t *sections_cache,
     libewf_section_descriptor_t **section,
     libcerror_error_t **error );

int libewf_segment_file_read_file_header_data(
     libewf_segment_file_t *segment_file,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

ssize_t libewf_segment_file_read_file_header_file_io_pool(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_file_header(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error );

off64_t libewf_segment_file_seek_offset(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t offset,
         libcerror_error_t **error );

ssize_t libewf_segment_file_read_table_section(
         libewf_segment_file_t *segment_file,
         libewf_section_descriptor_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         size32_t chunk_size,
         libcerror_error_t **error );

ssize_t libewf_segment_file_read_table2_section(
         libewf_segment_file_t *segment_file,
         libewf_section_descriptor_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error );

ssize_t libewf_segment_file_read_volume_section(
         libewf_segment_file_t *segment_file,
         libewf_section_descriptor_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_device_information_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **device_information,
         size_t *device_information_size,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_case_data_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **case_data,
         size_t *case_data_size,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_header_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_header2_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_xheader_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_header_sections(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_last_section(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         int last_segment_file,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_start(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **case_data,
         size_t *case_data_size,
         uint8_t **device_information,
         size_t *device_information_size,
         ewf_data_t **data_section,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_chunks_section_start(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *table_section_data,
         size_t table_section_data_size,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_table_entries,
         uint64_t number_of_chunks_written,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_chunks_section_end(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *table_section_data,
         size_t table_section_data_size,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_table_entries,
         off64_t chunks_section_offset,
         size64_t chunks_section_size,
         uint32_t chunks_section_padding_size,
         uint64_t first_chunk_index,
         uint64_t base_offset,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_chunk_data(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint64_t chunk_index,
         libewf_chunk_data_t *chunk_data,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_hash_sections(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_close(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint64_t number_of_chunks_written_to_segment_file,
         int last_segment_file,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libewf_media_values_t *media_values,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcdata_range_list_t *acquiry_errors,
         ewf_data_t **data_section,
         libcerror_error_t **error );

int libewf_segment_file_reopen(
     libewf_segment_file_t *segment_file,
     int last_section_index,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libfcache_cache_t *sections_cache,
     libcerror_error_t **error );

int libewf_segment_file_write_sections_correction(
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     uint64_t number_of_chunks_written_to_segment_file,
     int last_segment_file,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     time_t timestamp,
     libfvalue_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libcdata_array_t *sessions,
     libcdata_array_t *tracks,
     libcdata_range_list_t *acquiry_errors,
     uint8_t **case_data,
     size_t *case_data_size,
     uint8_t **device_information,
     size_t *device_information_size,
     ewf_data_t **data_section,
     libcerror_error_t **error );

int libewf_segment_file_read_element_data(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *segment_file_cache,
     int file_io_pool_entry,
     off64_t element_offset,
     size64_t segment_file_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libewf_segment_file_read_section_element_data(
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t section_data_offset,
     size64_t section_data_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libewf_segment_file_read_chunk_group_element_data(
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t chunk_group_data_offset,
     size64_t chunk_group_data_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libewf_segment_file_get_chunk_group_by_offset(
     libewf_segment_file_t *segment_file,
     libbfio_pool_t *file_io_pool,
     off64_t offset,
     int *chunk_group_index,
     off64_t *chunk_group_data_offset,
     libewf_chunk_group_t **chunk_group,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SEGMENT_FILE_H ) */

