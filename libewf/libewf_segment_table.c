/*
 * libewf segment table
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
#include "libewf_notify.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_string.h"

/* Allocates memory for a segment table struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_SEGMENT_TABLE *libewf_segment_table_alloc( uint16_t amount )
{
	LIBEWF_SEGMENT_TABLE *segment_table = NULL;
	static char *function               = "libewf_segment_table_alloc";

	segment_table = (LIBEWF_SEGMENT_TABLE *) libewf_common_alloc( LIBEWF_SEGMENT_TABLE_SIZE );

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate segment table.\n",
		 function );

		return( NULL );
	}
	segment_table->segment_file = (LIBEWF_SEGMENT_FILE **) libewf_common_alloc(
	                               ( amount * sizeof( LIBEWF_SEGMENT_FILE* ) ) );

	if( segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate segment file array.\n",
		 function );

		libewf_common_free( segment_table );

		return( NULL );
	}
	if( libewf_common_memset(
	     segment_table->segment_file,
	     0, 
	     ( amount * sizeof( LIBEWF_SEGMENT_FILE* ) ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear segment file array.\n",
		 function );

		libewf_common_free( segment_table->segment_file );
		libewf_common_free( segment_table );

		return( NULL );
	} 
	segment_table->amount = amount;

	return( segment_table );
}

/* Reallocates memory for the segment table values
 * Returns 1 if successful, or -1 on error
 */
int libewf_segment_table_realloc( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t amount )
{
	void *reallocation    = NULL;
	static char *function = "libewf_segment_table_realloc";

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( segment_table->amount >= amount )
	{
		LIBEWF_WARNING_PRINT( "%s: new amount must be greater than previous amount.\n",
		 function );

		return( -1 );
	}
	reallocation = libewf_common_realloc(
	                segment_table->segment_file,
	                ( amount * sizeof( LIBEWF_SEGMENT_FILE* ) ) );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic segment file array.\n",
		 function );

		return( -1 );
	}
	segment_table->segment_file = (LIBEWF_SEGMENT_FILE **) reallocation;

	if( libewf_common_memset(
	     &( segment_table->segment_file[ segment_table->amount ] ),
	     0, 
	     ( ( amount - segment_table->amount ) * sizeof( LIBEWF_SEGMENT_FILE* ) ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear segment file array.\n",
		 function );

		return( 1 );
	} 
	segment_table->amount = amount;

	return( 1 );
}

/* Frees memory of a file list struct including elements
 */
void libewf_segment_table_free( LIBEWF_SEGMENT_TABLE *segment_table )
{
	LIBEWF_SECTION_LIST_ENTRY *section_list_entry         = NULL;
	LIBEWF_SECTION_LIST_ENTRY *current_section_list_entry = NULL;
	static char *function                                 = "libewf_segment_table_free";
	uint16_t iterator                                     = 0;

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return;
	}
	for( iterator = 0; iterator < segment_table->amount; iterator++ )
	{
		if( segment_table->segment_file[ iterator ] != NULL )
		{
			if( segment_table->segment_file[ iterator ]->filename != NULL )
			{
				libewf_common_free( segment_table->segment_file[ iterator ]->filename );
			}
			if( segment_table->segment_file[ iterator ]->section_list != NULL )
			{
				section_list_entry = segment_table->segment_file[ iterator ]->section_list->first;

				while( section_list_entry != NULL )
				{
					current_section_list_entry = section_list_entry;
					section_list_entry         = section_list_entry->next;

					libewf_common_free( current_section_list_entry );
				}
				libewf_common_free( segment_table->segment_file[ iterator ]->section_list );
			}
			libewf_common_free( segment_table->segment_file[ iterator ] );
		}
	}
	libewf_common_free( segment_table->segment_file );
	libewf_common_free( segment_table );
}

/* Initializes the segment table
 * Opens EWF segment file(s) for writing
 * Returns 1 if successful, 0 if not, or -1 on error
 */
int libewf_segment_table_write_open( LIBEWF_SEGMENT_TABLE *segment_table, LIBEWF_FILENAME * const filenames[], uint16_t file_amount )
{
	LIBEWF_SEGMENT_FILE *segment_file = NULL;
	static char *function             = "libewf_segment_table_write_open";
	size_t filename_length            = 0;

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filenames.\n",
		 function );

		return( -1 );
	}
	if( file_amount < 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file amount at least 1 is required.\n",
		 function );

		return( -1 );
	}
	filename_length = libewf_filename_length( filenames[ 0 ] );

	if( filename_length == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: filename is empty.\n",
		 function );

		return( -1 );
	}
	/* Set segment table basename
	 */
	segment_file = libewf_segment_file_alloc();

	if( segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create segment file.\n",
		 function );

		return( -1 );
	}
	segment_table->segment_file[ 0 ] = segment_file;

	if( libewf_segment_file_set_filename(
	     segment_table->segment_file[ 0 ],
	     filenames[ 0 ],
	     filename_length ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename in segment table.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes all the EWF segment file(s) in the segment table
 * Returns 1 if successful, 0 if not, or -1 on error
 */
int libewf_segment_table_close_all( LIBEWF_SEGMENT_TABLE *segment_table )
{
	static char *function = "libewf_segment_table_close_all";
	uint16_t iterator     = 0;
	int result            = 1;

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table - missing segment files.\n",
		 function );

		return( -1 );
	}
	for( iterator = 1; iterator < segment_table->amount; iterator++ )
	{
		if( segment_table->segment_file[ iterator ]->file_descriptor > 0 )
		{
			if( libewf_common_close( segment_table->segment_file[ iterator ]->file_descriptor ) != 0 )
			{
				if( segment_table->segment_file[ iterator ]->filename == NULL )
				{
					LIBEWF_WARNING_PRINT( "%s: unable to close segment file: %" PRIu16 ".\n",
					 function, iterator );
				}
				else
				{
					LIBEWF_WARNING_PRINT( "%s: unable to close segment file: %" PRIu16 " (%" PRIs_EWF_filename ").\n",
					 function, iterator, segment_table->segment_file[ iterator ]->filename );
				}
				result = 0;
			}
		}
	}
	return( result );
}

