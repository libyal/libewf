/*
 * ewfacquire
 * Reads data from a file or device and writes it in EWF format
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (c) 2006-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#include <libcstring.h>
#include <liberror.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include <libsystem.h>

#include "byte_size_string.h"
#include "device_handle.h"
#include "ewfcommon.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "imaging_handle.h"
#include "log_handle.h"
#include "platform.h"
#include "process_status.h"
#include "storage_media_buffer.h"

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define EWFACQUIRE_2_TIB		0x20000000000UL
#else
#define EWFACQUIRE_2_TIB		0x20000000000ULL
#endif

#define EWFACQUIRE_INPUT_BUFFER_SIZE	64

device_handle_t *ewfacquire_device_handle   = NULL;
imaging_handle_t *ewfacquire_imaging_handle = NULL;
int ewfacquire_abort                        = 0;

/* Prints the executable usage information to the stream
 */
void usage_fprint(
      FILE *stream )
{
	libcstring_system_character_t default_segment_file_size_string[ 16 ];
	libcstring_system_character_t minimum_segment_file_size_string[ 16 ];
	libcstring_system_character_t maximum_32bit_segment_file_size_string[ 16 ];
	libcstring_system_character_t maximum_64bit_segment_file_size_string[ 16 ];

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
	fprintf( stream, "Use ewfacquire to acquire data from a file or device and store it in the EWF\n"
	                 "format (Expert Witness Compression Format).\n\n" );

	fprintf( stream, "Usage: ewfacquire [ -A codepage ] [ -b amount_of_sectors ] [ -B amount_of_bytes ]\n"
	                 "                  [ -c compression_type ] [ -C case_number ] [ -d digest_type ]\n"
	                 "                  [ -D description ] [ -e examiner_name ] [ -E evidence_number ]\n"
	                 "                  [ -f format ] [ -g amount_of_sectors ] [ -l log_filename ]\n"
	                 "                  [ -m media_type ] [ -M media_flags ] [ -N notes ] [ -o offset ]\n"
	                 "                  [ -p process_buffer_size ] [ -P bytes_per_sector ]\n"
	                 "                  [ -r read_error_retries ] [ -S segment_file_size ]\n"
	                 "                  [ -t target ] [ -2 secondary_target ] [ -hqRsuvVw ] source\n\n" );

	fprintf( stream, "\tsource: the source file or device\n\n" );

	fprintf( stream, "\t-A:     codepage of header section, options: ascii (default), windows-874,\n"
	                 "\t        windows-1250, windows-1251, windows-1252, windows-1253, windows-1254,\n"
	                 "\t        windows-1255, windows-1256, windows-1257, windows-1258\n" );
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
	                 "\t        encase2, encase3, encase4, encase5, encase6 (default), linen5, linen6,\n"
	                 "\t        ewfx\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs acquiry errors and the digest (hash) to the log_filename\n" );
	fprintf( stream, "\t-m:     specify the media type, options: fixed (default), removable, optical, memory\n" );
	fprintf( stream, "\t-M:     specify the media flags, options: logical, physical (default)\n" );
	fprintf( stream, "\t-N:     specify the notes (default is notes).\n" );
	fprintf( stream, "\t-o:     specify the offset to start to acquire (default is 0)\n" );
	fprintf( stream, "\t-p:     specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-P:     specify the amount of bytes per sector (default is 512)\n"
	                 "\t        (use this to override the automatic bytes per sector detection)\n" );
	fprintf( stream, "\t-q:     quiet shows no status information\n" );
	fprintf( stream, "\t-r:     specify the amount of retries when a read error occurs (default is 2)\n" );
	fprintf( stream, "\t-R:     resume acquiry at a safe point\n" );
	fprintf( stream, "\t-s:     swap byte pairs of the media data (from AB to BA)\n"
	                 "\t        (use this for big to little endian conversion and vice versa)\n" );

	if( result == 1 )
	{
		fprintf( stream, "\t-S:     specify the segment file size in bytes (default is %" PRIs_LIBCSTRING_SYSTEM ")\n"
		                 "\t        (minimum is %" PRIs_LIBCSTRING_SYSTEM ", maximum is %" PRIs_LIBCSTRING_SYSTEM " for encase6 format\n"
		                 "\t        and %" PRIs_LIBCSTRING_SYSTEM " for other formats)\n",
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

	fprintf( stream, "\t-t:     specify the target file (without extension) to write to\n" );
	fprintf( stream, "\t-u:     unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
	fprintf( stream, "\t-w:     wipe sectors on read error (mimic EnCase like behavior)\n" );
	fprintf( stream, "\t-2:     specify the secondary target file (without extension) to write to\n" );
}

/* Prints an overview of the acquiry parameters and asks the for confirmation
 * Return 1 if confirmed by user, 0 otherwise or -1 on error
 */
int8_t ewfacquire_confirm_acquiry_parameters(
        FILE *stream,
        libcstring_system_character_t *input_buffer,
        size_t input_buffer_size,
        uint8_t resume_acquiry,
        libcstring_system_character_t *filename,
        libcstring_system_character_t *filename_secondary_copy,
        libcstring_system_character_t *case_number,
        libcstring_system_character_t *description,
        libcstring_system_character_t *evidence_number,
        libcstring_system_character_t *examiner_name,
        libcstring_system_character_t *notes,
        uint8_t media_type,
        uint8_t media_flags,
        int8_t compression_level,
        uint8_t compression_flags,
        uint8_t ewf_format,
        off64_t acquiry_offset,
        off64_t resume_acquiry_offset,
        size64_t acquiry_size,
        size64_t segment_file_size,
        uint32_t bytes_per_sector,
        uint32_t sectors_per_chunk,
        uint32_t sector_error_granularity,
        uint8_t read_error_retries,
        uint8_t wipe_block_on_read_error,
        liberror_error_t **error )
{
	libcstring_system_character_t acquiry_size_string[ 16 ];
	libcstring_system_character_t segment_file_size_string[ 16 ];

	libcstring_system_character_t *fixed_string_variable = NULL;
	static char *function                                = "ewfacquire_confirm_acquiry_parameters";
	int8_t input_confirmed                               = -1;
	int result                                           = 0;

	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "The following acquiry parameters were provided:\n" );

	fprintf(
	 stream,
	 "Image path and filename:\t%" PRIs_LIBCSTRING_SYSTEM "",
	 filename );

	if( resume_acquiry == 0 )
	{
		if( ewf_format == LIBEWF_FORMAT_SMART )
		{
			fprintf(
			 stream,
			 ".s01" );
		}
		else if( ( ewf_format == LIBEWF_FORMAT_EWF )
		      || ( ewf_format == LIBEWF_FORMAT_EWFX ) )
		{
			fprintf(
			 stream,
			 ".e01" );
		}
		else
		{
			fprintf(
			 stream,
			 ".E01" );
		}
	}
	fprintf(
	 stream,
	 "\n" );

	if( filename_secondary_copy != NULL )
	{
		fprintf(
		 stream,
		 "Secondary copy:\t\t\t%" PRIs_LIBCSTRING_SYSTEM "",
		 filename_secondary_copy );

		if( resume_acquiry == 0 )
		{
			if( ewf_format == LIBEWF_FORMAT_SMART )
			{
				fprintf(
				 stream,
				 ".s01" );
			}
			else if( ( ewf_format == LIBEWF_FORMAT_EWF )
			      || ( ewf_format == LIBEWF_FORMAT_EWFX ) )
			{
				fprintf(
				 stream,
				 ".e01" );
			}
			else
			{
				fprintf(
				 stream,
				 ".E01" );
			}
		}
		fprintf(
		 stream,
		 "\n" );
	}
	fprintf(
	 stream,
	 "Case number:\t\t\t" );

	if( case_number != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "",
		 case_number );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Description:\t\t\t" );

	if( description != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "",
		 description );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Evidence number:\t\t" );

	if( evidence_number != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "",
		 evidence_number );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Examiner name:\t\t\t" );

	if( examiner_name != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "",
		 examiner_name );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Notes:\t\t\t\t" );

	if( notes != NULL )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "",
		 notes );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Media type:\t\t\t" );

	if( media_type == LIBEWF_MEDIA_TYPE_FIXED )
	{
		fprintf(
		 stream,
		 "fixed disk\n" );
	}
	else if( media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
	{
		fprintf(
		 stream,
		 "removable disk\n" );
	}
	else if( media_type == LIBEWF_MEDIA_TYPE_OPTICAL )
	{
		fprintf(
		 stream,
		 "optical disk (CD/DVD/BD)\n" );
	}
	else if( media_type == LIBEWF_MEDIA_TYPE_MEMORY )
	{
		fprintf(
		 stream,
		 "memory (RAM)\n" );
	}
	fprintf(
	 stream,
	 "Is physical:\t\t\t" );

	if( ( media_flags & LIBEWF_MEDIA_FLAG_PHYSICAL ) == LIBEWF_MEDIA_FLAG_PHYSICAL )
	{
		fprintf(
		 stream,
		 "yes\n" );
	}
	else
	{
		fprintf(
		 stream,
		 "no\n" );
	}
	fprintf(
	 stream,
	 "Compression used:\t\t" );

	if( compression_level == LIBEWF_COMPRESSION_FAST )
	{
		fprintf(
		 stream,
		 "fast\n" );
	}
	else if( compression_level == LIBEWF_COMPRESSION_BEST )
	{
		fprintf(
		 stream,
		 "best\n" );
	}
	else if( compression_level == LIBEWF_COMPRESSION_NONE )
	{
		if( ( compression_flags & LIBEWF_FLAG_COMPRESS_EMPTY_BLOCK ) == LIBEWF_FLAG_COMPRESS_EMPTY_BLOCK )
		{
			fprintf(
			 stream,
			 "empty block\n" );
		}
		else
		{
			fprintf(
			 stream,
			 "none\n" );
		}
	}
	fprintf(
	 stream,
	 "EWF file format:\t\t" );

	if( ewf_format == LIBEWF_FORMAT_EWF )
	{
		fprintf(
		 stream,
		 "original EWF\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_SMART )
	{
		fprintf(
		 stream,
		 "SMART\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_FTK )
	{
		fprintf(
		 stream,
		 "FTK Imager\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE1 )
	{
		fprintf(
		 stream,
		 "EnCase 1\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE2 )
	{
		fprintf(
		 stream,
		 "EnCase 2\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE3 )
	{
		fprintf(
		 stream,
		 "EnCase 3\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE4 )
	{
		fprintf(
		 stream,
		 "EnCase 4\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE5 )
	{
		fprintf(
		 stream,
		 "EnCase 5\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_ENCASE6 )
	{
		fprintf(
		 stream,
		 "EnCase 6\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_LINEN5 )
	{
		fprintf(
		 stream,
		 "linen 5\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_LINEN6 )
	{
		fprintf(
		 stream,
		 "linen 6\n" );
	}
	else if( ewf_format == LIBEWF_FORMAT_EWFX )
	{
		fprintf(
		 stream,
		 "extended EWF (libewf)\n" );
	}
	else
	{
		fprintf(
		 stream,
		 "\n" );
	}
	fprintf(
	 stream, "Acquiry start offset:\t\t%" PRIi64 "\n",
	 acquiry_offset );

	if( resume_acquiry != 0 )
	{
		fprintf(
		 stream, "Resuming acquiry at offset:\t%" PRIi64 "\n",
		 resume_acquiry_offset );
	}
	result = byte_size_string_create(
	          acquiry_size_string,
	          16,
	          acquiry_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	fprintf(
	 stream,
	 "Amount of bytes to acquire:\t" );

	if( acquiry_size == 0 )
	{
		fprintf(
		 stream,
		 "%" PRIu64 " (until end of input)",
		 acquiry_size );
	}
	else if( result == 1 )
	{
		fprintf(
		 stream,
		 "%" PRIs_LIBCSTRING_SYSTEM " (%" PRIu64 " bytes)",
		 acquiry_size_string, acquiry_size );
	}
	else
	{
		fprintf(
		 stream,
		 "%" PRIu64 " bytes",
		 acquiry_size );
	}
	fprintf(
	 stream,
	 "\n" );

	result = byte_size_string_create(
	          segment_file_size_string,
	          16,
	          segment_file_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	fprintf(
	 stream,
	 "Evidence segment file size:\t" );

	if( result == 1 )
	{
		fprintf(
		 stream, "%" PRIs_LIBCSTRING_SYSTEM " (%" PRIu64 " bytes)",
		 segment_file_size_string,
		 segment_file_size );
	}
	else
	{
		fprintf(
		 stream,
		 "%" PRIu64 " bytes",
		 segment_file_size );
	}
	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Bytes per sector:\t\t%" PRIu32 "\n",
	 bytes_per_sector );
	fprintf(
	 stream,
	 "Block size:\t\t\t%" PRIu32 " sectors\n",
	 sectors_per_chunk );
	fprintf(
	 stream,
	 "Error granularity:\t\t%" PRIu32 " sectors\n",
	 sector_error_granularity );
	fprintf(
	 stream,
	 "Retries on read error:\t\t%" PRIu8 "\n",
	 read_error_retries );

	fprintf(
	 stream,
	 "Wipe sectors on read error:\t" );

	if( wipe_block_on_read_error == 0 )
	{
		fprintf(
		 stream,
		 "no\n" );
	}
	else
	{
		fprintf(
		 stream,
		 "yes\n" );
	}
	fprintf(
	 stream,
	 "\n" );

	/* Ask for confirmation
	 */
	while( input_confirmed == -1 )
	{
		if( ewfinput_get_fixed_string_variable(
		     stream,
		     input_buffer,
		     input_buffer_size,
		     _LIBCSTRING_SYSTEM_STRING( "Continue acquiry with these values" ),
		     ewfinput_yes_no,
		     2,
		     0,
		     &fixed_string_variable,
		     error ) == -1 )
		{
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libsystem_notify_print_error_backtrace(
				 *error );
			}
			liberror_error_free(
			 error );

			fprintf(
			 stream,
			 "Unable to determine answer.\n" );
		}
		else if( ewfinput_determine_yes_no(
		          fixed_string_variable,
		          (uint8_t *) &input_confirmed,
		          error ) != 1 )
		{
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libsystem_notify_print_error_backtrace(
				 *error );
			}
			liberror_error_free(
			 error );

			fprintf(
			 stream,
			 "Selected option not supported, please try again or terminate using Ctrl^C.\n" );

			input_confirmed = -1;
		}
	}
	fprintf(
	 stream,
	 "\n" );

	return( input_confirmed );
}

/* Reads data from a file descriptor and writes it in EWF format
 * Returns the amount of bytes written or -1 on error
 */
ssize64_t ewfacquire_read_input(
           imaging_handle_t *imaging_handle,
           device_handle_t *device_handle,
           size64_t media_size,
           size64_t acquiry_size,
           off64_t acquiry_offset,
           off64_t resume_acquiry_offset,
           uint32_t bytes_per_sector,
           uint8_t swap_byte_pairs,
           uint32_t sector_error_granularity,
           size_t process_buffer_size,
           libcstring_system_character_t *calculated_md5_hash_string,
           size_t calculated_md5_hash_string_size,
           libcstring_system_character_t *calculated_sha1_hash_string,
           size_t calculated_sha1_hash_string_size,
           process_status_t *process_status,
           liberror_error_t **error )
{
	storage_media_buffer_t *storage_media_buffer = NULL;
	static char *function                        = "ewfacquire_read_input";
	off64_t read_error_offset                    = 0;
	size64_t read_error_size                     = 0;
	ssize64_t acquiry_count                      = 0;
	size_t read_size                             = 0;
	ssize_t read_count                           = 0;
	ssize_t process_count                        = 0;
	ssize_t write_count                          = 0;
	uint32_t byte_error_granularity              = 0;
	uint32_t chunk_size                          = 0;
	int amount_of_read_errors                    = 0;
	int read_error_iterator                      = 0;

	if( imaging_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( process_buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid process buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( process_status == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid process status.",
		 function );

		return( -1 );
	}
	if( ( acquiry_size == 0 )
         || ( acquiry_size > media_size )
         || ( acquiry_size > (ssize64_t) INT64_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid write size value out of range.",
		 function );

		return( -1 );
	}
	if( acquiry_offset > 0 )
	{
		if( ( acquiry_offset > (off64_t) media_size )
		 || ( ( acquiry_size + acquiry_offset ) > media_size ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: unable to acquire beyond media size.",
			 function );

			return( -1 );
		}
		if( device_handle_seek_offset(
		     device_handle,
		     acquiry_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to find acquiry offset.",
			 function );

			return( -1 );
		}
	}
	if( resume_acquiry_offset > 0 )
	{
		if( ( acquiry_offset + resume_acquiry_offset ) > (off64_t) media_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: unable to resume acquire beyond media size.",
			 function );

			return( -1 );
		}
		if( device_handle_seek_offset(
		     device_handle,
		     resume_acquiry_offset,
		     SEEK_CUR,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to find acquiry offset.",
			 function );

			return( -1 );
		}
		if( imaging_handle_seek_offset(
		     imaging_handle,
		     0,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek imaging offset.",
			 function );

			return( -1 );
		}
	}
	byte_error_granularity = sector_error_granularity * bytes_per_sector;

	if( imaging_handle_get_chunk_size(
	     imaging_handle,
	     &chunk_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk size.",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid chunk size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	process_buffer_size = (size_t) chunk_size;
#else
	if( process_buffer_size == 0 )
	{
		process_buffer_size = (size_t) chunk_size;
	}
#endif

	if( storage_media_buffer_initialize(
	     &storage_media_buffer,
	     process_buffer_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create storage media buffer.",
		 function );

		return( -1 );
	}
	while( acquiry_count < (ssize64_t) acquiry_size )
	{
		read_size = process_buffer_size;

		if( ( (ssize64_t) acquiry_size - acquiry_count ) < (ssize64_t) read_size )
		{
			read_size = (size_t) ( (ssize64_t) acquiry_size - acquiry_count );
		}
		if( acquiry_count >= resume_acquiry_offset )
		{
			read_count = device_handle_read_buffer(
				      device_handle,
				      storage_media_buffer->raw_buffer,
				      read_size,
				      error );

			if( read_count < 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: error reading data from input.",
				 function );

				storage_media_buffer_free(
				 &storage_media_buffer,
				 NULL );

				return( -1 );
			}
			if( read_count == 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unexpected end of input.",
				 function );

				storage_media_buffer_free(
				 &storage_media_buffer,
				 NULL );

				return( -1 );
			}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer->data_in_compression_buffer = 0;
#endif
			storage_media_buffer->raw_buffer_amount = read_count;

			/* Swap byte pairs
			 * The digest hashes are calcultated after swap
			 */
			if( ( swap_byte_pairs == 1 )
			 && ( imaging_handle_swap_byte_pairs(
			       imaging_handle,
			       storage_media_buffer,
			       read_count,
			       error ) != 1 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_CONVERSION,
				 LIBERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to swap byte pairs.",
				 function );

				storage_media_buffer_free(
				 &storage_media_buffer,
				 NULL );

				return( -1 );
			}
		}
		else
		{
			/* Align with resume acquiry offset if necessary
			 */
			if( ( resume_acquiry_offset - (off64_t) acquiry_count ) < (off64_t) read_size )
			{
				read_size = (size_t) ( resume_acquiry_offset - acquiry_count );
			}
			read_count = imaging_handle_read_buffer(
				      imaging_handle,
				      storage_media_buffer,
				      read_size,
				      error );

			if( read_count < 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				"%s: unable to read data.",
				 function );

				storage_media_buffer_free(
				 &storage_media_buffer,
				 NULL );

				return( -1 );
			}
			if( read_count == 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unexpected end of data.",
				 function );

				storage_media_buffer_free(
				 &storage_media_buffer,
				 NULL );

				return( -1 );
			}
			process_count = imaging_handle_prepare_read_buffer(
					 imaging_handle,
					 storage_media_buffer,
					 error );

			if( process_count < 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				"%s: unable to prepare buffer after read.",
				 function );

				storage_media_buffer_free(
				 &storage_media_buffer,
				 NULL );

				return( -1 );
			}
			if( process_count > (ssize_t) read_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: more bytes read than requested.",
				 function,
				 process_count, read_size );

				storage_media_buffer_free(
				 &storage_media_buffer,
				 NULL );

				return( -1 );
			}
			read_count = process_count;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			/* Set the amount of chunk data in the buffer
			 */
			if( storage_media_buffer->data_in_compression_buffer == 1 )
			{
				storage_media_buffer->compression_buffer_amount = process_count;
			}
#endif
		}
		if( imaging_handle_update_integrity_hash(
		     imaging_handle,
		     storage_media_buffer,
		     read_count,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to update integrity hash(es).",
			 function );

			storage_media_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		if( acquiry_count >= resume_acquiry_offset )
		{
			process_count = imaging_handle_prepare_write_buffer(
					 imaging_handle,
					 storage_media_buffer,
					 error );

			if( process_count < 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				"%s: unable to prepare buffer before write.",
				 function );

				storage_media_buffer_free(
				 &storage_media_buffer,
				 NULL );

				return( -1 );
			}
			write_count = imaging_handle_write_buffer(
				       imaging_handle,
				       storage_media_buffer,
				       process_count,
				       error );

			if( write_count < 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write data to file.",
				 function );

				storage_media_buffer_free(
				 &storage_media_buffer,
				 NULL );

				return( -1 );
			}
		}
		acquiry_count += read_count;

		if( process_status_update(
		     process_status,
		     (size64_t) acquiry_count,
		     acquiry_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update process status.",
			 function );

			storage_media_buffer_free(
			 &storage_media_buffer,
			 NULL );

			return( -1 );
		}
		if( ewfacquire_abort != 0 )
		{
			break;
		}
	}
	if( storage_media_buffer_free(
	     &storage_media_buffer,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free storage media buffer.",
		 function );

		return( -1 );
	}
	if( acquiry_count >= resume_acquiry_offset )
	{
		if( device_handle_get_amount_of_read_errors(
		     device_handle,
		     &amount_of_read_errors,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve amount of read errors.",
			 function );

			return( -1 );
		}
		for( read_error_iterator = 0;
		     read_error_iterator < amount_of_read_errors;
		     read_error_iterator++ )
		{
			if( device_handle_get_read_error(
			     device_handle,
			     read_error_iterator,
			     &read_error_offset,
			     &read_error_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve read error: %d.",
				 function,
				 read_error_iterator );

				return( -1 );
			}
			if( imaging_handle_add_read_error(
			     imaging_handle,
			     read_error_offset,
			     read_error_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add read errror: %d to imaging handle.",
				 function,
				 read_error_iterator );

				return( -1 );
			}
		}
		write_count = imaging_handle_finalize(
			       imaging_handle,
			       calculated_md5_hash_string,
			       calculated_md5_hash_string_size,
			       calculated_sha1_hash_string,
			       calculated_sha1_hash_string_size,
			       error );

		if( write_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to finalize.",
			 function );

			return( -1 );
		}
		acquiry_count += write_count;
	}
	return( acquiry_count );
}

/* Signal handler for ewfacquire
 */
void ewfacquire_signal_handler(
      libsystem_signal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "ewfacquire_signal_handler";

	ewfacquire_abort = 1;

	if( ewfacquire_device_handle != NULL )
	{
		if( device_handle_signal_abort(
		     ewfacquire_device_handle,
		     &error ) != 1 )
		{
			libsystem_notify_printf(
			 "%s: unable to signal device handle to abort.\n",
			 function );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( ewfacquire_imaging_handle != NULL )
	{
		if( imaging_handle_signal_abort(
		     ewfacquire_imaging_handle,
		     &error ) != 1 )
		{
			libsystem_notify_printf(
			 "%s: unable to signal imaging handle to abort.\n",
			 function );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libsystem_file_io_close(
	     0 ) != 0 )
	{
		libsystem_notify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcstring_system_character_t acquiry_operating_system[ 32 ];
	libcstring_system_character_t input_buffer[ EWFACQUIRE_INPUT_BUFFER_SIZE ];
	libcstring_system_character_t media_information_model[ 64 ];
	libcstring_system_character_t media_information_serial_number[ 64 ];

	liberror_error_t *error                                         = NULL;

	libcstring_system_character_t *acquiry_software_version         = NULL;
	libcstring_system_character_t *calculated_md5_hash_string       = NULL;
	libcstring_system_character_t *calculated_sha1_hash_string      = NULL;
	libcstring_system_character_t *case_number                      = NULL;
	libcstring_system_character_t *description                      = NULL;
	libcstring_system_character_t *evidence_number                  = NULL;
	libcstring_system_character_t *examiner_name                    = NULL;
	libcstring_system_character_t *fixed_string_variable            = NULL;
	libcstring_system_character_t *log_filename                     = NULL;
	libcstring_system_character_t *notes                            = NULL;
	libcstring_system_character_t *option_case_number               = NULL;
	libcstring_system_character_t *option_description               = NULL;
	libcstring_system_character_t *option_examiner_name             = NULL;
	libcstring_system_character_t *option_evidence_number           = NULL;
	libcstring_system_character_t *option_notes                     = NULL;
	libcstring_system_character_t *option_secondary_target_filename = NULL;
	libcstring_system_character_t *option_target_filename           = NULL;
	libcstring_system_character_t *program                          = _LIBCSTRING_SYSTEM_STRING( "ewfacquire" );
	libcstring_system_character_t *secondary_target_filename        = NULL;
	libcstring_system_character_t *target_filename                  = NULL;

	log_handle_t *log_handle                                        = NULL;

	process_status_t *process_status                                = NULL;

	libcstring_system_integer_t option                              = 0;
	off64_t resume_acquiry_offset                                   = 0;
	ssize64_t read_count                                            = 0;
	size_t string_length                                            = 0;
	size_t secondary_target_filename_size                           = 0;
	size_t target_filename_size                                     = 0;
	uint64_t acquiry_offset                                         = 0;
	uint64_t acquiry_size                                           = 0;
	uint64_t input_size_variable                                    = 0;
	uint64_t maximum_segment_file_size                              = 0;
	uint64_t media_size                                             = 0;
	uint64_t process_buffer_size                                    = EWFCOMMON_PROCESS_BUFFER_SIZE;
	uint64_t segment_file_size                                      = 0;
	uint32_t bytes_per_sector                                       = 512;
	uint32_t sector_error_granularity                               = 0;
	uint32_t sectors_per_chunk                                      = 0;
	uint8_t calculate_md5                                           = 1;
	uint8_t calculate_sha1                                          = 0;
	uint8_t compression_flags                                       = 0;
	uint8_t ewf_format                                              = LIBEWF_FORMAT_ENCASE6;
	uint8_t media_flags                                             = LIBEWF_MEDIA_FLAG_PHYSICAL;
	uint8_t media_type                                              = LIBEWF_MEDIA_TYPE_FIXED;
	uint8_t print_status_information                                = 1;
	uint8_t read_error_retries                                      = 2;
	uint8_t resume_acquiry                                          = 0;
	uint8_t swap_byte_pairs                                         = 0;
	uint8_t verbose                                                 = 0;
	uint8_t wipe_block_on_read_error                                = 0;
	int8_t acquiry_parameters_confirmed                             = 0;
	int8_t compression_level                                        = LIBEWF_COMPRESSION_NONE;
	int argument_set_compression                                    = 0;
	int argument_set_bytes_per_sector                               = 0;
	int argument_set_format                                         = 0;
	int argument_set_media_type                                     = 0;
	int argument_set_offset                                         = 0;
	int argument_set_read_error_retries                             = 0;
	int argument_set_sector_error_granularity                       = 0;
	int argument_set_sectors_per_chunk                              = 0;
	int argument_set_segment_file_size                              = 0;
	int argument_set_size                                           = 0;
	int argument_set_media_flags                                    = 0;
	int argument_set_wipe_block_on_read_error                       = 0;
	int default_media_flags                                         = 0;
	int default_media_type                                          = 0;
	int error_abort                                                 = 0;
	int header_codepage                                             = LIBEWF_CODEPAGE_ASCII;
	int interactive_mode                                            = 1;
	int result                                                      = 0;
	int status                                                      = 0;

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

	if( libsystem_initialize(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	ewfoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "A:b:B:c:C:d:D:e:E:f:g:hl:m:M:N:o:p:P:qr:RsS:t:uvVw2:" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'A':
				if( ewfinput_determine_header_codepage(
				     optarg,
				     &header_codepage,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					header_codepage = LIBEWF_CODEPAGE_ASCII;

					fprintf(
					 stderr,
					 "Unsuported header codepage defaulting to: ascii.\n" );
				}
				break;

			case (libcstring_system_integer_t) 'b':
				if( ewfinput_determine_sectors_per_chunk(
				     optarg,
				     &sectors_per_chunk,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					sectors_per_chunk = 64;

					fprintf(
					 stderr,
					 "Unsuported amount of sectors per chunk defaulting to: 64.\n" );
				}
				else
				{
					argument_set_sectors_per_chunk = 1;
				}
				break;

			case (libcstring_system_integer_t) 'B':
				string_length = libcstring_system_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &acquiry_size,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					acquiry_size = 0;

					fprintf(
					 stderr,
					 "Unsupported acquiry size defaulting to: all bytes.\n" );
				}
				argument_set_size = 1;

				break;

			case (libcstring_system_integer_t) 'c':
				if( ewfinput_determine_compression_level(
				     optarg,
				     &compression_level,
				     &compression_flags,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					compression_level = LIBEWF_COMPRESSION_NONE;
					compression_flags = 0;

					fprintf(
					 stderr,
					 "Unsupported compression type defaulting to: none.\n" );
				}
				else
				{
					argument_set_compression = 1;
				}
				break;

			case (libcstring_system_integer_t) 'C':
				option_case_number = optarg;

				break;

			case (libcstring_system_integer_t) 'd':
				if( libcstring_system_string_compare(
				     optarg,
				     _LIBCSTRING_SYSTEM_STRING( "sha1" ),
				     4 ) == 0 )
				{
					calculate_sha1 = 1;
				}
				else
				{
					fprintf(
					 stderr,
					 "Unsupported digest type.\n" );
				}
				break;

			case (libcstring_system_integer_t) 'D':
				option_description = optarg;

				break;

			case (libcstring_system_integer_t) 'e':
				option_examiner_name = optarg;

				break;

			case (libcstring_system_integer_t) 'E':
				option_evidence_number = optarg;

				break;

			case (libcstring_system_integer_t) 'f':
				if( ewfinput_determine_ewf_format(
				     optarg,
				     &ewf_format,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					ewf_format = LIBEWF_FORMAT_ENCASE6;

					fprintf(
					 stderr,
					 "Unsupported EWF file format type defaulting to: encase6.\n" );
				}
				else
				{
					argument_set_format = 1;
				}
				break;

			case (libcstring_system_integer_t) 'g':
				if( ewfinput_determine_sectors_per_chunk(
				     optarg,
				     &sector_error_granularity,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					sector_error_granularity = 64;

					fprintf(
					 stderr,
					 "Unsuported amount of sector error granularity defaulting to: 64.\n" );
				}
				else
				{
					argument_set_sector_error_granularity = 1;
				}
				break;

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libcstring_system_integer_t) 'm':
				if( ewfinput_determine_media_type(
				     optarg,
				     &media_type,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					media_type = LIBEWF_MEDIA_TYPE_FIXED;

					fprintf(
					 stderr,
					 "Unsupported media type defaulting to: fixed.\n" );
				}
				else
				{
					argument_set_media_type = 1;
				}
				break;

			case (libcstring_system_integer_t) 'M':
				if( ewfinput_determine_media_flags(
				     optarg,
				     &media_flags,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					media_flags = LIBEWF_MEDIA_FLAG_PHYSICAL;

					fprintf(
					 stderr,
					 "Unsupported media flags defaulting to: physical.\n" );
				}
				else
				{
					argument_set_media_flags = 1;
				}
				break;

			case (libcstring_system_integer_t) 'N':
				option_notes = optarg;

				break;

			case (libcstring_system_integer_t) 'o':
				string_length = libcstring_system_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &acquiry_offset,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					acquiry_offset = 0;

					fprintf(
					 stderr,
					 "Unsupported acquiry offset defaulting to: %" PRIu64 ".\n",
					 acquiry_offset );
				}
				argument_set_offset = 1;

				break;

			case (libcstring_system_integer_t) 'p':
				string_length = libcstring_system_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &process_buffer_size,
				          &error );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				if( ( result != 1 )
				 || ( process_buffer_size > (uint64_t) SSIZE_MAX ) )
				{
					process_buffer_size = 0;

					fprintf(
					 stderr,
					 "Unsupported process buffer size defaulting to: chunk size.\n" );
				}
				break;

			case (libcstring_system_integer_t) 'P':
				string_length = libcstring_system_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &input_size_variable,
				          &error );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				if( ( result != 1 )
				 || ( input_size_variable > (uint64_t) UINT32_MAX ) )
				{
					input_size_variable = 512;

					fprintf(
					 stderr,
					 "Unsupported amount of bytes per sector defaulting to: %" PRIu64 ".\n",
					 input_size_variable );
				}
				else
				{
					argument_set_bytes_per_sector = 1;
				}
				bytes_per_sector = (uint32_t) input_size_variable;

				break;

			case (libcstring_system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (libcstring_system_integer_t) 'r':
				string_length = libcstring_system_string_length(
				                 optarg );

				result = libsystem_string_to_uint64(
				          optarg,
				          string_length + 1,
				          &input_size_variable,
				          &error );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				if( ( result != 1 )
				 || ( input_size_variable > 255 ) )
				{
					input_size_variable = 2;

					fprintf(
					 stderr,
					 "Unsupported amount of read error retries defaulting to: %" PRIu64 ".\n",
					 input_size_variable );
				}
				read_error_retries = (uint8_t) input_size_variable;

				argument_set_read_error_retries = 1;

				break;

			case (libcstring_system_integer_t) 'R':
				resume_acquiry = 1;

				break;

			case (libcstring_system_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (libcstring_system_integer_t) 'S':
				string_length = libcstring_system_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &segment_file_size,
				          NULL );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
				argument_set_segment_file_size = 1;

				if( ( result != 1 )
				 || ( segment_file_size < EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE )
				 || ( ( ewf_format == LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT ) )
				 || ( ( ewf_format != LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT ) ) )
				{
					segment_file_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

					fprintf(
					 stderr,
					 "Unsupported segment file size defaulting to: %" PRIu64 ".\n",
					 segment_file_size );
				}
				break;

			case (libcstring_system_integer_t) 't':
				option_target_filename = optarg;

				break;

			case (libcstring_system_integer_t) 'u':
				interactive_mode = 0;

				break;

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				ewfoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'w':
				wipe_block_on_read_error              = 1;
				argument_set_wipe_block_on_read_error = 1;

				break;

			case (libcstring_system_integer_t) '2':
				option_secondary_target_filename = optarg;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	libsystem_notify_set_verbose(
	 verbose );
	libewf_notify_set_verbose(
	 verbose );
	libewf_notify_set_stream(
	 stderr,
	 NULL );

	/* Check if to read from stdin
	 */
	if( libcstring_system_string_compare(
	     argv[ optind ],
	     _LIBCSTRING_SYSTEM_STRING( "-" ),
	     1 ) == 0 )
	{
		fprintf(
		 stderr,
		 "Reading from stdin not supported.\n" );

		return( EXIT_FAILURE );
	}
	if( device_handle_initialize(
	     &ewfacquire_device_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create device handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	/* TODO test if argument is file or device
	 * move test into imaging handle ?
	 */

	/* Open the input file or device size
	 */
	if( device_handle_open_input(
	     ewfacquire_device_handle,
	     &( argv[ optind ] ),
	     argc - optind,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open file or device: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		device_handle_free(
		 &ewfacquire_device_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( device_handle_get_media_size(
	     ewfacquire_device_handle,
	     &media_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		device_handle_close(
		 ewfacquire_device_handle,
		 NULL );
		device_handle_free(
		 &ewfacquire_device_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( argument_set_bytes_per_sector == 0 )
	{
		if( device_handle_get_bytes_per_sector(
		     ewfacquire_device_handle,
		     &bytes_per_sector,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to retrieve bytes per sector.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			device_handle_close(
			 ewfacquire_device_handle,
			 NULL );
			device_handle_free(
			 &ewfacquire_device_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( argument_set_media_type == 0 )
	{
		 if( device_handle_get_media_type(
		      ewfacquire_device_handle,
		      &media_type,
		      &error ) != 1 )
		{
			media_type = LIBEWF_MEDIA_TYPE_FIXED;

			fprintf(
			 stderr,
			 "Unable to retrieve media type defaulting to: fixed.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( device_handle_media_information_fprint(
	     ewfacquire_device_handle,
	     stdout,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print media information.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	if( ( acquiry_size == 0 )
	 || ( acquiry_size > ( media_size - acquiry_offset ) ) )
	{
		acquiry_size = media_size - acquiry_offset;
	}
	/* Create the output targets
	 */
	if( option_target_filename != NULL )
	{
		string_length = libcstring_system_string_length(
				 option_target_filename );

		if( string_length > 0 )
		{
			target_filename_size = string_length + 1;

			target_filename = (libcstring_system_character_t *) memory_allocate(
			                                                     sizeof( libcstring_system_character_t ) * target_filename_size );

			if( target_filename == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create target filename string.\n" );

				error_abort = 1;
			}
			else if( libcstring_system_string_copy(
				  target_filename,
				  option_target_filename,
				  target_filename_size ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set target filename string.\n" );

				error_abort = 1;
			}
		}
	}
	else
	{
		target_filename_size = 1024;

		target_filename = (libcstring_system_character_t *) memory_allocate(
		                                                     sizeof( libcstring_system_character_t ) * target_filename_size );

		if( target_filename == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create target filename string.\n" );

			error_abort = 1;
		}
		/* Make sure to set the target filename if in unattended mode
		 */
		else if( interactive_mode == 0 )
		{
			if( libcstring_system_string_copy(
			     target_filename,
			     _LIBCSTRING_SYSTEM_STRING( "image" ),
			     7 ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set target filename string.\n" );

				error_abort = 1;
			}
			target_filename[ 7 ] = 0;
		}
	}
	if( option_secondary_target_filename != NULL )
	{
		string_length = libcstring_system_string_length(
				 option_secondary_target_filename );

		if( string_length > 0 )
		{
			secondary_target_filename_size = string_length + 1;

			secondary_target_filename = (libcstring_system_character_t *) memory_allocate(
			                                                               sizeof( libcstring_system_character_t ) * secondary_target_filename_size );

			if( secondary_target_filename == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create secondary target filename string.\n" );

				error_abort = 1;
			}
			else if( libcstring_system_string_copy(
				  secondary_target_filename,
				  option_secondary_target_filename,
				  secondary_target_filename_size ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set secondary target filename string.\n" );

				error_abort = 1;
			}
		}
		if( ( target_filename_size == secondary_target_filename_size )
		 && ( libcstring_system_string_compare(
		       target_filename,
		       secondary_target_filename,
		       target_filename_size ) == 0 ) )
		{
			fprintf(
			 stderr,
			 "Primary and secondary target cannot be the same.\n" );

			error_abort = 1;
		}
	}
	/* Create the input buffers
	 */
	if( error_abort == 0 )
	{
		if( option_case_number != NULL )
		{
			string_length = libcstring_system_string_length(
					 option_case_number );

			if( string_length > 0 )
			{
				case_number = (libcstring_system_character_t *) memory_allocate(
				                                                 sizeof( libcstring_system_character_t ) * ( string_length + 1 ) );

				if( case_number == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create case number string.\n" );

					error_abort = 1;
				}
				else if( libcstring_system_string_copy(
					  case_number,
					  option_case_number,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set case number string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			case_number = (libcstring_system_character_t *) memory_allocate(
			                                                 sizeof( libcstring_system_character_t ) * 256 );

			if( case_number == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create case number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_description != NULL )
		{
			string_length = libcstring_system_string_length(
					 option_description );

			if( string_length > 0 )
			{
				description = (libcstring_system_character_t *) memory_allocate(
				                                                 sizeof( libcstring_system_character_t ) * ( string_length + 1 ) );

				if( description == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create description string.\n" );

					error_abort = 1;
				}
				else if( libcstring_system_string_copy(
					  description,
					  option_description,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set description string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			description = (libcstring_system_character_t *) memory_allocate(
			                                                 sizeof( libcstring_system_character_t ) * 256 );

			if( description == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create description string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_examiner_name != NULL )
		{
			string_length = libcstring_system_string_length(
					 option_examiner_name );

			if( string_length > 0 )
			{
				examiner_name = (libcstring_system_character_t *) memory_allocate(
				                                                   sizeof( libcstring_system_character_t ) * ( string_length + 1 ) );

				if( examiner_name == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create examiner name string.\n" );

					error_abort = 1;
				}
				else if( libcstring_system_string_copy(
					  examiner_name,
					  option_examiner_name,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set examiner name string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			evidence_number = (libcstring_system_character_t *) memory_allocate(
			                                                     sizeof( libcstring_system_character_t ) * 256 );

			if( evidence_number == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create evidence number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_evidence_number != NULL )
		{
			string_length = libcstring_system_string_length(
					 option_evidence_number );

			if( string_length > 0 )
			{
				evidence_number = (libcstring_system_character_t *) memory_allocate(
				                                                     sizeof( libcstring_system_character_t ) * ( string_length + 1 ) );

				if( evidence_number == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create evidence number string.\n" );

					error_abort = 1;
				}
				else if( libcstring_system_string_copy(
					  evidence_number,
					  option_evidence_number,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set evidence number string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			examiner_name = (libcstring_system_character_t *) memory_allocate(
			                                                   sizeof( libcstring_system_character_t ) * 256 );

			if( examiner_name == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create examiner name string.\n" );

				error_abort = 1;
			}
		}
	}
	if( error_abort == 0 )
	{
		if( option_notes != NULL )
		{
			string_length = libcstring_system_string_length(
					 option_notes );

			if( string_length > 0 )
			{
				notes = (libcstring_system_character_t *) memory_allocate(
				                                           sizeof( libcstring_system_character_t ) * ( string_length + 1 ) );

				if( notes == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to create notes string.\n" );

					error_abort = 1;
				}
				else if( libcstring_system_string_copy(
					  notes,
					  option_notes,
					  string_length + 1 ) == NULL )
				{
					fprintf(
					 stderr,
					 "Unable to set notes string.\n" );

					error_abort = 1;
				}
			}
		}
		else
		{
			notes = (libcstring_system_character_t *) memory_allocate(
			                                           sizeof( libcstring_system_character_t ) * 256 );

			if( notes == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create notes string.\n" );

				error_abort = 1;
			}
		}
	}
	/* Set up the imaging handle
	 */
	if( imaging_handle_initialize(
	     &ewfacquire_imaging_handle,
	     calculate_md5,
	     calculate_sha1,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create imaging handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		error_abort = 1;
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
		if( secondary_target_filename != NULL )
		{
			memory_free(
			 secondary_target_filename );
		}
		if( target_filename != NULL )
		{
			memory_free(
			 target_filename );
		}
		device_handle_close(
		 ewfacquire_device_handle,
		 NULL );
		device_handle_free(
		 &ewfacquire_device_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Request the necessary case data
	 */
	while( ( interactive_mode == 1 )
	    && ( acquiry_parameters_confirmed == 0 ) )
	{
		fprintf(
		 stdout,
		 "Acquiry parameters required, please provide the necessary input\n" );

		/* Target filename
		 */
		if( resume_acquiry != 0 )
		{
			while( ewfinput_get_string_variable(
				stdout,
				_LIBCSTRING_SYSTEM_STRING( "Image path and filename with extension" ),
				target_filename,
				1024,
			        &error ) != 1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Filename is required, please try again or terminate using Ctrl^C.\n" );
			}
		}
		else if( option_target_filename == NULL )
		{
			while( ewfinput_get_string_variable(
				stdout,
				_LIBCSTRING_SYSTEM_STRING( "Image path and filename without extension" ),
				target_filename,
				1024,
			        &error ) != 1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Filename is required, please try again or terminate using Ctrl^C.\n" );
			}
		}
		if( resume_acquiry != 0 )
		{
			if( libsystem_signal_attach(
			     ewfacquire_signal_handler,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to attach signal handler.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );
			}
			if( imaging_handle_open_output(
			     ewfacquire_imaging_handle,
			     target_filename,
			     resume_acquiry,
			     &error ) != 1 )
			{
				fprintf(
				 stdout,
				 "Unable to resume acquire.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				resume_acquiry = 0;
			}
			if( libsystem_signal_detach(
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to detach signal handler.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );
			}
		}
		if( resume_acquiry != 0 )
		{
			if( imaging_handle_get_output_values(
			     ewfacquire_imaging_handle,
			     case_number,
			     256,
			     description,
			     256,
			     evidence_number,
			     256,
			     examiner_name,
			     256,
			     notes,
			     256,
			     &bytes_per_sector,
			     &acquiry_size,
			     &media_type,
			     &media_flags,
			     &compression_level,
			     &compression_flags,
			     &ewf_format,
			     &segment_file_size,
			     &sectors_per_chunk,
			     &sector_error_granularity,
			     &error ) != 1 )
			{
				fprintf(
				 stdout,
				 "Unable to determine previous acquiry parameters.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				imaging_handle_close(
				 ewfacquire_imaging_handle,
				 NULL );

				resume_acquiry = 0;
			}
		}
		if( resume_acquiry != 0 )
		{
			if( imaging_handle_get_offset(
			     ewfacquire_imaging_handle,
			     (off64_t *) &resume_acquiry_offset,
			     &error ) != 1 )
			{
				fprintf(
				 stdout,
				 "Unable to determine resume acquiry offset.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				imaging_handle_close(
				 ewfacquire_imaging_handle,
				 NULL );

				resume_acquiry = 0;
			}
		}
		if( resume_acquiry != 0 )
		{
			fprintf(
			 stdout,
			 "Resuming acquire at offset: %" PRIu64 ".\n",
			 resume_acquiry_offset );
		}
		if( resume_acquiry == 0 )
		{
			/* Case number
			 */
			if( ( option_case_number == NULL )
			 && ( ewfinput_get_string_variable(
			       stdout,
			       _LIBCSTRING_SYSTEM_STRING( "Case number" ),
			       case_number,
			       256,
			       &error ) == -1 ) )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Unable to set case number string.\n" );

				case_number[ 0 ] = 0;
			}
			/* Description
			 */
			if( ( option_description == NULL )
			 && ( ewfinput_get_string_variable(
			       stdout,
			       _LIBCSTRING_SYSTEM_STRING( "Description" ),
			       description,
			       256,
			       &error ) == -1 ) )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Unable to set description string.\n" );

				description[ 0 ] = 0;
			}
			/* Evidence number
			 */
			if( ( option_evidence_number == NULL )
			 && ( ewfinput_get_string_variable(
			       stdout,
			       _LIBCSTRING_SYSTEM_STRING( "Evidence number" ),
			       evidence_number,
			       256,
			       &error ) == -1 ) )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Unable to set evidence number string.\n" );

				evidence_number[ 0 ] = 0;
			}
			/* Examiner name
			 */
			if( ( option_examiner_name == NULL )
			 && ( ewfinput_get_string_variable(
			       stdout,
			       _LIBCSTRING_SYSTEM_STRING( "Examiner name" ),
			       examiner_name,
			       256,
			       &error ) == -1 ) )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Unable to set examiner name string.\n" );

				examiner_name[ 0 ] = 0;
			}
			/* Notes
			 */
			if( ( option_notes == NULL )
			 && ( ewfinput_get_string_variable(
			       stdout,
			       _LIBCSTRING_SYSTEM_STRING( "Notes" ),
			       notes,
			       256,
			       &error ) == -1 ) )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Unable to set notes string.\n" );

				notes[ 0 ] = 0;
			}
			/* Media type
			 */
			if( argument_set_media_type == 0 )
			{
				if( media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
				{
					default_media_type = 1;
				}
				else if( media_type == LIBEWF_MEDIA_TYPE_OPTICAL )
				{
					default_media_type = 2;
				}
				else if( media_type == LIBEWF_MEDIA_TYPE_MEMORY )
				{
					default_media_type = 3;
				}
				else
				{
					default_media_type = EWFINPUT_MEDIA_TYPES_DEFAULT;
				}
				if( ewfinput_get_fixed_string_variable(
				     stdout,
				     input_buffer,
				     EWFACQUIRE_INPUT_BUFFER_SIZE,
				     _LIBCSTRING_SYSTEM_STRING( "Media type" ),
				     ewfinput_media_types,
				     EWFINPUT_MEDIA_TYPES_AMOUNT,
				     (uint8_t) default_media_type,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unable to determine media type defaulting to: fixed.\n" );

					media_type = LIBEWF_MEDIA_TYPE_FIXED;
				}
				else if( ewfinput_determine_media_type(
					  fixed_string_variable,
					  &media_type,
				          &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unsupported media type defaulting to: fixed.\n" );

					media_type = LIBEWF_MEDIA_TYPE_FIXED;
				}
			}
			/* Media flags
			 */
			if( argument_set_media_flags == 0 )
			{
				default_media_flags = EWFINPUT_MEDIA_FLAGS_DEFAULT;

				if( ( media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
				 || ( media_type == LIBEWF_MEDIA_TYPE_OPTICAL ) )
				{
					default_media_flags = 0;
				}
				if( ewfinput_get_fixed_string_variable(
				     stdout,
				     input_buffer,
				     EWFACQUIRE_INPUT_BUFFER_SIZE,
				     _LIBCSTRING_SYSTEM_STRING( "Media characteristics" ),
				     ewfinput_media_flags,
				     EWFINPUT_MEDIA_FLAGS_AMOUNT,
				     (uint8_t) default_media_flags,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unable to determine media flags defaulting to: physical.\n" );

					media_flags = LIBEWF_MEDIA_FLAG_PHYSICAL;
				}
				else if( ewfinput_determine_media_flags(
					  fixed_string_variable,
					  &media_flags,
				          &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unsupported media flags defaulting to: physical.\n" );

					media_flags = LIBEWF_MEDIA_FLAG_PHYSICAL;
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
				     _LIBCSTRING_SYSTEM_STRING( "Use compression" ),
				     ewfinput_compression_levels,
				     EWFINPUT_COMPRESSION_LEVELS_AMOUNT,
				     EWFINPUT_COMPRESSION_LEVELS_DEFAULT,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unable to determine compression type defaulting to: none.\n" );

					compression_level = LIBEWF_COMPRESSION_NONE;
					compression_flags = 0;
				}
				else if( ewfinput_determine_compression_level(
					  fixed_string_variable,
					  &compression_level,
					  &compression_flags,
				          &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unsupported compression type defaulting to: none.\n" );

					compression_level = LIBEWF_COMPRESSION_NONE;
					compression_flags = 0;
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
				     _LIBCSTRING_SYSTEM_STRING( "Use EWF file format" ),
				     ewfinput_format_types,
				     EWFINPUT_FORMAT_TYPES_AMOUNT,
				     EWFINPUT_FORMAT_TYPES_DEFAULT,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unable to determine EWF file format type defaulting to: encase6.\n" );

					ewf_format = LIBEWF_FORMAT_ENCASE6;
				}
				else if( ewfinput_determine_ewf_format(
					  fixed_string_variable,
					  &ewf_format,
				          &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unsupported EWF file format type defaulting to: encase6.\n" );

					ewf_format = LIBEWF_FORMAT_ENCASE6;
				}
			}
		}
		if( ( resume_acquiry == 0 )
		 || ( acquiry_size != media_size ) )
		{
			/* Offset of data to acquire
			 */
			if( ( argument_set_offset == 0 )
			 && ( ewfinput_get_size_variable(
			       stdout,
			       input_buffer,
			       EWFACQUIRE_INPUT_BUFFER_SIZE,
			       _LIBCSTRING_SYSTEM_STRING( "Start to acquire at offset" ),
			       0,
			       media_size,
			       0,
			       &acquiry_offset,
			       &error ) == -1 ) )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				acquiry_offset = 0;

				fprintf(
				 stdout,
				 "Unable to determine acquiry offset defaulting to: %" PRIu64 ".\n",
				 acquiry_offset );
			}
		}
		if( resume_acquiry == 0 )
		{
			/* Size of data to acquire
			 */
			if( ( argument_set_size == 0 )
			 && ( ewfinput_get_size_variable(
			       stdout,
			       input_buffer,
			       EWFACQUIRE_INPUT_BUFFER_SIZE,
			       _LIBCSTRING_SYSTEM_STRING( "The amount of bytes to acquire" ),
			       0,
			       ( media_size - acquiry_offset ),
			       ( media_size - acquiry_offset ),
			       &acquiry_size,
			       &error ) == -1 ) )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				acquiry_size = media_size - acquiry_offset;

				fprintf(
				 stdout,
				 "Unable to determine input size defaulting to: %" PRIu64 ".\n",
				 acquiry_size );
			}
			/* Segment file size
			 */
			if( argument_set_segment_file_size == 0 )
			{
				if( ewf_format == LIBEWF_FORMAT_ENCASE6 )
				{
					maximum_segment_file_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT;
				}
				else
				{
					maximum_segment_file_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT;
				}
				if( segment_file_size == 0 )
				{
					segment_file_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;
				}
				if( ewfinput_get_byte_size_variable(
				     stdout,
				     input_buffer,
				     EWFACQUIRE_INPUT_BUFFER_SIZE,
				     _LIBCSTRING_SYSTEM_STRING( "Evidence segment file size in bytes" ),
				     EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
				     maximum_segment_file_size,
				     segment_file_size,
				     &segment_file_size,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					segment_file_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

					fprintf(
					 stdout,
					 "Unable to determine segment file size defaulting to: %" PRIu64 ".\n",
					 segment_file_size );
				}
				/* Make sure the segment file size is smaller than or equal to the maximum
				 */
				if( segment_file_size > maximum_segment_file_size )
				{
					segment_file_size = maximum_segment_file_size;
				}
			}
			/* Bytes per sector
			 */
			if( argument_set_bytes_per_sector == 0 )
			{
				if( ewfinput_get_size_variable(
				     stdout,
				     input_buffer,
				     EWFACQUIRE_INPUT_BUFFER_SIZE,
				     _LIBCSTRING_SYSTEM_STRING( "The amount of bytes per sector" ),
				     0,
				     UINT32_MAX,
				     bytes_per_sector,
				     &input_size_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					input_size_variable = 512;

					fprintf(
					 stdout,
					 "Unable to determine bytes per sector defaulting to: %" PRIu64 ".\n",
					 input_size_variable );
				}
				bytes_per_sector = (uint32_t) input_size_variable;
			}
			/* Chunk size (sectors per block)
			 */
			if( argument_set_sectors_per_chunk == 0 )
			{
				if( ewfinput_get_fixed_string_variable(
				     stdout,
				     input_buffer,
				     EWFACQUIRE_INPUT_BUFFER_SIZE,
				     _LIBCSTRING_SYSTEM_STRING( "The amount of sectors to read at once" ),
				     ewfinput_sector_per_block_sizes,
				     EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT,
				     EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unable to determine sectors per chunk defaulting to: 64.\n" );

					sectors_per_chunk = 64;
				}
				else if( ewfinput_determine_sectors_per_chunk(
					  fixed_string_variable,
					  &sectors_per_chunk,
				          &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stdout,
					 "Unsupported sectors per chunk defaulting to: 64.\n" );

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
				     _LIBCSTRING_SYSTEM_STRING( "The amount of sectors to be used as error granularity" ),
				     1,
				     (uint64_t) sectors_per_chunk,
				     64,
				     &input_size_variable,
				     &error ) == -1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					input_size_variable = 64;

					fprintf(
					 stdout,
					 "Unable to determine sector error granularity defaulting to: %" PRIu64 ".\n",
					 input_size_variable );
				}
				sector_error_granularity = (uint32_t) input_size_variable;
			}
		}
		/* The amount of read error retries
		 */
		if( argument_set_read_error_retries == 0 )
		{
			if( ewfinput_get_size_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _LIBCSTRING_SYSTEM_STRING( "The amount of retries when a read error occurs" ),
			     0,
			     255,
			     2,
			     &input_size_variable,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				input_size_variable = 2;

				fprintf(
				 stdout,
				 "Unable to determine read error retry defaulting to: %" PRIu64 ".\n",
				 input_size_variable );
			}
			read_error_retries = (uint8_t) input_size_variable;
		}
		/* Wipe the sector on error
		 */
		if( argument_set_wipe_block_on_read_error == 0 )
		{
			if( ewfinput_get_fixed_string_variable(
			     stdout,
			     input_buffer,
			     EWFACQUIRE_INPUT_BUFFER_SIZE,
			     _LIBCSTRING_SYSTEM_STRING( "Wipe sectors on read error (mimic EnCase like behavior)" ),
			     ewfinput_yes_no,
			     2,
			     1,
			     &fixed_string_variable,
			     &error ) == -1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Unable to determine wipe chunk on error defaulting to: no.\n" );

				wipe_block_on_read_error = 0;
			}
			else if( ewfinput_determine_yes_no(
				  fixed_string_variable,
				  &wipe_block_on_read_error,
			          &error ) != 1 )
			{
				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stdout,
				 "Unsupported wipe chunk on error defaulting to: no.\n" );

				wipe_block_on_read_error = 0;
			}
		}
		fprintf(
		 stdout,
		 "\n" );

		/* Check if user is content with the acquiry parameters
		 */
		acquiry_parameters_confirmed = ewfacquire_confirm_acquiry_parameters(
		                                stdout,
		                                input_buffer,
		                                EWFACQUIRE_INPUT_BUFFER_SIZE,
		                                resume_acquiry,
		                                target_filename,
		                                secondary_target_filename,
		                                case_number,
		                                description,
		                                evidence_number,
		                                examiner_name,
		                                notes,
		                                media_type,
		                                media_flags,
		                                compression_level,
		                                compression_flags,
		                                ewf_format,
		                                (off64_t) acquiry_offset,
		                                resume_acquiry_offset,
		                                (size64_t) acquiry_size,
		                                (size64_t) segment_file_size,
		                                bytes_per_sector,
		                                sectors_per_chunk,
		                                sector_error_granularity,
		                                read_error_retries,
		                                wipe_block_on_read_error,
		                                &error );

		if( acquiry_parameters_confirmed == -1 )
		{
			fprintf(
			 stdout,
			 "Unable to determine if acquiry parameters are correct aborting.\n" );

			ewfacquire_abort = 1;

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
			argument_set_read_error_retries       = 0;
			argument_set_sector_error_granularity = 0;
			argument_set_sectors_per_chunk        = 0;
			argument_set_segment_file_size        = 0;
			argument_set_size                     = 0;
			argument_set_media_flags              = 0;
			argument_set_wipe_block_on_read_error = 0;

			if( resume_acquiry != 0 )
			{
				if( imaging_handle_close(
				     ewfacquire_imaging_handle,
				     &error ) != 0 )
				{
					fprintf(
					 stdout,
					 "Unable to close output file(s).\n" );

					ewfacquire_abort = 1;

					break;
				}
			}
		}
		else if( acquiry_size > EWFACQUIRE_2_TIB )
		{
			if( ( ewf_format != LIBEWF_FORMAT_ENCASE6 )
			 && ( ewf_format != LIBEWF_FORMAT_EWFX ) )
			{
				fprintf(
				 stdout,
				 "Cannot acquire more than 2 TiB in selected ewf format.\n" );

				acquiry_parameters_confirmed = 0;
			}
		}
	}
	if( libsystem_signal_attach(
	     ewfacquire_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	if( resume_acquiry == 0 )
	{
		if( ewfacquire_abort == 0 )
		{
			if( platform_get_operating_system(
			     acquiry_operating_system,
			     32,
			     &error ) != 1 )
			{
				fprintf(
				 stdout,
				 "Unable to determine operating system.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				acquiry_operating_system[ 0 ] = 0;
			}
			acquiry_software_version = _LIBCSTRING_SYSTEM_STRING( LIBEWF_VERSION_STRING );

			if( device_handle_get_information_value(
			     ewfacquire_device_handle,
			     (uint8_t *) "model",
			     5,
			     media_information_model,
			     64,
			     &error ) != 1 )
			{
				fprintf(
				 stdout,
				 "Unable to retrieve model.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				media_information_model[ 0 ] = 0;
			}
			if( device_handle_get_information_value(
			     ewfacquire_device_handle,
			     (uint8_t *) "serial_number",
			     13,
			     media_information_serial_number,
			     64,
			     &error ) != 1 )
			{
				fprintf(
				 stdout,
				 "Unable to retrieve serial number.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				media_information_serial_number[ 0 ] = 0;
			}
		}
		if( ( ewfacquire_abort == 0 )
		 && ( error_abort == 0 ) )
		{
			if( imaging_handle_open_output(
			     ewfacquire_imaging_handle,
			     target_filename,
			     resume_acquiry,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to open output file(s).\n" );

				error_abort = 1;
			}
		}
		if( ( ewfacquire_abort == 0 )
		 && ( error_abort == 0 ) )
		{
			if( secondary_target_filename != NULL )
			{
				if( imaging_handle_open_secondary_output(
				     ewfacquire_imaging_handle,
				     secondary_target_filename,
				     resume_acquiry,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unable to open secondary output file(s).\n" );

					imaging_handle_close(
					 ewfacquire_imaging_handle,
					 NULL );

					error_abort = 1;
				}
			}
		}
		if( ( ewfacquire_abort == 0 )
		 && ( error_abort == 0 ) )
		{
			if( device_handle_set_error_values(
			     ewfacquire_device_handle,
			     read_error_retries,
			     sector_error_granularity * bytes_per_sector,
			     wipe_block_on_read_error,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to initialize output settings.\n" );

				imaging_handle_close(
				 ewfacquire_imaging_handle,
				 NULL );

				error_abort = 1;
			}
		}
		if( ( ewfacquire_abort == 0 )
		 && ( error_abort == 0 ) )
		{
			if( imaging_handle_set_output_values(
			     ewfacquire_imaging_handle,
			     case_number,
			     libcstring_system_string_length(
			      case_number ),
			     description,
			     libcstring_system_string_length(
			      description ),
			     evidence_number,
			     libcstring_system_string_length(
			      evidence_number ),
			     examiner_name,
			     libcstring_system_string_length(
			      examiner_name ),
			     notes,
			     libcstring_system_string_length(
			      notes ),
			     acquiry_operating_system,
			     libcstring_system_string_length(
			      acquiry_operating_system ),
			     program,
			     libcstring_system_string_length(
			      program ),
			     acquiry_software_version,
			     libcstring_system_string_length(
			      acquiry_software_version ),
			     media_information_model,
			     libcstring_system_string_length(
			      media_information_model ),
			     media_information_serial_number,
			     libcstring_system_string_length(
			      media_information_serial_number ),
			     header_codepage,
			     bytes_per_sector,
			     acquiry_size,
			     media_type,
			     media_flags,
			     compression_level,
			     compression_flags,
			     ewf_format,
			     segment_file_size,
			     sectors_per_chunk,
			     sector_error_granularity,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to initialize output settings.\n" );

				imaging_handle_close(
				 ewfacquire_imaging_handle,
				 NULL );

				error_abort = 1;
			}
		}
		if( ( ewfacquire_abort == 0 )
		 && ( error_abort == 0 ) )
		{
			/* TODO for now just fake one session
			 */
			if( media_type == LIBEWF_MEDIA_TYPE_OPTICAL )
			{
				if( media_size > (size_t) UINT32_MAX )
				{
					fprintf(
					 stderr,
					 "Invalid media size value out of range to add session.\n" );

					imaging_handle_close(
					 ewfacquire_imaging_handle,
					 NULL );

					error_abort = 1;
				}
				if( imaging_handle_add_session(
				     ewfacquire_imaging_handle,
				     bytes_per_sector,
				     media_size,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unable to add session.\n" );

					imaging_handle_close(
					 ewfacquire_imaging_handle,
					 NULL );

					error_abort = 1;
				}
			}
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

	if( secondary_target_filename != NULL )
	{
		memory_free(
		 secondary_target_filename );
	}
	memory_free(
	 target_filename );

	if( error_abort != 0 )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		imaging_handle_free(
		 &ewfacquire_imaging_handle,
		 NULL );

		device_handle_close(
		 ewfacquire_device_handle,
		 NULL );
		device_handle_free(
		 &ewfacquire_device_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (libcstring_system_character_t *) memory_allocate(
		                                                                sizeof( libcstring_system_character_t ) * DIGEST_HASH_STRING_SIZE_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated MD5 hash string.\n" );

			imaging_handle_close(
			 ewfacquire_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &ewfacquire_imaging_handle,
			 NULL );

			device_handle_close(
			 ewfacquire_device_handle,
			 NULL );
			device_handle_free(
			 &ewfacquire_device_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( calculate_sha1 == 1 )
	{
		calculated_sha1_hash_string = (libcstring_system_character_t *) memory_allocate(
		                                                                 sizeof( libcstring_system_character_t ) * DIGEST_HASH_STRING_SIZE_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated SHA1 hash string.\n" );

			memory_free(
			 calculated_md5_hash_string );

			imaging_handle_close(
			 ewfacquire_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &ewfacquire_imaging_handle,
			 NULL );

			device_handle_close(
			 ewfacquire_device_handle,
			 NULL );
			device_handle_free(
			 &ewfacquire_device_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( ewfacquire_abort == 0 )
	{
		if( process_status_initialize(
		     &process_status,
		     _LIBCSTRING_SYSTEM_STRING( "Acquiry" ),
		     _LIBCSTRING_SYSTEM_STRING( "acquired" ),
		     _LIBCSTRING_SYSTEM_STRING( "Written" ),
		     stdout,
		     print_status_information,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to initialize process status.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

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
			imaging_handle_close(
			 ewfacquire_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &ewfacquire_imaging_handle,
			 NULL );

			device_handle_close(
			 ewfacquire_device_handle,
			 NULL );
			device_handle_free(
			 &ewfacquire_device_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		if( process_status_start(
		     process_status,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to start process status.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			process_status_free(
			 &process_status,
			 NULL );

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
			imaging_handle_close(
			 ewfacquire_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &ewfacquire_imaging_handle,
			 NULL );

			device_handle_close(
			 ewfacquire_device_handle,
			 NULL );
			device_handle_free(
			 &ewfacquire_device_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		/* Start acquiring data
		 */
		read_count = ewfacquire_read_input(
		              ewfacquire_imaging_handle,
		              ewfacquire_device_handle,
		              media_size,
		              (size64_t) acquiry_size,
		              (off64_t) acquiry_offset,
		              resume_acquiry_offset,
		              bytes_per_sector,
		              swap_byte_pairs,
		              sector_error_granularity,
		              (size_t) process_buffer_size,
		              calculated_md5_hash_string,
		              DIGEST_HASH_STRING_SIZE_MD5,
		              calculated_sha1_hash_string,
		              DIGEST_HASH_STRING_SIZE_SHA1,
		              process_status,
		              &error );

		if( read_count <= -1 )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			status = PROCESS_STATUS_FAILED;
		}
		else
		{
			status = PROCESS_STATUS_COMPLETED;
		}
	}
	if( ewfacquire_abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	/* Done acquiring data
	 */
	if( process_status_stop(
	     process_status,
	     (size64_t) read_count,
	     status,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to stop process status.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		process_status_free(
		 &process_status,
		 NULL );

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
		imaging_handle_close(
		 ewfacquire_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &ewfacquire_imaging_handle,
		 NULL );

		device_handle_close(
		 ewfacquire_device_handle,
		 NULL );
		device_handle_free(
		 &ewfacquire_device_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( process_status_free(
	     &process_status,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free process status.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

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
		imaging_handle_close(
		 ewfacquire_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &ewfacquire_imaging_handle,
		 NULL );

		device_handle_close(
		 ewfacquire_device_handle,
		 NULL );
		device_handle_free(
		 &ewfacquire_device_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( status == PROCESS_STATUS_COMPLETED )
	{
		if( log_filename != NULL )
		{
			if( log_handle_initialize(
			     &log_handle,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to create log handle.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );
			}
			else if( log_handle_open(
			          log_handle,
			          log_filename,
			          &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to open log file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
				 log_filename );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				log_handle_free(
				 &log_handle,
				 NULL );
			}
		}
		if( device_handle_read_errors_fprint(
		     ewfacquire_device_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print device read errors.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
		if( log_handle != NULL )
		{
			if( device_handle_read_errors_fprint(
			    ewfacquire_device_handle,
			    log_handle->log_stream,
			    &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to write device read errors in log file.\n" );

				libsystem_notify_print_error_backtrace(
				 error );
				liberror_error_free(
				 &error );
			}
		}
	}
	if( imaging_handle_close(
	     ewfacquire_imaging_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close output file(s).\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		if( log_handle != NULL )
		{
			log_handle_close(
			 log_handle,
			 NULL );
			log_handle_free(
			 &log_handle,
			 NULL );
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
		imaging_handle_free(
		 &ewfacquire_imaging_handle,
		 NULL );

		device_handle_close(
		 ewfacquire_device_handle,
		 NULL );
		device_handle_free(
		 &ewfacquire_device_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( imaging_handle_free(
	     &ewfacquire_imaging_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free imaging handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		if( log_handle != NULL )
		{
			log_handle_close(
			 log_handle,
			 NULL );
			log_handle_free(
			 &log_handle,
			 NULL );
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
		device_handle_close(
		 ewfacquire_device_handle,
		 NULL );
		device_handle_free(
		 &ewfacquire_device_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( device_handle_close(
	     ewfacquire_device_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close input file or device.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		process_status_free(
		 &process_status,
		 NULL );

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
		device_handle_free(
		 &ewfacquire_device_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( device_handle_free(
	     &ewfacquire_device_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free device handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		process_status_free(
		 &process_status,
		 NULL );

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
	if( libsystem_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
        if( status != PROCESS_STATUS_COMPLETED )
        {
		if( log_handle != NULL )
		{
			log_handle_close(
			 log_handle,
			 NULL );
			log_handle_free(
			 &log_handle,
			 NULL );
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
		fprintf(
		 stdout,
		 "MD5 hash calculated over data:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
		 calculated_md5_hash_string );

		if( log_handle != NULL )
		{
			log_handle_printf(
			 log_handle,
			 "MD5 hash calculated over data:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
			 calculated_md5_hash_string );
		}
		memory_free(
		 calculated_md5_hash_string );
	}
	if( calculate_sha1 == 1 )
	{
		fprintf(
		 stdout,
		 "SHA1 hash calculated over data:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
		 calculated_sha1_hash_string );

		if( log_handle != NULL )
		{
			log_handle_printf(
			 log_handle,
			 "SHA1 hash calculated over data:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
			 calculated_sha1_hash_string );
		}
		memory_free(
		 calculated_sha1_hash_string );
	}
	if( log_handle != NULL )
	{
		if( log_handle_close(
		     log_handle,
		     &error ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to close log file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
			 log_filename );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			log_handle_free(
			 &log_handle,
			 NULL );
		}
		else if( log_handle_free(
		          &log_handle,
		          &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to free log handle.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	return( EXIT_SUCCESS );
}

