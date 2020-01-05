/*
 * EWF table section
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

#if !defined( _EWF_TABLE_H )
#define _EWF_TABLE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* The EWF version 1 table header
 */
typedef struct ewf_table_header_v1 ewf_table_header_v1_t;

struct ewf_table_header_v1
{
	/* The number of entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_entries[ 4 ];

	/* Padding
	 * Consists of 4 bytes
	 */
	uint8_t padding1[ 4 ];

	/* The base offset
	 * Consists of 8 bytes
	 */
	uint8_t base_offset[ 8 ];

	/* Padding
	 * Consists of 4 bytes
	 */
	uint8_t padding2[ 4 ];

	/* The section checksum of all (previous) table header data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

/* The EWF version 1 table entry (chunk descriptor)
 */
typedef struct ewf_table_entry_v1 ewf_table_entry_v1_t;

struct ewf_table_entry_v1
{
	/* The chunk data offset
	 * Consists of 4 bytes
	 */
	uint8_t chunk_data_offset[ 4 ];
};

/* The EWF version 2 table header
 */
typedef struct ewf_table_header_v2 ewf_table_header_v2_t;

struct ewf_table_header_v2
{
	/* The first chunk number
	 * Consists of 8 bytes
	 */
	uint8_t first_chunk_number[ 8 ];

	/* The number of entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_entries[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The section checksum of all (previous) table header data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Padding
	 * Consists of 12 bytes
	 */
	uint8_t padding[ 12 ];
};

/* The EWF version 2 table entry (chunk descriptor)
 */
typedef struct ewf_table_entry_v2 ewf_table_entry_v2_t;

struct ewf_table_entry_v2
{
	/* The chunk data offset
	 * Consists of 8 bytes
	 */
	uint8_t chunk_data_offset[ 8 ];

	/* The chunk data size
	 * Consists of 4 bytes
	 */
	uint8_t chunk_data_size[ 4 ];

	/* The chunk data flags
	 * Consists of 4 bytes
	 */
	uint8_t chunk_data_flags[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWF_TABLE_H ) */

