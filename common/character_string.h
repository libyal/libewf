/*
 * Character string functions
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

#if !defined( _CHARACTER_STRING_H )
#define _CHARACTER_STRING_H

#include "common.h"
#include "date_time.h"
#include "narrow_string.h"
#include "string_conversion.h"
#include "types.h"
#include "wide_string.h"

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#if defined( HAVE_LIBUCA_H )
#include <libuca.h>
#elif defined( HAVE_LOCAL_LIBUCA )
#include "../libuca/libuca_byte_stream.h"
#include "../libuca/libuca_unicode_character.h"
#include "../libuca/libuca_utf8_string.h"
#include "../libuca/libuca_utf16_stream.h"
#include "../libuca/libuca_utf16_string.h"
#include "../libuca/libuca_utf32_stream.h"
#include "../libuca/libuca_utf32_string.h"
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )

typedef wchar_t libewf_character_t;

#define character_t libewf_character_t

#define PRIc	"lc"
#define PRIs	"ls"

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _CHARACTER_T_STRING_INTERMEDIATE( string ) \
	L ## string

#define _CHARACTER_T_STRING( string ) \
	_CHARACTER_T_STRING_INTERMEDIATE( string )

#define string_length( string ) \
	wide_string_length( string )

#define string_size( string ) \
	wide_string_size( string )

#define string_compare( string1, string2, size ) \
	wide_string_compare( string1, string2, size )

#define string_copy( destination, source, size ) \
	wide_string_copy( destination, source, size )

#define string_search( string, character, size ) \
	wide_string_search( string, character, size )

#define string_search_reverse( string, character, size ) \
	wide_string_search_reverse( string, character, size )

#define string_snprintf( target, size, format, ... ) \
	wide_string_snprintf( target, size, format, __VA_ARGS__ )

#define string_get_from_stream( string, size, stream ) \
	wide_string_get_from_stream( string, size, stream )

#define string_to_signed_long_long( string, end_of_string, base ) \
	wide_string_to_signed_long_long( string, end_of_string, base )

#define string_to_unsigned_long_long( string, end_of_string, base ) \
	wide_string_to_unsigned_long_long( string, end_of_string, base )

#else

typedef char libewf_character_t;

#define character_t libewf_character_t

#define PRIc	"c"
#define PRIs	"s"

#define _CHARACTER_T_STRING( string ) \
	string

#define string_length( string ) \
	narrow_string_length( string )

#define string_size( string ) \
	narrow_string_size( string )

#define string_compare( string1, string2, size ) \
	narrow_string_compare( string1, string2, size )

#define string_copy( destination, source, size ) \
	narrow_string_copy( destination, source, size )

#define string_search( string, character, size ) \
	narrow_string_search( string, character, size )

#define string_search_reverse( string, character, size ) \
	narrow_string_search_reverse( string, character, size )

#define string_snprintf( target, size, format, ... ) \
	narrow_string_snprintf( target, size, format, __VA_ARGS__ )

#define string_get_from_stream( string, size, stream ) \
	narrow_string_get_from_stream( string, size, stream )

#define string_to_signed_long_long( string, end_of_string, base ) \
	narrow_string_to_signed_long_long( string, end_of_string, base )

#define string_to_unsigned_long_long( string, end_of_string, base ) \
	narrow_string_to_unsigned_long_long( string, end_of_string, base )

#endif

character_t *libewf_string_duplicate(
              character_t *string,
              size_t size );

#define string_duplicate( string, size ) \
	libewf_string_duplicate( string, size )

#if defined( string_to_signed_long_long )

int libewf_string_to_int64(
     const character_t *string,
     size_t size,
     int64_t *value );

#define string_to_int64( string, size, value ) \
	 libewf_string_to_int64( string, size, value )
#endif

#if defined( string_to_unsigned_long_long )

int libewf_string_to_uint64(
     const character_t *string,
     size_t size,
     uint64_t *value );

#define string_to_uint64( string, size, value ) \
	 libewf_string_to_uint64( string, size, value )
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#define string_copy_from_char( destination, source, size ) \
	string_copy_char_to_wchar( destination, source, size )

#define string_copy_to_char( destination, source, size ) \
	string_copy_wchar_to_char( destination, source, size )

#else
#define string_copy_from_char( destination, source, size ) \
	( string_copy( destination, source, size ) == NULL ) ? -1 : 1

#define string_copy_to_char( destination, source, size ) \
	( string_copy( destination, source, size ) == NULL ) ? -1 : 1

#endif

/* Define the character string ctime function
 * if date_time_ctime (or equivalent) was defined
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE )
#if defined( date_time_wctime )
#define string_ctime( timestamp, string, size ) \
	date_time_wctime( timestamp, string, size )

#elif defined( date_time_ctime )
character_t *libewf_string_ctime(
              const time_t *timestamp,
              character_t *string,
              size_t size );

#define string_ctime( timestamp, string, size ) \
	libewf_string_ctime( timestamp, string, size )
#endif

#elif defined( date_time_ctime )
#define string_ctime( timestamp, string, size ) \
	date_time_ctime( timestamp, string, size )
#endif

#if defined( HAVE_LIBUCA_H ) || defined( HAVE_LOCAL_LIBUCA )

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#if SIZEOF_WCHAR_T == 4
#define string_size_from_byte_stream( stream, size_stream, byte_order ) \
	libuca_utf32_string_size_from_byte_stream( stream, size_stream, byte_order, 0 )

#define string_copy_from_byte_stream( string, string_size, stream, size_stream, byte_order ) \
	libuca_utf32_string_copy_from_byte_stream( (libuca_utf32_character_t *) string, string_size, stream, size_stream, byte_order, 0 )

#define string_copy_to_byte_stream( string, string_size, stream, size_stream, byte_order ) \
	libuca_utf32_string_copy_to_byte_stream( (libuca_utf32_character_t *) string, string_size, stream, size_stream, byte_order, 0 )

#elif SIZEOF_WCHAR_T == 2
#define string_size_from_byte_stream( stream, size_stream, byte_order ) \
	libuca_utf16_string_size_from_byte_stream( stream, size_stream, byte_order, 0 )

#define string_copy_from_byte_stream( string, string_size, stream, size_stream, byte_order ) \
	libuca_utf16_string_copy_from_byte_stream( (libuca_utf16_character_t *) string, string_size, stream, size_stream, byte_order, 0 )

#define string_copy_to_byte_stream( string, string_size, stream, size_stream, byte_order ) \
	libuca_utf16_string_copy_to_byte_stream( (libuca_utf16_character_t *) string, string_size, stream, size_stream, byte_order, 0 )

#else
#error Unsupported size of wchar_t
#endif

#else
#define string_size_from_byte_stream( stream, size_stream, byte_order ) \
	libuca_utf8_string_size_from_byte_stream( stream, size_stream, byte_order, 0 )

#define string_copy_from_byte_stream( string, string_size, stream, size_stream, byte_order ) \
	libuca_utf8_string_copy_from_byte_stream( (libuca_utf8_character_t *) string, string_size, stream, size_stream, byte_order, 0 )

#define string_copy_to_byte_stream( string, string_size, stream, size_stream, byte_order ) \
	libuca_utf8_string_copy_to_byte_stream( (libuca_utf8_character_t *) string, string_size, stream, size_stream, byte_order, 0 )

#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#if SIZEOF_WCHAR_T == 4
#define string_size_from_utf16_stream( stream, size_stream, byte_order ) \
	libuca_utf32_string_size_from_utf16_stream( stream, size_stream, byte_order, 0 )

#define string_copy_from_utf16_stream( string, string_size, stream, size_stream, byte_order ) \
	libuca_utf32_string_copy_from_utf16_stream( (libuca_utf32_character_t *) string, string_size, stream, size_stream, byte_order, 0 )

#define utf16_stream_size_from_string( string, size_string ) \
	libuca_utf16_stream_size_from_utf32( string, size_string, 0 )

#define utf16_stream_copy_from_string( stream, size_stream, byte_order, string, string_size ) \
	libuca_utf16_stream_copy_from_utf32( stream, size_stream, byte_order, (libuca_utf32_character_t *) string, string_size, 0 )

#elif SIZEOF_WCHAR_T == 2
#define string_size_from_utf16_stream( stream, size_stream, byte_order ) \
	libuca_utf16_string_size_from_utf16_stream( stream, size_stream, byte_order, 0 )

#define string_copy_from_utf16_stream( string, string_size, stream, size_stream, byte_order ) \
	libuca_utf16_string_copy_from_utf16_stream( (libuca_utf16_character_t *) string, string_size, stream, size_stream, byte_order, 0 )

#define utf16_stream_size_from_string( string, size_string ) \
	libuca_utf16_stream_size_from_utf16( string, size_string, 0 )

#define utf16_stream_copy_from_string( stream, size_stream, byte_order, string, string_size ) \
	libuca_utf16_stream_copy_from_utf16( stream, size_stream, byte_order, (libuca_utf16_character_t *) string, string_size, 0 )

#else
#error Unsupported size of wchar_t
#endif

#else
#define string_size_from_utf16_stream( stream, size_stream, byte_order ) \
	libuca_utf8_string_size_from_utf16_stream( stream, size_stream, byte_order, 0 )

#define string_copy_from_utf16_stream( string, string_size, stream, size_stream, byte_order ) \
	libuca_utf8_string_copy_from_utf16_stream( (libuca_utf8_character_t *) string, string_size, stream, size_stream, byte_order, 0 )

#define utf16_stream_size_from_string( string, size_string ) \
	libuca_utf16_stream_size_from_utf8( string, size_string, 0 )

#define utf16_stream_copy_from_string( stream, size_stream, byte_order, string, string_size ) \
	libuca_utf16_stream_copy_from_utf8( stream, size_stream, byte_order, (libuca_utf8_character_t *) string, string_size, 0 )

#endif

#endif

#if defined( __cplusplus )
}
#endif

#endif

