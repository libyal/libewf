/*
 * libewf segment table
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

#include <stdlib.h>

#include "libewf_common.h"
#include "libewf_definitions.h"
#include "libewf_notify.h"
#include "libewf_segment_table.h"

/* Allocates memory for a segment table struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_SEGMENT_TABLE *libewf_segment_table_alloc( uint32_t size )
{
	LIBEWF_SEGMENT_TABLE *segment_table = NULL;
	uint32_t iterator                   = 0;

	segment_table = (LIBEWF_SEGMENT_TABLE *) libewf_alloc( LIBEWF_SEGMENT_TABLE_SIZE );

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_alloc: unable to allocate segment table.\n" );

		return( NULL );
	}
	segment_table->filename = (char **) libewf_alloc_cleared( ( size * LIBEWF_SEGMENT_TABLE_FILENAME_SIZE ), (intptr_t) NULL );

	if( segment_table->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_alloc: unable to allocate filename array.\n" );

		libewf_free( segment_table );

		return( NULL );
	}
	segment_table->file_descriptor = (int *) libewf_alloc_cleared( ( size * LIBEWF_SEGMENT_TABLE_FILE_DESCRIPTOR_SIZE ), -1 );

	if( segment_table->file_descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_alloc: unable to allocate file descriptor array.\n" );

		libewf_free( segment_table->filename );
		libewf_free( segment_table );

		return( NULL );
	}
	segment_table->section_list = (LIBEWF_SECTION_LIST **) libewf_alloc_cleared( ( size * LIBEWF_SEGMENT_TABLE_SECTION_LIST_SIZE ), (intptr_t) NULL );

	if( segment_table->section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_alloc: unable to allocate section list array.\n" );

		libewf_free( segment_table->filename );
		libewf_free( segment_table->file_descriptor );
		libewf_free( segment_table );

		return( NULL );
	}
	for( iterator = 0; iterator < size; iterator++ )
	{
		if( segment_table->section_list[ iterator ] == NULL )
		{
			segment_table->section_list[ iterator ] = libewf_section_list_alloc();

			if( segment_table->section_list[ iterator ] == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_segment_table_alloc: unable to allocate section list.\n" );

				/* The current entry does not need to be freed, because it was never allocated
				 * but the first entry 0 does, because the iterator is a unsigned integer
				 * the iterator - 1 construction is used
				 */
				for( ; iterator > 0; iterator-- )
				{
					libewf_section_list_free( segment_table->section_list[ iterator - 1 ] );
				}
				libewf_free( segment_table->filename );
				libewf_free( segment_table->file_descriptor );
				libewf_free( segment_table->section_list );
				libewf_free( segment_table );

				return( NULL );
			}
		}
		else
		{
			LIBEWF_WARNING_PRINT( "libewf_segment_table_alloc: section list already created.\n" );
		}
	}
	segment_table->amount = size;

	return( segment_table );
}

/* Reallocates memory for the segment table 
 * Returns a pointer to the instance, NULL on error
 */
LIBEWF_SEGMENT_TABLE *libewf_segment_table_realloc( LIBEWF_SEGMENT_TABLE *segment_table, uint32_t size )
{
	void *reallocation = NULL;
	uint32_t iterator  = 0;

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_realloc: invalid segment_table.\n" );

		return( NULL );
	}
	reallocation = libewf_realloc_new_cleared( segment_table->filename, segment_table->amount, ( size * LIBEWF_SEGMENT_TABLE_FILENAME_SIZE ), (intptr_t) NULL );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_realloc: unable to allocate dynamic filename array.\n" );

		return( NULL );
	}
	segment_table->filename = (char **) reallocation;
	reallocation            = libewf_realloc_new_cleared( segment_table->file_descriptor, segment_table->amount, ( size * LIBEWF_SEGMENT_TABLE_FILE_DESCRIPTOR_SIZE ), -1 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_realloc: unable to allocate dynamic file descriptor array.\n" );

		return( NULL );
	}
	segment_table->file_descriptor = (int *) reallocation;
	reallocation                   = libewf_realloc_new_cleared( segment_table->section_list, segment_table->amount, ( size * LIBEWF_SEGMENT_TABLE_SECTION_LIST_SIZE ), (intptr_t) NULL );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_realloc: unable to allocate dynamic section list array.\n" );

		return( NULL );
	}
	segment_table->section_list = (LIBEWF_SECTION_LIST **) reallocation;

	for( iterator = segment_table->amount; iterator < size; iterator++ )
	{
		if( segment_table->section_list[ iterator ] == NULL )
		{
			segment_table->section_list[ iterator ] = libewf_section_list_alloc();

			if( segment_table->section_list[ iterator ] == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_segment_table_realloc: unable to allocate section list.\n" );

				return( NULL );
			}
		}
		else
		{
			LIBEWF_WARNING_PRINT( "libewf_segment_table_realloc: section list already created.\n" );
		}
	}
	segment_table->amount = size;

	return( segment_table );
}

/* Frees memory of a file list struct including elements
 */
void libewf_segment_table_free( LIBEWF_SEGMENT_TABLE *segment_table )
{
	uint32_t iterator = 0;

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_free: invalid segment_table.\n" );

		return;
	}
	for( iterator = 0; iterator < segment_table->amount; iterator++ )
	{
		if( segment_table->filename[ iterator ] != NULL )
		{
			libewf_free( segment_table->filename[ iterator ] );
		}
		if( segment_table->section_list[ iterator ] != NULL )
		{
			libewf_section_list_free( segment_table->section_list[ iterator ] );
		}
	}
	libewf_free( segment_table->filename );
	libewf_free( segment_table->file_descriptor );
	libewf_free( segment_table->section_list );
	libewf_free( segment_table );
}

/* Sets the values for a specific segment
 * Returns a pointer to the instance, NULL on error
 */
LIBEWF_SEGMENT_TABLE *libewf_segment_table_set_values( LIBEWF_SEGMENT_TABLE *segment_table, uint32_t segment, const char *filename, int file_descriptor )
{
	void *data_set         = NULL;
	uint32_t filename_size = 0;

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_set_values: invalid segment_table.\n" );

		return( NULL );
	}
	if( segment_table->filename[ segment ] != NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_set_values: duplicate segments not supported: segment %d in %s was already specified in %s.\n", segment, filename, segment_table->filename[ segment ] );

		return( NULL );
	}
	/* Check if additional entries should be allocated
	 */
	if( segment > segment_table->amount )
	{
		LIBEWF_VERBOSE_PRINT( "libewf_segment_table_set_values: allocating additional segment_table entries.\n" );

		/* Segment has an offset of 1 so an additional values entry is needed
		 */
		segment_table = libewf_segment_table_realloc( segment_table, ( segment + 1 ) );

		if( segment_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_segment_table_set_values: unable to reallocate segment table.\n" );

			return( NULL );
		}
	}
	filename_size = libewf_strlen( filename );

	if( filename_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_set_values: unable to process empty filename.\n" );

		return( NULL );
	}
	/* One additional byte for the end of string character is needed
	 */
	segment_table->filename[ segment ] = (char *) libewf_alloc( sizeof( char ) * ( filename_size + 1 ) );

	if( segment_table->filename[ segment ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_set_values: unable to create filename.\n" );

		return( NULL );
	}
	data_set = libewf_memcpy( segment_table->filename[ segment ], filename, filename_size );

	if( data_set == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_set_values: unable to set filename.\n" );

		libewf_free( segment_table->filename[ segment ] );

		segment_table->filename[ segment ] = NULL;

		return( NULL );
	}
	/* Make sure the string is terminated
	 */
	segment_table->filename[ segment ][ filename_size ] = '\0';

	segment_table->file_descriptor[ segment ] = file_descriptor;

	return( segment_table );
}

/* Checks if a segment table entry is set
 * Returns 0 when entry is not set and 1 if set
 */
uint8_t libewf_segment_table_values_is_set( LIBEWF_SEGMENT_TABLE *segment_table, uint32_t segment )
{
	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_values_is_set: invalid segment_table.\n" );

		return( 0 );
	}
	if( segment > segment_table->amount )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_values_is_set: segment out of range.\n" );

		return( 0 );
	}
	return( segment_table->filename[ segment ] != NULL );
}

/* Gets the filename of a certain segment
 * Returns the filename pointer, or NULL on error
 */
char *libewf_segment_table_get_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint32_t segment )
{
	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_get_filename: invalid segment_table.\n" );

		return( NULL );
	}
	if( segment > segment_table->amount )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_get_filename: segment out of range.\n" );

		return( NULL );
	}
	return( segment_table->filename[ segment ] );
}

/* Gets the file descriptor of a certain segment
 * Returns the file descriptor, or -1 on error
 */
int libewf_segment_table_get_file_descriptor( LIBEWF_SEGMENT_TABLE *segment_table, uint32_t segment )
{
	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_get_file_descriptor: invalid segment_table.\n" );

		return( -1 );
	}
	if( segment > segment_table->amount )
	{
		LIBEWF_WARNING_PRINT( "libewf_segment_table_get_file_descriptor: segment out of range.\n" );

		return( -1 );
	}
	return( segment_table->file_descriptor[ segment ] );
}

