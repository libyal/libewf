/*
 * Segment table functions
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

#if !defined( _LIBEWF_SEGMENT_TABLE_H )
#define _LIBEWF_SEGMENT_TABLE_H

#include <common.h>
#include <system_string.h>
#include <types.h>

#include "libewf_file_io_pool.h"
#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_section_list.h"
#include "libewf_sector_table.h"
#include "libewf_segment_file_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_segment_table libewf_segment_table_t;

struct libewf_segment_table
{
	/* The amount of segments in the table
	 */
	uint16_t amount;

	/* A dynamic array containting references to segment file handles
	 */
	libewf_segment_file_handle_t **segment_file_handle;

	/* The basename
	 */
	system_character_t *basename;

	/* The basename length
	 */
	size_t basename_length;
};

int libewf_segment_table_initialize(
     libewf_segment_table_t **segment_table,
     uint16_t amount );

int libewf_segment_table_free(
     libewf_segment_table_t **segment_table );

int libewf_segment_table_resize(
     libewf_segment_table_t *segment_table,
     uint16_t amount );

int libewf_segment_table_build(
     libewf_segment_table_t *segment_table,
     libewf_file_io_pool_t *file_io_pool,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     int8_t *compression_level,
     uint8_t *format,
     uint8_t *ewf_format,
     size64_t *segment_file_size,
     uint8_t error_tollerance,
     int *abort );

int libewf_segment_table_get_basename(
     libewf_segment_table_t *segment_table,
     system_character_t *basename,
     size_t length );

int libewf_segment_table_set_basename(
     libewf_segment_table_t *segment_table,
     system_character_t *basename,
     size_t basename_length );

int libewf_segment_table_read_open(
     libewf_segment_table_t *segment_table,
     libewf_segment_table_t *delta_segment_table,
     libewf_file_io_pool_t *file_io_pool,
     system_character_t * const filenames[],
     uint16_t file_amount,
     uint8_t flags,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     int8_t *compression_level,
     uint8_t *format,
     uint8_t *ewf_format,
     size64_t *segment_file_size,
     uint8_t error_tollerance,
     int *abort );

int libewf_segment_table_write_open(
     libewf_segment_table_t *segment_table,
     system_character_t * const filenames[],
     uint16_t file_amount );

int libewf_segment_table_create_segment_file(
     libewf_segment_table_t *segment_table,
     uint16_t segment_number,
     libewf_file_io_pool_t *file_io_pool,
     int16_t maximum_amount_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     uint8_t ewf_format );

#if defined( __cplusplus )
}
#endif

#endif

