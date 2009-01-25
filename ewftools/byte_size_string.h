/*
 * Byte size string functions
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

#if !defined( _BYTE_SIZE_STRING_H )
#define _BYTE_SIZE_STRING_H

#include <common.h>
#include <types.h>

#include "character_string.h"
#include "system_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define BYTE_SIZE_STRING_UNIT_MEGABYTE		1000
#define BYTE_SIZE_STRING_UNIT_MEBIBYTE		1024

const character_t *byte_size_string_get_factor_string(
                    int8_t factor );

int8_t byte_size_string_get_factor(
        character_t factor );

int byte_size_string_create(
     character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t size,
     int units );

int byte_size_string_convert(
     const character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t *size );

int8_t byte_size_string_get_factor_system_character(
        system_character_t factor );

int byte_size_string_convert_system_character(
     const system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t *size );

#if defined( __cplusplus )
}
#endif

#endif

