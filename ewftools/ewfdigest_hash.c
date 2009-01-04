/*
 * Crypographic hash for ewftools
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

#include "ewfdigest_hash.h"

/* Converts the EWF digest hash to a printable string
 * Returns 1 if successful, 0 if hash was not set, or -1 on error
 */
int ewfdigest_copy_to_string(
     ewfdigest_hash_t *digest_hash,
     size_t size_digest_hash,
     character_t *string,
     size_t size_string )
{
	static char *function       = "ewfdigest_copy_to_string";
	size_t string_iterator      = 0;
	size_t digest_hash_iterator = 0;
	uint8_t digest_digit        = 0;

	if( digest_hash == NULL )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf( "%s: invalid digest hash.\n",
		 function );
#endif

		return( 0 );
	}
	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( ( size_string > (size_t) SSIZE_MAX )
	 || ( size_digest_hash > (size_t) SSIZE_MAX ) )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* The string requires space for 2 characters per digest hash digit and a end of string
	 */
	if( size_string < ( ( 2 * size_digest_hash ) + 1 ) )
	{
		notify_warning_printf( "%s: string too small.\n",
		 function );

		return( -1 );
	}
	for( digest_hash_iterator = 0; digest_hash_iterator < size_digest_hash; digest_hash_iterator++ )
	{
		digest_digit = digest_hash[ digest_hash_iterator ] / 16;

		if( digest_digit <= 9 )
		{
			string[ string_iterator++ ] = (character_t) ( (uint8_t) '0' + digest_digit );
		}
		else
		{
			string[ string_iterator++ ] = (character_t) ( (uint8_t) 'a' + ( digest_digit - 10 ) );
		}
		digest_digit = digest_hash[ digest_hash_iterator ] % 16;

		if( digest_digit <= 9 )
		{
			string[ string_iterator++ ] = (character_t) ( (uint8_t) '0' + digest_digit );
		}
		else
		{
			string[ string_iterator++ ] = (character_t) ( (uint8_t) 'a' + ( digest_digit - 10 ) );
		}
	}
	string[ string_iterator ] = 0;

	return( 1 );
}

