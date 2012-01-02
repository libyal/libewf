/*
 * Section reading/writing functions
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

#if !defined( _LIBEWF_SECTION_H )
#define _LIBEWF_SECTION_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libmfdata.h"
#include "libewf_media_values.h"
#include "libewf_sector_list.h"
#include "libewf_single_files.h"

#include "ewf_checksum.h"
#include "ewf_data.h"
#include "ewf_section.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_section libewf_section_t;

struct libewf_section
{
        /* The section type string
         * consists of maximum 16 bytes
         */
        uint8_t type[ 17 ];

	/* The section type string length
	 */
	size_t type_length;

	/* The start offset of the section
	 */
	off64_t start_offset;

	/* The end offset of the section
	 */
	off64_t end_offset;

	/* The size the section
	 */
	size64_t size;
};

int libewf_section_test_zero(
     uint8_t *buffer,
     size_t buffer_size,
     liberror_error_t **error );

int libewf_section_initialize(
     libewf_section_t **section,
     liberror_error_t **error );

int libewf_section_free(
     libewf_section_t **section,
     liberror_error_t **error );

int libewf_section_clone(
     libewf_section_t **destination_section,
     libewf_section_t *source_section,
     liberror_error_t **error );

int libewf_section_set(
     libewf_section_t *sections,
     uint8_t *type,
     size_t type_length,
     off64_t start_offset,
     off64_t end_offset,
     liberror_error_t **error );

ssize_t libewf_section_start_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t file_offset,
         liberror_error_t **error );

ssize_t libewf_section_start_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         liberror_error_t **error );

#if defined( HAVE_DEBUG_OUTPUT )
ssize_t libewf_section_debug_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         liberror_error_t **error );
#endif

ssize_t libewf_section_last_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint8_t ewf_format,
         liberror_error_t **error );

ssize_t libewf_section_compressed_string_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **uncompressed_string,
         size_t *uncompressed_string_size,
         liberror_error_t **error );

ssize_t libewf_section_write_compressed_string(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint8_t *uncompressed_string,
         size_t uncompressed_string_size,
         int8_t compression_level,
         liberror_error_t **error );

ssize_t libewf_section_data_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         liberror_error_t **error );

ssize_t libewf_section_data_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         ewf_data_t **cached_data_section,
         liberror_error_t **error );

ssize_t libewf_section_digest_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error );

ssize_t libewf_section_digest_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error );

ssize_t libewf_section_error2_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_sector_list_t *acquiry_errors,
         liberror_error_t **error );

ssize_t libewf_section_error2_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_sector_list_t *acquiry_errors,
         liberror_error_t **error );

ssize_t libewf_section_hash_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error );

ssize_t libewf_section_hash_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error );

ssize_t libewf_section_header_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error );

ssize_t libewf_section_header_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         liberror_error_t **error );

ssize_t libewf_section_header2_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error );

ssize_t libewf_section_header2_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         liberror_error_t **error );

ssize_t libewf_section_ltree_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **cached_ltree_data,
         size_t *cached_ltree_data_size,
         liberror_error_t **error );

ssize_t libewf_section_sectors_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         size64_t chunks_data_size,
         liberror_error_t **error );

ssize_t libewf_section_session_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libewf_sector_list_t *sessions,
         libewf_sector_list_t *tracks,
         liberror_error_t **error );

ssize_t libewf_section_session_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_sector_list_t *sessions,
         libewf_sector_list_t *tracks,
         liberror_error_t **error );

ssize_t libewf_section_table_header_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format,
         uint32_t *number_of_offsets,
         uint64_t *base_offset,
         liberror_error_t **error );

ssize_t libewf_section_table_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         off64_t base_offset,
         ewf_table_offset_t *table_offsets,
         uint32_t number_of_offsets,
         size64_t chunks_data_size,
         uint8_t ewf_format,
         liberror_error_t **error );

ssize_t libewf_section_volume_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         liberror_error_t **error );

ssize_t libewf_section_volume_e01_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         liberror_error_t **error );

ssize_t libewf_section_volume_e01_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         liberror_error_t **error );

ssize_t libewf_section_volume_s01_read(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         liberror_error_t **error );

ssize_t libewf_section_volume_s01_write(
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         liberror_error_t **error );

ssize_t libewf_section_xhash_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         liberror_error_t **error );

ssize_t libewf_section_xhash_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         int8_t compression_level,
         liberror_error_t **error );

ssize_t libewf_section_xheader_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_header_sections_t *header_sections,
         liberror_error_t **error );

ssize_t libewf_section_xheader_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         liberror_error_t **error );

ssize_t libewf_section_delta_chunk_read(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint32_t *chunk_index,
         uint32_t *chunk_size,
         liberror_error_t **error );

ssize_t libewf_section_delta_chunk_write(
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint32_t chunk_index,
         uint8_t *chunk_buffer,
         uint32_t chunk_size,
         uint8_t *checksum_buffer,
         uint32_t *chunk_checksum,
         uint8_t write_checksum,
         liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

