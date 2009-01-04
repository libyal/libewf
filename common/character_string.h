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
#include "string_conversion.h"
#include "types.h"

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#define HAVE_WIDE_CHARACTER_T 1
#endif

#if defined( HAVE_WIDE_CHARACTER_T )

typedef wchar_t character_t;

#define PRIc	"lc"
#define PRIs	"ls"

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _CHARACTER_T_STRING_INTERMEDIATE( string ) \
	L ## string

#define _CHARACTER_T_STRING( string ) \
	_CHARACTER_T_STRING_INTERMEDIATE( string )

#if defined( HAVE_WCSLEN )

#define string_length( string ) \
	wcslen( string )
#else
#error Missing wide character string length function (wcslen)
#endif

#if defined( HAVE_WMEMCMP ) || defined( HAVE_WINDOWS_API )
#define string_compare( string1, string2, length ) \
	wmemcmp( (void *) string1, (void *) string2, length )

#elif defined( HAVE_WCSNCMP )
#define string_compare( string1, string2, length ) \
	wcsncmp( string1, string2, length )

#elif defined( HAVE_WCSCMP )
#define string_compare( string1, string2, length ) \
	wcscmp( string1, string2 )

#else
#error Missing wide character string compare function (wmemcmp, wcsncmp and wcscmp)
#endif

#if defined( HAVE_WMEMCPY ) || defined( HAVE_WINDOWS_API )
#define string_copy( destination, source, length ) \
	(character_t *) wmemcpy( (void *) destination, (void *) source, length )

#elif defined( HAVE_WCSNCPY )
#define string_copy( destination, source, length ) \
	wcsncpy( destination, source, length )

#elif defined( HAVE_WCSCPY )
#define string_copy( destination, source, length ) \
	wcscpy( destination, source )

#else
#error Missing wide character string copy function (wmemcpy, wcsncpy and wcscpy)
#endif

#if defined( HAVE_WMEMCHR ) || defined( HAVE_WINDOWS_API )
#define string_search( string, character, length ) \
	(character_t *) wmemchr( (void *) string, (wchar_t) character, length )

#elif defined( HAVE_WCSCHR )
#define string_search( string, character, length ) \
	wcschr( string, (wchar_t) character )

#else
#error Missing wide character string search function (wmemchr and wcschr)
#endif

#if defined( HAVE_WINDOWS_API )
#define string_search_reverse( string, character, length ) \
	wcsrchr( string, (wchar_t) character )

#elif defined( HAVE_WMEMRCHR )
#define string_search_reverse( string, character, length ) \
	(character_t *) wmemrchr( (void *) string, (wchar_t) character, length )

#elif defined( HAVE_WCSRCHR )
#define string_search_reverse( string, character, length ) \
	wcsrchr( string, (wchar_t) character )

#else
#error Missing wide character reverse string search function (wmemrchr and wcsrchr)
#endif

#if defined( HAVE_WINDOWS_API )
#define string_snprintf( target, length, format, ... ) \
	swprintf_s( target, length, format, __VA_ARGS__ )

#elif defined( HAVE_SWPRINTF )
#define string_snprintf( target, length, format, ... ) \
	swprintf( target, length, format, __VA_ARGS__ )

#else
#error Missing swprintf
#endif

#if defined( HAVE_FGETWS ) || defined( HAVE_WINDOWS_API )
#define string_get_from_stream( string, length, stream ) \
	fgetws( string, length, stream )

#else
#error Missing wide character string get from stream function (fgetws)
#endif

#if defined( HAVE_WINDOWS_API )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOLL )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) wcstoll( string, end_of_string, base )

#else
#error Missing wide character string to signed long long (wcstoll)
#endif

#if defined( HAVE_WINDOWS_API )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOULL )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) wcstoull( string, end_of_string, base )

#else
#error Missing wide character string to unsigned long long (wcstoull)
#endif

#else

typedef char character_t;

#define PRIc	"c"
#define PRIs	"s"

#define _CHARACTER_T_STRING( string ) \
	string

#if defined( HAVE_STRLEN ) || defined( HAVE_WINDOWS_API )
#define string_length( string ) \
	strlen( string )
#else
#error Missing string length function (strlen)
#endif

#if defined( HAVE_MEMCMP ) || defined( HAVE_WINDOWS_API )
#define string_compare( string1, string2, length ) \
	memcmp( (void *) string1, (void *) string2, length )

#elif defined( HAVE_STRNCMP )
#define string_compare( string1, string2, length ) \
	strncmp( string1, string2, length )

#elif defined( HAVE_STRCMP )
#define string_compare( string1, string2, length ) \
	strcmp( string1, string2 )

#else
#error Missing string compare function (memcmp, strncmp and strcmp)
#endif

#if defined( HAVE_MEMCPY ) || defined( HAVE_WINDOWS_API )
#define string_copy( destination, source, length ) \
	(character_t *) memcpy( (void *) destination, (void *) source, length )

#elif defined( HAVE_STRNCPY )
#define string_copy( destination, source, length ) \
	strncpy( destination, source, length )

#elif defined( HAVE_STRCPY )
#define string_copy( destination, source, length ) \
	strcpy( destination, source )

#else
#error Missing string copy function (memcpy, strncpy and strcpy)
#endif

#if defined( HAVE_MEMCHR ) || defined( HAVE_WINDOWS_API )
#define string_search( string, character, length ) \
	(character_t *) memchr( (void *) string, (int) character, length )

#elif defined( HAVE_STRCHR )
#define string_search( string, character, length ) \
	strchr( string, (int) character )

#else
#error Missing string search function (memchr and strchr)
#endif

#if defined( HAVE_WINDOWS_API )
#define string_search_reverse( string, character, length ) \
	strrchr( string, (int) character )

#elif defined( HAVE_MEMRCHR ) && ( HAVE_DECL_MEMRCHR_ == 1 )
#define string_search_reverse( string, character, length ) \
	(character_t *) memrchr( (void *) string, (int) character, length )

#elif defined( HAVE_STRRCHR )
#define string_search_reverse( string, character, length ) \
	strrchr( string, (int) character )

#else
#error Missing reverse string search function (memrchr and strrchr)
#endif

#if defined( HAVE_WINDOWS_API )
#define string_snprintf( target, length, format, ... ) \
	sprintf_s( target, length, format, __VA_ARGS__ )

#elif defined( HAVE_SNPRINTF )
#define string_snprintf( target, length, format, ... ) \
	snprintf( target, length, format, __VA_ARGS__ )

#elif defined( HAVE_SPRINTF )
#define string_snprintf( target, length, format, ... ) \
	sprintf( target, format, __VA_ARGS__ )

#else
#error Missing snprintf and sprintf
#endif

#if defined( HAVE_FGETS ) || defined( HAVE_WINDOWS_API )
#define string_get_from_stream( string, length, stream ) \
	fgets( string, length, stream )

#else
#error Missing string get from stream function (fgets)
#endif

#if defined( HAVE_WINDOWS_API )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _atoi64( string )

#elif defined( HAVE_STRTOLL )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) strtoll( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) atoll( string )

#else
#error Missing string to signed long long (strroll and atoll)
#endif

#if defined( HAVE_WINDOWS_API )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _atoi64( string )

#elif defined( HAVE_STRTOULL )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) strtoull( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) atoll( string )

#else
#error Missing string to unsigned long long (strroull and atoull)
#endif

#endif

character_t *libewf_string_duplicate(
              character_t *string,
              size_t length );

#define string_duplicate( string, length ) \
	libewf_string_duplicate( string, length )

int64_t libewf_string_to_int64(
         const character_t *string,
         size_t length );

#define string_to_int64( string, length ) \
	 libewf_string_to_int64( string, length )

uint64_t libewf_string_to_uint64(
          const character_t *string,
          size_t length );

#define string_to_uint64( string, length ) \
	 libewf_string_to_uint64( string, length )

#if defined( HAVE_WIDE_CHARACTER_T )
#define string_copy_from_char( destination, source, length ) \
	string_copy_char_to_wchar( destination, source, length )

#define string_copy_to_char( destination, source, length ) \
	string_copy_wchar_to_char( destination, source, length )

#else
#define string_copy_from_char( destination, source, length ) \
	( string_copy( destination, source, length ) == NULL ) ? -1 : 1

#define string_copy_to_char( destination, source, length ) \
	( string_copy( destination, source, length ) == NULL ) ? -1 : 1

#endif

#define string_copy_from_utf16_stream( string, length_string, stream, size_stream, byte_order ) \
	libewf_string_copy_from_utf16_stream( string, length_string, stream, size_stream, byte_order ) \

int libewf_string_copy_from_utf16_stream(
     character_t *string,
     size_t length_string,
     uint8_t *utf16_stream,
     size_t size_utf16_stream,
     uint8_t byte_order );

#define string_copy_to_utf16_stream( string, length_string, stream, size_stream, byte_order ) \
	libewf_string_copy_to_utf16_stream( string, length_string, stream, size_stream, byte_order ) \

int libewf_string_copy_to_utf16_stream(
     character_t *string,
     size_t length_string,
     uint8_t *utf16_stream,
     size_t size_utf16_stream,
     uint8_t byte_order );

#if defined( HAVE_WIDE_CHARACTER_T )
#if defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
#define string_ctime( timestamp, string, length ) \
	date_time_wctime( timestamp, string, length )

#else
character_t *libewf_string_ctime(
              const time_t *timestamp,
              character_t *string,
              size_t length );

#define string_ctime( timestamp, string, length ) \
	libewf_string_ctime( timestamp, string, length )

#endif
#else
#define string_ctime( timestamp, string, length ) \
	date_time_ctime( timestamp, string, length )

#endif

#if defined( __cplusplus )
}
#endif

#endif

