/*
 * ewfacquire
 * Reads data from a file and writes it in EWF format
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

#include <liberror.h>

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

#if defined( HAVE_CYGWIN_FS_H )
#include <cygwin/fs.h>
#endif

#if defined( HAVE_LINUX_FS_H )

/* Required for Linux platforms that use a sizeof( u64 )
 * in linux/fs.h but have no typedef of it
 */
#if ! defined( HAVE_U64 )
typedef size_t u64;
#endif

#include <linux/fs.h>
#endif

#if defined( HAVE_SYS_DISK_H )
#include <sys/disk.h>
#endif

#if defined( HAVE_SYS_DISKLABEL_H )
#include <sys/disklabel.h>
#endif

#include <stdio.h>

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
#include "file_io.h"
#include "file_stream_io.h"
#include "glob.h"
#include "notify.h"
#include "process_status.h"
#include "system_string.h"

#define EWFACQUIRE_INPUT_BUFFER_SIZE	64

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
	fprintf( stream, "Usage: ewfacquire [ -b amount_of_sectors ] [ -B amount_of_bytes ]\n"
	                 "                  [ -c compression_type ] [ -C case_number ] [ -d digest_type ]\n"
	                 "                  [ -D description ] [ -e examiner_name ] [ -E evidence_number ]\n"
	                 "                  [ -f format ] [ -g amount_of_sectors ] [ -l log_filename ]\n"
	                 "                  [ -m media_type ] [ -M volume_type ] [ -N notes ] [ -o offset ]\n"
	                 "                  [ -p process_buffer_size ] [ -r read_error_retries ]\n"
	                 "                  [ -S segment_file_size ] [ -t target ] [ -hqsuvVw ] source\n\n" );

	fprintf( stream, "\tsource: the source file or device\n\n" );

	fprintf( stream, "\t-b:     specify the amount of sectors to read at once (per chunk), options:\n"
	                 "\t        64 (default), 128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n" );
	fprintf( stream, "\t-B:     specify the amount of bytes to acquire (default is all bytes)\n" );
	fprintf( stream, "\t-c:     specify the compression type, options: none (default), empty-block, fast\n"
	                 "\t        or best\n" );
	fprintf( stream, "\t-C:     specify the case number (default is case_number).\n" );
	fprintf( stream, "\t-d:     calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stream, "\t-D:     specify the description (default is description).\n" );
	fprintf( stream, "\t-e:     specify the examiner name (default is examiner_name).\n" );
	fprintf( stream, "\t-E:     specify the evidence number (default is evidence_number).\n" );
	fprintf( stream, "\t-g      specify the amount of sectors to be used as error granularity, options:\n"
	                 "\t        64 (default), 128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n" );
	fprintf( stream, "\t-f:     specify the EWF file format to write to, options: ewf, smart, ftk,\n"
	                 "\t        encase2, encase3, encase4, encase5 (default), encase6, linen5, linen6,\n"
	                 "\t        ewfx\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs acquiry errors and the digest (hash) to the log_filename\n" );
	fprintf( stream, "\t-m:     specify the media type, options: fixed (default), removable\n" );
	fprintf( stream, "\t-M:     specify the volume type, options: logical, physical (default)\n" );
	fprintf( stream, "\t-N:     specify the notes (default is notes).\n" );
	fprintf( stream, "\t-o:     specify the offset to start to acquire (default is 0)\n" );
	fprintf( stream, "\t-p:     specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-q:     quiet shows no status information\n" );
	fprintf( stream, "\t-r:     specify the amount of retries when a read error occurs (default is 2)\n" );
	fprintf( stream, "\t-s:     swap byte pairs of the media data (from AB to BA)\n"
	                 "\t        (use this for big to little endian conversion and vice versa)\n" );

	if( result == 1 )
	{
		fprintf( stream, "\t-S:     specify the segment file size in bytes (default is %" PRIs_SYSTEM ")\n"
		                 "\t        (minimum is %" PRIs_SYSTEM ", maximum is %" PRIs_SYSTEM " for encase6 format\n"
		                 "\t        and %" PRIs_SYSTEM " for other formats)\n",
		 default_segment_file_size_string,
		 minimum_segment_file_size_string,
		 maximum_64bit_segment_file_size_string,
		 maximum_32bit_segment_file_size_string );
	}
	else
	{
		fprintf( stream, "\t-S:     specify the segment file size in bytes (default is %" PRIu32 ")\n"
		                 "\t        (minimum is %" PRIu32 ", maximum is %" PRIu64 " for encase6 format\n"
		                 "\t        and %" PRIu32 " for other formats)\n",
		 (uint32_t) EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
		 (uint32_t) EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
		 (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
		 (uint32_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT );
	}

	fprintf( stream, "\t-t:     specify the target file (without extension) to write to (default is acquire)\n" );
	fprintf( stream, "\t-u:     unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
	fprintf( stream, "\t-w:     wipe sectors on read error (mimic EnCase like behavior)\n" );
}

/* Prints an overview of the acquiry parameters and asks the for confirmation
 * Return 1 if confirmed by user, 0 otherwise or -1 on error
 */
int8_t confirm_acquiry_parameters(
        FILE *output_stream,
        system_character_t *input_buffer,
        size_t input_buffer_size,
        system_character_t *target_filename,
        system_character_t *case_number,
        system_character_t *description,
        system_character_t *evidence_number,
        system_character_t *examiner_name,
        system_character_t *notes,
        uint8_t media_type,
        uint8_t volume_type,
        int8_t compression_level,
        uint8_t compress_empty_block,
        uint8_t libewf_format,
        off64_t acquiry_offset,
        size64_t acquiry_size,
        size64_t segment_file_size,
        uint32_t sectors_per_chunk,
        uint32_t sector_error_granularity,
        uint8_t read_error_retry,
        uint8_t wipe_chunk_on_error )
{
	system_character_t *fixed_string_variable = NULL;
	static char *function                     = "confirm_acquiry_parameters";
	int8_t input_confirmed                    = -1;

	if( output_stream == NULL )
	{
		notify_warning_printf( "%s: invalid output stream.\n",
		 function );

		return( -1 );
	}
	fprintf(
	 output_stream,
	 "The following acquiry parameters were provided:\n" );

	ewfoutput_acquiry_parameters_fprint(
	 output_stream,
	 target_filename,
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
	 acquiry_offset,
	 acquiry_size,
	 segment_file_size,
	 sectors_per_chunk,
	 sector_error_granularity,
	 read_error_retry,
	 wipe_chunk_on_error );

	/* Ask for confirmation
	 */
	while( input_confirmed == -1 )
	{
		if( ewfinput_get_fixed_string_variable(
		     output_stream,
		     input_buffer,
		     input_buffer_size,
		     _SYSTEM_CHARACTER_T_STRING( "Continue acquiry with these values" ),
		     ewfinput_yes_no,
		     2,
		     0,
		     &fixed_string_variable ) == -1 )
		{
			fprintf(
			 output_stream,
			 "Unable to determine answer.\n" );
		}
		else if( ewfinput_determine_yes_no(
		          fixed_string_variable,
		          (uint8_t *) &input_confirmed ) != 1 )
		{
			fprintf(
			 output_stream,
			 "Selected option not supported, please try again or terminate using Ctrl^C.\n" );

			input_confirmed = -1;
		}
	}
	fprintf(
	 output_stream,
	 "\n" );

	return( input_confirmed );
}

/* Determine the device size using a file descriptor
 */
size64_t determine_device_size(
          int file_descriptor )
{
#if !defined( DIOCGMEDIASIZE ) && defined( DIOCGDINFO )
	struct disklabel disk_label;
#endif
	size64_t input_size  = 0;
#if defined( DKIOCGETBLOCKCOUNT )
	uint64_t block_count = 0;
	uint32_t block_size  = 0;
#endif
	if( file_descriptor == -1 )
	{
		return( 0 );
	}
#if defined( BLKGETSIZE64 )
	if( ioctl(
	     file_descriptor,
	     BLKGETSIZE64,
	     &input_size ) == -1 )
	{
		return( 0 );
	}
#elif defined( DIOCGMEDIASIZE )
	if( ioctl(
	     file_descriptor,
	     DIOCGMEDIASIZE,
	     &input_size ) == -1 )
	{
		return( 0 );
	}
#elif defined( DIOCGDINFO )
	if( ioctl(
	     file_descriptor,
	     DIOCGDINFO,
	     &disk_label ) == -1 )
	{
		return( 0 );
	}
	input_size = disk_label.d_secperunit * disk_label.d_secsize;
#elif defined( DKIOCGETBLOCKCOUNT )
	if( ioctl(
	     file_descriptor,
	     DKIOCGETBLOCKSIZE, &block_size ) == -1 )
	{
		return( 0 );
	}
	if( ioctl(
	     file_descriptor,
	     DKIOCGETBLOCKCOUNT,
	     &block_count ) == -1 )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	fprintf(
	 stderr, "block size: %" PRIu32 " block count: %" PRIu64 " ",
	 block_size,
	 block_count );
#endif

	input_size = (size64_t) block_count * (size64_t) block_size;
#else
	input_size = 0;
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	fprintf(
	 stderr,
	 "device size: %" PRIu64 "\n",
	 input_size );
#endif
	return( input_size );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	system_character_t acquiry_operating_system[ 32 ];
	system_character_t input_buffer[ EWFACQUIRE_INPUT_BUFFER_SIZE ];

	struct stat input_file_stat;

	liberror_error_t *error                         = NULL;

	system_character_t *target_filenames[ 1 ]       = { NULL };

	system_character_t *acquiry_software_version    = NULL;
	system_character_t *calculated_md5_hash_string  = NULL;
	system_character_t *calculated_sha1_hash_string = NULL;
	system_character_t *case_number                 = NULL;
	system_character_t *description                 = NULL;
	system_character_t *evidence_number             = NULL;
	system_character_t *examiner_name               = NULL;
	system_character_t *fixed_string_variable       = NULL;
	system_character_t *log_filename                = NULL;
	system_character_t *notes                       = NULL;
	system_character_t *option_case_number          = NULL;
	system_character_t *option_description          = NULL;
	system_character_t *option_examiner_name        = NULL;
	system_character_t *option_evidence_number      = NULL;
	system_character_t *option_notes                = NULL;
	system_character_t *option_target_filename      = NULL;
	system_character_t *target_filename             = NULL;

	FILE *log_file_stream                           = NULL;
	char *program                                   = "ewfacquire";
	void *callback                                  = &process_status_update;

	system_integer_t option                         = 0;
	ssize64_t write_count                           = 0;
	size_t string_length                            = 0;
	uint64_t acquiry_offset                         = 0;
	uint64_t acquiry_size                           = 0;
	uint64_t input_size_variable                    = 0;
	uint64_t input_size                             = 0;
	uint64_t maximum_segment_file_size              = 0;
	uint64_t process_buffer_size                    = 0;
	uint64_t segment_file_size                      = 0;
	uint32_t amount_of_acquiry_errors               = 0;
	uint32_t sector_error_granularity               = 0;
	uint32_t sectors_per_chunk                      = 0;
	uint8_t calculate_md5                           = 1;
	uint8_t calculate_sha1                          = 0;
	uint8_t compress_empty_block                    = 0;
	uint8_t libewf_format                           = LIBEWF_FORMAT_ENCASE5;
	uint8_t media_type                              = LIBEWF_MEDIA_TYPE_FIXED;
	uint8_t read_error_retry                        = 2;
	uint8_t swap_byte_pairs                         = 0;
	uint8_t seek_on_error                           = 1;
	uint8_t verbose                                 = 0;
	uint8_t volume_type                             = LIBEWF_VOLUME_TYPE_LOGICAL;
	uint8_t wipe_chunk_on_error                     = 0;
	int8_t acquiry_parameters_confirmed             = 0;
	int8_t compression_level                        = LIBEWF_COMPRESSION_NONE;
	int argument_set_compression                    = 0;
	int argument_set_format                         = 0;
	int argument_set_media_type                     = 0;
	int argument_set_offset                         = 0;
	int argument_set_read_error_retry               = 0;
	int argument_set_sector_error_granularity       = 0;
	int argument_set_sectors_per_chunk              = 0;
	int argument_set_segment_file_size              = 0;
	int argument_set_size                           = 0;
	int argument_set_volume_type                    = 0;
	int argument_set_wipe_chunk_on_error            = 0;
	int error_abort                                 = 0;
	int file_descriptor                             = 0;
	int interactive_mode                            = 1;
	int result                                      = 0;
	int status                                      = 0;

	notify_set_values(
	 stderr,
	 verbose );

	if( system_string_initialize(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system string.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	ewfoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = ewfgetopt(
	                   argc,
	                   argv,
	                   _SYSTEM_CHARACTER_T_STRING( "b:B:c:C:d:D:e:E:f:g:hl:m:M:N:o:p:qr:sS:t:uvVw" ) ) ) != (system_integer_t) -1 )
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
				if( ewfinput_determine_sectors_per_chunk(
				     optarg,
				     &sector_error_granularity ) != 1 )
				{
					fprintf( stderr, "Unsuported amount of sector error granularity defaulting to: 64.\n" );

					sector_error_granularity = 64;
				}
				else
				{
					argument_set_sector_error_granularity = 1;
				}
				break;

			case (system_integer_t) 'B':
				string_length = system_string_length(
				                 optarg );

				if( system_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &acquiry_size,
				     NULL ) != 1 )
				{
					acquiry_size = 0;

					fprintf(
					 stderr,
					 "Unsupported acquiry size defaulting to: all bytes.\n" );
				}
				argument_set_size = 1;

				break;

			case (system_integer_t) 'c':
				if( ewfinput_determine_compression_level(
				     optarg,
				     &compression_level,
				     &compress_empty_block ) != 1 )
				{
					fprintf( stderr, "Unsupported compression type defaulting to: none.\n" );

					compression_level    = LIBEWF_COMPRESSION_NONE;
					compress_empty_block = 0;
				}
				else
				{
					argument_set_compression = 1;
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
				if( ewfinput_determine_libewf_format(
				     optarg,
				     &libewf_format ) != 1 )
				{
					fprintf( stderr, "Unsupported EWF file format type defaulting to: encase5.\n" );

					libewf_format = LIBEWF_FORMAT_ENCASE5;
				}
				else
				{
					argument_set_format = 1;
				}
				break;

			case (system_integer_t) 'g':
				if( ewfinput_determine_sectors_per_chunk(
				     optarg,
				     &sectors_per_chunk ) != 1 )
				{
					fprintf( stderr, "Unsuported amount of sectors per chunk defaulting to: 64.\n" );

					sectors_per_chunk = 64;
				}
				else
				{
					argument_set_sectors_per_chunk = 1;
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
				if( ewfinput_determine_media_type(
				     optarg,
				     &media_type ) != 1 )
				{
					fprintf( stderr, "Unsupported media type defaulting to: fixed.\n" );

					media_type = LIBEWF_MEDIA_TYPE_FIXED;
				}
				else
				{
					argument_set_media_type = 1;
				}
				break;

			case (system_integer_t) 'M':
				if( ewfinput_determine_volume_type(
				     optarg,
				     &volume_type ) != 1 )
				{
					fprintf( stderr, "Unsupported volume type defaulting to: logical.\n" );

					volume_type = LIBEWF_VOLUME_TYPE_LOGICAL;
				}
				else
				{
					argument_set_volume_type = 1;
				}
				break;

			case (system_integer_t) 'N':
				option_notes = optarg;

				break;

			case (system_integer_t) 'o':
				string_length = system_string_length(
				                 optarg );

				if( system_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &acquiry_offset,
				     NULL ) != 1 )
				{
					acquiry_offset = 0;

					fprintf(
					 stderr,
					 "Unsupported acquiry offset defaulting to: %" PRIu64 ".\n",
					 acquiry_offset );
				}
				argument_set_offset = 1;

				break;

			case (system_integer_t) 'p':
				string_length = system_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &process_buffer_size,
				          &error );

				if( result != 1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				if( ( result != 1 )
				 || ( process_buffer_size > (uint64_t) SSIZE_MAX ) )
				{
					process_buffer_size = 0;

					fprintf( stderr, "Unsupported process buffer size defaulting to: chunk size.\n" );
				}
				break;

			case (system_integer_t) 'q':
				callback = NULL;

				break;

			case (system_integer_t) 'r':
				string_length = system_string_length(
				                 optarg );

				if( ( system_string_to_uint64(
				       optarg,
				       string_length + 1,
				       &input_size_variable,
				       NULL ) != 1 )
				 || ( input_size_variable > 255 ) )
				{
					input_size_variable = 2;

					fprintf(
					 stderr,
					 "Unsupported amount of read error retries defaulting to: %" PRIu64 ".\n",
					 input_size_variable );
				}
				read_error_retry = (uint8_t) input_size_variable;

				argument_set_read_error_retry = 1;

				break;

			case (system_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (system_integer_t) 'S':
				string_length = system_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &segment_file_size,
				          NULL );

				if( result != 1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				argument_set_segment_file_size = 1;

				if( ( result != 1 )
				 || ( segment_file_size < EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE )
				 || ( ( libewf_format == LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT ) )
				 || ( ( libewf_format != LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT ) ) )
				{
					segment_file_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

					fprintf( stderr, "Unsupported segment file size defaulting to: %" PRIu64 ".\n",
					 segment_file_size );
				}
				break;

			case (system_integer_t) 't':
				option_target_filename = optarg;

				break;

			case (system_integer_t) 'u':
				interactive_mode = 0;

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

				argument_set_wipe_chunk_on_error = 1;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf( stderr, "Missing source file or device.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	libewf_set_notify_values(
	 stderr,
	 verbose );

	/* Check if to read from stdin
	 */
	if( system_string_compare(
	     argv[ optind ],
	     _SYSTEM_CHARACTER_T_STRING( "-" ),
	     1 ) == 0 )
	{
		fprintf( stderr, "Reading from stdin not supported.\n" );

		return( EXIT_FAILURE );
	}
	/* Open the input file or device size
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
	file_descriptor = file_io_wopen(
	                   argv[ optind ],
	                   FILE_IO_O_RDONLY );
#else
	file_descriptor = file_io_open(
	                   argv[ optind ],
	                   FILE_IO_O_RDONLY );
#endif

	if( file_descriptor == -1 )
	{
		ewfoutput_error_fprint(
		 stderr, "Error opening file or device: %" PRIs_SYSTEM "",
		 argv[ optind ] );

		return( EXIT_FAILURE );
	}
	/* Check the input file or device size
	 */
	input_size = 0;

	if( fstat(
	     file_descriptor,
	     &input_file_stat ) != 0 )
	{
		fprintf( stderr, "Unable to get status information of file.\n" );

		return( EXIT_FAILURE );
	}
#if !defined( WINAPI )
	if( S_ISBLK( input_file_stat.st_mode )
	 || S_ISCHR( input_file_stat.st_mode ) )
	{
		input_size = determine_device_size(
		              file_descriptor );
	}
	else
#endif
	{
		input_size = input_file_stat.st_size;
	}
	if( input_size <= 0 )
	{
		fprintf( stderr, "Unable to determine input size.\n" );

		return( EXIT_FAILURE );
	}
	if( ( acquiry_size == 0 )
	 || ( acquiry_size > ( input_size - acquiry_offset ) ) )
	{
		acquiry_size = input_size - acquiry_offset;
	}
	/* Create the input buffers
	 */
	if( option_target_filename != NULL )
	{
		string_length = system_string_length(
				 option_target_filename );

		if( string_length > 0 )
		{
			target_filename = (system_character_t *) memory_allocate(
			                                          sizeof( system_character_t ) * ( string_length + 1 ) );

			if( target_filename == NULL )
			{
				fprintf( stderr, "Unable to create target filename string.\n" );

				error_abort = 1;
			}
			else if( memory_copy(
				  target_filename,
				  option_target_filename,
				  string_length + 1 ) == NULL )
			{
				fprintf( stderr, "Unable to set target filename string.\n" );

				error_abort = 1;
			}
		}
	}
	else
	{
		target_filename = (system_character_t *) memory_allocate(
		                                          sizeof( system_character_t ) * 1024 );

		if( target_filename == NULL )
		{
			fprintf( stderr, "Unable to create target filename string.\n" );

			error_abort = 1;
		}
		/* Make sure to set the target filename if in unattended mode
		 */
		else if( interactive_mode == 0 )
		{
			if( memory_copy(
			     target_filename,
			     _SYSTEM_CHARACTER_T_STRING( "acquiry" ),
			     7 ) == NULL )
			{
				fprintf( stderr, "Unable to set target filename string.\n" );

				error_abort = 1;
			}
			target_filename[ 7 ] = 0;
		}
	}
	if( error_abort == 0 )
	{
		if( option_case_number != NULL )
		{
			string_length = system_string_length(
					 option_case_number );

			if( string_length > 0 )
			{
				case_number = (system_character_t *) memory_allocate(
				                                      sizeof( system_character_t ) * ( string_length + 1 ) );

				if( case_number == NULL )
				{
					fprintf( stderr, "Unable to create case number string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  case_number,
					  option_case_number,
					  string_length + 1 ) == NULL )
				{
					fprintf( stderr, "Unable to set case number string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			case_number = (system_character_t *) memory_allocate(
			                                      sizeof( system_character_t ) * 256 );

			if( case_number == NULL )
			{
				fprintf( stderr, "Unable to create case number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_description != NULL )
		{
			string_length = system_string_length(
					 option_description );

			if( string_length > 0 )
			{
				description = (system_character_t *) memory_allocate(
				                                      sizeof( system_character_t ) * ( string_length + 1 ) );

				if( description == NULL )
				{
					fprintf( stderr, "Unable to create description string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  description,
					  option_description,
					  string_length + 1 ) == NULL )
				{
					fprintf( stderr, "Unable to set description string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			description = (system_character_t *) memory_allocate(
			                                      sizeof( system_character_t ) * 256 );

			if( description == NULL )
			{
				fprintf( stderr, "Unable to create description string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_examiner_name != NULL )
		{
			string_length = system_string_length(
					 option_examiner_name );

			if( string_length > 0 )
			{
				examiner_name = (system_character_t *) memory_allocate(
				                                        sizeof( system_character_t ) * ( string_length + 1 ) );

				if( examiner_name == NULL )
				{
					fprintf( stderr, "Unable to create examiner name string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  examiner_name,
					  option_examiner_name,
					  string_length + 1 ) == NULL )
				{
					fprintf( stderr, "Unable to set examiner name string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			evidence_number = (system_character_t *) memory_allocate(
			                                          sizeof( system_character_t ) * 256 );

			if( evidence_number == NULL )
			{
				fprintf( stderr, "Unable to create evidence number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_evidence_number != NULL )
		{
			string_length = system_string_length(
					 option_evidence_number );

			if( string_length > 0 )
			{
				evidence_number = (system_character_t *) memory_allocate(
				                                          sizeof( system_character_t ) * ( string_length + 1 ) );

				if( evidence_number == NULL )
				{
					fprintf( stderr, "Unable to create evidence number string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  evidence_number,
					  option_evidence_number,
					  string_length + 1 ) == NULL )
				{
					fprintf( stderr, "Unable to set evidence number string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			examiner_name = (system_character_t *) memory_allocate(
			                                        sizeof( system_character_t ) * 256 );

			if( examiner_name == NULL )
			{
				fprintf( stderr, "Unable to create examiner name string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_notes != NULL )
		{
			string_length = system_string_length(
					 option_notes );

			if( string_length > 0 )
			{
				notes = (system_character_t *) memory_allocate(
				                                sizeof( system_character_t ) * ( string_length + 1 ) );

				if( notes == NULL )
				{
					fprintf( stderr, "Unable to create notes string.\n" );

					error_abort = 1;
				}
				else if( memory_copy(
					  notes,
					  option_notes,
					  string_length + 1 ) == NULL )
				{
					fprintf( stderr, "Unable to set notes string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			notes = (system_character_t *) memory_allocate(
			                                sizeof( system_character_t ) * 256 );

			if( notes == NULL )
			{
				fprintf( stderr, "Unable to create notes string.\n" );

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
		if( target_filename != NULL )
		{
			memory_free(
			 target_filename );
		}
		return( EXIT_FAILURE );
	}
	/* Request the necessary case data
	 */
	while( ( interactive_mode == 1 )
	 && ( acquiry_parameters_confirmed == 0 ) )
	{
		fprintf( stdout, "Acquiry parameters required, please provide the necessary input\n" );

		/* Target filename
		 */
		if( option_target_filename == NULL )
		{
			while( ewfinput_get_string_variable(
				stdout,
				_SYSTEM_CHARACTER_T_STRING( "Image path and filename without extension" ),
				target_filename,
				1024 ) != 1 )
			{
				fprintf( stdout, "Filename is required, please try again or terminate using Ctrl^C.\n" );
			}
		}
		/* Case number
		 */
		if( ( option_case_number == NULL )
		 && ( ewfinput_get_string_variable(
		       stdout,
		       _SYSTEM_CHARACTER_T_STRING( "Case number" ),
		       case_number,
		       256 ) == -1 ) )
		{
			fprintf( stdout, "Unable to set case number string.\n" );

			case_number[ 0 ] = 0;
		}
		/* Description
		 */
		if( ( option_description == NULL )
		 && ( ewfinput_get_string_variable(
		       stdout,
		       _SYSTEM_CHARACTER_T_STRING( "Description" ),
		       description,
		       256 ) == -1 ) )
		{
			fprintf( stdout, "Unable to set description string.\n" );

			description[ 0 ] = 0;
		}
		/* Evidence number
		 */
		if( ( option_evidence_number == NULL )
		 && ( ewfinput_get_string_variable(
		       stdout,
		       _SYSTEM_CHARACTER_T_STRING( "Evidence number" ),
		       evidence_number,
		       256 ) == -1 ) )
		{
			fprintf( stdout, "Unable to set evidence number string.\n" );

			evidence_number[ 0 ] = 0;
		}
		/* Examiner name
		 */
		if( ( option_examiner_name == NULL )
		 && ( ewfinput_get_string_variable(
		       stdout,
		       _SYSTEM_CHARACTER_T_STRING( "Examiner name" ),
		       examiner_name,
		       256 ) == -1 ) )
		{
			fprintf( stdout, "Unable to set examiner name string.\n" );

			examiner_name[ 0 ] = 0;
		}
		/* Notes
		 */
		if( ( option_notes == NULL )
		 && ( ewfinput_get_string_variable(
		       stdout,
		       _SYSTEM_CHARACTER_T_STRING( "Notes" ),
		       notes,
		       256 ) == -1 ) )
		{
			fprintf( stdout, "Unable to set notes string.\n" );

			notes[ 0 ] = 0;
		}
		/* Media type
		 */
		if( argument_set_media_type == 0 )
		{
			if( ewfinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "Media type" ),
			     ewfinput_media_types,
			     EWFINPUT_MEDIA_TYPES_AMOUNT,
			     EWFINPUT_MEDIA_TYPES_DEFAULT,
			     &fixed_string_variable ) == -1 )
			{
				fprintf( stdout, "Unable to determine media type defaulting to: fixed.\n" );

				media_type = LIBEWF_MEDIA_TYPE_FIXED;
			}
			else if( ewfinput_determine_media_type(
				  fixed_string_variable,
				  &media_type ) != 1 )
			{
				fprintf( stdout, "Unsupported media type defaulting to: fixed.\n" );

				media_type = LIBEWF_MEDIA_TYPE_FIXED;
			}
		}
		/* Volume type
		 */
		if( argument_set_volume_type == 0 )
		{
			if( ewfinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "Volume type" ),
			     ewfinput_volume_types,
			     EWFINPUT_VOLUME_TYPES_AMOUNT,
			     EWFINPUT_VOLUME_TYPES_DEFAULT,
			     &fixed_string_variable ) == -1 )
			{
				fprintf( stdout, "Unable to determine volume type defaulting to: logical.\n" );

				volume_type = LIBEWF_VOLUME_TYPE_LOGICAL;
			}
			else if( ewfinput_determine_volume_type(
				  fixed_string_variable,
				  &volume_type ) != 1 )
			{
				fprintf( stdout, "Unsupported volume type defaulting to: logical.\n" );

				volume_type = LIBEWF_VOLUME_TYPE_LOGICAL;
			}
		}
		/* Compression
		 */
		if( argument_set_compression == 0 )
		{
			if( ewfinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "Use compression" ),
			     ewfinput_compression_levels,
			     EWFINPUT_COMPRESSION_LEVELS_AMOUNT,
			     EWFINPUT_COMPRESSION_LEVELS_DEFAULT,
			     &fixed_string_variable ) == -1 )
			{
				fprintf( stdout, "Unable to determine compression type defaulting to: none.\n" );

				compression_level    = LIBEWF_COMPRESSION_NONE;
				compress_empty_block = 0;
			}
			else if( ewfinput_determine_compression_level(
				  fixed_string_variable,
				  &compression_level,
				  &compress_empty_block ) != 1 )
			{
				fprintf( stdout, "Unsupported compression type defaulting to: none.\n" );

				compression_level    = LIBEWF_COMPRESSION_NONE;
				compress_empty_block = 0;
			}
		}
		/* File format
		 */
		if( argument_set_format == 0 )
		{
			if( ewfinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "Use EWF file format" ),
			     ewfinput_format_types,
			     EWFINPUT_FORMAT_TYPES_AMOUNT,
			     EWFINPUT_FORMAT_TYPES_DEFAULT,
			     &fixed_string_variable ) == -1 )
			{
				fprintf( stdout, "Unable to determine EWF file format type defaulting to: encase5.\n" );

				libewf_format = LIBEWF_FORMAT_ENCASE5;
			}
			else if( ewfinput_determine_libewf_format(
				  fixed_string_variable,
				  &libewf_format ) != 1 )
			{
				fprintf( stdout, "Unsupported EWF file format type defaulting to: encase5.\n" );

				libewf_format = LIBEWF_FORMAT_ENCASE5;
			}
		}
		/* Offset of data to acquire
		 */
		if( ( argument_set_offset == 0 )
		 && ( ewfinput_get_size_variable(
		       stdout,
		       input_buffer,
		       EWFACQUIRE_INPUT_BUFFER_SIZE,
		       _SYSTEM_CHARACTER_T_STRING( "Start to acquire at offset" ),
		       0,
		       input_size,
		       0,
		       &acquiry_offset ) == -1 ) )
		{
			acquiry_offset = 0;

			fprintf( stdout, "Unable to determine acquiry offset defaulting to: %" PRIu64 ".\n",
			 acquiry_offset );
		}
		/* Size of data to acquire
		 */
		if( ( argument_set_size == 0 )
		 && ( ewfinput_get_size_variable(
		       stdout,
		       input_buffer,
		       EWFACQUIRE_INPUT_BUFFER_SIZE,
		       _SYSTEM_CHARACTER_T_STRING( "Amount of bytes to acquire" ),
		       0,
		       ( input_size - acquiry_offset ),
		       ( input_size - acquiry_offset ),
		       &acquiry_size ) == -1 ) )
		{
			acquiry_size = input_size - acquiry_offset;

			fprintf( stdout, "Unable to determine input size defaulting to: %" PRIu64 ".\n",
			 acquiry_size );
		}
		/* Segment file size
		 */
		if( argument_set_segment_file_size == 0 )
		{
			if( libewf_format == LIBEWF_FORMAT_ENCASE6 )
			{
				maximum_segment_file_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT;
			}
			else
			{
				maximum_segment_file_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT;
			}
			if( ewfinput_get_byte_size_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "Evidence segment file size in bytes" ),
			     ( EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE ),
			     ( maximum_segment_file_size ),
			     ( EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE ),
			     &segment_file_size,
			     &error ) == -1 )
			{
				notify_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				segment_file_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

				fprintf( stdout, "Unable to determine segment file size defaulting to: %" PRIu64 ".\n",
				 segment_file_size );
			}
			/* Make sure the segment file size is smaller than or equal to the maximum
			 */
			if( segment_file_size > maximum_segment_file_size )
			{
				segment_file_size = maximum_segment_file_size;
			}
		}
		/* Chunk size (sectors per block)
		 */
		if( argument_set_sectors_per_chunk == 0 )
		{
			if( ewfinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "The amount of sectors to read at once" ),
			     ewfinput_sector_per_block_sizes,
			     EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT,
			     EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT,
			     &fixed_string_variable ) == -1 )
			{
				fprintf( stdout, "Unable to determine sectors per chunk defaulting to: 64.\n" );

				sectors_per_chunk = 64;
			}
			else if( ewfinput_determine_sectors_per_chunk(
				  fixed_string_variable,
				  &sectors_per_chunk ) != 1 )
			{
				fprintf( stdout, "Unsupported sectors per chunk defaulting to: 64.\n" );

				sectors_per_chunk = 64;
			}
		}
		/* Error granularity
		 */
		if( argument_set_sector_error_granularity == 0 )
		{
			if( ewfinput_get_size_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "The amount of sectors to be used as error granularity" ),
			     1,
			     (uint64_t) sectors_per_chunk,
			     64,
			     &input_size_variable ) == -1 )
			{
				input_size_variable = 64;

				fprintf( stdout, "Unable to determine sector error granularity defaulting to: %" PRIu64 ".\n",
				 input_size_variable );
			}
			sector_error_granularity = (uint32_t) input_size_variable;
		}
		/* The amount of read error retry
		 */
		if( argument_set_read_error_retry == 0 )
		{
			if( ewfinput_get_size_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "The amount of retries when a read error occurs" ),
			     0,
			     255,
			     2,
			     &input_size_variable ) == -1 )
			{
				input_size_variable = 2;

				fprintf( stdout, "Unable to determine read error retry defaulting to: %" PRIu64 ".\n",
				 input_size_variable );
			}
			read_error_retry = (uint8_t) input_size_variable;
		}
		/* Wipe the sector on error
		 */
		if( argument_set_wipe_chunk_on_error == 0 )
		{
			if( ewfinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "Wipe sectors on read error (mimic EnCase like behavior)" ),
			     ewfinput_yes_no,
			     2,
			     1,
			     &fixed_string_variable ) == -1 )
			{
				fprintf( stdout, "Unable to determine wipe chunk on error defaulting to: no.\n" );

				wipe_chunk_on_error = 0;
			}
			else if( ewfinput_determine_yes_no(
				  fixed_string_variable,
				  &wipe_chunk_on_error ) != 1 )
			{
				fprintf( stdout, "Unsupported wipe chunk on error defaulting to: no.\n" );

				wipe_chunk_on_error = 0;
			}
		}
		fprintf( stdout, "\n" );

		/* Check if user is content with the acquiry parameters
		 */
		acquiry_parameters_confirmed = confirm_acquiry_parameters(
		                                stdout,
		                                input_buffer,
		                                EWFACQUIRE_INPUT_BUFFER_SIZE,
		                                target_filename,
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
		                                sectors_per_chunk,
		                                sector_error_granularity,
		                                read_error_retry,
		                                wipe_chunk_on_error );

		if( acquiry_parameters_confirmed == -1 )
		{
			fprintf( stdout, "Unable to determine if acquiry parameters are correct aborting.\n" );

			ewfcommon_abort = 1;

			break;
		}
		/* Reset all parameters provided as command line arguments
		 */
		else if( acquiry_parameters_confirmed == 0 )
		{
			argument_set_compression              = 0;
			argument_set_format                   = 0;
			argument_set_media_type               = 0;
			argument_set_offset                   = 0;
			argument_set_read_error_retry         = 0;
			argument_set_sector_error_granularity = 0;
			argument_set_sectors_per_chunk        = 0;
			argument_set_segment_file_size        = 0;
			argument_set_size                     = 0;
			argument_set_volume_type              = 0;
			argument_set_wipe_chunk_on_error      = 0;
		}
	}
	if( ewfsignal_attach(
	     ewfcommon_signal_handler ) != 1 )
	{
		fprintf( stderr, "Unable to attach signal handler.\n" );
	}
	if( ewfcommon_abort == 0 )
	{
		if( ewfcommon_determine_operating_system_string(
		     acquiry_operating_system,
		     32,
		     &error ) != 1 )
		{
			fprintf(
			 stdout,
			 "Unable to determine operating system string.\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			acquiry_operating_system[ 0 ] = 0;
		}
		acquiry_software_version = _SYSTEM_CHARACTER_T_STRING( LIBEWF_VERSION_STRING );

		/* Set up the libewf handle
		 */
		target_filenames[ 0 ] = target_filename;

#if defined( HAVE_V2_API )
		if( libewf_handle_initialize(
		     &ewfcommon_libewf_handle,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to create libewf handle.\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			error_abort = 1;
		}
		else if( libewf_open(
		          ewfcommon_libewf_handle,
		          (system_character_t * const *) target_filenames,
		          1,
		          LIBEWF_OPEN_WRITE,
		          &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to open EWF file(s).\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			error_abort = 1;
		}
#else
		ewfcommon_libewf_handle = libewf_open(
			                   (system_character_t * const *) target_filenames,
			                   1,
			                   LIBEWF_OPEN_WRITE );

		if( ewfcommon_libewf_handle == NULL )
		{
			ewfoutput_error_fprint(
			 stderr, "Unable to create EWF file(s)" );

			error_abort = 1;
		}
#endif
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
			  media_type,
			  volume_type,
			  compression_level,
			  compress_empty_block,
			  libewf_format,
			  (size64_t) segment_file_size,
			  sector_error_granularity ) != 1 )
		{
			fprintf( stderr, "Unable to initialize settings for EWF file(s).\n" );

			error_abort = 1;
		}
	}
	memory_free(
	 notes );
	memory_free(
	 examiner_name );
	memory_free(
	 evidence_number );
	memory_free(
	 description );
	memory_free(
	 case_number );
	memory_free(
	 target_filename );

	if( error_abort != 0 )
	{
#if defined( HAVE_V2_API )
		libewf_close(
		 ewfcommon_libewf_handle,
		 NULL );
		libewf_handle_free(
		 &ewfcommon_libewf_handle,
		 NULL );
#else
		libewf_close(
		 ewfcommon_libewf_handle );
#endif

		file_io_close(
		 file_descriptor );

		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (system_character_t *) memory_allocate(
		                                                     sizeof( system_character_t ) * DIGEST_HASH_STRING_SIZE_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated MD5 hash string.\n" );

#if defined( HAVE_V2_API )
			libewf_close(
			 ewfcommon_libewf_handle,
			 NULL );
			libewf_handle_free(
			 &ewfcommon_libewf_handle,
			 NULL );
#else
			libewf_close(
			 ewfcommon_libewf_handle );
#endif

			file_io_close(
			 file_descriptor );

			return( EXIT_FAILURE );
		}
	}
	if( calculate_sha1 == 1 )
	{
		calculated_sha1_hash_string = (system_character_t *) memory_allocate(
		                                                      sizeof( system_character_t ) * DIGEST_HASH_STRING_SIZE_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated SHA1 hash string.\n" );

			memory_free(
			 calculated_md5_hash_string );

#if defined( HAVE_V2_API )
			libewf_close(
			 ewfcommon_libewf_handle,
			 NULL );
			libewf_handle_free(
			 &ewfcommon_libewf_handle,
			 NULL );
#else
			libewf_close(
			 ewfcommon_libewf_handle );
#endif

			file_io_close(
			 file_descriptor );

			return( EXIT_FAILURE );
		}
	}
	if( ewfcommon_abort == 0 )
	{
		if( process_status_initialize(
		     &process_status,
		     _SYSTEM_CHARACTER_T_STRING( "Acquiry" ),
		     _SYSTEM_CHARACTER_T_STRING( "acquired" ),
		     _SYSTEM_CHARACTER_T_STRING( "Written" ),
		     stdout ) != 1 )
		{
			fprintf( stderr, "Unable to initialize process status.\n" );

			if( calculate_sha1 == 1 )
			{
				memory_free(
				 calculated_sha1_hash_string );
			}
			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
#if defined( HAVE_V2_API )
			libewf_close(
			 ewfcommon_libewf_handle,
			 NULL );
			libewf_handle_free(
			 &ewfcommon_libewf_handle,
			 NULL );
#else
			libewf_close(
			 ewfcommon_libewf_handle );
#endif

			file_io_close(
			 file_descriptor );

			return( EXIT_FAILURE );
		}
		if( process_status_start(
		     process_status ) != 1 )
		{
			fprintf( stderr, "Unable to start process status.\n" );

			process_status_free(
			 &process_status );

			if( calculate_sha1 == 1 )
			{
				memory_free(
				 calculated_sha1_hash_string );
			}
			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
#if defined( HAVE_V2_API )
			libewf_close(
			 ewfcommon_libewf_handle,
			 NULL );
			libewf_handle_free(
			 &ewfcommon_libewf_handle,
			 NULL );
#else
			libewf_close(
			 ewfcommon_libewf_handle );
#endif

			file_io_close(
			 file_descriptor );

			return( EXIT_FAILURE );
		}
		/* Start acquiring data
		 */
		write_count = ewfcommon_write_from_file_descriptor(
			       ewfcommon_libewf_handle,
			       file_descriptor,
			       acquiry_size,
			       acquiry_offset,
			       sectors_per_chunk,
			       512,
			       read_error_retry,
			       sector_error_granularity,
			       calculate_md5,
			       calculated_md5_hash_string,
			       DIGEST_HASH_STRING_SIZE_MD5,
			       calculate_sha1,
			       calculated_sha1_hash_string,
			       DIGEST_HASH_STRING_SIZE_MD5,
			       swap_byte_pairs,
			       wipe_chunk_on_error,
			       seek_on_error,
			       (size_t) process_buffer_size,
			       callback );

		if( write_count <= -1 )
		{
			status = PROCESS_STATUS_FAILED;
		}
		else
		{
			status = PROCESS_STATUS_COMPLETED;
		}
	}
	/* Done acquiring data
	 */
	if( file_io_close(
	     file_descriptor ) != 0 )
	{
		fprintf( stderr, "Unable to close input.\n" );

		process_status_free(
		 &process_status );

		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
#if defined( HAVE_V2_API )
		libewf_close(
		 ewfcommon_libewf_handle,
		 NULL );
		libewf_handle_free(
		 &ewfcommon_libewf_handle,
		 NULL );
#else
		libewf_close(
		 ewfcommon_libewf_handle );
#endif

		return( EXIT_FAILURE );
	}
	if( ewfcommon_abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     process_status,
	     (size64_t) write_count,
	     status ) != 1 )
	{
		fprintf( stderr, "Unable to stop process status.\n" );

		process_status_free(
		 &process_status );

		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
#if defined( HAVE_V2_API )
		libewf_close(
		 ewfcommon_libewf_handle,
		 NULL );
		libewf_handle_free(
		 &ewfcommon_libewf_handle,
		 NULL );
#else
		libewf_close(
		 ewfcommon_libewf_handle );
#endif

		return( EXIT_FAILURE );
	}
	if( process_status_free(
	     &process_status ) != 1 )
	{
		fprintf( stderr, "Unable to free process status.\n" );

		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
#if defined( HAVE_V2_API )
		libewf_close(
		 ewfcommon_libewf_handle,
		 NULL );
		libewf_handle_free(
		 &ewfcommon_libewf_handle,
		 NULL );
#else
		libewf_close(
		 ewfcommon_libewf_handle );
#endif

		return( EXIT_FAILURE );
	}
	if( status == PROCESS_STATUS_COMPLETED )
	{
		if( log_filename != NULL )
		{
			log_file_stream = ewfcommon_fopen(
					   log_filename,
					   _SYSTEM_CHARACTER_T_STRING( "a" ) );

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
#if defined( HAVE_V2_API )
	if( libewf_close(
	     ewfcommon_libewf_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close EWF file(s).\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		if( log_file_stream != NULL )
		{
			file_stream_io_fclose(
			 log_file_stream );
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		libewf_handle_free(
		 &ewfcommon_libewf_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libewf_handle_free(
	     &ewfcommon_libewf_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to free libewf handle.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		if( log_file_stream != NULL )
		{
			file_stream_io_fclose(
			 log_file_stream );
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		return( EXIT_FAILURE );
	}
#else
	if( libewf_close(
	     ewfcommon_libewf_handle ) != 0 )
	{
		fprintf( stderr, "Unable to close EWF file(s).\n" );

		if( log_file_stream != NULL )
		{
			file_stream_io_fclose(
			 log_file_stream );
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		return( EXIT_FAILURE );
	}
#endif
	if( ewfsignal_detach() != 1 )
	{
		fprintf( stderr, "Unable to detach signal handler.\n" );
	}
        if( status != PROCESS_STATUS_COMPLETED )
        {
		if( log_file_stream != NULL )
		{
			file_stream_io_fclose(
			 log_file_stream );
		}
		if( calculate_sha1 == 1 )
		{
			memory_free(
			 calculated_sha1_hash_string );
		}
		if( calculate_md5 == 1 )
		{
			memory_free(
			 calculated_md5_hash_string );
		}
		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		fprintf( stdout, "MD5 hash calculated over data:\t%" PRIs_SYSTEM "\n",
		 calculated_md5_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf( log_file_stream, "MD5 hash calculated over data:\t%" PRIs_SYSTEM "\n",
			 calculated_md5_hash_string );
		}
		memory_free(
		 calculated_md5_hash_string );
	}
	if( calculate_sha1 == 1 )
	{
		fprintf( stdout, "SHA1 hash calculated over data:\t%" PRIs_SYSTEM "\n",
		 calculated_sha1_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf( log_file_stream, "SHA1 hash calculated over data:\t%" PRIs_SYSTEM "\n",
			 calculated_sha1_hash_string );
		}
		memory_free(
		 calculated_sha1_hash_string );
	}
	if( log_file_stream != NULL )
	{
		if( file_stream_io_fclose(
		     log_file_stream ) != 0 )
		{
			fprintf( stderr, "Unable to close log file: %s.\n",
			 log_filename );
		}
	}
	return( EXIT_SUCCESS );
}

