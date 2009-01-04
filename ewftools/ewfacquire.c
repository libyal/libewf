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

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
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
	fprintf( stdout, "Usage: ewfacquire [ -d digest_type ] [ -hqsvV ] source\n\n" );

	fprintf( stdout, "\tsource: the source file or device\n\n" );

	fprintf( stdout, "\t-d:     calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stdout, "\t-h:     shows this help\n" );
	fprintf( stdout, "\t-q:     quiet shows no status information\n" );
	fprintf( stdout, "\t-s:     swap byte pairs of the media data (from AB to BA)\n" );
	fprintf( stdout, "\t        (use this for big to little endian conversion and vice versa)\n" );
	fprintf( stdout, "\t-v:     verbose output to stderr\n" );
	fprintf( stdout, "\t-V:     print version\n" );
}

/* Prints an overview of the user provided input
 * and asks the user for confirmation
 * Return 1 if confirmed by user, 0 otherwise
 */
int confirm_input( CHAR_T *filename, LIBEWF_CHAR *case_number, LIBEWF_CHAR *description, LIBEWF_CHAR *evidence_number, LIBEWF_CHAR *examiner_name, LIBEWF_CHAR *notes, uint8_t media_type, uint8_t volume_type, int8_t compression_level, uint8_t compress_empty_block, uint8_t libewf_format, off64_t acquiry_offset, size64_t acquiry_size, size64_t segment_file_size, uint32_t sectors_per_chunk, uint32_t sector_error_granularity, uint8_t read_error_retry, uint8_t wipe_block_on_read_error )
{
	LIBEWF_CHAR *user_input = NULL;
	int input_confirmed     = -1;

	fprintf( stdout, "The following acquiry parameters were provided:\n" );

	ewfcommon_acquiry_parameters_fprint(
	 stdout,
	 filename,
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
	 wipe_block_on_read_error );

	/* Ask for confirmation
	 */
	while( input_confirmed == -1 )
	{
		user_input = ewfcommon_get_user_input_fixed_value(
		              stdout,
		              _S_LIBEWF_CHAR( "Continue acquiry with these values" ),
		              ewfcommon_yes_no,
		              2,
		              0 );

		input_confirmed = ewfcommon_determine_yes_no( user_input );

		libewf_common_free( user_input );

		if( input_confirmed <= -1 )	
		{
			fprintf( stdout, "Selected option not supported, please try again or terminate using Ctrl^C.\n" );
		}
	}
	fprintf( stdout, "\n" );

	return( input_confirmed );
}

/* Determine the device size using a file descriptor
 */
uint64_t determine_device_size( int file_descriptor )
{
#if !defined( DIOCGMEDIASIZE ) && defined( DIOCGDINFO )
	struct disklabel disk_label;
#endif
	uint64_t input_size  = 0;
#if defined( DKIOCGETBLOCKCOUNT )
	uint64_t block_count = 0;
	uint32_t block_size  = 0;
#endif
	if( file_descriptor == -1 )
	{
		return( 0 );
	}
#if defined( BLKGETSIZE64 )
	if( ioctl( file_descriptor, BLKGETSIZE64, &input_size ) == -1 )
	{
		return( 0 );
	}
#elif defined( DIOCGMEDIASIZE )
	if( ioctl( file_descriptor, DIOCGMEDIASIZE, &input_size ) == -1 )
	{
		return( 0 );
	}
#elif defined( DIOCGDINFO )
	if( ioctl( file_descriptor, DIOCGDINFO, &disk_label ) == -1 )
	{
		return( 0 );
	}
	input_size = disk_label.d_secperunit * disk_label.d_secsize;
#elif defined( DKIOCGETBLOCKCOUNT )
	if( ioctl( file_descriptor, DKIOCGETBLOCKSIZE, &block_size ) == -1 )
	{
		return( 0 );
	}
	if( ioctl( file_descriptor, DKIOCGETBLOCKCOUNT, &block_count ) == -1 )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	fprintf( stderr, "block size: %" PRIu32 " block count: %" PRIu64 " ", block_size, block_count );
#endif

	input_size = block_count * block_size;
#else
	input_size = 0;
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	fprintf( stderr, "device size: %" PRIu64 "\n", input_size );
#endif
	return( input_size );
}

/* The main program
 */
#if defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	struct stat input_file_stat;

#if defined( HAVE_UUID_UUID_H ) && defined( HAVE_LIBUUID )
	uint8_t guid[ 16 ];
#endif
	CHAR_T *filenames[ 1 ]                   = { NULL };

	LIBEWF_HANDLE *handle                    = NULL;
	LIBEWF_CHAR *calculated_md5_hash_string  = NULL;
	LIBEWF_CHAR *calculated_sha1_hash_string = NULL;
	LIBEWF_CHAR *user_input                  = NULL;
	LIBEWF_CHAR *case_number                 = NULL;
	LIBEWF_CHAR *description                 = NULL;
	LIBEWF_CHAR *evidence_number             = NULL;
	LIBEWF_CHAR *examiner_name               = NULL;
	LIBEWF_CHAR *notes                       = NULL;
	LIBEWF_CHAR *acquiry_operating_system    = NULL;
	LIBEWF_CHAR *acquiry_software_version    = NULL;
	LIBEWF_CHAR *program                     = _S_LIBEWF_CHAR( "ewfacquire" );

	CHAR_T *filename                         = NULL;
	CHAR_T *time_string                      = NULL;
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
	CHAR_T *error_string                     = NULL;
#endif
	void *callback                           = &ewfcommon_process_status_fprint;

	INT_T option                             = 0;
	ssize64_t write_count                    = 0;
	size_t string_length                     = 0;
	time_t timestamp_start                   = 0;
	time_t timestamp_end                     = 0;
	uint64_t maximum_segment_file_size       = 0;
	uint64_t segment_file_size               = 0;
	uint64_t input_size                      = 0;
	uint64_t acquiry_offset                  = 0;
	uint64_t acquiry_size                    = 0;
	uint64_t sectors_per_chunk               = 0;
	uint64_t sector_error_granularity        = 0;
	int8_t compression_level                 = LIBEWF_COMPRESSION_NONE;
	int8_t compress_empty_block              = 0;
	int8_t media_type                        = LIBEWF_MEDIA_TYPE_FIXED;
	int8_t volume_type                       = LIBEWF_VOLUME_TYPE_LOGICAL;
	int8_t wipe_block_on_read_error          = 0;
	uint8_t libewf_format                    = LIBEWF_FORMAT_UNKNOWN;
	uint8_t read_error_retry                 = 2;
	uint8_t swap_byte_pairs                  = 0;
	uint8_t seek_on_error                    = 1;
	uint8_t calculate_md5                    = 1;
	uint8_t calculate_sha1                   = 0;
	uint8_t verbose                          = 0;
	int file_descriptor                      = 0;

	ewfsignal_initialize();

	ewfcommon_version_fprint( stdout, program );

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
					fprintf( stderr, "Unsupported digest type.\n" );
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
				ewfcommon_copyright_fprint( stdout );

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
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
		if( errno != 0 )
		{
			error_string = ewfcommon_strerror( errno );
		}
		if( error_string != NULL )
		{
			fprintf( stderr, "Error opening file or device: %" PRIs " with failure: %" PRIs ".\n",
			 argv[ optind ], error_string );

			libewf_common_free( error_string );
		}
		else
		{
			fprintf( stderr, "Error opening file or device: %" PRIs ".\n", argv[ optind ] );
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
#if !defined( HAVE_WINDOWS_API )
	if( S_ISBLK( input_file_stat.st_mode )
	 || S_ISCHR( input_file_stat.st_mode ) )
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
			filename = ewfcommon_get_user_input_variable_char_t(
			            stdout,
			            _S_LIBEWF_CHAR( "Image path and filename without extension" ) );

			if( filename == NULL )
			{
				fprintf( stdout, "Filename is required, please try again or terminate using Ctrl^C.\n" );
			}
		}
		/* Case number
		 */
		case_number = ewfcommon_get_user_input_variable(
		               stdout,
		               _S_LIBEWF_CHAR( "Case number" ) );

		/* Description
		 */
		description = ewfcommon_get_user_input_variable(
		               stdout,
		               _S_LIBEWF_CHAR( "Description" ) );

		/* Evidence number
		 */
		evidence_number = ewfcommon_get_user_input_variable(
		                   stdout,
		                   _S_LIBEWF_CHAR( "Evidence number" ) );

		/* Examiner name
		 */
		examiner_name = ewfcommon_get_user_input_variable(
		                 stdout,
		                 _S_LIBEWF_CHAR( "Examiner name" ) );

		/* Notes
		 */
		notes = ewfcommon_get_user_input_variable(
		         stdout,
		         _S_LIBEWF_CHAR( "Notes" ) );

		/* Media type
		 */
		user_input = ewfcommon_get_user_input_fixed_value(
		              stdout,
		              _S_LIBEWF_CHAR( "Media type" ),
		              ewfcommon_media_types,
		              EWFCOMMON_MEDIA_TYPES_AMOUNT,
		              EWFCOMMON_MEDIA_TYPES_DEFAULT );

		media_type = ewfcommon_determine_media_type( user_input );

		libewf_common_free( user_input );

		if( media_type <= -1 )
		{
			fprintf( stderr, "Unsupported media type.\n" );

			return( EXIT_FAILURE );
		}

		/* Volume type
		 */
		user_input = ewfcommon_get_user_input_fixed_value(
		              stdout,
		              _S_LIBEWF_CHAR( "Volume type" ),
		              ewfcommon_volume_types,
		              EWFCOMMON_VOLUME_TYPES_AMOUNT,
		              EWFCOMMON_VOLUME_TYPES_DEFAULT );

		volume_type = ewfcommon_determine_volume_type( user_input );

		libewf_common_free( user_input );

		if( volume_type <= -1 )
		{
			fprintf( stderr, "Unsupported volume type.\n" );

			return( EXIT_FAILURE );
		}

		/* Compression
		 */
		user_input = ewfcommon_get_user_input_fixed_value(
		              stdout,
		              _S_LIBEWF_CHAR( "Use compression" ),
		              ewfcommon_compression_levels,
		              EWFCOMMON_COMPRESSION_LEVELS_AMOUNT,
		              EWFCOMMON_COMPRESSION_LEVELS_DEFAULT );

		compression_level = ewfcommon_determine_compression_level( user_input );

		libewf_common_free( user_input );

		if( compression_level <= -1 )
		{
			fprintf( stderr, "Unsupported compression type.\n" );

			return( EXIT_FAILURE );
		}

		/* Empty block compression
		 */
		if( compression_level == LIBEWF_COMPRESSION_NONE )
		{
			user_input = ewfcommon_get_user_input_fixed_value(
			              stdout,
			              _S_LIBEWF_CHAR( "Compress empty blocks" ),
			              ewfcommon_yes_no,
			              2,
			              1 );

			compress_empty_block = ewfcommon_determine_yes_no( user_input );

			libewf_common_free( user_input );

			if( compress_empty_block <= -1 )
			{
				fprintf( stderr, "Unsupported answer.\n" );

				return( EXIT_FAILURE );
			}
		}

		/* File format
		 */
		user_input = ewfcommon_get_user_input_fixed_value(
		              stdout,
		              _S_LIBEWF_CHAR( "Use EWF file format" ),
		              ewfcommon_format_types,
		              EWFCOMMON_FORMAT_TYPES_AMOUNT,
		              EWFCOMMON_FORMAT_TYPES_DEFAULT );

		libewf_format = ewfcommon_determine_libewf_format( user_input );

		libewf_common_free( user_input );

		if( libewf_format == 0 )
		{
			fprintf( stderr, "Unsupported EWF file format type.\n" );

			exit( EXIT_FAILURE );
		}

		/* Size and offset of data to acquire
		 */
		acquiry_offset = ewfcommon_get_user_input_size_variable(
		                  stdout,
		                  _S_LIBEWF_CHAR( "Start to acquire at offset" ),
		                  0,
		                  input_size,
		                  0 );

		acquiry_size = ewfcommon_get_user_input_size_variable(
		                stdout,
		                _S_LIBEWF_CHAR( "Amount of bytes to acquire" ),
		                0,
		                ( input_size - acquiry_offset ),
		                ( input_size - acquiry_offset ) );

		/* Segment file size
		 */
		if( libewf_format == LIBEWF_FORMAT_ENCASE6 )
		{
			maximum_segment_file_size = INT64_MAX;
		}
		else
		{
			maximum_segment_file_size = INT32_MAX;
		}

		segment_file_size = ewfcommon_get_user_input_size_variable(
		                     stdout,
		                     _S_LIBEWF_CHAR( "Evidence segment file size in kbytes (2^10)" ),
		                     1440,
		                     ( maximum_segment_file_size / 1024 ),
		                     ( 650 * 1024 ) );

		segment_file_size *= 1024;

		/* Make sure the segment file size is 1 byte smaller than the maximum
		 */
		if( segment_file_size >= maximum_segment_file_size )
		{
			segment_file_size = maximum_segment_file_size - 1;
		}

		/* Chunk size (sectors per block)
		 */
		user_input = ewfcommon_get_user_input_fixed_value(
		              stdout,
		              _S_LIBEWF_CHAR( "The amount of sectors to read at once" ),
		              ewfcommon_sector_per_block_sizes,
		              EWFCOMMON_SECTOR_PER_BLOCK_SIZES_AMOUNT,
		              EWFCOMMON_SECTOR_PER_BLOCK_SIZES_DEFAULT );

		sectors_per_chunk = libewf_string_to_int64( user_input, libewf_string_length( user_input ) );

		libewf_common_free( user_input );

		/* Error granularity
		 */
		sector_error_granularity = ewfcommon_get_user_input_size_variable(
		                            stdout,
		                            _S_LIBEWF_CHAR( "The amount of sectors to be used as error granularity" ),
		                            1,
		                            sectors_per_chunk,
		                            64 );

		/* The amount of read error retry
		 */
		read_error_retry = (uint8_t) ewfcommon_get_user_input_size_variable(
		                              stdout,
		                              _S_LIBEWF_CHAR( "The amount of retries when a read error occurs" ),
		                              0,
		                              255,
		                              2 );

		/* Wipe the sector on error
		 */
		user_input = ewfcommon_get_user_input_fixed_value(
		              stdout,
		              _S_LIBEWF_CHAR( "Wipe sectors on read error (mimic EnCase like behavior)" ),
		              ewfcommon_yes_no,
		              2,
		              0 );

		wipe_block_on_read_error = ewfcommon_determine_yes_no( user_input );

		libewf_common_free( user_input );

		if( wipe_block_on_read_error <= -1 )
		{
			fprintf( stderr, "Unsupported answer.\n" );

			return( EXIT_FAILURE );
		}
		fprintf( stdout, "\n" );
	}
	/* Check if user is content with values
	 */
	while( confirm_input(
	        filename,
	        case_number,
	        description,
	        evidence_number,
	        examiner_name,
	        notes,
	        (uint8_t) media_type,
	        (uint8_t) volume_type,
	        compression_level,
	        (uint8_t) compress_empty_block,
	        libewf_format,
	        (off64_t) acquiry_offset,
	        (size64_t) acquiry_size,
	        (size64_t) segment_file_size,
	        (uint32_t) sectors_per_chunk,
	        (uint32_t) sector_error_granularity,
	        read_error_retry,
	        (uint8_t) wipe_block_on_read_error ) == 0 );

	/* Done asking user input set up the libewf handle
	 */
	filenames[ 0 ] = filename;

	handle = libewf_open( (CHAR_T * const *) filenames, 1, LIBEWF_OPEN_WRITE );

	if( handle == NULL )
	{
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
		if( errno != 0 )
		{
			error_string = ewfcommon_strerror( errno );
		}
		if( error_string != NULL )
		{
			fprintf( stderr, "Unable to open EWF file(s) with failure: %" PRIs ".\n",
			 error_string );

			libewf_common_free( error_string );
		}
		else
		{
			fprintf( stderr, "Unable to open EWF file(s).\n" );
		}
#else
		fprintf( stderr, "Unable to open EWF file(s).\n" );
#endif

		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_sectors_per_chunk( handle, (uint32_t) sectors_per_chunk ) != 1 )
	{
		fprintf( stderr, "Unable to set sectors per chunk in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_write_segment_file_size( handle, (size64_t) segment_file_size ) != 1 )
	{
		fprintf( stderr, "Unable to set write segment file size in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_write_error_granularity( handle, (uint32_t) sector_error_granularity ) != 1 )
	{
		fprintf( stderr, "Unable to set write error granularity in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_write_media_type( handle, (uint8_t) media_type, (uint8_t) volume_type ) != 1 )
	{
		fprintf( stderr, "Unable to set write media type in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_write_compression_values(
	     handle,
	     compression_level,
	     (uint8_t) compress_empty_block ) != 1 )
	{
		fprintf( stderr, "Unable to set write compression values in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
		if( libewf_set_header_value_acquiry_operating_system(
		     handle,
		     acquiry_operating_system,
		     libewf_string_length( acquiry_operating_system ) ) != 1 )
		{
			fprintf( stderr, "Unable to set header value acquiry operating system in handle.\n" );

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
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
	if( libewf_set_header_value(
	     handle,
	     _S_LIBEWF_CHAR( "acquiry_software" ),
	     program,
	     10 ) != 1 )
	{
		fprintf( stderr, "Unable to set header value acquiry software in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
	if( libewf_set_header_value_acquiry_software_version(
	     handle,
	     acquiry_software_version,
	     libewf_string_length( acquiry_software_version ) ) != 1 )
	{
		fprintf( stderr, "Unable to set header value acquiry software version number in handle.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
			fprintf( stderr, "Unable to close EWF file(s).\n" );
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
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		if( libewf_common_close( file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
		}
		libewf_common_free( filename );

		return( EXIT_FAILURE );
	}
#endif
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated MD5 hash string.\n" );

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			if( libewf_common_close( file_descriptor ) != 0 )
			{
				fprintf( stderr, "Unable to close input.\n" );
			}
			libewf_common_free( filename );

			return( EXIT_FAILURE );
		}
	}
	if( calculate_sha1 == 1 )
	{
		calculated_sha1_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated SHA1 hash string.\n" );

			libewf_common_free( calculated_md5_hash_string );

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			if( libewf_common_close( file_descriptor ) != 0 )
			{
				fprintf( stderr, "Unable to close input.\n" );
			}
			libewf_common_free( filename );

			return( EXIT_FAILURE );
		}
	}
	/* Start acquiring data
	 */
	timestamp_start = time( NULL );
	time_string     = libewf_common_ctime( &timestamp_start );

	if( time_string != NULL )
	{
		fprintf( stdout, "Acquiry started at: %" PRIs "\n", time_string );

		libewf_common_free( time_string );
	}
	else
	{
		fprintf( stdout, "Acquiry started.\n" );
	}
	if( callback != NULL )
	{
		ewfcommon_process_status_initialize( stdout, _S_LIBEWF_CHAR( "acquired" ), timestamp_start );
	}
	fprintf( stdout, "This could take a while.\n\n" );

	write_count = ewfcommon_write_from_file_descriptor(
	               handle,
	               file_descriptor,
	               acquiry_size,
	               acquiry_offset,
	               read_error_retry,
	               (uint32_t) sector_error_granularity,
	               (uint8_t) wipe_block_on_read_error,
	               seek_on_error,
	               calculate_md5,
	               calculated_md5_hash_string,
	               LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5,
	               calculate_sha1,
	               calculated_sha1_hash_string,
	               LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1,
	               callback );

	/* Done acquiring data
	 */
	if( libewf_common_close( file_descriptor ) != 0 )
	{
		fprintf( stderr, "Unable to close input.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		libewf_common_free( filename );

		if( calculate_md5 == 1 )
		{
			libewf_common_free( calculated_md5_hash_string );
		}
		if( calculate_sha1 == 1 )
		{
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	libewf_common_free( filename );

	timestamp_end = time( NULL );
	time_string   = libewf_common_ctime( &timestamp_end );

	if( write_count <= -1 )
	{
		if( time_string != NULL )
		{
			fprintf( stdout, "Acquiry failed at: %" PRIs "\n", time_string );

			libewf_common_free( time_string );
		}
		else
		{
			fprintf( stdout, "Acquiry failed.\n" );
		}
		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		if( calculate_md5 == 1 )
		{
			libewf_common_free( calculated_md5_hash_string );
		}
		if( calculate_sha1 == 1 )
		{
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	if( time_string != NULL )
	{
		fprintf( stdout, "Acquiry completed at: %" PRIs "\n", time_string );

		libewf_common_free( time_string );
	}
	else
	{
		fprintf( stdout, "Acquiry completed.\n" );
	}
	ewfcommon_process_summary_fprint( stdout, _S_LIBEWF_CHAR( "Written" ), write_count, timestamp_start, timestamp_end );

	fprintf( stdout, "\n" );

	ewfcommon_acquiry_errors_fprint( stdout, handle );

	if( libewf_close( handle ) != 0 )
	{
		fprintf( stderr, "Unable to close EWF file(s).\n" );

		if( calculate_md5 == 1 )
		{
			libewf_common_free( calculated_md5_hash_string );
		}
		if( calculate_sha1 == 1 )
		{
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		fprintf( stdout, "MD5 hash calculated over data:\t%" PRIs_EWF "\n", calculated_md5_hash_string );

		libewf_common_free( calculated_md5_hash_string );
	}
	if( calculate_sha1 == 1 )
	{
		fprintf( stdout, "SHA1 hash calculated over data:\t%" PRIs_EWF "\n", calculated_sha1_hash_string );

		libewf_common_free( calculated_sha1_hash_string );
	}
	return( EXIT_SUCCESS );
}

