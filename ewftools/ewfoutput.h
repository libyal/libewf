/*
 * User output functions for the ewftools
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

#if !defined( _EWFOUTPUT_H )
#define _EWFOUTPUT_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#include <libewf/handle.h>

#include "character_string.h"
#include "date_time.h"
#include "system_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

void ewfoutput_version_fprint(
      FILE *stream,
      character_t *program );

void ewfoutput_copyright_fprint(
      FILE *stream );

void ewfoutput_error_fprint(
      FILE *stream,
      char *format,
      ... );

void ewfoutput_timestamp_fprint(
      FILE *stream,
      time_t timestamp );

void ewfoutput_bytes_per_second_fprint(
      FILE *stream,
      size64_t bytes,
      time_t seconds );

void ewfoutput_bytes_fprint(
      FILE *stream,
      size64_t bytes );

void ewfoutput_acquiry_parameters_fprint(
      FILE *stream,
      system_character_t *filename,
      character_t *case_number,
      character_t *description,
      character_t *evidence_number,
      character_t *examiner_name,
      character_t *notes,
      uint8_t media_type,
      uint8_t volume_type,
      int8_t compression_level,
      uint8_t compress_empty_block,
      uint8_t libewf_format,
      off64_t acquiry_offset,
      size64_t acquiry_size,
      size64_t segment_file_size,
      uint32_t sectors_per_chunk,
      uint32_t sector_error_granularity,
      uint8_t read_error_retry,
      uint8_t wipe_block_on_read_error );

void ewfoutput_acquiry_errors_fprint(
      FILE *stream,
      libewf_handle_t *handle,
      uint32_t *amount_of_errors );

void ewfoutput_crc_errors_fprint(
      FILE *stream,
      libewf_handle_t *handle,
      uint32_t *amount_of_errors );

void ewfoutput_sessions_fprint(
      FILE *stream,
      libewf_handle_t *handle,
      uint32_t *amount_of_sessions );

int ewfoutput_get_header_value(
     libewf_handle_t *handle,
     const char *utf8_header_value_identifier,
     character_t *header_value,
     size_t header_value_size,
     liberror_error_t **error );

int ewfoutput_get_hash_value(
     libewf_handle_t *handle,
     const char *utf8_hash_value_identifier,
     character_t *hash_value,
     size_t hash_value_size,
     liberror_error_t **error );

void ewfoutput_header_values_fprint(
      FILE *stream,
      libewf_handle_t *handle );

void ewfoutput_hash_values_fprint(
      FILE *stream,
      libewf_handle_t *handle,
      character_t *indentation,
      uint8_t ignore_md5,
      uint8_t ignore_sha1 );

#if defined( __cplusplus )
}
#endif

#endif

