/*
 * Globbing functions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _EWFTOOLS_GLOB_H )
#define _EWFTOOLS_GLOB_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_GLOB_H )

typedef struct ewftools_glob ewftools_glob_t;

struct ewftools_glob
{
	/* The number of globs resolved
	 */
	int number_of_results;

	/* The resolved globs
	 */
	system_character_t **results;
};

int ewftools_glob_initialize(
     ewftools_glob_t **glob,
     libcerror_error_t **error );

int ewftools_glob_free(
     ewftools_glob_t **glob,
     libcerror_error_t **error );

int ewftools_glob_resize(
     ewftools_glob_t *glob,
     int new_number_of_results,
     libcerror_error_t **error );

int ewftools_glob_resolve(
     ewftools_glob_t *glob,
     system_character_t * const patterns[],
     int number_of_patterns,
     libcerror_error_t **error );

int ewftools_glob_get_results(
     ewftools_glob_t *glob,
     int *number_of_results,
     system_character_t ***results,
     libcerror_error_t **error );

#endif /* !defined( HAVE_GLOB_H ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWFTOOLS_GLOB_H ) */

