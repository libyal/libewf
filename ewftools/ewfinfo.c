/*
 * ewfinfo
 * Shows information stored in an EWF file
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
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_EXPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "ewfbyte_size_string.h"
#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfglob.h"
#include "ewfguid.h"
#include "ewfoutput.h"
#include "ewfsignal.h"

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Usage: ewfinfo [ -d date_format ] [ -himvV ] ewf_files\n\n" );

	fprintf( stream, "\t-d: specify the date format, options: ctime (default), dm (day/month), md (month/day), iso8601\n" );
	fprintf( stream, "\t-e: only show EWF read error information\n" );
	fprintf( stream, "\t-h: shows this help\n" );
	fprintf( stream, "\t-i: only show EWF acquiry information\n" );
	fprintf( stream, "\t-m: only show EWF media information\n" );
	fprintf( stream, "\t-v: verbose output to stderr\n" );
	fprintf( stream, "\t-V: print version\n" );
}

/* The main program
 */
#if defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	character_t media_size_string[ 16 ];
	character_t guid_string[ EWFGUID_STRING_LENGTH ];
	uint8_t guid[ 16 ];

	character_t *program                       = _CHARACTER_T_STRING( "ewfinfo" );

	system_character_t * const *argv_filenames = NULL;
	system_character_t **ewf_filenames         = NULL;

#if !defined( HAVE_GLOB_H )
	ewfglob_t *glob                            = NULL;
#endif
	char *file_format_string                   = NULL;
	system_integer_t option                    = 0;
	size64_t media_size                        = 0;
	uint32_t bytes_per_sector                  = 0;
	uint32_t amount_of_sectors                 = 0;
	uint32_t error_granularity                 = 0;
	uint32_t amount_of_acquiry_errors          = 0;
	uint32_t amount_of_sessions                = 0;
	int8_t compression_level                   = 0;
	uint8_t compress_empty_block               = 0;
	uint8_t media_type                         = 0;
	uint8_t media_flags                        = 0;
	uint8_t volume_type                        = 0;
	uint8_t format                             = 0;
	uint8_t verbose                            = 0;
	uint8_t date_format                        = LIBEWF_DATE_FORMAT_CTIME;
	char info_option                           = 'a';
	int amount_of_filenames                    = 0;
	int result                                 = 0;

	ewfoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = ewfgetopt(
	                   argc,
	                   argv,
	                   _SYSTEM_CHARACTER_T_STRING( "d:ehimvV" ) ) ) != (system_integer_t) -1 )
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
				     _SYSTEM_CHARACTER_T_STRING( "dm" ),
				     3 ) == 0 )
				{
					date_format = LIBEWF_DATE_FORMAT_DAYMONTH;
				}
				else if( system_string_compare(
				          optarg,
				          _SYSTEM_CHARACTER_T_STRING( "md" ),
				          3 ) == 0 )
				{
					date_format = LIBEWF_DATE_FORMAT_MONTHDAY;
				}
				else if( system_string_compare(
				          optarg,
				          _SYSTEM_CHARACTER_T_STRING( "iso8601" ),
				          8 ) == 0 )
				{
					date_format = LIBEWF_DATE_FORMAT_ISO8601;
				}
				else if( system_string_compare(
				          optarg,
				          _SYSTEM_CHARACTER_T_STRING( "ctime" ),
				          3 ) != 0 )
				{
					fprintf( stderr, "Unsupported date format: %" PRIs_SYSTEM " using default ctime.\n",
					 optarg );
				}
				break;

			case (system_integer_t) 'e':
				if( info_option != 'a' )
				{
					fprintf( stderr, "Conflicting options: %" PRIc_SYSTEM " and %c\n",
					 option, info_option );

					usage_fprint(
					 stdout );

					return( EXIT_FAILURE );
				}
				info_option = 'e';

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'i':
				if( info_option != 'a' )
				{
					fprintf( stderr, "Conflicting options: %" PRIc_SYSTEM " and %c\n",
					 option, info_option );

					usage_fprint(
					 stdout );

					return( EXIT_FAILURE );
				}
				info_option = 'i';

				break;

			case (system_integer_t) 'm':
				if( info_option != 'a' )
				{
					fprintf( stderr, "Conflicting options: %" PRIc_SYSTEM " and %c\n",
					 option, info_option );

					usage_fprint(
					 stdout );

					return( EXIT_FAILURE );
				}
				info_option = 'm';

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
		fprintf( stderr, "Missing EWF image file(s).\n" );

		usage_fprint(
		 stdout );

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
	if( ewfglob_initialize(
	     &glob ) != 1 )
	{
		fprintf( stderr, "Unable to initialize glob.\n" );

		return( EXIT_FAILURE );
	}
	amount_of_filenames = ewfglob_resolve(
	                       glob,
	                       &argv[ optind ],
	                       ( argc - optind ) );

	if( ( amount_of_filenames <= 0 )
	 || ( amount_of_filenames > (int) UINT16_MAX ) )
	{
		fprintf( stderr, "Unable to resolve glob.\n" );

		ewfglob_free(
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
			ewfglob_free(
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
	ewfglob_free(
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
	if( ( ewfcommon_abort == 0 )
	 && ( libewf_parse_header_values(
		     ewfcommon_libewf_handle,
		     date_format ) != 1 ) )
	{
		fprintf( stderr, "Unable to parse header values.\n" );
	}
	if( ( ewfcommon_abort == 0 )
	 && ( libewf_get_format(
	       ewfcommon_libewf_handle,
	       &format ) != 1 ) )
	{
		fprintf( stderr, "Unable to determine format.\n" );
	}
	else if( verbose == 1 )
	{
		switch( format )
		{
			case LIBEWF_FORMAT_EWF:
				file_format_string = "original EWF";
				break;

			case LIBEWF_FORMAT_SMART:
				file_format_string = "SMART";
				break;

			case LIBEWF_FORMAT_FTK:
				file_format_string = "FTK Imager";
				break;

			case LIBEWF_FORMAT_ENCASE1:
				file_format_string = "EnCase 1";
				break;

			case LIBEWF_FORMAT_ENCASE2:
				file_format_string = "EnCase 2";
				break;

			case LIBEWF_FORMAT_ENCASE3:
				file_format_string = "EnCase 3";
				break;

			case LIBEWF_FORMAT_ENCASE4:
				file_format_string = "EnCase 4";
				break;

			case LIBEWF_FORMAT_ENCASE5:
				file_format_string = "EnCase 5";
				break;

			case LIBEWF_FORMAT_ENCASE6:
				file_format_string = "EnCase 6";
				break;

			case LIBEWF_FORMAT_LINEN5:
				file_format_string = "linen 5";
				break;

			case LIBEWF_FORMAT_LINEN6:
				file_format_string = "linen 6";
				break;

			case LIBEWF_FORMAT_EWFX:
				file_format_string = "extended EWF (libewf)";
				break;

			case LIBEWF_FORMAT_UNKNOWN:
			default:
				file_format_string = "unknown";
				break;

		}
		fprintf( stdout, "File format:\t\t\t%s\n\n",
		 file_format_string );
	}
	if( ( ewfcommon_abort == 0 )
	 && ( ( info_option == 'a' )
	  || ( info_option == 'i' ) ) )
	{
		fprintf( stdout, "Acquiry information\n" );

		ewfoutput_header_values_fprint(
		 stdout,
		 ewfcommon_libewf_handle );

		fprintf( stdout, "\n" );
	}
	if( ( ewfcommon_abort == 0 )
	 && ( ( info_option == 'a' )
	  || ( info_option == 'm' ) ) )
	{
		fprintf( stdout, "Media information\n" );

		if( ( format != LIBEWF_FORMAT_EWF )
		 && ( format != LIBEWF_FORMAT_SMART ) )
		{
			if( libewf_get_media_type(
			     ewfcommon_libewf_handle,
			     &media_type ) != 1 )
			{
				fprintf( stderr, "Unable to determine media type.\n" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
			{
				fprintf( stdout, "\tMedia type:\t\tremovable disk\n" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_FIXED )
			{
				fprintf( stdout, "\tMedia type:\t\tfixed disk\n" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_CD )
			{
				fprintf( stdout, "\tMedia type:\t\tCD/DVD\n" );
			}
			else
			{
				fprintf( stdout, "\tMedia type:\t\tunknown (0x%" PRIx8 ")\n",
				 media_type );
			}
			if( libewf_get_media_flags(
			     ewfcommon_libewf_handle,
			     &media_flags ) != 1 )
			{
				fprintf( stderr, "Unable to determine media flags.\n" );
			}
			else if( verbose == 1 )
			{
				fprintf( stdout, "\tMedia flags:\t\t0x%" PRIx8 "\n",
				 media_flags );
			}
			if( libewf_get_volume_type(
			     ewfcommon_libewf_handle,
			     &volume_type ) != 1 )
			{
				fprintf( stderr, "Unable to determine volume type.\n" );
			}
			else if( volume_type == LIBEWF_VOLUME_TYPE_LOGICAL )
			{
				fprintf( stdout, "\tMedia is physical:\tno\n" );
			}
			else if( volume_type == LIBEWF_VOLUME_TYPE_PHYSICAL )
			{
				fprintf( stdout, "\tMedia is physical:\tyes\n" );
			}
			else
			{
				fprintf( stdout, "\tVolume type:\t\tunknown (0x%" PRIx8 ")\n",
				 volume_type );
			}
		}
		if( libewf_get_amount_of_sectors(
		     ewfcommon_libewf_handle,
		     &amount_of_sectors ) == 1 )
		{
			fprintf( stdout, "\tAmount of sectors:\t%" PRIu32 "\n",
			 amount_of_sectors );
		}
		else
		{
			fprintf( stderr, "Unable to determine amount of sectors.\n" );
		}
		if( libewf_get_bytes_per_sector(
		     ewfcommon_libewf_handle,
		     &bytes_per_sector ) == 1 )
		{
			fprintf( stdout, "\tBytes per sector:\t%" PRIu32 "\n",
			 bytes_per_sector );
		}
		else
		{
			fprintf( stderr, "Unable to determine bytes per sector.\n" );
		}
		if( libewf_get_media_size(
		     ewfcommon_libewf_handle,
		     &media_size ) == 1 )
		{
			result = ewfbyte_size_string_create(
				  media_size_string,
				  16,
				  media_size,
				  EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE );

			if( result == 1 )
			{
				fprintf( stdout, "\tMedia size:\t\t%" PRIs " (%" PRIu64 " bytes)\n",
				 media_size_string, media_size );
			}
			else
			{
				fprintf( stdout, "\tMedia size:\t\t%" PRIu64 " bytes\n",
				 media_size );
			}
		}
		else
		{
			fprintf( stderr, "Unable to determine media size.\n" );
		}
		if( ( format == LIBEWF_FORMAT_ENCASE5 )
		 || ( format == LIBEWF_FORMAT_ENCASE6 )
		 || ( format == LIBEWF_FORMAT_LINEN5 )
		 || ( format == LIBEWF_FORMAT_LINEN6 )
		 || ( format == LIBEWF_FORMAT_EWFX ) )
		{
			if( libewf_get_error_granularity(
			     ewfcommon_libewf_handle,
			     &error_granularity ) == 1 )
			{
				fprintf( stdout, "\tError granularity:\t%" PRIu32 "\n",
				 error_granularity );
			}
			else
			{
				fprintf( stderr, "Unable to determine error granularity.\n" );
			}
			if( libewf_get_compression_values(
			     ewfcommon_libewf_handle,
			     &compression_level,
			     &compress_empty_block ) == 1 )
			{
				if( compression_level == LIBEWF_COMPRESSION_NONE )
				{
					fprintf( stdout, "\tCompression type:\tno compression\n" );
				}
				else if( compression_level == LIBEWF_COMPRESSION_FAST )
				{
					fprintf( stdout, "\tCompression type:\tgood (fast) compression\n" );
				}
				else if( compression_level == LIBEWF_COMPRESSION_BEST )
				{
					fprintf( stdout, "\tCompression type:\tbest compression\n" );
				}
				else
				{
					fprintf( stdout, "\tCompression type:\tunknown compression\n" );
				}
			}
			else
			{
				fprintf( stderr, "Unable to determine compression level.\n" );
			}
			if( libewf_get_guid(
			     ewfcommon_libewf_handle,
			     guid,
			     16 ) == 1 )
			{
				if( ewfguid_to_string(
				     (ewfguid_t *) guid,
				     guid_string,
				     EWFGUID_STRING_LENGTH ) == 1 )
				{
					fprintf( stdout, "\tGUID:\t\t\t%" PRIs "\n",
					 guid_string );
				}
			}
		}
		ewfoutput_hash_values_fprint(
		 stdout,
		 ewfcommon_libewf_handle );

		fprintf( stdout, "\n" );

		ewfoutput_sessions_fprint(
		 stdout,
		 ewfcommon_libewf_handle,
		 &amount_of_sessions );
	}
	if( ( ewfcommon_abort == 0 )
	 && ( ( info_option == 'a' )
	  || ( info_option == 'e' ) ) )
	{
		ewfoutput_acquiry_errors_fprint(
		 stdout,
		 ewfcommon_libewf_handle,
		 &amount_of_acquiry_errors );
	}
	if( ewfsignal_detach() != 1 )
	{
		fprintf( stderr, "Unable to detach signal handler.\n" );
	}
	if( libewf_close(
	     ewfcommon_libewf_handle ) != 0 )
	{
		fprintf( stderr, "Unable to close EWF file(s).\n" );

		return( EXIT_FAILURE );
	}
	if( ewfcommon_abort != 0 )
	{
		fprintf( stdout, "%" PRIs ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

