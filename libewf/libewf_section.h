/*
 * Section reading/writing functions
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
#include "libewf_section_descriptor.h"
#include "libewf_single_files.h"

#include "ewf_data.h"
#include "ewf_section.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_section_test_zero(
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error );

int libewf_section_get_data_offset(
     libewf_section_descriptor_t *section_descriptor,
     uint8_t format_version,
     off64_t *data_offset,
     libcerror_error_t **error );

ssize_t libewf_section_read_data(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **section_data,
         size_t *section_data_size,
         libcerror_error_t **error );

ssize_t libewf_section_write_data(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         const uint8_t *section_data,
         size_t section_data_size,
         libcerror_error_t **error );

ssize_t libewf_section_compressed_string_read(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint16_t compression_method,
         uint8_t **uncompressed_string,
         size_t *uncompressed_string_size,
         libcerror_error_t **error );

ssize_t libewf_section_write_compressed_string(
         libewf_section_descriptor_t *section_descriptor,
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
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         int *set_identifier_change,
         libcerror_error_t **error );

ssize_t libewf_section_data_write(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_media_values_t *media_values,
         ewf_data_t **cached_data_section,
         libcerror_error_t **error );

ssize_t libewf_section_sectors_write(
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         size64_t chunks_data_size,
         uint32_t chunks_padding_size,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SECTION_H ) */

