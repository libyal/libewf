/*
 * Section descriptor functions
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

#if !defined( _LIBEWF_SECTION_DESCRIPTOR_H )
#define _LIBEWF_SECTION_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libewf_libbfio.h"
#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_section_descriptor libewf_section_descriptor_t;

struct libewf_section_descriptor
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

int libewf_section_descriptor_initialize(
     libewf_section_descriptor_t **section_descriptor,
     libcerror_error_t **error );

int libewf_section_descriptor_free(
     libewf_section_descriptor_t **section_descriptor,
     libcerror_error_t **error );

int libewf_section_descriptor_clone(
     libewf_section_descriptor_t **destination_section_descriptor,
     libewf_section_descriptor_t *source_section_descriptor,
     libcerror_error_t **error );

int libewf_section_descriptor_set(
     libewf_section_descriptor_t *section_descriptor,
     uint32_t type,
     const uint8_t *type_string,
     size_t type_string_length,
     off64_t section_offset,
     size64_t section_size,
     size64_t data_size,
     uint32_t padding_size,
     libcerror_error_t **error );

int libewf_section_descriptor_read_data(
     libewf_section_descriptor_t *section_descriptor,
     const uint8_t *data,
     size_t data_size,
     off64_t file_offset,
     uint8_t format_version,
     libcerror_error_t **error );

ssize_t libewf_section_descriptor_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t file_offset,
         uint8_t format_version,
         libcerror_error_t **error );

int libewf_section_descriptor_write_data(
     libewf_section_descriptor_t *section_descriptor,
     uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libcerror_error_t **error );

ssize_t libewf_section_descriptor_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SECTION_DESCRIPTOR_H ) */

