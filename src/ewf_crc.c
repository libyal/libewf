/*
 * EWF CRC handling
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"

#include "ewf_crc.h"

/* Allocates memory for a new ewf crc
 * Return a pointer to the new instance, NULL on error
 */
EWF_CRC *ewf_crc_alloc( void )
{
	EWF_CRC *crc = NULL;

	crc = (EWF_CRC *) libewf_alloc_cleared( EWF_CRC_SIZE );

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_alloc: unable to allocate ewf_crc.\n" );

		return( NULL );
	}
	return( crc );
}

/* Frees memory of a crc
 */
void ewf_crc_free( EWF_CRC *crc )
{
	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_free: invalid crc.\n" );

		return;
	}
	libewf_free( crc );
}

/* Reads the CRC from a file descriptor
 * Return a pointer to the new instance, NULL on error
 */
EWF_CRC *ewf_crc_read( int file_descriptor )
{
	uint8_t buffer[4];

	EWF_CRC *crc  = NULL;
	uint32_t size = 0;
	int32_t count = 0;

	crc = ewf_crc_alloc();

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_read: unable to create crc.\n" );

		return( NULL );
	}
	size  = EWF_CRC_SIZE;
	count = libewf_read( file_descriptor, buffer, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_read: unable to read crc.\n" );

		ewf_crc_free( crc );

		return( NULL );
	}
	*crc = convert_32bit( buffer );

	return( crc );
}

/* Writes the CRC to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_crc_write( EWF_CRC *crc, int file_descriptor )
{
	uint8_t buffer[4];

	int32_t count = 0;
	uint32_t size = 0;

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_write: invalid crc.\n" );

		return( -1 );
	}
	revert_32bit( *crc, buffer );

	size  = EWF_CRC_SIZE;
	count = libewf_write( file_descriptor, buffer, size );

	if( count < size )
	{
		return( -1 );
	}
	return( count );
}

/* Calculates the EWF CRC
 * Return a pointer to the new instance, NULL on error
 *
 * The original algorithm was taken from the ASR data web site
 * When calling this function to start a new CRC "previous_key" should be 1
 */
EWF_CRC *ewf_crc_calculate( void *buffer, int32_t buffer_size, uint32_t previous_key )
{
	EWF_CRC *crc      = NULL;
	uint8_t *cbuffer  = NULL;
	uint32_t b        = previous_key & 0xffff;
	uint32_t d        = ( previous_key >> 16 ) & 0xffff;
	uint64_t iterator = 0;

	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_calculate: invalid buffer.\n" );

		return( NULL );
	}
	cbuffer = (uint8_t *) buffer;
	crc     = ewf_crc_alloc();

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_calculate: unable to create crc.\n" );

		return( NULL );
	}
	while( iterator < buffer_size )
	{
		b += cbuffer[ iterator ];
		d += b;
    
		if( ( iterator != 0 ) && ( ( iterator % 0x15b0 == 0 ) || ( iterator == buffer_size - 1 ) ) )
		{
			b = b % 0xfff1;
			d = d % 0xfff1;
		}
		iterator++;
	}
	*crc = ( ( d << 16 ) | b );

	return( crc );
}

