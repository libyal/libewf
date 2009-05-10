/*
 * File stream IO functions
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

#if !defined( _FILE_STREAM_IO_H )
#define _FILE_STREAM_IO_H

#include <common.h>

#include <stdio.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( WINAPI )
#define FILE_STREAM_IO_OPEN_APPEND	"ab"
#define FILE_STREAM_IO_OPEN_READ	"rb"
#define FILE_STREAM_IO_OPEN_WRITE	"wb"
#else
#define FILE_STREAM_IO_OPEN_APPEND	"a"
#define FILE_STREAM_IO_OPEN_READ	"r"
#define FILE_STREAM_IO_OPEN_WRITE	"w"
#endif

#if defined( HAVE_FOPEN ) || defined( WINAPI )
#define file_stream_io_fopen( filename, mode ) \
	fopen( filename, mode )
#endif

#if defined( WINAPI )
#define file_stream_io_wfopen( filename, mode ) \
	_wfopen( filename, mode )
#endif

#if defined( HAVE_FCLOSE ) || defined( WINAPI )
#define file_stream_io_fclose( file_stream ) \
	fclose( file_stream )
#endif

#if defined( HAVE_FREAD ) || defined( WINAPI )
#define file_stream_io_fread( stream, data, size ) \
	fread( data, 1, size, stream )
#endif

#if defined( HAVE_FWRITE ) || defined( WINAPI )
#define file_stream_io_fwrite( stream, data, size ) \
	fwrite( data, 1, size, stream )
#endif

#if defined( HAVE_FEOF ) || defined( WINAPI )
#define file_stream_io_feof( stream ) \
        feof( stream )
#endif

#if defined( HAVE_FGETS ) || defined( WINAPI )
#define file_stream_io_get_narrow_string( string, size, stream ) \
        fgets( string, size, stream )
#endif

#if defined( HAVE_FGETWS ) || defined( WINAPI )
#define file_stream_io_get_wide_string( string, size, stream ) \
        fgetws( string, size, stream )
#endif

#if defined( __cplusplus )
}
#endif

#endif

