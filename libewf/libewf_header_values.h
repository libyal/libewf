/*
 * Header values functions
 *
 * Copyright (c) 2006-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_HEADER_VALUES_H )
#define _LIBEWF_HEADER_VALUES_H

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

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBEWF_HEADER_VALUES_INDEXES
{
	LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER,
	LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION,
	LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME,
	LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER,
	LIBEWF_HEADER_VALUES_INDEX_NOTES,
	LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE,
	LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE,
	LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM,
	LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION,
	LIBEWF_HEADER_VALUES_INDEX_PASSWORD,
	LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE,
	LIBEWF_HEADER_VALUES_INDEX_MODEL,
	LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER,
	LIBEWF_HEADER_VALUES_INDEX_DEVICE_LABEL,
	LIBEWF_HEADER_VALUES_INDEX_PROCESS_IDENTIFIER,
	LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC,
	LIBEWF_HEADER_VALUES_INDEX_EXTENTS,

	/* Value to indicate the default number of header values
	 */
	LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT
};

enum LIBEWF_HEADER_STRING_TYPE
{
	/* header string: original EWF, EnCase1
	 */
	LIBEWF_HEADER_STRING_TYPE_1	= 1,

	/* header string: EnCase2, EnCase3, FTK Imager 2
	 */
	LIBEWF_HEADER_STRING_TYPE_2	= 2,

	/* header string: EnCase4, EnCase5
	 */
	LIBEWF_HEADER_STRING_TYPE_3	= 3,

	/* header2 string: EnCase4
	 */
	LIBEWF_HEADER_STRING_TYPE_4	= 4,

	/* header2 string: EnCase5
	 */
	LIBEWF_HEADER_STRING_TYPE_5	= 5,

	/* header string: linen6
	 * header2 string: EnCase6
	 */
	LIBEWF_HEADER_STRING_TYPE_6	= 6,

	/* header string: linen5
	 */
	LIBEWF_HEADER_STRING_TYPE_7	= 7,

	/* header string: linen7
	 * header2 string: EnCase7
	 */
	LIBEWF_HEADER_STRING_TYPE_8	= 8
};

int libewf_header_values_initialize(
     libfvalue_table_t **header_values,
     libcerror_error_t **error );

int libewf_header_values_set_value_by_index(
     libfvalue_table_t *header_values,
     int value_index,
     const uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

int libewf_convert_date_header_value(
     const uint8_t *header_value,
     size_t header_value_size,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error );

int libewf_generate_date_header_value(
     time_t timestamp,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error );

int libewf_convert_date_header2_value(
     const uint8_t *header_value,
     size_t header_value_size,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error );

int libewf_generate_date_header2_value(
     time_t timestamp,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error );

int libewf_header_values_copy(
     libfvalue_table_t *destination_header_values,
     libfvalue_table_t *source_header_values,
     libcerror_error_t **error );

int libewf_header_values_parse_utf8_header_string(
     libfvalue_table_t *header_values,
     const uint8_t *header_string,
     size_t header_string_size,
     uint8_t header_section_number,
     uint8_t *format,
     libcerror_error_t **error );

int libewf_header_values_parse_header(
     libfvalue_table_t *header_values,
     uint8_t *header,
     size_t header_size,
     int codepage,
     uint8_t *format,
     libcerror_error_t **error );

int libewf_header_values_parse_header2(
     libfvalue_table_t *header_values,
     uint8_t *header2,
     size_t header2_size,
     uint8_t *format,
     libcerror_error_t **error );

int libewf_header_values_convert_utf8_header_string_to_header(
     const uint8_t *header_string,
     size_t header_string_size,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error );

int libewf_header_values_generate_utf8_header_string(
     libfvalue_table_t *header_values,
     uint8_t header_string_type,
     uint8_t *newline_string,
     size_t newline_string_length,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_header_values_generate_header_ewf(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error );

int libewf_header_values_generate_header_encase1(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error );

int libewf_header_values_generate_header_ftk(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error );

int libewf_header_values_generate_header_encase2(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error );

int libewf_header_values_generate_header_encase4(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error );

int libewf_header_values_generate_header_linen(
     libfvalue_table_t *header_values,
     uint8_t format,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error );

int libewf_header_values_generate_header2(
     libfvalue_table_t *header_values,
     uint8_t format,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header2,
     size_t *header2_size,
     libcerror_error_t **error );

int libewf_convert_date_xheader_value(
     const uint8_t *header_value,
     size_t header_value_length,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error );

int libewf_generate_date_xheader_value(
     time_t timestamp,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error );

int libewf_header_values_parse_xheader(
     libfvalue_table_t *header_values,
     const uint8_t *xheader,
     size_t size,
     libcerror_error_t **error );

int libewf_header_values_parse_xheader_date_value(
     libfvalue_table_t *header_values,
     const uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

int libewf_header_values_generate_xheader(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **xheader,
     size_t *xheader_size,
     libcerror_error_t **error );

int libewf_header_values_generate_header_ewfx(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

