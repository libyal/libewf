/*
 * Header values functions
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

#include "libewf_string.h"
#include "libewf_values_table.h"

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
	LIBEWF_HEADER_VALUES_INDEX_PROCESS_IDENTIFIER,
	LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC,
	LIBEWF_HEADER_VALUES_INDEX_EXTENTS,

	/* Value to indicate the amount of header values
	 */
	LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT
};

int libewf_header_values_initialize(
     libewf_values_table_t **header_values,
     liberror_error_t **error );

int libewf_header_values_realloc(
     libewf_values_table_t *header_values,
     uint32_t previous_amount,
     uint32_t new_amount,
     liberror_error_t **error );

void libewf_header_values_free(
      libewf_values_table_t *header_values );

int libewf_convert_date_header_value(
     libewf_character_t *header_value,
     size_t header_value_length,
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error );

int libewf_generate_date_header_value(
     time_t timestamp,
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error );

int libewf_convert_date_header2_value(
     libewf_character_t *header_value,
     size_t header_value_length,
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error );

int libewf_generate_date_header2_value(
     time_t timestamp,
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error );

int libewf_header_values_copy(
     libewf_values_table_t *destination_header_values,
     libewf_values_table_t *source_header_values,
     liberror_error_t **error );

int libewf_header_values_parse_header_string(
     libewf_values_table_t *header_values,
     libewf_character_t *header_string,
     size_t header_string_size,
     liberror_error_t **error );

int libewf_header_values_parse_header(
     libewf_values_table_t *header_values,
     uint8_t *header,
     size_t header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_parse_header2(
     libewf_values_table_t *header_values,
     uint8_t *header2,
     size_t header2_size,
     liberror_error_t **error );

int libewf_header_values_convert_header_string_to_header(
     libewf_character_t *header_string,
     size_t header_string_size,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_convert_header_string_to_header2(
     libewf_character_t *header_string,
     size_t header_string_size,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_string_type1(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_string_type2(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_string_type3(
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_string_type4(
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_string_type5(
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_string_type6(
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_string_type7(
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_ewf(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_generate_header_encase1(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_generate_header_ftk(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_generate_header_encase2(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_generate_header_encase4(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_generate_header_linen5(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_generate_header_linen6(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_generate_header2_encase4(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error );

int libewf_header_values_generate_header2_encase5(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error );

int libewf_header_values_generate_header2_encase6(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error );

int libewf_convert_date_xheader_value(
     libewf_character_t *header_value,
     size_t header_value_length,
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error );

int libewf_generate_date_xheader_value(
     time_t timestamp,
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error );

int libewf_header_values_parse_header_string_xml(
     libewf_values_table_t *header_values,
     libewf_character_t *header_string_xml,
     size_t header_string_xml_size,
     liberror_error_t **error );

int libewf_header_values_parse_xheader(
     libewf_values_table_t *header_values,
     uint8_t *xheader,
     size_t size,
     liberror_error_t **error );

int libewf_header_values_convert_header_string_xml_to_xheader(
     libewf_character_t *header_string_xml,
     size_t header_string_xml_size,
     uint8_t **xheader,
     size_t *xheader_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_string_xml(
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error );

int libewf_header_values_generate_header_ewfx(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error );

int libewf_header_values_generate_header2_ewfx(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error );

int libewf_header_values_generate_xheader_ewfx(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **xheader,
     size_t *xheader_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

