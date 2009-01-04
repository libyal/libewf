/*
 * File Input/Output (IO) handle
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

#if !defined( _LIBEWF_FILE_IO_HANDLE_H )
#define _LIBEWF_FILE_IO_HANDLE_H

#include <common.h>
#include <system_string.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_file_io_handle libewf_file_io_handle_t;

struct libewf_file_io_handle
{
	/* The filename
	 */
	system_character_t *filename;

	/* The size of the filename
	 */
	size_t filename_size;

	/* The file descriptor
	 */
	int file_descriptor;

	/* The current file offsets
	 */
	off64_t file_offset;

	/* The file flags
	 */
	int flags;
};

int libewf_file_io_handle_get_filename(
     libewf_file_io_handle_t *file_io_handle,
     system_character_t *filename,
     size_t filename_size );

int libewf_file_io_handle_set_filename(
     libewf_file_io_handle_t *file_io_handle,
     const system_character_t *filename,
     size_t filename_size );

int libewf_file_io_handle_open(
     libewf_file_io_handle_t *file_io_handle,
     int flags );

int libewf_file_io_handle_close(
     libewf_file_io_handle_t *file_io_handle );

ssize_t libewf_file_io_handle_read(
         libewf_file_io_handle_t *file_io_handle,
         uint8_t *buffer,
         size_t size );

ssize_t libewf_file_io_handle_write(
         libewf_file_io_handle_t *file_io_handle,
         uint8_t *buffer,
         size_t size );

off64_t libewf_file_io_handle_seek_offset(
         libewf_file_io_handle_t *file_io_handle,
         off64_t offset );

#if defined( __cplusplus )
}
#endif

#endif

