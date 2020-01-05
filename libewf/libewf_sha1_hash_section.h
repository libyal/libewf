/*
 * SHA1 hash section functions
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

#if !defined( _LIBEWF_SHA1_HASH_SECTION_H )
#define _LIBEWF_SHA1_HASH_SECTION_H

#include <common.h>
#include <types.h>

#include "libewf_hash_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_section_descriptor.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_sha1_hash_section_read_data(
     const uint8_t *data,
     size_t data_size,
     libewf_hash_sections_t *hash_sections,
     libcerror_error_t **error );

ssize_t libewf_sha1_hash_section_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error );

int libewf_sha1_hash_section_write_data(
     uint8_t *data,
     size_t data_size,
     libewf_hash_sections_t *hash_sections,
     libcerror_error_t **error );

ssize_t libewf_sha1_hash_section_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SHA1_HASH_SECTION_H ) */

