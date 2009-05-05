/*
 * ewfinfo
 * Shows information stored in an EWF file
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include <libewf.h>

#include "byte_size_string.h"
#include "ewfgetopt.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewfsignal.h"
#include "glob.h"
#include "guid.h"
#include "info_handle.h"
#include "notify.h"
#include "system_string.h"

info_handle_t *ewfinfo_info_handle = NULL;
int ewfinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use ewfinfo to determine information about the EWF format (Expert Witness\n"
	                 "Compression Format).\n\n" );

	fprintf( stream, "Usage: ewfinfo [ -A codepage ] [ -d date_format ] [ -ehimvV ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default), windows-1250,\n"
	                 "\t           windows-1251, windows-1252, windows-1253, windows-1254,\n"
	                 "\t           windows-1255, windows-1256, windows-1257, windows-1258\n" );
	fprintf( stream, "\t-d:        specify the date format, options: ctime (default), dm (day/month),\n"
	                 "\t           md (month/day), iso8601\n" );
	fprintf( stream, "\t-e:        only show EWF read error information\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-i:        only show EWF acquiry information\n" );
	fprintf( stream, "\t-m:        only show EWF media information\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
}

/* Signal handler for ewfinfo
 */
void ewfinfo_signal_handler(
      ewfsignal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "ewfinfo_signal_handler";

	ewfinfo_abort = 1;

	if( ( ewfinfo_info_handle != NULL )
	 && ( info_handle_signal_abort(
	       ewfinfo_info_handle,
	       &error ) != 1 ) )
	{
		notify_warning_printf(
		 "%s: unable to signal info handle to abort.\n",
		 function );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

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
	system_character_t * const *argv_filenames = NULL;

#if !defined( HAVE_GLOB_H )
	glob_t *glob                               = NULL;
#endif
	info_handle_t *info_handle                 = NULL;
	liberror_error_t *error                    = NULL;

	system_character_t *program                = _SYSTEM_CHARACTER_T_STRING( "ewfinfo" );
	system_integer_t option                    = 0;
	uint8_t verbose                            = 0;
	uint8_t date_format                        = LIBEWF_DATE_FORMAT_CTIME;
	char info_option                           = 'a';
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
	                   _SYSTEM_CHARACTER_T_STRING( "A:d:ehimvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
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

			case (system_integer_t) 'd':
				if( system_string_compare(
				     optarg,
				     _SYSTEM_CHARACTER_T_STRING( "dm" ),
				     3 ) == 0 )
				{
					date_format = LIBEWF_DATE_FORMAT_DAYMONTH;
				}
				else if( system_string_compare(
				          optarg,
				          _SYSTEM_CHARACTER_T_STRING( "md" ),
				          3 ) == 0 )
				{
					date_format = LIBEWF_DATE_FORMAT_MONTHDAY;
				}
				else if( system_string_compare(
				          optarg,
				          _SYSTEM_CHARACTER_T_STRING( "iso8601" ),
				          8 ) == 0 )
				{
					date_format = LIBEWF_DATE_FORMAT_ISO8601;
				}
				else if( system_string_compare(
				          optarg,
				          _SYSTEM_CHARACTER_T_STRING( "ctime" ),
				          3 ) != 0 )
				{
					fprintf(
					 stderr,
					 "Unsupported date format: %" PRIs_SYSTEM " using default ctime.\n",
					 optarg );
				}
				break;

			case (system_integer_t) 'e':
				if( info_option != 'a' )
				{
					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_SYSTEM " and %c\n",
					 option, info_option );

					usage_fprint(
					 stdout );

					return( EXIT_FAILURE );
				}
				info_option = 'e';

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'i':
				if( info_option != 'a' )
				{
					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_SYSTEM " and %c\n",
					 option, info_option );

					usage_fprint(
					 stdout );

					return( EXIT_FAILURE );
				}
				info_option = 'i';

				break;

			case (system_integer_t) 'm':
				if( info_option != 'a' )
				{
					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_SYSTEM " and %c\n",
					 option, info_option );

					usage_fprint(
					 stdout );

					return( EXIT_FAILURE );
				}
				info_option = 'm';

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
	     ewfinfo_signal_handler ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );
	}
#if !defined( HAVE_GLOB_H )
	if( glob_initialize(
	     &glob,
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

	if( info_handle_initialize(
	     &info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create info handle.\n" );

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
#if defined( HAVE_V2_API )
	if( info_handle_set_header_codepage(
	     info_handle,
	     header_codepage,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set header codepage in info handle.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		info_handle_free(
		 &info_handle,
		 NULL );

#if !defined( HAVE_GLOB_H )
		glob_free(
		 &glob,
		 NULL );
#endif

		return( EXIT_FAILURE );
	}
#endif
	result = info_handle_open_input(
	          info_handle,
	          argv_filenames,
	          amount_of_filenames,
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

	if( ( ewfinfo_abort == 0 )
	 && ( result != 1 ) )
	{
		ewfoutput_error_fprint(
		 stderr,
		 "Unable to open EWF file(s)" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		info_handle_free(
		 &info_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ( ewfinfo_abort == 0 )
	 && ( ( info_option == 'a' )
	  || ( info_option == 'i' ) ) )
	{
		if( info_handle_header_values_fprint(
		     info_handle,
		     date_format,
		     stdout,
		     &error ) != 1 )
		{
			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( ( ewfinfo_abort == 0 )
	 && ( ( info_option == 'a' )
	  || ( info_option == 'm' ) ) )
	{
		if( info_handle_media_information_fprint(
		     info_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print media information.\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
		if( info_handle_hash_values_fprint(
		     info_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print hash values.\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
		if( info_handle_sessions_fprint(
		     info_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print sessions.\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( ( ewfinfo_abort == 0 )
	 && ( ( info_option == 'a' )
	  || ( info_option == 'e' ) ) )
	{
		if( info_handle_acquiry_errors_fprint(
		     info_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print acquiry errors.\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( ewfsignal_detach() != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );
	}
	if( info_handle_close(
	     info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close EWF file(s).\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		info_handle_free(
		 &info_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( info_handle_free(
	     &info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( ewfinfo_abort != 0 )
	{
		fprintf(
		 stdout, "%s: ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

