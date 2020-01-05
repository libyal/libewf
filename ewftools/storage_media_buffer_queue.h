/*
 * Storage media buffer queue
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

#if !defined( _STORAGE_MEDIA_BUFFER_QUEUE_H )
#define _STORAGE_MEDIA_BUFFER_QUEUE_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"
#include "ewftools_libcthreads.h"
#include "ewftools_libewf.h"
#include "storage_media_buffer.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_MULTI_THREAD_SUPPORT )

int storage_media_buffer_queue_initialize(
     libcthreads_queue_t **queue,
     libewf_handle_t *handle,
     int maximum_number_of_values,
     uint8_t storage_media_buffer_mode,
     size_t storage_media_buffer_size,
     libcerror_error_t **error );

int storage_media_buffer_queue_free(
     libcthreads_queue_t **queue,
     libcerror_error_t **error );

int storage_media_buffer_queue_grab_buffer(
     libcthreads_queue_t *queue,
     storage_media_buffer_t **buffer,
     libcerror_error_t **error );

int storage_media_buffer_queue_release_buffer(
     libcthreads_queue_t *queue,
     storage_media_buffer_t *buffer,
     libcerror_error_t **error );

#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _STORAGE_MEDIA_BUFFER_QUEUE_H ) */

