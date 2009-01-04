/*
 * libewf file writing
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

#include <errno.h>

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include <libewf/libewf_definitions.h>

#include "libewf_char.h"
#include "libewf_chunk_cache.h"
#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_file.h"
#include "libewf_notify.h"
#include "libewf_offset_table.h"
#include "libewf_read.h"
#include "libewf_section.h"
#include "libewf_section_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_string.h"
#include "libewf_write.h"

#include "ewf_char.h"
#include "ewf_compress.h"
#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_file_header.h"

/* Calculates an estimate of the amount of chunks that fit within a segment file
 * Returns the size or 0 on error
 */
uint32_t libewf_write_calculate_chunks_per_segment( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number )
{
	static char *function               = "libewf_write_calculate_chunks_per_segment";
	int32_t available_segment_file_size = 0;
	int32_t maximum_chunks_per_segment  = 0;
	int32_t chunks_per_segment          = 0;
	int32_t remaining_amount_of_chunks  = 0;
	int32_t required_chunk_sections     = 1;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->write->segment_file_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file size value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->segment_table->file_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing file offsets.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->segment_table->amount_of_chunks == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing amount of chunks.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->segment_table->file_offset[ segment_number ] > (off_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file offset value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	/* If the amount of chunks already have been determined
	 */
	if( internal_handle->segment_table->amount_of_chunks[ segment_number ] > 0 )
	{
		return( internal_handle->segment_table->amount_of_chunks[ segment_number ] );
	}
	/* Calculate the available segment file size
	 */
	available_segment_file_size = (int32_t) internal_handle->write->segment_file_size
	                            - (int32_t) internal_handle->segment_table->file_offset[ segment_number ];

	/* Leave space for the done or next section
	 */
	available_segment_file_size -= EWF_SECTION_SIZE;

	/* Calculate the maximum amount of chunks within this segment file
	 */
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		/* The EWF-S01 format uses compression this will add 16 bytes on average
		 */
		maximum_chunks_per_segment = available_segment_file_size
		                           / ( internal_handle->media->chunk_size + 16 );
	}
	else
	{
		maximum_chunks_per_segment = available_segment_file_size
		                           / ( internal_handle->media->chunk_size + EWF_CRC_SIZE );
	}
	/* Determine the amount of required chunk sections
	 */
	if( internal_handle->write->unrestrict_offset_amount == 0 )
	{
		required_chunk_sections = maximum_chunks_per_segment
		                        % EWF_MAXIMUM_OFFSETS_IN_TABLE;
	}
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		/* Leave space for the chunk section starts
		 */
		available_segment_file_size -= ( required_chunk_sections * EWF_SECTION_SIZE );

		/* Leave space for the table offsets
		 */
		available_segment_file_size -= ( maximum_chunks_per_segment * EWF_TABLE_OFFSET_SIZE );
	}
	else if( internal_handle->format == LIBEWF_FORMAT_ENCASE1 )
	{
		/* Leave space for the chunk section starts and the offset table CRC
		 */
		available_segment_file_size -= ( required_chunk_sections * ( EWF_SECTION_SIZE + EWF_CRC_SIZE ) );

		/* Leave space for the table offsets
		 */
		available_segment_file_size -= ( maximum_chunks_per_segment * EWF_TABLE_OFFSET_SIZE );
	}
	else
	{
		/* Leave space for the chunk, table and table2 section starts and the offset table CRC
		 */
		available_segment_file_size -= ( required_chunk_sections * ( ( 3 * EWF_SECTION_SIZE ) + EWF_CRC_SIZE ) );

		/* Leave space for the table and table2 offsets
		 */
		available_segment_file_size -= 2 * ( maximum_chunks_per_segment * EWF_TABLE_OFFSET_SIZE );
	}
	/* Calculate the amount of chunks within this segment file
	 */
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		/* The EWF-S01 format uses compression this will add 16 bytes on average
		 */
		chunks_per_segment = available_segment_file_size
		                   / ( internal_handle->media->chunk_size + 16 );
	}
	else
	{
		chunks_per_segment = available_segment_file_size
		                   / ( internal_handle->media->chunk_size + EWF_CRC_SIZE );
	}
	/* If the input size is known
	 */
	if( internal_handle->write->input_write_size > 0 )
	{
		/* Calculate the amount of chunks that will remain
		 */
		remaining_amount_of_chunks = (int32_t) internal_handle->media->amount_of_chunks
		                           - (int32_t) internal_handle->write->amount_of_chunks;

		/* Check if the less chunks remain than the amount of chunks calculated
		 */
		if( remaining_amount_of_chunks < chunks_per_segment )
		{
			chunks_per_segment = remaining_amount_of_chunks;
		}
	}
	/* Make sure to return the total amount of chunks per segment
	 */
	if( internal_handle->write->segment_amount_of_chunks > 0 )
	{
		chunks_per_segment += internal_handle->write->segment_amount_of_chunks;
	}
	return( (uint32_t) chunks_per_segment );
}

/* Calculates the amount of chunks that fit within a chunks section
 * Returns the size or 0 on error
 */
uint32_t libewf_write_calculate_chunks_per_chunks_section( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function              = "libewf_write_calculate_chunks_per_chunks_section";
	int32_t remaining_amount_of_chunks = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->write->chunks_per_segment > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunks per segment value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->write->segment_file_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file size value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	if( internal_handle->write->chunks_section_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported section number: %" PRIu8 ".\n",
		 function, internal_handle->write->chunks_section_number );

		return( 0 );
	}
	remaining_amount_of_chunks = (int32_t) internal_handle->write->chunks_per_segment;

	if( internal_handle->write->chunks_section_number > 1 )
	{
		remaining_amount_of_chunks -= ( ( internal_handle->write->chunks_section_number - 1 ) * EWF_MAXIMUM_OFFSETS_IN_TABLE );
	}
	if( remaining_amount_of_chunks <= 0 )
	{
		return( 0 );
	}
	else if( ( remaining_amount_of_chunks > EWF_MAXIMUM_OFFSETS_IN_TABLE ) && ( internal_handle->write->unrestrict_offset_amount == 0 ) )
	{
		return( EWF_MAXIMUM_OFFSETS_IN_TABLE );
	}
	return( (uint32_t) remaining_amount_of_chunks );
}

/* Tests if the current segment file is full
 * Returns 1 if full, 0 if not, -1 on error
 */
int libewf_write_test_segment_file_full( LIBEWF_INTERNAL_HANDLE *internal_handle, off_t segment_file_offset )
{
	static char *function               = "libewf_write_test_segment_file_full";
	int32_t remaining_segment_file_size = 0;

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
	if( segment_file_offset > (off_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->segment_file_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* Check if a chunks segment file has been opened
	 */
	if( segment_file_offset == 0 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: no segment file has been created.\n",
		 function );

		return( 0 );
	}
	/* Check if the maximum amount of chunks has been reached
	 */
	if( ( internal_handle->media->amount_of_chunks != 0 )
	 && ( internal_handle->media->amount_of_chunks == internal_handle->write->amount_of_chunks ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: all required chunks have been written.\n",
		 function );

		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( internal_handle->write->input_write_size != 0 )
	 && ( internal_handle->write->input_write_count >= (int64_t) internal_handle->write->input_write_size ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: all required data has been written.\n",
		 function );

		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format should use the pre calculated size
	 */
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( internal_handle->write->segment_amount_of_chunks >= internal_handle->write->chunks_per_segment )
		{
			LIBEWF_VERBOSE_PRINT( "%s: no space left for additional chunk.\n",
			 function );

			return( 1 );
		}
	}
	else
	{
		/* Calculate the remaining segment file size
		 */
		remaining_segment_file_size = (int32_t) internal_handle->write->segment_file_size
		                            - (int32_t) segment_file_offset;

		/* Leave space for the done or next section
		 */
		remaining_segment_file_size -= EWF_SECTION_SIZE;

		/* Leave space for the table and table2 sections
		 */
		remaining_segment_file_size -= 2
		                             * ( EWF_SECTION_SIZE
		                               + ( internal_handle->write->section_amount_of_chunks
		                                 * EWF_TABLE_OFFSET_SIZE )
		                               + EWF_CRC_SIZE );

		/* Determine if a chunk would fit in the segment file
		 */
		remaining_segment_file_size -= internal_handle->media->chunk_size
		                             + EWF_CRC_SIZE;

		if( remaining_segment_file_size <= 0 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: no space left for additional chunk.\n",
			 function );

			return( 1 );
		}
	}
	LIBEWF_VERBOSE_PRINT( "%s: space left for additional chunk.\n",
	 function );

	return( 0 );
}

/* Tests if the current chunks section is full
 * Returns 1 if full, 0 if not, -1 on error
 */
int libewf_write_test_chunks_section_full( LIBEWF_INTERNAL_HANDLE *internal_handle, off_t segment_file_offset )
{
	static char *function               = "libewf_write_test_chunks_section_full";
	int32_t remaining_segment_file_size = 0;

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
	if( segment_file_offset > (off_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->segment_file_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file size value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	/* Check if a chunks section has been opened
	 */
	if( internal_handle->write->chunks_section_offset == 0 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: no chunks section has been created.\n",
		 function );

		return( 0 );
	}
	/* Check if the maximum amount of chunks has been reached
	 */
	if( ( internal_handle->media->amount_of_chunks != 0 )
	 && ( internal_handle->media->amount_of_chunks == internal_handle->write->amount_of_chunks ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: all required chunks have been written.\n",
		 function );

		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( internal_handle->write->input_write_size != 0 )
	 && ( internal_handle->write->input_write_count >= (int64_t) internal_handle->write->input_write_size ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: all required data has been written.\n",
		 function );

		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format do not allow for a growth of the offset table
	 */
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( internal_handle->write->section_amount_of_chunks >= internal_handle->write->chunks_per_chunks_section )
		{
			LIBEWF_VERBOSE_PRINT( "%s: no space left for additional chunk.\n",
			 function );

			return( 1 );
		}
	}
	else
	{
		remaining_segment_file_size = (int32_t) internal_handle->write->segment_file_size;

		/* Calculate the remaining segment file size
		 */
		remaining_segment_file_size -= (int32_t) segment_file_offset;

		/* Leave space for the done or next section
		 */
		remaining_segment_file_size -= EWF_SECTION_SIZE;

		/* Leave space for the table and table2 sections
		 */
		remaining_segment_file_size -= 2
		                             * ( EWF_SECTION_SIZE
		                               + ( (int32_t) internal_handle->write->section_amount_of_chunks
		                                 * EWF_TABLE_OFFSET_SIZE )
		                               + EWF_CRC_SIZE );

		/* Determine if a chunk would fit in the segment file
		 */
		remaining_segment_file_size -= internal_handle->media->chunk_size + EWF_CRC_SIZE;

		if( remaining_segment_file_size <= 0 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: no space left for additional chunk.\n",
			 function );

			return( 1 );
		}
	}
	/* If the maximum offsets in table restriction should apply
	 */
	if( ( internal_handle->write->unrestrict_offset_amount == 0 )
	 && ( internal_handle->write->section_amount_of_chunks >= EWF_MAXIMUM_OFFSETS_IN_TABLE ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: no space left for additional chunk.\n",
		 function );

		return( 1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: space left for additional chunk.\n",
	 function );

	return( 0 );
}

/* Prepares the chunk data, applies compression if necessary and calculates the CRC
 * Returns the amount of bytes of the processed chunk data, or -1 on error
 */
ssize_t libewf_write_prepare_chunk_data( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_CHUNK *chunk_data, size_t chunk_data_size, EWF_CHUNK *compressed_chunk_data, size_t *compressed_chunk_data_size, int8_t *is_compressed, EWF_CRC *chunk_crc, int8_t *write_crc )
{
	static char *function     = "libewf_write_prepare_chunk_data";
	size_t data_write_size    = 0;
	int8_t compression_level  = 0;
	int chunk_cache_data_used = 0;
	int result                = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
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
	if( chunk_data == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk data.\n",
		 function );

		return( -1 );
	}
	if( chunk_data == internal_handle->chunk_cache->compressed )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk data buffer - same as chunk cache compressed.\n",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk data size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) internal_handle->media->chunk_size )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk data size value exceeds chunk size.\n",
		 function );

		return( -1 );
	}
	if( chunk_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk CRC.\n",
		 function );

		return( -1 );
	}
	if( is_compressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid is compressed.\n",
		 function );

		return( -1 );
	}
	if( write_crc == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid write CRC.\n",
		 function );

		return( -1 );
	}
	*write_crc = 0;

	/* Determine the compression level
	 */
	compression_level = internal_handle->compression_level;

	/* Compress empty blocks if necessary
	 */
	if( ( compression_level == EWF_COMPRESSION_NONE )
	 && ( internal_handle->write->compress_empty_block == 1 )
	 && ( libewf_common_test_empty_block( chunk_data, chunk_data_size ) == 1 ) )
	{
		compression_level = EWF_COMPRESSION_DEFAULT;
	}
	/* The compressed data size contains the maximum allowed buffer size on entry
	 */
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( compression_level != EWF_COMPRESSION_NONE ) )
	{
		if( compressed_chunk_data == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid compressed chunk data.\n",
			 function );

			return( -1 );
		}
		if( chunk_data == compressed_chunk_data )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid compressed chunk data is the same as chunk data.\n",
			 function );

			return( -1 );
		}
		if( compressed_chunk_data_size == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid compressed chunk data size.\n",
			 function );

			return( -1 );
		}
		if( *compressed_chunk_data_size > (size_t) SSIZE_MAX )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid compressed chunk data size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		result = ewf_chunk_compress(
			  (uint8_t *) compressed_chunk_data,
			  compressed_chunk_data_size,
			  (uint8_t *) chunk_data,
			  chunk_data_size,
			  compression_level );

		/* Check if the compressed buffer was too small
		 * and the internal chunk cache is used to store the compressed chunk
		 * and a new compressed data size buffer was passed back
		 */
		if( ( result == -1 )
		 && ( compressed_chunk_data == internal_handle->chunk_cache->compressed )
		 && ( *compressed_chunk_data_size > 0 ) )
		{
			chunk_cache_data_used = (int) ( chunk_data == internal_handle->chunk_cache->data );

			if( libewf_internal_handle_chunk_cache_realloc( internal_handle, *compressed_chunk_data_size ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to reallocate chunk cache.\n",
				 function );

				return( -1 );
			}
			/* Make sure the pointer refers to the reallocated buffer
			 */
			compressed_chunk_data = internal_handle->chunk_cache->compressed;

			if( ( chunk_cache_data_used == 1 )
			 && ( chunk_data != internal_handle->chunk_cache->data ) )
			{
				chunk_data = internal_handle->chunk_cache->data;
			}
			result = ewf_chunk_compress(
				  (uint8_t *) compressed_chunk_data,
				  compressed_chunk_data_size,
				  (uint8_t *) chunk_data,
				  chunk_data_size,
				  compression_level );
		}
		if( result != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to compress chunk.\n",
			 function );

			return( -1 );
		}
	}
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( ( *compressed_chunk_data_size > 0 )
	  && ( *compressed_chunk_data_size < internal_handle->media->chunk_size ) ) )
	{
		data_write_size = *compressed_chunk_data_size;
		chunk_data      = compressed_chunk_data;
		*is_compressed  = 1;

		/* Zlib creates its own CRC
		 */
		if( libewf_common_memcpy(
		     chunk_crc,
		     &( chunk_data[ *compressed_chunk_data_size - EWF_CRC_SIZE ] ),
		     EWF_CRC_SIZE ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set CRC.\n",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( ewf_crc_calculate( chunk_crc, (uint8_t *) chunk_data, chunk_data_size, 1 ) != 1 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: unable to calculate CRC.\n",
			 function );

			return( -1 );
		}
		data_write_size = chunk_data_size;
		*is_compressed  = 0;

		/* If the chunk cache data is used add the CRC
		 */
		if( chunk_data == internal_handle->chunk_cache->data )
		{
			if( libewf_endian_revert_32bit( *chunk_crc, (uint8_t *) &( chunk_data[ chunk_data_size ] ) ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to revert CRC value.\n",
				 function );

				return( -1 );
			}
			data_write_size += EWF_CRC_SIZE;
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
 * This function swaps byte pairs if necessary
 * Returns the amount of data bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_write_new_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t raw_access, uint32_t chunk, uint32_t chunk_offset, void *buffer, size_t size, size_t data_size, int8_t is_compressed, EWF_CRC chunk_crc, int8_t write_crc, int8_t force_write )
{
	EWF_CHUNK *chunk_data             = NULL;
	static char *function             = "libewf_write_new_chunk";
	ssize_t chunk_data_size           = 0;
	ssize_t write_count               = 0;
	size_t write_size                 = 0;
	size_t read_size                  = 0;
	size_t remaining_chunk_size       = 0;
	size_t chunk_cache_offset         = 0;
	size_t compressed_chunk_data_size = 0;
	uint16_t segment_number           = 0;
	uint8_t last_segment_file         = 0;
	int chunk_cache_data_used         = 0;
	int result                        = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
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
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->file_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing file offsets.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->file_descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid offset table - missing file descriptors.\n",
		 function );

		return( -1 );
	}
	if( ( chunk < internal_handle->offset_table->amount )
	 && ( internal_handle->offset_table->file_descriptor[ chunk ] != -1 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk: %" PRIu32 " already exists.\n",
		 function, chunk );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer - same as chunk cache.\n",
		 function );

		return( -1 );
	}
	/* Check if the amount of bytes as specified have been written
	 */
	if( ( internal_handle->write->input_write_size != 0 )
	 && ( internal_handle->write->input_write_count >= (ssize64_t) internal_handle->write->input_write_size ) )
	{
		return( 0 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: writing buffer of size: %zu with data of size: %zd.\n",
	 function, size, data_size );

	segment_number = internal_handle->segment_table->amount - 1;

	/* Check if a new segment file should be created
	 */
	if( segment_number == 0 )
	{
		result = 0;
	}
	/* Check if a segment file already exists
	 */
	else
	{
		result = libewf_segment_file_exists( internal_handle->segment_table, segment_number );
	}

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to check if segment file exists.\n",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		internal_handle->write->create_chunks_section     = 1;
		internal_handle->write->chunks_section_number     = 0;
		internal_handle->write->chunks_per_chunks_section = 0;
		internal_handle->write->segment_amount_of_chunks  = 0;

		segment_number += 1;

		LIBEWF_VERBOSE_PRINT( "%s: creating segment file with segment number: %" PRIu16 ".\n",
		 function, segment_number );

		/* Create a new segment file
		 */
		if( libewf_segment_file_create(
		     internal_handle->segment_table,
		     segment_number,
		     internal_handle->write->maximum_amount_of_segments,
		     LIBEWF_SEGMENT_FILE_TYPE_EWF,
		     internal_handle->ewf_format,
		     internal_handle->format ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create segment file for segment: %" PRIu16 ".\n",
			 function, segment_number );

			return( -1 );
		}
		/* Write the start of the segment file
		 * like the file header, the header, volume and/or data section, etc.
		 */
		write_count = libewf_segment_file_write_start(
		               internal_handle,
		               internal_handle->segment_table,
		               segment_number,
		               LIBEWF_SEGMENT_FILE_TYPE_EWF );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write segment file start.\n",
			 function );

			return( -1 );
		}
		internal_handle->write->write_count += write_count;

		internal_handle->write->chunks_per_segment = libewf_write_calculate_chunks_per_segment(
		                                              internal_handle,
		                                              segment_number );

		if( internal_handle->write->chunks_per_segment == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to determine the amount of chunks per segment.\n",
			 function );

			return( -1 );
		}
		LIBEWF_VERBOSE_PRINT( "%s: calculated amount of chunks for segment: %" PRIu32 ".\n",
		 function, internal_handle->write->chunks_per_segment );
	}
	/* Check if another chunk section should be created
	 */
	if( internal_handle->write->create_chunks_section == 1 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: creating chunks section.\n",
		 function );

		internal_handle->write->create_chunks_section      = 0;
		internal_handle->write->section_amount_of_chunks   = 0;
		internal_handle->write->chunks_section_write_count = 0;
		internal_handle->write->chunks_section_offset      = internal_handle->segment_table->file_offset[ segment_number ];

		/* Start with chunks section number number 1, value is initialized with 0
		 */
		internal_handle->write->chunks_section_number += 1;

		/* Recalculate the amount of chunks per segment for a better segment file fill when compression is used
		 */
		internal_handle->write->chunks_per_segment = libewf_write_calculate_chunks_per_segment(
		                                              internal_handle,
		                                              segment_number );

		if( internal_handle->write->chunks_per_segment == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to determine the amount of chunks per segment.\n",
			 function );

			return( -1 );
		}
		LIBEWF_VERBOSE_PRINT( "%s: calculated amount of chunks for segment: %" PRIu32 ".\n",
		 function, internal_handle->write->chunks_per_segment );

		internal_handle->write->chunks_per_chunks_section = libewf_write_calculate_chunks_per_chunks_section( internal_handle );

		if( internal_handle->write->chunks_per_segment == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to determine the amount of chunks per chunks section.\n",
			function );

			return( -1 );
		}
		LIBEWF_VERBOSE_PRINT( "%s: calculated amount of chunks for chunks section: %" PRIu32 ".\n",
		 function, internal_handle->write->chunks_per_chunks_section );

		/* Write the section start of the chunks section
		 */
		write_count = libewf_segment_file_write_chunks_section_start(
		               internal_handle,
		               segment_number,
		               internal_handle->media->chunk_size,
		               internal_handle->write->amount_of_chunks,
		               internal_handle->write->chunks_per_chunks_section );

		if( write_count == -1 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: unable to write section start for chunks.\n",
			 function );

			return( -1 );
		}
		internal_handle->write->write_count += write_count;
	}
	chunk_cache_offset = internal_handle->chunk_cache->offset;

	/* Determine the size of data to read
	 */
	if( data_size < (size_t) internal_handle->media->chunk_size )
	{
		read_size = (size_t) data_size;
	}
	else
	{
		read_size = internal_handle->media->chunk_size;
	}
	if( read_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid read size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* Check if raw access is being used
	 */
	if( raw_access != 0 )
	{
		chunk_data = (EWF_CHUNK *) buffer;
		write_size = read_size;
	}
#if defined( HAVE_BUFFER_PASSTHROUGH )
	/* Check if buffer contain the necessary amount of bytes to fill a chunk
	 * and no data was previously copied into the chunk cache
	 */
	else if( ( chunk_cache_offset == 0 )
	 && ( data_size >= (size_t) internal_handle->media->chunk_size ) )
	{
		chunk_data = (EWF_CHUNK *) buffer;
		write_size = read_size;
	}
#endif
	else
	{
		/* Check if data is present in the chunk cache
		 * and calculate the amount of data to read from the buffer
		 */
		remaining_chunk_size = internal_handle->media->chunk_size - chunk_cache_offset;

		if( read_size > (size_t) remaining_chunk_size )
		{
			read_size = remaining_chunk_size;
		}
		LIBEWF_VERBOSE_PRINT( "%s: reading %zu bytes from buffer.\n",
		 function, read_size );

		if( libewf_common_memcpy(
		     &internal_handle->chunk_cache->data[ chunk_cache_offset ],
		     buffer,
		     read_size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write data.\n",
			 function );

			return( -1 );
		}
		internal_handle->chunk_cache->chunk  = chunk;
		internal_handle->chunk_cache->amount = chunk_cache_offset + read_size;

		/* Adjust the chunk cache offset
		 */
		if( internal_handle->chunk_cache->amount == internal_handle->media->chunk_size )
		{
			internal_handle->chunk_cache->offset = 0;
		}
		else if( internal_handle->chunk_cache->amount < internal_handle->media->chunk_size )
		{
			internal_handle->chunk_cache->offset = internal_handle->chunk_cache->amount;
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: invalid chunk cache amount.\n",
			 function );

			return( -1 );
		}
		chunk_data = internal_handle->chunk_cache->data;
		write_size = internal_handle->chunk_cache->amount;
	}
	if( ( write_size == internal_handle->media->chunk_size )
	 || ( ( internal_handle->write->input_write_size != 0 )
	  && ( ( internal_handle->write->input_write_count + (ssize64_t) write_size ) == (ssize64_t) internal_handle->write->input_write_size ) )
	 || ( force_write != 0 ) )
	{
		if( raw_access == 0 )
		{
			/* Swap bytes if necessary
			 */
			if( ( internal_handle->swap_byte_pairs == 1 )
			 && ( libewf_endian_swap_byte_pairs( chunk_data, write_size ) != 1 ) )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to swap byte pairs.\n",
				 function );

				return( -1 );
			}
			/* Update the MD5 hash
			 */
			if( libewf_md5_update( &internal_handle->md5_context, chunk_data, write_size ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to update MD5 context.\n",
				 function );

				return( -1 );
			}
			chunk_cache_data_used = (int) ( chunk_data == internal_handle->chunk_cache->data );

			/* The compressed data size contains the maximum allowed buffer size
			 */
			compressed_chunk_data_size = internal_handle->chunk_cache->allocated_size;

			/* Compress the chunk if necessary and determine its CRC
			 */
			chunk_data_size = libewf_write_prepare_chunk_data(
					   internal_handle, 
					   chunk_data,
					   write_size,
					   internal_handle->chunk_cache->compressed,
					   &compressed_chunk_data_size,
					   &is_compressed,
					   &chunk_crc,
					   &write_crc );

			if( chunk_data_size <= -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to prepare chunk data.\n",
				 function );

				return( -1 );
			}
			/* Make sure to update the chunk_data pointer if 
			 * internal_handle->chunk_cache->data has been reallocated by
			 * libewf_write_prepare_chunk_data()
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
		}
		else
		{
			chunk_data_size = (ssize_t) size;
		}
		LIBEWF_VERBOSE_PRINT( "%s: writing %zd bytes to file.\n",
		 function, chunk_data_size );

		write_count = libewf_segment_file_write_chunks_data(
		               internal_handle,
		               segment_number,
		               chunk,
		               chunk_data,
		               chunk_data_size,
	        	       is_compressed,
		               &chunk_crc,
		               write_crc,
		               internal_handle->write->amount_of_chunks );

		if( write_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write chunk data.\n",
			 function );

			return( -1 );
		}
		internal_handle->write->input_write_count          += write_size;
		internal_handle->write->write_count                += write_count;
		internal_handle->write->chunks_section_write_count += write_count;
		internal_handle->write->segment_amount_of_chunks   += 1;
		internal_handle->write->section_amount_of_chunks   += 1;
		internal_handle->write->amount_of_chunks           += 1;
	}
	/* Check if the current chunks section is full, if so close the current section
	 */
	result = libewf_write_test_chunks_section_full(
	          internal_handle,
	          internal_handle->segment_table->file_offset[ segment_number ] );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to determine if chunks section is full.\n",
		 function );

		return( -1 );
	}
	else if( result == 1 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: closing chunks section amount of data written: %" PRIi32 ".\n",
		 function, internal_handle->write->chunks_section_write_count );

		/* Correct the offset, size in the chunks section
		 */
		write_count = libewf_segment_file_write_chunks_correction(
		               internal_handle,
		               segment_number,
		               internal_handle->write->chunks_section_offset,
		               (size_t) internal_handle->write->chunks_section_write_count,
		               internal_handle->write->amount_of_chunks,
		               internal_handle->write->section_amount_of_chunks );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to correct chunks section.\n",
			 function );

			return( -1 );
		}
		internal_handle->write->write_count           += write_count;
		internal_handle->write->create_chunks_section  = 1;
	}
	/* Check if the current segment file is full, if so close the current segment file
	 */
	result = libewf_write_test_segment_file_full(
	          internal_handle,
	          internal_handle->segment_table->file_offset[ segment_number ] );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to determine if segment file is full.\n",
		 function );

		return( -1 );
	}
	else if( result == 1 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: closing segment file with segment number: %" PRIu16 ".\n",
		 function, segment_number );

		/* Check if this is the last segment file
		 */
		if( ( internal_handle->write->input_write_size != 0 )
		 && ( internal_handle->write->input_write_count >= (ssize64_t) internal_handle->write->input_write_size ) )
		{
			last_segment_file = 1;
		}
		/* Finish and close the segment file
		 */
		write_count = libewf_segment_file_write_close(
		               internal_handle,
		               segment_number,
		               internal_handle->write->segment_amount_of_chunks,
		               last_segment_file );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to close segment file.\n",
			 function );

			return( -1 );
		}
		if( last_segment_file == 1 )
		{
			internal_handle->write->write_finalized = 1;
		}
		internal_handle->write->write_count           += write_count;
		internal_handle->write->chunks_section_offset  = 0;
	}
	/* Report the amount of chunk data written
	 */
	return( (ssize_t) read_size );
}

/* Writes an existing chunk of data in EWF format from a buffer at the current offset
 * The necessary settings of the write values must have been made
 * This function swaps byte pairs if necessary
 * Returns the amount of data bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_write_existing_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t raw_access, uint32_t chunk, uint32_t chunk_offset, void *buffer, size_t size, size_t data_size, int8_t is_compressed, EWF_CRC chunk_crc, int8_t write_crc, int8_t force_write )
{
	EWF_CHUNK *chunk_data   = NULL;
	static char *function   = "libewf_write_existing_chunk";
	ssize_t write_count     = 0;
	size_t write_size       = 0;
	size_t chunk_data_size  = 0;
	uint16_t segment_number = 0;
	int file_descriptor     = -1;
	int last_segment_file   = 0;
	int result              = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
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
	if( internal_handle->delta_segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing delta segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->file_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid delta segment table - missing file offsets.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->file_descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid offset table - missing file descriptors.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->segment_number == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid offset table - missing segment numbers.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->dirty == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid offset table - missing dirty flags.\n",
		 function );

		return( -1 );
	}
	if( chunk >= internal_handle->offset_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk does not exists.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer - same as chunk cache.\n",
		 function );

		return( -1 );
	}

	LIBEWF_VERBOSE_PRINT( "%s: writing buffer of size: %zu with data of size: %zd.\n",
	 function, size, data_size );

	/* Determine the size of data to write
	 */
	if( data_size < (size_t) internal_handle->media->chunk_size )
	{
		write_size = (size_t) data_size;
	}
	else
	{
		write_size = internal_handle->media->chunk_size;
	}
	if( write_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid read size value exceeds maximum.\n",
		 function );

		return( -1 );
	}

	/* TODO passthrough if data is chunk size
	 * what about raw access?
	 */

	/* Check if additional chunk data needs to be read
	 */
	if( write_size == internal_handle->media->chunk_size )
	{
		/* Read the chunk data into the chunk cache
		 */
		if( libewf_read_chunk(
		     internal_handle,
		     raw_access,
		     chunk,
		     0,
		     internal_handle->chunk_cache->data,
		     internal_handle->chunk_cache->allocated_size ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read data from chunk.\n",
			 function );

			return( -1 );
		}
		/* Update the chunk data
		 */
		if( libewf_common_memcpy(
		     &internal_handle->chunk_cache->data[ chunk_offset ],
		     buffer,
		     write_size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to update data in chunk.\n",
			 function );

			return( -1 );
		}
		chunk_data      = internal_handle->chunk_cache->data;
		chunk_data_size = internal_handle->chunk_cache->amount;
	}
	else
	{
		chunk_data      = buffer;
		chunk_data_size = write_size;
	}

	/* Swap bytes if necessary
	 */
	if( ( internal_handle->swap_byte_pairs == 1 )
	 && ( libewf_endian_swap_byte_pairs( (uint8_t *) chunk_data, chunk_data_size ) != 1 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to swap byte pairs.\n",
		 function );

		return( -1 );
	}
	/* Calculate the new CRC
	 */
	if( ewf_crc_calculate( &chunk_crc, (uint8_t *) chunk_data, chunk_data_size, 1 ) != 1 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: unable to calculate CRC.\n",
		 function );

		return( -1 );
	}
	/* Check if the chunk does not already exists in a delta segment file
	 */
	if( internal_handle->offset_table->dirty[ chunk ] == 0 )
	{
		/* Write the chunk to the last delta segment file
		 */
		segment_number = internal_handle->delta_segment_table->amount - 1;

		/* Check if a new delta segment file should be created
		 */
		if( segment_number == 0 )
		{
			result = 0;
		}
		/* Check if chunk fits in exisiting delta segment file
		 */
		else if( ( internal_handle->delta_segment_table->file_offset[ segment_number ]
		    + chunk_data_size + EWF_CRC_SIZE ) > internal_handle->write->segment_file_size )
		{
			result = 0;
		}
		/* Check if a delta segment already exists
		 */
		else
		{
			result = libewf_segment_file_exists( internal_handle->segment_table, segment_number );
		}

		if( result == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to check if segment file exists.\n",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			segment_number++;

			/* Create a new delta segment file
			 */
			if( libewf_segment_file_create(
			     internal_handle->delta_segment_table,
			     segment_number,
			     internal_handle->write->maximum_amount_of_segments,
			     LIBEWF_SEGMENT_FILE_TYPE_DWF,
			     internal_handle->ewf_format,
			     internal_handle->format ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to create delta segment file for segment: %" PRIu16 ".\n",
				 function, segment_number );

				return( -1 );
			}
			/* Write the start of the segment file
			 * like the file header, the header, volume and/or data section, etc.
			 */
			write_count = libewf_segment_file_write_start(
				       internal_handle,
				       internal_handle->delta_segment_table,
				       segment_number,
				       LIBEWF_SEGMENT_FILE_TYPE_DWF );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to write segment file start.\n",
				 function );

				return( -1 );
			}
		}
	}
	else
	{
		segment_number = internal_handle->offset_table->segment_number[ chunk ];

		if( ( segment_number == 0 )
		 || ( segment_number >= internal_handle->delta_segment_table->amount ) )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid segment number.\n",
			 function );

			return( -1 );
		}
		if( libewf_segment_file_seek_chunk_offset( internal_handle, chunk ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to seek chunk offset.\n",
			 function );

			return( -1 );
		}
	}
	/* Write the chunk in the delta segment file
	 */
	write_count = libewf_segment_file_write_delta_chunk(
		       internal_handle->delta_segment_table,
		       segment_number,
		       chunk, 
		       chunk_data, 
		       chunk_data_size, 
		       &chunk_crc );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write delta chunk.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->dirty[ chunk ] == 0 )
	{
		/* Write the last section
		 */
		write_count = libewf_segment_file_write_last_section(
			       internal_handle,
			       internal_handle->delta_segment_table->file_descriptor[ segment_number ],
			       internal_handle->delta_segment_table->section_list[ segment_number ],
			       internal_handle->delta_segment_table->file_offset[ segment_number ],
			       last_segment_file );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write last section.\n",
			 function );

			return( -1 );
		}
		internal_handle->delta_segment_table->file_offset[ segment_number ] += write_count;

		internal_handle->offset_table->dirty[ chunk ] = 1;
	}
	/* Report the amount of chunk data written
	 */
	return( (ssize_t) chunk_data_size );
}

/* Writes a chunk of data in EWF format from a buffer at the current offset
 * The necessary settings of the write values must have been made
 * This function swaps byte pairs if necessary
 * Returns the amount of data bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_write_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t raw_access, uint32_t chunk, uint32_t chunk_offset, void *buffer, size_t size, size_t data_size, int8_t is_compressed, EWF_CRC chunk_crc, int8_t write_crc, int8_t force_write )
{
	static char *function = "libewf_write_chunk";
	ssize_t write_count   = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_internal_handle_write_is_initialized( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_write_initialize( internal_handle ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to initialize write values.\n",
			 function );

			return( -1 );
		}
	}
	/* Check if the write was already finalized
	 */
	if( internal_handle->write->write_finalized == 1 )
	{
		return( 0 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->file_descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid offset table - missing file descriptors.\n",
		 function );

		return( -1 );
	}
	/* Check if chunk has already been created within a segment file
	 */
	if( ( chunk < internal_handle->offset_table->amount )
	 && ( internal_handle->offset_table->file_descriptor[ chunk ] != -1 ) )
	{
		write_count = libewf_write_existing_chunk(
		               internal_handle,
		               raw_access,
		               internal_handle->current_chunk,
		               internal_handle->current_chunk_offset,
		               buffer,
		               size,
		               data_size,
		               is_compressed,
		               chunk_crc,
		               write_crc,
		               raw_access );
	}
	else
	{
		write_count = libewf_write_new_chunk(
		               internal_handle,
		               raw_access,
		               internal_handle->current_chunk,
		               internal_handle->current_chunk_offset,
		               buffer,
		               size,
		               data_size,
		               is_compressed,
		               chunk_crc,
		               write_crc,
		               raw_access );
	}
	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write chunk.\n",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Writes chunk data in EWF format from a buffer at the current offset
 * the necessary settings of the write values must have been made
 * This function swaps byte pairs if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_write_chunk_data( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t raw_access, void *buffer, size_t size, size_t data_size, int8_t is_compressed, EWF_CRC chunk_crc, int8_t write_crc )
{
	static char *function     = "libewf_write_chunk_data";
	ssize_t chunk_write_count = 0;
	ssize_t total_write_count = 0;
	size_t chunk_data_size    = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer - same as chunk cache.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid data size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: writing buffer of size: %zu with data of size: %zd.\n",
	 function, size, data_size );

	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents multiple reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media->chunk_size + EWF_CRC_SIZE;

	if( buffer == internal_handle->chunk_cache->compressed )
	{
		LIBEWF_WARNING_PRINT( "%s: chunk cache compressed cannot be used as buffer.\n",
		 function );

		return( -1 );
	}
	if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
	{
		LIBEWF_VERBOSE_PRINT( "%s: reallocating chunk data size: %zu.\n",
		 function, chunk_data_size );

		if( libewf_internal_handle_chunk_cache_realloc( internal_handle, chunk_data_size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate chunk cache.\n",
			 function );

			return( -1 );
		}
	}
	while( data_size > 0 )
	{
		/* The write is forced when the raw access mode is set
		 */
		chunk_write_count = libewf_write_chunk(
		                     internal_handle,
		                     raw_access,
		                     internal_handle->current_chunk,
		                     internal_handle->current_chunk_offset,
		                     (void *) &( (uint8_t *) buffer )[ total_write_count ],
		                     size,
		                     data_size,
		                     is_compressed,
		                     chunk_crc,
		                     write_crc,
		                     raw_access );

		if( chunk_write_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write data from chunk.\n",
			 function );

			return( -1 );
		}
		else if( chunk_write_count == 0 )
		{
			break;
		}
		total_write_count += chunk_write_count;
		data_size         -= chunk_write_count;

		if( raw_access == 0 )
		{
			internal_handle->current_chunk_offset += (uint32_t) chunk_write_count;

			if( internal_handle->current_chunk_offset == internal_handle->media->chunk_size )
			{
				internal_handle->current_chunk_offset = 0;
				internal_handle->current_chunk       += 1;
			}
			else if( internal_handle->current_chunk_offset > internal_handle->media->chunk_size )
			{
				LIBEWF_WARNING_PRINT( "%s: invalid current chunk offset.\n",
				 function );

				return( -1 );
			}
		}
		else
		{
			internal_handle->current_chunk += 1;
		}
	}
	return( total_write_count );
}

/* Prepares a buffer with chunk data before writing according to the handle settings
 * intended for raw write
 * The buffer size cannot be larger than the chunk size
 * The function sets the chunk crc, is compressed and write crc values
 * Returns the resulting chunk size, or -1 on error
 */
ssize_t libewf_raw_write_prepare_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t buffer_size, void *compressed_buffer, size_t *compressed_buffer_size, int8_t *is_compressed, uint32_t *chunk_crc, int8_t *write_crc )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_raw_write_prepare_buffer";
	ssize_t chunk_data_size                 = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer - same as chunk cache.\n",
		 function );

		return( -1 );
	}
	if( ( compressed_buffer == internal_handle->chunk_cache->data )
	 || ( compressed_buffer == internal_handle->chunk_cache->compressed ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compressed buffer - same as chunk cache.\n",
		 function );

		return( -1 );
	}
	chunk_data_size = libewf_write_prepare_chunk_data(
	                   internal_handle,
	                   (EWF_CHUNK *) buffer,
	                   buffer_size,
	                   (EWF_CHUNK *) compressed_buffer,
	                   compressed_buffer_size,
	                   is_compressed,
	                   (EWF_CRC *) chunk_crc,
	                   write_crc );

	if( chunk_data_size <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to prepare chunk data.\n",
		 function );

		return( -1 );
	}
	return( chunk_data_size );
}

/* Writes 'raw' data in EWF format from a buffer at the current offset
 * the necessary settings of the write values must have been made
 * size contains the size of the data within the buffer while
 * data size contains the size of the actual input data
 * Returns the amount of input bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_raw_write_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size, size_t data_size, int8_t is_compressed, uint32_t chunk_crc, int8_t write_crc )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_raw_write_buffer";
	ssize_t write_count                     = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	if( data_size > internal_handle->media->chunk_size )
	{
		LIBEWF_WARNING_PRINT( "%s: data size cannot be larger than maximum chunk size.\n",
		 function );

		return( -1 );
	}
	write_count = libewf_write_chunk_data(
	               internal_handle,
	               1,
	               buffer,
	               size,
	               data_size,
	               is_compressed,
	               (EWF_CRC) chunk_crc,
	               write_crc );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write chunk data.\n",
		 function );
	}
	return( write_count );
}

/* Writes data in EWF format from a buffer at the current offset
 * the necessary settings of the write values must have been made
 * This function swaps byte pairs if specified and not in raw mode
 * Returns the amount of input bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_write_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size )
{
	static char *function = "libewf_write_buffer";
	ssize_t write_count   = 0;

	write_count = libewf_write_chunk_data(
	               (LIBEWF_INTERNAL_HANDLE *) handle,
	               0,
	               buffer,
	               size,
	               size,
	               0,
	               0,
	               0 );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write chunk data.\n",
		 function );
	}
	return( write_count );
}

/* Writes data in EWF format from a buffer at an specific offset,
 * the necessary settings of the write values must have been made
 * This function swaps byte pairs
 * Returns the amount of input bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_write_random( LIBEWF_HANDLE *handle, void *buffer, size_t size, off64_t offset )
{
	static char *function = "libewf_write_random";
	ssize_t write_count   = 0;

	if( libewf_seek_offset( handle, offset ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to seek offset.\n",
		 function );

		return( -1 );
	}
	write_count = libewf_write_buffer( handle, buffer, size );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to write buffer.\n",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after write from stream
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_write_finalize( LIBEWF_HANDLE *handle )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle        = NULL;
	LIBEWF_SECTION_LIST_ENTRY *list_entry_iterator = NULL;
	static char *function                          = "libewf_write_finalize";
	ssize_t write_count_finalize                   = 0;
	ssize_t write_count                            = 0;
	uint16_t segment_table_iterator                = 0;
	uint16_t segment_number                        = 0;
	int file_descriptor                            = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing filenames.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table->file_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid segment table - missing file offsets.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->write_finalized == 1 )
	{
		return( 0 );
	}
	LIBEWF_VERBOSE_PRINT( "%s: finalizing write.\n",
	 function );

	/* Write data remaining in the chunk cache to file
	 */
	/* TODO improve test for RW
	 */
	if( ( internal_handle->current_chunk_offset != 0 )
	 && ( internal_handle->chunk_cache->amount != 0 )
	 && ( internal_handle->chunk_cache->offset != 0 ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: writing chunk remainder at offset: %" PRIu32 " with size: %" PRIu32 "\n",
		 function, internal_handle->current_chunk_offset, internal_handle->chunk_cache->amount );

		write_count = libewf_write_chunk(
		               internal_handle,
		               0,
		               internal_handle->current_chunk,
		               internal_handle->current_chunk_offset,
		               internal_handle->chunk_cache->data,
		               internal_handle->chunk_cache->amount,
		               internal_handle->chunk_cache->amount,
		               0,
		               0,
		               0,
		               1 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to write remaining chunk data.\n",
			 function );

			return( -1 );
		}
		write_count_finalize += write_count;
	}
	segment_number  = internal_handle->segment_table->amount - 1;
	file_descriptor = internal_handle->segment_table->file_descriptor[ segment_number ];

	/* Check if the last segment file has been closed
	 */
	/* TODO improve test for RW
	 */
	if( file_descriptor != -1 )
	{
		/* Correct the offset, size in the chunks section
		 */
		LIBEWF_VERBOSE_PRINT( "%s: correcting chunks section.\n",
		 function );

		write_count = libewf_segment_file_write_chunks_correction(
		               internal_handle,
		               segment_number,
		               internal_handle->write->chunks_section_offset,
		               (size_t) internal_handle->write->chunks_section_write_count,
		               internal_handle->write->amount_of_chunks,
		               internal_handle->write->section_amount_of_chunks );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to correct chunks section.\n",
			 function );

			return( -1 );
		}
		write_count_finalize                += write_count;
		internal_handle->write->write_count += write_count;

		/* Close the segment file
		 */
		LIBEWF_VERBOSE_PRINT( "%s: closing segment file.\n",
		 function );

		write_count = libewf_segment_file_write_close(
		               internal_handle,
		               segment_number,
		               internal_handle->write->segment_amount_of_chunks,
		               1 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to close segment file.\n",
			 function );

			return( -1 );
		}
		write_count_finalize                += write_count;
		internal_handle->write->write_count += write_count;
	}
	/* Calculate the media values
	 */
	internal_handle->media->amount_of_chunks  = internal_handle->write->amount_of_chunks;
	internal_handle->media->amount_of_sectors = (uint32_t) ( internal_handle->write->input_write_count / internal_handle->media->bytes_per_sector );

	/* Correct the segment files
	 */
	for( segment_table_iterator = 1; segment_table_iterator < internal_handle->segment_table->amount; segment_table_iterator++ )
	{
		if( internal_handle->segment_table->section_list[ segment_table_iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: missing section list.\n",
			 function );

			return( -1 );
		}
		if( internal_handle->segment_table->section_list[ segment_table_iterator ]->first == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: missing section list entries.\n",
			 function );

			return( -1 );
		}
		file_descriptor = internal_handle->segment_table->file_descriptor[ segment_table_iterator ];

		if( file_descriptor == -1 )
		{
			if( internal_handle->segment_table->filename[ segment_table_iterator ] == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: invalid filename for segment file: %" PRIu32 ".\n",
				 function, segment_table_iterator );

				return( -1 );
			}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
			file_descriptor = libewf_common_wide_open(
			                   internal_handle->segment_table->filename[ segment_table_iterator ],
			                   LIBEWF_OPEN_READ_WRITE );
#else
			file_descriptor = libewf_common_open(
			                   internal_handle->segment_table->filename[ segment_table_iterator ],
			                   LIBEWF_OPEN_READ_WRITE );
#endif

			if( file_descriptor == -1 )
			{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
				LIBEWF_WARNING_PRINT( "%s: unable to open file: %ls.\n",
				 function, internal_handle->segment_table->filename[ segment_table_iterator ] );
#else
				LIBEWF_WARNING_PRINT( "%s: unable to open file: %s.\n",
				 function, internal_handle->segment_table->filename[ segment_table_iterator ] );
#endif
				return( -1 );
			}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
			LIBEWF_VERBOSE_PRINT( "%s: correcting segment file: %ls.\n",
			 function, internal_handle->segment_table->filename[ segment_table_iterator ] );
#else
			LIBEWF_VERBOSE_PRINT( "%s: correcting segment file: %s.\n",
			 function, internal_handle->segment_table->filename[ segment_table_iterator ] );
#endif
		}
		list_entry_iterator = internal_handle->segment_table->section_list[ segment_table_iterator ]->first;

		while( list_entry_iterator != NULL )
		{
			if( libewf_common_memcmp( list_entry_iterator->type, "volume", 6 ) == 0 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: correcting volume section.\n",
				 function );

				if( libewf_common_lseek( file_descriptor, list_entry_iterator->start_offset, SEEK_SET ) == -1 )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to find offset to correct volume section.\n",
					 function );

					return( -1 );
				}
				if( internal_handle->ewf_format == EWF_FORMAT_S01 )
				{
					/* Write volume (SMART) section
					 */
					write_count = libewf_section_volume_s01_write( internal_handle, file_descriptor, list_entry_iterator->start_offset );
				}
				else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
				{
					/* Write volume section
					 */
					write_count = libewf_section_volume_e01_write( internal_handle, file_descriptor, list_entry_iterator->start_offset );
				}
				else
				{
					write_count = -1;
				}
				if( write_count == -1 )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to correct volume section.\n",
					 function );

					return( -1 );
				}
			}
			else if( libewf_common_memcmp( list_entry_iterator->type, "data", 4 ) == 0 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: correcting data section.\n",
				 function );

				if( libewf_common_lseek( file_descriptor, list_entry_iterator->start_offset, SEEK_SET ) == -1 )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to find offset to data volume section.\n",
					 function );

					return( -1 );
				}
				/* Write data section
				 */
				write_count = libewf_section_data_write( internal_handle, file_descriptor, list_entry_iterator->start_offset );

				if( write_count == -1 )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to correct data section.\n",
					 function );

					return( -1 );
				}
			}
			list_entry_iterator = list_entry_iterator->next;
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to close segment file.\n",
			 function );

			return( -1 );
		}
		internal_handle->segment_table->file_descriptor[ segment_table_iterator ] = -1;
	}
	return( write_count_finalize );
}

