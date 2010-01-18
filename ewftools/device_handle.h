/* 
 * Device handle
 *
 * Copyright (C) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if defined( HAVE_LOCAL_LIBSMDEV )
#include <libsmdev_types.h>
#elif defined( HAVE_LIBSMDEV_H )
#include <libsmdev.h>
#endif

#if defined( HAVE_LOCAL_LIBSMRAW )
#include <libsmraw_types.h>
#elif defined( HAVE_LIBSMRAW_H )
#include <libsmraw.h>
#endif

#include <libsystem.h>

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

	/* libsmdev input handle
	 */
	libsmdev_handle_t *dev_input_handle;

#if defined( HAVE_LIBSMRAW ) || defined( HAVE_LOCAL_LIBSMRAW )
	/* libsmraw input handle
	 */
	libsmdev_handle_t *raw_input_handle;
#else
#if defined( WINAPI )
	/* File handle
	 */
	HANDLE file_handle;
#else
	/* File descriptor
	 */
	int file_descriptor;
#endif

	/* The file size
	 */
	size64_t file_size;

#endif /* defined( HAVE_LIBSMRAW ) || defined( HAVE_LOCAL_LIBSMRAW ) */
};

int device_handle_initialize(
     device_handle_t **device_handle,
     liberror_error_t **error );

int device_handle_free(
     device_handle_t **device_handle,
     liberror_error_t **error );

int device_handle_signal_abort(
     device_handle_t *device_handle,
     liberror_error_t **error );

int device_handle_open_input(
     device_handle_t *device_handle,
     libsystem_character_t * const * filenames,
     int amount_of_filenames,
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

int device_handle_get_information_value(
     device_handle_t *device_handle,
     const uint8_t *information_value_identifier,
     size_t information_value_identifier_length,
     libsystem_character_t *information_value,
     size_t information_value_size,
     liberror_error_t **error );

int device_handle_set_error_values(
     device_handle_t *device_handle,
     uint8_t amount_of_error_retries,
     size_t error_granularity,
     uint8_t zero_buffer_on_error,
     liberror_error_t **error );

int device_handle_get_amount_of_read_errors(
     device_handle_t *device_handle,
     int *amount_of_errors,
     liberror_error_t **error );

int device_handle_get_read_error(
     device_handle_t *device_handle,
     int index,
     off64_t *offset,
     size64_t *size,
     liberror_error_t **error );

int device_handle_media_information_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     liberror_error_t **error );

int device_handle_read_errors_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

