/*
 * EWF CRC handling
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

#if defined( HAVE_ZLIB_H ) && defined( HAVE_LIBZ )
#include <zlib.h>
#endif

#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"

#include "ewf_crc.h"

/* Reads the CRC from a file descriptor
 * Returns the amount of bytes read if successful, or -1 on error
 */
ssize_t ewf_crc_read( EWF_CRC *crc, int file_descriptor )
{
	uint8_t buffer[ 4 ];

	ssize_t count = 0;
	size_t size   = EWF_CRC_SIZE;

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_read: invalid CRC.\n" );

		return( -1 );
	}
	count = libewf_common_read( file_descriptor, buffer, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_read: unable to read CRC.\n" );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( crc, buffer ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_read: unable to convert CRC value.\n" );

		return( -1 );
	}
	return( count );
}

/* Writes the CRC to a file descriptor
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t ewf_crc_write( EWF_CRC *crc, int file_descriptor )
{
	uint8_t buffer[ 4 ];

	ssize_t count = 0;
	size_t size   = EWF_CRC_SIZE;

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_write: invalid CRC.\n" );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( *crc, buffer ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_write: unable to revert CRC value.\n" );

		return( -1 );
	}
	count = libewf_common_write( file_descriptor, buffer, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_write: error writing CRC.\n" );

		return( -1 );
	}
	return( count );
}

/* Calculates the EWF CRC
 * When calling this function to start a new CRC "previous_key" should be 1
 * Returns 1 if calculation is successful, or -1 on error
 *
 * The original algorithm was taken from the ASR data web site
 */
int8_t ewf_crc_calculate( EWF_CRC *crc, uint8_t *buffer, size_t size, uint32_t previous_key )
{
	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_calculate: invalid CRC.\n" );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_calculate: invalid buffer.\n" );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "ewf_crc_calculate: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	*crc = (EWF_CRC) adler32( (uLong) previous_key, (const Bytef *) buffer, (uInt) size );

	return( 1 );
}

