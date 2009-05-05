/*
 * System character string functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _SYSTEM_STRING_H )
#define _SYSTEM_STRING_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "error_string.h"
#include "file_io.h"
#include "file_stream_io.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* Detect if the code is being compiled with Windows Unicode support
 */
#if defined( WINAPI ) && ( defined( _UNICODE ) || defined( UNICODE ) )
#define HAVE_WIDE_SYSTEM_CHARACTER_T	1
#endif

#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )

typedef wchar_t system_character_t;
typedef wint_t system_integer_t;

#define PRIc_SYSTEM	"lc"
#define PRIs_SYSTEM	"ls"

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _SYSTEM_CHARACTER_T_STRING_INTERMEDIATE( string ) \
	L ## string

#define _SYSTEM_CHARACTER_T_STRING( string ) \
	_SYSTEM_CHARACTER_T_STRING_INTERMEDIATE( string )

#define system_string_length( string ) \
	wide_string_length( string )

#define system_string_size( string ) \
	wide_string_size( string )

#define system_string_compare( string1, string2, size ) \
	wide_string_compare( string1, string2, size )

#define system_string_copy( destination, source, size ) \
	wide_string_copy( destination, source, size )

#define system_string_search( string, character, size ) \
	wide_string_search( string, character, size )

#define system_string_search_reverse( string, character, size ) \
	wide_string_search_reverse( string, character, size )

#define system_string_snprintf( target, size, format, ... ) \
	wide_string_snprintf( target, size, format, __VA_ARGS__ )

#define system_string_to_signed_long_long( string, end_of_string, base ) \
	wide_string_to_signed_long_long( string, end_of_string, base )

#define system_string_to_unsigned_long_long( string, end_of_string, base ) \
	wide_string_to_unsigned_long_long( string, end_of_string, base )

#define system_string_get_from_stream( string, size, stream ) \
	file_stream_io_get_wide_string( string, size, stream )

#define system_string_fopen( filename, mode ) \
        file_stream_io_wfopen( filename, mode )

#define system_string_open( filename, mode, error ) \
        file_io_wopen( filename, mode, error )

#define system_string_file_exists( filename, error ) \
        file_io_wexists( filename, error )

#define system_string_strerror( error_number ) \
        error_string_wcserror( error_number )

/* The system string type contains UTF-8 or ASCII with or without a codepage
 */
#else

typedef char system_character_t;
typedef int system_integer_t;

#define PRIc_SYSTEM	"c"
#define PRIs_SYSTEM	"s"

#define _SYSTEM_CHARACTER_T_STRING( string ) \
	string

#define system_string_length( string ) \
	narrow_string_length( string )

#define system_string_size( string ) \
	narrow_string_size( string )

#define system_string_compare( string1, string2, size ) \
	narrow_string_compare( string1, string2, size )

#define system_string_copy( destination, source, size ) \
	narrow_string_copy( destination, source, size )

#define system_string_search( string, character, size ) \
	narrow_string_search( string, character, size )

#define system_string_search_reverse( string, character, size ) \
	narrow_string_search_reverse( string, character, size )

#define system_string_snprintf( target, size, format, ... ) \
	narrow_string_snprintf( target, size, format, __VA_ARGS__ )

#define system_string_to_signed_long_long( string, end_of_string, base ) \
	narrow_string_to_signed_long_long( string, end_of_string, base )

#define system_string_to_unsigned_long_long( string, end_of_string, base ) \
	narrow_string_to_unsigned_long_long( string, end_of_string, base )

#define system_string_get_from_stream( string, size, stream ) \
	file_stream_io_get_narrow_string( string, size, stream )

#define system_string_fopen( filename, mode ) \
        file_stream_io_fopen( filename, mode )

#define system_string_open( filename, mode, error ) \
        file_io_open( filename, mode, error )

#define system_string_file_exists( filename, error ) \
        file_io_exists( filename, error )

#define system_string_strerror( error_number ) \
        error_string_strerror( error_number )

#endif

int system_string_initialize(
     liberror_error_t **error );

#if defined( system_string_to_signed_long_long )
int system_string_to_int64(
     const system_character_t *string,
     size_t string_size,
     int64_t *value,
     liberror_error_t **error );
#endif

#if defined( system_string_to_unsigned_long_long )
int system_string_to_uint64(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value,
     liberror_error_t **error );
#endif

int system_string_size_from_utf8_string(
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *string_size,
     liberror_error_t **error );

int system_string_copy_from_utf8_string(
     system_character_t *string,
     size_t string_size,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error );

int system_string_trim_copy_from_byte_stream(
     system_character_t *string,
     size_t string_size,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     liberror_error_t **error );

int utf8_string_size_from_system_string(
     const system_character_t *string,
     size_t string_size,
     size_t *utf8_string_size,
     liberror_error_t **error );

int utf8_string_copy_from_system_string(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     const system_character_t *string,
     size_t string_size,
     liberror_error_t **error );

int system_string_split(
     system_character_t *string,
     size_t string_size,
     system_character_t delimiter,
     system_character_t ***split_values,
     size_t *amount_of_split_values,
     liberror_error_t **error );

int system_string_split_values_free(
     system_character_t **split_values,
     size_t amount_of_split_values,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

