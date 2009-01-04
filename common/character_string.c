/*
 * Character string functions
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

#include "common.h"
#include "character_string.h"
#include "memory.h"
#include "notify.h"
#include "types.h"

#if defined( HAVE_WIDE_CHARACTER_TYPE ) || !defined( HAVE_STRNDUP )

/* Duplicates a string
 * Returns the pointer to the duplicate string, or NULL on error
 */
character_t *string_duplicate(
              character_t *string,
              size_t size )
{
	character_t *duplicate = NULL;
	static char *function  = "string_duplicate";

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
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	/* Add an additional character for the end of string
	 */
	size += 1;

	duplicate = (character_t *) memory_allocate(
	                             sizeof( character_t ) * size );

	if( duplicate == NULL )
	{
		notify_warning_printf( "%s: unable to create duplicate string.\n",
		 function );

		return( NULL );
	}
	if( string_copy(
	     duplicate,
	     string, size ) == NULL )
	{
		notify_warning_printf( "%s: unable to set duplicate string.\n",
		 function );

		memory_free(
		 duplicate );

		return( NULL );
	}
	duplicate[ size - 1 ] = (character_t) '\0';

	return( duplicate );
}

#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )

#if defined( HAVE_WINDOWS_API )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOLL )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) wcstoll( string, end_of_string, base )
#endif

#else

#if defined( HAVE_WINDOWS_API )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _atoi64( string )

#elif defined( HAVE_STRTOLL )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) strtoll( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) atoll( string )
#endif

#endif

/* Returns the value represented by a string, returns 0 error
 */
int64_t string_to_int64(
         const character_t *string,
         size_t size )
{
	character_t *end_of_string = NULL;
	static char *function      = "string_to_int64";
	int64_t value              = 0;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( 0 );
	}
	if( size == 0 )
	{
		notify_warning_printf( "%s: string is emtpy.\n",
		 function );

		return( 0 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	end_of_string = (character_t *) &string[ size - 1 ];

#if defined( string_to_signed_long_long )
	value = string_to_signed_long_long(
	         string,
	         &end_of_string,
	         0 );
#else
#error Missing equivalent of strtoll
#endif

	if( value == (int64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

#if defined( HAVE_WINDOWS_API )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _wtoi64( string )

#elif defined( HAVE_WCSTOULL )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) wcstoull( string, end_of_string, base )
#endif

#else

#if defined( HAVE_WINDOWS_API )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _atoi64( string )

#elif defined( HAVE_STRTOULL )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) strtoull( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) atoll( string )
#endif

#endif

/* Returns the value represented by a string, returns 0 on error
 */
uint64_t string_to_uint64(
          const character_t *string,
          size_t size )
{
	character_t *end_of_string = NULL;
	static char *function      = "string_to_uint64";
	uint64_t value             = 0;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( 0 );
	}
	if( size == 0 )
	{
		notify_warning_printf( "%s: string is emtpy.\n",
		 function );

		return( 0 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	end_of_string = (character_t *) &string[ size - 1 ];

#if defined( string_to_unsigned_long_long )
	value = string_to_unsigned_long_long(
	         string,
	         &end_of_string,
	         0 );
#else
#error Missing equivalent of strtoull
#endif

	if( value == (uint64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}

/* Split a string into elements using a delimiter character
 * Returns a pointer to the new instance, NULL on error
 */
character_t **string_split(
               character_t *string,
               size_t size,
               character_t delimiter,
               uint32_t *amount )
{
	character_t **lines     = NULL;
	character_t *line_start = NULL;
	character_t *line_end   = NULL;
	character_t *string_end = NULL;
	static char *function   = "string_split";
	size_t size_string      = 0;
	size_t line_size        = 0;
	uint32_t iterator       = 0;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( NULL );
	}
	if( amount == NULL )
	{
		notify_warning_printf( "%s: invalid amount.\n",
		 function );

		return( NULL );
	}
	if( size == 0 )
	{
		notify_warning_printf( "%s: string is empty.\n",
		 function );

		return( NULL );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	size_string = size;
	line_start  = string;
	line_end    = string;
	string_end  = &string[ size ];

	do
	{
		line_end = (character_t *) string_search(
		                            line_start,
		                            delimiter,
		                            size_string );

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
	lines   = (character_t **) memory_allocate(
	                            ( sizeof( character_t * ) * *amount ) );

	if( lines == NULL )
	{
		notify_warning_printf( "%s: unable to allocate dynamic array lines.\n",
		 function );

		return( NULL );
	}
	if( memory_set(
	     lines,
	     0,
	     *amount ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear dynamic array lines.\n",
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
		line_end = (character_t *) string_search(
		                            line_start,
		                            delimiter,
		                            size_string );

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

		lines[ iterator ] = (character_t *) memory_allocate(
                                                     sizeof( character_t ) * line_size );

		if( lines[ iterator ] == NULL )
		{
			notify_warning_printf( "%s: unable to allocate line string.\n",
			 function );

			string_split_values_free(
			 lines,
			 ( iterator - 1 ) );

			return( NULL );
		}
		if( string_copy(
		     lines[ iterator ],
		     line_start,
		     line_size ) == NULL )
		{
			notify_warning_printf( "%s: unable to set dynamic array lines.\n",
			 function );

			string_split_values_free(
			 lines,
			 iterator );

			return( NULL );
		}
		lines[ iterator ][ line_size - 1 ] = (character_t) '\0';

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
void string_split_values_free(
      character_t **split_values,
      uint32_t amount )
{
	static char *function = "string_split_values_free";
	uint32_t iterator     = 0;

	if( split_values == NULL )
	{
		notify_warning_printf( "%s: invalid split values array.\n",
		 function );

		return;
	}
	for( iterator = 0; iterator < amount; iterator++ )
	{
		if( split_values[ iterator ] == NULL )
		{
			notify_warning_printf( "%s: empty value.\n",
			 function );
		}
		else
		{
			memory_free(
			 split_values[ iterator ] );
		}
	}
	memory_free(
	 split_values );
}

