/*
 * Character type string functions
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

#if !defined( _LIBEWF_STRING_H )
#define _LIBEWF_STRING_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "libewf_libuna.h"

#if defined( _cplusplus )
extern "C" {
#endif

/* The internal string type contains UTF-8
 */
typedef libuna_utf8_character_t libewf_character_t;

#define PRIc_LIBEWF	"c"
#define PRIs_LIBEWF	"s"

#define _LIBEWF_STRING( string ) \
	(libewf_character_t *) string

#define libewf_string_compare( string1, string2, size ) \
	narrow_string_compare( (char *) string1, (char *) string2, size )

#define libewf_string_copy( destination, source, size ) \
	narrow_string_copy( (char *) destination, (char *) source, size )

#define libewf_string_length( string ) \
	narrow_string_length( (char *) string )

#define libewf_string_search( string, character, size ) \
	(libewf_character_t *) narrow_string_search( (char *) string, (int) character, size )

#define libewf_string_search_reverse( string, character, size ) \
	(libewf_character_t *) narrow_string_search_reverse( (char *) string, (int) character, size )

#define libewf_string_snprintf( target, size, format, ... ) \
	narrow_string_snprintf( (char *) target, size, format, __VA_ARGS__ )

#define libewf_string_to_int64( string, end_of_string, base ) \
	narrow_string_to_signed_long_long( (char *) string, (char **) end_of_string, base )

#define libewf_string_to_uint64( string, end_of_string, base ) \
	narrow_string_to_unsigned_long_long( (char *) string, (char **) end_of_string, base )

/* Byte stream functions
 */
#define libewf_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error ) \
	libuna_utf8_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error )

#define libewf_string_copy_from_byte_stream( string, string_size, stream, stream_size, codepage, error ) \
	libuna_utf8_string_copy_from_byte_stream( string, string_size, stream, stream_size, codepage, error )

#define byte_stream_size_from_libewf_string( string, string_size, codepage, stream_size, error ) \
	libuna_byte_stream_size_from_utf8( string, string_size, codepage, stream_size, error )

#define byte_stream_copy_from_libewf_string( stream, stream_size, codepage, string, string_size, error ) \
	libuna_byte_stream_copy_from_utf8( stream, stream_size, codepage, string, string_size, error )

/* UTF-8 stream functions
 */
#define libewf_string_size_from_utf8_stream( stream, stream_size, string_size, error ) \
	libuna_utf8_string_size_from_utf8_stream( stream, stream_size, string_size, error )

#define libewf_string_copy_from_utf8_stream( string, string_size, stream, stream_size, error ) \
	libuna_utf8_string_copy_from_utf8_stream( string, string_size, stream, stream_size, error )

#define utf8_stream_size_from_libewf_string( string, string_size, stream_size, error ) \
	libuna_utf8_stream_size_from_utf8( string, string_size, stream_size, error )

#define utf8_stream_copy_from_libewf_string( stream, stream_size, string, string_size, error ) \
	libuna_utf8_stream_copy_from_utf8( stream, stream_size, string, string_size, error )

/* UTF-16 stream functions
 */
#define libewf_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error ) \
	libuna_utf8_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error )

#define libewf_string_copy_from_utf16_stream( string, string_size, stream, stream_size, byte_order, error ) \
	libuna_utf8_string_copy_from_utf16_stream( string, string_size, stream, stream_size, byte_order, error )

#define utf16_stream_size_from_libewf_string( string, string_size, stream_size, error ) \
	libuna_utf16_stream_size_from_utf8( string, string_size, stream_size, error )

#define utf16_stream_copy_from_libewf_string( stream, stream_size, byte_order, string, string_size, error ) \
	libuna_utf16_stream_copy_from_utf8( stream, stream_size, byte_order, string, string_size, error )

int libewf_string_split(
     libewf_character_t *string,
     size_t string_size,
     libewf_character_t delimiter,
     libewf_character_t ***split_values,
     size_t *amount_of_split_values,
     liberror_error_t **error );

int libewf_string_split_values_free(
     libewf_character_t **split_values,
     size_t amount_of_split_values,
     liberror_error_t **error );

#if defined( _cplusplus )
}
#endif

#endif

