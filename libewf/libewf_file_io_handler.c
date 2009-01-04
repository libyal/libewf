/*
 * libewf file io handler
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
#include "libewf_file_io_handler.h"
#include "libewf_notify.h"
#include "libewf_string.h"

/* Allocates memory for a file io handler struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_file_io_handler_t *libewf_file_io_handler_alloc( size_t amount )
{
	libewf_file_io_handler_t *handler = NULL;
	static char *function             = "libewf_file_io_handler_alloc";
	uint16_t iterator                 = 0;

	handler = (libewf_file_io_handler_t *) libewf_common_alloc( sizeof( libewf_file_io_handler_t ) );

	if( handler == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file io handler.\n",
		 function );

		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	handler->filename = (wchar_t **) libewf_common_alloc_cleared(
	                     ( amount * sizeof( wchar_t ) ),
	                     0 );
#else
	handler->filename = (char **) libewf_common_alloc_cleared(
	                     ( amount * sizeof( char ) ),
	                     0 );
#endif

	if( handler->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate filename array.\n",
		 function );

		libewf_common_free( handler );

		return( NULL );
	}
	handler->descriptor = (int *) libewf_common_alloc_cleared(
	                       ( amount * sizeof( int ) ),
	                       -1 );

	if( handler->descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file descriptor array.\n",
		 function );

		libewf_common_free( handler->filename );
		libewf_common_free( handler );

		return( NULL );
	}
	handler->offset = (off64_t *) libewf_common_alloc_cleared(
	                   ( amount * sizeof( off64_t ) ),
	                   0 );

	if( handler->offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file offset array.\n",
		 function );

		libewf_common_free( handler->filename );
		libewf_common_free( handler->descriptor );
		libewf_common_free( handler );

		return( NULL );
	}
	handler->flags = (int *) libewf_common_alloc_cleared(
	                  ( amount * sizeof( int ) ),
	                  0 );

	if( handler->file_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file offset array.\n",
		 function );

		libewf_common_free( handler->filename );
		libewf_common_free( handler->descriptor );
		libewf_common_free( handler->offset );
		libewf_common_free( handler );

		return( NULL );
	}
	handler->amount     = amount;
	handler->open_files = 0;

	return( handler );
}

/* Reallocates memory for the file io handler entries
 * Returns 1 if successful, or -1 on error
 */
int libewf_file_io_handler_realloc( libewf_file_io_handler_t *handler, size_t amount )
{
	void *reallocation    = NULL;
	static char *function = "libewf_file_io_handler_realloc";
	uint16_t iterator     = 0;

	if( handler == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file io handler.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	reallocation = libewf_common_realloc_new_cleared(
	                handler->filename,
	                ( handler->amount * sizeof( wchar_t ) ),
	                ( amount * sizeof( wchar_t ) ),
	                0 );
#else
	reallocation = libewf_common_realloc_new_cleared(
	                handler->filename,
	                ( handler->amount * sizeof( char ) ),
	                ( amount * sizeof( char ) ),
	                0 );
#endif

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic filename array.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	handler->filename = (wchar_t **) reallocation;
#else
	handler->filename = (char **) reallocation;
#endif
	reallocation      = libewf_common_realloc_new_cleared(
	                     handler->descriptor,
	                     ( handler->amount * sizeof( int ) ),
	                     ( amount * sizeof( int ) ),
	                     -1 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic file descriptor array.\n",
		 function );

		return( -1 );
	}
	handler->descriptor = (int *) reallocation;
	reallocation        = libewf_common_realloc_new_cleared(
	                       handler->offset,
	                       ( handler->amount * sizeof( off64_t ) ),
	                       ( amount * sizeof( off64_t ) ),
	                       0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic file offset array.\n",
		 function );

		return( -1 );
	}
	handler->offset = (off64_t *) reallocation;
	reallocation    = libewf_common_realloc_new_cleared(
	                   handler->flags,
	                   ( handler->amount * sizeof( int ) ),
	                   ( amount * sizeof( int ) ),
	                   0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic file flags array.\n",
		 function );

		return( -1 );
	}
	handler->flags  = (int *) reallocation;
	handler->amount = amount;

	return( 1 );
}

/* Frees memory of a file io handler
 */
void libewf_file_io_handler_free( libewf_file_io_handler_t *handler )
{
	static char *function = "libewf_file_io_handler_free";

	if( handler == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file io handler.\n",
		 function );

		return;
	}
	libewf_common_free( handler->filename );
	libewf_common_free( handler->descriptor );
	libewf_common_free( handler->offset );
	libewf_common_free( handler->flags );
	libewf_common_free( handler );
}

/* Gets a filename of a certain segment
 * Returns the filename pointer, or NULL on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
wchar_t *libewf_file_io_handler_get_wide_filename( LIBEWF_FILE_IO_HANDLER *segment_table, uint16_t segment )
#else
char *libewf_file_io_handler_get_filename( LIBEWF_FILE_IO_HANDLER *segment_table, uint16_t segment )
#endif
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	static char *function = "libewf_file_io_handler_get_wide_filename";
#else
	static char *function = "libewf_file_io_handler_get_filename";
#endif

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return( NULL );
	}
	if( segment > segment_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: segment out of range.\n",
		 function );

		return( NULL );
	}
	return( segment_table->filename[ segment ] );
}

/* Sets a filename for a specific segment
 * Creates a duplicate of the string
 * Returns 1 if succesful, or -1 on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int8_t libewf_file_io_handler_set_wide_filename( LIBEWF_FILE_IO_HANDLER *segment_table, uint16_t segment, const wchar_t *filename, size_t length_filename )
#else
int8_t libewf_file_io_handler_set_filename( LIBEWF_FILE_IO_HANDLER *segment_table, uint16_t segment, const char *filename, size_t length_filename )
#endif
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	static char *function = "libewf_file_io_handler_set_wide_filename";
#else
	static char *function = "libewf_file_io_handler_set_filename";
#endif

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
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
	if( segment_table->filename[ segment ] != NULL )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
		LIBEWF_WARNING_PRINT( "%s: duplicate segments not supported: segment %d in %ls was already specified as %ls.\n",
		 function, segment, filename, segment_table->filename[ segment ] );
#else
		LIBEWF_WARNING_PRINT( "%s: duplicate segments not supported: segment %d in %s was already specified as %s.\n",
		 function, segment, filename, segment_table->filename[ segment ] );
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
	segment_table->filename[ segment ] = (wchar_t *) libewf_common_alloc( sizeof( wchar_t ) * ( length_filename + 1 ) );
#else
	segment_table->filename[ segment ] = (char *) libewf_common_alloc( sizeof( char ) * ( length_filename + 1 ) );
#endif

	if( segment_table->filename[ segment ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create filename.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	if( libewf_common_wide_memcpy( segment_table->filename[ segment ], filename, length_filename ) == NULL )
#else
	if( libewf_common_memcpy( segment_table->filename[ segment ], filename, length_filename ) == NULL )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename.\n",
		 function );

		libewf_common_free( segment_table->filename[ segment ] );

		segment_table->filename[ segment ] = NULL;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	segment_table->filename[ segment ][ length_filename ] = '\0';

	return( 1 );
}

