/*
 * ewfacquire
 * Reads data from a file and writes it in EWF format
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

#ifdef HAVE_CYGWIN_FS_H
#include <cygwin/fs.h>
#endif

#ifdef HAVE_LINUX_FS_H
#include <linux/fs.h>
#endif

#ifdef HAVE_SYS_DISK_H
#include <sys/disk.h>
#endif

#ifdef HAVE_SYS_DISKLABEL_H
#include <sys/disklabel.h>
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
	fprintf( stderr, "Usage: ewfacquire [ -d digest_type ] [ -hqsvV ] source\n\n" );

	fprintf( stderr, "\tsource: the source file or device\n\n" );

	fprintf( stderr, "\t-d:     calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stderr, "\t-h:     shows this help\n" );
	fprintf( stderr, "\t-q:     quiet shows no status information\n" );
	fprintf( stderr, "\t-s:     swap byte pairs of the media data (from AB to BA)\n" );
	fprintf( stderr, "\t        (use this for big to little endian conversion and vice versa)\n" );
	fprintf( stderr, "\t-v:     verbose output to stderr\n" );
	fprintf( stderr, "\t-V:     print version\n" );
}

/* Prints an overview of the user provided input
 * and asks the user for confirmation
 * Return 1 if confirmed by user, 0 otherwise
 */
int confirm_input( CHAR_T *filename, LIBEWF_CHAR *case_number, LIBEWF_CHAR *description, LIBEWF_CHAR *evidence_number, LIBEWF_CHAR *examiner_name, LIBEWF_CHAR *notes, uint8_t media_type, uint8_t volume_type, int8_t compression_level, uint8_t compress_empty_block, uint8_t libewf_format, uint64_t acquiry_offset, uint64_t acquiry_size, uint32_t segment_file_size, uint64_t sectors_per_chunk, uint32_t sector_error_granularity, uint8_t read_error_retry, uint8_t wipe_block_on_read_error )
{
	LIBEWF_CHAR *user_input  = NULL;
	LIBEWF_CHAR *yes_no[ 2 ] = { _S_LIBEWF_CHAR( "yes" ),
				     _S_LIBEWF_CHAR( "no" ) };
	int input_confirmed      = -1;

	fprintf( stdout, "The following acquiry parameters were provided:\n" );

	ewfcommon_acquiry_paramters_fprint( stdout, filename, case_number, description, evidence_number, examiner_name, notes, media_type, volume_type, compression_level, compress_empty_block, libewf_format, acquiry_offset, acquiry_size, segment_file_size, sectors_per_chunk, sector_error_granularity, read_error_retry, wipe_block_on_read_error );

	/* Ask for confirmation
	 */
	while( input_confirmed == -1 )
	{
		user_input = ewfcommon_get_user_input_fixed_value( stdout, _S_LIBEWF_CHAR( "Continue acquiry with these values" ), yes_no, 2, 0 );
	
		if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "yes" ), 3 ) == 0 )
		{
			input_confirmed = 1;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "no" ), 2 ) == 0 )
		{
			input_confirmed = 0;
		}
		else
		{
			fprintf( stdout, "Selected option not supported, please try again or terminate using Ctrl^C.\n" );
		}
		libewf_common_free( user_input );
	}
	fprintf( stdout, "\n" );

	return( input_confirmed );
}

/* Determine the device size using a file descriptor
 */
uint64_t determine_device_size( int file_descriptor )
{
#ifndef DIOCGMEDIASIZE
#ifdef DIOCGDINFO
	struct disklabel disk_label;
#endif
#endif
	uint64_t input_size  = 0;
#ifdef DKIOCGETBLOCKCOUNT
	uint64_t block_count = 0;
	uint32_t block_size  = 0;
#endif
	if( file_descriptor == -1 )
	{
		return( 0 );
	}
#ifdef BLKGETSIZE64
	if( ioctl( file_descriptor, BLKGETSIZE64, &input_size ) == -1 )
	{
		return( 0 );
	}
#else
#ifdef DIOCGMEDIASIZE
	if( ioctl( file_descriptor, DIOCGMEDIASIZE, &input_size ) == -1 )
	{
		return( 0 );
	}
#else
#ifdef DIOCGDINFO
	if( ioctl( file_descriptor, DIOCGDINFO, &disk_label ) == -1 )
	{
		return( 0 );
	}
	input_size = disk_label.d_secperunit * disk_label.d_secsize;
#else
#ifdef DKIOCGETBLOCKCOUNT
	if( ioctl( file_descriptor, DKIOCGETBLOCKSIZE, &block_size ) == -1 )
	{
		return( 0 );
	}
	if( ioctl( file_descriptor, DKIOCGETBLOCKCOUNT, &block_count ) == -1 )
	{
		return( 0 );
	}
#ifdef HAVE_DEBUG_OUTPUT
	fprintf( stderr, "block size: %" PRIu32 " block count: %" PRIu64 " ", block_size, block_count );
#endif
	input_size = block_count * block_size;
#else
	input_size = 0;

#endif /* DKIOCGETBLOCKCOUNT */
#endif /* DIOCGDINFO */
#endif /* DIOCGMEDIASIZE */
#endif /* BLKGETSIZE64 */

#ifdef HAVE_DEBUG_OUTPUT
	fprintf( stderr, "device size: %" PRIu64 "\n", input_size );
#endif
	return( input_size );
}

/* The main program
 */
#ifdef HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	struct stat input_file_stat;

#if defined(HAVE_UUID_UUID_H) && defined(HAVE_LIBUUID)
	uint8_t guid[ 16 ];
#endif
	CHAR_T *filenames[ 1 ]                    = { NULL };

	LIBEWF_HANDLE *handle                     = NULL;
	LIBEWF_CHAR *calculated_md5_hash_string   = NULL;
	LIBEWF_CHAR *calculated_sha1_hash_string  = NULL;
	LIBEWF_CHAR *user_input                   = NULL;
	LIBEWF_CHAR *case_number                  = NULL;
	LIBEWF_CHAR *description                  = NULL;
	LIBEWF_CHAR *evidence_number              = NULL;
	LIBEWF_CHAR *examiner_name                = NULL;
	LIBEWF_CHAR *notes                        = NULL;
	LIBEWF_CHAR *acquiry_operating_system     = NULL;
	LIBEWF_CHAR *acquiry_software_version     = NULL;
	CHAR_T *filename                          = NULL;
	CHAR_T *time_string                       = NULL;
#if defined(HAVE_STRERROR_R) || defined(HAVE_STRERROR)
	CHAR_T *error_string                      = NULL;
#endif
	void *callback                            = &ewfcommon_process_status_fprint;

	INT_T option                              = 0;
	size_t string_length                      = 0;
	time_t timestamp_start                    = 0;
	time_t timestamp_end                      = 0;
	int64_t count                             = 0;
	int64_t segment_file_size                 = 0;
	uint64_t input_size                       = 0;
	uint64_t acquiry_offset                   = 0;
	uint64_t acquiry_size                     = 0;
	uint64_t sectors_per_chunk                = 0;
	uint32_t sector_error_granularity         = 0;
	int8_t compression_level                  = LIBEWF_COMPRESSION_NONE;
	int8_t result_md5_hash                    = 0;
	int8_t result_sha1_hash                   = 0;
	uint8_t media_type                        = LIBEWF_MEDIA_TYPE_FIXED;
	uint8_t volume_type                       = LIBEWF_VOLUME_TYPE_LOGICAL;
	uint8_t compress_empty_block              = 0;
	uint8_t wipe_block_on_read_error          = 0;
	uint8_t libewf_format                     = LIBEWF_FORMAT_UNKNOWN;
	uint8_t read_error_retry                  = 2;
	uint8_t swap_byte_pairs                   = 0;
	uint8_t seek_on_error                     = 1;
	uint8_t calculate_sha1                    = 0;
	uint8_t verbose                           = 0;
	int file_descriptor                       = 0;

	LIBEWF_CHAR *compression_types[ 3 ]       = { _S_LIBEWF_CHAR( "none" ),
						      _S_LIBEWF_CHAR( "fast" ),
						      _S_LIBEWF_CHAR( "best" ) };
	LIBEWF_CHAR *format_types[ 12 ]           = { _S_LIBEWF_CHAR( "ewf" ),
						      _S_LIBEWF_CHAR( "smart" ),
						      _S_LIBEWF_CHAR( "ftk" ),
						      _S_LIBEWF_CHAR( "encase1" ),
						      _S_LIBEWF_CHAR( "encase2" ),
						      _S_LIBEWF_CHAR( "encase3" ),
						      _S_LIBEWF_CHAR( "encase4" ),
						      _S_LIBEWF_CHAR( "encase5" ),
						      _S_LIBEWF_CHAR( "encase6" ),
						      _S_LIBEWF_CHAR( "linen5" ),
						      _S_LIBEWF_CHAR( "linen6" ),
						      _S_LIBEWF_CHAR( "ewfx" ) };
	LIBEWF_CHAR *media_types[ 2 ]             = { _S_LIBEWF_CHAR( "fixed" ),
						      _S_LIBEWF_CHAR( "removable" ) };
	LIBEWF_CHAR *volume_types[ 2 ]            = { _S_LIBEWF_CHAR( "logical" ),
						      _S_LIBEWF_CHAR( "physical" ) };
	LIBEWF_CHAR *sector_per_block_sizes[ 10 ] = { _S_LIBEWF_CHAR( "64" ),
						      _S_LIBEWF_CHAR( "128" ),
						      _S_LIBEWF_CHAR( "256" ),
						      _S_LIBEWF_CHAR( "512" ),
						      _S_LIBEWF_CHAR( "1024" ),
						      _S_LIBEWF_CHAR( "2048" ),
						      _S_LIBEWF_CHAR( "4096" ),
						      _S_LIBEWF_CHAR( "8192" ),
						      _S_LIBEWF_CHAR( "16384" ),
						      _S_LIBEWF_CHAR( "32768" ) };
	LIBEWF_CHAR *yes_no[ 2 ]                  = { _S_LIBEWF_CHAR( "yes" ),
						      _S_LIBEWF_CHAR( "no" ) };

	ewfsignal_initialize();

	ewfcommon_version_fprint( stderr, _S_LIBEWF_CHAR( "ewfacquire" ) );

	while( ( option = ewfgetopt( argc, argv, _S_CHAR_T( "d:hqsvV" ) ) ) != (INT_T) -1 )
	{
		switch( option )
		{
			case (INT_T) '?':
			default:
				fprintf( stderr, "Invalid argument: %" PRIs "\n", argv[ optind ] );

				usage();

				return( EXIT_FAILURE );

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

			case (INT_T) 'h':
				usage();

				return( EXIT_SUCCESS );

			case (INT_T) 'q':
				callback = NULL;

				break;

			case (INT_T) 's':
				swap_byte_pairs = 1;

				break;

			case (INT_T) 'v':
				verbose = 1;

				break;

			case (INT_T) 'V':
				ewfcommon_copyright_fprint( stderr );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf( stderr, "Missing source file or device.\n" );

		usage();

		return( EXIT_FAILURE );
	}
	libewf_set_notify_values( stderr, verbose );

	/* Check if to read from stdin
	 */
	if( CHAR_T_COMPARE( argv[ optind ], _S_CHAR_T( "-" ), 1 ) == 0 )
	{
		fprintf( stderr, "Reading from stdin not supported.\n" );

		return( EXIT_FAILURE );
	}
	/* Open the input file or device size
	 */
	file_descriptor = libewf_common_open( argv[ optind ], LIBEWF_OPEN_READ );

	if( file_descriptor == -1 )
	{
#if defined(HAVE_STRERROR_R) || defined(HAVE_STRERROR)
		error_string = libewf_common_strerror( errno );

		if( error_string != NULL )
		{
			fprintf( stderr, "Error opening file or device: %" PRIs " with failure: %" PRIs ".\n", argv[ optind ], error_string );

			libewf_common_free( error_string );
		}
#else
		fprintf( stderr, "Error opening file or device: %" PRIs ".\n", argv[ optind ] );
#endif
		return( EXIT_FAILURE );
	}
	/* Check the input file or device size
	 */
	input_size = 0;

	if( fstat( file_descriptor, &input_file_stat ) != 0 )
	{
		fprintf( stderr, "Unable to get status information of file.\n" );

		return( EXIT_FAILURE );
	}
#ifndef HAVE_WINDOWS_API
	if( S_ISBLK( input_file_stat.st_mode ) || S_ISCHR( input_file_stat.st_mode ) )
	{
		input_size = determine_device_size( file_descriptor );
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
	acquiry_operating_system = ewfcommon_determine_operating_system();
	acquiry_software_version = LIBEWF_VERSION;

	do
	{
		libewf_common_free( case_number );
		libewf_common_free( description );
		libewf_common_free( evidence_number );
		libewf_common_free( examiner_name );
		libewf_common_free( notes );

		/* Request the necessary case data
		 */
		fprintf( stdout, "Acquiry parameters required, please provide the necessary input\n" );

		/* Output filename
		 */
		while( filename == NULL )
		{
			filename = ewfcommon_get_user_input_variable_char_t( stdout, _S_LIBEWF_CHAR( "Image path and filename without extension" ) );

			if( filename == NULL )
			{
				fprintf( stdout, "Filename is required, please try again or terminate using Ctrl^C.\n" );
			}
		}
		/* Case number
		 */
		case_number = ewfcommon_get_user_input_variable( stdout, _S_LIBEWF_CHAR( "Case number" ) );

		/* Description
		 */
		description = ewfcommon_get_user_input_variable( stdout, _S_LIBEWF_CHAR( "Description" ) );

		/* Evidence number
		 */
		evidence_number = ewfcommon_get_user_input_variable( stdout, _S_LIBEWF_CHAR( "Evidence number" ) );

		/* Examiner name
		 */
		examiner_name = ewfcommon_get_user_input_variable( stdout, _S_LIBEWF_CHAR( "Examiner name" ) );

		/* Notes
		 */
		notes = ewfcommon_get_user_input_variable( stdout, _S_LIBEWF_CHAR( "Notes" ) );

		/* Media type
		 */
		user_input = ewfcommon_get_user_input_fixed_value( stdout, _S_LIBEWF_CHAR( "Media type" ), media_types, 2, 0 );

		if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "fixed" ), 5 ) == 0 )
		{
			media_type = LIBEWF_MEDIA_TYPE_FIXED;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "removable" ), 9 ) == 0 )
		{
			media_type = LIBEWF_MEDIA_TYPE_REMOVABLE;
		}
		else
		{
			fprintf( stderr, "ewfacquire: unsuported media type.\n" );

			return( EXIT_FAILURE );
		}
		libewf_common_free( user_input );

		/* Volume type
		 */
		user_input = ewfcommon_get_user_input_fixed_value( stdout, _S_LIBEWF_CHAR( "Volume type" ), volume_types, 2, 1 );

		if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "logical" ), 7 ) == 0 )
		{
			volume_type = LIBEWF_VOLUME_TYPE_LOGICAL;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "physical" ), 8 ) == 0 )
		{
			volume_type = LIBEWF_VOLUME_TYPE_PHYSICAL;
		}
		else
		{
			fprintf( stderr, "ewfacquire: unsuported volume type.\n" );

			return( EXIT_FAILURE );
		}
		libewf_common_free( user_input );

		/* Compression
		 */
		user_input = ewfcommon_get_user_input_fixed_value( stdout, _S_LIBEWF_CHAR( "Use compression" ), compression_types, 3, 0 );

		if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "none" ), 4 ) == 0 )
		{
			compression_level = LIBEWF_COMPRESSION_NONE;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "fast" ), 4 ) == 0 )
		{
			compression_level = LIBEWF_COMPRESSION_FAST;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "best" ), 4 ) == 0 )
		{
			compression_level = LIBEWF_COMPRESSION_BEST;
		}
		else
		{
			fprintf( stderr, "ewfacquire: unsuported compression type.\n" );

			return( EXIT_FAILURE );
		}
		libewf_common_free( user_input );

		/* Empty block compression
		 */
		if( compression_level == LIBEWF_COMPRESSION_NONE )
		{
			user_input = ewfcommon_get_user_input_fixed_value( stdout, _S_LIBEWF_CHAR( "Compress empty blocks" ), yes_no, 2, 1 );

			if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "yes" ), 3 ) == 0 )
			{
				compress_empty_block = 1;
			}
			else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "no" ), 2 ) == 0 )
			{
				compress_empty_block = 0;
			}
			else
			{
				fprintf( stderr, "ewfacquire: unsuported answer.\n" );

				return( EXIT_FAILURE );
			}
			libewf_common_free( user_input );
		}

		/* File format
		 */
		user_input = ewfcommon_get_user_input_fixed_value( stdout, _S_LIBEWF_CHAR( "Use EWF file format" ), format_types, 12, 7 );

		if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "smart" ), 5 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_SMART;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "ftk" ), 3 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_FTK;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "encase1" ), 7 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_ENCASE1;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "encase2" ), 7 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_ENCASE2;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "encase3" ), 7 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_ENCASE3;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "encase4" ), 7 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_ENCASE4;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "encase5" ), 7 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_ENCASE5;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "encase6" ), 7 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_ENCASE6;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "linen5" ), 6 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_LINEN5;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "linen6" ), 6 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_LINEN6;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "ewfx" ), 4 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_EWFX;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "ewf" ), 3 ) == 0 )
		{
			libewf_format = LIBEWF_FORMAT_EWF;
		}
		else
		{
			fprintf( stderr, "ewfacquire: unsuported EWF file format type.\n" );

			libewf_common_free( user_input );

			exit( EXIT_FAILURE );
		}
		libewf_common_free( user_input );

		/* Size and offset of data to acquire
		 */
		acquiry_offset = ewfcommon_get_user_input_size_variable( stdout, _S_LIBEWF_CHAR( "Start to acquire at offset" ), 0, input_size, 0 );
		acquiry_size   = ewfcommon_get_user_input_size_variable( stdout, _S_LIBEWF_CHAR( "Amount of bytes to acquire" ), 0, input_size, input_size );

		/* File size
		 */
		segment_file_size  = ewfcommon_get_user_input_size_variable( stdout, _S_LIBEWF_CHAR( "Evidence segment file size in kbytes (2^10)" ), 1440, ( 2 * 1024 * 1024 ), ( 650 * 1024 ) );
		segment_file_size *= 1024;

		/* Make sure the segment file size is 1 byte smaller than 2 Gb (2 * 1024 * 1024 * 1024)
		 */
		if( segment_file_size >= (int64_t) INT32_MAX )
		{
			segment_file_size = (int64_t) INT32_MAX - 1;
		}
		/* Chunk size (sectors per block)
		 */
		user_input = ewfcommon_get_user_input_fixed_value( stdout, _S_LIBEWF_CHAR( "The amount of sectors to read at once" ), sector_per_block_sizes, 10, 0 );

		sectors_per_chunk = libewf_string_to_int64( user_input, libewf_string_length( user_input ) );

		libewf_common_free( user_input );

		/* Error granularity
		 */
		sector_error_granularity = (uint32_t) ewfcommon_get_user_input_size_variable( stdout, _S_LIBEWF_CHAR( "The amount of sectors to be used as error granularity" ), 1, sectors_per_chunk, 64 );

		/* The amount of read error retry
		 */
		read_error_retry = (uint8_t) ewfcommon_get_user_input_size_variable( stdout, _S_LIBEWF_CHAR( "The amount of retries when a read error occurs" ), 0, 255, 2 );

		/* Wipe the sector on error
		 */
		user_input = ewfcommon_get_user_input_fixed_value( stdout, _S_LIBEWF_CHAR( "Wipe sectors on read error (mimic EnCase like behavior)" ), yes_no, 2, 0 );

		if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "yes" ), 3 ) == 0 )
		{
			wipe_block_on_read_error = 1;
		}
		else if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "no" ), 2 ) == 0 )
		{
			wipe_block_on_read_error = 0;
		}
		else
		{
			fprintf( stderr, "ewfacquire: unsuported answer.\n" );

			return( EXIT_FAILURE );
		}
		libewf_common_free( user_input );

		fprintf( stdout, "\n" );
	}
	/* Check if user is content with values
	 */
	while( confirm_input( filename, case_number, description, evidence_number, examiner_name, notes, media_type, volume_type, compression_level, compress_empty_block, libewf_format, acquiry_offset, acquiry_size, (uint32_t) segment_file_size, sectors_per_chunk, sector_error_granularity, read_error_retry, wipe_block_on_read_error ) == 0 );

	/* Done asking user input set up the libewf handle
	 */
	filenames[ 0 ] = filename;

	handle = libewf_open( (CHAR_T * const *) filenames, 1, LIBEWF_OPEN_WRITE );

	if( handle == NULL )
	{
		fprintf( stderr, "Unable to create EWF file handle.\n" );

		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_media_values( handle, (uint32_t) sectors_per_chunk, 512 ) != 1 )
	{
		fprintf( stderr, "Unable to set media values in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_write_segment_file_size( handle, (uint32_t) segment_file_size ) != 1 )
	{
		fprintf( stderr, "Unable to set write segment file size in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_write_error_granularity( handle, sector_error_granularity ) != 1 )
	{
		fprintf( stderr, "Unable to set write error granularity in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_write_media_type( handle, media_type, volume_type ) != 1 )
	{
		fprintf( stderr, "Unable to set write media type in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_write_compression_values( handle, compression_level, compress_empty_block ) != 1 )
	{
		fprintf( stderr, "Unable to set write compression values in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_write_format( handle, libewf_format ) != 1 )
	{
		fprintf( stderr, "Unable to set write format in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_swap_byte_pairs( handle, swap_byte_pairs ) != 1 )
	{
		fprintf( stderr, "Unable to set swap byte pairs in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

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
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

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
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

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
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

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
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

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
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	libewf_common_free( notes );

	/* Password is not used within libewf
	 */
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
			if( libewf_common_close( file_descriptor ) != 0 )
			{
				fprintf( stderr, "Unable to close input.\n" );
			}
			libewf_common_free( filename );

			return( EXIT_FAILURE );
		}
		libewf_common_free( acquiry_operating_system );
	}
	if( libewf_set_header_value( handle, _S_LIBEWF_CHAR( "acquiry_software" ), _S_LIBEWF_CHAR( "ewfacquire" ), 10 ) != 1 )
	{
		fprintf( stderr, "Unable to set header value acquiry software in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_header_value_acquiry_software_version( handle, acquiry_software_version, libewf_string_length( acquiry_software_version ) ) != 1 )
	{
		fprintf( stderr, "Unable to set header value acquiry software version number in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

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
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_guid( handle, guid, 16 ) != 1 )
	{
		fprintf( stderr, "Unable to set GUID in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

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

	count = ewfcommon_write_from_file_descriptor( handle, file_descriptor, acquiry_size, acquiry_offset, read_error_retry, sector_error_granularity, wipe_block_on_read_error, seek_on_error, calculate_sha1, callback );

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
			if( libewf_common_close( file_descriptor ) != 0 )
			{
				fprintf( stderr, "Unable to close input.\n" );
			}
			libewf_common_free( filename );

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
				if( libewf_common_close( file_descriptor ) != 0 )
				{
					fprintf( stderr, "Unable to close input.\n" );
				}
				libewf_common_free( filename );

				return( EXIT_FAILURE );
			}
			result_sha1_hash = libewf_get_hash_value( handle, _S_LIBEWF_CHAR( "SHA1" ), calculated_sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );
		}
	}
	if( libewf_common_close( file_descriptor ) != 0 )
	{
		fprintf( stderr, "Unable to close input.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file handle.\n" );
		}
		libewf_common_free( filename );
		libewf_common_free( calculated_md5_hash_string );

		if( calculate_sha1 == 1 )
		{
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	libewf_common_free( filename );

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
		libewf_common_free( calculated_md5_hash_string );

		if( calculate_sha1 == 1 )
		{
			libewf_common_free( calculated_sha1_hash_string );
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
		fprintf( stderr, "MD5 hash calculated over data:\tN/A\n" );
	}
	else
	{
		fprintf( stderr, "MD5 hash calculated over data:\t%" PRIs_EWF "\n", calculated_md5_hash_string );
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

