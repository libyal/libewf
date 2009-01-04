/*
 * ewfgetopt
 * GetOpt functions for the libewf tools
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <common.h>
#include <system_string.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include "../libewf/libewf_notify.h"

#include "ewfgetopt.h"

#if !defined( HAVE_GETOPT )

/* The current option argument
 */
system_character_t *optarg = NULL;

/* The option index
 * Start with argument 1 (argument 0 is the program name)
 */
int optind = 1;

/* Value to indicate the current option
 */
system_integer_t optopt = 0;

/* Get the program options
 * Function for platforms that do not have the getopt function
 * Returns the option character processed, or -1 on error,
 * ? if the option was not in the options string, : if the option argument was missing
 */
system_integer_t ewfgetopt(
                  int argument_count,
                  system_character_t * const argument_values[],
                  const system_character_t *options_string )
{
	system_character_t *option_value   = NULL;
	system_character_t *argument_value = NULL;
	static char *function              = "ewfgetopt";

	if( optind >= argument_count )
	{
		return( -1 );
	}
	argument_value = argument_values[ optind ];

	/* Check if the argument value is not an empty string
	 */
	if( *argument_value == (system_character_t) '\0' )
	{
		return( -1 );
	}
	/* Check if the first character is a option marker '-'
	 */
	if( *argument_value != (system_character_t) '-' )
	{
		return( -1 );
	}
	argument_value++;

	/* Check if long options are provided '--'
	 */
	if( *argument_value == (system_character_t) '-' )
	{
		optind++;

		return( -1 );
	}
	optopt       = *argument_value;
	option_value = system_string_search(
	                options_string,
	                optopt,
	                system_string_length(
	                 options_string ) );

	argument_value++;

	/* Check if an argument was specified or that the option was not found
	 * in the option string
	 */
	if( ( optopt == (system_integer_t) ':' )
	 || ( option_value == NULL ) )
	{
		if( *argument_value == (system_character_t) '\0' )
		{
			optind++;
		}
		if( ( *options_string != (system_character_t) ':' )
		 && ( optopt != (system_integer_t) '?' ) )
		{
			LIBEWF_WARNING_PRINT( "%s: no such option: %" PRIc_SYSTEM ".\n",
			 function, optopt );
		}
		return( (system_integer_t) '?' );
	}
	option_value++;

	/* Check if no option argument is required
	 */
	if( *option_value != (system_character_t) ':' )
	{
		optarg = NULL;

		if( *argument_value == (system_character_t) '\0' )
		{
			optind++;
		}
	}
	/* Check if the argument is right after the option flag with no space in between
	 */
	else if( *argument_value != (system_character_t) '\0' )
	{
		optarg = argument_value;

		optind++;
	}
	else
	{
		optind++;

		/* Check if the argument was provided as the next argument value
		 */
		if( argument_count <= optind )
		{
			if( *option_value == ':' )
			{
				return( (system_integer_t) ':' );
			}
			LIBEWF_WARNING_PRINT( "%s: option: %" PRIc_SYSTEM " requires an argument.\n",
			 function, optopt );

			return( (system_integer_t) '?' );
		}
		optarg = argument_values[ optind ];

		optind++;
	}
	return( (system_integer_t) optopt );
}

#endif

