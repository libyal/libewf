/*
 * Character type string functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBEWF_STRING_H )
#define _LIBEWF_STRING_H

#include <common.h>
#include <character_string.h>
#include <types.h>

#include <stdio.h>

#include "ewf_char.h"
#include "ewf_digest_hash.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_STRING_LITTLE_ENDIAN	(uint8_t) 'l'
#define LIBEWF_STRING_BIG_ENDIAN	(uint8_t) 'b'

int libewf_string_split(
     character_t *string,
     size_t size,
     character_t delimiter,
     character_t ***split_values,
     size_t *amount_of_split_values );

int libewf_string_split_values_free(
      character_t **split_values,
      size_t amount_of_split_values );

int libewf_string_copy_from_utf16(
     character_t *string,
     size_t size_string,
     ewf_char_t *utf16_string,
     size_t size_utf16 );

int libewf_string_copy_to_utf16(
     character_t *string,
     size_t size,
     ewf_char_t *utf16_string,
     size_t size_utf16,
     uint8_t byte_order );

#if defined( __cplusplus )
}
#endif

#endif

