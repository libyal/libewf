/*
 * ewfgetopt
 * GetOpt functions for the libewf tools
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#include "../libewf/libewf_includes.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include "../libewf/libewf_notify.h"

#include "ewfgetopt.h"

#ifndef HAVE_GETOPT

/* The current option argument
 */
CHAR_T *optarg = NULL;

/* The option index
 * Start with argument 1 (argument 0 is the program name)
 */
int optind = 1;

/* Value to indicate the current option
 */
INT_T optopt = 0;

/* Get the program options
 * Function for platforms that do not have the getopt function
 * Returns the option character processed, or -1 on error,
 * ? if the option was not in the options string, : if the option argument was missing
 */
INT_T ewfgetopt( int argument_count, CHAR_T * const argument_values[], const CHAR_T *options_string )
{
	CHAR_T *option_value   = NULL;
	CHAR_T *argument_value = NULL;

	if( optind >= argument_count )
	{
		return( -1 );
	}
	argument_value = argument_values[ optind ];

	/* Check if the argument value is not an empty string
	 */
	if( *argument_value == (CHAR_T) '\0' )
	{
		return( -1 );
	}
	/* Check if the first character is a option marker '-'
	 */
	if( *argument_value != (CHAR_T) '-' )
	{
		return( -1 );
	}
	argument_value++;

	/* Check if long options are provided '--'
	 */
	if( *argument_value == (CHAR_T) '-' )
	{
		optind++;

		return( -1 );
	}
	optopt       = *argument_value;
	option_value = CHAR_T_SEARCH( options_string, optopt, CHAR_T_LENGTH( options_string ) );

	argument_value++;

	/* Check if an argument was specified or that the option was not found
	 * in the option string
	 */
	if( ( optopt == (INT_T) ':' ) || ( option_value == NULL ) )
	{
		if( *argument_value == (CHAR_T) '\0' )
		{
			optind++;
		}
		if( ( *options_string != (CHAR_T) ':' ) && ( optopt != (INT_T) '?' ) )
		{
			LIBEWF_WARNING_PRINT( "ewfgetopt: no such option: %" PRIc ".\n", optopt );
		}
		return( (INT_T) '?' );
	}
	option_value++;

	/* Check if no option argument is required
	 */
	if( *option_value != (CHAR_T) ':' )
	{
		optarg = NULL;

		if( *argument_value == (CHAR_T) '\0' )
		{
			optind++;
		}
	}
	/* Check if the argument is right after the option flag with no space in between
	 */
	else if( *argument_value != (CHAR_T) '\0' )
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
				return( (INT_T) ':' );
			}
			LIBEWF_WARNING_PRINT( "ewfgetopt: option: %" PRIc " requires an argument.\n", optopt );

			return( (INT_T) '?' );
		}
		optarg = argument_values[ optind ];

		optind++;
	}
	return( (INT_T) optopt );
}

#endif

