/*
 * Globbing functions
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

#if !defined( _GLOB_H )
#define _GLOB_H

#include <common.h>
#include <types.h>

#include "system_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_GLOB_H )

#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )

#define glob_finddata_t	_wfinddata_t
#define glob_makepath	_wmakepath_s
#define glob_findfirst	_wfindfirst
#define glob_findnext	_wfindnext
#define glob_splitpath	_wsplitpath_s

#else

#define glob_finddata_t	_finddata_t
#define glob_makepath	_makepath_s
#define glob_findfirst	_findfirst
#define glob_findnext	_findnext
#define glob_splitpath	_splitpath_s

#endif

#define glob_findclose	_findclose

typedef struct glob glob_t;

struct glob
{
	/* The amount of globs resolved
	 */
	int amount_of_results;

	/* The resolved globs
	 */
	system_character_t **result;
};

int glob_initialize(
     glob_t **glob );

int glob_free(
     glob_t **glob );

int glob_resize(
     glob_t *glob,
     int new_amount_of_results );

int glob_resolve(
     glob_t *glob,
     system_character_t * const patterns[],
     int amount_of_patterns );

#endif

#if defined( __cplusplus )
}
#endif

#endif

