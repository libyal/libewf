/*
 * System character string functions
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

#if !defined( _EWFTOOLS_SYSTEM_STRING_H )
#define _EWFTOOLS_SYSTEM_STRING_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int ewftools_system_string_decimal_copy_to_64_bit(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error );

int ewftools_string_size_to_utf8_string(
     const system_character_t *string,
     size_t string_size,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int ewftools_string_copy_to_utf8_string(
     const system_character_t *string,
     size_t string_size,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWFTOOLS_SYSTEM_STRING_H ) */

