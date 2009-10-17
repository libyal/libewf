/* 
 * Device handle
 *
 * Copyright (C) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _DEVICE_HANDLE_H )
#define _DEVICE_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <libsystem.h>

#if defined( WINAPI )
#include <windows.h>
#endif

#include "storage_media_buffer.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum DEVICE_HANDLE_TYPES
{
	DEVICE_HANDLE_TYPE_DEVICE	= (int) 'd',
	DEVICE_HANDLE_TYPE_FILE		= (int) 'f'
};

enum DEVICE_TYPES
{
	DEVICE_TYPE_HARDDISK		= (int) 'h',
	DEVICE_TYPE_OPTICAL_DISK	= (int) 'o'
};

typedef struct device_handle device_handle_t;

struct device_handle
{
	/* The device handle type
	 */
	int type;

#if defined( WINAPI )
	/* File handle
	 */
	HANDLE file_handle;
#else
	/* File descriptor
	 */
	int file_descriptor;
#endif

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

	/* The bus type
	 */
	uint8_t bus_type;

	/* The device type
	 */
	uint8_t device_type;

	/* Value to indicate if the device is removable
	 */
	uint8_t removable;

	/* The vendor string
	 */
	uint8_t vendor[ 64 ];

	/* The model string
	 */
	uint8_t model[ 64 ];

	/* The serial number string
	 */
	uint8_t serial_number[ 64 ];

	/* The amount of sessions for an optical disc
	 */
	uint16_t amount_of_sessions;

	/* Value to indicate the media information values were set
	 */
	uint8_t media_information_set;

	/* The amount of read error retries
	 */
	int8_t read_error_retry;

	/* The amount of bytes to skip on a read error
	 */
	uint32_t byte_error_granularity;

	/* Value to indicate if the block that is made up by the
	 * byte error granularity should be wiped on read error
	 */
	uint8_t wipe_block_on_read_error;
};

int device_handle_initialize(
     device_handle_t **device_handle,
     liberror_error_t **error );

int device_handle_free(
     device_handle_t **device_handle,
     liberror_error_t **error );

int device_handle_open_input(
     device_handle_t *device_handle,
     const libsystem_character_t *filename,
     liberror_error_t **error );

int device_handle_close(
     device_handle_t *device_handle,
     liberror_error_t **error );

ssize_t device_handle_read_buffer(
         device_handle_t *device_handle,
         uint8_t *buffer,
         size_t read_size,
         liberror_error_t **error );

off64_t device_handle_seek_offset(
         device_handle_t *device_handle,
         off64_t offset,
         int whence,
         liberror_error_t **error );

int device_handle_get_media_size(
     device_handle_t *device_handle,
     size64_t *media_size,
     liberror_error_t **error );

int device_handle_get_media_type(
     device_handle_t *device_handle,
     uint8_t *media_type,
     liberror_error_t **error );

int device_handle_get_bytes_per_sector(
     device_handle_t *device_handle,
     uint32_t *bytes_per_sector,
     liberror_error_t **error );

int device_handle_trim_copy_from_byte_stream(
     uint8_t *string,
     size_t string_size,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     liberror_error_t **error );

int device_handle_determine_media_information(
     device_handle_t *device_handle,
     liberror_error_t **error );

int device_handle_get_media_information_value(
     device_handle_t *device_handle,
     char *media_information_value_identifier,
     size_t media_information_value_identifier_length,
     libsystem_character_t *media_information_value,
     size_t media_information_value_size,
     liberror_error_t **error );

int device_handle_set_read_error_values(
     device_handle_t *device_handle,
     int8_t read_error_retry,
     uint32_t byte_error_granularity,
     uint8_t wipe_block_on_read_error,
     liberror_error_t **error );

int device_handle_media_information_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

