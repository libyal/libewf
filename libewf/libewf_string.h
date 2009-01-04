/*
 * Character type string functions
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

#if !defined( _LIBEWF_STRING_H )
#define _LIBEWF_STRING_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libewf_error.h"
#include "libewf_libuna.h"

#if defined( _cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )

typedef wchar_t libewf_character_t;

#define PRIc_LIBEWF	"lc"
#define PRIs_LIBEWF	"ls"

#define libewf_string_compare( string1, string2, size ) \
	wide_string_compare( string1, string2, size )

#define libewf_string_copy( destination, source, size ) \
	wide_string_copy( destination, source, size )

#define libewf_string_length( string ) \
	wide_string_length( string )

#define libewf_string_search( string, character, size ) \
	wide_string_search( string, character, size )

#define libewf_string_search_reverse( string, character, size ) \
	wide_string_search_reverse( string, character, size )

#define libewf_string_snprintf( target, size, format, ... ) \
	wide_string_snprintf( target, size, format, __VA_ARGS__ )

#define libewf_string_to_int64( string, end_of_string, base ) \
	wide_string_to_signed_long_long( string, end_of_string, base )

#define libewf_string_to_uint64( string, end_of_string, base ) \
	wide_string_to_unsigned_long_long( string, end_of_string, base )

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _LIBEWF_CHARACTER_T_STRING_INTERMEDIATE( string ) \
	L ## string

#define _LIBEWF_CHARACTER_T_STRING( string ) \
	_LIBEWF_CHARACTER_T_STRING_INTERMEDIATE( string )

/* The internal character type contains UTF-32
 */
#if SIZEOF_WCHAR_T == 4

/* Byte stream functions
 */
#define libewf_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error ) \
	libuna_utf32_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error )

#define libewf_string_copy_from_byte_stream( string, string_size, stream, stream_size, codepage, error ) \
	libuna_utf32_string_copy_from_byte_stream( (libuna_utf32_character_t *) string, string_size, stream, stream_size, codepage, error )

#define byte_stream_size_from_libewf_string( string, string_size, codepage, stream_size, error ) \
	libuna_byte_stream_size_from_utf32( (libuna_utf32_character_t *) string, string_size, codepage, stream_size, error )

#define byte_stream_copy_from_libewf_string( stream, stream_size, codepage, string, string_size, error ) \
	libuna_byte_stream_copy_from_utf32( stream, stream_size, codepage, (libuna_utf32_character_t *) string, string_size, error )

/* UTF-8 stream functions
 */
#define libewf_string_size_from_utf8_stream( stream, stream_size, string_size, error ) \
	libuna_utf32_string_size_from_utf8( stream, stream_size, string_size, error )

#define libewf_string_copy_from_utf8_stream( string, string_size, stream, stream_size, error ) \
	libuna_utf32_string_copy_from_utf8( (libuna_utf32_character_t *) string, string_size, stream, stream_size, error )

#define utf8_stream_size_from_libewf_string( string, string_size, stream_size, error ) \
	libuna_utf8_stream_size_from_utf32( (libuna_utf32_character_t *) string, string_size, stream_size, error )

#define utf8_stream_copy_from_libewf_string( stream, stream_size, string, string_size, error ) \
	libuna_utf8_stream_copy_from_utf32( stream, stream_size, (libuna_utf32_character_t *) string, string_size, error )

/* UTF-16 stream functions
 */
#define libewf_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error ) \
	libuna_utf32_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error )

#define libewf_string_copy_from_utf16_stream( string, string_size, stream, stream_size, byte_order, error ) \
	libuna_utf32_string_copy_from_utf16_stream( (libuna_utf32_character_t *) string, string_size, stream, stream_size, byte_order, error )

#define utf16_stream_size_from_libewf_string( string, string_size, stream_size, error ) \
	libuna_utf16_stream_size_from_utf32( (libuna_utf32_character_t *) string, string_size, stream_size, error )

#define utf16_stream_copy_from_libewf_string( stream, stream_size, byte_order, string, string_size, error ) \
	libuna_utf16_stream_copy_from_utf32( stream, stream_size, byte_order, (libuna_utf32_character_t *) string, string_size, error )

/* The internal character type contains UTF-16
 */
#elif SIZEOF_WCHAR_T == 2

/* Byte stream functions
 */
#define libewf_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error ) \
	libuna_utf16_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error )

#define libewf_string_copy_from_byte_stream( string, string_size, stream, stream_size, codepage, error ) \
	libuna_utf16_string_copy_from_byte_stream( (libuna_utf16_character_t *) string, string_size, stream, stream_size, codepage, error )

#define byte_stream_size_from_libewf_string( string, string_size, codepage, stream_size, error ) \
	libuna_byte_stream_size_from_utf16( (libuna_utf16_character_t *) string, string_size, codepage, stream_size, error )

#define byte_stream_copy_from_libewf_string( stream, stream_size, codepage, string, string_size, error ) \
	libuna_byte_stream_copy_from_utf16( stream, stream_size, codepage, (libuna_utf16_character_t *) string, string_size, error )

/* UTF-8 stream functions
 */
#define libewf_string_size_from_utf8_stream( stream, stream_size, string_size, error ) \
	libuna_utf16_string_size_from_utf8( stream, stream_size, string_size, error )

#define libewf_string_copy_from_utf8_stream( string, string_size, stream, stream_size, error ) \
	libuna_utf16_string_copy_from_utf8( (libuna_utf16_character_t *) string, string_size, stream, stream_size, error )

#define utf8_stream_size_from_libewf_string( string, string_size, stream_size, error ) \
	libuna_utf8_stream_size_from_utf16( (libuna_utf16_character_t *) string, string_size, stream_size, error )

#define utf8_stream_copy_from_libewf_string( stream, stream_size, string, string_size, error ) \
	libuna_utf8_stream_copy_from_utf16( stream, stream_size, (libuna_utf16_character_t *) string, string_size, error )

/* UTF-16 stream functions
 */
#define libewf_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error ) \
	libuna_utf16_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error )

#define libewf_string_copy_from_utf16_stream( string, string_size, stream, stream_size, byte_order, error ) \
	libuna_utf16_string_copy_from_utf16_stream( (libuna_utf16_character_t *) string, string_size, stream, stream_size, byte_order, error )

#define utf16_stream_size_from_libewf_string( string, string_size, stream_size, error ) \
	libuna_utf16_stream_size_from_utf16( (libuna_utf16_character_t *) string, string_size, stream_size, error )

#define utf16_stream_copy_from_libewf_string( stream, stream_size, byte_order, string, string_size, error ) \
	libuna_utf16_stream_copy_from_utf16( stream, stream_size, byte_order, (libuna_utf16_character_t *) string, string_size, error )

#else
#error Unsupported size of wchar_t
#endif

/* The internal character type contains UTF-8
 */
#else

typedef char libewf_character_t;

#define PRIc_LIBEWF	"c"
#define PRIs_LIBEWF	"s"

#define _LIBEWF_CHARACTER_T_STRING( string ) \
	string

#define libewf_string_compare( string1, string2, size ) \
	narrow_string_compare( string1, string2, size )

#define libewf_string_copy( destination, source, size ) \
	narrow_string_copy( destination, source, size )

#define libewf_string_length( string ) \
	narrow_string_length( string )

#define libewf_string_search( string, character, size ) \
	narrow_string_search( string, character, size )

#define libewf_string_search_reverse( string, character, size ) \
	narrow_string_search_reverse( string, character, size )

#define libewf_string_snprintf( target, size, format, ... ) \
	narrow_string_snprintf( target, size, format, __VA_ARGS__ )

#define libewf_string_to_int64( string, end_of_string, base ) \
	narrow_string_to_signed_long_long( string, end_of_string, base )

#define libewf_string_to_uint64( string, end_of_string, base ) \
	narrow_string_to_unsigned_long_long( string, end_of_string, base )

/* Byte stream functions
 */
#define libewf_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error ) \
	libuna_utf8_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error )

#define libewf_string_copy_from_byte_stream( string, string_size, stream, stream_size, codepage, error ) \
	libuna_utf8_string_copy_from_byte_stream( (libuna_utf8_character_t *) string, string_size, stream, stream_size, codepage, error )

#define byte_stream_size_from_libewf_string( string, string_size, codepage, stream_size, error ) \
	libuna_byte_stream_size_from_utf8( (libuna_utf8_character_t *) string, string_size, codepage, stream_size, error )

#define byte_stream_copy_from_libewf_string( stream, stream_size, codepage, string, string_size, error ) \
	libuna_byte_stream_copy_from_utf8( stream, stream_size, codepage, (libuna_utf8_character_t *) string, string_size, error )

/* UTF-8 stream functions
 */
#define libewf_string_size_from_utf8_stream( stream, stream_size, string_size, error ) \
	libuna_utf8_string_size_from_utf8_stream( stream, stream_size, string_size, error )

#define libewf_string_copy_from_utf8_stream( string, string_size, stream, stream_size, error ) \
	libuna_utf8_string_copy_from_utf8_stream( (libuna_utf8_character_t *) string, string_size, stream, stream_size, error )

#define utf8_stream_size_from_libewf_string( string, string_size, stream_size, error ) \
	libuna_utf8_stream_size_from_utf8( (libuna_utf8_character_t *) string, string_size, stream_size, error )

#define utf8_stream_copy_from_libewf_string( stream, stream_size, string, string_size, error ) \
	libuna_utf8_stream_copy_from_utf8( stream, stream_size, (libuna_utf8_character_t *) string, string_size, error )

/* UTF-16 stream functions
 */
#define libewf_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error ) \
	libuna_utf8_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error )

#define libewf_string_copy_from_utf16_stream( string, string_size, stream, stream_size, byte_order, error ) \
	libuna_utf8_string_copy_from_utf16_stream( (libuna_utf8_character_t *) string, string_size, stream, stream_size, byte_order, error )

#define utf16_stream_size_from_libewf_string( string, string_size, stream_size, error ) \
	libuna_utf16_stream_size_from_utf8( (libuna_utf8_character_t *) string, string_size, stream_size, error )

#define utf16_stream_copy_from_libewf_string( stream, stream_size, byte_order, string, string_size, error ) \
	libuna_utf16_stream_copy_from_utf8( stream, stream_size, byte_order, (libuna_utf8_character_t *) string, string_size, error )

#endif

int libewf_string_split(
     libewf_character_t *string,
     size_t string_size,
     libewf_character_t delimiter,
     libewf_character_t ***split_values,
     size_t *amount_of_split_values,
     libewf_error_t **error );

int libewf_string_split_values_free(
     libewf_character_t **split_values,
     size_t amount_of_split_values,
     libewf_error_t **error );

#if defined( _cplusplus )
}
#endif

#endif

