/*
 * Interface functions
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

#if !defined( _LIBEWF_INTERFACE_H )
#define _LIBEWF_INTERFACE_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <libewf/handle.h>

#include "libewf_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBEWF_EXTERN uint8_t libewf_get_flags_read(
                       void );

LIBEWF_EXTERN uint8_t libewf_get_flags_read_write(
                       void );

LIBEWF_EXTERN uint8_t libewf_get_flags_write(
                       void );

LIBEWF_EXTERN int libewf_get_sectors_per_chunk(
                   libewf_handle_t *handle,
                   uint32_t *sectors_per_chunk );

LIBEWF_EXTERN int libewf_get_bytes_per_sector(
                   libewf_handle_t *handle,
                   uint32_t *bytes_per_sector );

LIBEWF_EXTERN int libewf_get_amount_of_sectors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_sectors );

LIBEWF_EXTERN int libewf_get_chunk_size(
                   libewf_handle_t *handle,
                   size32_t *chunk_size );

LIBEWF_EXTERN int libewf_get_error_granularity(
                   libewf_handle_t *handle,
                   uint32_t *error_granularity );

LIBEWF_EXTERN int libewf_get_compression_values(
                   libewf_handle_t *handle,
                   int8_t *compression_level,
                   uint8_t *compress_empty_block );

LIBEWF_EXTERN int libewf_get_media_size(
                   libewf_handle_t *handle,
                   size64_t *media_size );

LIBEWF_EXTERN int libewf_get_media_type(
                   libewf_handle_t *handle,
                   uint8_t *media_type );

LIBEWF_EXTERN int libewf_get_media_flags(
                   libewf_handle_t *handle,
                   uint8_t *media_flags );

LIBEWF_EXTERN int libewf_get_volume_type(
                   libewf_handle_t *handle,
                   uint8_t *volume_type );

LIBEWF_EXTERN int libewf_get_format(
                   libewf_handle_t *handle,
                   uint8_t *format );

LIBEWF_EXTERN int libewf_get_guid(
                   libewf_handle_t *handle,
                   uint8_t *guid,
                   size_t size );

LIBEWF_EXTERN int libewf_get_md5_hash(
                   libewf_handle_t *handle,
                   uint8_t *md5_hash,
                   size_t size );

LIBEWF_EXTERN int libewf_get_segment_filename(
                   libewf_handle_t *handle,
                   char *filename,
                   size_t length );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_get_segment_filename_wide(
                   libewf_handle_t *handle,
                   wchar_t *filename,
                   size_t length );
#endif

LIBEWF_EXTERN int libewf_get_delta_segment_filename(
                   libewf_handle_t *handle,
                   char *filename,
                   size_t length );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_get_delta_segment_filename_wide(
                   libewf_handle_t *handle,
                   wchar_t *filename,
                   size_t length );
#endif

LIBEWF_EXTERN int libewf_get_amount_of_acquiry_errors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_errors );

LIBEWF_EXTERN int libewf_get_acquiry_error(
                   libewf_handle_t *handle,
                   uint32_t index,
                   off64_t *first_sector,
                   uint32_t *amount_of_sectors  );

LIBEWF_EXTERN int libewf_get_amount_of_crc_errors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_errors );

LIBEWF_EXTERN int libewf_get_crc_error(
                   libewf_handle_t *handle,
                   uint32_t index,
                   off64_t *first_sector,
                   uint32_t *amount_of_sectors );

LIBEWF_EXTERN int libewf_get_amount_of_sessions(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_sessions );

LIBEWF_EXTERN int libewf_get_session(
                   libewf_handle_t *handle,
                   uint32_t index,
                   off64_t *first_sector,
                   uint32_t *amount_of_sectors );

LIBEWF_EXTERN int libewf_get_write_amount_of_chunks(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_chunks );

LIBEWF_EXTERN int libewf_get_header_codepage(
                   libewf_handle_t *handle,
                   int *header_codepage );

LIBEWF_EXTERN int libewf_get_amount_of_header_values(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_values );

LIBEWF_EXTERN int libewf_get_header_value_identifier(
                   libewf_handle_t *handle,
                   uint32_t index,
                   char *value,
                   size_t length );

LIBEWF_EXTERN int libewf_get_header_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   char *value,
                   size_t length );

LIBEWF_EXTERN int libewf_get_amount_of_hash_values(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_values );

LIBEWF_EXTERN int libewf_get_hash_value_identifier(
                   libewf_handle_t *handle,
                   uint32_t index,
                   char *value,
                   size_t length );

LIBEWF_EXTERN int libewf_get_hash_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   char *value,
                   size_t length );

LIBEWF_EXTERN int libewf_set_sectors_per_chunk(
                   libewf_handle_t *handle,
                   uint32_t sectors_per_chunk );

LIBEWF_EXTERN int libewf_set_bytes_per_sector(
                   libewf_handle_t *handle,
                   uint32_t bytes_per_sector );

LIBEWF_EXTERN int libewf_set_error_granularity(
                   libewf_handle_t *handle,
                   uint32_t error_granularity );

LIBEWF_EXTERN int libewf_set_compression_values(
                   libewf_handle_t *handle,
                   int8_t compression_level,
                   uint8_t compress_empty_block );

LIBEWF_EXTERN int libewf_set_media_size(
                   libewf_handle_t *handle,
                   size64_t media_size );

LIBEWF_EXTERN int libewf_set_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t segment_file_size );

LIBEWF_EXTERN int libewf_set_delta_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t delta_segment_file_size );

LIBEWF_EXTERN int libewf_set_media_type(
                   libewf_handle_t *handle,
                   uint8_t media_type );

LIBEWF_EXTERN int libewf_set_volume_type(
                   libewf_handle_t *handle,
                   uint8_t volume_type );

LIBEWF_EXTERN int libewf_set_format(
                   libewf_handle_t *handle,
                   uint8_t format );

LIBEWF_EXTERN int libewf_set_guid(
                   libewf_handle_t *handle,
                   uint8_t *guid,
                   size_t size );

LIBEWF_EXTERN int libewf_set_md5_hash(
                   libewf_handle_t *handle,
                   uint8_t *md5_hash,
                   size_t size );

LIBEWF_EXTERN int libewf_set_segment_filename(
                   libewf_handle_t *handle,
                   const char *filename,
                   size_t length );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_set_segment_filename_wide(
                   libewf_handle_t *handle,
                   const wchar_t *filename,
                   size_t length );
#endif

LIBEWF_EXTERN int libewf_set_delta_segment_filename(
                   libewf_handle_t *handle,
                   const char *filename,
                   size_t length );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEWF_EXTERN int libewf_set_delta_segment_filename_wide(
                   libewf_handle_t *handle,
                   const wchar_t *filename,
                   size_t length );
#endif

LIBEWF_EXTERN int libewf_set_read_wipe_chunk_on_error(
                   libewf_handle_t *handle,
                   uint8_t wipe_on_error );

LIBEWF_EXTERN int libewf_set_header_codepage(
                   libewf_handle_t *handle,
                   int header_codepage );

LIBEWF_EXTERN int libewf_set_header_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   const char *value,
                   size_t length );

LIBEWF_EXTERN int libewf_set_hash_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   const char *value,
                   size_t length );

LIBEWF_EXTERN int libewf_parse_header_values(
                   libewf_handle_t *handle,
                   uint8_t date_format );

LIBEWF_EXTERN int libewf_parse_hash_values(
                   libewf_handle_t *handle );

LIBEWF_EXTERN int libewf_add_acquiry_error(
                   libewf_handle_t *handle,
                   off64_t first_sector,
                   uint32_t amount_of_sectors );

LIBEWF_EXTERN int libewf_add_crc_error(
                   libewf_handle_t *handle,
                   off64_t first_sector,
                   uint32_t amount_of_sectors );

LIBEWF_EXTERN int libewf_add_session(
                   libewf_handle_t *handle,
                   off64_t first_sector,
                   uint32_t amount_of_sectors );

LIBEWF_EXTERN int libewf_copy_header_values(
                   libewf_handle_t *destination_handle,
                   libewf_handle_t *source_handle );

LIBEWF_EXTERN int libewf_copy_media_values(
                   libewf_handle_t *destination_handle,
                   libewf_handle_t *source_handle );

#if defined( __cplusplus )
}
#endif

#endif

