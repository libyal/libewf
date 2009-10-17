/*
 * libewf EWF eXtention delta chunk
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

#if !defined( _EWFX_DELTA_CHUNK_H )
#define _EWFX_DELTA_CHUNK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewfx_delta_chunk_header ewfx_delta_chunk_header_t;

struct ewfx_delta_chunk_header
{
	/* The number of the chunk
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t chunk[ 4 ];

	/* The size of the chunk
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t chunk_size[ 4 ];

	/* Padding
	 * some padding reserved for future use
	 * consists of 6 bytes
	 * "DELTA\x00"
	 */
	uint8_t padding[ 6 ];

	/* The section crc of all (previous) delta chunk section data
	 * consists of 4 bytes
	 * starts with offset 76
	 */
	uint8_t crc[ 4 ];

	/* The chunk data with CRC
	 */
};

#if defined( __cplusplus )
}
#endif

#endif

