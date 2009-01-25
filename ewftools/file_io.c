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

#include <common.h>
#include <narrow_string.h>
#include <wide_string.h>
#include <types.h>

#include "file_io.h"
#include "notify.h"

#if defined( HAVE_OPEN ) && defined( HAVE_CLOSE )

/* Function to determine if a file exists
 * Return 1 if file exists, 0 if not or -1 on error
 */
int file_io_exists(
     const char *filename )
{
	static char *function = "file_io_exists";
	int file_descriptor   = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( _sopen_s(
	     &file_descriptor,
	     filename,
	     ( _O_RDONLY | _O_BINARY ),
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
#else
	file_descriptor = open(
	                   filename,
	                   O_RDONLY,
	                   0644 );

	if( file_descriptor == -1 )
#endif
	{
		return( 0 );
	}
	file_io_close(
	 file_descriptor );

	return( 1 );
}
#endif

#if defined( HAVE_WOPEN ) && defined( HAVE_CLOSE )

/* Function to determine if a file exists
 * Return 1 if file exists, 0 if not or -1 on error
 */
int file_io_wexists(
     const wchar_t *filename )
{
	static char *function = "file_io_wexists";
	int file_descriptor   = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( _wsopen_s(
	     &file_descriptor,
	     filename,
	     ( _O_RDONLY | _O_BINARY ),
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
	{
		return( 0 );
	}
	file_io_close(
	 file_descriptor );

	return( 1 );
#endif
}
#endif

#if defined( HAVE_OPEN )

/* Function to wrap open()
 */
int file_io_open(
     const char *filename,
     int flags )
{
	static char *function = "file_io_open";
	int file_descriptor   = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( _sopen_s(
	     &file_descriptor,
	     filename,
	     ( flags | _O_BINARY ),
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
#else
	file_descriptor = open(
	                   filename,
	                   flags,
	                   0644 );

	if( file_descriptor == -1 )
#endif
	{
#if defined( HAVE_DEBUG_OUTPUT )
		notify_warning_printf( "%s: error opening file: %s.\n",
		 function,
		 filename );
#endif

		return( -1 );
	}
	return( file_descriptor );
}
#endif

#if defined( HAVE_WOPEN )

/* Function to wrap wopen() which is the wide character equivalent of open()
 */
int file_io_wopen(
     const wchar_t *filename,
     int flags )
{
	static char *function = "file_io_wopen";
	int file_descriptor   = 0;

	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( _wsopen_s(
	     &file_descriptor,
	     filename,
	     ( flags | _O_BINARY ),
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		notify_warning_printf( "%s: error opening file: %s.\n",
		 function,
		 filename );
#endif

		return( -1 );
	}
	return( file_descriptor );
#endif
}
#endif

