/*
 * GetOpt functions
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

#if !defined( _EWFGETOPT_H )
#define _EWFGETOPT_H

#include <common.h>
#include <types.h>

#include "system_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_GETOPT )
#define ewfgetopt( argument_count, argument_values, options_string ) \
	getopt( argument_count, argument_values, options_string )

#else

/* The current option argument
 */
extern system_character_t *optarg;

/* The option index
 */
extern int optind;

/* Value to indicate the current option
 */
extern system_integer_t optopt;

system_integer_t ewfgetopt(
                  int argument_count,
                  system_character_t * const argument_values[],
                  const system_character_t *options_string );

#endif

#if defined( __cplusplus )
}
#endif

#endif

