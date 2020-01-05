/*
 * User input functions for the ewftools
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

#if !defined( _EWFINPUT_H )
#define _EWFINPUT_H

#include <common.h>
#include <types.h>

#include "ewftools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_BZIP2_SUPPORT )
#define EWFINPUT_COMPRESSION_METHODS_AMOUNT		2
#else
#define EWFINPUT_COMPRESSION_METHODS_AMOUNT		1
#endif
#define EWFINPUT_COMPRESSION_METHODS_DEFAULT		0

#define EWFINPUT_COMPRESSION_LEVELS_AMOUNT		4
#define EWFINPUT_COMPRESSION_LEVELS_DEFAULT		0

#define EWFINPUT_FORMAT_TYPES_AMOUNT			15
#define EWFINPUT_FORMAT_TYPES_DEFAULT			8

#define EWFINPUT_MEDIA_TYPES_AMOUNT			4
#define EWFINPUT_MEDIA_TYPES_DEFAULT			0

#define EWFINPUT_MEDIA_FLAGS_AMOUNT			2
#define EWFINPUT_MEDIA_FLAGS_DEFAULT			1

#define EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT		12
#define EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT		2

#if defined( HAVE_BZIP2_SUPPORT )
extern system_character_t *ewfinput_compression_methods[ 2 ];
#else
extern system_character_t *ewfinput_compression_methods[ 1 ];
#endif
extern system_character_t *ewfinput_compression_levels[ 4 ];
extern system_character_t *ewfinput_format_types[ 15 ];
extern system_character_t *ewfinput_media_types[ 4 ];
extern system_character_t *ewfinput_media_flags[ 2 ];
extern system_character_t *ewfinput_sector_per_block_sizes[ 12 ];
extern system_character_t *ewfinput_yes_no[ 2 ];

int ewfinput_determine_ewf_format(
     const system_character_t *string,
     uint8_t *ewf_format,
     libcerror_error_t **error );

int ewfinput_determine_sectors_per_chunk(
     const system_character_t *string,
     uint32_t *sectors_per_chunk,
     libcerror_error_t **error );

int ewfinput_determine_compression_method(
     const system_character_t *string,
     uint16_t *compression_method,
     libcerror_error_t **error );

int ewfinput_determine_compression_values(
     const system_character_t *string,
     int8_t *compression_level,
     uint8_t *compression_flags,
     libcerror_error_t **error );

int ewfinput_determine_media_type(
     const system_character_t *string,
     uint8_t *media_type,
     libcerror_error_t **error );

int ewfinput_determine_media_flags(
     const system_character_t *string,
     uint8_t *media_flags,
     libcerror_error_t **error );

int ewfinput_determine_header_codepage(
     const system_character_t *string,
     int *header_codepage,
     libcerror_error_t **error );

int ewfinput_determine_yes_no(
     const system_character_t *string,
     uint8_t *yes_no_value,
     libcerror_error_t **error );

int ewfinput_get_string_variable(
     FILE *stream,
     const system_character_t *request_string,
     system_character_t *string_variable,
     size_t string_variable_size,
     libcerror_error_t **error );

int ewfinput_get_size_variable(
     FILE *stream,
     system_character_t *input_buffer,
     size_t input_buffer_size,
     const system_character_t *request_string,
     uint64_t minimum_size,
     uint64_t maximum_size,
     uint64_t default_size,
     uint64_t *size_variable,
     libcerror_error_t **error );

int ewfinput_get_byte_size_variable(
     FILE *stream,
     system_character_t *input_buffer,
     size_t input_buffer_size,
     const system_character_t *request_string,
     uint64_t minimum_size,
     uint64_t maximum_size,
     uint64_t default_size,
     uint64_t *byte_size_variable,
     libcerror_error_t **error );

int ewfinput_get_fixed_string_variable(
     FILE *stream,
     system_character_t *input_buffer,
     size_t input_buffer_size,
     const system_character_t *request_string,
     system_character_t **values,
     uint8_t number_of_values,
     uint8_t default_value,
     system_character_t **fixed_string_variable,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWFINPUT_H ) */

