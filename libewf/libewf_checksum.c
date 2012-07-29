/*
 * Checksum functions
 *
 * Copyright (c) 2006-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include "libewf_checksum.h"
#include "libewf_libcerror.h"

/* The largest primary (or scalar) available
 * supported by a single load and store instruction
 */
typedef unsigned long int libewf_aligned_t;

/* Calculates the little-endian Adler-32 of a buffer
 * It uses the initial value to calculate a new Adler-32
 * Returns 1 if successful or -1 on error
 */
int libewf_checksum_calculate_little_endian_adler32(
     uint32_t *checksum_value,
     const uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     libcerror_error_t **error )
{
	libewf_aligned_t *aligned_buffer_iterator = NULL;
	uint8_t *buffer_iterator                    = NULL;
	static char *function                       = "libewf_checksum_calculate_little_endian_adler32";
	libewf_aligned_t value_aligned            = 0;
	uint32_t value_32bit                        = 0;
	uint8_t alignment_count                     = 0;
	uint8_t alignment_size                      = 0;
	uint8_t byte_count                          = 0;
	uint8_t byte_order                          = 0;
	uint8_t byte_size                           = 0;

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
	*checksum_value = initial_value;

	buffer_iterator = (uint8_t *) buffer;

	/* Only optimize when there is the alignment is a multitude of 32-bit
	 * and for buffers larger than the alignment
	 */
	if( ( ( sizeof( libewf_aligned_t ) % 4 ) == 0 )
	 && ( size > ( 2 * sizeof( libewf_aligned_t ) ) ) )
	{
		/* Align the buffer iterator
		 */
		alignment_size = (uint8_t) ( (intptr_t) buffer_iterator % sizeof( libewf_aligned_t ) );

		byte_size = alignment_size;

		while( byte_size != 0 )
		{
			value_32bit = 0;
			byte_count  = 1;

			if( byte_size >= 4 )
			{
				value_32bit |= buffer_iterator[ 3 ];
				value_32bit <<= 8;

				byte_count++;
			}
			if( byte_size >= 3 )
			{
				value_32bit |= buffer_iterator[ 2 ];
				value_32bit <<= 8;

				byte_count++;
			}
			if( byte_size >= 2 )
			{
				value_32bit |= buffer_iterator[ 1 ];
				value_32bit <<= 8;

				byte_count++;
			}
			value_32bit |= buffer_iterator[ 0 ];

			buffer_iterator += byte_count;
			byte_size       -= byte_count;

			*checksum_value ^= value_32bit;
		}
		aligned_buffer_iterator = (libewf_aligned_t *) buffer_iterator;

		size -= alignment_size;

		if( *buffer_iterator != (uint8_t) ( *aligned_buffer_iterator & 0xff ) )
		{
			byte_order = _BYTE_STREAM_ENDIAN_BIG;
		}
		else
		{
			byte_order = _BYTE_STREAM_ENDIAN_LITTLE;
		}
		/* Determine the aligned XOR value
		 */
		while( size > sizeof( libewf_aligned_t ) )
		{
			value_aligned ^= *aligned_buffer_iterator;

			aligned_buffer_iterator++;

			size -= sizeof( libewf_aligned_t );
		}
		/* Align the aligned XOR value with the 32-bit XOR value
		 */
		if( alignment_size > 0 )
		{
			byte_count     = ( alignment_size % 4 ) * 8;
			alignment_count = ( sizeof( libewf_aligned_t ) - alignment_size ) * 8;

			if( byte_order == _BYTE_STREAM_ENDIAN_BIG )
			{
				/* Shift twice to set unused bytes to 0
				 */
				value_32bit = (uint32_t) ( ( value_aligned >> alignment_count ) << byte_count );

				/* Strip-off the used part of the aligned value
				 */
				value_aligned <<= byte_count;
			}
			else if( byte_order == _BYTE_STREAM_ENDIAN_LITTLE )
			{
				value_32bit = (uint32_t) ( value_aligned << byte_count );

				/* Strip-off the used part of the aligned value
				 */
				value_aligned >>= alignment_count;
			}
			*checksum_value ^= value_32bit;
		}
		/* Update the 32-bit XOR value with the aligned XOR value
		 */
		byte_size = (uint8_t) sizeof( libewf_aligned_t );

		while( byte_size != 0 )
		{
			byte_count = ( ( byte_size / 4 ) - 1 ) * 32;

			if( byte_order == _BYTE_STREAM_ENDIAN_BIG )
			{
				value_32bit = (uint32_t) ( value_aligned >> byte_count );

				/* Change big-endian into little-endian
				 */
				value_32bit = ( ( value_32bit & 0x00ff ) << 24 )
				            | ( ( value_32bit & 0xff00 ) << 8 )
				            | ( ( value_32bit >> 8 ) & 0xff00 )
				            | ( ( value_32bit >> 24 ) & 0x00ff );

				value_aligned <<= byte_count;
			}
			else if( byte_order == _BYTE_STREAM_ENDIAN_LITTLE )
			{
				value_32bit = (uint32_t) value_aligned;

				value_aligned >>= byte_count;
			}
			byte_size -= 4;

			*checksum_value ^= value_32bit;
		}
		/* Re-align the buffer iterator
		 */
		buffer_iterator = (uint8_t *) aligned_buffer_iterator;

		byte_size = 4 - ( alignment_size % 4 );

		if( byte_size != 4 )
		{
			value_32bit   = buffer_iterator[ 0 ];
			value_32bit <<= 8;

			if( byte_size >= 2 )
			{
				value_32bit |= buffer_iterator[ 1 ];
			}
			value_32bit <<= 8;

			if( byte_size >= 3 )
			{
				value_32bit |= buffer_iterator[ 2 ];
			}
			value_32bit <<= 8;

			buffer_iterator += byte_size;
			size            -= byte_size;

			*checksum_value ^= value_32bit;
		}
	}
	while( size > 0 )
	{
		value_32bit = 0;
		byte_count  = 1;

		if( size >= 4 )
		{
			value_32bit |= buffer_iterator[ 3 ];
			value_32bit <<= 8;

			byte_count++;
		}
		if( size >= 3 )
		{
			value_32bit |= buffer_iterator[ 2 ];
			value_32bit <<= 8;

			byte_count++;
		}
		if( size >= 2 )
		{
			value_32bit |= buffer_iterator[ 1 ];
			value_32bit <<= 8;

			byte_count++;
		}
		value_32bit |= buffer_iterator[ 0 ];

		buffer_iterator += byte_count;
		size            -= byte_count;

		*checksum_value ^= value_32bit;
	}
	return( 1 );
}

