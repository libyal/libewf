/*
 * Shows information stored in an EWF file
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

#if defined( HAVE_SYS_RESOURCE_H )
#include <sys/resource.h>
#endif

#include "byte_size_string.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcstring.h"
#include "ewftools_libcsystem.h"
#include "ewftools_libewf.h"
#include "guid.h"
#include "info_handle.h"

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

	fprintf( stream, "Usage: ewfinfo [ -A codepage ] [ -d date_format ] [ -f format ]\n"
	                 "               [ -ehimvVx ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default),\n"
	                 "\t           windows-874, windows-932, windows-936, windows-949,\n"
	                 "\t           windows-950, windows-1250, windows-1251, windows-1252,\n"
	                 "\t           windows-1253, windows-1254, windows-1255, windows-1256,\n"
	                 "\t           windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-d:        specify the date format, options: ctime (default),\n"
	                 "\t           dm (day/month), md (month/day), iso8601\n" );
	fprintf( stream, "\t-e:        only show EWF read error information\n" );
	fprintf( stream, "\t-f:        specify the output format, options: text (default),\n"
	                 "\t           dfxml\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-i:        only show EWF acquiry information\n" );
	fprintf( stream, "\t-m:        only show EWF media information\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
}

/* Signal handler for ewfinfo
 */
void ewfinfo_signal_handler(
      libcsystem_signal_t signal LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "ewfinfo_signal_handler";

	LIBCSYSTEM_UNREFERENCED_PARAMETER( signal )

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
#if defined( HAVE_GETRLIMIT )
	struct rlimit limit_data;
#endif
	libcstring_system_character_t * const *argv_filenames = NULL;

#if !defined( LIBCSYSTEM_HAVE_GLOB )
	libcsystem_glob_t *glob                               = NULL;
#endif
	libcerror_error_t *error                              = NULL;

	libcstring_system_character_t *option_date_format     = NULL;
	libcstring_system_character_t *option_header_codepage = NULL;
	libcstring_system_character_t *option_output_format   = NULL;
	libcstring_system_character_t *program                = _LIBCSTRING_SYSTEM_STRING( "ewfinfo" );

	libcstring_system_integer_t option                    = 0;
	uint8_t verbose                                       = 0;
	char info_option                                      = 'a';
	int number_of_filenames                               = 0;
	int print_header                                      = 1;
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
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		goto on_error;
	}
	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "A:d:ef:himvV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				ewfoutput_version_fprint(
				 stderr,
				 program );

				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				goto on_error;

			case (libcstring_system_integer_t) 'A':
				option_header_codepage = optarg;

				break;

			case (libcstring_system_integer_t) 'd':
				option_date_format = optarg;

				break;

			case (libcstring_system_integer_t) 'e':
				if( info_option != 'a' )
				{
					ewfoutput_version_fprint(
					 stderr,
					 program );

					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_LIBCSTRING_SYSTEM " and %c\n",
					 option,
					 info_option );

					usage_fprint(
					 stdout );

					goto on_error;
				}
				info_option = 'e';

				break;

			case (libcstring_system_integer_t) 'f':
				option_output_format = optarg;

				break;

			case (libcstring_system_integer_t) 'h':
				ewfoutput_version_fprint(
				 stdout,
				 program );

				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'i':
				if( info_option != 'a' )
				{
					ewfoutput_version_fprint(
					 stderr,
					 program );

					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_LIBCSTRING_SYSTEM " and %c\n",
					 option,
					 info_option );

					usage_fprint(
					 stdout );

					goto on_error;
				}
				info_option = 'i';

				break;

			case (libcstring_system_integer_t) 'm':
				if( info_option != 'a' )
				{
					ewfoutput_version_fprint(
					 stderr,
					 program );

					fprintf(
					 stderr,
					 "Conflicting options: %" PRIc_LIBCSTRING_SYSTEM " and %c\n",
					 option,
					 info_option );

					usage_fprint(
					 stdout );

					goto on_error;
				}
				info_option = 'm';

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
		ewfoutput_version_fprint(
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Unable to create info handle.\n" );

		goto on_error;
	}
	if( option_output_format != NULL )
	{
		result = info_handle_set_output_format(
		          ewfinfo_info_handle,
		          option_output_format,
		          &error );

		if( result == -1 )
		{
			ewfoutput_version_fprint(
			 stderr,
			 program );

			fprintf(
			 stderr,
			 "Unable to set output format.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			ewfoutput_version_fprint(
			 stderr,
			 program );

			print_header = 0;

			fprintf(
			 stderr,
			 "Unsupported output format defaulting to: text.\n" );
		}
	}
	if( ewfinfo_info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		if( info_handle_dfxml_header_fprint(
		     ewfinfo_info_handle,
		     &error ) != 1 )
		{
			ewfoutput_version_fprint(
			 stderr,
			 program );

			fprintf(
			 stderr,
			 "Unable to print header.\n" );

			goto on_error;
		}
	}
	else if( ewfinfo_info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		ewfoutput_version_fprint(
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
				ewfoutput_version_fprint(
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
				ewfoutput_version_fprint(
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
				ewfoutput_version_fprint(
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
				ewfoutput_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unsupported header codepage defaulting to: ascii.\n" );
		}
	}
#if !defined( LIBCSYSTEM_HAVE_GLOB )
	if( libcsystem_glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewfoutput_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
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
		if( print_header != 0 )
		{
			ewfoutput_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
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
#endif
	if( libcsystem_signal_attach(
	     ewfinfo_signal_handler,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewfoutput_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	result = info_handle_open_input(
	          ewfinfo_info_handle,
	          argv_filenames,
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
			ewfoutput_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to open EWF file(s).\n" );

		goto on_error;
	}
#if !defined( LIBCSYSTEM_HAVE_GLOB )
	if( libcsystem_glob_free(
	     &glob,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewfoutput_version_fprint(
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
	if( ( info_option == 'a' )
	 || ( info_option == 'i' ) )
	{
		if( info_handle_header_values_fprint(
		     ewfinfo_info_handle,
		     &error ) != 1 )
		{
			if( print_header != 0 )
			{
				ewfoutput_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unable to print header values.\n" );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	if( ( info_option == 'a' )
	 || ( info_option == 'm' ) )
	{
		if( info_handle_media_information_fprint(
		     ewfinfo_info_handle,
		     &error ) != 1 )
		{
			if( print_header != 0 )
			{
				ewfoutput_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unable to print media information.\n" );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
		if( info_handle_hash_values_fprint(
		     ewfinfo_info_handle,
		     &error ) != 1 )
		{
			if( print_header != 0 )
			{
				ewfoutput_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unable to print hash values.\n" );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
		if( info_handle_sessions_fprint(
		     ewfinfo_info_handle,
		     &error ) != 1 )
		{
			if( print_header != 0 )
			{
				ewfoutput_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unable to print sessions.\n" );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
		if( info_handle_tracks_fprint(
		     ewfinfo_info_handle,
		     &error ) != 1 )
		{
			if( print_header != 0 )
			{
				ewfoutput_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unable to print tracks.\n" );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	if( ( info_option == 'a' )
	 || ( info_option == 'e' ) )
	{
		if( info_handle_acquiry_errors_fprint(
		     ewfinfo_info_handle,
		     &error ) != 1 )
		{
			if( print_header != 0 )
			{
				ewfoutput_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unable to print acquiry errors.\n" );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	if( info_handle_single_files_fprint(
	     ewfinfo_info_handle,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewfoutput_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to print single files.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( ewfinfo_info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		if( info_handle_dfxml_footer_fprint(
		     ewfinfo_info_handle,
		     &error ) != 1 )
		{
			if( print_header != 0 )
			{
				ewfoutput_version_fprint(
				 stderr,
				 program );

				print_header = 0;
			}
			fprintf(
			 stderr,
			 "Unable to print footer.\n" );

			goto on_error;
		}
	}
on_abort:
	if( info_handle_close(
	     ewfinfo_info_handle,
	     &error ) != 0 )
	{
		if( print_header != 0 )
		{
			ewfoutput_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( libcsystem_signal_detach(
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewfoutput_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( info_handle_free(
	     &ewfinfo_info_handle,
	     &error ) != 1 )
	{
		if( print_header != 0 )
		{
			ewfoutput_version_fprint(
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
			ewfoutput_version_fprint(
			 stderr,
			 program );

			print_header = 0;
		}
		fprintf(
		 stdout,
		 "%" PRIs_LIBCSTRING_SYSTEM ": ABORTED\n",
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

