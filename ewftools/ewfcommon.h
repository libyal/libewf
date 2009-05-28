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

#define EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE		LIBEWF_DEFAULT_SEGMENT_FILE_SIZE
#define EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE		( 1024 * 1024 )
#define EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT	INT32_MAX
#define EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT	INT64_MAX

/* This definition is intended for automated testing of variable process buffer sizes
 */
#if !defined( EWFCOMMON_PROCESS_BUFFER_SIZE )
#define EWFCOMMON_PROCESS_BUFFER_SIZE			0
#endif

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

#if defined( __cplusplus )
}
#endif

#endif

