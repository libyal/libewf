/*
 * Low level reading functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBEWF_READ_IO_HANDLE_H )
#define _LIBEWF_READ_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_chunk_cache.h"
#include "libewf_libbfio.h"
#include "libewf_io_handle.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_sector_table.h"

#include "ewf_crc.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_read_io_handle libewf_read_io_handle_t;

struct libewf_read_io_handle
{
	/* The sectors with CRC errors
	 */
	libewf_sector_table_t *crc_errors;

	/* A value to indicate if a chunk should be wiped on error
	 */
	uint8_t wipe_on_error;
};

int libewf_read_io_handle_initialize(
     libewf_read_io_handle_t **read_io_handle,
     liberror_error_t **error );

int libewf_read_io_handle_free(
     libewf_read_io_handle_t **read_io_handle,
     liberror_error_t **error );

ssize_t libewf_read_io_handle_process_chunk(
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         uint8_t *uncompressed_buffer,
         size_t *uncompressed_buffer_size,
         int8_t is_compressed,
         ewf_crc_t chunk_crc,
         int8_t read_crc,
         uint8_t *crc_mismatch,
         liberror_error_t **error );

ssize_t libewf_read_io_handle_read_chunk(
         libewf_io_handle_t *io_handle,
         libewf_offset_table_t *offset_table,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         int8_t *is_compressed,
         uint8_t *crc_buffer,
         ewf_crc_t *chunk_crc,
         int8_t *read_crc,
         liberror_error_t **error );

ssize_t libewf_read_io_handle_read_chunk_data(
         libewf_read_io_handle_t *read_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_chunk_cache_t *chunk_cache,
         uint32_t chunk,
         uint32_t chunk_offset,
         uint8_t *buffer,
         size_t size,
         liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

