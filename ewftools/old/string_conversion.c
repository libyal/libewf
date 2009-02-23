/*
 * String conversion functions
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
#include <types.h>

#include "notify.h"
#include "string_conversion.h"

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Copies the source string (of wchar_t) into the destination string (of char) for a certain size
 * Terminates the destination string with an end of string character
 * Returns 1 if successful or -1 on error
 */
int string_copy_wchar_to_char(
     char *destination,
     const wchar_t *source,
     size_t size )
{
	static char *function = "string_copy_wchar_to_char";
	size_t iterator       = 0;
	int character         = 0;

	if( source == NULL )
	{
		notify_warning_printf( "%s: invalid source.\n",
		 function );

		return( -1 );
	}
	if( destination == NULL )
	{
		notify_warning_printf( "%s: invalid destination.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < size; iterator++ )
	{
		character = wctob(
		             (wint_t) source[ iterator ] );

		/* If character is out of the basic ASCII range use '_' as a place holder
		 */
		if( character == EOF )
		{
			destination[ iterator ] = '_';
		}
		else
		{
			destination[ iterator ] = (char) character;
		}
	}
	destination[ size - 1 ] = (char) '\0';

	return( 1 );
}

/* Copies the source string (of char) into the destination string (of wchar_t) for a certain size
 * Terminates the destination string with an end of string character
 * Returns 1 if successful or -1 on error
 */
int string_copy_char_to_wchar(
     wchar_t *destination,
     const char *source,
     size_t size )
{
	static char *function = "string_copy_char_to_wchar";
	size_t iterator       = 0;

	if( source == NULL )
	{
		notify_warning_printf( "%s: invalid source.\n",
		 function );

		return( -1 );
	}
	if( destination == NULL )
	{
		notify_warning_printf( "%s: invalid destination.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < size; iterator++ )
	{
		destination[ iterator ] = (wchar_t) btowc(
		                                     (int) source[ iterator ] );
	}
	destination[ size - 1 ] = (wchar_t) '\0';

	return( 1 );
}

#endif

