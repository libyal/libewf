/*
 * EWF data section
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

#if !defined( _EWF_DATA_H )
#define _EWF_DATA_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_data ewf_data_t;

struct ewf_data
{
	/* The media type
	 * consists of 1 byte
	 * 0x00 => remobable disk
	 * 0x01 => fixed disk
	 * 0x03 => optical disk
	 * 0x0e => LVF
	 * 0x10 => memory (RAM/process)
	 */
	uint8_t media_type;

	/* Unknown
	 * consists of 3 bytes
	 * contains 0x00
	 */
	uint8_t unknown1[ 3 ];

	/* The number of chunks
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t number_of_chunks[ 4 ];

	/* The number of sectors per chunks
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t sectors_per_chunk[ 4 ];

	/* The number of bytes per chunks
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t bytes_per_sector[ 4 ];

	/* The number of sectors
	 * consists of 8 bytes (64 bits)
	 */
	uint8_t number_of_sectors[ 8 ];

	/* C:H:S values
	 * consists of 12 bytes, 4 bytes per value
	 */
	uint8_t chs_cylinders[ 4 ];
	uint8_t chs_heads[ 4 ];
	uint8_t chs_sectors[ 4 ];

	/* Media flags
	 * consists of 1 byte
	 * 0x01 => image (1) or device (0) ?
	 * 0x02 => physical (1)
	 * 0x04 => Fastblock Tableau write blocker (1)
	 * 0x08 => Tableau write blocker (1)
	 */
	uint8_t media_flags;

	/* Unknown
	 * consists of 3 bytes
	 * contains 0x00
	 */
	uint8_t unknown2[ 3 ];

	/* PALM volume start sector
	 * consists of 4 bytes
	 */
	uint8_t palm_volume_start_sector[ 4 ];

	/* Unknown
	 * consists of 4 bytes
	 * contains 0x00
	 */
	uint8_t unknown3[ 4 ];

	/* SMART logs start sector
	 * consists of 4 bytes
	 * contains 0x00
	 */
	uint8_t smart_logs_start_sector[ 4 ];

	/* Compression level
	 * consists of 1 byte
	 * 0x00 => no compression,
	 * 0x01 => fast/good compression
	 * 0x02 => best compression
	 */
	uint8_t compression_level;

	/* Unknown
	 * consists of 3 bytes
	 * contains 0x00
	 */
	uint8_t unknown4[ 3 ];

	/* The number of sectors to use for error granularity
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t error_granularity[ 4 ];

	/* Unknown
	 * consists of 4 bytes
	 * contains 0x00
	 */
	uint8_t unknown5[ 4 ];

	/* The segment file set identifier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t set_identifier[ 16 ];

	/* Unknown
	 * consists of 963 bytes
	 * contains 0x00
	 */
	uint8_t unknown6[ 963 ];

	/* Reserved (signature)
	 * consists of 5 bytes
	 */
	uint8_t signature[ 5 ];

	/* The section checksum of all (previous) data
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t checksum[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWF_DATA_H ) */

