/*
 * ewfalter
 * Alters media data in EWF files
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

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "byte_size_string.h"
#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewfsignal.h"
#include "glob.h"
#include "notify.h"

#define EWFALTER_INPUT_BUFFER_SIZE	64

/* Prints the executable usage information to the stream
 *  */
void usage_fprint(
      FILE *stream )
{
        if( stream == NULL )
        {
                return;
        }
	fprintf( stream, "Usage: ewfalter [ -p process_buffer_size ] [ -t target_file ] [ -hsqvV ]\n"
	                 "                ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-q:        quiet shows no status information\n" );
	fprintf( stream, "\t-s:        swap byte pairs of the media data (from AB to BA)\n"
	                 "\t           (use this for big to little endian conversion and vice versa)\n" );
	fprintf( stream, "\t-t:        specify the target delta path and base filename (default is the same\n"
	                 "\t           as the ewf_files)\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
}

/* The main program
 */
#if defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	character_t input_buffer[ EWFALTER_INPUT_BUFFER_SIZE ];

#if !defined( HAVE_GLOB_H )
	glob_t *glob                            = NULL;
#endif

	character_t *program                       = _CHARACTER_T_STRING( "ewfalter" );

	system_character_t * const *argv_filenames = NULL;
	system_character_t **ewf_filenames         = NULL;
	system_character_t *target_filename        = NULL;

	uint8_t *process_buffer                    = NULL;
	system_integer_t option                    = 0;
	off64_t write_offset                       = 0;
	size64_t media_size                        = 0;
	size_t string_length                       = 0;
	ssize_t write_size                         = 0;
	uint64_t alter_offset                      = 0;
	uint64_t alter_size                        = 0;
	uint64_t process_buffer_size               = 0;
	int64_t alter_count                        = 0;
	int64_t write_count                        = 0;
	uint8_t swap_byte_pairs                    = 0;
	uint8_t verbose                            = 0;
	int amount_of_filenames                    = 0;
	int result                                 = 0;

	ewfoutput_version_fprint(
	 stdout,
	 program );

	fprintf( stdout, "%" PRIs " is for testing purposes only.\n",
	 program );

	while( ( option = ewfgetopt(
	                   argc,
	                   argv,
	                   _SYSTEM_CHARACTER_T_STRING( "hp:qst:vV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf( stderr, "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'p':
				string_length = system_string_length(
				                 optarg );

				result = byte_size_string_convert_system_character(
				          optarg,
				          string_length,
				          &process_buffer_size );

				if( ( result != 1 )
				 || ( process_buffer_size > (uint64_t) SSIZE_MAX ) )
				{
					process_buffer_size = 0;

					fprintf( stderr, "Unsupported process buffer size defaulting to: chunk size.\n" );
				}
				break;

			case (system_integer_t) 'q':
				break;

			case (system_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (system_integer_t) 't':
				target_filename = optarg;

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
		fprintf( stderr, "Missing EWF image file(s).\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	libewf_set_notify_values(
	 stderr,
	 verbose );
	notify_set_values(
	 stderr,
	 verbose );

	if( ewfsignal_attach(
	     ewfcommon_signal_handler ) != 1 )
	{
		fprintf( stderr, "Unable to attach signal handler.\n" );
	}
	amount_of_filenames = argc - optind;

#if !defined( HAVE_GLOB_H )
	if( glob_initialize(
	     &glob ) != 1 )
	{
		fprintf( stderr, "Unable to initialize glob.\n" );

		return( EXIT_FAILURE );
	}
	amount_of_filenames = glob_resolve(
	                       glob,
	                       &argv[ optind ],
	                       ( argc - optind ) );

	if( ( amount_of_filenames <= 0 )
	 || ( amount_of_filenames > (int) UINT16_MAX ) )
	{
		fprintf( stderr, "Unable to resolve glob.\n" );

		glob_free(
		 &glob );

		return( EXIT_FAILURE );
	}
	argv_filenames = glob->result;
#else
	argv_filenames = &argv[ optind ];
#endif

	if( amount_of_filenames == 1 )
	{
		amount_of_filenames = libewf_glob(
		                       argv_filenames[ 0 ],
		                       system_string_length(
		                        argv_filenames[ 0 ] ),
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &ewf_filenames );

		if( amount_of_filenames <= 0 )
		{
			fprintf( stderr, "Unable to resolve ewf file(s).\n" );

#if !defined( HAVE_GLOB_H )
			glob_free(
			 &glob );
#endif

			return( EXIT_FAILURE );
		}
		argv_filenames = (system_character_t * const *) ewf_filenames;
	}
	ewfcommon_libewf_handle = libewf_open(
	                           argv_filenames,
	                           amount_of_filenames,
	                           LIBEWF_OPEN_READ_WRITE );
#if !defined( HAVE_GLOB_H )
	glob_free(
	 &glob );
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
	if( ( ewfcommon_abort == 0 )
	 && ( ewfcommon_libewf_handle == NULL ) )
	{
		ewfoutput_error_fprint(
		 stderr, "Unable to open EWF file(s)" );

		return( EXIT_FAILURE );
	}
	if( ( ewfcommon_abort == 0 )
	 && ( libewf_get_media_size(
		     ewfcommon_libewf_handle,
		     &media_size ) != 1 ) )
	{
		fprintf( stderr, "Unable to determine media size.\n" );

		return( EXIT_FAILURE );
	}
	if( ewfcommon_abort == 0 )
	{
		if( ewfsignal_detach() != 1 )
		{
			fprintf( stderr, "Unable to detach signal handler.\n" );
		}
		/* Request the necessary case data
		 */
		fprintf( stdout, "Information for alter required, please provide the necessary input\n" );

		if( ewfinput_get_size_variable(
		     stdout,
		     input_buffer,
		     EWFALTER_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Start altering at offset" ),
		     0,
		     media_size,
		     0,
		     &alter_offset ) == -1 )
		{
			alter_offset = 0;

			fprintf( stdout, "Unable to determine the altertion offset defaulting to: %" PRIu64 ".\n",
			 alter_offset );
		}
		if( ewfinput_get_size_variable(
		     stdout,
		     input_buffer,
		     EWFALTER_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Amount of bytes to alter" ),
		     0,
		     ( media_size - alter_offset ),
		     ( media_size - alter_offset ),
		     &alter_size ) == -1 )
		{
			alter_size = media_size - alter_offset;

			fprintf( stdout, "Unable to determine the altertion size defaulting to: %" PRIu64 ".\n",
			 alter_size );
		}
		if( process_buffer_size ==  0 )
		{
			if( ewfinput_get_size_variable(
			     stdout,
			     input_buffer,
			     EWFALTER_INPUT_BUFFER_SIZE,
			     _CHARACTER_T_STRING( "Alteration buffer size" ),
			     1,
			     SSIZE_MAX,
			     ( 64 * 512 ),
			     &process_buffer_size ) == -1 )
			{
				process_buffer_size = 64 * 512;

				fprintf( stdout, "Unable to determine the altertion buffer size defaulting to: %" PRIu64 ".\n",
				 process_buffer_size );
			}
			if( process_buffer_size > (size_t) SSIZE_MAX )
			{
				process_buffer_size = 64 * 512;

				fprintf( stdout, "Invalid alteration buffer size defaulting to: %" PRIu64 ".\n",
				 process_buffer_size );
			}
		}
		process_buffer = memory_allocate(
		                  sizeof( uint8_t ) * (size_t) process_buffer_size );

		if( process_buffer == NULL )
		{
			fprintf( stderr, "Unable to allocate process buffer.\n" );

			libewf_close(
			 ewfcommon_libewf_handle );

			return( EXIT_FAILURE );
		}
		if( memory_set(
		     process_buffer,
		     'X',
		     sizeof( uint8_t ) * (size_t) process_buffer_size ) == NULL )
		{
			fprintf( stderr, "Unable to set process buffer.\n" );

			memory_free(
			 process_buffer );

			libewf_close(
			 ewfcommon_libewf_handle );

			return( EXIT_FAILURE );
		}
		if( target_filename != NULL )
		{
			if( libewf_set_delta_segment_filename(
			     ewfcommon_libewf_handle,
			     target_filename,
			     system_string_length(
			      target_filename ) ) != 1 )
			{
				fprintf( stderr, "Unable to set delta segment filename in handle.\n" );

				memory_free(
				 process_buffer );

				libewf_close(
				 ewfcommon_libewf_handle );

				return( EXIT_FAILURE );
			}
		}
		fprintf( stderr, "\n" );

		if( ewfsignal_attach(
		     ewfcommon_signal_handler ) != 1 )
		{
			fprintf( stderr, "Unable to attach signal handler.\n" );
		}
	}
	if( ewfcommon_abort == 0 )
	{
		/* First alteration run
		 */
		alter_count  = (int64_t) alter_size;
		write_offset = (off64_t) alter_offset;

		while( alter_count > 0 )
		{
			if( alter_count > (int64_t) process_buffer_size )
			{
				write_size = (ssize_t) process_buffer_size;
			}
			else
			{
				write_size = (ssize_t) alter_count;
			}
			write_count = libewf_write_random(
			               ewfcommon_libewf_handle,
			               process_buffer,
			               (size_t) write_size,
			               write_offset );

			if( write_count <= -1 )
			{
				fprintf( stdout, "Alteration failed.\n" );

				memory_free(
				 process_buffer );

				libewf_close(
				 ewfcommon_libewf_handle );

				return( EXIT_FAILURE );
			}
			alter_count  -= write_count;
			write_offset += write_count;

			if( ewfcommon_abort != 0 )
			{
				break;
			}
		}
	}
	if( ewfcommon_abort == 0 )
	{
		/* Second alteration run
		 */
		alter_count  = (int64_t) alter_size;
		write_offset = (off64_t) alter_offset;

		while( alter_count > 0 )
		{
			if( alter_count > (int64_t) process_buffer_size )
			{
				write_size = (ssize_t) process_buffer_size;
			}
			else
			{
				write_size = (ssize_t) alter_count;
			}
			write_count = libewf_write_random(
			               ewfcommon_libewf_handle,
			               process_buffer,
			               (size_t) write_size,
			               write_offset );

			if( write_count <= -1 )
			{
				fprintf( stdout, "Alteration failed.\n" );

				memory_free(
				 process_buffer );

				libewf_close(
				 ewfcommon_libewf_handle );

				return( EXIT_FAILURE );
			}
			alter_count  -= write_count;
			write_offset += write_count;

			if( ewfcommon_abort != 0 )
			{
				break;
			}
		}
	}
	memory_free(
	 process_buffer );

	if( libewf_close(
	     ewfcommon_libewf_handle ) != 0 )
	{
		fprintf( stderr, "Unable to close EWF file(s).\n" );

		return( EXIT_FAILURE );
	}
	if( ewfsignal_detach() != 1 )
	{
		fprintf( stderr, "Unable to detach signal handler.\n" );
	}
	if( ewfcommon_abort != 0 )
	{
		fprintf( stdout, "%" PRIs ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	fprintf( stdout, "Alteration completed.\n" );

	return( EXIT_SUCCESS );
}

