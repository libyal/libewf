/*
 * EWF character type string functions
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

#if !defined( _EWF_STRING_H )
#define _EWF_STRING_H

#include <common.h>
#include <types.h>

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include "ewf_char.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_STRLEN )
#define ewf_string_length( string ) \
	strlen( (char *) string )
#else
#error Missing string length function (strlen)
#endif

#if defined( HAVE_MEMCMP )
#define ewf_string_compare( string1, string2, size ) \
	memcmp( (void *) string1, (void *) string2, size )
#elif defined( HAVE_STRNCMP )
#define ewf_string_compare( string1, string2, size ) \
	strncmp( string1, string2, size )
#elif defined( HAVE_STRCMP )
#define ewf_string_compare( string1, string2, size ) \
	strcmp( string1, string2 )
#else
#error Missing string compare function (memcmp, strncmp and strcmp)
#endif

#if defined( HAVE_MEMCPY )
#define ewf_string_copy( destination, source, size ) \
	(ewf_char_t *) memcpy( (void *) destination, (void *) source, size )
#elif defined( HAVE_STRNCPY )
#define ewf_string_copy( destination, source, size ) \
	strncpy( destination, source, size )
#elif defined( HAVE_STRCPY )
#define ewf_string_copy( destination, source, size ) \
	strcpy( destination, source )
#else
#error Missing string copy function (memcpy, strncpy and strcpy)
#endif

#if defined( __cplusplus )
}
#endif

#endif

