/*
 * EWF ltree section
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

#if !defined( _EWF_LTREE_H )
#define _EWF_LTREE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_ltree ewf_ltree_t;

struct ewf_ltree
{
	/* Unknown
	 * consists of 16 bytes
	 */
	uint8_t unknown1[ 16 ];

	/* The size of the tree in bytes
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t tree_size[ 4 ];

	/* Unknown
	 * consists of 4 bytes
	 * contains 0x00
	 */
	uint8_t unknown2[ 4 ];

	/* Unknown
	 * consists of 4 bytes
	 * possible CRC?
	 */
	uint8_t unknown3[ 4 ];

	/* Unknown
	 * consists of 20 bytes
	 * contains 0x00
	 */
	uint8_t unknown4[ 20 ];
};

#if defined( __cplusplus )
}
#endif

#endif

