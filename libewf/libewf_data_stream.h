/*
 * Data stream functions
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

#if !defined( _LIBEWF_DATA_STREAM_H )
#define _LIBEWF_DATA_STREAM_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_data_stream_initialize_from_buffer(
     libfdata_stream_t **data_stream,
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error );

int libewf_data_stream_initialize_from_section(
     libfdata_stream_t **data_stream,
     int file_io_pool_entry,
     off64_t data_offset,
     size64_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_DATA_STREAM_H ) */

