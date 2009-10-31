/*
 * Legacy functions
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

#if !defined( _LIBEWF_LEGACY_H )
#define _LIBEWF_LEGACY_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "libewf_extern.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_V1_API )

#if !defined( HAVE_LOCAL_LIBEWF )

LIBEWF_EXTERN void libewf_set_notify_values(
                    FILE *stream,
                    int verbose );

#endif

LIBEWF_EXTERN int libewf_signal_abort(
                   libewf_handle_t *handle );

/* Handle/File functions
 */
LIBEWF_EXTERN libewf_handle_t *libewf_open(
                                char * const filenames[],
                                int amount_of_files,
                                uint8_t flags );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN libewf_handle_t *libewf_open_wide(
                                wchar_t * const filenames[],
                                int amount_of_files,
                                uint8_t flags );
#endif

LIBEWF_EXTERN int libewf_close(
                   libewf_handle_t *handle );

LIBEWF_EXTERN off64_t libewf_seek_offset(
                       libewf_handle_t *handle,
                       off64_t offset );

LIBEWF_EXTERN off64_t libewf_get_offset(
                       libewf_handle_t *handle );

LIBEWF_EXTERN ssize_t libewf_raw_read_prepare_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       void *uncompressed_buffer,
                       size_t *uncompressed_buffer_size,
                       int8_t is_compressed,
                       uint32_t chunk_crc,
                       int8_t read_crc );

LIBEWF_EXTERN ssize_t libewf_raw_read_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       int8_t *is_compressed,
                       uint32_t *chunk_crc,
                       int8_t *read_crc );

LIBEWF_EXTERN ssize_t libewf_read_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size );

LIBEWF_EXTERN ssize_t libewf_read_random(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       off64_t offset );

LIBEWF_EXTERN ssize_t libewf_raw_write_prepare_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       void *compressed_buffer,
                       size_t *compressed_buffer_size,
                       int8_t *is_compressed,
                       uint32_t *chunk_crc,
                       int8_t *write_crc );

LIBEWF_EXTERN ssize_t libewf_raw_write_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       size_t data_size,
                       int8_t is_compressed,
                       uint32_t chunk_crc,
                       int8_t write_crc );

LIBEWF_EXTERN ssize_t libewf_write_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size );

LIBEWF_EXTERN ssize_t libewf_write_random(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       off64_t offset );

LIBEWF_EXTERN ssize_t libewf_write_finalize(
                       libewf_handle_t *handle );

LIBEWF_EXTERN int libewf_get_segment_filename(
                   libewf_handle_t *handle,
                   char *filename,
                   size_t filename_size );

LIBEWF_EXTERN int libewf_set_segment_filename(
                   libewf_handle_t *handle,
                   const char *filename,
                   size_t filename_length );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_get_segment_filename_wide(
                   libewf_handle_t *handle,
                   wchar_t *filename,
                   size_t filename_size );

LIBEWF_EXTERN int libewf_set_segment_filename_wide(
                   libewf_handle_t *handle,
                   const wchar_t *filename,
                   size_t filename_length );
#endif

LIBEWF_EXTERN int libewf_get_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t *segment_file_size );

LIBEWF_EXTERN int libewf_set_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t segment_file_size );

LIBEWF_EXTERN int libewf_get_delta_segment_filename(
                   libewf_handle_t *handle,
                   char *filename,
                   size_t filename_size );

LIBEWF_EXTERN int libewf_set_delta_segment_filename(
                   libewf_handle_t *handle,
                   const char *filename,
                   size_t filename_length );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_get_delta_segment_filename_wide(
                   libewf_handle_t *handle,
                   wchar_t *filename,
                   size_t filename_size );

LIBEWF_EXTERN int libewf_set_delta_segment_filename_wide(
                   libewf_handle_t *handle,
                   const wchar_t *filename,
                   size_t filename_length );
#endif

LIBEWF_EXTERN int libewf_get_delta_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t *delta_segment_file_size );

LIBEWF_EXTERN int libewf_set_delta_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t delta_segment_file_size );

/* Meta data functions
 */
LIBEWF_EXTERN int libewf_get_sectors_per_chunk(
                   libewf_handle_t *handle,
                   uint32_t *sectors_per_chunk );

LIBEWF_EXTERN int libewf_set_sectors_per_chunk(
                   libewf_handle_t *handle,
                   uint32_t sectors_per_chunk );

LIBEWF_EXTERN int libewf_get_bytes_per_sector(
                   libewf_handle_t *handle,
                   uint32_t *bytes_per_sector );

LIBEWF_EXTERN int libewf_set_bytes_per_sector(
                   libewf_handle_t *handle,
                   uint32_t bytes_per_sector );

LIBEWF_EXTERN int libewf_get_amount_of_sectors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_sectors );

LIBEWF_EXTERN int libewf_get_chunk_size(
                   libewf_handle_t *handle,
                   size32_t *chunk_size );

LIBEWF_EXTERN int libewf_get_error_granularity(
                   libewf_handle_t *handle,
                   uint32_t *error_granularity );

LIBEWF_EXTERN int libewf_set_error_granularity(
                   libewf_handle_t *handle,
                   uint32_t error_granularity );

LIBEWF_EXTERN int libewf_get_compression_values(
                   libewf_handle_t *handle,
                   int8_t *compression_level,
                   uint8_t *compress_empty_block );

LIBEWF_EXTERN int libewf_set_compression_values(
                   libewf_handle_t *handle,
                   int8_t compression_level,
                   uint8_t compress_empty_block );

LIBEWF_EXTERN int libewf_get_media_size(
                   libewf_handle_t *handle,
                   size64_t *media_size );

LIBEWF_EXTERN int libewf_set_media_size(
                   libewf_handle_t *handle,
                   size64_t media_size );

LIBEWF_EXTERN int libewf_get_media_type(
                   libewf_handle_t *handle,
                   uint8_t *media_type );

LIBEWF_EXTERN int libewf_set_media_type(
                   libewf_handle_t *handle,
                   uint8_t media_type );

LIBEWF_EXTERN int libewf_get_media_flags(
                   libewf_handle_t *handle,
                   uint8_t *media_flags );

LIBEWF_EXTERN int libewf_set_media_flags(
                   libewf_handle_t *handle,
                   uint8_t media_flags );

LIBEWF_EXTERN int libewf_get_volume_type(
                   libewf_handle_t *handle,
                   uint8_t *volume_type );

LIBEWF_EXTERN int libewf_set_volume_type(
                   libewf_handle_t *handle,
                   uint8_t volume_type );

LIBEWF_EXTERN int libewf_get_format(
                   libewf_handle_t *handle,
                   uint8_t *format );

LIBEWF_EXTERN int libewf_set_format(
                   libewf_handle_t *handle,
                   uint8_t format );

LIBEWF_EXTERN int libewf_get_guid(
                   libewf_handle_t *handle,
                   uint8_t *guid,
                   size_t size );

LIBEWF_EXTERN int libewf_set_guid(
                   libewf_handle_t *handle,
                   uint8_t *guid,
                   size_t size );

LIBEWF_EXTERN int libewf_get_md5_hash(
                   libewf_handle_t *handle,
                   uint8_t *md5_hash,
                   size_t size );

LIBEWF_EXTERN int libewf_set_md5_hash(
                   libewf_handle_t *handle,
                   uint8_t *md5_hash,
                   size_t size );

LIBEWF_EXTERN int libewf_get_sha1_hash(
                   libewf_handle_t *handle,
                   uint8_t *sha1_hash,
                   size_t size );

LIBEWF_EXTERN int libewf_set_sha1_hash(
                   libewf_handle_t *handle,
                   uint8_t *sha1_hash,
                   size_t size );

LIBEWF_EXTERN int libewf_get_write_amount_of_chunks(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_chunks );

LIBEWF_EXTERN int libewf_set_read_wipe_chunk_on_error(
                   libewf_handle_t *handle,
                   uint8_t wipe_on_error );

LIBEWF_EXTERN int libewf_copy_media_values(
                   libewf_handle_t *destination_handle,
                   libewf_handle_t *source_handle );

LIBEWF_EXTERN int libewf_get_amount_of_acquiry_errors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_errors );

LIBEWF_EXTERN int libewf_get_acquiry_error(
                   libewf_handle_t *handle,
                   uint32_t index,
                   off64_t *first_sector,
                   uint32_t *amount_of_sectors );

LIBEWF_EXTERN int libewf_add_acquiry_error(
                   libewf_handle_t *handle,
                   off64_t first_sector,
                   uint32_t amount_of_sectors );

LIBEWF_EXTERN int libewf_get_amount_of_crc_errors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_errors );

LIBEWF_EXTERN int libewf_get_crc_error(
                   libewf_handle_t *handle,
                   uint32_t index,
                   off64_t *first_sector,
                   uint32_t *amount_of_sectors );

LIBEWF_EXTERN int libewf_add_crc_error(
                   libewf_handle_t *handle,
                   off64_t first_sector,
                   uint32_t amount_of_sectors );

LIBEWF_EXTERN int libewf_get_amount_of_sessions(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_sessions );

LIBEWF_EXTERN int libewf_get_session(
                   libewf_handle_t *handle,
                   uint32_t index,
                   off64_t *first_sector,
                   uint32_t *amount_of_sectors );

LIBEWF_EXTERN int libewf_add_session(
                   libewf_handle_t *handle,
                   off64_t first_sector,
                   uint32_t amount_of_sectors );

LIBEWF_EXTERN int libewf_get_header_codepage(
                   libewf_handle_t *handle,
                   int *header_codepage );

LIBEWF_EXTERN int libewf_set_header_codepage(
                   libewf_handle_t *handle,
                   int header_codepage );

LIBEWF_EXTERN int libewf_get_amount_of_header_values(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_values );

LIBEWF_EXTERN int libewf_get_header_value_identifier_size(
                   libewf_handle_t *handle,
                   uint32_t index,
                   size_t *identifier_size );

LIBEWF_EXTERN int libewf_get_header_value_identifier(
                   libewf_handle_t *handle,
                   uint32_t index,
                   char *identifier,
                   size_t identifier_size );

LIBEWF_EXTERN int libewf_get_header_value_size(
                   libewf_handle_t *handle,
                   const char *identifier,
                   size_t *value_size );

LIBEWF_EXTERN int libewf_get_header_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   char *value,
                   size_t value_size );

LIBEWF_EXTERN int libewf_set_header_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   const char *value,
                   size_t value_length );

LIBEWF_EXTERN int libewf_parse_header_values(
                   libewf_handle_t *handle,
                   uint8_t date_format );

LIBEWF_EXTERN int libewf_copy_header_values(
                   libewf_handle_t *destination_handle,
                   libewf_handle_t *source_handle );

LIBEWF_EXTERN int libewf_get_amount_of_hash_values(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_values );

LIBEWF_EXTERN int libewf_get_hash_value_identifier_size(
                   libewf_handle_t *handle,
                   uint32_t index,
                   size_t *identifier_size );

LIBEWF_EXTERN int libewf_get_hash_value_identifier(
                   libewf_handle_t *handle,
                   uint32_t index,
                   char *identifier,
                   size_t identifier_size );

LIBEWF_EXTERN int libewf_get_hash_value_size(
                   libewf_handle_t *handle,
                   const char *identifier,
                   size_t *value_size );

LIBEWF_EXTERN int libewf_get_hash_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   char *value,
                   size_t value_size );

LIBEWF_EXTERN int libewf_set_hash_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   const char *value,
                   size_t value_length );

LIBEWF_EXTERN int libewf_parse_hash_values(
                   libewf_handle_t *handle );

#endif

#if defined( __cplusplus )
}
#endif

#endif

