/*
 * Character string functions
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

#include <common.h>
#include <narrow_string.h>
#include <memory.h>
#include <wide_string.h>
#include <types.h>

#if defined( HAVE_LOCALE_H )
#include <locale.h>
#endif

#if defined( HAVE_LANGINFO_H )
#include <langinfo.h>
#endif

#if defined( HAVE_LOCAL_LIBUNA )
#include <libuna_byte_stream.h>
#include <libuna_compare.h>
#include <libuna_error.h>
#include <libuna_unicode_character.h>
#include <libuna_utf8_stream.h>
#include <libuna_utf8_string.h>
#include <libuna_utf16_stream.h>
#include <libuna_utf16_string.h>
#include <libuna_utf32_stream.h>
#include <libuna_utf32_string.h>

#elif defined( HAVE_LIBUNA_H )
#include <libuna.h>
#endif

#include "notify.h"
#include "system_string.h"

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER_T )

int system_string_is_unicode     = 0;
int system_string_ascii_codepage = 0;

#endif

/* Initializes the system string values
 * Returns 1 if successful or -1 on error
 */
int system_string_initialize(
     liberror_error_t **error )
{
#if !defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
	static char *function = "system_string_initialize";
	char *locale          = NULL;
	char *charset         = NULL;
	size_t charset_length = 0;

#if !defined( HAVE_LANGINFO_CODESET )
	size_t locale_length  = 0;
#endif

	/* Determine the supported locales
	 */
	locale = setlocale(
	          LC_CTYPE,
	          "" );

	if( locale == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine locale.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LANGINFO_CODESET )
	charset = nl_langinfo(
	           CODESET );

	if( charset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine character set.",
		 function );

		return( -1 );
	}
	charset_length = narrow_string_length(
	                  charset );
#else
	locale_length = narrow_string_length(
	                 locale );

	charset = narrow_string_search(
	           locale,
	           '.',
	           locale_length + 1 );

	if( charset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine character set.",
		 function );

		return( -1 );
	}
	charset++;

	charset_length = locale_length - (size_t) ( charset - locale );
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	notify_verbose_printf(
	 "%s: charset: %s\n",
	 function,
	 charset );
#endif

	/* Determine codepage
	 */
	system_string_ascii_codepage = LIBUNA_CODEPAGE_ASCII;

	if( charset_length == 5 )
	{
		if( narrow_string_compare(
		     "UTF-8",
		     charset,
		     5 ) == 0 )
		{
			system_string_is_unicode = 1;
		}
	}
	else if( charset_length >= 4 )
	{
		if( narrow_string_compare(
		     "1250",
		     charset,
		     4 ) == 0 )
		{
			system_string_ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1250;
		}
		else if( narrow_string_compare(
		          "1251",
		          charset,
		          4 ) == 0 )
		{
			system_string_ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1251;
		}
		else if( narrow_string_compare(
		          "1252",
		          charset,
		          4 ) == 0 )
		{
			system_string_ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1252;
		}
		else if( narrow_string_compare(
		          "1253",
		          charset,
		          4 ) == 0 )
		{
			system_string_ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1253;
		}
		else if( narrow_string_compare(
		          "1254",
		          charset,
		          4 ) == 0 )
		{
			system_string_ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1254;
		}
		else if( narrow_string_compare(
		          "1255",
		          charset,
		          4 ) == 0 )
		{
			system_string_ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1255;
		}
		else if( narrow_string_compare(
		          "1256",
		          charset,
		          4 ) == 0 )
		{
			system_string_ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1256;
		}
		else if( narrow_string_compare(
		          "1257",
		          charset,
		          4 ) == 0 )
		{
			system_string_ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1257;
		}
		else if( narrow_string_compare(
		          "1258",
		          charset,
		          4 ) == 0 )
		{
			system_string_ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1258;
		}
		else if( narrow_string_compare(
		          "utf8",
		          charset,
		          4 ) == 0 )
		{
			system_string_is_unicode = 1;
		}
	}
#endif
	return( 1 );
}

#if defined( system_string_to_signed_long_long )

/* Determines the value represented by a string
 * Returns 1 if successful or -1 on error
 */
int system_string_to_int64(
     const system_character_t *string,
     size_t string_size,
     int64_t *value,
     liberror_error_t **error )
{
	system_character_t *end_of_string = NULL;
	static char *function             = "system_string_to_int64";

	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid string size s zero.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	end_of_string = (system_character_t *) &( string[ string_size - 1 ] );

	*value = system_string_to_signed_long_long(
	          string,
	          &end_of_string,
	          0 );

	if( *value == (int64_t) LONG_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine value.",
		 function );

		return( -1 );
	}
	return( 1 );
}
#endif

#if defined( system_string_to_unsigned_long_long )

/* Determines the value represented by a string
 * Returns 1 if successful or -1 on error
 */
int system_string_to_uint64(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value,
     liberror_error_t **error )
{
	system_character_t *end_of_string = NULL;
	static char *function             = "system_string_to_uint64";

	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid string size s zero.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	end_of_string = (system_character_t *) &( string[ string_size - 1 ] );

	*value = system_string_to_unsigned_long_long(
	          string,
	          &end_of_string,
	          0 );

	if( *value == (uint64_t) LONG_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine value.",
		 function );

		return( -1 );
	}
	return( 1 );
}
#endif

/* Determines the system string size from the UTF-8 string
 * Returns 1 if successful or -1 on error
 */
int system_string_size_from_utf8_string(
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *string_size,
     liberror_error_t **error )
{
	static char *function = "system_string_size_from_utf8_string";

	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#if SIZEOF_WCHAR_T == 4
	if( libuna_utf32_string_size_from_utf8(
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_size,
	     string_size,
	     error ) != 1 )
#elif SIZEOF_WCHAR_T == 2
	if( libuna_utf16_string_size_from_utf8(
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_size,
	     string_size,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine string size.",
		 function );

		return( -1 );
	}
#else
	if( system_string_is_unicode != 0 )
	{
		*string_size = 1 + narrow_string_length(
		                    (char *) utf8_string );
	}
	else if( libuna_byte_stream_size_from_utf8(
	          (libuna_utf8_character_t *) utf8_string,
	          utf8_string_size,
	          system_string_ascii_codepage,
	          string_size,
	          error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine string size.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Copies the system string size from the UTF-8 string
 * Returns 1 if successful or -1 on error
 */
int system_string_copy_from_utf8_string(
     system_character_t *string,
     size_t string_size,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	static char *function = "system_string_copy_from_utf8_string";

	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#if SIZEOF_WCHAR_T == 4
	if( libuna_utf32_string_copy_from_utf8(
	     (libuna_utf32_character_t *) string,
	     string_size,
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_size,
	     error ) != 1 )
#elif SIZEOF_WCHAR_T == 2
	if( libuna_utf16_string_copy_from_utf8(
	     (libuna_utf16_character_t *) string,
	     string_size,
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_size,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_is_unicode != 0 )
	{
		if( string_size < utf8_string_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: string too small.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     string,
		     utf8_string,
		     utf8_string_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set string.",
			 function );

			return( -1 );
		}
	}
	else if( libuna_byte_stream_copy_from_utf8(
	          (uint8_t *) string,
	          string_size,
	          system_string_ascii_codepage,
	          (libuna_utf8_character_t *) utf8_string,
	          utf8_string_size,
	          error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Copies and trims the system string size from the byte stream
 * Returns 1 if successful, 0 if the string is empty or -1 on error
 */
int system_string_trim_copy_from_byte_stream(
     system_character_t *string,
     size_t string_size,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     liberror_error_t **error )
{
	static char *function   = "system_string_trim_copy_from_byte_stream";
        ssize_t first_character = 0;
        ssize_t last_character  = 0;

	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( byte_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid byte stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	for( first_character = 0; first_character < (ssize_t) byte_stream_size; first_character++ )
	{
		if( ( byte_stream[ first_character ] >= (uint8_t) 0x21 )
		 && ( byte_stream[ first_character ] <= (uint8_t) 0x7e ) )
		{
			break;
		}
	}
	for( last_character = (ssize_t) byte_stream_size; last_character >= 0; last_character-- )
	{
		if( ( byte_stream[ last_character ] >= (uint8_t) 0x21 )
		 && ( byte_stream[ last_character ] <= (uint8_t) 0x7e ) )
		{
			break;
		}
	}
	if( last_character <= first_character )
	{
		return( 0 );
	}
	last_character  -= first_character;
	byte_stream_size = (size_t) ( last_character + 1 );
	byte_stream      = &( byte_stream[ first_character ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#if SIZEOF_WCHAR_T == 4
	if( libuna_utf32_string_copy_from_byte_stream(
	     (libuna_utf32_character_t *) string,
	     string_size,
	     byte_stream,
	     byte_stream_size,
	     error ) != 1 )
#elif SIZEOF_WCHAR_T == 2
	if( libuna_utf16_string_copy_from_utf8(
	     (libuna_utf16_character_t *) string,
	     string_size,
	     byte_stream,
	     byte_stream_size,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_is_unicode != 0 )
	{
		if( string_size < byte_stream_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: string too small.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     string,
		     byte_stream,
		     byte_stream_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set string.",
			 function );

			return( -1 );
		}
	}
	else if( libuna_byte_stream_copy_from_utf8(
	          (uint8_t *) string,
	          string_size,
	          system_string_ascii_codepage,
	          byte_stream,
	          byte_stream_size,
	          error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Determines the UTF-8 string size from the system string
 * Returns 1 if successful or -1 on error
 */
int utf8_string_size_from_system_string(
     const system_character_t *string,
     size_t string_size,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	static char *function = "utf8_string_size_from_system_string";

	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#if SIZEOF_WCHAR_T == 4
	if( libuna_utf8_string_size_from_utf32(
	     (libuna_utf32_character_t *) string,
	     string_size,
	     utf8_string_size,
	     error ) != 1 )
#elif SIZEOF_WCHAR_T == 2
	if( libuna_utf8_string_size_from_utf16(
	     (libuna_utf16_character_t *) string,
	     string_size,
	     utf8_string_size,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 string size.",
		 function );

		return( -1 );
	}
#else
	if( system_string_is_unicode != 0 )
	{
		*utf8_string_size = 1 + system_string_length(
		                         string );
	}
	else if( libuna_utf8_string_size_from_byte_stream(
	          (uint8_t *) string,
	          string_size,
	          system_string_ascii_codepage,
	          utf8_string_size,
	          error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 string size.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Copies the UTF-8 string size from the system string
 * Returns 1 if successful or -1 on error
 */
int utf8_string_copy_from_system_string(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     const system_character_t *string,
     size_t string_size,
     liberror_error_t **error )
{
	static char *function = "utf8_string_copy_from_system_string";

	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#if SIZEOF_WCHAR_T == 4
	if( libuna_utf8_string_copy_from_utf32(
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_size,
	     (libuna_utf32_character_t *) string,
	     string_size,
	     error ) != 1 )
#elif SIZEOF_WCHAR_T == 2
	if( libuna_utf8_string_copy_from_utf16(
	     (libuna_utf8_character_t *) utf8_string,
	     utf8_string_size,
	     (libuna_utf16_character_t *) string,
	     string_size,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_is_unicode != 0 )
	{
		if( utf8_string_size < string_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-8 string too small.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     utf8_string,
		     string,
		     string_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set string.",
			 function );

			return( -1 );
		}
	}
	else if( libuna_utf8_string_copy_from_byte_stream(
	          (libuna_utf8_character_t *) utf8_string,
	          utf8_string_size,
	          (uint8_t *) string,
	          string_size,
	          system_string_ascii_codepage,
	          error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Split a string into elements using a delimiter character
 * Returns 1 if successful or -1 on error
 */
int system_string_split(
     system_character_t *string,
     size_t string_size,
     system_character_t delimiter,
     system_character_t ***split_values,
     size_t *amount_of_split_values,
     liberror_error_t **error )
{
	system_character_t *split_value_start = NULL;
	system_character_t *split_value_end   = NULL;
	system_character_t *string_end        = NULL;
	static char *function                 = "system_string_split";
	size_t remaining_string_size          = 0;
	size_t split_value_iterator           = 0;
	ssize_t split_value_size              = 0;

	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split values.",
		 function );

		return( -1 );
	}
	if( *split_values != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: split values already set.",
		 function );

		return( -1 );
	}
	if( amount_of_split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of split values.",
		 function );

		return( -1 );
	}
	/* Include the last character if not the end of string character
	 */
	if( string[ string_size - 1 ] != 0 )
	{
		string_size += 1;
	}
	/* Do not bother with empty strings
	 */
	if( ( string_size == 0 )
	 || ( string[ 0 ] == 0 ) )
	{
		*split_values           = NULL;
		*amount_of_split_values = 0;

		return( 1 );
	}
	/* Determine the amount of split values
	 */
	remaining_string_size = string_size;
	split_value_start     = string;
	split_value_end       = string;
	string_end            = &string[ string_size - 1 ];

	do
	{
		split_value_end = system_string_search(
		                   split_value_start,
		                   delimiter,
		                   remaining_string_size );

		if( split_value_end > string_end )
		{
			break;
		}
		split_value_iterator++;

		if( split_value_end == NULL )
		{
			break;
		}
		/* Include delimiter character
		 */
		remaining_string_size -= (size_t) ( split_value_end - split_value_start ) + 1;

		if( split_value_end == split_value_start )
		{
			split_value_start += 1;
		}
		else if( split_value_end != string )
		{
			split_value_start = split_value_end + 1;
		}
	}
	while( split_value_end != NULL );

	*amount_of_split_values = split_value_iterator;

	if( *amount_of_split_values == 0 )
	{
		*split_values = NULL;

		return( 1 );
	}
	*split_values = (system_character_t **) memory_allocate(
	                                         ( sizeof( system_character_t * ) * *amount_of_split_values ) );

	if( *split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unable to create split values.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     *split_values,
	     0,
	     sizeof( system_character_t * ) * *amount_of_split_values ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unable to clear split values.",
		 function );

		memory_free(
		 *split_values );

		*split_values = NULL;

		return( -1 );
	}
	/* Determine the split values
	 */
	remaining_string_size = string_size;
	split_value_start     = string;
	split_value_end       = string;

	/* Empty values are stored as strings only containing the end of character
	 */
	for( split_value_iterator = 0; split_value_iterator < *amount_of_split_values; split_value_iterator++ )
	{
		if( split_value_end != string )
		{
			split_value_start = split_value_end + 1;
		}
		split_value_end = system_string_search(
		                   split_value_start,
		                   delimiter,
		                   remaining_string_size );

		/* Check for last value
		 */
		if( split_value_end == NULL )
		{
			split_value_size = (ssize_t) ( string_end - split_value_start );
		}
		else
		{
			split_value_size = (ssize_t) ( split_value_end - split_value_start );
		}
		if( split_value_size >= 0 )
		{
			/* Add 1 additional byte required for the end of string character
			 */
			split_value_size += 1;

			( *split_values )[ split_value_iterator ] = (system_character_t *) memory_allocate(
										            sizeof( system_character_t ) * split_value_size );

			if( ( *split_values )[ split_value_iterator ] == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
				 "%s: unable to create split value: %" PRIzd ".",
				 function,
				 split_value_iterator );

				system_string_split_values_free(
				 *split_values,
				 ( split_value_iterator - 1 ),
				 NULL );

				*split_values = NULL;

				return( -1 );
			}
			if( system_string_copy(
			     ( *split_values )[ split_value_iterator ],
			     split_value_start,
			     split_value_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
				 "%s: unable to set split value: %" PRIzd ".",
				 function,
				 split_value_iterator );

				system_string_split_values_free(
				 *split_values,
				 split_value_iterator,
				 NULL );

				*split_values = NULL;

				return( -1 );
			}
			( *split_values )[ split_value_iterator ][ split_value_size - 1 ] = 0;
		}
		/* Include delimiter character
		 */
		remaining_string_size -= (size_t) ( split_value_end - split_value_start ) + 1;

		/* Correct if first value is empty
		 */
		if( split_value_end == string )
		{
			split_value_start += 1;
		}
	}
	return( 1 );
}

/* Clears a split values array
 * Returns 1 if successful or -1 on error
 */
int system_string_split_values_free(
     system_character_t **split_values,
     size_t amount_of_split_values,
     liberror_error_t **error )
{
	static char *function       = "system_string_split_values_free";
	size_t split_value_iterator = 0;

	if( split_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split values array.",
		 function );

		return( -1 );
	}
	for( split_value_iterator = 0; split_value_iterator < amount_of_split_values; split_value_iterator++ )
	{
		if( split_values[ split_value_iterator ] != NULL )
		{
			memory_free(
			 split_values[ split_value_iterator ] );
		}
	}
	memory_free(
	 split_values );

	return( 1 );
}

