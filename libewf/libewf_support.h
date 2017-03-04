/*
 * Support functions
 *
 * Copyright (C) 2006-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBEWF )

LIBEWF_EXTERN \
const char *libewf_get_version(
             void );

LIBEWF_EXTERN \
int libewf_get_access_flags_read(
     void );

LIBEWF_EXTERN \
int libewf_get_access_flags_read_write(
     void );

LIBEWF_EXTERN \
int libewf_get_access_flags_write(
     void );

LIBEWF_EXTERN \
int libewf_get_access_flags_write_resume(
     void );

LIBEWF_EXTERN \
int libewf_get_codepage(
     int *codepage,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_set_codepage(
     int codepage,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBEWF ) */

LIBEWF_EXTERN \
int libewf_check_file_signature(
     const char *filename,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBEWF_EXTERN \
int libewf_check_file_signature_wide(
     const wchar_t *filename,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBEWF_EXTERN \
int libewf_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_glob(
     const char *filename,
     size_t length,
     uint8_t format,
     char **filenames[],
     int *number_of_filenames,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_glob_free(
     char *filenames[],
     int number_of_filenames,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBEWF_EXTERN \
int libewf_glob_wide(
     const wchar_t *filename,
     size_t length,
     uint8_t format,
     wchar_t **filenames[],
     int *number_of_filenames,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_glob_wide_free(
     wchar_t *filenames[],
     int number_of_filenames,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SUPPORT_H ) */

