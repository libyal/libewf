/*
 * Common functions for the ewftools
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

#if !defined( _EWFCOMMON_H )
#define _EWFCOMMON_H

#include <common.h>
#include <character_string.h>
#include <error_string.h>
#include <date_time.h>
#include <file_stream_io.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#include <libewf/definitions.h>
#include <libewf/handle.h>

#include "ewfdigest_context.h"
#include "ewfmd5.h"
#include "ewfprocess_status.h"
#include "ewfsignal.h"
#include "ewfsha1.h"

#define EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE		LIBEWF_DEFAULT_SEGMENT_FILE_SIZE
#define EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE		( 1024 * 1024 )
#define EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT	INT32_MAX
#define EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT	INT64_MAX

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#define ewfcommon_ctime( timestamp, string, length ) \
	date_time_wctime( timestamp, string, length )

#define ewfcommon_strerror( error_number ) \
        error_string_wcserror( error_number )

#define ewfcommon_fopen( filename, mode ) \
	file_stream_io_wfopen( filename, mode )

#else
#define ewfcommon_ctime( timestamp, string, length ) \
	date_time_ctime( timestamp, string, length )

#define ewfcommon_strerror( error_number ) \
        error_string_strerror( error_number )

#define ewfcommon_fopen( filename, mode ) \
	file_stream_io_fopen( filename, mode )

#endif

extern int ewfcommon_abort;
extern libewf_handle_t *ewfcommon_libewf_handle;

void ewfcommon_signal_handler(
      ewfsignal_t signal );

int ewfcommon_swap_byte_pairs(
     uint8_t *buffer,
     size_t size );

character_t *ewfcommon_determine_operating_system(
              void );

int8_t ewfcommon_determine_guid(
        uint8_t *guid,
        uint8_t libewf_format );

int ewfcommon_initialize_write(
     libewf_handle_t *handle,
     character_t *case_number,
     character_t *description,
     character_t *evidence_number,
     character_t *examiner_name,
     character_t *notes,
     character_t *acquiry_operating_system,
     character_t *acquiry_software,
     character_t *acquiry_software_version,
     uint8_t media_type,
     uint8_t volume_type,
     int8_t compression_level,
     uint8_t compress_empty_block,
     uint8_t libewf_format,
     size64_t segment_file_size,
     uint32_t sector_error_granularity );

ssize32_t ewfcommon_read_input(
           libewf_handle_t *handle,
           int file_descriptor,
           uint8_t *buffer,
           size_t buffer_size,
           size32_t chunk_size,
           uint32_t bytes_per_sector,
           ssize64_t total_read_count,
           size64_t total_input_size,
           uint8_t read_error_retry,
           uint32_t sector_error_granularity,
           uint8_t wipe_chunk_on_error,
           uint8_t seek_on_error );

#if defined( HAVE_RAW_ACCESS )
ssize_t ewfcommon_raw_read_ewf(
         libewf_handle_t *handle,
         uint8_t *raw_buffer,
         size_t raw_buffer_size,
         uint8_t **buffer,
         size_t buffer_size,
         size_t read_size,
         off64_t read_offset,
         size64_t media_size,
         uint32_t sectors_per_chunk,
         uint32_t bytes_per_sector,
         uint8_t wipe_chunk_on_error );

ssize_t ewfcommon_raw_write_ewf(
         libewf_handle_t *handle,
         uint8_t *raw_buffer,
         size_t raw_buffer_size,
         uint8_t *buffer,
         size_t buffer_size,
         size_t write_size );
#endif

ssize64_t ewfcommon_read_verify(
           libewf_handle_t *handle,
           uint8_t calculate_md5,
           character_t *md5_hash_string,
           size_t md5_hash_string_length,
           uint8_t calculate_sha1,
           character_t *sha1_hash_string,
           size_t sha1_hash_string_length,
           uint8_t swap_byte_pairs,
           uint8_t wipe_chunk_on_error,
           void (*callback)( ewfprocess_status_t *process_status, size64_t bytes_read, size64_t bytes_total ) );

ssize64_t ewfcommon_write_from_file_descriptor(
           libewf_handle_t *handle,
           int input_file_descriptor,
           size64_t write_size,
           off64_t write_offset,
           uint32_t sectors_per_chunk,
           uint32_t bytes_per_sector,
           uint8_t read_error_retry,
           uint32_t sector_error_granularity,
           uint8_t wipe_chunk_on_error,
           uint8_t seek_on_error,
           uint8_t calculate_md5,
           character_t *md5_hash_string,
           size_t md5_hash_string_length,
           uint8_t calculate_sha1,
           character_t *sha1_hash_string,
           size_t sha1_hash_string_length,
           uint8_t swap_byte_pairs,
           void (*callback)( ewfprocess_status_t *process_status, size64_t bytes_read, size64_t bytes_total ) );

ssize64_t ewfcommon_export_raw(
           libewf_handle_t *handle,
           system_character_t *target_filename,
           size64_t export_size,
           off64_t read_offset,
           uint8_t swap_byte_pairs,
           uint8_t wipe_chunk_on_error,
           void (*callback)( ewfprocess_status_t *process_status, size64_t bytes_read, size64_t bytes_total ) );

ssize64_t ewfcommon_export_ewf(
           libewf_handle_t *handle,
           libewf_handle_t *export_handle,
           int8_t compression_level,
           uint8_t compress_empty_block,
           uint8_t libewf_format,
           size64_t segment_file_size,
           size64_t export_size,
           off64_t read_offset,
           uint32_t export_sectors_per_chunk,
           uint8_t calculate_md5,
           uint8_t calculate_sha1,
           uint8_t swap_byte_pairs,
           uint8_t wipe_chunk_on_error,
           void (*callback)( ewfprocess_status_t *process_status, size64_t bytes_read, size64_t bytes_total ) );

#if defined( __cplusplus )
}
#endif

#endif

