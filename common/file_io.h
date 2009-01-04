/*
 * File IO functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined( _FILE_IO_H )
#define _FILE_IO_H

#include "common.h"
#include "system_string.h"

#if defined( HAVE_WINDOWS_API )
#include <io.h>
#include <share.h>

#else
#if defined( HAVE_SYS_TYPES_H )
#include <sys/types.h>
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

#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WINDOWS_API )
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

int libewf_file_io_open(
     const char *filename,
     int flags );

#define file_io_open( filename, flags ) \
	libewf_file_io_open( filename, flags )

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_file_io_wopen(
     const wchar_t *filename,
     int flags );

#define file_io_wopen( filename, flags ) \
	libewf_file_io_wopen( filename, flags )

#endif

#if defined( HAVE_WINDOWS_API )
#define file_io_read( file_descriptor, buffer, size ) \
	_read( file_descriptor, (void *) buffer, (unsigned int) size )

#else
#define file_io_read( file_descriptor, buffer, size ) \
	read( file_descriptor, (void *) buffer, size )
#endif

#if defined( HAVE_WINDOWS_API )
#define file_io_lseek( file_descriptor, offset, whence ) \
	_lseeki64( file_descriptor, offset, whence )

#else
#define file_io_lseek( file_descriptor, offset, whence ) \
	lseek( file_descriptor, offset, whence ) 

#endif

#if defined( HAVE_WINDOWS_API )
#define file_io_write( file_descriptor, buffer, size ) \
	_write( file_descriptor, (const void *) buffer, (unsigned int) size )

#else
#define file_io_write( file_descriptor, buffer, size ) \
	write( file_descriptor, (const void *) buffer, size )

#endif

#if defined( HAVE_WINDOWS_API )
#define file_io_close( file_descriptor ) \
	_close( file_descriptor )

#else
#define file_io_close( file_descriptor ) \
	close( file_descriptor )

#endif

#if defined( __cplusplus )
}
#endif

#endif

