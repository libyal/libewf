/*
 * Extent functions
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

#if !defined( _LIBEWF_EXTENT_H )
#define _LIBEWF_EXTENT_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_lef_extent.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_extent libewf_internal_extent_t;

struct libewf_internal_extent
{
	/* The extent
	 */
	libewf_lef_extent_t *lef_extent;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libewf_extent_initialize(
     libewf_extent_t **extent,
     libewf_lef_extent_t *lef_extent,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_extent_free(
     libewf_extent_t **extent,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_EXTENT_H ) */

