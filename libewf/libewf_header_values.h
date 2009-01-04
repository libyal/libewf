/*
 * libewf header values
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#ifndef _LIBEWF_HEADERVALUES_H
#define _LIBEWF_HEADERVALUES_H

#include "libewf_includes.h"
#include "libewf_char.h"

#include <time.h>

#include "ewf_header.h"
#include "ewf_header2.h"

#ifdef __cplusplus
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

#define LIBEWF_HEADER_VALUES libewf_header_values_t
#define LIBEWF_HEADER_VALUES_SIZE sizeof( LIBEWF_HEADER_VALUES )

typedef struct libewf_header_values libewf_header_values_t;

struct libewf_header_values
{
	/* The amount of header values
	 */
	uint32_t amount;

	/* The header value identifiers
	 */
	LIBEWF_CHAR **identifiers;

	/* The header values
	 */
	LIBEWF_CHAR **values;
};

LIBEWF_HEADER_VALUES *libewf_header_values_alloc( void );
LIBEWF_HEADER_VALUES *libewf_header_values_realloc( LIBEWF_HEADER_VALUES *header_values, uint32_t previous_amount, uint32_t new_amount );
void libewf_header_values_free( LIBEWF_HEADER_VALUES *header_values );

int8_t libewf_date_string_set_year( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements );
int8_t libewf_date_string_set_2digit_value( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements, uint8_t index );
int8_t libewf_date_string_set_month( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements );
int8_t libewf_date_string_set_day_of_month( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements );
int8_t libewf_date_string_set_hours( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements );
int8_t libewf_date_string_set_minutes( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements );
int8_t libewf_date_string_set_seconds( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements );

LIBEWF_CHAR *libewf_convert_date_header_value( LIBEWF_CHAR *header_value, uint8_t date_format );
LIBEWF_CHAR *libewf_generate_date_header_value( time_t timestamp );
LIBEWF_CHAR *libewf_convert_date_header2_value( LIBEWF_CHAR *header_value, uint8_t date_format );
LIBEWF_CHAR *libewf_generate_date_header2_value( time_t timestamp );

int32_t libewf_header_values_get_index( LIBEWF_HEADER_VALUES *header_values, LIBEWF_CHAR *identifier );
int8_t libewf_header_values_get_value( LIBEWF_HEADER_VALUES *header_values, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );
int8_t libewf_header_values_set_value( LIBEWF_HEADER_VALUES *header_values, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );

LIBEWF_HEADER_VALUES *libewf_header_values_parse_header_string( LIBEWF_CHAR *header_string, size_t length, uint8_t date_format );
LIBEWF_HEADER_VALUES *libewf_header_values_parse_header( EWF_HEADER *header, size_t size, uint8_t date_format );
LIBEWF_HEADER_VALUES *libewf_header_values_parse_header2( EWF_HEADER2 *header2, size_t size, uint8_t date_format );

EWF_HEADER *libewf_header_values_convert_header_string_to_header( LIBEWF_CHAR *header_string, size_t string_length, size_t *header_length );
EWF_HEADER2 *libewf_header_values_convert_header_string_to_header2( LIBEWF_CHAR *header_string, size_t string_length, size_t *header2_length );

LIBEWF_CHAR *libewf_header_values_generate_header_string_type1( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length );
LIBEWF_CHAR *libewf_header_values_generate_header_string_type2( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length );
LIBEWF_CHAR *libewf_header_values_generate_header_string_type3( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length );
LIBEWF_CHAR *libewf_header_values_generate_header_string_type4( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length );
LIBEWF_CHAR *libewf_header_values_generate_header_string_type5( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length );
LIBEWF_CHAR *libewf_header_values_generate_header_string_type6( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length );
LIBEWF_CHAR *libewf_header_values_generate_header_string_type7( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length );

EWF_HEADER *libewf_header_values_generate_header_string_ewf( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, size_t *header_length );
EWF_HEADER *libewf_header_values_generate_header_string_encase1( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, size_t *header_length );
EWF_HEADER *libewf_header_values_generate_header_string_ftk( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, size_t *header_length );
EWF_HEADER *libewf_header_values_generate_header_string_encase2( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, size_t *header_length );
EWF_HEADER *libewf_header_values_generate_header_string_encase4( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length );
EWF_HEADER *libewf_header_values_generate_header_string_encase5_linen( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length );
EWF_HEADER *libewf_header_values_generate_header_string_encase6_linen( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length );

EWF_HEADER2 *libewf_header_values_generate_header2_string_encase4( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header2_length );
EWF_HEADER2 *libewf_header_values_generate_header2_string_encase5( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header2_length );
EWF_HEADER2 *libewf_header_values_generate_header2_string_encase6( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header2_length );

LIBEWF_CHAR *libewf_generate_date_xheader_value( time_t timestamp );

LIBEWF_HEADER_VALUES *libewf_header_values_parse_header_string_xml( LIBEWF_CHAR *header_string_xml, size_t length, uint8_t date_format );
LIBEWF_HEADER_VALUES *libewf_header_values_parse_xheader( EWF_HEADER *xheader, size_t size, uint8_t date_format );

LIBEWF_CHAR *libewf_header_values_generate_header_string_xml( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *string_length );

EWF_HEADER *libewf_header_values_generate_header_string_ewfx( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length );
EWF_HEADER *libewf_header_values_generate_header2_string_ewfx( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length );
EWF_HEADER *libewf_header_values_generate_xheader_string_ewfx( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length );

#ifdef __cplusplus
}
#endif

#endif

