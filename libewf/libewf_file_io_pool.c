/*
 * libewf file io pool
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
#include "libewf_file_io_pool.h"
#include "libewf_notify.h"
#include "libewf_string.h"

/* Allocates memory for a file io pool struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_file_io_pool_t *libewf_file_io_pool_alloc( size_t amount )
{
	libewf_file_io_pool_t *pool = NULL;
	static char *function       = "libewf_file_io_pool_alloc";

	pool = (libewf_file_io_pool_t *) libewf_common_alloc( sizeof( libewf_file_io_pool_t ) );

	if( pool == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file io pool.\n",
		 function );

		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	pool->filename = (wchar_t **) libewf_common_alloc_cleared(
	                     ( amount * sizeof( wchar_t ) ),
	                     0 );
#else
	pool->filename = (char **) libewf_common_alloc_cleared(
	                     ( amount * sizeof( char ) ),
	                     0 );
#endif

	if( pool->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate filename array.\n",
		 function );

		libewf_common_free( pool );

		return( NULL );
	}
	pool->descriptor = (int *) libewf_common_alloc_cleared(
	                       ( amount * sizeof( int ) ),
	                       -1 );

	if( pool->descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file descriptor array.\n",
		 function );

		libewf_common_free( pool->filename );
		libewf_common_free( pool );

		return( NULL );
	}
	pool->offset = (off64_t *) libewf_common_alloc_cleared(
	                   ( amount * sizeof( off64_t ) ),
	                   0 );

	if( pool->offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file offset array.\n",
		 function );

		libewf_common_free( pool->filename );
		libewf_common_free( pool->descriptor );
		libewf_common_free( pool );

		return( NULL );
	}
	pool->flags = (int *) libewf_common_alloc_cleared(
	               ( amount * sizeof( int ) ),
	               0 );

	if( pool->offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file offset array.\n",
		 function );

		libewf_common_free( pool->filename );
		libewf_common_free( pool->descriptor );
		libewf_common_free( pool->offset );
		libewf_common_free( pool );

		return( NULL );
	}
	pool->amount     = amount;
	pool->open_files = 0;

	return( pool );
}

/* Reallocates memory for the file io pool entries
 * Returns 1 if successful, or -1 on error
 */
int libewf_file_io_pool_realloc( libewf_file_io_pool_t *pool, size_t amount )
{
	void *reallocation    = NULL;
	static char *function = "libewf_file_io_pool_realloc";

	if( pool == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file io pool.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	reallocation = libewf_common_realloc_new_cleared(
	                pool->filename,
	                ( pool->amount * sizeof( wchar_t ) ),
	                ( amount * sizeof( wchar_t ) ),
	                0 );
#else
	reallocation = libewf_common_realloc_new_cleared(
	                pool->filename,
	                ( pool->amount * sizeof( char ) ),
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
	pool->filename = (wchar_t **) reallocation;
#else
	pool->filename = (char **) reallocation;
#endif
	reallocation      = libewf_common_realloc_new_cleared(
	                     pool->descriptor,
	                     ( pool->amount * sizeof( int ) ),
	                     ( amount * sizeof( int ) ),
	                     -1 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic file descriptor array.\n",
		 function );

		return( -1 );
	}
	pool->descriptor = (int *) reallocation;
	reallocation        = libewf_common_realloc_new_cleared(
	                       pool->offset,
	                       ( pool->amount * sizeof( off64_t ) ),
	                       ( amount * sizeof( off64_t ) ),
	                       0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic file offset array.\n",
		 function );

		return( -1 );
	}
	pool->offset = (off64_t *) reallocation;
	reallocation    = libewf_common_realloc_new_cleared(
	                   pool->flags,
	                   ( pool->amount * sizeof( int ) ),
	                   ( amount * sizeof( int ) ),
	                   0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic file flags array.\n",
		 function );

		return( -1 );
	}
	pool->flags  = (int *) reallocation;
	pool->amount = amount;

	return( 1 );
}

/* Frees memory of a file io pool
 */
void libewf_file_io_pool_free( libewf_file_io_pool_t *pool )
{
	static char *function = "libewf_file_io_pool_free";

	if( pool == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file io pool.\n",
		 function );

		return;
	}
	libewf_common_free( pool->filename );
	libewf_common_free( pool->descriptor );
	libewf_common_free( pool->offset );
	libewf_common_free( pool->flags );
	libewf_common_free( pool );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_file_io_pool_wide_open( LIBEWF_FILE_IO_POOL *pool, wchar_t *filename, int flags )
#else
int libewf_file_io_pool_open( LIBEWF_FILE_IO_POOL *pool, char *filename, int flags )
#endif
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	static char *function = "libewf_file_io_pool_wide_open";
#else
	static char *function = "libewf_file_io_pool_open";
#endif

	if( pool == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file io pool.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
#ifdef TOEDIT
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
#endif

	return( 1 );
}

ssize_t libewf_file_io_pool_read( LIBEWF_FILE_IO_POOL *pool, size_t entry, uint8_t *buffer, size_t size );
ssize_t libewf_file_io_pool_write( LIBEWF_FILE_IO_POOL *pool, size_t entry, uint8_t *buffer, size_t size );
off64_t libewf_file_io_pool_seek( LIBEWF_FILE_IO_POOL *pool, size_t entry, off64_t offset, int whence );
int libewf_file_io_pool_close( LIBEWF_FILE_IO_POOL *pool, size_t entry );

