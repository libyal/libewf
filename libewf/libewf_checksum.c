/*
 * Checksum functions
 *
 * Copyright (C) 2006-2015, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <types.h>

#if defined( HAVE_ZLIB ) || defined( ZLIB_DLL )
#include <zlib.h>
#endif

#include "libewf_checksum.h"
#include "libewf_libcerror.h"
#include "libewf_types.h"

#if defined( HAVE_ADLER32 ) && ( defined( HAVE_ZLIB ) || defined( ZLIB_DLL ) )

/* Calculates the little-endian Adler-32 of a buffer
 * It uses the initial value to calculate a new Adler-32
 * Returns 1 if successful or -1 on error
 */
int libewf_checksum_calculate_adler32(
     uint32_t *checksum_value,
     const uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     libcerror_error_t **error )
{
	static char *function = "libewf_checksum_calculate_adler32";

	if( checksum_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checksum value.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) UINT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	*checksum_value = adler32(
	                   (uLong) initial_value,
	                   (const Bytef *) buffer,
	                   (uInt) size );

	return( 1 );
}

#else

/* Calculates the little-endian Adler-32 of a buffer
 * It uses the initial value to calculate a new Adler-32
 * Returns 1 if successful or -1 on error
 */
int libewf_checksum_calculate_adler32(
     uint32_t *checksum_value,
     const uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     libcerror_error_t **error )
{
	static char *function = "libewf_checksum_calculate_adler32";
	size_t buffer_offset  = 0;
	uint32_t lower_word   = 0;
	uint32_t upper_word   = 0;
	uint32_t value_32bit  = 0;
	int block_index       = 0;

	if( checksum_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checksum value.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	lower_word = initial_value & 0xffff;
	upper_word = ( initial_value >> 16 ) & 0xffff;

	while( size >= 0x15b0 )
	{
		/* The modulo calculation is needed per 5552 (0x15b0) bytes
		 * 5552 / 16 = 347
		 */
		for( block_index = 0;
		     block_index < 347;
		     block_index++ )
		{
			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;
		}
		/* Optimized equivalent of:
		 * lower_word %= 0xfff1
		 */
		value_32bit = lower_word >> 16;
		lower_word &= 0x0000ffffUL;
		lower_word += ( value_32bit << 4 ) - value_32bit;

		if( lower_word > 65521 )
		{
			value_32bit = lower_word >> 16;
			lower_word &= 0x0000ffffUL;
			lower_word += ( value_32bit << 4 ) - value_32bit;
		}
		if( lower_word >= 65521 )
		{
			lower_word -= 65521;
		}
		/* Optimized equivalent of:
		 * upper_word %= 0xfff1
		 */
		value_32bit = upper_word >> 16;
		upper_word &= 0x0000ffffUL;
		upper_word += ( value_32bit << 4 ) - value_32bit;

		if( upper_word > 65521 )
		{
			value_32bit = upper_word >> 16;
			upper_word &= 0x0000ffffUL;
			upper_word += ( value_32bit << 4 ) - value_32bit;
		}
		if( upper_word >= 65521 )
		{
			upper_word -= 65521;
		}
		size -= 0x15b0;
	}
	if( size > 0 )
	{
		while( size > 16 )
		{
			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			size -= 16;
		}
		while( size > 0 )
		{
			lower_word += buffer[ buffer_offset++ ];
			upper_word += lower_word;

			size--;
		}
		/* Optimized equivalent of:
		 * lower_word %= 0xfff1
		 */
		value_32bit = lower_word >> 16;
		lower_word &= 0x0000ffffUL;
		lower_word += ( value_32bit << 4 ) - value_32bit;

		if( lower_word > 65521 )
		{
			value_32bit = lower_word >> 16;
			lower_word &= 0x0000ffffUL;
			lower_word += ( value_32bit << 4 ) - value_32bit;
		}
		if( lower_word >= 65521 )
		{
			lower_word -= 65521;
		}
		/* Optimized equivalent of:
		 * upper_word %= 0xfff1
		 */
		value_32bit = upper_word >> 16;
		upper_word &= 0x0000ffffUL;
		upper_word += ( value_32bit << 4 ) - value_32bit;

		if( upper_word > 65521 )
		{
			value_32bit = upper_word >> 16;
			upper_word &= 0x0000ffffUL;
			upper_word += ( value_32bit << 4 ) - value_32bit;
		}
		if( upper_word >= 65521 )
		{
			upper_word -= 65521;
		}
	}
	*checksum_value = ( upper_word << 16 ) | lower_word;

	return( 1 );
}

#endif /* defined( HAVE_ADLER32 ) && ( defined( HAVE_ZLIB ) || defined( ZLIB_DLL ) ) */

