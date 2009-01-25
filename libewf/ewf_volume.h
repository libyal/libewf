/*
 * EWF volume section (EWF-E01 EWF-L01)
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _EWF_VOLUME_H )
#define _EWF_VOLUME_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_volume ewf_volume_t;

struct ewf_volume
{
	/* The media type
	 * consists of 1 byte
	 * EnCase uses 0x00 for remobable media
	 *             0x01 for fixed media
	 *             0x0e for LVF
	 * FTK Imager always uses 0x01
	 */
	uint8_t media_type;

	/* Unknown
	 * consists of 3 bytes
	 * contains 0x00
	 */
	uint8_t unknown1[ 3 ];

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

	/* Unknown
	 * consists of 16 bytes
	 * contains 0x00
	 */
	uint8_t unknown2[ 16 ];

	/* Media flags
	 * consists of 1 byte
	 * byte 2 is physical (1) or non physical (0)
	 */
	uint8_t media_flags;

	/* Unknown
	 * consists of 3 bytes
	 * contains 0x00
	 */
	uint8_t unknown3[ 3 ];

	/* Unknown
	 * consists of 12 bytes
	 * contains 0x00
	 */
	uint8_t unknown4[ 12 ];

	/* Compression level (Encase 5 only)
	 * consists of 1 byte
	 * value is 0x00 for no compression, 0x01 for fast/good compression, 0x02 for best compression
	 */
	uint8_t compression_level;

	/* Unknown
	 * consists of 3 bytes
	 * contains 0x00
	 */
	uint8_t unknown5[ 3 ];

	/* The amount sectors to use for error granularity
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t error_granularity[ 4 ];

	/* Unknown
	 * consists of 4 bytes
	 * contains 0x00
	 */
	uint8_t unknown6[ 4 ];

	/* The GUID (Encase 5 only)
	 * consists of 16 bytes
	 */
	uint8_t guid[ 16 ];

	/* Unknown
	 * consists of 963 bytes
	 * contains 0x00
	 */
	uint8_t unknown7[ 963 ];

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

