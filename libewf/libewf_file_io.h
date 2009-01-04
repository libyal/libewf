/*
 * File IO functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBEWF_FILE_IO_H )
#define _LIBEWF_FILE_IO_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( HAVE_IO_H )
#include <io.h>
#endif

#if defined( HAVE_SHARE_H )
#include <share.h>
#endif

#if defined( HAVE_SYS_STAT_H )
#include <sys/stat.h>
#endif

#if defined( HAVE_FCNTL_H )
#include <fcntl.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#include "libewf_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( WINAPI )
#define LIBEWF_FILE_IO_O_CREAT 		_O_CREAT
#define LIBEWF_FILE_IO_O_RDONLY		_O_RDONLY
#define LIBEWF_FILE_IO_O_RDWR		_O_RDWR
#define LIBEWF_FILE_IO_O_WRONLY		_O_WRONLY
#define LIBEWF_FILE_IO_O_TRUNC   	_O_TRUNC

#else
#define LIBEWF_FILE_IO_O_CREAT   	O_CREAT
#define LIBEWF_FILE_IO_O_RDONLY		O_RDONLY
#define LIBEWF_FILE_IO_O_RDWR		O_RDWR
#define LIBEWF_FILE_IO_O_WRONLY		O_WRONLY
#define LIBEWF_FILE_IO_O_TRUNC		O_TRUNC

#endif

int libewf_file_io_open(
     const char *filename,
     int flags,
     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libewf_file_io_open_wide(
     const wchar_t *filename,
     int flags,
     liberror_error_t **error );
#endif

#if defined( WINAPI )
#define libewf_file_io_close( file_descriptor ) \
	_close( file_descriptor )

#else
#define libewf_file_io_close( file_descriptor ) \
	close( file_descriptor )
#endif

#if defined( WINAPI )
#define libewf_file_io_read( file_descriptor, buffer, size ) \
	_read( file_descriptor, (void *) buffer, (unsigned int) size )

#else
#define libewf_file_io_read( file_descriptor, buffer, size ) \
	read( file_descriptor, (void *) buffer, size )
#endif

#if defined( WINAPI )
#define libewf_file_io_lseek( file_descriptor, offset, whence ) \
	_lseeki64( file_descriptor, offset, whence )

#else
#define libewf_file_io_lseek( file_descriptor, offset, whence ) \
	lseek( file_descriptor, offset, whence )
#endif

#if defined( WINAPI )
#define libewf_file_io_write( file_descriptor, buffer, size ) \
	_write( file_descriptor, (const void *) buffer, (unsigned int) size )

#else
#define libewf_file_io_write( file_descriptor, buffer, size ) \
	write( file_descriptor, (const void *) buffer, size )
#endif

int libewf_file_io_exists(
     const char *filename,
     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libewf_file_io_exists_wide(
     const wchar_t *filename,
     liberror_error_t **error );
#endif

#if defined( __cplusplus )
}
#endif

#endif

