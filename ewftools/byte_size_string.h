/*
 * Byte size string functions
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

#if !defined( _BYTE_SIZE_STRING_H )
#define _BYTE_SIZE_STRING_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"
#include "ewftools_libcstring.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum BYTE_SIZE_STRING_UNITS
{
	BYTE_SIZE_STRING_UNIT_MEGABYTE	= 1000,
	BYTE_SIZE_STRING_UNIT_MEBIBYTE	= 1024
};

int byte_size_string_create(
     libcstring_system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t size,
     int units,
     libcerror_error_t **error );

int byte_size_string_convert(
     const libcstring_system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

