/*
 * String conversion functions
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

#if !defined( _STRING_CONVERSION_H )
#define _STRING_CONVERSION_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_CHARACTER_TYPE )

int string_copy_wchar_to_char(
     char *destination,
     const wchar_t *source,
     size_t size );

int string_copy_char_to_wchar(
     wchar_t *destination,
     const char *source,
     size_t size );

#endif

#if defined( __cplusplus )
}
#endif

#endif

