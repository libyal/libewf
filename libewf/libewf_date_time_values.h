/*
 * Date and time values functions
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

#if !defined( _LIBEWF_DATE_TIME_VALUES_H )
#define _LIBEWF_DATE_TIME_VALUES_H

#include <common.h>
#include <types.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_date_time_values_copy_from_timestamp(
     uint8_t *date_time_values_string,
     size_t date_time_values_string_size,
     time_t timestamp,
     libcerror_error_t **error );

int libewf_date_time_values_copy_to_timestamp(
     const uint8_t *date_time_values_string,
     size_t date_time_values_string_size,
     time_t *timestamp,
     libcerror_error_t **error );

int libewf_utf8_string_day_of_week_copy_from_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf8_string_day_of_week_copy_to_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf8_string_day_of_month_copy_to_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf8_string_month_copy_from_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf8_string_month_copy_to_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf8_string_year_copy_to_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf8_string_hours_copy_to_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf8_string_minutes_copy_to_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf8_string_seconds_copy_to_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf8_string_time_copy_from_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf16_string_day_of_week_copy_from_time_elements(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf16_string_month_copy_from_time_elements(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_utf16_string_time_copy_from_time_elements(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     struct tm *time_elements,
     libcerror_error_t **error );

int libewf_date_time_values_copy_to_utf8_string(
     const uint8_t *date_time_values_string,
     size_t date_time_values_string_size,
     int date_format,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_date_time_values_copy_to_utf16_string(
     const uint8_t *date_time_values_string,
     size_t date_time_values_string_size,
     int date_format,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_DATE_TIME_VALUES_H ) */

