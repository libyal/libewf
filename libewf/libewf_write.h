/*
 * File writing functions
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

#if !defined( _LIBEWF_WRITE_H )
#define _LIBEWF_WRITE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <libewf/handle.h>

#include "libewf_chunk_cache.h"
#include "libewf_extern.h"
#include "libewf_handle.h"
#include "libewf_media_values.h"
#include "libewf_segment_file_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_write_test_empty_block(
     uint8_t *block_buffer,
     size_t size,
     liberror_error_t **error );

int libewf_write_calculate_chunks_per_segment(
     uint32_t *chunks_per_segment,
     size64_t remaining_segment_file_size,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t segment_amount_of_chunks,
     uint32_t amount_of_chunks,
     libewf_media_values_t *media_values,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_amount,
     liberror_error_t **error );

int libewf_write_calculate_chunks_per_chunks_section(
     uint32_t *chunks_per_chunks_section,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t chunks_per_segment,
     uint8_t chunks_section_number,
     uint8_t unrestrict_offset_amount,
     liberror_error_t **error );

int libewf_write_test_segment_file_full(
     ssize64_t remaining_segment_file_size,
     uint32_t segment_amount_of_chunks,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     uint32_t chunks_per_segment,
     uint32_t current_amount_of_chunks,
     uint8_t format,
     uint8_t ewf_format,
     liberror_error_t **error );

int libewf_write_test_chunks_section_full(
     off64_t chunks_section_offset,
     ssize64_t remaining_segment_file_size,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     off64_t segment_file_offset,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t section_amount_of_chunks,
     uint32_t current_amount_of_chunks,
     uint32_t chunks_per_chunks_section,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_amount,
     liberror_error_t **error );

ssize_t libewf_write_process_chunk_data(
         libewf_chunk_cache_t *chunk_cache,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t compress_empty_block,
         uint8_t ewf_format,
         uint8_t *chunk_data,
         size_t chunk_data_size,
         uint8_t *compressed_chunk_data,
         size_t *compressed_chunk_data_size,
         int8_t *is_compressed,
         ewf_crc_t *chunk_crc,
         int8_t *write_crc,
         liberror_error_t **error );

ssize_t libewf_raw_write_chunk_new(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         ewf_crc_t chunk_crc,
         int8_t write_crc,
         liberror_error_t **error );

ssize_t libewf_raw_write_chunk_existing(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         ewf_crc_t chunk_crc,
         int8_t write_crc,
         liberror_error_t **error );

ssize_t libewf_write_chunk_data_new(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         int8_t force_write,
         liberror_error_t **error );

ssize_t libewf_write_chunk_data_existing(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint32_t chunk_offset,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         liberror_error_t **error );

#if defined( HAVE_V2_API )
LIBEWF_EXTERN ssize_t libewf_raw_write_prepare_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       void *compressed_buffer,
                       size_t *compressed_buffer_size,
                       int8_t *is_compressed,
                       uint32_t *chunk_crc,
                       int8_t *write_crc,
                       liberror_error_t **error );
#else
LIBEWF_EXTERN ssize_t libewf_raw_write_prepare_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       void *compressed_buffer,
                       size_t *compressed_buffer_size,
                       int8_t *is_compressed,
                       uint32_t *chunk_crc,
                       int8_t *write_crc );
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN ssize_t libewf_raw_write_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       size_t data_size,
                       int8_t is_compressed,
                       uint32_t chunk_crc,
                       int8_t write_crc,
                       liberror_error_t **error );
#else
LIBEWF_EXTERN ssize_t libewf_raw_write_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       size_t data_size,
                       int8_t is_compressed,
                       uint32_t chunk_crc,
                       int8_t write_crc );
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN ssize_t libewf_write_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t size,
                       liberror_error_t **error );
#else
LIBEWF_EXTERN ssize_t libewf_write_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t size );
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN ssize_t libewf_write_random(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t size,
                       off64_t offset,
                       liberror_error_t **error );
#else
LIBEWF_EXTERN ssize_t libewf_write_random(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t size,
                       off64_t offset );
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN ssize_t libewf_write_finalize(
                       libewf_handle_t *handle,
                       liberror_error_t **error );
#else
LIBEWF_EXTERN ssize_t libewf_write_finalize(
                       libewf_handle_t *handle );
#endif

#if defined( __cplusplus )
}
#endif

#endif

