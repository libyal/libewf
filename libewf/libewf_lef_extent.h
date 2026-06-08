/*
 * Logical Evidence File (LEF) extent functions
 *
 * Copyright (C) 2006-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_LEF_EXTENT_H )
#define _LIBEWF_LEF_EXTENT_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_lef_extent libewf_lef_extent_t;

struct libewf_lef_extent
{
	/* The data offset
	 */
	off64_t data_offset;

	/* The data size
	 */
	size64_t data_size;

	/* The data flags
	 */
	uint32_t data_flags;
};

int libewf_lef_extent_initialize(
     libewf_lef_extent_t **lef_extent,
     libcerror_error_t **error );

int libewf_lef_extent_free(
     libewf_lef_extent_t **lef_extent,
     libcerror_error_t **error );

int libewf_lef_extent_clone(
     libewf_lef_extent_t **destination_lef_extent,
     libewf_lef_extent_t *source_lef_extent,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_LEF_EXTENT_H ) */

