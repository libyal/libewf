/*
 * Header sections functions
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

#if !defined( _LIBEWF_HEADER_SECTIONS_H )
#define _LIBEWF_HEADER_SECTIONS_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_values_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_header_sections libewf_header_sections_t;

/* Additional subhandle for media specific parameters
 */
struct libewf_header_sections
{
	/* The header codepage
	 */
	int header_codepage;

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

	/* Value to indicate how much header sections were found
	 */
	uint8_t amount_of_header_sections;
};

int libewf_header_sections_initialize(
     libewf_header_sections_t **header_sections,
     liberror_error_t **error );

int libewf_header_sections_free(
     libewf_header_sections_t **header_sections,
     liberror_error_t **error );

int libewf_header_sections_create(
     libewf_header_sections_t *header_sections,
     libewf_values_table_t *header_values,
     int8_t compression_level,
     uint8_t format,
     liberror_error_t **error );

int libewf_header_sections_determine_format(
     libewf_header_sections_t *header_sections,
     uint8_t ewf_format,
     uint8_t *format,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

