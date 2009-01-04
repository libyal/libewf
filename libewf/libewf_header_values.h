/*
 * libewf header values
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined( _LIBEWF_HEADER_VALUES_H )
#define _LIBEWF_HEADER_VALUES_H

#include <common.h>
#include <character_string.h>

#include <libewf/types.h>

#include "libewf_values_table.h"

#include <time.h>

#include "ewf_char.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT			14

#define LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER			0
#define LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION			1
#define LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME		2
#define LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER		3
#define LIBEWF_HEADER_VALUES_INDEX_NOTES			4
#define LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE			5
#define LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE			6
#define LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM	7
#define LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION	8
#define LIBEWF_HEADER_VALUES_INDEX_PASSWORD			9
#define LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE		10
#define LIBEWF_HEADER_VALUES_INDEX_MODEL			11
#define LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER		12
#define LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC			13

int libewf_header_values_initialize(
     libewf_values_table_t *header_values );

int libewf_header_values_realloc(
     libewf_values_table_t *header_values,
     uint32_t previous_amount,
     uint32_t new_amount );

void libewf_header_values_free(
      libewf_values_table_t *header_values );

int libewf_convert_timestamp(
     time_t timestamp,
     uint8_t date_format,
     character_t **date_string,
     size_t *date_string_length );

int libewf_convert_date_header_value(
     character_t *header_value,
     size_t header_value_length,
     uint8_t date_format,
     character_t **date_string,
     size_t *date_string_length );

int libewf_generate_date_header_value(
     time_t timestamp,
     character_t **date_string,
     size_t *date_string_length );

int libewf_convert_date_header2_value(
     character_t *header_value,
     size_t header_value_length,
     uint8_t date_format,
     character_t **date_string,
     size_t *date_string_length );

int libewf_generate_date_header2_value(
     time_t timestamp,
     character_t **date_string,
     size_t *date_string_length );

int libewf_header_values_copy(
     libewf_values_table_t *destination_header_values,
     libewf_values_table_t *source_header_values );

int libewf_header_values_parse_header_string(
     libewf_values_table_t **header_values,
     character_t *header_string,
     size_t length,
     uint8_t date_format );

int libewf_header_values_parse_header(
     libewf_values_table_t **header_values,
     ewf_char_t *header,
     size_t size,
     uint8_t date_format );

int libewf_header_values_parse_header2(
     libewf_values_table_t **header_values,
     ewf_char_t *header2,
     size_t size,
     uint8_t date_format );

int libewf_header_values_convert_header_string_to_header(
     character_t *header_string,
     size_t header_string_length,
     ewf_char_t **header,
     size_t *header_length );

int libewf_header_values_convert_header_string_to_header2(
     character_t *header_string,
     size_t header_string_length,
     ewf_char_t **header2,
     size_t *header2_length );

int libewf_header_values_generate_header_string_type1(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_length );

int libewf_header_values_generate_header_string_type2(
     libewf_values_table_t *header_values,
     time_t timestamp, 
     int8_t compression_level,
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_length );

int libewf_header_values_generate_header_string_type3(
     libewf_values_table_t *header_values,
     time_t timestamp,
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_length );

int libewf_header_values_generate_header_string_type4(
     libewf_values_table_t *header_values,
     time_t timestamp,
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_length );

int libewf_header_values_generate_header_string_type5(
     libewf_values_table_t *header_values,
     time_t timestamp,
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_length );

int libewf_header_values_generate_header_string_type6(
     libewf_values_table_t *header_values,
     time_t timestamp,
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_length );

int libewf_header_values_generate_header_string_type7(
     libewf_values_table_t *header_values,
     time_t timestamp,
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_length );

int libewf_header_values_generate_header_ewf(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     ewf_char_t **header,
     size_t *header_length );

int libewf_header_values_generate_header_encase1(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     ewf_char_t **header,
     size_t *header_length );

int libewf_header_values_generate_header_ftk(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     ewf_char_t **header,
     size_t *header_length );

int libewf_header_values_generate_header_encase2(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     ewf_char_t **header,
     size_t *header_length );

int libewf_header_values_generate_header_encase4(
     libewf_values_table_t *header_values,
     time_t timestamp,
     ewf_char_t **header,
     size_t *header_length );

int libewf_header_values_generate_header_linen5(
     libewf_values_table_t *header_values,
     time_t timestamp,
     ewf_char_t **header,
     size_t *header_length );

int libewf_header_values_generate_header_linen6(
     libewf_values_table_t *header_values,
     time_t timestamp,
     ewf_char_t **header,
     size_t *header_length );

int libewf_header_values_generate_header2_encase4(
     libewf_values_table_t *header_values,
     time_t timestamp,
     ewf_char_t **header2,
     size_t *header2_length );

int libewf_header_values_generate_header2_encase5(
     libewf_values_table_t *header_values,
     time_t timestamp,
     ewf_char_t **header2,
     size_t *header2_length );

int libewf_header_values_generate_header2_encase6(
     libewf_values_table_t *header_values,
     time_t timestamp,
     ewf_char_t **header2,
     size_t *header2_length );

int libewf_convert_date_xheader_value(
     character_t *header_value,
     size_t header_value_length,
     uint8_t date_format,
     character_t **date_string,
     size_t *date_string_length );

int libewf_generate_date_xheader_value(
     time_t timestamp,
     character_t **date_string,
     size_t *date_string_length );

int libewf_header_values_parse_header_string_xml(
     libewf_values_table_t **header_values,
     character_t *header_string_xml,
     size_t length,
     uint8_t date_format );

int libewf_header_values_parse_xheader(
     libewf_values_table_t **header_values,
     ewf_char_t *xheader,
     size_t size,
     uint8_t date_format );

int libewf_header_values_generate_header_string_xml(
     libewf_values_table_t *header_values,
     time_t timestamp,
     character_t **header_string,
     size_t *header_string_length );

int libewf_header_values_generate_header_ewfx(
     libewf_values_table_t *header_values,
     time_t timestamp,
     ewf_char_t **header,
     size_t *header_length );

int libewf_header_values_generate_header2_ewfx(
     libewf_values_table_t *header_values,
     time_t timestamp,
     ewf_char_t **header2,
     size_t *header2_length );

int libewf_header_values_generate_xheader_ewfx(
     libewf_values_table_t *header_values,
     time_t timestamp,
     ewf_char_t **xheader,
     size_t *xheader_length );

#if defined( __cplusplus )
}
#endif

#endif

