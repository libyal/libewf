/*
 * Chunk data functions
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

#if !defined( _LIBEWF_CHUNK_DATA_H )
#define _LIBEWF_CHUNK_DATA_H

#include <common.h>
#include <types.h>

#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_chunk_data libewf_chunk_data_t;

struct libewf_chunk_data
{
	/* The chunk index
	 */
	uint64_t chunk_index;

	/* The chunk size
	 */
	size32_t chunk_size;

	/* The allocated data size
	 */
	size_t allocated_data_size;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The compressed data
	 */
	uint8_t *compressed_data;

	/* The compressed data offset
	 */
	size_t compressed_data_offset;

	/* The compressed data size
	 */
	size_t compressed_data_size;

	/* The padding size
	 */
	size_t padding_size;

	/* The chunk data range flags
	 */
	uint32_t range_flags;

	/* The checksum
	 */
	uint32_t checksum;

	/* The flags
	 */
	uint8_t flags;

	/* The chunk IO flags
	 */
	int8_t chunk_io_flags;

	/* The range start offset
	 */
	off64_t range_start_offset;

	/* The range end offset
	 */
	off64_t range_end_offset;
};

int libewf_chunk_data_initialize(
     libewf_chunk_data_t **chunk_data,
     size32_t chunk_size,
     uint8_t clear_data,
     libcerror_error_t **error );

int libewf_chunk_data_free(
     libewf_chunk_data_t **chunk_data,
     libcerror_error_t **error );

ssize_t libewf_chunk_data_read_buffer(
         libewf_chunk_data_t *chunk_data,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

ssize_t libewf_chunk_data_write_buffer(
         libewf_chunk_data_t *chunk_data,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

int libewf_chunk_data_pack_determine_pack_flags(
     libewf_chunk_data_t *chunk_data,
     libewf_io_handle_t *io_handle,
     uint8_t *pack_flags,
     libcerror_error_t **error );

int libewf_chunk_data_pack_with_64_bit_pattern_fill(
     libewf_chunk_data_t *chunk_data,
     libcerror_error_t **error );

int libewf_chunk_data_pack_with_empty_block_compression(
     libewf_chunk_data_t *chunk_data,
     const uint8_t *compressed_zero_byte_empty_block,
     size_t compressed_zero_byte_empty_block_size,
     libcerror_error_t **error );

int libewf_chunk_data_pack_with_compression(
     libewf_chunk_data_t *chunk_data,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error );

int libewf_chunk_data_pack(
     libewf_chunk_data_t *chunk_data,
     libewf_io_handle_t *io_handle,
     const uint8_t *compressed_zero_byte_empty_block,
     size_t compressed_zero_byte_empty_block_size,
     uint8_t pack_flags,
     libcerror_error_t **error );

int libewf_chunk_data_unpack(
     libewf_chunk_data_t *chunk_data,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error );

int libewf_chunk_data_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_chunk_data_check_for_64_bit_pattern_fill(
     const uint8_t *data,
     size_t data_size,
     uint64_t *pattern,
     libcerror_error_t **error );

ssize_t libewf_chunk_data_write(
         libewf_chunk_data_t *chunk_data,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error );

int libewf_chunk_data_get_write_size(
     libewf_chunk_data_t *chunk_data,
     uint32_t *write_size,
     libcerror_error_t **error );

int libewf_chunk_data_get_checksum(
     libewf_chunk_data_t *chunk_data,
     uint16_t compression_method,
     uint32_t *checksum,
     libcerror_error_t **error );

ssize_t libewf_chunk_data_read_from_file_io_pool(
         libewf_chunk_data_t *chunk_data,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t chunk_data_offset,
         size64_t chunk_data_size,
         uint32_t chunk_data_flags,
         libcerror_error_t **error );

int libewf_chunk_data_read_element_data(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t chunk_data_offset,
     size64_t chunk_data_size,
     uint32_t chunk_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_CHUNK_DATA_H ) */

