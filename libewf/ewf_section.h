/*
 * EWF section start
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

#if !defined( _EWF_SECTION_H )
#define _EWF_SECTION_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_section_start ewf_section_start_t;

struct ewf_section_start
{
	/* The section type string
	 * consists of 16 bytes
	 */
	uint8_t type[ 16 ];

	/* The offset to the next section
	 * consists of 8 bytes (64 bits)
	 */
	uint8_t next_offset[ 8 ];

	/* The size of the section
	 * consists of 8 bytes (64 bits)
	 */
	uint8_t size[ 8 ];

	/* The padding
	 * consists of 40 bytes
	 */
	uint8_t padding[ 40 ];

	/* The checksum of the section start data
	 * consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The section data
	 */
};

#if defined( __cplusplus )
}
#endif

#endif

