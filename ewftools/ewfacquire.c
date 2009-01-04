/*
 * ewfacquire
 * Reads data from a file and writes it in EWF format
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <character_string.h>
#include <file_io.h>
#include <file_stream_io.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

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
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Usage: ewfacquire [ -d digest_type ] [ -l filename ] [ -hqsvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file or device\n\n" );

	fprintf( stream, "\t-d:     calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs acquiry errors and the digest (hash) to the filename\n" );
	fprintf( stream, "\t-q:     quiet shows no status information\n" );
	fprintf( stream, "\t-s:     swap byte pairs of the media data (from AB to BA)\n" );
	fprintf( stream, "\t        (use this for big to little endian conversion and vice versa)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Prints an overview of the user provided input
 * and asks the user for confirmation
 * Return 1 if confirmed by user, 0 otherwise
 */
int confirm_input(
     system_character_t *filename,
     character_t *case_number,
     character_t *description,
     character_t *evidence_number,
     character_t *examiner_name,
     character_t *notes,
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
	character_t *user_input = NULL;
	int input_confirmed     = -1;

	fprintf( stdout, "The following acquiry parameters were provided:\n" );

	ewfoutput_acquiry_parameters_fprint(
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
	 wipe_chunk_on_error );

	/* Ask for confirmation
	 */
	while( input_confirmed == -1 )
	{
		user_input = ewfinput_get_fixed_value(
		              stdout,
		              _CHARACTER_T_STRING( "Continue acquiry with these values" ),
		              ewfinput_yes_no,
		              2,
		              0 );

		input_confirmed = ewfinput_determine_yes_no(
		                   user_input );

		memory_free(
		 user_input );

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
	fprintf( stderr, "block size: %" PRIu32 " block count: %" PRIu64 " ",
	 block_size, block_count );
#endif

	input_size = (size64_t) block_count * (size64_t) block_size;
#else
	input_size = 0;
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	fprintf( stderr, "device size: %" PRIu64 "\n",
	 input_size );
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

	system_character_t time_string[ 32 ];

	system_character_t *filenames[ 1 ]       = { NULL };

	character_t *calculated_md5_hash_string  = NULL;
	character_t *calculated_sha1_hash_string = NULL;
	character_t *user_input                  = NULL;
	character_t *case_number                 = NULL;
	character_t *description                 = NULL;
	character_t *evidence_number             = NULL;
	character_t *examiner_name               = NULL;
	character_t *notes                       = NULL;
	character_t *acquiry_operating_system    = NULL;
	character_t *acquiry_software_version    = NULL;
	character_t *program                     = _CHARACTER_T_STRING( "ewfacquire" );

	system_character_t *filename             = NULL;
	system_character_t *log_filename         = NULL;

	FILE *log_file_stream                    = NULL;
	void *callback                           = &ewfoutput_process_status_fprint;

	system_integer_t option                  = 0;
	ssize64_t write_count                    = 0;
	time_t timestamp_start                   = 0;
	time_t timestamp_end                     = 0;
	uint64_t maximum_segment_file_size       = 0;
	uint64_t segment_file_size               = 0;
	uint64_t input_size                      = 0;
	uint64_t acquiry_offset                  = 0;
	uint64_t acquiry_size                    = 0;
	uint64_t sectors_per_chunk               = 0;
	uint64_t sector_error_granularity        = 0;
	uint32_t amount_of_acquiry_errors        = 0;
	int8_t compression_level                 = LIBEWF_COMPRESSION_NONE;
	int8_t compress_empty_block              = 0;
	int8_t media_type                        = LIBEWF_MEDIA_TYPE_FIXED;
	int8_t volume_type                       = LIBEWF_VOLUME_TYPE_LOGICAL;
	int8_t wipe_chunk_on_error               = 0;
	uint8_t libewf_format                    = LIBEWF_FORMAT_UNKNOWN;
	uint8_t read_error_retry                 = 2;
	uint8_t swap_byte_pairs                  = 0;
	uint8_t seek_on_error                    = 1;
	uint8_t calculate_md5                    = 1;
	uint8_t calculate_sha1                   = 0;
	uint8_t verbose                          = 0;
	int file_descriptor                      = 0;
	int error_abort                          = 0;

	ewfoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = ewfgetopt(
	                   argc,
	                   argv,
	                   _SYSTEM_CHARACTER_T_STRING( "d:hl:qsvV" ) ) ) != (system_integer_t) -1 )
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

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'q':
				callback = NULL;

				break;

			case (system_integer_t) 's':
				swap_byte_pairs = 1;

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
#if !defined( HAVE_WINDOWS_API )
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
	acquiry_operating_system = ewfcommon_determine_operating_system();
	acquiry_software_version = LIBEWF_VERSION_STRING;

	do
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

		/* Request the necessary case data
		 */
		fprintf( stdout, "Acquiry parameters required, please provide the necessary input\n" );

		/* Output filename
		 */
		while( filename == NULL )
		{
			filename = ewfinput_get_variable_system_character(
			            stdout,
			            _CHARACTER_T_STRING( "Image path and filename without extension" ) );

			if( filename == NULL )
			{
				fprintf( stdout, "Filename is required, please try again or terminate using Ctrl^C.\n" );
			}
		}
		/* Case number
		 */
		case_number = ewfinput_get_variable(
		               stdout,
		               _CHARACTER_T_STRING( "Case number" ) );

		/* Description
		 */
		description = ewfinput_get_variable(
		               stdout,
		               _CHARACTER_T_STRING( "Description" ) );

		/* Evidence number
		 */
		evidence_number = ewfinput_get_variable(
		                   stdout,
		                   _CHARACTER_T_STRING( "Evidence number" ) );

		/* Examiner name
		 */
		examiner_name = ewfinput_get_variable(
		                 stdout,
		                 _CHARACTER_T_STRING( "Examiner name" ) );

		/* Notes
		 */
		notes = ewfinput_get_variable(
		         stdout,
		         _CHARACTER_T_STRING( "Notes" ) );

		/* Media type
		 */
		user_input = ewfinput_get_fixed_value(
		              stdout,
		              _CHARACTER_T_STRING( "Media type" ),
		              ewfinput_media_types,
		              EWFINPUT_MEDIA_TYPES_AMOUNT,
		              EWFINPUT_MEDIA_TYPES_DEFAULT );

		media_type = ewfinput_determine_media_type(
		              user_input );

		memory_free(
		 user_input );

		if( media_type <= -1 )
		{
			fprintf( stderr, "Unsupported media type defaulting to: fixed.\n" );

			media_type = LIBEWF_MEDIA_TYPE_FIXED;
		}

		/* Volume type
		 */
		user_input = ewfinput_get_fixed_value(
		              stdout,
		              _CHARACTER_T_STRING( "Volume type" ),
		              ewfinput_volume_types,
		              EWFINPUT_VOLUME_TYPES_AMOUNT,
		              EWFINPUT_VOLUME_TYPES_DEFAULT );

		volume_type = ewfinput_determine_volume_type(
		               user_input );

		memory_free(
		 user_input );

		if( volume_type <= -1 )
		{
			fprintf( stderr, "Unsupported volume type defaulting to: logical.\n" );

			volume_type = LIBEWF_VOLUME_TYPE_LOGICAL;
		}

		/* Compression
		 */
		user_input = ewfinput_get_fixed_value(
		              stdout,
		              _CHARACTER_T_STRING( "Use compression" ),
		              ewfinput_compression_levels,
		              EWFINPUT_COMPRESSION_LEVELS_AMOUNT,
		              EWFINPUT_COMPRESSION_LEVELS_DEFAULT );

		compression_level = ewfinput_determine_compression_level(
		                     user_input );

		memory_free(
		 user_input );

		if( compression_level <= -1 )
		{
			fprintf( stderr, "Unsupported compression type defaulting to: none.\n" );

			compression_level = LIBEWF_COMPRESSION_NONE;
		}

		/* Empty block compression
		 */
		if( compression_level == LIBEWF_COMPRESSION_NONE )
		{
			user_input = ewfinput_get_fixed_value(
			              stdout,
			              _CHARACTER_T_STRING( "Compress empty blocks" ),
			              ewfinput_yes_no,
			              2,
			              1 );

			compress_empty_block = ewfinput_determine_yes_no(
			                        user_input );

			memory_free(
			 user_input );

			if( compress_empty_block <= -1 )
			{
				fprintf( stderr, "Unsupported compress emtpy blocks defaulting to: no.\n" );

				compress_empty_block = 0;
			}
		}

		/* File format
		 */
		user_input = ewfinput_get_fixed_value(
		              stdout,
		              _CHARACTER_T_STRING( "Use EWF file format" ),
		              ewfinput_format_types,
		              EWFINPUT_FORMAT_TYPES_AMOUNT,
		              EWFINPUT_FORMAT_TYPES_DEFAULT );

		libewf_format = ewfinput_determine_libewf_format(
		                 user_input );

		memory_free(
		 user_input );

		if( libewf_format == 0 )
		{
			fprintf( stderr, "Unsupported EWF file format type defaulting to: encase5.\n" );

			libewf_format = LIBEWF_FORMAT_ENCASE5;
		}

		/* Size and offset of data to acquire
		 */
		acquiry_offset = ewfinput_get_size_variable(
		                  stdout,
		                  _CHARACTER_T_STRING( "Start to acquire at offset" ),
		                  0,
		                  input_size,
		                  0 );

		acquiry_size = ewfinput_get_size_variable(
		                stdout,
		                _CHARACTER_T_STRING( "Amount of bytes to acquire" ),
		                0,
		                ( input_size - acquiry_offset ),
		                ( input_size - acquiry_offset ) );

		/* Segment file size
		 */
		if( libewf_format == LIBEWF_FORMAT_ENCASE6 )
		{
			maximum_segment_file_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT;
		}
		else
		{
			maximum_segment_file_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT;
		}
		segment_file_size = ewfinput_get_byte_size_variable(
		                     stdout,
		                     _CHARACTER_T_STRING( "Evidence segment file size in bytes" ),
		                     ( EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE ),
		                     ( maximum_segment_file_size ),
		                     ( EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE ) );

		/* Make sure the segment file size is smaller than or equal to the maximum
		 */
		if( segment_file_size > maximum_segment_file_size )
		{
			segment_file_size = maximum_segment_file_size;
		}

		/* Chunk size (sectors per block)
		 */
		user_input = ewfinput_get_fixed_value(
		              stdout,
		              _CHARACTER_T_STRING( "The amount of sectors to read at once" ),
		              ewfinput_sector_per_block_sizes,
		              EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT,
		              EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT );

		if( string_to_uint64(
		     user_input,
		     string_length(
		      user_input ),
		     &sectors_per_chunk ) != 1 )
		{
			fprintf( stderr, "Unsupported sectors per chunk on error defaulting to: %d.\n",
			 EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT );

			sectors_per_chunk = EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT;
		}
		memory_free(
		 user_input );

		/* Error granularity
		 */
		sector_error_granularity = ewfinput_get_size_variable(
		                            stdout,
		                            _CHARACTER_T_STRING( "The amount of sectors to be used as error granularity" ),
		                            1,
		                            sectors_per_chunk,
		                            64 );

		/* The amount of read error retry
		 */
		read_error_retry = (uint8_t) ewfinput_get_size_variable(
		                              stdout,
		                              _CHARACTER_T_STRING( "The amount of retries when a read error occurs" ),
		                              0,
		                              255,
		                              2 );

		/* Wipe the sector on error
		 */
		user_input = ewfinput_get_fixed_value(
		              stdout,
		              _CHARACTER_T_STRING( "Wipe sectors on read error (mimic EnCase like behavior)" ),
		              ewfinput_yes_no,
		              2,
		              1 );

		wipe_chunk_on_error = ewfinput_determine_yes_no(
		                       user_input );

		memory_free(
		 user_input );

		if( wipe_chunk_on_error <= -1 )
		{
			fprintf( stderr, "Unsupported wipe chunk on error defaulting to: no.\n" );

			wipe_chunk_on_error = 0;
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
	        (uint8_t) wipe_chunk_on_error ) == 0 );

	if( ewfsignal_attach(
	     ewfcommon_signal_handler ) != 1 )
	{
		fprintf( stderr, "Unable to attach signal handler.\n" );
	}
	if( ewfcommon_abort == 0 )
	{
		/* Set up the libewf handle
		 */
		filenames[ 0 ] = filename;

		ewfcommon_libewf_handle = libewf_open(
			                   (system_character_t * const *) filenames,
			                   1,
			                   LIBEWF_OPEN_WRITE );

		memory_free(
		 filename );

		if( ewfcommon_libewf_handle == NULL )
		{
			ewfoutput_error_fprint(
			 stderr, "Unable to create EWF file(s)" );

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
		if( file_io_close(
		     file_descriptor ) != 0 )
		{
			fprintf( stderr, "Unable to close input.\n" );
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
			if( file_io_close(
			     file_descriptor ) != 0 )
			{
				fprintf( stderr, "Unable to close input.\n" );
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
			if( file_io_close(
			     file_descriptor ) != 0 )
			{
				fprintf( stderr, "Unable to close input.\n" );
			}
			return( EXIT_FAILURE );
		}
	}
	if( ewfcommon_abort == 0 )
	{
		/* Start acquiring data
		 */
		timestamp_start = time( NULL );

		if( ewfcommon_ctime(
		     &timestamp_start,
		     time_string,
		     32 ) == NULL )
		{
			fprintf( stdout, "Acquiry started.\n" );
		}
		else
		{
			fprintf( stdout, "Acquiry started at: %" PRIs_SYSTEM "\n",
			 time_string );
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
			       file_descriptor,
			       acquiry_size,
			       acquiry_offset,
			       (uint32_t) sectors_per_chunk,
			       512,
			       read_error_retry,
			       (uint32_t) sector_error_granularity,
			       (uint8_t) wipe_chunk_on_error,
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
	/* Done acquiring data
	 */
	if( file_io_close(
	     file_descriptor ) != 0 )
	{
		fprintf( stderr, "Unable to close input.\n" );

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
	if( ewfcommon_abort == 0 )
	{
		timestamp_end = time( NULL );

		if( write_count <= -1 )
		{
			if( ewfcommon_ctime(
			     &timestamp_end,
			     time_string,
			     32 ) == NULL )
			{
				fprintf( stdout, "Acquiry failed.\n" );
			}
			else
			{
				fprintf( stdout, "Acquiry failed at: %" PRIs_SYSTEM "\n",
				 time_string );
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
		if( ewfcommon_ctime(
		     &timestamp_end,
		     time_string,
		     32 ) == NULL )
		{
			fprintf( stdout, "Acquiry completed.\n" );
		}
		else
		{
			fprintf( stdout, "Acquiry completed at: %" PRIs_SYSTEM "\n",
			 time_string );
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
			file_stream_io_fclose(
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
			file_stream_io_fclose(
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
		if( file_stream_io_fclose(
		     log_file_stream ) != 0 )
		{
			fprintf( stderr, "Unable to close log file: %s.\n",
			 log_filename );
		}
	}
	return( EXIT_SUCCESS );
}

