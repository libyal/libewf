/*
 * Handle functions
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

#if !defined( _LIBEWF_INTERNAL_HANDLE_H )
#define _LIBEWF_INTERNAL_HANDLE_H

#include <common.h>
#include <types.h>

#include "libewf_chunk_data.h"
#include "libewf_chunk_table.h"
#include "libewf_extern.h"
#include "libewf_hash_sections.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_libfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_io_handle.h"
#include "libewf_media_values.h"
#include "libewf_read_io_handle.h"
#include "libewf_segment_table.h"
#include "libewf_single_files.h"
#include "libewf_types.h"
#include "libewf_write_io_handle.h"

#if defined( _MSC_VER ) || defined( __BORLANDC__ ) || defined( __MINGW32_VERSION ) || defined( __MINGW64_VERSION_MAJOR )

/* This inclusion is needed otherwise some linkers
 * mess up exporting the legacy functions
 */
#include "libewf_legacy.h"
#endif

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_handle libewf_internal_handle_t;

struct libewf_internal_handle
{
	/* The IO handle
	 */
	libewf_io_handle_t *io_handle;

	/* The current chunk data
	 */
	libewf_chunk_data_t *chunk_data;

	/* The media values
	 */
	libewf_media_values_t *media_values;

	/* The stored sessions information
	 */
	libcdata_array_t *sessions;

	/* The stored tracks information
	 */
	libcdata_array_t *tracks;

	/* The sectors with acquiry read errors
	 */
	libcdata_range_list_t *acquiry_errors;

	/* The file IO pool
	 */
	libbfio_pool_t *file_io_pool;

	/* Value to indicate if the pool was created inside the library
	 */
	uint8_t file_io_pool_created_in_library;

	/* The read IO handle
	 */
	libewf_read_io_handle_t *read_io_handle;

	/* The write IO handle
	 */
	libewf_write_io_handle_t *write_io_handle;

	/* The maximum number of open handles in the pool
	 */
	int maximum_number_of_open_handles;

	/* The segment files list
	 */
	libmfdata_file_list_t *segment_files_list;

	/* The delta segment files list
	 */
	libmfdata_file_list_t *delta_segment_files_list;

	/* The segment files cache
	 */
	libfcache_cache_t *segment_files_cache;

	/* The segment file table
	 */
	libewf_segment_table_t *segment_table;

	/* The delta segment file table
	 */
	libewf_segment_table_t *delta_segment_table;

	/* The chunk table (data) list
	 */
	libmfdata_list_t *chunk_table_list;

	/* The chunk table cache
	 */
	libfcache_cache_t *chunk_table_cache;

	/* The date format for certain header values
	 */
	int date_format;

	/* The hash sections
	 */
	libewf_hash_sections_t *hash_sections;

	/* The header values
	 */
	libfvalue_table_t *header_values;

	/* Value to indicate the header values were parsed
	 */
	uint8_t header_values_parsed;

	/* The hash values
	 */
	libfvalue_table_t *hash_values;

	/* Value to indicate the hash values were parsed
	 */
	uint8_t hash_values_parsed;

	/* The single files
	 */
	libewf_single_files_t *single_files;
};

LIBEWF_EXTERN \
int libewf_handle_initialize(
     libewf_handle_t **handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_free(
     libewf_handle_t **handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_clone(
     libewf_handle_t **destination_handle,
     libewf_handle_t *source_handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_signal_abort(
     libewf_handle_t *handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_open(
     libewf_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN \
int libewf_handle_open_wide(
     libewf_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error );
#endif

LIBEWF_EXTERN \
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libcerror_error_t **error );

int libewf_handle_open_read_segment_files(
     libewf_internal_handle_t *internal_handle,
     libbfio_pool_t *file_io_pool,
     libewf_chunk_table_t *chunk_table,
     libcerror_error_t **error );

int libewf_handle_open_read_delta_segment_files(
     libewf_internal_handle_t *internal_handle,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_close(
     libewf_handle_t *handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_handle_prepare_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         void *uncompressed_chunk_buffer,
         size_t *uncompressed_chunk_buffer_size,
         int8_t is_compressed,
         uint32_t chunk_checksum,
         int8_t chunk_io_flags,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_handle_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         int8_t *is_compressed,
         void *checksum_buffer,
         uint32_t *chunk_checksum,
         int8_t *chunk_io_flags,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_handle_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_handle_read_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_handle_prepare_write_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         void *compressed_chunk_buffer,
         size_t *compressed_chunk_buffer_size,
         int8_t *is_compressed,
         uint32_t *chunk_checksum,
         int8_t *chunk_io_flags,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_handle_write_chunk(
         libewf_handle_t *handle,
         const void *chunk_buffer,
         size_t chunk_buffer_size,
         size_t data_size,
         int8_t is_compressed,
         void *checksum_buffer,
         uint32_t chunk_checksum,
         int8_t chunk_io_flags,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_handle_write_buffer(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_handle_write_random(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_handle_write_finalize(
         libewf_handle_t *handle,
         libcerror_error_t **error );

LIBEWF_EXTERN \
off64_t libewf_handle_seek_offset(
         libewf_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_offset(
     libewf_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_maximum_number_of_open_handles(
     libewf_handle_t *handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_segment_files_corrupted(
     libewf_handle_t *handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_segment_files_encrypted(
     libewf_handle_t *handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN \
int libewf_handle_get_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error );
#endif

LIBEWF_EXTERN \
int libewf_handle_get_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t *maximum_segment_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_delta_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_delta_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_delta_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN \
int libewf_handle_get_delta_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_delta_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_delta_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error );
#endif

LIBEWF_EXTERN \
int libewf_handle_get_maximum_delta_segment_size(
     libewf_handle_t *handle,
     size64_t *maximum_delta_segment_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_maximum_delta_segment_size(
     libewf_handle_t *handle,
     size64_t maximum_delta_segment_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN \
int libewf_handle_get_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error );
#endif

LIBEWF_EXTERN \
int libewf_handle_get_file_io_handle(
     libewf_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error );

int libewf_internal_handle_get_media_values(
     libewf_internal_handle_t *internal_handle,
     size64_t *media_size,
     libcerror_error_t **error );

int libewf_internal_handle_set_media_values(
     libewf_internal_handle_t *internal_handle,
     uint32_t sectors_per_chunk,
     uint32_t bytes_per_sector,
     size64_t media_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_root_file_entry(
     libewf_handle_t *handle,
     libewf_file_entry_t **root_file_entry,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_get_file_entry_by_utf8_path(
     libewf_handle_t *handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_get_file_entry_by_utf16_path(
     libewf_handle_t *handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

