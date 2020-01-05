/*
 * EWF file header
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

#if !defined( _EWF_FILE_HEADER_H )
#define _EWF_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_file_header_v1 ewf_file_header_v1_t;

struct ewf_file_header_v1
{
	/* The signature
	 * Consists of 8 bytes
         * Contains:
         * EVF 0x09 0x0d 0x0a 0xff 0x00
         * LVF 0x09 0x0d 0x0a 0xff 0x00
	 */
	uint8_t signature[ 8 ];

	/* The fields start
	 * Consists of 1 byte
	 * Contains: 0x01
	 */
	uint8_t fields_start;

	/* The segment number
	 * Consists of 2 bytes
	 */
	uint8_t segment_number[ 2 ];

	/* The fields end
	 * Consists of 2 bytes
	 * Contains: 0x00 0x00
	 */
	uint8_t fields_end[ 2 ];
};

typedef struct ewf_file_header_v2 ewf_file_header_v2_t;

struct ewf_file_header_v2
{
	/* The signature
	 * Consists of 8 bytes
         * Contains:
         * EVF2 0x0d 0x0a 0x81 0x00
         * LEF2 0x0d 0x0a 0x81 0x00
	 */
	uint8_t signature[ 8 ];

	/* The major version
	 * Consists of 1 byte
	 */
	uint8_t major_version;

	/* The minor version
	 * Consists of 1 byte
	 */
	uint8_t minor_version;

	/* The compression method
	 * Consists of 2 bytes
	 */
	uint8_t compression_method[ 2 ];

	/* The segment number
	 * Consists of 4 bytes
	 */
	uint8_t segment_number[ 4 ];

	/* The segment set identifier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t set_identifier[ 16 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWF_FILE_HEADER_H ) */

