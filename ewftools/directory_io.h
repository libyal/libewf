/*
 * Directory IO functions
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

#if !defined( _DIRECTORY_IO_H )
#define _DIRECTORY_IO_H

#include <common.h>
#include <types.h>

#if defined( HAVE_SYS_STAT_H )
#include <sys/stat.h>
#endif

#if defined( WINAPI )
#include <direct.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( WINAPI )
#define directory_io_chdir( pathname ) \
	_chdir( pathname )

#elif defined( HAVE_CHDIR )
#define directory_io_chdir( pathname ) \
	chdir( pathname )
#endif

#if defined( WINAPI )
#define directory_io_wchdir( pathname ) \
	_wchdir( pathname )

#endif

#if defined( WINAPI )
#define directory_io_mkdir( pathname ) \
	_mkdir( pathname )

#elif defined( HAVE_MKDIR )
#define directory_io_mkdir( pathname ) \
	mkdir( pathname, 0755 )
#endif

#if defined( WINAPI )
#define directory_io_wmkdir( pathname ) \
	_wmkdir( pathname )

#endif

#if defined( __cplusplus )
}
#endif

#endif

