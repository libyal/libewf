/*
 * libewf file reading
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

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_debug.h"
#include "libewf_endian.h"
#include "libewf_header_values.h"
#include "libewf_notify.h"
#include "libewf_segment_table.h"

#include "ewf_chunk.h"
#include "ewf_compress.h"
#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_error2.h"
#include "ewf_file_header.h"
#include "ewf_hash.h"
#include "ewf_header.h"
#include "ewf_header2.h"
#include "ewf_ltree.h"
#include "ewf_section.h"
#include "ewf_volume.h"
#include "ewf_volume_smart.h"
#include "ewf_table.h"

/* Reads and processes a section start
 * Returns the section start, or NULL on error
 */
EWF_SECTION *libewf_section_start_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor )
{
	EWF_SECTION *section   = NULL;
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_read: invalid handle.\n" );

		return( NULL );
	}
	section = (EWF_SECTION *) libewf_common_alloc( EWF_SECTION_SIZE );

	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_read: unable to allocate section start.\n" );

		return( NULL );
	}
	if( ewf_section_read( section, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_read: unable to read section start.\n" );

		libewf_common_free( section );

		return( NULL );
	}
	LIBEWF_VERBOSE_EXEC( libewf_debug_section_fprint( stderr, section ); );

#ifdef HAVE_DEBUG_OUTPUT
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( section->padding, 40 ); );
#endif

	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) section, ( EWF_SECTION_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_read: unable to calculate CRC.\n" );

		libewf_common_free( section );

		return( NULL );
	}
	if( libewf_endian_convert_32bit( &stored_crc, section->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_read: unable to convert stored CRC value.\n" );

		libewf_common_free( section );

		return( NULL );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( section );

			return( NULL );
		}
	}
	return( section );
}

/* Write a section start to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_start_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, EWF_CHAR *section_type, size_t section_data_size, off_t start_offset )
{
	EWF_SECTION *section     = NULL;
	ssize_t write_count      = 0;
	size_t section_type_size = 0;
	uint64_t section_size    = 0;
	uint64_t section_offset  = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_write: invalid handle.\n" );

		return( -1 );
	}
	section_type_size = ewf_string_length( section_type );

	if( section_type_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_write: section type is empty.\n" );

		return( -1 );
	}
	if( section_type_size >= 16 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_write: section type is too long.\n" );

		return( -1 );
	}
	section = (EWF_SECTION *) libewf_common_alloc( EWF_SECTION_SIZE );

	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_write: unable to create section.\n" );

		return( -1 );
	}
	if( libewf_common_memset( section, 0, EWF_SECTION_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_write: unable to clear section.\n" );

		libewf_common_free( section );

		return( -1 );
	}
	/* Add one character for the end of string
	 */
	if( ewf_string_copy( section->type, section_type, ( section_type_size + 1 ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_write: unable to set section type.\n" );

		libewf_common_free( section );

		return( -1 );
	}
	section_size   = EWF_SECTION_SIZE + section_data_size;
	section_offset = start_offset + section_size;

	if( libewf_endian_revert_64bit( section_size, section->size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_write: unable to revert size value.\n" );

		libewf_common_free( section );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( section_offset, section->next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_write: unable to revert next offset value.\n" );

		libewf_common_free( section );

		return( -1 );
	}
	write_count = ewf_section_write( section, file_descriptor );

	libewf_common_free( section );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_start_write: unable to write section to file.\n" );

		return( -1 );
	}
	return( write_count );
}

/* Write a compressed string section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_compressed_string_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_CHAR *section_type, EWF_CHAR *uncompressed_string, size_t size, int8_t compression_level )
{
	EWF_CHAR *compressed_string = NULL;
	ssize_t section_write_count = 0;
	ssize_t string_write_count  = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_compressed_string_write: invalid handle.\n" );

		return( -1 );
	}
	if( section_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_compressed_string_write: invalid section type.\n" );

		return( -1 );
	}
	if( uncompressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_compressed_string_write: invalid uncompressed string.\n" );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_compressed_string_write: String:\n" );
	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, uncompressed_string, size ); );

	compressed_string = ewf_string_compress( uncompressed_string, &size, compression_level );

	if( compressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_compressed_string_write: unable to compress string.\n" );

		return( -1 );
	}
	section_write_count = libewf_section_start_write( internal_handle, file_descriptor, section_type, size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_compressed_string_write: unable to write section to file.\n" );

		libewf_common_free( compressed_string );

		return( -1 );
	}
	string_write_count = ewf_string_write_from_buffer( compressed_string, file_descriptor, size );

	libewf_common_free( compressed_string );

	if( string_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_compressed_string_write: unable to write string to file.\n" );

		return( -1 );
	}
	return( section_write_count + string_write_count );
}

/* Reads a header section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_header_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_HEADER *header = NULL;
	ssize_t read_count = (ssize_t) size;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_header_read: invalid handle.\n" );

		return( -1 );
	}
	header = ewf_header_read( file_descriptor, &size );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_header_read: unable to read header.\n" );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_header_read: Header:\n" );
	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, header, size ); );

	if( libewf_internal_handle_is_set_header( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_set_header( internal_handle, header, size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_header_read: unable to set header in handle.\n" );

			libewf_common_free( header );

			if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
	}
	else
	{
		libewf_common_free( header );
	}
	internal_handle->amount_of_header_sections++;

	return( read_count );
}

/* Write a header section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_header_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_HEADER *header, size_t size, int8_t compression_level )
{
	ssize_t section_write_count = 0;

	section_write_count = libewf_section_compressed_string_write( internal_handle, file_descriptor, start_offset, (EWF_CHAR *) "header", header, size, compression_level );

	if( section_write_count != -1 )
	{
		internal_handle->amount_of_header_sections++;
	}
	return( section_write_count );
}

/* Reads a header2 section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_header2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_HEADER *header2 = NULL;
	ssize_t read_count  = (ssize_t) size;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_header2_read: invalid handle.\n" );

		return( -1 );
	}
	header2 = ewf_header2_read( file_descriptor, &size );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_header2_read: unable to read header2.\n" );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_header2_read: Header2:\n" );
	LIBEWF_VERBOSE_EXEC( libewf_debug_header2_fprint( stderr, header2, size ); );

	if( libewf_internal_handle_is_set_header2( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_set_header2( internal_handle, header2, size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_header2_read: unable to set header2 in handle.\n" );

			libewf_common_free( header2 );

			if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
	}
	else
	{
		libewf_common_free( header2 );
	}
	internal_handle->amount_of_header_sections++;

	return( read_count );
}

/* Write a header2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_header2_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_HEADER2 *header2, size_t size, int8_t compression_level )
{
	ssize_t section_write_count = 0;

	section_write_count = libewf_section_compressed_string_write( internal_handle, file_descriptor, start_offset, (EWF_CHAR *) "header2", header2, size, compression_level );

	if( section_write_count != -1 )
	{
		internal_handle->amount_of_header_sections++;
	}
	return( section_write_count );
}

/* Reads an EWF-S01 (SMART) volume section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_s01_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_VOLUME_SMART *volume_smart = NULL;
	EWF_CRC calculated_crc         = 0;
	EWF_CRC stored_crc             = 0;
	int32_t bytes_per_chunk        = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( size != EWF_VOLUME_SMART_SIZE )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: mismatch in section volume size.\n" );

		return( -1 );
	}
	volume_smart = (EWF_VOLUME_SMART *) libewf_common_alloc( EWF_VOLUME_SMART_SIZE );

	if( volume_smart == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to allocate volume.\n" );

		return( -1 );
	}
	if( ewf_volume_smart_read( volume_smart, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to read volume.\n" );

		libewf_common_free( volume_smart );

		return( -1 );
	}
#ifdef HAVE_DEBUG_OUTPUT
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown2, 20 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown3, 45 ); );
#endif

	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) volume_smart, ( EWF_VOLUME_SMART_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to calculate CRC.\n" );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, volume_smart->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to convert stored CRC value.\n" );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	bytes_per_chunk = ewf_volume_smart_calculate_chunk_size( volume_smart );

	if( bytes_per_chunk <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to calculate chunk size - using default.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume_smart );

			return( -1 );
		}
		bytes_per_chunk = EWF_MINIMUM_CHUNK_SIZE;
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume_smart );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->amount_of_chunks, volume_smart->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to convert amount of chunks value.\n" );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->sectors_per_chunk, volume_smart->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to convert sectors per chunk value.\n" );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->bytes_per_sector, volume_smart->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to convert bytes per sector value.\n" );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->amount_of_sectors, volume_smart->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_read: unable to convert amount of sectors value.\n" );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	internal_handle->media->chunk_size = (uint32_t) bytes_per_chunk;

	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_s01_read: This volume has %" PRIu32 " chunks of %" PRIi32 " bytes each, CRC %" PRIu32 " (%" PRIu32 ").\n", internal_handle->media->amount_of_chunks, bytes_per_chunk, stored_crc, calculated_crc );
	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_s01_read: This volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n", internal_handle->media->amount_of_sectors, internal_handle->media->bytes_per_sector );

	if( libewf_common_memcmp( (void *) volume_smart->signature, (void *) "SMART", 5 ) == 0 )
	{
		internal_handle->format = LIBEWF_FORMAT_SMART;
	}
	else
	{
		internal_handle->format = LIBEWF_FORMAT_EWF;
	}
	libewf_common_free( volume_smart );

	return( (int32_t) size );
}

/* Write an EWF-S01 (SMART) volume section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_volume_s01_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset )
{
	EWF_VOLUME_SMART *volume    = NULL;
	ssize_t section_write_count = 0;
	ssize_t volume_write_count  = 0;
	size_t size               = EWF_VOLUME_SMART_SIZE;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_write: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_write: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	volume = (EWF_VOLUME_SMART *) libewf_common_alloc( size );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_write: unable to create volume.\n" );

		return( -1 );
	}
	if( libewf_common_memset( volume, 0, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_write: unable to clear volume.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	volume->unknown1[ 0 ] = 1;

	if( libewf_endian_revert_32bit( internal_handle->media->amount_of_chunks, volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_write: unable to revert amount of chunks value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_write: unable to revert sectors per chunk value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_write: unable to revert bytes per sector value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_write: unable to revert amount of sectors value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_s01_write: amount_of_chunks: %" PRIu32 ", sectors_per_chunk: %" PRIu32 ", bytes_per_sector: %" PRIu32 ", amount_of_sectors: %" PRIu32 ".\n", internal_handle->media->amount_of_chunks, internal_handle->media->sectors_per_chunk, internal_handle->media->bytes_per_sector, internal_handle->media->amount_of_sectors );

	if( internal_handle->format == LIBEWF_FORMAT_SMART )
	{
		volume->signature[ 0 ] = (uint8_t) 'S';
		volume->signature[ 1 ] = (uint8_t) 'M';
		volume->signature[ 2 ] = (uint8_t) 'A';
		volume->signature[ 3 ] = (uint8_t) 'R';
		volume->signature[ 4 ] = (uint8_t) 'T';
	}
	section_write_count = libewf_section_start_write( internal_handle, file_descriptor, (EWF_CHAR *) "volume", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_write: unable to write section to file.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	volume_write_count = ewf_volume_smart_write( volume, file_descriptor );

	libewf_common_free( volume );

	if( volume_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_s01_write: unable to write volume to file.\n" );

		return( -1 );
	}
	return( section_write_count + volume_write_count );
}

/* Reads an EWF-E01 (EnCase) volume section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_e01_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_VOLUME *volume      = NULL;
	EWF_CRC calculated_crc  = 0;
	EWF_CRC stored_crc      = 0;
	int32_t bytes_per_chunk = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( size != EWF_VOLUME_SIZE )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: mismatch in section volume size.\n" );

		return( -1 );
	}
	volume = (EWF_VOLUME *) libewf_common_alloc( size );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to read volume.\n" );

		return( -1 );
	}
	if( ewf_volume_read( volume, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to read volume.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
#ifdef HAVE_DEBUG_OUTPUT
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown1, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown2, 16 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown3, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown4, 12 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown5, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown6, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->unknown7, 963 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume->signature, 5 ); );
#endif

	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) volume, ( EWF_VOLUME_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to calculate CRC.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, volume->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to convert stored CRC value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	bytes_per_chunk = ewf_volume_calculate_chunk_size( volume );

	if( bytes_per_chunk <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to calculate chunk size - using default.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume );

			return( -1 );
		}
		bytes_per_chunk = EWF_MINIMUM_CHUNK_SIZE;
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->amount_of_chunks, volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to convert amount of chunks value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to convert sectors per chunk value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to convert bytes per sector value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to convert amount of sectors value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->error_granularity, volume->error_granularity ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to convert error granularity value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	internal_handle->media->chunk_size  = (uint32_t) bytes_per_chunk;
	internal_handle->media->media_type  = volume->media_type;
	internal_handle->media->media_flags = volume->media_flags;
	internal_handle->compression_level  = (int8_t) volume->compression_level;

	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_e01_read: this volume has %" PRIu32 " chunks of %" PRIi32 " bytes each, CRC %" PRIu32 " (%" PRIu32 ").\n", internal_handle->media->amount_of_chunks, bytes_per_chunk, stored_crc, calculated_crc );
	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_e01_read: This volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n", internal_handle->media->amount_of_sectors, internal_handle->media->bytes_per_sector );

	if( libewf_common_memcpy( internal_handle->guid, volume->guid, 16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_read: unable to set GUID.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume );

			return( -1 );
		}
	}
	if( internal_handle->amount_of_header_sections == 1 )
	{
		internal_handle->format = LIBEWF_FORMAT_ENCASE1;
	}
	libewf_common_free( volume );

	return( (int32_t) size );
}

/* Reads a volume section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	ssize_t count = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_read: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_read: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( size == EWF_VOLUME_SMART_SIZE )
	{
		internal_handle->ewf_format = EWF_FORMAT_S01;
		count              = libewf_section_volume_s01_read( internal_handle, file_descriptor, size );
	}
	else if( size == EWF_VOLUME_SIZE )
	{
		internal_handle->ewf_format = EWF_FORMAT_E01;
		count              = libewf_section_volume_e01_read( internal_handle, file_descriptor, size );
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
	if( internal_handle->media->amount_of_chunks == 0 )
	{
		internal_handle->ewf_format = EWF_FORMAT_L01;
	}
	return( count );
}

/* Write an EWF-E01 (EnCase) volume section to file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_e01_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset )
{
	EWF_VOLUME *volume          = NULL;
	ssize_t section_write_count = 0;
	ssize_t volume_write_count  = 0;
	size_t size                 = EWF_VOLUME_SIZE;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	volume = (EWF_VOLUME *) libewf_common_alloc( size );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: unable to create volume.\n" );

		return( -1 );
	}
	if( libewf_common_memset( volume, 0, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_write: unable to clear volume.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( internal_handle->format == LIBEWF_FORMAT_FTK )
	{
		volume->media_type = 0x01;
	}
	else
	{
		volume->media_type = internal_handle->media->media_type;
	}
	volume->media_flags = internal_handle->media->media_flags;

	if( libewf_endian_revert_32bit( internal_handle->media->amount_of_chunks, volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: unable to revert amount of chunks value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: unable to revert sectors per chunk value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: unable to revert bytes per sector value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: unable to revert amount of sectors value.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_volume_e01_write: amount_of_chunks: %" PRIu32 ", sectors_per_chunk: %" PRIu32 ", bytes_per_sector: %" PRIu32 ", amount_of_sectors: %" PRIu32 ".\n", internal_handle->media->amount_of_chunks, internal_handle->media->sectors_per_chunk, internal_handle->media->bytes_per_sector, internal_handle->media->amount_of_sectors );

	if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) )
	{
		volume->compression_level = (uint8_t) internal_handle->compression_level;

		if( libewf_common_memcpy( volume->guid, internal_handle->guid, 16 ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: unable to set GUID.\n" );

			libewf_common_free( volume );

			return( -1 );
		}
		if( libewf_endian_revert_32bit( internal_handle->media->error_granularity, volume->error_granularity ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: unable to revert error granularity value.\n" );

			libewf_common_free( volume );

			return( -1 );
		}
	}
	section_write_count = libewf_section_start_write( internal_handle, file_descriptor, (EWF_CHAR *)  "volume", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: unable to write section to file.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	volume_write_count = ewf_volume_write( volume, file_descriptor );

	libewf_common_free( volume );

	if( volume_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_volume_e01_write: unable to write volume to file.\n" );

		return( -1 );
	}
	return( section_write_count + volume_write_count );
}

/* Fills the offset table
 * Returns the pointer to the offset table, or NULL on error
 */
LIBEWF_OFFSET_TABLE *libewf_fill_offset_table( LIBEWF_OFFSET_TABLE *offset_table, EWF_TABLE_OFFSET *offsets, uint32_t amount_of_chunks, int file_descriptor, uint16_t segment_number, uint8_t error_tollerance )
{
	uint32_t chunk_size     = 0;
	uint32_t current_offset = 0;
	uint32_t next_offset    = 0;
	uint32_t raw_offset     = 0;
	uint32_t iterator       = 0;
	uint8_t compressed      = 0;

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
	/* Allocate additional entries in the offset table if needed
	 * - a single reallocation saves processing time
	 */
	if( offset_table->amount < ( offset_table->last + amount_of_chunks ) )
	{
		offset_table = libewf_offset_table_realloc( offset_table, ( offset_table->last + amount_of_chunks ) );

		if( offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: unable to reallocate offset table.\n" );

			return( NULL );
		}
	}
	/* Read the offsets from file
	 */
	if( libewf_endian_convert_32bit( &raw_offset, offsets[ iterator ].offset ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: unable to convert raw offset value.\n" );

		return( NULL );
	}
	/* The size of the last chunk must be determined differently
	 */
	while( iterator < ( amount_of_chunks - 1 ) )
	{
		compressed     = (uint8_t) ( raw_offset >> 31 );
		current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;

		if( libewf_endian_convert_32bit( &raw_offset, offsets[ iterator + 1 ].offset ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: unable to convert raw offset value.\n" );

			return( NULL );
		}
		next_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;

		if( next_offset < current_offset )
		{
			LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: invalid chunk offset larger than next.\n" );

			return( NULL );
		}
		chunk_size = next_offset - current_offset;

		if( current_offset > (uint32_t) INT32_MAX )
		{
			LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: invalid chunk offset only values below 2^32 are supported.\n" );

			return( NULL );
		}
		if( chunk_size == 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: invalid chunk size - size is zero.\n" );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( NULL );
			}
		}
		if( chunk_size > (uint32_t) INT32_MAX )
		{
			LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: invalid chunk size only values below 2^32 are supported.\n" );

			return( NULL );
		}
		if( libewf_offset_table_set_values( offset_table, offset_table->last, file_descriptor, compressed, (off_t) current_offset, (size_t) chunk_size, segment_number ) == -1 )
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
			LIBEWF_VERBOSE_PRINT( "libewf_fill_offset_table: uncompressed chunk %" PRIu32 " read with offset %" PRIu32 " and size %" PRIu32 ".\n",
                                              offset_table->last, current_offset, chunk_size );
		}
		else
		{
			LIBEWF_VERBOSE_PRINT( "libewf_fill_offset_table: compressed chunk %" PRIu32 " read with offset %" PRIu32 " and size %" PRIu32 ".\n",
			                      offset_table->last, current_offset, chunk_size );
		}
		current_offset = next_offset;

		iterator++;
	}
	if( libewf_endian_convert_32bit( &raw_offset, offsets[ iterator ].offset ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: unable to convert raw offset value.\n" );

		return( NULL );
	}
	compressed     = (uint8_t) ( raw_offset >> 31 );
	current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;

	if( current_offset > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: invalid chunk offset only values below 2^32 are supported.\n" );

		return( NULL );
	}
	if( libewf_offset_table_set_values( offset_table, offset_table->last, file_descriptor, compressed, current_offset, 0, segment_number ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_fill_offset_table: unable to set value in offset table.\n" );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( NULL );
		}
	}
	if( compressed == 0 )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_fill_offset_table: uncompressed last chunk %" PRIu32 " read with offset %" PRIu32 ".\n",
		                      ( offset_table->last + 1 ), current_offset );
	}
	else
	{
		LIBEWF_VERBOSE_PRINT( "libewf_fill_offset_table: compressed last chunk %" PRIu32 " read with offset %" PRIu64 ".\n",
                                      ( offset_table->last + 1 ), current_offset );
	}
	return( offset_table );
}

/* Calculate the last offset
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_calculate_last_offset( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, int file_descriptor, uint16_t segment_number, uint8_t error_tollerance )
{
	LIBEWF_SECTION_LIST_ENTRY *section_list_entry = NULL;
	off_t last_offset                             = 0;
	size_t chunk_size                             = 0;

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: invalid offset table.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: invalid section list.\n" );

		return( -1 );
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
#ifdef HAVE_DEBUG_OUTPUT
		LIBEWF_VERBOSE_PRINT( "libewf_calculate_last_offset: start offset: %jd last offset: %jd.\n", section_list_entry->start_offset, last_offset );
#endif

		if( ( section_list_entry->start_offset < last_offset ) && ( last_offset < section_list_entry->end_offset ) )
		{
			chunk_size = section_list_entry->end_offset - last_offset;

			if( last_offset > (off_t) INT32_MAX )
			{
				LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: invalid last chunk offset only values below 2^32 are supported.\n" );

				return( -1 );
			}
			if( chunk_size == 0 )
			{
				LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: invalid chunk size - size is zero.\n" );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
			if( chunk_size > (size_t) INT32_MAX )
			{
				LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: invalid chunk size only values below 2^32 are supported.\n" );

				return( -1 );
			}
			if( libewf_offset_table_set_values( offset_table, offset_table->last, file_descriptor, offset_table->compressed[ offset_table->last ], last_offset, chunk_size, segment_number ) == -1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_calculate_last_offset: unable to set value in offset table.\n" );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
			LIBEWF_VERBOSE_PRINT( "libewf_calculate_last_offset: last chunk %" PRIu32 " calculated with offset: %jd and size %zu.\n", ( offset_table->last + 1 ), last_offset, chunk_size );

			break;
		}
		section_list_entry = section_list_entry->next;
	}
	return( 1 );
}

/* Reads an offset table
 * Returns the pointer to the offset table, or NULL on error
 */
LIBEWF_OFFSET_TABLE *libewf_offset_table_read( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, uint32_t *amount_of_chunks, int file_descriptor, uint16_t segment_number, size_t size, uint8_t ewf_format, uint8_t error_tollerance )
{
	LIBEWF_OFFSET_TABLE *reallocation = NULL;
	EWF_TABLE *table                  = NULL;
	EWF_TABLE_OFFSET *offsets         = NULL;
	EWF_CRC calculated_crc            = 0;
	EWF_CRC stored_crc                = 0;

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: invalid offset table.\n" );

		return( NULL );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: invalid section list.\n" );

		return( NULL );
	}
	if( amount_of_chunks == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: invalid amount of chunks.\n" );

		return( NULL );
	}
	table = (EWF_TABLE *) libewf_common_alloc( EWF_TABLE_SIZE );

	if( table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to allocate table.\n" );

		return( NULL );
	}
	if( ewf_table_read( table, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to read table.\n" );

		libewf_common_free( table );

		return( NULL );
	}
#ifdef HAVE_DEBUG_OUTPUT
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( table->padding, 16 ); );
#endif

	/* The table size contains the size of the CRC (4 bytes)
	 */
	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) table, ( EWF_TABLE_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to calculate CRC.\n" );

		libewf_common_free( table );

		return( NULL );
	}
	if( libewf_endian_convert_32bit( &stored_crc, table->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to convert stored CRC value.\n" );

		libewf_common_free( table );

		return( NULL );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );
		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( table );

			return( NULL );
		}
	}
	if( libewf_endian_convert_32bit( amount_of_chunks, table->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to convert amount of chunks value.\n" );

		libewf_common_free( table );

		return( NULL );
	}
	libewf_common_free( table );

	LIBEWF_VERBOSE_PRINT( "libewf_offset_table_read: table is of size %" PRIu32 " chunks CRC %" PRIu32 " (%" PRIu32 ").\n", *amount_of_chunks, stored_crc, calculated_crc );

	size -= EWF_TABLE_SIZE;

	if( *amount_of_chunks == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_offset_table_read: table contains no offsets.\n" );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( NULL );
		}
	}
	else
	{
		/* Check if the maximum amount of offsets is not exceeded
		 */
		if( *amount_of_chunks > EWF_MAXIMUM_OFFSETS_IN_TABLE )
		{
			LIBEWF_WARNING_PRINT( "libewf_offset_table_read: table contains more than %d offsets!.\n", EWF_MAXIMUM_OFFSETS_IN_TABLE );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( NULL );
			}
		}
		offsets = (EWF_TABLE_OFFSET *) libewf_common_alloc( EWF_TABLE_OFFSET_SIZE * *amount_of_chunks );

		if( offsets == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to allocated table offsets.\n" );

			return( NULL );
		}
		if( ewf_table_offsets_read( offsets, file_descriptor, *amount_of_chunks ) <= -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to read table offsets.\n" );

			libewf_common_free( offsets );

			return( NULL );
		}
		size -= ( EWF_TABLE_OFFSET_SIZE * *amount_of_chunks );

		/* The EWF-S01 format does not contain a CRC after the offsets
		 */
		if( ewf_format != EWF_FORMAT_S01 )
		{
			/* Check if the offset table CRC matches
			 */
			if( ewf_crc_calculate( &calculated_crc, (uint8_t *) offsets, ( EWF_TABLE_OFFSET_SIZE * *amount_of_chunks ), 1 ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to calculate CRC.\n" );

				libewf_common_free( offsets );

				return( NULL );
			}
			if( ewf_crc_read( &stored_crc, file_descriptor ) != (int32_t) EWF_CRC_SIZE )
			{
				LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to read CRC from file descriptor.\n" );

				libewf_common_free( offsets );

				return( NULL );
			}
			if( stored_crc != calculated_crc )
			{
				LIBEWF_WARNING_PRINT( "libewf_offset_table_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					libewf_common_free( offsets );

					return( NULL );
				}
			}
			size -= EWF_CRC_SIZE;
		}
		reallocation = libewf_fill_offset_table( offset_table, offsets, *amount_of_chunks, file_descriptor, segment_number, error_tollerance );

		libewf_common_free( offsets );

		if( reallocation == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to fill offset table.\n" );

			return( NULL );
		}
		offset_table = reallocation;

		if( libewf_calculate_last_offset( offset_table, section_list, file_descriptor, segment_number, error_tollerance ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to calculate last offset.\n" );

			return( NULL );
		}
	}
	/* Skip the chunk data within the section
	 */
	if( ( size > 0 ) && ( size <= (uint64_t) INT32_MAX ) )
	{
		if( libewf_common_lseek( file_descriptor, size, SEEK_CUR ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_offset_table_read: unable to align with next section.\n" );

			return( NULL );
		}
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
ssize_t libewf_section_table_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size, LIBEWF_SECTION_LIST *section_list, uint16_t segment_number )
{
	LIBEWF_OFFSET_TABLE *reallocation = NULL;
	uint32_t amount_of_chunks         = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_read: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_read: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_read: invalid handle - missing segment table.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_read: invalid section list.\n" );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		internal_handle->offset_table = libewf_offset_table_alloc( internal_handle->media->amount_of_chunks );

		if( internal_handle->offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_table_read: unable to create offset table.\n" );

			return( -1 );
		}
	}
	reallocation = libewf_offset_table_read( internal_handle->offset_table, section_list, &amount_of_chunks, file_descriptor, segment_number, size, internal_handle->ewf_format, internal_handle->error_tollerance );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_read: unable to read offset table.\n" );

		return( -1 );
	}
	internal_handle->offset_table = reallocation;

	internal_handle->segment_table->amount_of_chunks[ segment_number ] += amount_of_chunks;

	return( (int32_t) size );
}

/* Write a table or table2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_table_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, LIBEWF_OFFSET_TABLE *offset_table, uint32_t offset_table_index, uint32_t amount_of_offsets, EWF_CHAR *section_header, size_t additional_size )
{
	EWF_TABLE *table                  = NULL;
	EWF_TABLE_OFFSET *offsets         = NULL;
	ssize_t section_write_count       = 0;
	ssize_t table_write_count         = 0;
	ssize_t table_offsets_write_count = 0;
	size_t size                       = 0;
	uint32_t offset_value             = 0;
	uint32_t iterator                 = 0;
	uint8_t write_crc                 = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_write: invalid handle.\n" );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_write: invalid offset table.\n" );

		return( -1 );
	}
	size = EWF_TABLE_SIZE + ( EWF_TABLE_OFFSET_SIZE * amount_of_offsets ) + additional_size;

	if( internal_handle->ewf_format != EWF_FORMAT_S01 )
	{
		write_crc = 1;
		size     += EWF_CRC_SIZE;
	}
	table = (EWF_TABLE *) libewf_common_alloc( EWF_TABLE_SIZE );

	if( table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_write: unable to create table.\n" );

		return( -1 );
	}
	if( libewf_common_memset( table, 0, EWF_TABLE_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_write: unable to clear table.\n" );

		libewf_common_free( table );

		return( -1 );
	}
	offsets = (EWF_TABLE_OFFSET *) libewf_common_alloc( EWF_TABLE_OFFSET_SIZE * amount_of_offsets );

	if( offsets == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_write: unable to create offsets.\n" );

		libewf_common_free( table );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( amount_of_offsets, table->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_write: unable to revert amount of chunks value.\n" );

		libewf_common_free( table );
		libewf_common_free( offsets );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_offsets; iterator++ )
	{
		offset_value = offset_table->offset[ offset_table_index + iterator ];

		if( offset_table->compressed[ offset_table_index + iterator ] != 0 )
		{
			offset_value |= EWF_OFFSET_COMPRESSED_WRITE_MASK;
		}
		if( libewf_endian_revert_32bit( offset_value, (uint8_t *) offsets[ iterator ].offset ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_table_write: unable to revert start offset.\n" );

			libewf_common_free( table );
			libewf_common_free( offsets );

			return( -1 );
		}
	}
	section_write_count = libewf_section_start_write( internal_handle, file_descriptor, section_header, size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_write: unable to write section to file.\n" );

		libewf_common_free( table );
		libewf_common_free( offsets );

		return( -1 );
	}
	table_write_count         = ewf_table_write( table, file_descriptor );
	table_offsets_write_count = ewf_table_offsets_write( offsets, file_descriptor, amount_of_offsets, write_crc );

	libewf_common_free( table );
	libewf_common_free( offsets );

	if( table_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_write: unable to write table to file.\n" );

		return( -1 );
	}
	if( table_offsets_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table_write: unable to write table offsets to file.\n" );

		return( -1 );
	}
	return( section_write_count + table_write_count + table_offsets_write_count );
}

/* Reads a table2 section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_table2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size, LIBEWF_SECTION_LIST *section_list, uint16_t segment_number )
{
	LIBEWF_OFFSET_TABLE *reallocation = NULL;
	uint32_t amount_of_chunks         = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_read: invalid handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_read: invalid section list.\n" );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_read: invalid size only values below 2^32 are supported.\n" );

		return( -1 );
	}
	if( internal_handle->secondary_offset_table == NULL )
	{
		internal_handle->secondary_offset_table = libewf_offset_table_alloc( internal_handle->media->amount_of_chunks );

		if( internal_handle->secondary_offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_table2_read: unable to create secondairy offset table.\n" );

			return( -1 );
		}
	}
	reallocation = libewf_offset_table_read( internal_handle->secondary_offset_table, section_list, &amount_of_chunks, file_descriptor, segment_number, size, internal_handle->ewf_format, internal_handle->error_tollerance );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_read: unable to read offset table.\n" );

		return( -1 );
	}
	internal_handle->secondary_offset_table = reallocation;

	if( libewf_compare_offset_tables( internal_handle->offset_table, internal_handle->secondary_offset_table ) == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_table2_read: table1 and table2 differ.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		/* TODO Try to correct the table
		 */
	}
	return( (int32_t) size );
}

/* Reads a sectors section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_sectors_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: invalid handle.\n" );

		return( -1 );
	}
	/* In the EWF-E01 format the sectors section holds the actual data chunks
	 */
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: EWF-S01 format should not contain sectors section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	/* Skip the chunk data within the section
	 */
	if( libewf_common_lseek( file_descriptor, size, SEEK_CUR ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: unable to align with next section.\n" );

		return( -1 );
	}
	return( (int32_t) size );
}

/* Reads a ltree section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_ltree_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_LTREE *ltree          = NULL;
	EWF_LTREE_DATA *tree_data = NULL;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_ltree_read: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: EWF-S01 format should not contain ltree section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	internal_handle->ewf_format = EWF_FORMAT_L01;

	ltree = (EWF_LTREE *) libewf_common_alloc( EWF_LTREE_SIZE );

	if( ltree == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_ltree_read: unable to allocate ltree.\n" );

		return( -1 );
	}
	if( ewf_ltree_read( ltree, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_ltree_read: unable to read ltree.\n" );

		libewf_common_free( ltree );

		return( -1 );
	}
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown1, 16 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->tree_size, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown2, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown3, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( ltree->unknown4, 20 ); );

	libewf_common_free( ltree );

	tree_data = ewf_tree_data_read( file_descriptor, ( size - EWF_LTREE_SIZE ) );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header2_fprint( stderr, tree_data, size ); );

	libewf_common_free( tree_data );

	return( (int32_t) size );
}

/* Reads a data section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_data_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_DATA *data             = NULL;
	EWF_CRC calculated_crc     = 0;
	EWF_CRC stored_crc         = 0;
	uint32_t amount_of_chunks  = 0;
	uint32_t sectors_per_chunk = 0;
	uint32_t bytes_per_sector  = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t error_granularity = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_sectors_read: EWF-S01 format should not contain data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	if( size != EWF_DATA_SIZE )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: mismatch in section data size.\n" );

		return( -1 );
	}
	data = (EWF_DATA *) libewf_common_alloc( EWF_DATA_SIZE );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to create data.\n" );

		return( -1 );
	}
	if( ewf_data_read( data, file_descriptor ) < -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to read data.\n" );

		libewf_common_free( data );

		return( -1 );
	}
#ifdef HAVE_DEBUG_OUTPUT
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown1, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown2, 16 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown3, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown4, 12 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown5, 3 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown6, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->unknown7, 963 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( data->signature, 5 ); );
#endif
	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) data, ( EWF_DATA_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to calculate CRC.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, data->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to convert stored CRC value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	/* TODO add more checks
	 */
	if( internal_handle->media->media_type != data->media_type )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: media type does not match in data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &amount_of_chunks, data->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to convert amount of chunks value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->amount_of_chunks != amount_of_chunks )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: amount of chunks does not match in data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &sectors_per_chunk, data->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to convert sectors per chunk value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->sectors_per_chunk != sectors_per_chunk )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: sectors per chunk does not match in data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &bytes_per_sector, data->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to convert bytes per sector value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->bytes_per_sector != bytes_per_sector )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: bytes per sector does not match in data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &amount_of_sectors, data->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to convert amount of sectors value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->amount_of_sectors != amount_of_sectors )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: amount of sectors does not match in data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &error_granularity, data->error_granularity ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: unable to convert error granularity value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->error_granularity != error_granularity )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: error granularity does not match in data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( internal_handle->media->media_flags != data->media_flags )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: media flags do not match in data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_common_memcmp( internal_handle->guid, data->guid, 16 ) != 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_read: GUID does not match in data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	libewf_common_free( data );

	return( (int32_t) size );
}

/* Write a data section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_data_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset )
{
	EWF_DATA *data              = NULL;
	ssize_t section_write_count = 0;
	ssize_t data_write_count    = 0;
	size_t size                 = EWF_DATA_SIZE;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	data = (EWF_DATA *) libewf_common_alloc( size );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to create data.\n" );

		return( -1 );
	}
	if( libewf_common_memset( data, 0, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to clear data.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->format == LIBEWF_FORMAT_FTK )
	{
		data->media_type = 0x01;
	}
	else
	{
		data->media_type = internal_handle->media->media_type;
	}
	data->media_flags = internal_handle->media->media_flags;

	if( libewf_endian_revert_32bit( internal_handle->media->amount_of_chunks, data->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to revert amount of chunks value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->sectors_per_chunk, data->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to revert sectors per chunk value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->bytes_per_sector, data->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to revert bytes per sector value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->amount_of_sectors, data->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to revert amount of sectors value.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) )
	{
		data->compression_level = (uint8_t) internal_handle->compression_level;

		if( libewf_common_memcpy( data->guid, internal_handle->guid, 16 ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to set GUID.\n" );

			libewf_common_free( data );

			return( -1 );
		}
		if( libewf_endian_revert_32bit( internal_handle->media->error_granularity, data->error_granularity ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to revert error granularity value.\n" );

			libewf_common_free( data );

			return( -1 );
		}
	}
	section_write_count = libewf_section_start_write( internal_handle, file_descriptor, (EWF_CHAR *) "data", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to write section to file.\n" );

		libewf_common_free( data );

		return( -1 );
	}
	data_write_count = ewf_data_write( data, file_descriptor );

	libewf_common_free( data );

	if( data_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_data_write: unable to write data to file.\n" );

		return( -1 );
	}
	return( section_write_count + data_write_count );
}

/* Reads a error2 section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_error2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_ERROR2 *error2                = NULL;
	EWF_ERROR2_SECTOR *error2_sectors = NULL;
	EWF_CRC calculated_crc            = 0;
	EWF_CRC stored_crc                = 0;
	uint32_t amount_of_errors         = 0;
	uint32_t iterator                 = 0;
	uint32_t sector                   = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: EWF-S01 format should not contain data section.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	error2 = (EWF_ERROR2 *) libewf_common_alloc( EWF_ERROR2_SIZE );

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to allocate error2.\n" );

		return( -1 );
	}
	if( ewf_error2_read( error2, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to read error2.\n" );

		libewf_common_free( error2 );

		return( -1 );
	}
#ifdef HAVE_DEBUG_OUTPUT
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( error2->unknown, 200 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( (uint8_t *) error2_sectors, ( EWF_ERROR2_SECTOR_SIZE * amount_of_errors ) ); );
#endif

	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) error2, ( EWF_ERROR2_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to calculate CRC.\n" );

		libewf_common_free( error2 );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, error2->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to convert stored CRC value.\n" );

		libewf_common_free( error2 );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &amount_of_errors, error2->amount_of_errors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to convert amount of errors value.\n" );

		libewf_common_free( error2 );

		return( -1 );
	}
	libewf_common_free( error2 );

	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	if( amount_of_errors == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_read: error2 contains no sectors!.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	else
	{
		error2_sectors = (EWF_ERROR2_SECTOR *) libewf_common_alloc( EWF_ERROR2_SECTOR_SIZE * amount_of_errors );

		if( error2_sectors == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to allocate error2 sectors.\n" );

			return( -1 );
		}
		if( ewf_error2_sectors_read( error2_sectors, file_descriptor, amount_of_errors ) <= -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to read error2 sectors.\n" );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( ewf_crc_calculate( &calculated_crc, (uint8_t *) error2_sectors, ( EWF_ERROR2_SECTOR_SIZE * amount_of_errors ), 1 ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to calculate CRC.\n" );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( ewf_crc_read( &stored_crc, file_descriptor ) != EWF_CRC_SIZE )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to read CRC from file descriptor.\n" );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( stored_crc != calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );

			if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				libewf_common_free( error2_sectors );

				return( -1 );
			}
		}
		if( internal_handle->acquiry_error_sectors != NULL )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_section_error2_read: acquiry error sectors already set in handle - removing previous one.\n" );

			libewf_common_free( internal_handle->acquiry_error_sectors );
		}
		internal_handle->acquiry_error_sectors = (LIBEWF_ERROR_SECTOR *) libewf_common_alloc( LIBEWF_ERROR_SECTOR_SIZE * amount_of_errors );

		if( internal_handle->acquiry_error_sectors == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to create acquiry error sectors.\n" );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		internal_handle->acquiry_amount_of_errors = amount_of_errors;

		for( iterator = 0; iterator < amount_of_errors; iterator++ )
		{
			if( libewf_endian_convert_32bit( &sector, error2_sectors[ iterator ].sector ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to convert sector value.\n" );

				libewf_common_free( error2_sectors );

				return( -1 );
			}
			internal_handle->acquiry_error_sectors[ iterator ].sector = (uint64_t) sector;

			if( libewf_endian_convert_32bit( &internal_handle->acquiry_error_sectors[ iterator ].amount_of_sectors, error2_sectors[ iterator ].amount_of_sectors ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_section_error2_read: unable to convert amount of sectors value.\n" );

				libewf_common_free( error2_sectors );

				return( -1 );
			}
		}
		libewf_common_free( error2_sectors );
	}
	return( (int32_t) size );
}

/* Write a error2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_error2_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, LIBEWF_ERROR_SECTOR *sectors, uint32_t amount_of_errors )
{
	EWF_ERROR2 *error2                 = NULL;
	EWF_ERROR2_SECTOR *error2_sectors  = NULL;
	ssize_t section_write_count        = 0;
	ssize_t error2_write_count         = 0;
	ssize_t error2_sectors_write_count = 0;
	size_t size                        = 0;
	uint32_t iterator                  = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_write: invalid handle.\n" );

		return( -1 );
	}
	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_write: invalid acquiry read error sectors.\n" );

		return( -1 );
	}
	size   = EWF_ERROR2_SIZE + ( EWF_ERROR2_SECTOR_SIZE * amount_of_errors ) + EWF_CRC_SIZE;
	error2 = (EWF_ERROR2 *) libewf_common_alloc( EWF_ERROR2_SIZE );

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_write: unable to create error2.\n" );

		return( -1 );
	}
	if( libewf_common_memset( error2, 0, EWF_ERROR2_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_write: unable to clear error2.\n" );

		libewf_common_free( error2 );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( amount_of_errors, error2->amount_of_errors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_write: unable to revert amount of errors value.\n" );

		libewf_common_free( error2 );

		return( -1 );
	}
	error2_sectors = (EWF_ERROR2_SECTOR *) libewf_common_alloc( EWF_ERROR2_SECTOR_SIZE * amount_of_errors );

	if( error2_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_write: unable to create error2 sectors.\n" );

		libewf_common_free( error2 );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_errors; iterator++ )
	{
		if( libewf_endian_revert_32bit( (uint32_t) sectors[ iterator ].sector, error2_sectors[ iterator ].sector ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_write: unable to revert sector value.\n" );

			libewf_common_free( error2 );
			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( libewf_endian_revert_32bit( sectors[ iterator ].amount_of_sectors, error2_sectors[ iterator ].amount_of_sectors ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_error2_write: unable to revert amount of sectors value.\n" );

			libewf_common_free( error2 );
			libewf_common_free( error2_sectors );

			return( -1 );
		}
	}
	section_write_count = libewf_section_start_write( internal_handle, file_descriptor, (EWF_CHAR *) "error2", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_write: unable to write section to file.\n" );

		libewf_common_free( error2 );
		libewf_common_free( error2_sectors );

		return( -1 );
	}
	error2_write_count         = ewf_error2_write( error2, file_descriptor );
	error2_sectors_write_count = ewf_error2_sectors_write( error2_sectors, file_descriptor, amount_of_errors );

	libewf_common_free( error2 );
	libewf_common_free( error2_sectors );

	if( error2_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_write: unable to write error2 to file.\n" );

		return( -1 );
	}
	if( error2_sectors_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_error2_write: unable to write error2 sectors to file.\n" );

		return( -1 );
	}
	return( section_write_count + error2_write_count + error2_sectors_write_count );
}

/* Reads a hash section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_hash_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_HASH *hash         = NULL;
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: invalid handle.\n" );

		return( -1 );
	}
	if( size != EWF_HASH_SIZE )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: mismatch in section data size.\n" );

		return( -1 );
	}
	hash = (EWF_HASH *) libewf_common_alloc( EWF_HASH_SIZE );

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: unable to allocate hash.\n" );

		return( -1 );
	}
	if( ewf_hash_read( hash, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: unable to read hash.\n" );

		libewf_common_free( hash );

		return( -1 );
	}
#ifdef HAVE_DEBUG_OUTPUT
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash->unknown1, 16 ); );
#endif

	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) hash, ( EWF_HASH_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: unable to calculate CRC.\n" );

		libewf_common_free( hash );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, hash->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: unable to convert stored CRC value.\n" );

		libewf_common_free( hash );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( hash );

			return( -1 );
		}
	}
	if( libewf_internal_handle_set_stored_md5_hash( internal_handle, hash->md5_hash ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_read: unable to set MD5 hash in handle.\n" );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( hash );

			return( -1 );
		}
	}
	libewf_common_free( hash );

	return( (int32_t) size );
}

/* Write a hash section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_hash_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_DIGEST_HASH *md5_hash )
{
	EWF_HASH *hash              = NULL;
	ssize_t section_write_count = 0;
	ssize_t hash_write_count    = 0;
	size_t size                 = EWF_HASH_SIZE;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_write: invalid handle.\n" );

		return( -1 );
	}
	hash = (EWF_HASH *) libewf_common_alloc( size );

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_write: unable to create hash.\n" );

		return( -1 );
	}
	if( libewf_common_memset( hash, 0, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_write: unable to clear hash.\n" );

		libewf_common_free( hash );

		return( -1 );
	}
	if( libewf_common_memcpy( hash->md5_hash, md5_hash, EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_write: unable to set hash.\n" );

		libewf_common_free( hash );

		return( -1 );
	}
	section_write_count = libewf_section_start_write( internal_handle, file_descriptor, (EWF_CHAR *) "hash", size, start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_write: unable to write section to file.\n" );

		libewf_common_free( hash );

		return( -1 );
	}
	hash_write_count = ewf_hash_write( hash, file_descriptor );

	libewf_common_free( hash );

	if( hash_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_hash_write: unable to write hash to file.\n" );

		return( -1 );
	}
	return( section_write_count + hash_write_count );
}

/* Write the last section start to file
 * This is used for the next and done sections, these sections point back towards themselves
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_last_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, EWF_CHAR *section_type, off_t start_offset )
{
	EWF_SECTION *section     = NULL;
	size_t section_type_size = 0;
	ssize_t write_count      = 0;
	uint64_t section_size    = 0;
	uint64_t section_offset  = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: invalid handle.\n" );

		return( -1 );
	}
	if( section_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: invalid section type.\n" );

		return( -1 );
	}
	section = (EWF_SECTION *) libewf_common_alloc( EWF_SECTION_SIZE );

	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: unable to create section.\n" );

		return( -1 );
	}
	if( libewf_common_memset( section, 0, EWF_SECTION_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: unable to clear section.\n" );

		libewf_common_free( section );

		return( -1 );
	}
	/* The EnCase (EWF-E01) format leaves the size of this section empty
	 */
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 ) || ( internal_handle->format == LIBEWF_FORMAT_FTK ) )
	{
		section_size = EWF_SECTION_SIZE;
	}
	section_type_size = ewf_string_length( section_type );
	section_offset    = start_offset;

	if( section_type_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: section type is empty.\n" );

		return( -1 );
	}
	if( section_type_size >= 16 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: section type is too long.\n" );

		libewf_common_free( section );

		return( -1 );
	}
	/* Add one character for the end of string
	 */
	if( ewf_string_copy( section->type, section_type, ( section_type_size + 1 ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: unable to set section type.\n" );

		libewf_common_free( section );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( section_size, section->size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: unable to revert size value.\n" );

		libewf_common_free( section );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( section_offset, section->next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: unable to revert next offset value.\n" );

		libewf_common_free( section );

		return( -1 );
	}
	write_count = ewf_section_write( section, file_descriptor );

	libewf_common_free( section );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_last_write: unable to write section to file.\n" );

		return( -1 );
	}
	return( write_count );
}

/* Reads a xheader section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_xheader_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_CHAR *xheader  = NULL;
	ssize_t read_count = (ssize_t) size;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_xheader_read: invalid handle.\n" );

		return( -1 );
	}
	xheader = ewf_string_read_compressed( file_descriptor, &size );

	if( xheader == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_xheader_read: unable to read xheader.\n" );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_xheader_read: Header:\n" );
	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xheader, size ); );

	if( libewf_internal_handle_is_set_xheader( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_set_xheader( internal_handle, xheader, size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_xheader_read: unable to set xheader in handle.\n" );

			libewf_common_free( xheader );

			if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
	}
	else
	{
		libewf_common_free( xheader );
	}
	internal_handle->amount_of_header_sections++;

	return( read_count );
}

/* Write a xheader section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_xheader_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_CHAR *xheader, size_t size, int8_t compression_level )
{
	ssize_t section_write_count = 0;

	section_write_count = libewf_section_compressed_string_write( internal_handle, file_descriptor, start_offset, (EWF_CHAR *) "xheader", xheader, size, compression_level );

	if( section_write_count != -1 )
	{
		internal_handle->amount_of_header_sections++;
	}
	return( section_write_count );
}

/* Reads a xhash section
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_xhash_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_CHAR *xhash    = NULL;
	ssize_t read_count = (ssize_t) size;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_xhash_read: invalid handle.\n" );

		return( -1 );
	}
	xhash = ewf_string_read_compressed( file_descriptor, &size );

	if( xhash == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_xhash_read: unable to read xhash.\n" );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_section_xhash_read: Hash:\n" );
	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xhash, size ); );

	if( libewf_internal_handle_is_set_xhash( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_set_xhash( internal_handle, xhash, size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_xhash_read: unable to set xhash in handle.\n" );

			libewf_common_free( xhash );

			if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
	}
	else
	{
		libewf_common_free( xhash );
	}
	return( read_count );
}

/* Reads and processes a section
 * The section start offset will be updated
 * Returns the section start of the section read, or NULL on error
 */
EWF_SECTION *libewf_section_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, LIBEWF_SECTION_LIST *section_list, uint16_t segment_number, off_t *section_start_offset )
{
	EWF_SECTION *section     = NULL;
	off_t section_end_offset = 0;
	ssize_t count            = 0;
	uint64_t size            = 0;
	uint64_t next_offset     = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: invalid handle.\n" );

		return( NULL );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: invalid section list.\n" );

		return( NULL );
	}
	if( *section_start_offset > (off_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: invalid section start offset only values below 2^32 are supported.\n" );

		return( NULL );
	}
	section = libewf_section_start_read( internal_handle, file_descriptor );

	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: unable to read section start.\n" );

		return( NULL );
	}
	if( libewf_endian_convert_64bit( &size, section->size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: unable to convert size value.\n" );

		libewf_common_free( section );

		return( NULL );
	}
	if( size > (uint64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: invalid size only values below 2^32 are supported.\n" );

		libewf_common_free( section );

		return( NULL );
	}
	if( libewf_endian_convert_64bit( &next_offset, section->next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: unable to convert next offset value.\n" );

		libewf_common_free( section );

		return( NULL );
	}
	if( next_offset > (uint64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: invalid next_offset only values below 2^32 are supported.\n" );

		libewf_common_free( section );

		return( NULL );
	}
	section_end_offset = *section_start_offset + (off_t) size;

	if( section_end_offset > (off_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: invalid section end offset only values below 2^32 are supported.\n" );

		libewf_common_free( section );

		return( NULL );
	}
	if( libewf_section_list_append( section_list, section->type, *section_start_offset, section_end_offset ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: unable to append value to section list.\n" );

		libewf_common_free( section );

		return( NULL );
	}
	*section_start_offset += EWF_SECTION_SIZE;

	/* No need to correct empty sections like done and next
	 */
	if( size > 0 )
	{
		size -= EWF_SECTION_SIZE;
	}
	if( size > (uint64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: invalid size only values below 2^32 are supported.\n" );

		libewf_common_free( section );

		return( NULL );
	}

	/* Nothing to do for the next and done section
	 */
	if( ( ewf_section_is_type_next( section ) == 1 ) || ( ewf_section_is_type_done( section ) == 1 ) )
	{
		/* Determine the size of the largest segment file for read and write mode only
		 */
		if( ( internal_handle->write != NULL )
		 && ( internal_handle->write->segment_file_size < ( *section_start_offset + EWF_SECTION_SIZE ) ) )
		{
			internal_handle->write->segment_file_size = (uint32_t) ( *section_start_offset + EWF_SECTION_SIZE );
		}
	}
	/* Read the header2 section
	 */
	else if( ewf_section_is_type_header2( section ) == 1 )
	{
		count = libewf_section_header2_read( internal_handle, file_descriptor, (size_t) size );
	}
	/* Read the header section
	 */
	else if( ewf_section_is_type_header( section ) == 1 )
	{
		count = libewf_section_header_read( internal_handle, file_descriptor, (size_t) size );
	}
	/* Read the xheader section
	 */
	else if( ewf_section_is_type_xheader( section ) == 1 )
	{
		count = libewf_section_xheader_read( internal_handle, file_descriptor, (size_t) size );
	}
	/* Read the volume or disk section
	 */
	else if( ( ewf_section_is_type_volume( section ) == 1 ) || ( ewf_section_is_type_disk( section ) == 1 ) )
	{
		count = libewf_section_volume_read( internal_handle, file_descriptor, (size_t) size );
	}
	/* Read the table2 section
	 */
	else if( ewf_section_is_type_table2( section ) == 1 )
	{
		count = libewf_section_table2_read( internal_handle, file_descriptor, (size_t) size, section_list, segment_number );
	}
	/* Read the table section
	 */
	else if( ewf_section_is_type_table( section ) == 1 )
	{
		count = libewf_section_table_read( internal_handle, file_descriptor, (size_t) size, section_list, segment_number );
	}
	/* Read the sectors section
	 */
	else if( ewf_section_is_type_sectors( section ) == 1 )
	{
		count = libewf_section_sectors_read( internal_handle, file_descriptor, (size_t) size );
	}
	/* Read the ltree section
	 */
	else if( ewf_section_is_type_ltree( section ) == 1 )
	{
		count = libewf_section_ltree_read( internal_handle, file_descriptor, (size_t) size );
	}
	/* Read the data section
	 */
	else if( ewf_section_is_type_data( section ) == 1 )
	{
		count = libewf_section_data_read( internal_handle, file_descriptor, (size_t) size );
	}
	/* Read the hash section
	 */
	else if( ewf_section_is_type_hash( section ) == 1 )
	{
		count = libewf_section_hash_read( internal_handle, file_descriptor, (size_t) size );
	}
	/* Read the xhash section
	 */
	else if( ewf_section_is_type_xhash( section ) == 1 )
	{
		count = libewf_section_xhash_read( internal_handle, file_descriptor, (size_t) size );
	}
	/* Read the error2 section
	 */
	else if( ewf_section_is_type_error2( section ) == 1 )
	{
		count = libewf_section_error2_read( internal_handle, file_descriptor, (size_t) size );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: unsupported section type: %s.\n", (char *) section->type );

#ifdef HAVE_DEBUG_OUTPUT
		LIBEWF_VERBOSE_EXEC( libewf_debug_read_section( internal_handle, file_descriptor, (size_t) size ); );
#else
		/* Skip the data within the section
		 */
		if( libewf_common_lseek( file_descriptor, size, SEEK_CUR ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_section_read: unable to align with next section.\n" );

			libewf_common_free( section );

			return( NULL );
		}
#endif
		count = (ssize_t) size;
	}
	if( count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: unable to read section: %s.\n", (char *) section->type );

		libewf_common_free( section );

		return( NULL );
	}
	*section_start_offset += (off_t) count;

	if( count != (ssize_t) size )
	{
		LIBEWF_WARNING_PRINT( "libewf_section_read: section: %s was not entirely read.\n", (char *) section->type );

		libewf_common_free( section );

		return( NULL );
	}
	return( section );
}

