/*
 * Shows information stored in an EWF file
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_SYS_RESOURCE_H )
#include <sys/resource.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_GLOB_H )
#include <glob.h>
#endif

#include "byte_size_string.h"
#include "ewfinput.h"
#include "ewftools_getopt.h"
#include "ewftools_glob.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libewf.h"
#include "ewftools_output.h"
#include "ewftools_signal.h"
#include "ewftools_unused.h"
#include "guid.h"
#include "info_handle.h"

enum EWFINFO_MODES
{
	EWFINFO_MODE_FILE_ENTRY,
	EWFINFO_MODE_FILE_SYSTEM_HIERARCHY,
	EWFINFO_MODE_IMAGE
};

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

	fprintf( stream, "Usage: ewfinfo [ -A codepage ] [ -B bodyfile ] [ -d date_format ]\n"
	                 "               [ -f format ]  [ -F path ] [ -ehHimvVx ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default),\n"
	                 "\t           windows-874, windows-932, windows-936, windows-949,\n"
	                 "\t           windows-950, windows-1250, windows-1251, windows-1252,\n"
	                 "\t           windows-1253, windows-1254, windows-1255, windows-1256,\n"
	                 "\t           windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-B:        output logical files information as a bodyfile\n" );
	fprintf( stream, "\t-d:        specify the date format, options: ctime (default),\n"
	                 "\t           dm (day/month), md (month/day), iso8601\n" );
	fprintf( stream, "\t-e:        only show EWF read error information\n" );
	fprintf( stream, "\t-f:        specify the output format, options: text (default),\n"
	                 "\t           dfxml\n" );
	fprintf( stream, "\t-F:        show information about a specific file entry path.\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-H:        shows the logical files hierarchy\n" );
	fprintf( stream, "\t-i:        only show EWF acquiry information\n" );
	fprintf( stream, "\t-m:        only show EWF media information\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
}

/* Signal handler for ewfinfo
 */
void ewfinfo_signal_handler(
      ewftools_signal_t signal EWFTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "ewfinfo_signal_handler";

	EWFTOOLS_UNREFERENCED_PARAMETER( signal )

	ewfinfo_abort = 1;

	if( ( ewfinfo_info_handle != NULL )
	 && ( info_handle_signal_abort(
	       ewfinfo_info_handle,
	       &error ) != 1 ) )
	{
		libcnotify_printf(
		 "%s: unable to signal info handle to abort.\n",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined( HAVE_GETRLIMIT )
	struct rlimit limit_data;
#endif

	system_character_t * const *source_filenames = NULL;
	libcerror_error_t *error                     = NULL;
	system_character_t *option_bodyfile          = NULL;
	system_character_t *option_date_format       = NULL;
	system_character_t *option_file_entry        = NULL;
	system_character_t *option_header_codepage   = NULL;
	system_character_t *option_output_format     = NULL;
	system_character_t *program                  = _SYSTEM_STRING( "ewfinfo" );
	system_integer_t option                      = 0;
	uint8_t verbose                              = 0;
	int number_of_filenames                      = 0;
	int option_mode                              = EWFINFO_MODE_IMAGE;
	int print_header                             = 1;
	int result                                   = 0;
	char info_option                             = 'a';

#if !defined( HAVE_GLOB_H )
	ewftools_glob_t *glob                        = NULL;
#endif

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
	if( ewftools_output_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		ewftools_output_version_fprint(
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	while( ( option = ewftools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "A:B:d:ef:F:hHimvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				ewftools_output_version_fprint(
				 stderr,
				 program );

				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				goto on_error;

			case (system_integer_t) 'A':
				option_header_codepage = optarg;

				break;

			case (system_integer_t) 'B':
				option_bodyfile = optarg;

				break;

			case (system_integer_t) 'd':
				option_date_format = optarg;

				break;

			case (system_integer_t) 'e':
				if( info_option != 'a' )
				{
					ewftools_output_version_fprint(
					 stderr,
					 program );

					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_SYSTEM " and %c\n",
					 option,
					 info_option );

					usage_fprint(
					 stdout );

					goto on_error;
				}
				info_option = 'e';

				break;

			case (system_integer_t) 'f':
				option_output_format = optarg;

				break;

			case (system_integer_t) 'F':
				option_mode       = EWFINFO_MODE_FILE_ENTRY;
				option_file_entry = optarg;

				break;

			case (system_integer_t) 'h':
				ewftools_output_version_fprint(
				 stdout,
				 program );

				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'H':
				option_mode = EWFINFO_MODE_FILE_SYSTEM_HIERARCHY;

				break;

			case (system_integer_t) 'i':
				if( info_option != 'a' )
				{
					ewftools_output_version_fprint(
					 stderr,
					 program );

					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_SYSTEM " and %c\n",
					 option,
					 info_option );

					usage_fprint(
					 stdout );

					goto on_error;
				}
				info_option = 'i';

				break;

			case (system_integer_t) 'm':
				if( info_option != 'a' )
				{
					ewftools_output_version_fprint(
					 stderr,
					 program );

					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_SYSTEM " and %c\n",
					 option,
					 info_option );

					usage_fprint(
					 stdout );

					goto on_error;
				}
				info_option = 'm';

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				ewftools_output_version_fprint(
				 stdout,
				 program );

				ewftools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		ewftools_output_version_fprint(
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Missing EWF image file(s).\n" );

		usage_fprint(
		 stdout );

		goto on_error;
	}
	libcnotify_verbose_set(
	 verbose );

#if !defined( HAVE_LOCAL_LIBEWF )
	libewf_notify_set_verbose(
	 verbose );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif

	if( info_handle_initialize(
	     &ewfinfo_info_handle,
	     &error ) != 1 )
	{
		ewftools_output_version_fprint(
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Unable to create info handle.\n" );

		goto on_error;
	}
	if( option_bodyfile != NULL )
	{
		if( info_handle_set_bodyfile(
		     ewfinfo_info_handle,
		     option_bodyfile,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set bodyfile.\n" );

			goto on_error;
		}
	}
	if( option_output_format != NULL )
	{
		result = info_handle_set_output_format(
		          ewfinfo_info_handle,
		          option_output_format,
		          &error );

		if( result == -1 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			fprintf(
			 stderr,
			 "Unable to set output format.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;

			fprintf(
			 stderr,
			 "Unsupported output format defaulting to: text.\n" );
		}
	}
	if( ewfinfo_info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		ewftools_output_version_fprint(
		 stdout,
		 program );

		print_header = 0;
	}
	if( ( option_output_format == NULL )
	 && ( option_date_format != NULL ) )
	{
		result = info_handle_set_date_format(
		          ewfinfo_info_handle,
		          option_date_format,
		          &error );

		if( result == -1 )
		{
			if( print_header != 0 )
			{
				ewftools_output_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unable to set date format.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( print_header != 0 )
			{
				ewftools_output_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unsupported date format defaulting to: ctime.\n" );
		}
	}
	if( option_header_codepage != NULL )
	{
		result = info_handle_set_header_codepage(
		          ewfinfo_info_handle,
		          option_header_codepage,
		          &error );

		if( result == -1 )
		{
			if( print_header != 0 )
			{
				ewftools_output_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unable to set header codepage in info handle.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( print_header != 0 )
			{
				ewftools_output_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unsupported header codepage defaulting to: ascii.\n" );
		}
	}
#if !defined( HAVE_GLOB_H )
	if( ewftools_glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to initialize glob.\n" );

		goto on_error;
	}
	if( ewftools_glob_resolve(
	     glob,
	     &( argv[ optind ] ),
	     argc - optind,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to resolve glob.\n" );

		goto on_error;
	}
	if( ewftools_glob_get_results(
	     glob,
	     &number_of_filenames,
	     (system_character_t ***) &source_filenames,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve glob results.\n" );

		goto on_error;
	}
#else
	source_filenames    = &( argv[ optind ] );
	number_of_filenames = argc - optind;

#endif
#if defined( HAVE_GETRLIMIT )
	if( getrlimit(
            RLIMIT_NOFILE,
            &limit_data ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to determine limit: number of open file descriptors.\n" );
	}
	if( limit_data.rlim_max > (rlim_t) INT_MAX )
	{
		limit_data.rlim_max = (rlim_t) INT_MAX;
	}
	if( limit_data.rlim_max > 0 )
	{
		limit_data.rlim_max /= 2;
	}
	if( info_handle_set_maximum_number_of_open_handles(
	     ewfinfo_info_handle,
	     (int) limit_data.rlim_max,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set maximum number of open file handles.\n" );

		goto on_error;
	}
#endif /* defined( HAVE_GETRLIMIT ) */

	if( ewftools_signal_attach(
	     ewfinfo_signal_handler,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		goto on_error;
	}
	result = info_handle_open_input(
	          ewfinfo_info_handle,
	          source_filenames,
	          number_of_filenames,
	          &error );

	if( ewfinfo_abort != 0 )
	{
		goto on_abort;
	}
	if( result != 1 )
	{
		if( print_header != 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to open EWF file(s).\n" );

		goto on_error;
	}
#if !defined( HAVE_GLOB_H )
	if( ewftools_glob_free(
	     &glob,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
#endif
	switch( option_mode )
	{
		case EWFINFO_MODE_FILE_ENTRY:
			if( info_handle_file_entry_fprint_by_path(
			     ewfinfo_info_handle,
			     option_file_entry,
			     &error ) != 1 )
			{
				if( print_header != 0 )
				{
					ewftools_output_version_fprint(
					 stderr,
					 program );

					print_header = 0;
				}
				fprintf(
				 stderr,
				 "Unable to print file entry information.\n" );

				goto on_error;
			}
			break;

		case EWFINFO_MODE_FILE_SYSTEM_HIERARCHY:
			if( info_handle_logical_files_hierarchy_fprint(
			     ewfinfo_info_handle,
			     &error ) != 1 )
			{
				if( print_header != 0 )
				{
					ewftools_output_version_fprint(
					 stderr,
					 program );

					print_header = 0;
				}
				fprintf(
				 stderr,
				 "Unable to print logical files hierarchy.\n" );

				goto on_error;
			}
			break;

		case EWFINFO_MODE_IMAGE:
			if( info_handle_image_fprint(
			     ewfinfo_info_handle,
			     info_option,
			     &error ) != 1 )
			{
				if( print_header != 0 )
				{
					ewftools_output_version_fprint(
					 stderr,
					 program );

					print_header = 0;
				}
				fprintf(
				 stderr,
				 "Unable to print image.\n" );

				goto on_error;
			}
			break;
	}
on_abort:
	if( info_handle_close(
	     ewfinfo_info_handle,
	     &error ) != 0 )
	{
		if( print_header != 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( ewftools_signal_detach(
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		goto on_error;
	}
	if( info_handle_free(
	     &ewfinfo_info_handle,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		goto on_error;
	}
	if( ewfinfo_abort != 0 )
	{
		if( print_header != 0 )
		{
			ewftools_output_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stdout,
		 "%" PRIs_SYSTEM ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( ewfinfo_info_handle != NULL )
	{
		info_handle_free(
		 &ewfinfo_info_handle,
		 NULL );
	}
#if !defined( HAVE_GLOB_H )
	if( glob != NULL )
	{
		ewftools_glob_free(
		 &glob,
		 NULL );
	}
#endif
	return( EXIT_FAILURE );
}

