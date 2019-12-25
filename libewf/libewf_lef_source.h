/*
 * Logical Evidence File (LEF) source functions
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_LEF_SOURCE_H )
#define _LIBEWF_LEF_SOURCE_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_lef_source libewf_lef_source_t;

struct libewf_lef_source
{
	/* Dummy
	 */
	int dummy;
};

int libewf_lef_source_initialize(
     libewf_lef_source_t **source,
     libcerror_error_t **error );

int libewf_lef_source_free(
     libewf_lef_source_t **source,
     libcerror_error_t **error );

int libewf_lef_source_clone(
     libewf_lef_source_t **destination_source,
     libewf_lef_source_t *source_source,
     libcerror_error_t **error );

int libewf_lef_source_read_data(
     libewf_lef_source_t *source,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_LEF_SOURCE_H ) */

