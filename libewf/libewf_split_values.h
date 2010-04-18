/*
 * Split string values functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (c) 2006-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBEWF_SPLIT_VALUES_H )
#define _LIBEWF_SPLIT_VALUES_H

#include <common.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_split_values libewf_split_values_t;

struct libewf_split_values
{
	/* The amount of split values
	 */
	int amount_of_values;

	/* The string
	 */
	libcstring_character_t *string;

	/* The values
	 */
	libcstring_character_t **values;

	/* The sizes of the values
	 */
	size_t *sizes;
};

int libewf_split_values_initialize(
     libewf_split_values_t **split_values,
     const libcstring_character_t *string,
     size_t string_size,
     int amount_of_values,
     liberror_error_t **error );

int libewf_split_values_free(
     libewf_split_values_t **split_values,
     liberror_error_t **error );

int libewf_split_values_parse_string(
     libewf_split_values_t **split_values,
     const libcstring_character_t *string,
     size_t string_size,
     libcstring_character_t delimiter,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

