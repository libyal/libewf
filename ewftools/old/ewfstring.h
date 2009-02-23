/*
 * String functions for the ewftools
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

#if !defined( _EWFSTRING_H )
#define _EWFSTRING_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "character_string.h"
#include "system_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The length of hash strings
 */
#define EWFSTRING_DIGEST_HASH_LENGTH_MD5	33
#define EWFSTRING_DIGEST_HASH_LENGTH_SHA1	41

int ewfstring_copy_system_string_to_character_string(
     character_t *destination,
     const system_character_t *source,
     size_t length );

int ewfstring_copy_character_string_to_system_string(
     system_character_t *destination,
     const character_t *source,
     size_t length );

#if defined( __cplusplus )
}
#endif

#endif

