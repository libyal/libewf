/*
 * Segment file handle
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

#if !defined( _LIBEWF_SEGMENT_FILE_HANDLE_H )
#define _LIBEWF_SEGMENT_FILE_HANDLE_H

#include <common.h>
#include <system_string.h>
#include <types.h>

#include "libewf_section_list.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_segment_file_handle libewf_segment_file_handle_t;

struct libewf_segment_file_handle
{
	/* The filename
	 */
	system_character_t *filename;

	/* The length of the filename
	 */
	size_t length_filename;

	/* The file descriptor
	 */
	int file_descriptor;

	/* The file offset
	 */
	off64_t file_offset;

	/* The amount of chunks
	 */
	uint32_t amount_of_chunks;

        /* The list of all the sections
         */
        libewf_section_list_t *section_list;

	/* The segment file type
	 */
	uint8_t file_type;
};

int libewf_segment_file_handle_initialize(
     libewf_segment_file_handle_t **segment_file_handle );

int libewf_segment_file_handle_free(
     libewf_segment_file_handle_t **segment_file_handle );

int libewf_segment_file_handle_get_filename(
     libewf_segment_file_handle_t *segment_file_handle,
     system_character_t *filename,
     size_t length_filename );

int libewf_segment_file_handle_set_filename(
     libewf_segment_file_handle_t *segment_file_handle,
     const system_character_t *filename,
     size_t length_filename );

int libewf_segment_file_handle_open(
     libewf_segment_file_handle_t *segment_file_handle,
     int flags );

int libewf_segment_file_handle_reopen(
     libewf_segment_file_handle_t *segment_file_handle,
     int flags );

ssize_t libewf_segment_file_handle_read(
         libewf_segment_file_handle_t *segment_file_handle,
         void *buffer,
         size_t size );

ssize_t libewf_segment_file_handle_write(
         libewf_segment_file_handle_t *segment_file_handle,
         void *buffer,
         size_t size );

off64_t libewf_segment_file_handle_seek_offset(
         libewf_segment_file_handle_t *segment_file_handle,
         off64_t offset );

int libewf_segment_file_handle_close(
     libewf_segment_file_handle_t *segment_file_handle );

#if defined( __cplusplus )
}
#endif

#endif

