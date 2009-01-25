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

#if defined( HAVE_TIME )
#define date_time_time( timestamp ) \
	time( timestamp )
#endif

#if defined( HAVE_WCTIME_R )
#if defined( WINAPI )
#define date_time_wctime( timestamp, string, size ) \
	( _wctime_s( string, size, timestamp ) != 0 ? NULL : string )
#endif
#endif

#if defined( HAVE_LOCALTIME_R )
#if defined( WINAPI )
#define date_time_localtime_r( timestamp, time_elements ) \
	localtime_s( time_elements, timestamp )

#else
#define date_time_localtime_r( timestamp, time_elements ) \
	localtime_r( timestamp, time_elements )
#endif
#endif

#if defined( HAVE_GMTIME_R )
#if defined( WINAPI )
#define date_time_gmtime_r( timestamp, time_elements ) \
	gmtime_s( time_elements, timestamp )

#else
#define date_time_gmtime_r( timestamp, time_elements ) \
	gmtime_r( timestamp, time_elements )
#endif
#endif

#if defined( HAVE_CTIME_R )
#if defined( WINAPI )
#define date_time_ctime( timestamp, string, size ) \
	( ctime_s( string, size, timestamp ) != 0 ? NULL : string )

#elif defined( HAVE_CTIME_R_SIZE )
#define date_time_ctime( timestamp, string, size ) \
	ctime_r( timestamp, string, size )

#else
#define date_time_ctime( timestamp, string, size ) \
	ctime_r( timestamp, string )
#endif

#elif defined( HAVE_CTIME )
char *libewf_date_time_ctime(
       const time_t *timestamp,
       char *string,
       size_t length );

#define date_time_ctime( timestamp, string, size ) \
	libewf_date_time_ctime( timestamp, string, size )
#endif

#if defined( HAVE_MKTIME )
#if defined( WINAPI )
#define date_time_mktime( time_elements ) \
	mktime( time_elements )

#else
#define date_time_mktime( time_elements ) \
	mktime( time_elements )
#endif
#endif

#if defined( date_time_localtime_r ) || defined( HAVE_LOCALTIME )
struct tm *libewf_date_time_localtime(
            const time_t *timestamp );

#define date_time_localtime( timestamp ) \
	libewf_date_time_localtime( timestamp )
#endif

#if defined( date_time_gmtime_r ) || defined( HAVE_GMTIME )
struct tm *libewf_date_time_gmtime(
            const time_t *timestamp );

#define date_time_gmtime( timestamp ) \
	libewf_date_time_gmtime( timestamp )
#endif

#if defined( __cplusplus )
}
#endif

#endif

