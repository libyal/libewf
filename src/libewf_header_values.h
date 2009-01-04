/*
 * libewf header values
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <inttypes.h>
#include <time.h>

#include "ewf_header.h"

#include "libewf_string.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIBEWF_FORMAT_UNKNOWN 0x00
#define LIBEWF_FORMAT_ENCASE1 0x01
#define LIBEWF_FORMAT_ENCASE2 0x02
#define LIBEWF_FORMAT_ENCASE3 0x03
#define LIBEWF_FORMAT_ENCASE4 0x04
#define LIBEWF_FORMAT_ENCASE5 0x05
#define LIBEWF_FORMAT_SMART   0x0E
#define LIBEWF_FORMAT_FTK     0x0F

#define LIBEWF_DATE_FORMAT_DAYMONTH 0x01
#define LIBEWF_DATE_FORMAT_MONTHDAY 0x02

#define LIBEWF_COMPRESSION_TYPE_NONE "n"
#define LIBEWF_COMPRESSION_TYPE_FAST "f"
#define LIBEWF_COMPRESSION_TYPE_BEST "b"

#define LIBEWF_HEADER_VALUES libewf_header_values_t
#define LIBEWF_HEADER_VALUES_SIZE sizeof( LIBEWF_HEADER_VALUES )

typedef struct libewf_header_values libewf_header_values_t;

struct libewf_header_values
{
	/* Case number
	 */
	LIBEWF_STRING *case_number;

	/* Description
	 */
	LIBEWF_STRING *description;

	/* Examiner name
	 */
	LIBEWF_STRING *examiner_name;

	/* Evidence number
	 */
	LIBEWF_STRING *evidence_number;

	/* Notes
	 */
	LIBEWF_STRING *notes;

	/* Acquiry date
	 */
	LIBEWF_STRING *acquiry_date;

	/* System date
	 */
	LIBEWF_STRING *system_date;

	/* Acquiry operating system
	 */
	LIBEWF_STRING *acquiry_operating_system;

	/* Acquiry software version
	 */
	LIBEWF_STRING *acquiry_software_version;

	/* Password
	 */
	LIBEWF_STRING *password;

	/* Compression type
	 */
	LIBEWF_STRING *compression_type;

	/* Unknown dc
	 */
	LIBEWF_STRING *unknown_dc;
};

LIBEWF_HEADER_VALUES *libewf_header_values_alloc( void );
void libewf_header_values_free( LIBEWF_HEADER_VALUES *header_values );

LIBEWF_STRING *libewf_convert_date_header_value( LIBEWF_STRING *header_value, uint8_t date_format );
LIBEWF_STRING *libewf_generate_date_header_value( time_t timestamp );
LIBEWF_STRING *libewf_convert_date_header2_value( LIBEWF_STRING *header_value, uint8_t date_format );
LIBEWF_STRING *libewf_generate_date_header2_value( time_t timestamp );
LIBEWF_STRING *libewf_header_values_set_value( LIBEWF_STRING* header_value, char *value );

LIBEWF_HEADER_VALUES *libewf_header_values_parse_header( EWF_HEADER *header, uint8_t date_format );
void libewf_header_values_fprint( FILE *stream, LIBEWF_HEADER_VALUES *header_values );

EWF_HEADER *libewf_header_values_generate_header_string_encase3( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, uint8_t compression_level );
EWF_HEADER *libewf_header_values_generate_header_string_encase4( LIBEWF_HEADER_VALUES *header_values, time_t timestamp );
EWF_HEADER *libewf_header_values_generate_header2_string_encase4( LIBEWF_HEADER_VALUES *header_values, time_t timestamp );
EWF_HEADER *libewf_header_values_generate_header2_string_encase5( LIBEWF_HEADER_VALUES *header_values, time_t timestamp );
EWF_HEADER *libewf_header_values_generate_header_string_ftk( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, uint8_t compression_level );

#ifdef __cplusplus
}
#endif

#endif

