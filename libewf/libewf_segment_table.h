/*
 * Segment table functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcstring.h"
#include "libewf_libmfdata.h"
#include "libewf_media_values.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_segment_table libewf_segment_table_t;

struct libewf_segment_table
{
	/* The basename
	 */
	libcstring_system_character_t *basename;

	/* The basename size
	 */
	size_t basename_size;

	/* The maximum segment size
	 */
	size64_t maximum_segment_size;

	/* The segment files (handles) array
	 */
	libcdata_array_t *segment_files_array;

	/* Flags
	 */
	uint8_t flags;
};

int libewf_segment_table_initialize(
     libewf_segment_table_t **segment_table,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

int libewf_segment_table_free(
     libewf_segment_table_t **segment_table,
     libcerror_error_t **error );

int libewf_segment_table_clone(
     libewf_segment_table_t **destination_segment_table,
     libewf_segment_table_t *source_segment_table,
     libcerror_error_t **error );

int libewf_segment_table_get_basename_size(
     libewf_segment_table_t *segment_table,
     size_t *basename_size,
     libcerror_error_t **error );

int libewf_segment_table_get_basename(
     libewf_segment_table_t *segment_table,
     char *basename,
     size_t basename_size,
     libcerror_error_t **error );

int libewf_segment_table_set_basename(
     libewf_segment_table_t *segment_table,
     const char *basename,
     size_t basename_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libewf_segment_table_get_basename_size_wide(
     libewf_segment_table_t *segment_table,
     size_t *basename_size,
     libcerror_error_t **error );

int libewf_segment_table_get_basename_wide(
     libewf_segment_table_t *segment_table,
     wchar_t *basename,
     size_t basename_size,
     libcerror_error_t **error );

int libewf_segment_table_set_basename_wide(
     libewf_segment_table_t *segment_table,
     const wchar_t *basename,
     size_t basename_length,
     libcerror_error_t **error );
#endif

int libewf_segment_table_set_maximum_segment_size(
     libewf_segment_table_t *segment_table,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

