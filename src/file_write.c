/*
 * libewf file writing
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
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <time.h>
#include <zlib.h>
#include <errno.h>

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
#include "ewf_section.h"
#include "ewf_volume.h"
#include "ewf_volume_smart.h"
#include "ewf_table.h"
#include "file_write.h"
#include "handle.h"
#include "section_list.h"
#include "libewf_offset_table.h"
#include "segment_table.h"

/* Check for empty block, a block that contains the same value for every byte
 * Returns 1 if block is empty
 * Returns 0 otherwise
 */
int test_empty_block( uint8_t *block_buffer, uint64_t block_size )
{
	ssize_t iterator = 0;

	if( block_buffer == NULL )
	{
		LIBEWF_FATAL_PRINT( "test_empty_block: invalid block buffer.\n" );
	}
	for( iterator = 1; iterator < block_size; iterator++ )
	{
		if( block_buffer[ 0 ] != block_buffer[ iterator ] )
		{
			return( 0 );
		}
	}
	return( 1 );
}

/* Write a section start to file
 */
int64_t libewf_section_write( LIBEWF_HANDLE *handle, int file_descriptor, char *section_type, uint64_t section_data_size, uint64_t start_offset )
{
	EWF_SECTION *section       = NULL;
	uint32_t section_type_size = 0;
	uint64_t section_size      = 0;
	uint64_t section_offset    = 0;
	ssize_t write_count        = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_write: incorrect handle.\n" );
	}
	section = ewf_section_alloc();

	if( section == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_write: unable to create section.\n" );
	}
	section_type_size = libewf_strlen( section_type );
	section_size      = EWF_SECTION_SIZE + section_data_size;
	section_offset    = start_offset + section_size;

	if( section_type_size >= 16 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_write: section type is too long.\n" );
	}
	memcpy( (uint8_t *) section->type, (uint8_t *) section_type, section_type_size );

	revert_64bit( section_size, section->size );
	revert_64bit( section_offset, section->next );

	write_count = ewf_section_write( section, file_descriptor );

	ewf_section_free( section );

	if( write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_write: unable to write section to file.\n" );
	}
	return( write_count );
}

/* Write the last section start to file
 * This is used for the next and done sections, these sections point back towards themselves
 */
int64_t libewf_last_section_write( LIBEWF_HANDLE *handle, int file_descriptor, char *section_type, uint64_t start_offset )
{
	EWF_SECTION *section       = NULL;
	uint64_t section_size      = 0;
	uint64_t section_offset    = 0;
	uint32_t section_type_size = 0;
	int32_t write_count        = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_last_section_write: incorrect handle.\n" );
	}
	section = ewf_section_alloc();

	if( section == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_last_section_write: unable to create section.\n" );
	}
	section_type_size = libewf_strlen( section_type );
	section_size      = EWF_SECTION_SIZE;
	section_offset    = start_offset;

	if( section_type_size >= 16 )
	{
		LIBEWF_FATAL_PRINT( "libewf_last_section_write: section type is too long.\n" );
	}
	memcpy( (uint8_t *) section->type, (uint8_t *) section_type, section_type_size );

	revert_64bit( section_size, section->size );
	revert_64bit( section_offset, section->next );

	write_count = ewf_section_write( section, file_descriptor );

	ewf_section_free( section );

	if( write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_last_section_write: unable to write section to file.\n" );
	}
	return( write_count );
}

/* Write a header section to file
 */
int64_t libewf_section_header_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset, EWF_HEADER *header, uint32_t size, int8_t compression_level )
{
	EWF_HEADER *compressed_header = NULL;
	int64_t section_write_count   = 0;
	int64_t header_write_count    = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header_write: incorrect handle.\n" );
	}
	if( header == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header_write: incorrect header.\n" );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_header_write: Header:.\n" );
	LIBEWF_VERBOSE_EXEC( ewf_header_fprint( stderr, header ); );

	compressed_header = ewf_header_compress( header, &size, compression_level );

	if( compressed_header == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header_write: unable to compress header.\n" );
	}
	section_write_count = libewf_section_write( handle, file_descriptor, "header", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header_write: unable to write section to file.\n" );
	}
	header_write_count = ewf_header_write( compressed_header, file_descriptor, size );

	ewf_header_free( compressed_header );

	if( header_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header_write: unable to write header to file.\n" );
	}
	return( section_write_count + header_write_count );
}

/* Write a header2 section to file
 */
int64_t libewf_section_header2_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset, EWF_HEADER *header, uint32_t size, int8_t compression_level )
{
	EWF_HEADER *utf16_header      = NULL;
	EWF_HEADER *compressed_header = NULL;
	uint32_t size_utf16           = 0;
	int64_t section_write_count   = 0;
	int64_t  header_write_count   = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header2_write: incorrect handle.\n" );
	}
	if( header == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header2_write: incorrect header.\n" );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_header2_write: Header:.\n" );
	LIBEWF_VERBOSE_EXEC( ewf_header_fprint( stderr, header ); );

	size_utf16   = ( size * 2 ) + 4;
	utf16_header = ewf_header2_convert_ascii_to_utf16( header, size, EWF_HEADER2_LITTLE_ENDIAN );

	if( utf16_header == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header2_write: unable to convert header to UTF16.\n" );
	}
	compressed_header = ewf_header_compress( utf16_header, &size_utf16, compression_level );

	if( compressed_header == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header2_write: unable to compress header.\n" );
	}
	section_write_count = libewf_section_write( handle, file_descriptor, "header2", size_utf16, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header2_write: unable to write section to file.\n" );
	}
	header_write_count = ewf_header_write( compressed_header, file_descriptor, size_utf16 );

	ewf_header_free( utf16_header );
	ewf_header_free( compressed_header );

	if( header_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_header2_write: unable to write header to file.\n" );
	}
	return( section_write_count + header_write_count );
}

/* Write a volume section to file
 */
int64_t libewf_section_volume_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset )
{
	EWF_VOLUME *volume          = NULL;
	int64_t section_write_count = 0;
	int64_t volume_write_count  = 0;
	size_t size                 = EWF_VOLUME_SIZE;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_write: incorrect handle.\n" );
	}
	volume = ewf_volume_alloc();

	if( volume == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_write: unable to create volume.\n" );
	}
	revert_32bit( handle->chunk_count, volume->chunk_count );
	revert_32bit( handle->sectors_per_chunk, volume->sectors_per_chunk );
	revert_32bit( handle->bytes_per_sector, volume->bytes_per_sector );
	revert_32bit( handle->sector_count, volume->sector_count );

	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_write: chunk_count: %" PRIu32 ", sectors_per_chunk: %" PRIu32 ", bytes_per_sector: %" PRIu32 ", sector_count: %" PRIu32 ".\n", volume->chunk_count, volume->sectors_per_chunk, volume->bytes_per_sector, volume->sector_count );

	if( handle->format == LIBEWF_FORMAT_ENCASE5 )
	{
		volume->compression_level = handle->compression_level;
	}
	section_write_count = libewf_section_write( handle, file_descriptor, "volume", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_write: unable to write section to file.\n" );
	}
	volume_write_count  = ewf_volume_write( volume, file_descriptor );

	ewf_volume_free( volume );

	if( volume_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_write: unable to write volume to file.\n" );
	}
	return( section_write_count + volume_write_count );
}

/* Write a volume smart section to file
 */
int64_t libewf_section_volume_smart_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset )
{
	EWF_VOLUME_SMART *volume    = NULL;
	int64_t section_write_count = 0;
	int64_t volume_write_count  = 0;
	size_t size                 = EWF_VOLUME_SMART_SIZE;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_smart_write: incorrect handle.\n" );
	}
	volume = ewf_volume_smart_alloc();

	if( volume == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_smart_write: unable to create volume.\n" );
	}
	revert_32bit( handle->chunk_count, volume->chunk_count );
	revert_32bit( handle->sectors_per_chunk, volume->sectors_per_chunk );
	revert_32bit( handle->bytes_per_sector, volume->bytes_per_sector );
	revert_32bit( handle->sector_count, volume->sector_count );

	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_smart_write: chunk_count: %" PRIu32 ", sectors_per_chunk: %" PRIu32 ", bytes_per_sector: %" PRIu32 ", sector_count: %" PRIu32 ".\n", volume->chunk_count, volume->sectors_per_chunk, volume->bytes_per_sector, volume->sector_count );

	if( handle->format == LIBEWF_FORMAT_SMART )
	{
		volume->signature[ 0 ] = 'S';
		volume->signature[ 1 ] = 'M';
		volume->signature[ 2 ] = 'A';
		volume->signature[ 3 ] = 'R';
		volume->signature[ 4 ] = 'T';
	}
	section_write_count = libewf_section_write( handle, file_descriptor, "volume", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_smart_write: unable to write section to file.\n" );
	}
	volume_write_count = ewf_volume_smart_write( volume, file_descriptor );

	ewf_volume_smart_free( volume );

	if( volume_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_volume_smart_write: unable to write volume to file.\n" );
	}
	return( section_write_count + volume_write_count );
}

/* Write a table or table2 section to file
 */
int64_t libewf_section_table_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset, EWF_TABLE_OFFSET *offsets, uint32_t offsets_amount, char *section_header, uint64_t additional_size )
{
	EWF_TABLE *table                  = NULL;
	int64_t section_write_count       = 0;
	int64_t table_write_count         = 0;
	int64_t table_offsets_write_count = 0;
	size_t size                       = EWF_TABLE_SIZE + ( EWF_TABLE_OFFSET_SIZE * offsets_amount ) + EWF_CRC_SIZE + additional_size;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_table_write: incorrect handle.\n" );
	}
	table = ewf_table_alloc();

	if( table == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_table_write: unable to create table.\n" );
	}
	revert_32bit( offsets_amount, table->chunk_count );

	section_write_count = libewf_section_write( handle, file_descriptor, section_header, size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_table_write: unable to write section to file.\n" );
	}
	table_write_count         = ewf_table_write( table, file_descriptor );
	table_offsets_write_count = ewf_table_offsets_write( offsets, file_descriptor, offsets_amount );

	ewf_table_free( table );

	if( table_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_table_write: unable to write table to file.\n" );
	}
	if( table_offsets_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_table_write: unable to write table offsets to file.\n" );
	}
	return( section_write_count + table_write_count + table_offsets_write_count );
}

/* Write a data section to file
 */
int64_t libewf_section_data_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset )
{
	EWF_DATA *data              = NULL;
	int64_t section_write_count = 0;
	int64_t data_write_count    = 0;
	size_t size                 = EWF_DATA_SIZE;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_data_write: incorrect handle.\n" );
	}
	data = ewf_data_alloc();

	if( data == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_data_write: unable to create data.\n" );
	}
	revert_32bit( handle->chunk_count, data->chunk_count );
	revert_32bit( handle->sectors_per_chunk, data->sectors_per_chunk );
	revert_32bit( handle->bytes_per_sector, data->bytes_per_sector );
	revert_32bit( handle->sector_count, data->sector_count );

	if( handle->format == LIBEWF_FORMAT_ENCASE5 )
	{
		data->compression_level = handle->compression_level;
	}
	section_write_count = libewf_section_write( handle, file_descriptor, "data", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_data_write: unable to write section to file.\n" );
	}
	data_write_count = ewf_data_write( data, file_descriptor );

	ewf_data_free( data );

	if( data_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_data_write: unable to write data to file.\n" );
	}
	return( section_write_count + data_write_count );
}

/* Write a error2 section to file
 */
int64_t libewf_section_error2_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset, EWF_ERROR2_SECTOR *sectors, uint32_t sectors_amount )
{
	EWF_ERROR2 *error2                 = NULL;
	int64_t section_write_count        = 0;
	int64_t error2_write_count         = 0;
	int64_t error2_sectors_write_count = 0;
	size_t size                        = EWF_ERROR2_SIZE + ( EWF_ERROR2_SECTOR_SIZE * sectors_amount ) + EWF_CRC_SIZE;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_write: incorrect handle.\n" );
	}
	error2 = ewf_error2_alloc();

	if( error2 == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_write: unable to create error2.\n" );
	}
	revert_32bit( sectors_amount, error2->error_count );

	section_write_count = libewf_section_write( handle, file_descriptor, "error2", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_write: unable to write section to file.\n" );
	}
	error2_write_count         = ewf_error2_write( error2, file_descriptor );
	error2_sectors_write_count = ewf_error2_sectors_write( sectors, file_descriptor, sectors_amount );

	ewf_error2_free( error2 );

	if( error2_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_write: unable to write error2 to file.\n" );
	}
	if( error2_sectors_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_error2_write: unable to write error2 sectors to file.\n" );
	}
	return( section_write_count + error2_write_count + error2_sectors_write_count );
}

/* Write a hash section to file
 */
int64_t libewf_section_hash_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset, EWF_MD5HASH *md5hash )
{
	EWF_HASH *hash              = NULL;
	int64_t section_write_count = 0;
	int64_t hash_write_count    = 0;
	size_t size                 = EWF_HASH_SIZE;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_hash_write: incorrect handle.\n" );
	}
	hash = ewf_hash_alloc();

	if( hash == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_hash_write: unable to create hash.\n" );
	}
	memcpy( (uint8_t *) hash->md5hash, (uint8_t *) md5hash, EWF_MD5HASH_SIZE );

	section_write_count = libewf_section_write( handle, file_descriptor, "hash", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_hash_write: unable to write section to file.\n" );
	}
	hash_write_count = ewf_hash_write( hash, file_descriptor );

	ewf_hash_free( hash );

	if( hash_write_count == -1 )
	{
		LIBEWF_FATAL_PRINT( "libewf_section_hash_write: unable to write hash to file.\n" );
	}
	return( section_write_count + hash_write_count );
}

/* Create the headers
 */
void libewf_headers_create( LIBEWF_HANDLE *handle, LIBEWF_HEADER_VALUES *header_values )
{
	time_t timestamp = time( NULL );

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_headers_create: incorrect handle.\n" );
	}
	if( ( handle->format == LIBEWF_FORMAT_ENCASE1 ) || ( handle->format == LIBEWF_FORMAT_ENCASE2 ) || ( handle->format == LIBEWF_FORMAT_ENCASE3 ) )
	{
		handle->header = libewf_header_values_generate_header_string_encase3( header_values, timestamp, handle->compression_level );

		if( handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_headers_create: unable to create header values.\n" );
		}
	}
	else if( ( handle->format == LIBEWF_FORMAT_FTK ) || ( handle->format == LIBEWF_FORMAT_SMART ) )
	{
		handle->header = libewf_header_values_generate_header_string_ftk( header_values, timestamp, handle->compression_level );

		if( handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_headers_create: unable to create header values.\n" );
		}
	}
	else if( handle->format == LIBEWF_FORMAT_ENCASE4 )
	{
		handle->header = libewf_header_values_generate_header_string_encase4( header_values, timestamp );

		if( handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_headers_create: unable to create header values.\n" );
		}
		handle->header2 = libewf_header_values_generate_header2_string_encase4( header_values, timestamp );

		if( handle->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_headers_create: unable to create header2 values.\n" );
		}
	}
	else if( handle->format == LIBEWF_FORMAT_ENCASE5 )
	{
		handle->header  = libewf_header_values_generate_header_string_encase4( header_values, timestamp );

		if( handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_headers_create: unable to create header values.\n" );
		}
		handle->header2 = libewf_header_values_generate_header2_string_encase5( header_values, timestamp );

		if( handle->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_headers_create: unable to create header2 values.\n" );
		}
	}
}

/* Write the headers to file
 */
int64_t libewf_headers_write( LIBEWF_HANDLE *handle, int file_descriptor, uint64_t start_offset )
{
	uint32_t header_size         = 0;
	uint32_t header2_size        = 0;
	uint64_t segment_file_offset = start_offset;
	int64_t write_count          = 0;
	int64_t total_count          = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_headers_write: incorrect handle.\n" );
	}
	if( handle->header == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_headers_write: incorrect header.\n" );
	}
	header_size = strlen( (char *) handle->header );

	if( handle->format == LIBEWF_FORMAT_SMART )
	{
		/* The header should be written only once
		 * and using the compression used in the file
		 */
		write_count          = libewf_section_header_write( handle, file_descriptor, segment_file_offset, handle->header, header_size, handle->compression_level );
		segment_file_offset += write_count;
		total_count         += write_count;
	}
	if( ( handle->format == LIBEWF_FORMAT_ENCASE1 ) || ( handle->format == LIBEWF_FORMAT_ENCASE2 ) || ( handle->format == LIBEWF_FORMAT_ENCASE3 ) || ( handle->format == LIBEWF_FORMAT_FTK ) )
	{
		/* The header should be written twice
		 * the default compression is used
		 */
		write_count          = libewf_section_header_write( handle, file_descriptor, segment_file_offset, handle->header, header_size, EWF_COMPRESSION_DEFAULT );
		segment_file_offset += write_count;
		total_count         += write_count;

		write_count          = libewf_section_header_write( handle, file_descriptor, segment_file_offset, handle->header, header_size, EWF_COMPRESSION_DEFAULT );
		segment_file_offset += write_count;
		total_count         += write_count;
	}
	else if( ( handle->format == LIBEWF_FORMAT_ENCASE4 ) || ( handle->format == LIBEWF_FORMAT_ENCASE5 ) )
	{
		if( handle->header2 == NULL )
		{
			LIBEWF_FATAL_PRINT( "libewf_headers_write: incorrect header2.\n" );
		}
		header2_size = strlen( (char *) handle->header2 );

		/* The header2 should be written twice
		 * the default compression is used
		 */
		write_count          = libewf_section_header2_write( handle, file_descriptor, segment_file_offset, handle->header2, header2_size, EWF_COMPRESSION_DEFAULT );
		segment_file_offset += write_count;
		total_count         += write_count;

		write_count          = libewf_section_header2_write( handle, file_descriptor, segment_file_offset, handle->header2, header2_size, EWF_COMPRESSION_DEFAULT );
		segment_file_offset += write_count;
		total_count         += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count          = libewf_section_header_write( handle, file_descriptor, segment_file_offset, handle->header, header_size, EWF_COMPRESSION_DEFAULT );
		segment_file_offset += write_count;
		total_count         += write_count;
	}
	return( total_count );
}

/* Sets file writing parametes
 */
LIBEWF_HANDLE *libewf_set_write_parameters( LIBEWF_HANDLE *handle, uint64_t input_file_size, uint32_t sectors_per_chunk, uint32_t bytes_per_sector, uint32_t error_granularity_sectors, uint64_t ewf_file_size, int8_t compression_level, uint8_t format, LIBEWF_HEADER_VALUES *header_values, uint8_t read_error_retry, uint8_t wipe_block_on_read_error, uint8_t compress_empty_block )
{
	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_set_write_parameters: incorrect handle.\n" );
	}
	if( input_file_size <= 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_set_write_parameters: invalid value for parameter: input_file_size.\n" );
	}
	if( sectors_per_chunk <= 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_set_write_parameters: invalid value for parameter: sectors_per_chunk.\n" );
	}
	if( bytes_per_sector <= 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_set_write_parameters: invalid value for parameter: bytes_per_sector.\n" );
	}
	if( error_granularity_sectors <= 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_set_write_parameters: invalid value for parameter: error_granularity_sectors.\n" );
	}
	if( ewf_file_size <= 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_set_write_parameters: invalid value for parameter: ewf_file_size.\n" );
	}
	if( header_values == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_set_write_parameters: invalid value for parameter: header_values.\n" );
	}
	handle->input_file_size           = input_file_size;
	handle->sectors_per_chunk         = sectors_per_chunk;
	handle->bytes_per_sector          = bytes_per_sector;
	handle->error_granularity_sectors = error_granularity_sectors;
	handle->ewf_file_size             = ewf_file_size;
	handle->compression_level         = compression_level;
	handle->compress_empty_block      = compress_empty_block;
	handle->wipe_block_on_read_error  = wipe_block_on_read_error;
	handle->format                    = format;
	handle->read_error_retry          = read_error_retry;
	handle->chunk_size                = sectors_per_chunk * bytes_per_sector;
	handle->chunks_per_file           = ( ewf_file_size - EWF_FILE_HEADER_SIZE - EWF_DATA_SIZE ) / handle->chunk_size;

	handle->chunk_count = handle->input_file_size / handle->chunk_size;

	if( handle->input_file_size % handle->chunk_size != 0 )
	{
		handle->chunk_count += 1;
	}
	handle->sector_count = handle->input_file_size / handle->bytes_per_sector;

	libewf_headers_create( handle, header_values );

	LIBEWF_VERBOSE_PRINT( "libewf_set_write_parameters: input file size: %" PRIu32 ".\n", handle->input_file_size );
	LIBEWF_VERBOSE_PRINT( "libewf_set_write_parameters: requested ewf segment file size: %" PRIu64 ".\n", handle->ewf_file_size );

	return( handle );
}

/* Reads chunk data from a file descriptor
 */
int64_t libewf_read_chunk_data( int input_file_descriptor, uint8_t *buffer, uint32_t bytes_to_read )
{
	int32_t read_count = 0;

	if( buffer == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_chunk: incorrect buffer.\n" );
	}
	read_count = libewf_read( input_file_descriptor, buffer, bytes_to_read );

	if( read_count <= -1 )
	{
		if( errno == ESPIPE )
		{
			LIBEWF_FATAL_PRINT( "libewf_read_chunk: error reading data: Invalid seek.\n" );
		}
		else if( errno == EPERM )
		{
			LIBEWF_FATAL_PRINT( "libewf_read_chunk: error reading data: Operation not permitted.\n" );
		}
		else if( errno == ENXIO )
		{
			LIBEWF_FATAL_PRINT( "libewf_read_chunk: error reading data: No such device or address.\n" );
		}
		else if( errno == ENODEV )
		{
			LIBEWF_FATAL_PRINT( "libewf_read_chunk: error reading data: No such device.\n" );
		}
	}
	return( read_count );
}

/* Writes data in EWF format from a file descriptor
 */
int64_t libewf_write_from_file_descriptor( LIBEWF_HANDLE *handle, int input_file_descriptor, void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) )
{
	LIBEWF_MD5_CTX md5;

	char *filename                          = NULL;
	char *extension                         = NULL;
	char *calculated_md5hash_string         = NULL;
	char *table_section_string              = NULL;
	EWF_FILE_HEADER *file_header            = NULL;
	EWF_TABLE_OFFSET *offsets               = NULL;
	EWF_MD5HASH *calculated_md5hash         = NULL;
	EWF_CRC *crc                            = NULL;

	int64_t total_write_count               = 0;
	int64_t total_read_count                = 0;
	int64_t read_count                      = 0;
	int64_t write_count                     = 0;
	int64_t sectors_read_count              = 0;
	int64_t sectors_write_count             = 0;
	int64_t read_error_count                = 0;
	uint64_t segment_file_offset            = 0;
	uint64_t read_error_offset              = 0;
	uint64_t write_size                     = 0;
	uint64_t data_chunks_offset             = 0;
	uint64_t sectors_size                   = 0;
	uint64_t additional_size                = 0;
	int32_t total_read_error_count          = 0;
	uint32_t compressed_data_size           = 0;
	uint32_t maximum_compressed_data_size   = 0;
	uint32_t total_chunk_write_count        = 0;
	uint32_t segment                        = 1;
	uint32_t chunk_amount                   = 0;
	uint32_t bytes_to_read                  = 0;
	uint32_t error_granularity_bytes        = 0;
	uint32_t error_granularity_offset       = 0;
	uint32_t error_remaining_bytes          = 0;
	uint32_t read_remaining_bytes           = 0;
	uint32_t error_skip_bytes               = 0;
	uint32_t sectors_chunk_amount           = 0;
	uint32_t remaining_sectors_chunk_amount = 0;
	uint32_t remaining_chunk_amount         = 0;
	uint32_t error2_sector                  = 0;
	uint32_t error2_sector_count            = 0;
	uint16_t filename_size                  = 0;
	uint8_t maximum_chunk_write_iterations  = 1;
	uint8_t chunk_write_iterator            = 0;
	int result                              = 0;

	if( handle == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: incorrect handle.\n" );
	}
	extension = strdup( "E01" );

	if( handle->format == LIBEWF_FORMAT_SMART )
	{
		handle->ewf_format = EWF_FORMAT_S01;
		extension[ 0 ]     = 's';
	}
	error_granularity_bytes = handle->error_granularity_sectors * handle->bytes_per_sector;

	if( handle->chunk_size <= 0 )
	{
		LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: invalid chunk size.\n" );
	}
	filename_size = strlen( handle->segment_table->filename[ 0 ] );
	filename      = (char *) malloc( sizeof( char ) * ( filename_size + 5 ) );

	if( filename == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to allocate filename.\n" );
	}
	file_header = ewf_file_header_alloc();

	if( file_header == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to create file header.\n" );
	}
	crc = ewf_crc_alloc();

	if( crc == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to create CRC.\n" );
	}
	/* Make sure the compressed data size buffer is large enough
	 * zlib compression can enlarge the data
	 * about 1024 bytes should be enough
	 */
	maximum_compressed_data_size = handle->chunk_size + 1024;

	handle = libewf_handle_cache_realloc( handle, maximum_compressed_data_size );

	LIBEWF_MD5_INIT( &md5 );

	while( total_read_count < handle->input_file_size )
	{
		/* Create the filename for the specific segment file
		 */
		snprintf( filename, ( filename_size + 5 ), "%s.%s", handle->segment_table->filename[ 0 ], extension );

		/* Make sure the string is terminated
		 */
		filename[ filename_size + 4 ] = '\0';

		if( handle->segment_table->amount <= segment )
		{
			/* One additional entry in the segment table is needed,
			 * because the 0 entry is used to store the base filename
			 */
			handle->segment_table = libewf_segment_table_values_realloc( handle->segment_table, ( segment + 1 ) );
		}
		handle->segment_table = libewf_segment_table_set_values( handle->segment_table, segment, filename, -1 );

		LIBEWF_VERBOSE_PRINT( ".\nlibewf_write_from_file_descriptor: segment file to write: %" PRIu32 " with name: %s.\n", segment, filename );

		if( segment != 1 )
		{
			/* Write next section
			 */
			write_count        = libewf_last_section_write( handle, handle->segment_table->file_descriptor[ segment - 1 ], "next", segment_file_offset );
			total_write_count += write_count;

			close( handle->segment_table->file_descriptor[ segment - 1 ] );
		}
		handle->segment_table->file_descriptor[ segment ] = open( handle->segment_table->filename[ segment ], O_WRONLY | O_CREAT | O_TRUNC, 0644 );

		revert_16bit( segment, file_header->fields_segment );

		segment_file_offset = 0;
		write_count         = ewf_file_header_write( file_header, handle->segment_table->file_descriptor[ segment ] );

		if( write_count == -1 )
		{
			LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to write file header to file.\n" );
		}
		segment_file_offset += write_count;
		total_write_count   += write_count;

		if( segment == 1 )
		{
			/* Write header section(s)
			 */
			write_count          = libewf_headers_write( handle, handle->segment_table->file_descriptor[ segment ], segment_file_offset );
			segment_file_offset += write_count;
			total_write_count   += write_count;

			if( handle->ewf_format == EWF_FORMAT_E01 )
			{
				/* Write volume section
				 */
				write_count          = libewf_section_volume_write( handle, handle->segment_table->file_descriptor[ segment ], segment_file_offset );
				segment_file_offset += write_count;
				total_write_count   += write_count;
			}
			else if( handle->ewf_format == EWF_FORMAT_S01 )
			{
				/* Write volume (SMART) section
				 */
				write_count          = libewf_section_volume_smart_write( handle, handle->segment_table->file_descriptor[ segment ], segment_file_offset );
				segment_file_offset += write_count;
				total_write_count   += write_count;
			}
		}
		else if( handle->ewf_format == EWF_FORMAT_E01 )
		{
			/* Write data section
			 */
			write_count          = libewf_section_data_write( handle, handle->segment_table->file_descriptor[ segment ], segment_file_offset );
			segment_file_offset += write_count;
			total_write_count   += write_count;
		}
		sectors_size  = handle->ewf_file_size;
		sectors_size -= segment_file_offset;

		/* Leave space for at least the sectors, table and table 2 and next or done sections
		 */
		sectors_size -= ( 4 * EWF_SECTION_SIZE ) + ( 2 * handle->chunks_per_file * sizeof( EWF_TABLE_OFFSET ) );

		/* Determine how many chunks will fit in the remaining space
		 * This will be an estimate
		 */
		sectors_chunk_amount   = sectors_size / ( handle->chunk_size + EWF_CRC_SIZE );
		remaining_chunk_amount = handle->chunk_count - total_chunk_write_count;

		LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: calculated amount of chunks: %d.\n", sectors_chunk_amount );

		if( remaining_chunk_amount < handle->chunks_per_file )
		{
			sectors_chunk_amount = remaining_chunk_amount;
		}
		/* Check if the maximum allowed amounts of chunks per segment file is not exceeded
		 * Only required for EWF-S01 format
		 */
		maximum_chunk_write_iterations = 1;

		if( ( handle->ewf_format == EWF_FORMAT_S01 ) && ( sectors_chunk_amount > 16375 ) )
		{
			remaining_sectors_chunk_amount = sectors_chunk_amount - 16375;
			sectors_chunk_amount           = 16375;
			maximum_chunk_write_iterations = 2;

			if( remaining_sectors_chunk_amount > 16375 )
			{
				remaining_sectors_chunk_amount = 16375;
			}
		}
		/* This loop is needed to write the table2 section for the EWF-S01 format
		 */
		for( chunk_write_iterator = 1; chunk_write_iterator <= maximum_chunk_write_iterations; chunk_write_iterator++ )
		{
			if( chunk_write_iterator == 2 )
			{
				sectors_chunk_amount = remaining_sectors_chunk_amount;
			}
			sectors_size = sectors_chunk_amount * ( handle->chunk_size + EWF_CRC_SIZE );

			LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: calculated sectors size: %" PRIu64 ".\n", sectors_size );

			offsets = ewf_table_offsets_alloc( sectors_chunk_amount );

			if( offsets == NULL )
			{
				LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to create table offsets.\n" );
			}
			data_chunks_offset = segment_file_offset;

			if( handle->ewf_format == EWF_FORMAT_E01 )
			{
				if( chunk_write_iterator > 1 )
				{
					LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: no more than 1 sectors section supported per segment file.\n" );
				}
				/* Write sectors section start
				 */
				write_count = libewf_section_write( handle, handle->segment_table->file_descriptor[ segment ], "sectors", sectors_size, segment_file_offset );

				if( write_count == -1 )
				{
					LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to write section to file.\n" );
				}
				segment_file_offset += write_count;
				total_write_count   += write_count;
			}
			else if( handle->ewf_format == EWF_FORMAT_S01 )
			{
				/* Write table section start
				 */
				if( chunk_write_iterator == 1 )
				{
					table_section_string = "table";
				}
				else if( chunk_write_iterator == 2 )
				{
					table_section_string = "table2";
				}
				else
				{
					LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: no more than 2 table sections supported per segment file.\n" );
				}
				write_count          = libewf_section_table_write( handle, handle->segment_table->file_descriptor[ segment ], segment_file_offset, offsets, sectors_chunk_amount, table_section_string, 0 );
				segment_file_offset += write_count;
				total_write_count   += write_count;
			}
			sectors_read_count  = 0;
			sectors_write_count = 0;
			chunk_amount        = 0;

			/* Calculate additional space for the sectors, table and table 2 and next or done sections
			 */
			additional_size = ( 4 * EWF_SECTION_SIZE ) + ( 2 * handle->chunks_per_file * sizeof( EWF_TABLE_OFFSET ) );

			/* Check if another chunk would fit
			 */
			while( 1 )
			{
				/* Fill the segment file for EWF-E01
				 */
				if( handle->ewf_format == EWF_FORMAT_E01 )
				{
					if( ( segment_file_offset + handle->chunk_size + EWF_CRC_SIZE + additional_size ) >= handle->ewf_file_size )
					{
						break;
					}
				}
				/* Use the calculated amount of sectors for EWF-S01
				 */
				else if( handle->ewf_format == EWF_FORMAT_S01 )
				{
					if( chunk_amount >= sectors_chunk_amount )
					{
						break;
					}
				}
				/* Unsupported format
				 */
				else
				{
					LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: specified EWF format is not supported.\n" );
				}
				/* At the end of the input
				 */
				if( total_read_count == handle->input_file_size )
				{
					break;
				}

				/* Make sure there is no data contamination whatsoever
				 */
				handle = libewf_handle_cache_wipe( handle );

				LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: reading chunk: %d with size: %" PRIu32 ".\n", chunk_amount, handle->chunk_size );

				bytes_to_read     = handle->chunk_size;
				read_error_offset = 0;
				read_error_count  = 0;

				while( 1 )
				{
					read_count = libewf_read_chunk_data( input_file_descriptor, &handle->raw_data[ read_error_offset ], bytes_to_read );

					LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: read chunk: %d with size: %" PRIi64 ".\n", chunk_amount, read_count );

					/* The last read is OK, correct read_count
					 */
					if( read_count == bytes_to_read )
					{
						read_count = read_error_offset + bytes_to_read;
					}
					/* The entire read is OK
					 */
					if( read_count == handle->chunk_size )
					{
						break;
					}
					/* At the end of the input
					 */
					if( ( total_read_count + read_count ) == handle->input_file_size )
					{
						break;
					}
					/* No bytes were read
					 */
					if( read_count == 0 )
					{
						LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: error reading data: unexpected end of data bytes read: %" PRIu64 " total bytes to read: %" PRIu64 ".\n", total_read_count, handle->input_file_size );
					}
					/* There was a read error at a certain offset
					 */
					if( read_count > 0 )
					{
						read_error_offset += read_count;
					}
					LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: read error: %d at offset %" PRIu64 ".\n", errno, ( total_read_count + read_error_offset ) );

					if( read_error_count >= handle->read_error_retry )
					{
						if( handle->error2_sectors == NULL )
						{
							 handle->error2_sectors = ewf_error2_sectors_alloc( ( total_read_error_count + 1 ) );
						}
						else
						{
							 handle->error2_sectors = ewf_error2_sectors_realloc( handle->error2_sectors, total_read_error_count, ( total_read_error_count + 1 ) );
						}
						if( handle->error2_sectors == NULL )
						{
							LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: unable to create error2 sectors.\n" );
						}
						total_read_error_count++;

						/* Check if last chunk is smaller than the chunk size and take corrective measures
						 */
						if( ( total_read_count + handle->chunk_size ) > handle->input_file_size )
						{
							read_remaining_bytes = handle->input_file_size - total_read_count;
						}
						else
						{
							read_remaining_bytes = handle->chunk_size;
						}
						error_remaining_bytes    = read_remaining_bytes - read_error_offset;
						error2_sector            = total_read_count;
						error_granularity_offset = ( read_error_offset / error_granularity_bytes ) * error_granularity_bytes;
						error_skip_bytes         = ( error_granularity_offset + error_granularity_bytes ) - read_error_offset;

						if( handle->wipe_block_on_read_error == 1 )
						{
							LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: wiping block of %" PRIu32 " bytes at offset %" PRIu32 ".\n", error_granularity_bytes, error_granularity_offset );

							memset( &handle->raw_data[ error_granularity_offset ], 0, error_granularity_bytes );

							error2_sector      += error_granularity_offset;
							error2_sector_count = error_granularity_bytes;
						}
						else
						{
							error2_sector      += read_error_offset;
							error2_sector_count = error_skip_bytes;
						}
						error2_sector       /= handle->bytes_per_sector;
						error2_sector_count /= handle->bytes_per_sector;

						LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: adding error2: %" PRIi32 " sector: %" PRIu32 ", count: %" PRIu32 ".\n", total_read_error_count, error2_sector, error2_sector_count );
						revert_32bit( error2_sector, handle->error2_sectors[ total_read_error_count - 1 ].sector );
						revert_32bit( error2_sector_count, handle->error2_sectors[ total_read_error_count - 1 ].sector_count );

						LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: skipping %" PRIu32 " bytes.\n", error_skip_bytes );
						lseek( input_file_descriptor, error_skip_bytes, SEEK_CUR );

						if( error_remaining_bytes > error_granularity_bytes )
						{
							bytes_to_read      = error_remaining_bytes - error_skip_bytes;
							read_error_offset += error_skip_bytes;

							LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: remaining to read from chunk %" PRIu32 " bytes.\n", bytes_to_read );
						}
						else
						{
							read_count = read_remaining_bytes;

							LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: no remaining bytes to read from chunk.\n" );
							break;
						}
					}
					read_error_count++;
				}
				total_read_count += read_count;

				if( handle->swap_byte_pairs == 1 )
				{
					swap_byte_pairs( handle->raw_data, read_count );
				}
				/* Callback for status update
				 */
				if( callback != NULL )
				{
					callback( total_read_count, handle->input_file_size );
				}
				LIBEWF_MD5_UPDATE( &md5, handle->raw_data, read_count );

				sectors_read_count  += read_count;
				write_size           = 0;
				compressed_data_size = maximum_compressed_data_size;

				if( ( handle->ewf_format == EWF_FORMAT_S01 ) || ( handle->compression_level != EWF_COMPRESSION_NONE ) )
				{
					result = ewf_sectors_chunk_compress( handle->chunk_data, &compressed_data_size, handle->raw_data, read_count, handle->compression_level );

					if( result != 1 )
					{
						LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to compress chunk: %d.\n", chunk_amount );
					}
				}
				else if( ( handle->compress_empty_block == 1 ) && ( test_empty_block( handle->raw_data, read_count ) == 1 ) )
				{
					result = ewf_sectors_chunk_compress( handle->chunk_data, &compressed_data_size, handle->raw_data, read_count, EWF_COMPRESSION_DEFAULT );

					if( result != 1 )
					{
						LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to compress empty chunk: %d.\n", chunk_amount );
					}
				}
				if( ( handle->ewf_format == EWF_FORMAT_S01 ) || ( compressed_data_size < handle->chunk_size ) )
				{
					/* No additional CRC required, zlib creates its own CRC
					 */
					write_size  = compressed_data_size;
					write_count = ewf_sectors_chunk_write( handle->chunk_data, handle->segment_table->file_descriptor[ segment ], write_size );

					memcpy( (uint8_t *) &crc, (uint8_t *) &handle->chunk_data[ compressed_data_size - EWF_CRC_SIZE ], EWF_CRC_SIZE );

					LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: writing COMPRESSED chunk: %d at offset: %" PRIu64 " with size: %" PRIu64 ", with CRC: %" PRIu32 ".\n", chunk_amount, segment_file_offset, write_size, crc );

					if( sectors_chunk_amount <= chunk_amount )
					{
						LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: enlarging offsets size: %" PRIu32 ", required: %" PRIu32 ".\n", sectors_chunk_amount, chunk_amount );

						offsets = ewf_table_offsets_realloc( offsets, sectors_chunk_amount, ( chunk_amount + 1 ) );

						sectors_chunk_amount = chunk_amount + 1;
					}
					revert_32bit( ( segment_file_offset | EWF_OFFSET_COMPRESSED_WRITE_MASK ), offsets[ chunk_amount ].offset );
				}
				else
				{
					write_size = read_count + EWF_CRC_SIZE;

					crc = ewf_crc_calculate( (void *) handle->raw_data, read_count, 1 );

					if( crc == NULL )
					{
						LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to calculate CRC.\n" );
					}
					revert_32bit( *crc, &handle->raw_data[ read_count ] );

					LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: writing UNCOMPRESSED chunk: %d at offset: %" PRIu64 " with size: %" PRIu64 ", with CRC: %" PRIu32 ".\n", chunk_amount, segment_file_offset, write_size, *crc );

					write_count = ewf_sectors_chunk_write( handle->raw_data, handle->segment_table->file_descriptor[ segment ], write_size );

					if( sectors_chunk_amount <= chunk_amount )
					{
						LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: enlarging offsets size: %" PRIu32 ", required: %" PRIu32 ".\n", sectors_chunk_amount, chunk_amount );

						offsets = ewf_table_offsets_realloc( offsets, sectors_chunk_amount, ( chunk_amount + 1 ) );

						sectors_chunk_amount = chunk_amount + 1;
					}
					revert_32bit( segment_file_offset, offsets[ chunk_amount ].offset );
				}

				if( write_count != write_size )
				{
					LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to write data.\n" );
				}
				segment_file_offset += write_count;
				total_write_count   += write_count;
				sectors_write_count += write_count;

				total_chunk_write_count++;
				chunk_amount++;
			}
			/* Seek the start of the data chunks
			*/
			LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: written sectors size: %" PRIu64 ".\n", sectors_write_count );

			if( lseek( handle->segment_table->file_descriptor[ segment ], (off_t) data_chunks_offset, SEEK_SET ) == -1 )
			{
				LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to find offset to correct sectors size.\n" );
			}
			if( handle->ewf_format == EWF_FORMAT_E01 )
			{
				/* Rewrite sectors section start
				 */
				write_count = libewf_section_write( handle, handle->segment_table->file_descriptor[ segment ], "sectors", sectors_write_count, data_chunks_offset );

				if( write_count == -1 )
				{
					LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to rewrite sectors section to file.\n" );
				}
				LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: correcting sectors section size: %" PRIu64 " offset: %" PRIu64 ".\n", sectors_write_count, data_chunks_offset );
			}
			else if( handle->ewf_format == EWF_FORMAT_S01 )
			{
				/* Rewrite table section start
				 */
				libewf_section_table_write( handle, handle->segment_table->file_descriptor[ segment ], data_chunks_offset, offsets, sectors_chunk_amount, table_section_string, sectors_write_count );
				LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: correcting table section size: %" PRIu64 " offset: %" PRIu64 ".\n", sectors_write_count, data_chunks_offset );
			}
			LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: back to end of data at offset: %" PRIu64 ".\n", segment_file_offset );

			if( lseek( handle->segment_table->file_descriptor[ segment ], (off_t) segment_file_offset, SEEK_SET ) == -1 )
			{
				LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to find offset to continue.\n" );
			}
			if( handle->ewf_format == EWF_FORMAT_E01 )
			{
				/* Write table section start
				 */
				write_count          = libewf_section_table_write( handle, handle->segment_table->file_descriptor[ segment ], segment_file_offset, offsets, sectors_chunk_amount, "table", 0 );
				segment_file_offset += write_count;
				total_write_count   += write_count;

				/* Write table2 section start
				 */
				write_count          = libewf_section_table_write( handle, handle->segment_table->file_descriptor[ segment ], segment_file_offset, offsets, sectors_chunk_amount, "table2", 0 );
				segment_file_offset += write_count;
				total_write_count   += write_count;
			}
			ewf_table_offsets_free( offsets );
		}
		segment++;

		if( segment <= 99 )
		{
			snprintf( extension, 4, "%c%.2" PRIu32, extension[ 0 ], segment );
		}
		else if( segment == 100 )
		{
			if( handle->ewf_format == EWF_FORMAT_E01 )
			{
				snprintf( extension, 4, "%cAA" PRIu32, extension[ 0 ] );
			}
			else if( handle->ewf_format == EWF_FORMAT_S01 )
			{
				snprintf( extension, 4, "%caa" PRIu32, extension[ 0 ] );
			}
		}
		else
		{
			extension[ 2 ]++;

			if( extension[ 2 ] == '{' )
			{
				extension[ 2 ] = 'a';
				extension[ 1 ]++;
			}
			else if( extension[ 2 ] == '[' )
			{
				extension[ 2 ] = 'A';
				extension[ 1 ]++;
			}
			if( extension[ 1 ] == '{' )
			{
				extension[ 1 ] = 'a';
				extension[ 0 ]++;
			}
			else if( extension[ 1 ] == '[' )
			{
				extension[ 1 ] = 'A';
				extension[ 0 ]++;
			}
			if( ( extension[ 0 ] == '{' ) || ( extension[ 0 ] == '[' ) )
			{
				LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to support for more segment files.\n" );
			}
		}
	}
	free( filename );
	free( extension );
	ewf_file_header_free( file_header );
	ewf_crc_free( crc );

	/* Write the data section for a single segment file
	 * only for EWF-E01 and the segment count must be 2 (one segment file)
	 */
	if( ( handle->ewf_format == EWF_FORMAT_E01 ) && ( segment == 2 ) )
	{
		write_count          = libewf_section_data_write( handle, handle->segment_table->file_descriptor[ segment - 1 ], segment_file_offset );
		segment_file_offset += write_count;
		total_write_count   += write_count;
	}
	/* Write the error2 section if required 
	 */
	if( ( total_read_error_count > 0 ) && ( ( handle->format == LIBEWF_FORMAT_ENCASE3 ) || ( handle->format == LIBEWF_FORMAT_ENCASE4 ) || ( handle->format == LIBEWF_FORMAT_ENCASE5 ) ) )
	{
		write_count          = libewf_section_error2_write( handle, handle->segment_table->file_descriptor[ segment - 1 ], segment_file_offset, handle->error2_sectors, total_read_error_count );
		segment_file_offset += write_count;
		total_write_count   += write_count;
	}
	calculated_md5hash = ewf_md5hash_alloc();

	if( calculated_md5hash == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_write_from_file_descriptor: unable to create MD5 hash.\n" );
	}
  	LIBEWF_MD5_FINAL( calculated_md5hash, &md5 );

	/* Write the hash section
	 */
	write_count          = libewf_section_hash_write( handle, handle->segment_table->file_descriptor[ segment - 1 ], segment_file_offset, calculated_md5hash );
	segment_file_offset += write_count;
	total_write_count   += write_count;

	calculated_md5hash_string = ewf_md5hash_to_string( calculated_md5hash );

	if( calculated_md5hash_string == NULL )
	{
		LIBEWF_FATAL_PRINT( "libewf_read_to_file_descriptor: unable to create MD5 hash string.\n" );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_write_from_file_descriptor: MD5 calculated: %s.\n", calculated_md5hash_string );

	ewf_md5hash_free( calculated_md5hash );

	free( calculated_md5hash_string );

	/* Write the done section
	 */
	write_count          = libewf_last_section_write( handle, handle->segment_table->file_descriptor[ segment - 1 ], "done", segment_file_offset );
	segment_file_offset += write_count;
	total_write_count   += write_count;

	close( handle->segment_table->file_descriptor[ segment - 1 ] );

	return( total_write_count );
}

