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

#include <errno.h>

#ifdef HAVE_STRING_H
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
	int32_t available_segment_file_size = 0;
	int32_t maximum_chunks_per_segment  = 0;
	int32_t chunks_per_segment          = 0;
	int32_t remaining_amount_of_chunks  = 0;
	int32_t required_chunk_sections     = 1;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_segment: invalid handle.\n" );

		return( 0 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_segment: invalid handle - missing subhandle media.\n" );

		return( 0 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_segment: invalid handle - missing subhandle write.\n" );

		return( 0 );
	}
	if( internal_handle->write->segment_file_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_segment: invalid segment file size only values below 2^32 are supported.\n" );

		return( 0 );
	}
	if( internal_handle->segment_table->file_offset[ segment_number ] > (off_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_segment: invalid segment file offset only values below 2^32 are supported.\n" );

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
	available_segment_file_size = (int32_t) internal_handle->write->segment_file_size - (int32_t) internal_handle->segment_table->file_offset[ segment_number ];

	/* Leave space for the done or next section
	 */
	available_segment_file_size -= EWF_SECTION_SIZE;

	/* Calculate the maximum amount of chunks within this segment file
	 */
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		/* The EWF-S01 format uses compression this will add 16 bytes on average
		 */
		maximum_chunks_per_segment = available_segment_file_size / ( internal_handle->media->chunk_size + 16 );
	}
	else
	{
		maximum_chunks_per_segment = available_segment_file_size / ( internal_handle->media->chunk_size + EWF_CRC_SIZE );
	}
	/* Determine the amount of required chunk sections
	 */
	if( internal_handle->write->unrestrict_offset_amount == 0 )
	{
		required_chunk_sections = maximum_chunks_per_segment % EWF_MAXIMUM_OFFSETS_IN_TABLE;
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
		chunks_per_segment = available_segment_file_size / ( internal_handle->media->chunk_size + 16 );
	}
	else
	{
		chunks_per_segment = available_segment_file_size / ( internal_handle->media->chunk_size + EWF_CRC_SIZE );
	}
	/* If the input size is known
	 */
	if( internal_handle->write->input_write_size > 0 )
	{
		/* Calculate the amount of chunks that will remain
		 */
		remaining_amount_of_chunks = (int32_t) internal_handle->media->amount_of_chunks - (int32_t) internal_handle->write->amount_of_chunks;

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
	int32_t remaining_amount_of_chunks = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_chunks_section: invalid handle.\n" );

		return( 0 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_chunks_section: invalid handle - missing subhandle media.\n" );

		return( 0 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_chunks_section: invalid handle - missing subhandle write.\n" );

		return( 0 );
	}
	if( internal_handle->write->chunks_per_segment > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_chunks_section: invalid chunks per segment only values below 2^32 are supported.\n" );

		return( 0 );
	}
	if( internal_handle->write->segment_file_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_chunks_section: invalid segment file size only values below 2^32 are supported.\n" );

		return( 0 );
	}
	if( internal_handle->write->chunks_section_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_calculate_chunks_per_chunks_section: unsupported section number: %" PRIu8 ".\n", internal_handle->write->chunks_section_number );

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
int8_t libewf_write_test_segment_file_full( LIBEWF_INTERNAL_HANDLE *internal_handle, off_t segment_file_offset )
{
	int32_t remaining_segment_file_size = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_segment_file_full: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_segment_file_full: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_segment_file_full: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( segment_file_offset > (off_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_segment_file_full: invalid segment file offset only values below 2^32 are supported.\n" );

		return( -1 );
	}
	if( internal_handle->write->segment_file_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_segment_file_full: invalid segment file size only values below 2^32 are supported.\n" );

		return( -1 );
	}
	/* Check if a chunks segment file has been opened
	 */
	if( segment_file_offset == 0 )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_test_segment_file_full: no segment file has been created.\n" );

		return( 0 );
	}
	/* Check if the maximum amount of chunks has been reached
	 */
	if( ( internal_handle->media->amount_of_chunks != 0 ) && ( internal_handle->media->amount_of_chunks == internal_handle->write->amount_of_chunks ) )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_test_segment_file_full: all required chunks have been written.\n" );

		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( internal_handle->write->input_write_size != 0 ) && ( internal_handle->write->input_write_count >= (int64_t) internal_handle->write->input_write_size ) )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_test_segment_file_full: all required data has been written.\n" );

		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format should use the pre calculated size
	 */
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 ) || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( internal_handle->write->segment_amount_of_chunks >= internal_handle->write->chunks_per_segment )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_write_test_segment_file_full: no space left for additional chunk.\n" );

			return( 1 );
		}
	}
	else
	{
		/* Calculate the remaining segment file size
		 */
		remaining_segment_file_size = (int32_t) internal_handle->write->segment_file_size - (int32_t) segment_file_offset;

		/* Leave space for the done or next section
		 */
		remaining_segment_file_size -= EWF_SECTION_SIZE;

		/* Leave space for the table and table2 sections
		 */
		remaining_segment_file_size -= 2 * ( EWF_SECTION_SIZE + ( internal_handle->write->section_amount_of_chunks * EWF_TABLE_OFFSET_SIZE ) + EWF_CRC_SIZE );

		/* Determine if a chunk would fit in the segment file
		 */
		remaining_segment_file_size -= internal_handle->media->chunk_size + EWF_CRC_SIZE;

		if( remaining_segment_file_size <= 0 )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_write_test_segment_file_full: no space left for additional chunk.\n" );

			return( 1 );
		}
	}
	LIBEWF_VERBOSE_PRINT( "libewf_write_test_segment_file_full: space left for additional chunk.\n" );

	return( 0 );
}

/* Tests if the current chunks section is full
 * Returns 1 if full, 0 if not, -1 on error
 */
int8_t libewf_write_test_chunks_section_full( LIBEWF_INTERNAL_HANDLE *internal_handle, off_t segment_file_offset )
{
	int32_t remaining_segment_file_size = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_chunks_section_full: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_chunks_section_full: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_chunks_section_full: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( segment_file_offset > (off_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_chunks_section_full: invalid segment file offset only values below 2^32 are supported.\n" );

		return( -1 );
	}
	if( internal_handle->write->segment_file_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_test_chunks_section_full: invalid segment file size only values below 2^32 are supported.\n" );

		return( 0 );
	}
	/* Check if a chunks section has been opened
	 */
	if( internal_handle->write->chunks_section_offset == 0 )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_test_chunks_section_full: no chunks section has been created.\n" );

		return( 0 );
	}
	/* Check if the maximum amount of chunks has been reached
	 */
	if( ( internal_handle->media->amount_of_chunks != 0 ) && ( internal_handle->media->amount_of_chunks == internal_handle->write->amount_of_chunks ) )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_test_chunks_section_full: all required chunks have been written.\n" );

		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( internal_handle->write->input_write_size != 0 ) && ( internal_handle->write->input_write_count >= (int64_t) internal_handle->write->input_write_size ) )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_test_chunks_section_full: all required data has been written.\n" );

		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format do not allow for a growth of the offset table
	 */
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 ) || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( internal_handle->write->section_amount_of_chunks >= internal_handle->write->chunks_per_chunks_section )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_write_test_chunks_section_full: no space left for additional chunk.\n" );

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
		remaining_segment_file_size -= 2 * ( EWF_SECTION_SIZE + ( (int32_t) internal_handle->write->section_amount_of_chunks * EWF_TABLE_OFFSET_SIZE ) + EWF_CRC_SIZE );

		/* Determine if a chunk would fit in the segment file
		 */
		remaining_segment_file_size -= internal_handle->media->chunk_size + EWF_CRC_SIZE;

		if( remaining_segment_file_size <= 0 )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_write_test_chunks_section_full: no space left for additional chunk.\n" );

			return( 1 );
		}
	}
	/* If the maximum offsets in table restriction should apply
	 */
	if( ( internal_handle->write->unrestrict_offset_amount == 0 ) && ( internal_handle->write->section_amount_of_chunks >= EWF_MAXIMUM_OFFSETS_IN_TABLE ) )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_test_chunks_section_full: no space left for additional chunk.\n" );

		return( 1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_write_test_chunks_section_full: space left for additional chunk.\n" );

	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Determines a wide character extension for a certain segment file
 * For EWF-E01, EWF-S01 segment file extension naming scheme
 * Returns 1 on success, -1 on error
 */
int8_t libewf_write_determine_wide_segment_file_extension( LIBEWF_INTERNAL_HANDLE *internal_handle, wchar_t* extension, uint16_t segment )
{
	wchar_t extension_first_character       = (wchar_t) '\0';
	wchar_t extension_additional_characters = (wchar_t) '\0';

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_wide_segment_file_extension: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_wide_segment_file_extension: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( extension == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_wide_segment_file_extension: invalid extension.\n" );

		return( -1 );
	}
	if( segment == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_wide_segment_file_extension: invalid segment 0.\n" );

		return( -1 );
	}
	if( internal_handle->write->maximum_amount_of_segments <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_wide_segment_file_extension: unable to determine the maximum amount of segment files.\n" );

		return( -1 );
	}
	if( segment > (uint16_t) internal_handle->write->maximum_amount_of_segments )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_wide_segment_file_extension: segment exceeds the maximum amount of segment files.\n" );

		return( -1 );
	}
	if( ( internal_handle->format == LIBEWF_FORMAT_EWF ) || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) )
	{
		extension_first_character       = (wchar_t) 'e';
		extension_additional_characters = (wchar_t) 'a';
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		extension_first_character       = (wchar_t) 's';
		extension_additional_characters = (wchar_t) 'a';
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
	{
		extension_first_character       = (wchar_t) 'E';
		extension_additional_characters = (wchar_t) 'A';
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_L01 )
	{
		extension_first_character       = (wchar_t) 'L';
		extension_additional_characters = (wchar_t) 'A';
	}
	else
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_wide_segment_file_extension: unsupported EWF format.\n" );

		return( -1 );
	}
	extension[ 0 ] = extension_first_character;

	if( segment <= 99 )
	{
		extension[ 2 ] = (wchar_t) '0' + (wchar_t) ( segment % 10 );
		extension[ 1 ] = (wchar_t) '0' + (wchar_t) ( segment / 10 );
	}
	else if( segment >= 100 )
	{
		segment        -= 100;
		extension[ 2 ]  = extension_additional_characters + (wchar_t) ( segment % 26 );
		segment        /= 26;
		extension[ 1 ]  = extension_additional_characters + (wchar_t) ( segment % 26 );
		segment        /= 26;

		if( segment >= 26 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_determine_wide_segment_file_extension: unable to support for more segment files.\n" );

			return( -1 );
		}
		extension[ 0 ] = extension_first_character + (wchar_t) segment;
	}
	/* Safety check
	 */
	if( ( extension[ 0 ] > (wchar_t) 'z' ) || ( ( extension[ 0 ] > (wchar_t) 'Z' ) && ( extension[ 0 ] < (wchar_t) 'a' ) ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_wide_segment_file_extension: unable to support for more segment files.\n" );

		return( -1 );
	}
	extension[ 3 ] = (wchar_t) '\0';

	return( 1 );
}
#else

/* Determines an extension for a certain segment file
 * For EWF-E01, EWF-S01 segment file extension naming scheme
 * Returns 1 on success, -1 on error
 */
int8_t libewf_write_determine_segment_file_extension( LIBEWF_INTERNAL_HANDLE *internal_handle, char* extension, uint16_t segment )
{
	char extension_first_character       = (char) '\0';
	char extension_additional_characters = (char) '\0';

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_segment_file_extension: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_segment_file_extension: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( extension == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_segment_file_extension: invalid extension.\n" );

		return( -1 );
	}
	if( segment == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_segment_file_extension: invalid segment 0.\n" );

		return( -1 );
	}
	if( internal_handle->write->maximum_amount_of_segments <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_segment_file_extension: unable to determine the maximum amount of segment files.\n" );

		return( -1 );
	}
	if( segment > (uint16_t) internal_handle->write->maximum_amount_of_segments )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_segment_file_extension: segment exceeds the maximum amount of segment files.\n" );

		return( -1 );
	}
	if( ( internal_handle->format == LIBEWF_FORMAT_EWF ) || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) )
	{
		extension_first_character       = (char) 'e';
		extension_additional_characters = (char) 'a';
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		extension_first_character       = (char) 's';
		extension_additional_characters = (char) 'a';
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
	{
		extension_first_character       = (char) 'E';
		extension_additional_characters = (char) 'A';
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_L01 )
	{
		extension_first_character       = (char) 'L';
		extension_additional_characters = (char) 'A';
	}
	else
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_segment_file_extension: unsupported EWF format.\n" );

		return( -1 );
	}
	extension[ 0 ] = extension_first_character;

	if( segment <= 99 )
	{
		extension[ 2 ] = (char) '0' + (char) ( segment % 10 );
		extension[ 1 ] = (char) '0' + (char) ( segment / 10 );
	}
	else if( segment >= 100 )
	{
		segment        -= 100;
		extension[ 2 ]  = extension_additional_characters + (char) ( segment % 26 );
		segment        /= 26;
		extension[ 1 ]  = extension_additional_characters + (char) ( segment % 26 );
		segment        /= 26;

		if( segment >= 26 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_determine_segment_file_extension: unable to support for more segment files.\n" );

			return( -1 );
		}
		extension[ 0 ] = extension_first_character + (char) segment;
	}
	/* Safety check
	 */
	if( ( extension[ 0 ] > (char) 'z' ) || ( ( extension[ 0 ] > (char) 'Z' ) && ( extension[ 0 ] < (char) 'a' ) ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_determine_segment_file_extension: unable to support for more segment files.\n" );

		return( -1 );
	}
	extension[ 3 ] = (char) '\0';

	return( 1 );
}
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Creates a wide character filename for a certain segment file
 * Returns the pointer to the filename, NULL on error
 */
wchar_t *libewf_write_create_wide_segment_filename( LIBEWF_INTERNAL_HANDLE *internal_handle, wchar_t* basename, uint16_t segment )
{
	wchar_t *filename = NULL;
	size_t length     = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_wide_segment_filename: invalid handle.\n" );

		return( NULL );
	}
	if( basename == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_wide_segment_filename: invalid basename.\n" );

		return( NULL );
	}
	if( segment == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_wide_segment_filename: invalid segment 0.\n" );

		return( NULL );
	}
	length = libewf_common_wide_string_length( basename );

	if( length == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_wide_segment_filename: an emtpy basename is not supported.\n" );

		return( NULL );
	}
	/* The actual filename also contain a . 3 character extension and a end of string byte
	 */
	filename = libewf_common_alloc( ( length + 5 ) * sizeof( wchar_t ) );

	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_wide_segment_filename: unable to allocate filename.\n" );

		return( NULL );
	}
	/* Add one additional character for the end of line
	 */
	if( libewf_common_wide_string_copy( filename, basename, ( length + 1 ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_wide_segment_filename: unable to copy basename.\n" );

		libewf_common_free( filename );

		return( NULL );
	}
	filename[ length ] = '.';

	if( libewf_write_determine_wide_segment_file_extension( internal_handle, &filename[ length + 1 ], segment ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_wide_segment_filename: unable to determine extension.\n" );

		libewf_common_free( filename );

		return( NULL );
	}
	return( filename );
}
#else

/* Creates a filename for a certain segment file
 * Returns the pointer to the filename, NULL on error
 */
char *libewf_write_create_segment_filename( LIBEWF_INTERNAL_HANDLE *internal_handle, char* basename, uint16_t segment )
{
	char *filename = NULL;
	size_t length  = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_filename: invalid handle.\n" );

		return( NULL );
	}
	if( basename == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_filename: invalid basename.\n" );

		return( NULL );
	}
	if( segment == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_filename: invalid segment 0.\n" );

		return( NULL );
	}
	length = libewf_common_string_length( basename );

	if( length == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_filename: an emtpy basename is not supported.\n" );

		return( NULL );
	}
	/* The actual filename also contain a . 3 character extension and a end of string byte
	 */
	filename = libewf_common_alloc( ( length + 5 ) * sizeof( char ) );

	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_filename: unable to allocate filename.\n" );

		return( NULL );
	}
	/* Add one additional character for the end of line
	 */
	if( libewf_common_string_copy( filename, basename, ( length + 1 ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_filename: unable to copy basename.\n" );

		libewf_common_free( filename );

		return( NULL );
	}
	filename[ length ] = '.';

	if( libewf_write_determine_segment_file_extension( internal_handle, &filename[ length + 1 ], segment ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_filename: unable to determine extension.\n" );

		libewf_common_free( filename );

		return( NULL );
	}
	return( filename );
}
#endif

/* Creates a new segment file entry within the segment table
 * Returns 1 on success, -1 on error
 */
int8_t libewf_write_create_segment_file_entry( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment )
{
	LIBEWF_SEGMENT_TABLE *segment_table = NULL;
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	wchar_t *filename                   = NULL;
#else
	char *filename                      = NULL;
#endif
	size_t length_filename              = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_file_entry: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_file_entry: invalid handle - missing segment table.\n" );

		return( -1 );
	}
	if( segment == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_file_entry: invalid segment 0.\n" );

		return( -1 );
	}
	/* Check if one additional entry in the segment table is needed
	 */
	if( internal_handle->segment_table->amount <= segment )
	{
		/* Add one additional entry because the 0 entry is used for the basename
		 */
		segment_table = libewf_segment_table_realloc( internal_handle->segment_table, ( segment + 1 ) );

		if( segment_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_create_segment_file_entry: unable to reallocate segment table.\n" );

			return( -1 );
		}
		internal_handle->segment_table = segment_table;

	}
	/* Check if the entry has already been filled
	 */
	else if( libewf_segment_table_values_is_set( internal_handle->segment_table, segment ) == 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_file_entry: segment file has already been created.\n" );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	filename = libewf_write_create_wide_segment_filename( internal_handle, internal_handle->segment_table->filename[ 0 ], segment );
#else
	filename = libewf_write_create_segment_filename( internal_handle, internal_handle->segment_table->filename[ 0 ], segment );
#endif

	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_file_entry: unable to create filename.\n" );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	length_filename = libewf_common_wide_string_length( filename );
#else
	length_filename = libewf_common_string_length( filename );
#endif

	if( length_filename == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_file_entry: filename cannot be empty.\n" );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	if( libewf_segment_table_set_wide_filename( internal_handle->segment_table, segment, filename, length_filename ) != 1 )
#else
	if( libewf_segment_table_set_filename( internal_handle->segment_table, segment, filename, length_filename ) != 1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_file_entry: unable to set filename in segment table.\n" );

		libewf_common_free( filename );

		return( -1 );
	}
	if( libewf_segment_table_set_file_descriptor( internal_handle->segment_table, segment, -1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_create_segment_file_entry: unable to set file descriptor in segment table.\n" );

		libewf_common_free( filename );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	LIBEWF_VERBOSE_PRINT( "libewf_write_create_segment_file_entry: segment file created: %" PRIu32 " with name: %ls.\n", segment, filename );
#else
	LIBEWF_VERBOSE_PRINT( "libewf_write_create_segment_file_entry: segment file created: %" PRIu32 " with name: %s.\n", segment, filename );
#endif
	libewf_common_free( filename );

	return( 1 );
}

/* Write the headers to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_write_headers( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, LIBEWF_SECTION_LIST *section_list )
{
	size_t header_size        = 0;
	size_t header2_size       = 0;
	size_t xheader_size       = 0;
	ssize_t write_count       = 0;
	ssize_t total_write_count = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_headers: invalid handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_headers: invalid section list.\n" );

		return( -1 );
	}
	if( file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_headers: invalid file descriptor.\n" );

		return( -1 );
	}
	if( ( internal_handle->header == NULL ) || ( internal_handle->header_size == 0 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_headers: invalid header.\n" );

		return( -1 );
	}
	header_size = internal_handle->header_size - 1;

	if( ( internal_handle->format == LIBEWF_FORMAT_EWF )
	 || ( internal_handle->format == LIBEWF_FORMAT_SMART )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* The header should be written only once
		 * and using the compression used in the file
		 */
		write_count = libewf_section_header_write( internal_handle, file_descriptor, start_offset, internal_handle->header, header_size, internal_handle->compression_level );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to write single header section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "header", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to append header section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	else if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE2 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE3 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_FTK ) )
	{
		/* The header should be written twice
		 * the default compression is used
		 */
		write_count = libewf_section_header_write( internal_handle, file_descriptor, start_offset, internal_handle->header, header_size, EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to write first header section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "header", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to append first header section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;

		write_count = libewf_section_header_write( internal_handle, file_descriptor, start_offset, internal_handle->header, header_size, EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to write second header section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "header", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to append second header section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	else if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE4 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 ) )
	{
		if( ( internal_handle->header2 == NULL ) && ( internal_handle->header2_size == 0 ) )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: invalid header2.\n" );

			return( -1 );
		}
		header2_size = internal_handle->header2_size - 2;

		/* The header2 should be written twice
		 * the default compression is used
		 */
		write_count = libewf_section_header2_write( internal_handle, file_descriptor, start_offset, internal_handle->header2, internal_handle->header2_size, EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to write first header2 section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "header2", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to append first header2 section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;

		write_count = libewf_section_header2_write( internal_handle, file_descriptor, start_offset, internal_handle->header2, internal_handle->header2_size, EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to write second header2 section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "header2", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to append second header2 section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_header_write( internal_handle, file_descriptor, start_offset, internal_handle->header, header_size, EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to write third header section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "header", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to append third header section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	/* EWFX uses the header and header2 for backwards compatibility
	 */
	else if( internal_handle->format == LIBEWF_FORMAT_EWFX )
	{
		if( ( internal_handle->xheader == NULL ) && ( internal_handle->xheader_size == 0 ) )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: invalid xheader.\n" );

			return( -1 );
		}
		xheader_size = internal_handle->xheader_size;

		if( ( internal_handle->header2 == NULL ) && ( internal_handle->header2_size == 0 ) )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: invalid header2.\n" );

			return( -1 );
		}
		header2_size = internal_handle->header2_size - 2;

		/* The xheader should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_xheader_write( internal_handle, file_descriptor, start_offset, internal_handle->xheader, xheader_size, EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to write firts xheader section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "xheader", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to append first xheader section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;

		/* The header2 should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_header2_write( internal_handle, file_descriptor, start_offset, internal_handle->header2, internal_handle->header2_size, EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to write second header2 section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "header2", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to append second header2 section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;

		/* The header should be written once
		 * the default compression is used
		 */
		write_count = libewf_section_header_write( internal_handle, file_descriptor, start_offset, internal_handle->header, header_size, EWF_COMPRESSION_DEFAULT );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to write third header section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "header", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_headers: unable to append third header section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Write the necessary sections at the start of the segment file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_write_segment_file_start( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int file_descriptor, LIBEWF_SECTION_LIST *section_list )
{
	EWF_FILE_HEADER file_header;

	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: invalid handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: invalid section list.\n" );

		return( -1 );
	}
	if( file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: invalid file descriptor.\n" );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: invalid segment number: 0.\n" );

		return( -1 );
	}
	if( libewf_common_memcpy( file_header.signature, evf_file_signature, 8 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: unable to set data.\n" );

		return( -1 );
	}
	if( libewf_endian_revert_16bit( segment_number, file_header.fields_segment ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: unable to revert segment number.\n" );

		return( -1 );
	}
	file_header.fields_start    = 1;
	file_header.fields_end[ 0 ] = 0;
	file_header.fields_end[ 1 ] = 0;

	write_count = ewf_file_header_write( &file_header, file_descriptor );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: unable to write file header to file.\n" );

		return( -1 );
	}
	total_write_count += write_count;

	if( segment_number == 1 )
	{
		/* Write header section(s)
		 */
		write_count = libewf_write_headers( internal_handle, file_descriptor, (uint32_t) total_write_count, section_list );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: unable to write header sections.\n" );

			return( -1 );
		}
		total_write_count += write_count;

		if( internal_handle->ewf_format == EWF_FORMAT_S01 )
		{
			/* Write volume (SMART) section
			 */
			write_count = libewf_section_volume_s01_write( internal_handle, file_descriptor, (uint32_t) total_write_count );
		}
		else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
		{
			/* Write volume section
			 */
			write_count = libewf_section_volume_e01_write( internal_handle, file_descriptor, (uint32_t) total_write_count );
		}
		else
		{
			/* Fail safe
			 */
			write_count = -1;
		}
		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: unable to write volume section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "volume", (uint32_t) total_write_count, (uint32_t) ( total_write_count + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: unable to append volume section to section list.\n" );

			return( -1 );
		}
		total_write_count += write_count;
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
	{
		/* Write data section
		 */
		write_count = libewf_section_data_write( internal_handle, file_descriptor, (uint32_t) total_write_count );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: unable to write data section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "data", (uint32_t) total_write_count, (uint32_t) ( total_write_count + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_start: unable to append data section to section list.\n" );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Write the necessary sections before the actual data chunks to file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_write_segment_file_chunks_section_start( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, uint32_t total_chunk_amount, uint32_t segment_chunk_amount )
{
	LIBEWF_OFFSET_TABLE *reallocation = NULL;
	ssize_t total_write_count         = 0;
	ssize_t write_count               = 0;
	size_t section_size               = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_start: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_start: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_start: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_start: invalid handle - missing offset table.\n" );

		return( -1 );
	}
	if( file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_start: invalid file descriptor.\n" );

		return( -1 );
	}
	/* The segment_chunk_amount contain the estimated amount of chunks for this section
	 */
	if( internal_handle->offset_table->amount < ( total_chunk_amount + segment_chunk_amount ) )
	{
		reallocation = libewf_offset_table_realloc( internal_handle->offset_table, ( total_chunk_amount + segment_chunk_amount ) );

		if( reallocation == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_start: unable to reallocate offset table.\n" );

			return( -1 );
		}
		internal_handle->offset_table = reallocation;
	}
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* Write table section start
		 */
		write_count = libewf_section_table_write( internal_handle, file_descriptor, start_offset, internal_handle->offset_table, total_chunk_amount, segment_chunk_amount, (EWF_CHAR *) "table", 0 );

		if( write_count == -1 )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_write_segment_file_chunks_section_start: unable to write table section.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
	{
		section_size = segment_chunk_amount * ( internal_handle->media->chunk_size + EWF_CRC_SIZE );

		/* Write sectors section start
		 */
		write_count = libewf_section_start_write( internal_handle, file_descriptor, (EWF_CHAR *) "sectors", section_size, start_offset );

		if( write_count == -1 )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_write_segment_file_chunks_section_start: unable to write sectors section.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Write a chunk to a segment file (compressed or uncompressed)
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_write_segment_file_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, int file_descriptor, EWF_CHUNK *chunk_data, off_t start_offset, size_t size, uint32_t chunk )
{
	LIBEWF_OFFSET_TABLE *reallocation = NULL;
	EWF_CRC calculated_crc            = 0;
	ssize_t total_write_count         = 0;
	ssize_t write_count               = 0;
	size_t compressed_data_size       = 0;
	size_t data_write_size            = 0;
	int8_t compression_level          = 0;
	int8_t chunk_cache_used           = 0;
	int8_t write_crc                  = 0;
	int8_t result                     = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: invalid handle - missing chunk cache.\n" );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: invalid handle - missing offset table.\n" );

		return( -1 );
	}
	if( file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: invalid file descriptor.\n" );

		return( -1 );
	}
	if( chunk_data == internal_handle->chunk_cache->compressed )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: invalid chunk data buffer - chunk cache compressed cannot be used as chunk data buffer.\n" );

		return( -1 );
	}
	if( internal_handle->offset_table->amount < ( chunk + 1 ) )
	{
		reallocation = libewf_offset_table_realloc( internal_handle->offset_table, ( chunk + 1 ) );

		if( reallocation == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to reallocate offset table.\n" );

			return( -1 );
		}
		internal_handle->offset_table = reallocation;
	}
	/* Swap bytes if necessary
	 */
	if( ( internal_handle->swap_byte_pairs == 1 ) && ( libewf_endian_swap_byte_pairs( chunk_data, size ) != 1 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to swap byte pairs.\n" );

		return( -1 );
	}
	/* Update the MD5 hash
	 */
	if( libewf_md5_update( &internal_handle->md5_context, chunk_data, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to update MD5 context.\n" );

		return( -1 );
	}
	/* Determine the compression level
	 */
	compression_level = internal_handle->compression_level;

	if( compression_level == EWF_COMPRESSION_NONE )
	{
		if( internal_handle->write->compress_empty_block == 1 )
		{
			if( libewf_common_test_empty_block( chunk_data, size ) == 1 )
			{
				compression_level = EWF_COMPRESSION_DEFAULT;
			}
		}
	}
	/* The compressed data size contains the maximum allowed buffer size
	 */
	compressed_data_size = internal_handle->chunk_cache->allocated_size;

	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( compression_level != EWF_COMPRESSION_NONE ) )
	{
		result = ewf_chunk_compress( internal_handle->chunk_cache->compressed, &compressed_data_size, chunk_data, size, compression_level );

		/* Check if the compressed buffer was too small
		 * and a new compressed data size buffer was passed back
		 */
		if( ( result == -1 ) && ( compressed_data_size > 0 ) )
		{
			/* Check if chunk cache passthrough is used
			 * if the chunk cache is used as the chunk data buffer
			 */
			chunk_cache_used = (int8_t) ( chunk_data == internal_handle->chunk_cache->data );

			if( libewf_internal_handle_chunk_cache_realloc( internal_handle, compressed_data_size ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to reallocate chunk cache.\n" );

				return( -1 );
			}
			/* Adjust the chunk data buffer if necessary
			 */
			if( ( chunk_cache_used == 1 ) && ( chunk_data != internal_handle->chunk_cache->data ) )
			{
				chunk_data = internal_handle->chunk_cache->data;
			}
			result = ewf_chunk_compress( internal_handle->chunk_cache->compressed, &compressed_data_size, chunk_data, size, internal_handle->compression_level );
		}
		if( result != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to compress chunk: %" PRIu32 ".\n", chunk );

			return( -1 );
		}
	}
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( ( compressed_data_size > 0 ) && ( compressed_data_size < internal_handle->media->chunk_size ) ) )
	{
		data_write_size = compressed_data_size;
		chunk_data      = internal_handle->chunk_cache->compressed;

		/* Zlib creates its own CRC
		 */
		if( libewf_common_memcpy( &calculated_crc, &chunk_data[ compressed_data_size - EWF_CRC_SIZE ], EWF_CRC_SIZE ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to set CRC.\n" );

			return( -1 );
		}
		LIBEWF_VERBOSE_PRINT( "libewf_write_segment_file_chunk: writing COMPRESSED chunk: %" PRIu32 " at offset: %jd with size: %zu, with CRC: %" PRIu32 ".\n", ( chunk + 1 ), start_offset, data_write_size, calculated_crc );

		if( libewf_offset_table_set_values( internal_handle->offset_table, internal_handle->write->amount_of_chunks, file_descriptor, 1, start_offset, compressed_data_size, segment_number ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to set offset value in offset table.\n" );

			return( -1 );
		}
	}
	else
	{
		if( ewf_crc_calculate( &calculated_crc, (uint8_t *) chunk_data, size, 1 ) != 1 )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_write_segment_file_chunk: unable to calculate CRC.\n" );

			return( -1 );
		}
		data_write_size = size;

		/* If the chunk cache data is used add the CRC
		 */
		if( chunk_data == internal_handle->chunk_cache->data )
		{
			if( libewf_endian_revert_32bit( calculated_crc, (uint8_t *) &chunk_data[ size ] ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to revert CRC value.\n" );

				return( -1 );
			}
			data_write_size += EWF_CRC_SIZE;
		}
		else
		{
			write_crc = 1;
		}
		LIBEWF_VERBOSE_PRINT( "libewf_write_segment_file_chunk: writing UNCOMPRESSED chunk: %" PRIu32 " at offset: %jd with size: %zu, with CRC: %" PRIu32 ".\n", ( chunk + 1 ), start_offset, data_write_size, calculated_crc );

		if( libewf_offset_table_set_values( internal_handle->offset_table, internal_handle->write->amount_of_chunks, file_descriptor, 0, start_offset, ( size + EWF_CRC_SIZE ), segment_number ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to set offset value in offset table.\n" );

			return( -1 );
		}
	}
	write_count = ewf_chunk_write( chunk_data, file_descriptor, data_write_size );

	if( write_count != (int32_t) data_write_size )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to write data.\n" );

		return( -1 );
	}
	start_offset      += write_count;
	total_write_count += write_count;

	/* Write the CRC if necessary
	 */
	if( write_crc == 1 )
	{
		write_count = ewf_crc_write( &calculated_crc, file_descriptor );

		if( write_count != (int32_t) EWF_CRC_SIZE )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunk: unable to write CRC.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Correct the sections before the actual data chunks
 * Also write the necessary sections after the actual data chunks to file (like table and table2 sections for EWF-E01 format)
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_write_segment_file_chunks_section_correction( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, LIBEWF_SECTION_LIST *section_list, off_t chunks_section_offset, size_t chunks_section_size )
{
	EWF_CHAR *table_section_string = NULL;
	ssize_t total_write_count      = 0;
	ssize_t write_count            = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: invalid handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: invalid section list.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: invalid file descriptor.\n" );

		return( -1 );
	}
	/* Seek the start of the data chunks
	*/
	LIBEWF_VERBOSE_PRINT( "libewf_write_segment_file_chunks_section_correction: setting file descriptor to start of chunks section offset: %" PRIu32 ".\n", chunks_section_offset );

	if( libewf_common_lseek( file_descriptor, chunks_section_offset, SEEK_SET ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to find offset to correct sectors size.\n" );

		return( -1 );
	}
	if( ( internal_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_segment_file_chunks_section_correction: correcting table section size: %zu offset: %jd.\n", chunks_section_size, chunks_section_offset );

		/* Rewrite table section start
		 */
		write_count = libewf_section_table_write( internal_handle, file_descriptor, chunks_section_offset, internal_handle->offset_table, ( internal_handle->write->amount_of_chunks - internal_handle->write->section_amount_of_chunks ), internal_handle->write->section_amount_of_chunks, (EWF_CHAR *) "table", chunks_section_size );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to rewrite %" PRIs_EWF " section.\n", table_section_string );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "table", chunks_section_offset, ( chunks_section_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to append table section to section list.\n" );

			return( -1 );
		}
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_segment_file_chunks_section_correction: correcting sectors section size: %zu offset: %jd.\n", chunks_section_size, chunks_section_offset );

		/* Rewrite sectors section start
		 */
		write_count = libewf_section_start_write( internal_handle, file_descriptor, (EWF_CHAR *) "sectors", chunks_section_size, chunks_section_offset );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to rewrite sectors section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "sectors", chunks_section_offset, ( chunks_section_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to append sectors section to section list.\n" );

			return( -1 );
		}
	}
	/* Seek the end of the chunks section
	*/
	LIBEWF_VERBOSE_PRINT( "libewf_write_segment_file_chunks_section_correction: setting file descriptor back to end of data at offset: %" PRIu32 ".\n", start_offset );

	if( libewf_common_lseek( file_descriptor, start_offset, SEEK_SET ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to find offset to continue.\n" );

		return( -1 );
	}
	if( ( internal_handle->ewf_format == EWF_FORMAT_E01 ) && ( internal_handle->format != LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* Write table section start
		 */
		write_count = libewf_section_table_write( internal_handle, file_descriptor, start_offset, internal_handle->offset_table, ( internal_handle->write->amount_of_chunks - internal_handle->write->section_amount_of_chunks ), internal_handle->write->section_amount_of_chunks, (EWF_CHAR *) "table", 0 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to rewrite table section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "table", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to append table section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;

		/* Write table2 section start
		 */
		write_count = libewf_section_table_write( internal_handle, file_descriptor, start_offset, internal_handle->offset_table, ( internal_handle->write->amount_of_chunks - internal_handle->write->section_amount_of_chunks ), internal_handle->write->section_amount_of_chunks, (EWF_CHAR *) "table2", 0 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to rewrite table2 section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "table2", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_chunks_section_correction: unable to append table2 section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Write the necessary sections at the end of the segment file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_write_segment_file_end( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_SECTION_LIST *section_list, int file_descriptor, off_t start_offset, uint8_t last_segment_file )
{
	LIBEWF_CHAR *md5_hash_string = NULL;
	ssize_t total_write_count    = 0;
	ssize_t write_count          = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: invalid handle.\n" );

		return( -1 );
	}
	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: invalid section list.\n" );

		return( -1 );
	}
	if( file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: invalid file descriptor.\n" );

		return( -1 );
	}
	if( last_segment_file == 0 )
	{
		/* Write next section
		 */
		write_count = libewf_section_last_write( internal_handle, file_descriptor, (EWF_CHAR *) "next", start_offset );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to write next section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "next", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to append next section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	else
	{
		if( internal_handle->calculated_md5_hash != NULL )
		{
			/* Write the hash section
			 */
			write_count = libewf_section_hash_write( internal_handle, file_descriptor, start_offset, internal_handle->calculated_md5_hash );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to write hash section.\n" );

				return( -1 );
			}
			if( libewf_section_list_append( section_list, (EWF_CHAR *) "hash", start_offset, ( start_offset + write_count ) ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to append hash section to section list.\n" );

				return( -1 );
			}
			start_offset      += write_count;
			total_write_count += write_count;

			if( libewf_internal_handle_set_stored_md5_hash( internal_handle, internal_handle->calculated_md5_hash ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to set stored MD5 hash in handle.\n" );

				return( -1 );
			}
			if( internal_handle->format == LIBEWF_FORMAT_EWFX )
			{
				md5_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 );

				if( md5_hash_string == NULL )
				{
					LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to create MD5 hash string.\n" );

					return( -1 );
				}
				if( libewf_string_copy_from_digest_hash( md5_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5, internal_handle->stored_md5_hash, EWF_DIGEST_HASH_SIZE_MD5 ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to set MD5 hash string.\n" );

					libewf_common_free( md5_hash_string );

					return( -1 );
				}
				if( libewf_internal_handle_set_hash_value( internal_handle, _S_LIBEWF_CHAR( "MD5" ), md5_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to set MD5 hash string in hash values.\n" );

					libewf_common_free( md5_hash_string );

					return( -1 );
				}
				libewf_common_free( md5_hash_string );
			}
		}
		/* Write the xhash section
		 */
		if( internal_handle->format == LIBEWF_FORMAT_EWFX )
		{
			if( internal_handle->xhash != NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: xhash already set - cleaning previous defintion.\n" );

				libewf_common_free( internal_handle->xhash );
			}
			internal_handle->xhash = libewf_hash_values_generate_xhash_string_ewfx( internal_handle->hash_values, &internal_handle->xhash_size );

			if( internal_handle->xhash == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to generate xhash.\n" );

				return( -1 );
			}
			write_count = libewf_section_xhash_write( internal_handle, file_descriptor, start_offset, internal_handle->xhash, internal_handle->xhash_size, EWF_COMPRESSION_DEFAULT );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to write xhash section.\n" );

				return( -1 );
			}
			if( libewf_section_list_append( section_list, (EWF_CHAR *) "xhash", start_offset, ( start_offset + write_count ) ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to append xhash section to section list.\n" );

				return( -1 );
			}
			start_offset      += write_count;
			total_write_count += write_count;
		}
		/* Write the done section
		 */
		write_count = libewf_section_last_write( internal_handle, file_descriptor, (EWF_CHAR *) "done", start_offset );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to write done section.\n" );

			return( -1 );
		}
		if( libewf_section_list_append( section_list, (EWF_CHAR *) "done", start_offset, ( start_offset + write_count ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_end: unable to append done section to section list.\n" );

			return( -1 );
		}
		start_offset      += write_count;
		total_write_count += write_count;
	}
	return( total_write_count );
}

/* Creates and opens a new segment file and writes the necessary sections at the start of the segment file
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_write_segment_file_open( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number )
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	wchar_t *filename         = NULL;
	wchar_t *error_string     = NULL;
#else
	char *filename            = NULL;
	char *error_string        = NULL;
#endif
	off_t start_offset        = 0;
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;
	int file_descriptor       = -1;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: invalid handle - missing segment table.\n" );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: invalid segment number: 0.\n" );

		return( -1 );
	}
	/* Create a new segment file
	 */
	if( libewf_write_create_segment_file_entry( internal_handle, segment_number ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: unable to create entry for segment file: %" PRIu32 ".\n", segment_number );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	filename = libewf_segment_table_get_wide_filename( internal_handle->segment_table, segment_number );
#else
	filename = libewf_segment_table_get_filename( internal_handle->segment_table, segment_number );
#endif

	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: invalid filename for segment file: %" PRIu32 ".\n", segment_number );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	file_descriptor = libewf_common_wide_open( filename, LIBEWF_OPEN_WRITE );

	if( file_descriptor == -1 )
	{
		error_string = libewf_common_wide_strerror( errno );

		if( error_string == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: unable to open file: %ls.\n", filename );
		}
		else
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: unable to open file: %ls with error: %ls.\n", filename, error_string );

			libewf_common_free( error_string );
		}
		return( -1 );
	}
#else
	file_descriptor = libewf_common_open( filename, LIBEWF_OPEN_WRITE );

	if( file_descriptor == -1 )
	{
		error_string = libewf_common_strerror( errno );

		if( error_string == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: unable to open file: %s.\n", filename );
		}
		else
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: unable to open file: %s with error: %s.\n", filename, error_string );

			libewf_common_free( error_string );
		}
		return( -1 );
	}
#endif
	if( libewf_segment_table_set_file_descriptor( internal_handle->segment_table, segment_number, file_descriptor ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: unable to set file descriptor in segment table.\n" );

		return( -1 );
	}

	/* Write the start of the segment file
	 * like the file header, the header, volume and/or data section, etc.
	 */
	write_count = libewf_write_segment_file_start( internal_handle, segment_number, file_descriptor, internal_handle->segment_table->section_list[ segment_number ] );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_open: unable to write segment file start.\n" );

		return( -1 );
	}
	start_offset      += write_count;
	total_write_count += write_count;

	return( total_write_count );
}

/* Closes the segment file, necessary sections at the end of the segment file will be written
 * Returns the amount of bytes written, or -1 on error
 */
ssize_t libewf_write_segment_file_close( LIBEWF_INTERNAL_HANDLE *internal_handle, uint16_t segment_number, uint8_t last_segment_file )
{
	LIBEWF_SECTION_LIST *section_list = NULL;
	off_t segment_file_offset         = 0;
	size_t md5_hash_size              = 0;
	ssize_t total_write_count         = 0;
	ssize_t write_count               = 0;
	int file_descriptor               = -1;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: invalid segment number: 0.\n" );

		return( -1 );
	}
	section_list        = internal_handle->segment_table->section_list[ segment_number ];
	file_descriptor     = internal_handle->segment_table->file_descriptor[ segment_number ];
	segment_file_offset = internal_handle->segment_table->file_offset[ segment_number ];

	if( file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: invalid file descriptor.\n" );

		return( -1 );
	}
	if( last_segment_file == 1 )
	{
		md5_hash_size = EWF_DIGEST_HASH_SIZE_MD5;

		if( internal_handle->calculated_md5_hash == NULL )
		{
			internal_handle->calculated_md5_hash = (EWF_DIGEST_HASH *) libewf_common_alloc( md5_hash_size );

			if( internal_handle->calculated_md5_hash == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: unable to create MD5 hash.\n" );

				return( -1 );
			}
		}
		/* Complete the MD5 hash calculation
		 */
		if( ( libewf_md5_finalize( &internal_handle->md5_context, internal_handle->calculated_md5_hash, &md5_hash_size ) != 1 )
		 || ( md5_hash_size != EWF_DIGEST_HASH_SIZE_MD5 ) )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: unable to finalize MD5 context.\n" );

			return( -1 );
		}
		/* Write the data section for a single segment file only for EWF-E01
		 */
		if( ( internal_handle->ewf_format == EWF_FORMAT_E01 ) && ( segment_number == 1 ) )
		{
			write_count = libewf_section_data_write( internal_handle, file_descriptor, segment_file_offset );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: unable to write data section.\n" );

				return( -1 );
			}
			if( libewf_section_list_append( section_list, (EWF_CHAR *) "data", segment_file_offset, ( segment_file_offset + write_count ) ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: unable to append data section to section list.\n" );

				return( -1 );
			}
			segment_file_offset += write_count;
			total_write_count   += write_count;
		}
		/* Write the error2 section if required 
		 */
		if( ( internal_handle->acquiry_amount_of_errors > 0 )
		 && ( ( internal_handle->format == LIBEWF_FORMAT_ENCASE3 )
		   || ( internal_handle->format == LIBEWF_FORMAT_ENCASE4 )
		   || ( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
		   || ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
		   || ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
		   || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
		   || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) ) )
		{
			write_count = libewf_section_error2_write( internal_handle, file_descriptor, segment_file_offset, internal_handle->acquiry_error_sectors, internal_handle->acquiry_amount_of_errors );

			if( write_count == -1 )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: unable to write error2 section.\n" );

				return( -1 );
			}
			if( libewf_section_list_append( section_list, (EWF_CHAR *) "error2", segment_file_offset, ( segment_file_offset + write_count ) ) == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: unable to append error2 section to section list.\n" );

				return( -1 );
			}
			segment_file_offset += write_count;
			total_write_count   += write_count;
		}
	}
	/* Write the end of the segment file
	 * like the next, done and/or hash section
	 */
	write_count = libewf_write_segment_file_end( internal_handle, section_list, file_descriptor, segment_file_offset, last_segment_file );

	if( write_count == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: unable to write end of segment file.\n" );

		return( -1 );
	}
	segment_file_offset += write_count;
	total_write_count   += write_count;

	if( libewf_common_close( file_descriptor ) != 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_segment_file_close: unable to close segment file.\n" );

		return( -1 );
	}
	internal_handle->segment_table->file_descriptor[ segment_number ]  = -1;
	internal_handle->segment_table->amount_of_chunks[ segment_number ] = internal_handle->write->segment_amount_of_chunks;

	if( last_segment_file == 1 )
	{
		internal_handle->write->write_finalized = 1;
	}
	return( total_write_count );
}

/* Writes a chunk of data in EWF format from a buffer, the necessary settings of the write values must have been made
 * This function swaps byte pairs
 * Returns the amount of input bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_write_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk, uint32_t chunk_offset, void *buffer, size_t size, uint8_t force_write )
{
	EWF_CHUNK *chunk_data       = NULL;
	ssize_t write_count         = 0;
	size_t write_size           = 0;
	size_t read_size            = 0;
	size_t remaining_chunk_size = 0;
	size_t chunk_cache_offset   = 0;
	uint16_t segment_number     = 0;
	int8_t result               = 0;
	int8_t chunk_cache_used     = 0;
	uint8_t last_segment_file   = 0;
	uint8_t existing_chunk      = 0;
	int file_descriptor         = -1;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid handle.\n" );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid handle - missing chunk cache.\n" );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid handle - missing segment table.\n" );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid buffer.\n" );

		return( -1 );
	}
	if( buffer == internal_handle->chunk_cache->compressed )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid buffer - chunk cache compressed cannot be used as buffer.\n" );

		return( -1 );
	}
	/* Check if chunk cache passthrough is used
	 * if the chunk cache is used as the chunk data buffer
	 */
	chunk_cache_used = (int8_t) ( buffer == internal_handle->chunk_cache->data );

	if( libewf_internal_handle_write_is_initialized( internal_handle ) == 0 )
	{
		if( libewf_internal_handle_write_initialize( internal_handle ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to initialize write values.\n" );

			return( -1 );
		}
		if( ( chunk_cache_used == 1 ) && ( buffer != internal_handle->chunk_cache->data ) )
		{
			buffer = (void *) internal_handle->chunk_cache->data;
		}
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid handle - missing offset table.\n" );

		return( -1 );
	}
	/* Check if the write was already finalized
	 */
	if( internal_handle->write->write_finalized == 1 )
	{
		return( 0 );
	}
	/* Check if the amount of bytes as specified have been written
	 */
	if( ( internal_handle->write->input_write_size != 0 ) && ( internal_handle->write->input_write_count >= (int64_t) internal_handle->write->input_write_size ) )
	{
		return( 0 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: writing buffer of size: %zu.\n", size );

	/* Check if chunk has already been created within a segment file
	 */
	if( chunk < internal_handle->offset_table->amount )
	{
		file_descriptor = internal_handle->offset_table->file_descriptor[ chunk ];
		segment_number  = internal_handle->offset_table->segment_number[ chunk ];
	}
	/* If no valid segment number was provided get the last valid one
	 */
	if( ( segment_number == 0 ) || ( segment_number > internal_handle->segment_table->amount ) )
	{
		segment_number = ( internal_handle->segment_table->amount - 1 );
	}
	existing_chunk = (uint8_t) ( file_descriptor != -1 );

	/* If the chunk has not been created check if the segment file has been created
	 */
	if( existing_chunk == 0 )
	{
		file_descriptor = internal_handle->segment_table->file_descriptor[ segment_number ];
	}
	/* Check if a new segment file should be created
	 */
	if( file_descriptor == -1 )
	{
		internal_handle->write->create_chunks_section     = 1;
		internal_handle->write->chunks_section_number     = 0;
		internal_handle->write->chunks_per_chunks_section = 0;
		internal_handle->write->segment_amount_of_chunks  = 0;

		segment_number += 1;

		LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: creating segment file with segment number: %" PRIu16 ".\n", segment_number );

		write_count = libewf_write_segment_file_open( internal_handle, segment_number );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to create segment file for segment: %" PRIu16 ".\n", segment_number );

			return( -1 );
		}
		internal_handle->segment_table->file_offset[ segment_number ] += write_count;
		internal_handle->write->write_count                           += write_count;

		internal_handle->write->chunks_per_segment = libewf_write_calculate_chunks_per_segment( internal_handle, segment_number );

		if( internal_handle->write->chunks_per_segment == 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to determine the amount of chunks per segment.\n" );

			return( -1 );
		}
		LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: calculated amount of chunks for segment: %" PRIu32 ".\n", internal_handle->write->chunks_per_segment );

		file_descriptor = internal_handle->segment_table->file_descriptor[ segment_number ];
	}
	/* Check if another chunk section should be created
	 */
	if( internal_handle->write->create_chunks_section == 1 )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: creating chunks section.\n" );

		internal_handle->write->create_chunks_section      = 0;
		internal_handle->write->section_amount_of_chunks   = 0;
		internal_handle->write->chunks_section_write_count = 0;
		internal_handle->write->chunks_section_offset      = internal_handle->segment_table->file_offset[ segment_number ];

		/* Start with chunks section number number 1, value is initialized with 0
		 */
		internal_handle->write->chunks_section_number += 1;

		/* Recalculate the amount of chunks per segment for a better segment file fill when compression is used
		 */
		internal_handle->write->chunks_per_segment = libewf_write_calculate_chunks_per_segment( internal_handle, segment_number );

		if( internal_handle->write->chunks_per_segment == 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to determine the amount of chunks per segment.\n" );

			return( -1 );
		}
		LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: calculated amount of chunks for segment: %" PRIu32 ".\n", internal_handle->write->chunks_per_segment );

		internal_handle->write->chunks_per_chunks_section = libewf_write_calculate_chunks_per_chunks_section( internal_handle );

		if( internal_handle->write->chunks_per_segment == 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to determine the amount of chunks per chunks section.\n" );

			return( -1 );
		}
		LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: calculated amount of chunks for chunks section: %" PRIu32 ".\n", internal_handle->write->chunks_per_chunks_section );

		/* Write the section start of the chunks section
		 */
		write_count = libewf_write_segment_file_chunks_section_start( internal_handle, file_descriptor, internal_handle->segment_table->file_offset[ segment_number ], internal_handle->write->amount_of_chunks, internal_handle->write->chunks_per_chunks_section );

		if( write_count == -1 )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: unable to write section start for chunks.\n" );

			return( -1 );
		}
		internal_handle->segment_table->file_offset[ segment_number ] += write_count;
		internal_handle->write->write_count                           += write_count;
	}
	chunk_cache_offset = internal_handle->chunk_cache->offset;

	/* Determine the size of data to read
	 */
	if( size < (uint64_t) internal_handle->media->chunk_size )
	{
		read_size = (uint32_t) size;
	}
	else
	{
		read_size = internal_handle->media->chunk_size;
	}
	if( read_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid read size only values below 2^32 are supported.\n" );

		return( -1 );
	}
	/* If the chunk exists retrieve the chunk data if necessary
	 */
	if( ( existing_chunk == 1 ) && ( read_size < internal_handle->media->chunk_size ) )
	{
		/* TODO how to handle chunk cache passthrough? especially compressed
		 */
		if( chunk_cache_used == 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: cannot handle chunk cache passthrough here yet.\n" );

			return( -1 );
		}
		if( internal_handle->offset_table->compressed[ chunk ] == 1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: cannot over write compressed chunks safely yet.\n" );

			return( -1 );
		}
		if( internal_handle->chunk_cache->offset != 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: cannot handle remaining data in chunk cache here yet.\n" );

			return( -1 );
		}
		if( libewf_read_chunk( internal_handle, chunk, 0, internal_handle->chunk_cache->data, internal_handle->chunk_cache->allocated_size ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to read data from chunk.\n" );

			return( -1 );
		}
		/* Make sure to put the file offset back to the beginning of the chunk
		 */
		if( libewf_seek_chunk( internal_handle, chunk ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to seek chunk.\n" );

			return( -1 );
		}
		if( libewf_common_memcpy( &internal_handle->chunk_cache->data[ chunk_offset ], buffer, read_size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to write data.\n" );

			return( -1 );
		}
		chunk_data = internal_handle->chunk_cache->data;
		write_size = internal_handle->chunk_cache->amount;
	}
	/* Check if chunk cache passthrough is being used
	 */
	else if( chunk_cache_used == 1 )
	{
		chunk_data = internal_handle->chunk_cache->data;
		write_size = read_size;

		/* Make sure the chunk cache amount and offset are set correctly
		 */
		internal_handle->chunk_cache->amount = read_size;

		if( read_size < internal_handle->chunk_cache->allocated_size )
		{
			internal_handle->chunk_cache->offset = read_size;
		}
		else
		{
			internal_handle->chunk_cache->offset = 0;
		}
	}
#ifdef HAVE_BUFFER_PASSTHROUGH
	/* Check if buffer contain the necessary amount of bytes to fill a chunk
	 * and no data was previously copied into the chunk cache
	 */
	else if( ( chunk_cache_offset == 0 ) && ( size >= (uint64_t) internal_handle->media->chunk_size ) )
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

		if( read_size > (uint32_t) remaining_chunk_size )
		{
			read_size = remaining_chunk_size;
		}
		LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: reading %zu bytes from buffer.\n", read_size );

		if( libewf_common_memcpy( &internal_handle->chunk_cache->data[ chunk_cache_offset ], buffer, read_size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to write data.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: invalid chunk cache amount.\n" );

			return( -1 );
		}
		chunk_data = internal_handle->chunk_cache->data;
		write_size = internal_handle->chunk_cache->amount;
	}
	if( ( write_size == internal_handle->media->chunk_size )
	 || ( ( internal_handle->write->input_write_size != 0 ) && ( ( internal_handle->write->input_write_count + write_size ) == (int64_t) internal_handle->write->input_write_size ) )
	 || ( force_write != 0 ) )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: writing %zu bytes to file.\n", write_size );

		write_count = libewf_write_segment_file_chunk( internal_handle, segment_number, file_descriptor, chunk_data, internal_handle->segment_table->file_offset[ segment_number ], write_size, internal_handle->write->amount_of_chunks );

		/* Adjust chunk data buffer if necessary
		 */
		if( ( chunk_cache_used == 1 ) && ( buffer != internal_handle->chunk_cache->data ) )
		{
			buffer = (void *) internal_handle->chunk_cache->data;
		}
		if( write_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to write data.\n" );

			return( -1 );
		}
		internal_handle->write->input_write_count                     += write_size;
		internal_handle->segment_table->file_offset[ segment_number ] += write_count;
		internal_handle->write->write_count                           += write_count;
		internal_handle->write->chunks_section_write_count            += write_count;
		internal_handle->write->segment_amount_of_chunks              += 1;
		internal_handle->write->section_amount_of_chunks              += 1;
		internal_handle->write->amount_of_chunks                      += 1;
	}
	/* Check if the current chunks section is full, if so close the current section
	 */
	result = libewf_write_test_chunks_section_full( internal_handle, internal_handle->segment_table->file_offset[ segment_number ] );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to determine if chunks section is full.\n" );

		return( -1 );
	}
	else if( result == 1 )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: closing chunks section amount of data written: %" PRIi32 ".\n", internal_handle->write->chunks_section_write_count );

		/* Correct the offset, size in the chunks section
		 */
		write_count = libewf_write_segment_file_chunks_section_correction( internal_handle, file_descriptor, internal_handle->segment_table->file_offset[ segment_number ], internal_handle->segment_table->section_list[ segment_number ], internal_handle->write->chunks_section_offset, (uint32_t) internal_handle->write->chunks_section_write_count );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to correct chunks section.\n" );

			return( -1 );
		}
		internal_handle->segment_table->file_offset[ segment_number ] += write_count;
		internal_handle->write->write_count                           += write_count;
		internal_handle->write->create_chunks_section                 = 1;
	}
	/* Check if the current segment file is full, if so close the current segment file
	 */
	result = libewf_write_test_segment_file_full( internal_handle, internal_handle->segment_table->file_offset[ segment_number ] );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to determine if segment file is full.\n" );

		return( -1 );
	}
	else if( result == 1 )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_chunk: closing segment file with segment number: %" PRIu16 ".\n", segment_number );

		/* Check if this is the last segment file
		 */
		if( ( internal_handle->write->input_write_size != 0 ) && ( internal_handle->write->input_write_count >= (int64_t) internal_handle->write->input_write_size ) )
		{
			last_segment_file = 1;
		}
		/* Finish and close the segment file
		 */
		write_count = libewf_write_segment_file_close( internal_handle, segment_number, last_segment_file );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_chunk: unable to close segment file.\n" );

			return( -1 );
		}
		internal_handle->segment_table->file_offset[ segment_number ] += write_count;
		internal_handle->write->write_count                           += write_count;
		internal_handle->write->chunks_section_offset                  = 0;
	}
	/* Report the amount of data written to the chunk cache
	 */
	return( (int32_t) read_size );
}

/* Writes data in EWF format from a buffer, the necessary settings of the write values must have been made
 * This function swaps byte pairs
 * Returns the amount of input bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_write_buffer( LIBEWF_HANDLE *handle, void *buffer, size_t size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	ssize_t chunk_write_count               = 0;
	ssize_t total_write_count               = 0;
	size_t chunk_data_size                  = 0;
	int8_t chunk_cache_used                 = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_buffer: invalid handle.\n" );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_buffer: invalid chunk cache.\n" );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_buffer: invalid buffer.\n" );

		return( -1 );
	}
	if( buffer == internal_handle->chunk_cache->compressed )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_buffer: invalid buffer - same as chunk cache compressed.\n" );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_buffer: invalid size value exceeds maximum.\n" );

		return( -1 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_write_buffer: writing size: %zu.\n", size );

	/* Check if chunk cache passthrough is used
	 * if the chunk cache is used as the chunk data buffer
	 */
	chunk_cache_used = (int8_t) ( buffer == internal_handle->chunk_cache->data );

	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents multiple reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media->chunk_size + EWF_CRC_SIZE;

	if( buffer == internal_handle->chunk_cache->compressed )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_buffer: chunk cache compressed cannot be used as buffer.\n");

		return( -1 );
	}
	if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_buffer: reallocating chunk data size: %zu.\n", chunk_data_size );

		if( libewf_internal_handle_chunk_cache_realloc( internal_handle, chunk_data_size ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_buffer: unable to reallocate chunk cache.\n");

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
		chunk_write_count = libewf_write_chunk( internal_handle, internal_handle->current_chunk, internal_handle->current_chunk_offset, (void *) &( (uint8_t *) buffer )[ total_write_count ], size, 0 );

		/* libewf_write_chunk could relocate the chunk cache
		 * correct buffer is chunk cache passthrough is used
		 */
		if( ( chunk_cache_used == 1 ) && ( buffer != internal_handle->chunk_cache->data ) )
		{
			buffer = (void *) internal_handle->chunk_cache->data;
		}
		if( chunk_write_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_buffer: unable to write data from chunk.\n" );

			return( -1 );
		}
		else if( chunk_write_count == 0 )
		{
			break;
		}
		size                                  -= chunk_write_count;
		total_write_count                     += chunk_write_count;
		internal_handle->current_chunk_offset += (uint32_t) chunk_write_count;

		if( internal_handle->current_chunk_offset == internal_handle->media->chunk_size )
		{
			internal_handle->current_chunk_offset = 0;
			internal_handle->current_chunk       += 1;
		}
		else if( internal_handle->current_chunk_offset > internal_handle->media->chunk_size )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_buffer: invalid current chunk offset.\n" );

			return( -1 );
		}
	}
	return( total_write_count );
}

/* Writes data in EWF format from a buffer at an specific offset,
 * the necessary settings of the write values must have been made
 * This function swaps byte pairs
 * Returns the amount of input bytes written, 0 when no longer bytes can be written, or -1 on error
 */
ssize_t libewf_write_random( LIBEWF_HANDLE *handle, void *buffer, size_t size, off_t offset )
{
	ssize_t write_count = 0;

	if( libewf_seek_offset( handle, offset ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_random: unable to seek offset.\n" );

		return( -1 );
	}
	write_count = libewf_write_buffer( handle, buffer, size );

	if( write_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_random: unable to write buffer.\n" );

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
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	wchar_t *filename                              = NULL;
	wchar_t *error_string                          = NULL;
#else
	char *filename                                 = NULL;
	char *error_string                             = NULL;
#endif
	ssize_t write_count_finalize                   = 0;
	ssize_t write_count                            = 0;
	uint16_t segment_table_iterator                = 0;
	uint16_t segment_number                        = 0;
	int file_descriptor                            = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_finalize: invalid handle.\n" );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_finalize: invalid handle - missing subhandle media.\n" );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_finalize: invalid handle - missing subhandle write.\n" );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_finalize: invalid handle - missing chunk cache.\n" );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_write_finalize: invalid handle - missing segment table.\n" );

		return( -1 );
	}
	if( internal_handle->write->write_finalized == 1 )
	{
		return( 0 );
	}
	LIBEWF_VERBOSE_PRINT( "libewf_write_finalize: finalizing write.\n" );

	/* Write data remaining in the chunk cache to file
	 */
	/* TODO improve test for RW
	 */
	if( ( internal_handle->current_chunk_offset != 0 ) && ( internal_handle->chunk_cache->amount != 0 ) && ( internal_handle->chunk_cache->offset != 0 ) )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_write_finalize: writing chunk remainder at offset: %" PRIu32 " with size: %" PRIu32 "\n", internal_handle->current_chunk_offset, internal_handle->chunk_cache->amount );

		write_count = libewf_write_chunk( internal_handle, internal_handle->current_chunk, internal_handle->current_chunk_offset, internal_handle->chunk_cache->data, internal_handle->chunk_cache->amount, 1 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to write remaining chunk data.\n" );

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
		LIBEWF_VERBOSE_PRINT( "libewf_write_finalize: correcting chunks section.\n" );

		write_count = libewf_write_segment_file_chunks_section_correction( internal_handle, file_descriptor, internal_handle->segment_table->file_offset[ segment_number ], internal_handle->segment_table->section_list[ segment_number ], internal_handle->write->chunks_section_offset, (uint32_t) internal_handle->write->chunks_section_write_count );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to correct chunks section.\n" );

			return( -1 );
		}
		write_count_finalize                                          += write_count;
		internal_handle->segment_table->file_offset[ segment_number ] += write_count;
		internal_handle->write->write_count                           += write_count;

		/* Close the segment file
		 */
		LIBEWF_VERBOSE_PRINT( "libewf_write_finalize: closing segment file.\n" );

		write_count = libewf_write_segment_file_close( internal_handle, segment_number, 1 );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to close segment file.\n" );

			return( -1 );
		}
		write_count_finalize                                          += write_count;
		internal_handle->segment_table->file_offset[ segment_number ] += write_count;
		internal_handle->write->write_count                           += write_count;
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
			LIBEWF_WARNING_PRINT( "libewf_write_finalize: missing section list.\n" );

			return( -1 );
		}
		if( internal_handle->segment_table->section_list[ segment_table_iterator ]->first == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_finalize: missing section list entries.\n" );

			return( -1 );
		}
		file_descriptor = internal_handle->segment_table->file_descriptor[ segment_table_iterator ];

		if( file_descriptor == -1 )
		{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
			filename = libewf_segment_table_get_wide_filename( internal_handle->segment_table, segment_table_iterator );
#else
			filename = libewf_segment_table_get_filename( internal_handle->segment_table, segment_table_iterator );
#endif

			if( filename == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_write_finalize: invalid filename for segment file: %" PRIu32 ".\n", segment_table_iterator );

				return( -1 );
			}
			file_descriptor = libewf_common_open( filename, LIBEWF_OPEN_READ_WRITE );

			if( file_descriptor == -1 )
			{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
				error_string = libewf_common_wide_strerror( errno );

				if( error_string == NULL )
				{
					LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to open file: %ls.\n", filename );
				}
				else
				{
					LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to open file: %ls with error: %ls.\n", filename, error_string );

					libewf_common_free( error_string );
				}
#else
				error_string = libewf_common_strerror( errno );

				if( error_string == NULL )
				{
					LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to open file: %s.\n", filename );
				}
				else
				{
					LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to open file: %s with error: %s.\n", filename, error_string );

					libewf_common_free( error_string );
				}
#endif
				return( -1 );
			}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
			LIBEWF_VERBOSE_PRINT( "libewf_write_finalize: correcting segment file: %ls.\n", filename );
#else
			LIBEWF_VERBOSE_PRINT( "libewf_write_finalize: correcting segment file: %s.\n", filename );
#endif
		}
		list_entry_iterator = internal_handle->segment_table->section_list[ segment_table_iterator ]->first;

		while( list_entry_iterator != NULL )
		{
			if( libewf_common_memcmp( list_entry_iterator->type, "volume", 6 ) == 0 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_write_finalize: correcting volume section.\n" );

				if( libewf_common_lseek( file_descriptor, list_entry_iterator->start_offset, SEEK_SET ) == -1 )
				{
					LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to find offset to correct volume section.\n" );

					return( -1 );
				}
				if( internal_handle->ewf_format == EWF_FORMAT_S01 )
				{
					/* Write volume (SMART) section
					 */
					write_count = libewf_section_volume_s01_write( internal_handle, file_descriptor, (uint32_t) list_entry_iterator->start_offset );
				}
				else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
				{
					/* Write volume section
					 */
					write_count = libewf_section_volume_e01_write( internal_handle, file_descriptor, (uint32_t) list_entry_iterator->start_offset );
				}
				else
				{
					write_count = -1;
				}
				if( write_count == -1 )
				{
					LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to correct volume section.\n" );

					return( -1 );
				}
			}
			else if( libewf_common_memcmp( list_entry_iterator->type, "data", 4 ) == 0 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_write_finalize: correcting data section.\n" );

				if( libewf_common_lseek( file_descriptor, list_entry_iterator->start_offset, SEEK_SET ) == -1 )
				{
					LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to find offset to data volume section.\n" );

					return( -1 );
				}
				/* Write data section
				 */
				write_count = libewf_section_data_write( internal_handle, file_descriptor, (uint32_t) list_entry_iterator->start_offset );

				if( write_count == -1 )
				{
					LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to correct data section.\n" );

					return( -1 );
				}
			}
			list_entry_iterator = list_entry_iterator->next;
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_write_finalize: unable to close segment file.\n" );

			return( -1 );
		}
		internal_handle->segment_table->file_descriptor[ segment_table_iterator ] = -1;
	}
	return( write_count_finalize );
}

