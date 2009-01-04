/*
 * libewf character type string functions
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

#include "libewf_includes.h"

#include <errno.h>

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_notify.h"
#include "libewf_string.h"

#include "ewf_char.h"

/* Duplicates a string
 * Returns the pointer to the duplicate string, or NULL on error
 */
libewf_char_t *libewf_string_duplicate( libewf_char_t *string, size_t size )
{
	libewf_char_t *duplicate = NULL;
	static char *function    = "libewf_string_duplicate";

	if( string == NULL )
	{
		return( NULL );
	}
	if( size == 0 )
	{
		return( NULL );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	/* Add an additional character for the end of string
	 */
	size += 1;

	duplicate = (libewf_char_t *) libewf_common_alloc(
	                               sizeof( libewf_char_t ) * size );

	if( duplicate == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create duplicate string.\n",
		 function );

		return( NULL );
	}
	if( libewf_string_copy( duplicate, string, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set duplicate string.\n",
		 function );

		libewf_common_free( duplicate );

		return( NULL );
	}
	duplicate[ size - 1 ] = (libewf_char_t) '\0';

	return( duplicate );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

#if defined( HAVE_WINDOWS_API )
#define libewf_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOLL )
#define libewf_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) wcstoll( string, end_of_string, base )
#endif

#else

#if defined( HAVE_WINDOWS_API )
#define libewf_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _atoi64( string )

#elif defined( HAVE_STRTOLL )
#define libewf_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) strtoll( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define libewf_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) atoll( string )
#endif

#endif

/* Returns the value represented by a string, returns 0 and sets errno on error
 */
int64_t libewf_string_to_int64( const libewf_char_t *string, size_t size )
{
	libewf_char_t *end_of_string = NULL;
	static char *function        = "libewf_string_to_int64";
	int64_t value                = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string.\n",
		 function );

		errno = EINVAL;

		return( 0 );
	}
	if( size == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: string is emtpy.\n",
		 function );

		errno = EINVAL;

		return( 0 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		errno = EINVAL;

		return( 0 );
	}
	end_of_string = (libewf_char_t *) &string[ size - 1 ];

#if defined( libewf_string_to_signed_long_long )
	value = libewf_string_to_signed_long_long( string, &end_of_string, 0 );
#else
#error Missing equivalent of strtoll
#endif

	if( value == (int64_t) LONG_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

#if defined( HAVE_WINDOWS_API )
#define libewf_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOULL )
#define libewf_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) wcstoull( string, end_of_string, base )
#endif

#else

#if defined( HAVE_WINDOWS_API )
#define libewf_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _atoi64( string )

#elif defined( HAVE_STRTOULL )
#define libewf_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) strtoull( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define libewf_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) atoll( string )
#endif

#endif

/* Returns the value represented by a string, returns 0 and sets errno on error
 */
uint64_t libewf_string_to_uint64( const libewf_char_t *string, size_t size )
{
	libewf_char_t *end_of_string = NULL;
	static char *function        = "libewf_string_to_uint64";
	uint64_t value               = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string.\n",
		 function );

		errno = EINVAL;

		return( 0 );
	}
	if( size == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: string is emtpy.\n",
		 function );

		errno = EINVAL;

		return( 0 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		errno = EINVAL;

		return( 0 );
	}
	end_of_string = (libewf_char_t *) &string[ size - 1 ];

#if defined( libewf_string_to_unsigned_long_long )
	value = libewf_string_to_unsigned_long_long( string, &end_of_string, 0 );
#else
#error Missing equivalent of strtoull
#endif

	if( value == (uint64_t) LONG_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}

/* Split a string into elements using a delimiter character
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t **libewf_string_split( libewf_char_t *string, size_t size, libewf_char_t delimiter, uint32_t *amount )
{
	libewf_char_t **lines     = NULL;
	libewf_char_t *line_start = NULL;
	libewf_char_t *line_end   = NULL;
	libewf_char_t *string_end = NULL;
	static char *function     = "libewf_string_split";
	size_t size_string        = 0;
	size_t line_size          = 0;
	uint32_t iterator         = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string.\n",
		 function );

		return( NULL );
	}
	if( amount == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount.\n",
		 function );

		return( NULL );
	}
	if( size == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: string is empty.\n",
		 function );

		return( NULL );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	size_string = size;
	line_start  = string;
	line_end    = string;
	string_end  = &string[ size ];

	do
	{
		line_end = (libewf_char_t *) libewf_string_search( line_start, delimiter, size_string );

		iterator++;

		if( line_end == NULL )
		{
			break;
		}
		/* Include delimiter character
		 */
		size_string -= (size_t) ( line_end - line_start ) + 1;

		if( line_end == line_start )
		{
			line_start += 1;
		}
		else if( line_end != string )
		{
			line_start = line_end + 1;
		}
	}
	while( line_end != NULL );

	*amount = iterator;
	lines   = (libewf_char_t **) libewf_common_alloc( ( sizeof( libewf_char_t * ) * *amount ) );

	if( lines == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate dynamic array lines.\n",
		 function );

		return( NULL );
	}
	if( libewf_common_memset( lines, 0, *amount ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear dynamic array lines.\n",
		 function );

		return( NULL );
	}
	size_string = size;
	line_start  = string;
	line_end    = string;

	for( iterator = 0; iterator < *amount; iterator++ )
	{
		if( line_end != string )
		{
			line_start = line_end + 1;
		}
		line_end = (libewf_char_t *) libewf_string_search( line_start, delimiter, size_string );

		/* Check for last value
		 */
		if( line_end == NULL )
		{
			line_size = (size_t) ( string_end - line_start );
		}
		else
		{
			line_size = (size_t) ( line_end - line_start );
		}
		/* Add 1 additional byte required for the end of string character
		 */
		line_size += 1;

		lines[ iterator ] = (libewf_char_t *) libewf_common_alloc(
                                                       sizeof( libewf_char_t ) * line_size );

		if( lines[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocate line string.\n",
			 function );

			libewf_string_split_values_free( lines, ( iterator - 1 ) );

			return( NULL );
		}
		if( libewf_string_copy( lines[ iterator ], line_start, line_size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set dynamic array lines.\n",
			 function );

			libewf_string_split_values_free( lines, iterator );

			return( NULL );
		}
		lines[ iterator ][ line_size - 1 ] = (libewf_char_t) '\0';

		/* Include delimiter character
		 */
		size_string -= (size_t) ( line_end - line_start ) + 1;

		/* Correct if first value is empty
		 */
		if( line_end == string )
		{
			line_start += 1;
		}
	}
	return( lines );
}

/* Clears a split values array
 */
void libewf_string_split_values_free( libewf_char_t **split_values, uint32_t amount )
{
	static char *function = "libewf_string_split_values_free";
	uint32_t iterator     = 0;

	if( split_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid split values array.\n",
		 function );

		return;
	}
	for( iterator = 0; iterator < amount; iterator++ )
	{
		if( split_values[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: empty value.\n",
			 function );
		}
		else
		{
			libewf_common_free( split_values[ iterator ] );
		}
	}
	libewf_common_free( split_values );
}

/* Copies a multi byte UTF16 string to a single byte ASCII string
 * Returns 1 if successful, on -1 on error
 */
int libewf_string_copy_utf16_to_ascii( libewf_char_t *utf16_string, size_t size_utf16, libewf_char_t* ascii_string, size_t size_ascii )
{
	static char *function = "libewf_string_copy_utf16_to_ascii";
	size_t utf16_iterator = 2;
	size_t ascii_iterator = 0;
	uint8_t byte_order    = 0;

	if( utf16_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid UTF16 string.\n",
		 function );

		return( -1 );
	}
	if( ascii_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid ASCII string.\n",
		 function );

		return( -1 );
	}
	if( ( size_utf16 > (size_t) SSIZE_MAX )
	 || ( size_ascii > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* The UTF16 string contains twice as much bytes needed for the ASCII string
	 * with two additional bytes representing byte order
	 */
	if( size_ascii < ( ( size_utf16 - 2 ) / 2 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: ASCII string too small.\n",
		 function );

		return( -1 );
	}
	/* Check if UTF16 string is in big or little endian
	 */
	if( ( (uint8_t) utf16_string[ 0 ] == 0xff )
	 && ( (uint8_t) utf16_string[ 1 ] == 0xfe ) )
	{
		byte_order = LIBEWF_STRING_LITTLE_ENDIAN;
	}
	else if( ( (uint8_t) utf16_string[ 0 ] == 0xfe )
	 && ( (uint8_t) utf16_string[ 1 ] == 0xff ) )
	{
		byte_order = LIBEWF_STRING_BIG_ENDIAN;
	}
	else
	{
		LIBEWF_VERBOSE_PRINT( "%s: no byte order in UTF16 string.\n",
		 function );

		if( ( utf16_string[ 0 ] == (libewf_char_t) '\0' )
		 && ( utf16_string[ 1 ] != (libewf_char_t) '\0' ) )
		{
			byte_order = LIBEWF_STRING_LITTLE_ENDIAN;
		}
		else if( ( utf16_string[ 0 ] != (libewf_char_t) '\0' )
		 && ( utf16_string[ 1 ] == (libewf_char_t) '\0' ) )
		{
			byte_order = LIBEWF_STRING_LITTLE_ENDIAN;
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: unable to determine byte order in UTF16 string.\n",
			 function );

			return( -1 );
		}
		utf16_iterator = 0;
	}
	/* Convert string
	 */
	while( utf16_iterator < size_utf16 )
	{
		if( byte_order == LIBEWF_STRING_BIG_ENDIAN )
		{
			if( utf16_string[ utf16_iterator ] == (libewf_char_t) '\0' )
			{
				ascii_string[ ascii_iterator ] = utf16_string[ utf16_iterator + 1 ];
			}
			else
			{
				/* Add a place holder character
				 */
				ascii_string[ ascii_iterator ] = '_';
			}
		}
		else if( byte_order == LIBEWF_STRING_LITTLE_ENDIAN )
		{
			if( utf16_string[ utf16_iterator + 1 ] == (libewf_char_t) '\0' )
			{
				ascii_string[ ascii_iterator ] = utf16_string[ utf16_iterator ];
			}
			else
			{
				/* Add a place holder character
				 */
				ascii_string[ ascii_iterator ] = '_';
			}
		}
		utf16_iterator += 2;
		ascii_iterator += 1;
	}
	ascii_string[ size_ascii - 1 ] = (libewf_char_t) '\0';

	return( 1 );
}

/* Copies a single byte ASCII string to a multi byte UTF16 string
 * Returns 1 if successful, on -1 on error
 */
int libewf_string_copy_ascii_to_utf16( libewf_char_t *ascii_string, size_t size_ascii, libewf_char_t *utf16_string, size_t size_utf16, uint8_t byte_order )
{
	static char *function = "libewf_string_copy_ascii_to_utf16";
	size_t ascii_iterator = 0;
	size_t utf16_iterator = 2;

	if( ascii_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid ASCII string.\n",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid UTF16 string.\n",
		 function );

		return( -1 );
	}
	if( ( size_ascii > (size_t) SSIZE_MAX )
	 || ( size_utf16 > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* Two additional bytes required for the byte order indicator
	 */
	if( size_utf16 < ( ( size_ascii * 2 ) + 2 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: UTF16 string too small.\n",
		 function );

		return( -1 );
	}
	/* Add the endian byte order
	 */
	if( byte_order == LIBEWF_STRING_LITTLE_ENDIAN )
	{
		utf16_string[ 0 ] = 0xff;
		utf16_string[ 1 ] = 0xfe;
	}
	else if( byte_order == LIBEWF_STRING_BIG_ENDIAN )
	{
		utf16_string[ 0 ] = 0xfe;
		utf16_string[ 1 ] = 0xff;
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: undefined byte order.\n",
		 function );

		return( -1 );
	}
	/* Convert the string
	 */
	while( ascii_iterator < size_ascii )
	{
		if( byte_order == LIBEWF_STRING_LITTLE_ENDIAN )
		{
			utf16_string[ utf16_iterator     ] = ascii_string[ ascii_iterator ];
			utf16_string[ utf16_iterator + 1 ] = (libewf_char_t) '\0';
		}
		else if( byte_order == LIBEWF_STRING_BIG_ENDIAN )
		{
			utf16_string[ utf16_iterator     ] = (libewf_char_t) '\0';
			utf16_string[ utf16_iterator + 1 ] = ascii_string[ ascii_iterator ];
		}
		ascii_iterator += 1;
		utf16_iterator += 2;
	}
	utf16_string[ size_utf16 - 2 ] = (libewf_char_t) '\0';
	utf16_string[ size_utf16 - 1 ] = (libewf_char_t) '\0';

	return( 1 );
}

/* Converts an EWF character string to a LIBEWF character string
 * Returns 1 if successful, 0 if string was not set, or -1 on error
 */
int libewf_string_copy_from_ewf_char( libewf_char_t *string, size_t size_string, ewf_char_t *ewf_char_string, size_t size_ewf_char_string )
{
	static char *function = "libewf_string_copy_from_ewf_char";
	size_t iterator       = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( ewf_char_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid EWF character string.\n",
		 function );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX )
	 || ( size_ewf_char_string > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( size_string < size_ewf_char_string )
	{
		LIBEWF_WARNING_PRINT( "%s: string too small.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < size_ewf_char_string; iterator++ )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE )
		string[ iterator ] = btowc( (int) ewf_char_string[ iterator ] );
#else
		string[ iterator ] = (char) ewf_char_string[ iterator ];
#endif
	}
	string[ size_ewf_char_string - 1 ] = (libewf_char_t) '\0';

	return( 1 );
}

/* Converts a LIBEWF character string to an EWF character string
 * Returns 1 if successful, 0 if string was not set, or -1 on error
 */
int libewf_string_copy_to_ewf_char( libewf_char_t *string, size_t size_string, ewf_char_t *ewf_char_string, size_t size_ewf_char_string )
{
	static char *function = "libewf_string_copy_to_ewf_char";
	size_t iterator       = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( ewf_char_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid EWF character string.\n",
		 function );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX )
	 || ( size_ewf_char_string > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( size_ewf_char_string < size_string )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF character string too small.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < size_string; iterator++ )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE )
		ewf_char_string[ iterator ] = (ewf_char_t) wctob( string[ iterator ] );

		/* If character is out of the basic ASCII range use '_' as a place holder
		 */
		if( ewf_char_string[ iterator ] == (ewf_char_t) EOF )
		{
			ewf_char_string[ iterator ] = (ewf_char_t) '_';
		}
#else
		ewf_char_string[ iterator ] = (ewf_char_t) string[ iterator ];
#endif
	}
	ewf_char_string[ size_string - 1 ] = (ewf_char_t) '\0';

	return( 1 );
}

/* Converts an EWF header2 to a string
 * Returns 1 if successful, 0 if string was not set, or -1 on error
 */
int libewf_string_copy_from_header2( libewf_char_t *string, size_t size_string, ewf_char_t *header2, size_t size_header2 )
{
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	mbstate_t conversion_state;

	char *header2_pointer = NULL;
#endif
	static char *function = "libewf_string_copy_from_header2";

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header2.\n",
		 function );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX )
	 || ( size_header2 > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( size_string < ( ( size_header2 - 2 ) / 2 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: string too small.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( libewf_common_memset(
	     &conversion_state,
	     0,
	     sizeof( mbstate_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear converion state.\n",
		 function );

		return( -1 );
	}
	if( mbsinit( &conversion_state ) == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to initialize converion state.\n",
		 function );

		return( -1 );
	}
	if( ( header2[ 0 ] == (ewf_char_t) 0xff )
	 || ( header2[ 0 ] == (ewf_char_t) 0xfe ) )
	{
		header2_pointer = (char *) &header2[ 2 ];
	}
	else
	{
		header2_pointer = (char *) &header2[ 0 ];
	}
	if( mbsrtowcs(
	     string,
	     (const char **) &header2_pointer,
	     size_string,
	     &conversion_state ) != ( size_string - 1 ) )
#else
	if( libewf_string_copy_utf16_to_ascii(
	     (libewf_char_t *) header2,
	     size_header2,
	     string,
	     size_string ) != 1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy header2 to string.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Converts a string to an EWF header2
 * Returns 1 if successful, 0 if header2 was not set, or -1 on error
 */
int libewf_string_copy_to_header2( libewf_char_t *string, size_t size_string, ewf_char_t *header2, size_t size_header2 )
{
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	mbstate_t conversion_state;
#endif
	static char *function = "libewf_string_copy_to_header2";

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header2.\n",
		 function );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX )
	 || ( size_header2 > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( size_header2 < ( ( size_string * 2 ) + 2 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: header2 too small.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( libewf_common_memset(
	     &conversion_state,
	     0,
	     sizeof( mbstate_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear converion state.\n",
		 function );

		return( -1 );
	}
	if( mbsinit( &conversion_state ) == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to initialize converion state.\n",
		 function );

		return( -1 );
	}
	if( wcsrtombs(
	     (char *) &header2[ 2 ],
	     (const wchar_t **) &string,
	     size_header2,
	     &conversion_state ) != ( size_string - 1 ) )
#else
	if( libewf_string_copy_ascii_to_utf16(
	     string,
	     size_string,
	     (libewf_char_t *) header2,
	     size_header2,
	     LIBEWF_STRING_LITTLE_ENDIAN ) != 1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy string to header2.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( header2[ 4 ] == (ewf_char_t) '\0' )
	{
		header2[ 0 ] = (ewf_char_t) 0xfe;
		header2[ 1 ] = (ewf_char_t) 0xff;
	}
	else
	{
		header2[ 0 ] = (ewf_char_t) 0xff;
		header2[ 1 ] = (ewf_char_t) 0xfe;
	}
#endif
	return( 1 );
}

/* Generate ctime string
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_string_ctime( const time_t *timestamp )
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && !defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
        char *narrow_ctime_string   = NULL;
#endif
        libewf_char_t *ctime_string = NULL;
        static char *function       = "libewf_string_ctime";

	if( timestamp == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid time stamp.\n",
		 function );

		return( NULL );
	}
        /* The libewf_common_ctime function returns a string of length 32
	 */
#if !defined( HAVE_WIDE_CHARACTER_TYPE )
        ctime_string = libewf_common_ctime( timestamp );

#elif defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
        ctime_string = libewf_common_wide_ctime( timestamp );
#else
        narrow_ctime_string = libewf_common_ctime( timestamp );

        if( narrow_ctime_string == NULL )
        {
                LIBEWF_WARNING_PRINT( "%s: unable to create narrow ctime string.\n",
                 function );

                return( NULL );
        }
        ctime_string = (libewf_char_t *) libewf_common_alloc(
                                          sizeof( libewf_char_t ) * 32 );

        if( ctime_string == NULL )
        {
                LIBEWF_WARNING_PRINT( "%s: unable to create ctime string.\n",
                 function );

		libewf_common_free( narrow_ctime_string );

                return( NULL );
        }
        if( libewf_common_copy_char_to_wchar( ctime_string, narrow_ctime_string, 32 ) != 1 )
        {
                LIBEWF_WARNING_PRINT( "%s: unable to copy narrow ctime string to ctime string.\n",
                 function );

		libewf_common_free( narrow_ctime_string );
		libewf_common_free( ctime_string );

                return( NULL );
        }
	libewf_common_free( narrow_ctime_string );
#endif
	return( ctime_string );
}

