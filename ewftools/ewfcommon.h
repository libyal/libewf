/*
 * Common functions for the ewftools
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

#if !defined( _EWFCOMMON_H )
#define _EWFCOMMON_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#include <libewf/definitions.h>

#include "error_string.h"
#include "system_string.h"

#define EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE		LIBEWF_DEFAULT_SEGMENT_FILE_SIZE
#define EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE		( 1024 * 1024 )
#define EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT	INT32_MAX
#define EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT	INT64_MAX

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#define ewfcommon_strerror( error_number ) \
        error_string_wcserror( error_number )

#else
#define ewfcommon_strerror( error_number ) \
        error_string_strerror( error_number )

#endif

int ewfcommon_swap_byte_pairs(
     uint8_t *buffer,
     size_t size );

int ewfcommon_determine_operating_system_string(
     system_character_t *operating_system_string,
     size_t operating_system_string_size,
     liberror_error_t **error );

int ewfcommon_determine_guid(
     uint8_t *guid,
     uint8_t libewf_format,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

