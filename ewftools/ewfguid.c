/*
 * GUID functions
 *
 * Copyright (c) 2008, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <notify.h>
#include <types.h>

#include "ewfguid.h"

/* Converts the GUID into a string
 * Returns 1 if successful or -1 on error
 */
int ewfguid_to_string(
     ewfguid_t *guid,
     character_t *string,
     size_t length )
{
	static char *function = "ewfguid_to_string";
	ssize_t print_count   = 0;

	if( guid == NULL )
	{
		notify_warning_printf( "%s: invalid guid.\n",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( length < 37 )
	{
		notify_warning_printf( "%s: string too small.\n",
		 function );

		return( -1 );
	}
	if( length > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* Create the GUID string
	 */
	print_count = string_snprintf(
	               string,
	               length,
	               "%.2" PRIx8 "%.2" PRIx8 "%.2" PRIx8 "%.2" PRIx8
	               "-%.2" PRIx8 "%.2" PRIx8
	               "-%.2" PRIx8 "%.2" PRIx8
	               "-%.2" PRIx8 "%.2" PRIx8
	               "-%.2" PRIx8 "%.2" PRIx8 "%.2" PRIx8 "%.2" PRIx8 "%.2" PRIx8 "%.2" PRIx8 "\n",
	               guid[ 0 ], guid[ 1 ], guid[ 2 ], guid[ 3 ],
	               guid[ 4 ], guid[ 5 ],
	               guid[ 6 ], guid[ 7 ],
	               guid[ 8 ], guid[ 9 ],
	               guid[ 10 ], guid[ 11 ], guid[ 12 ], guid[ 13 ], guid[ 14 ], guid[ 15 ] );

	if( print_count != 37 )
	{
		notify_warning_printf( "%s: unable to format string.\n",
		 function );

		return( -1 );
	}
	string[ 36 ] = (character_t) '\0';

	return( 1 );
}

