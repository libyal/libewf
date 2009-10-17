/*
 * Values table functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBEWF_VALUES_TABLE_H )
#define _LIBEWF_VALUES_TABLE_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libewf_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_values_table libewf_values_table_t;

struct libewf_values_table
{
	/* The amount of values
	 */
	int amount_of_values;

	/* The value identifiers
	 */
	uint8_t **identifier;

	/* The value identifier lengths
	 */
	size_t *identifier_length;

	/* The values
	 */
	uint8_t **value;

	/* The value lengths
	 */
	size_t *value_length;
};

int libewf_values_table_initialize(
     libewf_values_table_t **values_table,
     int amount_of_values,
     liberror_error_t **error );

int libewf_values_table_free(
     libewf_values_table_t **values_table,
     liberror_error_t **error );

int libewf_values_table_resize(
     libewf_values_table_t *values_table,
     int amount_of_values,
     liberror_error_t **error );

int libewf_values_table_get_amount_of_values(
     libewf_values_table_t *values_table,
     int *amount_of_values,
     liberror_error_t **error );

int libewf_values_table_get_index(
     libewf_values_table_t *values_table,
     const uint8_t *identifier,
     size_t identifier_length,
     int *index,
     liberror_error_t **error );

int libewf_values_table_get_identifier_size(
     libewf_values_table_t *values_table,
     int index,
     size_t *identifier_size,
     liberror_error_t **error );

int libewf_values_table_get_identifier(
     libewf_values_table_t *values_table,
     int index,
     uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error );

int libewf_values_table_set_identifier(
     libewf_values_table_t *values_table,
     int index,
     const uint8_t *identifier,
     size_t indentifier_length,
     liberror_error_t **error );

int libewf_values_table_get_value_size(
     libewf_values_table_t *values_table,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     liberror_error_t **error );

int libewf_values_table_get_value(
     libewf_values_table_t *values_table,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     liberror_error_t **error );

int libewf_values_table_set_value(
     libewf_values_table_t *values_table,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

