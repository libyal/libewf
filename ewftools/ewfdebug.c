/*
 * ewfdebug
 * Debugs EWF files
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include <libewf.h>

#include "ewfgetopt.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewfsignal.h"
#include "glob.h"
#include "notify.h"

libewf_handle_t *ewfdebug_input_handle = NULL;
int ewfdebug_abort                     = 0;

/* Prints the executable usage information to the stream
 *  */
void usage_fprint(
      FILE *stream )
{
        if( stream == NULL )
        {
                return;
        }
	fprintf( stream, "Use ewfdebug to analyze EWF file(s).\n\n" );

	fprintf( stream, "Usage: ewfdebug [ -A codepage ] [ -hqvV ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default), windows-1250,\n"
	                 "\t           windows-1251, windows-1252, windows-1253, windows-1254,\n"
	                 "\t           windows-1255, windows-1256, windows-1257, windows-1258\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-q:        quiet shows no status information\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
}

/* Signal handler for ewfdebug
 */
void ewfdebug_signal_handler(
      ewfsignal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "ewfdebug_signal_handler";

	ewfdebug_abort = 1;

	if( ( ewfdebug_input_handle != NULL )
#if defined( HAVE_V2_API )
	 && ( libewf_handle_signal_abort(
	       ewfdebug_input_handle,
	       &error ) != 1 ) )
#else
	 && ( libewf_signal_abort(
	       ewfdebug_input_handle ) != 1 ) )
#endif
	{
		notify_warning_printf(
		 "%s: unable to signal input handle to abort.\n",
		 function );

#if defined( HAVE_V2_API )
		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
#endif

		return;
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( file_io_close(
	     0 ) != 0 )
	{
		notify_warning_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if !defined( HAVE_GLOB_H )
	glob_t *glob                               = NULL;
#endif

	liberror_error_t *error                    = NULL;

	system_character_t * const *argv_filenames = NULL;
	system_character_t **ewf_filenames         = NULL;

	system_character_t *program                = _SYSTEM_CHARACTER_T_STRING( "ewfdebug" );

	system_integer_t option                    = 0;
	uint8_t verbose                            = 0;
	int amount_of_filenames                    = 0;
	int header_codepage                        = LIBEWF_CODEPAGE_ASCII;
	int result                                 = 0;

	notify_set_values(
	 stderr,
	 1 );

	if( system_string_initialize(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system string.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	ewfoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = ewfgetopt(
			   argc,
			   argv,
			   _SYSTEM_CHARACTER_T_STRING( "A:hqvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'A':
				if( ewfinput_determine_header_codepage(
				     optarg,
				     &header_codepage,
				     &error ) != 1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unsuported header codepage defaulting to: ascii.\n" );

					header_codepage = LIBEWF_CODEPAGE_ASCII;
				}
				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'q':
				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				ewfoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing EWF image file(s).\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	notify_set_values(
	 stderr,
	 verbose );
	libewf_set_notify_values(
	 stderr,
	 verbose );

	if( ewfsignal_attach(
	     ewfdebug_signal_handler ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );
	}
#if !defined( HAVE_GLOB_H )
	if( glob_initialize(
	     &glob
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize glob.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( glob_resolve(
	     glob,
	     &argv[ optind ],
	     argc - optind,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to resolve glob.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		glob_free(
		 &glob,
		 NULL );

		return( EXIT_FAILURE );
	}
	argv_filenames      = glob->result;
	amount_of_filenames = glob->amount_of_results;
#else
	argv_filenames      = &argv[ optind ];
	amount_of_filenames = argc - optind;
#endif

	if( amount_of_filenames == 1 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
#if defined( HAVE_V2_API )
		if( libewf_glob_wide(
		     argv_filenames[ 0 ],
		     system_string_length(
		     argv_filenames[ 0 ] ),
		     LIBEWF_FORMAT_UNKNOWN,
		     &ewf_filenames,
		     &amount_of_filenames,
		     &error ) != 1 )
#else
		amount_of_filenames = libewf_glob_wide(
		                       argv_filenames[ 0 ],
		                       system_string_length(
		                        argv_filenames[ 0 ] ),
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &ewf_filenames );

		if( amount_of_filenames <= 0 )
#endif
#else
#if defined( HAVE_V2_API )
		if( libewf_glob(
		     argv_filenames[ 0 ],
		     system_string_length(
		     argv_filenames[ 0 ] ),
		     LIBEWF_FORMAT_UNKNOWN,
		     &ewf_filenames,
		     &amount_of_filenames,
		     &error ) != 1 )
#else
		amount_of_filenames = libewf_glob(
		                       argv_filenames[ 0 ],
		                       system_string_length(
		                        argv_filenames[ 0 ] ),
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &ewf_filenames );

		if( amount_of_filenames <= 0 )
#endif
#endif
		{
			fprintf(
			 stderr,
			 "Unable to resolve ewf file(s).\n" );

#if defined( HAVE_V2_API )
			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
#endif

#if !defined( HAVE_GLOB_H )
			glob_free(
			 &glob,
			 NULL );
#endif

			return( EXIT_FAILURE );
		}
		argv_filenames = (system_character_t * const *) ewf_filenames;
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_initialize(
	     &ewfdebug_input_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize input handle.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

#if !defined( HAVE_GLOB_H )
		glob_free(
		 &glob,
		 NULL );
#endif

		return( EXIT_FAILURE );
	}
	result = libewf_handle_open(
	          ewfdebug_input_handle,
	          argv_filenames,
	          amount_of_filenames,
	          LIBEWF_OPEN_READ_WRITE,
	          &error );

#if !defined( HAVE_GLOB_H )
	if( glob_free(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
#endif
	if( ewf_filenames != NULL )
	{
		for( ; amount_of_filenames > 0; amount_of_filenames-- )
		{
			memory_free(
			 ewf_filenames[ amount_of_filenames - 1 ] );
		}
		memory_free(
		 ewf_filenames );
	}
	if( ( ewfdebug_abort == 0 )
	 && ( result != 1 ) )
	{
		ewfoutput_error_fprint(
		 stderr,
		 "Unable to open EWF file(s)" );

		libewf_handle_close(
		 ewfdebug_input_handle,
		 NULL );
		libewf_handle_free(
		 &ewfdebug_input_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* TODO */

	if( libewf_handle_close(
	     ewfdebug_input_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close EWF file(s).\n" );

		libewf_handle_free(
		 &ewfdebug_input_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libewf_handle_free(
	     &ewfdebug_input_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free input handle.\n" );

		return( EXIT_FAILURE );
	}
	if( ewfsignal_detach() != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );
	}
	if( ewfdebug_abort != 0 )
	{
		fprintf(
		 stdout,
		 "%s: ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Debug completed.\n" );

	return( EXIT_SUCCESS );
#else
	fprintf(
	 stdout,
	 "Requires libewf version 2 API.\n" );

	return( EXIT_SUCCESS );
	
#endif
}

