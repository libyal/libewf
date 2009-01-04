/*
 * libewf offset table
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <common.h>
#include <endian.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#include <libewf/definitions.h>

#include "libewf_offset_table.h"

#include "ewf_definitions.h"

/* Allocates memory for a new offset table struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_offset_table_t *libewf_offset_table_alloc(
                        uint32_t amount )
{
	libewf_offset_table_t *offset_table = NULL;
	static char *function               = "libewf_offset_table_alloc";

	offset_table = (libewf_offset_table_t *) memory_allocate(
	                                          sizeof( libewf_offset_table_t ) );

	if( offset_table == NULL )
	{
		notify_warning_printf( "%s: unable to allocate offset table.\n",
		 function );

		return( NULL );
	}
	offset_table->chunk_offset = NULL;

	if( amount > 0 )
	{
		offset_table->chunk_offset = (libewf_chunk_offset_t *) memory_allocate(
		                                                        sizeof( libewf_chunk_offset_t ) * amount );

		if( offset_table->chunk_offset == NULL )
		{
			notify_warning_printf( "%s: unable to allocate chunk offsets.\n",
			 function );

			memory_free(
			 offset_table );

			return( NULL );
		}
		if( memory_set(
		     offset_table->chunk_offset,
		     0,
		     ( sizeof( libewf_chunk_offset_t ) * amount ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear chunk offsets.\n",
			 function );

			memory_free(
			 offset_table->chunk_offset );
			memory_free(
			 offset_table );

			return( NULL );
		}
	}
	offset_table->amount = amount;
        offset_table->last   = 0;

	return( offset_table );
}

/* Reallocates memory for the dynamic file descriptor, offset and size array
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_realloc(
     libewf_offset_table_t *offset_table,
     uint32_t amount )
{
	void *reallocation    = NULL;
	static char *function = "libewf_offset_table_realloc";

	if( offset_table == NULL )
	{
		notify_warning_printf( "%s: invalid offset_table.\n",
		 function );

		return( -1 );
	}
	if( offset_table->amount >= amount )
	{
		notify_warning_printf( "%s: new amount must be greater than previous amount.\n",
		 function );

		return( -1 );
	}
	reallocation = memory_reallocate(
	                offset_table->chunk_offset,
	                ( sizeof( libewf_chunk_offset_t ) * amount ) );

	if( reallocation == NULL )
	{
		notify_warning_printf( "%s: unable to reallocate chunk offsets.\n",
		 function );

		return( -1 );
	}
	offset_table->chunk_offset = (libewf_chunk_offset_t *) reallocation;

	if( memory_set(
	     &( offset_table->chunk_offset[ offset_table->amount ] ),
	     0,
	     ( sizeof( libewf_chunk_offset_t ) * ( amount - offset_table->amount ) ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear chunk offsets.\n",
		 function );

		return( -1 );
	}
	offset_table->amount = amount;

	return( 1 );
}

/* Frees memory of a offset table struct including elements
 */
void libewf_offset_table_free(
      libewf_offset_table_t *offset_table )
{
	static char *function = "libewf_offset_table_free";

	if( offset_table == NULL )
	{
		notify_warning_printf( "%s: invalid offset table.\n",
		 function );

		return;
	}
	/* The segment file reference is freed in the segment table
	 */
	if( offset_table->chunk_offset != NULL )
	{
		memory_free(
		 offset_table->chunk_offset );
	}
	memory_free(
	 offset_table );
}

/* Fills the offset table from the table offsets
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_fill(
     libewf_offset_table_t *offset_table,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t amount_of_chunks,
     libewf_segment_file_handle_t *segment_file_handle,
     uint8_t error_tollerance )
{
#if defined( HAVE_VERBOSE_OUTPUT )
	char *chunk_type        = NULL;
#endif
	static char *function   = "libewf_offset_table_fill";
	uint32_t chunk_size     = 0;
	uint32_t current_offset = 0;
	uint32_t next_offset    = 0;
	uint32_t raw_offset     = 0;
	uint32_t iterator       = 0;
	uint8_t compressed      = 0;
	uint8_t overflow        = 0;

	if( offset_table == NULL )
	{
		notify_warning_printf( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		notify_warning_printf( "%s: invalid base offset.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file.\n",
		 function );

		return( -1 );
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
		if( libewf_offset_table_realloc(
		     offset_table,
		     ( offset_table->last + amount_of_chunks ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to reallocate offset table.\n",
			 function );

			return( -1 );
		}
	}
	endian_little_convert_32bit(
	 raw_offset,
	 offsets[ iterator ].offset );

	/* The size of the last chunk must be determined differently
	 */
	while( iterator < ( amount_of_chunks - 1 ) )
	{
		if( overflow == 0 )
		{
			compressed     = (uint8_t) ( raw_offset >> 31 );
			current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			current_offset = raw_offset;
		}
		endian_little_convert_32bit(
		 raw_offset,
		 offsets[ iterator + 1 ].offset );

		if( overflow == 0 )
		{
			next_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			next_offset = raw_offset;
		}

		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6
		 */
		if( next_offset < current_offset )
		{
			if( raw_offset < current_offset )
			{
				notify_warning_printf( "%s: chunk offset %" PRIu32 " larger than raw %" PRIu32 ".\n",
				 function, current_offset, raw_offset );

				return( -1 );
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf( "%s: chunk offset %" PRIu32 " larger than next %" PRIu32 ".\n",
			 function, current_offset, next_offset );
#endif

			chunk_size = raw_offset - current_offset;
		}
		else
		{
			chunk_size = next_offset - current_offset;
		}

		if( chunk_size == 0 )
		{
			notify_warning_printf( "%s: invalid chunk size - size is zero.\n",
			 function );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
		if( chunk_size > (uint32_t) INT32_MAX )
		{
			notify_warning_printf( "%s: invalid chunk size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		offset_table->chunk_offset[ offset_table->last ].segment_file_handle = segment_file_handle;
		offset_table->chunk_offset[ offset_table->last ].file_offset         = (off64_t) ( base_offset + current_offset );
		offset_table->chunk_offset[ offset_table->last ].size                = (size_t) chunk_size;
		offset_table->chunk_offset[ offset_table->last ].compressed          = compressed;

		offset_table->last++;

#if defined( HAVE_VERBOSE_OUTPUT )
		if( compressed == 0 )
		{
			chunk_type = "uncompressed";
		}
		else
		{
			chunk_type = "compressed";
		}
		notify_verbose_printf( "%s: %s chunk %" PRIu32 " read with: base %" PRIu64 ", offset %" PRIu32 " and size %" PRIu32 ".\n",
		 function, chunk_type, offset_table->last, base_offset, current_offset, chunk_size );
#endif
		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf( "%s: chunk offset overflow at: %" PRIu32 ".\n",
			 function, current_offset );
#endif

			overflow   = 1;
			compressed = 0;
		}
		iterator++;
	}
	endian_little_convert_32bit(
	 raw_offset,
	 offsets[ iterator ].offset );

	if( overflow == 0 )
	{
		compressed     = (uint8_t) ( raw_offset >> 31 );
		current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	}
	else
	{
		current_offset = raw_offset;
	}
	offset_table->chunk_offset[ offset_table->last ].segment_file_handle = segment_file_handle;
	offset_table->chunk_offset[ offset_table->last ].file_offset         = (off64_t) ( base_offset + current_offset );
	offset_table->chunk_offset[ offset_table->last ].compressed          = compressed;

#if defined( HAVE_VERBOSE_OUTPUT )
	if( compressed == 0 )
	{
		chunk_type = "uncompressed";
	}
	else
	{
		chunk_type = "compressed";
	}
	notify_verbose_printf( "%s: %s last chunk %" PRIu32 " read with: base %" PRIu64 " and offset %" PRIu32 ".\n",
	 function, chunk_type, ( offset_table->last + 1 ), base_offset, current_offset );
#endif

	return( 1 );
}

/* Fills the offsets from the offset table
 * amount_of_chunk_offsets contains the amount of chunk offsets to fill
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_fill_offsets(
     libewf_offset_table_t *offset_table,
     uint32_t offset_table_index,
     uint32_t amount_of_chunk_offsets,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t amount_of_offsets )
{
	static char *function   = "libewf_offset_table_fill_offsets";
	off64_t offset64_value  = 0;
	uint32_t offset32_value = 0;
	uint32_t iterator       = 0;

	if( offset_table == NULL )
	{
		notify_warning_printf( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( offset_table->chunk_offset == NULL )
	{
		notify_warning_printf( "%s: invalid offset table - missing chunk offsets.\n",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		notify_warning_printf( "%s: invalid base offset.\n",
		 function );

		return( -1 );
	}
	if( offsets == NULL )
	{
		notify_warning_printf( "%s: invalid table offsets.\n",
		 function );

		return( -1 );
	}
	if( amount_of_offsets < amount_of_chunk_offsets )
	{
		notify_warning_printf( "%s: offsets too small.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_chunk_offsets; iterator++ )
	{
		offset64_value = offset_table->chunk_offset[ offset_table_index + iterator ].file_offset
		               - base_offset;

		if( ( offset64_value < 0 )
		 || ( offset64_value > (off64_t) INT32_MAX ) )
		{
			notify_warning_printf( "%s: invalid chunk offset value.\n",
			 function );

			return( -1 );
		}
		offset32_value = (uint32_t) offset64_value;

		if( offset_table->chunk_offset[ offset_table_index + iterator ].compressed != 0 )
		{
			offset32_value |= EWF_OFFSET_COMPRESSED_WRITE_MASK;
		}
		endian_little_revert_32bit(
		 offsets[ iterator ].offset,
		 offset32_value );
	}
	return( 1 );
}

/* Calculate the last offset
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_calculate_last_offset(
     libewf_offset_table_t *offset_table,
     libewf_section_list_t *section_list,
     uint8_t error_tollerance )
{
	libewf_section_list_entry_t *section_list_entry = NULL;
	static char *function                           = "libewf_offset_table_calculate_last_offset";
	off64_t last_offset                             = 0;
	off64_t chunk_size                              = 0;

	if( offset_table == NULL )
	{
		notify_warning_printf( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( offset_table->chunk_offset == NULL )
	{
		notify_warning_printf( "%s: invalid offset table - missing chunk offsets.\n",
		 function );

		return( -1 );
	}
	if( section_list == NULL )
	{
		notify_warning_printf( "%s: invalid section list.\n",
		 function );

		return( -1 );
	}
	/*
	 * There is no indication how large the last chunk is. The only thing known is where it starts.
	 * However it can be determined where the next section starts within the file.
	 * The size of the last chunk is determined by subtracting the last offset from the offset of the section that follows.
	 */
	section_list_entry = section_list->first;
	last_offset        = offset_table->chunk_offset[ offset_table->last ].file_offset;

	while( section_list_entry != NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		notify_verbose_printf( "%s: start offset: %" PRIi64 " last offset: %" PRIi64 " \n",
		 function, section_list_entry->start_offset, last_offset );
#endif

		if( ( section_list_entry->start_offset < last_offset )
		 && ( last_offset < section_list_entry->end_offset ) )
		{
			chunk_size = section_list_entry->end_offset - last_offset;

			if( last_offset > (off64_t) INT64_MAX )
			{
				notify_warning_printf( "%s: invalid last chunk offset value exceeds maximum.\n",
				 function );

				return( -1 );
			}
			if( chunk_size == 0 )
			{
				notify_warning_printf( "%s: invalid chunk size - size is zero.\n",
				 function );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
			if( chunk_size > (off64_t) INT32_MAX )
			{
				notify_warning_printf( "%s: invalid chunk size value exceeds maximum.\n",
				 function );

				return( -1 );
			}
			offset_table->chunk_offset[ offset_table->last ].size = (size_t) chunk_size;

#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf( "%s: last chunk %" PRIu32 " calculated with offset: %" PRIu64 " and size %" PRIzu ".\n",
			 function, ( offset_table->last + 1 ), last_offset, (size_t) chunk_size );
#endif
			break;
		}
		section_list_entry = section_list_entry->next;
	}
	return( 1 );
}

/* Compare the offsets in tablel and table2 sections
 * Returns 1 if tables match, 0 if table differ, or -1 on error
 */
int libewf_offset_table_compare(
     libewf_offset_table_t *offset_table1,
     libewf_offset_table_t *offset_table2 )
{
	static char *function = "libewf_offset_table_compare";
	uint64_t iterator     = 0;

	if( offset_table1 == NULL )
	{
		notify_warning_printf( "%s: invalid offset table1.\n",
		 function );

		return( -1 );
	}
	if( offset_table2 == NULL )
	{
		notify_warning_printf( "%s: invalid offset table2.\n",
		 function );

		return( -1 );
	}
	if( offset_table1->chunk_offset == NULL )
	{
		notify_warning_printf( "%s: invalid offset table1 - missing chunk offsets.\n",
		 function );

		return( -1 );
	}
	if( offset_table2->chunk_offset == NULL )
	{
		notify_warning_printf( "%s: invalid offset table2 - missing chunk offsets.\n",
		 function );

		return( -1 );
	}
	/* Check if table and table2 have the same amount of chunk offsets
	 */
	if( offset_table1->amount != offset_table2->amount )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf( "%s: offset tables differ in size.\n",
		 function );
#endif

		return( 0 );
	}
	/* Check if the file offsets of the chunk offsets are the same
	 */
	for( iterator = 0; iterator < offset_table1->amount; iterator++ )
	{
		if( offset_table1->chunk_offset[ iterator ].file_offset != offset_table2->chunk_offset[ iterator ].file_offset )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf( "%s: offset tables differ in offset for chunk: %" PRIu64 " (table1: %" PRIu64 ", table2: %" PRIu64 ").\n",
			 function, iterator, offset_table1->chunk_offset[ iterator ].file_offset, offset_table2->chunk_offset[ iterator ].file_offset );
#endif

			return( 0 );
		}
	}
	return( 1 );
}

/* Seeks a certain chunk offset within the offset table
 * Returns the chunk segment file offset if the seek is successful, or -1 on error
 */
off64_t libewf_offset_table_seek_chunk_offset(
         libewf_offset_table_t *offset_table,
          uint32_t chunk )
{
	static char *function = "libewf_segment_table_seek_chunk_offset";

	if( offset_table == NULL )
	{
		notify_warning_printf( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( offset_table->chunk_offset == NULL )
	{
		notify_warning_printf( "%s: invalid offset table - missing chunk offsets.\n",
		 function );

		return( -1 );
	}
	if( chunk >= offset_table->amount )
	{
		notify_warning_printf( "%s: chunk: %" PRIu32 " not in offset table.\n",
		 function, chunk );

		return( -1 );
	}
	if( libewf_segment_file_handle_seek_offset(
	     offset_table->chunk_offset[ chunk ].segment_file_handle,
	     offset_table->chunk_offset[ chunk ].file_offset ) == -1 )
	{
		notify_warning_printf( "%s: cannot find chunk offset: %" PRIjd ".\n",
		 function, offset_table->chunk_offset[ chunk ].file_offset );

		return( -1 );
	}
	return( offset_table->chunk_offset[ chunk ].file_offset );
}

