/*
 * Low level writing functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libewf_chunk_cache.h"
#include "libewf_compression.h"
#include "libewf_definitions.h"
#include "libewf_header_values.h"
#include "libewf_libbfio.h"
#include "libewf_media_values.h"
#include "libewf_notify.h"
#include "libewf_read_io_handle.h"
#include "libewf_section.h"
#include "libewf_sector_table.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_values_table.h"
#include "libewf_write_io_handle.h"

#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_section.h"
#include "ewf_table.h"
#include "ewfx_delta_chunk.h"

/* Initialize the write io handle
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_initialize(
     libewf_write_io_handle_t **write_io_handle,
     liberror_error_t **error )
{
	static char *function = "libewf_write_io_handle_initialize";

	if( write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( -1 );
	}
	if( *write_io_handle == NULL )
	{
		*write_io_handle = (libewf_write_io_handle_t *) memory_allocate(
		                                                 sizeof( libewf_write_io_handle_t ) );

		if( write_io_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create write io handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *write_io_handle,
		     0,
		     sizeof( libewf_write_io_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear write io handle.",
			 function );

			memory_free(
			 *write_io_handle );

			*write_io_handle = NULL;

			return( -1 );
		}
		( *write_io_handle )->maximum_segment_file_size        = INT32_MAX;
		( *write_io_handle )->remaining_segment_file_size      = LIBEWF_DEFAULT_SEGMENT_FILE_SIZE;
		( *write_io_handle )->maximum_section_amount_of_chunks = EWF_MAXIMUM_OFFSETS_IN_TABLE;
	}
	return( 1 );
}

/* Frees the write io handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_free(
     libewf_write_io_handle_t **write_io_handle,
     liberror_error_t **error )
{
	static char *function = "libewf_write_io_handle_free";

	if( write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( 1 );
	}
	if( *write_io_handle != NULL )
	{
		if( ( *write_io_handle )->data_section != NULL )
		{
			memory_free(
			 ( *write_io_handle )->data_section );
		}
		if( ( *write_io_handle )->table_offsets != NULL )
		{
			memory_free(
			 ( *write_io_handle )->table_offsets );
		}
		memory_free(
		 *write_io_handle );

		*write_io_handle = NULL;
	}
	return( 1 );
}

/* Initializes the write io handle value to start writing
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_initialize_values(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     liberror_error_t **error )
{
	static char *function               = "libewf_write_io_handle_initialize_values";
	int64_t required_amount_of_segments = 0;

	if( write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( -1 );
	}
	if( write_io_handle->values_initialized != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: write values were initialized and cannot be initialized anymore.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_table->maximum_segment_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid segment file size value out of range.",
		 function );

		return( -1 );
	}
	/* Determine the EWF file format
	 */
	if( io_handle->format == LIBEWF_FORMAT_LVF )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: writing format LVF currently not supported.",
		 function );

		return( -1 );
	}
	/* If no input write size was provided check if EWF file format allows for streaming
	 */
	if( media_values->media_size == 0 )
	{
		if( ( io_handle->format != LIBEWF_FORMAT_ENCASE2 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE3 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE4 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE5 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE6 )
		 && ( io_handle->format != LIBEWF_FORMAT_LINEN5 )
		 && ( io_handle->format != LIBEWF_FORMAT_LINEN6 )
		 && ( io_handle->format != LIBEWF_FORMAT_FTK )
		 && ( io_handle->format != LIBEWF_FORMAT_EWFX ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: EWF file format does not allow for streaming write.",
			 function );

			return( -1 );
		}
	}
	else
	{
		/* Determine the required amount of segments allowed to write
		 */
		required_amount_of_segments = (int64_t) media_values->media_size / (int64_t) segment_table->maximum_segment_size;

		if( required_amount_of_segments > (int64_t) write_io_handle->maximum_amount_of_segments )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: the maximum amount of allowed segment files will be exceeded with the segment file size: %" PRIu64 ".",
			 function,
			 segment_table->maximum_segment_size );

			return( -1 );
		}
	}
	if( media_values->media_size > LIBEWF_2_TIB )
	{
		if( ( io_handle->format != LIBEWF_FORMAT_ENCASE6 )
		 && ( io_handle->format != LIBEWF_FORMAT_LINEN6 )
		 && ( io_handle->format != LIBEWF_FORMAT_EWFX ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: EWF file format does not allow for a media size greater than 2 TiB.",
			 function );

			return( -1 );
		}
	}

	/* Flag that the write values were initialized
	 */
	write_io_handle->values_initialized = 1;

	return( 1 );
}

/* Initializes the write io handle to resume writing
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_initialize_resume(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_segment_table_t *segment_table,
     liberror_error_t **error )
{
	libewf_list_element_t *section_list_element       = NULL;
	libewf_section_list_values_t *section_list_values = NULL;
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_write_io_handle_initialize_resume";
	uint16_t segment_number                           = 0;
	uint8_t backtrace_to_last_chunks_sections         = 0;
	uint8_t reopen_segment_file                       = 0;

	if( write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment table - missing segment file handles.",
		 function );

		return( -1 );
	}
	segment_number = segment_table->amount - 1;

	if( segment_number == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment files.",
		 function );

		return( -1 );
	}
	segment_file_handle = segment_table->segment_file_handle[ segment_number ];

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment file handle: %" PRIu16 ".",
		 function,
		 segment_number );

		return( -1 );
	}
	if( segment_file_handle->section_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing section list.",
		 function );

		return( -1 );
	}
	section_list_element = segment_file_handle->section_list->last;

	if( section_list_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing last section list element.",
		 function );

		return( -1 );
	}
	section_list_values = (libewf_section_list_values_t *) section_list_element->value;

	if( section_list_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing last section list values.",
		 function );

		return( -1 );
	}
	if( ( segment_number == 1 )
	 && ( memory_compare(
	       (void *) section_list_values->type,
	       (void *) "data",
	       5 ) == 0 ) )
	{
		backtrace_to_last_chunks_sections = 1;
	}
	else if( memory_compare(
	          (void *) section_list_values->type,
	          (void *) "session",
	          8 ) == 0 )
	{
		backtrace_to_last_chunks_sections = 1;
	}
	else if( memory_compare(
	          (void *) section_list_values->type,
	          (void *) "error2",
	          7 ) == 0 )
	{
		backtrace_to_last_chunks_sections = 1;
	}
	else if( memory_compare(
	          (void *) section_list_values->type,
	          (void *) "digest",
	          7 ) == 0 )
	{
		backtrace_to_last_chunks_sections = 1;
	}
	else if( memory_compare(
	          (void *) section_list_values->type,
	          (void *) "hash",
	          5 ) == 0 )
	{
		backtrace_to_last_chunks_sections = 1;
	}
	else if( memory_compare(
	          (void *) section_list_values->type,
	          (void *) "xhash",
	          6 ) == 0 )
	{
		backtrace_to_last_chunks_sections = 1;
	}
	if( backtrace_to_last_chunks_sections != 0 )
	{
		while( section_list_element->previous != NULL )
		{
			section_list_element = section_list_element->previous;

			if( section_list_element->value == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing section list values.",
				 function );

				return( -1 );
			}
			section_list_values = (libewf_section_list_values_t *) section_list_element->value;

			if( memory_compare(
			     (void *) section_list_values->type,
			     (void *) "table",
			     5 ) == 0 )
			{
				break;
			}
		}
		if( section_list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing last chunks section.",
			 function );

			return( -1 );
		}
	}
	if( memory_compare(
	     (void *) section_list_values->type,
	     (void *) "data",
	     5 ) == 0 )
	{
		/* The sections containing the chunks and offsets were read entirely
		 * in the previous segment file
		 */
		reopen_segment_file                         = 1;
		write_io_handle->resume_segment_file_offset = section_list_values->end_offset;
		write_io_handle->create_chunks_section      = 1;
	}
	else if( memory_compare(
	          (void *) section_list_values->type,
	          (void *) "sectors",
	          8 ) == 0 )
	{
		/* Uncertain if the sections containing the chunks was read entirely
		 * the offsets to the chunks are missing so the chunks need to be rewritten anyway
		 */
		reopen_segment_file                         = 1;
		write_io_handle->resume_segment_file_offset = section_list_values->start_offset;
		write_io_handle->create_chunks_section      = 1;
	}
	else if( memory_compare(
	          (void *) section_list_values->type,
	          (void *) "table",
	          6 ) == 0 )
	{
		/* Determine if the table section also contains chunks
		 */
		if( section_list_element->previous == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing previous section list element.",
			 function );

			return( -1 );
		}
		if( section_list_element->previous->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing previous section list values.",
			 function );

			return( -1 );
		}
		if( memory_compare(
		     (void *) ( (libewf_section_list_values_t *) section_list_element->previous->value )->type,
		     (void *) "sectors",
		     8 ) == 0 )
		{
			if( offset_table->last_chunk_offset_compared >= offset_table->last_chunk_offset_filled )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
				 "%s: last chunk offset compared cannot be greater than last chunk offset filled.",
				 function );

				return( -1 );
			}
			/* The sections containing the chunks and offsets were read partially
			 */
			section_list_element = section_list_element->previous;
			section_list_values  = (libewf_section_list_values_t *) section_list_element->value;

			/* Reset the chunk offsets in the offset table
			 */
			if( memory_set(
			     &( offset_table->chunk_offset[ offset_table->last_chunk_offset_compared ] ),
			     0,
			     sizeof( libewf_chunk_offset_t ) * ( offset_table->last_chunk_offset_filled - offset_table->last_chunk_offset_compared ) ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear offsets table.",
				 function );

				return( -1 );
			}
			segment_file_handle->amount_of_chunks -= offset_table->last_chunk_offset_filled - offset_table->last_chunk_offset_compared;
			offset_table->last_chunk_offset_filled = offset_table->last_chunk_offset_compared;
		}
		else
		{
			/* TODO handle ENCASE1/SMART table section */
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported previous section: %s.",
			 function,
			 ( (libewf_section_list_values_t *) section_list_element->previous->value )->type );

			return( -1 );
		}
		reopen_segment_file                         = 1;
		write_io_handle->resume_segment_file_offset = section_list_values->start_offset;
		write_io_handle->create_chunks_section      = 1;
	}
	else if( memory_compare(
	          (void *) section_list_values->type,
	          (void *) "table2",
	          7 ) == 0 )
	{
		/* The sections containing the chunks and offsets were read entirely
		 */
		reopen_segment_file                         = 1;
		write_io_handle->resume_segment_file_offset = section_list_values->end_offset;

		/* Write a new chunks section if necessary
		 */
		if( offset_table->last_chunk_offset_compared < offset_table->amount_of_chunk_offsets )
		{
			write_io_handle->create_chunks_section = 1;
		}
	}
	else if( memory_compare(
	          (void *) section_list_values->type,
	          (void *) "next",
	          5 ) == 0 )
	{
		/* The segment file was read entirely
		 */
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write resume from section: %s not supported.",
		 function,
		 (char *) section_list_values->type );

		return( -1 );
	}
	/* Set offset into media data
	 */
	io_handle->current_chunk        = offset_table->last_chunk_offset_compared;
	io_handle->current_chunk_offset = 0;

	/* Set write io handle values
	 */
	write_io_handle->input_write_count = offset_table->last_chunk_offset_compared * media_values->chunk_size;
	write_io_handle->amount_of_chunks  = offset_table->last_chunk_offset_compared;
	write_io_handle->write_finalized   = 0;

	if( reopen_segment_file != 0 )
	{
		if( write_io_handle->resume_segment_file_offset > (off64_t) segment_table->maximum_segment_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: resume segment file offset cannot be greater than segment file size.",
			 function );

			return( -1 );
		}
		write_io_handle->remaining_segment_file_size = segment_table->maximum_segment_size
		                                             - write_io_handle->resume_segment_file_offset;
		write_io_handle->segment_amount_of_chunks    = segment_file_handle->amount_of_chunks; 

		/* TODO set the following write io handle values to the correct value
		 * currently only required for ENCASE1/SMART
		 */
		write_io_handle->chunks_section_number       = 0;

		if( libbfio_pool_reopen(
		     io_handle->file_io_pool,
		     segment_file_handle->file_io_pool_entry,
		     LIBBFIO_OPEN_READ_WRITE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to reopen segment file: %" PRIu16 ".",
			 function,
			 segment_number );

			return( -1 );
		}
		segment_file_handle->write_open = 1;
	}
	return( 1 );
}

/* Check for empty block, a block that contains the same value for every byte
 * Returns 1 if block is empty, 0 if not or -1 on error
 */
int libewf_write_io_handle_test_empty_block(
     uint8_t *buffer,
     size_t size,
     liberror_error_t **error )
{
	static char *function = "libewf_write_io_handle_test_empty_block";
	size_t iterator       = 0;

	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	for( iterator = 1; iterator < size; iterator++ )
	{
		if( buffer[ 0 ] != buffer[ iterator ] )
		{
			return( 0 );
		}
	}
	return( 1 );
}

/* Calculates an estimate of the amount of chunks that fit within a segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_calculate_chunks_per_segment(
     uint32_t *chunks_per_segment,
     size64_t remaining_segment_file_size,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t segment_amount_of_chunks,
     uint32_t amount_of_chunks,
     libewf_media_values_t *media_values,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_amount,
     liberror_error_t **error )
{
	static char *function                 = "libewf_write_io_handle_calculate_chunks_per_segment";
	int64_t calculated_chunks_per_segment = 0;
	int64_t maximum_chunks_per_segment    = 0;
	int64_t remaining_amount_of_chunks    = 0;
	int64_t required_chunk_sections       = 0;

	if( chunks_per_segment == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunks per segment.",
		 function );

		return( -1 );
	}
	if( maximum_section_amount_of_chunks == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum section amount of chunks value.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
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
		/* Leave space for the chunk, table and table2 section starts and the table and table2 offset table CRCs
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
	*chunks_per_segment = (uint32_t) calculated_chunks_per_segment;

	return( 1 );
}

/* Calculates the amount of chunks that fit within a chunks section
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_calculate_chunks_per_chunks_section(
     uint32_t *chunks_per_chunks_section,
     uint32_t maximum_section_amount_of_chunks,
     uint32_t chunks_per_segment,
     uint8_t chunks_section_number,
     uint8_t unrestrict_offset_amount,
     liberror_error_t **error )
{
	static char *function              = "libewf_write_io_handle_calculate_chunks_per_chunks_section";
	int64_t remaining_amount_of_chunks = 0;

	if( chunks_per_chunks_section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunks per chunks section.",
		 function );

		return( -1 );
	}
	if( maximum_section_amount_of_chunks == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum section amount of chunks value.",
		 function );

		return( -1 );
	}
	if( chunks_section_number == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported section number: %" PRIu8 ".",
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
int libewf_write_io_handle_test_segment_file_full(
     ssize64_t remaining_segment_file_size,
     uint32_t segment_amount_of_chunks,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     uint32_t chunks_per_segment,
     uint32_t current_amount_of_chunks,
     uint8_t format,
     uint8_t ewf_format,
     liberror_error_t **error )
{
	static char *function = "libewf_write_io_handle_test_segment_file_full";

	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	/* Check if the maximum amount of chunks has been reached
	 */
	if( ( media_values->amount_of_chunks != 0 )
	 && ( media_values->amount_of_chunks == current_amount_of_chunks ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
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
		libewf_notify_verbose_printf(
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
			libewf_notify_verbose_printf(
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
		libewf_notify_verbose_printf(
		 "%s: no space left for additional chunk - file size exceeded.\n",
		 function );
#endif

		return( 1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: space left for additional chunk.\n",
	 function );
#endif

	return( 0 );
}

/* Tests if the current chunks section is full
 * Returns 1 if full, 0 if not or -1 on error
 */
int libewf_write_io_handle_test_chunks_section_full(
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
     liberror_error_t **error )
{
	static char *function = "libewf_write_io_handle_test_chunks_section_full";

	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( segment_file_offset > (off64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment file offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( maximum_section_amount_of_chunks == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum section amount of chunks value.",
		 function );

		return( -1 );
	}
	/* Check if a chunks section has been opened
	 */
	if( chunks_section_offset == 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
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
		libewf_notify_verbose_printf(
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
		libewf_notify_verbose_printf(
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
		libewf_notify_verbose_printf(
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
		libewf_notify_verbose_printf(
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
		libewf_notify_verbose_printf(
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
			libewf_notify_verbose_printf(
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
		libewf_notify_verbose_printf(
		 "%s: no space left for additional chunk - file size exceeded.\n",
		 function );
#endif

		return( 1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: space left for additional chunk.\n",
	 function );
#endif

	return( 0 );
}

/* Processes the chunk data, applies compression if necessary and calculates the CRC
 * Returns the amount of bytes of the processed chunk data or -1 on error
 */
ssize_t libewf_write_io_handle_process_chunk(
         libewf_chunk_cache_t *chunk_cache,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t compression_flags,
         uint8_t ewf_format,
         uint8_t *chunk_data,
         size_t chunk_data_size,
         uint8_t *compressed_chunk_data,
         size_t *compressed_chunk_data_size,
         int8_t *is_compressed,
         uint8_t chunk_exists,
         ewf_crc_t *chunk_crc,
         int8_t *write_crc,
         liberror_error_t **error )
{
	uint8_t *chunk_data_crc        = NULL;
	static char *function          = "libewf_write_io_handle_process_chunk";
	size_t data_write_size         = 0;
	int8_t chunk_compression_level = 0;
	int chunk_cache_data_used      = 0;
	int result                     = 0;

	if( chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk cache.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data == chunk_cache->compressed )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data buffer - same as chunk cache compressed.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) media_values->chunk_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
		 "%s: invalid chunk data size: %" PRIzd " value exceeds media values chunk size: %" PRIzd ".",
		 function,
		 chunk_data_size,
		 media_values->chunk_size );

		return( -1 );
	}
	if( chunk_crc == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk CRC.",
		 function );

		return( -1 );
	}
	if( is_compressed == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid is compressed.",
		 function );

		return( -1 );
	}
	if( write_crc == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write CRC.",
		 function );

		return( -1 );
	}
	*write_crc = 0;

	/* Determine the compression level
	 */
	chunk_compression_level = compression_level;

	/* Compress empty blocks if necessary
	 */
	if( ( compression_flags & LIBEWF_FLAG_COMPRESS_EMPTY_BLOCK ) == LIBEWF_FLAG_COMPRESS_EMPTY_BLOCK )
	{
		result = libewf_write_io_handle_test_empty_block(
		          chunk_data,
		          chunk_data_size,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if chunk data is empty.",
			 function );

			return( -1 );
		}
		else if( result == 1 )
		{
			if( chunk_compression_level == EWF_COMPRESSION_NONE )
			{
				chunk_compression_level = EWF_COMPRESSION_DEFAULT;
			}
		}
		else
		{
		 	chunk_compression_level = EWF_COMPRESSION_NONE;
		}
	}
	/* The compressed data size contains the maximum allowed buffer size on entry
	 */
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( chunk_compression_level != EWF_COMPRESSION_NONE ) )
	{
		if( compressed_chunk_data == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid compressed chunk data.",
			 function );

			return( -1 );
		}
		if( chunk_data == compressed_chunk_data )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid compressed chunk data is the same as chunk data.",
			 function );

			return( -1 );
		}
		if( compressed_chunk_data_size == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid compressed chunk data size.",
			 function );

			return( -1 );
		}
		if( *compressed_chunk_data_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid compressed chunk data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		result = libewf_compress(
			  (uint8_t *) compressed_chunk_data,
			  compressed_chunk_data_size,
			  (uint8_t *) chunk_data,
			  chunk_data_size,
			  chunk_compression_level,
		          error );

		/* Check if the compressed buffer was too small
		 * and the chunk cache is used to store the compressed chunk
		 * and a new compressed data size buffer was passed back
		 */
		if( ( result == -1 )
		 && ( compressed_chunk_data == chunk_cache->compressed )
		 && ( *compressed_chunk_data_size > 0 ) )
		{
			liberror_error_free(
			 error );

			chunk_cache_data_used = (int) ( chunk_data == chunk_cache->data );

			if( libewf_chunk_cache_resize(
			     chunk_cache,
			     *compressed_chunk_data_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize chunk cache.",
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
				  chunk_compression_level,
			          error );
		}
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
			 "%s: unable to compress chunk.",
			 function );

			return( -1 );
		}
	}
	if( ( chunk_exists == 0 )
	 && ( ( ewf_format == EWF_FORMAT_S01 )
	  || ( ( *compressed_chunk_data_size > 0 )
	   && ( *compressed_chunk_data_size < media_values->chunk_size ) ) ) )
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set CRC.",
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
ssize_t libewf_write_io_handle_write_new_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *segment_table,
         libewf_values_table_t **header_values,
         libewf_values_table_t *hash_values,
         libewf_header_sections_t *header_sections,
         libewf_hash_sections_t *hash_sections,
         libewf_sector_table_t *sessions,
         libewf_sector_table_t *acquiry_errors,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         uint8_t *crc_buffer,
         ewf_crc_t chunk_crc,
         int8_t write_crc,
         liberror_error_t **error )
{
	void *reallocation          = NULL;
	static char *function       = "libewf_write_io_handle_write_new_chunk";
	off64_t segment_file_offset = 0;
	ssize_t total_write_count   = 0;
	ssize_t write_count         = 0;
	uint16_t segment_number     = 0;
	int result                  = 0;

	if( write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment table - missing segment file handles.",
		 function );

		return( -1 );
	}
	if( header_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header values.",
		 function );

		return( -1 );
	}
	if( ( chunk < offset_table->amount_of_chunk_offsets )
	 && ( offset_table->chunk_offset != NULL )
	 && ( offset_table->chunk_offset[ chunk ].segment_file_handle != NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk: %" PRIu32 " already set.",
		 function,
		 chunk );

		return( -1 );
	}
	/* Check if the write was already finalized
	 */
	if( write_io_handle->write_finalized == 1 )
	{
		return( 0 );
	}
	/* Allocate the necessary amount of chunk offsets
	 * this reduces the amount of reallocations
	 */
	if( offset_table->amount_of_chunk_offsets < media_values->amount_of_chunks )
        {
		if( libewf_offset_table_resize(
		     offset_table,
		     media_values->amount_of_chunks,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize offset table.",
			 function );

			return( -1 );
		}
	}
	if( chunk_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid chunk size value is zero.",
		 function );

		return( -1 );
	}
	if( chunk_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Check if the amount of bytes as specified have been written
	 */
	if( ( media_values->media_size != 0 )
	 && ( write_io_handle->input_write_count >= (ssize64_t) media_values->media_size ) )
	{
		return( 0 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: writing chunk with size: %" PRIzd " (data size: %" PRIzd ").\n",
	 function,
	 chunk_size,
	 chunk_data_size );
#endif

	segment_number = segment_table->amount - 1;

	/* Check if a segment number is valid
	 */
	if( segment_number > segment_table->amount )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid segment number value out of range.",
		 function );

		return( -1 );
	}
	/* Check if a new segment file should be created
	 */
	if( ( segment_number == 0 )
	 || ( segment_table->segment_file_handle[ segment_number ]->write_open == 0 ) )
	{
		/* Create the headers if required
		 */
		if( header_sections == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid header sections.",
			 function );

			return( -1 );
		}
		if( ( header_sections->header == NULL )
		 && ( header_sections->header2 == NULL )
		 && ( header_sections->xheader == NULL ) )
		{
			if( *header_values == NULL )
			{
				if( libewf_values_table_initialize(
				     header_values,
				     LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create header values.",
					 function );

					return( -1 );
				}
				if( libewf_header_values_initialize(
				     *header_values,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to initialize header values.",
					 function );

					return( -1 );
				}
			}
			if( libewf_header_sections_create(
			     header_sections,
			     *header_values,
			     io_handle->compression_level,
			     io_handle->format,
			     error ) == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create header(s).",
				 function );

				return( -1 );
			}
		}
		write_io_handle->create_chunks_section     = 1;
		write_io_handle->chunks_section_number     = 0;
		write_io_handle->chunks_per_chunks_section = 0;
		write_io_handle->segment_amount_of_chunks  = 0;

		segment_number += 1;

#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: creating segment file with segment number: %" PRIu16 ".\n",
		 function,
		 segment_number );
#endif

		/* Create a new segment file
		 */
		if( libewf_segment_table_create_segment_file(
		     segment_table,
		     segment_number,
		     io_handle,
		     write_io_handle->maximum_amount_of_segments,
		     LIBEWF_SEGMENT_FILE_TYPE_EWF,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to create segment file for segment: %" PRIu16 ".",
			 function,
			 segment_number );

			return( -1 );
		}
		write_io_handle->remaining_segment_file_size = segment_table->maximum_segment_size;

		/* Leave space for the done or next section
		 */
		write_io_handle->remaining_segment_file_size -= sizeof( ewf_section_t );

		/* Write the start of the segment file
		 * like the file header, the header, volume and/or data section, etc.
		 */
		write_count = libewf_segment_file_write_start(
		               segment_table->segment_file_handle[ segment_number ],
		               io_handle,
		               segment_number,
		               LIBEWF_SEGMENT_FILE_TYPE_EWF,
		               media_values,
		               header_sections,
		               &( write_io_handle->data_section ),
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write segment file start.",
			 function );

			return( -1 );
		}
		total_write_count                            += write_count;
		write_io_handle->remaining_segment_file_size -= write_count;

		/* Determine the amount of chunks per segment
		 */
		if( segment_table->segment_file_handle[ segment_number ]->amount_of_chunks == 0 )
		{
			if( libewf_write_io_handle_calculate_chunks_per_segment(
			     &( write_io_handle->chunks_per_segment ),
			     write_io_handle->remaining_segment_file_size,
			     write_io_handle->maximum_section_amount_of_chunks,
			     write_io_handle->segment_amount_of_chunks,
			     write_io_handle->amount_of_chunks,
			     media_values,
			     io_handle->format,
			     io_handle->ewf_format,
			     write_io_handle->unrestrict_offset_amount,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine the amount of chunks per segment.",
				 function );

				return( -1 );
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: calculated amount of chunks for segment: %" PRIu32 ".\n",
			 function,
			 write_io_handle->chunks_per_segment );
#endif
		}
		else
		{
			write_io_handle->chunks_per_segment = segment_table->segment_file_handle[ segment_number ]->amount_of_chunks;
		}
	}
	/* Set segment file to the correct offset if write is resumed
	 */
	if( write_io_handle->resume_segment_file_offset > 0 )
	{
		if( libbfio_pool_seek_offset(
		     io_handle->file_io_pool,
		     segment_table->segment_file_handle[ segment_number ]->file_io_pool_entry,
		     write_io_handle->resume_segment_file_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to seek resume segment file offset: %" PRIu64 " in segment file: %" PRIu16 ".",
			 function,
			 write_io_handle->resume_segment_file_offset,
			 segment_number );

			return( -1 );
		}
		write_io_handle->resume_segment_file_offset = 0;
	}
	/* Check if a chunk section should be created
	 */
	if( write_io_handle->create_chunks_section == 1 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: creating chunks section.\n",
		 function );
#endif

		write_io_handle->create_chunks_section      = 0;
		write_io_handle->section_amount_of_chunks   = 0;
		write_io_handle->chunks_section_write_count = 0;

		if( io_handle->ewf_format == EWF_FORMAT_S01 )
		{
			/* Leave space for the chunk section start
			 */
			write_io_handle->remaining_segment_file_size -= sizeof( ewf_section_t );
		}
		else if( io_handle->format == LIBEWF_FORMAT_ENCASE1 )
		{
			/* Leave space for the chunk section start and the offset table CRC
			 */
			write_io_handle->remaining_segment_file_size -= sizeof( ewf_section_t ) + sizeof( ewf_crc_t );
		}
		else
		{
			/* Leave space for the chunk, table and table2 section starts and the table and table2 offset table CRCs
			 */
			write_io_handle->remaining_segment_file_size -= ( 3 * sizeof( ewf_section_t ) ) + ( 2 * sizeof( ewf_crc_t ) );
		}
		if( libbfio_pool_get_offset(
		     io_handle->file_io_pool,
		     segment_table->segment_file_handle[ segment_number ]->file_io_pool_entry,
		     &( write_io_handle->chunks_section_offset ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve current offset in segment file.",
			 function );

			return( -1 );
		}
		/* Start with chunks section number number 1, value is initialized with 0
		 */
		write_io_handle->chunks_section_number += 1;

		/* Recalculate the amount of chunks per segment for a better segment file fill when compression is used
		 */
		if( segment_table->segment_file_handle[ segment_number ]->amount_of_chunks == 0 )
		{
			if( libewf_write_io_handle_calculate_chunks_per_segment(
			     &( write_io_handle->chunks_per_segment ),
			     write_io_handle->remaining_segment_file_size,
			     write_io_handle->maximum_section_amount_of_chunks,
			     write_io_handle->segment_amount_of_chunks,
			     write_io_handle->amount_of_chunks,
			     media_values,
			     io_handle->format,
			     io_handle->ewf_format,
			     write_io_handle->unrestrict_offset_amount,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine the amount of chunks per segment.",
				 function );

				return( -1 );
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: calculated amount of chunks for segment: %" PRIu32 ".\n",
			 function,
			 write_io_handle->chunks_per_segment );
#endif
		}
		else
		{
			write_io_handle->chunks_per_segment = segment_table->segment_file_handle[ segment_number ]->amount_of_chunks;
		}
		if( libewf_write_io_handle_calculate_chunks_per_chunks_section(
		     &( write_io_handle->chunks_per_chunks_section ),
		     write_io_handle->maximum_section_amount_of_chunks,
		     write_io_handle->chunks_per_segment,
		     write_io_handle->chunks_section_number,
		     write_io_handle->unrestrict_offset_amount,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the amount of chunks per chunks section.",
			 function );

			return( -1 );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: calculated amount of chunks for chunks section: %" PRIu32 ".\n",
		 function,
		 write_io_handle->chunks_per_chunks_section );
#endif

		if( write_io_handle->amount_of_table_offsets < write_io_handle->chunks_per_chunks_section )
		{
			reallocation = memory_reallocate(
			                write_io_handle->table_offsets,
			                sizeof( ewf_table_offset_t ) * write_io_handle->chunks_per_chunks_section );

			if( reallocation == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create table offsets.",
				 function );

				return( -1 );
			}
			write_io_handle->table_offsets           = (ewf_table_offset_t *) reallocation;
			write_io_handle->amount_of_table_offsets = write_io_handle->chunks_per_chunks_section;
		}
		/* Write the section start of the chunks section
		 */
		write_count = libewf_segment_file_write_chunks_section_start(
		               segment_table->segment_file_handle[ segment_number ],
		               io_handle,
		               offset_table,
		               write_io_handle->table_offsets,
		               write_io_handle->amount_of_table_offsets,
		               media_values->chunk_size,
		               write_io_handle->amount_of_chunks,
		               write_io_handle->chunks_per_chunks_section,
		               error );

		if( write_count == -1 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: unable to write section start for chunks.\n",
			 function );
#endif

			return( -1 );
		}
		total_write_count                            += write_count;
		write_io_handle->remaining_segment_file_size -= write_count;
	}
	/* Write the chunk data
	 */
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: writing %" PRIzd " bytes to segment file: %" PRIu16 ".\n",
	 function,
	 chunk_size,
	 segment_number );
#endif

	write_count = libewf_segment_file_write_chunk(
		       segment_table->segment_file_handle[ segment_number ],
		       io_handle,
		       offset_table,
		       chunk,
		       chunk_buffer,
		       chunk_size,
		       is_compressed,
		       crc_buffer,
		       &chunk_crc,
		       write_crc,
	               error );

	if( write_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data.",
		 function );

		return( -1 );
	}
	total_write_count                            += write_count;
	write_io_handle->input_write_count           += chunk_data_size;
	write_io_handle->chunks_section_write_count  += write_count;
	write_io_handle->remaining_segment_file_size -= write_count;
	write_io_handle->segment_amount_of_chunks    += 1;
	write_io_handle->section_amount_of_chunks    += 1;
	write_io_handle->amount_of_chunks            += 1;

	if( ( io_handle->ewf_format == EWF_FORMAT_S01 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		/* Leave space for the chunk offset in the offset table
		 */
		write_io_handle->remaining_segment_file_size -= 2 * sizeof( ewf_table_offset_t );
	}
	else
	{
		/* Leave space for the chunk offset in the table and table2 sections
		 */
		write_io_handle->remaining_segment_file_size -= 2 * sizeof( ewf_table_offset_t );
	}
	if( libbfio_pool_get_offset(
	     io_handle->file_io_pool,
	     segment_table->segment_file_handle[ segment_number ]->file_io_pool_entry,
	     &segment_file_offset,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.",
		 function );

		return( -1 );
	}
	/* Check if the current chunks section is full, if so close the current section
	 */
	result = libewf_write_io_handle_test_chunks_section_full(
	          write_io_handle->chunks_section_offset,
	          write_io_handle->remaining_segment_file_size,
	          media_values,
	          write_io_handle->input_write_count,
	          segment_file_offset,
	          write_io_handle->maximum_section_amount_of_chunks,
	          write_io_handle->section_amount_of_chunks,
	          write_io_handle->amount_of_chunks,
	          write_io_handle->chunks_per_chunks_section,
	          io_handle->format,
	          io_handle->ewf_format,
	          write_io_handle->unrestrict_offset_amount,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if chunks section is full.",
		 function );

		return( -1 );
	}
	else if( result == 1 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: closing chunks section amount of data written: %" PRIi64 ".\n",
		 function,
		 write_io_handle->chunks_section_write_count );
#endif
		if( write_io_handle->amount_of_table_offsets < write_io_handle->section_amount_of_chunks )
		{
			reallocation = memory_reallocate(
			                write_io_handle->table_offsets,
			                sizeof( ewf_table_offset_t ) * write_io_handle->section_amount_of_chunks );

			if( reallocation == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create table offsets.",
				 function );

				return( -1 );
			}
			write_io_handle->table_offsets           = (ewf_table_offset_t *) reallocation;
			write_io_handle->amount_of_table_offsets = write_io_handle->section_amount_of_chunks;
		}

		/* Correct the offset, size in the chunks section
		 */
		write_count = libewf_segment_file_write_chunks_section_correction(
		               segment_table->segment_file_handle[ segment_number ],
		               io_handle,
		               offset_table,
		               write_io_handle->table_offsets,
		               write_io_handle->amount_of_table_offsets,
		               write_io_handle->chunks_section_offset,
		               (size64_t) write_io_handle->chunks_section_write_count,
		               write_io_handle->amount_of_chunks,
		               write_io_handle->section_amount_of_chunks,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to correct chunks section.",
			 function );

			return( -1 );
		}
		total_write_count                      += write_count;
		write_io_handle->create_chunks_section  = 1;
		write_io_handle->chunks_section_offset  = 0;

		/* Check if the current segment file is full, if so close the current segment file
		 */
		result = libewf_write_io_handle_test_segment_file_full(
			  write_io_handle->remaining_segment_file_size,
			  write_io_handle->segment_amount_of_chunks,
			  media_values,
			  write_io_handle->input_write_count,
			  write_io_handle->chunks_per_segment,
			  write_io_handle->amount_of_chunks,
			  io_handle->format,
			  io_handle->ewf_format,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if segment file is full.",
			 function );

			return( -1 );
		}
		else if( result == 1 )
		{
			/* Check if this is not the last segment file
			 */
			if( ( media_values->media_size == 0 )
			 || ( write_io_handle->input_write_count < (ssize64_t) media_values->media_size ) )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				libewf_notify_verbose_printf(
				 "%s: closing segment file with segment number: %" PRIu16 ".\n",
				 function,
				 segment_number );
#endif

				/* Finish and close the segment file
				 */
				write_count = libewf_segment_file_write_close(
					       segment_table->segment_file_handle[ segment_number ],
					       io_handle,
					       segment_number,
					       write_io_handle->segment_amount_of_chunks,
					       0,
					       hash_sections,
					       hash_values,
					       media_values,
					       sessions,
					       acquiry_errors,
					       &( write_io_handle->data_section ),
				               error );

				if( write_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to close segment file.",
					 function );

					return( -1 );
				}
				total_write_count += write_count;
			}
		}
	}
	return( total_write_count );
}

/* Writes an existing chunk of data in EWF format from a buffer at the current offset
 * The necessary settings of the write values must have been made
 * Returns the amount of data bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_io_handle_write_existing_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *delta_segment_table,
         libewf_header_sections_t *header_sections,
         uint32_t chunk,
         uint8_t *chunk_buffer,
         size_t chunk_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         uint8_t *crc_buffer,
         ewf_crc_t chunk_crc,
         int8_t write_crc,
         liberror_error_t **error )
{
	libewf_list_element_t *last_list_element          = NULL;
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_write_io_handle_write_existing_chunk";
	off64_t last_section_start_offset                 = 0;
	off64_t segment_file_offset                       = 0;
	ssize_t total_write_count                         = 0;
	ssize_t write_count                               = 0;
	uint16_t segment_number                           = 0;
	uint8_t segment_file_type                         = 0;
	uint8_t no_section_append                         = 0;
	int result                                        = 0;

	if( write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( offset_table->chunk_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offset table - missing chunk offsets.",
		 function );

		return( -1 );
	}
	if( delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid delta segment table.",
		 function );

		return( -1 );
	}
	if( delta_segment_table->segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid delta segment table - missing segment file handles.",
		 function );

		return( -1 );
	}
	if( chunk >= offset_table->amount_of_chunk_offsets )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid chunk value out of range.",
		 function );

		return( -1 );
	}
	if( chunk_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid chunk size value is zero.",
		 function );

		return( -1 );
	}
	if( chunk_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_size != media_values->chunk_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_CONFLICTING_VALUE,
		 "%s: invalid chunk size: %" PRIzd " expected size: %" PRIzd ".",
		 function,
		 chunk_size,
		 media_values->chunk_size );

		return( -1 );
	}
	if( is_compressed != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_CONFLICTING_VALUE,
		 "%s: chunk compression cannot be used.",
		 function );

		return( -1 );
	}
	segment_file_handle = offset_table->chunk_offset[ chunk ].segment_file_handle;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file handle.",
		 function );

		return( -1 );
	}
	segment_file_type = segment_file_handle->file_type;

#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: writing delta chunk: %" PRIu32 " with size: %" PRIzd " (data size: %" PRIzd ").\n",
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
		segment_number = delta_segment_table->amount - 1;

		/* Check if a segment number is valid
		 */
		if( segment_number > delta_segment_table->amount )
		{
			liberror_error_set(
			 error,
		 	 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid segment number value out of range.",
			 function );

			return( -1 );
		}
		/* Check if a new delta segment file should be created
		 */
		if( segment_number != 0 )
		{
			segment_file_handle = delta_segment_table->segment_file_handle[ segment_number ];

			if( segment_file_handle == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid segment file.",
				 function );

				return( -1 );
			}
			if( segment_file_handle->section_list == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid segment file - missing section list.",
				 function );

				return( -1 );
			}
			last_list_element = segment_file_handle->section_list->last;

			if( last_list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing last section list element.",
				 function );

				return( -1 );
			}
			if( last_list_element->value == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid last section list element element - missing values.",
				 function );

				return( -1 );
			}
			last_section_start_offset = ( (libewf_section_list_values_t * ) last_list_element->value )->start_offset;

			if( libbfio_pool_get_offset(
			     io_handle->file_io_pool,
			     segment_file_handle->file_io_pool_entry,
			     &segment_file_offset,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve current offset in segment file.",
				 function );

				return( -1 );
			}
			/* Make sure the current segment file offset points to the start of the last section
			 */
			if( ( segment_file_offset != last_section_start_offset )
			 && ( libbfio_pool_seek_offset(
			       io_handle->file_io_pool,
			       segment_file_handle->file_io_pool_entry,
			       last_section_start_offset,
			       SEEK_SET,
			       error ) == -1 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_SEEK_FAILED,
				 "%s: cannot find offset: %" PRIjd ".",
				 function,
				 last_section_start_offset );

				return( -1 );
			}
			segment_file_offset = last_section_start_offset
			                    + chunk_size
			                    + sizeof( ewf_crc_t )
			                    + sizeof( ewf_section_t );

			/* Check if chunk fits in exisiting delta segment file
			 */
			if( segment_file_offset > (off64_t) delta_segment_table->maximum_segment_size )
			{
				/* Make sure to write a next section in the the previous delta segment file
				 * The segment file offset is updated by the function
				 */
				write_count = libewf_segment_file_write_last_section(
					       segment_file_handle,
					       io_handle,
					       0,
				               error );

				if( write_count == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write last section.",
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
				     last_list_element,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_REMOVE_FAILED,
					 "%s: unable to remove last section from list.",
					 function );

					return( -1 );
				}
				if( libewf_section_list_values_free(
				     last_list_element->value ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free last section values.",
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
			     delta_segment_table,
			     segment_number,
			     io_handle,
			     write_io_handle->maximum_amount_of_segments,
			     LIBEWF_SEGMENT_FILE_TYPE_DWF,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to create delta segment file for segment: %" PRIu16 ".",
				 function,
				 segment_number );

				return( -1 );
			}
			segment_file_handle = delta_segment_table->segment_file_handle[ segment_number ];

			/* Write the start of the segment file
			 * like the file header, the header, volume and/or data section, etc.
			 */
			write_count = libewf_segment_file_write_start(
				       segment_file_handle,
				       io_handle,
				       segment_number,
				       LIBEWF_SEGMENT_FILE_TYPE_DWF,
				       media_values,
				       header_sections,
			               &( write_io_handle->data_section ),
			               error );

			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write segment file start.",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
	}
	else
	{
		segment_file_offset = offset_table->chunk_offset[ chunk ].file_offset
		                    - sizeof( ewfx_delta_chunk_header_t )
		                    - sizeof( ewf_section_t );

		if( libbfio_pool_seek_offset(
		     io_handle->file_io_pool,
		     segment_file_handle->file_io_pool_entry,
		     segment_file_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek chunk offset.",
			 function );

			return( -1 );
		}
		no_section_append = 1;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libbfio_pool_get_offset(
	     io_handle->file_io_pool,
	     segment_file_handle->file_io_pool_entry,
	     &segment_file_offset,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.",
		 function );

		return( -1 );
	}
	libewf_notify_verbose_printf(
	 "%s: writing chunk at offset: %" PRIjd ".\n",
	 function,
	 segment_file_offset );
#endif

	/* Write the chunk in the delta segment file
	 */
	write_count = libewf_segment_file_write_delta_chunk(
		       segment_file_handle,
		       io_handle,
		       offset_table,
		       chunk,
		       chunk_buffer,
		       chunk_size,
		       crc_buffer,
		       &chunk_crc,
	               write_crc,
	               no_section_append,
	               error );

	if( write_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write delta chunk.",
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
			       io_handle,
			       1,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write last section.",
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
ssize_t libewf_write_io_handle_write_new_chunk_data(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *segment_table,
         libewf_values_table_t **header_values,
         libewf_values_table_t *hash_values,
         libewf_header_sections_t *header_sections,
         libewf_hash_sections_t *hash_sections,
         libewf_sector_table_t *sessions,
         libewf_sector_table_t *acquiry_errors,
         libewf_chunk_cache_t *chunk_cache,
         uint32_t chunk,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         int8_t force_write,
         liberror_error_t **error )
{
	uint8_t stored_crc_buffer[ 4 ];

	uint8_t *chunk_buffer             = NULL;
	uint8_t *crc_buffer               = NULL;
	static char *function             = "libewf_write_io_handle_write_new_chunk_data";
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

	if( write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk cache.",
		 function );

		return( -1 );
	}
	/* Check if the write was already finalized
	 */
	if( write_io_handle->write_finalized == 1 )
	{
		return( 0 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer == chunk_cache->compressed )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache compressed.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: writing buffer of size: %" PRIzu " with data of size: %" PRIzd ".\n",
	 function,
	 buffer_size,
	 data_size );
#endif

	/* Determine the size of data to read
	 */
	if( data_size < (size_t) media_values->chunk_size )
	{
		read_size = (size_t) data_size;
	}
	else
	{
		read_size = media_values->chunk_size;
	}
	if( read_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid read size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Directy write the buffer if
	 *  the chunk cache data is directly being passed (for finalize)
	 *  or no data was previously copied into the chunk cache
	 *   and the buffer contains the necessary amount of bytes to fill a chunk
	 */
	if( ( buffer == chunk_cache->data )
	 || ( ( chunk_cache->offset == 0 )
	 && ( data_size >= (size_t) media_values->chunk_size ) ) )
	{
		chunk_buffer = (uint8_t *) buffer;
		write_size   = read_size;
	}
	else
	{
		/* Check if data is present in the chunk cache
		 * and calculate the amount of data to read from the buffer
		 */
		remaining_chunk_size = media_values->chunk_size
		                     - chunk_cache->offset;

		if( read_size > (size_t) remaining_chunk_size )
		{
			read_size = remaining_chunk_size;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: reading %" PRIzu " bytes from buffer.\n",
		 function,
		 read_size );
#endif

		if( memory_copy(
		     &( chunk_cache->data[ chunk_cache->offset ] ),
		     buffer,
		     read_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to write data to chunk cache.",
			 function );

			return( -1 );
		}
		chunk_cache->chunk  = chunk;
		chunk_cache->amount = chunk_cache->offset + read_size;

		/* Adjust the chunk cache offset
		 */
		if( chunk_cache->amount == media_values->chunk_size )
		{
			chunk_cache->offset = 0;
		}
		else if( chunk_cache->amount < media_values->chunk_size )
		{
			chunk_cache->offset = chunk_cache->amount;
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid chunk cache amount value out of range.",
			 function );

			return( -1 );
		}
		chunk_buffer = chunk_cache->data;
		write_size   = chunk_cache->amount;
	}
	/* Use chunk and crc buffer alignment when the chunk cache data is directly being passed
	 */
	if( chunk_buffer == chunk_cache->data )
	{
		crc_buffer = &( chunk_buffer[ media_values->chunk_size ] );
	}
	else
	{
		crc_buffer = stored_crc_buffer;
	}
	if( ( write_size == media_values->chunk_size )
	 || ( ( media_values->media_size != 0 )
	  && ( ( write_io_handle->input_write_count + (ssize64_t) write_size ) == (ssize64_t) media_values->media_size ) )
	 || ( force_write != 0 ) )
	{
		chunk_cache_data_used = (int) ( chunk_buffer == chunk_cache->data );

		/* The compressed data size contains the maximum allowed buffer size
		 */
		compressed_chunk_data_size = chunk_cache->allocated_size;

		/* Compress the chunk if necessary and determine its CRC
		 */
		chunk_data_size = libewf_write_io_handle_process_chunk(
				   chunk_cache,
				   media_values,
				   io_handle->compression_level,
				   io_handle->compression_flags,
				   io_handle->ewf_format,
				   chunk_buffer,
				   write_size,
				   chunk_cache->compressed,
				   &compressed_chunk_data_size,
				   &is_compressed,
				   0,
				   &chunk_crc,
				   &write_crc,
		                   error );

		if( chunk_data_size <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_OUTPUT_FAILED,
			 "%s: unable to process chunk data.",
			 function );

			return( -1 );
		}
		/* Make sure to update the chunk_buffer pointer if
		 * chunk_cache->data has been reallocated by
		 * libewf_write_io_handle_process_chunk()
		 */
		if( ( chunk_cache_data_used == 1 )
		 && ( chunk_buffer != chunk_cache->data ) )
		{
			chunk_buffer = chunk_cache->data;
		}
		if( is_compressed != 0 )
		{
			chunk_buffer = chunk_cache->compressed;
		}
		write_count = libewf_write_io_handle_write_new_chunk(
		               write_io_handle,
		               io_handle,
		               media_values,
		               offset_table,
		               segment_table,
		               header_values,
		               hash_values,
		               header_sections,
		               hash_sections,
		               sessions,
		               acquiry_errors,
		               chunk,
		               chunk_buffer,
		               chunk_data_size,
		               write_size,
		               is_compressed,
		               crc_buffer,
		               chunk_crc,
		               write_crc,
		               error );

		if( write_count <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write chunk.",
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
ssize_t libewf_write_io_handle_write_existing_chunk_data(
         libewf_write_io_handle_t *write_io_handle,
         libewf_read_io_handle_t *read_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *delta_segment_table,
         libewf_header_sections_t *header_sections,
         libewf_chunk_cache_t *chunk_cache,
         uint32_t chunk,
         uint32_t chunk_offset,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         liberror_error_t **error )
{
	uint8_t stored_crc_buffer[ 4 ];

	uint8_t *chunk_buffer       = NULL;
	uint8_t *crc_buffer         = NULL;
	static char *function       = "libewf_write_io_handle_write_existing_chunk_data";
	ewf_crc_t chunk_crc         = 0;
	size_t remaining_chunk_size = 0;
	ssize_t read_count          = 0;
	ssize_t write_count         = 0;
	size_t write_size           = 0;

	if( write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk cache.",
		 function );

		return( -1 );
	}
	if( chunk >= offset_table->amount_of_chunk_offsets )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid chunk value out of range.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( ( buffer == chunk_cache->data )
	 || ( buffer == chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: writing buffer of size: %" PRIzu " with data of size: %" PRIzd ".\n",
	 function,
	 buffer_size,
	 data_size );
#endif

	/* Check if the data size exceeds the chunk size
	 */
	if( data_size > (size_t) media_values->chunk_size )
	{
		data_size = (size_t) media_values->chunk_size;
	}
	/* Check if the data in the buffer aligns with a chunk
	 */
	if( ( chunk_offset == 0 )
	 && ( data_size == media_values->chunk_size ) )
	{
		chunk_buffer = buffer;
		write_size   = (size_t) media_values->chunk_size;
	}
	else
	{
		/* Read the chunk data into the chunk cache
		 */
		read_count = libewf_read_io_handle_read_chunk_data(
		              read_io_handle,
		              io_handle,
		              media_values,
		              offset_table,
		              chunk_cache,
		              chunk,
		              0,
		              chunk_cache->data,
		              chunk_cache->allocated_size,
		              error );

		if( read_count <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data from chunk.",
			 function );

			return( -1 );
		}
		io_handle->current_chunk_offset = chunk_offset;

		remaining_chunk_size = media_values->chunk_size
		                     - io_handle->current_chunk_offset;

		if( data_size > (size_t) remaining_chunk_size )
		{
			data_size = remaining_chunk_size;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: updating data in chunk: %" PRIu32 " at offset: %" PRIu32 " with size: %" PRIzd ".\n",
		 function,
		 ( chunk + 1 ),
		 chunk_offset,
		 data_size );
#endif

		/* Update the chunk data
		 */
		if( memory_copy(
		     &( chunk_cache->data[ chunk_offset ] ),
		     buffer,
		     data_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to update data in chunk: %" PRIu32 ".",
			 function,
			 (chunk + 1 ) );

			return( -1 );
		}
		chunk_buffer = chunk_cache->data;
		write_size   = (size_t) read_count;
	}
	/* Use chunk and crc buffer alignment when the chunk cache data is directly being passed
	 */
	if( chunk_buffer == chunk_cache->data )
	{
		crc_buffer = &( chunk_buffer[ media_values->chunk_size ] );
	}
	else
	{
		crc_buffer = stored_crc_buffer;
	}
	/* Calculate the new CRC
         */
        chunk_crc = ewf_crc_calculate(
	             chunk_buffer,
	             write_size,
	             1 );

	write_count = libewf_write_io_handle_write_existing_chunk(
	               write_io_handle,
	               io_handle,
	               media_values,
	               offset_table,
	               delta_segment_table,
	               header_sections,
	               chunk,
	               chunk_buffer,
	               write_size,
	               write_size,
	               0,
	               crc_buffer,
	               chunk_crc,
	               1,
	               error );

	if( write_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write delta chunk.",
		 function );

		return( -1 );
	}
	/* Report the amount of data written
	 */
	return( (ssize_t) data_size );
}

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after write from stream
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_write_io_handle_finalize(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libewf_media_values_t *media_values,
         libewf_offset_table_t *offset_table,
         libewf_segment_table_t *segment_table,
         libewf_values_table_t **header_values,
         libewf_values_table_t *hash_values,
         libewf_header_sections_t *header_sections,
         libewf_hash_sections_t *hash_sections,
         libewf_sector_table_t *sessions,
         libewf_sector_table_t *acquiry_errors,
         libewf_chunk_cache_t *chunk_cache,
         liberror_error_t **error )
{
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	void *reallocation                                = NULL;
	static char *function                             = "libewf_write_io_handle_finalize";
	ssize_t write_count                               = 0;
	ssize_t write_finalize_count                      = 0;
	uint16_t segment_number                           = 0;

	if( write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write io handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment table - missing segment file handles",
		 function );

		return( -1 );
	}
	if( chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk cache.",
		 function );

		return( -1 );
	}
	/* No need for finalization in R or RW mode
	 */
	if( ( ( io_handle->flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	 && ( ( io_handle->flags & LIBEWF_FLAG_RESUME ) == 0 ) )
	{
		return( 0 );
	}
	if( write_io_handle->write_finalized == 1 )
	{
		return( 0 );
	}
	/* Write data remaining in the chunk cache to file
	 */
	if( ( io_handle->current_chunk_offset != 0 )
	 && ( chunk_cache->amount != 0 )
	 && ( chunk_cache->offset != 0 )
	 && ( ( media_values->media_size == 0 )
	  || ( write_io_handle->input_write_count < (ssize64_t) media_values->media_size ) ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: writing chunk remainder at offset: %" PRIu32 " with size: %" PRIu32 "\n",
		 function,
		 io_handle->current_chunk_offset,
		 chunk_cache->amount );
#endif

		write_count = libewf_write_io_handle_write_new_chunk_data(
		               write_io_handle,
		               io_handle,
		               media_values,
		               offset_table,
		               segment_table,
		               header_values,
		               hash_values,
		               header_sections,
		               hash_sections,
		               sessions,
		               acquiry_errors,
		               chunk_cache,
		               io_handle->current_chunk,
		               chunk_cache->data,
		               chunk_cache->amount,
		               chunk_cache->amount,
		               1,
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write remaining chunk data.",
			 function );

			return( -1 );
		}
		write_finalize_count += write_count;
	}
	/* Check if all the media data has been written
	 */
	if( ( media_values->media_size != 0 )
	 && ( write_io_handle->input_write_count < (ssize64_t) media_values->media_size ) )
	{
		return( write_finalize_count );
	}
	/* Check last segment file
	 */
	segment_number = segment_table->amount - 1;

	/* No segment file was created
	 */
	if( segment_number == 0 )
	{
		return( write_finalize_count );
	}
	segment_file_handle = segment_table->segment_file_handle[ segment_number ];

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file: %" PRIu16 ".",
		 function,
		 segment_number );

		return( -1 );
	}
	/* Set segment file to the correct offset if write is resumed
	 */
	if( write_io_handle->resume_segment_file_offset > 0 )
	{
		if( libbfio_pool_seek_offset(
		     io_handle->file_io_pool,
		     segment_file_handle->file_io_pool_entry,
		     write_io_handle->resume_segment_file_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to seek resume segment file offset: %" PRIu64 " in segment file: %" PRIu16 ".",
			 function,
			 write_io_handle->resume_segment_file_offset,
			 segment_number );

			return( -1 );
		}
		write_io_handle->resume_segment_file_offset = 0;
	}
	/* Check if the last segment file is still open for writing
	 */
	if( segment_file_handle->write_open != 0 )
	{
		/* Check if chunks section needs to be corrected
		 */
		if( write_io_handle->chunks_section_offset != 0 )
		{
			/* Correct the offset, size in the chunks section
			 */
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: correcting chunks section.\n",
			 function );
#endif

			if( write_io_handle->amount_of_table_offsets < write_io_handle->section_amount_of_chunks )
			{
				reallocation = memory_reallocate(
				                write_io_handle->table_offsets,
				                sizeof( ewf_table_offset_t ) * write_io_handle->section_amount_of_chunks );

				if( reallocation == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create table offsets.",
					 function );

					return( -1 );
				}
				write_io_handle->table_offsets           = (ewf_table_offset_t *) reallocation;
				write_io_handle->amount_of_table_offsets = write_io_handle->section_amount_of_chunks;
			}
			write_count = libewf_segment_file_write_chunks_section_correction(
				       segment_file_handle,
				       io_handle,
				       offset_table,
			               write_io_handle->table_offsets,
			               write_io_handle->amount_of_table_offsets,
				       write_io_handle->chunks_section_offset,
				       (size64_t) write_io_handle->chunks_section_write_count,
				       write_io_handle->amount_of_chunks,
				       write_io_handle->section_amount_of_chunks,
			               error );

			if( write_count == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to correct chunks section.",
				 function );

				return( -1 );
			}
			write_finalize_count += write_count;
		}
		/* Close the segment file
		 */
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: closing last segment file.\n",
		 function );
#endif

		write_count = libewf_segment_file_write_close(
		               segment_file_handle,
		               io_handle,
		               segment_number,
		               write_io_handle->segment_amount_of_chunks,
		               1,
		               hash_sections,
		               hash_values,
		               media_values,
		               sessions,
		               acquiry_errors,
		               &( write_io_handle->data_section ),
		               error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to close segment file.",
			 function );

			return( -1 );
		}
		write_finalize_count += write_count;
	}
	/* Correct the media values if streamed write was used
	 */
	if( media_values->media_size == 0 )
	{
		/* Determine the media values
		 */
		media_values->amount_of_chunks  = write_io_handle->amount_of_chunks;
		media_values->amount_of_sectors = (uint64_t) ( write_io_handle->input_write_count / media_values->bytes_per_sector );
		media_values->media_size        = (size64_t) write_io_handle->input_write_count;

		/* Flush the data section write cache
		 */
		memory_free(
		 write_io_handle->data_section );

		write_io_handle->data_section = NULL;

		/* Correct the sections in the segment files
		 */
		if( libewf_segment_table_write_sections_corrections(
		     segment_table,
		     io_handle,
		     write_io_handle->segment_amount_of_chunks,
		     media_values,
		     hash_values,
		     hash_sections,
		     sessions,
		     acquiry_errors,
		     &( write_io_handle->data_section ),
		     error ) != 1 )

		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sections corrections to segment files.",
			 function );

			return( -1 );
		}
	}
	write_io_handle->write_finalized = 1;

	return( write_finalize_count );
}

