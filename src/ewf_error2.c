/*
 * EWF error2 section
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

#include <stdlib.h>

#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"

#include "ewf_crc.h"
#include "ewf_error2.h"

/* Allocates memory for a new ewf error2 struct
 * Return a pointer to the new instance, NULL on error
 */
EWF_ERROR2 *ewf_error2_alloc( void )
{
	EWF_ERROR2 *error2 = NULL;

	error2 = (EWF_ERROR2 *) libewf_alloc_cleared( EWF_ERROR2_SIZE );

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_alloc: unable to allocate ewf_error2.\n" );

		return( NULL );
	}
	return( error2 );
}

/* Allocates memory for a buffer of ewf error2 sectors 
 * Return a pointer to the new instance, NULL on error
 */
EWF_ERROR2_SECTOR *ewf_error2_sectors_alloc( uint32_t amount )
{
	EWF_ERROR2_SECTOR *sectors = NULL;
	uint32_t size              = 0;

	size    = EWF_ERROR2_SECTOR_SIZE * amount;
	sectors = (EWF_ERROR2_SECTOR *) libewf_alloc_cleared( size );

	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_alloc: unable to allocate sectors.\n" );

		return( NULL );
	}
	return( sectors );
}

/* Reallocates memory for a buffer of ewf error2 sectors 
 * Return a pointer to the instance, NULL on error
 */
EWF_ERROR2_SECTOR *ewf_error2_sectors_realloc( EWF_ERROR2_SECTOR *sectors, uint32_t previous_amount, uint32_t new_amount )
{
	uint32_t previous_size = previous_amount * EWF_ERROR2_SECTOR_SIZE;
	uint32_t new_size      = new_amount * EWF_ERROR2_SECTOR_SIZE;

	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_realloc: invalid sectors.\n" );

		return( NULL );
	}
	sectors = (EWF_ERROR2_SECTOR *) libewf_realloc_new_cleared( sectors, previous_size, new_size );

	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_realloc: unable to reallocate sectors.\n" );

		return( NULL );
	}
	return( sectors );
}

/* Frees memory of a error2
 */
void ewf_error2_free( EWF_ERROR2 *error2 )
{
	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_free: invalid error2.\n" );

		return;
	}
	libewf_free( error2 );
}

/* Frees memory of a buffer of ewf error2 sectors
 */
void ewf_error2_sectors_free( EWF_ERROR2_SECTOR *sectors )
{
	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_free: invalid sectors.\n" );

		return;
	}
	libewf_free( sectors );
}

/* Reads the error2 from a file descriptor
 * Return a pointer to the new instance, NULL on error
 */
EWF_ERROR2 *ewf_error2_read( int file_descriptor )
{
	EWF_ERROR2 *error2 = NULL;
	uint32_t size      = EWF_ERROR2_SIZE;
	int32_t count      = 0;

	error2 = ewf_error2_alloc();

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_read: unable to create error2.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, error2, size );

	if( count < size )
	{
		ewf_error2_free( error2 );

		LIBEWF_WARNING_PRINT( "ewf_error2_read: unable to read ewf_error2.\n" );

		return( NULL );
	}
	return( error2 );
}

/* Reads the ewf error2 sectors from a file descriptor
 * Return a pointer to the new instance, NULL on error
 */
EWF_ERROR2_SECTOR *ewf_error2_sectors_read( int file_descriptor, uint32_t amount )
{
	EWF_ERROR2_SECTOR *sectors = NULL;
	uint32_t size              = amount * EWF_ERROR2_SECTOR_SIZE;
	int32_t count              = 0;

	sectors = ewf_error2_sectors_alloc( amount );

	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_read: unable to create error2 sectors.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, sectors, size );

	if( count < size )
	{
		ewf_error2_sectors_free( sectors );

		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_read: unable to read error2 sectors.\n" );

		return( NULL );
	}
	return( sectors );
}

/* Writes the error2 to a file descriptor, without the first sector
 * this should be written by the sectors write function
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_error2_write( EWF_ERROR2 *error2, int file_descriptor )
{
	EWF_CRC *crc  = NULL;
	uint32_t size = EWF_ERROR2_SIZE;
	int32_t count = 0;

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_write: invalid error2.\n" );

		return( -1 );
	}
	crc = ewf_crc_calculate( (void *) error2, ( size - EWF_CRC_SIZE ), 1 );

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	revert_32bit( *crc, error2->crc );

	ewf_crc_free( crc );

	count = libewf_write( file_descriptor, error2, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_write: unable to write error2.\n" );

		return( -1 );
	}
	return( count );
}

/* Writes the sectors to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_error2_sectors_write( EWF_ERROR2_SECTOR *sectors, int file_descriptor, uint32_t amount )
{
	EWF_CRC *crc      = NULL;
	uint32_t size     = EWF_ERROR2_SECTOR_SIZE * amount;
	int32_t count     = 0;
	int32_t crc_count = 0;

	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_write: invalid sectors.\n" );

		return( -1 );
	}
	count = libewf_write( file_descriptor, sectors, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_write: unable write error2 sectors.\n" );

		return( -1 );
	}
	crc = ewf_crc_calculate( sectors, size, 1 );

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	crc_count = ewf_crc_write( crc, file_descriptor );

	ewf_crc_free( crc );

	if( crc_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_error2_sectors_write: unable to write CRC.\n" );

		return( -1 );
	}
	return( count + crc_count );
}

