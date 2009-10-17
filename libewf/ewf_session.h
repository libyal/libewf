/*
 * EWF session section (EWF-E01)
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

#if !defined( _EWF_SESSION_H )
#define _EWF_SESSION_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_session ewf_session_t;

struct ewf_session
{
	/* Amount of sessions
	 * consists of 4 bytes
	 */
	uint8_t amount_of_sessions[ 4 ];

	/* Unknown
	 * consists of 28 bytes
	 * contains 0x00
	 */
	uint8_t unknown1[ 28 ];

	/* The section crc of all (previous) session data
	 * consists of 4 bytes (32 bits)
	 * starts with offset 76
	 */
	uint8_t crc[ 4 ];

	/* The session entry array
	 * consists of 32 bytes per sector
	 * as long as necessary
	 */

	/* The last session entry is followed by a 4 byte CRC
	 */
};

typedef struct ewf_session_entry ewf_session_entry_t;

struct ewf_session_entry
{
	/* Unknown
	 * contains 0x00
	 * consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The first sector of the session
	 * consists of 4 bytes
	 */
	uint8_t first_sector[ 4 ];

	/* Unknown
	 * consists of 24 bytes
	 * contains 0x00
	 */
	uint8_t unknown2[ 24 ];
};

#if defined( __cplusplus )
}
#endif

#endif

