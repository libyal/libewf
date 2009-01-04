/*
 * libewf section
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
#include "libewf_section.h"
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
#include "ewf_session.h"
#include "ewf_volume.h"
#include "ewf_volume_smart.h"
#include "ewf_table.h"

/* Reads a section start from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_start_read( int file_descriptor, EWF_SECTION *section, uint8_t error_tollerance )
{
	static char *function  = "libewf_section_start_read";
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;

	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section.\n",
		 function );

		return( -1 );
	}
	if( ewf_section_read( section, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read section start.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_EXEC( libewf_debug_section_fprint( stderr, section ); );

#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( section->padding, 40 ); );
#endif

	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) section, ( EWF_SECTION_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, section->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	return( (ssize_t) EWF_SECTION_SIZE );
}

/* Writes a section start to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_start_write( int file_descriptor, EWF_CHAR *section_type, size_t section_data_size, off64_t start_offset )
{
	EWF_SECTION section;

	static char *function    = "libewf_section_start_write";
	ssize_t write_count      = 0;
	size_t section_type_size = 0;
	uint64_t section_size    = 0;
	uint64_t section_offset  = 0;

	if( section_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section type.\n",
		 function );

		return( -1 );
	}
	section_type_size = ewf_string_length( section_type );

	if( section_type_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: section type is empty.\n",
		 function );

		return( -1 );
	}
	if( section_type_size >= 16 )
	{
		LIBEWF_WARNING_PRINT( "%s: section type is too long.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( &section, 0, EWF_SECTION_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear section.\n",
		 function );

		return( -1 );
	}
	/* Add one character for the end of string
	 */
	if( ewf_string_copy( section.type, section_type, ( section_type_size + 1 ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set section type.\n",
		 function );

		return( -1 );
	}
	section_size   = EWF_SECTION_SIZE + section_data_size;
	section_offset = start_offset + section_size;

	if( libewf_endian_revert_64bit( section_size, section.size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert size value.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( section_offset, section.next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert next offset value.\n",
		 function );

		return( -1 );
	}
	write_count = ewf_section_write( &section, file_descriptor );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Writes a compressed string section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_compressed_string_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset, EWF_CHAR *section_type, EWF_CHAR *uncompressed_string, size_t size, int8_t compression_level )
{
	EWF_CHAR *compressed_string = NULL;
	static char *function       = "libewf_section_compressed_string_write";
	ssize_t section_write_count = 0;
	ssize_t string_write_count  = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( section_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section type.\n",
		 function );

		return( -1 );
	}
	if( uncompressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid uncompressed string.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: string:\n",
	 function );

	if( ( uncompressed_string[ 0 ] == 0xff )
	 || ( uncompressed_string[ 0 ] == 0xfe ) )
	{
		LIBEWF_VERBOSE_EXEC( libewf_debug_header2_fprint( stderr, uncompressed_string, size ); );
	}
	else
	{
		LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, uncompressed_string, size ); );
	}
	compressed_string = ewf_string_compress( uncompressed_string, &size, compression_level );

	if( compressed_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to compress string.\n",
		 function );

		return( -1 );
	}
	section_write_count = libewf_section_start_write(
	                       file_descriptor,
	                       section_type,
	                       size,
	                       start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( compressed_string );

		return( -1 );
	}
	string_write_count = ewf_string_write_from_buffer( compressed_string, file_descriptor, size );

	libewf_common_free( compressed_string );

	if( string_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write string to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count + string_write_count );
}

/* Reads a header section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_header_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_HEADER *header    = NULL;
	static char *function = "libewf_section_header_read";
	ssize_t read_count    = (ssize_t) size;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	header = ewf_header_read( file_descriptor, &size );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read header.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, header, size ); );

	if( libewf_internal_handle_is_set_header( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_set_header( internal_handle, header, size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set header in handle.\n",
			 function );

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

/* Writes a header section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_header_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset, EWF_HEADER *header, size_t size, int8_t compression_level )
{
	ssize_t section_write_count = 0;

	section_write_count = libewf_section_compressed_string_write(
	                       internal_handle,
	                       file_descriptor,
	                       start_offset,
	                       (EWF_CHAR *) "header",
	                       header,
	                       size,
	                       compression_level );

	if( section_write_count != -1 )
	{
		internal_handle->amount_of_header_sections++;
	}
	return( section_write_count );
}

/* Reads a header2 section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_header2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_HEADER *header2   = NULL;
	static char *function = "libewf_section_header2_read";
	ssize_t read_count    = (ssize_t) size;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	header2 = ewf_header2_read( file_descriptor, &size );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read header2.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header2:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header2_fprint( stderr, header2, size ); );

	if( libewf_internal_handle_is_set_header2( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_set_header2( internal_handle, header2, size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set header2 in handle.\n",
			 function );

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

/* Writes a header2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_header2_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset, EWF_HEADER2 *header2, size_t size, int8_t compression_level )
{
	ssize_t section_write_count = 0;

	section_write_count = libewf_section_compressed_string_write(
	                       internal_handle,
	                       file_descriptor,
	                       start_offset,
	                       (EWF_CHAR *) "header2",
	                       header2,
	                       size,
	                       compression_level );

	if( section_write_count != -1 )
	{
		internal_handle->amount_of_header_sections++;
	}
	return( section_write_count );
}

/* Reads an EWF-S01 (SMART) volume section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_s01_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_VOLUME_SMART *volume_smart = NULL;
	static char *function          = "libewf_section_volume_s01_read";
	EWF_CRC calculated_crc         = 0;
	EWF_CRC stored_crc             = 0;
	int32_t bytes_per_chunk        = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	if( size != EWF_VOLUME_SMART_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: mismatch in section volume size.\n",
		 function );

		return( -1 );
	}
	volume_smart = (EWF_VOLUME_SMART *) libewf_common_alloc( EWF_VOLUME_SMART_SIZE );

	if( volume_smart == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate volume.\n",
		 function );

		return( -1 );
	}
	if( ewf_volume_smart_read( volume_smart, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume.\n",
		 function );

		libewf_common_free( volume_smart );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown2, 20 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( volume_smart->unknown3, 45 ); );
#endif

	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) volume_smart, ( EWF_VOLUME_SMART_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
		 function );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, volume_smart->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	bytes_per_chunk = ewf_volume_smart_calculate_chunk_size( volume_smart );

	if( bytes_per_chunk <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to calculate chunk size - using default.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume_smart );

			return( -1 );
		}
		bytes_per_chunk = EWF_MINIMUM_CHUNK_SIZE;
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume_smart );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->amount_of_chunks, volume_smart->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->sectors_per_chunk, volume_smart->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->bytes_per_sector, volume_smart->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert bytes per sector value.\n",
		 function );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->amount_of_sectors, volume_smart->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
		 function );

		libewf_common_free( volume_smart );

		return( -1 );
	}
	internal_handle->media->chunk_size = (uint32_t) bytes_per_chunk;

	LIBEWF_VERBOSE_PRINT( "%s: this volume has %" PRIu32 " chunks of %" PRIi32 " bytes each, CRC %" PRIu32 " (%" PRIu32 ").\n",
	 function, internal_handle->media->amount_of_chunks, bytes_per_chunk, stored_crc, calculated_crc );

	LIBEWF_VERBOSE_PRINT( "%s: this volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n",
	 function, internal_handle->media->amount_of_sectors, internal_handle->media->bytes_per_sector );

	if( libewf_common_memcmp( (void *) volume_smart->signature, (void *) "SMART", 5 ) == 0 )
	{
		internal_handle->format = LIBEWF_FORMAT_SMART;
	}
	else
	{
		internal_handle->format = LIBEWF_FORMAT_EWF;
	}
	libewf_common_free( volume_smart );

	return( (ssize_t) size );
}

/* Writes an EWF-S01 (SMART) volume section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_volume_s01_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset )
{
	EWF_VOLUME_SMART *volume    = NULL;
	static char *function       = "libewf_section_volume_s01_write";
	ssize_t section_write_count = 0;
	ssize_t volume_write_count  = 0;
	size_t size                 = EWF_VOLUME_SMART_SIZE;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	volume = (EWF_VOLUME_SMART *) libewf_common_alloc( size );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create volume.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( volume, 0, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear volume.\n" );

		libewf_common_free( volume );

		return( -1 );
	}
	volume->unknown1[ 0 ] = 1;

	if( libewf_endian_revert_32bit( internal_handle->media->amount_of_chunks, volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: amount_of_chunks: %" PRIu32 ", sectors_per_chunk: %" PRIu32 ", bytes_per_sector: %" PRIu32 ", amount_of_sectors: %" PRIu32 ".\n",
	 function, internal_handle->media->amount_of_chunks, internal_handle->media->sectors_per_chunk,
	 internal_handle->media->bytes_per_sector, internal_handle->media->amount_of_sectors );

	if( internal_handle->format == LIBEWF_FORMAT_SMART )
	{
		volume->signature[ 0 ] = (uint8_t) 'S';
		volume->signature[ 1 ] = (uint8_t) 'M';
		volume->signature[ 2 ] = (uint8_t) 'A';
		volume->signature[ 3 ] = (uint8_t) 'R';
		volume->signature[ 4 ] = (uint8_t) 'T';
	}
	section_write_count = libewf_section_start_write(
	                       file_descriptor,
	                       (EWF_CHAR *) "volume",
	                       size,
	                       start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	volume_write_count = ewf_volume_smart_write( volume, file_descriptor );

	libewf_common_free( volume );

	if( volume_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write volume to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count + volume_write_count );
}

/* Reads an EWF-E01 (EnCase) volume section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_e01_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_VOLUME *volume      = NULL;
	static char *function   = "libewf_section_volume_e01_read";
	EWF_CRC calculated_crc  = 0;
	EWF_CRC stored_crc      = 0;
	int32_t bytes_per_chunk = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	if( size != EWF_VOLUME_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: mismatch in section volume size.\n",
		 function );

		return( -1 );
	}
	volume = (EWF_VOLUME *) libewf_common_alloc( size );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume.\n",
		 function );

		return( -1 );
	}
	if( ewf_volume_read( volume, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
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
		LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, volume->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	bytes_per_chunk = ewf_volume_calculate_chunk_size( volume );

	if( bytes_per_chunk <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to calculate chunk size - using default.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume );

			return( -1 );
		}
		bytes_per_chunk = EWF_MINIMUM_CHUNK_SIZE;
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( volume );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->amount_of_chunks, volume->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &internal_handle->media->error_granularity, volume->error_granularity ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert error granularity value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	internal_handle->media->chunk_size  = (uint32_t) bytes_per_chunk;
	internal_handle->media->media_type  = volume->media_type;
	internal_handle->media->media_flags = volume->media_flags;
	internal_handle->compression_level  = (int8_t) volume->compression_level;

	LIBEWF_VERBOSE_PRINT( "%s: this volume has %" PRIu32 " chunks of %" PRIi32 " bytes each, CRC %" PRIu32 " (%" PRIu32 ").\n",
	 function, internal_handle->media->amount_of_chunks, bytes_per_chunk, stored_crc, calculated_crc );
	LIBEWF_VERBOSE_PRINT( "%s: this volume has %" PRIu32 " sectors of %" PRIi32 " bytes each.\n",
	 function, internal_handle->media->amount_of_sectors, internal_handle->media->bytes_per_sector );

	if( libewf_common_memcpy( internal_handle->guid, volume->guid, 16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
		 function );

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

	return( (ssize_t) size );
}

/* Reads a volume section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	static char *function = "libewf_section_volume_read";
	ssize_t count         = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

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
		LIBEWF_WARNING_PRINT( "%s: mismatch in section data size.\n",
		 function );

		return( -1 );
	}
	if( ( count <= -1 ) || ( count != (ssize_t) size ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read volume section.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->amount_of_chunks == 0 )
	{
		internal_handle->ewf_format = EWF_FORMAT_L01;
	}
	return( count );
}

/* Writes an EWF-E01 (EnCase) volume section to file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_volume_e01_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset )
{
	EWF_VOLUME *volume          = NULL;
	static char *function       = "libewf_section_volume_e01_write";
	ssize_t section_write_count = 0;
	ssize_t volume_write_count  = 0;
	size_t size                 = EWF_VOLUME_SIZE;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	volume = (EWF_VOLUME *) libewf_common_alloc( size );

	if( volume == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create volume.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( volume, 0, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear volume.\n",
		 function );

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
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->sectors_per_chunk, volume->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert sectors per chunk value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->bytes_per_sector, volume->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert bytes per sector value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( internal_handle->media->amount_of_sectors, volume->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: amount_of_chunks: %" PRIu32 ", sectors_per_chunk: %" PRIu32 ", bytes_per_sector: %" PRIu32 ", amount_of_sectors: %" PRIu32 ".\n",
	 function, internal_handle->media->amount_of_chunks, internal_handle->media->sectors_per_chunk, internal_handle->media->bytes_per_sector, internal_handle->media->amount_of_sectors );

	if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) )
	{
		volume->compression_level = (uint8_t) internal_handle->compression_level;

		if( libewf_common_memcpy( volume->guid, internal_handle->guid, 16 ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
			 function );

			libewf_common_free( volume );

			return( -1 );
		}
		if( libewf_endian_revert_32bit( internal_handle->media->error_granularity, volume->error_granularity ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert error granularity value.\n",
			 function );

			libewf_common_free( volume );

			return( -1 );
		}
	}
	section_write_count = libewf_section_start_write(
	                       file_descriptor,
	                       (EWF_CHAR *) "volume",
	                       size,
	                       start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( volume );

		return( -1 );
	}
	volume_write_count = ewf_volume_write( volume, file_descriptor );

	libewf_common_free( volume );

	if( volume_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write volume to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count + volume_write_count );
}

/* Reads an offset table from file
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_read( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, uint32_t *amount_of_chunks, int file_descriptor, uint16_t segment_number, size_t size, uint8_t ewf_format, uint8_t error_tollerance )
{
	EWF_TABLE *table          = NULL;
	EWF_TABLE_OFFSET *offsets = NULL;
	static char *function     = "libewf_offset_table_read";
	uint64_t base_offset      = 0;
	EWF_CRC calculated_crc    = 0;
	EWF_CRC stored_crc        = 0;

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section list.\n",
		 function );

		return( -1 );
	}
	if( amount_of_chunks == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of chunks.\n",
		 function );

		return( -1 );
	}
	table = (EWF_TABLE *) libewf_common_alloc( EWF_TABLE_SIZE );

	if( table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate table.\n",
		 function );

		return( -1 );
	}
	if( ewf_table_read( table, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read table.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	if( libewf_endian_convert_64bit( &base_offset, table->base_offset ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert base offset value.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( table->padding1, 4 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( table->padding2, 4 ); );
#endif

	/* The table size contains the size of the CRC (4 bytes)
	 */
	if( ewf_crc_calculate(
	     &calculated_crc,
	     (uint8_t *) table,
	     ( EWF_TABLE_SIZE - EWF_CRC_SIZE ),
	     1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, table->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( table );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( amount_of_chunks, table->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	libewf_common_free( table );

	LIBEWF_VERBOSE_PRINT( "%s: table is of size %" PRIu32 " chunks CRC %" PRIu32 " (%" PRIu32 ").\n",
	 function, *amount_of_chunks, stored_crc, calculated_crc );

	size -= EWF_TABLE_SIZE;

	if( *amount_of_chunks == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: table contains no offsets.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	else
	{
		/* Check if the maximum amount of offsets is not exceeded
		 */
		if( *amount_of_chunks > EWF_MAXIMUM_OFFSETS_IN_TABLE )
		{
			LIBEWF_WARNING_PRINT( "%s: table contains more than %d offsets!.\n",
			 function, EWF_MAXIMUM_OFFSETS_IN_TABLE );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
		offsets = (EWF_TABLE_OFFSET *) libewf_common_alloc( EWF_TABLE_OFFSET_SIZE * *amount_of_chunks );

		if( offsets == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocated table offsets.\n",
			 function );

			return( -1 );
		}
		if( ewf_table_offsets_read( offsets, file_descriptor, *amount_of_chunks ) <= -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read table offsets.\n",
			 function );

			libewf_common_free( offsets );

			return( -1 );
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
				LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
				 function );

				libewf_common_free( offsets );

				return( -1 );
			}
			if( ewf_crc_read( &stored_crc, file_descriptor ) != (int32_t) EWF_CRC_SIZE )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to read CRC from file descriptor.\n",
				 function );

				libewf_common_free( offsets );

				return( -1 );
			}
			if( stored_crc != calculated_crc )
			{
				LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
				 function, stored_crc, calculated_crc );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					libewf_common_free( offsets );

					return( -1 );
				}
			}
			size -= EWF_CRC_SIZE;
		}
		if( libewf_offset_table_fill(
		     offset_table,
		     (off64_t) base_offset,
		     offsets,
		     *amount_of_chunks,
		     file_descriptor,
		     segment_number,
		     error_tollerance ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to fill offset table.\n",
			 function );

			libewf_common_free( offsets );

			return( -1 );
		}
		libewf_common_free( offsets );

		if( libewf_offset_table_calculate_last_offset(
		     offset_table,
		     section_list,
		     file_descriptor,
		     segment_number,
		     error_tollerance ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to calculate last offset.\n",
			 function );

			return( -1 );
		}
	}
	/* Skip the chunk data within the section
	 * for chunks after the table section
	 */
	if( ( size > 0 )
	 && ( size <= (size_t) INT32_MAX ) )
	{
		if( libewf_common_lseek( file_descriptor, size, SEEK_CUR ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads a table section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_table_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size, LIBEWF_SECTION_LIST *section_list, uint16_t segment_number )
{
	static char *function     = "libewf_section_table_read";
	uint32_t amount_of_chunks = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section list.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		internal_handle->offset_table = libewf_offset_table_alloc(
		                                 internal_handle->media->amount_of_chunks );

		if( internal_handle->offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create offset table.\n",
			 function );

			return( -1 );
		}
	}
	if( libewf_offset_table_read(
	     internal_handle->offset_table,
	     section_list,
	     &amount_of_chunks,
	     file_descriptor,
	     segment_number,
	     size,
	     internal_handle->ewf_format,
	     internal_handle->error_tollerance ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read offset table.\n",
		 function );

		return( -1 );
	}
	internal_handle->segment_table->amount_of_chunks[ segment_number ] += amount_of_chunks;

	return( (ssize_t) size );
}

/* Writes a table or table2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_table_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset, off64_t base_offset, LIBEWF_OFFSET_TABLE *offset_table, uint32_t offset_table_index, uint32_t amount_of_offsets, EWF_CHAR *section_header, size_t additional_size )
{
	EWF_TABLE *table                  = NULL;
	EWF_TABLE_OFFSET *offsets         = NULL;
	static char *function             = "libewf_section_table_write";
	off64_t offset64_value            = 0;
	ssize_t section_write_count       = 0;
	ssize_t table_write_count         = 0;
	ssize_t table_offsets_write_count = 0;
	size_t size                       = 0;
	uint32_t offset32_value           = 0;
	uint32_t iterator                 = 0;
	uint8_t overflow                  = 0;
	uint8_t write_crc                 = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( base_offset <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid base offset.\n",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

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
		LIBEWF_WARNING_PRINT( "%s: unable to create table.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( table, 0, EWF_TABLE_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear table.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( amount_of_offsets, table->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( base_offset, table->base_offset ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert base offset value.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	offsets = (EWF_TABLE_OFFSET *) libewf_common_alloc( EWF_TABLE_OFFSET_SIZE * amount_of_offsets );

	if( offsets == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create offsets.\n",
		 function );

		libewf_common_free( table );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_offsets; iterator++ )
	{
		offset64_value = offset_table->offset[ offset_table_index + iterator ]
		               - base_offset;

		if( ( overflow == 0 )
		 && ( ( offset64_value < 0 )
		  || ( offset64_value > (off64_t) INT32_MAX ) ) )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid chunk offset value.\n",
			 function );

			libewf_common_free( table );
			libewf_common_free( offsets );

			return( -1 );
		}
		offset32_value = (uint32_t) offset64_value;

		if( offset_table->compressed[ offset_table_index + iterator ] != 0 )
		{
			if( overflow == 0 )
			{
				offset32_value |= EWF_OFFSET_COMPRESSED_WRITE_MASK;
			}
			else
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write compressed chunks after chunk overflow.\n",
				 function );

				libewf_common_free( table );
				libewf_common_free( offsets );

				return( -1 );
			}
		}
		if( libewf_endian_revert_32bit( offset32_value, (uint8_t *) offsets[ iterator ].offset ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert start offset.\n",
			 function );

			libewf_common_free( table );
			libewf_common_free( offsets );

			return( -1 );
		}
		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6
		 */
		if( ( overflow == 0 )
		 && ( ( offset64_value + offset_table->size[ offset_table_index + iterator ] ) > (off64_t) INT32_MAX ) )
		{
			if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
			 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 ) )
			{
				LIBEWF_VERBOSE_PRINT( "%s: chunk offset overflow at: %" PRIi64 ".\n",
				 function, offset64_value );

				overflow = 1;
			}
			else
			{
				LIBEWF_WARNING_PRINT( "%s: chunk offset overflow at: %" PRIi64 ".\n",
				 function, offset64_value );

				libewf_common_free( table );
				libewf_common_free( offsets );

				return( -1 );
			}
		}
	}
	section_write_count = libewf_section_start_write(
	                       file_descriptor,
	                       section_header,
	                       size,
	                       start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

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
		LIBEWF_WARNING_PRINT( "%s: unable to write table to file.\n",
		 function );

		return( -1 );
	}
	if( table_offsets_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write table offsets to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count + table_write_count + table_offsets_write_count );
}

/* Reads a table2 section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_table2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size, LIBEWF_SECTION_LIST *section_list, uint16_t segment_number )
{
	static char *function     = "libewf_section_table2_read";
	uint32_t amount_of_chunks = 0;
	int result                = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section list.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->secondary_offset_table == NULL )
	{
		internal_handle->secondary_offset_table = libewf_offset_table_alloc( internal_handle->media->amount_of_chunks );

		if( internal_handle->secondary_offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create secondairy offset table.\n",
			 function );

			return( -1 );
		}
	}
	if( libewf_offset_table_read(
	     internal_handle->secondary_offset_table,
	     section_list,
	     &amount_of_chunks,
	     file_descriptor,
	     segment_number,
	     size,
	     internal_handle->ewf_format,
	     internal_handle->error_tollerance ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read offset table.\n",
		 function );

		return( -1 );
	}
	result = libewf_offset_table_compare(
	          internal_handle->offset_table,
	          internal_handle->secondary_offset_table );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to compare table1 and table2.\n",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: table1 and table2 differ.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
		/* TODO Try to correct the table
		 */
	}
	return( (ssize_t) size );
}

/* Reads a sectors section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize64_t libewf_section_sectors_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size64_t size )
{
	static char *function = "libewf_section_sectors_read";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	/* In the EWF-E01 format the sectors section holds the actual data chunks
	 */
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain sectors section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	/* Skip the chunk data within the section
	 */
	if( libewf_common_lseek( file_descriptor, size, SEEK_CUR ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
		 function );

		return( -1 );
	}
	return( (ssize64_t) size );
}

/* Reads a ltree section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_ltree_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_LTREE *ltree          = NULL;
	EWF_LTREE_DATA *tree_data = NULL;
	static char *function     = "libewf_section_ltree_read";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain ltree section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	internal_handle->ewf_format = EWF_FORMAT_L01;

	ltree = (EWF_LTREE *) libewf_common_alloc( EWF_LTREE_SIZE );

	if( ltree == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate ltree.\n",
		 function );

		return( -1 );
	}
	if( ewf_ltree_read( ltree, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read ltree.\n",
		 function );

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

	return( (ssize_t) size );
}

/* Reads a session section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_session_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_SESSION *session  = NULL;
	static char *function = "libewf_section_session_read";
	EWF_CRC calculated_crc         = 0;
	EWF_CRC stored_crc             = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( size != EWF_SESSION_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: mismatch in section session size.\n",
		 function );

		return( -1 );
	}
	session = (EWF_SESSION *) libewf_common_alloc( EWF_SESSION_SIZE );

	if( session == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate session.\n",
		 function );

		return( -1 );
	}
	if( ewf_session_read( session, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read session.\n",
		 function );

		libewf_common_free( session );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( session->unknown, 68 ); );
#endif

	if( ewf_crc_calculate(
	     &calculated_crc,
	     (uint8_t *) session,
	     ( EWF_SESSION_SIZE - EWF_CRC_SIZE ),
	     1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
		 function );

		libewf_common_free( session );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, session->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( session );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( session );

			return( -1 );
		}
	}
	libewf_common_free( session );

	return( (ssize_t) size );
}

/* Reads a data section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_data_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_DATA *data             = NULL;
	static char *function      = "libewf_section_data_read";
	EWF_CRC calculated_crc     = 0;
	EWF_CRC stored_crc         = 0;
	uint32_t amount_of_chunks  = 0;
	uint32_t sectors_per_chunk = 0;
	uint32_t bytes_per_sector  = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t error_granularity = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	if( size != EWF_DATA_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: mismatch in section data size.\n",
		 function );

		return( -1 );
	}
	data = (EWF_DATA *) libewf_common_alloc( EWF_DATA_SIZE );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create data.\n",
		 function );

		return( -1 );
	}
	if( ewf_data_read( data, file_descriptor ) < -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read data.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
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
		LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, data->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

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
		LIBEWF_WARNING_PRINT( "%s: media type does not match in data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &amount_of_chunks, data->amount_of_chunks ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of chunks value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->amount_of_chunks != amount_of_chunks )
	{
		LIBEWF_WARNING_PRINT( "%s: amount of chunks does not match in data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &sectors_per_chunk, data->sectors_per_chunk ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert sectors per chunk value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->sectors_per_chunk != sectors_per_chunk )
	{
		LIBEWF_WARNING_PRINT( "%s: sectors per chunk does not match in data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &bytes_per_sector, data->bytes_per_sector ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert bytes per sector value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->bytes_per_sector != bytes_per_sector )
	{
		LIBEWF_WARNING_PRINT( "%s: bytes per sector does not match in data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &amount_of_sectors, data->amount_of_sectors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->amount_of_sectors != amount_of_sectors )
	{
		LIBEWF_WARNING_PRINT( "%s: amount of sectors does not match in data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_endian_convert_32bit( &error_granularity, data->error_granularity ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert error granularity value.\n",
		 function );

		libewf_common_free( data );

		return( -1 );
	}
	if( internal_handle->media->error_granularity != error_granularity )
	{
		LIBEWF_WARNING_PRINT( "%s: error granularity does not match in data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( internal_handle->media->media_flags != data->media_flags )
	{
		LIBEWF_WARNING_PRINT( "%s: media flags do not match in data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	if( libewf_common_memcmp( internal_handle->guid, data->guid, 16 ) != 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: GUID does not match in data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( data );

			return( -1 );
		}
	}
	libewf_common_free( data );

	return( (ssize_t) size );
}

/* Writes a data section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_data_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset )
{
	static char *function       = "libewf_section_data_write";
	ssize_t section_write_count = 0;
	ssize_t data_write_count    = 0;
	size_t size                 = EWF_DATA_SIZE;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( -1 );
	}
	/* Check if the data section was already created
	 */
	if( internal_handle->write->data_section == NULL )
	{
		internal_handle->write->data_section = (EWF_DATA *) libewf_common_alloc( size );

		if( internal_handle->write->data_section == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create data.\n",
			 function );

			return( -1 );
		}
		if( libewf_common_memset( internal_handle->write->data_section, 0, size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to clear data.\n",
			 function );

			return( -1 );
		}
		if( internal_handle->format == LIBEWF_FORMAT_FTK )
		{
			internal_handle->write->data_section->media_type = 0x01;
		}
		else
		{
			internal_handle->write->data_section->media_type = internal_handle->media->media_type;
		}
		internal_handle->write->data_section->media_flags = internal_handle->media->media_flags;

		if( libewf_endian_revert_32bit(
		     internal_handle->media->amount_of_chunks,
		     internal_handle->write->data_section->amount_of_chunks ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert amount of chunks value.\n",
			 function );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     internal_handle->media->sectors_per_chunk,
		     internal_handle->write->data_section->sectors_per_chunk ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert sectors per chunk value.\n",
			 function );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     internal_handle->media->bytes_per_sector,
		     internal_handle->write->data_section->bytes_per_sector ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert bytes per sector value.\n",
			 function );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     internal_handle->media->amount_of_sectors,
		     internal_handle->write->data_section->amount_of_sectors ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
			 function );

			return( -1 );
		}
		if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
		 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
		 || ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
		 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
		 || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) )
		{
			internal_handle->write->data_section->compression_level = (uint8_t) internal_handle->compression_level;

			if( libewf_common_memcpy(
			     internal_handle->write->data_section->guid,
			     internal_handle->guid,
			     16 ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
				 function );

				return( -1 );
			}
			if( libewf_endian_revert_32bit(
			     internal_handle->media->error_granularity,
			     internal_handle->write->data_section->error_granularity ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to revert error granularity value.\n",
				 function );

				return( -1 );
			}
		}
	}
	section_write_count = libewf_section_start_write(
	                       file_descriptor,
	                       (EWF_CHAR *) "data",
	                       size,
	                       start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		return( -1 );
	}
	data_write_count = ewf_data_write( internal_handle->write->data_section, file_descriptor );

	if( data_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write data to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count + data_write_count );
}

/* Reads a error2 section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_error2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_ERROR2 *error2                = NULL;
	EWF_ERROR2_SECTOR *error2_sectors = NULL;
	static char *function             = "libewf_section_error2_read";
	EWF_CRC calculated_crc            = 0;
	EWF_CRC stored_crc                = 0;
	uint32_t amount_of_errors         = 0;
	uint32_t iterator                 = 0;
	uint32_t sector                   = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		LIBEWF_WARNING_PRINT( "%s: EWF-S01 format should not contain data section.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	error2 = (EWF_ERROR2 *) libewf_common_alloc( EWF_ERROR2_SIZE );

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate error2.\n",
		 function );

		return( -1 );
	}
	if( ewf_error2_read( error2, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read error2.\n",
		 function );

		libewf_common_free( error2 );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( error2->unknown, 200 ); );
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( (uint8_t *) error2_sectors, ( EWF_ERROR2_SECTOR_SIZE * amount_of_errors ) ); );
#endif

	if( ewf_crc_calculate( &calculated_crc, (uint8_t *) error2, ( EWF_ERROR2_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
		 function );

		libewf_common_free( error2 );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, error2->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( error2 );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &amount_of_errors, error2->amount_of_errors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert amount of errors value.\n",
		 function );

		libewf_common_free( error2 );

		return( -1 );
	}
	libewf_common_free( error2 );

	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	if( amount_of_errors == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: error2 contains no sectors!.\n",
		 function );

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
			LIBEWF_WARNING_PRINT( "%s: unable to allocate error2 sectors.\n",
			 function );

			return( -1 );
		}
		if( ewf_error2_sectors_read( error2_sectors, file_descriptor, amount_of_errors ) <= -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read error2 sectors.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( ewf_crc_calculate(
		     &calculated_crc,
		     (uint8_t *) error2_sectors,
		     ( EWF_ERROR2_SECTOR_SIZE * amount_of_errors ), 1 ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( ewf_crc_read( &stored_crc, file_descriptor ) != EWF_CRC_SIZE )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read CRC from file descriptor.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( stored_crc != calculated_crc )
		{
			LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
			 function, stored_crc, calculated_crc );

			if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				libewf_common_free( error2_sectors );

				return( -1 );
			}
		}
		if( internal_handle->acquiry_error_sectors != NULL )
		{
			LIBEWF_VERBOSE_PRINT( "%s: acquiry error sectors already set in handle - removing previous one.\n",
			 function );

			libewf_common_free( internal_handle->acquiry_error_sectors );
		}
		internal_handle->acquiry_error_sectors = (LIBEWF_ERROR_SECTOR *) libewf_common_alloc( LIBEWF_ERROR_SECTOR_SIZE * amount_of_errors );

		if( internal_handle->acquiry_error_sectors == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create acquiry error sectors.\n",
			 function );

			libewf_common_free( error2_sectors );

			return( -1 );
		}
		internal_handle->acquiry_amount_of_errors = amount_of_errors;

		for( iterator = 0; iterator < amount_of_errors; iterator++ )
		{
			if( libewf_endian_convert_32bit( &sector, error2_sectors[ iterator ].sector ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to convert sector value.\n",
				 function );

				libewf_common_free( error2_sectors );

				return( -1 );
			}
			internal_handle->acquiry_error_sectors[ iterator ].sector = (uint64_t) sector;

			if( libewf_endian_convert_32bit(
			     &internal_handle->acquiry_error_sectors[ iterator ].amount_of_sectors,
			     error2_sectors[ iterator ].amount_of_sectors ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to convert amount of sectors value.\n",
				 function );

				libewf_common_free( error2_sectors );

				return( -1 );
			}
		}
		libewf_common_free( error2_sectors );
	}
	return( (ssize_t) size );
}

/* Writes a error2 section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_error2_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset, LIBEWF_ERROR_SECTOR *sectors, uint32_t amount_of_errors )
{
	EWF_ERROR2 *error2                 = NULL;
	EWF_ERROR2_SECTOR *error2_sectors  = NULL;
	static char *function              = "libewf_section_error2_write";
	ssize_t section_write_count        = 0;
	ssize_t error2_write_count         = 0;
	ssize_t error2_sectors_write_count = 0;
	size_t size                        = 0;
	uint32_t iterator                  = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid acquiry read error sectors.\n",
		 function );

		return( -1 );
	}
	size   = EWF_ERROR2_SIZE + ( EWF_ERROR2_SECTOR_SIZE * amount_of_errors ) + EWF_CRC_SIZE;
	error2 = (EWF_ERROR2 *) libewf_common_alloc( EWF_ERROR2_SIZE );

	if( error2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create error2.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( error2, 0, EWF_ERROR2_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear error2.\n",
		 function );

		libewf_common_free( error2 );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( amount_of_errors, error2->amount_of_errors ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert amount of errors value.\n",
		 function );

		libewf_common_free( error2 );

		return( -1 );
	}
	error2_sectors = (EWF_ERROR2_SECTOR *) libewf_common_alloc( EWF_ERROR2_SECTOR_SIZE * amount_of_errors );

	if( error2_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create error2 sectors.\n",
		 function );

		libewf_common_free( error2 );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_errors; iterator++ )
	{
		if( libewf_endian_revert_32bit(
		     (uint32_t) sectors[ iterator ].sector,
		     error2_sectors[ iterator ].sector ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert sector value.\n",
			 function );

			libewf_common_free( error2 );
			libewf_common_free( error2_sectors );

			return( -1 );
		}
		if( libewf_endian_revert_32bit(
		     sectors[ iterator ].amount_of_sectors,
		     error2_sectors[ iterator ].amount_of_sectors ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to revert amount of sectors value.\n",
			 function );

			libewf_common_free( error2 );
			libewf_common_free( error2_sectors );

			return( -1 );
		}
	}
	section_write_count = libewf_section_start_write(
	                       file_descriptor,
	                       (EWF_CHAR *) "error2",
	                       size,
	                       start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

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
		LIBEWF_WARNING_PRINT( "%s: unable to write error2 to file.\n",
		 function );

		return( -1 );
	}
	if( error2_sectors_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write error2 sectors to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count + error2_write_count + error2_sectors_write_count );
}

/* Reads a hash section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_hash_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_HASH *hash         = NULL;
	static char *function  = "libewf_section_hash_read";
	EWF_CRC calculated_crc = 0;
	EWF_CRC stored_crc     = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( size != EWF_HASH_SIZE )
	{
		LIBEWF_WARNING_PRINT( "%s: mismatch in section data size.\n",
		 function );

		return( -1 );
	}
	hash = (EWF_HASH *) libewf_common_alloc( EWF_HASH_SIZE );

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate hash.\n",
		 function );

		return( -1 );
	}
	if( ewf_hash_read( hash, file_descriptor ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read hash.\n",
		 function );

		libewf_common_free( hash );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	LIBEWF_VERBOSE_EXEC( libewf_dump_data( hash->unknown1, 16 ); );
#endif

	if( ewf_crc_calculate(
	     &calculated_crc,
	     (uint8_t *) hash,
	     ( EWF_HASH_SIZE - EWF_CRC_SIZE ), 1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to calculate CRC.\n",
		 function );

		libewf_common_free( hash );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &stored_crc, hash->crc ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert stored CRC value.\n",
		 function );

		libewf_common_free( hash );

		return( -1 );
	}
	if( stored_crc != calculated_crc )
	{
		LIBEWF_WARNING_PRINT( "%s: CRC does not match (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n",
		 function, stored_crc, calculated_crc );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( hash );

			return( -1 );
		}
	}
	if( libewf_common_memcpy( internal_handle->md5_hash, hash->md5_hash, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set MD5 hash in handle.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			libewf_common_free( hash );

			return( -1 );
		}
	}
	internal_handle->md5_hash_set = 1;

	libewf_common_free( hash );

	return( (ssize_t) size );
}

/* Writes a hash section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_hash_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset, EWF_DIGEST_HASH *md5_hash )
{
	EWF_HASH *hash              = NULL;
	static char *function       = "libewf_section_hash_write";
	ssize_t section_write_count = 0;
	ssize_t hash_write_count    = 0;
	size_t size                 = EWF_HASH_SIZE;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	hash = (EWF_HASH *) libewf_common_alloc( size );

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create hash.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( hash, 0, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear hash.\n",
		 function );

		libewf_common_free( hash );

		return( -1 );
	}
	if( libewf_common_memcpy( hash->md5_hash, md5_hash, EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash.\n",
		 function );

		libewf_common_free( hash );

		return( -1 );
	}
	section_write_count = libewf_section_start_write(
	                       file_descriptor,
	                       (EWF_CHAR *) "hash",
	                       size,
	                       start_offset );

	if( section_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		libewf_common_free( hash );

		return( -1 );
	}
	hash_write_count = ewf_hash_write( hash, file_descriptor );

	libewf_common_free( hash );

	if( hash_write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write hash to file.\n",
		 function );

		return( -1 );
	}
	return( section_write_count + hash_write_count );
}

/* Writes the last section start to file
 * This is used for the next and done sections, these sections point back towards themselves
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_last_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, EWF_CHAR *section_type, off64_t start_offset )
{
	EWF_SECTION *section     = NULL;
	static char *function    = "libewf_section_last_write";
	size_t section_type_size = 0;
	ssize_t write_count      = 0;
	uint64_t section_size    = 0;
	uint64_t section_offset  = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( section_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section type.\n",
		 function );

		return( -1 );
	}
	section = (EWF_SECTION *) libewf_common_alloc( EWF_SECTION_SIZE );

	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create section.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memset( section, 0, EWF_SECTION_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear section.\n",
		 function );

		libewf_common_free( section );

		return( -1 );
	}
	/* The EnCase (EWF-E01) format leaves the size of this section empty
	 */
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( internal_handle->format == LIBEWF_FORMAT_FTK ) )
	{
		section_size = EWF_SECTION_SIZE;
	}
	section_type_size = ewf_string_length( section_type );
	section_offset    = start_offset;

	if( section_type_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: section type is empty.\n",
		 function );

		return( -1 );
	}
	if( section_type_size >= 16 )
	{
		LIBEWF_WARNING_PRINT( "%s: section type is too long.\n",
		 function );

		libewf_common_free( section );

		return( -1 );
	}
	/* Add one character for the end of string
	 */
	if( ewf_string_copy( section->type, section_type, ( section_type_size + 1 ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set section type.\n",
		 function );

		libewf_common_free( section );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( section_size, section->size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert size value.\n",
		 function );

		libewf_common_free( section );

		return( -1 );
	}
	if( libewf_endian_revert_64bit( section_offset, section->next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert next offset value.\n",
		 function );

		libewf_common_free( section );

		return( -1 );
	}
	write_count = ewf_section_write( section, file_descriptor );

	libewf_common_free( section );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Reads a xheader section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_xheader_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_CHAR *xheader     = NULL;
	static char *function = "libewf_section_xheader_read";
	ssize_t read_count    = (ssize_t) size;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	xheader = ewf_string_read_compressed( file_descriptor, &size );

	if( xheader == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read xheader.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Header:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xheader, size ); );

	if( libewf_internal_handle_is_set_xheader( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_set_xheader( internal_handle, xheader, size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set xheader in handle.\n",
			 function );

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

/* Writes a xheader section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_xheader_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off64_t start_offset, EWF_CHAR *xheader, size_t size, int8_t compression_level )
{
	ssize_t section_write_count = 0;

	section_write_count = libewf_section_compressed_string_write(
	                       internal_handle,
	                       file_descriptor,
	                       start_offset,
	                       (EWF_CHAR *) "xheader",
	                       xheader,
	                       size,
	                       compression_level );

	if( section_write_count != -1 )
	{
		internal_handle->amount_of_header_sections++;
	}
	return( section_write_count );
}

/* Reads a xhash section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_xhash_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size )
{
	EWF_CHAR *xhash       = NULL;
	static char *function = "libewf_section_xhash_read";
	ssize_t read_count    = (ssize_t) size;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	xhash = ewf_string_read_compressed( file_descriptor, &size );

	if( xhash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read xhash.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: Hash:\n",
	 function );

	LIBEWF_VERBOSE_EXEC( libewf_debug_header_fprint( stderr, xhash, size ); );

	if( libewf_internal_handle_is_set_xhash( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_set_xhash( internal_handle, xhash, size ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set xhash in handle.\n",
			 function );

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

/* Reads a delta chunk section from file
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_section_delta_chunk_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size, uint16_t segment_number, off64_t start_offset )
{
	EWF_CHAR buffer[ 4 ];

	static char *function = "libewf_section_delta_chunk_read";
	ssize_t read_count    = (ssize_t) size;
	uint32_t chunk        = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	read_count = ewf_string_read_to_buffer( buffer, file_descriptor, sizeof( uint32_t ) );

	if( read_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read chunk value from file.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_convert_32bit( &chunk, buffer ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert chunk value.\n",
		 function );

		return( -1 );
	}
	/* Skip the chunk data within the section
	 */
	if( libewf_common_lseek( file_descriptor, ( size - sizeof( uint32_t ) ), SEEK_CUR ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
		 function );

		return( -1 );
	}
	/* Update the chunk data in the offset table
	 */
	if( libewf_offset_table_set_values(
	     internal_handle->offset_table,
	     chunk,
	     file_descriptor,
	     0,
	     (off64_t) ( start_offset + sizeof( uint32_t ) ),
	     (size_t) ( size - sizeof( uint32_t ) ),
	     segment_number,
	     1 ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set value in offset table.\n",
		 function );

		if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
	return( (ssize_t) size );
}

/* Writes a delta chunk section to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_section_delta_chunk_write( int file_descriptor, off64_t start_offset, uint32_t chunk, EWF_CHUNK *chunk_data, size_t chunk_size, EWF_CRC *chunk_crc )
{
	EWF_CHAR buffer[ 4 ];

	static char *function       = "libewf_section_delta_chunk_write";
	ssize_t section_write_count = 0;
	ssize_t write_count         = 0;

	if( chunk_size > (size_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_revert_32bit( chunk, buffer ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to revert chunk value.\n" );

		return( -1 );
	}
	write_count = libewf_section_start_write(
	               file_descriptor,
	               (EWF_CHAR *) "delta_chunk",
	               ( sizeof( uint32_t ) + chunk_size + EWF_CRC_SIZE ),
	               start_offset );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write section to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = ewf_string_write_from_buffer( buffer, file_descriptor, sizeof( uint32_t ) );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write chunk value to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = ewf_chunk_write( chunk_data, file_descriptor, chunk_size );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write chunk data to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	write_count = ewf_crc_write( chunk_crc, file_descriptor );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write CRC to file.\n",
		 function );

		return( -1 );
	}
	section_write_count += write_count;

	return( section_write_count );
}

/* Reads and processes a section
 * The section start offset will be updated
 * Returns 1 if successful, -1 on error
 */
int libewf_section_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, EWF_SECTION *section, LIBEWF_SECTION_LIST *section_list, uint16_t segment_number, off64_t *section_start_offset )
{
	static char *function      = "libewf_section_read";
	off64_t section_end_offset = 0;
	ssize64_t count            = 0;
	uint64_t size              = 0;
	uint64_t next_offset       = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( section == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section start.\n",
		 function );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section list.\n",
		 function );

		return( -1 );
	}
	if( section_start_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section start offset.\n",
		 function );

		return( -1 );
	}
	if( *section_start_offset > (off64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section start offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_section_start_read( file_descriptor, section, internal_handle->error_tollerance ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read section start.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_convert_64bit( &size, section->size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert size value.\n",
		 function );

		return( -1 );
	}
	if( size > (uint64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_endian_convert_64bit( &next_offset, section->next ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert next offset value.\n",
		 function );

		return( -1 );
	}
	if( next_offset > (uint64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid next offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	section_end_offset = *section_start_offset + (off64_t) size;

	if( section_end_offset > (off64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section end offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_section_list_append(
	     section_list,
	     section->type,
	     *section_start_offset,
	     section_end_offset ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to append value to section list.\n",
		 function );

		return( -1 );
	}
	*section_start_offset += EWF_SECTION_SIZE;

	/* No need to correct empty sections like done and next
	 */
	if( size > 0 )
	{
		size -= EWF_SECTION_SIZE;
	}
	if( size > (uint64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}

	/* Nothing to do for the next and done section
	 */
	if( ( ewf_section_is_type_next( section ) == 1 )
	 || ( ewf_section_is_type_done( section ) == 1 ) )
	{
		/* Determine the size of the largest segment file for read and write mode only
		 */
		if( ( internal_handle->write != NULL )
		 && ( (off64_t) internal_handle->write->segment_file_size < ( *section_start_offset + (off64_t) EWF_SECTION_SIZE ) ) )
		{
			internal_handle->write->segment_file_size = (size64_t) ( *section_start_offset + EWF_SECTION_SIZE );
		}
	}
	/* Read the header2 section
	 */
	else if( ewf_section_is_type_header2( section ) == 1 )
	{
		count = libewf_section_header2_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the header section
	 */
	else if( ewf_section_is_type_header( section ) == 1 )
	{
		count = libewf_section_header_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the xheader section
	 */
	else if( ewf_section_is_type_xheader( section ) == 1 )
	{
		count = libewf_section_xheader_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the volume or disk section
	 */
	else if( ( ewf_section_is_type_volume( section ) == 1 )
	 || ( ewf_section_is_type_disk( section ) == 1 ) )
	{
		count = libewf_section_volume_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the table2 section
	 */
	else if( ewf_section_is_type_table2( section ) == 1 )
	{
		count = libewf_section_table2_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size,
		         section_list,
		         segment_number );
	}
	/* Read the table section
	 */
	else if( ewf_section_is_type_table( section ) == 1 )
	{
		count = libewf_section_table_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size,
		         section_list,
		         segment_number );
	}
	/* Read the sectors section
	 */
	else if( ewf_section_is_type_sectors( section ) == 1 )
	{
		count = libewf_section_sectors_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the dtchunk section
	 */
	else if( ewf_section_is_type_delta_chunk( section ) == 1 )
	{
		count = libewf_section_delta_chunk_read(
		         internal_handle,
 		         file_descriptor,
 		         (size_t) size,
 		         segment_number,
 		         *section_start_offset );
	}
	/* Read the ltree section
	 */
	else if( ewf_section_is_type_ltree( section ) == 1 )
	{
		count = libewf_section_ltree_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the session section
	 */
	else if( ewf_section_is_type_session( section ) == 1 )
	{
		count = libewf_section_session_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the data section
	 */
	else if( ewf_section_is_type_data( section ) == 1 )
	{
		count = libewf_section_data_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the hash section
	 */
	else if( ewf_section_is_type_hash( section ) == 1 )
	{
		count = libewf_section_hash_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the xhash section
	 */
	else if( ewf_section_is_type_xhash( section ) == 1 )
	{
		count = libewf_section_xhash_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	/* Read the error2 section
	 */
	else if( ewf_section_is_type_error2( section ) == 1 )
	{
		count = libewf_section_error2_read(
		         internal_handle,
		         file_descriptor,
		         (size_t) size );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported section type: %s.\n",
		 function, (char *) section->type );

#if defined( HAVE_DEBUG_OUTPUT )
		if( size > SSIZE_MAX )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
		LIBEWF_VERBOSE_EXEC( libewf_debug_read_section( internal_handle, file_descriptor, (size_t) size ); );
#else
		/* Skip the data within the section
		 */
		if( libewf_common_lseek( file_descriptor, size, SEEK_CUR ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to align with next section.\n",
			 function );

			return( -1 );
		}
#endif
		count = (ssize64_t) size;
	}
	if( count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read section: %s.\n",
		 function, (char *) section->type );

		return( -1 );
	}
	*section_start_offset += (off64_t) count;

	if( count != (ssize64_t) size )
	{
		LIBEWF_WARNING_PRINT( "%s: section: %s was not entirely read.\n",
		 function, (char *) section->type );

		return( -1 );
	}
	return( 1 );
}

