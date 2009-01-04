/*
 * Endian function definition for libewf
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#include "libewf_includes.h"
#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"

/* Converts a byte array into a 16bit little endian value
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_endian_convert_16bit( uint16_t *value, uint8_t *bytes )
{
	if( value == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_convert_16bit: invalid value.\n" );

		return( -1 );
	}
	if( bytes == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_convert_16bit: invalid bytes.\n" );

		return( -1 );
	}
	*value   = bytes[ 1 ];
	*value <<= 8;
	*value  |= bytes[ 0 ];

	return( 1 );
}

/* Converts a byte array into a 32bit little endian value
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_endian_convert_32bit( uint32_t *value, uint8_t *bytes )
{
	if( value == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_convert_32bit: invalid value.\n" );

		return( -1 );
	}
	if( bytes == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_convert_32bit: invalid bytes.\n" );

		return( -1 );
	}
	*value   = bytes[ 3 ];
	*value <<= 8;
	*value  |= bytes[ 2 ];
	*value <<= 8;
	*value  |= bytes[ 1 ];
	*value <<= 8;
	*value  |= bytes[ 0 ];

	return( 1 );
}

/* Converts a byte array into a 64bit little endian value
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_endian_convert_64bit( uint64_t *value, uint8_t *bytes )
{
	if( value == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_convert_64bit: invalid value.\n" );

		return( -1 );
	}
	if( bytes == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_convert_64bit: invalid bytes.\n" );

		return( -1 );
	}
	*value   = bytes[ 7 ];
	*value <<= 8;
	*value  |= bytes[ 6 ];
	*value <<= 8;
	*value  |= bytes[ 5 ];
	*value <<= 8;
	*value  |= bytes[ 4 ];
	*value <<= 8;
	*value  |= bytes[ 3 ];
	*value <<= 8;
	*value  |= bytes[ 2 ];
	*value <<= 8;
	*value  |= bytes[ 1 ];
	*value <<= 8;
	*value  |= bytes[ 0 ];

	return( 1 );
}

/* Reverts a 16bit value into a little endian byte array
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_endian_revert_16bit( uint16_t value, uint8_t *bytes )
{
	if( bytes == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_revert_16bit: invalid bytes.\n" );

		return( -1 );
	}
	bytes[ 0 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 1 ] = ( uint8_t ) ( value & 0x0ff );

	return( 1 );
}

/* Reverts a 32bit value into a little endian byte array
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_endian_revert_32bit( uint32_t value, uint8_t *bytes )
{
	if( bytes == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_revert_32bit: invalid bytes.\n" );

		return( -1 );
	}
	bytes[ 0 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 1 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 2 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 3 ] = ( uint8_t ) ( value & 0x0ff );

	return( 1 );
}

/* Reverts a 64bit value into a little endian byte array
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_endian_revert_64bit( uint64_t value, uint8_t *bytes )
{
	if( bytes == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_revert_64bit: invalid bytes.\n" );

		return( -1 );
	}
	bytes[ 0 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 1 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 2 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 3 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 4 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 5 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 6 ] = ( uint8_t ) ( value & 0x0ff );
	value    >>= 8;
	bytes[ 7 ] = ( uint8_t ) ( value & 0x0ff );

	return( 1 );
}

/* Swaps the byte order of byte pairs within a buffer of a certain size
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_endian_swap_byte_pairs( uint8_t *buffer, size_t size )
{
	uint8_t byte    = 0;
	size_t iterator = 0;

	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_swap_byte_pairs: invalid buffer.\n" );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_endian_swap_byte_pairs: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	for( iterator = 0; iterator < size; iterator += 2 )
	{
		byte                   = buffer[ iterator ];
		buffer[ iterator ]     = buffer[ iterator + 1 ];
		buffer[ iterator + 1 ] = byte;
	}
	return( 1 );
}

