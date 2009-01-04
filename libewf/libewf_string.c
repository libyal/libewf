/*
 * Character type string functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <character_string.h>
#include <date_time.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#include <libewf/definitions.h>

#include "libewf_string.h"

#include "ewf_char.h"

/* Split a string into elements using a delimiter character
 * Returns a pointer to the new instance, NULL on error
 */
character_t **libewf_string_split(
               character_t *string,
               size_t size,
               character_t delimiter,
               uint32_t *amount )
{
	character_t **lines     = NULL;
	character_t *line_start = NULL;
	character_t *line_end   = NULL;
	character_t *string_end = NULL;
	static char *function   = "libewf_string_split";
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

			libewf_string_split_values_free(
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

			libewf_string_split_values_free(
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
void libewf_string_split_values_free(
      character_t **split_values,
      uint32_t amount )
{
	static char *function = "libewf_string_split_values_free";
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

/* Copies a multi byte UTF16 string to a single byte string
 * Returns 1 if successful, on -1 on error
 */
int libewf_string_copy_from_utf16(
     character_t *string,
     size_t size_string,
     ewf_char_t *utf16_string,
     size_t size_utf16 )
{
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	mbstate_t conversion_state;
#endif
	static char *function  = "libewf_string_copy_from_utf16";
	size_t utf16_iterator  = 2;
	size_t string_iterator = 0;
	uint8_t byte_order     = 0;

	if( utf16_string == NULL )
	{
		notify_warning_printf( "%s: invalid UTF16 string.\n",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX )
	 || ( size_utf16 > (size_t) SSIZE_MAX ) )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* The UTF16 string contains twice as much bytes needed for the string
	 * with two additional bytes representing byte order
	 */
	if( size_string < ( ( size_utf16 - 2 ) / 2 ) )
	{
		notify_warning_printf( "%s: string too small.\n",
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
		notify_verbose_printf( "%s: no byte order in UTF16 string.\n",
		 function );

		if( ( utf16_string[ 0 ] == (character_t) '\0' )
		 && ( utf16_string[ 1 ] != (character_t) '\0' ) )
		{
			byte_order = LIBEWF_STRING_LITTLE_ENDIAN;
		}
		else if( ( utf16_string[ 0 ] != (character_t) '\0' )
		 && ( utf16_string[ 1 ] == (character_t) '\0' ) )
		{
			byte_order = LIBEWF_STRING_LITTLE_ENDIAN;
		}
		else
		{
			notify_warning_printf( "%s: unable to determine byte order in UTF16 string.\n",
			 function );

			return( -1 );
		}
		utf16_iterator = 0;
	}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	if( memory_set(
	     &conversion_state,
	     0,
	     sizeof( mbstate_t ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear converion state.\n",
		 function );

		return( -1 );
	}
	if( mbsinit(
	     &conversion_state ) == 0 )
	{
		notify_warning_printf( "%s: unable to initialize converion state.\n",
		 function );

		return( -1 );
	}
#endif
	/* Convert string
	 */
	while( utf16_iterator < size_utf16 )
	{
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
		if( mbrtowc(
		     &( string[ string_iterator ] ),
		     (const char *) &( utf16_string[ utf16_iterator ] ),
		     2,
		     &conversion_state ) >= (size_t) -2 )
		{
			notify_warning_printf( "%s: unable to convert UTF16 character: %02x %02x.\n",
			 function, utf16_string[ utf16_iterator ], utf16_string[ utf16_iterator + 1 ] );
		}
#else
		if( byte_order == LIBEWF_STRING_BIG_ENDIAN )
		{
			if( utf16_string[ utf16_iterator ] == (ewf_char_t) '\0' )
			{
				string[ string_iterator ] = utf16_string[ utf16_iterator + 1 ];
			}
			else
			{
				/* Add a place holder character
				 */
				string[ string_iterator ] = '_';
			}
		}
		else if( byte_order == LIBEWF_STRING_LITTLE_ENDIAN )
		{
			if( utf16_string[ utf16_iterator + 1 ] == (ewf_char_t) '\0' )
			{
				string[ string_iterator ] = utf16_string[ utf16_iterator ];
			}
			else
			{
				/* Add a place holder character
				 */
				string[ string_iterator ] = '_';
			}
		}
#endif
		utf16_iterator  += 2;
		string_iterator += 1;
	}
	string[ size_string - 1 ] = (character_t) '\0';

	return( 1 );
}

/* Copies a single byte string to a multi byte UTF16 string
 * Returns 1 if successful, on -1 on error
 */
int libewf_string_copy_to_utf16(
     character_t *string,
     size_t size_string,
     ewf_char_t *utf16_string,
     size_t size_utf16,
     uint8_t byte_order )
{
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	mbstate_t conversion_state;
#endif
	static char *function  = "libewf_string_copy_to_utf16";
	size_t string_iterator = 0;
	size_t utf16_iterator  = 2;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		notify_warning_printf( "%s: invalid UTF16 string.\n",
		 function );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX )
	 || ( size_utf16 > (size_t) SSIZE_MAX ) )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	if( memory_set(
	     &conversion_state,
	     0,
	     sizeof( mbstate_t ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear converion state.\n",
		 function );

		return( -1 );
	}
	if( mbsinit(
	     &conversion_state ) == 0 )
	{
		notify_warning_printf( "%s: unable to initialize converion state.\n",
		 function );

		return( -1 );
	}
#endif
	/* Two additional bytes required for the byte order indicator
	 */
	if( size_utf16 < ( ( size_string * 2 ) + 2 ) )
	{
		notify_warning_printf( "%s: UTF16 string too small.\n",
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
		notify_warning_printf( "%s: undefined byte order.\n",
		 function );

		return( -1 );
	}
	/* Convert the string
	 */
	while( string_iterator < size_string )
	{
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
		if( wcrtomb(
		     (char *) &( utf16_string[ utf16_iterator ] ),
		     string[ string_iterator ],
		     &conversion_state ) >= (size_t) -1 )
		{
			notify_warning_printf( "%s: unable to convert character: %04x.\n",
			 function, string[ string_iterator ] );
		}
#else
		if( byte_order == LIBEWF_STRING_LITTLE_ENDIAN )
		{
			utf16_string[ utf16_iterator     ] = (ewf_char_t) string[ string_iterator ];
			utf16_string[ utf16_iterator + 1 ] = (ewf_char_t) '\0';
		}
		else if( byte_order == LIBEWF_STRING_BIG_ENDIAN )
		{
			utf16_string[ utf16_iterator     ] = (ewf_char_t) '\0';
			utf16_string[ utf16_iterator + 1 ] = (ewf_char_t) string[ string_iterator ];
		}
#endif
		string_iterator += 1;
		utf16_iterator  += 2;
	}
	utf16_string[ size_utf16 - 2 ] = (ewf_char_t) '\0';
	utf16_string[ size_utf16 - 1 ] = (ewf_char_t) '\0';

	return( 1 );
}

