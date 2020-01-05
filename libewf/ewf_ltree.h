/*
 * EWF ltree section
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

#if !defined( _EWF_LTREE_H )
#define _EWF_LTREE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_ltree_header ewf_ltree_header_t;

struct ewf_ltree_header
{
	/* The MD5 hash of the ltree data
	 * Consists of 16 bytes
	 */
	uint8_t integrity_hash[ 16 ];

	/* The size of the data in bytes
	 * consists of 8 bytes (64 bits)
	 */
	uint8_t data_size[ 8 ];

	/* The section checksum of all data in the header
	 * where the checksum value itself is zeroed out
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t checksum[ 4 ];

	/* Unknown
	 * consists of 20 bytes
	 * contains 0x00
	 */
	uint8_t unknown1[ 20 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWF_LTREE_H ) */

