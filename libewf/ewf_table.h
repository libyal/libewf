/*
 * EWF table section
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

#if !defined( _EWF_TABLE_H )
#define _EWF_TABLE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_table ewf_table_t;
typedef struct ewf_table_offset ewf_table_offset_t;

struct ewf_table
{
	/* The amount of chunks in the table
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t amount_of_chunks[ 4 ];

	/* Padding
	 * consists of 4 bytes
	 * value should be 0x00
	 */
	uint8_t padding1[ 4 ];

	/* The base offset
	 * consists of 8 bytes
	 */
	uint8_t base_offset[ 8 ];

	/* Padding
	 * consists of 4 bytes
	 * value should be 0x00
	 */
	uint8_t padding2[ 4 ];

	/* The section crc of all (previous) table data
	 * consists of 4 bytes
	 * starts with offset 76
	 */
	uint8_t crc[ 4 ];

	/* The offset array
	 * consists of mulitple 4 byte offsets
	 */

	/* The last offset is followed by a 4 byte CRC
	 */
};

struct ewf_table_offset
{
	/* An offset
	 * consists of 4 bytes
	 */
	uint8_t offset[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif

