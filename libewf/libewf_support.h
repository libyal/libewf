/*
 * Support functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _LIBEWF_SUPPORT_H )
#define _LIBEWF_SUPPORT_H

#include <common.h>
#include <types.h>

#include <stdio.h>

#include <libewf/handle.h>

#include "libewf_error.h"
#include "libewf_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBEWF_EXTERN const char *libewf_get_version(
                           void );

LIBEWF_EXTERN int libewf_signal_abort(
                   libewf_handle_t *handle );

LIBEWF_EXTERN void libewf_error_free(
                    libewf_error_t **error );

LIBEWF_EXTERN void libewf_error_fprint(
                    libewf_error_t *error,
                    FILE *stream );

LIBEWF_EXTERN void libewf_error_backtrace_fprint(
                    libewf_error_t *error,
                    FILE *stream );

#if defined( __cplusplus )
}
#endif

#endif

