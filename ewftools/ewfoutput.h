/*
 * User output functions for the ewftools
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _EWFOUTPUT_H )
#define _EWFOUTPUT_H

#include <common.h>
#include <date_time.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#include <libewf/handle.h>

#include "character_string.h"


#if defined( HAVE_WIDE_CHARACTER_TYPE )

#define ewfoutput_get_hash_value_identifier( handle, index, identifier, identifier_length ) \
	libewf_get_hash_value_identifier_wide( handle, index, identifier, identifier_length )

#define ewfoutput_get_hash_value( handle, identifier, value, value_length ) \
	libewf_get_hash_value_wide( handle, identifier, value, value_length )

#define ewfoutput_get_header_value_identifier( handle, index, identifier, identifier_length ) \
	libewf_get_header_value_identifier_wide( handle, index, identifier, identifier_length )

#define ewfoutput_get_header_value( handle, identifier, value, value_length ) \
	libewf_get_header_value_wide( handle, identifier, value, value_length )

#define ewfoutput_get_header_value_case_number( handle, case_number, case_number_length ) \
	libewf_get_header_value_case_number_wide( handle, case_number, case_number_length )

#define ewfoutput_get_header_value_description( handle, description, description_length ) \
	libewf_get_header_value_description_wide( handle, description, description_length )

#define ewfoutput_get_header_value_evidence_number( handle, evidence_number, evidence_number_length ) \
	libewf_get_header_value_evidence_number_wide( handle, evidence_number, evidence_number_length )

#define ewfoutput_get_header_value_examiner_name( handle, examiner_name, examiner_name_length ) \
	libewf_get_header_value_examiner_name_wide( handle, examiner_name, examiner_name_length )

#define ewfoutput_get_header_value_notes( handle, notes, notes_length ) \
	libewf_get_header_value_notes_wide( handle, notes, notes_length )

#define ewfoutput_get_header_value_acquiry_date( handle, acquiry_date, acquiry_date_length ) \
	libewf_get_header_value_acquiry_date_wide( handle, acquiry_date, acquiry_date_length )

#define ewfoutput_get_header_value_system_date( handle, system_date, system_date_length ) \
	libewf_get_header_value_system_date_wide( handle, system_date, system_date_length )

#define ewfoutput_get_header_value_password( handle, password, password_length ) \
	libewf_get_header_value_password_wide( handle, password, password_length )

#define ewfoutput_get_header_value_compression_type( handle, compression_type, compression_type_length ) \
	libewf_get_header_value_compression_type_wide( handle, compression_type, compression_type_length )

#define ewfoutput_get_header_value_acquiry_operating_system( handle, acquiry_operating_system, acquiry_operating_system_length ) \
	libewf_get_header_value_acquiry_operating_system_wide( handle, acquiry_operating_system, acquiry_operating_system_length )

#define ewfoutput_get_header_value_acquiry_software_version( handle, acquiry_software_version, acquiry_software_version_length ) \
	libewf_get_header_value_acquiry_software_version_wide( handle, acquiry_software_version, acquiry_software_version_length )

#define ewfoutput_get_header_value_model( handle, model, model_length ) \
	libewf_get_header_value_model_wide( handle, model, model_length )

#define ewfoutput_get_header_value_serial_number( handle, serial_number, serial_number_length ) \
	libewf_get_header_value_serial_number_wide( handle, serial_number, serial_number_length )

#else
#define ewfoutput_get_hash_value_identifier( handle, index, identifier, identifier_length ) \
	libewf_get_hash_value_identifier( handle, index, identifier, identifier_length )

#define ewfoutput_get_hash_value( handle, identifier, value, value_length ) \
	libewf_get_hash_value( handle, identifier, value, value_length )

#define ewfoutput_get_header_value_identifier( handle, index, identifier, identifier_length ) \
	libewf_get_header_value_identifier( handle, index, identifier, identifier_length )

#define ewfoutput_get_header_value( handle, identifier, value, value_length ) \
	libewf_get_header_value( handle, identifier, value, value_length )

#define ewfoutput_get_header_value_case_number( handle, case_number, case_number_length ) \
	libewf_get_header_value_case_number( handle, case_number, case_number_length )

#define ewfoutput_get_header_value_description( handle, description, description_length ) \
	libewf_get_header_value_description( handle, description, description_length )

#define ewfoutput_get_header_value_evidence_number( handle, evidence_number, evidence_number_length ) \
	libewf_get_header_value_evidence_number( handle, evidence_number, evidence_number_length )

#define ewfoutput_get_header_value_examiner_name( handle, examiner_name, examiner_name_length ) \
	libewf_get_header_value_examiner_name( handle, examiner_name, examiner_name_length )

#define ewfoutput_get_header_value_notes( handle, notes, notes_length ) \
	libewf_get_header_value_notes( handle, notes, notes_length )

#define ewfoutput_get_header_value_acquiry_date( handle, acquiry_date, acquiry_date_length ) \
	libewf_get_header_value_acquiry_date( handle, acquiry_date, acquiry_date_length )

#define ewfoutput_get_header_value_system_date( handle, system_date, system_date_length ) \
	libewf_get_header_value_system_date( handle, system_date, system_date_length )

#define ewfoutput_get_header_value_password( handle, password, password_length ) \
	libewf_get_header_value_password( handle, password, password_length )

#define ewfoutput_get_header_value_compression_type( handle, compression_type, compression_type_length ) \
	libewf_get_header_value_compression_type( handle, compression_type, compression_type_length )

#define ewfoutput_get_header_value_acquiry_operating_system( handle, acquiry_operating_system, acquiry_operating_system_length ) \
	libewf_get_header_value_acquiry_operating_system( handle, acquiry_operating_system, acquiry_operating_system_length )

#define ewfoutput_get_header_value_acquiry_software_version( handle, acquiry_software_version, acquiry_software_version_length ) \
	libewf_get_header_value_acquiry_software_version( handle, acquiry_software_version, acquiry_software_version_length )

#define ewfoutput_get_header_value_model( handle, model, model_length ) \
	libewf_get_header_value_model( handle, model, model_length )

#define ewfoutput_get_header_value_serial_number( handle, serial_number, serial_number_length ) \
	libewf_get_header_value_serial_number( handle, serial_number, serial_number_length )

#endif

#if defined( __cplusplus )
extern "C" {
#endif

void ewfoutput_version_fprint(
      FILE *stream,
      character_t *program );

void ewfoutput_copyright_fprint(
      FILE *stream );

void ewfoutput_error_fprint(
      FILE *stream,
      char *format,
      ... );

void ewfoutput_timestamp_fprint(
      FILE *stream,
      time_t timestamp );

void ewfoutput_bytes_per_second_fprint(
      FILE *stream,
      size64_t bytes,
      time_t seconds );

void ewfoutput_bytes_fprint(
      FILE *stream,
      size64_t bytes );

void ewfoutput_acquiry_parameters_fprint(
      FILE *stream,
      system_character_t *filename,
      character_t *case_number,
      character_t *description,
      character_t *evidence_number,
      character_t *examiner_name,
      character_t *notes,
      uint8_t media_type,
      uint8_t volume_type,
      int8_t compression_level,
      uint8_t compress_empty_block,
      uint8_t libewf_format,
      off64_t acquiry_offset,
      size64_t acquiry_size,
      size64_t segment_file_size,
      uint32_t sectors_per_chunk,
      uint32_t sector_error_granularity,
      uint8_t read_error_retry,
      uint8_t wipe_block_on_read_error );

void ewfoutput_acquiry_errors_fprint(
      FILE *stream,
      libewf_handle_t *handle,
      uint32_t *amount_of_errors );

void ewfoutput_crc_errors_fprint(
      FILE *stream,
      libewf_handle_t *handle,
      uint32_t *amount_of_errors );

void ewfoutput_sessions_fprint(
      FILE *stream,
      libewf_handle_t *handle,
      uint32_t *amount_of_sessions );

void ewfoutput_header_values_fprint(
      FILE *stream,
      libewf_handle_t *handle );

void ewfoutput_hash_values_fprint(
      FILE *stream,
      libewf_handle_t *handle,
      character_t *indentation,
      uint8_t ignore_md5,
      uint8_t ignore_sha1 );

#if defined( __cplusplus )
}
#endif

#endif

