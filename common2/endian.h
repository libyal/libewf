/*
 * Endian functions
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

#if !defined( _ENDIAN_H )
#define _ENDIAN_H

#include <common.h>

#include <libewf/types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define endian_big_convert_16bit( value, bytes ) \
	value = ( (uint16_t) ( bytes[ 0 ] ) << 8 ) \
	      | (uint16_t) ( bytes[ 1 ] )

#define endian_big_convert_32bit( value, bytes ) \
	value = ( (uint32_t) ( bytes[ 0 ] ) << 24 ) \
	      | ( (uint32_t) ( bytes[ 1 ] ) << 16 ) \
	      | ( (uint32_t) ( bytes[ 2 ] ) << 8 ) \
	      | (uint32_t) ( bytes[ 3 ] )

#define endian_big_convert_64bit( value, bytes ) \
	value = ( (uint64_t) ( bytes[ 0 ] ) << 56 ) \
	      | ( (uint64_t) ( bytes[ 1 ] ) << 48 ) \
	      | ( (uint64_t) ( bytes[ 2 ] ) << 40 ) \
	      | ( (uint64_t) ( bytes[ 3 ] ) << 32 ) \
	      | ( (uint64_t) ( bytes[ 4 ] ) << 24 ) \
	      | ( (uint64_t) ( bytes[ 5 ] ) << 16 ) \
	      | ( (uint64_t) ( bytes[ 6 ] ) << 8 ) \
	      | (uint64_t) ( bytes[ 7 ] )

#define endian_little_convert_16bit( value, bytes ) \
	value = ( (uint16_t) ( bytes[ 1 ] ) << 8 ) \
	      | (uint16_t) ( bytes[ 0 ] )

#define endian_little_convert_32bit( value, bytes ) \
	value = ( (uint32_t) ( bytes[ 3 ] ) << 24 ) \
	      | ( (uint32_t) ( bytes[ 2 ] ) << 16 ) \
	      | ( (uint32_t) ( bytes[ 1 ] ) << 8 ) \
	      | (uint32_t) ( bytes[ 0 ] )

#define endian_little_convert_64bit( value, bytes ) \
	value = ( (uint64_t) ( bytes[ 7 ] ) << 56 ) \
	      | ( (uint64_t) ( bytes[ 6 ] ) << 48 ) \
	      | ( (uint64_t) ( bytes[ 5 ] ) << 40 ) \
	      | ( (uint64_t) ( bytes[ 4 ] ) << 32 ) \
	      | ( (uint64_t) ( bytes[ 3 ] ) << 24 ) \
	      | ( (uint64_t) ( bytes[ 2 ] ) << 16 ) \
	      | ( (uint64_t) ( bytes[ 1 ] ) << 8 ) \
	      | (uint64_t) ( bytes[ 0 ] )

#define endian_big_revert_16bit( bytes, value ) \
	bytes[ 0 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( value & 0x0ff )

#define endian_big_revert_32bit( bytes, value ) \
	bytes[ 0 ] = (uint8_t) ( ( value >> 24 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( ( value >> 16 ) & 0x0ff ); \
	bytes[ 2 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 3 ] = (uint8_t) ( value & 0x0ff )

#define endian_big_revert_64bit( bytes, value ) \
	bytes[ 0 ] = (uint8_t) ( ( value >> 56 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( ( value >> 48 ) & 0x0ff ); \
	bytes[ 2 ] = (uint8_t) ( ( value >> 40 ) & 0x0ff ); \
	bytes[ 3 ] = (uint8_t) ( ( value >> 32 ) & 0x0ff ); \
	bytes[ 4 ] = (uint8_t) ( ( value >> 24 ) & 0x0ff ); \
	bytes[ 5 ] = (uint8_t) ( ( value >> 16 ) & 0x0ff ); \
	bytes[ 6 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 7 ] = (uint8_t) ( value & 0x0ff )

#define endian_little_revert_16bit( bytes, value ) \
	bytes[ 1 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 0 ] = (uint8_t) ( value & 0x0ff )

#define endian_little_revert_32bit( bytes, value ) \
	bytes[ 3 ] = (uint8_t) ( ( value >> 24 ) & 0x0ff ); \
	bytes[ 2 ] = (uint8_t) ( ( value >> 16 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 0 ] = (uint8_t) ( value & 0x0ff )

#define endian_little_revert_64bit( bytes, value ) \
	bytes[ 7 ] = (uint8_t) ( ( value >> 56 ) & 0x0ff ); \
	bytes[ 6 ] = (uint8_t) ( ( value >> 48 ) & 0x0ff ); \
	bytes[ 5 ] = (uint8_t) ( ( value >> 40 ) & 0x0ff ); \
	bytes[ 4 ] = (uint8_t) ( ( value >> 32 ) & 0x0ff ); \
	bytes[ 3 ] = (uint8_t) ( ( value >> 24 ) & 0x0ff ); \
	bytes[ 2 ] = (uint8_t) ( ( value >> 16 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 0 ] = (uint8_t) ( value & 0x0ff )

#if defined( __cplusplus )
}
#endif

#endif

