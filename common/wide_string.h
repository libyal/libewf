/*
 * Wide character string functions
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

#if !defined( _WIDE_STRING_H )
#define _WIDE_STRING_H

#include "common.h"
#include "types.h"

/* Because wchar_t is defined in wchar.h
 * and its included in types.h
 * it's not included here
 */

#if defined( __cplusplus )
extern "C" {
#endif

/* String length
 */
#if defined( HAVE_WCSLEN )
#define wide_string_length( string ) \
	wcslen( string )
#endif

/* String compare
 */
#if defined( HAVE_WCSNCMP )
#define wide_string_compare( string1, string2, size ) \
	wcsncmp( string1, string2, size )

#elif defined( HAVE_WMEMCMP )
#define wide_string_compare( string1, string2, size ) \
	wmemcmp( (void *) string1, (void *) string2, size )

#elif defined( HAVE_WCSCMP )
#define wide_string_compare( string1, string2, size ) \
	wcscmp( string1, string2 )
#endif

/* Caseless string compare
 */
#if defined( WINAPI )
#define wide_string_compare_no_case( string1, string2, size ) \
	_wcsnicmp( string1, string2, size )

#elif defined( HAVE_WCSNCASECMP )
#define wide_string_compare_no_case( string1, string2, size ) \
	wcsncasecmp( string1, string2, size )

#elif defined( HAVE_WCSCASECMP )
#define wide_string_compare_no_case( string1, string2, size ) \
	wcscasecmp( string1, string2 )
#endif

/* String copy
 */
#if defined( HAVE_WCSNCPY )
#define wide_string_copy( destination, source, size ) \
	wcsncpy( destination, source, size )

#elif defined( HAVE_WMEMCPY )
#define wide_string_copy( destination, source, size ) \
	(wchar_t *) wmemcpy( (void *) destination, (void *) source, size )

#elif defined( HAVE_WCSCPY )
#define wide_string_copy( destination, source, size ) \
	wcscpy( destination, source )
#endif

/* String search
 */
#if defined( HAVE_WCSCHR )
#define wide_string_search( string, character, size ) \
	wcschr( string, (wchar_t) character )

#elif defined( HAVE_WMEMCHR )
#define wide_string_search( string, character, size ) \
	(wchar_t *) wmemchr( (void *) string, (wchar_t) character, size )
#endif

/* String reverse search
 */
#if defined( HAVE_WCSRCHR )
#define wide_string_search_reverse( string, character, size ) \
	wcsrchr( string, (wchar_t) character )

#elif defined( HAVE_WMEMRCHR )
#define wide_string_search_reverse( string, character, size ) \
	(wchar_t *) wmemrchr( (void *) string, (wchar_t) character, size )
#endif

/* String formatted print (snprinf)
 */
#if defined( WINAPI )
#define wide_string_snprintf( target, size, format, ... ) \
	swprintf_s( target, size, format, __VA_ARGS__ )

#elif defined( HAVE_SWPRINTF )
#define wide_string_snprintf( target, size, format, ... ) \
	swprintf( target, size, format, __VA_ARGS__ )
#endif

/* String retrieve from stream (fgets)
 */
#if defined( HAVE_FGETWS )
#define wide_string_get_from_stream( string, size, stream ) \
	fgetws( string, size, stream )
#endif

/* String to singed long long (int64)
 */
#if defined( WINAPI )
#define wide_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOLL )
#define wide_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) wcstoll( string, end_of_string, base )
#endif

/* String to unsinged long long (uint64)
 */
#if defined( WINAPI )
#define wide_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOULL )
#define wide_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) wcstoull( string, end_of_string, base )
#endif

#if defined( __cplusplus )
}
#endif

#endif

