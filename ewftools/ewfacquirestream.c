/*
 * ewfacquirestream
 * Reads data from a stdin and writes it in EWF format
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <common.h>
#include <character_string.h>
#include <memory.h>
#include <system_string.h>

#include <errno.h>

#include <stdio.h>

#if defined( HAVE_SYS_IOCTL_H )
#include <sys/ioctl.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include <stdio.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "../libewf/libewf_common.h"
#include "../libewf/libewf_notify.h"

#include "ewfbyte_size_string.h"
#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfglob.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewfsignal.h"
#include "ewfstring.h"

/* Prints the executable usage information to the stream
 */
void usage_fprint(
      FILE *stream )
{
	character_t default_segment_file_size_string[ 16 ];
	character_t minimum_segment_file_size_string[ 16 ];
	character_t maximum_32bit_segment_file_size_string[ 16 ];
	character_t maximum_64bit_segment_file_size_string[ 16 ];

	int result = 0;

	if( stream == NULL )
	{
		return;
	}
	result = ewfbyte_size_string_create(
	          default_segment_file_size_string,
	          16,
	          EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
	          EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE );

	if( result == 1 )
	{
		result = ewfbyte_size_string_create(
			  minimum_segment_file_size_string,
			  16,
			  EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
			  EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE );
	}
	if( result == 1 )
	{
		result = ewfbyte_size_string_create(
			  maximum_32bit_segment_file_size_string,
			  16,
			  EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT,
			  EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE );
	}
	if( result == 1 )
	{
		result = ewfbyte_size_string_create(
			  maximum_64bit_segment_file_size_string,
			  16,
			  EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
			  EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE );
	}
	fprintf( stream, "Usage: ewfacquirestream [ -b amount_of_sectors ] [ -c compression_type ] [ -C case_number ] [ -d digest_type ] [ -D description ]\n" );
	fprintf( stream, "                        [ -e examiner_name ] [ -E evidence_number ] [ -f format ] [ -l filename ] [ -m media_type ] [ -M volume_type ]\n" );
	fprintf( stream, "                        [ -N notes ] [ -S segment_file_size ] [ -t target ] [ -hsvVw ]\n\n" );

	fprintf( stream, "\tReads data from stdin\n\n" );

	fprintf( stream, "\t-b: specify the amount of sectors to read at once (per chunk), options: 64 (default),\n" );
	fprintf( stream, "\t    128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n" );
	fprintf( stream, "\t-c: specify the compression type, options: none (is default), empty_block, fast, best\n" );
	fprintf( stream, "\t-C: specify the case number (default is case_number).\n" );
	fprintf( stream, "\t-d: calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stream, "\t-D: specify the description (default is description).\n" );
	fprintf( stream, "\t-e: specify the examiner name (default is examiner_name).\n" );
	fprintf( stream, "\t-E: specify the evidence number (default is evidence_number).\n" );
	fprintf( stream, "\t-f: specify the EWF file format to write to, options: ftk, encase2, encase3, encase4,\n" );
	fprintf( stream, "\t    encase5 (is default), encase6, linen5, linen6, ewfx\n" );
	fprintf( stream, "\t-h: shows this help\n" );
	fprintf( stream, "\t-l: logs acquiry errors and the digest (hash) to the filename\n" );
	fprintf( stream, "\t-m: specify the media type, options: fixed (is default), removable\n" );
	fprintf( stream, "\t-M: specify the volume type, options: logical, physical (is default)\n" );
	fprintf( stream, "\t-N: specify the notes (default is notes).\n" );
	fprintf( stream, "\t-s: swap byte pairs of the media data (from AB to BA)\n" );
	fprintf( stream, "\t    (use this for big to little endian conversion and vice versa)\n" );

	if( result == 1 )
	{
		fprintf( stream, "\t-S: specify the segment file size in bytes (default is %" PRIs ")\n",
		 default_segment_file_size_string );
		fprintf( stream, "\t    (minimum is %" PRIs ", maximum is %" PRIs " for encase6 format and %" PRIs " for other formats)\n",
		 minimum_segment_file_size_string,
		 maximum_64bit_segment_file_size_string,
		 maximum_32bit_segment_file_size_string );
	}
	else
	{
		fprintf( stream, "\t-S: specify the segment file size in bytes (default is %" PRIu32 ")\n",
		 (uint32_t) EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE );
		fprintf( stream, "\t    (minimum is %" PRIu32 ", maximum is %" PRIu64 " for encase6 format and %" PRIu32 " for other formats)\n",
		 (uint32_t) EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
		 (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
		 (uint32_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT );
	}
	fprintf( stream, "\t-t: specify the target file (without extension) to write to (default is stream)\n" );
	fprintf( stream, "\t-v: verbose output to stderr\n" );
	fprintf( stream, "\t-V: print version\n" );
	fprintf( stream, "\t-w: wipe sectors on read error (mimic EnCase like behavior)\n" );
}

/* The main program
 */
#if defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	system_character_t *filenames[ 1 ]         = { _SYSTEM_CHARACTER_T_STRING( "stream" ) };

	character_t *calculated_md5_hash_string    = NULL;
	character_t *calculated_sha1_hash_string   = NULL;
	character_t *case_number                   = NULL;
	character_t *description                   = NULL;
	character_t *evidence_number               = NULL;
	character_t *examiner_name                 = NULL;
	character_t *notes                         = NULL;
	character_t *acquiry_operating_system      = NULL;
	character_t *acquiry_software_version      = NULL;
	character_t *program                       = _CHARACTER_T_STRING( "ewfacquirestream" );

	system_character_t *log_filename           = NULL;
	system_character_t *option_case_number     = NULL;
	system_character_t *option_description     = NULL;
	system_character_t *option_examiner_name   = NULL;
	system_character_t *option_evidence_number = NULL;
	system_character_t *option_notes           = NULL;
	system_character_t *time_string            = NULL;
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
        system_character_t *error_string           = NULL;
#endif

	FILE *log_file_stream                      = NULL;
	void *callback                             = &ewfoutput_stream_process_status_fprint;

	system_integer_t option                    = 0;
	size_t string_length                       = 0;
	time_t timestamp_start                     = 0;
	time_t timestamp_end                       = 0;
	int64_t write_count                        = 0;
	uint64_t segment_file_size                 = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;
	uint64_t acquiry_offset                    = 0;
	uint64_t acquiry_size                      = 0;
	uint64_t sectors_per_chunk                 = 64;
	uint32_t sector_error_granularity          = 64;
	uint32_t amount_of_acquiry_errors          = 0;
	int8_t compression_level                   = LIBEWF_COMPRESSION_NONE;
	uint8_t media_type                         = LIBEWF_MEDIA_TYPE_FIXED;
	uint8_t volume_type                        = LIBEWF_VOLUME_TYPE_PHYSICAL;
	uint8_t compress_empty_block               = 0;
	uint8_t libewf_format                      = LIBEWF_FORMAT_ENCASE5;
	uint8_t wipe_chunk_on_error                = 0;
	uint8_t read_error_retry                   = 2;
	uint8_t swap_byte_pairs                    = 0;
	uint8_t seek_on_error                      = 0;
	uint8_t calculate_md5                      = 1;
	uint8_t calculate_sha1                     = 0;
	uint8_t verbose                            = 0;
	int error_abort                            = 0;
	int result                                 = 0;

	ewfoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = ewfgetopt(
	                   argc,
	                   argv,
	                   _SYSTEM_CHARACTER_T_STRING( "b:c:C:d:D:e:E:f:hl:m:M:N:sS:t:vVw" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf( stderr, "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'b':
				sectors_per_chunk = ewfinput_determine_sectors_per_chunk_char_t(
				                     optarg );

				if( sectors_per_chunk == 0 )
				{
					fprintf( stderr, "Unsuported amount of sectors per chunk defaulting to 64.\n" );

					sectors_per_chunk = 64;
				}
				break;

			case (system_integer_t) 'c':
				if( system_string_compare(
				     optarg,
				     _SYSTEM_CHARACTER_T_STRING( "empty_block" ),
				     11 ) == 0 )
				{
					compress_empty_block = 1;
				}
				else
				{
					compression_level = ewfinput_determine_compression_level_char_t(
					                     optarg );
				
					if( compression_level <= -1 )
					{
						fprintf( stderr, "Unsupported compression type defaulting to none.\n" );

						compression_level = LIBEWF_COMPRESSION_NONE;
					}
				}
				break;

			case (system_integer_t) 'C':
				option_case_number = optarg;

				break;

			case (system_integer_t) 'd':
				if( system_string_compare(
				     optarg,
				     _SYSTEM_CHARACTER_T_STRING( "sha1" ),
				     4 ) == 0 )
				{
					calculate_sha1 = 1;
				}
				else
				{
					fprintf( stderr, "Unsupported digest type.\n" );
				}
				break;

			case (system_integer_t) 'D':
				option_description = optarg;

				break;

			case (system_integer_t) 'e':
				option_examiner_name = optarg;

				break;

			case (system_integer_t) 'E':
				option_evidence_number = optarg;

				break;

			case (system_integer_t) 'f':
				libewf_format = ewfinput_determine_libewf_format_char_t(
				                 optarg );

				if( ( libewf_format == 0 )
				 || ( libewf_format == LIBEWF_FORMAT_EWF )
				 || ( libewf_format == LIBEWF_FORMAT_SMART ) )
				{
					fprintf( stderr, "Unsupported EWF file format type defaulting to encase5.\n" );

					libewf_format = LIBEWF_FORMAT_ENCASE5;
				}
				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'm':
				if( system_string_compare(
				     optarg,
				     _SYSTEM_CHARACTER_T_STRING( "fixed" ),
				     5 ) == 0 )
				{
					media_type = LIBEWF_MEDIA_TYPE_FIXED;
				}
				else if( system_string_compare(
				          optarg,
				          _SYSTEM_CHARACTER_T_STRING( "removable" ),
				          9 ) == 0 )
				{
					media_type = LIBEWF_MEDIA_TYPE_REMOVABLE;
				}
				else
				{
					fprintf( stderr, "Unsupported media type defaulting to fixed.\n" );
				}
				break;

			case (system_integer_t) 'M':
				if( system_string_compare(
				     optarg,
				     _SYSTEM_CHARACTER_T_STRING( "logical" ),
				     7 ) == 0 )
				{
					volume_type = LIBEWF_VOLUME_TYPE_LOGICAL;
				}
				else if( system_string_compare(
				          optarg,
				          _SYSTEM_CHARACTER_T_STRING( "physical" ),
				          8 ) == 0 )
				{
					volume_type = LIBEWF_VOLUME_TYPE_PHYSICAL;
				}
				else
				{
					fprintf( stderr, "Unsupported volume type defaulting to logical.\n" );
				}
				break;

			case (system_integer_t) 'N':
				option_notes = optarg;

				break;

			case (system_integer_t) 'q':

				break;

			case (system_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (system_integer_t) 'S':
				string_length = system_string_length(
				                 optarg );

				result = ewfbyte_size_string_convert_char_t(
				          optarg,
				          string_length,
				          &segment_file_size );

				if( ( result != 1 )
				 || ( segment_file_size < EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE )
				 || ( ( libewf_format == LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (int64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT ) )
				 || ( ( libewf_format != LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (int64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT ) ) )
				{
					fprintf( stderr, "Unsupported segment file size defaulting to %" PRIu32 ".\n",
					 (uint32_t) EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE );

					segment_file_size = (int64_t) EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;
				}
				break;

			case (system_integer_t) 't':
				filenames[ 0 ] = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				ewfoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'w':
				wipe_chunk_on_error = 1;

				break;
		}
	}
	if( ewfsignal_attach(
	     ewfcommon_signal_handler ) != 1 )
	{
		fprintf( stderr, "Unable to attach signal handler.\n" );
	}
	libewf_set_notify_values(
	 stderr,
	 verbose );

	if( option_case_number != NULL )
	{
		string_length = system_string_length(
		                 option_case_number );

		if( string_length > 0 )
		{
			string_length += 1;
			case_number    = (character_t *) memory_allocate(
			                                  sizeof( character_t ) * string_length );

			if( case_number == NULL )
			{
				fprintf( stderr, "Unable to create case number string.\n" );

				error_abort = 1;
			}
			else if( ewfstring_copy_system_string_to_character_string(
			          case_number,
			          option_case_number,
			          string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set case number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( ( error_abort == 0 )
	 && ( option_description != NULL ) )
	{
		string_length = system_string_length(
		                 option_description );

		if( string_length > 0 )
		{
			string_length += 1;
			description    = (character_t *) memory_allocate(
			                                  sizeof( character_t ) * string_length );

			if( description == NULL )
			{
				fprintf( stderr, "Unable to create description string.\n" );

				error_abort = 1;
			}
			else if( ewfstring_copy_system_string_to_character_string(
			          description,
			          option_description,
			          string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set description string.\n" );

				error_abort = 1;
			}
		}
	}
	if( ( error_abort == 0 )
	 && ( option_examiner_name != NULL ) )
	{
		string_length = system_string_length(
		                 option_examiner_name );

		if( string_length > 0 )
		{
			string_length += 1;
			examiner_name  = (character_t *) memory_allocate(
			                                  sizeof( character_t ) * string_length );

			if( examiner_name == NULL )
			{
				fprintf( stderr, "Unable to create examiner name string.\n" );

				error_abort = 1;
			}
			else if( ewfstring_copy_system_string_to_character_string(
			          examiner_name,
			          option_examiner_name,
			          string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set examiner name string.\n" );

				error_abort = 1;
			}
		}
	}
	if( ( error_abort == 0 )
	 && ( option_evidence_number != NULL ) )
	{
		string_length = system_string_length(
		                 option_evidence_number );

		if( string_length > 0 )
		{
			string_length  += 1;
			evidence_number = (character_t *) memory_allocate(
			                                   sizeof( character_t ) * string_length );

			if( evidence_number == NULL )
			{
				fprintf( stderr, "Unable to create evidence number string.\n" );

				error_abort = 1;
			}
			else if( ewfstring_copy_system_string_to_character_string(
			          evidence_number,
			          option_evidence_number,
			          string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set evidence number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( ( error_abort == 0 )
	 && ( option_notes != NULL ) )
	{
		string_length = system_string_length(
		                 option_notes );

		if( string_length > 0 )
		{
			string_length += 1;
			notes          = (character_t *) memory_allocate(
			                                  sizeof( character_t ) * string_length );

			if( notes == NULL )
			{
				fprintf( stderr, "Unable to create notes string.\n" );

				error_abort = 1;
			}
			else if( ewfstring_copy_system_string_to_character_string(
			          notes,
			          option_notes,
			          string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set notes string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort != 0 )
	{
		if( case_number != NULL )
		{
			memory_free(
			 case_number );
		}
		if( description != NULL )
		{
			memory_free(
			 description );
		}
		if( examiner_name != NULL )
		{
			memory_free(
			 examiner_name );
		}
		if( evidence_number != NULL )
		{
			memory_free(
			 evidence_number );
		}
		if( notes != NULL )
		{
			memory_free(
			 notes );
		}
		return( EXIT_FAILURE );
	}
	if( ewfcommon_abort == 0 )
	{
		acquiry_operating_system = ewfcommon_determine_operating_system();
		acquiry_software_version = LIBEWF_VERSION_STRING;

		fprintf( stdout, "Using the following acquiry parameters:\n" );

		ewfoutput_acquiry_parameters_fprint(
		 stdout,
		 filenames[ 0 ],
		 case_number,
		 description,
		 evidence_number,
		 examiner_name,
		 notes,
		 media_type,
		 volume_type,
		 compression_level,
		 compress_empty_block,
		 libewf_format,
		 (off64_t) acquiry_offset,
		 (size64_t) acquiry_size,
		 (size64_t) segment_file_size,
		 (uint32_t) sectors_per_chunk,
		 sector_error_granularity,
		 read_error_retry,
		 wipe_chunk_on_error );

		ewfcommon_libewf_handle = libewf_open(
					   (system_character_t * const *) filenames,
					   1,
					   LIBEWF_OPEN_WRITE );

		if( ewfcommon_libewf_handle == NULL )
		{
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
			if( errno != 0 )
			{
				error_string = ewfstring_strerror(
				                errno );
			}
			if( error_string != NULL )
			{
				fprintf( stderr, "Unable to open EWF file(s) with failure: %" PRIs_SYSTEM ".\n",
				 error_string );

				memory_free(
				 error_string );
			}
			else
			{
				fprintf( stderr, "Unable to create EWF file(s).\n" );
			}
#else
			fprintf( stderr, "Unable to create EWF file(s).\n" );
#endif

			error_abort = 1;
		}
		else if( ewfcommon_initialize_write(
			  ewfcommon_libewf_handle,
			  case_number,
			  description,
			  evidence_number,
			  examiner_name,
			  notes,
			  acquiry_operating_system,
			  program,
			  acquiry_software_version,
			  (uint8_t) media_type,
			  (uint8_t) volume_type,
			  compression_level,
			  (uint8_t) compress_empty_block,
			  libewf_format,
			  (size64_t) segment_file_size,
			  (uint32_t) sector_error_granularity ) != 1 )
		{
			fprintf( stderr, "Unable to initialize settings for EWF file(s).\n" );

			error_abort = 1;
		}
	}
	if( case_number != NULL )
	{
		memory_free(
		 case_number );
	}
	if( description != NULL )
	{
		memory_free(
		 description );
	}
	if( evidence_number != NULL )
	{
		memory_free(
		 evidence_number );
	}
	if( examiner_name != NULL )
	{
		memory_free(
		 examiner_name );
	}
	if( notes != NULL )
	{
		memory_free(
		 notes );
	}
	if( acquiry_operating_system != NULL )
	{
		memory_free(
		 acquiry_operating_system );
	}
	if( error_abort != 0 )
	{
		if( libewf_close(
		     ewfcommon_libewf_handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (character_t *) memory_allocate(
		                                              sizeof( character_t ) * EWFSTRING_DIGEST_HASH_LENGTH_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated MD5 hash string.\n" );

			if( libewf_close(
			     ewfcommon_libewf_handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			return( EXIT_FAILURE );
		}
	}
	if( calculate_sha1 == 1 )
	{
		calculated_sha1_hash_string = (character_t *) memory_allocate(
		                                               sizeof( character_t ) * EWFSTRING_DIGEST_HASH_LENGTH_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated SHA1 hash string.\n" );

			memory_free(
			 calculated_md5_hash_string );

			if( libewf_close(
			     ewfcommon_libewf_handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			return( EXIT_FAILURE );
		}
	}
	if( ewfcommon_abort == 0 )
	{
		/* Start acquiring data
		 */
		timestamp_start = time( NULL );
		time_string     = libewf_common_ctime(
				   &timestamp_start );

		if( time_string != NULL )
		{
			fprintf( stdout, "Acquiry started at: %" PRIs_SYSTEM "\n",
			 time_string );

			memory_free(
			 time_string );
		}
		else
		{
			fprintf( stdout, "Acquiry started.\n" );
		}
		if( callback != NULL )
		{
			ewfoutput_process_status_initialize(
			 stdout,
			 _CHARACTER_T_STRING( "acquired" ),
			 timestamp_start );
		}
		fprintf( stdout, "This could take a while.\n\n" );

		write_count = ewfcommon_write_from_file_descriptor(
			       ewfcommon_libewf_handle,
			       0,
			       acquiry_size,
			       acquiry_offset,
			       (uint32_t) sectors_per_chunk,
			       512,
			       read_error_retry,
			       sector_error_granularity,
			       wipe_chunk_on_error,
			       seek_on_error,
			       calculate_md5,
			       calculated_md5_hash_string,
			       EWFSTRING_DIGEST_HASH_LENGTH_MD5,
			       calculate_sha1,
			       calculated_sha1_hash_string,
			       EWFSTRING_DIGEST_HASH_LENGTH_SHA1,
			       swap_byte_pairs,
			       callback );
	}
	if( ewfcommon_abort == 0 )
	{
		/* Done acquiring data
		 */
		timestamp_end = time( NULL );
		time_string   = libewf_common_ctime(
				 &timestamp_end );

		if( write_count <= -1 )
		{
			if( time_string != NULL )
			{
				fprintf( stdout, "Acquiry failed at: %" PRIs_SYSTEM "\n",
				 time_string );

				memory_free(
				 time_string );
			}
			else
			{
				fprintf( stdout, "Acquiry failed.\n" );
			}
			if( libewf_close(
			     ewfcommon_libewf_handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
			if( calculate_sha1 == 1 )
			{
				memory_free(
				 calculated_sha1_hash_string );
			}
			return( EXIT_FAILURE );
		}
		if( time_string != NULL )
		{
			fprintf( stdout, "Acquiry completed at: %" PRIs_SYSTEM "\n",
			 time_string );

			memory_free(
			 time_string );
		}
		else
		{
			fprintf( stdout, "Acquiry completed.\n" );
		}
		ewfoutput_process_summary_fprint(
		 stdout,
		 _CHARACTER_T_STRING( "Written" ),
		 write_count,
		 timestamp_start,
		 timestamp_end );

		fprintf( stdout, "\n" );

		if( log_filename != NULL )
		{
			log_file_stream = fopen(
					   log_filename,
					   "w" );

			if( log_file_stream == NULL )
			{
				fprintf( stderr, "Unable to open log file: %s.\n",
				 log_filename );
			}
		}
		ewfoutput_acquiry_errors_fprint(
		 stdout,
		 ewfcommon_libewf_handle,
		 &amount_of_acquiry_errors );

		if( log_file_stream != NULL )
		{
			ewfoutput_acquiry_errors_fprint(
			 log_file_stream,
			 ewfcommon_libewf_handle,
			 &amount_of_acquiry_errors );
		}
	}
	if( libewf_close(
	     ewfcommon_libewf_handle ) != 0 )
	{
		fprintf( stderr, "Unable to close EWF file(s).\n" );

		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( log_file_stream != NULL )
		{
			fclose(
			 log_file_stream );
		}
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

		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( log_file_stream != NULL )
		{
			fclose(
			 log_file_stream );
		}
		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		fprintf( stdout, "MD5 hash calculated over data:\t%" PRIs "\n",
		 calculated_md5_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf( log_file_stream, "MD5 hash calculated over data:\t%" PRIs "\n",
			 calculated_md5_hash_string );
		}
		memory_free(
		 calculated_md5_hash_string );
	}
	if( calculate_sha1 == 1 )
	{
		fprintf( stdout, "SHA1 hash calculated over data:\t%" PRIs "\n",
		 calculated_sha1_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf( log_file_stream, "SHA1 hash calculated over data:\t%" PRIs "\n",
			 calculated_sha1_hash_string );
		}
		memory_free(
		 calculated_sha1_hash_string );
	}
	if( log_file_stream != NULL )
	{
		if( fclose(
		     log_file_stream ) != 0 )
		{
			fprintf( stderr, "Unable to close log file: %s.\n",
			 log_filename );
		}
	}
	return( EXIT_SUCCESS );
}

