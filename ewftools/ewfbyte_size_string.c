/*
 * ewfbyte_size_string
 * Byte size string functions for the libewf tools
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include "../libewf/libewf_includes.h"

#include <errno.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_INTTYPES_H )
#include <inttypes.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "../libewf/libewf_common.h"
#include "../libewf/libewf_notify.h"
#include "../libewf/libewf_string.h"

#include "ewfbyte_size_string.h"

/* Determines the factor string of a certain factor value
 * Returns the string if successful or NULL on error
 */
const libewf_char_t *ewfbyte_size_string_get_factor_string(
                      int8_t factor )
{
	switch( factor )
	{
		case 0:
			return( _S_LIBEWF_CHAR( "" ) );
		case 1:
			return( _S_LIBEWF_CHAR( "K" ) );
		case 2:
			return( _S_LIBEWF_CHAR( "M" ) );
		case 3:
			return( _S_LIBEWF_CHAR( "G" ) );
		case 4:
			return( _S_LIBEWF_CHAR( "T" ) );
		case 5:
			return( _S_LIBEWF_CHAR( "P" ) );
		case 6:
			return( _S_LIBEWF_CHAR( "E" ) );
		case 7:
			return( _S_LIBEWF_CHAR( "Z" ) );
		case 8:
			return( _S_LIBEWF_CHAR( "Y" ) );
		default :
			break;
	}
	return( NULL );
}

/* Determines the factor from a factor string
 * Returns the factor if successful or -1 on error
 */
int8_t ewfbyte_size_string_get_factor(
        libewf_char_t factor )
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
     libewf_char_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t size,
     int units )
{
	const libewf_char_t *factor_string = NULL;
	const libewf_char_t *units_string  = NULL;
	static char *function              = "ewfbyte_size_string_create";
	ssize_t print_count                = 0;
	uint64_t factored_size             = 0;
	uint64_t last_factored_size        = 0;
	int8_t factor                      = 0;
	int8_t remainder                   = -1;

	if( byte_size_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid byte size string.\n",
		 function );

		return( -1 );
	}
	/* Minimum of 4 digits and seperator, space, 3 letter unit, end of string
	 */
	if( byte_size_string_length < 9 )
	{
		LIBEWF_WARNING_PRINT( "%s: byte size string too small.\n",
		 function );

		return( -1 );
	}
	if( ( size < 1024 )
	 || ( units == EWFBYTE_SIZE_STRING_UNIT_MEGABYTE ) )
	{
		units_string = _S_LIBEWF_CHAR( "B" );
	}
	else if( units == EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE )
	{
		units_string = _S_LIBEWF_CHAR( "iB" );
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
			LIBEWF_WARNING_PRINT( "%s: a size with a factor larger than 8 currently not supported.\n",
			 function );

			return( -1 );
		}
	}
	factor_string = ewfbyte_size_string_get_factor_string(
	                 factor );

	if( factor_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create factor string.\n",
		 function );

		return( -1 );
	}
	if( remainder > 9 )
	{
		remainder = 9;
	}
	if( remainder >= 0 )
	{
		print_count = libewf_string_snprintf(
		               byte_size_string,
		               byte_size_string_length,
		               _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIu64 )
		               _S_LIBEWF_CHAR( ".%" ) _S_LIBEWF_CHAR( PRIu8 )
		               _S_LIBEWF_CHAR( " %" ) _S_LIBEWF_CHAR( PRIs_EWF )
		               _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIs_EWF ),
		               factored_size,
		               remainder,
		               factor_string,
		               units_string );
	}
	else
	{
		print_count = libewf_string_snprintf(
		               byte_size_string,
		               byte_size_string_length,
		               _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIu64 )
		               _S_LIBEWF_CHAR( " %" ) _S_LIBEWF_CHAR( PRIs_EWF )
		               _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIs_EWF ),
		               factored_size,
		               factor_string,
		               units_string );
	}
	if( print_count < 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set byte size string.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Converts a human readable byte size string into a value
 * Returns 1 if successful or -1 on error
 */
int ewfbyte_size_string_convert(
     libewf_char_t *byte_size_string,
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
		LIBEWF_WARNING_PRINT( "%s: invalid byte size string.\n",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size.\n",
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

		remainder = byte_size_string[ byte_size_string_iterator ] - '0';

		byte_size_string_iterator++;
	}
	if( byte_size_string[ byte_size_string_iterator ] == ' ' )
	{
		byte_size_string_iterator++;
	}
	factor = ewfbyte_size_string_get_factor(
	          byte_size_string[ byte_size_string_iterator ] );

	if( factor < 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid factor.\n",
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
		LIBEWF_WARNING_PRINT( "%s: invalid units.\n",
		 function );

		return( -1 );
	}
	if( factor > 0 )
	{
		if( remainder > 0 )
		{
			byte_size *= units;

			factor--;

			byte_size += ( remainder * 100 );
		}
		for( ; factor > 0; factor-- )
		{
			byte_size *= units;
		}
	}
	else if( remainder >= 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: ignoring byte value remainder.\n",
		 function );
	}
	if( byte_size_string[ byte_size_string_iterator ] != '\0' )
	{
		LIBEWF_WARNING_PRINT( "%s: trailing data in byte size string.\n",
		 function );
	}
	*size = byte_size;

	return( 1 );
}

