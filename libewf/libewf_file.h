/*
 * File handling functions
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

#if !defined( _LIBEWF_FILE_H )
#define _LIBEWF_FILE_H

#include <common.h>

#include <liberror.h>

#include <libewf/handle.h>

#include "libewf_extern.h"

#if defined( __cplusplus )
extern "C" {
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

#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_glob(
                   const char *filename,
                   size_t length,
                   uint8_t format,
                   char **filenames[],
                   liberror_error_t **error );
#else
LIBEWF_EXTERN int libewf_glob(
                   const char *filename,
                   size_t length,
                   uint8_t format,
                   char **filenames[] );
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_glob_wide(
                   const wchar_t *filename,
                   size_t length,
                   uint8_t format,
                   wchar_t **filenames[],
                   liberror_error_t **error );
#else
LIBEWF_EXTERN int libewf_glob_wide(
                   const wchar_t *filename,
                   size_t length,
                   uint8_t format,
                   wchar_t **filenames[] );
#endif
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_handle_open(
                   libewf_handle_t *handle,
                   char * const filenames[],
                   int amount_of_files,
                   uint8_t flags,
                   liberror_error_t **error );
#else
LIBEWF_EXTERN libewf_handle_t *libewf_open(
                                char * const filenames[],
                                int amount_of_files,
                                uint8_t flags );
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_handle_open_wide(
                   libewf_handle_t *handle,
                   wchar_t * const filenames[],
                   int amount_of_files,
                   uint8_t flags,
                   liberror_error_t **error );
#else
LIBEWF_EXTERN libewf_handle_t *libewf_open_wide(
                                wchar_t * const filenames[],
                                int amount_of_files,
                                uint8_t flags );
#endif
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN int libewf_handle_close(
                   libewf_handle_t *handle,
                   liberror_error_t **error );
#else
LIBEWF_EXTERN int libewf_close(
                   libewf_handle_t *handle );
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN off64_t libewf_handle_seek_offset(
                       libewf_handle_t *handle,
                       off64_t offset,
                       liberror_error_t **error );
#else
LIBEWF_EXTERN off64_t libewf_seek_offset(
                       libewf_handle_t *handle,
                       off64_t offset );
#endif

#if defined( HAVE_V2_API )
LIBEWF_EXTERN off64_t libewf_handle_get_offset(
                       libewf_handle_t *handle,
                       liberror_error_t **error );
#else
LIBEWF_EXTERN off64_t libewf_get_offset(
                       libewf_handle_t *handle );
#endif

#if defined( __cplusplus )
}
#endif

#endif

