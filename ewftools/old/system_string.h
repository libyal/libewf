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

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
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

#define system_string_get_from_stream( string, size, stream ) \
	wide_string_get_from_stream( string, size, stream )

#define system_string_to_signed_long_long( string, end_of_string, base ) \
	wide_string_to_signed_long_long( string, end_of_string, base )

#define system_string_to_unsigned_long_long( string, end_of_string, base ) \
	wide_string_to_unsigned_long_long( string, end_of_string, base )

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

#define system_string_get_from_stream( string, size, stream ) \
	narrow_string_get_from_stream( string, size, stream )

#define system_string_to_signed_long_long( string, end_of_string, base ) \
	narrow_string_to_signed_long_long( string, end_of_string, base )

#define system_string_to_unsigned_long_long( string, end_of_string, base ) \
	narrow_string_to_unsigned_long_long( string, end_of_string, base )

#endif

#if defined( system_string_to_signed_long_long )
int64_t system_string_to_int64(
         const system_character_t *string,
         size_t size );
#endif

#if defined( system_string_to_unsigned_long_long )
uint64_t system_string_to_uint64(
          const system_character_t *string,
          size_t size );
#endif

#if defined( __cplusplus )
}
#endif

#endif

