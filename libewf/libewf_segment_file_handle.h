/*
 * Segment file handle functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_segment_file_handle libewf_segment_file_handle_t;

struct libewf_segment_file_handle
{
	/* The segment file index
	 */
	int segment_file_index;
};

int libewf_segment_file_handle_initialize(
     libewf_segment_file_handle_t **segment_file_handle,
     int file_io_pool_entry,
     libcerror_error_t **error );

int libewf_segment_file_handle_free(
     libewf_segment_file_handle_t **segment_file_handle,
     libcerror_error_t **error );

int libewf_segment_file_handle_clone(
     libewf_segment_file_handle_t **destination_segment_file_handle,
     libewf_segment_file_handle_t *source_segment_file_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

