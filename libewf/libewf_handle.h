/*
 * libewf main handle
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

#if !defined( _LIBEWF_INTERNAL_HANDLE_H )
#define _LIBEWF_INTERNAL_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_chunk_cache.h"
#include "libewf_extern.h"
#include "libewf_hash_sections.h"
#include "libewf_libbfio.h"
#include "libewf_header_sections.h"
#include "libewf_io_handle.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_read_io_handle.h"
#include "libewf_segment_table.h"
#include "libewf_sector_table.h"
#include "libewf_types.h"
#include "libewf_values_table.h"
#include "libewf_write_io_handle.h"

#if defined( _MSC_VER ) || defined( __BORLANDC__ )

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
	/* The io handle
	 */
	libewf_io_handle_t *io_handle;

	/* The read io handle
	 */
	libewf_read_io_handle_t *read_io_handle;

	/* The write io handle
	 */
	libewf_write_io_handle_t *write_io_handle;

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

	/* The date format for certain header values
	 */
	int date_format;

	/* The header values
	 */
	libewf_values_table_t *header_values;

	/* Value to indicate the header values were parsed
	 */
	uint8_t header_values_parsed;

	/* The hash values
	 */
	libewf_values_table_t *hash_values;

	/* Value to indicate the hash values were parsed
	 */
	uint8_t hash_values_parsed;

	/* The stored sessions information
	 */
	libewf_sector_table_t *sessions;

	/* The sectors with acquiry read errors
	 */
	libewf_sector_table_t *acquiry_errors;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

LIBEWF_EXTERN int libewf_handle_initialize(
                   libewf_handle_t **handle,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_free(
                   libewf_handle_t **handle,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_signal_abort(
                   libewf_handle_t *handle,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_open(
                   libewf_handle_t *handle,
                   char * const filenames[],
                   int amount_of_filenames,
                   uint8_t flags,
                   liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_handle_open_wide(
                   libewf_handle_t *handle,
                   wchar_t * const filenames[],
                   int amount_of_filenames,
                   uint8_t flags,
                   liberror_error_t **error );
#endif

LIBEWF_EXTERN int libewf_handle_open_file_io_pool(
                   libewf_handle_t *handle,
                   libbfio_pool_t *file_io_pool,
                   uint8_t flags,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_close(
                   libewf_handle_t *handle,
                   liberror_error_t **error );

LIBEWF_EXTERN off64_t libewf_handle_seek_offset(
                       libewf_handle_t *handle,
                       off64_t offset,
                       int whence,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_handle_prepare_read_chunk(
                       libewf_handle_t *handle,
                       void *chunk_buffer,
                       size_t chunk_buffer_size,
                       void *uncompressed_buffer,
                       size_t *uncompressed_buffer_size,
                       int8_t is_compressed,
                       uint32_t chunk_crc,
                       int8_t read_crc,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_handle_read_chunk(
                       libewf_handle_t *handle,
                       void *chunk_buffer,
                       size_t chunk_buffer_size,
                       int8_t *is_compressed,
                       void *crc_buffer,
                       uint32_t *chunk_crc,
                       int8_t *read_crc,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_handle_read_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t size,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_handle_read_random(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t size,
                       off64_t offset,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_handle_prepare_write_chunk(
                       libewf_handle_t *handle,
                       void *chunk_buffer,
                       size_t chunk_buffer_size,
                       void *compressed_buffer,
                       size_t *compressed_buffer_size,
                       int8_t *is_compressed,
                       uint32_t *chunk_crc,
                       int8_t *write_crc,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_handle_write_chunk(
                       libewf_handle_t *handle,
                       void *chunk_buffer,
                       size_t chunk_buffer_size,
                       size_t data_size,
                       int8_t is_compressed,
                       void *crc_buffer,
                       uint32_t chunk_crc,
                       int8_t write_crc,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_handle_write_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_handle_write_random(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       off64_t offset,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_handle_write_finalize(
                       libewf_handle_t *handle,
                       liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_offset(
                   libewf_handle_t *handle,
                   off64_t *offset,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_segment_filename_size(
                   libewf_handle_t *handle,
                   size_t *filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_segment_filename(
                   libewf_handle_t *handle,
                   char *filename,
                   size_t filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_segment_filename(
                   libewf_handle_t *handle,
                   const char *filename,
                   size_t filename_length,
                   liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_handle_get_segment_filename_size_wide(
                   libewf_handle_t *handle,
                   size_t *filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_segment_filename_wide(
                   libewf_handle_t *handle,
                   wchar_t *filename,
                   size_t filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_segment_filename_wide(
                   libewf_handle_t *handle,
                   const wchar_t *filename,
                   size_t filename_length,
                   liberror_error_t **error );
#endif

LIBEWF_EXTERN int libewf_handle_get_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t *segment_file_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t segment_file_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_delta_segment_filename_size(
                   libewf_handle_t *handle,
                   size_t *filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_delta_segment_filename(
                   libewf_handle_t *handle,
                   char *filename,
                   size_t filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_delta_segment_filename(
                   libewf_handle_t *handle,
                   const char *filename,
                   size_t filename_length,
                   liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_handle_get_delta_segment_filename_size_wide(
                   libewf_handle_t *handle,
                   size_t *filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_delta_segment_filename_wide(
                   libewf_handle_t *handle,
                   wchar_t *filename,
                   size_t filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_delta_segment_filename_wide(
                   libewf_handle_t *handle,
                   const wchar_t *filename,
                   size_t filename_length,
                   liberror_error_t **error );
#endif

LIBEWF_EXTERN int libewf_handle_get_delta_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t *delta_segment_file_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_delta_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t delta_segment_file_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_filename_size(
                   libewf_handle_t *handle,
                   size_t *filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_filename(
                   libewf_handle_t *handle,
                   char *filename,
                   size_t filename_size,
                   liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_handle_get_filename_size_wide(
                   libewf_handle_t *handle,
                   size_t *filename_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_filename_wide(
                   libewf_handle_t *handle,
                   wchar_t *filename,
                   size_t filename_size,
                   liberror_error_t **error );
#endif

LIBEWF_EXTERN int libewf_handle_get_file_io_handle(
                   libewf_handle_t *handle,
                   libbfio_handle_t **file_io_handle,
                   liberror_error_t **error );

int libewf_internal_handle_add_segment_file(
     libewf_internal_handle_t *internal_handle,
     int file_io_pool_entry,
     int flags,
     uint16_t *segment_number,
     uint8_t *segment_file_type,
     liberror_error_t **error );

int libewf_internal_handle_get_write_maximum_amount_of_segments(
     uint8_t ewf_format,
     uint16_t *maximum_amount_of_segments,
     liberror_error_t **error );

int libewf_internal_handle_set_media_values(
     libewf_internal_handle_t *internal_handle,
     uint32_t sectors_per_chunk,
     uint32_t bytes_per_sector,
     size64_t media_size,
     liberror_error_t **error );

int libewf_internal_handle_set_format(
     libewf_internal_handle_t *internal_handle,
     uint8_t format,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

