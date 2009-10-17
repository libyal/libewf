/*
 * Debugging functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBEWF_DEBUG_H )
#define _LIBEWF_DEBUG_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#include "libewf_string.h"

#include "ewf_section.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_debug_dump_data(
     libewf_character_t *header,
     uint8_t *data,
     size_t data_size,
     liberror_error_t **error );

int libewf_debug_section_print(
     ewf_section_t *section,
     liberror_error_t **error );

int libewf_debug_byte_stream_print(
     libewf_character_t *header,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     liberror_error_t **error );

int libewf_debug_utf8_stream_print(
     libewf_character_t *header,
     uint8_t *utf8_stream,
     size_t utf8_stream_size,
     liberror_error_t **error );

int libewf_debug_utf16_stream_print(
     libewf_character_t *header,
     uint8_t *utf16_stream,
     size_t utf16_stream_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

