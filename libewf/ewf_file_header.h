/*
 * EWF file header
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

#if !defined( _EWF_FILE_HEADER_H )
#define _EWF_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_file_header ewf_file_header_t;

struct ewf_file_header
{
	/* The EWF file signature (magic header)
	 * consists of 8 bytes containing
         * EVF 0x09 0x0d 0x0a 0xff 0x00
	 */
	uint8_t signature[ 8 ];

	/* The fields start
	 * consists of 1 byte (8 bit) containing
	 * 0x01
	 */
	uint8_t fields_start;

	/* The fields segment number
	 * consists of 2 bytes (16 bits) containing
	 */
	uint8_t fields_segment[ 2 ];

	/* The fields end
	 * consists of 2 bytes (16 bits) containing
	 * 0x00 0x00
	 */
	uint8_t fields_end[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif

