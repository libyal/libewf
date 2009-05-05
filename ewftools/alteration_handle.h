/* 
 * Alteration handle
 *
 * Copyright (C) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _ALTERATION_HANDLE_H )
#define _ALTERATION_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#include <libewf.h>

#include "storage_media_buffer.h"
#include "system_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct alteration_handle alteration_handle_t;

struct alteration_handle
{
	/* The libewf input handle
	 */
	libewf_handle_t *input_handle;
};

int alteration_handle_initialize(
     alteration_handle_t **alteration_handle,
     liberror_error_t **error );

int alteration_handle_free(
     alteration_handle_t **alteration_handle,
     liberror_error_t **error );

int alteration_handle_signal_abort(
     alteration_handle_t *alteration_handle,
     liberror_error_t **error );

int alteration_handle_open_input(
     alteration_handle_t *alteration_handle,
     system_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error );

int alteration_handle_close(
     alteration_handle_t *alteration_handle,
     liberror_error_t **error );

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
ssize_t alteration_handle_prepare_read_buffer(
         alteration_handle_t *alteration_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error );

ssize_t alteration_handle_read_buffer(
         alteration_handle_t *alteration_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error );
#endif

ssize_t alteration_handle_prepare_write_buffer(
         alteration_handle_t *alteration_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error );

ssize_t alteration_handle_write_buffer(
         alteration_handle_t *alteration_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t write_size,
         liberror_error_t **error );

off64_t alteration_handle_seek_offset(
         alteration_handle_t *alteration_handle,
         off64_t offset,
         liberror_error_t **error );

int alteration_handle_get_media_size(
     alteration_handle_t *alteration_handle,
     size64_t *media_size,
     liberror_error_t **error );

int alteration_handle_get_chunk_size(
     alteration_handle_t *alteration_handle,
     size32_t *chunk_size,
     liberror_error_t **error );

int alteration_handle_set_header_codepage(
     alteration_handle_t *alteration_handle,
     int header_codepage,
     liberror_error_t **error );

int alteration_handle_set_output_values(
     alteration_handle_t *alteration_handle,
     system_character_t *delta_segment_filename,
     size_t delta_segment_filename_length,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

