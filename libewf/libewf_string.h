/*
 * libewf character type string functions
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

#ifndef _LIBEWF_STRING_H
#define _LIBEWF_STRING_H

#include "libewf_includes.h"
#include "libewf_char.h"

#include "ewf_digest_hash.h"
#include "ewf_header.h"
#include "ewf_header2.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )

#if defined( HAVE_WCSLEN )
#define libewf_string_length( string ) \
	wcslen( string )
#else
#error Missing wide character string length function (wcslen)
#endif

#if defined( HAVE_WMEMCMP )
#define libewf_string_compare( string1, string2, size ) \
	wmemcmp( (void *) string1, (void *) string2, size )
#elif defined( HAVE_WCSNCMP )
#define libewf_string_compare( string1, string2, size ) \
	wcsncmp( string1, string2, size )
#elif defined( HAVE_WCSCMP )
#define libewf_string_compare( string1, string2, size ) \
	wcscmp( string1, string2 )
#else
#error Missing wide character string compare function (wmemcmp, wcsncmp and wcscmp)
#endif

#if defined( HAVE_WMEMCPY )
#define libewf_string_copy( destination, source, size ) \
	(LIBEWF_CHAR *) wmemcpy( (void *) destination, (void *) source, size )
#elif defined( HAVE_WCSNCPY )
#define libewf_string_copy( destination, source, size ) \
	wcsncpy( destination, source, size )
#elif defined( HAVE_WCSCPY )
#define libewf_string_copy( destination, source, size ) \
	wcscpy( destination, source )
#else
#error Missing wide character string copy function (wmemcpy, wcsncpy and wcscpy)
#endif

#if defined( HAVE_WMEMCHR )
#define libewf_string_search( string, character, size ) \
	(LIBEWF_CHAR *) wmemchr( (void *) string, (wchar_t) character, size )
#elif defined( HAVE_WCSCHR )
#define libewf_string_search( string, character, size ) \
	wcschr( string, (wchar_t) character )
#else
#error Missing wide character string search function (wmemchr and wcschr)
#endif

#if defined( HAVE_WMEMRCHR )
#define libewf_string_search_reverse( string, character, size ) \
	(LIBEWF_CHAR *) wmemrchr( (void *) string, (wchar_t) character, size )
#elif defined( HAVE_WCSRCHR )
#define libewf_string_search_reverse( string, character, size ) \
	wcsrchr( string, (wchar_t) character )
#else
#error Missing wide character reverse string search function (wmemrchr and wcsrchr)
#endif

#if defined( HAVE_WINDOWS_API )
#define libewf_string_snprintf( target, size, format, ... ) \
	swprintf_s( target, size, format, __VA_ARGS__ )
#elif defined( HAVE_SWPRINTF )
#define libewf_string_snprintf( target, size, format, ... ) \
	swprintf( target, size, format, __VA_ARGS__ )
#else
#error Missing swprintf
#endif

#if defined( HAVE_FGETWS )
#define libewf_string_get_from_stream( string, size, stream ) \
	fgetws( string, size, stream )
#else
#error Missing wide character string get from stream function (fgetws)
#endif

#else

#if defined( HAVE_STRLEN )
#define libewf_string_length( string ) \
	strlen( string )
#else
#error Missing string length function (strlen)
#endif

#if defined( HAVE_MEMCMP )
#define libewf_string_compare( string1, string2, size ) \
	memcmp( (void *) string1, (void *) string2, size )
#elif defined( HAVE_STRNCMP )
#define libewf_string_compare( string1, string2, size ) \
	strncmp( string1, string2, size )
#elif defined( HAVE_STRCMP )
#define libewf_string_compare( string1, string2, size ) \
	strcmp( string1, string2 )
#else
#error Missing string compare function (memcmp, strncmp and strcmp)
#endif

#if defined( HAVE_MEMCPY )
#define libewf_string_copy( destination, source, size ) \
	(LIBEWF_CHAR *) memcpy( (void *) destination, (void *) source, size )
#elif defined( HAVE_STRNCPY )
#define libewf_string_copy( destination, source, size ) \
	strncpy( destination, source, size )
#elif defined( HAVE_STRCPY )
#define libewf_string_copy( destination, source, size ) \
	strcpy( destination, source )
#else
#error Missing string copy function (memcpy, strncpy and strcpy)
#endif

#if defined( HAVE_MEMCHR )
#define libewf_string_search( string, character, size ) \
	(LIBEWF_CHAR *) memchr( (void *) string, (int) character, size )
#elif defined( HAVE_STRCHR )
#define libewf_string_search( string, character, size ) \
	strchr( string, (int) character )
#else
#error Missing string search function (memchr and strchr)
#endif

#if defined( HAVE_MEMRCHR ) && HAVE_DECL_MEMRCHR_ == 1
#define libewf_string_search_reverse( string, character, size ) \
	(LIBEWF_CHAR *) memrchr( (void *) string, (int) character, size )
#elif defined( HAVE_STRRCHR )
#define libewf_string_search_reverse( string, character, size ) \
	strrchr( string, (int) character )
#else
#error Missing reverse string search function (memrchr and strrchr)
#endif

#if defined( HAVE_WINDOWS_API )
#define libewf_string_snprintf( target, size, format, ... ) \
	sprintf_s( target, size, format, __VA_ARGS__ )
#elif defined(HAVE_SNPRINTF)
#define libewf_string_snprintf( target, size, format, ... ) \
	snprintf( target, size, format, __VA_ARGS__ )
#elif defined(HAVE_SPRINTF)
#define libewf_string_snprintf( target, size, format, ... ) \
	sprintf( target, format, __VA_ARGS__ )
#else
#error Missing snprintf and sprintf
#endif

#if defined( HAVE_FGETS )
#define libewf_string_get_from_stream( string, size, stream ) \
	fgets( string, size, stream )
#else
#error Missing string get from stream function (fgets)
#endif

#endif

typedef char libewf_string_t;

#define LIBEWF_STRING_LITTLE_ENDIAN	(uint8_t) 'l'
#define LIBEWF_STRING_BIG_ENDIAN	(uint8_t) 'b'

LIBEWF_CHAR *libewf_string_duplicate( LIBEWF_CHAR *string, size_t size );

int64_t libewf_string_to_int64( const LIBEWF_CHAR *string, size_t size );
uint64_t libewf_string_to_uint64( const LIBEWF_CHAR *string, size_t size );

LIBEWF_CHAR **libewf_string_split( LIBEWF_CHAR *string, size_t size, LIBEWF_CHAR delimiter, uint32_t *amount );
void libewf_string_split_values_free( LIBEWF_CHAR **split_values, uint32_t amount );

int8_t libewf_string_copy_utf16_to_ascii( LIBEWF_CHAR *utf16_string, size_t size_utf16, LIBEWF_CHAR* ascii_string, size_t size_ascii );
int8_t libewf_string_copy_ascii_to_utf16( LIBEWF_CHAR *ascii_string, size_t size_ascii, LIBEWF_CHAR *utf16_string, size_t size_utf16, uint8_t byte_order );

int8_t libewf_string_copy_from_ewf_char( LIBEWF_CHAR *string, size_t size_string, EWF_CHAR *ewf_char_string, size_t size_ewf_char_string );
int8_t libewf_string_copy_to_ewf_char( LIBEWF_CHAR *string, size_t size_string, EWF_CHAR *ewf_char_string, size_t size_ewf_char_string );

int8_t libewf_string_copy_from_digest_hash( LIBEWF_CHAR *string, size_t size_string, EWF_DIGEST_HASH *digest_hash, size_t size_digest_hash );

#define libewf_string_copy_from_header( string, size_string, header, size_header ) \
	libewf_string_copy_from_ewf_char( string, size_string, header, size_header )
#define libewf_string_copy_to_header( string, size_string, header, size_header ) \
	libewf_string_copy_to_ewf_char( string, size_string, header, size_header )

int8_t libewf_string_copy_from_header2( LIBEWF_CHAR *string, size_t size_string, EWF_HEADER2 *header2, size_t size_header2 );
int8_t libewf_string_copy_to_header2( LIBEWF_CHAR *string, size_t size_string, EWF_HEADER2 *header2, size_t size_header2 );

#ifdef __cplusplus
}
#endif

#endif

