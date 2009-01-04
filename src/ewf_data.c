/*
 * EWF data section
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
#include "ewf_data.h"

/* Allocates memory for a new ewf data struct
 * Returns a pointer to the new instance, NULL on error
 */
EWF_DATA *ewf_data_alloc( void )
{
	EWF_DATA *data = NULL;

	data = (EWF_DATA *) libewf_alloc_cleared( EWF_DATA_SIZE, 0 );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_data_alloc: unable to allocate ewf_data.\n" );

		return( NULL );
	}
	data->unknown3[0]          = 1;
	data->sectors_per_chunk[0] = 64;

	/* Set the bytes per sector to 512
	 * 512/256 = 2
	 */
	data->bytes_per_sector[1] = 2;

	return( data );
}

/* Frees memory of a data
 */
void ewf_data_free( EWF_DATA *data )
{
	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_data_free: invalid data.\n" );

		return;
	}
	libewf_free( data );
}

/* Reads the data from a file descriptor
 * Returns a pointer to the new instance, NULL on error
 */
EWF_DATA *ewf_data_read( int file_descriptor )
{
	EWF_DATA *data = NULL;
	uint32_t size  = EWF_DATA_SIZE;
	int32_t count  = 0;

	data = ewf_data_alloc();

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_data_read: unable to create data.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, data, size );

	if( count < size )
	{
		ewf_data_free( data );

		LIBEWF_WARNING_PRINT( "ewf_data_read: unable to read ewf_data.\n" );

		return( NULL );
	}
	return( data );
}

/* Writes the data to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_data_write( EWF_DATA *data, int file_descriptor )
{
	EWF_CRC *crc  = NULL;
	uint32_t size = EWF_DATA_SIZE;
	int32_t count = 0;

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_data_write: invalid data.\n" );

		return( -1 );
	}
	crc = ewf_crc_calculate( (void *) data, ( size - EWF_CRC_SIZE ), 1 );

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_data_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	revert_32bit( *crc, data->crc );

	ewf_crc_free( crc );
	
	count = libewf_write( file_descriptor, data, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_data_write: unable to write data.\n" );

		return( -1 );
	}
	return( count );
}

