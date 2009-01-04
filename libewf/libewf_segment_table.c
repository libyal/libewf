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
#include "libewf_segment_table.h"
#include "libewf_string.h"

/* Allocates memory for a segment table struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_SEGMENT_TABLE *libewf_segment_table_alloc( uint16_t amount )
{
	LIBEWF_SEGMENT_TABLE *segment_table = NULL;
	static char *function               = "libewf_segment_table_alloc";
	uint16_t iterator                   = 0;

	segment_table = (LIBEWF_SEGMENT_TABLE *) libewf_common_alloc( LIBEWF_SEGMENT_TABLE_SIZE );

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate segment table.\n",
		 function );

		return( NULL );
	}
	segment_table->segment_file = (LIBEWF_SEGMENT_FILE *) libewf_common_alloc(
	                               ( amount * LIBEWF_SEGMENT_FILE_SIZE ) );

	if( segment_table->segment_file == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate segment file array.\n",
		 function );

		libewf_common_free( segment_table );

		return( NULL );
	}
	for( iterator = 0; iterator < amount; iterator++ )
	{
		segment_table->segment_file[ iterator ].filename         = NULL;
		segment_table->segment_file[ iterator ].file_descriptor  = -1;
		segment_table->segment_file[ iterator ].file_offset      = 0;
		segment_table->segment_file[ iterator ].amount_of_chunks = 0;

		segment_table->segment_file[ iterator ].section_list = (LIBEWF_SECTION_LIST *) libewf_common_alloc( LIBEWF_SECTION_LIST_SIZE );

		if( segment_table->segment_file[ iterator ].section_list == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocate section list.\n",
			 function );

			/* The current entry does not need to be freed, because it was never allocated
			 * but the first entry 0 does, because the iterator is a unsigned integer
			 * the iterator - 1 construction is used
			 * There are no entries in the list
			 */
			for( ; iterator > 0; iterator-- )
			{
				libewf_common_free( segment_table->segment_file[ iterator - 1 ].section_list );
			}
			libewf_common_free( segment_table->segment_file );
			libewf_common_free( segment_table );

			return( NULL );
		}
		segment_table->segment_file[ iterator ].section_list->first = NULL;
		segment_table->segment_file[ iterator ].section_list->last  = NULL;
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
	uint16_t iterator     = 0;

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
	                ( amount * LIBEWF_SEGMENT_FILE_SIZE ) );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic segment file array.\n",
		 function );

		return( -1 );
	}
	segment_table->segment_file = (LIBEWF_SEGMENT_FILE *) reallocation;

	for( iterator = segment_table->amount; iterator < amount; iterator++ )
	{
		segment_table->segment_file[ iterator ].filename         = NULL;
		segment_table->segment_file[ iterator ].file_descriptor  = -1;
		segment_table->segment_file[ iterator ].file_offset      = 0;
		segment_table->segment_file[ iterator ].amount_of_chunks = 0;

		segment_table->segment_file[ iterator ].section_list = (LIBEWF_SECTION_LIST *) libewf_common_alloc( LIBEWF_SECTION_LIST_SIZE );

		if( segment_table->segment_file[ iterator ].section_list == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocate section list.\n",
			 function );

			return( -1 );
		}
		segment_table->segment_file[ iterator ].section_list->first = NULL;
		segment_table->segment_file[ iterator ].section_list->last  = NULL;
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
		if( segment_table->segment_file[ iterator ].filename != NULL )
		{
			libewf_common_free( segment_table->segment_file[ iterator ].filename );
		}
		if( segment_table->segment_file[ iterator ].section_list != NULL )
		{
			section_list_entry = segment_table->segment_file[ iterator ].section_list->first;

			while( section_list_entry != NULL )
			{
				current_section_list_entry = section_list_entry;
				section_list_entry         = section_list_entry->next;

				libewf_common_free( current_section_list_entry );
			}
			libewf_common_free( segment_table->segment_file[ iterator ].section_list );
		}
	}
	libewf_common_free( segment_table->segment_file );
	libewf_common_free( segment_table );
}

/* Retrieves a filename of a certain segment
 * Returns 1 if succesful, or -1 on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_segment_table_get_wide_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment, wchar_t *filename, size_t length_filename )
#else
int libewf_segment_table_get_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment, char *filename, size_t length_filename )
#endif
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	static char *function  = "libewf_segment_table_get_wide_filename";
#else
	static char *function  = "libewf_segment_table_get_filename";
#endif
	size_t filename_length = 0;

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
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( segment > segment_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: segment out of range.\n",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file[ segment ].filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table - missing filename for segment file: %" PRIu16 ".\n",
		 function, segment );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	filename_length = libewf_common_wide_string_length( segment_table->segment_file[ segment ].filename );
#else
	filename_length = libewf_common_string_length( segment_table->segment_file[ segment ].filename );
#endif

	/* Add one additional character for the end of line
	 */
	filename_length += 1;

	if( length_filename < filename_length )
	{
		LIBEWF_WARNING_PRINT( "%s: filename too small.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	if( libewf_common_wide_memcpy(
	     filename,
	     segment_table->segment_file[ segment ].filename,
	     filename_length ) == NULL )
#else
	if( libewf_common_memcpy(
	     filename,
	     segment_table->segment_file[ segment ].filename,
	     filename_length ) == NULL )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets a filename for a specific segment
 * Creates a duplicate of the string
 * Returns 1 if succesful, or -1 on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_segment_table_set_wide_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment, const wchar_t *filename, size_t length_filename )
#else
int libewf_segment_table_set_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment, const char *filename, size_t length_filename )
#endif
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	static char *function = "libewf_segment_table_set_wide_filename";
#else
	static char *function = "libewf_segment_table_set_filename";
#endif

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
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( segment > segment_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: segment out of range.\n",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file[ segment ].filename != NULL )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
		LIBEWF_WARNING_PRINT( "%s: duplicate segments not supported: segment %d in %ls was already specified as %ls.\n",
		 function, segment, filename, segment_table->segment_file[ segment ].filename );
#else
		LIBEWF_WARNING_PRINT( "%s: duplicate segments not supported: segment %d in %s was already specified as %s.\n",
		 function, segment, filename, segment_table->segment_file[ segment ].filename );
#endif

		return( -1 );
	}
	if( length_filename == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename length is zero.\n",
		 function );

		return( -1 );
	}
	if( length_filename >= (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* One additional byte for the end of string character is needed
	 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	segment_table->segment_file[ segment ].filename = (wchar_t *) libewf_common_alloc( sizeof( wchar_t ) * ( length_filename + 1 ) );
#else
	segment_table->segment_file[ segment ].filename = (char *) libewf_common_alloc( sizeof( char ) * ( length_filename + 1 ) );
#endif

	if( segment_table->segment_file[ segment ].filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create filename.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	if( libewf_common_wide_memcpy( segment_table->segment_file[ segment ].filename, filename, length_filename ) == NULL )
#else
	if( libewf_common_memcpy( segment_table->segment_file[ segment ].filename, filename, length_filename ) == NULL )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename.\n",
		 function );

		libewf_common_free( segment_table->segment_file[ segment ].filename );

		segment_table->segment_file[ segment ].filename = NULL;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	segment_table->segment_file[ segment ].filename[ length_filename ] = '\0';

	return( 1 );
}

