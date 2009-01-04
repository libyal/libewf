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

#include "libewf_common.h"
#include "libewf_definitions.h"
#include "libewf_debug.h"
#include "libewf_endian.h"
#include "libewf_notify.h"
#include "libewf_md5.h"
#include "libewf_file.h"
#include "libewf_file_read.h"
#include "libewf_segment_table.h"

/* Reads a header section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_header_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	EWF_HEADER *header = NULL;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_header_read: incorrect handle.\n" );

		return( -1 );
	}
	header = ewf_header_read( file_descriptor, &size );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_header_read: unable to read header.\n" );

		return( -1 );
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
	return( (int32_t) size );
}

/* Reads a header2 section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_header2_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	EWF_HEADER *header2 = NULL;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_header2_read: incorrect handle.\n" );

		return( -1 );
	}
	header2 = ewf_header2_read( file_descriptor, size );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_header2_read: unable to read header2.\n" );

		return( -1 );
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
	return( (int32_t) size );
}

/* Reads an EWF-S01 (SMART) volume section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_volume_s01_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	EWF_VOLUME_SMART *volume_smart = NULL;
	EWF_CRC *calculated_crc        = NULL;
	EWF_CRC stored_crc             = 0;
	int32_t bytes_per_chunk        = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: incorrect handle.\n" );

		return( -1 );
	}
	if( size != EWF_VOLUME_SMART_SIZE )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: mismatch in section volume size.\n" );

		return( -1 );
	}
	volume_smart = ewf_volume_smart_read( file_descriptor );

	if( volume_smart == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to read volume.\n" );

		return( -1 );
	}
#ifdef _LIBEWF_DEBUG_
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown2, 20 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown3, 45 ); );
#endif

	calculated_crc = ewf_crc_calculate( (void *) volume_smart, ( EWF_VOLUME_SMART_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to calculate CRC.\n" );

		ewf_volume_smart_free( volume_smart );

		return( -1 );
	}
	stored_crc      = libewf_endian_convert_32bit( volume_smart->crc );
	bytes_per_chunk = ewf_volume_smart_calculate_chunk_size( volume_smart );

	if( bytes_per_chunk <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to calculate chunk size - using default.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		bytes_per_chunk = EWF_MINIMUM_CHUNK_SIZE;
	}
	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	handle->chunk_count       = libewf_endian_convert_32bit( volume_smart->chunk_count );
	handle->sectors_per_chunk = libewf_endian_convert_32bit( volume_smart->sectors_per_chunk );
	handle->bytes_per_sector  = libewf_endian_convert_32bit( volume_smart->bytes_per_sector );
	handle->sector_count      = libewf_endian_convert_32bit( volume_smart->sector_count );
	handle->media_type        = libewf_endian_convert_32bit( volume_smart->unknown3 );

	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_s01_read: This volume has %" PRIu32 " chunks of %" PRIi32 " bytes each, CRC %" PRIu32 " (%" PRIu32 ").\n", handle->chunk_count, bytes_per_chunk, stored_crc, *calculated_crc );

	ewf_volume_smart_free( volume_smart );
	ewf_crc_free( calculated_crc );

	return( (int32_t) size );
}

/* Reads an EWF-E01 (EnCase) volume section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_volume_e01_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	EWF_VOLUME *volume      = NULL;
	EWF_CRC *calculated_crc = NULL;
	void *data_set          = NULL;
	EWF_CRC stored_crc      = 0;
	int32_t bytes_per_chunk = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: incorrect handle.\n" );

		return( -1 );
	}
	if( size != EWF_VOLUME_SIZE )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: mismatch in section volume size.\n" );

		return( -1 );
	}
	volume = ewf_volume_read( file_descriptor );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to read volume.\n" );

		return( -1 );
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
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to calculate CRC.\n" );

		ewf_volume_free( volume );

		return( -1 );
	}
	stored_crc      = libewf_endian_convert_32bit( volume->crc );
	bytes_per_chunk = ewf_volume_calculate_chunk_size( volume );

	if( bytes_per_chunk <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to calculate chunk size - using default.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		bytes_per_chunk = EWF_MINIMUM_CHUNK_SIZE;
	}
	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	handle->chunk_count       = libewf_endian_convert_32bit( volume->chunk_count );
	handle->sectors_per_chunk = libewf_endian_convert_32bit( volume->sectors_per_chunk );
	handle->bytes_per_sector  = libewf_endian_convert_32bit( volume->bytes_per_sector );
	handle->sector_count      = libewf_endian_convert_32bit( volume->sector_count );
	handle->media_type        = libewf_endian_convert_32bit( volume->unknown3 );
	handle->compression_level = volume->compression_level;

	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_e01_read: this volume has %" PRIu32 " chunks of %" PRIi32 " bytes each, CRC %" PRIu32 " (%" PRIu32 ").\n", handle->chunk_count, bytes_per_chunk, stored_crc, *calculated_crc );

	data_set = libewf_memcpy( (void *) handle->guid, (void *) volume->guid, 16 );

	if( data_set == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to set GUID.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	ewf_volume_free( volume );
	ewf_crc_free( calculated_crc );

	return( (int32_t) size );
}

/* Reads a volume section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_volume_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	int32_t count = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_read: incorrect handle.\n" );

		return( -1 );
	}
	if( size == EWF_VOLUME_SMART_SIZE )
	{
		handle->ewf_format = EWF_FORMAT_S01;
		count              = libewf_section_volume_s01_read( handle, file_descriptor, size );
	}
	else if( size == EWF_VOLUME_SIZE )
	{
		handle->ewf_format = EWF_FORMAT_E01;
		count              = libewf_section_volume_e01_read( handle, file_descriptor, size );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_read: mismatch in section data size.\n" );

		return( -1 );
	}
	if( ( count <= -1 ) || ( count != (int32_t) size ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_read: unable to read volume section.\n" );

		return( -1 );
	}
	if( handle->chunk_count == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_read: volume chunk count is 0 - compensating.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		handle->chunk_count = 1;
	}
	return( count );
}

/* Fills the offset table
 * Returns a pointer to the instance, or NULL on error
 */
LIBEWF_OFFSET_TABLE *libewf_fill_offset_table( LIBEWF_OFFSET_TABLE *offset_table, EWF_TABLE_OFFSET *offsets, uint32_t chunk_amount, int file_descriptor, uint8_t error_tollerance )
{
	LIBEWF_OFFSET_TABLE *data_set = NULL;
	uint64_t chunk_size           = 0;
	uint64_t iterator             = 0;
	uint64_t current_offset       = 0;
	uint64_t next_offset          = 0;
	uint32_t size_of_chunks       = 0;
	uint32_t raw_offset           = 0;
	uint8_t compressed            = 0;

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: invalid offset table.\n" );

		return( NULL );
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
		offset_table = libewf_offset_table_realloc( offset_table, size_of_chunks );

		if( offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: unable to reallocate offset table.\n" );

			return( NULL );
		}
	}
	/* Read the offsets from file
	 */
	raw_offset = libewf_endian_convert_32bit( offsets[ iterator ].offset );

	/* The size of the last chunk must be determined differently
	 */
	while( iterator < ( chunk_amount - 1 ) )
	{
		compressed     = raw_offset >> 31;
		current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		raw_offset     = libewf_endian_convert_32bit( offsets[ iterator + 1 ].offset );
		next_offset    = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		chunk_size     = next_offset - current_offset;

		data_set       = libewf_offset_table_set_values( offset_table, offset_table->last, file_descriptor, compressed, current_offset, chunk_size );

		if( data_set == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: unable to set value in offset table.\n" );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( NULL );
			}
		}
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
	raw_offset     = libewf_endian_convert_32bit( offsets[ iterator ].offset );
	compressed     = raw_offset >> 31;
	current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	offset_table   = libewf_offset_table_set_values( offset_table, offset_table->last, file_descriptor, compressed, current_offset, 0 );

	if( data_set == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: unable to set value in offset table.\n" );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( NULL );
		}
	}
	return( offset_table );
}

/* Calculate the last offset
 * Returns a pointer to the instance, or NULL on error
 */
LIBEWF_OFFSET_TABLE *libewf_calculate_last_offset( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, int file_descriptor, uint8_t error_tollerance )
{
	LIBEWF_OFFSET_TABLE *data_set                 = NULL;
	LIBEWF_SECTION_LIST_ENTRY *section_list_entry = NULL;
	uint64_t last_offset                          = 0;
	uint64_t chunk_size                           = 0;

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: incorrect offset table.\n" );

		return( NULL );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: incorrect section list.\n" );

		return( NULL );
	}
	if( section_list->file_descriptor != file_descriptor )
	{
		LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: file descriptor does not match the one in the section list.\n" );

		return( NULL );
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
#ifdef _LIBEWF_DEBUG_
		LIBEWF_VERBOSE_PRINT( "libewf_calculate_last_offset: start offset: %" PRIu64 " last offset: %" PRIu64 ".\n", section_list_entry->start_offset, last_offset );
#endif

		if( ( section_list_entry->start_offset < last_offset ) && ( last_offset < section_list_entry->end_offset ) )
		{
			chunk_size = section_list_entry->end_offset - last_offset;
			data_set   = libewf_offset_table_set_values( offset_table, offset_table->last, file_descriptor, offset_table->compressed[ offset_table->last ], last_offset, chunk_size );

			if( data_set == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: unable to set value in offset table.\n" );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( NULL );
				}
			}
			LIBEWF_VERBOSE_PRINT( "libewf_calculate_last_offset: last chunk %" PRIu64 " calculated with offset %" PRIu64 " and size %" PRIu64 ".\n", ( offset_table->last + 1 ), last_offset, chunk_size );

			break;
		}
		section_list_entry = section_list_entry->next;
	}
	return( offset_table );
}

/* Reads an offset table
 * Returns a pointer to the instance, or NULL on error
 */
LIBEWF_OFFSET_TABLE *libewf_offset_table_read( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, int file_descriptor, uint32_t size, uint8_t ewf_format, uint8_t error_tollerance )
{
	EWF_TABLE *table          = NULL;
	EWF_TABLE_OFFSET *offsets = NULL;
	EWF_CRC *calculated_crc   = NULL;
	EWF_CRC *stored_crc_read  = NULL;
	EWF_CRC stored_crc        = 0;
	uint32_t chunk_count      = 0;

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: incorrect offset table.\n" );

		return( NULL );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: incorrect section list.\n" );

		return( NULL );
	}
	table = ewf_table_read( file_descriptor );

	if( table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to read table.\n" );

		return( NULL );
	}
#ifdef _LIBEWF_DEBUG_
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( table->padding, 16 ); );
#endif

	/* The table size contains the size of the CRC (4 bytes)
	 */
	calculated_crc = ewf_crc_calculate( (void *) table, ( EWF_TABLE_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to calculate CRC.\n" );

		ewf_table_free( table );

		return( NULL );
	}
	stored_crc = libewf_endian_convert_32bit( table->crc );

	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			ewf_crc_free( calculated_crc );
			ewf_table_free( table );

			return( NULL );
		}
	}
	chunk_count = libewf_endian_convert_32bit( table->chunk_count );

	LIBEWF_VERBOSE_PRINT( "libewf_offset_table_read: table is of size %" PRIu32 " chunks CRC %" PRIu32 " (%" PRIu32 ").\n", chunk_count, stored_crc, *calculated_crc );

	ewf_crc_free( calculated_crc );
	ewf_table_free( table );

	if( chunk_count <= 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: table contains no offsets!.\n" );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( NULL );
		}
	}
	else
	{
		offsets = ewf_table_offsets_read( file_descriptor, chunk_count );

		if( offsets == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to read table offsets.\n" );

			return( NULL );
		}
		if( ewf_format == EWF_FORMAT_E01 )
		{
			calculated_crc = ewf_crc_calculate( offsets, ( EWF_TABLE_OFFSET_SIZE * chunk_count ), 1 );

			if( calculated_crc == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to calculate CRC.\n" );

				ewf_table_offsets_free( offsets );

				return( NULL );
			}
			stored_crc_read = ewf_crc_read( file_descriptor );

			if( stored_crc_read == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to read CRC from file descriptor.\n" );

				ewf_table_offsets_free( offsets );
				ewf_crc_free( calculated_crc );

				return( NULL );
			}
			if( *stored_crc_read != *calculated_crc )
			{
				LIBEWF_WARNING_PRINT( "libewf_offset_table_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", *stored_crc_read, *calculated_crc );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					ewf_table_offsets_free( offsets );
					ewf_crc_free( calculated_crc );
					ewf_crc_free( stored_crc_read );

					return( NULL );
				}
			}
			ewf_crc_free( calculated_crc );
			ewf_crc_free( stored_crc_read );
		}
		offset_table = libewf_fill_offset_table( offset_table, offsets, chunk_count, file_descriptor, error_tollerance );
		offset_table = libewf_calculate_last_offset( offset_table, section_list, file_descriptor, error_tollerance );

		ewf_table_offsets_free( offsets );
	}
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
		LIBEWF_WARNING_PRINT( "libewf_compare_offset_tables: invalid offset table.\n" );

		return( 0 );
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
				LIBEWF_VERBOSE_PRINT( "libewf_compare_offset_tables: offset tables differ in offset for chunk: %" PRIu64 " (table1: %" PRIu64 ", table2: %" PRIu64 ").\n", iterator, offset_table1->offset[ iterator ], offset_table2->offset[ iterator ] );

				return( 0 );
			}
		}
	}
	return( 1 );
}

/* Reads a table section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_table_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size, LIBEWF_SECTION_LIST *section_list )
{
	LIBEWF_OFFSET_TABLE *offset_table = NULL;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_read: incorrect handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_read: incorrect section list.\n" );

		return( -1 );
	}
	if( handle->offset_table == NULL )
	{
		handle->offset_table = libewf_offset_table_alloc( handle->chunk_count );

		if( handle->offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_table_read: unable to create offset table.\n" );

			return( -1 );
		}
	}
	offset_table = libewf_offset_table_read( handle->offset_table, section_list, file_descriptor, size, handle->ewf_format, handle->error_tollerance );

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_read: unable to read offset table.\n" );

		return( -1 );
	}
	handle->offset_table = offset_table;

	return( (int32_t) size );
}

/* Reads an EWF-S01 table2 section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_table2_s01_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size, LIBEWF_SECTION_LIST *section_list )
{
	LIBEWF_OFFSET_TABLE *offset_table = NULL;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_s01_read: incorrect handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_s01_read: incorrect section list.\n" );

		return( -1 );
	}
	offset_table = libewf_offset_table_read( handle->offset_table, section_list, file_descriptor, size, handle->ewf_format, handle->error_tollerance );

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_s01_read: unable to read offset table.\n" );

		return( -1 );
	}
	handle->offset_table = offset_table;

	return( (int32_t) size );
}

/* Reads an EWF-E01 table2 section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_table2_e01_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size, LIBEWF_SECTION_LIST *section_list )
{
	LIBEWF_OFFSET_TABLE *offset_table = NULL;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_e01_read: incorrect handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_e01_read: incorrect section list.\n" );

		return( -1 );
	}
	if( handle->secondary_offset_table == NULL )
	{
		handle->secondary_offset_table = libewf_offset_table_alloc( handle->chunk_count );

		if( handle->secondary_offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_table2_e01_read: unable to create secondairy offset table.\n" );

			return( -1 );
		}
	}
	offset_table = libewf_offset_table_read( handle->secondary_offset_table, section_list, file_descriptor, size, handle->ewf_format, handle->error_tollerance );

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_e01_read: unable to read offset table.\n" );

		return( -1 );
	}
	handle->secondary_offset_table = offset_table;

	if( libewf_compare_offset_tables( handle->offset_table, handle->secondary_offset_table ) == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_e01_read: table1 and table2 differ.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		/* TODO Try to correct the table
		 */
	}
	return( (int32_t) size );
}

/* Reads a table2 section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_table2_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size, LIBEWF_SECTION_LIST *section_list )
{
	int32_t count = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_read: incorrect handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_read: incorrect section list.\n" );

		return( -1 );
	}
	/* In the EWF-S01 format the table2 section is an addition on the table section
	 */
	if( handle->ewf_format == EWF_FORMAT_S01 )
	{
		count = libewf_section_table2_s01_read( handle, file_descriptor, size, section_list );
	}
	/* In the EWF-E01 format the table2 section is a replicate of the table section
	 */
	else if( handle->ewf_format == EWF_FORMAT_E01 )
	{
		count = libewf_section_table2_e01_read( handle, file_descriptor, size, section_list );
	}
	if( ( count <= -1 ) || ( count != (int32_t) size ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_read: unable to read table2 section.\n" );

		return( -1 );
	}
	return( count );
}

/* Reads a sectors section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_sectors_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: incorrect handle.\n" );

		return( -1 );
	}
	/* In the EWF-E01 format the sectors section holds the actual data chunks
	 */
	if( handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: EWF-S01 format should not contain sectors section.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	return( (int32_t) size );
}

/* Reads a ltree section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_ltree_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	EWF_LTREE *ltree       = NULL;
	EWF_HEADER *tree_data  = NULL;
/*
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;
*/

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_ltree_read: incorrect handle.\n" );

		return( -1 );
	}
	if( handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: EWF-S01 format should not contain ltree section.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	handle->ewf_format = EWF_FORMAT_L01;
/*
	if( size != EWF_LTREE_SIZE )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_ltree_read: mismatch in section data size.\n" );

		return( -1 );
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
	LIBEWF_VERBOSE_EXEC( libewf_debug_read_section( handle, file_descriptor, ( size - EWF_LTREE_SIZE ) ); );
*/
	tree_data = ewf_tree_data_read( file_descriptor, ( size - EWF_LTREE_SIZE ) );

	LIBEWF_VERBOSE_EXEC( ewf_header_fprint( stderr, tree_data ); );

	ewf_header_free( tree_data );

	return( (int32_t) size );
}

/* Reads a data section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_data_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	EWF_DATA *data          = NULL;
	EWF_CRC *calculated_crc = NULL;
	EWF_CRC stored_crc      = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: incorrect handle.\n" );

		return( -1 );
	}
	if( handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: EWF-S01 format should not contain data section.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	if( size != EWF_DATA_SIZE )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: mismatch in section data size.\n" );

		return( -1 );
	}
	data = ewf_data_read( file_descriptor );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to read data.\n" );

		return( -1 );
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
	calculated_crc = ewf_crc_calculate( (void *) data, ( EWF_DATA_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to calculate CRC.\n" );

		ewf_data_free( data );

		return( -1 );
	}
	stored_crc = libewf_endian_convert_32bit( data->crc );

	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			ewf_data_free( data );
			ewf_crc_free( calculated_crc );

			return( -1 );
		}
	}
	ewf_crc_free( calculated_crc );

	/* TODO add more checks
	 */
	if( handle->chunk_count != libewf_endian_convert_32bit( data->chunk_count ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: chunk count does not match in data section.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			ewf_data_free( data );

			return( -1 );
		}
	}
	if( handle->sectors_per_chunk != libewf_endian_convert_32bit( data->sectors_per_chunk ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: sectors per chunk does not match in data section.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			ewf_data_free( data );

			return( -1 );
		}
	}
	if( handle->bytes_per_sector != libewf_endian_convert_32bit( data->bytes_per_sector ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: bytes per sector does not match in data section.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			ewf_data_free( data );

			return( -1 );
		}
	}
	if( handle->sector_count != libewf_endian_convert_32bit( data->sector_count ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: sector count does not match in data section.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			ewf_data_free( data );

			return( -1 );
		}
	}
	ewf_data_free( data );

	return( (int32_t) size );
}

/* Reads a error2 section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_error2_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	EWF_ERROR2 *error2         = NULL;
	EWF_ERROR2_SECTOR *sectors = NULL;
	EWF_CRC *calculated_crc    = NULL;
	EWF_CRC *stored_crc_read   = NULL;
	EWF_CRC stored_crc         = 0;
	uint32_t error_count       = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: incorrect handle.\n" );

		return( -1 );
	}
	if( handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: EWF-S01 format should not contain data section.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	error2 = ewf_error2_read( file_descriptor );

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to read error2.\n" );

		return( -1 );
	}
#ifdef _LIBEWF_DEBUG_
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( error2->unknown, 200 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( (uint8_t *) sectors, ( EWF_ERROR2_SECTOR_SIZE * error_count ) ); );
#endif

	calculated_crc = ewf_crc_calculate( (void *) error2, ( EWF_ERROR2_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to calculate CRC.\n" );

		ewf_error2_free( error2 );

		return( -1 );
	}
	stored_crc  = libewf_endian_convert_32bit( error2->crc );
	error_count = libewf_endian_convert_32bit( error2->error_count );

	ewf_error2_free( error2 );

	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			ewf_crc_free( calculated_crc );

			return( -1 );
		}
	}
	ewf_crc_free( calculated_crc );

	if( error_count <= 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: error2 contains no sectors!.\n" );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	else
	{
		sectors = ewf_error2_sectors_read( file_descriptor, error_count );

		if( sectors == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to read error2 sectors.\n" );

			return( -1 );
		}
		calculated_crc = ewf_crc_calculate( (void *) sectors, ( EWF_ERROR2_SECTOR_SIZE * error_count ), 1 );

		if( calculated_crc == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to calculate CRC.\n" );

			ewf_error2_sectors_free( sectors );

			return( -1 );
		}
		stored_crc_read = ewf_crc_read( file_descriptor );

		if( stored_crc_read == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to read CRC from file descriptor.\n" );

			ewf_error2_sectors_free( sectors );
			ewf_crc_free( calculated_crc );

			return( -1 );
		}
		if( *stored_crc_read != *calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", *stored_crc_read, *calculated_crc );

			if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				ewf_error2_sectors_free( sectors );
				ewf_crc_free( calculated_crc );
				ewf_crc_free( stored_crc_read );

				return( -1 );
			}
		}
		handle->error2_error_count = error_count;
		handle->error2_sectors     = sectors;

		ewf_crc_free( calculated_crc );
		ewf_crc_free( stored_crc_read );
	}
	return( (int32_t) size );
}

/* Reads a hash section
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_hash_read( LIBEWF_HANDLE *handle, int file_descriptor, uint32_t size )
{
	EWF_HASH *hash          = NULL;
	EWF_CRC *calculated_crc = NULL;
	EWF_CRC stored_crc      = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: incorrect handle.\n" );

		return( -1 );
	}
	if( size != EWF_HASH_SIZE )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: mismatch in section data size.\n" );

		return( -1 );
	}
	hash = ewf_hash_read( file_descriptor );

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: unable to read hash.\n" );

		return( -1 );
	}
#ifdef _LIBEWF_DEBUG_
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash->unknown1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash->unknown2, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash->signature, 8 ); );
#endif

	calculated_crc = ewf_crc_calculate( (void *) hash, ( EWF_HASH_SIZE - EWF_CRC_SIZE ), 1 );

	if( calculated_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: unable to calculate CRC.\n" );

		ewf_hash_free( hash );

		return( -1 );
	}
	stored_crc = libewf_endian_convert_32bit( hash->crc );

	if( stored_crc != *calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );

		if( handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			ewf_hash_free( hash );
			ewf_crc_free( calculated_crc );

			return( -1 );
		}
	}
	libewf_handle_set_md5hash( handle, hash->md5hash );

	ewf_hash_free( hash );
	ewf_crc_free( calculated_crc );

	return( (int32_t) size );
}

/* Reads and processes a section's data from a segment
 * Returns the amount of bytes read, or -1 on error
 */
int32_t libewf_section_data_read_segment( LIBEWF_HANDLE *handle, uint32_t segment, EWF_SECTION *section, int file_descriptor, LIBEWF_SECTION_LIST *section_list )
{
	uint32_t size = 0;
	int32_t count = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read_segment: incorrect handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read_segment: incorrect section list.\n" );

		return( -1 );
	}
	size = libewf_endian_convert_64bit( section->size ) - EWF_SECTION_SIZE;

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
		count = libewf_section_header2_read( handle, file_descriptor, size );
	}
	/* Read the header section
	 */
	else if( ewf_section_is_type_header( section ) )
	{
		count = libewf_section_header_read( handle, file_descriptor, size );
	}
	/* Read the volume or disk section
	 */
	else if( ewf_section_is_type_volume( section ) || ewf_section_is_type_disk( section ) )
	{
		count = libewf_section_volume_read( handle, file_descriptor, size );
	}
	/* Read the table2 section
	 */
	else if( ewf_section_is_type_table2( section ) )
	{
		count = libewf_section_table2_read( handle, file_descriptor, size, section_list );
	}
	/* Read the table section
	 */
	else if( ewf_section_is_type_table( section ) )
	{
		count = libewf_section_table_read( handle, file_descriptor, size, section_list );
	}
	/* Read the sectors section
	 */
	else if( ewf_section_is_type_sectors( section ) )
	{
		count = libewf_section_sectors_read( handle, file_descriptor, size );
	}
	/* Read the ltree section
	 */
	else if( ewf_section_is_type_ltree( section ) )
	{
		count = libewf_section_ltree_read( handle, file_descriptor, size );
	}
	/* Read the data section
	 */
	else if( ewf_section_is_type_data( section ) )
	{
		count = libewf_section_data_read( handle, file_descriptor, size );
	}
	/* Read the hash section
	 */
	else if( ewf_section_is_type_hash( section ) )
	{
		count = libewf_section_hash_read( handle, file_descriptor, size );
	}
	/* Read the error2 section
	 */
	else if( ewf_section_is_type_error2( section ) )
	{
		count = libewf_section_error2_read( handle, file_descriptor, size );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read_segment: unsupported section type: %s.\n", section->type );

#ifdef _LIBEWF_DEBUG_
		LIBEWF_VERBOSE_EXEC( libewf_debug_read_section( handle, file_descriptor, size ); );
#endif
	}
	if( count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read_segment: unable to read section: %s.\n", section->type );
	}
	return( (int32_t) size );
}

/* Reads and processes section in a segment
 * Returns the last section read
 */
EWF_SECTION *libewf_sections_read_segment( LIBEWF_HANDLE *handle, uint32_t segment )
{
	EWF_SECTION *section          = NULL;
	EWF_CRC *calculated_crc       = NULL;
	LIBEWF_SECTION_LIST *data_set = NULL;
	char *filename                = NULL;
	EWF_CRC stored_crc            = 0;
	uint64_t offset_end           = 0;
	int file_descriptor           = 0;

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

	if( file_descriptor <= -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_sections_read_segment: invalid file descriptor.\n" );
	}
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
		stored_crc = libewf_endian_convert_32bit( section->crc );

		if( stored_crc != *calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "libewf_sections_read_segment: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, *calculated_crc );
		}
		ewf_crc_free( calculated_crc );

		next_offset = libewf_endian_convert_64bit( section->next );

		LIBEWF_VERBOSE_EXEC( ewf_section_fprint( stderr, section ); );

#ifdef _LIBEWF_DEBUG_
		LIBEWF_VERBOSE_EXEC( libewf_dump_data( section->padding, 40 ); );
#endif

		offset_end = previous_offset + libewf_endian_convert_64bit( section->size );
		data_set   = libewf_section_list_append( handle->segment_table->section_list[ segment ], file_descriptor, previous_offset, offset_end );

		if( data_set == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_sections_read_segment: unable to append value.\n" );
		}
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
	int32_t count           = 0;
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
		stored_crc = libewf_endian_convert_32bit( (uint8_t *) ( buffer + count - EWF_CRC_SIZE ) );

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
	LIBEWF_CHUNK_CACHE *chunk_cache = NULL;
	uint64_t buffer_offset          = 0;
	uint64_t chunk_data_size        = 0;
	uint64_t available              = 0;
	int64_t chunk_read_count        = 0;
	int64_t count_read              = 0;
	uint32_t chunk                  = 0;
	uint32_t raw_data_size          = 0;
	int8_t result                   = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_random: incorrect handle.\n" );
	}
	if( handle->index_build == 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_random: index was not build.\n" );
	}
	if( handle->chunk_cache == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_random: invalid chunk cache.\n" );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_read_random: reading from offset: %" PRIu64 " size: %" PRIu64 ".\n", offset, size );

	/* The chunk we are after
	 */
	chunk = (uint32_t) ( offset / handle->chunk_size );

	if( chunk >= handle->offset_table->amount )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_random: Attempting to read past the end of the file.\n");
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
		if( handle->chunk_cache->identifier != chunk )
		{
			/* Determine the size of the chunk including the CRC
			 */
			chunk_data_size = handle->offset_table->size[ chunk ];

			if( chunk_data_size > handle->chunk_cache->allocated_size )
			{
				chunk_cache = libewf_chunk_cache_realloc( handle->chunk_cache, chunk_data_size );

				if( chunk_cache == NULL )
				{
					LIBEWF_FATAL_PRINT( "libewf_read_random: unable to reallocate chunk cache.\n");
				}
				handle->chunk_cache = chunk_cache;
			}
			/* Prevent data contamination, wipe the cache buffers clean
			 */
			chunk_cache = libewf_chunk_cache_wipe( handle->chunk_cache );

			if( chunk_cache == NULL )
			{
				LIBEWF_FATAL_PRINT( "libewf_read_random: unable to wipe chunk cache.\n");
			}
			handle->chunk_cache = chunk_cache;

			/* Read the chunk data
			 */
			chunk_read_count = libewf_read_chunk( handle, chunk, handle->chunk_cache->read, handle->chunk_cache->allocated_size );

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
				result = ewf_sectors_chunk_uncompress( handle->chunk_cache->data, &raw_data_size, handle->chunk_cache->read, chunk_read_count );

				LIBEWF_VERBOSE_PRINT( "libewf_read_random: chunk %" PRIu32 " of %" PRIu32 " (%i%%) is COMPRESSED.\n",
				( chunk + 1 ), handle->offset_table->amount,
				(int) ( ( handle->offset_table->last > 0 ) ? ( ( chunk * 100 ) / handle->offset_table->last ) : 1 ) ) ;

				if( result != 1 )
				{
					LIBEWF_FATAL_PRINT( "libewf_read_random: unable to uncompress chunk.\n" );
				}
				handle->compression_used    = 1;
				handle->chunk_cache->amount = raw_data_size;
			}
			else
			{
				LIBEWF_VERBOSE_PRINT( "libewf_read_random: chunk %" PRIu32 " of %" PRIu32 " (%i%%) is UNCOMPRESSED.\n",
				( chunk + 1 ), handle->offset_table->amount,
				(int) ( ( handle->offset_table->last > 0 ) ? ( ( chunk * 100 ) / handle->offset_table->last ) : 1 ) ) ;

				libewf_memcpy( (uint8_t *) handle->chunk_cache->data, (uint8_t *) handle->chunk_cache->read, chunk_data_size );

				handle->chunk_cache->amount = chunk_data_size;
			}
			handle->chunk_cache->identifier = chunk;
		}
		/* The available amount of data within the cached chunk
		 */
		available = handle->chunk_cache->amount - buffer_offset;

		/* Correct the available amount of bytes is larger
		 * than the requested amount of bytes
		 */
		if( available > size )
		{
			available = size;
		}
		/* Copy the relevant data to buffer
		 */
		libewf_memcpy( ( buffer + count_read ), (void *) &handle->chunk_cache->data[ buffer_offset ], available );

		size          -= available;
		count_read    += available;
		buffer_offset  = 0;

		chunk++;
	}
	if( handle->swap_byte_pairs == 1 )
	{
		libewf_endian_swap_byte_pairs( buffer, count_read );
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

		if( libewf_memcmp( calculated_md5hash, handle->md5hash, 16 ) != 0 )
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

