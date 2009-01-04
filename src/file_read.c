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

#include <sys/stat.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"
#include "libewf_md5.h"

#include "ewf_compress.h"
#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_error2.h"
#include "ewf_md5hash.h"
#include "ewf_file_header.h"
#include "ewf_hash.h"
#include "ewf_header.h"
#include "ewf_header2.h"
#include "ewf_ltree.h"
#include "ewf_section.h"
#include "ewf_sectors.h"
#include "ewf_volume.h"
#include "ewf_volume_smart.h"
#include "ewf_table.h"
#include "libewf_file.h"
#include "handle.h"
#include "section_list.h"
#include "offset_table.h"
#include "segment_table.h"

/* Prints a dump of section data
 */
void libewf_dump_section_data( uint8_t *data, size_t size )
{
	EWF_CRC *calculated_crc = NULL;
	EWF_CRC stored_crc      = 0;

	calculated_crc = ewf_crc_calculate( (void *) data, ( size - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_dump_section_data: unable to calculate CRC.\n" );
	}
	libewf_dump_data( data, size );

	memcpy( (uint8_t *) &stored_crc, (uint8_t *) ( data + size - EWF_CRC_SIZE ), EWF_CRC_SIZE );

	fprintf( stderr, "libewf_dump_section_data: possible CRC (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );

	ewf_crc_free( calculated_crc );
}

/* Reads section data
 */
void libewf_section_read_data( LIBEWF_HANDLE *handle, int file_descriptor, size_t size )
{
	uint8_t *data              = NULL;
	uint8_t *uncompressed_data = NULL;
	uint32_t uncompressed_size = 0;
	int32_t read_count         = 0;
	int result                 = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_read_data: incorrect handle.\n" );
	}
	data       = (uint8_t *) malloc( sizeof( uint8_t ) * size );
	read_count = libewf_read( file_descriptor, data, size );

	if( read_count < size )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_read_data: unable to read section data.\n" );
	}
	uncompressed_size = size + 1024;
	uncompressed_data = (uint8_t *) malloc( sizeof( uint8_t ) * uncompressed_size );

	result = ewf_uncompress( uncompressed_data, &uncompressed_size, data, size );

	if( result == 0 )
	{
		fprintf( stderr, "libewf_section_read_data: data is not zlib compressed.\n" );

		libewf_dump_section_data( data, size );
	}
	else if( result == 1 )
	{
		fprintf( stderr, "libewf_section_read_data: zlib uncompressed data:.\n\n" );

		libewf_dump_section_data( uncompressed_data, uncompressed_size );
	}
	else
	{
		LIBEWF_FATAL_PRINT( "libewf_section_read_data: unable to uncompress data.\n" );
	}
	free( data );
	free( uncompressed_data );
}

/* Reads a header section
 */
void libewf_section_header_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size )
{
	EWF_HEADER *header = NULL;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header_read: incorrect handle.\n" );
	}
	header = ewf_header_read( file_descriptor, &size );

	if( header == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header_read: unable to read header.\n" );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_header_read: Header:.\n" );
	LIBEWF_VERBOSE_EXEC( ewf_header_fprint( stderr, header ); );

	if( libewf_handle_is_set_header( handle ) == 0 )
	{
		libewf_handle_set_header( handle, header );
	}
	else
	{
		ewf_header_free( header );
	}
}

/* Reads a header2 section
 */
void libewf_section_header2_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size )
{
	EWF_HEADER *header2 = NULL;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header2_read: incorrect handle.\n" );
	}
	header2 = ewf_header2_read( file_descriptor, size );

	if( header2 == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header2_read: unable to read header2.\n" );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_header2_read: Header2:.\n" );
	LIBEWF_VERBOSE_EXEC( ewf_header_fprint( stderr, header2 ); );

	if( libewf_handle_is_set_header2( handle ) == 0 )
	{
		libewf_handle_set_header2( handle, header2 );
	}
	else
	{
		ewf_header_free( header2 );
	}
}

/* Reads a volume section
 */
void libewf_section_volume_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size )
{
	EWF_VOLUME *volume             = NULL;
	EWF_VOLUME_SMART *volume_smart = NULL;
	EWF_CRC *calculated_crc        = NULL;
	EWF_CRC stored_crc             = 0;
	int32_t bytes_per_chunk        = 0;
	uint32_t volume_chunk_count    = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_read: incorrect handle.\n" );
	}
	if( size == EWF_VOLUME_SIZE )
	{
		volume = ewf_volume_read( file_descriptor );

		if( volume == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_section_volume_read: unable to read volume.\n" );
		}
#ifdef _LIBEWF_DEBUG_
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown1, 4 ); );
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown2, 16 ); );
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown3, 4 ); );
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown4, 12 ); );
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown5, 3 ); );
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown6, 4 ); );
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown7, 963 ); );
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->signature, 5 ); );
#endif

		calculated_crc = ewf_crc_calculate( (void *) volume, ( EWF_VOLUME_SIZE - EWF_CRC_SIZE ), 1 );

		if( calculated_crc == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_section_volume_read: unable to calculate CRC.\n" );
		}
		stored_crc      = convert_32bit( volume->crc );
		bytes_per_chunk = ewf_volume_calculate_chunk_size( volume );

		if( bytes_per_chunk <= -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_volume_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
		}
		if( stored_crc != *calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_volume_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
		}
		volume_chunk_count        = convert_32bit( volume->chunk_count );
		handle->chunk_count       = convert_32bit( volume->chunk_count );
	        handle->sectors_per_chunk = convert_32bit( volume->sectors_per_chunk );
	        handle->bytes_per_sector  = convert_32bit( volume->bytes_per_sector );
	        handle->sector_count      = convert_32bit( volume->sector_count );
	        handle->media_type        = convert_32bit( volume->unknown3 );
	        handle->compression_level = volume->compression_level;
		handle->ewf_format        = EWF_FORMAT_E01;

		LIBEWF_VERBOSE_PRINT( "libewf_section_volume_read: this volume has %" PRIu32 " chunks of %" PRIi32 " bytes each, CRC %" PRIu32 " (%" PRIu32 ").\n", volume_chunk_count, bytes_per_chunk, stored_crc, *calculated_crc );

		memcpy( (uint8_t *) handle->guid, (uint8_t *) volume->guid, 16 );

		ewf_volume_free( volume );
		ewf_crc_free( calculated_crc );
	}
	else if( size == EWF_VOLUME_SMART_SIZE )
	{
		volume_smart = ewf_volume_smart_read( file_descriptor );

		if( volume_smart == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_section_volume_read: unable to read volume smart.\n" );
		}
#ifdef _LIBEWF_DEBUG_
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown1, 4 ); );
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown2, 20 ); );
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown3, 45 ); );
#endif

		calculated_crc = ewf_crc_calculate( (void *) volume_smart, ( EWF_VOLUME_SMART_SIZE - EWF_CRC_SIZE ), 1 );

		if( calculated_crc == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_section_volume_read: unable to calculate CRC.\n" );
		}
		stored_crc      = convert_32bit( volume_smart->crc );
		bytes_per_chunk = ewf_volume_smart_calculate_chunk_size( volume_smart );

		if( stored_crc != *calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_volume_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
		}
		volume_chunk_count        = convert_32bit( volume_smart->chunk_count );
		handle->chunk_count       = convert_32bit( volume_smart->chunk_count );
	        handle->sectors_per_chunk = convert_32bit( volume_smart->sectors_per_chunk );
	        handle->bytes_per_sector  = convert_32bit( volume_smart->bytes_per_sector );
	        handle->sector_count      = convert_32bit( volume_smart->sector_count );
	        handle->media_type        = convert_32bit( volume_smart->unknown3 );
		handle->ewf_format        = EWF_FORMAT_S01;

		LIBEWF_VERBOSE_PRINT( "libewf_section_volume_read: This volume has %" PRIu32 " chunks of %" PRIi32 " bytes each, CRC %" PRIu32 " (%" PRIu32 ").\n", volume_chunk_count, bytes_per_chunk, stored_crc, *calculated_crc );

		ewf_volume_smart_free( volume_smart );
		ewf_crc_free( calculated_crc );
	}
	else
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_read: mismatch in section data size.\n" );
	}
	if( ( bytes_per_chunk + EWF_CRC_SIZE ) > handle->allocated_chunk_data_size )
	{
		handle = libewf_handle_cache_realloc( handle, (uint32_t) ( bytes_per_chunk + EWF_CRC_SIZE ) );

		handle->chunk_size = (uint32_t) bytes_per_chunk;
	}
	if( volume_chunk_count == 0 )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_section_volume_read: compensating for 0 volume chunk count.\n" );

		volume_chunk_count = 1;
	}
	handle->offset_table           = libewf_offset_table_values_alloc( handle->offset_table, volume_chunk_count );
	handle->secondary_offset_table = libewf_offset_table_values_alloc( handle->secondary_offset_table, volume_chunk_count );
}

/* Fills the offset table
 */
LIBEWF_OFFSET_TABLE *libewf_fill_offset_table( LIBEWF_OFFSET_TABLE *offset_table, EWF_TABLE_OFFSET *offsets, uint32_t chunk_amount, int file_descriptor )
{
	uint32_t size_of_chunks = 0;
	uint32_t raw_offset     = 0;
	uint64_t chunk_size     = 0;
	uint64_t iterator       = 0;
	uint64_t current_offset = 0;
	uint64_t next_offset    = 0;
	uint8_t compressed      = 0;

	if( offset_table == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_fill_offset_table: invalid offset_table.\n" );
	}
	/* Correct the last offset, to fill the table it should point to the first empty entry
	 * the the last filled entry
	 */
	if( offset_table->last > 0 )
	{
		offset_table->last++;
	}
	size_of_chunks = chunk_amount + offset_table->last;

	/* Allocate additional entries in the offset table if needed
	 * - a single reallocation saves processing time
	 */
	if( offset_table->amount < size_of_chunks )
	{
		offset_table = libewf_offset_table_values_realloc( offset_table, size_of_chunks );
	}
	/* Read the offsets from file
	 */
	raw_offset = convert_32bit( offsets[ iterator ].offset );

	/* The size of the last chunk must be determined differently
	 */
	while( iterator < ( chunk_amount - 1 ) )
	{
		compressed     = raw_offset >> 31;
		current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		raw_offset     = convert_32bit( offsets[ iterator + 1 ].offset );
		next_offset    = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		chunk_size     = next_offset - current_offset;

		offset_table = libewf_offset_table_set_values( offset_table, offset_table->last, file_descriptor, compressed, current_offset, chunk_size );

		offset_table->last++;

		if( compressed == 0 )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_fill_offset_table: uncompressed chunk %" PRIu64 " read with offset %" PRIu64 " and size %" PRIu64 ".\n", offset_table->last, current_offset, chunk_size );
		}
		else
		{
			LIBEWF_VERBOSE_PRINT( "libewf_fill_offset_table: compressed chunk %" PRIu64 " read with offset %" PRIu64 " and size %" PRIu64 ".\n", offset_table->last, current_offset, chunk_size );

		}
		current_offset = next_offset;

		iterator++;
	}
	raw_offset     = convert_32bit( offsets[ iterator ].offset );
	compressed     = raw_offset >> 31;
	current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	offset_table   = libewf_offset_table_set_values( offset_table, offset_table->last, file_descriptor, compressed, current_offset, 0 );

	return( offset_table );
}

/* Calculate the last offset
 */
LIBEWF_OFFSET_TABLE *libewf_calculate_last_offset( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, int file_descriptor )
{
	LIBEWF_SECTION_LIST_ENTRY *section_list_entry = NULL;
	uint64_t last_offset                          = 0;

	if( offset_table == NULL || section_list == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_calculate_last_offset: incorrect offset table or section list.\n" );
	}
	/*
	 * There is no indication how large the last chunk is. The only thing known is where it starts.
	 * However it can be determined where the next section starts within the file.
	 * The size of the last chunk is determined by subtracting the last offset from the offset of the section that follows.
	 */
	section_list_entry = section_list->first;
	last_offset        = offset_table->offset[ offset_table->last ];

	while( section_list_entry != NULL )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_calculate_last_offset: start offset: %" PRIu64 " last offset: %" PRIu64 ".\n", section_list_entry->start_offset, last_offset );

		if( ( section_list_entry->file_descriptor == file_descriptor ) && ( section_list_entry->start_offset < last_offset ) && ( last_offset < section_list_entry->end_offset ) )
		{
			uint64_t chunk_size = section_list_entry->end_offset - last_offset;

			offset_table = libewf_offset_table_set_values( offset_table, offset_table->last, file_descriptor, offset_table->compressed[ offset_table->last ], last_offset, chunk_size );

			LIBEWF_VERBOSE_PRINT( "libewf_calculate_last_offset: last chunk %" PRIu64 " calculated with offset %" PRIu64 " and size %" PRIu64 ".\n", ( offset_table->last + 1 ), last_offset, chunk_size );

			break;
		}
		section_list_entry = section_list_entry->next;
	}
	return( offset_table );
}

/* Reads an offset table
 */
LIBEWF_OFFSET_TABLE *libewf_offset_table_read( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, int file_descriptor, size_t size, uint8_t ewf_format )
{
	EWF_TABLE *table          = NULL;
	EWF_TABLE_OFFSET *offsets = NULL;
	EWF_CRC *calculated_crc   = NULL;
	EWF_CRC *stored_crc_read  = NULL;
	EWF_CRC stored_crc        = 0;
	uint32_t chunk_count      = 0;

	if( offset_table == NULL || section_list == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_offset_table_read: incorrect offset table or section list.\n" );
	}
	table = ewf_table_read( file_descriptor );

	if( table == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_table_read: unable to read table.\n" );
	}
#ifdef _LIBEWF_DEBUG_
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( table->padding, 16 ); );
#endif

	/* table size contains both the size of the CRC (4 bytes)
	 */
	calculated_crc = ewf_crc_calculate( (void *) table, ( EWF_TABLE_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_table_read: unable to calculate CRC.\n" );
	}
	stored_crc = convert_32bit( table->crc );

	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
	}
	chunk_count = convert_32bit( table->chunk_count );

	LIBEWF_VERBOSE_PRINT( "libewf_offset_table_read: Table is of size %" PRIu32 " chunks CRC %" PRIu32 " (%" PRIu32 ").\n", chunk_count, stored_crc, *calculated_crc );

	ewf_crc_free( calculated_crc );

	if( chunk_count <= 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_offset_table_read: table contains no offsets!.\n" );
	}
	offsets = ewf_table_offsets_read( file_descriptor, chunk_count );

	if( offsets == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_offset_table_read: unable to read table offsets.\n" );
	}
	if( ewf_format == EWF_FORMAT_E01 )
	{
		calculated_crc = ewf_crc_calculate( offsets, ( EWF_TABLE_OFFSET_SIZE * chunk_count ), 1 );

		if( calculated_crc == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_section_table_read: unable to calculate CRC.\n" );
		}
		stored_crc_read = ewf_crc_read( file_descriptor );

		if( stored_crc_read == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_section_table_read: unable to read CRC from file descriptor.\n" );
		}
		if( *stored_crc_read != *calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "libewf_offset_table_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", *stored_crc_read, *calculated_crc );
		}
		ewf_crc_free( calculated_crc );
		ewf_crc_free( stored_crc_read );
	}
	offset_table = libewf_fill_offset_table( offset_table, offsets, chunk_count, file_descriptor );
	offset_table = libewf_calculate_last_offset( offset_table, section_list, file_descriptor );

	ewf_table_free( table );
	ewf_table_offsets_free( offsets );

	return( offset_table );
}

/* Compare the offsets in tabel and table2 sections
 * Returns a 0 if table differ
 */
uint8_t libewf_compare_offset_tables( LIBEWF_OFFSET_TABLE *offset_table1, LIBEWF_OFFSET_TABLE *offset_table2 )
{
	uint64_t iterator = 0;

	if( ( offset_table1 == NULL ) || ( offset_table2 == NULL ) )
	{
		LIBEWF_FATAL_PRINT( "libewf_compare_offset_tables: invalid offset tables.\n" );
	}
	/* Check if table and table2 are the same
	 */
	if( offset_table1->amount != offset_table2->amount )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_compare_offset_tables: offset tables differ in size.\n" );

		return( 0 );
	}
	else
	{
		for( iterator = 0; iterator < offset_table1->amount; iterator++ )
		{
			if( offset_table1->offset[ iterator ] != offset_table2->offset[ iterator ] )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_compare_offset_tables: offset tables differ in offset for chunk: %" PRIu64 " (table1: %" PRIu64 ", table2: %" PRIu64 ").\n", iterator,
				offset_table1->offset[ iterator ], offset_table2->offset[ iterator ] );

				return( 0 );
			}
		}
	}
	return( 1 );
}

/* Reads a table section
 */
void libewf_section_table_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size, LIBEWF_SECTION_LIST *section_list )
{
	if( handle == NULL || section_list == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_table_read: incorrect handle or section list.\n" );
	}
	handle->offset_table = libewf_offset_table_read( handle->offset_table, section_list, file_descriptor, size, handle->ewf_format );
}

/* Reads a table2 section
 */
void libewf_section_table2_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size, LIBEWF_SECTION_LIST *section_list )
{
	if( handle == NULL || section_list == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_table2_read: incorrect handle or section list.\n" );
	}
	/* In the EWF-E01 format the table2 section is a replicate of the table section
	 */
	if( handle->ewf_format == EWF_FORMAT_E01 )
	{
		handle->secondary_offset_table = libewf_offset_table_read( handle->secondary_offset_table, section_list, file_descriptor, size, handle->ewf_format );

		libewf_compare_offset_tables( handle->offset_table, handle->secondary_offset_table );
	}
	/* In the EWF-S01 format the table2 section is an addition on the table section
	 */
	else if( handle->ewf_format == EWF_FORMAT_S01 )
	{
		handle->offset_table = libewf_offset_table_read( handle->offset_table, section_list, file_descriptor, size, handle->ewf_format );
	}
}

/* Reads a sectors section
 */
void libewf_section_sectors_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size )
{
	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_sectors_read: incorrect handle.\n" );
	}
	/* In the EWF-E01 format the sectors section holds the actual data chunks
	 */
}

/* Reads a ltree section
 */
void libewf_section_ltree_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size )
{
	EWF_LTREE *ltree       = NULL;
	EWF_HEADER *tree_data  = NULL;
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_ltree_read: incorrect handle.\n" );
	}
/*
	if( size != EWF_LTREE_SIZE )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_ltree_read: mismatch in section data size.\n" );
	}
*/
	ltree = ewf_ltree_read( file_descriptor );

	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown1, 16 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->tree_size, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown2, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown3, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown4, 20 ); );

	fprintf( stderr, "remaining lree size: %d (0x%x).\n", ( size - EWF_LTREE_SIZE ), ( size - EWF_LTREE_SIZE ) );

/*
	LIBEWF_VERBOSE_EXEC( libewf_section_read_data( handle, file_descriptor, ( size - EWF_LTREE_SIZE ) ); );
*/
	tree_data = ewf_tree_data_read( file_descriptor, ( size - EWF_LTREE_SIZE ) );

	LIBEWF_VERBOSE_EXEC( ewf_header_fprint( stderr, tree_data ); );

	ewf_header_free( tree_data );
}

/* Reads a data section
 */
void libewf_section_data_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size )
{
	EWF_DATA *data          = NULL;
	EWF_CRC *calculated_crc = NULL;
	EWF_CRC stored_crc      = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_data_read: incorrect handle.\n" );
	}
	if( size != EWF_DATA_SIZE )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_data_read: mismatch in section data size.\n" );
	}
	data = ewf_data_read( file_descriptor );

	if( data == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_data_read: unable to read data.\n" );
	}
	calculated_crc = ewf_crc_calculate( (void *) data, ( EWF_DATA_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_data_read: unable to calculate CRC.\n" );
	}
	stored_crc = convert_32bit( data->crc );

	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
	}
#ifdef _LIBEWF_DEBUG_
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown2, 16 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown3, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown4, 12 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown5, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown6, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown7, 963 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->signature, 5 ); );
#endif

	ewf_data_free( data );
	ewf_crc_free( calculated_crc );
}

/* Reads a error2 section
 */
void libewf_section_error2_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size )
{
	EWF_ERROR2 *error2         = NULL;
	EWF_ERROR2_SECTOR *sectors = NULL;
	EWF_CRC *calculated_crc    = NULL;
	EWF_CRC *stored_crc_read   = NULL;
	EWF_CRC stored_crc         = 0;
	uint32_t error_count       = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_read: incorrect handle.\n" );
	}
	error2 = ewf_error2_read( file_descriptor );

	if( error2 == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_read: unable to read error2.\n" );
	}
	calculated_crc = ewf_crc_calculate( (void *) error2, ( EWF_ERROR2_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_read: unable to calculate CRC.\n" );
	}
	stored_crc  = convert_32bit( error2->crc );
	error_count = convert_32bit( error2->error_count );

	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
	}
	ewf_crc_free( calculated_crc );

	if( error_count <= 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_read: error2 contains no sectors!.\n" );
	}
	sectors = ewf_error2_sectors_read( file_descriptor, error_count );

	if( sectors == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_read: unable to read error2 sectors.\n" );
	}
#ifdef _LIBEWF_DEBUG_
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( error2->unknown, 200 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( (uint8_t *) sectors, ( EWF_ERROR2_SECTOR_SIZE * error_count ) ); );
#endif

	calculated_crc = ewf_crc_calculate( (void *) sectors, ( EWF_ERROR2_SECTOR_SIZE * error_count ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_read: unable to calculate CRC.\n" );
	}
	stored_crc_read = ewf_crc_read( file_descriptor );

	if( stored_crc_read == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_read: unable to read CRC from file descriptor.\n" );
	}
	if( *stored_crc_read != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", *stored_crc_read, *calculated_crc );
	}
	handle->error2_error_count = error_count;
	handle->error2_sectors     = sectors;

	ewf_error2_free( error2 );
	ewf_crc_free( calculated_crc );
	ewf_crc_free( stored_crc_read );
}

/* Reads a hash section
 */
void libewf_section_hash_read( LIBEWF_HANDLE *handle, int file_descriptor, size_t size )
{
	EWF_HASH *hash          = NULL;
	EWF_CRC *calculated_crc = NULL;
	EWF_CRC stored_crc      = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_hash_read: incorrect handle.\n" );
	}
	if( size != EWF_HASH_SIZE )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_hash_read: mismatch in section data size.\n" );
	}
	hash = ewf_hash_read( file_descriptor );

	if( hash == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_hash_read: unable to read hash.\n" );
	}
	calculated_crc = ewf_crc_calculate( (void *) hash, ( EWF_HASH_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_hash_read: unable to calculate CRC.\n" );
	}
	stored_crc = convert_32bit( hash->crc );

	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
	}
	libewf_handle_set_md5hash( handle, hash->md5hash );

#ifdef _LIBEWF_DEBUG_
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash->unknown1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash->unknown2, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash->signature, 8 ); );
#endif

	ewf_hash_free( hash );
	ewf_crc_free( calculated_crc );
}

/* Reads and processes a section's data from a segment
 */
void libewf_section_data_read_segment( LIBEWF_HANDLE *handle, uint32_t segment, EWF_SECTION *section, int file_descriptor, LIBEWF_SECTION_LIST *section_list )
{
	size_t size = 0;

	if( handle == NULL || section_list == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_data_read_segment: incorrect handle or section list.\n" );
	}
	size = convert_64bit( section->size ) - EWF_SECTION_SIZE;

	/* Skip the next and done section
	 */
	if( ewf_section_is_type_next( section ) || ewf_section_is_type_done( section ) )
	{
		/* Just skip these */
	}
	/* Read the header2 section
	 */
	else if( ewf_section_is_type_header2( section ) )
	{
		libewf_section_header2_read( handle, file_descriptor, size );
	}
	/* Read the header section
	 */
	else if( ewf_section_is_type_header( section ) )
	{
		libewf_section_header_read( handle, file_descriptor, size );
	}
	/* Read the volume or disk section
	 */
	else if( ewf_section_is_type_volume( section ) || ewf_section_is_type_disk( section ) )
	{
		libewf_section_volume_read( handle, file_descriptor, size );
	}
	/* Read the table2 section
	 */
	else if( ewf_section_is_type_table2( section ) )
	{
		libewf_section_table2_read( handle, file_descriptor, size, section_list );
	}
	/* Read the table section
	 */
	else if( ewf_section_is_type_table( section ) )
	{
		libewf_section_table_read( handle, file_descriptor, size, section_list );
	}
	/* Read the sectors section
	 */
	else if( ewf_section_is_type_sectors( section ) )
	{
		libewf_section_sectors_read( handle, file_descriptor, size );
	}
	/* Read the ltree section
	 */
	else if( ewf_section_is_type_ltree( section ) )
	{
		libewf_section_ltree_read( handle, file_descriptor, size );
	}
	/* Read the data section
	 */
	else if( ewf_section_is_type_data( section ) )
	{
		libewf_section_data_read( handle, file_descriptor, size );
	}
	/* Read the hash section
	 */
	else if( ewf_section_is_type_hash( section ) )
	{
		libewf_section_hash_read( handle, file_descriptor, size );
	}
	/* Read the error2 section
	 */
	else if( ewf_section_is_type_error2( section ) )
	{
		libewf_section_error2_read( handle, file_descriptor, size );
	}
	else
	{
		LIBEWF_VERBOSE_PRINT( "libewf_section_data_read_segment: Unknown section type: %s.\n", section->type );
		LIBEWF_VERBOSE_EXEC( libewf_section_read_data( handle, file_descriptor, size ); );
	}
}

/* Reads and processes section in a segment
 * Returns the last section read
 */
EWF_SECTION *libewf_sections_read_segment( LIBEWF_HANDLE *handle, uint32_t segment )
{
	EWF_SECTION *section    = NULL;
	EWF_CRC *calculated_crc = NULL;
	char *filename          = NULL;
	EWF_CRC stored_crc      = 0;
	uint64_t offset_end     = 0;
	int file_descriptor     = 0;

	/* The first offset is directly after the file header (13 byte)
	 */
	uint64_t previous_offset = EWF_FILE_HEADER_SIZE;
	uint64_t next_offset     = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_sections_read_segment: incorrect handle.\n" );
	}
	if( libewf_segment_table_values_is_set( handle->segment_table, segment ) == 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_sections_read_segment: missing a segment file for segment %" PRIu32 ".\n", segment );
	}
	file_descriptor = libewf_segment_table_get_file_descriptor( handle->segment_table, segment );

	while( 1 )
	{
		section = ewf_section_read( file_descriptor );

		if( section == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_sections_read_segment: unable to read section start.\n" );
		}
		calculated_crc = ewf_crc_calculate( (void *) section, ( EWF_SECTION_SIZE - EWF_CRC_SIZE ), 1 );

		if( calculated_crc == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_sections_read_segment: unable to calculate CRC.\n" );
		}
		stored_crc = convert_32bit( section->crc );

		if( stored_crc != *calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "libewf_sections_read_segment: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
		}
		ewf_crc_free( calculated_crc );

		next_offset = convert_64bit( section->next );

		LIBEWF_VERBOSE_EXEC( ewf_section_fprint( stderr, section ); );

#ifdef _LIBEWF_DEBUG_
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( section->padding, 40 ); );
#endif

		offset_end = previous_offset + convert_64bit( section->size );

		libewf_section_list_append( handle->segment_table->section_list[ segment ], file_descriptor, previous_offset, offset_end );

		libewf_section_data_read_segment( handle, segment, section, file_descriptor, handle->segment_table->section_list[ segment ] );

		/* Check if the section alignment is correct.
		 * The done and next section point back at themselves, they should be the last section withing the segment file
		 */
		if( previous_offset < next_offset )
		{
			/* Seek the next section, it should be within the segment file
			 */
			if( lseek( file_descriptor, next_offset, SEEK_SET ) != next_offset )
			{
				ewf_section_free( section );

				filename = libewf_segment_table_get_filename( handle->segment_table, segment );

				LIBEWF_FATAL_PRINT( "libewf_sections_read_segment: next section not found segment file: %s.\n", filename );
			}
			previous_offset = next_offset;
		}
		else if( ewf_section_is_type_next( section ) || ewf_section_is_type_done( section ) )
		{
			break;
		}
		else
		{
			LIBEWF_FATAL_PRINT( "libewf_sections_read_segment: section skip for section type: %s not allowed.\n", section->type );
		}
		ewf_section_free( section );
	}
	return( section );
}

/* Reads a certain chunk within the sectors section according to the offset table
 */
int64_t libewf_read_chunk( LIBEWF_HANDLE *handle, uint32_t chunk, void *buffer, uint64_t buffer_size )
{
	EWF_CRC *calculated_crc = NULL;
	EWF_CRC stored_crc      = 0;
	uint64_t size           = 0;
	uint64_t offset         = 0;
	ssize_t count           = 0;
	int file_descriptor     = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_chunk: incorrect handle.\n" );
	}
	if( handle->index_build == 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_chunk: index was not build.\n" );
	}
	if( chunk >= handle->offset_table->amount )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_chunk: chunk: %" PRIu32 " not in offset table.\n", chunk );
	}
	file_descriptor = handle->offset_table->file_descriptor[ chunk ];
	size            = handle->offset_table->size[ chunk ];
	offset          = handle->offset_table->offset[ chunk ];

	LIBEWF_VERBOSE_PRINT( "libewf_read_chunk: read file descriptor: %d, for offset: %" PRIu64 ", for size: %" PRIu64 ".\n", file_descriptor, offset, size );

	if( size == 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_chunk: invalid chunk offset data - size of chunk is zero.\n" );
	}
	else if( size > buffer_size )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_chunk: size of chunk larger than specified buffer size.\n" );
	}
	count = ewf_sectors_chunk_read( buffer, file_descriptor, offset, size );

	if( count < size )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_chunk: cannot read chunk: %" PRIu32 " from file.\n", chunk );
	}
	/* Only verify CRC if the chunk is not compressed
	 */
	if( handle->offset_table->compressed[ chunk ] == 0 )
	{
		calculated_crc = ewf_crc_calculate( buffer, ( count - EWF_CRC_SIZE ), 1 );

		if( calculated_crc == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_read_chunk: unable to calculate CRC.\n" );
		}
		stored_crc = convert_32bit( (uint8_t *) ( buffer + count - EWF_CRC_SIZE ) );

		LIBEWF_VERBOSE_PRINT( "libewf_read_chunk: CRC for chunk: %" PRIu32 " (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", chunk, stored_crc, calculated_crc );

		if( stored_crc != *calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "libewf_read_chunk: CRC does not match for chunk: %" PRIu32 " (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", chunk, stored_crc, *calculated_crc );
		}
		ewf_crc_free( calculated_crc );
	}
	return( count );
}

/* Reads a random offset within the data within the EWF file
 */
int64_t libewf_read_random( LIBEWF_HANDLE *handle, void *buffer, uint64_t size, uint64_t offset )
{
	int result               = 0;
	uint32_t chunk           = 0;
	uint32_t raw_data_size   = 0;
	uint64_t buffer_offset   = 0;
	uint64_t chunk_data_size = 0;
	uint64_t available       = 0;
	int64_t chunk_read_count = 0;
	int64_t count_read       = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_random: incorrect handle.\n" );
	}
	if( handle->index_build == 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_random: index was not build.\n" );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_read_random: reading from offset: %" PRIu64 " size: %" PRIu64 ".\n", offset, size );

	/* The chunk we are after
	 */
	chunk = (uint32_t) ( offset / handle->chunk_size );

	if( chunk >= handle->offset_table->amount )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_random: Attempting to read past the end of the file.");
	}
	/* Offset within the decompressed buffer
	 */
	buffer_offset = offset % handle->chunk_size;

	while( size > 0 )
	{
		/* If we no longer have any more blocks the end of file was reached
		 */
		if( chunk >= handle->offset_table->amount )
		{
			break;
		}
		/* Work out if this is a cache miss */
		if( handle->cached_chunk != chunk )
		{
			/* Determine the size of the chunk including the CRC
			 */
			chunk_data_size = handle->offset_table->size[ chunk ];

			if( chunk_data_size > handle->allocated_chunk_data_size )
			{
				handle = libewf_handle_cache_realloc( handle, chunk_data_size );
			}
			/* Prevent data contamination, wipe the cache buffers clean
			 */
			handle = libewf_handle_cache_wipe( handle );

			/* Read the chunk data
			 */
			chunk_read_count = libewf_read_chunk( handle, chunk, handle->chunk_data, handle->allocated_chunk_data_size );

			/* The size of the data within the chunk is the amount of bytes read
			 * minus the 4 bytes for the CRC
			 */
			chunk_data_size = chunk_read_count - EWF_CRC_SIZE;

			/* Determine if the chunk is compressed
			 */
			if( handle->offset_table->compressed[ chunk ] == 1 )
			{
				raw_data_size = handle->chunk_size;

				/* Try to decompress the chunk
				 */
				result = ewf_sectors_chunk_uncompress( handle->raw_data, &raw_data_size, handle->chunk_data, chunk_read_count );

				LIBEWF_VERBOSE_PRINT( "libewf_read_random: chunk %" PRIu32 " of %" PRIu32 " (%i%%) is COMPRESSED.\n",
				( chunk + 1 ), handle->offset_table->amount,
				(int) ( ( handle->offset_table->last > 0 ) ? ( ( chunk * 100 ) / handle->offset_table->last ) : 1 ) ) ;

				if( result != 1 )
				{
					LIBEWF_FATAL_PRINT( "libewf_read_random: unable to uncompress chunk.\n" );
				}
				handle->compression_used = 1;
				handle->cached_data_size = raw_data_size;
			}
			else
			{
				LIBEWF_VERBOSE_PRINT( "libewf_read_random: chunk %" PRIu32 " of %" PRIu32 " (%i%%) is UNCOMPRESSED.\n",
				( chunk + 1 ), handle->offset_table->amount,
				(int) ( ( handle->offset_table->last > 0 ) ? ( ( chunk * 100 ) / handle->offset_table->last ) : 1 ) ) ;

				memcpy( (uint8_t *) handle->raw_data, (uint8_t *) handle->chunk_data, chunk_data_size );

				handle->cached_data_size = chunk_data_size;
			}
			handle->cached_chunk = chunk;
		}
		/* The available amount of data within the raw data buffer
		 */
		available = handle->cached_data_size - buffer_offset;

		/* Correct the available amount of bytes is larger
		 * than the requested amount of bytes
		 */
		if( available > size )
		{
			available = size;
		}

		/* Copy the relevant data to buffer
		 */
		memcpy( (uint8_t *) ( buffer + count_read ), (uint8_t *) ( handle->raw_data + buffer_offset ), available );

		size          -= available;
		count_read    += available;
		buffer_offset  = 0;

		chunk++;
	}
	if( handle->swap_byte_pairs == 1 )
	{
		swap_byte_pairs( buffer, count_read );
	}
	return( count_read );
}

/* Reads the data to a file descriptor
 * Returns a -1 on error, the amount of bytes read on success
 */
int64_t libewf_read_to_file_descriptor( LIBEWF_HANDLE *handle, int output_file_descriptor, void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) )
{
	LIBEWF_MD5_CTX md5;

	char *data                      = NULL;
	char *calculated_md5hash_string = NULL;
	char *stored_md5hash_string     = NULL;
	EWF_MD5HASH *calculated_md5hash = NULL;
	uint64_t offset                 = 0;
	int64_t count                   = 0;
	int64_t total_count             = 0;
	uint64_t iterator               = 0;
	uint64_t total_size             = ( handle->offset_table->amount * handle->chunk_size );

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_to_file_descriptor: incorrect handle.\n" );
	}
	if( handle->index_build == 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_to_file_descriptor: index was not build.\n" );
	}
	LIBEWF_MD5_INIT( &md5 );

	data = (char *) malloc( sizeof( char ) * handle->chunk_size );

	if( data == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_to_file_descriptor: unable to allocate data.\n" );
	}
	for( iterator = 0; iterator < handle->offset_table->amount; iterator++ )
	{
		offset = iterator * handle->chunk_size;
		count  = libewf_read_random( handle, data, handle->chunk_size, offset );

		LIBEWF_MD5_UPDATE( &md5, data, count );

		if( libewf_write( output_file_descriptor, data, count ) < count )
		{
			libewf_free( data );

			LIBEWF_FATAL_PRINT( "libewf_read_to_file_descriptor: error writing data.\n" );
		}
		total_count += count;

		if( callback != NULL )
		{
			callback( total_count, total_size );
		}
  	}
	free( data ) ;

	calculated_md5hash = ewf_md5hash_alloc();

	if( calculated_md5hash == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_to_file_descriptor: unable to create MD5 hash.\n" );
	}
  	LIBEWF_MD5_FINAL( calculated_md5hash, &md5 );

	calculated_md5hash_string = ewf_md5hash_to_string( calculated_md5hash );

	if( calculated_md5hash_string == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_to_file_descriptor: unable to create MD5 hash string.\n" );
	}
	/* If MD5 hash is NULL no hash section was found in the file
	 */
	if( handle->md5hash != NULL )
	{
		stored_md5hash_string = ewf_md5hash_to_string( handle->md5hash );

		LIBEWF_VERBOSE_PRINT( "libewf_read_to_file_descriptor: MD5 hash stored: %s, calculated: %s.\n", stored_md5hash_string, calculated_md5hash_string );

		if( memcmp( calculated_md5hash, handle->md5hash, 16 ) != 0 )
		{
			LIBEWF_FATAL_PRINT( "libewf_read_to_file_descriptor: MD5 hash does not match.\n" );
		}
		free( stored_md5hash_string );
	}
	else
	{
		LIBEWF_VERBOSE_PRINT( "libewf_read_to_file_descriptor: MD5 hash stored: NONE, calculated: %s.\n", calculated_md5hash_string );
	}
	ewf_md5hash_free( calculated_md5hash );

	free( calculated_md5hash_string );

	return( total_count );
}

