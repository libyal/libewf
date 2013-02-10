/*
 * Debugs EWF files
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcstring.h"
#include "ewftools_libcsystem.h"
#include "ewftools_libewf.h"

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

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default),\n"
	                 "\t           windows-874, windows-932, windows-936, windows-949,\n"
	                 "\t           windows-950, windows-1250, windows-1251, windows-1252,\n"
	                 "\t           windows-1253, windows-1254, windows-1255, windows-1256,\n"
	                 "\t           windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-q:        quiet shows minimal status information\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
}

/* Signal handler for ewfdebug
 */
void ewfdebug_signal_handler(
      libcsystem_signal_t signal LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "ewfdebug_signal_handler";

	LIBCSYSTEM_UNREFERENCED_PARAMETER( signal )

	ewfdebug_abort = 1;

	if( ( ewfdebug_input_handle != NULL )
	 && ( libewf_handle_signal_abort(
	       ewfdebug_input_handle,
	       &error ) != 1 ) )
	{
		libcnotify_printf(
		 "%s: unable to signal input handle to abort.\n",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return;
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libcsystem_file_io_close(
	     0 ) != 0 )
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if !defined( LIBCSYSTEM_HAVE_GLOB )
	libcsystem_glob_t *glob                                = NULL;
#endif

	libcerror_error_t *error                               = NULL;

	libcstring_system_character_t * const *argv_filenames = NULL;
	libcstring_system_character_t **ewf_filenames         = NULL;

	libcstring_system_character_t *option_header_codepage = NULL;
	libcstring_system_character_t *program                = _LIBCSTRING_SYSTEM_STRING( "ewfdebug" );

	libcstring_system_integer_t option                    = 0;
	size_t first_filename_length                          = 0;
	uint8_t verbose                                       = 0;
	int number_of_filenames                               = 0;
	int header_codepage                                   = LIBEWF_CODEPAGE_ASCII;
	int result                                            = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "ewftools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( libcsystem_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		ewfoutput_version_fprint(
		 stdout,
		 program );

		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		goto on_error;
	}
	while( ( option = libcsystem_getopt(
			   argc,
			   argv,
			   _LIBCSTRING_SYSTEM_STRING( "A:hqvV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				ewfoutput_version_fprint(
				 stdout,
				 program );

				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'A':
				option_header_codepage = optarg;

				break;

			case (libcstring_system_integer_t) 'h':
				ewfoutput_version_fprint(
				 stdout,
				 program );

				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'q':
				break;

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				ewfoutput_version_fprint(
				 stdout,
				 program );

				ewfoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		ewfoutput_version_fprint(
		 stdout,
		 program );

		fprintf(
		 stderr,
		 "Missing EWF image file(s).\n" );

		usage_fprint(
		 stdout );

		goto on_error;
	}
	ewfoutput_version_fprint(
	 stdout,
	 program );

	libcnotify_verbose_set(
	 verbose );

#if !defined( HAVE_LOCAL_LIBEWF )
	libewf_notify_set_verbose(
	 verbose );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif

	if( option_header_codepage != NULL )
	{
		if( ewfinput_determine_header_codepage(
		     option_header_codepage,
		     &header_codepage,
		     &error ) != 1 )
		{
			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );

			fprintf(
			 stderr,
			 "Unsupported header codepage defaulting to: ascii.\n" );

			header_codepage = LIBEWF_CODEPAGE_ASCII;
		}
	}
	if( libcsystem_signal_attach(
	     ewfdebug_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if !defined( LIBCSYSTEM_HAVE_GLOB )
	if( libcsystem_glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize glob.\n" );

		goto on_error;
	}
	if( libcsystem_glob_resolve(
	     glob,
	     &( argv[ optind ] ),
	     argc - optind,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to resolve glob.\n" );

		goto on_error;
	}
	argv_filenames      = glob->result;
	number_of_filenames = glob->number_of_results;
#else
	argv_filenames      = &( argv[ optind ] );
	number_of_filenames = argc - optind;
#endif

	if( number_of_filenames == 1 )
	{
		first_filename_length = libcstring_system_string_length(
		                         argv_filenames[ 0 ] );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_glob_wide(
		     argv_filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &ewf_filenames,
		     &number_of_filenames,
		     &error ) != 1 )
#else
		if( libewf_glob(
		     argv_filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &ewf_filenames,
		     &number_of_filenames,
		     &error ) != 1 )
#endif
		{
			fprintf(
			 stderr,
			 "Unable to resolve ewf file(s).\n" );

			goto on_error;
		}
		argv_filenames = (libcstring_system_character_t * const *) ewf_filenames;
	}
	if( libewf_handle_initialize(
	     &ewfdebug_input_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize input handle.\n" );

		goto on_error;
	}
	result = libewf_handle_open(
	          ewfdebug_input_handle,
	          argv_filenames,
	          number_of_filenames,
	          LIBEWF_OPEN_READ_WRITE,
	          &error );

#if !defined( LIBCSYSTEM_HAVE_GLOB )
	if( libcsystem_glob_free(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
#endif
	if( ewf_filenames != NULL )
	{
		for( ; number_of_filenames > 0; number_of_filenames-- )
		{
			memory_free(
			 ewf_filenames[ number_of_filenames - 1 ] );
		}
		memory_free(
		 ewf_filenames );
	}
	if( ( ewfdebug_abort == 0 )
	 && ( result != 1 ) )
	{
		fprintf(
		 stderr,
		 "Unable to open EWF file(s).\n" );

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
	if( libcsystem_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( ewfdebug_abort != 0 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_LIBCSTRING_SYSTEM ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Debug completed.\n" );

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if !defined( LIBCSYSTEM_HAVE_GLOB )
	if( glob != NULL )
	{
		libcsystem_glob_free(
		 &glob,
		 NULL );
	}
#endif
	return( EXIT_FAILURE );
}

