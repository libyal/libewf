/*
 * ewfacquire
 * Reads data from a file and writes it in EWF format
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * This code is derrived from information and software contributed by
 * - Expert Witness Compression Format specification by Andrew Rosen
 *   (http://www.arsdata.com/SMART/whitepaper.html)
 * - libevf from PyFlag by Michael Cohen
 *   (http://pyflag.sourceforge.net/)
 * - Open SSL for the implementation of the MD5 hash algorithm
 * - Wietse Venema for error handling code
 *
 * Additional credits go to
 * - Robert Jan Mora for testing and other contribution
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
 *   must acknowledge the contribution by people stated above.
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

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/utsname.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef LINUX
#include <linux/fs.h>
#endif

#ifdef CYGWIN
#include <cygwin/fs.h>
#endif

#ifdef BSD
#include <sys/ioctl.h>
#include <sys/disk.h>
#include <sys/disklabel.h>
#endif

#ifdef DARWIN
#include <sys/ioctl.h>
#include <sys/disk.h>
#endif

#include "libewf.h"

/* Prints the executable usage information
 */
void usage( void )
{
	fprintf( stderr, "Usage: ewfacquire [ -hqvV ] source\n" );

	fprintf( stderr, "\tsource: the source file or device\n" );

	fprintf( stderr, "\t-h:     shows this help\n" );
	fprintf( stderr, "\t-q:     quiet shows no status information\n" );
	fprintf( stderr, "\t-v:     verbose output to stderr\n" );
	fprintf( stderr, "\t-V:     print version\n" );

	exit( EXIT_FAILURE );
}

/* Prints the executable version information
 */
void version( void )
{
	fprintf( stderr, "ewfacquire version: %s\n", VERSION );

	exit( EXIT_SUCCESS );
}

/* Determines the units strings of a certain factor value
 */
char *determine_units_string( int factor )
{
	switch( factor )
	{
		case 0:
			return( "B" );
		case 1:
			return( "kB" );
		case 2:
			return( "MB" );
		case 3:
			return( "GB" );
		case 4:
			return( "TB" );
		case 5:
			return( "PB" );
		case 6:
			return( "EB" );
		default :
			break;
	}
	return( "?B" );
}

/* Get variable input from the user
 * with a maximum of 1023 characters
 */
char *get_user_input_variable( char *request_string )
{
	char user_input_buffer[ 1024 ];

	char *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	char *user_input            = NULL;
	char *end_of_input          = NULL;
	uint32_t input_length       = 0;
	uint32_t string_iterator    = 0;
	uint8_t string_valid        = 1;

	if( request_string == NULL )
	{
		return( NULL );
	}
	while( 1 )
	{
		fprintf( stdout, "%s: ", request_string );

		user_input_buffer_ptr = fgets( user_input_buffer_ptr, 1023, stdin );

		if( user_input_buffer_ptr != NULL )
		{
			end_of_input = memchr( user_input_buffer_ptr, '\n', 1024 );

			if( end_of_input == NULL )
			{
				return( NULL );
			}
			input_length = end_of_input - user_input_buffer_ptr;

			if( input_length <= 0 )
			{
				return( NULL );
			}
			for( string_iterator = 0; string_iterator < input_length; string_iterator++ )
			{
				if( user_input_buffer[ string_iterator ] < 0x20 )
				{
					fprintf( stdout, "Invalid character in input, please try again or terminate using Ctrl^C.\n" );

					string_valid = 0;
				}
				else if( user_input_buffer[ string_iterator ] >= 0x7f )
				{
					fprintf( stdout, "Invalid character in input, please try again or terminate using Ctrl^C.\n" );

					string_valid = 0;
				}
			}
			if( string_valid == 1 )
			{
				user_input = malloc( sizeof( char ) * ( input_length + 1 ) );

				if( user_input == NULL )
				{
					fprintf( stdout, "Unable to allocate memory for string.\n" );

					exit( EXIT_FAILURE );
				}
				memcpy( user_input, user_input_buffer_ptr, input_length );

				user_input[ input_length ] = '\0';

				break;
			}
		}
		else
		{
			fprintf( stdout, "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( user_input );
}

/* Get variable containing a size definnition input from the user
 * with a maximum of 1023 characters
 */
uint32_t get_user_input_size_variable( char *request_string, uint32_t minimum, uint32_t maximum, uint32_t default_value )
{
	char user_input_buffer[ 1024 ];

	char *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	char *last_character        = NULL;
	uint32_t input_length       = 0;
	uint32_t size_value         = 0;

	if( request_string == NULL )
	{
		return( 0 );
	}
	while( 1 )
	{
		fprintf( stdout, "%s (%" PRIu32 " >= value >= %" PRIu32 ") [%" PRIu32 "]: ", request_string, minimum, maximum, default_value );

		user_input_buffer_ptr = fgets( user_input_buffer_ptr, 1023, stdin );

		if( user_input_buffer_ptr != NULL )
		{
			/* Remove the trailing newline character
			 */
			input_length = strlen( user_input_buffer_ptr ) - 1;

			if( input_length <= 0 )
			{
				return( default_value );
			}
			last_character = &user_input_buffer_ptr[ input_length ];
			size_value     = strtoul( user_input_buffer_ptr, &last_character, 0 );

			if( ( size_value >= minimum ) && ( size_value <= maximum ) )
			{
				break;
			}
			else
			{
				fprintf( stdout, "Value not within specified range, please try again or terminate using Ctrl^C.\n" );
			}
		}
		else
		{
			fprintf( stdout, "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( size_value );
}

/* Get fixed value input from the user
 * The first value is considered the default value
 */
char *get_user_input_fixed_value( char *request_string, char **values, uint8_t amount, uint8_t default_value )
{
	char user_input_buffer[ 1024 ];

	char *user_input_buffer_ptr = &user_input_buffer[ 0 ];

	uint32_t input_length = 0;
	uint8_t iterator      = 0;
	uint8_t value_match   = 0;
	uint8_t value_size    = 0;
	char *user_input      = NULL;

	if( request_string == NULL )
	{
		return( NULL );
	}
	if( default_value >= amount )
	{
		fprintf( stderr, "Default value cannot be larger than or equal as amount.\n" );

		exit( EXIT_FAILURE );
	}
	while( 1 )
	{
		fprintf( stdout, "%s", request_string );

		iterator = 0;

		while( iterator < amount )
		{
			if( iterator == default_value )
			{
				fprintf( stdout, " [%s] (%s", values[ iterator ], values[ iterator ] );
			}
			else
			{
				fprintf( stdout, ", %s", values[ iterator ] );
			}
			iterator++;
		}
		fprintf( stdout, "): " );

		user_input_buffer_ptr = fgets( user_input_buffer_ptr, 1023, stdin );

		if( user_input_buffer_ptr != NULL )
		{
			iterator = 0;

			/* Remove the trailing newline character
			 */
			input_length = strlen( user_input_buffer_ptr ) - 1;

			/* Check if the default value was selected
			 */
			if( input_length == 0 )
			{
				iterator     = default_value;
				input_length = strlen( values[ iterator ] );
				value_match  = 1;
			}
			else
			{
				while( iterator < amount )
				{
					value_size = strlen( values[ iterator ] );

					if( strncmp( user_input_buffer_ptr, values[ iterator ], value_size ) == 0 )
					{
						/* Make sure no trailing characters were given
						 */
						if( user_input_buffer_ptr[ value_size ] == '\n' )
						{
							value_match = 1;

							break;
						}
					}
					iterator++;
				}
			}
		}
		else
		{
			fprintf( stdout, "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
		if( value_match == 1 )
		{
			value_size = strlen( values[ iterator ] );

			user_input = malloc( sizeof( char ) * ( value_size + 1 ) );

			if( user_input == NULL )
			{
				fprintf( stderr, "Unable to allocate memory for string.\n" );

				exit( EXIT_FAILURE );
			}
			memcpy( user_input, values[ iterator ], input_length );

			break;
		}
		else
		{
			fprintf( stdout, "Selected option not supported, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( user_input );
}

/* Prints an overview of the user provided input
 * and asks the user for confirmation
 * Return 1 if confirmed by user, 0 otherwise
 */
int confirm_input( char *filename, char *case_number, char *description, char *evidence_number, char *examiner_name, char *notes, int8_t compression_level, uint8_t compress_empty_block, uint8_t ewf_format, uint64_t ewf_file_size, uint64_t sectors_per_block, uint64_t error_granularity, uint8_t read_error_retry, uint8_t wipe_block_on_read_error )
{
	char *user_input    = NULL;
	char *yes_no[ 2 ]   = { "yes", "no" };
	int input_confirmed = 0;

	fprintf( stdout, "The following information was provided:\n" );

	fprintf( stdout, "Image path and filename:\t%s.", filename );

	if( ewf_format == LIBEWF_FORMAT_SMART )
	{
		fprintf( stdout, "s01\n" );
	}
	else
	{
		fprintf( stdout, "E01\n" );
	}
	fprintf( stdout, "Case number:\t\t\t" );

	if( case_number != NULL )
	{
		fprintf( stdout, "%s", case_number );
	}
	fprintf( stdout, "\n" );

	fprintf( stdout, "Description:\t\t\t" );

	if( description != NULL )
	{
		fprintf( stdout, "%s", description );
	}
	fprintf( stdout, "\n" );

	fprintf( stdout, "Evidence number:\t\t" );

	if( evidence_number != NULL )
	{
		fprintf( stdout, "%s", evidence_number );
	}
	fprintf( stdout, "\n" );

	fprintf( stdout, "Examiner name:\t\t\t" );

	if( examiner_name != NULL )
	{
		fprintf( stdout, "%s", examiner_name );
	}
	fprintf( stdout, "\n" );

	fprintf( stdout, "Notes:\t\t\t\t" );

	if( notes != NULL )
	{
		fprintf( stdout, "%s", notes );
	}
	fprintf( stdout, "\n" );

	fprintf( stdout, "Compression used:\t\t" );

	if( compression_level == EWF_COMPRESSION_FAST )
	{
		fprintf( stdout, "fast\n" );
	}
	else if( compression_level == EWF_COMPRESSION_BEST )
	{
		fprintf( stdout, "best\n" );
	}
	else if( compression_level == EWF_COMPRESSION_NONE )
	{
		fprintf( stdout, "none\n" );

		fprintf( stdout, "Compress empty blocks:\t\t" );

		if( compress_empty_block == 0 )
		{
			fprintf( stdout, "no\n" );
		}
		else
		{
			fprintf( stdout, "yes\n" );
		}
	}
	fprintf( stdout, "EWF file format:\t\t" );

	if( ewf_format == LIBEWF_FORMAT_SMART )
	{
		fprintf( stdout, "SMART\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_FTK )
	{
		fprintf( stdout, "FTK Imager\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE1 )
	{
		fprintf( stdout, "Encase 1\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE2 )
	{
		fprintf( stdout, "Encase 2\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE3 )
	{
		fprintf( stdout, "Encase 3\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE4 )
	{
		fprintf( stdout, "Encase 4\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE5 )
	{
		fprintf( stdout, "Encase 5\n" );
	}
	else
	{
		fprintf( stdout, "\n" );
	}
	fprintf( stdout, "Evidence file size:\t\t%" PRIu64 " kbytes\n", ( ewf_file_size / 1024 ) );
	fprintf( stdout, "Block size:\t\t\t%" PRIu64 " sectors\n", sectors_per_block );
	fprintf( stdout, "Error granularity:\t\t%" PRIu64 " sectors\n", error_granularity );
	fprintf( stdout, "Retries on read error:\t\t%" PRIu8 "\n", read_error_retry );

	fprintf( stdout, "Wipe sectors on read error:\t" );

	if( wipe_block_on_read_error == 0 )
	{
		fprintf( stdout, "no\n" );
	}
	else
	{
		fprintf( stdout, "yes\n" );
	}
	fprintf( stdout, "\n" );

	/* Ask for confirmation
	 */
	user_input = get_user_input_fixed_value( "Continue acquiry with these values", yes_no, 2, 0 );

	if( strncmp( user_input, "yes", 3 ) == 0 )
	{
		input_confirmed = 1;
	}
	else if( strncmp( user_input, "no", 2 ) == 0 )
	{
		input_confirmed = 0;
	}
	else
	{
		fprintf( stderr, "ewfacquire: unsuported answer.\n" );

		exit( EXIT_FAILURE );
	}
	free( user_input );

	fprintf( stdout, "\n" );

	return( input_confirmed );
}

/* Print the status of the acquire process
 */
int8_t last_percentage = -1;

void print_percentage_callback( uint64_t bytes_read, uint64_t bytes_total )
{
	int8_t new_percentage = ( bytes_total > 0 ) ? ( ( bytes_read * 100 ) / bytes_total ) : 1;

	if( new_percentage > last_percentage )
	{
		last_percentage = new_percentage;

		fprintf( stderr, "Status: bytes read: %" PRIu64 "\tof total: %" PRIu64 " (%" PRIi8 "%%).\n", bytes_read, bytes_total, last_percentage );
	}
}

/* The main program
 */
int main( int argc, const char **argv )
{
	struct stat input_file_stat;
	struct utsname utsname_buffer;
	char *filenames[ 1 ];

	LIBEWF_HANDLE *handle               = NULL;
	LIBEWF_HEADER_VALUES *header_values = NULL;
	char *user_input                    = NULL;
	char *filename                      = NULL;
	char *case_number                   = NULL;
	char *description                   = NULL;
	char *evidence_number               = NULL;
	char *examiner_name                 = NULL;
	char *notes                         = NULL;
	void *callback                      = &print_percentage_callback;

	int file_descriptor                 = 0;
	int option                          = 0;
	int64_t count                       = 0;
	uint64_t size_input_file            = 0;
	uint64_t ewf_file_size              = 0;
	uint64_t sectors_per_block          = 0;
	uint64_t error_granularity          = 0;
	int8_t compression_level            = EWF_COMPRESSION_NONE;
	uint8_t compress_empty_block        = 0;
	uint8_t wipe_block_on_read_error    = 0;
	uint8_t ewf_format                  = LIBEWF_FORMAT_UNKNOWN;
	uint8_t read_error_retry            = 3;
	char *compression_types[ 3 ]        = { "none", "fast", "best" };
	char *format_types[ 7 ]             = { "smart", "ftk", "encase1", "encase2", "encase3", "encase4", "encase5" };
	char *sector_per_block_sizes[ 7 ]   = { "64", "128", "256", "512", "1024", "2048", "4096" };
	char *yes_no[ 2 ]                   = { "yes", "no" };

	while( ( option = getopt( argc, (char **) argv, "hqvV" ) ) > 0 )
	{
		switch( option )
		{
			case '?':
			default:
				fprintf( stderr, "Invalid argument: %s\n", argv[ optind ] );
				usage();

			case 'h':
				usage();

			case 'q':
				callback = NULL;
				break;

			case 'v':
				libewf_verbose = 1;
				break;

			case 'V':
				version();
		}
	}
	if( optind == argc )
	{
		fprintf( stderr, "Missing source file or device.\n" );
		usage();
	}
	/* Check if to read from stdin
	 */
	if( strncmp( argv[ optind ], "-", 1 ) == 0 )
	{
		fprintf( stderr, "Reading from stdin not supported.\n" );

		exit( EXIT_FAILURE );
	}
	/* Check the input file or device size
	 */
	file_descriptor = open( argv[ optind ], O_RDONLY );

	if( file_descriptor == -1 )
	{
		fprintf( stderr, "Error opening file: %s.\n", argv[ optind ] );

		exit( EXIT_FAILURE );
	}
	size_input_file = 0;

	if( fstat( file_descriptor, &input_file_stat ) == 0 )
	{
		size_input_file = input_file_stat.st_size;
	}
	if( size_input_file <= 0 )
	{
#ifdef BLKGETSIZE64
		ioctl( file_descriptor, BLKGETSIZE64, &size_input_file );
#else
#ifdef DIOCGMEDIASIZE
		ioctl( file_descriptor, DIOCGMEDIASIZE, &size_input_file );
#else
#ifdef DIOCGDINFO
		struct disklabel disk_label;

		if( ioctl( file_descriptor, DIOCGDINFO, &disk_label ) != -1 )
		{
			size_input_file = disk_label.d_secperunit * disk_label.d_secsize;
		}
#else
#ifdef DKIOCGETBLOCKCOUNT
		uint32_t block_size  = 0;
		uint64_t block_count = 0;

		ioctl( file_descriptor, DKIOCGETBLOCKSIZE, &block_size );
		ioctl( file_descriptor, DKIOCGETBLOCKCOUNT, &block_count );

		size_input_file = block_count * block_size;

#ifdef _LIBEWF_DEBUG_
		/* Debug code */
		fprintf( stderr, "block size: %" PRIu32 " block count: %" PRIu64 " ", block_size, block_count );
#endif

#else
		size_input_file = 0;
#endif
#endif
#endif
#endif

#ifdef _LIBEWF_DEBUG_
		/* Debug code */
		fprintf( stderr, "device size: %" PRIu64 "\n", size_input_file );
#endif
	}
	if( size_input_file <= 0 )
	{
		fprintf( stderr, "Unable to determine file or device size.\n" );

		exit( EXIT_FAILURE );
	}
	header_values = libewf_header_values_alloc();

	/* Determine acquiry system type
	 */
	if( uname( &utsname_buffer ) == -1 )
	{
		header_values->acquiry_operating_system = libewf_header_values_set_value( header_values->acquiry_operating_system, "Undetermined" );
	}
	else
	{
		header_values->acquiry_operating_system = libewf_header_values_set_value( header_values->acquiry_operating_system, utsname_buffer.sysname );
	}
	header_values->acquiry_software_version = libewf_header_values_set_value( header_values->acquiry_software_version, VERSION );

	/* Both time values will be generated automatically when set to NULL
	 */
	header_values->system_date      = NULL;
	header_values->acquiry_date     = NULL;
	header_values->password         = NULL;
	header_values->compression_type = NULL;

	do
	{
		if( case_number != NULL )
		{
			free( case_number );
		}
		if( description != NULL )
		{
			free( description );
		}
		if( evidence_number != NULL )
		{
			free( evidence_number );
		}
		if( examiner_name != NULL )
		{
			free( examiner_name );
		}
		if( notes != NULL )
		{
			free( notes );
		}

		/* Request the necessary case data
		 */
		fprintf( stdout, "Information about acquiry required, please provide the necessary input\n" );

		/* Output filename
		 */
		while( filename == NULL )
		{
			filename = get_user_input_variable( "Image path and filename without extension" );

			if( filename == NULL )
			{
				fprintf( stdout, "Filename is required, please try again or terminate using Ctrl^C.\n" );
			}
		}
		/* Case number
		 */
		case_number = get_user_input_variable( "Case number" );

		/* Description
		 */
		description = get_user_input_variable( "Description" );

		/* Evidence number
		 */
		evidence_number = get_user_input_variable( "Evidence number" );

		/* Examiner name
		 */
		examiner_name = get_user_input_variable( "Examiner name" );

		/* Notes
		 */
		notes = get_user_input_variable( "Notes" );

		/* Compression
		 */
		user_input = get_user_input_fixed_value( "Use compression", compression_types, 3, 0 );

		if( strncmp( user_input, "none", 4 ) == 0 )
		{
			compression_level = EWF_COMPRESSION_NONE;
		}
		else if( strncmp( user_input, "fast", 4 ) == 0 )
		{
			compression_level = EWF_COMPRESSION_FAST;
		}
		else if( strncmp( user_input, "best", 4 ) == 0 )
		{
			compression_level = EWF_COMPRESSION_BEST;
		}
		else
		{
			fprintf( stderr, "ewfacquire: unsuported compression type.\n" );

			exit( EXIT_FAILURE );
		}
		free( user_input );

		/* Empty block compression
		 */
		if( compression_level == EWF_COMPRESSION_NONE )
		{
			user_input = get_user_input_fixed_value( "Compress empty blocks", yes_no, 2, 1 );

			if( strncmp( user_input, "yes", 3 ) == 0 )
			{
				compress_empty_block = 1;
			}
			else if( strncmp( user_input, "no", 2 ) == 0 )
			{
				compress_empty_block = 0;
			}
			else
			{
				fprintf( stderr, "ewfacquire: unsuported answer.\n" );

				exit( EXIT_FAILURE );
			}
			free( user_input );
		}

		/* File format
		 */
		user_input = get_user_input_fixed_value( "Use EWF file format", format_types, 7, 6 );

		if( strncmp( user_input, "smart", 5 ) == 0 )
		{
			ewf_format = LIBEWF_FORMAT_SMART;
		}
		else if( strncmp( user_input, "ftk", 3 ) == 0 )
		{
			ewf_format = LIBEWF_FORMAT_FTK;
		}
		else if( strncmp( user_input, "encase1", 7 ) == 0 )
		{
			ewf_format = LIBEWF_FORMAT_ENCASE1;
		}
		else if( strncmp( user_input, "encase2", 7 ) == 0 )
		{
			ewf_format = LIBEWF_FORMAT_ENCASE2;
		}
		else if( strncmp( user_input, "encase3", 7 ) == 0 )
		{
			ewf_format = LIBEWF_FORMAT_ENCASE3;
		}
		else if( strncmp( user_input, "encase4", 7 ) == 0 )
		{
			ewf_format = LIBEWF_FORMAT_ENCASE4;
		}
		else if( strncmp( user_input, "encase5", 7 ) == 0 )
		{
			ewf_format = LIBEWF_FORMAT_ENCASE5;
		}
		else
		{
			fprintf( stderr, "ewfacquire: unsuported EWF file format type.\n" );

			exit( EXIT_FAILURE );
		}
		free( user_input );

		/* File size
		 */
		ewf_file_size  = get_user_input_size_variable( "Evidence file size in kbytes (2^10)", 1440, ( 2 * 1024 * 1024 ), ( 650 * 1024 ) );
		ewf_file_size *= 1024;

		/* Chunk size (sectors per block)
		 */
		user_input = get_user_input_fixed_value( "The amount of sectors to read at once", sector_per_block_sizes, 7, 0 );

		sectors_per_block = atoll( user_input );

		free( user_input );

		/* Error granularity
		 */
		error_granularity = get_user_input_size_variable( "The amount of sectors to be used as error granularity", 1, sectors_per_block, 64 );

		/* The amount of read error retry
		 */
		read_error_retry = get_user_input_size_variable( "The amount of retries when a reading error occurs", 0, 255, 3 );

		/* Wipe the sector on error
		 */
		user_input = get_user_input_fixed_value( "Wipe sectors on read error (mimic EnCase like behavior)", yes_no, 2, 0 );

		if( strncmp( user_input, "yes", 3 ) == 0 )
		{
			wipe_block_on_read_error = 1;
		}
		else if( strncmp( user_input, "no", 2 ) == 0 )
		{
			wipe_block_on_read_error = 0;
		}
		else
		{
			fprintf( stderr, "ewfacquire: unsuported answer.\n" );

			exit( EXIT_FAILURE );
		}
		free( user_input );

		fprintf( stdout, "\n" );
	}
	/* Check if user is content with values
	 */
	while( confirm_input( filename, case_number, description, evidence_number, examiner_name, notes, compression_level, compress_empty_block, ewf_format, ewf_file_size, sectors_per_block, error_granularity, read_error_retry, wipe_block_on_read_error ) == 0 );

	if( case_number != NULL )
	{
		header_values->case_number = libewf_header_values_set_value( header_values->case_number, case_number );

		free( case_number );
	}
	if( description != NULL )
	{
		header_values->description = libewf_header_values_set_value( header_values->description, description );

		free( description );
	}
	if( evidence_number != NULL )
	{
		header_values->evidence_number = libewf_header_values_set_value( header_values->evidence_number, evidence_number );

		free( evidence_number );
	}
	if( examiner_name != NULL )
	{
		header_values->examiner_name = libewf_header_values_set_value( header_values->examiner_name, examiner_name );

		free( examiner_name );
	}
	if( notes != NULL )
	{
		header_values->notes = libewf_header_values_set_value( header_values->notes, notes );

		free( notes );
	}
	/* Done asking user input
	 */
	fprintf( stdout, "\nStarting acquiry, this could take a while.\n" );

	filenames[ 0 ] = filename;

	handle = libewf_open( (const char **) filenames, 1, LIBEWF_OPEN_WRITE );
	handle = libewf_set_write_parameters( handle, size_input_file, sectors_per_block, 512, error_granularity, ewf_file_size, compression_level, ewf_format, header_values, read_error_retry, wipe_block_on_read_error, compress_empty_block );

	count = libewf_write_from_file_descriptor( handle, file_descriptor, callback );

	libewf_close( handle );

	close( file_descriptor );

	fprintf( stderr, "Success: bytes written: %" PRIi64 "\n", count );

	free( filename );

	libewf_header_values_free( header_values );

	return( 0 );
}

