/*
 * EWF error2 section
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

#include "ewf_crc.h"
#include "ewf_error2.h"

/* Reads the error2 from a file descriptor
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t ewf_error2_read( EWF_ERROR2 *error2, int file_descriptor )
{
	ssize_t count = 0;
	size_t size   = EWF_ERROR2_SIZE;

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_read: invalid error2.\n" );

		return( -1 );
	}
	count = libewf_common_read( file_descriptor, error2, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_read: unable to read error2.\n" );

		return( -1 );
	}
	return( count );
}

/* Reads the ewf error2 sectors from a file descriptor
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t ewf_error2_sectors_read( EWF_ERROR2_SECTOR *sectors, int file_descriptor, uint32_t amount )
{
	ssize_t count = 0;
	size_t size   = EWF_ERROR2_SECTOR_SIZE * amount;

	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_read: invalid error2 sectors.\n" );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_read: invalid amount exceeds maximum.\n" );

		return( -1 );
	}
	count = libewf_common_read( file_descriptor, sectors, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_read: unable to read error2 sectors.\n" );

		return( -1 );
	}
	return( count );
}

/* Writes the error2 to a file descriptor
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t ewf_error2_write( EWF_ERROR2 *error2, int file_descriptor )
{
	EWF_CRC crc   = 0;
	ssize_t count = 0;
	size_t size   = EWF_ERROR2_SIZE;

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_write: invalid error2.\n" );

		return( -1 );
	}
	if( ewf_crc_calculate( &crc, (uint8_t *) error2, ( size - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( crc, error2->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_write: unable to revert CRC value.\n" );

		return( -1 );
	}
	count = libewf_common_write( file_descriptor, error2, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_write: unable to write error2.\n" );

		return( -1 );
	}
	return( count );
}

/* Writes the sectors to a file descriptor
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t ewf_error2_sectors_write( EWF_ERROR2_SECTOR *sectors, int file_descriptor, uint32_t amount )
{
	EWF_CRC crc       = 0;
	ssize_t count     = 0;
	ssize_t crc_count = 0;
	size_t size       = EWF_ERROR2_SECTOR_SIZE * amount;

	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_write: invalid sectors.\n" );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_write: invalid amount exceeds maximum.\n" );

		return( -1 );
	}
	count = libewf_common_write( file_descriptor, sectors, size );

	if( count < (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_write: unable write error2 sectors.\n" );

		return( -1 );
	}
	if( ewf_crc_calculate( &crc, (uint8_t *) sectors, size, 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	crc_count = ewf_crc_write( &crc, file_descriptor );

	if( crc_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_write: unable to write CRC.\n" );

		return( -1 );
	}
	return( count + crc_count );
}

