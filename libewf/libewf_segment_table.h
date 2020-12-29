/*
 * Segment table functions
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

#if !defined( _LIBEWF_SEGMENT_TABLE_H )
#define _LIBEWF_SEGMENT_TABLE_H

#include <common.h>
#include <types.h>

#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_segment_file.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_segment_table libewf_segment_table_t;

struct libewf_segment_table
{
	/* The basename
	 */
	system_character_t *basename;

	/* The basename size
	 */
	size_t basename_size;

	/* The maximum segment size
	 */
	size64_t maximum_segment_size;

	/* The segment files list
	 */
	libfdata_list_t *segment_files_list;

	/* The number of segments
	 */
	uint32_t number_of_segments;

	/* The segment files cache
	 */
	libfcache_cache_t *segment_files_cache;

	/* The current segment file
	 */
	libewf_segment_file_t *current_segment_file;

	/* Flags
	 */
	uint8_t flags;
};

int libewf_segment_table_initialize(
     libewf_segment_table_t **segment_table,
     libewf_io_handle_t *io_handle,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

int libewf_segment_table_free(
     libewf_segment_table_t **segment_table,
     libcerror_error_t **error );

int libewf_segment_table_clear(
     libewf_segment_table_t *segment_table,
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

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

int libewf_segment_table_set_maximum_segment_size(
     libewf_segment_table_t *segment_table,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

int libewf_segment_table_get_number_of_segments(
     libewf_segment_table_t *segment_table,
     uint32_t *number_of_segments,
     libcerror_error_t **error );

int libewf_segment_table_get_segment_by_index(
     libewf_segment_table_t *segment_table,
     uint32_t segment_number,
     int *file_io_pool_entry,
     size64_t *segment_file_size,
     libcerror_error_t **error );

int libewf_segment_table_get_segment_at_offset(
     libewf_segment_table_t *segment_table,
     off64_t offset,
     int *file_io_pool_entry,
     size64_t *segment_file_size,
     libcerror_error_t **error );

int libewf_segment_table_get_segment_storage_media_size_by_index(
     libewf_segment_table_t *segment_table,
     uint32_t segment_number,
     size64_t *storage_media_size,
     libcerror_error_t **error );

int libewf_segment_table_set_segment_storage_media_size_by_index(
     libewf_segment_table_t *segment_table,
     uint32_t segment_number,
     size64_t storage_media_size,
     libcerror_error_t **error );

int libewf_segment_table_get_segment_file_by_index(
     libewf_segment_table_t *segment_table,
     uint32_t segment_number,
     libbfio_pool_t *file_io_pool,
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error );

int libewf_segment_table_get_segment_file_at_offset(
     libewf_segment_table_t *segment_table,
     off64_t offset,
     libbfio_pool_t *file_io_pool,
     uint32_t *segment_number,
     off64_t *segment_file_data_offset,
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error );

int libewf_segment_table_append_segment_by_segment_file(
     libewf_segment_table_t *segment_table,
     libewf_segment_file_t *segment_file,
     int file_io_pool_entry,
     size64_t segment_file_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SEGMENT_TABLE_H ) */

