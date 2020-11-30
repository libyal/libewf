/*
 * Export media data from EWF files to a file
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_SYS_RESOURCE_H )
#include <sys/resource.h>
#endif

#if defined( HAVE_FCNTL_H ) || defined( WINAPI )
#include <fcntl.h>
#endif

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_GLOB_H )
#include <glob.h>
#endif

#include "byte_size_string.h"
#include "ewfcommon.h"
#include "ewfinput.h"
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
#include "export_handle.h"
#include "log_handle.h"
#include "platform.h"

#define EWFEXPORT_INPUT_BUFFER_SIZE		64

export_handle_t *ewfexport_export_handle = NULL;
int ewfexport_abort                      = 0;

/* Prints the executable usage information to the stream
 */
void usage_fprint(
      FILE *stream )
{
	system_character_t default_segment_file_size_string[ 16 ];
	system_character_t minimum_segment_file_size_string[ 16 ];
	system_character_t maximum_32bit_segment_file_size_string[ 16 ];
	system_character_t maximum_64bit_segment_file_size_string[ 16 ];

	int result = 0;

	if( stream == NULL )
	{
		return;
	}
	result = byte_size_string_create(
	          default_segment_file_size_string,
	          16,
	          EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	if( result == 1 )
	{
		result = byte_size_string_create(
			  minimum_segment_file_size_string,
			  16,
			  EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );
	}
	if( result == 1 )
	{
		result = byte_size_string_create(
			  maximum_32bit_segment_file_size_string,
			  16,
			  EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );
	}
	if( result == 1 )
	{
		result = byte_size_string_create(
			  maximum_64bit_segment_file_size_string,
			  16,
			  EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );
	}
	fprintf( stream, "Use ewfexport to export data from the EWF format (Expert Witness Compression\n"
	                 "Format) to raw data or another EWF format.\n\n" );

	fprintf( stream, "Usage: ewfexport [ -A codepage ] [ -b number_of_sectors ]\n"
	                 "                 [ -B number_of_bytes ] [ -c compression_values ]\n"
	                 "                 [ -d digest_type ] [ -f format ] [ -j jobs ] [ -l log_filename ]\n"
	                 "                 [ -o offset ] [ -p process_buffer_size ]\n"
	                 "                 [ -S segment_file_size ] [ -t target ] [ -hqsuvVwx ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default),\n"
	                 "\t           windows-874, windows-932, windows-936, windows-949,\n"
	                 "\t           windows-950, windows-1250, windows-1251, windows-1252,\n"
	                 "\t           windows-1253, windows-1254, windows-1255, windows-1256,\n"
	                 "\t           windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-b:        specify the number of sectors to read at once (per chunk),\n"
	                 "\t           options: 16, 32, 64 (default), 128, 256, 512, 1024, 2048,\n"
	                 "\t           4096, 8192, 16384 or 32768 (not used for raw and files\n"
	                 "\t           formats)\n" );
	fprintf( stream, "\t-B:        specify the number of bytes to export (default is all bytes)\n" );
	fprintf( stream, "\t-c:        specify the compression values as: level or method:level\n"
#if defined( HAVE_BZIP2_SUPPORT )
	                 "\t           compression method options: deflate (default), bzip2\n"
	                 "\t           (bzip2 is only supported by EWF2 formats)\n"
#else
	                 "\t           compression method options: deflate (default)\n"
#endif
	                 "\t           compression level options: none (default), empty-block,\n"
	                 "\t           fast or best\n" );
	fprintf( stream, "\t-d:        calculate additional digest (hash) types besides md5,\n"
	                 "\t           options: sha1, sha256 (not used for raw and files format)\n" );
	fprintf( stream, "\t-f:        specify the output format to write to, options:\n"
	                 "\t           raw (default), files (restricted to logical volume files), ewf,\n"
	                 "\t           smart, encase1, encase2, encase3, encase4, encase5, encase6,\n"
	                 "\t           encase7, encase7-v2, linen5, linen6, linen7, ewfx\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-j:        the number of concurrent processing jobs (threads), where\n"
	                 "\t           a number of 0 represents single-threaded mode (default is 4\n"
	                 "\t           if multi-threaded mode is supported)\n" );
	fprintf( stream, "\t-l:        logs export errors and the digest (hash) to the log_filename\n" );
	fprintf( stream, "\t-o:        specify the offset to start the export (default is 0)\n" );
	fprintf( stream, "\t-p:        specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-q:        quiet shows minimal status information\n" );
	fprintf( stream, "\t-s:        swap byte pairs of the media data (from AB to BA)\n"
	                 "\t           (use this for big to little endian conversion and vice\n"
	                 "\t           versa)\n" );

	if( result == 1 )
	{
		fprintf( stream, "\t-S:        specify the segment file size in bytes (default is %" PRIs_SYSTEM ")\n"
		                 "\t           (minimum is %" PRIs_SYSTEM ", maximum is %" PRIs_SYSTEM " for raw, encase6\n"
		                 "\t           and later formats and %" PRIs_SYSTEM " for other formats)\n"
		                 "\t           (not used for files format)\n",
		 default_segment_file_size_string,
		 minimum_segment_file_size_string,
		 maximum_64bit_segment_file_size_string,
		 maximum_32bit_segment_file_size_string );
	}
	else
	{
		fprintf( stream, "\t-S:        specify the segment file size in bytes (default is %" PRIu32 ")\n"
		                 "\t           (minimum is %" PRIu32 ", maximum is %" PRIu64 " for raw, encase6\n"
		                 "\t           and later formats and %" PRIu32 " for other formats)\n"
		                 "\t           (not used for files format)\n",
		 (uint32_t) EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
		 (uint32_t) EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
		 (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
		 (uint32_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT );
	}

	fprintf( stream, "\t-t:        specify the target file to export to, use - for stdout\n"
	                 "\t           (default is export) stdout is only supported for the raw\n"
	                 "\t           format\n" );
	fprintf( stream, "\t-u:        unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
	fprintf( stream, "\t-w:        zero sectors on checksum error (mimic EnCase like behavior)\n" );
	fprintf( stream, "\t-x:        use the data chunk functions instead of the buffered read and\n"
	                 "\t           write functions.\n" );
}

/* Signal handler for ewfexport
 */
void ewfexport_signal_handler(
      ewftools_signal_t signal EWFTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "ewfexport_signal_handler";

	EWFTOOLS_UNREFERENCED_PARAMETER( signal )

	ewfexport_abort = 1;

	if( ewfexport_export_handle != NULL )
	{
		if( export_handle_signal_abort(
		     ewfexport_export_handle,
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
	system_character_t acquiry_operating_system[ 32 ];

#if defined( HAVE_GETRLIMIT )
	struct rlimit limit_data;
#endif

	system_character_t * const *source_filenames       = NULL;
	libcerror_error_t *error                           = NULL;
	log_handle_t *log_handle                           = NULL;
	system_character_t *acquiry_software_version       = NULL;
	system_character_t *log_filename                   = NULL;
	system_character_t *option_additional_digest_types = NULL;
	system_character_t *option_compression_values      = NULL;
	system_character_t *option_format                  = NULL;
	system_character_t *option_header_codepage         = NULL;
	system_character_t *option_maximum_segment_size    = NULL;
	system_character_t *option_number_of_jobs          = NULL;
	system_character_t *option_offset                  = NULL;
	system_character_t *option_process_buffer_size     = NULL;
	system_character_t *option_sectors_per_chunk       = NULL;
	system_character_t *option_size                    = NULL;
	system_character_t *option_target_path             = NULL;
	system_character_t *program                        = _SYSTEM_STRING( "ewfexport" );
	system_character_t *request_string                 = NULL;
	system_integer_t option                            = 0;
	uint8_t calculate_md5                              = 1;
	uint8_t print_status_information                   = 1;
	uint8_t swap_byte_pairs                            = 0;
	uint8_t use_data_chunk_functions                   = 0;
	uint8_t verbose                                    = 0;
	uint8_t zero_chunk_on_error                        = 0;
	int interactive_mode                               = 1;
	int number_of_filenames                            = 0;
	int result                                         = 1;

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
		ewftools_output_version_fprint(
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

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
		ewftools_output_version_fprint(
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Unable to set stdout to binary mode.\n" );

		goto on_error;
	}
#endif
	while( ( option = ewftools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "A:b:B:c:d:f:hj:l:o:p:qsS:t:uvVwx" ) ) ) != (system_integer_t) -1 )
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
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind ] );

				usage_fprint(
				 stderr );

				goto on_error;

			case (system_integer_t) 'A':
				option_header_codepage = optarg;

				break;

			case (system_integer_t) 'b':
				option_sectors_per_chunk = optarg;

				break;

			case (system_integer_t) 'B':
				option_size = optarg;

				break;

			case (system_integer_t) 'c':
				option_compression_values = optarg;

				break;

			case (system_integer_t) 'd':
				option_additional_digest_types = optarg;

				break;

			case (system_integer_t) 'f':
				option_format = optarg;

				break;

			case (system_integer_t) 'h':
				ewftools_output_version_fprint(
				 stderr,
				 program );

				usage_fprint(
				 stderr );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'j':
				option_number_of_jobs = optarg;

				break;

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'o':
				option_offset = optarg;

				break;

			case (system_integer_t) 'p':
				option_process_buffer_size = optarg;

				break;

			case (system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (system_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (system_integer_t) 'S':
				option_maximum_segment_size = optarg;

				break;

			case (system_integer_t) 't':
				option_target_path = optarg;

				break;

			case (system_integer_t) 'u':
				interactive_mode = 0;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				ewftools_output_version_fprint(
				 stderr,
				 program );

				ewftools_output_copyright_fprint(
				 stderr );

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
		ewftools_output_version_fprint(
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Missing EWF image file(s).\n" );

		usage_fprint(
		 stderr );

		goto on_error;
	}
	ewftools_output_version_fprint(
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

	if( export_handle_initialize(
	     &ewfexport_export_handle,
	     calculate_md5,
	     use_data_chunk_functions,
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
	     ewfexport_export_handle,
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
	     ewfexport_signal_handler,
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
	          ewfexport_export_handle,
	          source_filenames,
	          number_of_filenames,
	          &error );

	if( ewfexport_abort != 0 )
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
	if( option_header_codepage != NULL )
	{
		result = export_handle_set_header_codepage(
			  ewfexport_export_handle,
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
		     ewfexport_export_handle,
		     option_target_path,
		     &( ewfexport_export_handle->target_path ),
		     &( ewfexport_export_handle->target_path_size ),
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set target path.\n" );

			goto on_error;
		}
	}
	else if( interactive_mode == 0 )
	{
		/* Make sure the target filename is set in unattended mode
		 */
		if( export_handle_set_string(
		     ewfexport_export_handle,
		     _SYSTEM_STRING( "export" ),
		     &( ewfexport_export_handle->target_path ),
		     &( ewfexport_export_handle->target_path_size ),
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set target path.\n" );

			goto on_error;
		}
	}
	/* Make sure we can write the target file
	 */
	if( interactive_mode == 0 )
	{
		if( export_handle_check_write_access(
		     ewfexport_export_handle,
		     ewfexport_export_handle->target_path,
		     &error ) != 1 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libcnotify_print_error_backtrace(
			 error );
#endif
			libcerror_error_free(
			 &error );

			fprintf(
			 stdout,
			 "Unable to write target file.\n" );

			goto on_error;
		}
	}
	if( option_format != NULL )
	{
		result = export_handle_set_output_format(
			  ewfexport_export_handle,
			  option_format,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set output format.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported output format defaulting to: raw.\n" );
		}
	}
	if( option_compression_values != NULL )
	{
		result = export_handle_set_compression_values(
			  ewfexport_export_handle,
			  option_compression_values,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set compression values.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported compression values defaulting to method: deflate with level: none.\n" );
		}
	}
	if( option_sectors_per_chunk != NULL )
	{
		result = export_handle_set_sectors_per_chunk(
			  ewfexport_export_handle,
			  option_sectors_per_chunk,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set sectors per chunk.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported sectors per chunk defaulting to: %" PRIu32 ".\n",
			 ewfexport_export_handle->output_sectors_per_chunk );
		}
		if( use_data_chunk_functions != 0 )
		{
			fprintf(
			 stderr,
			 "Setting sectors per chunk when using data chunk functions currently not supported defaulting to: %" PRIu32 ".\n",
			 ewfexport_export_handle->input_sectors_per_chunk );
		}
	}
	if( use_data_chunk_functions != 0 )
	{
		ewfexport_export_handle->output_sectors_per_chunk = ewfexport_export_handle->input_sectors_per_chunk;
	}
	if( option_maximum_segment_size != NULL )
	{
		result = export_handle_set_maximum_segment_size(
			  ewfexport_export_handle,
			  option_maximum_segment_size,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set maximum segment size.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported maximum segment size defaulting to: %" PRIu64 ".\n",
			 ewfexport_export_handle->maximum_segment_size );
		}
	}
	if( option_offset != NULL )
	{
		result = export_handle_set_export_offset(
			  ewfexport_export_handle,
			  option_offset,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set export offset.\n" );

			goto on_error;
		}
		else if( ( result == 0 )
		      || ( ewfexport_export_handle->export_offset >= ewfexport_export_handle->input_media_size ) )
		{
			ewfexport_export_handle->export_offset = 0;

			fprintf(
			 stderr,
			 "Unsupported export offset defaulting to: 0.\n" );
		}
	}
	if( option_size != NULL )
	{
		result = export_handle_set_export_size(
			  ewfexport_export_handle,
			  option_size,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set export size.\n" );

			goto on_error;
		}
		else if( ( result == 0 )
		      || ( ewfexport_export_handle->export_size > ( ewfexport_export_handle->input_media_size - ewfexport_export_handle->export_offset ) ) )
		{
			ewfexport_export_handle->export_size = 0;

			fprintf(
			 stderr,
			 "Unsupported export size defaulting to: all bytes.\n" );
		}
	}
	if( option_process_buffer_size != NULL )
	{
		result = export_handle_set_process_buffer_size(
			  ewfexport_export_handle,
			  option_process_buffer_size,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set process buffer size.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported process buffer size defaulting to: chunk size.\n" );
		}
	}
	if( option_number_of_jobs != NULL )
	{
#if defined( HAVE_MULTI_THREAD_SUPPORT )
		result = export_handle_set_number_of_threads(
			  ewfexport_export_handle,
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
		      || ( ewfexport_export_handle->number_of_threads > (int) 32 ) )
		{
			ewfexport_export_handle->number_of_threads = 4;

			fprintf(
			 stderr,
			 "Unsupported number of jobs (threads) defaulting to: %d.\n",
			 ewfexport_export_handle->number_of_threads );
		}
#else
		ewfexport_export_handle->number_of_threads = 0;

		fprintf(
		 stderr,
		 "Unsupported number of jobs (threads) defaulting to: %d.\n",
		 ewfexport_export_handle->number_of_threads );
#endif
	}
	if( option_additional_digest_types != NULL )
	{
		result = export_handle_set_additional_digest_types(
			  ewfexport_export_handle,
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
	/* Initialize values
	 */
	if( ewfexport_export_handle->export_size == 0 )
	{
		ewfexport_export_handle->export_size = ewfexport_export_handle->input_media_size
		                                     - ewfexport_export_handle->export_offset;
	}
	/* Request the necessary case data
	 */
	if( interactive_mode != 0 )
	{
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
		fprintf(
		 stderr,
		 "Information for export required, please provide the necessary input\n" );

		if( option_format == NULL )
		{
			result = export_handle_prompt_for_output_format(
				  ewfexport_export_handle,
			          _SYSTEM_STRING( "Export to format" ),
				  &error );

			if( result == -1 )
			{
				fprintf(
				 stderr,
				 "Unable to determine output format.\n" );

				goto on_error;
			}
		}
		if( option_target_path == NULL )
		{
			if( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
			{
				request_string = _SYSTEM_STRING( "Target path and filename without extension" );
			}
			else if( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_FILES )
			{
				request_string = _SYSTEM_STRING( "Target path" );
			}
			else if( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
			{
				request_string = _SYSTEM_STRING( "Target path and filename without extension or - for stdout" );
			}
		}
		if( request_string != NULL )
		{
			do
			{
				result = export_handle_prompt_for_string(
					  ewfexport_export_handle,
					  request_string,
					  &( ewfexport_export_handle->target_path ),
					  &( ewfexport_export_handle->target_path_size ),
					  &error );

				if( result == -1 )
				{
					fprintf(
					 stderr,
					 "Unable to determine target.\n" );

					goto on_error;
				}
				else if( result == 0 )
				{
					fprintf(
					 stderr,
					 "Target is required, please try again or terminate using Ctrl^C.\n" );
				}
			}
			while( result != 1 );
		}
		if( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
		{
			if( option_compression_values == NULL )
			{
				result = export_handle_prompt_for_compression_method(
					  ewfexport_export_handle,
				          _SYSTEM_STRING( "Compression method" ),
					  &error );

				if( result == -1 )
				{
					fprintf(
					 stderr,
					 "Unable to determine compression method.\n" );

					goto on_error;
				}
				result = export_handle_prompt_for_compression_level(
					  ewfexport_export_handle,
				          _SYSTEM_STRING( "Compression level" ),
					  &error );

				if( result == -1 )
				{
					fprintf(
					 stderr,
					 "Unable to determine compression level.\n" );

					goto on_error;
				}
			}
			if( option_maximum_segment_size == NULL )
			{
				result = export_handle_prompt_for_maximum_segment_size(
					  ewfexport_export_handle,
				          _SYSTEM_STRING( "Evidence segment file size in bytes" ),
					  &error );

				if( result == -1 )
				{
					fprintf(
					 stderr,
					 "Unable to determine maximum segment size.\n" );

					goto on_error;
				}
				if( ( ewfexport_export_handle->maximum_segment_size < EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE )
				 || ( ( ewfexport_export_handle->ewf_format == LIBEWF_FORMAT_ENCASE6 )
				  &&  ( ewfexport_export_handle->maximum_segment_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT ) )
				 || ( ( ewfexport_export_handle->ewf_format != LIBEWF_FORMAT_ENCASE6 )
				  &&  ( ewfexport_export_handle->maximum_segment_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT ) ) )
				{
					ewfexport_export_handle->maximum_segment_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

					fprintf(
					 stderr,
					 "Unsupported maximum segment size defaulting to: %" PRIu64 ".\n",
					 ewfexport_export_handle->maximum_segment_size );
				}
			}
			if( option_sectors_per_chunk == NULL )
			{
				result = export_handle_prompt_for_sectors_per_chunk(
					  ewfexport_export_handle,
				          _SYSTEM_STRING( "The number of sectors to read at once" ),
					  &error );

				if( result == -1 )
				{
					fprintf(
					 stderr,
					 "Unable to determine sectors per chunk.\n" );

					goto on_error;
				}
			}
		}
		else if( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
		{
			if( ( ewfexport_export_handle->target_path != NULL )
			 && ( ( ewfexport_export_handle->target_path )[ 0 ] == (system_character_t) '-' )
			 && ( ( ewfexport_export_handle->target_path )[ 1 ] == 0 ) )
			{
				/* No need for segment files when exporting to stdout */
			}
			else if( option_maximum_segment_size == NULL )
			{
				result = export_handle_prompt_for_maximum_segment_size(
					  ewfexport_export_handle,
				          _SYSTEM_STRING( "Evidence segment file size in bytes (0 is unlimited)" ),
					  &error );

				if( result == -1 )
				{
					fprintf(
					 stderr,
					 "Unable to determine maximum segment size.\n" );

					goto on_error;
				}
				if( ( ewfexport_export_handle->maximum_segment_size != 0 )
				 && ( ewfexport_export_handle->maximum_segment_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT ) )
				{
					ewfexport_export_handle->maximum_segment_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

					fprintf(
					 stderr,
					 "Unsupported maximum segment size defaulting to: %" PRIu64 ".\n",
					 ewfexport_export_handle->maximum_segment_size );
				}
			}
		}
		if( ( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
		 || ( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW ) )
		{
			if( option_offset == NULL )
			{
				result = export_handle_prompt_for_export_offset(
					  ewfexport_export_handle,
				          _SYSTEM_STRING( "Start export at offset" ),
					  &error );

				if( result == -1 )
				{
					libcnotify_print_error_backtrace(
					 error );
					libcerror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unable to determine export offset defaulting to: %" PRIu64 ".\n",
					 ewfexport_export_handle->export_offset );
				}
			}
			if( option_size == NULL )
			{
				result = export_handle_prompt_for_export_size(
					  ewfexport_export_handle,
				          _SYSTEM_STRING( "Number of bytes to export" ),
					  &error );

				if( result == -1 )
				{
					libcnotify_print_error_backtrace(
					 error );
					libcerror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unable to determine export size defaulting to: %" PRIu64 ".\n",
					 ewfexport_export_handle->export_size );
				}
			}
		}
		if( ewftools_signal_attach(
		     ewfexport_signal_handler,
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
	}
	else
	{
		if( ewfexport_export_handle->maximum_segment_size == 0 )
		{
			if( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
			{
				if( ewfexport_export_handle->ewf_format == LIBEWF_FORMAT_ENCASE6 )
				{
					ewfexport_export_handle->maximum_segment_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT;
				}
				else
				{
					ewfexport_export_handle->maximum_segment_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT;
				}
			}
			else if( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
			{
				ewfexport_export_handle->maximum_segment_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT;
			}
		}
	}
	fprintf(
	 stderr,
	 "\n" );

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
	if( ewfexport_export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_FILES )
	{
		result = export_handle_export_single_files(
		          ewfexport_export_handle,
		          ewfexport_export_handle->target_path,
		          print_status_information,
		          log_handle,
		          &error );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to export single files.\n" );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	else
	{
		if( export_handle_open_output(
		     ewfexport_export_handle,
		     ewfexport_export_handle->target_path,
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
		acquiry_software_version = _SYSTEM_STRING( LIBEWF_VERSION_STRING );

		if( export_handle_set_output_values(
		     ewfexport_export_handle,
		     acquiry_operating_system,
		     program,
		     acquiry_software_version,
		     zero_chunk_on_error,
		     0,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set output values.\n" );

			goto on_error;
		}
		result = export_handle_export_input(
		          ewfexport_export_handle,
		          swap_byte_pairs,
		          print_status_information,
		          log_handle,
		          &error );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to export input.\n" );

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
			 "Unable to close log file: %" PRIs_SYSTEM ".\n",
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
	     ewfexport_export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close export handle.\n" );

		goto on_error;
	}
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
	if( export_handle_free(
	     &ewfexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		goto on_error;
	}
	if( ewfexport_abort != 0 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_SYSTEM ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_SYSTEM ": FAILURE\n",
		 program );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stderr,
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
	if( ewfexport_export_handle != NULL )
	{
		export_handle_close(
		 ewfexport_export_handle,
		 NULL );
		export_handle_free(
		 &ewfexport_export_handle,
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

