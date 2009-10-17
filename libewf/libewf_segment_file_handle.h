/*
 * Segment file handle
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
#include <types.h>

#include <liberror.h>

#include "libewf_list_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_segment_file_handle libewf_segment_file_handle_t;

struct libewf_segment_file_handle
{
	/* The file io pool entry
	 */
	int file_io_pool_entry;

	/* The amount of chunks
	 */
	uint32_t amount_of_chunks;

        /* The list of all the sections
         */
        libewf_list_t *section_list;

	/* The segment file type
	 */
	uint8_t file_type;

	/* Value to indicate that the segment file is open for writing
	 */
	uint8_t write_open;
};

int libewf_segment_file_handle_initialize(
     libewf_segment_file_handle_t **segment_file_handle,
     int file_io_pool_entry,
     liberror_error_t **error );

int libewf_segment_file_handle_free(
     libewf_segment_file_handle_t **segment_file_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

