/*
 * File writing functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _LIBEWF_WRITE_H )
#define _LIBEWF_WRITE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <libewf/handle.h>

#include "libewf_chunk_cache.h"
#include "libewf_extern.h"
#include "libewf_handle.h"
#include "libewf_media_values.h"
#include "libewf_segment_file_handle.h"

#include "ewf_crc.h"

#if defined( __cplusplus )
extern "C" {
#endif

ssize_t libewf_write_chunk_data_new(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         int8_t force_write,
         liberror_error_t **error );

ssize_t libewf_write_chunk_data_existing(
         libewf_internal_handle_t *internal_handle,
         uint32_t chunk,
         uint32_t chunk_offset,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

