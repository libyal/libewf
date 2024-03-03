/*
 * The section data handle functions
 *
 * Copyright (C) 2006-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_SECTION_DATA_HANDLE_H )
#define _LIBEWF_SECTION_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libewf_libbfio.h"
#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_section_data_handle libewf_section_data_handle_t;

struct libewf_section_data_handle
{
	/* The current offset
	 */
	off64_t current_offset;

	/* The (section) data offset
	 */
	off64_t data_offset;

	/* The (section) data size
	 */
	size64_t data_size;
};

int libewf_section_data_handle_initialize(
     libewf_section_data_handle_t **data_handle,
     off64_t data_offset,
     size64_t data_size,
     libcerror_error_t **error );

int libewf_section_data_handle_free(
     libewf_section_data_handle_t **data_handle,
     libcerror_error_t **error );

ssize_t libewf_section_data_handle_read_segment_data(
         libewf_section_data_handle_t *data_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         int file_io_pool_entry,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libewf_section_data_handle_seek_segment_offset(
         libewf_section_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SECTION_DATA_HANDLE_H ) */

