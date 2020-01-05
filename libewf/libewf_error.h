/*
 * Error functions
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

#if !defined( _LIBEWF_INTERNAL_ERROR_H )
#define _LIBEWF_INTERNAL_ERROR_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if !defined( HAVE_LOCAL_LIBEWF )
#include <libewf/error.h>
#endif

#include "libewf_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBEWF )

LIBEWF_EXTERN \
void libewf_error_free(
      libewf_error_t **error );

LIBEWF_EXTERN \
int libewf_error_fprint(
     libewf_error_t *error,
     FILE *stream );

LIBEWF_EXTERN \
int libewf_error_sprint(
     libewf_error_t *error,
     char *string,
     size_t size );

LIBEWF_EXTERN \
int libewf_error_backtrace_fprint(
     libewf_error_t *error,
     FILE *stream );

LIBEWF_EXTERN \
int libewf_error_backtrace_sprint(
     libewf_error_t *error,
     char *string,
     size_t size );

#endif /* !defined( HAVE_LOCAL_LIBEWF ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_INTERNAL_ERROR_H ) */

