/*
 * Find functions
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

#if !defined( _EWFTOOLS_FIND_H )
#define _EWFTOOLS_FIND_H

#include <common.h>
#include <types.h>

#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0520
#include <dir.h>
#elif defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* The find data structure type
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0520
#define ewftools_find_data_t	struct _wffblk
#else
#define ewftools_find_data_t	struct _wfinddata_t
#endif

#else

#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0520
#define ewftools_find_data_t	struct ffblk
#else
#define ewftools_find_data_t	struct _finddata_t
#endif

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

/* The find first function
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

#if defined( __BORLANDC__ )
#if __BORLANDC__ <= 0x0520
#define ewftools_find_first( filter, fileinfo ) \
	_wfindfirst( filter, fileinfo, 0 )
#else
#define ewftools_find_first( filter, fileinfo ) \
	__wfindfirst( filter, fileinfo )
#endif

#else
#define ewftools_find_first( filter, fileinfo ) \
	_wfindfirst( filter, fileinfo )
#endif

#else

#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0520
#define ewftools_find_first( filter, fileinfo ) \
	findfirst( filter, fileinfo, 0 )
#else
#define ewftools_find_first( filter, fileinfo ) \
	_findfirst( filter, fileinfo )
#endif

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

/* The find next function
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

#if defined( __BORLANDC__ )
#if __BORLANDC__ <= 0x0520
#define ewftools_find_next( handle, fileinfo ) \
	_wfindnext( fileinfo )
#else
#define ewftools_find_next( handle, fileinfo ) \
	__wfindnext( handle, fileinfo )
#endif

#else
#define ewftools_find_next( handle, fileinfo ) \
	_wfindnext( handle, fileinfo )
#endif

#else

#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0520
#define ewftools_find_next( handle, fileinfo ) \
	findnext( fileinfo )
#else
#define ewftools_find_next( handle, fileinfo ) \
	_findnext( handle, fileinfo )
#endif

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

/* The find close function
 */
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0520
#define ewftools_find_close( handle ) \
	0
#else
#define ewftools_find_close( handle ) \
	_findclose( handle )
#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWFTOOLS_FIND_H ) */

