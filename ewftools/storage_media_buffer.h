/*
 * Storage media buffer
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _STORAGE_MEDIA_BUFFER_H )
#define _STORAGE_MEDIA_BUFFER_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"
#include "ewftools_libewf.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum STORAGE_MEDIA_BUFFER_MODES
{
	STORAGE_MEDIA_BUFFER_MODE_BUFFERED	= 0,
	STORAGE_MEDIA_BUFFER_MODE_CHUNK_DATA	= 1
};

typedef struct storage_media_buffer storage_media_buffer_t;

struct storage_media_buffer
{
	/* The mode
	 */
	uint8_t mode;

	/* The storage media offset
	 */
	off64_t storage_media_offset;

	/* The raw buffer
	 */
	uint8_t *raw_buffer;

	/* The raw buffer size
	 */
	size_t raw_buffer_size;

	/* The size of the data in the raw buffer
	 */
	size_t raw_buffer_data_size;

	/* The data chunk
	 */
	libewf_data_chunk_t *data_chunk;

	/* The requested size
	 */
	size_t requested_size;

	/* The processed size
	 */
	size_t processed_size;

	/* Value to indicate the data is corrupted
	 */
	uint8_t is_corrupted;
};

int storage_media_buffer_initialize(
     storage_media_buffer_t **buffer,
     libewf_handle_t *handle,
     uint8_t mode,
     size_t size,
     libcerror_error_t **error );

int storage_media_buffer_free(
     storage_media_buffer_t **buffer,
     libcerror_error_t **error );

int storage_media_buffer_get_data(
     storage_media_buffer_t *buffer,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error );

int storage_media_buffer_compare(
     storage_media_buffer_t *first_buffer,
     storage_media_buffer_t *second_buffer,
     libcerror_error_t **error );

int storage_media_buffer_is_corrupted(
     storage_media_buffer_t *storage_media_buffer,
     libcerror_error_t **error );

ssize_t storage_media_buffer_read_from_handle(
         storage_media_buffer_t *storage_media_buffer,
         libewf_handle_t *handle,
         size_t read_size,
         libcerror_error_t **error );

ssize_t storage_media_buffer_read_process(
         storage_media_buffer_t *storage_media_buffer,
         libcerror_error_t **error );

ssize_t storage_media_buffer_write_process(
         storage_media_buffer_t *storage_media_buffer,
         libcerror_error_t **error );

ssize_t storage_media_buffer_write_to_handle(
         storage_media_buffer_t *storage_media_buffer,
         libewf_handle_t *handle,
         size_t write_size,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _STORAGE_MEDIA_BUFFER_H ) */

