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
#include "libewf_list_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_FILE_IO_POOL_UNLIMITED_AMOUNT_OF_OPEN_FILES	0

typedef struct libewf_file_io_pool libewf_file_io_pool_t;

struct libewf_file_io_pool
{
	/* The amount of files in the pool
	 */
	int amount_of_files;

	/* The amount of open file descriptors in the pool
	 */
	int amount_of_open_files;

	/* The maximum amount of open file descriptors in the pool
	 */
	int maximum_amount_of_open_files;

	/* The amount of file io handles in the pool
	 */
	int amount_of_files_io_handles;

	/* A dynamic array containing the file io handles
	 */
	libewf_file_io_handle_t *file_io_handle;

	/* A list containing the file io handles in order of the last use
	 * it starts with the last used at the beginning of the list
	 * the value of the list element refers to the corresponding file io handle
	 */
	libewf_list_t *last_used_list;
};

int libewf_file_io_pool_initialize(
     libewf_file_io_pool_t **file_io_pool,
     int amount_of_files_io_handles,
     int maximum_amount_of_open_files );

int libewf_file_io_pool_free(
     libewf_file_io_pool_t **file_io_pool );

int libewf_file_io_pool_resize(
     libewf_file_io_pool_t *file_io_pool,
     int amount_of_files_io_handles );

int libewf_file_io_pool_open_file_io_handle(
     libewf_file_io_pool_t *file_io_pool,
     libewf_file_io_handle_t *file_io_handle,
     int flags );

int libewf_file_io_pool_open(
     libewf_file_io_pool_t *file_io_pool,
     const system_character_t *filename,
     int flags );

int libewf_file_io_pool_reopen(
     libewf_file_io_pool_t *file_io_pool,
     int entry,
     int flags );

int libewf_file_io_pool_close(
     libewf_file_io_pool_t *file_io_pool,
     int entry );

ssize_t libewf_file_io_pool_read(
         libewf_file_io_pool_t *file_io_pool,
         int entry,
         uint8_t *buffer,
         size_t size );

ssize_t libewf_file_io_pool_write(
         libewf_file_io_pool_t *file_io_pool,
         int entry,
         uint8_t *buffer,
         size_t size );

off64_t libewf_file_io_pool_seek_offset(
         libewf_file_io_pool_t *file_io_pool,
         int entry,
         off64_t offset,
         int whence );

#if defined( __cplusplus )
}
#endif

#endif

