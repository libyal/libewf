/*
 * User input functions for the ewftools
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _EWFINPUT_H )
#define _EWFINPUT_H

#include <common.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define EWFINPUT_COMPRESSION_LEVELS_AMOUNT		4
#define EWFINPUT_COMPRESSION_LEVELS_DEFAULT		0

#define EWFINPUT_FORMAT_TYPES_AMOUNT			12
#define EWFINPUT_FORMAT_TYPES_DEFAULT			8

#define EWFINPUT_MEDIA_TYPES_AMOUNT			4
#define EWFINPUT_MEDIA_TYPES_DEFAULT			0

#define EWFINPUT_MEDIA_FLAGS_AMOUNT			2
#define EWFINPUT_MEDIA_FLAGS_DEFAULT			1

#define EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT		12
#define EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT		2

extern libcstring_system_character_t *ewfinput_compression_levels[ 4 ];
extern libcstring_system_character_t *ewfinput_format_types[ 12 ];
extern libcstring_system_character_t *ewfinput_media_types[ 4 ];
extern libcstring_system_character_t *ewfinput_media_flags[ 2 ];
extern libcstring_system_character_t *ewfinput_sector_per_block_sizes[ 12 ];
extern libcstring_system_character_t *ewfinput_yes_no[ 2 ];

int ewfinput_determine_ewf_format(
     const libcstring_system_character_t *argument,
     uint8_t *ewf_format,
     liberror_error_t **error );

int ewfinput_determine_sectors_per_chunk(
     const libcstring_system_character_t *argument,
     uint32_t *sectors_per_chunk,
     liberror_error_t **error );

int ewfinput_determine_compression_values(
     const libcstring_system_character_t *string,
     int8_t *compression_level,
     uint8_t *compression_flags,
     liberror_error_t **error );

int ewfinput_determine_media_type(
     const libcstring_system_character_t *argument,
     uint8_t *media_type,
     liberror_error_t **error );

int ewfinput_determine_media_flags(
     const libcstring_system_character_t *argument,
     uint8_t *media_flags,
     liberror_error_t **error );

int ewfinput_determine_header_codepage(
     const libcstring_system_character_t *argument,
     int *header_codepage,
     liberror_error_t **error );

int ewfinput_determine_yes_no(
     const libcstring_system_character_t *argument,
     uint8_t *yes_no_value,
     liberror_error_t **error );

int ewfinput_get_string_variable(
     FILE *stream,
     const libcstring_system_character_t *request_string,
     libcstring_system_character_t *string_variable,
     size_t string_variable_size,
     liberror_error_t **error );

int ewfinput_get_size_variable(
     FILE *stream,
     libcstring_system_character_t *input_buffer,
     size_t input_buffer_size,
     const libcstring_system_character_t *request_string,
     uint64_t minimum_size,
     uint64_t maximum_size,
     uint64_t default_size,
     uint64_t *size_variable,
     liberror_error_t **error );

int ewfinput_get_byte_size_variable(
     FILE *stream,
     libcstring_system_character_t *input_buffer,
     size_t input_buffer_size,
     const libcstring_system_character_t *request_string,
     uint64_t minimum_size,
     uint64_t maximum_size,
     uint64_t default_size,
     uint64_t *byte_size_variable,
     liberror_error_t **error );

int ewfinput_get_fixed_string_variable(
     FILE *stream,
     libcstring_system_character_t *input_buffer,
     size_t input_buffer_size,
     const libcstring_system_character_t *request_string,
     libcstring_system_character_t **values,
     uint8_t number_of_values,
     uint8_t default_value,
     libcstring_system_character_t **fixed_string_variable,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

