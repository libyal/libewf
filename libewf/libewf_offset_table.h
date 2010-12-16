/*
 * Offset table functions
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBEWF_OFFSET_TABLE_H )
#define _LIBEWF_OFFSET_TABLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_array_type.h"
#include "libewf_chunk_value.h"
#include "libewf_libbfio.h"
#include "libewf_list_type.h"

#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_offset_table libewf_offset_table_t;

struct libewf_offset_table
{
	/* The last chunk value that was filled
	 */
	uint32_t last_chunk_value_filled;

	/* The last chunk value that was compared
	 */
	uint32_t last_chunk_value_compared;

	/* Dynamic array of chunk values
	 */
	libewf_array_t *chunk_values;
};

int libewf_offset_table_initialize(
     libewf_offset_table_t **offset_table,
     uint32_t number_of_chunk_values,
     liberror_error_t **error );

int libewf_offset_table_free(
     libewf_offset_table_t **offset_table,
     liberror_error_t **error );

int libewf_offset_table_clone(
     libewf_offset_table_t **destination_offset_table,
     libewf_offset_table_t *source_offset_table,
     liberror_error_t **error );

int libewf_offset_table_resize(
     libewf_offset_table_t *offset_table,
     uint32_t number_of_chunk_values,
     liberror_error_t **error );

int libewf_offset_table_get_number_of_chunk_values(
     libewf_offset_table_t *offset_table,
     uint32_t *number_of_chunk_values,
     liberror_error_t **error );

int libewf_offset_table_chunk_exists(
     libewf_offset_table_t *offset_table,
     uint32_t chunk,
     liberror_error_t **error );

int libewf_offset_table_get_chunk_value(
     libewf_offset_table_t *offset_table,
     uint32_t chunk,
     libewf_chunk_value_t **chunk_value,
     liberror_error_t **error );

int libewf_offset_table_set_chunk_value(
     libewf_offset_table_t *offset_table,
     uint32_t chunk,
     libewf_chunk_value_t *chunk_value,
     liberror_error_t **error );

int libewf_offset_table_fill(
     libewf_offset_table_t *offset_table,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t number_of_offsets,
     int segment_table_index,
     uint8_t tainted,
     liberror_error_t **error );

int libewf_offset_table_fill_last_offset(
     libewf_offset_table_t *offset_table,
     libewf_list_t *section_list,
     uint8_t tainted,
     liberror_error_t **error );

int libewf_offset_table_fill_offsets(
     libewf_offset_table_t *offset_table,
     uint32_t offset_table_index,
     uint32_t number_of_chunk_values,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t number_of_offsets,
     liberror_error_t **error );

int libewf_offset_table_compare(
     libewf_offset_table_t *offset_table,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t number_of_chunks,
     int segment_table_index,
     uint8_t tainted,
     liberror_error_t **error );

int libewf_offset_table_compare_last_offset(
     libewf_offset_table_t *offset_table,
     libewf_list_t *section_list,
     uint8_t tainted,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

