/*
 * EWF table section
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
#include "ewf_table.h"

/* Allocates memory for a new ewf table struct
 * Returns a pointer to the new instance, NULL on error
 */
EWF_TABLE *ewf_table_alloc( void )
{
	EWF_TABLE *table = NULL;

	table = (EWF_TABLE *) libewf_alloc_cleared( EWF_TABLE_SIZE, 0 );

	if( table == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_alloc: unable to allocate ewf_table.\n" );

		return( NULL );
	}
	return( table );
}

/* Allocates memory for a buffer of ewf table offsets 
 * Returns a pointer to the new instance, NULL on error
 */
EWF_TABLE_OFFSET *ewf_table_offsets_alloc( uint32_t amount )
{
	EWF_TABLE_OFFSET *offsets = NULL;
	uint32_t size             = 0;

	size    = EWF_TABLE_OFFSET_SIZE * amount;
	offsets = (EWF_TABLE_OFFSET *) libewf_alloc_cleared( size, 0 );

	if( offsets == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_offsets_alloc: unable to allocate offsets.\n" );

		return( NULL );
	}
	return( offsets );
}

/* Reallocates memory for a buffer of ewf table offsets 
 * Returns a pointer to the instance, NULL on error
 */
EWF_TABLE_OFFSET *ewf_table_offsets_realloc( EWF_TABLE_OFFSET *offsets, uint32_t previous_amount, uint32_t new_amount )
{
	uint32_t previous_size = EWF_TABLE_OFFSET_SIZE * previous_amount;
	uint32_t new_size      = EWF_TABLE_OFFSET_SIZE * new_amount;

	if( offsets == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_offsets_realloc: invalid offsets.\n" );

		return( NULL );
	}
	offsets = (EWF_TABLE_OFFSET *) libewf_realloc_new_cleared( offsets, previous_size, new_size, 0 );

	if( offsets == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_offsets_realloc: unable to reallocate offsets.\n" );

		return( NULL );
	}
	return( offsets );
}

/* Frees memory of a table
 */
void ewf_table_free( EWF_TABLE *table )
{
	if( table == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_free: invalid table.\n" );

		return;
	}
	libewf_free( table );
}

/* Frees memory of a buffer of ewf table offsets
 */
void ewf_table_offsets_free( EWF_TABLE_OFFSET *offsets )
{
	if( offsets == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_offsets_free: invalid offsets.\n" );

		return;
	}
	libewf_free( offsets );
}

/* Reads the table from a file descriptor
 * Returns a pointer to the new instance, NULL on error
 */
EWF_TABLE *ewf_table_read( int file_descriptor )
{
	EWF_TABLE *table = NULL;
	int32_t count    = 0;

	table = ewf_table_alloc();

	if( table == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_read: unable to create table.\n" );

		return( NULL );
	}
	count = libewf_read( file_descriptor, table, EWF_TABLE_SIZE );

	if( count < EWF_TABLE_SIZE )
	{
		ewf_table_free( table );

		LIBEWF_WARNING_PRINT( "ewf_table_read: unable to read ewf_table.\n" );

		return( NULL );
	}
	return( table );
}

/* Reads the ewf table offsets from a file descriptor
 * Returns a pointer to the new instance, NULL on error
 */
EWF_TABLE_OFFSET *ewf_table_offsets_read( int file_descriptor, uint32_t amount )
{
	EWF_TABLE_OFFSET *offsets = NULL;
	uint32_t size             = 0;
	int32_t count             = 0;

	offsets = ewf_table_offsets_alloc( amount );

	if( offsets == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_offsets_read: unable to create table offsets.\n" );

		return( NULL );
	}
	size  = EWF_TABLE_OFFSET_SIZE * amount;
	count = libewf_read( file_descriptor, offsets, size );

	if( count < size )
	{
		ewf_table_offsets_free( offsets );

		LIBEWF_WARNING_PRINT( "ewf_table_offsets_read: unable to read offsets.\n" );

		return( NULL );
	}
	return( offsets );
}

/* Writes the table to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_table_write( EWF_TABLE *table, int file_descriptor )
{
	EWF_CRC *crc  = NULL;
	uint32_t size = EWF_TABLE_SIZE;
	int32_t count = 0;

	if( table == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_write: invalid table.\n" );

		return( -1 );
	}
	crc = ewf_crc_calculate( (void *) table, ( size - EWF_CRC_SIZE ), 1 );

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	libewf_endian_revert_32bit( *crc, table->crc );

	ewf_crc_free( crc );

	count = libewf_write( file_descriptor, table, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_write: error writing table.\n" );

		return( -1 );
	}
	return( count );
}

/* Writes the offsets to a file descriptor
 * Returns a -1 on error, the amount of bytes written on success
 */
int32_t ewf_table_offsets_write( EWF_TABLE_OFFSET *offsets, int file_descriptor, uint32_t amount )
{
	EWF_CRC *crc      = NULL;
	uint32_t size     = EWF_TABLE_OFFSET_SIZE * amount;
	int32_t count     = 0;
	int32_t crc_count = 0;

	if( offsets == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_offsets_write: invalid offsets.\n" );

		return( -1 );
	}
	count = libewf_write( file_descriptor, offsets, size );

	if( count < size )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_offsets_write: unable to write table offsets.\n" );

		return( -1 );
	}
	crc = ewf_crc_calculate( offsets, size, 1 );

	if( crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_offsets_write: unable to calculate CRC.\n" );

		return( -1 );
	}
	crc_count = ewf_crc_write( crc, file_descriptor );

	ewf_crc_free( crc );

	if( crc_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewf_table_offsets_write: unable to write CRC.\n" );

		return( -1 );
	}
	return( count + crc_count );
}

