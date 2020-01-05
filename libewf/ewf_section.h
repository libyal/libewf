/*
 * EWF section descriptor
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

#if !defined( _EWF_SECTION_H )
#define _EWF_SECTION_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_section_descriptor_v1 ewf_section_descriptor_v1_t;

struct ewf_section_descriptor_v1
{
	/* The section type string
	 * Consists of 16 bytes
	 */
	uint8_t type_string[ 16 ];

	/* The offset to the next section
	 * Consists of 8 bytes
	 */
	uint8_t next_offset[ 8 ];

	/* The section size
	 * Consists of 8 bytes
	 */
	uint8_t size[ 8 ];

	/* The padding
	 * Consists of 40 bytes
	 */
	uint8_t padding[ 40 ];

	/* The checksum of the section descriptor data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

typedef struct ewf_section_descriptor_v2 ewf_section_descriptor_v2_t;

struct ewf_section_descriptor_v2
{
	/* The section type
	 * Consists of 4 bytes
	 */
	uint8_t type[ 4 ];

	/* The data flags
	 * Consists of 4 bytes
	 */
	uint8_t data_flags[ 4 ];

	/* The offset to the previous section
	 * Consists of 8 bytes
	 */
	uint8_t previous_offset[ 8 ];

	/* The data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* The section descriptor size
	 * Consists of 4 bytes
	 */
	uint8_t descriptor_size[ 4 ];

	/* The padding size
	 * Consists of 4 bytes
	 */
	uint8_t padding_size[ 4 ];

	/* The data integrity hash
	 * Consists of 16 bytes
	 * Contains a MD5 of the data
	 */
	uint8_t data_integrity_hash[ 16 ];

	/* The padding
	 * Consists of 12 bytes
	 */
	uint8_t padding[ 12 ];

	/* The checksum of the section descriptor data
	 * consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWF_SECTION_H ) */

