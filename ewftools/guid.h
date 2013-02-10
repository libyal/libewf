/*
 * GUID functions
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

#if !defined( _GUID_H )
#define _GUID_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"
#include "ewftools_libcstring.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define GUID_SIZE		16
#define GUID_STRING_SIZE	37

enum GUID_TYPES
{
	GUID_TYPE_RANDOM	= (uint8_t) 'r',
	GUID_TYPE_TIME		= (uint8_t) 't'
};

#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )
int guid_generate(
     uint8_t *guid,
     size_t guid_size,
     uint8_t guid_type,
     libcerror_error_t **error );
#endif

int guid_to_string(
     uint8_t *guid,
     size_t guid_size,
     int byte_order,
     libcstring_system_character_t *string,
     size_t string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

