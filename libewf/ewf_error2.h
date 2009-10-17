/*
 * EWF error2 section
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

#if !defined( _EWF_ERROR2_H )
#define _EWF_ERROR2_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_error2 ewf_error2_t;
typedef struct ewf_error2_sector ewf_error2_sector_t;

struct ewf_error2
{
	/* The amount of errors
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t amount_of_errors[ 4 ];

	/* Unknown
	 * consists of 512 bytes
	 * value should be 0x00
	 */
	uint8_t unknown[ 512 ];

	/* The section crc of all (previous) error2 data
	 * consists of 4 bytes
	 * starts with sector 76
	 */
	uint8_t crc[ 4 ];

	/* The sector array
	 * consists of 8 bytes per sector
	 * as long as necessary
	 */

	/* The last sector is followed by a 4 byte CRC
	 */
};

struct ewf_error2_sector
{
	/* The first error2 sector
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t first_sector[ 4 ];

	/* The amount of sectors
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t amount_of_sectors[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif

