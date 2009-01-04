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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#ifndef _EWFCOMMON_H
#define _EWFCOMMON_H

#include "../libewf/libewf_includes.h"

#include <stdio.h>

#include <libewf/libewf_handle.h>

#include "ewfsha1.h"

#include "../libewf/libewf_char.h"
#include "../libewf/libewf_internal_handle.h"

#include "../libewf/ewf_chunk.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Have wide character support for the functions:
 * main (wmain)
 * open (wopen)
 */
#define CHAR_T			wchar_t
#define INT_T			wint_t

#define PRIc			"lc"
#define PRIs			"ls"

/* Intermediate version of the macro
 * required for correct evaluation
 * predefined string
 */
#define _S_CHAR_T_I( string )	L ## string
#define _S_CHAR_T( string )	_S_CHAR_T_I( string )

#if defined( HAVE_WCSLEN )
#define CHAR_T_LENGTH( string ) \
	wcslen( string )
#else
#error Missing wide character string length function (wcslen)
#endif

#if defined( HAVE_WMEMCMP )
#define CHAR_T_COMPARE( string1, string2, length ) \
	wmemcmp( (void *) string1, (void *) string2, length )
#elif defined( HAVE_WCSNCMP )
#define CHAR_T_COMPARE( string1, string2, length ) \
	wcsncmp( string1, string2, length )
#elif defined( HAVE_WCSCMP )
#define CHAR_T_COMPARE( string1, string2, length ) \
	wcscmp( string1, string2 )
#else
#error Missing wide character string compare function (wmemcmp, wcsncmp and wcscmp)
#endif

#if defined( HAVE_WMEMCPY )
#define CHAR_T_COPY( destination, source, length ) \
	(CHAR_T *) wmemcpy( (void *) destination, (void *) source, length )
#elif defined( HAVE_WCSNCPY )
#define CHAR_T_COPY( destination, source, length ) \
	wcsncpy( destination, source, length )
#elif defined( HAVE_WCSCPY )
#define CHAR_T_COPY( destination, source, length ) \
	wcscpy( destination, source )
#else
#error Missing wide character string copy function (wmemcpy, wcsncpy and wcscpy)
#endif

#if defined( HAVE_WINDOWS_API )
#define CHAR_T_DUPLICATE( string ) \
	_wcsdup( string )
#elif defined( HAVE_WCSDUP )
#define CHAR_T_DUPLICATE( string ) \
	wcsdup( string )
#else
#error Missing wide character string duplicate function (_wcssup and wcsdup)
#endif

#if defined( HAVE_WMEMCHR )
#define CHAR_T_SEARCH( string, character, length ) \
	(CHAR_T *) wmemchr( (void *) string, character, length )
#elif defined( HAVE_WCSCHR )
#define CHAR_T_SEARCH( string, character, length ) \
	wcschr( string, character )
#else
#error Missing wide character string search function (wmemchr and wcschr)
#endif

#if defined( HAVE_WCSTOL )
#define CHAR_T_TOLONG( string, end_of_string, base ) \
	(int64_t) wcstol( string, end_of_string, base )
#else
#error Missing wide character string to long (wcstol)
#endif

#if defined( HAVE_FGETWS )
#define CHAR_T_GET_FROM_STREAM( string, size, stream ) \
	fgetws( string, size, stream )
#else
#error Missing wide character string get from stream function (fgetws)
#endif

#else

/* Have character support for the functions:
 * main
 * open
 */
#define CHAR_T			char
#define INT_T			int

#define PRIc			"c"
#define PRIs			"s"

#define _S_CHAR_T( string )	string

#if defined( HAVE_STRLEN )
#define CHAR_T_LENGTH( string ) \
	strlen( string )
#else
#error Missing string length function (strlen)
#endif

#if defined( HAVE_MEMCMP )
#define CHAR_T_COMPARE( string1, string2, length ) \
	memcmp( (void *) string1, (void *) string2, length )
#elif defined( HAVE_STRNCMP )
#define CHAR_T_COMPARE( string1, string2, length ) \
	strncmp( string1, string2, length )
#elif defined( HAVE_STRCMP )
#define CHAR_T_COMPARE( string1, string2, length ) \
	strcmp( string1, string2 )
#else
#error Missing string compare function (memcmp, strncmp and strcmp)
#endif

#if defined( HAVE_MEMCPY )
#define CHAR_T_COPY( destination, source, length ) \
	(CHAR_T *) memcpy( (void *) destination, (void *) source, length )
#elif defined( HAVE_STRNCPY )
#define CHAR_T_COPY( destination, source, length ) \
	strncpy( destination, source, length )
#elif defined( HAVE_STRCPY )
#define CHAR_T_COPY( destination, source, length ) \
	strcpy( destination, source )
#else
#error Missing string copy function (memcpy, strncpy and strcpy)
#endif

#if defined( HAVE_WINDOWS_API )
#define CHAR_T_DUPLICATE( string ) \
	_strdup( string )
#elif defined( HAVE_STRDUP )
#define CHAR_T_DUPLICATE( string ) \
	strdup( string )
#else
#error Missing string duplicate function (_strsup and strdup)
#endif

#if defined( HAVE_MEMCHR )
#define CHAR_T_SEARCH( string, character, length ) \
	(CHAR_T *) memchr( (void *) string, character, length )
#elif defined( HAVE_STRCHR )
#define CHAR_T_SEARCH( string, character, length ) \
	strchr( string, character )
#else
#error Missing string search function (memchr and strchr)
#endif

#if defined( HAVE_STRTOL )
#define CHAR_T_TOLONG( string, end_of_string, base ) \
	(int64_t) strtol( string, end_of_string, base )
#elif defined( HAVE_ATOL )
#define CHAR_T_TOLONG( string, end_of_string, base ) \
	(int64_t) atol( string )
#else
#error Missing string to long function (strtol and atol)
#endif

#if defined( HAVE_FGETS )
#define CHAR_T_GET_FROM_STREAM( string, size, stream ) \
	fgets( string, size, stream )
#else
#error Missing string get from stream function (fgets)
#endif

#endif

LIBEWF_CHAR *ewfcommon_determine_operating_system( void );
int8_t ewfcommon_determine_guid( uint8_t *guid, uint8_t libewf_format );
LIBEWF_CHAR *ewfcommon_determine_units_string( int factor );
LIBEWF_CHAR *ewfcommon_determine_human_readable_size_string( uint64_t size );

int8_t ewfcommon_copy_libewf_char_from_char_t( LIBEWF_CHAR *destination, const CHAR_T *source, size_t length );
int8_t ewfcommon_copy_libewf_char_to_char_t( const LIBEWF_CHAR *source, CHAR_T *destination, size_t length );

LIBEWF_CHAR *ewfcommon_get_user_input_variable( FILE *stream, LIBEWF_CHAR *request_string );
CHAR_T *ewfcommon_get_user_input_variable_char_t( FILE *stream, LIBEWF_CHAR *request_string );
uint64_t ewfcommon_get_user_input_size_variable( FILE *stream, LIBEWF_CHAR *request_string, uint64_t minimum, uint64_t maximum, uint64_t default_value );
LIBEWF_CHAR *ewfcommon_get_user_input_fixed_value( FILE *stream, LIBEWF_CHAR *request_string, LIBEWF_CHAR **values, uint8_t amount, uint8_t default_value );

int8_t ewfcommon_get_sha1_hash( EWFSHA1_CONTEXT *sha1_context, LIBEWF_CHAR *sha1_hash_string, size_t size );

void ewfcommon_version_fprint( FILE *stream, LIBEWF_CHAR *program );
void ewfcommon_copyright_fprint( FILE *stream );
void ewfcommon_acquiry_paramters_fprint( FILE *stream, CHAR_T *filename, LIBEWF_CHAR *case_number, LIBEWF_CHAR *description, LIBEWF_CHAR *evidence_number, LIBEWF_CHAR *examiner_name, LIBEWF_CHAR *notes, uint8_t media_type, uint8_t volume_type, int8_t compression_level, uint8_t compress_empty_block, uint8_t libewf_format, uint64_t acquiry_offset, uint64_t acquiry_size, uint32_t segment_file_size, uint64_t sectors_per_chunk, uint32_t sector_error_granularity, uint8_t read_error_retry, uint8_t wipe_block_on_read_error );
void ewfcommon_acquiry_errors_fprint( FILE *stream, LIBEWF_HANDLE *handle );
void ewfcommon_crc_errors_fprint( FILE *stream, LIBEWF_HANDLE *handle );
void ewfcommon_header_values_fprint( FILE *stream, LIBEWF_HANDLE *handle );
void ewfcommon_hash_values_fprint( FILE *stream, LIBEWF_HANDLE *handle );
void ewfcommon_timestamp_fprint( FILE *stream, time_t timestamp );
void ewfcommon_bytes_per_second_fprint( FILE *stream, uint64_t bytes, uint64_t seconds );
void ewfcommon_bytes_fprint( FILE *stream, uint64_t bytes );

void ewfcommon_process_status_initialize( FILE *stream, LIBEWF_CHAR *string, time_t timestamp_start );
void ewfcommon_process_status_fprint( uint64_t bytes_read, uint64_t bytes_total );
void ewfcommon_stream_process_status_fprint( uint64_t bytes_read, uint64_t bytes_total );

void ewfcommon_process_summary_fprint( FILE *stream, LIBEWF_CHAR *string, int64_t byte_count, time_t timestamp_start, time_t timestamp_end );

int32_t ewfcommon_read_input( LIBEWF_HANDLE *handle, int file_descriptor, EWF_CHUNK *buffer, uint64_t size, int64_t total_read_count, uint64_t total_input_size, uint8_t read_error_retry, uint32_t sector_error_granularity, uint8_t wipe_block_on_read_error, uint8_t seek_on_error );

int64_t ewfcommon_read( LIBEWF_HANDLE *handle, uint8_t calculate_sha1, void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) );
int64_t ewfcommon_read_to_file_descriptor( LIBEWF_HANDLE *handle, int output_file_descriptor, uint64_t read_size, uint64_t read_offset, void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) );

int64_t ewfcommon_write_from_file_descriptor( LIBEWF_HANDLE *handle, int input_file_descriptor, uint64_t write_size, uint64_t write_offset, uint8_t read_error_retry, uint32_t sector_error_granularity, uint8_t wipe_block_on_read_error, uint8_t seek_on_error, uint8_t calculate_sha1, void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) );

#ifdef __cplusplus
}
#endif

#endif

