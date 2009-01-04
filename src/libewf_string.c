/*
 * libewf string
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <stdlib.h>

#include "libewf_common.h"
#include "libewf_notify.h"
#include "libewf_string.h"

/* Allocates memory for a new ewf string struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_STRING *libewf_string_alloc( uint32_t size )
{
	LIBEWF_STRING *string = NULL;
	uint32_t string_size  = 0;

	string_size = size * sizeof( LIBEWF_STRING );
	string      = (LIBEWF_STRING *) libewf_alloc_cleared( string_size, 0 );

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_alloc: unable to allocate string.\n" );

		return( NULL );
	}
	return( string );
}

/* Frees memory of a string
 */
void libewf_string_free( LIBEWF_STRING *string )
{
	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_free: invalid string.\n" );

		return;
	}
	libewf_free( string );
}

/* Determines the length of the string, the string should be 0 terminated
 * Returns the amount of characters/bytes in the string the end of string is not included, a value of 0 is returned if the string is empty
 */
uint32_t libewf_string_length( LIBEWF_STRING *string )
{
	uint32_t length = 0;

	if( string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_length: invalid string.\n" );

		return( 0 );
	}
	while( string[ length ] != 0 )
	{
		length++;

		if( length == 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_string_length: overflow in length.\n" );

			return( 0 );
		}
	}
	return( length );
}

/* Compares two strings for a certain length, both strings must also correctly terminate
 * Returns 1 if both strings are equal, 0 if not, -1 on error
 */
int8_t libewf_string_compare( LIBEWF_STRING *string1, LIBEWF_STRING *string2, uint32_t size )
{
	uint32_t iterator = 0;

	if( string1 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_compare: invalid string1.\n" );

		return( -1 );
	}
	if( string2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_compare: invalid string2.\n" );

		return( -1 );
	}
	for( iterator = 0; iterator < size; iterator++ )
	{
		if( string1[ iterator ] == 0 )
		{
			return( -1 );
		}
		if( string2[ iterator ] == 0 )
		{
			return( -1 );
		}
		if( string1[ iterator ] != string2[ iterator ] )
		{
			return( 0 );
		}
	}
	return( 1 );
}

/* Split a string into elements using a delimiter character
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_STRING **libewf_string_split( LIBEWF_STRING *string, uint8_t delimiter, uint32_t *amount )
{
	LIBEWF_STRING **lines     = NULL;
	LIBEWF_STRING **data_set  = NULL;
	LIBEWF_STRING *line_start = NULL;
	LIBEWF_STRING *line_end   = NULL;
	LIBEWF_STRING *string_end = NULL;
	uint32_t iterator         = 0;
	uint32_t line_size        = 0;
	uint32_t lines_size       = 0;
	uint32_t string_size      = 0;

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
	string_size = libewf_string_length( string );
	line_start  = string;
	line_end    = string;
	string_end  = &string[ string_size ];

	while( 1 )
	{
		line_end = (LIBEWF_STRING *) libewf_strchr( (char *) line_start, delimiter );

		iterator++;

		if( line_end == NULL )
		{
			break;
		}
		if( line_end == line_start )
		{
			line_start += 1;
		}
		else if( line_end != string )
		{
			line_start = line_end + 1;
		}
	}
	*amount    = iterator;
	lines_size = *amount * sizeof( LIBEWF_STRING * );
	lines      = (LIBEWF_STRING **) libewf_alloc_cleared( lines_size, 0 );

	if( lines == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_string_split: unable to allocate dynamic array lines.\n" );

		return( NULL );
	}
	line_start = string;
	line_end   = string;

	for( iterator = 0; iterator < *amount; iterator++ )
	{
		if( line_end != string )
		{
			line_start = line_end + 1;
		}
		line_end = (LIBEWF_STRING *) libewf_strchr( (char *) line_start, delimiter );

		/* Check for last value
		 */
		if( line_end == NULL )
		{
			line_size = string_end - line_start;
		}
		else
		{
			line_size = line_end - line_start;
		}
		lines[ iterator ] = libewf_string_alloc( line_size + 1 );

		if( lines[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_string_split: unable to allocate line string.\n" );

			libewf_string_split_values_free( lines, ( iterator - 1 ) );

			return( NULL );
		}
		if( line_size > 0 )
		{
			data_set = (LIBEWF_STRING **) libewf_memcpy( (void *) lines[ iterator ], (void *) line_start, line_size );

			if( data_set == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_string_split: unable to set dynamic array lines.\n" );

				libewf_string_split_values_free( lines, iterator );

				return( NULL );
			}
		}
		lines[ iterator ][ line_size ] = '\0';

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
void libewf_string_split_values_free( LIBEWF_STRING **split_values, uint32_t amount )
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
			libewf_string_free( split_values[ iterator ] );
		}
	}
	libewf_free( split_values );
}

