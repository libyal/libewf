/*
 * libewf file reading
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

#include "ewf_compress.h"
#include "ewf_crc.h"

#include "libewf_common.h"
#include "libewf_debug.h"
#include "libewf_endian.h"
#include "libewf_notify.h"

/* Prints a dump of data
 */
void libewf_debug_dump_data( uint8_t *data, uint32_t size )
{
	EWF_CRC *calculated_crc = NULL;
	void * data_set         = NULL;
	EWF_CRC stored_crc      = 0;

	calculated_crc = ewf_crc_calculate( (void *) data, ( size - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_dump_data: unable to calculate CRC.\n" );

		return;
	}
	libewf_dump_data( data, size );

	data_set = libewf_memcpy( (void *) &stored_crc, (void *) &data[ size - EWF_CRC_SIZE ], EWF_CRC_SIZE );

	if( data_set == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_dump_data: unable to set CRC.\n" );
	}
	else
	{
		fprintf( stderr, "libewf_debug_dump_data: possible CRC (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
	}
	ewf_crc_free( calculated_crc );
}

/* Reads section data
 */
void libewf_debug_read_section( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	uint8_t *data              = NULL;
	uint8_t *uncompressed_data = NULL;
	uint32_t uncompressed_size = 0;
	int32_t read_count         = 0;
	int8_t result              = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: incorrect handle.\n" );

		return;
	}
	data = (uint8_t *) libewf_alloc( sizeof( uint8_t ) * size );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: unable to allocate data.\n" );

		return;
	}
	read_count = libewf_read( file_descriptor, data, size );

	if( read_count < size )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: unable to read section data.\n" );

		libewf_free( data );

		return;
	}
	uncompressed_size = size + 1024;
	uncompressed_data = (uint8_t *) libewf_alloc( sizeof( uint8_t ) * uncompressed_size );

	if( uncompressed_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_debug_read_section: unable to allocate uncompressed data.\n" );

		libewf_free( data );

		return;
	}
	result = ewf_uncompress( uncompressed_data, &uncompressed_size, data, size );

	if( result == 0 )
	{
		fprintf( stderr, "libewf_debug_read_section: data is UNCOMPRESSED.\n" );

		libewf_debug_dump_data( data, size );
	}
	else if( result == 1 )
	{
		fprintf( stderr, "libewf_debug_read_section: data is zlib COMPRESSED.\n" );

		libewf_debug_dump_data( uncompressed_data, uncompressed_size );
	}
	else
	{
		LIBEWF_FATAL_PRINT( "libewf_debug_read_section: unable to uncompress data.\n" );
	}
	libewf_free( data );
	libewf_free( uncompressed_data );
}

