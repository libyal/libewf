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
#include <notify.h>
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
#include "ewfprocess_status.h"
#include "ewfsignal.h"
#include "ewfstring.h"

#define EWFACQUIRE_INPUT_BUFFER_SIZE	64

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

/* Prints an overview of the acquiry parameters and asks the for confirmation
 * Return 1 if confirmed by user, 0 otherwise or -1 on error
 */
int8_t confirm_acquiry_parameters(
        FILE *output_stream,
        character_t *input_buffer,
        size_t input_buffer_size,
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
	character_t *fixed_string_variable = NULL;
	static char *function              = "confirm_acquiry_parameters";
	int8_t input_confirmed             = -1;

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
		if( ewfinput_get_fixed_string_variable(
		     output_stream,
		     input_buffer,
		     input_buffer_size,
		     _CHARACTER_T_STRING( "Continue acquiry with these values" ),
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
	character_t input_buffer[ EWFACQUIRE_INPUT_BUFFER_SIZE ];

	struct stat input_file_stat;

	system_character_t *filenames[ 1 ]       = { NULL };

	character_t *acquiry_operating_system    = NULL;
	character_t *acquiry_software_version    = NULL;
	character_t *calculated_md5_hash_string  = NULL;
	character_t *calculated_sha1_hash_string = NULL;
	character_t *case_number                 = NULL;
	character_t *description                 = NULL;
	character_t *evidence_number             = NULL;
	character_t *examiner_name               = NULL;
	character_t *fixed_string_variable       = NULL;
	character_t *notes                       = NULL;
	character_t *program                     = _CHARACTER_T_STRING( "ewfacquire" );

	system_character_t *filename             = NULL;
	system_character_t *log_filename         = NULL;

	FILE *log_file_stream                    = NULL;
	void *callback                           = &ewfprocess_status_update;

	system_integer_t option                  = 0;
	ssize64_t write_count                    = 0;
	uint64_t acquiry_offset                  = 0;
	uint64_t acquiry_size                    = 0;
	uint32_t amount_of_acquiry_errors        = 0;
	uint64_t input_size_variable             = 0;
	uint64_t input_size                      = 0;
	uint64_t maximum_segment_file_size       = 0;
	uint64_t segment_file_size               = 0;
	uint32_t sector_error_granularity        = 0;
	uint32_t sectors_per_chunk               = 0;
	uint8_t calculate_md5                    = 1;
	uint8_t calculate_sha1                   = 0;
	uint8_t compress_empty_block             = 0;
	uint8_t libewf_format                    = LIBEWF_FORMAT_UNKNOWN;
	uint8_t media_type                       = LIBEWF_MEDIA_TYPE_FIXED;
	uint8_t read_error_retry                 = 2;
	uint8_t swap_byte_pairs                  = 0;
	uint8_t seek_on_error                    = 1;
	uint8_t verbose                          = 0;
	uint8_t volume_type                      = LIBEWF_VOLUME_TYPE_LOGICAL;
	uint8_t wipe_chunk_on_error              = 0;
	int8_t acquiry_parameters_confirmed      = 0;
	int8_t compression_level                 = LIBEWF_COMPRESSION_NONE;
	int error_abort                          = 0;
	int file_descriptor                      = 0;
	int status                               = 0;

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
	/* Create the input buffers
	 */
	filename = (system_character_t *) memory_allocate(
	                                   sizeof( system_character_t ) * 1024 );

	if( filename == NULL )
	{
		fprintf( stderr, "Unable to create output filename string.\n" );


		return( EXIT_FAILURE );
	}
	case_number = (character_t *) memory_allocate(
				       sizeof( character_t ) * 256 );

	if( case_number == NULL )
	{
		fprintf( stderr, "Unable to create case number string.\n" );

		memory_free(
		 filename );

		return( EXIT_FAILURE );
	}
	description = (character_t *) memory_allocate(
				       sizeof( character_t ) * 256 );

	if( description == NULL )
	{
		fprintf( stderr, "Unable to create description string.\n" );

		memory_free(
		 case_number );
		memory_free(
		 filename );

		return( EXIT_FAILURE );
	}
	evidence_number = (character_t *) memory_allocate(
					   sizeof( character_t ) * 256 );

	if( evidence_number == NULL )
	{
		fprintf( stderr, "Unable to create evidence number string.\n" );

		memory_free(
		 description );
		memory_free(
		 case_number );
		memory_free(
		 filename );

		return( EXIT_FAILURE );
	}
	examiner_name = (character_t *) memory_allocate(
					 sizeof( character_t ) * 256 );

	if( examiner_name == NULL )
	{
		fprintf( stderr, "Unable to create examiner name string.\n" );

		memory_free(
		 evidence_number );
		memory_free(
		 description );
		memory_free(
		 case_number );
		memory_free(
		 filename );

		return( EXIT_FAILURE );
	}
	notes = (character_t *) memory_allocate(
				 sizeof( character_t ) * 256 );

	if( notes == NULL )
	{
		fprintf( stderr, "Unable to create notes string.\n" );

		memory_free(
		 examiner_name );
		memory_free(
		 evidence_number );
		memory_free(
		 description );
		memory_free(
		 case_number );
		memory_free(
		 filename );

		return( EXIT_FAILURE );
	}
	/* Request the necessary case data
	 */
	do
	{
		fprintf( stdout, "Acquiry parameters required, please provide the necessary input\n" );

		/* Output filename
		 */
		while( ewfinput_get_string_variable_system_character(
			stdout,
			_CHARACTER_T_STRING( "Image path and filename without extension" ),
			filename,
			1024 ) != 1 )
		{
			fprintf( stdout, "Filename is required, please try again or terminate using Ctrl^C.\n" );
		}
		/* Case number
		 */
		if( ewfinput_get_string_variable(
		     stdout,
		     _CHARACTER_T_STRING( "Case number" ),
		     case_number,
		     256 ) == -1 )
		{
			fprintf( stdout, "Unable to set case number string.\n" );

			case_number[ 0 ] = 0;
		}
		/* Description
		 */
		if( ewfinput_get_string_variable(
		     stdout,
		     _CHARACTER_T_STRING( "Description" ),
		     description,
		     256 ) == -1 )
		{
			fprintf( stdout, "Unable to set description string.\n" );

			description[ 0 ] = 0;
		}
		/* Evidence number
		 */
		if( ewfinput_get_string_variable(
		     stdout,
		     _CHARACTER_T_STRING( "Evidence number" ),
		     evidence_number,
		     256 ) == -1 )
		{
			fprintf( stdout, "Unable to set evidence number string.\n" );

			evidence_number[ 0 ] = 0;
		}
		/* Examiner name
		 */
		if( ewfinput_get_string_variable(
		     stdout,
		     _CHARACTER_T_STRING( "Examiner name" ),
		     examiner_name,
		     256 ) == -1 )
		{
			fprintf( stdout, "Unable to set examiner name string.\n" );

			examiner_name[ 0 ] = 0;
		}
		/* Notes
		 */
		if( ewfinput_get_string_variable(
		     stdout,
		     _CHARACTER_T_STRING( "Notes" ),
		     notes,
		     256 ) == -1 )
		{
			fprintf( stdout, "Unable to set notes string.\n" );

			notes[ 0 ] = 0;
		}
		/* Media type
		 */
		if( ewfinput_get_fixed_string_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Media type" ),
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
		/* Volume type
		 */
		if( ewfinput_get_fixed_string_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Volume type" ),
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
		/* Compression
		 */
		if( ewfinput_get_fixed_string_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Use compression" ),
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
		/* File format
		 */
		if( ewfinput_get_fixed_string_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Use EWF file format" ),
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
		/* Offset of data to acquire
		 */
		if( ewfinput_get_size_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Start to acquire at offset" ),
		     0,
		     input_size,
		     0,
		     &acquiry_offset ) == -1 )
		{
			acquiry_offset = 0;

			fprintf( stdout, "Unable to determine acquiry offset defaulting to: %" PRIu64 ".\n",
			 acquiry_offset );
		}
		/* Size of data to acquire
		 */
		if( ewfinput_get_size_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Amount of bytes to acquire" ),
		     0,
		     ( input_size - acquiry_offset ),
		     ( input_size - acquiry_offset ),
		     &acquiry_size ) == -1 )
		{
			acquiry_size = input_size - acquiry_offset;

			fprintf( stdout, "Unable to determine input size defaulting to: %" PRIu64 ".\n",
			 acquiry_size );
		}
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
		if( ewfinput_get_byte_size_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Evidence segment file size in bytes" ),
		     ( EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE ),
		     ( maximum_segment_file_size ),
		     ( EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE ),
		     &segment_file_size ) == -1 )
		{
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
		/* Chunk size (sectors per block)
		 */
		if( ewfinput_get_fixed_string_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "The amount of sectors to read at once" ),
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
		/* Error granularity
		 */
		if( ewfinput_get_size_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "The amount of sectors to be used as error granularity" ),
		     1,
		     (uint64_t) sectors_per_chunk,
		     64,
		     &input_size_variable ) == -1 )
		{
			input_size_variable = 64;

			fprintf( stdout, "Unable to determine error granularity defaulting to: %" PRIu64 ".\n",
			 input_size_variable );
		}
		sector_error_granularity = (uint32_t) input_size_variable;

		/* The amount of read error retry
		 */
		if( ewfinput_get_size_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "The amount of retries when a read error occurs" ),
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

		/* Wipe the sector on error
		 */
		if( ewfinput_get_fixed_string_variable(
		     stdout,
		     input_buffer,
		     EWFACQUIRE_INPUT_BUFFER_SIZE,
		     _CHARACTER_T_STRING( "Wipe sectors on read error (mimic EnCase like behavior)" ),
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
		fprintf( stdout, "\n" );

		/* Check if user is content with the acquiry parameters
		 */
		acquiry_parameters_confirmed = confirm_acquiry_parameters(
		                                stdout,
		                                input_buffer,
		                                EWFACQUIRE_INPUT_BUFFER_SIZE,
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
	}
	while( acquiry_parameters_confirmed == 0 );

	if( ewfsignal_attach(
	     ewfcommon_signal_handler ) != 1 )
	{
		fprintf( stderr, "Unable to attach signal handler.\n" );
	}
	if( ewfcommon_abort == 0 )
	{
		acquiry_operating_system = ewfcommon_determine_operating_system();
		acquiry_software_version = LIBEWF_VERSION_STRING;

		/* Set up the libewf handle
		 */
		filenames[ 0 ] = filename;

		ewfcommon_libewf_handle = libewf_open(
			                   (system_character_t * const *) filenames,
			                   1,
			                   LIBEWF_OPEN_WRITE );

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
		if( acquiry_operating_system != NULL )
		{
			memory_free(
			 acquiry_operating_system );
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
	 filename );

	if( error_abort != 0 )
	{
		libewf_close(
		 ewfcommon_libewf_handle );

		file_io_close(
		 file_descriptor );

		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (character_t *) memory_allocate(
		                                              sizeof( character_t ) * EWFSTRING_DIGEST_HASH_LENGTH_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated MD5 hash string.\n" );

			libewf_close(
			 ewfcommon_libewf_handle );

			file_io_close(
			 file_descriptor );

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

			libewf_close(
			 ewfcommon_libewf_handle );

			file_io_close(
			 file_descriptor );

			return( EXIT_FAILURE );
		}
	}
	if( ewfcommon_abort == 0 )
	{
		if( ewfprocess_status_initialize(
		     &process_status,
		     _CHARACTER_T_STRING( "Acquiry" ),
		     _CHARACTER_T_STRING( "acquired" ),
		     _CHARACTER_T_STRING( "Written" ),
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
			libewf_close(
			 ewfcommon_libewf_handle );

			file_io_close(
			 file_descriptor );

			return( EXIT_FAILURE );
		}
		if( ewfprocess_status_start(
		     process_status ) != 1 )
		{
			fprintf( stderr, "Unable to start process status.\n" );

			ewfprocess_status_free(
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
			libewf_close(
			 ewfcommon_libewf_handle );

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

		if( write_count <= -1 )
		{
			status = EWFPROCESS_STATUS_FAILED;
		}
		else
		{
			status = EWFPROCESS_STATUS_COMPLETED;
		}
	}
	/* Done acquiring data
	 */
	if( file_io_close(
	     file_descriptor ) != 0 )
	{
		fprintf( stderr, "Unable to close input.\n" );

		ewfprocess_status_free(
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
		libewf_close(
		 ewfcommon_libewf_handle );

		return( EXIT_FAILURE );
	}
	if( ewfcommon_abort != 0 )
	{
		status = EWFPROCESS_STATUS_ABORTED;
	}
	if( ewfprocess_status_stop(
	     process_status,
	     (size64_t) write_count,
	     status ) != 1 )
	{
		fprintf( stderr, "Unable to stop process status.\n" );

		ewfprocess_status_free(
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
		libewf_close(
		 ewfcommon_libewf_handle );

		return( EXIT_FAILURE );
	}
	if( ewfprocess_status_free(
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
		libewf_close(
		 ewfcommon_libewf_handle );

		return( EXIT_FAILURE );
	}
	if( status == EWFPROCESS_STATUS_COMPLETED )
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
	if( ewfsignal_detach() != 1 )
	{
		fprintf( stderr, "Unable to detach signal handler.\n" );
	}
        if( status != EWFPROCESS_STATUS_COMPLETED )
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

