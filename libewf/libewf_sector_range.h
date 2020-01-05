/*
 * Sector range functions
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

#if !defined( _LIBEWF_SECTOR_RANGE_H )
#define _LIBEWF_SECTOR_RANGE_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_sector_range libewf_sector_range_t;

struct libewf_sector_range
{
	/* The start sector
	 */
	uint64_t start_sector;

	/* The end sector
	 */
	uint64_t end_sector;

	/* The number of sectors
	 */
	uint64_t number_of_sectors;
};

int libewf_sector_range_initialize(
     libewf_sector_range_t **sector_range,
     libcerror_error_t **error );

int libewf_sector_range_free(
     libewf_sector_range_t **sector_range,
     libcerror_error_t **error );

int libewf_sector_range_clone(
     libewf_sector_range_t **destination_sector_range,
     libewf_sector_range_t *source_sector_range,
     libcerror_error_t **error );

int libewf_sector_range_get(
     libewf_sector_range_t *sector_range,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

int libewf_sector_range_set(
     libewf_sector_range_t *sector_range,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SECTOR_RANGE_H ) */

