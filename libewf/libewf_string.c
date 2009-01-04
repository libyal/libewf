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
LIBEWF_CHAR *libewf_string_duplicate( LIBEWF_CHAR *string, size_t size )
{
	LIBEWF_CHAR *duplicate = NULL;

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
		LIBEWF_WARNING_PRINT( "libewf_string_duplicate: invalid size value exceeds maximum.\n" );

		return( NULL );
	}
	/* Add an additional character for the end of string
	 */
	size += 1;

	duplicate = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * size );

	if( duplicate == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_duplicate: unable to create duplicate string.\n" );

		return( NULL );
	}
	if( libewf_string_copy( duplicate, string, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_duplicate: unable to set duplicate string.\n" );

		libewf_common_free( duplicate );

		return( NULL );
	}
	duplicate[ size - 1 ] = (LIBEWF_CHAR) '\0';

	return( duplicate );
}

#ifdef HAVE_WIDE_CHARACTER_TYPE

#ifdef HAVE_WCSTOL
#define libewf_string_to_signed_long( string, end_of_string, base )	(int64_t) wcstol( string, end_of_string, base )
#endif

#else

#ifdef HAVE_STRTOL
#define libewf_string_to_signed_long( string, end_of_string, base )	(int64_t) strtol( string, end_of_string, base )
#elif defined(HAVE_ATOL)
#define libewf_string_to_signed_long( string, end_of_string, base )	(int64_t) atol( string )
#endif

#endif

/* Returns the value represented by a string, returns 0 and sets errno on error
 */
int64_t libewf_string_to_int64( const LIBEWF_CHAR *string, size_t size )
{
	LIBEWF_CHAR *end_of_string = NULL;
	int64_t value              = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_to_int64: invalid string.\n" );

		errno = EINVAL;

		return( 0 );
	}
	if( size == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_to_int64: string is emtpy.\n" );

		errno = EINVAL;

		return( 0 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_to_int64: invalid size value exceeds maximum.\n" );

		errno = EINVAL;

		return( 0 );
	}
	end_of_string = (LIBEWF_CHAR *) &string[ size - 1 ];

#if defined( libewf_string_to_signed_long )
	value = libewf_string_to_signed_long( string, &end_of_string, 0 );
#else
#error Missing equivalent of strtol
#endif

	if( value == (int64_t) LONG_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_to_int64: unable to convert string.\n" );

		return( 0 );
	}
	return( value );
}


#ifdef HAVE_WIDE_CHARACTER_TYPE

#ifdef HAVE_WCSTOUL
#define libewf_string_to_unsigned_long( string, end_of_string, base )	(uint64_t) wcstoul( string, end_of_string, base )
#endif

#else

#ifdef HAVE_STRTOUL
#define libewf_string_to_unsigned_long( string, end_of_string, base )	(uint64_t) strtoul( string, end_of_string, base )
#elif defined(HAVE_ATOL)
#define libewf_string_to_unsigned_long( string, end_of_string, base )	(uint64_t) atol( string )
#endif

#endif

/* Returns the value represented by a string, returns 0 and sets errno on error
 */
uint64_t libewf_string_to_uint64( const LIBEWF_CHAR *string, size_t size )
{
	LIBEWF_CHAR *end_of_string = NULL;
	uint64_t value             = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_to_uint64: invalid string.\n" );

		errno = EINVAL;

		return( 0 );
	}
	if( size == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_to_uint64: string is emtpy.\n" );

		errno = EINVAL;

		return( 0 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_to_uint64: invalid size value exceeds maximum.\n" );

		errno = EINVAL;

		return( 0 );
	}
	end_of_string = (LIBEWF_CHAR *) &string[ size - 1 ];

#if defined( libewf_string_to_unsigned_long )
	value = libewf_string_to_unsigned_long( string, &end_of_string, 0 );
#else
#error Missing equivalent of strtoul
#endif

	if( value == (uint64_t) LONG_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_to_uint64: unable to convert string.\n" );

		return( 0 );
	}
	return( value );
}

/* Split a string into elements using a delimiter character
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR **libewf_string_split( LIBEWF_CHAR *string, size_t size, LIBEWF_CHAR delimiter, uint32_t *amount )
{
	LIBEWF_CHAR **lines     = NULL;
	LIBEWF_CHAR *line_start = NULL;
	LIBEWF_CHAR *line_end   = NULL;
	LIBEWF_CHAR *string_end = NULL;
	size_t size_string      = 0;
	size_t line_size        = 0;
	uint32_t iterator       = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_split: invalid string.\n" );

		return( NULL );
	}
	if( amount == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_split: invalid amount.\n" );

		return( NULL );
	}
	if( size == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_split: string is empty.\n" );

		return( NULL );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_split: invalid size value exceeds maximum.\n" );

		return( NULL );
	}
	size_string = size;
	line_start  = string;
	line_end    = string;
	string_end  = &string[ size ];

	do
	{
		line_end = (LIBEWF_CHAR *) libewf_string_search( line_start, delimiter, size_string );

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
	lines   = (LIBEWF_CHAR **) libewf_common_alloc_cleared( ( sizeof( LIBEWF_CHAR * ) * *amount ), 0 );

	if( lines == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_split: unable to allocate dynamic array lines.\n" );

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
		line_end = (LIBEWF_CHAR *) libewf_string_search( line_start, delimiter, size_string );

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

		lines[ iterator ] = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * line_size );

		if( lines[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_string_split: unable to allocate line string.\n" );

			libewf_string_split_values_free( lines, ( iterator - 1 ) );

			return( NULL );
		}
		if( libewf_string_copy( lines[ iterator ], line_start, line_size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_string_split: unable to set dynamic array lines.\n" );

			libewf_string_split_values_free( lines, iterator );

			return( NULL );
		}
		lines[ iterator ][ line_size - 1 ] = (LIBEWF_CHAR) '\0';

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
void libewf_string_split_values_free( LIBEWF_CHAR **split_values, uint32_t amount )
{
	uint32_t iterator = 0;

	if( split_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_split_values_free: invalid split values array.\n" );

		return;
	}
	for( iterator = 0; iterator < amount; iterator++ )
	{
		if( split_values[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_string_split_values_free: empty value.\n" );
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
int8_t libewf_string_copy_utf16_to_ascii( LIBEWF_CHAR *utf16_string, size_t size_utf16, LIBEWF_CHAR* ascii_string, size_t size_ascii )
{
	size_t utf16_iterator = 2;
	size_t ascii_iterator = 0;
	uint8_t byte_order    = 0;

	if( utf16_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_utf16_to_ascii: invalid UTF16 string.\n" );

		return( -1 );
	}
	if( ascii_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_utf16_to_ascii: invalid ASCII string.\n" );

		return( -1 );
	}
	if( ( size_utf16 > (size_t) SSIZE_MAX ) || ( size_ascii > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_utf16_to_ascii: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	/* The UTF16 string contains twice as much bytes needed for the ASCII string
	 * with two additional bytes representing byte order
	 */
	if( size_ascii < ( ( size_utf16 - 2 ) / 2 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_utf16_to_ascii: ASCII string too small.\n" );

		return( -1 );
	}
	/* Check if UTF16 string is in big or little endian
	 */
	if( ( (uint8_t) utf16_string[ 0 ] == 0xff ) && ( (uint8_t) utf16_string[ 1 ] == 0xfe ) )
	{
		byte_order = LIBEWF_STRING_LITTLE_ENDIAN;
	}
	else if( ( (uint8_t) utf16_string[ 0 ] == 0xfe ) && ( (uint8_t) utf16_string[ 1 ] == 0xff ) )
	{
		byte_order = LIBEWF_STRING_BIG_ENDIAN;
	}
	else
	{
		LIBEWF_VERBOSE_PRINT( "libewf_string_copy_utf16_to_ascii: no byte order in UTF16 string.\n" );

		if( ( utf16_string[ 0 ] == (LIBEWF_CHAR) '\0' ) && ( utf16_string[ 1 ] != (LIBEWF_CHAR) '\0' ) )
		{
			byte_order = LIBEWF_STRING_LITTLE_ENDIAN;
		}
		else if( ( utf16_string[ 0 ] != (LIBEWF_CHAR) '\0' ) && ( utf16_string[ 1 ] == (LIBEWF_CHAR) '\0' ) )
		{
			byte_order = LIBEWF_STRING_LITTLE_ENDIAN;
		}
		else
		{
			LIBEWF_WARNING_PRINT( "libewf_string_copy_utf16_to_ascii: unable to determine byte order in UTF16 string.\n" );

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
			if( utf16_string[ utf16_iterator ] == (LIBEWF_CHAR) '\0' )
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
			if( utf16_string[ utf16_iterator + 1 ] == (LIBEWF_CHAR) '\0' )
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
	ascii_string[ size_ascii - 1 ] = (LIBEWF_CHAR) '\0';

	return( 1 );
}

/* Copies a single byte ASCII string to a multi byte UTF16 string
 * Returns 1 if successful, on -1 on error
 */
int8_t libewf_string_copy_ascii_to_utf16( LIBEWF_CHAR *ascii_string, size_t size_ascii, LIBEWF_CHAR *utf16_string, size_t size_utf16, uint8_t byte_order )
{
	size_t ascii_iterator = 0;
	size_t utf16_iterator = 2;

	if( ascii_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_ascii_to_utf16: invalid ASCII string.\n" );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_ascii_to_utf16: invalid UTF16 string.\n" );

		return( -1 );
	}
	if( ( size_ascii > (size_t) SSIZE_MAX ) || ( size_utf16 > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_ascii_to_utf16: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	/* Two additional bytes required for the byte order indicator
	 */
	if( size_utf16 < ( ( size_ascii * 2 ) + 2 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_ascii_to_utf16: UTF16 string too small.\n" );

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
		LIBEWF_WARNING_PRINT( "libewf_string_copy_ascii_to_utf16: undefined byte order.\n" );

		return( -1 );
	}
	/* Convert the string
	 */
	while( ascii_iterator < size_ascii )
	{
		if( byte_order == LIBEWF_STRING_LITTLE_ENDIAN )
		{
			utf16_string[ utf16_iterator     ] = ascii_string[ ascii_iterator ];
			utf16_string[ utf16_iterator + 1 ] = (LIBEWF_CHAR) '\0';
		}
		else if( byte_order == LIBEWF_STRING_BIG_ENDIAN )
		{
			utf16_string[ utf16_iterator     ] = (LIBEWF_CHAR) '\0';
			utf16_string[ utf16_iterator + 1 ] = ascii_string[ ascii_iterator ];
		}
		ascii_iterator += 1;
		utf16_iterator += 2;
	}
	utf16_string[ size_utf16 - 2 ] = (LIBEWF_CHAR) '\0';
	utf16_string[ size_utf16 - 1 ] = (LIBEWF_CHAR) '\0';

	return( 1 );
}

/* Converts an EWF character string to a LIBEWF character string
 * Returns 1 if successful, 0 if string was not set, or -1 on error
 */
int8_t libewf_string_copy_from_ewf_char( LIBEWF_CHAR *string, size_t size_string, EWF_CHAR *ewf_char_string, size_t size_ewf_char_string )
{
	size_t iterator = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_ewf_char: invalid string.\n" );

		return( -1 );
	}
	if( ewf_char_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_ewf_char: invalid EWF character string.\n" );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX ) || ( size_ewf_char_string > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_ewf_char: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	if( size_string < size_ewf_char_string )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_ewf_char: string too small.\n" );

		return( -1 );
	}
	for( iterator = 0; iterator < size_ewf_char_string; iterator++ )
	{
#ifdef HAVE_WIDE_CHARACTER_TYPE
		string[ iterator ] = btowc( (int) ewf_char_string[ iterator ] );
#else
		string[ iterator ] = (char) ewf_char_string[ iterator ];
#endif
	}
	string[ size_ewf_char_string - 1 ] = (LIBEWF_CHAR) '\0';

	return( 1 );
}

/* Converts a LIBEWF character string to an EWF character string
 * Returns 1 if successful, 0 if string was not set, or -1 on error
 */
int8_t libewf_string_copy_to_ewf_char( LIBEWF_CHAR *string, size_t size_string, EWF_CHAR *ewf_char_string, size_t size_ewf_char_string )
{
	size_t iterator = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_ewf_char: invalid string.\n" );

		return( -1 );
	}
	if( ewf_char_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_ewf_char: invalid EWF character string.\n" );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX ) || ( size_ewf_char_string > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_ewf_char: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	if( size_ewf_char_string < size_string )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_ewf_char: EWF character string too small.\n" );

		return( -1 );
	}
	for( iterator = 0; iterator < size_string; iterator++ )
	{
#ifdef HAVE_WIDE_CHARACTER_TYPE
		ewf_char_string[ iterator ] = (EWF_HEADER) wctob( string[ iterator ] );

		/* If character is out of the basic ASCII range use '_' as a place holder
		 */
		if( ewf_char_string[ iterator ] == (EWF_HEADER) EOF )
		{
			ewf_char_string[ iterator ] = (EWF_HEADER) '_';
		}
#else
		ewf_char_string[ iterator ] = (EWF_HEADER) string[ iterator ];
#endif
	}
	ewf_char_string[ size_string - 1 ] = (EWF_HEADER) '\0';

	return( 1 );
}

/* Converts the EWF digest hash to a printable string
 * Returns 1 if successful, 0 if hash was not set, or -1 on error
 */
int8_t libewf_string_copy_from_digest_hash( LIBEWF_CHAR *string, size_t size_string, EWF_DIGEST_HASH *digest_hash, size_t size_digest_hash )
{
	size_t string_iterator      = 0;
	size_t digest_hash_iterator = 0;
	uint8_t digest_digit        = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_digest_hash: invalid string.\n" );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX ) || ( size_digest_hash > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_digest_hash: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	/* The string requires space for 2 characters per digest hash digit and a end of string
	 */
	if( size_string < ( ( 2 * size_digest_hash ) + 1 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_digest_hash: string too small.\n" );

		return( -1 );
	}
	if( digest_hash == NULL )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_string_copy_from_digest_hash: invalid digest hash.\n" );

		return( 0 );
	}
	for( digest_hash_iterator = 0; digest_hash_iterator < size_digest_hash; digest_hash_iterator++ )
	{
		digest_digit = digest_hash[ digest_hash_iterator ] / 16;

		if( digest_digit <= 9 )
		{
			string[ string_iterator++ ] = (LIBEWF_CHAR) ( (uint8_t) '0' + digest_digit );
		}
		else
		{
			string[ string_iterator++ ] = (LIBEWF_CHAR) ( (uint8_t) 'a' + ( digest_digit - 10 ) );
		}
		digest_digit = digest_hash[ digest_hash_iterator ] % 16;

		if( digest_digit <= 9 )
		{
			string[ string_iterator++ ] = (LIBEWF_CHAR) ( (uint8_t) '0' + digest_digit );
		}
		else
		{
			string[ string_iterator++ ] = (LIBEWF_CHAR) ( (uint8_t) 'a' + ( digest_digit - 10 ) );
		}
	}
	string[ string_iterator ] = (LIBEWF_CHAR) '\0';

	return( 1 );
}

/* Converts an EWF header2 to a string
 * Returns 1 if successful, 0 if string was not set, or -1 on error
 */
int8_t libewf_string_copy_from_header2( LIBEWF_CHAR *string, size_t size_string, EWF_HEADER2 *header2, size_t size_header2 )
{
#ifdef HAVE_WIDE_CHARACTER_TYPE
	mbstate_t conversion_state;

	char *header2_pointer = NULL;
#endif

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_header2: invalid string.\n" );

		return( -1 );
	}
	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_header2: invalid header2.\n" );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX ) || ( size_header2 > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_header2: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	if( size_string < ( ( size_header2 - 2 ) / 2 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_header2: string too small.\n" );

		return( -1 );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
	if( libewf_common_memset( &conversion_state, 0, sizeof( mbstate_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_header2: unable to clear converion state.\n" );

		return( -1 );
	}
	if( mbsinit( &conversion_state ) == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_header2: unable to initialize converion state.\n" );

		return( -1 );
	}
	if( ( header2[ 0 ] == (EWF_HEADER2) 0xff ) || ( header2[ 0 ] == (EWF_HEADER2) 0xfe ) )
	{
		header2_pointer = (char *) &header2[ 2 ];
	}
	else
	{
		header2_pointer = (char *) &header2[ 0 ];
	}
	if( mbsrtowcs( string, (const char **) &header2_pointer, size_string, &conversion_state ) != ( size_string - 1 ) )
#else
	if( libewf_string_copy_utf16_to_ascii( (LIBEWF_CHAR *) header2, size_header2, string, size_string ) != 1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_from_header2: unable to copy header2 to string.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Converts a string to an EWF header2
 * Returns 1 if successful, 0 if header2 was not set, or -1 on error
 */
int8_t libewf_string_copy_to_header2( LIBEWF_CHAR *string, size_t size_string, EWF_HEADER2 *header2, size_t size_header2 )
{
#ifdef HAVE_WIDE_CHARACTER_TYPE
	mbstate_t conversion_state;
#endif

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_header2: invalid string.\n" );

		return( -1 );
	}
	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_header2: invalid header2.\n" );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX ) || ( size_header2 > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_header2: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	if( size_header2 < ( ( size_string * 2 ) + 2 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_header2: header2 too small.\n" );

		return( -1 );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
	if( libewf_common_memset( &conversion_state, 0, sizeof( mbstate_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_header2: unable to clear converion state.\n" );

		return( -1 );
	}
	if( mbsinit( &conversion_state ) == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_header2: unable to initialize converion state.\n" );

		return( -1 );
	}
	if( wcsrtombs( (char *) &header2[ 2 ], (const wchar_t **) &string, size_header2, &conversion_state ) != ( size_string - 1 ) )
#else
	if( libewf_string_copy_ascii_to_utf16( string, size_string, (LIBEWF_CHAR *) header2, size_header2, LIBEWF_STRING_LITTLE_ENDIAN ) != 1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "libewf_string_copy_to_header2: unable to copy string to header2.\n" );

		return( -1 );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
	if( header2[ 4 ] == (EWF_HEADER2) '\0' )
	{
		header2[ 0 ] = (EWF_HEADER2) 0xfe;
		header2[ 1 ] = (EWF_HEADER2) 0xff;
	}
	else
	{
		header2[ 0 ] = (EWF_HEADER2) 0xff;
		header2[ 1 ] = (EWF_HEADER2) 0xfe;
	}
#endif
	return( 1 );
}

