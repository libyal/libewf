/*
 * Segment table functions
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

#if !defined( _LIBEWF_SEGMENT_TABLE_H )
#define _LIBEWF_SEGMENT_TABLE_H

#include <common.h>
#include <types.h>

#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_section_list.h"
#include "libewf_sector_table.h"
#include "libewf_segment_file_handle.h"
#include "libewf_system_string.h"

#include "ewf_data.h"

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
	libewf_system_character_t *basename;

	/* The basename size
	 */
	size_t basename_size;

	/* The maximum segment size
	 */
	size64_t maximum_segment_size;
};

int libewf_segment_table_initialize(
     libewf_segment_table_t **segment_table,
     uint16_t amount,
     size64_t maximum_segment_size,
     liberror_error_t **error );

int libewf_segment_table_free(
     libewf_segment_table_t **segment_table,
     liberror_error_t **error );

int libewf_segment_table_resize(
     libewf_segment_table_t *segment_table,
     uint16_t amount,
     liberror_error_t **error );

int libewf_segment_table_build(
     libewf_segment_table_t *segment_table,
     libewf_io_handle_t *io_handle,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     int *abort,
     liberror_error_t **error );

int libewf_segment_table_get_basename_size(
     libewf_segment_table_t *segment_table,
     size_t *basename_size,
     liberror_error_t **error );

int libewf_segment_table_get_basename(
     libewf_segment_table_t *segment_table,
     char *basename,
     size_t basename_size,
     liberror_error_t **error );

int libewf_segment_table_set_basename(
     libewf_segment_table_t *segment_table,
     const char *basename,
     size_t basename_size,
     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libewf_segment_table_get_basename_size_wide(
     libewf_segment_table_t *segment_table,
     size_t *basename_size,
     liberror_error_t **error );

int libewf_segment_table_get_basename_wide(
     libewf_segment_table_t *segment_table,
     wchar_t *basename,
     size_t basename_size,
     liberror_error_t **error );

int libewf_segment_table_set_basename_wide(
     libewf_segment_table_t *segment_table,
     const wchar_t *basename,
     size_t basename_size,
     liberror_error_t **error );
#endif

int libewf_segment_table_create_segment_file(
     libewf_segment_table_t *segment_table,
     uint16_t segment_number,
     libewf_io_handle_t *io_handle,
     int16_t maximum_amount_of_segments,
     uint8_t segment_file_type,
     liberror_error_t **error );

int libewf_segment_table_write_sections_corrections(
     libewf_segment_table_t *segment_table,
     libewf_io_handle_t *io_handle,
     uint32_t last_segment_amount_of_chunks,
     libewf_media_values_t *media_values,
     libewf_values_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     ewf_data_t **cached_data_section,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

