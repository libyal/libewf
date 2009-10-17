/*
 * System character type string functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBEWF_SYSTEM_STRING_H )
#define _LIBEWF_SYSTEM_STRING_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libewf_libuna.h"

#if defined( _cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#define LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE
#endif

#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )

typedef wchar_t libewf_system_character_t;

#define PRIc_LIBEWF_SYSTEM	"lc"
#define PRIs_LIBEWF_SYSTEM	"ls"

/* Intermediate version of the macro required
 * for correct evaluation predefined string
 */
#define _LIBEWF_SYSTEM_STRING_INTERMEDIATE( string ) \
	L ## string

#define _LIBEWF_SYSTEM_STRING( string ) \
	_LIBEWF_SYSTEM_STRING_INTERMEDIATE( string )

#define libewf_system_string_copy( destination, source, size ) \
	wide_string_copy( destination, source, size )

#define libewf_system_string_length( string ) \
	wide_string_length( string )

/* The internal system string type contains UTF-32
 */
#if SIZEOF_WCHAR_T == 4

/* narrow string conversion functions
 */
#define libewf_system_string_size_from_narrow_string( narrow_string, narrow_string_size, system_string_size, error ) \
	libuna_utf32_string_size_from_utf8( (libuna_utf8_character_t *) narrow_string, narrow_string_size, system_string_size, error )

#define libewf_system_string_copy_from_narrow_string( system_string, system_string_size, narrow_string, narrow_string_size, error ) \
	libuna_utf32_string_copy_from_utf8( (libuna_utf32_character_t *) system_string, system_string_size, (libuna_utf8_character_t *) narrow_string, narrow_string_size, error )

#define narrow_string_size_from_libewf_system_string( system_string, system_string_size, narrow_string_size, error ) \
	libuna_utf8_string_size_from_utf32( (libuna_utf32_character_t *) system_string, system_string_size, narrow_string_size, error )

#define narrow_string_copy_from_libewf_system_string( narrow_string, narrow_string_size, system_string, system_string_size, error ) \
	libuna_utf8_string_copy_from_utf32( (libuna_utf8_character_t *) narrow_string, narrow_string_size, (libuna_utf32_character_t *) system_string, system_string_size, error )

/* The internal system string type contains UTF-16
 */
#elif SIZEOF_WCHAR_T == 2

/* narrow string conversion functions
 */
#define libewf_system_string_size_from_narrow_string( narrow_string, narrow_string_size, system_string_size, error ) \
	libuna_utf16_string_size_from_utf8( (libuna_utf8_character_t *) narrow_string, narrow_string_size, system_string_size, error )

#define libewf_system_string_copy_from_narrow_string( system_string, system_string_size, narrow_string, narrow_string_size, error ) \
	libuna_utf16_string_copy_from_utf8( (libuna_utf16_character_t *) system_string, system_string_size, (libuna_utf8_character_t *) narrow_string, narrow_string_size, error )

#define narrow_string_size_from_libewf_system_string( system_string, system_string_size, narrow_string_size, error ) \
	libuna_utf8_string_size_from_utf16( (libuna_utf16_character_t *) system_string, system_string_size, narrow_string_size, error )

#define narrow_string_copy_from_libewf_system_string( narrow_string, narrow_string_size, system_string, system_string_size, error ) \
	libuna_utf8_string_copy_from_utf16( (libuna_utf8_character_t *) narrow_string, narrow_string_size, (libuna_utf16_character_t *) system_string, system_string_size, error )

#else
#error Unsupported size of wchar_t
#endif

/* The internal system string type contains UTF-8
 */
#else

typedef char libewf_system_character_t;

#define PRIc_LIBEWF_SYSTEM	"c"
#define PRIs_LIBEWF_SYSTEM	"s"

#define _LIBEWF_SYSTEM_STRING( string ) \
	string

#define libewf_system_string_copy( destination, source, size ) \
	narrow_string_copy( destination, source, size )

#define libewf_system_string_length( string ) \
	narrow_string_length( string )

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* The wide character string type contains UTF-32
 */
#if SIZEOF_WCHAR_T == 4

/* wide string conversion functions
 */
#define libewf_system_string_size_from_wide_string( wide_string, wide_string_size, system_string_size, error ) \
	libuna_utf8_string_size_from_utf32( (libuna_utf32_character_t *) wide_string, wide_string_size, system_string_size, error )

#define libewf_system_string_copy_from_wide_string( system_string, system_string_size, wide_string, wide_string_size, error ) \
	libuna_utf8_string_copy_from_utf32( (libuna_utf8_character_t *) system_string, system_string_size, (libuna_utf32_character_t *) wide_string, wide_string_size, error )

#define wide_string_size_from_libewf_system_string( system_string, system_string_size, wide_string_size, error ) \
	libuna_utf32_string_size_from_utf8( (libuna_utf8_character_t *) system_string, system_string_size, wide_string_size, error )

#define wide_string_copy_from_libewf_system_string( wide_string, wide_string_size, system_string, system_string_size, error ) \
	libuna_utf32_string_copy_from_utf8( (libuna_utf32_character_t *) wide_string, wide_string_size, (libuna_utf8_character_t *) system_string, system_string_size, error )

/* The wide character string type contains UTF-16
 */
#elif SIZEOF_WCHAR_T == 2

/* wide string conversion functions
 */
#define libewf_system_string_size_from_wide_string( wide_string, wide_string_size, string_size, error ) \
	libuna_utf8_string_size_from_utf16( (libuna_utf16_character_t *) wide_string, wide_string_size, system_string_size, error )

#define libewf_system_string_copy_from_wide_string( system_string, system_string_size, wide_string, wide_string_size, error ) \
	libuna_utf8_string_copy_from_utf16( (libuna_utf8_character_t *) system_string, system_string_size, (libuna_utf16_character_t *) wide_string, wide_string_size, error )

#define wide_string_size_from_libewf_system_string( system_string, system_string_size, wide_string_size, error ) \
	libuna_utf16_string_size_from_utf8( (libuna_utf8_character_t *) system_string, system_string_size, wide_string_size, error )

#define wide_string_copy_from_libewf_system_string( wide_string, wide_string_size, system_string, system_string_size, error ) \
	libuna_utf16_string_copy_from_utf8( (libuna_utf16_character_t *) wide_string, wide_string_size, (libuna_utf8_character_t *) system_string, system_string_size, error )

#else
#error Unsupported size of wchar_t
#endif

#endif

#endif

#if defined( _cplusplus )
}
#endif

#endif

