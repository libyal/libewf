/*
 * GUID functions
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

#if !defined( _GUID_H )
#define _GUID_H

#include <common.h>
#include <types.h>

#include "character_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define GUID_STRING_LENGTH	37

typedef uint8_t guid_t;

int guid_to_string(
     guid_t *guid,
     character_t *string,
     size_t length );

#if defined( __cplusplus )
}
#endif

#endif

