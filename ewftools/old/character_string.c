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
#include <memory.h>
#include <types.h>

#include "character_string.h"
#include "date_time.h"
#include "notify.h"

#if defined( string_to_signed_long_long )

/* Determines the 64-bit integer value represented by a string
 * Size should include the size of the end of string character
 * Returns 1 if successful or -1 on error
 */
int string_to_int64(
     const character_t *string,
     size_t size,
     int64_t *value )
{
	character_t *end_of_string = NULL;
	static char *function      = "string_to_int64";

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( size == 0 )
	{
		notify_warning_printf( "%s: string is emtpy.\n",
		 function );

		return( -1 );
	}
	if( ( sizeof( character_t ) * size ) > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		notify_warning_printf( "%s: invalid value.\n",
		 function );

		return( -1 );
	}
	end_of_string = (character_t *) &string[ size - 1 ];

	*value = string_to_signed_long_long(
	          string,
	          &end_of_string,
	          0 );

	if( *value == (int64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}
#endif

#if defined( string_to_unsigned_long_long )

/* Determines the 64-bit integer value represented by a string
 * Size should include the size of the end of string character
 * Returns 1 if successful or -1 on error
 */
int string_to_uint64(
     const character_t *string,
     size_t size,
     uint64_t *value )
{
	character_t *end_of_string = NULL;
	static char *function      = "string_to_uint64";

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( size == 0 )
	{
		notify_warning_printf( "%s: string is emtpy.\n",
		 function );

		return( -1 );
	}
	if( ( sizeof( character_t ) * size ) > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		notify_warning_printf( "%s: invalid value.\n",
		 function );

		return( -1 );
	}
	end_of_string = (character_t *) &string[ size - 1 ];

	*value = string_to_unsigned_long_long(
	          string,
	          &end_of_string,
	          0 );

	if( *value == (uint64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( date_time_ctime ) && !defined( date_time_wctime )

/* Sets ctime in the string
 * The string must be at least 32 characters of size
 * Returns the pointer to the string if successful or NULL on error
 */
character_t *string_ctime(
              const time_t *timestamp,
              character_t *string,
              size_t size )
{
	char ctime_string[ 32 ];

	static char *function = "string_ctime";

	if( timestamp == NULL )
	{
		notify_warning_printf( "%s: invalid timestamp.\n",
		 function );

		return( NULL );
	}
	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( NULL );
	}
	if( ( sizeof( character_t ) * size ) > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	if( size < 32 )
	{
		notify_warning_printf( "%s: string too small.\n",
		 function );

		return( NULL );
	}
	if( date_time_ctime(
	     timestamp,
	     ctime_string,
	     32 ) == NULL )
	{
		notify_warning_printf( "%s: unable to set ctime string.\n",
		 function );

		return( NULL );
	}
	if( string_copy_char_to_wchar(
	     string,
	     ctime_string,
	     32 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set string.\n",
		 function );

		return( NULL );
	}
	return( string );
}

#endif

