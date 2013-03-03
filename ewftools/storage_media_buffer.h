/* 
 * Storage media buffer
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

#if !defined( _STORAGE_MEDIA_BUFFER_H )
#define _STORAGE_MEDIA_BUFFER_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct storage_media_buffer storage_media_buffer_t;

struct storage_media_buffer
{
	/* The raw buffer
	 */
	uint8_t *raw_buffer;

	/* The raw buffer size
	 */
	size_t raw_buffer_size;

	/* The size of the data in the raw buffer
	 */
	size_t raw_buffer_data_size;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
        /* Value to indicate if the compression buffer
	 * contains uncompressed data
	 */
        int8_t data_in_compression_buffer;

	/* Value to indicate if the data is compressed
	 */
	int8_t is_compressed;

	/* The compression buffer
	 */
	uint8_t *compression_buffer;

	/* The compression buffer size
	 */
	size_t compression_buffer_size;

	/* The size of the data in the compression buffer
	 */
	size_t compression_buffer_data_size;

	/* The checksum buffer
	 */
	uint8_t *checksum_buffer;

	/* Value to indicate if the checksum should be processed
	 * read or written
	 */
	int8_t process_checksum;

	/* The checksum of the data within the buffer
	 */
	uint32_t checksum;
#endif
};

int storage_media_buffer_initialize(
     storage_media_buffer_t **buffer,
     size_t size,
     libcerror_error_t **error );

int storage_media_buffer_free(
     storage_media_buffer_t **buffer,
     libcerror_error_t **error );

int storage_media_buffer_resize(
     storage_media_buffer_t *buffer,
     size_t size,
     libcerror_error_t **error );

int storage_media_buffer_get_data(
     storage_media_buffer_t *buffer,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

