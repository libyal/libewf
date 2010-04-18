/* 
 * Array type functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (C) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBEWF_ARRAY_TYPE_H )
#define _LIBEWF_ARRAY_TYPE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_array libewf_array_t;

struct libewf_array
{
	/* The amount of entries
	 */
	int amount_of_entries;

	/* The entries
	 */
	intptr_t **entry;
};

int libewf_array_initialize(
     libewf_array_t **array,
     int amount_of_entries,
     liberror_error_t **error );

int libewf_array_free(
     libewf_array_t **array,
     int (*entry_free_function)(
            intptr_t *entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libewf_array_resize(
     libewf_array_t *array,
     int amount_of_entries,
     liberror_error_t **error );

int libewf_array_get_amount_of_entries(
     libewf_array_t *array,
     int *amount_of_entries,
     liberror_error_t **error );

int libewf_array_get_entry(
     libewf_array_t *array,
     int entry_index,
     intptr_t **entry,
     liberror_error_t **error );

int libewf_array_set_entry(
     libewf_array_t *array,
     int entry_index,
     intptr_t *entry,
     liberror_error_t **error );

int libewf_array_append_entry(
     libewf_array_t *array,
     int *entry_index,
     intptr_t *entry,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

