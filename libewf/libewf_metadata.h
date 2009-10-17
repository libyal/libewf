/*
 * Meta data functions
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

#if !defined( _LIBEWF_METADATA_H )
#define _LIBEWF_METADATA_H

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

LIBEWF_EXTERN int libewf_handle_get_sectors_per_chunk(
                   libewf_handle_t *handle,
                   uint32_t *sectors_per_chunk,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_sectors_per_chunk(
                   libewf_handle_t *handle,
                   uint32_t sectors_per_chunk,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_bytes_per_sector(
                   libewf_handle_t *handle,
                   uint32_t *bytes_per_sector,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_bytes_per_sector(
                   libewf_handle_t *handle,
                   uint32_t bytes_per_sector,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_amount_of_sectors(
                   libewf_handle_t *handle,
                   uint64_t *amount_of_sectors,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_chunk_size(
                   libewf_handle_t *handle,
                   size32_t *chunk_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_error_granularity(
                   libewf_handle_t *handle,
                   uint32_t *error_granularity,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_error_granularity(
                   libewf_handle_t *handle,
                   uint32_t error_granularity,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_compression_values(
                   libewf_handle_t *handle,
                   int8_t *compression_level,
                   uint8_t *compression_flags,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_compression_values(
                   libewf_handle_t *handle,
                   int8_t compression_level,
                   uint8_t compression_flags,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_media_size(
                   libewf_handle_t *handle,
                   size64_t *media_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_media_size(
                   libewf_handle_t *handle,
                   size64_t media_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_media_type(
                   libewf_handle_t *handle,
                   uint8_t *media_type,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_media_type(
                   libewf_handle_t *handle,
                   uint8_t media_type,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_media_flags(
                   libewf_handle_t *handle,
                   uint8_t *media_flags,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_media_flags(
                   libewf_handle_t *handle,
                   uint8_t media_flags,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_format(
                   libewf_handle_t *handle,
                   uint8_t *format,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_format(
                   libewf_handle_t *handle,
                   uint8_t format,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_guid(
                   libewf_handle_t *handle,
                   uint8_t *guid,
                   size_t size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_guid(
                   libewf_handle_t *handle,
                   uint8_t *guid,
                   size_t size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_md5_hash(
                   libewf_handle_t *handle,
                   uint8_t *md5_hash,
                   size_t size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_md5_hash(
                   libewf_handle_t *handle,
                   uint8_t *md5_hash,
                   size_t size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_sha1_hash(
                   libewf_handle_t *handle,
                   uint8_t *sha1_hash,
                   size_t size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_sha1_hash(
                   libewf_handle_t *handle,
                   uint8_t *sha1_hash,
                   size_t size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_write_amount_of_chunks(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_chunks,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_read_wipe_chunk_on_error(
                   libewf_handle_t *handle,
                   uint8_t wipe_on_error,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_copy_media_values(
                   libewf_handle_t *destination_handle,
                   libewf_handle_t *source_handle,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_amount_of_acquiry_errors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_errors,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_acquiry_error(
                   libewf_handle_t *handle,
                   uint32_t index,
                   uint64_t *first_sector,
                   uint64_t *amount_of_sectors,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_add_acquiry_error(
                   libewf_handle_t *handle,
                   uint64_t first_sector,
                   uint64_t amount_of_sectors,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_amount_of_crc_errors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_errors,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_crc_error(
                   libewf_handle_t *handle,
                   uint32_t index,
                   uint64_t *first_sector,
                   uint64_t *amount_of_sectors,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_add_crc_error(
                   libewf_handle_t *handle,
                   uint64_t first_sector,
                   uint64_t amount_of_sectors,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_amount_of_sessions(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_sessions,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_session(
                   libewf_handle_t *handle,
                   uint32_t index,
                   uint64_t *first_sector,
                   uint64_t *amount_of_sectors,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_add_session(
                   libewf_handle_t *handle,
                   uint64_t first_sector,
                   uint64_t amount_of_sectors,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_header_codepage(
                   libewf_handle_t *handle,
                   int *header_codepage,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_header_codepage(
                   libewf_handle_t *handle,
                   int header_codepage,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_header_values_date_format(
                   libewf_handle_t *handle,
                   int *date_format,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_header_values_date_format(
                   libewf_handle_t *handle,
                   int date_format,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_amount_of_header_values(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_values,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_header_value_identifier_size(
                   libewf_handle_t *handle,
                   uint32_t index,
                   size_t *identifier_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_header_value_identifier(
                   libewf_handle_t *handle,
                   uint32_t index,
                   uint8_t *identifier,
                   size_t identifier_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_header_value_size(
                   libewf_handle_t *handle,
                   const uint8_t *identifier,
                   size_t identifier_length,
                   size_t *value_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_header_value(
                   libewf_handle_t *handle,
                   const uint8_t *identifier,
                   size_t identifier_length,
                   uint8_t *value,
                   size_t value_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_header_value(
                   libewf_handle_t *handle,
                   const uint8_t *identifier,
                   size_t identifier_length,
                   const uint8_t *value,
                   size_t value_length,
                   liberror_error_t **error );

int libewf_handle_parse_header_values(
     libewf_internal_handle_t *internal_handle,
     liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_copy_header_values(
                   libewf_handle_t *destination_handle,
                   libewf_handle_t *source_handle,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_amount_of_hash_values(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_values,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_hash_value_identifier_size(
                   libewf_handle_t *handle,
                   uint32_t index,
                   size_t *identifier_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_hash_value_identifier(
                   libewf_handle_t *handle,
                   uint32_t index,
                   uint8_t *identifier,
                   size_t identifier_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_hash_value_size(
                   libewf_handle_t *handle,
                   const uint8_t *identifier,
                   size_t identifier_length,
                   size_t *value_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_get_hash_value(
                   libewf_handle_t *handle,
                   const uint8_t *identifier,
                   size_t identifier_length,
                   uint8_t *value,
                   size_t value_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_handle_set_hash_value(
                   libewf_handle_t *handle,
                   const uint8_t *identifier,
                   size_t identifier_length,
                   const uint8_t *value,
                   size_t value_length,
                   liberror_error_t **error );

int libewf_handle_parse_hash_values(
     libewf_internal_handle_t *internal_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

