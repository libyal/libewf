/*
 * Exports corrupt EWF files to new files
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

#include "ewfcommon.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcstring.h"
#include "ewftools_libcsystem.h"
#include "ewftools_libewf.h"
#include "export_handle.h"
#include "log_handle.h"
#include "platform.h"
#include "storage_media_buffer.h"

#define EWFRECOVER_INPUT_BUFFER_SIZE		64

export_handle_t *ewfrecover_export_handle = NULL;
int ewfrecover_abort                      = 0;

/* Prints the executable usage information to the stream
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use ewfrecover to recover data from corrupt EWF (Expert Witness\n"
	                 "Compression Format) files.\n\n" );

	fprintf( stream, "Usage: ewfrecover [ -A codepage ]\n"
	                 "                  [ -l log_filename ]\n"
	                 "                  [ -p process_buffer_size ]\n"
	                 "                  [ -t target ] [ -hquvV ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default),\n"
	                 "\t           windows-874, windows-932, windows-936, windows-949,\n"
	                 "\t           windows-950, windows-1250, windows-1251, windows-1252,\n"
	                 "\t           windows-1253, windows-1254, windows-1255, windows-1256,\n"
	                 "\t           windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-l:        logs recover errors and the digest (hash) to the\n"
	                 "\t           log_filename\n" );
	fprintf( stream, "\t-p:        specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-q:        quiet shows minimal status information\n" );
	fprintf( stream, "\t-t:        specify the target file to recover to (default is recover)\n" );
	fprintf( stream, "\t-u:        unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
}

/* Signal handler for ewfrecover
 */
void ewfrecover_signal_handler(
      libcsystem_signal_t signal LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "ewfrecover_signal_handler";

	LIBCSYSTEM_UNREFERENCED_PARAMETER( signal )

	ewfrecover_abort = 1;

	if( ewfrecover_export_handle != NULL )
	{
		if( export_handle_signal_abort(
		     ewfrecover_export_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal export handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
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
	libcstring_system_character_t acquiry_operating_system[ 32 ];

	libcstring_system_character_t * const *argv_filenames      = NULL;

	libcerror_error_t *error                                   = NULL;

#if !defined( LIBCSYSTEM_HAVE_GLOB )
	libcsystem_glob_t *glob                                    = NULL;
#endif

	libcstring_system_character_t *acquiry_software_version    = NULL;
	libcstring_system_character_t *log_filename                = NULL;
	libcstring_system_character_t *option_header_codepage      = NULL;
	libcstring_system_character_t *option_process_buffer_size  = NULL;
	libcstring_system_character_t *option_target_path          = NULL;
	libcstring_system_character_t *program                     = _LIBCSTRING_SYSTEM_STRING( "ewfrecover" );

	log_handle_t *log_handle                                   = NULL;

	libcstring_system_integer_t option                         = 0;
	uint8_t calculate_md5                                      = 1;
	uint8_t print_status_information                           = 1;
	uint8_t verbose                                            = 0;
	int number_of_filenames                                    = 0;
	int result                                                 = 1;

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
#if defined( WINAPI ) && !defined( __CYGWIN__ )
#if defined( _MSC_VER )
	if( _setmode(
	     _fileno(
	      stdout ),
	     _O_BINARY ) == -1 )
#else
	if( setmode(
	     _fileno(
	      stdout ),
	     _O_BINARY ) == -1 )
#endif
	{
		ewfoutput_version_fprint(
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Unable to set stdout to binary mode.\n" );

		usage_fprint(
		 stdout );

		goto on_error;
	}
#endif
	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "A:f:hl:p:qt:uvV" ) ) ) != (libcstring_system_integer_t) -1 )
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
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stderr );

				goto on_error;

			case (libcstring_system_integer_t) 'A':
				option_header_codepage = optarg;

				break;

			case (libcstring_system_integer_t) 'h':
				ewfoutput_version_fprint(
				 stderr,
				 program );

				usage_fprint(
				 stderr );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libcstring_system_integer_t) 'p':
				option_process_buffer_size = optarg;

				break;

			case (libcstring_system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (libcstring_system_integer_t) 't':
				option_target_path = optarg;

				break;

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				ewfoutput_version_fprint(
				 stderr,
				 program );

				ewfoutput_copyright_fprint(
				 stderr );

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
		 stderr );

		goto on_error;
	}
	ewfoutput_version_fprint(
	 stderr,
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

	if( export_handle_initialize(
	     &ewfrecover_export_handle,
	     calculate_md5,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create export handle.\n" );

		goto on_error;
	}
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
	if( export_handle_set_maximum_number_of_open_handles(
	     ewfrecover_export_handle,
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
	     ewfrecover_signal_handler,
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
	result = export_handle_open_input(
	          ewfrecover_export_handle,
	          argv_filenames,
	          number_of_filenames,
	          &error );

	if( ewfrecover_abort != 0 )
	{
		goto on_abort;
	}
	if( result != 1 )
	{
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
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
#endif
	result = export_handle_input_is_corrupted(
	          ewfrecover_export_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to determine if EWF file(s) are corrupted.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "EWF file(s) are not corrupted.\n" );

		goto on_error;
	}
	ewfrecover_export_handle->output_format = EXPORT_HANDLE_OUTPUT_FORMAT_EWF;
	ewfrecover_export_handle->export_size   = ewfrecover_export_handle->input_media_size;

	if( option_header_codepage != NULL )
	{
		result = export_handle_set_header_codepage(
			  ewfrecover_export_handle,
		          option_header_codepage,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set header codepage.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported header codepage defaulting to: ascii.\n" );
		}
	}
	if( option_target_path != NULL )
	{
		if( export_handle_set_string(
		     ewfrecover_export_handle,
		     option_target_path,
		     &( ewfrecover_export_handle->target_path ),
		     &( ewfrecover_export_handle->target_path_size ),
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set target path.\n" );

			goto on_error;
		}
	}
	else
	{
		/* Make sure the target filename is set in unattended mode
		 */
		if( export_handle_set_string(
		     ewfrecover_export_handle,
		     _LIBCSTRING_SYSTEM_STRING( "recover" ),
		     &( ewfrecover_export_handle->target_path ),
		     &( ewfrecover_export_handle->target_path_size ),
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set target filename.\n" );

			goto on_error;
		}
	}
	if( option_process_buffer_size != NULL )
	{
		result = export_handle_set_process_buffer_size(
			  ewfrecover_export_handle,
			  option_process_buffer_size,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set process buffer size.\n" );

			goto on_error;
		}
		else if( ( result == 0 )
		      || ( ewfrecover_export_handle->process_buffer_size > (size_t) SSIZE_MAX ) )
		{
			ewfrecover_export_handle->process_buffer_size = 0;

			fprintf(
			 stderr,
			 "Unsupported process buffer size defaulting to: chunk size.\n" );
		}
	}
	/* Initialize values
	 */
	if( log_filename != NULL )
	{
		if( log_handle_initialize(
		     &log_handle,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to create log handle.\n" );

			goto on_error;
		}
		if( log_handle_open(
		     log_handle,
		     log_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to open log file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
			 log_filename );

			goto on_error;
		}
	}
	if( export_handle_open_output(
	     ewfrecover_export_handle,
	     ewfrecover_export_handle->target_path,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open output.\n" );

		goto on_error;
	}
	if( platform_get_operating_system(
	     acquiry_operating_system,
	     32,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to determine operating system.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		acquiry_operating_system[ 0 ] = 0;
	}
	acquiry_software_version = _LIBCSTRING_SYSTEM_STRING( LIBEWF_VERSION_STRING );

	if( export_handle_set_output_values(
	     ewfrecover_export_handle,
	     acquiry_operating_system,
	     program,
	     acquiry_software_version,
	     0,
	     1,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set output values.\n" );

		goto on_error;
	}
	result = export_handle_export_input(
		  ewfrecover_export_handle,
		  0,
		  print_status_information,
		  log_handle,
		  &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to recover input.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( log_handle != NULL )
	{
		if( log_handle_close(
		     log_handle,
		     &error ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to close log file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
			 log_filename );

			goto on_error;
		}
		if( log_handle_free(
		     &log_handle,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to free log handle.\n" );

			goto on_error;
		}
	}
on_abort:
	if( export_handle_close(
	     ewfrecover_export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close export handle.\n" );

		goto on_error;
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
	if( export_handle_free(
	     &ewfrecover_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		goto on_error;
	}
	if( ewfrecover_abort != 0 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_LIBCSTRING_SYSTEM ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	if( result != 1 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_LIBCSTRING_SYSTEM ": FAILURE\n",
		 program );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "%" PRIs_LIBCSTRING_SYSTEM ": SUCCESS\n",
	 program );

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( log_handle != NULL )
	{
		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );
	}
	if( ewfrecover_export_handle != NULL )
	{
		export_handle_close(
		 ewfrecover_export_handle,
		 NULL );
		export_handle_free(
		 &ewfrecover_export_handle,
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

