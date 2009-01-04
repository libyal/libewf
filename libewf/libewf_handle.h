/*
 * libewf main handle
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

#if !defined( _LIBEWF_INTERNAL_HANDLE_H )
#define _LIBEWF_INTERNAL_HANDLE_H

#include <common.h>
#include <types.h>

#include <libewf/handle.h>

#include "libewf_chunk_cache.h"
#include "libewf_file_io_pool.h"
#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_segment_table.h"
#include "libewf_sector_table.h"
#include "libewf_values_table.h"

#include "ewf_data.h"
#include "ewf_digest_hash.h"
#include "ewf_error2.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_handle libewf_internal_handle_t;
typedef struct libewf_internal_handle_read libewf_internal_handle_read_t;
typedef struct libewf_internal_handle_write libewf_internal_handle_write_t;

struct libewf_internal_handle
{
	/* A specific subhandle for read specific values
	 */
	libewf_internal_handle_read_t *read;

	/* A specific subhandle for write specific values
	 */
	libewf_internal_handle_write_t *write;

	/* The file io pool
	 */
	libewf_file_io_pool_t *file_io_pool;

	/* The list of segment files
	 */
	libewf_segment_table_t *segment_table;

	/* The list of delta segment files
	 */
	libewf_segment_table_t *delta_segment_table;

	/* The list of offsets within the segment files within the table sections
	 */
	libewf_offset_table_t *offset_table;

	/* A simple cache is implemented here to avoid having to read and decompress the
	 * same chunk while reading the data.
	 */
	libewf_chunk_cache_t *chunk_cache;

	/* The media values
	 */
	libewf_media_values_t *media_values;

	/* The stored header sections
	 */
	libewf_header_sections_t *header_sections;

	/* The stored hash sections
	 */
	libewf_hash_sections_t *hash_sections;

	/* The header values
	 */
	libewf_values_table_t *header_values;

	/* The hash values
	 */
	libewf_values_table_t *hash_values;

	/* The stored sessions information
	 */
	libewf_sector_table_t *sessions;

	/* The sectors with acquiry read errors
	 */
	libewf_sector_table_t *acquiry_errors;

	/* The current chunk
	 */
	uint32_t current_chunk;

	/* The current chunk offset
	 */
	uint32_t current_chunk_offset;

	/* Value to indicate the compression level used
	 */
	int8_t compression_level;

	/* Value to indicate if empty block should be compressed
	 * even if no compression is used
	 */
	uint8_t compress_empty_block;

	/* Value to indicate which file format is used
	 */
	uint8_t format;

	/* Value to indicate which ewf format is used
	 */
	uint8_t ewf_format;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

/* Additional subhandle for read specific parameters
 */
struct libewf_internal_handle_read
{
	/* The sectors with CRC errors
	 */
	libewf_sector_table_t *crc_errors;

	/* A value to indicate if a chunk should be wiped on error
	 */
	uint8_t wipe_on_error;
};

/* Additional subhandle for write specific parameters
 */
struct libewf_internal_handle_write
{
	/* A cached version of the data section
	 */
	ewf_data_t *data_section;

	/* A cached version of the table offsets
	 */
	ewf_table_offset_t *table_offsets;

	/* The amount of allocated table offsets
	 */
	uint32_t amount_of_table_offsets;

	/* The amount of bytes of the input written
	 */
	ssize64_t input_write_count;

	/* The total amount of bytes written
	 */
	ssize64_t write_count;

	/* The maximum segment file size
	 */
	size64_t maximum_segment_file_size;

	/* The segment file size
	 */
	size64_t segment_file_size;

	/* The remaining segment file size
	 */
	ssize64_t remaining_segment_file_size;

	/* The delta segment file size
	 */
	size64_t delta_segment_file_size;

	/* The maximum amount of segments
	 */
	uint16_t maximum_amount_of_segments;

	/* The amount of bytes written to a section containing chunks
	 */
	ssize64_t chunks_section_write_count;

        /* The amount of chunks written
         */
        uint32_t amount_of_chunks;

        /* The determined (estimated) amount of chunks per segment
         */
        uint32_t chunks_per_segment;

        /* The determined (estimated) amount of chunks per chunks section
         */
        uint32_t chunks_per_chunks_section;

        /* The amount of chunks written of the current segment file
         */
        uint32_t segment_amount_of_chunks;

        /* The maximum amount of chunks that can be written to a chunks section
         */
        uint32_t maximum_section_amount_of_chunks;

        /* The amount of chunks written of the current chunks section
         */
        uint32_t section_amount_of_chunks;

	/* The offset of the chunks section within the current segment file
	 */
	off64_t chunks_section_offset;

	/* The current chunks section number
	 */
	uint8_t chunks_section_number;

	/* Value to indicate if the offset table should not be restricted
	 * to the maximum amount of offsets
	 */
	uint8_t unrestrict_offset_amount;

	/* Value to indicate the write values were initialized
	 */
	uint8_t values_initialized;

	/* Value to indicate a new chunks section should be created
	 */
	uint8_t create_chunks_section;

	/* Value to indicate if the write has been finalized
	 */
	uint8_t write_finalized;
};

int libewf_handle_initialize(
     libewf_handle_t **handle,
     uint8_t flags,
     libewf_error_t **error );

int libewf_handle_free(
     libewf_handle_t **handle,
     libewf_error_t **error );

int libewf_internal_handle_subhandle_read_initialize(
     libewf_internal_handle_read_t **subhandle_read,
     libewf_error_t **error );

int libewf_internal_handle_subhandle_read_free(
     libewf_internal_handle_read_t **subhandle_read,
     libewf_error_t **error );

int libewf_internal_handle_subhandle_write_initialize(
     libewf_internal_handle_write_t **subhandle_write,
     libewf_error_t **error );

int libewf_internal_handle_subhandle_write_free(
     libewf_internal_handle_write_t **subhandle_write,
     libewf_error_t **error );

int libewf_internal_handle_add_segment_file(
     libewf_internal_handle_t *internal_handle,
     int file_io_pool_entry,
     int flags,
     uint16_t *segment_number,
     libewf_error_t **error );

int libewf_internal_handle_get_write_maximum_amount_of_segments(
     uint8_t ewf_format,
     uint16_t *maximum_amount_of_segments,
     libewf_error_t **error );

int libewf_internal_handle_set_media_values(
     libewf_internal_handle_t *internal_handle,
     uint32_t sectors_per_chunk,
     uint32_t bytes_per_sector,
     size64_t media_size,
     libewf_error_t **error );

int libewf_internal_handle_set_format(
     libewf_internal_handle_t *internal_handle,
     uint8_t format,
     libewf_error_t **error );

int libewf_internal_handle_create_header_values(
     libewf_internal_handle_t *internal_handle,
     libewf_error_t **error );

int libewf_internal_handle_write_initialize(
     libewf_internal_handle_t *internal_handle,
     libewf_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

