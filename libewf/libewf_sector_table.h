/*
 * Error sector table functions, for CRC and acquiry read errrors
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

#if !defined( _LIBEWF_SECTOR_TABLE_H )
#define _LIBEWF_SECTOR_TABLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_sector_table_entry libewf_sector_table_entry_t;

struct libewf_sector_table_entry
{
	/* The first sector
	 */
	uint64_t first_sector;

	/* The amount of sectors
	 */
	uint64_t amount_of_sectors;
};

typedef struct libewf_sector_table libewf_sector_table_t;

struct libewf_sector_table
{
	/* The amount of sectors in the table
	 */
	uint32_t amount;

	/* A dynamic array containting references to the sectors
	 */
	libewf_sector_table_entry_t *sector;
};

int libewf_sector_table_initialize(
     libewf_sector_table_t **sector_table,
     uint32_t amount,
     liberror_error_t **error );

int libewf_sector_table_free(
     libewf_sector_table_t **sector_table,
     liberror_error_t **error );

int libewf_sector_table_resize(
     libewf_sector_table_t *sector_table,
     uint32_t amount,
     liberror_error_t **error );

int libewf_sector_table_get_sector(
     libewf_sector_table_t *sector_table,
     uint32_t index,
     uint64_t *first_sector,
     uint64_t *amount_of_sectors,
     liberror_error_t **error );

int libewf_sector_table_add_sector(
     libewf_sector_table_t *sector_table,
     uint64_t first_sector,
     uint64_t amount_of_sectors,
     int merge_continious_entries,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

