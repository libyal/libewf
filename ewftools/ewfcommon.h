/*
 * ewfcommon
 * Common functions for the libewf tools
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include "../libewf/libewf_includes.h"

#include <stdio.h>

#include <libewf/libewf_handle.h>

#include "ewfdigest_context.h"
#include "ewfmd5.h"
#include "ewfsha1.h"
#include "ewfstring.h"

#include "../libewf/libewf_char.h"
#include "../libewf/libewf_internal_handle.h"

#include "../libewf/ewf_chunk.h"

#if defined( __cplusplus )
extern "C" {
#endif

int ewfcommon_swap_byte_pairs( uint8_t *buffer, size_t size );

LIBEWF_CHAR *ewfcommon_determine_operating_system( void );
int8_t ewfcommon_determine_guid( uint8_t *guid, uint8_t libewf_format );
LIBEWF_CHAR *ewfcommon_determine_units_string( int factor );
LIBEWF_CHAR *ewfcommon_determine_human_readable_size_string( uint64_t size );

struct tm *ewfcommon_gmtime( const time_t *timestamp );

void ewfcommon_version_fprint( FILE *stream, LIBEWF_CHAR *program );
void ewfcommon_copyright_fprint( FILE *stream );
void ewfcommon_acquiry_parameters_fprint( FILE *stream, CHAR_T *filename, LIBEWF_CHAR *case_number, LIBEWF_CHAR *description, LIBEWF_CHAR *evidence_number, LIBEWF_CHAR *examiner_name, LIBEWF_CHAR *notes, uint8_t media_type, uint8_t volume_type, int8_t compression_level, uint8_t compress_empty_block, uint8_t libewf_format, off64_t acquiry_offset, size64_t acquiry_size, size64_t segment_file_size, uint32_t sectors_per_chunk, uint32_t sector_error_granularity, uint8_t read_error_retry, uint8_t wipe_block_on_read_error );
void ewfcommon_acquiry_errors_fprint( FILE *stream, LIBEWF_HANDLE *handle );
void ewfcommon_crc_errors_fprint( FILE *stream, LIBEWF_HANDLE *handle );
void ewfcommon_header_values_fprint( FILE *stream, LIBEWF_HANDLE *handle );
void ewfcommon_hash_values_fprint( FILE *stream, LIBEWF_HANDLE *handle );
void ewfcommon_timestamp_fprint( FILE *stream, time_t timestamp );
void ewfcommon_bytes_per_second_fprint( FILE *stream, size64_t bytes, time_t seconds );
void ewfcommon_bytes_fprint( FILE *stream, size64_t bytes );

void ewfcommon_process_status_initialize( FILE *stream, LIBEWF_CHAR *string, time_t timestamp_start );
void ewfcommon_process_status_fprint( size64_t bytes_read, size64_t bytes_total );
void ewfcommon_stream_process_status_fprint( size64_t bytes_read, size64_t bytes_total );

void ewfcommon_process_summary_fprint( FILE *stream, LIBEWF_CHAR *string, ssize64_t byte_count, time_t timestamp_start, time_t timestamp_end );

ssize32_t ewfcommon_read_input( LIBEWF_HANDLE *handle, int file_descriptor, EWF_CHUNK *buffer, size_t buffer_size, size32_t chunk_size, uint32_t bytes_per_sector, ssize64_t total_read_count, size64_t total_input_size, uint8_t read_error_retry, uint32_t sector_error_granularity, uint8_t wipe_block_on_read_error, uint8_t seek_on_error );

ssize64_t ewfcommon_read_verify( LIBEWF_HANDLE *handle, uint8_t calculate_md5, LIBEWF_CHAR *md5_hash_string, size_t md5_hash_string_length, uint8_t calculate_sha1, LIBEWF_CHAR *sha1_hash_string, size_t sha1_hash_string_length, uint8_t swap_byte_pairs, void (*callback)( size64_t bytes_read, size64_t bytes_total ) );

ssize64_t ewfcommon_write_from_file_descriptor( LIBEWF_HANDLE *handle, int input_file_descriptor, size64_t write_size, off64_t write_offset, uint8_t read_error_retry, uint32_t sector_error_granularity, uint8_t wipe_block_on_read_error, uint8_t seek_on_error, uint8_t calculate_md5, LIBEWF_CHAR *md5_hash_string, size_t md5_hash_string_length, uint8_t calculate_sha1, LIBEWF_CHAR *sha1_hash_string, size_t sha1_hash_string_length, uint8_t swap_byte_pairs, void (*callback)( size64_t bytes_read, size64_t bytes_total ) );

ssize64_t ewfcommon_export_raw( LIBEWF_HANDLE *handle, CHAR_T *target_filename, size64_t maximum_file_size, size64_t read_size, off64_t read_offset, uint8_t swap_byte_pairs, void (*callback)( size64_t bytes_read, size64_t bytes_total ) );
ssize64_t ewfcommon_export_ewf( LIBEWF_HANDLE *handle, LIBEWF_HANDLE *export_handle, size64_t read_size, off64_t read_offset, uint8_t swap_byte_pairs, void (*callback)( size64_t bytes_read, size64_t bytes_total ) );

#if defined( __cplusplus )
}
#endif

#endif

