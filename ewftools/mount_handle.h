/* 
 * Mount handle
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

#if !defined( _MOUNT_HANDLE_H )
#define _MOUNT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "ewftools_libcerror.h"
#include "ewftools_libcstring.h"
#include "ewftools_libewf.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum MOUNT_HANDLE_INPUT_FORMATS
{
	MOUNT_HANDLE_INPUT_FORMAT_FILES	= (int) 'f',
	MOUNT_HANDLE_INPUT_FORMAT_RAW	= (int) 'r'
};

typedef struct mount_handle mount_handle_t;

struct mount_handle
{
	/* The input format
	 */
	uint8_t input_format;

	/* The libewf input handle
	 */
	libewf_handle_t *input_handle;

	/* The libewf root file entry
	 */
	libewf_file_entry_t *root_file_entry;

	/* The nofication output stream
	 */
	FILE *notify_stream;
};

int mount_handle_initialize(
     mount_handle_t **mount_handle,
     libcerror_error_t **error );

int mount_handle_free(
     mount_handle_t **mount_handle,
     libcerror_error_t **error );

int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     libcerror_error_t **error );

int mount_handle_set_maximum_number_of_open_handles(
     mount_handle_t *mount_handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error );

int mount_handle_set_format(
     mount_handle_t *mount_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int mount_handle_open_input(
     mount_handle_t *mount_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error );

int mount_handle_close(
     mount_handle_t *mount_handle,
     libcerror_error_t **error );

ssize_t mount_handle_read_buffer(
         mount_handle_t *mount_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

off64_t mount_handle_seek_offset(
         mount_handle_t *mount_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

int mount_handle_get_media_size(
     mount_handle_t *mount_handle,
     size64_t *size,
     libcerror_error_t **error );

int mount_handle_get_file_entry_by_path(
     mount_handle_t *mount_handle,
     const libcstring_system_character_t *path,
     size_t path_length,
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

