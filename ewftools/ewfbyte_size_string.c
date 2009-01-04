/*
 * Byte size string functions for the ewftools
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
#include <notify.h>
#include <system_string.h>
#include <types.h>

#include "ewfbyte_size_string.h"

/* Determines the factor string of a certain factor value
 * Returns the string if successful or NULL on error
 */
const character_t *ewfbyte_size_string_get_factor_string(
                    int8_t factor )
{
	switch( factor )
	{
		case 0:
			return( _CHARACTER_T_STRING( "" ) );
		case 1:
			return( _CHARACTER_T_STRING( "K" ) );
		case 2:
			return( _CHARACTER_T_STRING( "M" ) );
		case 3:
			return( _CHARACTER_T_STRING( "G" ) );
		case 4:
			return( _CHARACTER_T_STRING( "T" ) );
		case 5:
			return( _CHARACTER_T_STRING( "P" ) );
		case 6:
			return( _CHARACTER_T_STRING( "E" ) );
		case 7:
			return( _CHARACTER_T_STRING( "Z" ) );
		case 8:
			return( _CHARACTER_T_STRING( "Y" ) );
		default :
			break;
	}
	return( NULL );
}

/* Determines the factor from a factor string
 * Returns the factor if successful or -1 on error
 */
int8_t ewfbyte_size_string_get_factor(
        character_t factor )
{
	switch( factor )
	{
		case 'k':
		case 'K':
			return( 1 );
		case 'm':
		case 'M':
			return( 2 );
		case 'g':
		case 'G':
			return( 3 );
		case 't':
		case 'T':
			return( 4 );
		case 'p':
		case 'P':
			return( 5 );
		case 'e':
		case 'E':
			return( 6 );
		case 'z':
		case 'Z':
			return( 7 );
		case 'y':
		case 'Y':
			return( 8 );
		default :
			break;
	}
	return( -1 );
}

/* Creates a human readable byte size string
 * Returns 1 if successful or -1 on error
 */
int ewfbyte_size_string_create(
     character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t size,
     int units )
{
	const character_t *factor_string = NULL;
	const character_t *units_string  = NULL;
	static char *function            = "ewfbyte_size_string_create";
	ssize_t print_count              = 0;
	uint64_t factored_size           = 0;
	uint64_t last_factored_size      = 0;
	int8_t factor                    = 0;
	int8_t remainder                 = -1;

	if( byte_size_string == NULL )
	{
		notify_warning_printf( "%s: invalid byte size string.\n",
		 function );

		return( -1 );
	}
	/* Minimum of 4 digits and separator, space, 3 letter unit, end of string
	 */
	if( byte_size_string_length < 9 )
	{
		notify_warning_printf( "%s: byte size string too small.\n",
		 function );

		return( -1 );
	}
	if( ( size < 1024 )
	 || ( units == EWFBYTE_SIZE_STRING_UNIT_MEGABYTE ) )
	{
		units_string = _CHARACTER_T_STRING( "B" );
	}
	else if( units == EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE )
	{
		units_string = _CHARACTER_T_STRING( "iB" );
	}
	factored_size = size;

	if( factored_size >= (uint64_t) units )
	{
		while( factored_size >= (uint64_t) units )
		{
			last_factored_size = factored_size;
			factored_size     /= units;

			factor++;
		}
		if( factored_size < 10 )
		{
			last_factored_size %= units;
			remainder           = (int8_t) ( last_factored_size / 100 );
		}
		if( factor > 8 )
		{
			notify_warning_printf( "%s: a size with a factor larger than 8 currently not supported.\n",
			 function );

			return( -1 );
		}
	}
	factor_string = ewfbyte_size_string_get_factor_string(
	                 factor );

	if( factor_string == NULL )
	{
		notify_warning_printf( "%s: unable to create factor string.\n",
		 function );

		return( -1 );
	}
	if( remainder > 9 )
	{
		remainder = 9;
	}
	if( remainder >= 0 )
	{
		print_count = string_snprintf(
		               byte_size_string,
		               byte_size_string_length,
		               _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIu64 )
		               _CHARACTER_T_STRING( ".%" ) _CHARACTER_T_STRING( PRIu8 )
		               _CHARACTER_T_STRING( " %" ) _CHARACTER_T_STRING( PRIs )
		               _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIs ),
		               factored_size,
		               remainder,
		               factor_string,
		               units_string );
	}
	else
	{
		print_count = string_snprintf(
		               byte_size_string,
		               byte_size_string_length,
		               _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIu64 )
		               _CHARACTER_T_STRING( " %" ) _CHARACTER_T_STRING( PRIs )
		               _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIs ),
		               factored_size,
		               factor_string,
		               units_string );
	}
	if( print_count < 0 )
	{
		notify_warning_printf( "%s: unable to set byte size string.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Converts a human readable byte size string into a value
 * Returns 1 if successful or -1 on error
 */
int ewfbyte_size_string_convert(
     character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t *size )
{
	static char *function            = "ewfbyte_size_string_convert";
	size_t byte_size_string_iterator = 0;
	uint64_t byte_size               = 0;
	int8_t factor                    = 0;
	int8_t remainder                 = -1;
	int units                        = 0;

	if( byte_size_string == NULL )
	{
		notify_warning_printf( "%s: invalid byte size string.\n",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		notify_warning_printf( "%s: invalid size.\n",
		 function );

		return( -1 );
	}
	while( byte_size_string_iterator < byte_size_string_length )
	{
		if( ( byte_size_string[ byte_size_string_iterator ] < '0' )
		 || ( byte_size_string[ byte_size_string_iterator ] > '9' ) )
		{
			break;
		}
		byte_size *= 10;
		byte_size += ( byte_size_string[ byte_size_string_iterator ] - '0' );

		byte_size_string_iterator++;
	}
	if( byte_size_string[ byte_size_string_iterator ] == '.' )
	{
		byte_size_string_iterator++;

		if( ( byte_size_string[ byte_size_string_iterator ] >= '0' )
		 && ( byte_size_string[ byte_size_string_iterator ] <= '9' ) )
		{
			remainder = ( byte_size_string[ byte_size_string_iterator ] - '0' );

			byte_size_string_iterator++;
		}
		remainder *= 10;

		if( ( byte_size_string[ byte_size_string_iterator ] >= '0' )
		 && ( byte_size_string[ byte_size_string_iterator ] <= '9' ) )
		{
			remainder += ( byte_size_string[ byte_size_string_iterator ] - '0' );

			byte_size_string_iterator++;
		}
		/* Ignore more than 2 digits after separator
		 */
		while( byte_size_string_iterator < byte_size_string_length )
		{
			if( ( byte_size_string[ byte_size_string_iterator ] < '0' )
			 || ( byte_size_string[ byte_size_string_iterator ] > '9' ) )
			{
				break;
			}
			byte_size_string_iterator++;
		}
	}
	if( byte_size_string[ byte_size_string_iterator ] == ' ' )
	{
		byte_size_string_iterator++;
	}
	factor = ewfbyte_size_string_get_factor(
	          byte_size_string[ byte_size_string_iterator ] );

	if( factor < 0 )
	{
		notify_warning_printf( "%s: invalid factor.\n",
		 function );

		return( -1 );
	}
	byte_size_string_iterator++;

	if( ( byte_size_string[ byte_size_string_iterator ] == 'i' )
	 && ( byte_size_string[ byte_size_string_iterator + 1 ] == 'B' ) )
	{
		units = EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE;

		byte_size_string_iterator += 2;
	}
	else if( byte_size_string[ byte_size_string_iterator ] == 'B' )
	{
		units = EWFBYTE_SIZE_STRING_UNIT_MEGABYTE;

		byte_size_string_iterator++;
	}
	else
	{
		notify_warning_printf( "%s: invalid units.\n",
		 function );

		return( -1 );
	}
	if( factor > 0 )
	{
		if( remainder > 0 )
		{
			byte_size *= units;

			factor--;

			byte_size += ( remainder * 10 );
		}
		for( ; factor > 0; factor-- )
		{
			byte_size *= units;
		}
	}
	else if( remainder >= 0 )
	{
		notify_warning_printf( "%s: ignoring byte value remainder.\n",
		 function );
	}
	if( byte_size_string[ byte_size_string_iterator ] != '\0' )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf( "%s: trailing data in byte size string.\n",
		 function );
#endif
	}
	*size = byte_size;

	return( 1 );
}

/* Determines the factor from a factor string
 * Returns the factor if successful or -1 on error
 */
int8_t ewfbyte_size_string_get_factor_system_character(
        system_character_t factor )
{
	switch( factor )
	{
		case 'k':
		case 'K':
			return( 1 );
		case 'm':
		case 'M':
			return( 2 );
		case 'g':
		case 'G':
			return( 3 );
		case 't':
		case 'T':
			return( 4 );
		case 'p':
		case 'P':
			return( 5 );
		case 'e':
		case 'E':
			return( 6 );
		case 'z':
		case 'Z':
			return( 7 );
		case 'y':
		case 'Y':
			return( 8 );
		default :
			break;
	}
	return( -1 );
}

/* Converts a human readable byte size string into a value
 * Returns 1 if successful or -1 on error
 */
int ewfbyte_size_string_convert_system_character(
     system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t *size )
{
	static char *function            = "ewfbyte_size_string_convert_system_character";
	size_t byte_size_string_iterator = 0;
	uint64_t byte_size               = 0;
	int8_t factor                    = 0;
	int8_t remainder                 = -1;
	int units                        = 0;

	if( byte_size_string == NULL )
	{
		notify_warning_printf( "%s: invalid byte size string.\n",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		notify_warning_printf( "%s: invalid size.\n",
		 function );

		return( -1 );
	}
	while( byte_size_string_iterator < byte_size_string_length )
	{
		if( ( byte_size_string[ byte_size_string_iterator ] < '0' )
		 || ( byte_size_string[ byte_size_string_iterator ] > '9' ) )
		{
			break;
		}
		byte_size *= 10;
		byte_size += ( byte_size_string[ byte_size_string_iterator ] - '0' );

		byte_size_string_iterator++;
	}
	if( byte_size_string[ byte_size_string_iterator ] == '.' )
	{
		byte_size_string_iterator++;

		if( ( byte_size_string[ byte_size_string_iterator ] >= '0' )
		 && ( byte_size_string[ byte_size_string_iterator ] <= '9' ) )
		{
			remainder = ( byte_size_string[ byte_size_string_iterator ] - '0' );

			byte_size_string_iterator++;
		}
		remainder *= 10;

		if( ( byte_size_string[ byte_size_string_iterator ] >= '0' )
		 && ( byte_size_string[ byte_size_string_iterator ] <= '9' ) )
		{
			remainder += ( byte_size_string[ byte_size_string_iterator ] - '0' );

			byte_size_string_iterator++;
		}
		/* Ignore more than 2 digits after separator
		 */
		while( byte_size_string_iterator < byte_size_string_length )
		{
			if( ( byte_size_string[ byte_size_string_iterator ] < '0' )
			 || ( byte_size_string[ byte_size_string_iterator ] > '9' ) )
			{
				break;
			}
			byte_size_string_iterator++;
		}
	}
	if( byte_size_string[ byte_size_string_iterator ] == ' ' )
	{
		byte_size_string_iterator++;
	}
	factor = ewfbyte_size_string_get_factor_system_character(
	          byte_size_string[ byte_size_string_iterator ] );

	if( factor < 0 )
	{
		notify_warning_printf( "%s: invalid factor.\n",
		 function );

		return( -1 );
	}
	byte_size_string_iterator++;

	if( ( byte_size_string[ byte_size_string_iterator ] == 'i' )
	 && ( byte_size_string[ byte_size_string_iterator + 1 ] == 'B' ) )
	{
		units = EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE;

		byte_size_string_iterator += 2;
	}
	else if( byte_size_string[ byte_size_string_iterator ] == 'B' )
	{
		units = EWFBYTE_SIZE_STRING_UNIT_MEGABYTE;

		byte_size_string_iterator++;
	}
	else
	{
		notify_warning_printf( "%s: invalid units.\n",
		 function );

		return( -1 );
	}
	if( factor > 0 )
	{
		if( remainder > 0 )
		{
			byte_size *= units;

			factor--;

			byte_size += ( remainder * 10 );
		}
		for( ; factor > 0; factor-- )
		{
			byte_size *= units;
		}
	}
	else if( remainder >= 0 )
	{
		notify_warning_printf( "%s: ignoring byte value remainder.\n",
		 function );
	}
	if( byte_size_string[ byte_size_string_iterator ] != '\0' )
	{
		notify_warning_printf( "%s: trailing data in byte size string.\n",
		 function );
	}
	*size = byte_size;

	return( 1 );
}
