/*
 * Date and time values functions
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

#if !defined( _LIBEWF_DATE_TIME_VALUES_H )
#define _LIBEWF_DATE_TIME_VALUES_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libewf_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_date_time_values_copy_from_timestamp(
     libewf_character_t *date_time_values_string,
     size_t date_time_values_string_size,
     time_t timestamp,
     liberror_error_t **error );

int libewf_date_time_values_copy_to_timestamp(
     libewf_character_t *date_time_values_string,
     size_t date_time_values_string_length,
     time_t *timestamp,
     liberror_error_t **error );

int libewf_date_time_values_copy_to_string(
     libewf_character_t *date_time_values_string,
     size_t date_time_values_string_length,
     int date_format,
     libewf_character_t *string,
     size_t string_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

