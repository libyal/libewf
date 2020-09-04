/*
 * Case data functions
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

#if !defined( _LIBEWF_CASE_DATA_H )
#define _LIBEWF_CASE_DATA_H

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
#include "libewf_libfvalue.h"
#include "libewf_media_values.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_case_data_generate_utf8_string(
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t format,
     libcerror_error_t **error );

int libewf_case_data_generate(
     uint8_t **case_data,
     size_t *case_data_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t format,
     libcerror_error_t **error );

int libewf_case_data_parse_utf8_string(
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     uint8_t *format,
     libcerror_error_t **error );

int libewf_case_data_parse_utf8_string_value(
     uint8_t *type_string,
     size_t type_string_size,
     uint8_t *value_string,
     size_t value_string_size,
     int value_index,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     libcerror_error_t **error );

int libewf_case_data_parse(
     const uint8_t *case_data,
     size_t case_data_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     uint8_t *format,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_CASE_DATA_H ) */

