/*
 * Byte size string functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include "byte_size_string.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcstring.h"

/* Creates a human readable byte size string
 * Returns 1 if successful or -1 on error
 */
int byte_size_string_create(
     libcstring_system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t size,
     int units,
     libcerror_error_t **error )
{
	const libcstring_system_character_t *factor_string = NULL;
	const libcstring_system_character_t *units_string  = NULL;
	static char *function                              = "byte_size_string_create";
	ssize_t print_count                                = 0;
	uint64_t factored_size                             = 0;
	uint64_t last_factored_size                        = 0;
	int8_t factor                                      = 0;
	int8_t remainder                                   = -1;
	int decimal_point                                  = 0;

	if( byte_size_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte size string.",
		 function );

		return( -1 );
	}
	/* Minimum of 4 digits and separator, space, 3 letter unit, end of string
	 */
	if( byte_size_string_length < 9 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: byte size string too small.",
		 function );

		return( -1 );
	}
	if( ( size < 1024 )
	 || ( units == BYTE_SIZE_STRING_UNIT_MEGABYTE ) )
	{
		units_string = _LIBCSTRING_SYSTEM_STRING( "B" );
	}
	else if( units == BYTE_SIZE_STRING_UNIT_MEBIBYTE )
	{
		units_string = _LIBCSTRING_SYSTEM_STRING( "iB" );
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
	}
	if( factor > 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: factor size greater than 8 unsupported.",
		 function );

		return( -1 );
	}
	switch( factor )
	{
		case 0:
			factor_string = _LIBCSTRING_SYSTEM_STRING( "" );
			break;
		case 1:
			factor_string = _LIBCSTRING_SYSTEM_STRING( "K" );
			break;
		case 2:
			factor_string = _LIBCSTRING_SYSTEM_STRING( "M" );
			break;
		case 3:
			factor_string = _LIBCSTRING_SYSTEM_STRING( "G" );
			break;
		case 4:
			factor_string = _LIBCSTRING_SYSTEM_STRING( "T" );
			break;
		case 5:
			factor_string = _LIBCSTRING_SYSTEM_STRING( "P" );
			break;
		case 6:
			factor_string = _LIBCSTRING_SYSTEM_STRING( "E" );
			break;
		case 7:
			factor_string = _LIBCSTRING_SYSTEM_STRING( "Z" );
			break;
		case 8:
			factor_string = _LIBCSTRING_SYSTEM_STRING( "Y" );
			break;
	}
	if( remainder > 9 )
	{
		remainder = 9;
	}
	if( remainder >= 0 )
	{
		if( libclocale_locale_get_decimal_point(
		     &decimal_point,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve locale decimal point.",
			 function );

			return( -1 );
		}
		print_count = libcstring_system_string_sprintf(
		               byte_size_string,
		               byte_size_string_length,
		               _LIBCSTRING_SYSTEM_STRING( "%" ) _LIBCSTRING_SYSTEM_STRING( PRIu64 )
		               _LIBCSTRING_SYSTEM_STRING( "%" ) _LIBCSTRING_SYSTEM_STRING( PRIc_LIBCSTRING_SYSTEM )
		               _LIBCSTRING_SYSTEM_STRING( "%" ) _LIBCSTRING_SYSTEM_STRING( PRIu8 )
		               _LIBCSTRING_SYSTEM_STRING( " %" ) _LIBCSTRING_SYSTEM_STRING( PRIs_LIBCSTRING_SYSTEM )
		               _LIBCSTRING_SYSTEM_STRING( "%" ) _LIBCSTRING_SYSTEM_STRING( PRIs_LIBCSTRING_SYSTEM ),
		               factored_size,
		               (libcstring_system_character_t) decimal_point,
		               remainder,
		               factor_string,
		               units_string );
	}
	else
	{
		print_count = libcstring_system_string_sprintf(
		               byte_size_string,
		               byte_size_string_length,
		               _LIBCSTRING_SYSTEM_STRING( "%" ) _LIBCSTRING_SYSTEM_STRING( PRIu64 )
		               _LIBCSTRING_SYSTEM_STRING( " %" ) _LIBCSTRING_SYSTEM_STRING( PRIs_LIBCSTRING_SYSTEM )
		               _LIBCSTRING_SYSTEM_STRING( "%" ) _LIBCSTRING_SYSTEM_STRING( PRIs_LIBCSTRING_SYSTEM ),
		               factored_size,
		               factor_string,
		               units_string );
	}
	if( ( print_count < 0 )
	 || ( (size_t) print_count > byte_size_string_length ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set byte size string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Converts a human readable byte size string into a value
 * Returns 1 if successful or -1 on error
 */
int byte_size_string_convert(
     const libcstring_system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t *size,
     libcerror_error_t **error )
{
	static char *function            = "byte_size_string_convert";
	size_t byte_size_string_iterator = 0;
	uint64_t byte_size               = 0;
	int8_t factor                    = 0;
	int8_t remainder                 = -1;
	int decimal_point                = 0;
	int units                        = 0;

	if( byte_size_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte size string.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	if( libclocale_locale_get_decimal_point(
	     &decimal_point,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve locale decimal point.",
		 function );

		return( -1 );
	}
	while( byte_size_string_iterator < byte_size_string_length )
	{
		if( ( byte_size_string[ byte_size_string_iterator ] < (libcstring_system_character_t) '0' )
		 || ( byte_size_string[ byte_size_string_iterator ] > (libcstring_system_character_t) '9' ) )
		{
			break;
		}
		byte_size *= 10;
		byte_size += ( byte_size_string[ byte_size_string_iterator ] - (libcstring_system_character_t) '0' );

		byte_size_string_iterator++;
	}
	if( byte_size_string[ byte_size_string_iterator ] == (libcstring_system_character_t) decimal_point )
	{
		byte_size_string_iterator++;

		if( ( byte_size_string[ byte_size_string_iterator ] >= (libcstring_system_character_t) '0' )
		 && ( byte_size_string[ byte_size_string_iterator ] <= (libcstring_system_character_t) '9' ) )
		{
			remainder = (int8_t) ( byte_size_string[ byte_size_string_iterator ] - (libcstring_system_character_t) '0' );

			byte_size_string_iterator++;
		}
		remainder *= 10;

		if( ( byte_size_string[ byte_size_string_iterator ] >= (libcstring_system_character_t) '0' )
		 && ( byte_size_string[ byte_size_string_iterator ] <= (libcstring_system_character_t) '9' ) )
		{
			remainder += (int8_t) ( byte_size_string[ byte_size_string_iterator ] - (libcstring_system_character_t) '0' );

			byte_size_string_iterator++;
		}
		/* Ignore more than 2 digits after separator
		 */
		while( byte_size_string_iterator < byte_size_string_length )
		{
			if( ( byte_size_string[ byte_size_string_iterator ] < (libcstring_system_character_t) '0' )
			 || ( byte_size_string[ byte_size_string_iterator ] > (libcstring_system_character_t) '9' ) )
			{
				break;
			}
			byte_size_string_iterator++;
		}
	}
	if( byte_size_string[ byte_size_string_iterator ] == (libcstring_system_character_t) ' ' )
	{
		byte_size_string_iterator++;
	}
	switch( byte_size_string[ byte_size_string_iterator ] )
	{
		case 'k':
		case 'K':
			factor = 1;
			break;
		case 'm':
		case 'M':
			factor = 2;
			break;
		case 'g':
		case 'G':
			factor = 3;
			break;
		case 't':
		case 'T':
			factor = 4;
			break;
		case 'p':
		case 'P':
			factor = 5;
			break;
		case 'e':
		case 'E':
			factor = 6;
			break;
		case 'z':
		case 'Z':
			factor = 7;
			break;
		case 'y':
		case 'Y':
			factor = 8;
			break;
	}
	if( factor < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid factor.",
		 function );

		return( -1 );
	}
	else if( factor > 1 )
	{
		byte_size_string_iterator++;
	}
	if( byte_size_string_iterator >= byte_size_string_length )
	{
		units = BYTE_SIZE_STRING_UNIT_MEBIBYTE;
	}
	else if( ( byte_size_string[ byte_size_string_iterator ] == (libcstring_system_character_t) 'i' )
	      && ( byte_size_string[ byte_size_string_iterator + 1 ] == (libcstring_system_character_t) 'B' ) )
	{
		units = BYTE_SIZE_STRING_UNIT_MEBIBYTE;

		byte_size_string_iterator += 2;
	}
	else if( byte_size_string[ byte_size_string_iterator ] == (libcstring_system_character_t) 'B' )
	{
		units = BYTE_SIZE_STRING_UNIT_MEGABYTE;

		byte_size_string_iterator++;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid units.",
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
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( remainder >= 0 )
	{
		libcnotify_printf(
		 "%s: ignoring byte value remainder.\n",
		 function );
	}
#endif
#if defined( HAVE_VERBOSE_OUTPUT )
	if( ( byte_size_string[ byte_size_string_iterator ] != 0 )
	 && ( byte_size_string[ byte_size_string_iterator ] != (libcstring_system_character_t) ' ' )
	 && ( byte_size_string[ byte_size_string_iterator ] != (libcstring_system_character_t) '\n' )
	 && ( byte_size_string[ byte_size_string_iterator ] != (libcstring_system_character_t) '\r' ) )
	{
		libcnotify_printf(
		 "%s: trailing data in byte size string.\n",
		 function );
	}
#endif
	*size = byte_size;

	return( 1 );
}

