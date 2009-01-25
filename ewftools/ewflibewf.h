/*
 * Libewf function wrapper for the ewftools
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _EWFLIBEWF_H )
#define _EWFLIBEWF_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <libewf/handle.h>

#include "character_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

int ewflibewf_get_header_value(
     libewf_handle_t *handle,
     const char *utf8_header_value_identifier,
     size_t utf8_header_value_identifier_length,
     character_t *header_value,
     size_t header_value_size,
     liberror_error_t **error );

int ewflibewf_set_header_value(
     libewf_handle_t *handle,
     const char *header_value_identifier,
     size_t utf8_header_value_identifier_length,
     character_t *header_value,
     liberror_error_t **error );

int ewflibewf_get_hash_value(
     libewf_handle_t *handle,
     const char *utf8_hash_value_identifier,
     size_t utf8_header_value_identifier_length,
     character_t *hash_value,
     size_t hash_value_size,
     liberror_error_t **error );

int ewflibewf_set_hash_value(
     libewf_handle_t *handle,
     const char *hash_value_identifier,
     size_t utf8_header_value_identifier_length,
     character_t *hash_value,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

