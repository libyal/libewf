/*
 * Section reading/writing functions
 *
 * Copyright (C) 2006-2017, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_SECTION_H )
#define _LIBEWF_SECTION_H

#include <common.h>
#include <types.h>

#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_media_values.h"
#include "libewf_single_files.h"

#include "ewf_data.h"
#include "ewf_section.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_section libewf_section_t;

struct libewf_section
{
	/* The section type
	 */
	uint32_t type;

        /* The section type string
         * consists of maximum 16 bytes
         */
        uint8_t type_string[ 17 ];

	/* The section type string length
	 */
	size_t type_string_length;

	/* The data flags
	 */
	uint32_t data_flags;

	/* The start offset of the section
	 */
	off64_t start_offset;

	/* The end offset of the section
	 */
	off64_t end_offset;

	/* The size of the section
	 */
	size64_t size;

	/* The data size
	 */
	size64_t data_size;

	/* The padding size
	 */
	uint32_t padding_size;

	/* The data integrity hash
	 */
	uint8_t data_integrity_hash[ 16 ];
};

int libewf_section_test_zero(
     uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error );

int libewf_section_initialize(
     libewf_section_t **section,
     libcerror_error_t **error );

int libewf_section_free(
     libewf_section_t **section,
     libcerror_error_t **error );

int libewf_section_clone(
     libewf_section_t **destination_section,
     libewf_section_t *source_section,
     libcerror_error_t **error );

int libewf_section_get_data_offset(
     libewf_section_t *section,
     uint8_t format_version,
     off64_t *data_offset,
     libcerror_error_t **error );

int libewf_section_set_values(
     libewf_section_t *section,
     uint32_t type,
     const uint8_t *type_string,
     size_t type_string_length,
     off64_t section_offset,
     size64_t section_size,
     size64_t data_size,
     uint32_t padding_size,
     libcerror_error_t **error );

ssize_t libewf_section_descriptor_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t file_offset,
         uint8_t format_version,
         libcerror_error_t **error );

ssize_t libewf_section_descriptor_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libcerror_error_t **error );

ssize_t libewf_section_read_data(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **section_data,
         size_t *section_data_size,
         libcerror_error_t **error );

ssize_t libewf_section_write_data(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         const uint8_t *section_data,
         size_t section_data_size,
         libcerror_error_t **error );

ssize_t libewf_section_compressed_string_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint16_t compression_method,
         uint8_t **uncompressed_string,
         size_t *uncompressed_string_size,
         libcerror_error_t **error );

ssize_t libewf_section_write_compressed_string(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint32_t type,
         const uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint16_t compression_method,
         int8_t compression_level,
         uint8_t *uncompressed_string,
         size_t uncompressed_string_size,
         size_t fill_size,
         libcerror_error_t **error );

ssize_t libewf_section_data_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         int *set_identifier_change,
         libcerror_error_t **error );

ssize_t libewf_section_data_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         ewf_data_t **cached_data_section,
         libcerror_error_t **error );

ssize_t libewf_section_digest_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error );

ssize_t libewf_section_digest_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error );

ssize_t libewf_section_error_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libcdata_range_list_t *acquiry_errors,
         libcerror_error_t **error );

ssize_t libewf_section_error_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libcdata_range_list_t *acquiry_errors,
         libcerror_error_t **error );

ssize_t libewf_section_md5_hash_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error );

ssize_t libewf_section_md5_hash_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error );

ssize_t libewf_section_sha1_hash_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error );

ssize_t libewf_section_sha1_hash_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error );

ssize_t libewf_section_ltree_read(
         libewf_section_t *section,
          libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t **section_data,
         size_t *section_data_size,
         uint8_t **ltree_data,
         size_t *ltree_data_size,
         libcerror_error_t **error );

ssize_t libewf_section_ltree_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         uint8_t *section_data,
         size_t section_data_size,
         uint8_t *ltree_data,
         size_t ltree_data_size,
         libcerror_error_t **error );

ssize_t libewf_section_sectors_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         size64_t chunks_data_size,
         uint32_t chunks_padding_size,
         libcerror_error_t **error );

ssize_t libewf_section_session_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libewf_media_values_t *media_values,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcerror_error_t **error );

ssize_t libewf_section_session_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcerror_error_t **error );

ssize_t libewf_section_table_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t segment_file_type,
         uint8_t **section_data,
         size_t *section_data_size,
         uint64_t *first_chunk_index,
         uint64_t *base_offset,
         uint8_t **table_entries_data,
         size_t *table_entries_data_size,
         uint32_t *number_of_entries,
         uint8_t *entries_corrupted,
         libcerror_error_t **error );

ssize_t libewf_section_table_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t segment_file_type,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint8_t *section_data,
         size_t section_data_size,
         uint64_t first_chunk_index,
         uint64_t base_offset,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_entries,
         size64_t chunks_data_size,
         libcerror_error_t **error );

ssize_t libewf_section_volume_e01_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libcerror_error_t **error );

ssize_t libewf_section_volume_e01_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         libcerror_error_t **error );

ssize_t libewf_section_volume_s01_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libcerror_error_t **error );

ssize_t libewf_section_volume_s01_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SECTION_H ) */

