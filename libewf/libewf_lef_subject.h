/*
 * Logical Evidence File (LEF) subject functions
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

#if !defined( _LIBEWF_LEF_SUBJECT_H )
#define _LIBEWF_LEF_SUBJECT_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_lef_subject libewf_lef_subject_t;

struct libewf_lef_subject
{
	/* Dummy
	 */
	int dummy;
};

int libewf_lef_subject_initialize(
     libewf_lef_subject_t **lef_subject,
     libcerror_error_t **error );

int libewf_lef_subject_free(
     libewf_lef_subject_t **lef_subject,
     libcerror_error_t **error );

int libewf_lef_subject_clone(
     libewf_lef_subject_t **destination_lef_subject,
     libewf_lef_subject_t *source_lef_subject,
     libcerror_error_t **error );

int libewf_lef_subject_read_data(
     libewf_lef_subject_t *lef_subject,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_LEF_SUBJECT_H ) */

