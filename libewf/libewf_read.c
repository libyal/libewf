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
#include "libewf_endian.h"
#include "libewf_notify.h"
#include "libewf_file.h"
#include "libewf_read.h"
#include "libewf_section.h"
#include "libewf_segment_table.h"

#include "ewf_crc.h"
#include "ewf_file_header.h"

/* Builds the index (section list and offset table) from the input files
 * Returns 1 if successful, or -1 on error
 */
int8_t libewf_read_build_index( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	EWF_SECTION *last_section = NULL;
	uint16_t segment_number   = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_build_index: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->index_build != 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_build_index: index has already been build.\n" );

		return( -1 );
	}
	if( libewf_internal_handle_read_initialize( internal_handle ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_build_index: unable to initialize read values in handle.\n" );

		return( -1 );
	}
	for( segment_number = 1; segment_number < internal_handle->segment_table->amount; segment_number++ )
	{
		if( last_section != NULL )
		{
			libewf_common_free( last_section );
		}
		LIBEWF_VERBOSE_PRINT( "libewf_read_build_index: building index for segment number: %" PRIu32 ".\n", segment_number );

		last_section = libewf_read_sections_from_segment( internal_handle, segment_number );
	}
	/* Check to see if the done section has been found
	 */
	if( last_section == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_build_index: invalid last section.\n" );

		return( -1 );
	}
	else if( ewf_section_is_type_done( last_section ) == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_build_index: unable to find the last segment file (done section).\n" );

		libewf_common_free( last_section );

		return( -1 );
	}
	libewf_common_free( last_section );

	/* Determine the EWF file format
	 */
	if( libewf_internal_handle_determine_format( internal_handle ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_build_index: unable to determine file format.\n" );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_read_build_index: index successful build.\n" );

	/* Calculate the media size
	 */
	internal_handle->media->media_size = (uint64_t) internal_handle->media->amount_of_sectors * (uint64_t) internal_handle->media->bytes_per_sector;

	/* Flag that the index was build
	 */
	internal_handle->index_build = 1;

	return( 1 );
}

/* Reads and processes sections of a segment file
 * Returns the last section read, or NULL on error
 */
EWF_SECTION *libewf_read_sections_from_segment( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number )
{
	EWF_SECTION *section = NULL;
	int file_descriptor  = 0;

	/* The first offset is directly after the file header (13 byte)
	 */
	off_t previous_offset = (off_t) EWF_FILE_HEADER_SIZE;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_sections_read_segment: invalid handle.\n" );

		return( NULL );
	}
	if( libewf_segment_table_values_is_set( internal_handle->segment_table, segment_number ) == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_sections_read_segment: missing a segment file for segment %" PRIu32 ".\n", segment_number );

		return( NULL );
	}
	file_descriptor = libewf_segment_table_get_file_descriptor( internal_handle->segment_table, segment_number );

	if( file_descriptor <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_sections_read_segment: invalid file descriptor.\n" );

		return( NULL );
	}
	do
	{
		if( section != NULL )
		{
			libewf_common_free( section );
		}
		section = libewf_section_read( internal_handle, file_descriptor, internal_handle->segment_table->section_list[ segment_number ], segment_number, &previous_offset );

		if( section == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_sections_read_segment: unable to read section.\n" );

			return( NULL );
		}
		/* The done and next sections point back at themselves
		 */
		if( ( ewf_section_is_type_next( section ) == 1 ) || ( ewf_section_is_type_done( section ) == 1 ) )
		{
			break;
		}
	}
	while( section != NULL );

	return( section );
}

/* Reads a certain chunk of data from the segment file(s)
 * Will read until the requested size is filled or the entire chunk is read
 * This function swaps byte pairs if specified
 * Returns the amount of bytes read, 0 if no bytes can be read, or -1 on error
 */
ssize_t libewf_read_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk, uint32_t chunk_offset, void *buffer, size_t size )
{
	EWF_CHUNK *chunk_data       = NULL;
	EWF_CHUNK *chunk_read       = NULL;
	EWF_CRC calculated_crc      = 0;
	EWF_CRC stored_crc          = 0;
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	wchar_t *filename           = NULL;
#else
	char *filename              = NULL;
#endif
	ssize_t chunk_read_count    = 0;
	ssize_t crc_read_count      = 0;
	size_t chunk_data_size      = 0;
	size_t bytes_available      = 0;
	size_t md5_hash_size        = 0;
	uint16_t segment_number     = 0;
	int8_t chunk_cache_used     = 0;
	int8_t result               = 0;
	uint8_t percentage          = 0;
	int file_descriptor         = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: invalid handle - missing subhandle read.\n" );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: invalid handle - missing offset table.\n" );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: invalid handle - missing segment table.\n" );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: invalid chunk cache.\n" );

		return( -1 );
	}
	if( internal_handle->index_build == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: index was not build.\n" );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: invalid buffer.\n" );

		return( -1 );
	}
	if( buffer == internal_handle->chunk_cache->compressed )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: invalid buffer - same as chunk cache compressed.\n" );

		return( -1 );
	}
	/* Check if the chunk is available
	 */
	if( chunk >= internal_handle->offset_table->amount )
	{
		return( 0 );
	}
	/* Check if the chunk is cached
	 */
	if( ( internal_handle->chunk_cache->chunk != chunk ) || ( internal_handle->chunk_cache->cached == 0 ) )
	{
		file_descriptor = internal_handle->offset_table->file_descriptor[ chunk ];
		segment_number  = internal_handle->offset_table->segment_number[ chunk ];

		/* Check if chunk cache passthrough is used
		 * if the chunk cache is used as the chunk data buffer
		 */
		chunk_cache_used = (int8_t) ( buffer == internal_handle->chunk_cache->data );

		/* Determine the size of the chunk including the CRC
		 */
		chunk_data_size = internal_handle->offset_table->size[ chunk ];

		/* Make sure the chunk cache is large enough
		 */
		if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_read_chunk: reallocating chunk data size: %zu.\n", chunk_data_size );

			if( libewf_internal_handle_chunk_cache_realloc( internal_handle, chunk_data_size ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to reallocate chunk cache.\n");

				return( -1 );
			}
			/* Adjust chunk data buffer if necessary
			 */
			if( ( chunk_cache_used == 1 ) && ( buffer != internal_handle->chunk_cache->data ) )
			{
				buffer = internal_handle->chunk_cache->data;
			}
		}
		chunk_data = internal_handle->chunk_cache->data;

#ifdef HAVE_BUFFER_PASSTHROUGH
		/* Determine if the chunk data should be put directly in the buffer
		 */
		if( ( buffer != internal_handle->chunk_cache->data ) && ( chunk_offset == 0 ) && ( size >= (uint64_t) internal_handle->media->chunk_size ) )
		{
			chunk_data = (EWF_CHUNK *) buffer;
		}
#endif
		/* Determine if the chunk data should be directly read into chunk data buffer
		 * or to use the intermediate storage for a compressed chunk
		 */
		if( internal_handle->offset_table->compressed[ chunk ] == 1 )
		{
			chunk_read = internal_handle->chunk_cache->compressed;
		}
		else
		{
			chunk_read = chunk_data;
		}
		/* If buffer passthrough is used the CRC is read seperately
		 */
		if( ( chunk_read != internal_handle->chunk_cache->compressed ) && ( chunk_read != internal_handle->chunk_cache->data ) )
		{
			chunk_data_size -= EWF_CRC_SIZE;
		}
		/* Make sure the file offset is in the right place
		 */
		if( libewf_seek_chunk( internal_handle, chunk ) <= -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to seek chunk.\n");

			return( -1 );
		}
		/* Read the chunk data
		 */
		chunk_read_count = ewf_chunk_read( chunk_read, file_descriptor, chunk_data_size );

		if( chunk_read_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to read chunk.\n");

			return( -1 );
		}
		internal_handle->segment_table->file_offset[ segment_number ] += (off_t) chunk_read_count;

		if( ( internal_handle->offset_table->last > 0 ) && ( chunk > 0 ) )
		{
			percentage = (uint8_t) ( (uint32_t) ( chunk * 100 ) / internal_handle->offset_table->last );
		}
		/* Determine if the chunk is not compressed
		 */
		if( internal_handle->offset_table->compressed[ chunk ] == 0 )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_read_chunk: chunk %" PRIu32 " of %" PRIu32 " (%" PRIu8 "%%) is UNCOMPRESSED.\n", ( chunk + 1 ), internal_handle->offset_table->amount, percentage );

			/* If buffer passthrough is used the CRC needs to be read seperately
			 */
			if( ( chunk_read != internal_handle->chunk_cache->compressed ) && ( chunk_read != internal_handle->chunk_cache->data ) )
			{
				crc_read_count = ewf_crc_read( &stored_crc, file_descriptor );

				if( crc_read_count != (ssize_t) EWF_CRC_SIZE )
				{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
					filename = libewf_segment_table_get_wide_filename( internal_handle->segment_table, segment_number );
#else
					filename = libewf_segment_table_get_filename( internal_handle->segment_table, segment_number );
#endif

					if( filename == NULL )
					{
						LIBEWF_WARNING_PRINT( "libewf_read_chunk: error reading CRC of chunk: %" PRIu32 " from segment file: %" PRIu16 ".\n",
							chunk, segment_number );
					}
					else
					{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
						LIBEWF_WARNING_PRINT( "libewf_read_chunk: error reading CRC of chunk: %" PRIu32 " from segment file: %" PRIu16 " (%ls).\n",
							chunk, segment_number, filename );
#else
						LIBEWF_WARNING_PRINT( "libewf_read_chunk: error reading CRC of chunk: %" PRIu32 " from segment file: %" PRIu16 " (%s).\n",
							chunk, segment_number, filename );
#endif
					}
					return( -1 );
				}
				internal_handle->segment_table->file_offset[ segment_number ] += (off_t) crc_read_count;
			}
			/* Otherwise convert the last 4 bytes of the chunk cache
			 */
			else
			{
				if( libewf_endian_convert_32bit( &stored_crc, &chunk_data[ chunk_data_size - EWF_CRC_SIZE ] ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to convert stored CRC value.\n" );

					return( -1 );
				}
				chunk_data_size -= (uint32_t) EWF_CRC_SIZE;
			}
			/* Calculate the CRC
			 */
			if( ewf_crc_calculate( &calculated_crc, (uint8_t *) chunk_data, chunk_data_size, 1 ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to calculate CRC.\n" );

				return( -1 );
			}
			LIBEWF_VERBOSE_PRINT( "libewf_read_chunk: CRC for chunk: %" PRIu32 " (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", ( chunk + 1 ), stored_crc, calculated_crc );

			if( stored_crc != calculated_crc )
			{
				LIBEWF_WARNING_PRINT( "libewf_read_chunk: CRC does not match for chunk: %" PRIu32 " (in file: %" PRIu32 ", calculated: %" PRIu32 ").\n", chunk, stored_crc, calculated_crc );

#ifdef WIPEONERROR
				/* The chunk data is wiped
				 */
				if( libewf_common_memset( chunk_read, 0, internal_handle->media->chunk_size ) == NULL )
				{
					LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to wipe chunk data.\n" );

					return( -1 );
				}
#endif
				if( libewf_internal_handle_add_crc_error_chunk( internal_handle, chunk ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to set CRC error chunk.\n" );

					return( -1 );
				}
				if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
		}
		/* Determine if the chunk is compressed
		 */
		else if( internal_handle->offset_table->compressed[ chunk ] == 1 )
		{
			chunk_data_size = internal_handle->media->chunk_size + EWF_CRC_SIZE;
			result          = ewf_chunk_uncompress( chunk_data, &chunk_data_size, chunk_read, chunk_read_count );

			LIBEWF_VERBOSE_PRINT( "libewf_read_chunk: chunk %" PRIu32 " of %" PRIu32 " (%" PRIu8 "%%) is COMPRESSED.\n", ( chunk + 1 ), internal_handle->offset_table->amount, percentage );

			if( result != 1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to uncompress chunk.\n" );

#ifdef WIPEONERROR
				/* The chunk data is wiped
				 */
				if( libewf_common_memset( chunk_data, 0, internal_handle->media->chunk_size ) == NULL )
				{
					LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to wipe chunk data.\n" );

					return( -1 );
				}
#endif
				if( libewf_internal_handle_add_crc_error_chunk( internal_handle, chunk ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to set CRC error chunk.\n" );

					return( -1 );
				}
				if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
		}
		else
		{
			LIBEWF_WARNING_PRINT( "libewf_read_chunk: unsupported compressed chunk value.\n" );

			return( -1 );
		}
		/* Check if the chunk was processed in the MD5 hash calculation
		 */
		if( internal_handle->offset_table->hashed[ chunk ] != 1 )
		{
			if( libewf_md5_update( &internal_handle->md5_context, chunk_data, chunk_data_size ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to update MD5 context.\n" );

				if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
			internal_handle->offset_table->hashed[ chunk ] = 1;
		}
		/* Check if the last chunk was processed and finalize MD5 hash
		 */
		if( chunk == ( internal_handle->offset_table->amount - 1 ) )
		{
			md5_hash_size = EWF_DIGEST_HASH_SIZE_MD5;

			if( internal_handle->calculated_md5_hash == NULL )
			{
				internal_handle->calculated_md5_hash = (EWF_DIGEST_HASH *) libewf_common_alloc( md5_hash_size );

				if( internal_handle->calculated_md5_hash == NULL )
				{
					LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to created calculated MD5 hash.\n" );

					return( -1 );
				}
			}
			if( ( libewf_md5_finalize( &internal_handle->md5_context, internal_handle->calculated_md5_hash, &md5_hash_size ) != 1 )
			 || ( md5_hash_size != EWF_DIGEST_HASH_SIZE_MD5 ) )
			{
				LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to finalize MD5 context.\n" );

				if( internal_handle->error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
		}
		/* Swap bytes after MD5 calculation
		 */
		if( ( internal_handle->swap_byte_pairs == 1 ) && ( libewf_endian_swap_byte_pairs( chunk_data, chunk_data_size ) != 1 ) )
		{
			LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to swap byte pairs.\n" );

			return( -1 );
		}
		/* Flag that the chunk was cached
		 */
		if( chunk_data == internal_handle->chunk_cache->data )
		{
			internal_handle->chunk_cache->chunk  = chunk;
			internal_handle->chunk_cache->amount = chunk_data_size;
			internal_handle->chunk_cache->offset = 0;
			internal_handle->chunk_cache->cached = 1;
		}
	}
	else
	{
		chunk_data      = internal_handle->chunk_cache->data;
		chunk_data_size = internal_handle->chunk_cache->amount;
	}
	/* Determine the available amount of data within the cached chunk
	 */
	if( chunk_data_size < chunk_offset )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: chunk offset exceeds amount of bytes available in chunk.\n" );

		return( -1 );
	}
	bytes_available = chunk_data_size - chunk_offset;

	/* Correct the available amount of bytes is larger than the requested amount of bytes
	 */
	if( (uint64_t) bytes_available > size )
	{
		bytes_available = (uint32_t) size;
	}
	if( bytes_available > (uint64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_chunk: invalid available amount of bytes only values below 2^32 are supported.\n" );

		return( -1 );
	}
	/* If the data was read into the chunk cache copy it to the buffer
	 */
	if( chunk_data == internal_handle->chunk_cache->data )
	{
		/* Copy the relevant data to buffer
		 */
		if( ( bytes_available > 0 ) && ( libewf_common_memcpy( buffer, &chunk_data[ chunk_offset ], bytes_available ) == NULL ) )
		{
			LIBEWF_WARNING_PRINT( "libewf_read_chunk: unable to set chunk data in buffer.\n" );

			return( -1 );
		}
	}
	return( (int32_t) bytes_available );
}

/* Reads media data from the last current into a buffer
 * This function swaps byte pairs if specified
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_read_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	ssize_t chunk_read_count                = 0;
	ssize_t total_read_count                = 0;
	size_t chunk_data_size                  = 0;
	int8_t chunk_cache_used                 = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_buffer: invalid handle.\n" );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_buffer: invalid chunk cache.\n" );

		return( -1 );
	}
	if( internal_handle->index_build == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_buffer: index was not build.\n" );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_buffer: invalid buffer.\n" );

		return( -1 );
	}
	if( buffer == internal_handle->chunk_cache->compressed )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_buffer: invalid buffer - chunk cache compressed cannot be used as buffer.\n");

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_buffer: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_read_buffer: reading size: %zu.\n", size );

	/* Check if chunk cache passthrough is used
	 * if the chunk cache is used as the chunk data buffer
	 */
	chunk_cache_used = (int8_t) ( buffer == internal_handle->chunk_cache->data );

	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents multiple reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media->chunk_size + EWF_CRC_SIZE;

	if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_read_buffer: reallocating chunk data size: %zu.\n", chunk_data_size );

		if( libewf_internal_handle_chunk_cache_realloc( internal_handle, chunk_data_size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_read_buffer: unable to reallocate chunk cache.\n");

			return( -1 );
		}
		/* Adjust chunk data buffer if necessary
		 */
		if( ( chunk_cache_used == 1 ) && ( buffer != internal_handle->chunk_cache->data ) )
		{
			buffer = (void *) internal_handle->chunk_cache->data;
		}
	}
	while( size > 0 )
	{
		chunk_read_count = libewf_read_chunk( internal_handle, internal_handle->current_chunk, internal_handle->current_chunk_offset, (void *) &( (uint8_t *) buffer )[ total_read_count ], size );

		/* libewf_read_chunk could relocate the chunk cache
		 * correct buffer is chunk cache passthrough is used
		 */
		if( ( chunk_cache_used == 1 ) && ( buffer != internal_handle->chunk_cache->data ) )
		{
			buffer = (void *) internal_handle->chunk_cache->data;
		}
		if( chunk_read_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_read_buffer: unable to read data from chunk.\n" );

			return( -1 );
		}
		else if( chunk_read_count == 0 )
		{
			break;
		}
		size                                  -= chunk_read_count;
		total_read_count                      += chunk_read_count;
		internal_handle->current_chunk_offset += (uint32_t) chunk_read_count;

		if( internal_handle->current_chunk_offset == internal_handle->media->chunk_size )
		{
			internal_handle->current_chunk_offset = 0;
			internal_handle->current_chunk       += 1;
		}
		else if( internal_handle->current_chunk_offset > internal_handle->media->chunk_size )
		{
			LIBEWF_WARNING_PRINT( "libewf_read_buffer: invalid current chunk offset.\n" );

			return( -1 );
		}
	}
	return( total_read_count );
}

/* Reads media data from an offset into a buffer
 * This function swaps byte pairs if specified
 * Returns the amount of bytes read, or -1 on error
 */
ssize_t libewf_read_random( LIBEWF_HANDLE *handle, void *buffer, size_t size, off_t offset )
{
	ssize_t read_count = 0;

	if( libewf_seek_offset( handle, offset ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_random: unable to seek offset.\n" );

		return( -1 );
	}
	read_count = libewf_read_buffer( handle, buffer, size );

	if( read_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_read_random: unable to read buffer.\n" );

		return( -1 );
	}
	return( read_count );
}

