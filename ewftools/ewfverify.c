/*
 * ewfverify
 * Verifies the integrity of the media data within the EWF file
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>
#include <liberror.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include <libsystem.h>

#include "byte_size_string.h"
#include "digest_context.h"
#include "ewfcommon.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "log_handle.h"
#include "md5.h"
#include "process_status.h"
#include "sha1.h"
#include "storage_media_buffer.h"
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
	fprintf( stream, "Use ewfverify to verify data stored in the EWF format (Expert Witness Compression\n"
	                 "Format).\n\n" );

	fprintf( stream, "Usage: ewfverify [ -A codepage ] [ -d digest_type ] [ -l log_filename ]\n"
	                 "                 [ -p process_buffer_size ] [ -hqvVw ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default),\n"
	                 "\t           windows-874, windows-1250, windows-1251, windows-1252,\n"
	                 "\t           windows-1253, windows-1254, windows-1255, windows-1256,\n"
	                 "\t           windows-1257, windows-1258\n" );
	fprintf( stream, "\t-d:        calculate additional digest (hash) types besides md5,\n"
	                 "\t           options: sha1\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-l:        logs verification errors and the digest (hash) to the\n"
	                 "\t           log_filename\n" );
	fprintf( stream, "\t-p:        specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-q:        quiet shows minimal status information\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
	fprintf( stream, "\t-w:        zero sectors on checksum error (mimic EnCase like behavior)\n" );
}

/* Reads the data to calculate the MD5 and SHA1 integrity hashes
 * Returns the number of bytes read if successful or -1 on error
 */
ssize64_t ewfverify_read_input(
           verification_handle_t *verification_handle,
           uint8_t zero_chunk_on_error,
           size_t process_buffer_size,
           process_status_t *process_status,
           liberror_error_t **error )
{
	storage_media_buffer_t *storage_media_buffer = NULL;
	static char *function                        = "ewfverify_read_input";
	size64_t media_size                          = 0;
	size32_t chunk_size                          = 0;
	size_t read_size                             = 0;
	ssize64_t verify_count                       = 0;
	ssize_t process_count                        = 0;
	ssize_t read_count                           = 0;

	if( verification_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid verification handle.",
		 function );

		return( -1 );
	}
	if( process_buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid process buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( process_status == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( verification_handle_get_values(
	     verification_handle,
	     &media_size,
	     &chunk_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine verification handle values.",
		 function );

		goto on_error;
	}
	if( chunk_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk size.",
		 function );

		goto on_error;
	}
	if( chunk_size > (uint32_t) INT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( verification_handle_set_error_handling_values(
	     verification_handle,
	     zero_chunk_on_error,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set verification handle error handling values.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	process_buffer_size = chunk_size;
#else
	if( process_buffer_size == 0 )
	{
		process_buffer_size = chunk_size;
	}
#endif
	if( storage_media_buffer_initialize(
	     &storage_media_buffer,
	     process_buffer_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create storage media buffer.",
		 function );

		goto on_error;
	}

	while( verify_count < (ssize64_t) media_size )
	{
		read_size = process_buffer_size;

		if( ( media_size - verify_count ) < read_size )
		{
			read_size = (size_t) ( media_size - verify_count );
		}
		read_count = verification_handle_read_buffer(
		              verification_handle,
		              storage_media_buffer,
		              read_size,
		              error );

		if( read_count < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			"%s: unable to read data.",
			 function );

			goto on_error;
		}
		if( read_count == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unexpected end of data.",
			 function );

			goto on_error;
		}
		process_count = verification_handle_prepare_read_buffer(
		                 verification_handle,
		                 storage_media_buffer,
		                 error );

		if( process_count < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			"%s: unable to prepare buffer after read.",
			 function );

			goto on_error;
		}
		if( process_count > (ssize_t) read_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: more bytes read than requested.",
			 function );

			goto on_error;
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		/* Set the chunk data size in the compression buffer
		 */
		if( storage_media_buffer->data_in_compression_buffer == 1 )
		{
			storage_media_buffer->compression_buffer_data_size = (ssize_t) process_count;
		}
#endif
		verify_count += (ssize64_t) process_count;

		if( verification_handle_update_integrity_hash(
		     verification_handle,
		     storage_media_buffer,
		     process_count,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to update integrity hash(es).",
			 function );

			goto on_error;
		}
		 if( process_status_update(
		      process_status,
		      (size64_t) verify_count,
		      media_size,
		      error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update process status.",
			 function );

			goto on_error;
		}
		if( ewfverify_abort != 0 )
		{
			break;
		}
  	}
	if( storage_media_buffer_free(
	     &storage_media_buffer,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free storage media buffer.",
		 function );

		goto on_error;
	}
	return( verify_count );

on_error:
	if( storage_media_buffer != NULL )
	{
		storage_media_buffer_free(
		 &storage_media_buffer,
		 NULL );
	}
	return( -1 );
}

/* Signal handler for ewfverify
 */
void ewfverify_signal_handler(
      libsystem_signal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "ewfverify_signal_handler";

	ewfverify_abort = 1;

	if( ( ewfverify_verification_handle != NULL )
	 && ( verification_handle_signal_abort(
	       ewfverify_verification_handle,
	       &error ) != 1 ) )
	{
		libsystem_notify_printf(
		 "%s: unable to signal verification handle to abort.\n",
		 function );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return;
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libsystem_file_io_close(
	     0 ) != 0 )
	{
		libsystem_notify_printf(
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
	liberror_error_t *error                                = NULL;

	libcstring_system_character_t * const *argv_filenames  = NULL;

#if !defined( LIBSYSTEM_HAVE_GLOB )
	libsystem_glob_t *glob                                 = NULL;
#endif

	libcstring_system_character_t *log_filename            = NULL;
	libcstring_system_character_t *program                 = _LIBCSTRING_SYSTEM_STRING( "ewfverify" );
	libcstring_system_character_t *option_header_codepage  = NULL;
	libcstring_system_character_t *stored_md5_hash_string  = NULL;
	libcstring_system_character_t *stored_sha1_hash_string = NULL;

	log_handle_t *log_handle                               = NULL;

	process_status_t *process_status                       = NULL;

	verification_handle_t *verification_handle             = NULL;

	libcstring_system_integer_t option                     = 0;
	ssize64_t verify_count                                 = 0;
	size_t string_length                                   = 0;
	uint64_t process_buffer_size                           = EWFCOMMON_PROCESS_BUFFER_SIZE;
	uint32_t number_of_checksum_errors                     = 0;
	uint8_t calculate_md5                                  = 1;
	uint8_t calculate_sha1                                 = 0;
	uint8_t print_status_information                       = 1;
	uint8_t zero_chunk_on_error                            = 0;
	uint8_t verbose                                        = 0;
	int number_of_filenames                                = 0;
	int match_md5_hash                                     = 0;
	int match_sha1_hash                                    = 0;
	int result                                             = 0;
	int status                                             = 0;
	int stored_md5_hash_available                          = 0;
	int stored_sha1_hash_available                         = 0;

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

	if( libsystem_initialize(
	     "ewftools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		goto on_error;
	}
	ewfoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "A:d:hl:p:qvVw" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				goto on_error;

			case (libcstring_system_integer_t) 'A':
				option_header_codepage = optarg;

				break;

			case (libcstring_system_integer_t) 'd':
				if( libcstring_system_string_compare(
				     optarg,
				     _LIBCSTRING_SYSTEM_STRING( "sha1" ),
				     4 ) == 0 )
				{
					calculate_sha1 = 1;
				}
				else
				{
					fprintf(
					 stderr,
					 "Unsupported digest type.\n" );
				}
				break;

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libcstring_system_integer_t) 'p':
				string_length = libcstring_system_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &process_buffer_size,
				          &error );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				if( ( result != 1 )
				 || ( process_buffer_size > (uint64_t) SSIZE_MAX ) )
				{
					process_buffer_size = 0;

					fprintf(
					 stderr,
					 "Unsupported process buffer size defaulting to: chunk size.\n" );
				}
				break;

			case (libcstring_system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				ewfoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'w':
				zero_chunk_on_error = 1;

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
	libsystem_notify_set_verbose(
	 verbose );
	libewf_notify_set_verbose(
	 verbose );
	libewf_notify_set_stream(
	 stderr,
	 NULL );

#if !defined( LIBSYSTEM_HAVE_GLOB )
	if( libsystem_glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize glob.\n" );

		goto on_error;
	}
	if( libsystem_glob_resolve(
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

	if( verification_handle_initialize(
	     &verification_handle,
	     calculate_md5,
	     calculate_sha1,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create verification handle.\n" );

		goto on_error;
	}
	if( libsystem_signal_attach(
	     ewfverify_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	result = verification_handle_open_input(
	          verification_handle,
	          argv_filenames,
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
#if !defined( LIBSYSTEM_HAVE_GLOB )
	if( libsystem_glob_free(
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
		result = verification_handle_set_header_codepage(
			  verification_handle,
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
	if( process_status_initialize(
	     &process_status,
	     _LIBCSTRING_SYSTEM_STRING( "Verify" ),
	     _LIBCSTRING_SYSTEM_STRING( "verified" ),
	     _LIBCSTRING_SYSTEM_STRING( "Read" ),
	     stdout,
	     print_status_information,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize process status.\n" );

		goto on_error;
	}
	if( process_status_start(
	     process_status,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to start process status.\n" );

		goto on_error;
	}
	/* Start verifying data
	 */
	verify_count = ewfverify_read_input(
			verification_handle,
			zero_chunk_on_error,
			(size_t) process_buffer_size,
			process_status,
			&error );

	if( verify_count <= -1 )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		status = PROCESS_STATUS_FAILED;
	}
	else
	{
		status = PROCESS_STATUS_COMPLETED;
	}
	if( process_status_stop(
	     process_status,
	     (size64_t) verify_count,
	     status,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to stop process status.\n" );

		goto on_error;
	}
	if( process_status_free(
	     &process_status,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free process status.\n" );

		goto on_error;
	}
	if( ewfverify_abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( status == PROCESS_STATUS_COMPLETED )
	{
		if( calculate_md5 == 1 )
		{
			stored_md5_hash_string = libcstring_system_string_allocate(
			                          DIGEST_HASH_STRING_SIZE_MD5 );

			if( stored_md5_hash_string == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create stored MD5 hash string.\n" );

				goto on_error;
			}
		}
		if( calculate_sha1 == 1 )
		{
			stored_sha1_hash_string = libcstring_system_string_allocate(
			                           DIGEST_HASH_STRING_SIZE_SHA1 );

			if( stored_sha1_hash_string == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create stored SHA1 hash string.\n" );

				goto on_error;
			}
		}
		if( verification_handle_finalize(
		     verification_handle,
		     stored_md5_hash_string,
		     DIGEST_HASH_STRING_SIZE_MD5,
		     &stored_md5_hash_available,
		     stored_sha1_hash_string,
		     DIGEST_HASH_STRING_SIZE_SHA1,
		     &stored_sha1_hash_available,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to finalize verification handle.\n" );

			goto on_error;
		}
		if( verification_handle_get_number_of_checksum_errors(
		     verification_handle,
		     &number_of_checksum_errors,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to determine the number of checksum errors.\n" );

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
				 "Unable to open log file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
				 log_filename );

				goto on_error;
			}
		}
		fprintf(
		 stdout,
		 "\n" );

		if( verification_handle_checksum_errors_fprint(
		     verification_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print checksum errors.\n" );

			goto on_error;
		}
		if( log_handle != NULL )
		{
			if( verification_handle_checksum_errors_fprint(
			     verification_handle,
			     log_handle->log_stream,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to write checksum errors in log file.\n" );

				goto on_error;
			}
		}
		if( calculate_md5 == 1 )
		{
			if( stored_md5_hash_available == 0 )
			{
				fprintf(
				 stdout,
				 "MD5 hash stored in file:\tN/A\n" );

				if( log_handle != NULL )
				{
					log_handle_printf(
					 log_handle,
					 "MD5 hash stored in file:\tN/A\n" );
				}
			}
			else
			{
				fprintf(
				 stdout,
				 "MD5 hash stored in file:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
				 stored_md5_hash_string );

				if( log_handle != NULL )
				{
					log_handle_printf(
					 log_handle,
					 "MD5 hash stored in file:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
					 stored_md5_hash_string );
				}
			}
			fprintf(
			 stdout,
			 "MD5 hash calculated over data:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
			 verification_handle->md5_hash_string );

			if( log_handle != NULL )
			{
				log_handle_printf(
				 log_handle,
				 "MD5 hash calculated over data:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
				 verification_handle->md5_hash_string );
			}
			match_md5_hash = ( libcstring_system_string_compare(
					    stored_md5_hash_string,
					    verification_handle->md5_hash_string,
					    DIGEST_HASH_STRING_SIZE_MD5 ) == 0 );
		}
		if( calculate_sha1 == 1 )
		{
			if( stored_sha1_hash_available == 0 )
			{
				fprintf(
				 stdout, "SHA1 hash stored in file:\tN/A\n" );

				if( log_handle != NULL )
				{
					log_handle_printf(
					 log_handle,
					 "SHA1 hash stored in file:\tN/A\n" );
				}
			}
			else
			{
				fprintf(
				 stdout,
				 "SHA1 hash stored in file:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
				 stored_sha1_hash_string );

				if( log_handle != NULL )
				{
					log_handle_printf(
					 log_handle,
					 "SHA1 hash stored in file:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
					 stored_sha1_hash_string );
				}
			}
			fprintf(
			 stdout,
			 "SHA1 hash calculated over data:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
			 verification_handle->sha1_hash_string );

			if( log_handle != NULL )
			{
				log_handle_printf(
				 log_handle,
				 "SHA1 hash calculated over data:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
				 verification_handle->sha1_hash_string );
			}
			match_sha1_hash = ( libcstring_system_string_compare(
					     stored_sha1_hash_string,
					     verification_handle->sha1_hash_string,
					     DIGEST_HASH_STRING_SIZE_SHA1 ) == 0 );
		}
		if( verification_handle_additional_hash_values_fprint(
		     verification_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print additional hash values.\n" );

			goto on_error;
		}
		if( log_handle != NULL )
		{
			if( verification_handle_additional_hash_values_fprint(
			     verification_handle,
			     log_handle->log_stream,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to write additional hash values in log file.\n" );

				goto on_error;
			}
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 stored_md5_hash_string );

			stored_md5_hash_string = NULL;
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 stored_sha1_hash_string );

			stored_sha1_hash_string = NULL;
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
	if( verification_handle_close(
	     verification_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close EWF file(s).\n" );

		goto on_error;

	}
	if( libsystem_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	if( verification_handle_free(
	     &verification_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free verification handle.\n" );

		goto on_error;
	}
	if( status == PROCESS_STATUS_ABORTED )
	{
		fprintf(
		 stdout, "%" PRIs_LIBCSTRING_SYSTEM ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	/* Note that a set of EWF files can be verified without an integrity hash
	 */
	else if( ( status == PROCESS_STATUS_COMPLETED )
	      && ( number_of_checksum_errors == 0 )
	      && ( ( calculate_md5 == 0 )
	       || ( stored_md5_hash_available == 0 )
	       || match_md5_hash )
	      && ( ( calculate_sha1 == 0 )
	       || ( stored_sha1_hash_available == 0 )
	       || match_sha1_hash ) )
	{
		fprintf(
		 stdout,
		 "\n%" PRIs_LIBCSTRING_SYSTEM ": SUCCESS\n",
		 program );

		result = EXIT_SUCCESS;
	}
	else
	{
		fprintf(
		 stdout,
		 "\n%" PRIs_LIBCSTRING_SYSTEM ": FAILURE\n",
		 program );

		result = EXIT_FAILURE;
	}
	return( result );

on_error:
	if( error != NULL )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
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
	if( stored_sha1_hash_string != NULL )
	{
		memory_free(
		 stored_sha1_hash_string );
	}
	if( stored_md5_hash_string != NULL )
	{
		memory_free(
		 stored_md5_hash_string );
	}
	if( process_status != NULL )
	{
		process_status_free(
		 &process_status,
		 NULL );
	}
	if( verification_handle != NULL )
	{
		verification_handle_close(
		 verification_handle,
		 NULL );
		verification_handle_free(
		 &verification_handle,
		 NULL );
	}
#if !defined( LIBSYSTEM_HAVE_GLOB )
	if( glob != NULL )
	{
		libsystem_glob_free(
		 &glob,
		 NULL );
	}
#endif
	return( EXIT_FAILURE );
}

