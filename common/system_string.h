/*
 * System character string functions
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

#if !defined( _SYSTEM_STRING_H )
#define _SYSTEM_STRING_H

#include "common.h"

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include <libewf/types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
#define HAVE_WIDE_SYSTEM_CHARACTER_T 1
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

#if defined( HAVE_WCSLEN ) || defined( HAVE_WINDOWS_API )

#define system_string_length( string ) \
	wcslen( string )
#else
#error Missing wide character string length function (wcslen)
#endif

#if defined( HAVE_WMEMCMP )  || defined( HAVE_WINDOWS_API )
#define system_string_compare( string1, string2, size ) \
	wmemcmp( (void *) string1, (void *) string2, size )

#elif defined( HAVE_WCSNCMP )
#define system_string_compare( string1, string2, size ) \
	wcsncmp( string1, string2, size )

#elif defined( HAVE_WCSCMP )
#define system_string_compare( string1, string2, size ) \
	wcscmp( string1, string2 )

#else
#error Missing wide character string compare function (wmemcmp, wcsncmp and wcscmp)
#endif

#if defined( HAVE_WMEMCPY ) || defined( HAVE_WINDOWS_API )
#define system_string_copy( destination, source, size ) \
	(system_character_t *) wmemcpy( (void *) destination, (void *) source, size )

#elif defined( HAVE_WCSNCPY )
#define system_string_copy( destination, source, size ) \
	wcsncpy( destination, source, size )

#elif defined( HAVE_WCSCPY )
#define system_string_copy( destination, source, size ) \
	wcscpy( destination, source )

#else
#error Missing wide character string copy function (wmemcpy, wcsncpy and wcscpy)
#endif

#if defined( HAVE_WMEMCHR ) || defined( HAVE_WINDOWS_API )
#define system_string_search( string, character, size ) \
	(system_character_t *) wmemchr( (void *) string, (wchar_t) character, size )

#elif defined( HAVE_WCSCHR )
#define system_string_search( string, character, size ) \
	wcschr( string, (wchar_t) character )

#else
#error Missing wide character string search function (wmemchr and wcschr)
#endif

#if defined( HAVE_WINDOWS_API )
#define system_string_search_reverse( string, character, size ) \
	wcsrchr( string, (wchar_t) character )

#elif defined( HAVE_WMEMRCHR )
#define system_string_search_reverse( string, character, size ) \
	(system_character_t *) wmemrchr( (void *) string, (wchar_t) character, size )

#elif defined( HAVE_WCSRCHR )
#define system_string_search_reverse( string, character, size ) \
	wcsrchr( string, (wchar_t) character )

#else
#error Missing wide character reverse string search function (wmemrchr and wcsrchr)
#endif

#if defined( HAVE_WINDOWS_API )
#define system_string_snprintf( target, size, format, ... ) \
	swprintf_s( target, size, format, __VA_ARGS__ )

#elif defined( HAVE_SWPRINTF )
#define system_string_snprintf( target, size, format, ... ) \
	swprintf( target, size, format, __VA_ARGS__ )

#else
#error Missing swprintf
#endif

#if defined( HAVE_FGETWS ) || defined( HAVE_WINDOWS_API )
#define system_string_get_from_stream( string, size, stream ) \
	fgetws( string, size, stream )

#else
#error Missing wide character string get from stream function (fgetws)
#endif

#if defined( HAVE_WINDOWS_API )
#define system_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOLL )
#define system_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) wcstoll( string, end_of_string, base )

#else
#error Missing wide character string to signed long long (wcstoll)
#endif

#if defined( HAVE_WINDOWS_API )
#define system_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOULL )
#define system_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) wcstoull( string, end_of_string, base )

#else
#error Missing wide character string to unsigned long long (wcstoull)
#endif

#else

typedef char system_character_t;
typedef int system_integer_t;

#define PRIc_SYSTEM	"c"
#define PRIs_SYSTEM	"s"

#define _SYSTEM_CHARACTER_T_STRING( string ) \
	string

#if defined( HAVE_STRLEN ) || defined( HAVE_WINDOWS_API )
#define system_string_length( string ) \
	strlen( string )
#else
#error Missing string length function (strlen)
#endif

#if defined( HAVE_MEMCMP ) || defined( HAVE_WINDOWS_API )
#define system_string_compare( string1, string2, size ) \
	memcmp( (void *) string1, (void *) string2, size )

#elif defined( HAVE_STRNCMP )
#define system_string_compare( string1, string2, size ) \
	strncmp( string1, string2, size )

#elif defined( HAVE_STRCMP )
#define system_string_compare( string1, string2, size ) \
	strcmp( string1, string2 )

#else
#error Missing string compare function (memcmp, strncmp and strcmp)
#endif

#if defined( HAVE_MEMCPY ) || defined( HAVE_WINDOWS_API )
#define system_string_copy( destination, source, size ) \
	(system_character_t *) memcpy( (void *) destination, (void *) source, size )

#elif defined( HAVE_STRNCPY )
#define system_string_copy( destination, source, size ) \
	strncpy( destination, source, size )

#elif defined( HAVE_STRCPY )
#define system_string_copy( destination, source, size ) \
	strcpy( destination, source )

#else
#error Missing string copy function (memcpy, strncpy and strcpy)
#endif

#if defined( HAVE_MEMCHR ) || defined( HAVE_WINDOWS_API )
#define system_string_search( string, character, size ) \
	(system_character_t *) memchr( (void *) string, (int) character, size )

#elif defined( HAVE_STRCHR )
#define system_string_search( string, character, size ) \
	strchr( string, (int) character )

#else
#error Missing string search function (memchr and strchr)
#endif

#if defined( HAVE_WINDOWS_API )
#define system_string_search_reverse( string, character, size ) \
	strrchr( string, (int) character )

#elif defined( HAVE_MEMRCHR ) && ( HAVE_DECL_MEMRCHR_ == 1 )
#define system_string_search_reverse( string, character, size ) \
	(system_character_t *) memrchr( (void *) string, (int) character, size )

#elif defined( HAVE_STRRCHR )
#define system_string_search_reverse( string, character, size ) \
	strrchr( string, (int) character )

#else
#error Missing reverse string search function (memrchr and strrchr)
#endif

#if defined( HAVE_WINDOWS_API )
#define system_string_snprintf( target, size, format, ... ) \
	sprintf_s( target, size, format, __VA_ARGS__ )

#elif defined( HAVE_SNPRINTF )
#define system_string_snprintf( target, size, format, ... ) \
	snprintf( target, size, format, __VA_ARGS__ )

#elif defined( HAVE_SPRINTF )
#define system_string_snprintf( target, size, format, ... ) \
	sprintf( target, format, __VA_ARGS__ )

#else
#error Missing snprintf and sprintf
#endif

#if defined( HAVE_FGETS ) || defined( HAVE_WINDOWS_API )
#define system_string_get_from_stream( string, size, stream ) \
	fgets( string, size, stream )

#else
#error Missing string get from stream function (fgets)
#endif

#if defined( HAVE_WINDOWS_API )
#define system_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _atoi64( string )

#elif defined( HAVE_STRTOLL )
#define system_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) strtoll( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define system_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) atoll( string )

#else
#error Missing string to signed long long (strroll and atoll)
#endif

#if defined( HAVE_WINDOWS_API )
#define system_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _atoi64( string )

#elif defined( HAVE_STRTOULL )
#define system_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) strtoull( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define system_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) atoll( string )

#else
#error Missing string to unsigned long long (strroull and atoull)
#endif

#endif

system_character_t *libewf_system_string_duplicate(
                     system_character_t *string,
                     size_t size );

#define system_string_duplicate( string, size ) \
	libewf_system_string_duplicate( string, size )

int64_t libewf_system_string_to_int64(
         const system_character_t *string,
         size_t size );

#define system_string_to_int64( string, size ) \
	 libewf_system_string_to_int64( string, size )

uint64_t libewf_system_string_to_uint64(
          const system_character_t *string,
          size_t size );

#define system_string_to_uint64( string, size ) \
	 libewf_system_string_to_uint64( string, size )

#if defined( __cplusplus )
}
#endif

#endif

