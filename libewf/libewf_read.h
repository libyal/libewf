/*
 * File reading functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _LIBEWF_READ_H )
#define _LIBEWF_READ_H

#include <common.h>
#include <types.h>

#include <libewf/handle.h>

#include "libewf_extern.h"
#include "libewf_handle.h"

#include "ewf_crc.h"

#if defined( __cplusplus )
extern "C" {
#endif

ssize_t libewf_read_process_chunk_data(
         libewf_internal_handle_t *internal_handle,
         uint8_t *chunk_data,
         size_t chunk_data_size,
         uint8_t *uncompressed_chunk_data,
         size_t *uncompressed_chunk_data_size,
         int8_t is_compressed,
         ewf_crc_t chunk_crc,
         int8_t read_crc );

ssize_t libewf_raw_read_chunk(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         int8_t *is_compressed,
         ewf_crc_t *chunk_crc,
         int8_t *read_crc );

ssize_t libewf_read_chunk_data(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint32_t chunk_offset,
         uint8_t *buffer,
         size_t size );

LIBEWF_EXTERN ssize_t libewf_raw_read_prepare_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       void *uncompressed_buffer,
                       size_t *uncompressed_buffer_size,
                       int8_t is_compressed,
                       uint32_t chunk_crc,
                       int8_t read_crc );

LIBEWF_EXTERN ssize_t libewf_raw_read_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t size,
                       int8_t *is_compressed,
                       uint32_t *chunk_crc,
                       int8_t *read_crc );

LIBEWF_EXTERN ssize_t libewf_read_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t size );

LIBEWF_EXTERN ssize_t libewf_read_random(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t size,
                       off64_t offset );

#if defined( __cplusplus )
}
#endif

#endif

