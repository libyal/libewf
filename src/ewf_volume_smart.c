/*
 * EWF SMART volume section (EWF-S01)
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
#include "ewf_volume_smart.h"

/* Allocates memory for a new ewf volume struct
 * Returns a pointer to the new instance, NULL on error
 */
EWF_VOLUME_SMART *ewf_volume_smart_alloc( void )
{
	EWF_VOLUME_SMART *volume = NULL;

	volume = (EWF_VOLUME_SMART *) libewf_alloc_cleared( EWF_VOLUME_SMART_SIZE, 0 );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_volume_smart_alloc: unable to allocate ewf_volume_smart.\n" );

		return( NULL );
	}
	volume->unknown3[0]          = 1;
	volume->sectors_per_chunk[0] = 64;

	/* Set the bytes per sector to 512
	 * 512/256 = 2
	 */
	volume->bytes_per_sector[1] = 2;

	return( volume );
}

/* Frees memory of a volume
 */
void ewf_volume_smart_free( EWF_VOLUME_SMART *volume )
{
	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_volume_smart_free: invalid volume.\n" );

		return;
	}
	libewf_free( volume );
}

/* Reads the volume from a file descriptor
 * Returns a pointer to the new instance, NULL on error
 */
EWF_VOLUME_SMART *ewf_volume_smart_read( int file_descriptor )
{
	EWF_VOLUME_SMART *volume = NULL;
	int32_t count            = 0;

	volume = ewf_volume_smart_alloc();

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_volume_smart_read: unable to create volume.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, volume, EWF_VOLUME_SMART_SIZE );

	if( count < EWF_VOLUME_SMART_SIZE )
	{
		ewf_volume_smart_free( volume );

		LIBEWF_WARNING_PRINT( "ewf_volume_smart_read: unable to read volume.\n" );

		return( NULL );
	}
	return( volume );
}

/* Writes the volume to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_volume_smart_write( EWF_VOLUME_SMART *volume, int file_descriptor )
{
	EWF_CRC *crc  = NULL;
	uint32_t size = EWF_VOLUME_SMART_SIZE;
	int32_t count = 0;

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_volume_smart_write: invalid volume.\n" );

		return( -1 );
	}
	crc = ewf_crc_calculate( (void *) volume, ( size - EWF_CRC_SIZE ), 1 );

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_volume_smart_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	libewf_endian_revert_32bit( *crc, volume->crc );

	ewf_crc_free( crc );

	count = libewf_write( file_descriptor, volume, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_volume_smart_write: unable to write volume.\n" );

		return( -1 );
	}
	return( count );
}

/* Calculates the chunck size = sectors per chunk * bytes per sector
 * Returns the amount of bytes per sector, -1 on error
 */
int32_t ewf_volume_smart_calculate_chunk_size( EWF_VOLUME_SMART *volume )
{
	uint32_t sectors_per_chunk = 0;
	uint32_t bytes_per_sector  = 0;

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_volume_smart_calculate_chunk_size: invalid volume.\n" );

		return( -1 );
	}
	sectors_per_chunk = libewf_endian_convert_32bit( volume->sectors_per_chunk );
	bytes_per_sector  = libewf_endian_convert_32bit( volume->bytes_per_sector );

	return( sectors_per_chunk * bytes_per_sector );
}

