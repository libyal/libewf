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

#if !defined( _LIBEWF_FILE_IO_POOL_H )
#define _LIBEWF_FILE_IO_POOL_H

#include <common.h>
#include <system_string.h>
#include <types.h>

#include "libewf_file_io_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_file_io_pool libewf_file_io_pool_t;

struct libewf_file_io_pool
{
	/* The amount of files in the pool
	 */
	size_t amount_of_files;

	/* The amount of open file descriptors
	 */
	size_t amount_of_open_files;

	/* A dynamic array containting the file io handles
	 */
	libewf_file_io_handle_t *handle;
};

int libewf_file_io_pool_initialize(
     libewf_file_io_pool_t **file_io_pool,
     size_t amount_of_files );

int libewf_file_io_pool_free(
     libewf_file_io_pool_t **file_io_pool );

int libewf_file_io_pool_resize(
     libewf_file_io_pool_t *file_io_pool,
     size_t amount_of_files );

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

#if defined( __cplusplus )
}
#endif

#endif

