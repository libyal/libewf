/* 
 * Verification handle
 *
 * Copyright (C) 2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _VERIFICATION_HANDLE_H )
#define _VERIFICATION_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_EXPORT
#endif

#include <libewf.h>

#include "digest_context.h"
#include "digest_hash.h"
#include "md5.h"
#include "sha1.h"
#include "storage_media_buffer.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct verification_handle verification_handle_t;

struct verification_handle
{
	/* The MD5 digest context
	 */
	md5_context_t md5_context;

	/* The SHA1 digest context
	 */
	sha1_context_t sha1_context;

	/* The libewf input handle
	 */
	libewf_handle_t *input_handle;

	/* The last offset of the input data
	 */
	off64_t input_offset;

	/* The last offset of the output data
	 */
	off64_t output_offset;
};

int verification_handle_initialize(
     verification_handle_t **verification_handle,
     liberror_error_t **error );

int verification_handle_free(
     verification_handle_t **verification_handle,
     liberror_error_t **error );

int verification_handle_open_input(
     verification_handle_t *verification_handle,
     const char *filename,
     size_t length_filename,
     liberror_error_t **error );

#if defined( HAVE_RAW_ACCESS )
ssize_t verification_handle_read_prepare_buffer(
         verification_handle_t *verification_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error );
#endif

ssize_t verification_handle_read_buffer(
         verification_handle_t *verification_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error );

int verification_handle_update_integrity_hash(
     verification_handle_t *verification_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error );

int verification_handle_close(
     verification_handle_t *verification_handle,
     liberror_error_t **error );

int verification_handle_get_values(
     verification_handle_t *verification_handle,
     size64_t *media_size,
     uint32_t *chunk_size,
     liberror_error_t **error );

int verification_handle_set_input_values(
     verification_handle_t *verification_handle,
     int format,
     liberror_error_t **error );

int verification_handle_finalize(
     verification_handle_t *verification_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

