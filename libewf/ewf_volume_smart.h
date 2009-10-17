/*
 * EWF SMART volume section (EWF-S01)
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

#if !defined( _EWF_VOLUME_SMART_H )
#define _EWF_VOLUME_SMART_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_volume_smart ewf_volume_smart_t;

struct ewf_volume_smart
{
	/* Reserved (The media type)
	 * consists of 4 bytes (32 bits)
	 * contains 0x01
	 */
	uint8_t unknown1[ 4 ];

	/* The amount of chunks
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t amount_of_chunks[ 4 ];

	/* The amount of sectors per chunks
	 * consists of 4 bytes (32 bits)
	 * value should be 64
	 */
	uint8_t sectors_per_chunk[ 4 ];

	/* The amount of bytes per chunks
	 * consists of 4 bytes (32 bits)
	 * value should be 512
	 */
	uint8_t bytes_per_sector[ 4 ];

	/* The amount of sectors
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t amount_of_sectors[ 4 ];

	/* Unknown - reserved
	 * consists of 20 bytes
	 * contains 0x00
	 */
	uint8_t unknown2[ 20 ];

	/* Unknown - padding
	 * consists of 45 bytes
	 * contains 0x00
	 */
	uint8_t unknown3[ 45 ];

	/* Reserved (signature)
	 * consists of 5 bytes
	 */
	uint8_t signature[ 5 ];

	/* The section crc of all (previous) volume data
	 * consists of 4 bytes (32 bits)
	 * starts with offset 76
	 */
	uint8_t crc[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif

