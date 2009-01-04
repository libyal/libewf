/*
 * ewfacquirestream
 * Reads data from a stdin and writes it in EWF format
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#include "../libewf/libewf_includes.h"

#include <errno.h>
#include <stdio.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <libewf.h>

#include "../libewf/libewf_char.h"
#include "../libewf/libewf_common.h"
#include "../libewf/libewf_notify.h"
#include "../libewf/libewf_string.h"

#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfglob.h"
#include "ewfsignal.h"

/* Prints the executable usage information
 */
void usage( void )
{
	fprintf( stderr, "Usage: ewfacquirestream [ -b amount_of_sectors ] [ -c compression_type ] [ -C case_number ] [ -d digest_type ] [ -D description ]\n" );
	fprintf( stderr, "                        [ -e examiner_name ] [ -E evidence_number ] [ -f format ] [ -m media_type ] [ -M volume_type ] [ -N notes ]\n" );
	fprintf( stderr, "                        [ -S segment_file_size ] [ -t target ] [ -hsvV ]\n\n" );

	fprintf( stderr, "\tReads data from stdin\n\n" );

	fprintf( stderr, "\t-b: specify the amount of sectors to read at once (per chunk), options: 64 (default), 128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n" );
	fprintf( stderr, "\t-c: specify the compression type, options: none (is default), empty_block, fast, best\n" );
	fprintf( stderr, "\t-C: specify the case number (default is case_number).\n" );
	fprintf( stderr, "\t-d: calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stderr, "\t-D: specify the description (default is description).\n" );
	fprintf( stderr, "\t-e: specify the examiner name (default is examiner_name).\n" );
	fprintf( stderr, "\t-E: specify the evidence number (default is evidence_number).\n" );
	fprintf( stderr, "\t-f: specify the EWF file format to write to, options: ftk, encase2, encase3, encase4,\n" );
	fprintf( stderr, "\t    encase5 (is default), encase6, linen5, linen6, ewfx\n" );
	fprintf( stderr, "\t-h: shows this help\n" );
	fprintf( stderr, "\t-m: specify the media type, options: fixed (is default), removable\n" );
	fprintf( stderr, "\t-M: specify the volume type, options: logical (is default), physical\n" );
	fprintf( stderr, "\t-N: specify the notes (default is notes).\n" );
	fprintf( stderr, "\t-s: swap byte pairs of the media data (from AB to BA)\n" );
	fprintf( stderr, "\t    (use this for big to little endian conversion and vice versa)\n" );
	fprintf( stderr, "\t-S: specify the segment file size in kbytes (2^10) (default is %" PRIu32 ")\n", (uint32_t) ( 650 * 1024 ) );
	fprintf( stderr, "\t-t: specify the target file (without extension) to write to (default is stream)\n" );
	fprintf( stderr, "\t-v: verbose output to stderr\n" );
	fprintf( stderr, "\t-V: print version\n" );
}

/* The main program
 */
#ifdef HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined(HAVE_UUID_UUID_H) && defined(HAVE_LIBUUID)
	uint8_t guid[ 16 ];
#endif
	CHAR_T *filenames[ 1 ]                  = { "stream" };

	LIBEWF_HANDLE *handle                    = NULL;
	LIBEWF_CHAR *calculated_md5_hash_string  = NULL;
	LIBEWF_CHAR *calculated_sha1_hash_string = NULL;
	LIBEWF_CHAR *case_number                 = NULL;
	LIBEWF_CHAR *description                 = NULL;
	LIBEWF_CHAR *evidence_number             = NULL;
	LIBEWF_CHAR *examiner_name               = NULL;
	LIBEWF_CHAR *notes                       = NULL;
	LIBEWF_CHAR *acquiry_operating_system    = NULL;
	LIBEWF_CHAR *acquiry_software_version    = NULL;
	CHAR_T *option_case_number               = NULL;
	CHAR_T *option_description               = NULL;
	CHAR_T *option_examiner_name             = NULL;
	CHAR_T *option_evidence_number           = NULL;
	CHAR_T *option_notes                     = NULL;
	CHAR_T *time_string                      = NULL;
	CHAR_T *end_of_string                    = NULL;
	void *callback                           = &ewfcommon_stream_process_status_fprint;

	INT_T option                             = 0;
	size_t string_length                     = 0;
	time_t timestamp_start                   = 0;
	time_t timestamp_end                     = 0;
	int64_t segment_file_size                = ( 650 * 1024 );
	int64_t count                            = 0;
	uint64_t acquiry_offset                  = 0;
	uint64_t acquiry_size                    = 0;
	uint64_t sectors_per_chunk               = 64;
	uint32_t sector_error_granularity        = 64;
	int8_t compression_level                 = LIBEWF_COMPRESSION_NONE;
	int8_t result_md5_hash                   = 0;
	int8_t result_sha1_hash                  = 0;
	uint8_t media_type                       = LIBEWF_MEDIA_TYPE_FIXED;
	uint8_t volume_type                      = LIBEWF_VOLUME_TYPE_LOGICAL;
	uint8_t compress_empty_block             = 0;
	uint8_t libewf_format                    = LIBEWF_FORMAT_ENCASE5;
	uint8_t wipe_block_on_read_error         = 0;
	uint8_t read_error_retry                 = 2;
	uint8_t swap_byte_pairs                  = 0;
	uint8_t seek_on_error                    = 0;
	uint8_t calculate_sha1                   = 0;
	uint8_t verbose                          = 0;

	ewfsignal_initialize();

	ewfcommon_version_fprint( stderr, _S_LIBEWF_CHAR( "ewfacquirestream" ) );

	while( ( option = ewfgetopt( argc, argv, _S_CHAR_T( "b:c:C:d:D:e:E:f:hm:M:N:sS:t:vV" ) ) ) != (INT_T) -1 )
	{
		switch( option )
		{
			case (INT_T) '?':
			default:
				fprintf( stderr, "Invalid argument: %" PRIs "\n", argv[ optind ] );

				usage();

				return( EXIT_FAILURE );

			case (INT_T) 'b':
				if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "32768" ), 5 ) == 0 )
				{
					sectors_per_chunk = 32768;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "16384" ), 5 ) == 0 )
				{
					sectors_per_chunk = 16384;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "8192" ), 4 ) == 0 )
				{
					sectors_per_chunk = 8192;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "4096" ), 4 ) == 0 )
				{
					sectors_per_chunk = 4096;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "2048" ), 4 ) == 0 )
				{
					sectors_per_chunk = 2048;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "1024" ), 4 ) == 0 )
				{
					sectors_per_chunk = 1024;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "512" ), 3 ) == 0 )
				{
					sectors_per_chunk = 512;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "256" ), 3 ) == 0 )
				{
					sectors_per_chunk = 256;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "128" ), 3 ) == 0 )
				{
					sectors_per_chunk = 128;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "64" ), 2 ) == 0 )
				{
					sectors_per_chunk = 64;
				}
				else
				{
					fprintf( stderr, "unsuported amount of sectors per chunk defaulting to 64.\n" );
				}
				break;

			case (INT_T) 'c':
				if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "none" ), 4 ) == 0 )
				{
					compression_level = LIBEWF_COMPRESSION_NONE;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "empty_block" ), 11 ) == 0 )
				{
					compress_empty_block = 1;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "fast" ), 4 ) == 0 )
				{
					compression_level = LIBEWF_COMPRESSION_FAST;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "best" ), 4 ) == 0 )
				{
					compression_level = LIBEWF_COMPRESSION_BEST;
				}
				else
				{
					fprintf( stderr, "unsuported compression type defaulting to none.\n" );
				}
				break;

			case (INT_T) 'C':
				option_case_number = optarg;

				break;

			case (INT_T) 'd':
				if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "sha1" ), 4 ) == 0 )
				{
					calculate_sha1 = 1;
				}
				else
				{
					fprintf( stderr, "unsuported digest type.\n" );
				}
				break;

			case (INT_T) 'D':
				option_description = optarg;

				break;

			case (INT_T) 'e':
				option_examiner_name = optarg;

				break;

			case (INT_T) 'E':
				option_evidence_number = optarg;

				break;

			case (INT_T) 'f':
				if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "ftk" ), 3 ) == 0 )
				{
					libewf_format = LIBEWF_FORMAT_FTK;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "encase2" ), 7 ) == 0 )
				{
					libewf_format = LIBEWF_FORMAT_ENCASE2;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "encase3" ), 7 ) == 0 )
				{
					libewf_format = LIBEWF_FORMAT_ENCASE3;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "encase4" ), 7 ) == 0 )
				{
					libewf_format = LIBEWF_FORMAT_ENCASE4;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "encase5" ), 7 ) == 0 )
				{
					libewf_format = LIBEWF_FORMAT_ENCASE5;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "encase6" ), 7 ) == 0 )
				{
					libewf_format = LIBEWF_FORMAT_ENCASE6;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "linen5" ), 6 ) == 0 )
				{
					libewf_format = LIBEWF_FORMAT_LINEN5;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "linen6" ), 6 ) == 0 )
				{
					libewf_format = LIBEWF_FORMAT_LINEN6;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "ewfx" ), 4 ) == 0 )
				{
					libewf_format = LIBEWF_FORMAT_EWFX;
				}
				else
				{
					fprintf( stderr, "unsuported EWF file format type defaulting to encase5.\n" );
				}
				break;

			case (INT_T) 'h':
				usage();

				return( EXIT_SUCCESS );

			case (INT_T) 'm':
				if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "fixed" ), 5 ) == 0 )
				{
					media_type = LIBEWF_MEDIA_TYPE_FIXED;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "removable" ), 9 ) == 0 )
				{
					media_type = LIBEWF_MEDIA_TYPE_REMOVABLE;
				}
				else
				{
					fprintf( stderr, "unsuported media type defaulting to fixed.\n" );
				}
				break;

			case (INT_T) 'M':
				if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "logical" ), 7 ) == 0 )
				{
					volume_type = LIBEWF_VOLUME_TYPE_LOGICAL;
				}
				else if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "physical" ), 8 ) == 0 )
				{
					volume_type = LIBEWF_VOLUME_TYPE_PHYSICAL;
				}
				else
				{
					fprintf( stderr, "unsuported volume type defaulting to logical.\n" );
				}
				break;

			case (INT_T) 'N':
				option_notes = optarg;

				break;

			case (INT_T) 'q':

				break;

			case (INT_T) 's':
				swap_byte_pairs = 1;

				break;

			case (INT_T) 'S':
				string_length     = CHAR_T_LENGTH( optarg );
				end_of_string     = &optarg[ string_length - 1 ];
				segment_file_size = CHAR_T_TOLONG( optarg, &end_of_string, 0 );

				break;

			case (INT_T) 't':
				filenames[ 0 ] = optarg;

				break;

			case (INT_T) 'v':
				verbose = 1;

				break;

			case (INT_T) 'V':
				ewfcommon_copyright_fprint( stderr );

				return( EXIT_SUCCESS );
		}
	}
	libewf_set_notify_values( stderr, verbose );

	segment_file_size *= 1024;

	/* Make sure the segment file size is 1 byte smaller than 2 Gb (2 * 1024 * 1024 * 1024)
	 */
	if( segment_file_size >= (int64_t) INT32_MAX )
	{
		segment_file_size = (int64_t) INT32_MAX - 1;
	}
	/* And larger than 1440 kb (1440 * 1024)
	 */
	else if( segment_file_size < (1440 * 1024) )
	{
		segment_file_size = 1440 * 1024;
	}
	if( option_case_number != NULL )
	{
		string_length = CHAR_T_LENGTH( option_case_number );

		if( string_length > 0 )
		{
			string_length += 1;
			case_number    = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * string_length );

			if( case_number == NULL )
			{
				fprintf( stderr, "Unable to create case number string.\n" );

				return( EXIT_FAILURE );
			}
			if( ewfcommon_copy_libewf_char_from_char_t( case_number, option_case_number, string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set case number string.\n" );

				return( EXIT_FAILURE );
			}
		}
	}
	if( option_description != NULL )
	{
		string_length = CHAR_T_LENGTH( option_description );

		if( string_length > 0 )
		{
			string_length += 1;
			description    = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * string_length );

			if( description == NULL )
			{
				fprintf( stderr, "Unable to create description string.\n" );

				return( EXIT_FAILURE );
			}
			if( ewfcommon_copy_libewf_char_from_char_t( description, option_description, string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set description string.\n" );

				return( EXIT_FAILURE );
			}
		}
	}
	if( option_examiner_name != NULL )
	{
		string_length = CHAR_T_LENGTH( option_examiner_name );

		if( string_length > 0 )
		{
			string_length += 1;
			examiner_name  = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * string_length );

			if( examiner_name == NULL )
			{
				fprintf( stderr, "Unable to create examiner name string.\n" );

				return( EXIT_FAILURE );
			}
			if( ewfcommon_copy_libewf_char_from_char_t( examiner_name, option_examiner_name, string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set examiner name string.\n" );

				return( EXIT_FAILURE );
			}
		}
	}
	if( option_evidence_number != NULL )
	{
		string_length = CHAR_T_LENGTH( option_evidence_number );

		if( string_length > 0 )
		{
			string_length  += 1;
			evidence_number = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * string_length );

			if( evidence_number == NULL )
			{
				fprintf( stderr, "Unable to create evidence number string.\n" );

				return( EXIT_FAILURE );
			}
			if( ewfcommon_copy_libewf_char_from_char_t( evidence_number, option_evidence_number, string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set evidence number string.\n" );

				return( EXIT_FAILURE );
			}
		}
	}
	if( option_notes != NULL )
	{
		string_length = CHAR_T_LENGTH( option_notes );

		if( string_length > 0 )
		{
			string_length += 1;
			notes          = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * string_length );

			if( notes == NULL )
			{
				fprintf( stderr, "Unable to create notes string.\n" );

				return( EXIT_FAILURE );
			}
			if( ewfcommon_copy_libewf_char_from_char_t( notes, option_notes, string_length ) != 1 )
			{
				fprintf( stderr, "Unable to set notes string.\n" );

				return( EXIT_FAILURE );
			}
		}
	}
	acquiry_operating_system = ewfcommon_determine_operating_system();
	acquiry_software_version = LIBEWF_VERSION;

	fprintf( stdout, "Using the following acquiry parameters:\n" );

	ewfcommon_acquiry_paramters_fprint( stdout, filenames[ 0 ], case_number, description, evidence_number, examiner_name, notes, media_type, volume_type, compression_level, compress_empty_block, libewf_format, acquiry_offset, acquiry_size, (uint32_t) segment_file_size, sectors_per_chunk, sector_error_granularity, read_error_retry, wipe_block_on_read_error );

	handle = libewf_open( (CHAR_T * const *) filenames, 1, LIBEWF_OPEN_WRITE );

	if( handle == NULL )
	{
		fprintf( stderr, "Unable to create EWF file handle.\n" );

		return( EXIT_FAILURE );
	}
	if( libewf_set_media_values( handle, (uint32_t) sectors_per_chunk, 512 ) != 1 )
	{
		fprintf( stderr, "Unable to set media values in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_set_write_segment_file_size( handle, (uint32_t) segment_file_size ) != 1 )
	{
		fprintf( stderr, "Unable to set write segment file size in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_set_write_media_type( handle, media_type, volume_type ) != 1 )
	{
		fprintf( stderr, "Unable to set write media type in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_set_write_compression_values( handle, compression_level, compress_empty_block ) != 1 )
	{
		fprintf( stderr, "Unable to set write compression values in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_set_write_format( handle, libewf_format ) != 1 )
	{
		fprintf( stderr, "Unable to set write format in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_set_swap_byte_pairs( handle, swap_byte_pairs ) != 1 )
	{
		fprintf( stderr, "Unable to set swap byte pairs in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( case_number == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = libewf_string_length( case_number );
	}
	if( libewf_set_header_value_case_number( handle, case_number, string_length ) != 1 )
	{
		fprintf( stderr, "Unable to set header value case number in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	libewf_common_free( case_number );

	if( description == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = libewf_string_length( description );
	}
	if( libewf_set_header_value_description( handle, description, string_length ) != 1 )
	{
		fprintf( stderr, "Unable to set header value description in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	libewf_common_free( description );

	if( examiner_name == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = libewf_string_length( examiner_name );
	}
	if( libewf_set_header_value_examiner_name( handle, examiner_name, string_length ) != 1 )
	{
		fprintf( stderr, "Unable to set header value examiner name in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	libewf_common_free( examiner_name );

	if( evidence_number == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = libewf_string_length( evidence_number );
	}
	if( libewf_set_header_value_evidence_number( handle, evidence_number, string_length ) != 1 )
	{
		fprintf( stderr, "Unable to set header value evidence number in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	libewf_common_free( evidence_number );

	if( notes == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = libewf_string_length( notes );
	}
	if( libewf_set_header_value_notes( handle, notes, string_length ) != 1 )
	{
		fprintf( stderr, "Unable to set header value notes in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	libewf_common_free( notes );

	/* Acquiry date, system date and compression type will be generated automatically when set to NULL
	 */
	if( acquiry_operating_system != NULL )
	{
		if( libewf_set_header_value_acquiry_operating_system( handle, acquiry_operating_system, libewf_string_length( acquiry_operating_system ) ) != 1 )
		{
			fprintf( stderr, "Unable to set header value acquiry operating system in handle.\n" );

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file handle.\n" );
			}
			return( EXIT_FAILURE );
		}
		libewf_common_free( acquiry_operating_system );
	}
	if( libewf_set_header_value( handle, _S_LIBEWF_CHAR( "acquiry_software" ), _S_LIBEWF_CHAR( "ewfacquirestream" ), 16 ) != 1 )
	{
		fprintf( stderr, "Unable to set header value acquiry software in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_set_header_value_acquiry_software_version( handle, acquiry_software_version, libewf_string_length( acquiry_software_version ) ) != 1 )
	{
		fprintf( stderr, "Unable to set header value acquiry software version number in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
#if defined(HAVE_UUID_UUID_H) && defined(HAVE_LIBUUID)
	/* Add a system GUID if necessary
	 */
	if( ewfcommon_determine_guid( guid, libewf_format ) != 1 )
	{
		fprintf( stderr, "Unable to create GUID.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_set_guid( handle, guid, 16 ) != 1 )
	{
		fprintf( stderr, "Unable to set GUID in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
#endif
	/* Start acquiring data
	 */
	timestamp_start = time( NULL );
	time_string     = libewf_common_ctime( &timestamp_start );

	if( time_string != NULL )
	{
		fprintf( stderr, "Acquiry started at: %" PRIs "\n", time_string );

		libewf_common_free( time_string );
	}
	else
	{
		fprintf( stderr, "Acquiry started.\n" );
	}
	if( callback != NULL )
	{
		ewfcommon_process_status_initialize( stderr, _S_LIBEWF_CHAR( "acquired" ), timestamp_start );
	}
	fprintf( stderr, "This could take a while.\n\n" );

	count = ewfcommon_write_from_file_descriptor( handle, 0, acquiry_size, acquiry_offset, read_error_retry, sector_error_granularity, wipe_block_on_read_error, seek_on_error, calculate_sha1, callback );

	/* Done acquiring data
	 */
	if( count > -1 )
	{
		calculated_md5_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated MD5 hash string.\n" );

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file handle.\n" );
			}
			return( EXIT_FAILURE );
		}
		result_md5_hash = libewf_get_calculated_md5_hash( handle, calculated_md5_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 );

		if( calculate_sha1 == 1 )
		{
			calculated_sha1_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );

			if( calculated_sha1_hash_string == NULL )
			{
				fprintf( stderr, "Unable to create calculated SHA1 hash string.\n" );

				libewf_common_free( calculated_md5_hash_string );

				if( libewf_close( handle ) != 0 )
				{
					fprintf( stderr, "Unable to close EWF file handle.\n" );
				}
				return( EXIT_FAILURE );
			}
			result_sha1_hash = libewf_get_hash_value( handle, _S_LIBEWF_CHAR( "SHA1" ), calculated_sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );
		}
	}
	timestamp_end = time( NULL );
	time_string   = libewf_common_ctime( &timestamp_end );

	if( count <= -1 )
	{
		if( time_string != NULL )
		{
			fprintf( stderr, "Acquiry failed at: %" PRIs "\n", time_string );

			libewf_common_free( time_string );
		}
		else
		{
			fprintf( stderr, "Acquiry failed.\n" );
		}
		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( time_string != NULL )
	{
		fprintf( stderr, "Acquiry completed at: %" PRIs "\n", time_string );

		libewf_common_free( time_string );
	}
	else
	{
		fprintf( stderr, "Acquiry completed.\n" );
	}
	ewfcommon_process_summary_fprint( stderr, _S_LIBEWF_CHAR( "Written" ), count, timestamp_start, timestamp_end );

	fprintf( stderr, "\n" );

	ewfcommon_acquiry_errors_fprint( stderr, handle );

	if( libewf_close( handle ) != 0 )
	{
		fprintf( stderr, "Unable to close EWF file handle.\n" );

		libewf_common_free( calculated_md5_hash_string );

		if( calculate_sha1 == 1 )
		{
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	if( result_md5_hash == -1 )
	{
		fprintf( stderr, "Unable to get calculated MD5 hash.\n" );

		libewf_common_free( calculated_md5_hash_string );

		if( calculate_sha1 == 1 )
		{
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	else if( result_md5_hash == 0 )
	{
		fprintf( stderr, "MD5 hash calculated over data: N/A\n" );
	}
	else
	{
		fprintf( stderr, "MD5 hash calculated over data: %" PRIs_EWF "\n", calculated_md5_hash_string );
	}
	libewf_common_free( calculated_md5_hash_string );

	if( calculate_sha1 == 1 )
	{
		if( result_sha1_hash == -1 )
		{
			fprintf( stderr, "Unable to get calculated SHA1 hash.\n" );

			libewf_common_free( calculated_sha1_hash_string );

			return( EXIT_FAILURE );
		}
		else if( result_sha1_hash == 0 )
		{
			fprintf( stderr, "SHA1 hash calculated over data:\tN/A\n" );
		}
		else
		{
			fprintf( stderr, "SHA1 hash calculated over data:\t%" PRIs_EWF "\n", calculated_sha1_hash_string );
		}
		libewf_common_free( calculated_sha1_hash_string );
	}
	return( EXIT_SUCCESS );
}

