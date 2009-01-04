/*
 * GUID functions
 *
 * Copyright (c) 2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _EWFGUID_H )
#define _EWFGUID_H

#include <common.h>
#include <types.h>

#include "character_string.h"

#if defined( HAVE_TIME_H )
#include <time.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#define EWFGUID_STRING_LENGTH	37

typedef uint8_t ewfguid_t;

#define guid_t ewfguid_t;

int ewfguid_to_string(
     ewfguid_t *guid,
     character_t *string,
     size_t length );

#if defined( __cplusplus )
}
#endif

#endif

