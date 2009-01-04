/*
 * ewfcommon
 * Common functions for the libewf tools
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined( _EWFCOMMON_H )
#define _EWFCOMMON_H

#include <common.h>
#include <character_string.h>
#include <system_string.h>

#include <stdio.h>

#include <libewf/definitions.h>
#include <libewf/handle.h>
#include <libewf/types.h>

#include "ewfdigest_context.h"
#include "ewfmd5.h"
#include "ewfsignal.h"
#include "ewfsha1.h"

#define EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE		LIBEWF_DEFAULT_SEGMENT_FILE_SIZE
#define EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE		( 1024 * 1024 )
#define EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT	INT32_MAX
#define EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT	INT64_MAX

#if defined( __cplusplus )
extern "C" {
#endif

extern int ewfcommon_abort;
extern LIBEWF_HANDLE *ewfcommon_libewf_handle;

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
     LIBEWF_HANDLE *handle,
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
           LIBEWF_HANDLE *handle,
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
         LIBEWF_HANDLE *handle,
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
         LIBEWF_HANDLE *handle,
         uint8_t *raw_buffer,
         size_t raw_buffer_size,
         uint8_t *buffer,
         size_t buffer_size,
         size_t write_size );
#endif

ssize64_t ewfcommon_read_verify(
           LIBEWF_HANDLE *handle,
           uint8_t calculate_md5,
           character_t *md5_hash_string,
           size_t md5_hash_string_length,
           uint8_t calculate_sha1,
           character_t *sha1_hash_string,
           size_t sha1_hash_string_length,
           uint8_t swap_byte_pairs,
           uint8_t wipe_chunk_on_error,
           void (*callback)( size64_t bytes_read, size64_t bytes_total ) );

ssize64_t ewfcommon_write_from_file_descriptor(
           LIBEWF_HANDLE *handle,
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
           void (*callback)( size64_t bytes_read, size64_t bytes_total ) );

ssize64_t ewfcommon_export_raw(
           LIBEWF_HANDLE *handle,
           system_character_t *target_filename,
           size64_t export_size,
           off64_t read_offset,
           uint8_t swap_byte_pairs,
           uint8_t wipe_chunk_on_error,
           void (*callback)( size64_t bytes_read, size64_t bytes_total ) );

ssize64_t ewfcommon_export_ewf(
           LIBEWF_HANDLE *handle,
           LIBEWF_HANDLE *export_handle,
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
           void (*callback)( size64_t bytes_read, size64_t bytes_total ) );

#if defined( __cplusplus )
}
#endif

#endif

