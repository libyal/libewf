/*
 * Date and time functions
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

#if !defined( _DATE_TIME_H )
#define _DATE_TIME_H

#include "common.h"
#include "types.h"

#include <time.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_TIME ) || defined( WINAPI )
#define date_time_time( timestamp ) \
	time( timestamp )
#endif

#if defined( HAVE_MKTIME ) || defined( WINAPI )
#define date_time_mktime( time_elements ) \
	mktime( time_elements )
#endif

/* Wide character ctime
 */
#if defined( _MSC_VER )
#define date_time_wctime( timestamp, string, size ) \
	( _wctime_s( string, size, timestamp ) != 0 ? NULL : string )

#elif defined( WINAPI )

#define HAVE_LOCAL_WCTIME

wchar_t *_date_time_wctime(
		  const time_t *timestamp,
		  wchar_t *string,
		  size_t length );

#define date_time_wctime( timestamp, string, size ) \
	_date_time_wctime( timestamp, string, size )
#endif

/* Narrow character ctime
 */
#if defined( _MSC_VER )
#define date_time_ctime( timestamp, string, size ) \
	( ctime_s( string, size, timestamp ) != 0 ? NULL : string )

#elif defined( HAVE_CTIME_R_SIZE )
#define date_time_ctime( timestamp, string, size ) \
	ctime_r( timestamp, string, size )

#elif defined( HAVE_CTIME_R )
#define date_time_ctime( timestamp, string, size ) \
	ctime_r( timestamp, string )

#elif defined( HAVE_CTIME ) || defined( WINAPI )

#define HAVE_LOCAL_CTIME

char *_date_time_ctime(
	   const time_t *timestamp,
	   char *string,
	   size_t length );

#define date_time_ctime( timestamp, string, size ) \
	_date_time_ctime( timestamp, string, size )
#endif

/* Local time function
 */
#if defined( _MSC_VER )
#define date_time_localtime_r( timestamp, time_elements ) \
	localtime_s( time_elements, timestamp )

#elif defined( HAVE_LOCALTIME_R )
#define date_time_localtime_r( timestamp, time_elements ) \
	localtime_r( timestamp, time_elements )
#endif

#if defined( date_time_localtime_r ) || defined( HAVE_LOCALTIME ) || defined( WINAPI )
struct tm *_date_time_localtime(
			const time_t *timestamp );

#define date_time_localtime( timestamp ) \
	_date_time_localtime( timestamp )
#endif

/* GMT/UTC time function
 */
#if defined( _MSC_VER )
#define date_time_gmtime_r( timestamp, time_elements ) \
	gmtime_s( time_elements, timestamp )

#elif defined( HAVE_GMTIME_R )
#define date_time_gmtime_r( timestamp, time_elements ) \
	gmtime_r( timestamp, time_elements )
#endif

#if defined( date_time_gmtime_r ) || defined( HAVE_GMTIME ) || defined( WINAPI )
struct tm *_date_time_gmtime(
			const time_t *timestamp );

#define date_time_gmtime( timestamp ) \
	_date_time_gmtime( timestamp )
#endif

#if defined( __cplusplus )
}
#endif

#endif

