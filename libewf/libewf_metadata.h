/*
 * Metadata functions
 *
 * Copyright (C) 2006-2015, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_METADATA_H )
#define _LIBEWF_METADATA_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_libcerror.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBEWF_EXTERN \
int libewf_handle_get_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t *sectors_per_chunk,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t sectors_per_chunk,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t bytes_per_sector,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_number_of_sectors(
     libewf_handle_t *handle,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_chunk_size(
     libewf_handle_t *handle,
     size32_t *chunk_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_error_granularity(
     libewf_handle_t *handle,
     uint32_t *error_granularity,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_error_granularity(
     libewf_handle_t *handle,
     uint32_t error_granularity,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_compression_method(
     libewf_handle_t *handle,
     uint16_t *compression_method,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_compression_method(
     libewf_handle_t *handle,
     uint16_t compression_method,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_compression_values(
     libewf_handle_t *handle,
     int8_t *compression_level,
     uint8_t *compression_flags,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_compression_values(
     libewf_handle_t *handle,
     int8_t compression_level,
     uint8_t compression_flags,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_media_size(
     libewf_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_media_size(
     libewf_handle_t *handle,
     size64_t media_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_media_type(
     libewf_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_media_type(
     libewf_handle_t *handle,
     uint8_t media_type,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_media_flags(
     libewf_handle_t *handle,
     uint8_t *media_flags,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_media_flags(
     libewf_handle_t *handle,
     uint8_t media_flags,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_format(
     libewf_handle_t *handle,
     uint8_t *format,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_format(
     libewf_handle_t *handle,
     uint8_t format,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_segment_file_version(
     libewf_handle_t *handle,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_segment_file_set_identifier(
     libewf_handle_t *handle,
     uint8_t *set_identifier,
     size_t size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_segment_file_set_identifier(
     libewf_handle_t *handle,
     const uint8_t *set_identifier,
     size_t size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_md5_hash(
     libewf_handle_t *handle,
     uint8_t *md5_hash,
     size_t size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_md5_hash(
     libewf_handle_t *handle,
     const uint8_t *md5_hash,
     size_t size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_sha1_hash(
     libewf_handle_t *handle,
     uint8_t *sha1_hash,
     size_t size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_sha1_hash(
     libewf_handle_t *handle,
     const uint8_t *sha1_hash,
     size_t size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_read_zero_chunk_on_error(
     libewf_handle_t *handle,
     uint8_t zero_on_error,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_copy_media_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_number_of_acquiry_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_acquiry_error(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_append_acquiry_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_number_of_checksum_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_checksum_error(
     libewf_handle_t *handle,
     uint32_t error_index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_append_checksum_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_number_of_sessions(
     libewf_handle_t *handle,
     uint32_t *number_of_sessions,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_session(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_append_session(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_number_of_tracks(
     libewf_handle_t *handle,
     uint32_t *number_of_tracks,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_track(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_append_track(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_header_codepage(
     libewf_handle_t *handle,
     int *header_codepage,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_header_codepage(
     libewf_handle_t *handle,
     int header_codepage,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_header_values_date_format(
     libewf_handle_t *handle,
     int *date_format,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_header_values_date_format(
     libewf_handle_t *handle,
     int date_format,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_number_of_header_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_header_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_header_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_utf8_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_utf16_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_copy_header_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_number_of_hash_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_hash_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_hash_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_utf8_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_utf16_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_get_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_handle_set_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

int libewf_handle_parse_hash_values(
     libewf_internal_handle_t *internal_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

