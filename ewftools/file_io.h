/*
 * File IO functions
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

#if !defined( _FILE_IO_H )
#define _FILE_IO_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

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

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( WINAPI )
#define FILE_IO_O_CREAT 	_O_CREAT
#define FILE_IO_O_RDONLY	_O_RDONLY
#define FILE_IO_O_RDWR		_O_RDWR
#define FILE_IO_O_WRONLY	_O_WRONLY
#define FILE_IO_O_TRUNC   	_O_TRUNC

#else
#define FILE_IO_O_CREAT   	O_CREAT
#define FILE_IO_O_RDONLY	O_RDONLY
#define FILE_IO_O_RDWR		O_RDWR
#define FILE_IO_O_WRONLY	O_WRONLY
#define FILE_IO_O_TRUNC		O_TRUNC

#endif

#if defined( HAVE_OPEN ) && defined( HAVE_CLOSE )
int file_io_exists(
     const char *filename,
     liberror_error_t **error );
#endif

#if defined( HAVE_WOPEN ) && defined( HAVE_CLOSE )
int file_io_wexists(
     const wchar_t *filename,
     liberror_error_t **error );
#endif

#if defined( HAVE_OPEN )
int file_io_open(
     const char *filename,
     int flags,
     liberror_error_t **error );
#endif

#if defined( HAVE_WOPEN )
int file_io_wopen(
     const wchar_t *filename,
     int flags,
     liberror_error_t **error );
#endif

#if defined( WINAPI )
#define file_io_read( file_descriptor, buffer, size ) \
	_read( file_descriptor, (void *) buffer, (unsigned int) size )

#elif defined( HAVE_READ )
#define file_io_read( file_descriptor, buffer, size ) \
	read( file_descriptor, (void *) buffer, size )
#endif

#if defined( WINAPI )
#define file_io_lseek( file_descriptor, offset, whence ) \
	_lseeki64( file_descriptor, offset, whence )

#elif defined( HAVE_LSEEK )
#define file_io_lseek( file_descriptor, offset, whence ) \
	lseek( file_descriptor, offset, whence )
#endif

#if defined( WINAPI )
#define file_io_write( file_descriptor, buffer, size ) \
	_write( file_descriptor, (const void *) buffer, (unsigned int) size )

#elif defined( HAVE_WRITE )
#define file_io_write( file_descriptor, buffer, size ) \
	write( file_descriptor, (const void *) buffer, size )
#endif

#if defined( WINAPI )
#define file_io_close( file_descriptor ) \
	_close( file_descriptor )

#elif defined( HAVE_CLOSE )
#define file_io_close( file_descriptor ) \
	close( file_descriptor )
#endif

#if defined( __cplusplus )
}
#endif

#endif

