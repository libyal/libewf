/*
 * File IO pool
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <memory.h>
#include <notify.h>
#include <system_string.h>

#include <libewf/definitions.h>

#include "libewf_file_io_pool.h"

/* Allocates memory for a file io pool struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_file_io_pool_t *libewf_file_io_pool_alloc(
                        size_t amount )
{
	libewf_file_io_pool_t *file_io_pool = NULL;
	static char *function               = "libewf_file_io_pool_alloc";
	size_t iterator                     = 0;

	file_io_pool = (libewf_file_io_pool_t *) memory_allocate(
	                                          sizeof( libewf_file_io_pool_t ) );

	if( file_io_pool == NULL )
	{
		notify_warning_printf( "%s: unable to allocate file io pool.\n",
		 function );

		return( NULL );
	}
	file_io_pool->handle = (libewf_file_io_handle_t *) memory_allocate(
	                                                    sizeof( libewf_file_io_handle_t ) * amount );

	if( file_io_pool->handle == NULL )
	{
		notify_warning_printf( "%s: unable to allocate file io handles.\n",
		 function );

		memory_free(
		 file_io_pool );

		return( NULL );
	}
	for( iterator = 0; iterator < amount; iterator++ )
	{
		file_io_pool->handle[ iterator ].filename        = NULL;
		file_io_pool->handle[ iterator ].file_descriptor = -1;
		file_io_pool->handle[ iterator ].file_offset     = 0;
		file_io_pool->handle[ iterator ].flags           = 0;
	}
	file_io_pool->amount     = amount;
	file_io_pool->open_files = 0;

	return( file_io_pool );
}

/* Reallocates memory for the file io pool entries
 * Returns 1 if successful, or -1 on error
 */
int libewf_file_io_pool_realloc(
     libewf_file_io_pool_t *file_io_pool,
     size_t amount )
{
	void *reallocation    = NULL;
	static char *function = "libewf_file_io_pool_realloc";
	size_t iterator       = 0;

	if( file_io_pool == NULL )
	{
		notify_warning_printf( "%s: invalid file io pool.\n",
		 function );

		return( -1 );
	}
	if( file_io_pool->amount >= amount )
	{
		notify_warning_printf( "%s: new amount must be greater than previous amount.\n",
		 function );

		return( -1 );
	}
	reallocation = memory_reallocate(
	                file_io_pool->handle,
	                ( sizeof( libewf_file_io_handle_t ) * amount ) );

	if( reallocation == NULL )
	{
		notify_warning_printf( "%s: unable to reallocate dynamic file io handles array.\n",
		 function );

		return( -1 );
	}
	file_io_pool->handle = (libewf_file_io_handle_t *) reallocation;

	for( iterator = file_io_pool->amount; iterator < amount; iterator++ )
	{
		file_io_pool->handle[ iterator ].filename        = NULL;
		file_io_pool->handle[ iterator ].file_descriptor = -1;
		file_io_pool->handle[ iterator ].file_offset     = 0;
		file_io_pool->handle[ iterator ].flags           = 0;
	}
	file_io_pool->amount = amount;

	return( 1 );
}

/* Frees memory of a file io pool
 */
void libewf_file_io_pool_free(
      libewf_file_io_pool_t *file_io_pool )
{
	static char *function = "libewf_file_io_pool_free";
	size_t iterator       = 0;

	if( file_io_pool == NULL )
	{
		notify_warning_printf( "%s: invalid file io pool.\n",
		 function );

		return;
	}
	for( iterator = 0; iterator < file_io_pool->amount; iterator++ )
	{
		if( file_io_pool->handle[ iterator ].filename != NULL )
		{
			memory_free(
			 file_io_pool->handle[ iterator ].filename );
		}
	}
	memory_free(
	 file_io_pool->handle );
	memory_free(
	 file_io_pool );
}

int libewf_file_io_pool_open(
     libewf_file_io_pool_t *file_io_pool,
     system_character_t *filename,
     int flags );

ssize_t libewf_file_io_pool_read(
         libewf_file_io_pool_t *pool,
         size_t entry,
         uint8_t *buffer,
         size_t size );

ssize_t libewf_file_io_pool_write(
         libewf_file_io_pool_t *pool,
         size_t entry,
         uint8_t *buffer,
         size_t size );

off64_t libewf_file_io_pool_seek(
         libewf_file_io_pool_t *pool,
         size_t entry,
         off64_t offset,
         int whence );

int libewf_file_io_pool_close(
     libewf_file_io_pool_t *pool,
     size_t entry );

