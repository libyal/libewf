/*
 * Verifies the integrity of the media data within the EWF file
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
#include "digest_hash.h"
#include "ewfcommon.h"
#include "ewftools_getopt.h"
#include "ewftools_glob.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcthreads.h"
#include "ewftools_libewf.h"
#include "ewftools_output.h"
#include "ewftools_signal.h"
#include "ewftools_unused.h"
#include "log_handle.h"
#include "verification_handle.h"

verification_handle_t *ewfverify_verification_handle = NULL;
int ewfverify_abort                                  = 0;

/* Prints the executable usage information to the stream
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use ewfverify to verify data stored in the EWF format (Expert Witness\n"
	                 "Compression Format).\n\n" );

	fprintf( stream, "Usage: ewfverify [ -A codepage ] [ -d digest_type ] [ -f format ]\n"
	                 "                 [ -j jobs ] [ -l log_filename ] [ -p process_buffer_size ]\n"
	                 "                 [ -hqvVwx ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default),\n"
	                 "\t           windows-874, windows-932, windows-936, windows-949,\n"
	                 "\t           windows-950, windows-1250, windows-1251, windows-1252,\n"
	                 "\t           windows-1253, windows-1254, windows-1255, windows-1256,\n"
	                 "\t           windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-d:        calculate additional digest (hash) types besides md5,\n"
	                 "\t           options: sha1, sha256\n" );
	fprintf( stream, "\t-f:        specify the input format, options: raw (default),\n"
	                 "\t           files (restricted to logical volume files)\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-j:        the number of concurrent processing jobs (threads), where\n"
	                 "\t           a number of 0 represents single-threaded mode (default is 4\n"
	                 "\t           if multi-threaded mode is supported)\n" );
	fprintf( stream, "\t-l:        logs verification errors and the digest (hash) to the\n"
	                 "\t           log_filename\n" );
	fprintf( stream, "\t-p:        specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-q:        quiet shows minimal status information\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
	fprintf( stream, "\t-w:        zero sectors on checksum error (mimic EnCase like behavior)\n" );
	fprintf( stream, "\t-x:        use the data chunk funcsion instead of the buffered read and\n"
	                 "\t           write functions.\n" );
}

/* Signal handler for ewfverify
 */
void ewfverify_signal_handler(
      ewftools_signal_t signal EWFTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "ewfverify_signal_handler";

	EWFTOOLS_UNREFERENCED_PARAMETER( signal )

	ewfverify_abort = 1;

	if( ewfverify_verification_handle != NULL )
	{
		if( verification_handle_signal_abort(
		     ewfverify_verification_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal verification handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
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

	system_character_t * const *source_filenames       = NULL;
	libcerror_error_t *error                           = NULL;
	log_handle_t *log_handle                           = NULL;
	system_character_t *log_filename                   = NULL;
	system_character_t *option_additional_digest_types = NULL;
	system_character_t *option_format                  = NULL;
	system_character_t *option_header_codepage         = NULL;
	system_character_t *option_number_of_jobs          = NULL;
	system_character_t *option_process_buffer_size     = NULL;
	system_character_t *program                        = _SYSTEM_STRING( "ewfverify" );
	system_integer_t option                            = 0;
	uint8_t calculate_md5                              = 1;
	uint8_t print_status_information                   = 1;
	uint8_t use_data_chunk_functions                   = 0;
	uint8_t verbose                                    = 0;
	uint8_t zero_chunk_on_error                        = 0;
	int number_of_filenames                            = 0;
	int result                                         = 0;

#if !defined( HAVE_GLOB_H )
	ewftools_glob_t *glob                            = NULL;
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
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	ewftools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = ewftools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "A:d:f:j:hl:p:qvVwx" ) ) ) != (system_integer_t) -1 )
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

				goto on_error;

			case (system_integer_t) 'A':
				option_header_codepage = optarg;

				break;

			case (system_integer_t) 'd':
				option_additional_digest_types = optarg;

				break;

			case (system_integer_t) 'f':
				option_format = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'j':
				option_number_of_jobs = optarg;

				break;

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'p':
				option_process_buffer_size = optarg;

				break;

			case (system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				ewftools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'w':
				zero_chunk_on_error = 1;

				break;

			case (system_integer_t) 'x':
				use_data_chunk_functions = 1;

				break;
		}
	}
	if( optind == argc )
	{
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
	if( verification_handle_initialize(
	     &ewfverify_verification_handle,
	     calculate_md5,
	     use_data_chunk_functions,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create verification handle.\n" );

		goto on_error;
	}
	if( option_header_codepage != NULL )
	{
		result = verification_handle_set_header_codepage(
			  ewfverify_verification_handle,
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
	if( option_format != NULL )
	{
		result = verification_handle_set_format(
			  ewfverify_verification_handle,
			  option_format,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set format.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported input format defaulting to: raw.\n" );
		}
	}
	if( option_process_buffer_size != NULL )
	{
		result = verification_handle_set_process_buffer_size(
			  ewfverify_verification_handle,
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
		      || ( ewfverify_verification_handle->process_buffer_size > (size_t) SSIZE_MAX ) )
		{
			ewfverify_verification_handle->process_buffer_size = 0;

			fprintf(
			 stderr,
			 "Unsupported process buffer size defaulting to: chunk size.\n" );
		}
	}
	if( option_number_of_jobs != NULL )
	{
#if defined( HAVE_MULTI_THREAD_SUPPORT )
		result = verification_handle_set_number_of_threads(
			  ewfverify_verification_handle,
			  option_number_of_jobs,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set number of jobs (threads).\n" );

			goto on_error;
		}
		else if( ( result == 0 )
		      || ( ewfverify_verification_handle->number_of_threads > (int) 32 ) )
		{
			ewfverify_verification_handle->number_of_threads = 4;

			fprintf(
			 stderr,
			 "Unsupported number of jobs (threads) defaulting to: %d.\n",
			 ewfverify_verification_handle->number_of_threads );
		}
#else
		ewfverify_verification_handle->number_of_threads = 0;

		fprintf(
		 stderr,
		 "Unsupported number of jobs (threads) defaulting to: %d.\n",
		 ewfverify_verification_handle->number_of_threads );
#endif
	}
	if( option_additional_digest_types != NULL )
	{
		result = verification_handle_set_additional_digest_types(
			  ewfverify_verification_handle,
			  option_additional_digest_types,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set additional digest types.\n" );

			goto on_error;
		}
	}
#if !defined( HAVE_GLOB_H )
	if( ewftools_glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
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
	if( verification_handle_set_maximum_number_of_open_handles(
	     ewfverify_verification_handle,
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
	     ewfverify_signal_handler,
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
	result = verification_handle_open_input(
	          ewfverify_verification_handle,
	          source_filenames,
	          number_of_filenames,
	          &error );

	if( ewfverify_abort != 0 )
	{
		goto on_abort;
	}
	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open EWF image file(s).\n" );

		goto on_error;
	}
#if !defined( HAVE_GLOB_H )
	if( ewftools_glob_free(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
#endif
	if( verification_handle_set_zero_chunk_on_error(
	     ewfverify_verification_handle,
	     zero_chunk_on_error,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set zero on chunk error.\n" );

		goto on_error;
	}
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
			 "Unable to open log file: %" PRIs_SYSTEM ".\n",
			 log_filename );

			goto on_error;
		}
	}
	if( ewfverify_verification_handle->input_format == VERIFICATION_HANDLE_INPUT_FORMAT_FILES )
	{
		result = verification_handle_verify_single_files(
		          ewfverify_verification_handle,
		          print_status_information,
		          log_handle,
		          &error );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to verify single files.\n" );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	else
	{
		result = verification_handle_verify_input(
		          ewfverify_verification_handle,
		          print_status_information,
		          log_handle,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to verify input.\n" );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	if( log_handle != NULL )
	{
		if( log_handle_close(
		     log_handle,
		     &error ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to close log handle.\n" );

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
	if( ewftools_signal_detach(
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
	if( verification_handle_close(
	     ewfverify_verification_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close verification handle.\n" );

		goto on_error;

	}
	if( verification_handle_free(
	     &ewfverify_verification_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free verification handle.\n" );

		goto on_error;
	}
	if( ewfverify_abort != 0 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_SYSTEM ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	if( result != 1 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_SYSTEM ": FAILURE\n",
		 program );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "%" PRIs_SYSTEM ": SUCCESS\n",
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
	if( ewfverify_verification_handle != NULL )
	{
		verification_handle_close(
		 ewfverify_verification_handle,
		 NULL );
		verification_handle_free(
		 &ewfverify_verification_handle,
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

