/*
 * ewfexport
 * Export media data from EWF files to a file
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

#include "../libewf/libewf_char.h"
#include "../libewf/libewf_common.h"
#include "../libewf/libewf_string.h"

#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfglob.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewfsignal.h"
#include "ewfstring.h"

/* Prints the executable usage information
 */
void usage( void )
{
	fprintf( stderr, "Usage: ewfexport [ -b amount_of_sectors ] [ -B amount_of_bytes ] [ -c compression_type ] [ -d digest_type ] [ -f format ]\n" );
	fprintf( stderr, "                 [ -o offset ] [ -S segment_file_size ] [ -t target ] [ -hsquvVw ] ewf_files\n\n" );

	fprintf( stderr, "\t-b: specify the amount of sectors to read at once (per chunk), options: 64 (default),\n" );
	fprintf( stderr, "\t    128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n" );
	fprintf( stdout, "\t    (not used for raw format)\n" );
	fprintf( stderr, "\t-B: specify the amount of bytes to export (default is all bytes)\n" );
	fprintf( stderr, "\t-c: specify the compression type, options: none (is default), empty_block, fast, best\n" );
	fprintf( stdout, "\t    (not used for raw format)\n" );
	fprintf( stdout, "\t-d: calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stdout, "\t    (not used for raw format)\n" );
	fprintf( stderr, "\t-f: specify the file format to write to, options: raw (default), ewf, smart,\n" );
	fprintf( stderr, "\t    encase1, encase2, encase3, encase4, encase5, encase6, linen5, linen6, ewfx\n" );
	fprintf( stderr, "\t-h: shows this help\n" );
	fprintf( stderr, "\t-q: quiet shows no status information\n" );
	fprintf( stderr, "\t-o: specify the offset to start the export (default is 0)\n" );
	fprintf( stderr, "\t-s: swap byte pairs of the media data (from AB to BA)\n" );
	fprintf( stderr, "\t    (use this for big to little endian conversion and vice versa)\n" );
	fprintf( stderr, "\t-t: specify the target file to export to, use - for stdout (default is export)\n" );
	fprintf( stderr, "\t    stdout is only supported for the raw format\n" );
	fprintf( stdout, "\t-S: specify the segment file size in kibibytes (KiB) (default is %" PRIu32 ")\n",
	 (uint32_t) ( EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE / 1024 ) );
	fprintf( stdout, "\t    (minimum is %" PRIu32 ", maximum is %" PRIu64 " for encase6 format and %" PRIu32 " for other EWF formats)\n",
	 (uint32_t) ( EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE / 1024 ),
	 (uint64_t) ( EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT / 1024 ),
	 (uint32_t) ( EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT / 1024 ) );
	fprintf( stdout, "\t    (not used for raw format)\n" );
	fprintf( stderr, "\t-u: unattended mode (disables user interaction)\n" );
	fprintf( stderr, "\t-v: verbose output to stderr\n" );
	fprintf( stderr, "\t-V: print version\n" );
	fprintf( stdout, "\t-w: wipe sectors on CRC error (mimic EnCase like behavior)\n" );
}

/* The main program
 */
#if defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if !defined( HAVE_GLOB_H )
	EWFGLOB *glob                      = NULL;
	int32_t glob_count                 = 0;
#endif
	CHAR_T *filenames[ 1 ]             = { NULL };

	LIBEWF_HANDLE *handle              = NULL;
	LIBEWF_HANDLE *export_handle       = NULL;
	LIBEWF_CHAR *user_input            = NULL;
	LIBEWF_CHAR *program               = _S_LIBEWF_CHAR( "ewfexport" );

	CHAR_T *end_of_string              = NULL;
	CHAR_T *target_filename            = NULL;
	CHAR_T *time_string                = NULL;
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
        CHAR_T *error_string               = NULL;
#endif
	void *callback                     = &ewfoutput_process_status_fprint;
	INT_T option                       = 0;
	size64_t media_size                = 0;
	size_t string_length               = 0;
	time_t timestamp_start             = 0;
	time_t timestamp_end               = 0;
	uint64_t maximum_segment_file_size = 0;
	uint64_t segment_file_size         = 0;
	uint64_t export_offset             = 0;
	uint64_t export_size               = 0;
	uint64_t sectors_per_chunk         = 64;
	int64_t count                      = 0;
	uint8_t libewf_format              = LIBEWF_FORMAT_ENCASE5;
	uint8_t swap_byte_pairs            = 0;
	uint8_t wipe_chunk_on_error        = 0;
	uint8_t verbose                    = 0;
	int8_t compression_level           = LIBEWF_COMPRESSION_NONE;
	int8_t compress_empty_block        = 0;
	int argument_set_compression       = 0;
	int argument_set_format            = 0;
	int argument_set_offset            = 0;
	int argument_set_sectors_per_chunk = 0;
	int argument_set_segment_file_size = 0;
	int argument_set_size              = 0;
	int interactive_mode               = 1;
	uint8_t calculate_md5              = 1;
	uint8_t calculate_sha1             = 0;
	int output_raw                     = 1;

	LIBEWF_CHAR *ewfexport_format_types[ 13 ] = \
	 { _S_LIBEWF_CHAR( "raw" ),
	   _S_LIBEWF_CHAR( "ewf" ),
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

	ewfsignal_initialize();

	ewfoutput_version_fprint( stderr, program );

	while( ( option = ewfgetopt( argc, argv, _S_CHAR_T( "b:B:c:d:f:ho:qsS:t:uvVw" ) ) ) != (INT_T) -1 )
	{
		switch( option )
		{
			case (INT_T) '?':
			default:
				fprintf( stderr, "Invalid argument: %" PRIs ".\n", argv[ optind ] );

				usage();

				return( EXIT_FAILURE );

			case (INT_T) 'b':
				sectors_per_chunk = ewfinput_determine_sectors_per_chunk_char_t( optarg );

				if( sectors_per_chunk == 0 )
				{
					fprintf( stderr, "Unsupported amount of sectors per chunk defaulting to 64.\n" );

					sectors_per_chunk = 64;
				}
				break;

			case (INT_T) 'B':
				string_length = CHAR_T_LENGTH( optarg );
				end_of_string = &optarg[ string_length - 1 ];
				export_size   = CHAR_T_TOLONG( optarg, &end_of_string, 0 );

				break;

			case (INT_T) 'c':
				if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "empty_block" ), 11 ) == 0 )
				{
					compress_empty_block     = 1;
					argument_set_compression = 1;
				}
				else
				{
					compression_level = ewfinput_determine_compression_level_char_t( optarg );
				
					if( compression_level <= -1 )
					{
						fprintf( stderr, "Unsupported compression type defaulting to none.\n" );

						compression_level = LIBEWF_COMPRESSION_NONE;
					}
					else
					{
						argument_set_compression = 1;
					}
				}
				break;

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

			case (INT_T) 'f':
				if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "raw" ), 3 ) == 0 )
				{
					output_raw          = 1;
					argument_set_format = 1;
				}
				else
				{
					libewf_format = ewfinput_determine_libewf_format_char_t( optarg );

					if( libewf_format == 0 )
					{
						fprintf( stderr, "Unsupported file format type defaulting to raw.\n" );
					}
					else
					{
						output_raw          = 0;
						argument_set_format = 1;
					}
				}
				break;

			case (INT_T) 'h':
				usage();

				return( EXIT_SUCCESS );

			case (INT_T) 'o':
				string_length       = CHAR_T_LENGTH( optarg );
				end_of_string       = &optarg[ string_length - 1 ];
				export_offset       = CHAR_T_TOLONG( optarg, &end_of_string, 0 );
				argument_set_offset = 1;

				break;

			case (INT_T) 'q':
				callback = NULL;

				break;

			case (INT_T) 's':
				swap_byte_pairs = 1;

				break;

			case (INT_T) 'S':
				string_length      = CHAR_T_LENGTH( optarg );
				end_of_string      = &optarg[ string_length - 1 ];
				segment_file_size  = (uint64_t) CHAR_T_TOLONG( optarg, &end_of_string, 0 );
				argument_set_size  = 1;
				segment_file_size *= 1024;

				if( ( segment_file_size < EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE )
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

			case (INT_T) 't':
				target_filename = CHAR_T_DUPLICATE( optarg );

				break;

			case (INT_T) 'u':
				interactive_mode = 0;

				break;

			case (INT_T) 'v':
				verbose = 1;

				break;

			case (INT_T) 'V':
				ewfoutput_copyright_fprint( stderr );

				return( EXIT_SUCCESS );

			case (INT_T) 'w':
				wipe_chunk_on_error = 1;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf( stderr, "Missing EWF image file(s).\n" );

		usage();

		libewf_common_free( target_filename );

		return( EXIT_FAILURE );
	}
	libewf_set_notify_values( stderr, verbose );

#if !defined( HAVE_GLOB_H )
	glob = ewfglob_alloc();

	if( glob == NULL )
	{
		fprintf( stderr, "Unable to create glob.\n" );

		libewf_common_free( target_filename );

		return( EXIT_FAILURE );
	}
	glob_count = ewfglob_resolve( glob, &argv[ optind ], ( argc - optind ) );

	if( glob_count <= 0 )
	{
		fprintf( stderr, "Unable to resolve glob.\n" );

		ewfglob_free( glob );
		libewf_common_free( target_filename );

		return( EXIT_FAILURE );
	}
	handle = libewf_open( glob->results, glob->amount, LIBEWF_OPEN_READ );

	ewfglob_free( glob );
#else
	handle = libewf_open( &argv[ optind ], ( argc - optind ), LIBEWF_OPEN_READ );
#endif

	if( handle == NULL )
	{
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
		if( errno != 0 )
		{
			error_string = ewfstring_strerror( errno );
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

		libewf_common_free( target_filename );

		return( EXIT_FAILURE );
	}
	if( libewf_get_media_size( handle, &media_size ) != 1 )
	{
		fprintf( stderr, "Unable to determine media size.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		libewf_common_free( target_filename );

		return( EXIT_FAILURE );
	}
	if( export_size == 0 )
	{
		export_size = media_size - export_offset;
	}
	/* Request the necessary case data
	 */
	if( interactive_mode == 1 )
	{
		fprintf( stderr, "Information for export required, please provide the necessary input\n" );

		if( argument_set_format == 0 )
		{
			/* File format
			 */
			user_input = ewfinput_get_fixed_value(
				      stderr,
				      _S_LIBEWF_CHAR( "Export to file format" ),
				      ewfexport_format_types,
				      13,
				      0 );

			if( libewf_string_compare( user_input, _S_LIBEWF_CHAR( "raw" ), 3 ) == 0 )
			{
				output_raw = 1;
			}
			else
			{
				libewf_format = ewfinput_determine_libewf_format( user_input );

				if( libewf_format == 0 )
				{
					fprintf( stderr, "Unsupported file format.\n" );

					if( libewf_close( handle ) != 0 )
					{
						fprintf( stderr, "Unable to close EWF file(s).\n" );
					}
					libewf_common_free( target_filename );
					libewf_common_free( user_input );

					exit( EXIT_FAILURE );
				}
				else
				{
					output_raw = 0;
				}
			}
			libewf_common_free( user_input );
		}
		if( output_raw == 0 )
		{
			/* Target filename
			 */
			while( target_filename == NULL )
			{
				target_filename = ewfinput_get_variable_char_t(
				                   stderr,
				                   _S_LIBEWF_CHAR( "Target path and filename without extension" ) );

				if( target_filename == NULL )
				{
					fprintf( stderr, "Filename is required, please try again or terminate using Ctrl^C.\n" );
				}
			}
			if( argument_set_compression == 0 )
			{
				/* Compression
				 */
				user_input = ewfinput_get_fixed_value(
					      stderr,
					      _S_LIBEWF_CHAR( "Use compression" ),
					      ewfinput_compression_levels,
					      EWFINPUT_COMPRESSION_LEVELS_AMOUNT,
					      EWFINPUT_COMPRESSION_LEVELS_DEFAULT );

				compression_level = ewfinput_determine_compression_level( user_input );

				if( compression_level <= -1 )
				{
					fprintf( stderr, "Unsupported compression type.\n" );

					if( libewf_close( handle ) != 0 )
					{
						fprintf( stderr, "Unable to close EWF file(s).\n" );
					}
					libewf_common_free( target_filename );

					return( EXIT_FAILURE );
				}
				libewf_common_free( user_input );

				/* Empty block compression
				 */
				if( compression_level == LIBEWF_COMPRESSION_NONE )
				{
					user_input = ewfinput_get_fixed_value(
						      stderr,
						      _S_LIBEWF_CHAR( "Compress empty blocks" ),
						      ewfinput_yes_no,
						      2,
						      1 );

					compress_empty_block = ewfinput_determine_yes_no( user_input );

					libewf_common_free( user_input );

					if( compress_empty_block <= -1 )
					{
						fprintf( stderr, "Unsupported answer.\n" );

						if( libewf_close( handle ) != 0 )
						{
							fprintf( stderr, "Unable to close EWF file(s).\n" );
						}
						libewf_common_free( target_filename );

						return( EXIT_FAILURE );
					}
				}
			}
			if( argument_set_segment_file_size == 0 )
			{
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
				/* Segment file size
				 */
				segment_file_size = ewfinput_get_size_variable(
						     stderr,
						     _S_LIBEWF_CHAR( "Evidence segment file size in kibitytes (KiB)" ),
						     1440,
						     ( 2 * 1024 * 1024 ),
						     EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE );

				segment_file_size *= 1024;

				/* Make sure the segment file size is smaller than or equal to the maximum
				 */
				if( segment_file_size > maximum_segment_file_size )
				{
					segment_file_size = maximum_segment_file_size;
				}
			}
			if( argument_set_sectors_per_chunk == 0 )
			{
				/* Chunk size (sectors per block)
				 */
				user_input = ewfinput_get_fixed_value(
					      stderr,
					      _S_LIBEWF_CHAR( "The amount of sectors to read at once" ),
					      ewfinput_sector_per_block_sizes,
					      EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT,
					      EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT );

				sectors_per_chunk = libewf_string_to_int64( user_input, libewf_string_length( user_input ) );

				libewf_common_free( user_input );
			}
		}
		else
		{
			/* Target filename
			 */
			while( target_filename == NULL )
			{
				target_filename = ewfinput_get_variable_char_t(
				                   stderr,
				                   _S_LIBEWF_CHAR( "Target path and filename with extension or - for stdout" ) );

				if( target_filename == NULL )
				{
					fprintf( stderr, "Filename is required, please try again or terminate using Ctrl^C.\n" );
				}
			}
		}
		if( argument_set_offset == 0 )
		{
			export_offset = ewfinput_get_size_variable(
					 stderr,
					 _S_LIBEWF_CHAR( "Start export at offset" ),
					 0,
					 media_size,
					 export_offset );

		}
		if( argument_set_size == 0 )
		{
			export_size = ewfinput_get_size_variable(
				       stderr,
				       _S_LIBEWF_CHAR( "Amount of bytes to export" ),
				       0,
				       ( media_size - export_offset ),
				       export_size );
		}
	}
	if( target_filename == NULL )
	{
		fprintf( stderr, "Missing target filename defaulting to export.\n" );

		target_filename = CHAR_T_DUPLICATE( _S_CHAR_T( "export" ) );
	}
	fprintf( stderr, "\n" );

	/* Start exporting data
	 */
	timestamp_start = time( NULL );
	time_string     = libewf_common_ctime( &timestamp_start );

	if( time_string != NULL )
	{
		fprintf( stderr, "Export started at: %" PRIs "\n", time_string );

		libewf_common_free( time_string );
	}
	else
	{
		fprintf( stderr, "Export started.\n" );
	}
	if( callback != NULL )
	{
		ewfoutput_process_status_initialize( stderr, _S_LIBEWF_CHAR( "exported" ), timestamp_start );
	}
	fprintf( stderr, "This could take a while.\n\n" );

	if( output_raw == 0 )
	{
		filenames[ 0 ] = target_filename;

		export_handle = libewf_open(
		                 (CHAR_T * const *) filenames,
		                 1,
		                 LIBEWF_OPEN_WRITE );

		libewf_common_free( target_filename );

		if( export_handle == NULL )
		{
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
			if( errno != 0 )
			{
				error_string = ewfstring_strerror( errno );
			}
			if( error_string != NULL )
			{
				fprintf( stderr, "Unable to open export EWF file(s) with failure: %" PRIs ".\n",
				 error_string );

				libewf_common_free( error_string );
			}
			else
			{
				fprintf( stderr, "Unable to open export EWF file(s).\n" );
			}
#else
			fprintf( stderr, "Unable to open export EWF file(s).\n" );
#endif

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			return( EXIT_FAILURE );
		}
		if( libewf_set_sectors_per_chunk( export_handle, (uint32_t) sectors_per_chunk ) != 1 )
		{
			fprintf( stderr, "Unable to set sectors per chunk in handle.\n" );

			if( libewf_close( export_handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			return( EXIT_FAILURE );
		}
		if( libewf_set_write_segment_file_size( export_handle, (uint32_t) segment_file_size ) != 1 )
		{
			fprintf( stderr, "Unable to set write segment file size in handle.\n" );

			if( libewf_close( export_handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			return( EXIT_FAILURE );
		}
		if( libewf_set_write_compression_values(
		     export_handle,
		     compression_level,
		     (uint8_t) compress_empty_block ) != 1 )
		{
			fprintf( stderr, "Unable to set write compression values in handle.\n" );

			if( libewf_close( export_handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			return( EXIT_FAILURE );
		}
		if( libewf_set_write_format( export_handle, libewf_format ) != 1 )
		{
			fprintf( stderr, "Unable to set write format in handle.\n" );

			if( libewf_close( export_handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			return( EXIT_FAILURE );
		}
		/* TODO copy the necessary metadata
		 */
		count = ewfcommon_export_ewf(
		         handle,
		         export_handle,
		         export_size,
		         export_offset,
		         calculate_md5,
		         calculate_sha1,
		         swap_byte_pairs,
		         wipe_chunk_on_error,
		         callback );

		if( libewf_close( export_handle ) != 0 )
		{
			fprintf( stderr, "Unable to close export EWF file handle.\n" );

			return( EXIT_FAILURE );
		}
	}
	else
	{
		count = ewfcommon_export_raw(
		         handle,
		         target_filename,
		         segment_file_size,
		         export_size,
		         export_offset,
		         swap_byte_pairs,
		         wipe_chunk_on_error,
		         callback );

		libewf_common_free( target_filename );
	}
	timestamp_end = time( NULL );
	time_string   = libewf_common_ctime( &timestamp_end );

	if( count <= -1 )
	{
		if( time_string != NULL )
		{
			fprintf( stderr, "Export failed at: %" PRIs "\n", time_string );

			libewf_common_free( time_string );
		}
		else
		{
			fprintf( stderr, "Export failed.\n" );
		}
		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		return( EXIT_FAILURE );
	}
	if( time_string != NULL )
	{
		fprintf( stderr, "Export completed at: %" PRIs "\n", time_string );

		libewf_common_free( time_string );
	}
	else
	{
		fprintf( stderr, "Export completed.\n" );
	}
	ewfoutput_process_summary_fprint( stderr, _S_LIBEWF_CHAR( "Written" ), count, timestamp_start, timestamp_end );

	fprintf( stderr, "\n" );

	ewfoutput_crc_errors_fprint( stderr, handle );

	if( libewf_close( handle ) != 0 )
	{
		fprintf( stderr, "Unable to close EWF file(s).\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

