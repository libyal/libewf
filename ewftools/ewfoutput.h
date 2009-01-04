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
#include <character_string.h>
#include <system_string.h>

#include <stdio.h>

#include <libewf/handle.h>
#include <libewf/types.h>

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
      LIBEWF_HANDLE *handle,
      uint32_t *amount_of_errors );

void ewfoutput_crc_errors_fprint(
      FILE *stream,
      LIBEWF_HANDLE *handle,
      uint32_t *amount_of_errors );

void ewfoutput_sessions_fprint(
      FILE *stream,
      LIBEWF_HANDLE *handle,
      uint32_t *amount_of_sessions );

void ewfoutput_header_values_fprint(
      FILE *stream,
      LIBEWF_HANDLE *handle );

void ewfoutput_hash_values_fprint(
      FILE *stream,
      LIBEWF_HANDLE *handle );

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

void ewfoutput_process_status_initialize(
      FILE *stream,
      character_t *string,
      time_t timestamp_start );

void ewfoutput_process_status_fprint(
      size64_t bytes_read,
      size64_t bytes_total );

void ewfoutput_stream_process_status_fprint(
      size64_t bytes_read,
      size64_t bytes_total );

void ewfoutput_process_summary_fprint(
      FILE *stream,
      character_t *string,
      ssize64_t byte_count,
      time_t timestamp_start,
      time_t timestamp_end );

#if defined( __cplusplus )
}
#endif

#endif

