/* 
 * Device handle
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

#if !defined( _DEVICE_HANDLE_H )
#define _DEVICE_HANDLE_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"
#include "ewftools_libcstring.h"
#include "ewftools_libodraw.h"
#include "ewftools_libsmdev.h"
#include "ewftools_libsmraw.h"
#include "storage_media_buffer.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The device handle type definitions
 */
enum DEVICE_HANDLE_TYPES
{
	DEVICE_HANDLE_TYPE_DEVICE			= (uint8_t) 'd',
	DEVICE_HANDLE_TYPE_FILE				= (uint8_t) 'f',
	DEVICE_HANDLE_TYPE_OPTICAL_DISC_FILE		= (uint8_t) 'o'
};

/* The media type definitions
 */
enum DEVICE_HANDLE_MEDIA_TYPES
{
	DEVICE_HANDLE_MEDIA_TYPE_REMOVABLE		= 0x00,
	DEVICE_HANDLE_MEDIA_TYPE_FIXED			= 0x01,
	DEVICE_HANDLE_MEDIA_TYPE_OPTICAL		= 0x03,
	DEVICE_HANDLE_MEDIA_TYPE_MEMORY			= 0x10
};

/* The (optical disc) track type definitions
 */
enum DEVICE_HANDLE_TRACK_TYPES
{
	DEVICE_HANDLE_TRACK_TYPE_UNKNOWN,
	DEVICE_HANDLE_TRACK_TYPE_AUDIO,
	DEVICE_HANDLE_TRACK_TYPE_CDG,
	DEVICE_HANDLE_TRACK_TYPE_MODE1_2048,
	DEVICE_HANDLE_TRACK_TYPE_MODE1_2352,
	DEVICE_HANDLE_TRACK_TYPE_MODE2_2048,
	DEVICE_HANDLE_TRACK_TYPE_MODE2_2324,
	DEVICE_HANDLE_TRACK_TYPE_MODE2_2336,
	DEVICE_HANDLE_TRACK_TYPE_MODE2_2352,
	DEVICE_HANDLE_TRACK_TYPE_CDI_2336,
	DEVICE_HANDLE_TRACK_TYPE_CDI_2352,
};

typedef struct device_handle device_handle_t;

struct device_handle
{
	/* The user input buffer
	 */
	libcstring_system_character_t *input_buffer; 

	/* The device handle type
	 */
	uint8_t type;

	/* The TOC filename
	 */
	libcstring_system_character_t *toc_filename;

	/* The TOC filename size
	 */
	size_t toc_filename_size;

	/* libodraw input handle
	 */
	libodraw_handle_t *odraw_input_handle;

	/* libsmdev input handle
	 */
	libsmdev_handle_t *smdev_input_handle;

	/* libsmraw input handle
	 */
	libsmdev_handle_t *smraw_input_handle;

	/* The number of error retries
	 */
	uint8_t number_of_error_retries;

	/* Value to indicate the buffer should be zeroed on error
	 */
	uint8_t zero_buffer_on_error;

	/* The nofication output stream
	 */
	FILE *notify_stream;
};

const char *device_handle_get_track_type(
             uint8_t track_type );

int device_handle_initialize(
     device_handle_t **device_handle,
     libcerror_error_t **error );

int device_handle_free(
     device_handle_t **device_handle,
     libcerror_error_t **error );

int device_handle_signal_abort(
     device_handle_t *device_handle,
     libcerror_error_t **error );

int device_handle_open_input(
     device_handle_t *device_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error );

int device_handle_open_smdev_input(
     device_handle_t *device_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error );

int device_handle_open_odraw_input(
     device_handle_t *device_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error );

int device_handle_open_smraw_input(
     device_handle_t *device_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error );

int device_handle_close(
     device_handle_t *device_handle,
     libcerror_error_t **error );

ssize_t device_handle_read_buffer(
         device_handle_t *device_handle,
         uint8_t *buffer,
         size_t read_size,
         libcerror_error_t **error );

off64_t device_handle_seek_offset(
         device_handle_t *device_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

int device_handle_prompt_for_string(
     device_handle_t *device_handle,
     const libcstring_system_character_t *request_string,
     libcstring_system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error );

int device_handle_prompt_for_number_of_error_retries(
     device_handle_t *device_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error );

int device_handle_prompt_for_zero_buffer_on_error(
     device_handle_t *device_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error );

int device_handle_get_type(
     device_handle_t *device_handle,
     uint8_t *type,
     libcerror_error_t **error );

int device_handle_get_media_size(
     device_handle_t *device_handle,
     size64_t *media_size,
     libcerror_error_t **error );

int device_handle_get_media_type(
     device_handle_t *device_handle,
     uint8_t *media_type,
     libcerror_error_t **error );

int device_handle_get_bytes_per_sector(
     device_handle_t *device_handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error );

int device_handle_get_information_value(
     device_handle_t *device_handle,
     const uint8_t *information_value_identifier,
     size_t information_value_identifier_length,
     libcstring_system_character_t *information_value,
     size_t information_value_size,
     libcerror_error_t **error );

int device_handle_get_number_of_sessions(
     device_handle_t *device_handle,
     int *number_of_sessions,
     libcerror_error_t **error );

int device_handle_get_session(
     device_handle_t *device_handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

int device_handle_get_number_of_tracks(
     device_handle_t *device_handle,
     int *number_of_tracks,
     libcerror_error_t **error );

int device_handle_get_track(
     device_handle_t *device_handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     uint8_t *type,
     libcerror_error_t **error );

int device_handle_set_string(
     device_handle_t *device_handle,
     const libcstring_system_character_t *string,
     libcstring_system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error );

int device_handle_set_number_of_error_retries(
     device_handle_t *device_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int device_handle_set_error_values(
     device_handle_t *device_handle,
     size_t error_granularity,
     libcerror_error_t **error );

int device_handle_get_number_of_read_errors(
     device_handle_t *device_handle,
     int *number_of_errors,
     libcerror_error_t **error );

int device_handle_get_read_error(
     device_handle_t *device_handle,
     int index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error );

int device_handle_media_information_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     libcerror_error_t **error );

int device_handle_read_errors_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     libcerror_error_t **error );

int device_handle_sessions_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     libcerror_error_t **error );

int device_handle_tracks_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

