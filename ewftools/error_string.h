/*
 * Error string functions
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

#if !defined( _ERROR_STRING_H )
#define _ERROR_STRING_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WCSERROR_R )
#if defined( WINAPI )
#define error_string_wcserror_r( error_number, string, size ) \
	_wcserror_s( string, size, error_number )

#define ERROR_STRING_WCSTRERROR_R_RETURN_ERROR	0
#endif
#endif

#if defined( HAVE_STRERROR_R )
#if defined( WINAPI )
#define error_string_strerror_r( error_number, string, size ) \
	strerror_s( string, size, error_number )

#define ERROR_STRING_STRERROR_R_RETURN_ERROR	0

#else
#define error_string_strerror_r( error_number, string, size ) \
	strerror_r( error_number, string, size )

#if defined( STRERROR_R_CHAR_P )
#define ERROR_STRING_STRERROR_R_RETURN_ERROR	NULL

#else
#define ERROR_STRING_STRERROR_R_RETURN_ERROR	0
#endif

#endif
#endif

#if defined( error_string_strerror_r ) || defined( HAVE_STRERROR )
char *error_string_strerror(
       int error_number );
#endif

#if defined( error_string_wcserror_r )
wchar_t *error_string_wcserror(
          int error_number );
#endif

#if defined( __cplusplus )
}
#endif

#endif

