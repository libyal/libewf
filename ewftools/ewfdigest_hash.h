/*
 * Crypographic hash for ewftools
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

#if !defined( _EWFDIGEST_HASH_H )
#define _EWFDIGEST_HASH_H

#include <common.h>
#include <types.h>

#include "character_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define EWFDIGEST_HASH_SIZE_MD5		(size_t) ( sizeof( ewfdigest_hash_t ) * 16 )
#define EWFDIGEST_HASH_SIZE_SHA1	(size_t) ( sizeof( ewfdigest_hash_t ) * 20 )

typedef uint8_t ewfdigest_hash_t;

int ewfdigest_copy_to_string(
     ewfdigest_hash_t *digest_hash,
     size_t size_digest_hash,
     character_t *string,
     size_t size_string );

#if defined( __cplusplus )
}
#endif

#endif

