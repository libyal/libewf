/*
 * Support functions
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

#if !defined( _LIBEWF_SUPPORT_H )
#define _LIBEWF_SUPPORT_H

#include <common.h>
#include <types.h>

#include <stdio.h>

#include "libewf_extern.h"
#include "libewf_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBEWF )

LIBEWF_EXTERN const char *libewf_get_version(
                           void );

LIBEWF_EXTERN uint8_t libewf_get_flags_read(
                       void );

LIBEWF_EXTERN uint8_t libewf_get_flags_read_write(
                       void );

LIBEWF_EXTERN uint8_t libewf_get_flags_write(
                       void );

LIBEWF_EXTERN uint8_t libewf_get_flags_write_resume(
                       void );

#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_check_file_signature(
                   const char *filename,
                   liberror_error_t **error );
#else
LIBEWF_EXTERN int libewf_check_file_signature(
                   const char *filename );
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_check_file_signature_wide(
                   const wchar_t *filename,
                   liberror_error_t **error );
#else
LIBEWF_EXTERN int libewf_check_file_signature_wide(
                   const wchar_t *filename );
#endif
#endif

LIBEWF_EXTERN int libewf_check_file_signature_file_io_handle(
                   libbfio_handle_t *file_io_handle,
                   liberror_error_t **error );

#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_glob(
                   const char *filename,
                   size_t length,
                   uint8_t format,
                   char **filenames[],
                   int *amount_of_filenames,
                   liberror_error_t **error );
#else
LIBEWF_EXTERN int libewf_glob(
                   const char *filename,
                   size_t length,
                   uint8_t format,
                   char **filenames[] );
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_glob_free(
                   char *filenames[],
                   int amount_of_filenames,
                   liberror_error_t **error );
#endif

/* TODO
LIBEWF_EXTERN int libewf_glob_file_io_handle(
                   libbfio_handle_t *file_io_handle,
                   uint8_t format,
                   libbfio_pool_t *file_io_pool,
                   liberror_error_t **error );
*/

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_glob_wide(
                   const wchar_t *filename,
                   size_t length,
                   uint8_t format,
                   wchar_t **filenames[],
                   int *amount_of_filenames,
                   liberror_error_t **error );
#else
LIBEWF_EXTERN int libewf_glob_wide(
                   const wchar_t *filename,
                   size_t length,
                   uint8_t format,
                   wchar_t **filenames[] );
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_glob_free_wide(
                   wchar_t *filenames[],
                   int amount_of_filenames,
                   liberror_error_t **error );
#endif
#endif

#if defined( __cplusplus )
}
#endif

#endif

