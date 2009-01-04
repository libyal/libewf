/*
 * File writing functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <endian.h>
#include <file_io.h>
#include <memory.h>
#include <notify.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include "libewf_definitions.h"
#include "libewf_chunk_cache.h"
#include "libewf_compression.h"
#include "libewf_error.h"
#include "libewf_file.h"
#include "libewf_filename.h"
#include "libewf_file_io_pool.h"
#include "libewf_hash_values.h"
#include "libewf_list_type.h"
#include "libewf_offset_table.h"
#include "libewf_read.h"
#include "libewf_section.h"
#include "libewf_section_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_file_handle.h"
#include "libewf_segment_table.h"
#include "libewf_write.h"

#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_file_header.h"
#include "ewf_section.h"
#include "ewfx_delta_chunk.h"

/* Check for empty block, a block that contains the same value for every byte
 * Returns 1 if block is empty, 0 if not or -1 on error
 */
int libewf_write_test_empty_block(
     uint8_t *block_buffer,
     size_t size,
     libewf_error_t **error )
{
	static char *function = "libewf_write_test_empty_block";
	size_t iterator       = 0;

	if( block_buffer == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid block buffer.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 1; iterator < size; iterator++ )
	{
		if( block_buffer[ 0 ] != block_buffer[ iterator ] )
		{
			return( 0 );
		}
	}
	return( 1 );
}

/* Calculates an estimate of the amount of chunks that fit within a segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_write_calculate_chunks_per_segment(
     uint32_t *chunks_per_segment,
     size64_t remaining_segment_file_size,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t segment_amount_of_chunks,
     uint32_t amount_of_chunks,
     libewf_media_values_t *media_values,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_amount,
     libewf_error_t **error )
{
	static char *function                 = "libewf_write_calculate_chunks_per_segment";
	int64_t calculated_chunks_per_segment = 0;
	int64_t maximum_chunks_per_segment    = 0;
	int64_t remaining_amount_of_chunks    = 0;
	int64_t required_chunk_sections       = 0;

	if( chunks_per_segment == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid chunks per segment.\n",
		 function );

		return( -1 );
	}
	if( maximum_section_amount_of_chunks == 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid maximum section amount of chunks value.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	/* Calculate the maximum amount of chunks within this segment
	 */
	maximum_chunks_per_segment = remaining_segment_file_size;

	if( ewf_format == EWF_FORMAT_S01 )
	{
		/* The EWF-S01 format uses compression this will add 16 bytes on average
		 */
		maximum_chunks_per_segment /= media_values->chunk_size + 16;
	}
	else
	{
		maximum_chunks_per_segment /= media_values->chunk_size + sizeof( ewf_crc_t );
	}
	/* Determine the amount of required chunk sections
	 */
	if( unrestrict_offset_amount == 0 )
	{
		required_chunk_sections = maximum_chunks_per_segment
		                        % maximum_section_amount_of_chunks;
	}
	else
	{
		required_chunk_sections = 1;
	}
	calculated_chunks_per_segment = remaining_segment_file_size;

	if( ewf_format == EWF_FORMAT_S01 )
	{
		/* Leave space for the chunk section starts
		 */
		calculated_chunks_per_segment -= required_chunk_sections
		                               * sizeof( ewf_section_t );

		/* Leave space for the table offsets
		 */
		calculated_chunks_per_segment -= maximum_chunks_per_segment
		                               * sizeof( ewf_table_offset_t );
	}
	else if( format == LIBEWF_FORMAT_ENCASE1 )
	{
		/* Leave space for the chunk section starts and the offset table CRC
		 */
		calculated_chunks_per_segment -= required_chunk_sections
		                               * ( sizeof( ewf_section_t ) + sizeof( ewf_crc_t ) );

		/* Leave space for the table offsets
		 */
		calculated_chunks_per_segment -= maximum_chunks_per_segment
		                               * sizeof( ewf_table_offset_t );
	}
	else
	{
		/* Leave space for the chunk,
		table and table2 section starts and the table and table2 offset table CRCs
		 */
		calculated_chunks_per_segment -= required_chunk_sections
		                               * ( ( 3 * sizeof( ewf_section_t ) ) + ( 2 * sizeof( ewf_crc_t ) ) );

		/* Leave space for the table and table2 offsets
		 */
		calculated_chunks_per_segment -= 2 * maximum_chunks_per_segment
		                               * sizeof( ewf_table_offset_t );
	}
	/* Calculate the amount of chunks within this segment file
	 */
	if( ewf_format == EWF_FORMAT_S01 )
	{
		/* The EWF-S01 format uses compression this will add 16 bytes on average
		 */
		calculated_chunks_per_segment /= media_values->chunk_size + 16;
	}
	else
	{
		calculated_chunks_per_segment /= media_values->chunk_size + sizeof( ewf_crc_t );
	}
	/* If the input size is known determine the remaining amount of chunks
	 */
	if( media_values->media_size > 0 )
	{
		remaining_amount_of_chunks = (int64_t) media_values->amount_of_chunks
		                           - (int64_t) amount_of_chunks;

		/* Check if less chunks remain than the amount of chunks calculated
		 */
		if( remaining_amount_of_chunks < calculated_chunks_per_segment )
		{
			calculated_chunks_per_segment = remaining_amount_of_chunks;
		}
	}
	/* Make sure to return the total amount of chunks per segment
	 */
	if( segment_amount_of_chunks > 0 )
	{
		calculated_chunks_per_segment += segment_amount_of_chunks;
	}
	/* Fail safe no more than 2^32 values are allowed
	 */
	if( calculated_chunks_per_segment > (int64_t) UINT32_MAX )
	{
		calculated_chunks_per_segment = UINT32_MAX;
	}
	*chunks_per_segment = calculated_chunks_per_segment;

	return( 1 );
}

/* Calculates the amount of chunks that fit within a chunks section
 * Returns 1 if successful or -1 on error
 */
int libewf_write_calculate_chunks_per_chunks_section(
     uint32_t *chunks_per_chunks_section,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t chunks_per_segment,
     uint8_t chunks_section_number,
     uint8_t unrestrict_offset_amount,
     libewf_error_t **error )
{
	static char *function              = "libewf_write_calculate_chunks_per_chunks_section";
	int64_t remaining_amount_of_chunks = 0;

	if( chunks_per_chunks_section == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid chunks per chunks section.\n",
		 function );

		return( -1 );
	}
	if( maximum_section_amount_of_chunks == 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid maximum section amount of chunks value.\n",
		 function );

		return( -1 );
	}
	if( chunks_section_number == 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported section number: %" PRIu8 ".\n",
		 function,
		 chunks_section_number );

		return( -1 );
	}
	remaining_amount_of_chunks = (int64_t) chunks_per_segment;

	if( chunks_section_number > 1 )
	{
		remaining_amount_of_chunks -= ( chunks_section_number - 1 )
		                            * maximum_section_amount_of_chunks;
	}
	if( remaining_amount_of_chunks <= 0 )
	{
		return( -1 );
	}
	if( ( unrestrict_offset_amount == 0 )
	 && ( remaining_amount_of_chunks > (int64_t) maximum_section_amount_of_chunks ) )
	{
		remaining_amount_of_chunks = (int64_t) maximum_section_amount_of_chunks;
	}
	/* Fail safe no more than 2^31 values are allowed
	 */
	if( remaining_amount_of_chunks > (int64_t) INT32_MAX )
	{
		remaining_amount_of_chunks = INT32_MAX;
	}
	*chunks_per_chunks_section = (uint32_t) remaining_amount_of_chunks;

	return( 1 );
}

/* Tests if the current segment file is full
 * Returns 1 if full, 0 if not or -1 on error
 */
int libewf_write_test_segment_file_full(
     ssize64_t remaining_segment_file_size,
     uint32_t segment_amount_of_chunks,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     uint32_t chunks_per_segment,
     uint32_t current_amount_of_chunks,
     uint8_t format,
     uint8_t ewf_format,
     libewf_error_t **error )
{
	static char *function = "libewf_write_test_segment_file_full";

	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	/* Check if the maximum amount of chunks has been reached
	 */
	if( ( media_values->amount_of_chunks != 0 )
	 && ( media_values->amount_of_chunks == current_amount_of_chunks ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: all required chunks have been written.\n",
		 function );
#endif

		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( media_values->media_size != 0 )
	 && ( input_write_count >= (ssize64_t) media_values->media_size ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: all required data has been written.\n",
		 function );
#endif

		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format should use the pre calculated size
	 */
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( segment_amount_of_chunks >= chunks_per_segment )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf(
			 "%s: no space left for additional chunk.\n",
			 function );
#endif

			return( 1 );
		}
	}
	/* Determine if a chunk would fit in the segment file
	 */
	else if( remaining_segment_file_size < (ssize64_t) ( media_values->chunk_size + sizeof( ewf_crc_t ) ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: no space left for additional chunk - file size exceeded.\n",
		 function );
#endif

		return( 1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: space left for additional chunk.\n",
	 function );
#endif

	return( 0 );
}

/* Tests if the current chunks section is full
 * Returns 1 if full, 0 if not or -1 on error
 */
int libewf_write_test_chunks_section_full(
     off64_t chunks_section_offset,
     ssize64_t remaining_segment_file_size,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     off64_t segment_file_offset,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t section_amount_of_chunks,
     uint32_t current_amount_of_chunks,
     uint32_t chunks_per_chunks_section,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_amount,
     libewf_error_t **error )
{
	static char *function = "libewf_write_test_chunks_section_full";

	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( segment_file_offset > (off64_t) INT64_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM,
		 "%s: invalid segment file offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( maximum_section_amount_of_chunks == 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid maximum section amount of chunks value.\n",
		 function );

		return( -1 );
	}
	/* Check if a chunks section has been opened
	 */
	if( chunks_section_offset == 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: no chunks section has been created.\n",
		 function );
#endif

		return( 0 );
	}
	/* Check if the maximum amount of chunks has been reached
	 */
	if( ( media_values->amount_of_chunks != 0 )
	 && ( media_values->amount_of_chunks == current_amount_of_chunks ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: all required chunks have been written.\n",
		 function );
#endif

		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( media_values->media_size != 0 )
	 && ( input_write_count >= (ssize64_t) media_values->media_size ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: all required data has been written.\n",
		 function );
#endif

		return( 1 );
	}
	/* Check if the maximum amount of chunks restriction should apply
	 */
	if( ( unrestrict_offset_amount == 0 )
	 && ( section_amount_of_chunks >= maximum_section_amount_of_chunks ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: no space left for additional chunk - maximum reached.\n",
		 function );
#endif

		return( 1 );
	}
	/* Fail safe no more than 2^31 chunk values are allowed
	 */
	if( section_amount_of_chunks > (uint32_t) INT32_MAX )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: no space left for additional chunk - preventing chunk overflow.\n",
		 function );
#endif

		return( 1 );
	}
	/* Prevent offset overflow
	 */
	if( ( segment_file_offset - chunks_section_offset ) > (off64_t) INT32_MAX )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: no space left for additional chunk - preventing offset overflow.\n",
		 function );
#endif

		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format do not allow for a growth of the offset table
	 */
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( section_amount_of_chunks >= chunks_per_chunks_section )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf(
			 "%s: no space left for additional chunk.\n",
			 function );
#endif

			return( 1 );
		}
	}
	/* Determine if a chunk would fit in the segment file
	 */
	else if( remaining_segment_file_size < (ssize64_t) ( media_values->chunk_size + sizeof( ewf_crc_t ) ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: no space left for additional chunk - file size exceeded.\n",
		 function );
#endif

		return( 1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: space left for additional chunk.\n",
	 function );
#endif

	return( 0 );
}

/* Processes the chunk data, applies compression if necessary and calculates the CRC
 * Returns the amount of bytes of the processed chunk data or -1 on error
 */
ssize_t libewf_write_process_chunk_data(
         libewf_chunk_cache_t *chunk_cache,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t compress_empty_block,
         uint8_t ewf_format,
         uint8_t *chunk_data,
         size_t chunk_data_size,
         uint8_t *compressed_chunk_data,
         size_t *compressed_chunk_data_size,
         int8_t *is_compressed,
         ewf_crc_t *chunk_crc,
         int8_t *write_crc,
         libewf_error_t **error )
{
	uint8_t *chunk_data_crc        = NULL;
	static char *function          = "libewf_write_process_chunk_data";
	size_t data_write_size         = 0;
	int8_t chunk_compression_level = 0;
	int chunk_cache_data_used      = 0;
	int result                     = 0;

	if( chunk_cache == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid chunk cache.\n",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid media values.\n",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid chunk data.\n",
		 function );

		return( -1 );
	}
	if( chunk_data == chunk_cache->compressed )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid chunk data buffer - same as chunk cache compressed.\n",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk data size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) media_values->chunk_size )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_TOO_LARGE,
		 "%s: invalid chunk data size value exceeds media values chunk size.\n",
		 function );

		return( -1 );
	}
	if( chunk_crc == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid chunk CRC.\n",
		 function );

		return( -1 );
	}
	if( is_compressed == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid is compressed.\n",
		 function );

		return( -1 );
	}
	if( write_crc == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid write CRC.\n",
		 function );

		return( -1 );
	}
	*write_crc = 0;

	/* Determine the compression level
	 */
	chunk_compression_level = compression_level;

	/* Compress empty blocks if necessary
	 */
	if( ( chunk_compression_level == EWF_COMPRESSION_NONE )
	 && ( compress_empty_block == 1 ) )
	{
		result = libewf_write_test_empty_block(
		          chunk_data,
		          chunk_data_size,
		          error );

		if( result == -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if chunk data is empty.\n",
			 function );

			return( -1 );
		}
		else if( result == 1 )
		{
			chunk_compression_level = EWF_COMPRESSION_DEFAULT;
		}
	}
	/* The compressed data size contains the maximum allowed buffer size on entry
	 */
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( chunk_compression_level != EWF_COMPRESSION_NONE ) )
	{
		if( compressed_chunk_data == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
			 LIBEWF_ARGUMENT_ERROR_INVALID,
			 "%s: invalid compressed chunk data.\n",
			 function );

			return( -1 );
		}
		if( chunk_data == compressed_chunk_data )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
			 LIBEWF_ARGUMENT_ERROR_INVALID,
			 "%s: invalid compressed chunk data is the same as chunk data.\n",
			 function );

			return( -1 );
		}
		if( compressed_chunk_data_size == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
			 LIBEWF_ARGUMENT_ERROR_INVALID,
			 "%s: invalid compressed chunk data size.\n",
			 function );

			return( -1 );
		}
		if( *compressed_chunk_data_size > (size_t) SSIZE_MAX )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
			 LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM,
			 "%s: invalid compressed chunk data size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		result = libewf_compress(
			  (uint8_t *) compressed_chunk_data,
			  compressed_chunk_data_size,
			  (uint8_t *) chunk_data,
			  chunk_data_size,
			  chunk_compression_level );

		/* Check if the compressed buffer was too small
		 * and the chunk cache is used to store the compressed chunk
		 * and a new compressed data size buffer was passed back
		 */
		if( ( result == -1 )
		 && ( compressed_chunk_data == chunk_cache->compressed )
		 && ( *compressed_chunk_data_size > 0 ) )
		{
			chunk_cache_data_used = (int) ( chunk_data == chunk_cache->data );

			if( libewf_chunk_cache_resize(
			     chunk_cache,
			     *compressed_chunk_data_size ) != 1 )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize chunk cache.\n",
				 function );

				return( -1 );
			}
			/* Make sure the pointer refers to the reallocated buffer
			 */
			compressed_chunk_data = chunk_cache->compressed;

			if( ( chunk_cache_data_used == 1 )
			 && ( chunk_data != chunk_cache->data ) )
			{
				chunk_data = chunk_cache->data;
			}
			result = libewf_compress(
				  (uint8_t *) compressed_chunk_data,
				  compressed_chunk_data_size,
				  (uint8_t *) chunk_data,
				  chunk_data_size,
				  chunk_compression_level );
		}
		if( result != 1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_COMPRESSION,
			 LIBEWF_COMPRESSION_ERROR_COMPRESS_FAILED,
			 "%s: unable to compress chunk.\n",
			 function );

			return( -1 );
		}
	}
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( ( *compressed_chunk_data_size > 0 )
	  && ( *compressed_chunk_data_size < media_values->chunk_size ) ) )
	{
		data_write_size = *compressed_chunk_data_size;
		chunk_data      = compressed_chunk_data;
		*is_compressed  = 1;

		/* Zlib creates its own CRC
		 */
		if( memory_copy(
		     chunk_crc,
		     &( chunk_data[ *compressed_chunk_data_size - sizeof( ewf_crc_t ) ] ),
		     sizeof( ewf_crc_t ) ) == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set CRC.\n",
			 function );

			return( -1 );
		}
	}
	else
	{
		*chunk_crc = ewf_crc_calculate(
		              chunk_data,
		              chunk_data_size,
		              1 );

		data_write_size = chunk_data_size;
		*is_compressed  = 0;

		/* If the chunk cache data is used add the CRC
		 */
		if( chunk_data == chunk_cache->data )
		{
			chunk_data_crc = &( chunk_data[ chunk_data_size ] );

			endian_little_revert_32bit(
			 chunk_data_crc,
			 *chunk_crc );

			data_write_size += sizeof( ewf_crc_t );
		}
		else
		{
			*write_crc = 1;
		}
	}
	return( (ssize_t) data_write_size );
}

/* Writes a new chunk of data in EWF format from a buffer at the current offset
 * The necessary settings of the write values must have been made
 * Returns the amount of bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_raw_write_chunk_new(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         ewf_crc_t chunk_crc,
         int8_t write_crc,
         libewf_error_t **error )
{
	void *reallocation          = NULL;
	static char *function       = "libewf_raw_write_chunk_new";
	off64_t segment_file_offset = 0;
	ssize_t total_write_count   = 0;
	ssize_t write_count         = 0;
	uint16_t segment_number     = 0;
	int result                  = 0;

	if( internal_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( -1 );
	}
	/* Check if the write was already finalized
	 */
	if( internal_handle->write->write_finalized == 1 )
	{
		return( 0 );
	}
	if( internal_handle->segment_table == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid segment table - missing segment file handles.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( ( chunk < internal_handle->offset_table->amount_of_chunk_offsets )
	 && ( internal_handle->offset_table->chunk_offset != NULL )
	 && ( internal_handle->offset_table->chunk_offset[ chunk ].segment_file_handle != NULL ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_CONFLICTING_VALUE,
		 "%s: invalid chunk: %" PRIu32 " already exists.\n",
		 function,
		 chunk );

		return( -1 );
	}
	/* Allocate the necessary amount of chunk offsets
	 * this reduces the amount of reallocations
	 */
	if( internal_handle->offset_table->amount_of_chunk_offsets < internal_handle->media_values->amount_of_chunks )
        {
		if( libewf_offset_table_resize(
		     internal_handle->offset_table,
		     internal_handle->media_values->amount_of_chunks ) != 1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize offset table.\n",
			 function );

			return( -1 );
		}
	}
	if( chunk_buffer == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid chunk buffer.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_ZERO_OR_LESS,
		 "%s: invalid chunk size value is zero.\n",
		 function );

		return( -1 );
	}
	if( chunk_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* Check if the amount of bytes as specified have been written
	 */
	if( ( internal_handle->media_values->media_size != 0 )
	 && ( internal_handle->write->input_write_count >= (ssize64_t) internal_handle->media_values->media_size ) )
	{
		return( 0 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: writing chunk of size: %" PRIzu " with data of size: %" PRIzd ".\n",
	 function,
	 chunk_size,
	 chunk_data_size );
#endif

	segment_number = internal_handle->segment_table->amount - 1;

	/* Check if a segment number is valid
	 */
	if( segment_number > internal_handle->segment_table->amount )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_OUT_OF_RANGE,
		 "%s: invalid segment number value out of range.\n",
		 function );

		return( -1 );
	}
	/* Check if a new segment file should be created
	 */
	if( ( segment_number == 0 )
	 || ( internal_handle->segment_table->segment_file_handle[ segment_number ]->write_open == 0 ) )
	{
		/* Create the headers if required
		 */
		if( internal_handle->header_sections == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing header sections.\n",
			 function );

			return( -1 );
		}
		if( ( internal_handle->header_sections->header == NULL )
		 && ( internal_handle->header_sections->header2 == NULL )
		 && ( internal_handle->header_sections->xheader == NULL ) )
		{
			if( internal_handle->header_values == NULL )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				notify_verbose_printf(
				 "%s: empty header values - using default.\n",
				 function );
#endif

				if( libewf_internal_handle_create_header_values(
				     internal_handle ) != 1 )
				{
					libewf_error_set(
					 error,
					 LIBEWF_ERROR_DOMAIN_RUNTIME,
					 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create header values.\n",
					 function );

					return( -1 );
				}
			}
			if( libewf_header_sections_create(
			     internal_handle->header_sections,
			     internal_handle->header_values,
			     internal_handle->compression_level,
			     internal_handle->format ) == -1 )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create header(s).\n",
				 function );

				return( -1 );
			}
		}
		internal_handle->write->create_chunks_section     = 1;
		internal_handle->write->chunks_section_number     = 0;
		internal_handle->write->chunks_per_chunks_section = 0;
		internal_handle->write->segment_amount_of_chunks  = 0;

		segment_number += 1;

#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: creating segment file with segment number: %" PRIu16 ".\n",
		 function,
		 segment_number );
#endif

		/* Create a new segment file
		 */
		if( libewf_segment_table_create_segment_file(
		     internal_handle->segment_table,
		     segment_number,
		     internal_handle->file_io_pool,
		     internal_handle->write->maximum_amount_of_segments,
		     LIBEWF_SEGMENT_FILE_TYPE_EWF,
		     internal_handle->format,
		     internal_handle->ewf_format ) != 1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_OUTPUT,
			 LIBEWF_OUTPUT_ERROR_OPEN_FAILED,
			 "%s: unable to create segment file for segment: %" PRIu16 ".\n",
			 function,
			 segment_number );

			return( -1 );
		}
		internal_handle->write->remaining_segment_file_size = internal_handle->write->segment_file_size;

		/* Leave space for the done or next section
		 */
		internal_handle->write->remaining_segment_file_size -= sizeof( ewf_section_t );

		/* Write the start of the segment file
		 * like the file header, the header, volume and/or data section, etc.
		 */
		write_count = libewf_segment_file_write_start(
		               internal_handle->segment_table->segment_file_handle[ segment_number ],
		               internal_handle->file_io_pool,
		               segment_number,
		               LIBEWF_SEGMENT_FILE_TYPE_EWF,
		               internal_handle->media_values,
		               internal_handle->header_sections,
		               internal_handle->compression_level,
		               internal_handle->format,
		               internal_handle->ewf_format,
		               &( internal_handle->write->data_section ) );

		if( write_count == -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_OUTPUT,
			 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
			 "%s: unable to write segment file start.\n",
			 function );

			return( -1 );
		}
		total_write_count                                   += write_count;
		internal_handle->write->write_count                 += write_count;
		internal_handle->write->remaining_segment_file_size -= write_count;

		/* Determine the amount of chunks per segment
		 */
		if( internal_handle->segment_table->segment_file_handle[ segment_number ]->amount_of_chunks == 0 )
		{
			if( libewf_write_calculate_chunks_per_segment(
			     &( internal_handle->write->chunks_per_segment ),
			     internal_handle->write->remaining_segment_file_size,
			     internal_handle->write->maximum_section_amount_of_chunks,
			     internal_handle->write->segment_amount_of_chunks,
			     internal_handle->write->amount_of_chunks,
			     internal_handle->media_values,
			     internal_handle->format,
			     internal_handle->ewf_format,
			     internal_handle->write->unrestrict_offset_amount,
			     error ) != 1 )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine the amount of chunks per segment.\n",
				 function );

				return( -1 );
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf(
			 "%s: calculated amount of chunks for segment: %" PRIu32 ".\n",
			 function,
			 internal_handle->write->chunks_per_segment );
#endif
		}
		else
		{
			internal_handle->write->chunks_per_segment = internal_handle->segment_table->segment_file_handle[ segment_number ]->amount_of_chunks;
		}
	}
	/* Check if a chunk section should be created
	 */
	if( internal_handle->write->create_chunks_section == 1 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: creating chunks section.\n",
		 function );
#endif

		internal_handle->write->create_chunks_section      = 0;
		internal_handle->write->section_amount_of_chunks   = 0;
		internal_handle->write->chunks_section_write_count = 0;

		if( internal_handle->ewf_format == EWF_FORMAT_S01 )
		{
			/* Leave space for the chunk section start
			 */
			internal_handle->write->remaining_segment_file_size -= sizeof( ewf_section_t );
		}
		else if( internal_handle->format == LIBEWF_FORMAT_ENCASE1 )
		{
			/* Leave space for the chunk section start and the offset table CRC
			 */
			internal_handle->write->remaining_segment_file_size -= sizeof( ewf_section_t ) + sizeof( ewf_crc_t );
		}
		else
		{
			/* Leave space for the chunk,
			table and table2 section starts and the table and table2 offset table CRCs
			 */
			internal_handle->write->remaining_segment_file_size -= ( 3 * sizeof( ewf_section_t ) ) + ( 2 * sizeof( ewf_crc_t ) );
		}
		if( libewf_file_io_pool_get_offset(
		     internal_handle->file_io_pool,
		     internal_handle->segment_table->segment_file_handle[ segment_number ]->file_io_pool_entry,
		     &( internal_handle->write->chunks_section_offset ) ) != 1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve current offset in segment file.\n",
			 function );

			return( -1 );
		}
		/* Start with chunks section number number 1,
		value is initialized with 0
		 */
		internal_handle->write->chunks_section_number += 1;

		/* Recalculate the amount of chunks per segment for a better segment file fill when compression is used
		 */
		if( internal_handle->segment_table->segment_file_handle[ segment_number ]->amount_of_chunks == 0 )
		{
			if( libewf_write_calculate_chunks_per_segment(
			     &( internal_handle->write->chunks_per_segment ),
			     internal_handle->write->remaining_segment_file_size,
			     internal_handle->write->maximum_section_amount_of_chunks,
			     internal_handle->write->segment_amount_of_chunks,
			     internal_handle->write->amount_of_chunks,
			     internal_handle->media_values,
			     internal_handle->format,
			     internal_handle->ewf_format,
			     internal_handle->write->unrestrict_offset_amount,
			     error ) != 1 )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine the amount of chunks per segment.\n",
				 function );

				return( -1 );
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf(
			 "%s: calculated amount of chunks for segment: %" PRIu32 ".\n",
			 function,
			 internal_handle->write->chunks_per_segment );
#endif
		}
		else
		{
			internal_handle->write->chunks_per_segment = internal_handle->segment_table->segment_file_handle[ segment_number ]->amount_of_chunks;
		}
		if( libewf_write_calculate_chunks_per_chunks_section(
		     &( internal_handle->write->chunks_per_chunks_section ),
		     internal_handle->write->maximum_section_amount_of_chunks,
		     internal_handle->write->chunks_per_segment,
		     internal_handle->write->chunks_section_number,
		     internal_handle->write->unrestrict_offset_amount,
		     error ) != 1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the amount of chunks per chunks section.\n",
			 function );

			return( -1 );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: calculated amount of chunks for chunks section: %" PRIu32 ".\n",
		 function,
		 internal_handle->write->chunks_per_chunks_section );
#endif

		if( internal_handle->write->amount_of_table_offsets < internal_handle->write->chunks_per_chunks_section )
		{
			reallocation = memory_reallocate(
			                internal_handle->write->table_offsets,
			                sizeof( ewf_table_offset_t ) * internal_handle->write->chunks_per_chunks_section );

			if( reallocation == NULL )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_MEMORY,
				 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create table offsets.\n",
				 function );

				return( -1 );
			}
			internal_handle->write->table_offsets           = (ewf_table_offset_t *) reallocation;
			internal_handle->write->amount_of_table_offsets = internal_handle->write->chunks_per_chunks_section;
		}
		/* Write the section start of the chunks section
		 */
		write_count = libewf_segment_file_write_chunks_section_start(
		               internal_handle->segment_table->segment_file_handle[ segment_number ],
		               internal_handle->file_io_pool,
		               internal_handle->offset_table,
		               internal_handle->write->table_offsets,
		               internal_handle->write->amount_of_table_offsets,
		               internal_handle->media_values->chunk_size,
		               internal_handle->write->amount_of_chunks,
		               internal_handle->write->chunks_per_chunks_section,
		               internal_handle->format,
		               internal_handle->ewf_format );

		if( write_count == -1 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf(
			 "%s: unable to write section start for chunks.\n",
			 function );
#endif

			return( -1 );
		}
		total_write_count                                   += write_count;
		internal_handle->write->write_count                 += write_count;
		internal_handle->write->remaining_segment_file_size -= write_count;
	}
	/* Write the chunk data
	 */
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: writing %" PRIzd " bytes to segment file: %" PRIu16 ".\n",
	 function,
	 chunk_size,
	 segment_number );
#endif

	write_count = libewf_segment_file_write_chunks_data(
		       internal_handle->segment_table->segment_file_handle[ segment_number ],
		       internal_handle->file_io_pool,
		       internal_handle->offset_table,
		       chunk,
		       chunk_buffer,
		       chunk_size,
		       is_compressed,
		       &chunk_crc,
		       write_crc );

	if( write_count <= -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_OUTPUT,
		 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data.\n",
		 function );

		return( -1 );
	}
	total_write_count                                   += write_count;
	internal_handle->write->input_write_count           += chunk_data_size;
	internal_handle->write->write_count                 += write_count;
	internal_handle->write->chunks_section_write_count  += write_count;
	internal_handle->write->remaining_segment_file_size -= write_count;
	internal_handle->write->segment_amount_of_chunks    += 1;
	internal_handle->write->section_amount_of_chunks    += 1;
	internal_handle->write->amount_of_chunks            += 1;

	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* Leave space for the chunk offset in the offset table
		 */
		internal_handle->write->remaining_segment_file_size -= 2 * sizeof( ewf_table_offset_t );
	}
	else
	{
		/* Leave space for the chunk offset in the table and table2 sections
		 */
		internal_handle->write->remaining_segment_file_size -= 2 * sizeof( ewf_table_offset_t );
	}
	if( libewf_file_io_pool_get_offset(
	     internal_handle->file_io_pool,
	     internal_handle->segment_table->segment_file_handle[ segment_number ]->file_io_pool_entry,
	     &segment_file_offset ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	/* Check if the current chunks section is full,
	if so close the current section
	 */
	result = libewf_write_test_chunks_section_full(
	          internal_handle->write->chunks_section_offset,
	          internal_handle->write->remaining_segment_file_size,
	          internal_handle->media_values,
	          internal_handle->write->input_write_count,
	          segment_file_offset,
	          internal_handle->write->maximum_section_amount_of_chunks,
	          internal_handle->write->section_amount_of_chunks,
	          internal_handle->write->amount_of_chunks,
	          internal_handle->write->chunks_per_chunks_section,
	          internal_handle->format,
	          internal_handle->ewf_format,
	          internal_handle->write->unrestrict_offset_amount,
	          error );

	if( result == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if chunks section is full.\n",
		 function );

		return( -1 );
	}
	else if( result == 1 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: closing chunks section amount of data written: %" PRIi64 ".\n",
		 function,
		 internal_handle->write->chunks_section_write_count );
#endif
		if( internal_handle->write->amount_of_table_offsets < internal_handle->write->section_amount_of_chunks )
		{
			reallocation = memory_reallocate(
			                internal_handle->write->table_offsets,
			                sizeof( ewf_table_offset_t ) * internal_handle->write->section_amount_of_chunks );

			if( reallocation == NULL )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_MEMORY,
				 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create table offsets.\n",
				 function );

				return( -1 );
			}
			internal_handle->write->table_offsets           = (ewf_table_offset_t *) reallocation;
			internal_handle->write->amount_of_table_offsets = internal_handle->write->section_amount_of_chunks;
		}

		/* Correct the offset,
		size in the chunks section
		 */
		write_count = libewf_segment_file_write_chunks_correction(
		               internal_handle->segment_table->segment_file_handle[ segment_number ],
		               internal_handle->file_io_pool,
		               internal_handle->offset_table,
		               internal_handle->write->table_offsets,
		               internal_handle->write->amount_of_table_offsets,
		               internal_handle->write->chunks_section_offset,
		               (size64_t) internal_handle->write->chunks_section_write_count,
		               internal_handle->write->amount_of_chunks,
		               internal_handle->write->section_amount_of_chunks,
		               internal_handle->format,
		               internal_handle->ewf_format );

		if( write_count == -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_OUTPUT,
			 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
			 "%s: unable to correct chunks section.\n",
			 function );

			return( -1 );
		}
		total_write_count                             += write_count;
		internal_handle->write->write_count           += write_count;
		internal_handle->write->create_chunks_section  = 1;
		internal_handle->write->chunks_section_offset  = 0;

		/* Check if the current segment file is full,
		if so close the current segment file
		 */
		result = libewf_write_test_segment_file_full(
			  internal_handle->write->remaining_segment_file_size,
			  internal_handle->write->segment_amount_of_chunks,
			  internal_handle->media_values,
			  internal_handle->write->input_write_count,
			  internal_handle->write->chunks_per_segment,
			  internal_handle->write->amount_of_chunks,
			  internal_handle->format,
			  internal_handle->ewf_format,
		          error );

		if( result == -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if segment file is full.\n",
			 function );

			return( -1 );
		}
		else if( result == 1 )
		{
			/* Check if this is not the last segment file
			 */
			if( ( internal_handle->media_values->media_size == 0 )
			 || ( internal_handle->write->input_write_count < (ssize64_t) internal_handle->media_values->media_size ) )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				notify_verbose_printf(
				 "%s: closing segment file with segment number: %" PRIu16 ".\n",
				 function,
				 segment_number );
#endif

				/* Finish and close the segment file
				 */
				write_count = libewf_segment_file_write_close(
					       internal_handle->segment_table->segment_file_handle[ segment_number ],
					       internal_handle->file_io_pool,
					       segment_number,
					       internal_handle->write->segment_amount_of_chunks,
					       0,
					       internal_handle->hash_sections,
					       internal_handle->hash_values,
					       internal_handle->media_values,
					       internal_handle->sessions,
					       internal_handle->acquiry_errors,
					       internal_handle->compression_level,
					       internal_handle->format,
					       internal_handle->ewf_format,
					       &( internal_handle->write->data_section ),
				               error );

				if( write_count == -1 )
				{
					libewf_error_set(
					 error,
					 LIBEWF_ERROR_DOMAIN_OUTPUT,
					 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
					 "%s: unable to close segment file.\n",
					 function );

					return( -1 );
				}
				total_write_count                   += write_count;
				internal_handle->write->write_count += write_count;
			}
		}
	}
	return( total_write_count );
}

/* Writes an existing chunk of data in EWF format from a buffer at the current offset
 * The necessary settings of the write values must have been made
 * Returns the amount of data bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_raw_write_chunk_existing(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         ewf_crc_t chunk_crc,
         int8_t write_crc,
         libewf_error_t **error )
{
	libewf_list_element_t *last_list_element          = NULL;
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_raw_write_chunk_existing";
	off64_t last_section_start_offset                 = 0;
	off64_t segment_file_offset                       = 0;
	ssize_t total_write_count                         = 0;
	ssize_t write_count                               = 0;
	uint16_t segment_number                           = 0;
	uint8_t segment_file_type                         = 0;
	uint8_t no_section_append                         = 0;
	int result                                        = 0;

	if( internal_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid delta segment table - missing segment file handles.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->chunk_offset == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid offset table - missing chunk offsets.\n",
		 function );

		return( -1 );
	}
	if( chunk >= internal_handle->offset_table->amount_of_chunk_offsets )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_OUT_OF_RANGE,
		 "%s: invalid chunk value out of range.\n",
		 function );

		return( -1 );
	}
	if( chunk_buffer == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid chunk buffer.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_ZERO_OR_LESS,
		 "%s: invalid chunk size value is zero.\n",
		 function );

		return( -1 );
	}
	if( chunk_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( is_compressed != 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_CONFLICTING_VALUE,
		 "%s: chunk compression cannot be used.\n",
		 function );

		return( -1 );
	}
	segment_file_handle = internal_handle->offset_table->chunk_offset[ chunk ].segment_file_handle;

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	segment_file_type = segment_file_handle->file_type;

#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: writing delta chunk: %" PRIu32 " of size: %" PRIzu " with data of size: %" PRIzd ".\n",
	 function,
	 ( chunk + 1 ),
	 chunk_size,
	 chunk_data_size );
#endif

	/* Check if the chunk already exists in a delta segment file
	 */
	if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_DWF )
	{
		/* Write the chunk to the last delta segment file
		 */
		segment_number = internal_handle->delta_segment_table->amount - 1;

		/* Check if a segment number is valid
		 */
		if( segment_number > internal_handle->delta_segment_table->amount )
		{
			libewf_error_set(
			 error,
		 	 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid segment number value out of range.\n",
			 function );

			return( -1 );
		}
		/* Check if a new delta segment file should be created
		 */
		if( segment_number != 0 )
		{
			segment_file_handle = internal_handle->delta_segment_table->segment_file_handle[ segment_number ];

			if( segment_file_handle == NULL )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid segment file.\n",
				 function );

				return( -1 );
			}
			if( segment_file_handle->section_list == NULL )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid segment file - missing section list.\n",
				 function );

				return( -1 );
			}
			last_list_element = segment_file_handle->section_list->last;

			if( last_list_element == NULL )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing last section list element.\n",
				 function );

				return( -1 );
			}
			if( last_list_element->value == NULL )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid last section list element element - missing values.\n",
				 function );

				return( -1 );
			}
			last_section_start_offset = ( (libewf_section_list_values_t * ) last_list_element->value )->start_offset;

			if( libewf_file_io_pool_get_offset(
			     internal_handle->file_io_pool,
			     segment_file_handle->file_io_pool_entry,
			     &segment_file_offset ) != 1 )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve current offset in segment file.\n",
				 function );

				return( -1 );
			}
			/* Make sure the current segment file offset points to the start of the last section
			 */
			if( ( segment_file_offset != last_section_start_offset )
			 && ( libewf_file_io_pool_seek_offset(
			       internal_handle->file_io_pool,
			       segment_file_handle->file_io_pool_entry,
			       last_section_start_offset,
			       SEEK_SET ) == -1 ) )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_OUTPUT,
				 LIBEWF_OUTPUT_ERROR_SEEK_FAILED,
				 "%s: cannot find offset: %" PRIjd ".\n",
				 function,
				 last_section_start_offset );

				return( -1 );
			}
			segment_file_offset = last_section_start_offset + chunk_size + sizeof( ewf_crc_t ) + sizeof( ewf_section_t );

			/* Check if chunk fits in exisiting delta segment file
			 */
			if( segment_file_offset > (off64_t) internal_handle->write->segment_file_size )
			{
				/* Make sure to write a next section in the the previous delta segment file
				 * The segment file offset is updated by the function
				 */
				write_count = libewf_segment_file_write_last_section(
					       segment_file_handle,
					       internal_handle->file_io_pool,
					       0,
					       internal_handle->format,
					       internal_handle->ewf_format );

				if( write_count == -1 )
				{
					libewf_error_set(
					 error,
					 LIBEWF_ERROR_DOMAIN_OUTPUT,
					 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
					 "%s: unable to write last section.\n",
					 function );

					return( -1 );
				}
				total_write_count += write_count;
				result             = 0;
			}
			else
			{
				if( libewf_list_remove_element(
				     segment_file_handle->section_list,
				     last_list_element ) != 1 )
				{
					libewf_error_set(
					 error,
					 LIBEWF_ERROR_DOMAIN_RUNTIME,
					 LIBEWF_RUNTIME_ERROR_REMOVE_FAILED,
					 "%s: unable to remove last section from list.\n",
					 function );

					return( -1 );
				}
				if( libewf_section_list_values_free(
				     last_list_element->value ) != 1 )
				{
					libewf_error_set(
					 error,
					 LIBEWF_ERROR_DOMAIN_RUNTIME,
					 LIBEWF_RUNTIME_ERROR_FREE_FAILED,
					 "%s: unable to free last section values.\n",
					 function );

					memory_free(
					 last_list_element );

					return( -1 );
				}
				memory_free(
				 last_list_element );

				result = 1;
			}
		}
		else
		{
			result = 0;
		}
		if( result == 0 )
		{
			segment_number++;

			/* Create a new delta segment file
			 */
			if( libewf_segment_table_create_segment_file(
			     internal_handle->delta_segment_table,
			     segment_number,
			     internal_handle->file_io_pool,
			     internal_handle->write->maximum_amount_of_segments,
			     LIBEWF_SEGMENT_FILE_TYPE_DWF,
			     internal_handle->format,
			     internal_handle->ewf_format ) != 1 )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_OUTPUT,
				 LIBEWF_OUTPUT_ERROR_OPEN_FAILED,
				 "%s: unable to create delta segment file for segment: %" PRIu16 ".\n",
				 function,
				 segment_number );

				return( -1 );
			}
			segment_file_handle = internal_handle->delta_segment_table->segment_file_handle[ segment_number ];

			/* Write the start of the segment file
			 * like the file header, the header, volume and/or data section, etc.
			 */
			write_count = libewf_segment_file_write_start(
				       segment_file_handle,
				       internal_handle->file_io_pool,
				       segment_number,
				       LIBEWF_SEGMENT_FILE_TYPE_DWF,
				       internal_handle->media_values,
				       internal_handle->header_sections,
				       internal_handle->compression_level,
				       internal_handle->format,
				       internal_handle->ewf_format,
			               &( internal_handle->write->data_section ) );

			if( write_count == -1 )
			{
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_OUTPUT,
				 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
				 "%s: unable to write segment file start.\n",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
	}
	else
	{
		segment_file_offset = internal_handle->offset_table->chunk_offset[ chunk ].file_offset - sizeof( ewfx_delta_chunk_header_t ) - sizeof( ewf_section_t );

		if( libewf_file_io_pool_seek_offset(
		     internal_handle->file_io_pool,
		     segment_file_handle->file_io_pool_entry,
		     segment_file_offset,
		     SEEK_SET ) == -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_OUTPUT,
			 LIBEWF_OUTPUT_ERROR_SEEK_FAILED,
			 "%s: unable to seek chunk offset.\n",
			 function );

			return( -1 );
		}
		no_section_append = 1;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libewf_file_io_pool_get_offset(
	     internal_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &segment_file_offset ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.\n",
		 function );

		return( -1 );
	}
	notify_verbose_printf(
	 "%s: writing chunk at offset: %" PRIjd ".\n",
	 function,
	 segment_file_offset );
#endif

	/* Write the chunk in the delta segment file
	 */
	write_count = libewf_segment_file_write_delta_chunk(
		       segment_file_handle,
		       internal_handle->file_io_pool,
		       internal_handle->offset_table,
		       chunk,
		       chunk_buffer,
		       chunk_size,
		       &chunk_crc,
	               write_crc,
	               no_section_append );

	if( write_count == -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_OUTPUT,
		 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
		 "%s: unable to write delta chunk.\n",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( no_section_append == 0 )
	{
		/* Write the last section
		 * The segment file offset is updated by the function
		 */
		write_count = libewf_segment_file_write_last_section(
			       segment_file_handle,
			       internal_handle->file_io_pool,
			       1,
			       internal_handle->format,
			       internal_handle->ewf_format );

		if( write_count == -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_OUTPUT,
			 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
			 "%s: unable to write last section.\n",
			 function );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Writes a new chunk of data in EWF format from a buffer at the current offset
 * The necessary settings of the write values must have been made
 * Returns the amount of data bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_chunk_data_new(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         int8_t force_write,
         libewf_error_t **error )
{
	uint8_t *chunk_data               = NULL;
	static char *function             = "libewf_write_chunk_data_new";
	ewf_crc_t chunk_crc               = 0;
	ssize_t chunk_data_size           = 0;
	ssize_t write_count               = 0;
	size_t write_size                 = 0;
	size_t read_size                  = 0;
	size_t remaining_chunk_size       = 0;
	size_t compressed_chunk_data_size = 0;
	int chunk_cache_data_used         = 0;
	int8_t is_compressed              = 0;
	int8_t write_crc                  = 0;

	if( internal_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( -1 );
	}
	/* Check if the write was already finalized
	 */
	if( internal_handle->write->write_finalized == 1 )
	{
		return( 0 );
	}
	if( buffer == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( buffer == internal_handle->chunk_cache->compressed )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid buffer - same as chunk cache compressed.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: writing buffer of size: %" PRIzu " with data of size: %" PRIzd ".\n",
	 function,
	 buffer_size,
	 data_size );
#endif

	/* Determine the size of data to read
	 */
	if( data_size < (size_t) internal_handle->media_values->chunk_size )
	{
		read_size = (size_t) data_size;
	}
	else
	{
		read_size = internal_handle->media_values->chunk_size;
	}
	if( read_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM,
		 "%s: invalid read size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* Directy write the buffer if
	 *  the chunk cache data is directly being passed (for finalize)
	 *  or no data was previously copied into the chunk cache
	 *   and the buffer contains the necessary amount of bytes to fill a chunk
	 */
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( ( internal_handle->chunk_cache->offset == 0 )
	 && ( data_size >= (size_t) internal_handle->media_values->chunk_size ) ) )
	{
		chunk_data = (uint8_t *) buffer;
		write_size = read_size;
	}
	else
	{
		/* Check if data is present in the chunk cache
		 * and calculate the amount of data to read from the buffer
		 */
		remaining_chunk_size = internal_handle->media_values->chunk_size
		                     - internal_handle->chunk_cache->offset;

		if( read_size > (size_t) remaining_chunk_size )
		{
			read_size = remaining_chunk_size;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: reading %" PRIzu " bytes from buffer.\n",
		 function,
		 read_size );
#endif

		if( memory_copy(
		     &internal_handle->chunk_cache->data[ internal_handle->chunk_cache->offset ],
		     buffer,
		     read_size ) == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to write data to chunk cache.\n",
			 function );

			return( -1 );
		}
		internal_handle->chunk_cache->chunk  = chunk;
		internal_handle->chunk_cache->amount = internal_handle->chunk_cache->offset + read_size;

		/* Adjust the chunk cache offset
		 */
		if( internal_handle->chunk_cache->amount == internal_handle->media_values->chunk_size )
		{
			internal_handle->chunk_cache->offset = 0;
		}
		else if( internal_handle->chunk_cache->amount < internal_handle->media_values->chunk_size )
		{
			internal_handle->chunk_cache->offset = internal_handle->chunk_cache->amount;
		}
		else
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid chunk cache amount value out of range.\n",
			 function );

			return( -1 );
		}
		chunk_data = internal_handle->chunk_cache->data;
		write_size = internal_handle->chunk_cache->amount;
	}
	if( ( write_size == internal_handle->media_values->chunk_size )
	 || ( ( internal_handle->media_values->media_size != 0 )
	  && ( ( internal_handle->write->input_write_count + (ssize64_t) write_size ) == (ssize64_t) internal_handle->media_values->media_size ) )
	 || ( force_write != 0 ) )
	{
		chunk_cache_data_used = (int) ( chunk_data == internal_handle->chunk_cache->data );

		/* The compressed data size contains the maximum allowed buffer size
		 */
		compressed_chunk_data_size = internal_handle->chunk_cache->allocated_size;

		/* Compress the chunk if necessary and determine its CRC
		 */
		chunk_data_size = libewf_write_process_chunk_data(
				   internal_handle->chunk_cache,
				   internal_handle->media_values,
				   internal_handle->compression_level,
				   internal_handle->compress_empty_block,
				   internal_handle->ewf_format,
				   chunk_data,
				   write_size,
				   internal_handle->chunk_cache->compressed,
				   &compressed_chunk_data_size,
				   &is_compressed,
				   &chunk_crc,
				   &write_crc,
		                   error );

		if( chunk_data_size <= -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_CONVERSION,
			 LIBEWF_CONVERSION_ERROR_OUTPUT_FAILED,
			 "%s: unable to process chunk data.\n",
			 function );

			return( -1 );
		}
		/* Make sure to update the chunk_data pointer if
		 * internal_handle->chunk_cache->data has been reallocated by
		 * libewf_write_process_chunk_data()
		 */
		if( ( chunk_cache_data_used == 1 )
		 && ( chunk_data != internal_handle->chunk_cache->data ) )
		{
			chunk_data = internal_handle->chunk_cache->data;
		}
		if( is_compressed != 0 )
		{
			chunk_data = internal_handle->chunk_cache->compressed;
		}
		write_count = libewf_raw_write_chunk_new(
		               internal_handle,
		               chunk,
		               chunk_data,
		               chunk_data_size,
		               write_size,
		               is_compressed,
		               chunk_crc,
		               write_crc,
		               error );

		if( write_count <= -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_OUTPUT,
			 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
			 "%s: unable to write chunk.\n",
			 function );

			return( -1 );
		}
	}
	/* Report the amount of chunk data written
	 */
	return( (ssize_t) read_size );
}

/* Writes an existing chunk of data in EWF format from a buffer at the current offset
 * The necessary settings of the write values must have been made
 * Returns the amount of data bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_chunk_data_existing(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint32_t chunk_offset,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         libewf_error_t **error )
{
	uint8_t *chunk_data         = NULL;
	static char *function       = "libewf_write_chunk_data_existing";
	ewf_crc_t chunk_crc         = 0;
	size_t remaining_chunk_size = 0;
	ssize_t read_count          = 0;
	ssize_t write_count         = 0;
	size_t write_size           = 0;

	if( internal_handle == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( chunk >= internal_handle->offset_table->amount_of_chunk_offsets )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_OUT_OF_RANGE,
		 "%s: invalid chunk value out of range.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid buffer - same as chunk cache.\n",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: writing buffer of size: %" PRIzu " with data of size: %" PRIzd ".\n",
	 function,
	 buffer_size,
	 data_size );
#endif

	/* Check if the data size exceeds the chunk size
	 */
	if( data_size > (size_t) internal_handle->media_values->chunk_size )
	{
		data_size = (size_t) internal_handle->media_values->chunk_size;
	}
	/* Check if the data in the buffer aligns with a chunk
	 */
	if( ( chunk_offset == 0 )
	 && ( data_size == internal_handle->media_values->chunk_size ) )
	{
		chunk_data = buffer;
		write_size = (size_t) internal_handle->media_values->chunk_size;
	}
	else
	{
		/* Read the chunk data into the chunk cache
		 */
		read_count = libewf_read_chunk_data(
		              internal_handle,
		              chunk,
		              0,
		              internal_handle->chunk_cache->data,
		              internal_handle->chunk_cache->allocated_size,
		              error );

		if( read_count <= -1 )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_INPUT,
			 LIBEWF_INPUT_ERROR_READ_FAILED,
			 "%s: unable to read data from chunk.\n",
			 function );

			return( -1 );
		}
		internal_handle->current_chunk_offset = chunk_offset;

		remaining_chunk_size = internal_handle->media_values->chunk_size
		                     - internal_handle->current_chunk_offset;

		if( data_size > (size_t) remaining_chunk_size )
		{
			data_size = remaining_chunk_size;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: updating data in chunk: %" PRIu32 " at offset: %" PRIu32 " with size: %" PRIzd ".\n",
		 function,
		 ( chunk + 1 ),
		 chunk_offset,
		 data_size );
#endif

		/* Update the chunk data
		 */
		if( memory_copy(
		     &( internal_handle->chunk_cache->data[ chunk_offset ] ),
		     buffer,
		     data_size ) == NULL )
		{
			libewf_error_set(
			 error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to update data in chunk: %" PRIu32 ".\n",
			 function,
			 (chunk + 1 ) );

			return( -1 );
		}
		chunk_data = internal_handle->chunk_cache->data;
		write_size = (size_t) read_count;
	}
	/* Calculate the new CRC
         */
        chunk_crc = ewf_crc_calculate(
	             chunk_data,
	             write_size,
	             1 );

	write_count = libewf_raw_write_chunk_existing(
	               internal_handle,
	               chunk,
	               chunk_data,
	               write_size,
	               write_size,
	               0,
	               chunk_crc,
	               1,
	               error );

	if( write_count <= -1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_OUTPUT,
		 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
		 "%s: unable to write delta chunk.\n",
		 function );

		return( -1 );
	}
	/* Report the amount of data written
	 */
	return( (ssize_t) data_size );
}

/* Prepares a buffer with chunk data before writing according to the handle settings
 * intended for raw write
 * The buffer size cannot be larger than the chunk size
 * The function sets the chunk crc, is compressed and write crc values
 * Returns the resulting chunk size or -1 on error
 */
ssize_t libewf_raw_write_prepare_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         void *compressed_buffer,
         size_t *compressed_buffer_size,
         int8_t *is_compressed,
         uint32_t *chunk_crc,
         int8_t *write_crc )
{
	libewf_error_t *error                     = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_raw_write_prepare_buffer";
	ssize_t chunk_data_size                   = 0;

	if( handle == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid buffer - same as chunk cache.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( compressed_buffer == internal_handle->chunk_cache->data )
	 || ( compressed_buffer == internal_handle->chunk_cache->compressed ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid compressed buffer - same as chunk cache.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	chunk_data_size = libewf_write_process_chunk_data(
	                   internal_handle->chunk_cache,
	                   internal_handle->media_values,
	                   internal_handle->compression_level,
	                   internal_handle->compress_empty_block,
	                   internal_handle->ewf_format,
	                   (uint8_t *) buffer,
	                   buffer_size,
	                   (uint8_t *) compressed_buffer,
	                   compressed_buffer_size,
	                   is_compressed,
	                   (ewf_crc_t *) chunk_crc,
	                   write_crc,
	                   &error );

	if( chunk_data_size <= -1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_CONVERSION,
		 LIBEWF_CONVERSION_ERROR_OUTPUT_FAILED,
		 "%s: unable to prepare chunk data.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	return( chunk_data_size );
}

/* Writes 'raw' data in EWF format from a buffer at the current offset
 * the necessary settings of the write values must have been made
 * size contains the size of the data within the buffer while
 * data size contains the size of the actual input data
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_raw_write_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         int8_t is_compressed,
         uint32_t chunk_crc,
         int8_t write_crc )
{
	libewf_error_t *error                     = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_raw_write_buffer";
	ssize_t write_count                       = 0;

	if( handle == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( internal_handle->write->values_initialized == 0 )
	 && ( libewf_internal_handle_write_initialize( internal_handle ) != 1 ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize write values.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid buffer.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid buffer - same as chunk cache.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( data_size > internal_handle->media_values->chunk_size )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_OUT_OF_RANGE,
		 "%s: data size cannot be larger than maximum chunk size.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: writing chunk: %d of total: %d.\n",
	 function,
	 ( internal_handle->current_chunk + 1 ),
	 internal_handle->offset_table->amount_of_chunk_offsets );
	notify_verbose_printf(
	 "%s: writing buffer of size: %" PRIzd " with data of size: %" PRIzd ".\n",
	 function,
	 buffer_size,
	 data_size );
#endif

	/* Check if chunk has already been created within a segment file
	 */
	if( ( internal_handle->current_chunk < internal_handle->offset_table->amount_of_chunk_offsets )
	 && ( internal_handle->offset_table->chunk_offset != NULL )
	 && ( internal_handle->offset_table->chunk_offset[ internal_handle->current_chunk ].segment_file_handle != NULL ) )
	{
		if( internal_handle->read == NULL )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: cannot rewrite existing chunk.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		write_count = libewf_raw_write_chunk_existing(
		               internal_handle,
		               internal_handle->current_chunk,
		               buffer,
		               buffer_size,
		               data_size,
		               is_compressed,
		               (ewf_crc_t) chunk_crc,
		               write_crc,
		               &error );
	}
	else
	{
		write_count = libewf_raw_write_chunk_new(
		               internal_handle,
		               internal_handle->current_chunk,
		               buffer,
		               buffer_size,
		               data_size,
		               is_compressed,
		               (ewf_crc_t) chunk_crc,
		               write_crc,
		               &error );
	}
	if( write_count <= -1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_OUTPUT,
		 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
		 "%s: unable to write raw chunk data.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	internal_handle->current_chunk += 1;

	return( (ssize_t) buffer_size );
}

/* Writes data in EWF format from a buffer at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t size )
{
	libewf_error_t *error                     = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_write_buffer";
	ssize_t total_write_count                 = 0;
	ssize_t write_count                       = 0;
	size_t chunk_data_size                    = 0;

	if( handle == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( internal_handle->write->values_initialized == 0 )
	 && ( libewf_internal_handle_write_initialize(
	       internal_handle ) != 1 ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize write values.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid buffer.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid buffer - same as chunk cache.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents multiple reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media_values->chunk_size + sizeof( ewf_crc_t );

	if( buffer == internal_handle->chunk_cache->compressed )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: chunk cache compressed cannot be used as buffer.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: reallocating chunk data size: %" PRIzu ".\n",
		 function,
		 chunk_data_size );
#endif

		if( libewf_chunk_cache_resize(
		     internal_handle->chunk_cache,
		     chunk_data_size ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk cache.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
	}
	while( size > 0 )
	{
		/* Check if chunk has already been created within a segment file
		 */
		if( ( internal_handle->current_chunk < internal_handle->offset_table->amount_of_chunk_offsets )
		 && ( internal_handle->offset_table->chunk_offset != NULL )
		 && ( internal_handle->offset_table->chunk_offset[ internal_handle->current_chunk ].segment_file_handle != NULL ) )
		{
			if( internal_handle->read == NULL )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: cannot rewrite existing chunk.\n",
				 function );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				return( -1 );
			}
			write_count = libewf_write_chunk_data_existing(
			               internal_handle,
			               internal_handle->current_chunk,
			               internal_handle->current_chunk_offset,
			               (void *) &( (uint8_t *) buffer )[ total_write_count ],
			               size,
			               size,
			               &error );
		}
		else
		{
			write_count = libewf_write_chunk_data_new(
			               internal_handle,
			               internal_handle->current_chunk,
			               (void *) &( (uint8_t *) buffer )[ total_write_count ],
			               size,
			               size,
			               0,
			               &error );
		}
		if( write_count <= -1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_OUTPUT,
			 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
			 "%s: unable to write data from buffer.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		else if( write_count == 0 )
		{
			break;
		}
		total_write_count += write_count;
		size              -= write_count;

		internal_handle->current_chunk_offset += (uint32_t) write_count;

		if( internal_handle->current_chunk_offset == internal_handle->media_values->chunk_size )
		{
			internal_handle->current_chunk_offset = 0;
			internal_handle->current_chunk       += 1;
		}
		else if( internal_handle->current_chunk_offset > internal_handle->media_values->chunk_size )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid current chunk offset: %" PRIu32 " larger than chunk size: %" PRIu32 ".\n",
			 function,
			 internal_handle->current_chunk_offset,
			 internal_handle->media_values->chunk_size );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		if( internal_handle->abort == 1 )
		{
			break;
		}
	}
	return( total_write_count );
}

/* Writes data in EWF format from a buffer at an specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         off64_t offset )
{
	libewf_error_t *error = NULL;
	static char *function = "libewf_write_random";
	ssize_t write_count   = 0;

	if( libewf_seek_offset(
	     handle,
	     offset ) == -1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_OUTPUT,
		 LIBEWF_OUTPUT_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	write_count = libewf_write_buffer(
	               handle,
	               buffer,
	               size );

	if( write_count <= -1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_OUTPUT,
		 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	return( write_count );
}

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after write from stream
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_write_finalize(
         libewf_handle_t *handle )
{
	libewf_error_t *error                             = NULL;
	libewf_internal_handle_t *internal_handle         = NULL;
	libewf_list_element_t *list_element               = NULL;
	libewf_section_list_values_t *section_list_values = NULL;
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	void *reallocation                                = NULL;
	static char *function                             = "libewf_write_finalize";
	ssize_t write_count_finalize                      = 0;
	ssize_t write_count                               = 0;
	uint16_t segment_table_iterator                   = 0;
	uint16_t segment_number                           = 0;

	if( handle == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID,
		 "%s: invalid handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( internal_handle->segment_table->segment_file_handle == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid segment table - missing segment file handles\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	/* No need for finalization in R or RW mode
	 */
	if( internal_handle->read != NULL )
	{
		return( 0 );
	}
	if( internal_handle->write->write_finalized == 1 )
	{
		return( 0 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: finalizing write.\n",
	 function );
#endif

	/* Write data remaining in the chunk cache to file
	 */
	if( ( internal_handle->current_chunk_offset != 0 )
	 && ( internal_handle->chunk_cache->amount != 0 )
	 && ( internal_handle->chunk_cache->offset != 0 )
	 && ( ( internal_handle->media_values->media_size == 0 )
	  || ( internal_handle->write->input_write_count < (ssize64_t) internal_handle->media_values->media_size ) ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: writing chunk remainder at offset: %" PRIu32 " with size: %" PRIu32 "\n",
		 function,
		 internal_handle->current_chunk_offset,
		 internal_handle->chunk_cache->amount );
#endif

		write_count = libewf_write_chunk_data_new(
		               internal_handle,
		               internal_handle->current_chunk,
		               internal_handle->chunk_cache->data,
		               internal_handle->chunk_cache->amount,
		               internal_handle->chunk_cache->amount,
		               1,
		               &error );

		if( write_count == -1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_OUTPUT,
			 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
			 "%s: unable to write remaining chunk data.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		write_count_finalize += write_count;
	}
	segment_number = internal_handle->segment_table->amount - 1;

	/* Check if no segment file was created
	 */
	if( segment_number == 0 )
	{
		return( 0 );
	}
	segment_file_handle = internal_handle->segment_table->segment_file_handle[ segment_number ];

	if( segment_file_handle == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file: %" PRIu16 ".\n",
		 function,
		 segment_number );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	/* Check if the last segment file is still open for writing
	 */
	if( segment_file_handle->write_open != 0 )
	{
		/* Check if chunks section needs to be corrected
		 */
		if( internal_handle->write->chunks_section_offset != 0 )
		{
			/* Correct the offset,
			size in the chunks section
			 */
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf(
			 "%s: correcting chunks section.\n",
			 function );
#endif

			if( internal_handle->write->amount_of_table_offsets < internal_handle->write->section_amount_of_chunks )
			{
				reallocation = memory_reallocate(
				                internal_handle->write->table_offsets,
				                sizeof( ewf_table_offset_t ) * internal_handle->write->section_amount_of_chunks );

				if( reallocation == NULL )
				{
					libewf_error_set(
					 &error,
					 LIBEWF_ERROR_DOMAIN_MEMORY,
					 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create table offsets.\n",
					 function );

					libewf_error_backtrace_notify(
					 error );
					libewf_error_free(
					 &error );

					return( -1 );
				}
				internal_handle->write->table_offsets           = (ewf_table_offset_t *) reallocation;
				internal_handle->write->amount_of_table_offsets = internal_handle->write->section_amount_of_chunks;
			}
			write_count = libewf_segment_file_write_chunks_correction(
				       segment_file_handle,
				       internal_handle->file_io_pool,
				       internal_handle->offset_table,
			               internal_handle->write->table_offsets,
			               internal_handle->write->amount_of_table_offsets,
				       internal_handle->write->chunks_section_offset,
				       (size64_t) internal_handle->write->chunks_section_write_count,
				       internal_handle->write->amount_of_chunks,
				       internal_handle->write->section_amount_of_chunks,
				       internal_handle->format,
				       internal_handle->ewf_format );

			if( write_count == -1 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_OUTPUT,
				 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
				 "%s: unable to correct chunks section.\n",
				 function );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				return( -1 );
			}
			write_count_finalize                += write_count;
			internal_handle->write->write_count += write_count;
		}
		/* Close the segment file
		 */
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: closing last segment file.\n",
		 function );
#endif

		write_count = libewf_segment_file_write_close(
		               segment_file_handle,
		               internal_handle->file_io_pool,
		               segment_number,
		               internal_handle->write->segment_amount_of_chunks,
		               1,
		               internal_handle->hash_sections,
		               internal_handle->hash_values,
		               internal_handle->media_values,
		               internal_handle->sessions,
		               internal_handle->acquiry_errors,
		               internal_handle->compression_level,
		               internal_handle->format,
		               internal_handle->ewf_format,
		               &( internal_handle->write->data_section ),
		               &error );

		if( write_count == -1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_OUTPUT,
			 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
			 "%s: unable to close segment file.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		write_count_finalize                += write_count;
		internal_handle->write->write_count += write_count;
	}
	/* Correct the media values if streamed write was used
	 */
	if( internal_handle->media_values->media_size == 0 )
	{
		/* Calculate the media values
		 */
		internal_handle->media_values->amount_of_chunks  = internal_handle->write->amount_of_chunks;
		internal_handle->media_values->amount_of_sectors = (uint32_t) ( internal_handle->write->input_write_count
		                                                 / internal_handle->media_values->bytes_per_sector );
		internal_handle->media_values->media_size        = (size32_t) internal_handle->write->input_write_count;

		/* Correct the segment files
		 */
		for( segment_table_iterator = 1; segment_table_iterator < internal_handle->segment_table->amount; segment_table_iterator++ )
		{
			segment_file_handle = internal_handle->segment_table->segment_file_handle[ segment_table_iterator ];

			if( segment_file_handle == NULL )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid segment file: %" PRIu16 ".\n",
				 function,
				 segment_table_iterator );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				return( -1 );
			}
			if( segment_file_handle->section_list == NULL )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid segment file: %" PRIu16 " - missing section list.\n",
				 function,
				 segment_table_iterator );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				return( -1 );
			}
			if( segment_file_handle->section_list->first == NULL )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid segment file: %" PRIu16 " - invalid section list - missing entries.\n",
				 function,
				 segment_table_iterator );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				return( -1 );
			}
			if( segment_file_handle->file_io_pool_entry == -1 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid file io pool entry for segment file: %" PRIu16 ".\n",
				 function,
				 segment_table_iterator );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				return( -1 );
			}
			if( libewf_file_io_pool_open(
			     internal_handle->file_io_pool,
			     segment_file_handle->file_io_pool_entry,
			     FILE_IO_O_RDWR ) != 1 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_OUTPUT,
				 LIBEWF_OUTPUT_ERROR_OPEN_FAILED,
				 "%s: unable to open segment file: %" PRIu16 ".\n",
				 function,
				 segment_table_iterator );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				return( -1 );
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf(
			 "%s: correcting segment file: %" PRIu16 ".\n",
			 function,
			 segment_table_iterator );
#endif

			list_element = segment_file_handle->section_list->first;

			while( list_element != NULL )
			{
				section_list_values = (libewf_section_list_values_t *) list_element->value;

				if( section_list_values == NULL )
				{
					libewf_error_set(
					 &error,
					 LIBEWF_ERROR_DOMAIN_RUNTIME,
					 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing section list values for segment file: %" PRIu16 ".\n",
					 function,
					 segment_table_iterator );

					libewf_error_backtrace_notify(
					 error );
					libewf_error_free(
					 &error );

					return( -1 );
				}
				if( memory_compare(
				     section_list_values->type,
				     "volume",
				     6 ) == 0 )
				{
#if defined( HAVE_VERBOSE_OUTPUT )
					notify_verbose_printf(
					 "%s: correcting volume section.\n",
					 function );
#endif

					if( libewf_file_io_pool_seek_offset(
					     internal_handle->file_io_pool,
					     segment_file_handle->file_io_pool_entry,
					     section_list_values->start_offset,
					     SEEK_SET ) == -1 )
					{
						libewf_error_set(
						 &error,
						 LIBEWF_ERROR_DOMAIN_OUTPUT,
						 LIBEWF_OUTPUT_ERROR_SEEK_FAILED,
						 "%s: unable to find offset to correct volume section.\n",
						 function );

						libewf_error_backtrace_notify(
						 error );
						libewf_error_free(
						 &error );

						return( -1 );
					}
					if( internal_handle->ewf_format == EWF_FORMAT_S01 )
					{
						/* Write volume (SMART) section
						 */
						write_count = libewf_section_volume_s01_write(
							       internal_handle->file_io_pool,
							       segment_file_handle,
							       internal_handle->media_values,
							       internal_handle->format,
							       1 );
					}
					else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
					{
						/* Write volume section
						 */
						write_count = libewf_section_volume_e01_write(
							       internal_handle->file_io_pool,
							       segment_file_handle,
							       internal_handle->media_values,
							       internal_handle->compression_level,
							       internal_handle->format,
							       1 );
					}
					else
					{
						write_count = -1;
					}
					if( write_count == -1 )
					{
						libewf_error_set(
						 &error,
						 LIBEWF_ERROR_DOMAIN_OUTPUT,
						 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
						 "%s: unable to correct volume section.\n",
						 function );

						libewf_error_backtrace_notify(
						 error );
						libewf_error_free(
						 &error );

						return( -1 );
					}
				}
				else if( memory_compare(
				          section_list_values->type,
				          "data",
				          4 ) == 0 )
				{
#if defined( HAVE_VERBOSE_OUTPUT )
					notify_verbose_printf(
					 "%s: correcting data section.\n",
					 function );
#endif

					if( libewf_file_io_pool_seek_offset(
					     internal_handle->file_io_pool,
					     segment_file_handle->file_io_pool_entry,
					     section_list_values->start_offset,
					     SEEK_SET ) == -1 )
					{
						libewf_error_set(
						 &error,
						 LIBEWF_ERROR_DOMAIN_OUTPUT,
						 LIBEWF_OUTPUT_ERROR_SEEK_FAILED,
						 "%s: unable to find offset to data volume section.\n",
						 function );

						libewf_error_backtrace_notify(
						 error );
						libewf_error_free(
						 &error );

						return( -1 );
					}
					/* Write data section
					 */
					write_count = libewf_section_data_write(
					               internal_handle->file_io_pool,
					               segment_file_handle,
					               internal_handle->media_values,
					               internal_handle->compression_level,
					               internal_handle->format,
					               &( internal_handle->write->data_section ),
					               1 );

					if( write_count == -1 )
					{
						libewf_error_set(
						 &error,
						 LIBEWF_ERROR_DOMAIN_OUTPUT,
						 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
						 "%s: unable to correct data section.\n",
						 function );

						libewf_error_backtrace_notify(
						 error );
						libewf_error_free(
						 &error );

						return( -1 );
					}
				}
				/* The last segment file should be terminated with a done section and not with a next section
				 */
				else if( ( segment_table_iterator == ( internal_handle->segment_table->amount - 1 ) )
				      && ( memory_compare(
				            section_list_values->type,
				            "next",
				            4 ) == 0 ) )
		
				{
#if defined( HAVE_VERBOSE_OUTPUT )
					notify_verbose_printf(
					 "%s: correcting next section - closing last segment file.\n",
					 function );
#endif

					if( libewf_file_io_pool_seek_offset(
					     internal_handle->file_io_pool,
					     segment_file_handle->file_io_pool_entry,
					     section_list_values->start_offset,
					     SEEK_SET ) == -1 )
					{
						libewf_error_set(
						 &error,
						 LIBEWF_ERROR_DOMAIN_OUTPUT,
						 LIBEWF_OUTPUT_ERROR_SEEK_FAILED,
						 "%s: unable to find offset to data volume section.\n",
						 function );

						libewf_error_backtrace_notify(
						 error );
						libewf_error_free(
						 &error );

						return( -1 );
					}
					write_count = libewf_segment_file_write_close(
						       segment_file_handle,
						       internal_handle->file_io_pool,
						       segment_number,
						       internal_handle->write->segment_amount_of_chunks,
						       1,
						       internal_handle->hash_sections,
						       internal_handle->hash_values,
						       internal_handle->media_values,
						       internal_handle->sessions,
						       internal_handle->acquiry_errors,
						       internal_handle->compression_level,
						       internal_handle->format,
						       internal_handle->ewf_format,
						       &( internal_handle->write->data_section ),
					               &error );

					if( write_count == -1 )
					{
						libewf_error_set(
						 &error,
						 LIBEWF_ERROR_DOMAIN_OUTPUT,
						 LIBEWF_OUTPUT_ERROR_WRITE_FAILED,
						 "%s: unable to close segment file.\n",
						 function );

						libewf_error_backtrace_notify(
						 error );
						libewf_error_free(
						 &error );

						return( -1 );
					}
				}
				list_element = list_element->next;
			}
			if( libewf_file_io_pool_close(
			     internal_handle->file_io_pool,
			     segment_file_handle->file_io_pool_entry ) != 0 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_OUTPUT,
				 LIBEWF_OUTPUT_ERROR_CLOSE_FAILED,
				 "%s: unable to close segment file: %" PRIu16 ".\n",
				 function,
				 segment_table_iterator );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				return( -1 );
			}
		}
	}
	internal_handle->write->write_finalized = 1;

	return( write_count_finalize );
}

