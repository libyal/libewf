/*
 * String functions for the ewftools
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
#include <notify.h>
#include <types.h>

#include "character_string.h"
#include "ewfstring.h"
#include "system_string.h"

/* Copies the source string (of system_character_t) into the destination string for a certain length
 * Terminates the destination string with \0 at ( length - 1 )
 * Returns 1 if successful, -1 on error
 */
int ewfstring_copy_system_string_to_character_string(
     character_t *destination,
     const system_character_t *source,
     size_t length )
{
	static char *function = "ewfstring_copy_system_string_to_character_string";
	ssize_t conversion    = (ssize_t) ( sizeof( character_t ) - sizeof( system_character_t ) );
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
	for( iterator = 0; iterator < length; iterator++ )
	{
		if( conversion == 0 )
		{
			destination[ iterator ] = (character_t) source[ iterator ];
		}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
		else if( conversion > 0 )
		{
			destination[ iterator ] = (character_t) btowc(
			                                         (int) source[ iterator ] );
		}
		else if( conversion < 0 )
		{
			destination[ iterator ] = (character_t) wctob(
			                                         (wint_t) source[ iterator ] );

			/* If character is out of the basic ASCII range use '_' as a place holder
			 */
			if( destination[ iterator ] == EOF )
			{
				destination[ iterator ] = '_';
			}
		}
#endif
		else
		{
			notify_warning_printf( "%s: unsupported conversion.\n",
			 function );

			return( -1 );
		}
	}
	destination[ length - 1 ] = (character_t) '\0';

	return( 1 );
}

/* Copies the source string into the destination string (of system_character_t) for a certain length
 * Terminates the destination string with \0 at ( length - 1 )
 * Returns 1 if successful, -1 on error
 */
int ewfstring_copy_character_string_to_system_string(
     system_character_t *destination,
     const character_t *source,
     size_t length )
{
	static char *function = "ewfstring_copy_character_string_to_system_string";
	ssize_t conversion    = (ssize_t) ( sizeof( character_t ) - sizeof( system_character_t ) );
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
	for( iterator = 0; iterator < length; iterator++ )
	{
		if( conversion == 0 )
		{
			destination[ iterator ] = (system_character_t) source[ iterator ];
		}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
		else if( conversion > 0 )
		{
			destination[ iterator ] = (system_character_t) wctob(
			                                                (wint_t) source[ iterator ] );

			/* If character is out of the basic ASCII range use '_' as a place holder
			 */
			if( destination[ iterator ] == EOF )
			{
				destination[ iterator ] = '_';
			}
		}
		else if( conversion < 0 )
		{
			destination[ iterator ] = (system_character_t) btowc(
			                                                (int) source[ iterator ] );
		}
#endif
		else
		{
			notify_warning_printf( "%s: unsupported conversion.\n",
			 function );

			return( -1 );
		}
	}
	destination[ length - 1 ] = (system_character_t) '\0';

	return( 1 );
}

