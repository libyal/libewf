/*
 * libewf offset table
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
#include "libewf_endian.h"
#include "libewf_notify.h"
#include "libewf_offset_table.h"

#include "ewf_definitions.h"

/* Allocates memory for a new offset table struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_OFFSET_TABLE *libewf_offset_table_alloc( uint32_t amount )
{
	LIBEWF_OFFSET_TABLE *offset_table = NULL;
	static char *function             = "libewf_offset_table_alloc";

	offset_table = (LIBEWF_OFFSET_TABLE *) libewf_common_alloc( LIBEWF_OFFSET_TABLE_SIZE );

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate offset table.\n",
		 function );

		return( NULL );
	}
	offset_table->file_descriptor = (int *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_FILE_DESCRIPTOR_SIZE ), -1 );

	if( offset_table->file_descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file descriptors.\n",
		 function );

		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->compressed = (uint8_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_COMPRESSED_SIZE ), -1 );

	if( offset_table->compressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate compressed flags.\n",
		 function );

		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->offset = (off64_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_OFFSET_SIZE ), 0 );

	if( offset_table->offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate offsets.\n",
		 function );

		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table->compressed );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->size = (size_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_SIZE_SIZE ), 0 );

	if( offset_table->size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate sizes.\n",
		 function );

		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table->compressed );
		libewf_common_free( offset_table->offset );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->segment_number = (uint16_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_SEGMENT_NUMBER_SIZE ), 0 );

	if( offset_table->segment_number == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate segment numbers.\n",
		 function );

		libewf_common_free( offset_table->size );
		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table->compressed );
		libewf_common_free( offset_table->offset );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->dirty = (uint8_t *) libewf_common_alloc_cleared( ( amount * LIBEWF_OFFSET_TABLE_DIRTY_SIZE ), 0 );

	if( offset_table->dirty == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate dirty flags.\n",
		 function );

		libewf_common_free( offset_table->size );
		libewf_common_free( offset_table->file_descriptor );
		libewf_common_free( offset_table->compressed );
		libewf_common_free( offset_table->offset );
		libewf_common_free( offset_table->segment_number );
		libewf_common_free( offset_table );

		return( NULL );
	}
	offset_table->amount = amount;
        offset_table->last   = 0;

	return( offset_table );
}

/* Reallocates memory for the dynamic file descriptor, offset and size array
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_realloc( LIBEWF_OFFSET_TABLE *offset_table, uint32_t amount )
{
	void *reallocation    = NULL;
	static char *function = "libewf_offset_table_realloc";

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset_table.\n",
		 function );

		return( -1 );
	}
	reallocation = libewf_common_realloc_new_cleared(
	                offset_table->file_descriptor,
	                ( offset_table->amount * LIBEWF_OFFSET_TABLE_FILE_DESCRIPTOR_SIZE ),
	                ( amount * LIBEWF_OFFSET_TABLE_FILE_DESCRIPTOR_SIZE ),
	                -1 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate file descriptors.\n",
		 function );

		return( -1 );
	}
	offset_table->file_descriptor = (int *) reallocation;
	reallocation                  = libewf_common_realloc_new_cleared(
	                                 offset_table->compressed,
	                                 ( offset_table->amount * LIBEWF_OFFSET_TABLE_COMPRESSED_SIZE ),
	                                 ( amount * LIBEWF_OFFSET_TABLE_COMPRESSED_SIZE ),
	                                 -1 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate compressed flags.\n",
		 function );

		return( -1 );
	}
	offset_table->compressed = (uint8_t *) reallocation;
	reallocation             = libewf_common_realloc_new_cleared(
	                            offset_table->offset,
	                            ( offset_table->amount * LIBEWF_OFFSET_TABLE_OFFSET_SIZE ),
	                            ( amount * LIBEWF_OFFSET_TABLE_OFFSET_SIZE ),
	                            0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate offsets.\n",
		 function );

		return( -1 );
	}
	offset_table->offset = (off64_t *) reallocation;
	reallocation         = libewf_common_realloc_new_cleared(
	                        offset_table->size,
	                        ( offset_table->amount * LIBEWF_OFFSET_TABLE_SIZE_SIZE ),
	                        ( amount * LIBEWF_OFFSET_TABLE_SIZE_SIZE ),
	                        0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate sizes.\n",
		 function );

		return( -1 );
	}
	offset_table->size = (size_t *) reallocation;
	reallocation       = libewf_common_realloc_new_cleared(
	                      offset_table->segment_number,
	                      ( offset_table->amount * LIBEWF_OFFSET_TABLE_SEGMENT_NUMBER_SIZE ),
	                      ( amount * LIBEWF_OFFSET_TABLE_SEGMENT_NUMBER_SIZE ),
	                      0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate segment numbers.\n",
		 function );

		return( -1 );
	}
	offset_table->segment_number = (uint16_t *) reallocation;
	reallocation                 = libewf_common_realloc_new_cleared(
	                                offset_table->dirty,
	                                ( offset_table->amount * LIBEWF_OFFSET_TABLE_DIRTY_SIZE ),
	                                ( amount * LIBEWF_OFFSET_TABLE_DIRTY_SIZE ),
	                                0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dirty flags.\n",
		 function );

		return( -1 );
	}
	offset_table->dirty  = (uint8_t *) reallocation;
	offset_table->amount = amount;

	return( 1 );
}

/* Frees memory of a offset table struct including elements
 */
void libewf_offset_table_free( LIBEWF_OFFSET_TABLE *offset_table )
{
	static char *function = "libewf_offset_table_free";

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return;
	}
	libewf_common_free( offset_table->file_descriptor );
	libewf_common_free( offset_table->compressed );
	libewf_common_free( offset_table->offset );
	libewf_common_free( offset_table->size );
	libewf_common_free( offset_table->segment_number );
	libewf_common_free( offset_table->dirty );
	libewf_common_free( offset_table );
}

/* Sets the values for a specific offset
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_set_values( LIBEWF_OFFSET_TABLE *offset_table, uint32_t chunk, int file_descriptor, uint8_t compressed, off64_t offset, size_t size, uint16_t segment_number, uint8_t dirty )
{
	static char *function = "libewf_offset_table_set_values";

	if( offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( offset_table->file_descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing file descriptors.\n",
		 function );

		return( -1 );
	}
	if( offset_table->compressed == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing compressed values.\n",
		 function );

		return( -1 );
	}
	if( offset_table->offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing offsets.\n",
		 function );

		return( -1 );
	}
	if( offset_table->size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing sizes.\n",
		 function );

		return( -1 );
	}
	if( offset_table->segment_number == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing segment numbers.\n",
		 function );

		return( -1 );
	}
	if( offset_table->dirty == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table - missing dirty values.\n",
		 function );

		return( -1 );
	}
	if( chunk > offset_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: chunk: %" PRIu32 " not in offset table.\n",
		 function, chunk );

		return( -1 );
	}
	offset_table->file_descriptor[ chunk ] = file_descriptor;
	offset_table->compressed[ chunk ]      = compressed;
	offset_table->offset[ chunk ]          = offset;
	offset_table->size[ chunk ]            = size;
	offset_table->segment_number[ chunk ]  = segment_number;
	offset_table->dirty[ chunk ]           = dirty;

	return( 1 );
}

/* Fills the offset table
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_fill( LIBEWF_OFFSET_TABLE *offset_table, off64_t base_offset, EWF_TABLE_OFFSET *offsets, uint32_t amount_of_chunks, int file_descriptor, uint16_t segment_number, uint8_t error_tollerance )
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
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid base offset.\n",
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
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate offset table.\n",
			 function );

			return( -1 );
		}
	}
	/* Read the offsets from file
	 */
	if( libewf_endian_convert_32bit( &raw_offset, offsets[ iterator ].offset ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert raw offset value.\n",
		 function );

		return( -1 );
	}
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
		if( libewf_endian_convert_32bit( &raw_offset, offsets[ iterator + 1 ].offset ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to convert raw offset value.\n",
			 function );

			return( -1 );
		}
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
				LIBEWF_WARNING_PRINT( "%s: chunk offset %" PRIu32 " larger than raw %" PRIu32 ".\n",
				 function, current_offset, raw_offset );

				return( -1 );
			}
			LIBEWF_VERBOSE_PRINT( "%s: chunk offset %" PRIu32 " larger than next %" PRIu32 ".\n",
			 function, current_offset, next_offset );

			chunk_size = raw_offset - current_offset;
		}
		else
		{
			chunk_size = next_offset - current_offset;
		}

		if( chunk_size == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid chunk size - size is zero.\n",
			 function );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
		if( chunk_size > (uint32_t) INT32_MAX )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid chunk size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		if( libewf_offset_table_set_values(
		     offset_table,
		     offset_table->last,
		     file_descriptor,
		     compressed,
		     (off64_t) ( base_offset + current_offset ),
		     (size_t) chunk_size,
		     segment_number,
		     0 ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set value in offset table.\n",
			 function );

			if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
			{
				return( -1 );
			}
		}
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
		LIBEWF_VERBOSE_PRINT( "%s: %s chunk %" PRIu32 " read with: base %" PRIu64 ", offset %" PRIu32 " and size %" PRIu32 ".\n",
		 function, chunk_type, offset_table->last, base_offset, current_offset, chunk_size );
#endif
		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_size ) > (uint32_t) INT32_MAX ) )
		{
			LIBEWF_VERBOSE_PRINT( "%s: chunk offset overflow at: %" PRIu32 ".\n",
			 function, current_offset );

			overflow = 1;
		}
		iterator++;
	}
	if( libewf_endian_convert_32bit( &raw_offset, offsets[ iterator ].offset ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to convert raw offset value.\n",
		 function );

		return( -1 );
	}
	if( overflow == 0 )
	{
		compressed     = (uint8_t) ( raw_offset >> 31 );
		current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	}
	else
	{
		current_offset = raw_offset;
	}

	if( libewf_offset_table_set_values(
	     offset_table,
	     offset_table->last,
	     file_descriptor,
	     compressed,
	     (off64_t) ( base_offset + current_offset ),
	     0,
	     segment_number,
	     0 ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set value in offset table.\n",
		 function );

		if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
		{
			return( -1 );
		}
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( compressed == 0 )
	{
		chunk_type = "uncompressed";
	}
	else
	{
		chunk_type = "compressed";
	}
	LIBEWF_VERBOSE_PRINT( "%s: %s last chunk %" PRIu32 " read with: base %" PRIu64 " and offset %" PRIu32 ".\n",
	 function, chunk_type, ( offset_table->last + 1 ), base_offset, current_offset );
#endif

	return( 1 );
}

/* Calculate the last offset
 * Returns 1 if successful, or -1 on error
 */
int libewf_offset_table_calculate_last_offset( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, int file_descriptor, uint16_t segment_number, uint8_t error_tollerance )
{
	LIBEWF_SECTION_LIST_ENTRY *section_list_entry = NULL;
	static char *function                         = "libewf_offset_table_calculate_last_offset";
	off64_t last_offset                           = 0;
	off64_t chunk_size                            = 0;

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
	/*
	 * There is no indication how large the last chunk is. The only thing known is where it starts.
	 * However it can be determined where the next section starts within the file.
	 * The size of the last chunk is determined by subtracting the last offset from the offset of the section that follows.
	 */
	section_list_entry = section_list->first;
	last_offset        = offset_table->offset[ offset_table->last ];

	while( section_list_entry != NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		LIBEWF_VERBOSE_PRINT( "%s: start offset: %" PRIi64 " last offset: %" PRIi64 " \n",
		 function, section_list_entry->start_offset, last_offset );
#endif

		if( ( section_list_entry->start_offset < last_offset )
		 && ( last_offset < section_list_entry->end_offset ) )
		{
			chunk_size = section_list_entry->end_offset - last_offset;

			if( last_offset > (off64_t) INT64_MAX )
			{
				LIBEWF_WARNING_PRINT( "%s: invalid last chunk offset value exceeds maximum.\n",
				 function );

				return( -1 );
			}
			if( chunk_size == 0 )
			{
				LIBEWF_WARNING_PRINT( "%s: invalid chunk size - size is zero.\n",
				 function );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
			if( chunk_size > (off64_t) INT32_MAX )
			{
				LIBEWF_WARNING_PRINT( "%s: invalid chunk size value exceeds maximum.\n",
				 function );

				return( -1 );
			}
			if( libewf_offset_table_set_values(
			     offset_table,
			     offset_table->last,
			     file_descriptor,
			     offset_table->compressed[ offset_table->last ],
			     last_offset,
			     (size_t) chunk_size,
			     segment_number,
			     0 ) == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to set value in offset table.\n",
				 function );

				if( error_tollerance < LIBEWF_ERROR_TOLLERANCE_COMPENSATE )
				{
					return( -1 );
				}
			}
			LIBEWF_VERBOSE_PRINT( "%s: last chunk %" PRIu32 " calculated with offset: %" PRIu64 " and size %zu.\n",
			 function, ( offset_table->last + 1 ), last_offset, (size_t) chunk_size );

			break;
		}
		section_list_entry = section_list_entry->next;
	}
	return( 1 );
}

/* Compare the offsets in tablel and table2 sections
 * Returns 1 if tables match, 0 if table differ, or -1 on error
 */
int libewf_offset_table_compare( LIBEWF_OFFSET_TABLE *offset_table1, LIBEWF_OFFSET_TABLE *offset_table2 )
{
	static char *function = "libewf_offset_table_compare";
	uint64_t iterator     = 0;

	if( ( offset_table1 == NULL )
	 || ( offset_table2 == NULL ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset table.\n",
		 function );

		return( -1 );
	}
	/* Check if table and table2 are the same
	 */
	if( offset_table1->amount != offset_table2->amount )
	{
		LIBEWF_VERBOSE_PRINT( "%s: offset tables differ in size.\n",
		 function );

		return( 0 );
	}
	else
	{
		for( iterator = 0; iterator < offset_table1->amount; iterator++ )
		{
			if( offset_table1->offset[ iterator ] != offset_table2->offset[ iterator ] )
			{
				LIBEWF_VERBOSE_PRINT( "%s: offset tables differ in offset for chunk: %" PRIu64 " (table1: %" PRIu64 ", table2: %" PRIu64 ").\n",
				 function, iterator, offset_table1->offset[ iterator ], offset_table2->offset[ iterator ] );

				return( 0 );
			}
		}
	}
	return( 1 );
}

