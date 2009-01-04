/*
 * Globbing functions for the ewftools
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

#if !defined( _EWFGLOB_H )
#define _EWFGLOB_H

#include <common.h>
#include <system_string.h>

#include <libewf/types.h>

#include "ewfcommon.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_GLOB_H )

typedef struct ewfglob ewfglob_t;

struct ewfglob
{
	/* The amount of globs resolved
	 */
	uint16_t amount;

	/* The resolved globs
	 */
	system_character_t **results;
};

ewfglob_t *ewfglob_alloc(
            void );

ewfglob_t *ewfglob_realloc(
            ewfglob_t *glob,
            uint16_t new_amount );

void ewfglob_free(
      ewfglob_t *glob );

int32_t ewfglob_resolve(
         ewfglob_t *glob,
         system_character_t * const patterns[],
         uint32_t amount_of_patterns );

#endif

#if defined( __cplusplus )
}
#endif

#endif

