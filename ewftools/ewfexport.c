/*
 * ewfexport
 * Export media data from EWF files to a file
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
#include <memory.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_FCNTL_H )
#include <fcntl.h>
#endif

#if defined( HAVE_IO_H )
#include <io.h>
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

#include "character_string.h"
#include "byte_size_string.h"
#include "digest_context.h"
#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewfsignal.h"
#include "ewfstring.h"
#include "glob.h"
#include "md5.h"
#include "process_status.h"
#include "sha1.h"
#include "system_string.h"

#define EWFEXPORT_INPUT_BUFFER_SIZE	64

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
	result = byte_size_string_create(
	          default_segment_file_size_string,
	          16,
	          EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE );

	if( result == 1 )
	{
		result = byte_size_string_create(
			  minimum_segment_file_size_string,
			  16,
			  EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE );
	}
	if( result == 1 )
	{
		result = byte_size_string_create(
			  maximum_32bit_segment_file_size_string,
			  16,
			  EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE );
	}
	if( result == 1 )
	{
		result = byte_size_string_create(
			  maximum_64bit_segment_file_size_string,
			  16,
			  EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
			  BYTE_SIZE_STRING_UNIT_MEBIBYTE );
	}
	fprintf( stream, "Usage: ewfexport [ -b amount_of_sectors ] [ -B amount_of_bytes ]\n"
	                 "                 [ -c compression_type ] [ -d digest_type ] [ -f format ]\n"
	                 "                 [ -l log_filename ] [ -o offset ] [ -p process_buffer_size ]\n"
	                 "                 [ -S segment_file_size ] [ -t target ] [ -hqsuvVw ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-b:        specify the amount of sectors to read at once (per chunk), options:\n"
	                 "\t           64 (default), 128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n"
	                 "\t           (not used for raw format)\n" );
	fprintf( stream, "\t-B:        specify the amount of bytes to export (default is all bytes)\n" );
	fprintf( stream, "\t-c:        specify the compression type, options: none (default), empty-block,\n"
	                 "\t           fast or best (not used for raw format)\n" );
	fprintf( stream, "\t-d:        calculate additional digest (hash) types besides md5, options: sha1\n"
	                 "\t           (not used for raw format)\n" );
	fprintf( stream, "\t-f:        specify the file format to write to, options: raw (default), ewf,\n"
	                 "\t           smart, encase1, encase2, encase3, encase4, encase5, encase6, linen5,\n"
	                 "\t           linen6, ewfx\n" );
	fprintf( stream, "\t-h:        shows this help\n" );
	fprintf( stream, "\t-l:        logs verification errors and the digest (hash) to the log_filename\n" );
	fprintf( stream, "\t-o:        specify the offset to start the export (default is 0)\n" );
	fprintf( stream, "\t-p:        specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-q:        quiet shows no status information\n" );
	fprintf( stream, "\t-s:        swap byte pairs of the media data (from AB to BA)\n"
	                 "\t           (use this for big to little endian conversion and vice versa)\n" );

	if( result == 1 )
	{
		fprintf( stream, "\t-S:        specify the segment file size in bytes (default is %" PRIs ")\n"
		                 "\t           (minimum is %" PRIs ", maximum is %" PRIs " for encase6 format\n"
		                 "\t           and %" PRIs " for other formats)\n"
		                 "\t           (not used for raw format)\n",
		 default_segment_file_size_string,
		 minimum_segment_file_size_string,
		 maximum_64bit_segment_file_size_string,
		 maximum_32bit_segment_file_size_string );
	}
	else
	{
		fprintf( stream, "\t-S:        specify the segment file size in bytes (default is %" PRIu32 ")\n"
		                 "\t           (minimum is %" PRIu32 ", maximum is %" PRIu64 " for encase6 format\n"
		                 "\t           and %" PRIu32 " for other formats)\n"
		                 "\t           (not used for raw format)\n",
		 (uint32_t) EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
		 (uint32_t) EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
		 (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
		 (uint32_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT );
	}

	fprintf( stream, "\t-t:        specify the target file to export to, use - for stdout (default is\n"
	                 "\t           export) stdout is only supported for the raw format\n" );
	fprintf( stream, "\t-u:        unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:        verbose output to stderr\n" );
	fprintf( stream, "\t-V:        print version\n" );
	fprintf( stream, "\t-w:        wipe sectors on CRC error (mimic EnCase like behavior)\n" );
}

/* The main program
 */
#if defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	character_t acquiry_operating_system[ 32 ];
	character_t input_buffer[ EWFEXPORT_INPUT_BUFFER_SIZE ];

#if !defined( HAVE_GLOB_H )
	glob_t *glob                            = NULL;
#endif
	system_character_t *filenames[ 1 ]         = { NULL };

	libewf_handle_t *export_handle             = NULL;
	character_t *acquiry_software_version      = NULL;
	character_t *calculated_md5_hash_string    = NULL;
	character_t *calculated_sha1_hash_string   = NULL;
	character_t *fixed_string_variable         = NULL;
	character_t *program                       = _CHARACTER_T_STRING( "ewfexport" );

	system_character_t * const *argv_filenames = NULL;
	system_character_t **ewf_filenames         = NULL;
	system_character_t *log_filename           = NULL;
	system_character_t *option_target_filename = NULL;
	system_character_t *target_filename        = NULL;

	FILE *log_file_stream                      = NULL;
	void *callback                             = &process_status_update;

	system_integer_t option                    = 0;
	size64_t media_size                        = 0;
	ssize64_t export_count                     = 0;
	size_t string_length                       = 0;
	uint64_t export_offset                     = 0;
	uint64_t export_size                       = 0;
	uint64_t maximum_segment_file_size         = 0;
	uint64_t process_buffer_size               = 0;
	uint64_t segment_file_size                 = 0;
	uint32_t amount_of_crc_errors              = 0;
	uint32_t sectors_per_chunk                 = 64;
	uint8_t calculate_md5                      = 1;
	uint8_t calculate_sha1                     = 0;
	uint8_t compress_empty_block               = 0;
	uint8_t libewf_format                      = LIBEWF_FORMAT_ENCASE5;
	uint8_t swap_byte_pairs                    = 0;
	uint8_t wipe_chunk_on_error                = 0;
	uint8_t verbose                            = 0;
	int8_t compression_level                   = LIBEWF_COMPRESSION_NONE;
	int amount_of_filenames                    = 0;
	int argument_set_compression               = 0;
	int argument_set_format                    = 0;
	int argument_set_offset                    = 0;
	int argument_set_sectors_per_chunk         = 0;
	int argument_set_segment_file_size         = 0;
	int argument_set_size                      = 0;
	int error_abort                            = 0;
	int interactive_mode                       = 1;
	int output_raw                             = 1;
	int result                                 = 1;
	int status                                 = 0;

	character_t *ewfexport_format_types[ 13 ] = \
	 { _CHARACTER_T_STRING( "raw" ),
	   _CHARACTER_T_STRING( "ewf" ),
	   _CHARACTER_T_STRING( "smart" ),
	   _CHARACTER_T_STRING( "ftk" ),
	   _CHARACTER_T_STRING( "encase1" ),
	   _CHARACTER_T_STRING( "encase2" ),
	   _CHARACTER_T_STRING( "encase3" ),
	   _CHARACTER_T_STRING( "encase4" ),
	   _CHARACTER_T_STRING( "encase5" ),
	   _CHARACTER_T_STRING( "encase6" ),
	   _CHARACTER_T_STRING( "linen5" ),
	   _CHARACTER_T_STRING( "linen6" ),
	   _CHARACTER_T_STRING( "ewfx" ) };

	ewfoutput_version_fprint(
	 stderr,
	 program );
#if defined( WINAPI )
	if( _setmode(
	     _fileno(
	      stdout ),
	     _O_BINARY ) == -1 )
	{
		fprintf( stderr, "Unable to set stdout to binary mode.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
#endif

	while( ( option = ewfgetopt(
	                   argc,
	                   argv,
	                   _SYSTEM_CHARACTER_T_STRING( "b:B:c:d:f:hl:o:p:qsS:t:uvVw" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf( stderr, "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind ] );

				usage_fprint(
				 stderr );

				return( EXIT_FAILURE );

			case (system_integer_t) 'b':
				if( ewfinput_determine_sectors_per_chunk_system_character(
				     optarg,
				     &sectors_per_chunk ) != 1 )
				{
					fprintf( stderr, "Unsupported amount of sectors per chunk defaulting to: 64.\n" );

					sectors_per_chunk = 64;
				}
				else
				{
					argument_set_sectors_per_chunk = 1;
				}
				break;

			case (system_integer_t) 'B':
				string_length = system_string_length(
				                 optarg );

				export_size = system_string_to_uint64(
				               optarg,
				               string_length );

				argument_set_size = 1;

				break;

			case (system_integer_t) 'c':
				if( ewfinput_determine_compression_level_system_character(
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

			case (system_integer_t) 'f':
				if( system_string_compare(
				     optarg,
				     _SYSTEM_CHARACTER_T_STRING( "raw" ),
				     3 ) == 0 )
				{
					output_raw          = 1;
					argument_set_format = 1;
				}
				else if( ewfinput_determine_libewf_format_system_character(
				          optarg,
				          &libewf_format ) != 1 )
				{
					fprintf( stderr, "Unsupported file format type defaulting to: raw.\n" );

					output_raw = 1;
				}
				else
				{
					output_raw          = 0;
					argument_set_format = 1;
				}
				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stderr );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'o':
				string_length = system_string_length(
				                 optarg );

				export_offset = system_string_to_int64(
				                 optarg,
				                 string_length );

				argument_set_offset = 1;

				break;

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
				callback = NULL;

				break;

			case (system_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (system_integer_t) 'S':
				string_length = system_string_length(
				                 optarg );

				result = byte_size_string_convert_system_character(
				          optarg,
				          string_length,
				          &segment_file_size );

				argument_set_segment_file_size = 1;

				if( ( result != 1 )
				 || ( segment_file_size < EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE )
				 || ( ( libewf_format == LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (int64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT ) )
				 || ( ( libewf_format != LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (int64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT ) ) )
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
				 stderr );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'w':
				wipe_chunk_on_error = 1;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf( stderr, "Missing EWF image file(s).\n" );

		usage_fprint(
		 stderr );

		return( EXIT_FAILURE );
	}
	libewf_set_notify_values(
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
	                           LIBEWF_OPEN_READ );
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
	if( ewfcommon_abort == 0 )
	{
		if( libewf_get_media_size(
		     ewfcommon_libewf_handle,
		     &media_size ) != 1 )
		{
			fprintf( stderr, "Unable to determine media size.\n" );

			libewf_close(
			 ewfcommon_libewf_handle );

			return( EXIT_FAILURE );
		}
		if( ( export_size == 0 )
		 || ( export_size > ( media_size - export_offset ) ) )
		{
			export_size = media_size - export_offset;
		}
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
				fprintf( stderr, "Unable to create target filename.\n" );

				error_abort = 1;
			}
			else if( system_string_copy(
				  target_filename,
				  option_target_filename,
				  string_length ) == NULL )
			{
				fprintf( stderr, "Unable to set target filename.\n" );

				error_abort = 1;
			}
			target_filename[ string_length ] = 0;
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
			if( system_string_copy(
			     target_filename,
			     _SYSTEM_CHARACTER_T_STRING( "export" ),
			     7 ) == NULL )
			{
				fprintf( stderr, "Unable to set target filename string.\n" );

				error_abort = 1;
			}
			target_filename[ 7 ] = 0;
		}
	}
	if( error_abort != 0 )
	{
		if( target_filename != NULL )
		{
			memory_free(
			 target_filename );
		}
		return( EXIT_FAILURE );
	}
	/* Request the necessary case data
	 */
	if( ( ewfcommon_abort == 0 )
	 && ( interactive_mode == 1 ) )
	{
		if( ewfsignal_detach() != 1 )
		{
			fprintf( stderr, "Unable to detach signal handler.\n" );
		}
		fprintf( stderr, "Information for export required, please provide the necessary input\n" );

		/* File format
		 */
		if( argument_set_format == 0 )
		{
			if( ewfinput_get_fixed_string_variable(
			     stderr,
			     input_buffer,
			     EWFEXPORT_INPUT_BUFFER_SIZE,
			     _CHARACTER_T_STRING( "Export to file format" ),
			     ewfexport_format_types,
			     13,
			     0,
			     &fixed_string_variable ) == -1 )
			{
				fprintf( stderr, "Unable to determine file format defaulting to: raw.\n" );

				output_raw = 1;
			}
			else if( string_compare(
			          fixed_string_variable,
			          _CHARACTER_T_STRING( "raw" ),
			          3 ) == 0 )
			{
				output_raw = 1;
			}
			else if( ewfinput_determine_libewf_format(
			          fixed_string_variable,
			          &libewf_format ) != 1 )
			{
				fprintf( stderr, "Unsupported file format defaulting to: raw.\n" );

				output_raw = 1;
			}
			else
			{
				output_raw = 0;
			}
		}
		if( output_raw == 0 )
		{
			/* Target filename
			 */
			if( option_target_filename == NULL )
			{
				while( ewfinput_get_string_variable_system_character(
					stderr,
					_CHARACTER_T_STRING( "Target path and filename without extension" ),
				        target_filename,
				        1024 ) != 1 )
				{
					fprintf( stderr, "Filename is required, please try again or terminate using Ctrl^C.\n" );
				}
			}
			/* Compression
			 */
			if( argument_set_compression == 0 )
			{
				if( ewfinput_get_fixed_string_variable(
				     stderr,
				     input_buffer,
				     EWFEXPORT_INPUT_BUFFER_SIZE,
				     _CHARACTER_T_STRING( "Use compression" ),
				     ewfinput_compression_levels,
				     EWFINPUT_COMPRESSION_LEVELS_AMOUNT,
				     EWFINPUT_COMPRESSION_LEVELS_DEFAULT,
				     &fixed_string_variable ) == -1 )
				{
					fprintf( stderr, "Unable to determine compression type defaulting to: none.\n" );

					compression_level    = LIBEWF_COMPRESSION_NONE;
					compress_empty_block = 0;
				}
				else if( ewfinput_determine_compression_level(
				          fixed_string_variable,
				          &compression_level,
				          &compress_empty_block ) != 1 )
				{
					fprintf( stderr, "Unsupported compression type defaulting to: none.\n" );

					compression_level    = LIBEWF_COMPRESSION_NONE;
					compress_empty_block = 0;
				}
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
				     stderr,
				     input_buffer,
				     EWFEXPORT_INPUT_BUFFER_SIZE,
				     _CHARACTER_T_STRING( "Evidence segment file size in bytes" ),
				     EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
				     maximum_segment_file_size,
				     EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
				     &segment_file_size ) == -1 )
				{
					segment_file_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

					fprintf( stderr, "Unable to determine segment file size defaulting to: %" PRIu64 ".\n",
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
				     stderr,
				     input_buffer,
				     EWFEXPORT_INPUT_BUFFER_SIZE,
				     _CHARACTER_T_STRING( "The amount of sectors to read at once" ),
				     ewfinput_sector_per_block_sizes,
				     EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT,
				     EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT,
				     &fixed_string_variable ) == -1 )
				{
					fprintf( stderr, "Unable to determine sectors per chunk on error defaulting to: 64.\n" );

					sectors_per_chunk = 64;
				}
				else if( ewfinput_determine_sectors_per_chunk(
				          fixed_string_variable,
				          &sectors_per_chunk ) != 1 )
				{
					fprintf( stderr, "Unsupported sectors per chunk on error defaulting to: 64.\n" );

					sectors_per_chunk = 64;
				}
			}
		}
		else
		{
			/* Target filename
			 */
			if( option_target_filename == NULL )
			{
				while( ewfinput_get_string_variable_system_character(
					stderr,
					_CHARACTER_T_STRING( "Target path and filename with extension or - for stdout" ),
					target_filename,
					1024 ) != 1 )
				{
					fprintf( stderr, "Filename is required, please try again or terminate using Ctrl^C.\n" );
				}
			}
		}
                /* Offset of data to export
		 */
		if( ( argument_set_offset == 0 )
		 && ( ewfinput_get_size_variable(
		       stderr,
		       input_buffer,
		       EWFEXPORT_INPUT_BUFFER_SIZE,
		       _CHARACTER_T_STRING( "Start export at offset" ),
		       0,
		       media_size,
		       export_offset,
		       &export_offset ) == -1 ) )
		{
			export_offset = 0;

			fprintf( stderr, "Unable to determine export offset defaulting to: %" PRIu64 ".\n",
			 export_offset );
		}
                /* Size of data to export
		 */
		if( ( argument_set_size == 0 )
		 && ( ewfinput_get_size_variable(
		       stderr,
		       input_buffer,
		       EWFEXPORT_INPUT_BUFFER_SIZE,
		       _CHARACTER_T_STRING( "Amount of bytes to export" ),
		       0,
		       ( media_size - export_offset ),
		       export_size,
		       &export_size ) == -1 ) )
		{
			export_size = media_size - export_offset;

			fprintf( stderr, "Unable to determine export size defaulting to: %" PRIu64 ".\n",
			 export_size );
		}
		if( ewfsignal_attach(
		     ewfcommon_signal_handler ) != 1 )
		{
			fprintf( stderr, "Unable to attach signal handler.\n" );
		}
	}
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (character_t *) memory_allocate(
		                                              sizeof( character_t )* EWFSTRING_DIGEST_HASH_LENGTH_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated MD5 hash string.\n" );

			libewf_close(
			 ewfcommon_libewf_handle );

			return( EXIT_FAILURE );
		}
	}
	if( calculate_sha1 == 1 )
	{
		calculated_sha1_hash_string = (character_t *) memory_allocate(
		                                               sizeof( character_t )* EWFSTRING_DIGEST_HASH_LENGTH_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated SHA1 hash string.\n" );

			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
			libewf_close(
			 ewfcommon_libewf_handle );

			return( EXIT_FAILURE );
		}
	}
	if( ewfcommon_abort == 0 )
	{
		fprintf( stderr, "\n" );

		if( process_status_initialize(
		     &process_status,
		     _CHARACTER_T_STRING( "Export" ),
		     _CHARACTER_T_STRING( "exported" ),
		     _CHARACTER_T_STRING( "Written" ),
		     stderr ) != 1 )
		{
			fprintf( stderr, "Unable to initialize process status.\n" );

			memory_free(
			 target_filename );

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
		if( process_status_start(
		     process_status ) != 1 )
		{
			fprintf( stderr, "Unable to start process status.\n" );

			process_status_free(
			 &process_status );

			memory_free(
			 target_filename );

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
		/* Start exporting data
		 */
		if( output_raw == 0 )
		{
			filenames[ 0 ] = target_filename;

			export_handle = libewf_open(
					 (system_character_t * const *) filenames,
					 1,
					 LIBEWF_OPEN_WRITE );

			memory_free(
			 target_filename );

			if( export_handle == NULL )
			{
				ewfoutput_error_fprint(
				 stderr, "Unable to open export EWF file(s)" );

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
				libewf_close(
				 ewfcommon_libewf_handle );

				return( EXIT_FAILURE );
			}
			if( ewfcommon_determine_operating_system_string(
			     acquiry_operating_system,
			     32 ) != 1 )
			{
				fprintf( stderr, "Unable to determine operating system string.\n" );

				acquiry_operating_system[ 0 ] = 0;
			}
			acquiry_software_version = _CHARACTER_T_STRING( LIBEWF_VERSION_STRING );

			export_count = ewfcommon_export_ewf(
			                ewfcommon_libewf_handle,
			                export_handle,
			                compression_level,
			                (uint8_t) compress_empty_block,
			                libewf_format,
			                segment_file_size,
			                export_size,
			                export_offset,
			                sectors_per_chunk,
			                calculate_md5,
			                calculated_md5_hash_string,
			                EWFSTRING_DIGEST_HASH_LENGTH_MD5,
			                calculate_sha1,
			                calculated_sha1_hash_string,
			                EWFSTRING_DIGEST_HASH_LENGTH_SHA1,
			                swap_byte_pairs,
			                wipe_chunk_on_error,
			                (size_t) process_buffer_size,
			                acquiry_operating_system,
			                program,
			                acquiry_software_version,
			                callback );

			if( libewf_close(
			     export_handle ) != 0 )
			{
				fprintf( stderr, "Unable to close export EWF file handle.\n" );

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
				libewf_close(
				 ewfcommon_libewf_handle );

				return( EXIT_FAILURE );
			}
		}
		else
		{
			export_count = ewfcommon_export_raw(
			                ewfcommon_libewf_handle,
			                target_filename,
			                export_size,
			                export_offset,
			                calculate_md5,
			                calculated_md5_hash_string,
			                EWFSTRING_DIGEST_HASH_LENGTH_MD5,
			                calculate_sha1,
			                calculated_sha1_hash_string,
			                EWFSTRING_DIGEST_HASH_LENGTH_SHA1,
			                swap_byte_pairs,
			                wipe_chunk_on_error,
			                (size_t) process_buffer_size,
			                callback );

			memory_free(
			 target_filename );
		}
		if( export_count <= -1 )
		{
			status = PROCESS_STATUS_FAILED;
		}
		else
		{
			status = PROCESS_STATUS_COMPLETED;
		}
	}
	if( ewfcommon_abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     process_status,
	     (size64_t) export_count,
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
		libewf_close(
		 ewfcommon_libewf_handle );

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
		libewf_close(
		 ewfcommon_libewf_handle );

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
		ewfoutput_crc_errors_fprint(
		 stderr,
		 ewfcommon_libewf_handle,
		 &amount_of_crc_errors );

		if( log_file_stream != NULL )
		{
			ewfoutput_crc_errors_fprint(
			 log_file_stream,
			 ewfcommon_libewf_handle,
			 &amount_of_crc_errors );
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
		fprintf( stderr, "MD5 hash calculated over data:\t%" PRIs "\n",
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
		fprintf( stderr, "SHA1 hash calculated over data:\t%" PRIs "\n",
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

