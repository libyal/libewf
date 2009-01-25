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

#include "notify.h"
#include "system_string.h"

#if defined( system_string_to_signed_long_long )

/* Returns the value represented by a string, returns 0 error
 */
int64_t system_string_to_int64(
         const system_character_t *string,
         size_t size )
{
	system_character_t *end_of_string = NULL;
	static char *function             = "system_string_to_int64";
	int64_t value                     = 0;

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
	end_of_string = (system_character_t *) &string[ size - 1 ];

	value = system_string_to_signed_long_long(
	         string,
	         &end_of_string,
	         0 );

	if( value == (int64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}
#endif

#if defined( system_string_to_unsigned_long_long )

/* Returns the value represented by a string, returns 0 on error
 */
uint64_t system_string_to_uint64(
          const system_character_t *string,
          size_t size )
{
	system_character_t *end_of_string = NULL;
	static char *function             = "system_string_to_uint64";
	uint64_t value                    = 0;

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
	end_of_string = (system_character_t *) &string[ size - 1 ];

	value = system_string_to_unsigned_long_long(
	         string,
	         &end_of_string,
	         0 );

	if( value == (uint64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}
#endif

