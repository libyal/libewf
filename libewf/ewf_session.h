/*
 * EWF session section (EWF-E01)
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

#if !defined( _EWF_SESSION_H )
#define _EWF_SESSION_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_session_header_v1 ewf_session_header_v1_t;

struct ewf_session_header_v1
{
	/* Number of entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_entries[ 4 ];

	/* Unknown
	 * Consists of 28 bytes
	 * Contains 0x00
	 */
	uint8_t unknown1[ 28 ];

	/* The section checksum of all (previous) session header data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

typedef struct ewf_session_entry_v1 ewf_session_entry_v1_t;

struct ewf_session_entry_v1
{
	/* The flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];

	/* The start sector of the session
	 * Consists of 4 bytes
	 */
	uint8_t start_sector[ 4 ];

	/* Unknown
	 * Consists of 24 bytes
	 * Contains 0x00
	 */
	uint8_t unknown1[ 24 ];
};

typedef struct ewf_session_header_v2 ewf_session_header_v2_t;

struct ewf_session_header_v2
{
	/* Number of entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_entries[ 4 ];

	/* Unknown
	 * Consists of 12 bytes
	 */
	uint8_t unknown1[ 12 ];

	/* The section checksum of all (previous) session header data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Padding
	 * Consists of 12 bytes
	 */
	uint8_t padding[ 12 ];
};

typedef struct ewf_session_entry_v2 ewf_session_entry_v2_t;

struct ewf_session_entry_v2
{
	/* The start sector of the session
	 * Consists of 8 bytes
	 */
	uint8_t start_sector[ 8 ];

	/* The flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];

	/* Unknown
	 * Consists of 20 bytes
	 * Contains 0x00
	 */
	uint8_t unknown1[ 20 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWF_SESSION_H ) */

