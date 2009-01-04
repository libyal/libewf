/*
 * ewfstring
 * String functions for the libewf tools
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

#if !defined( _EWFSTRING_H )
#define _EWFSTRING_H

#include <common.h>

#include "../libewf/libewf_char.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The length of hash strings
 */
#define EWFSTRING_DIGEST_HASH_LENGTH_MD5	33
#define EWFSTRING_DIGEST_HASH_LENGTH_SHA1	41

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Have wide character support for the functions:
 * main (wmain)
 * open (wopen)
 */
#define CHAR_T			wchar_t
#define INT_T			wint_t

#define PRIc			"lc"
#define PRIs			"ls"

/* Intermediate version of the macro required for
 * correct evaluation predefined string
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

#if defined( HAVE_WINDOWS_API )
#define CHAR_T_TOLONG( string, end_of_string, base ) \
	(int64_t) _wtoi64( string )
#elif defined( HAVE_WCSTOLL )
#define CHAR_T_TOLONG( string, end_of_string, base ) \
	(int64_t) wcstoll( string, end_of_string, base )
#else
#error Missing wide character string to long (wcstoll)
#endif

#if defined( HAVE_WINDOWS_API )
#define CHAR_T_SNPRINTF( target, size, format, ... ) \
	swprintf_s( target, size, format, __VA_ARGS__ )
#elif defined( HAVE_SWPRINTF )
#define CHAR_T_SNPRINTF( target, size, format, ... ) \
	swprintf( target, size, format, __VA_ARGS__ )
#else
#error Missing wide character string print to stream (swprintf)
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

#if defined( HAVE_WINDOWS_API )
#define CHAR_T_TOLONG( string, end_of_string, base ) \
	(int64_t) _atoi64( string )
#elif defined( HAVE_STRTOLL )
#define CHAR_T_TOLONG( string, end_of_string, base ) \
	(int64_t) strtoll( string, end_of_string, base )
#elif defined( HAVE_ATOLL )
#define CHAR_T_TOLONG( string, end_of_string, base ) \
	(int64_t) atoll( string )
#else
#error Missing string to long long function (strtoll and atoll)
#endif

#if defined( HAVE_WINDOWS_API )
#define CHAR_T_SNPRINTF( target, size, format, ... ) \
        sprintf_s( target, size, format, __VA_ARGS__ )
#elif defined(HAVE_SNPRINTF)
#define CHAR_T_SNPRINTF( target, size, format, ... ) \
        snprintf( target, size, format, __VA_ARGS__ )
#elif defined(HAVE_SPRINTF)
#define CHAR_T_SNPRINTF( target, size, format, ... ) \
        sprintf( target, format, __VA_ARGS__ )
#else
#error Missing string print to stream (snprintf and sprintf)
#endif

#if defined( HAVE_FGETS )
#define CHAR_T_GET_FROM_STREAM( string, size, stream ) \
	fgets( string, size, stream )
#else
#error Missing string get from stream function (fgets)
#endif

#endif

char *ewfstring_strerror(
       int error_number );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
wchar_t *ewfstring_wide_strerror(
          int error_number );
#endif

int ewfstring_copy_libewf_char_from_char_t(
     libewf_char_t *destination,
     const CHAR_T *source,
     size_t length );

int ewfstring_copy_libewf_char_to_char_t(
     const libewf_char_t *source,
     CHAR_T *destination,
     size_t length );

#if defined( __cplusplus )
}
#endif

#endif

