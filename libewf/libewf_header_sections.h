/*
 * Header sections functions
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

#if !defined( _LIBEWF_HEADER_SECTIONS_H )
#define _LIBEWF_HEADER_SECTIONS_H

#include <common.h>
#include <types.h>

#include "libewf_io_handle.h"
#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_header_sections libewf_header_sections_t;

struct libewf_header_sections
{
	/* The stored header
	 */
	uint8_t *header;

	/* The size of the stored header
	 */
	size_t header_size;

	/* The stored header2
	 */
	uint8_t *header2;

	/* The size of the stored header2
	 */
	size_t header2_size;

	/* The stored xheader
	 */
	uint8_t *xheader;

	/* The size of the stored xheader
	 */
	size_t xheader_size;

	/* Value to indicate the number of header sections found
	 */
	uint8_t number_of_header_sections;
};

int libewf_header_sections_initialize(
     libewf_header_sections_t **header_sections,
     libcerror_error_t **error );

int libewf_header_sections_free(
     libewf_header_sections_t **header_sections,
     libcerror_error_t **error );

int libewf_header_sections_clone(
     libewf_header_sections_t **destination_header_sections,
     libewf_header_sections_t *source_header_sections,
     libcerror_error_t **error );

int libewf_header_sections_generate(
     libewf_header_sections_t *header_sections,
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t format,
     int header_codepage,
     libcerror_error_t **error );

int libewf_header_sections_parse(
     libewf_header_sections_t *header_sections,
     libewf_io_handle_t *io_handle,
     libfvalue_table_t *header_values,
     uint8_t *format,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_HEADER_SECTIONS_H ) */

