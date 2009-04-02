/* 
 * Device handle
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

#if !defined( _DEVICE_HANDLE_H )
#define _DEVICE_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( WINAPI )
#include <winbase.h>
#endif

#include "system_string.h"
#include "storage_media_buffer.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum DEVICE_HANDLE_TYPES
{
	DEVICE_HANDLE_TYPE_DEVICE	= (int) 'd',
	DEVICE_HANDLE_TYPE_FILE		= (int) 'f'
};

typedef struct device_handle device_handle_t;

struct device_handle
{
#if defined( WINAPI )
	/* File handle
	 */
	HANDLE file_handle;
#else
	/* File descriptor
	 */
	int file_descriptor;
#endif

	/* The type
	 */
	int type;

	/* The amount of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* Value to indicate the bytes per sector value was set
	 */
	uint8_t bytes_per_sector_set;

	/* The media size
	 */
	size64_t media_size;

	/* Value to indicate the media size value was set
	 */
	uint8_t media_size_set;
};

int device_handle_initialize(
     device_handle_t **device_handle,
     liberror_error_t **error );

int device_handle_free(
     device_handle_t **device_handle,
     liberror_error_t **error );

int device_handle_open_input(
     device_handle_t *device_handle,
     const system_character_t *filename,
     liberror_error_t **error );

off64_t device_handle_seek_offset(
         device_handle_t *device_handle,
         off64_t offset,
         int whence,
         liberror_error_t **error );

ssize_t device_handle_read_buffer(
         device_handle_t *device_handle,
         uint8_t *buffer,
         size_t read_size,
         liberror_error_t **error );

int device_handle_close(
     device_handle_t *device_handle,
     liberror_error_t **error );

int device_handle_get_media_size(
     device_handle_t *device_handle,
     size64_t *media_size,
     liberror_error_t **error );

int device_handle_get_bytes_per_sector(
     device_handle_t *device_handle,
     uint32_t *bytes_per_sector,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

