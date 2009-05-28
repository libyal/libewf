/*
 * ewfexport
 * Export media data from EWF files to a file
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

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include <libewf.h>

#include "byte_size_string.h"
#include "digest_context.h"
#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewfsignal.h"
#include "export_handle.h"
#include "glob.h"
#include "md5.h"
#include "notify.h"
#include "platform.h"
#include "process_status.h"
#include "sha1.h"
#include "storage_media_buffer.h"
#include "system_string.h"

#define EWFEXPORT_INPUT_BUFFER_SIZE	64

export_handle_t *ewfexport_export_handle = NULL;
int ewfexport_abort                      = 0;

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
	fprintf( stream, "Use ewfexport to export data from the EWF format (Expert Witness Compression\n"
	                 "Format) to raw data or another EWF format.\n\n" );

	fprintf( stream, "Usage: ewfexport [ -A codepage ] [ -b amount_of_sectors ] [ -B amount_of_bytes ]\n"
	                 "                 [ -c compression_type ] [ -d digest_type ] [ -f format ]\n"
	                 "                 [ -l log_filename ] [ -o offset ] [ -p process_buffer_size ]\n"
	                 "                 [ -S segment_file_size ] [ -t target ] [ -hqsuvVw ] ewf_files\n\n" );

	fprintf( stream, "\tewf_files: the first or the entire set of EWF segment files\n\n" );

	fprintf( stream, "\t-A:        codepage of header section, options: ascii (default), windows-1250,\n"
	                 "\t           windows-1251, windows-1252, windows-1253, windows-1254,\n"
	                 "\t           windows-1255, windows-1256, windows-1257, windows-1258\n" );
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
	fprintf( stream, "\t-l:        logs export errors and the digest (hash) to the log_filename\n" );
	fprintf( stream, "\t-o:        specify the offset to start the export (default is 0)\n" );
	fprintf( stream, "\t-p:        specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-q:        quiet shows no status information\n" );
	fprintf( stream, "\t-s:        swap byte pairs of the media data (from AB to BA)\n"
	                 "\t           (use this for big to little endian conversion and vice versa)\n" );

	if( result == 1 )
	{
		fprintf( stream, "\t-S:        specify the segment file size in bytes (default is %" PRIs_SYSTEM ")\n"
		                 "\t           (minimum is %" PRIs_SYSTEM ", maximum is %" PRIs_SYSTEM " for encase6 format\n"
		                 "\t           and %" PRIs_SYSTEM " for other formats)\n"
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

/* Reads the media data and exports it
 * Returns a -1 on error, the amount of bytes read on success
 */
ssize64_t ewfexport_read_input(
           export_handle_t *export_handle,
           size64_t media_size,
           size64_t export_size,
           off64_t read_offset,
           system_character_t *calculated_md5_hash_string,
           size_t calculated_md5_hash_string_size,
           system_character_t *calculated_sha1_hash_string,
           size_t calculated_sha1_hash_string_size,
           uint8_t swap_byte_pairs,
           size_t process_buffer_size,
           process_status_t *process_status,
	   liberror_error_t **error )
{
	storage_media_buffer_t *storage_media_buffer        = NULL;
	static char *function                               = "ewfexport_read_input";
	ssize64_t export_count                              = 0;
	size32_t input_chunk_size                           = 0;
	size_t read_size                                    = 0;
	ssize_t process_count                               = 0;
	ssize_t read_count                                  = 0;
	ssize_t write_count                                 = 0;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	storage_media_buffer_t *output_storage_media_buffer = NULL;
	size32_t output_chunk_size                          = 0;
#endif

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
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
	if( export_handle_get_input_chunk_size(
	     export_handle,
	     &input_chunk_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine the input chunk size.",
		 function );

		return( -1 );
	}
	if( input_chunk_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid input chunk size.",
		 function );

		return( -1 );
	}
	if( ( export_size == 0 )
	 || ( export_size > media_size )
	 || ( export_size > (ssize64_t) INT64_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid size value out of range.",
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
	if( read_offset > 0 )
	{
		if( read_offset >= (off64_t) media_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid offset.",
			 function );

			return( -1 );
		}
		if( ( export_size + read_offset ) > media_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: unable to export beyond size of media.",
			 function );

			return( -1 );
		}
		if( export_handle_seek_offset(
		     export_handle,
		     read_offset,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			"%s: unable to seek offset.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	process_buffer_size = (size_t) input_chunk_size;
#else
	if( process_buffer_size == 0 )
	{
		process_buffer_size = (size_t) input_chunk_size;
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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( export_handle_get_output_chunk_size(
	     export_handle,
	     &output_chunk_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine the output chunk size.",
		 function );

		storage_media_buffer_free(
		 &storage_media_buffer,
		 NULL );

		return( -1 );
	}
	if( output_chunk_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid output chunk size.",
		 function );

		storage_media_buffer_free(
		 &storage_media_buffer,
		 NULL );

		return( -1 );
	}
	if( storage_media_buffer_initialize(
	     &output_storage_media_buffer,
	     output_chunk_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create output storage media buffer.",
		 function );

		storage_media_buffer_free(
		 &storage_media_buffer,
		 NULL );

		return( -1 );
	}
#endif
	while( export_count < (int64_t) export_size )
	{
		read_size = process_buffer_size;

		if( ( media_size - export_count ) < read_size )
		{
			read_size = (size_t) ( media_size - export_count );
		}
		read_count = export_handle_read_buffer(
		              export_handle,
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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer_free(
			 &output_storage_media_buffer,
			 NULL );
#endif

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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer_free(
			 &output_storage_media_buffer,
			 NULL );
#endif

			return( -1 );
		}
		process_count = export_handle_prepare_read_buffer(
		                 export_handle,
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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer_free(
			 &output_storage_media_buffer,
			 NULL );
#endif

			return( -1 );
		}
		if( process_count > (ssize_t) read_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: more bytes read than requested.",
			 function );

			storage_media_buffer_free(
			 &storage_media_buffer,
			 NULL );
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer_free(
			 &output_storage_media_buffer,
			 NULL );
#endif

			return( -1 );
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		/* Set the amount of chunk data in the buffer
		 */
		if( storage_media_buffer->data_in_compression_buffer == 1 )
		{
			storage_media_buffer->compression_buffer_amount = process_count;
		}
#endif
		/* Swap byte pairs
		 */
		if( ( swap_byte_pairs == 1 )
		 && ( export_handle_swap_byte_pairs(
		       export_handle,
		       storage_media_buffer,
		       process_count,
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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer_free(
			 &output_storage_media_buffer,
			 NULL );
#endif

			return( -1 );
		}
		/* Digest hashes are calcultated after swap
		 */
		if( export_handle_update_integrity_hash(
		     export_handle,
		     storage_media_buffer,
		     process_count,
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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer_free(
			 &output_storage_media_buffer,
			 NULL );
#endif

			return( -1 );
		}
		export_count += process_count;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		/* TODO implement use of output storage media buffer
		 */

		/* if the data is in compression buffer move data to raw buffer
		 */
		if( storage_media_buffer->data_in_compression_buffer == 1 )
		{
			if( memory_copy(
			     storage_media_buffer->raw_buffer,
			     storage_media_buffer->compression_buffer,
			     storage_media_buffer->compression_buffer_amount ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to data from compression buffer to raw buffer.",
				 function );

				return( -1 );
			}
			storage_media_buffer->data_in_compression_buffer = 0;
			storage_media_buffer->raw_buffer_amount          = storage_media_buffer->compression_buffer_amount;
		}
		process_count = export_handle_prepare_write_buffer(
		                 export_handle,
		                 storage_media_buffer,
		                 error );
#else
		process_count = export_handle_prepare_write_buffer(
		                 export_handle,
		                 storage_media_buffer,
		                 error );
#endif

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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer_free(
			 &output_storage_media_buffer,
			 NULL );
#endif

			return( -1 );
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		/* TODO implement use of output storage media buffer
		 */
		write_count = export_handle_write_buffer(
		               export_handle,
		               storage_media_buffer,
		               process_count,
		               error );
#else
		write_count = export_handle_write_buffer(
		               export_handle,
		               storage_media_buffer,
		               process_count,
		               error );
#endif

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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer_free(
			 &output_storage_media_buffer,
			 NULL );
#endif

			return( -1 );
		}
		 if( process_status_update(
		      process_status,
		      (size64_t) export_count,
		      export_size,
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
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			storage_media_buffer_free(
			 &output_storage_media_buffer,
			 NULL );
#endif

			return( -1 );
		}
		if( ewfexport_abort != 0 )
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

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		storage_media_buffer_free(
		 &output_storage_media_buffer,
		 NULL );
#endif

		return( -1 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( storage_media_buffer_free(
	     &output_storage_media_buffer,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free output storage media buffer.",
		 function );

		return( -1 );
	}
#endif
	write_count = export_handle_finalize(
	               export_handle,
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
	return( export_count );
}

/* Signal handler for ewfexport
 */
void ewfexport_signal_handler(
      ewfsignal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "ewfexport_signal_handler";

	ewfexport_abort = 1;

	if( ( ewfexport_export_handle != NULL )
	 && ( export_handle_signal_abort(
	       ewfexport_export_handle,
	       &error ) != 1 ) )
	{
		notify_warning_printf(
		 "%s: unable to signal export handle to abort.\n",
		 function );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return;
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( file_io_close(
	     0 ) != 0 )
	{
		notify_warning_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
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
	system_character_t input_buffer[ EWFEXPORT_INPUT_BUFFER_SIZE ];

	system_character_t * const *argv_filenames      = NULL;

	export_handle_t *export_handle                  = NULL;

#if !defined( HAVE_GLOB_H )
	glob_t *glob                                    = NULL;
#endif
	liberror_error_t *error                         = NULL;

	process_status_t *process_status                = NULL;

	system_character_t *acquiry_software_version    = NULL;
	system_character_t *calculated_md5_hash_string  = NULL;
	system_character_t *calculated_sha1_hash_string = NULL;
	system_character_t *fixed_string_variable       = NULL;
	system_character_t *log_filename                = NULL;
	system_character_t *option_target_filename      = NULL;
	system_character_t *program                     = _SYSTEM_CHARACTER_T_STRING( "ewfexport" );
	system_character_t *target_filename             = NULL;

	FILE *log_file_stream                           = NULL;

	system_integer_t option                         = 0;
	size64_t media_size                             = 0;
	ssize64_t export_count                          = 0;
	size_t string_length                            = 0;
	uint64_t export_offset                          = 0;
	uint64_t export_size                            = 0;
	uint64_t maximum_segment_file_size              = 0;
	uint64_t process_buffer_size                    = EWFCOMMON_PROCESS_BUFFER_SIZE;
	uint64_t segment_file_size                      = 0;
	uint32_t sectors_per_chunk                      = 64;
	uint8_t calculate_md5                           = 1;
	uint8_t calculate_sha1                          = 0;
	uint8_t compression_flags                       = 0;
	uint8_t export_handle_output_format             = 0;
	uint8_t ewf_format                              = LIBEWF_FORMAT_ENCASE6;
	uint8_t print_status_information                = 1;
	uint8_t swap_byte_pairs                         = 0;
	uint8_t wipe_chunk_on_error                     = 0;
	uint8_t verbose                                 = 0;
	int8_t compression_level                        = LIBEWF_COMPRESSION_NONE;
	int amount_of_filenames                         = 0;
	int argument_set_compression                    = 0;
	int argument_set_format                         = 0;
	int argument_set_offset                         = 0;
	int argument_set_sectors_per_chunk              = 0;
	int argument_set_segment_file_size              = 0;
	int argument_set_size                           = 0;
	int error_abort                                 = 0;
	int header_codepage                             = LIBEWF_CODEPAGE_ASCII;
	int interactive_mode                            = 1;
	int output_raw                                  = 1;
	int result                                      = 1;
	int status                                      = 0;

	system_character_t *ewfexport_format_types[ 13 ] = \
	 { _SYSTEM_CHARACTER_T_STRING( "raw" ),
	   _SYSTEM_CHARACTER_T_STRING( "ewf" ),
	   _SYSTEM_CHARACTER_T_STRING( "smart" ),
	   _SYSTEM_CHARACTER_T_STRING( "ftk" ),
	   _SYSTEM_CHARACTER_T_STRING( "encase1" ),
	   _SYSTEM_CHARACTER_T_STRING( "encase2" ),
	   _SYSTEM_CHARACTER_T_STRING( "encase3" ),
	   _SYSTEM_CHARACTER_T_STRING( "encase4" ),
	   _SYSTEM_CHARACTER_T_STRING( "encase5" ),
	   _SYSTEM_CHARACTER_T_STRING( "encase6" ),
	   _SYSTEM_CHARACTER_T_STRING( "linen5" ),
	   _SYSTEM_CHARACTER_T_STRING( "linen6" ),
	   _SYSTEM_CHARACTER_T_STRING( "ewfx" ) };

	notify_set_values(
	 stderr,
	 1 );

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
	 stderr,
	 program );

#if defined( WINAPI )
	if( _setmode(
	     _fileno(
	      stdout ),
	     _O_BINARY ) == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to set stdout to binary mode.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
#endif

	while( ( option = ewfgetopt(
	                   argc,
	                   argv,
	                   _SYSTEM_CHARACTER_T_STRING( "A:b:B:c:d:f:hl:o:p:qsS:t:uvVw" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind ] );

				usage_fprint(
				 stderr );

				return( EXIT_FAILURE );

			case (system_integer_t) 'A':
				if( ewfinput_determine_header_codepage(
				     optarg,
				     &header_codepage,
				     &error ) != 1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unsuported header codepage defaulting to: ascii.\n" );

					header_codepage = LIBEWF_CODEPAGE_ASCII;
				}
				break;

			case (system_integer_t) 'b':
				if( ewfinput_determine_sectors_per_chunk(
				     optarg,
				     &sectors_per_chunk,
				     &error ) != 1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unsupported amount of sectors per chunk defaulting to: 64.\n" );

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

				if( system_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &export_size,
				     NULL ) != 1 )
				{
					export_size = 0;

					fprintf(
					 stderr,
					 "Unsupported export size defaulting to: all bytes.\n" );
				}
				argument_set_size = 1;

				break;

			case (system_integer_t) 'c':
				if( ewfinput_determine_compression_level(
				     optarg,
				     &compression_level,
				     &compression_flags,
				     &error ) != 1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unsupported compression type defaulting to: none.\n" );

					compression_level = LIBEWF_COMPRESSION_NONE;
					compression_flags = 0;
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
					fprintf(
					 stderr,
					 "Unsupported digest type.\n" );
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
				else if( ewfinput_determine_ewf_format(
				          optarg,
				          &ewf_format,
				          &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unsupported file format type defaulting to: raw.\n" );

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

				if( system_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &export_offset,
				     NULL ) != 1 )
				{
					export_offset = 0;

					fprintf(
					 stderr,
					 "Unsupported export offset defaulting to: %" PRIu64 ".\n",
					 export_offset );
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

					fprintf(
					 stderr,
					 "Unsupported process buffer size defaulting to: chunk size.\n" );
				}
				break;

			case (system_integer_t) 'q':
				print_status_information = 0;

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
				          &error );

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
				 || ( ( ewf_format == LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (int64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT ) )
				 || ( ( ewf_format != LIBEWF_FORMAT_ENCASE6 )
				  && ( segment_file_size >= (int64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT ) ) )
				{
					segment_file_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

					fprintf(
					 stderr,
					 "Unsupported segment file size defaulting to: %" PRIu64 ".\n",
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
		fprintf(
		 stderr,
		 "Missing EWF image file(s).\n" );

		usage_fprint(
		 stderr );

		return( EXIT_FAILURE );
	}
	notify_set_values(
	 stderr,
	 verbose );
	libewf_set_notify_values(
	 stderr,
	 verbose );

	if( ewfsignal_attach(
	     ewfexport_signal_handler ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );
	}
#if !defined( HAVE_GLOB_H )
	if( glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize glob.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( glob_resolve(
	     glob,
	     &argv[ optind ],
	     argc - optind,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to resolve glob.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		glob_free(
		 &glob,
		 NULL );

		return( EXIT_FAILURE );
	}
	argv_filenames      = glob->result;
	amount_of_filenames = glob->amount_of_results;
#else
	argv_filenames      = &argv[ optind ];
	amount_of_filenames = argc - optind;
#endif

	if( export_handle_initialize(
	     &export_handle,
	     calculate_md5,
	     calculate_sha1,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create export handle.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

#if !defined( HAVE_GLOB_H )
		glob_free(
		 &glob,
		 NULL );
#endif

		return( EXIT_FAILURE );
	}
#if defined( HAVE_V2_API )
	if( export_handle_set_header_codepage(
	     export_handle,
	     header_codepage,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set header codepage in export handle.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
		 NULL );

#if !defined( HAVE_GLOB_H )
		glob_free(
		 &glob,
		 NULL );
#endif

		return( EXIT_FAILURE );
	}
#endif
	result = export_handle_open_input(
	          export_handle,
	          argv_filenames,
	          amount_of_filenames,
	          &error );

#if !defined( HAVE_GLOB_H )
	if( glob_free(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
#endif

	if( ( ewfexport_abort == 0 )
	 && ( result != 1 ) )
	{
		ewfoutput_error_fprint(
		 stderr,
		 "Unable to open EWF file(s)" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ( ewfexport_abort == 0 )
	 && ( export_handle_get_input_media_size(
	       export_handle,
	       &media_size,
	       &error ) != 1 ) )
	{
		ewfoutput_error_fprint(
		 stderr,
		 "Unable to determine input media size.\n" );

		notify_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_close(
		 export_handle,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ( export_size == 0 )
	 || ( export_size > ( media_size - export_offset ) ) )
	{
		export_size = media_size - export_offset;
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
				fprintf(
				 stderr,
				 "Unable to create target filename.\n" );

				error_abort = 1;
			}
			else if( system_string_copy(
				  target_filename,
				  option_target_filename,
				  string_length ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set target filename.\n" );

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
			fprintf(
			 stderr,
			 "Unable to create target filename string.\n" );

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
				fprintf(
				 stderr,
				 "Unable to set target filename string.\n" );

				error_abort = 1;
			}
			target_filename[ 7 ] = 0;
		}
	}
	if( error_abort != 0 )
	{
		memory_free(
		 target_filename );

		export_handle_close(
		 export_handle,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Request the necessary case data
	 */
	if( ( ewfexport_abort == 0 )
	 && ( interactive_mode == 1 ) )
	{
		if( ewfsignal_detach() != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to detach signal handler.\n" );
		}
		fprintf(
		 stderr,
		 "Information for export required, please provide the necessary input\n" );

		/* File format
		 */
		if( argument_set_format == 0 )
		{
			if( ewfinput_get_fixed_string_variable(
			     stderr,
			     input_buffer,
			     EWFEXPORT_INPUT_BUFFER_SIZE,
			     _SYSTEM_CHARACTER_T_STRING( "Export to file format" ),
			     ewfexport_format_types,
			     13,
			     0,
			     &fixed_string_variable,
			     &error ) == -1 )
			{
				notify_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stderr,
				 "Unable to determine file format defaulting to: raw.\n" );

				output_raw = 1;
			}
			else if( system_string_compare(
			          fixed_string_variable,
			          _SYSTEM_CHARACTER_T_STRING( "raw" ),
			          3 ) == 0 )
			{
				output_raw = 1;
			}
			else if( ewfinput_determine_ewf_format(
			          fixed_string_variable,
			          &ewf_format,
			          &error ) != 1 )
			{
				notify_error_backtrace(
				 error );
				liberror_error_free(
				 &error );

				fprintf(
				 stderr,
				 "Unsupported file format defaulting to: raw.\n" );

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
				while( ewfinput_get_string_variable(
					stderr,
					_SYSTEM_CHARACTER_T_STRING( "Target path and filename without extension" ),
				        target_filename,
				        1024,
				        &error ) != 1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Filename is required, please try again or terminate using Ctrl^C.\n" );
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
				     _SYSTEM_CHARACTER_T_STRING( "Use compression" ),
				     ewfinput_compression_levels,
				     EWFINPUT_COMPRESSION_LEVELS_AMOUNT,
				     EWFINPUT_COMPRESSION_LEVELS_DEFAULT,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
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
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unsupported compression type defaulting to: none.\n" );

					compression_level = LIBEWF_COMPRESSION_NONE;
					compression_flags = 0;
				}
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
				if( ewfinput_get_byte_size_variable(
				     stderr,
				     input_buffer,
				     EWFEXPORT_INPUT_BUFFER_SIZE,
				     _SYSTEM_CHARACTER_T_STRING( "Evidence segment file size in bytes" ),
				     EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
				     maximum_segment_file_size,
				     EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
				     &segment_file_size,
				     &error ) == -1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					segment_file_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

					fprintf(
					 stderr,
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
			/* Chunk size (sectors per block)
			 */
			if( argument_set_sectors_per_chunk == 0 )
			{
				if( ewfinput_get_fixed_string_variable(
				     stderr,
				     input_buffer,
				     EWFEXPORT_INPUT_BUFFER_SIZE,
				     _SYSTEM_CHARACTER_T_STRING( "The amount of sectors to read at once" ),
				     ewfinput_sector_per_block_sizes,
				     EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT,
				     EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT,
				     &fixed_string_variable,
				     &error ) == -1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unable to determine sectors per chunk on error defaulting to: 64.\n" );

					sectors_per_chunk = 64;
				}
				else if( ewfinput_determine_sectors_per_chunk(
				          fixed_string_variable,
				          &sectors_per_chunk,
				          &error ) != 1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unsupported sectors per chunk on error defaulting to: 64.\n" );

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
				while( ewfinput_get_string_variable(
					stderr,
					_SYSTEM_CHARACTER_T_STRING( "Target path and filename with extension or - for stdout" ),
					target_filename,
					1024,
					&error ) != 1 )
				{
					notify_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Filename is required, please try again or terminate using Ctrl^C.\n" );
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
		       _SYSTEM_CHARACTER_T_STRING( "Start export at offset" ),
		       0,
		       media_size,
		       export_offset,
		       &export_offset,
		       &error ) == -1 ) )
		{
			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			export_offset = 0;

			fprintf(
			 stderr,
			 "Unable to determine export offset defaulting to: %" PRIu64 ".\n",
			 export_offset );
		}
                /* Size of data to export
		 */
		if( ( argument_set_size == 0 )
		 && ( ewfinput_get_size_variable(
		       stderr,
		       input_buffer,
		       EWFEXPORT_INPUT_BUFFER_SIZE,
		       _SYSTEM_CHARACTER_T_STRING( "Amount of bytes to export" ),
		       0,
		       ( media_size - export_offset ),
		       export_size,
		       &export_size,
		       &error ) == -1 ) )
		{
			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			export_size = media_size - export_offset;

			fprintf(
			 stderr,
			 "Unable to determine export size defaulting to: %" PRIu64 ".\n",
			 export_size );
		}
		if( ewfsignal_attach(
		     ewfexport_signal_handler ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to attach signal handler.\n" );
		}
	}
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (system_character_t *) memory_allocate(
		                                                     sizeof( system_character_t )* DIGEST_HASH_STRING_SIZE_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated MD5 hash string.\n" );

			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( calculate_sha1 == 1 )
	{
		calculated_sha1_hash_string = (system_character_t *) memory_allocate(
		                                                      sizeof( system_character_t )* DIGEST_HASH_STRING_SIZE_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated SHA1 hash string.\n" );

			if( calculate_md5 == 1 )
			{
				memory_free(
				 calculated_md5_hash_string );
			}
			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( ewfexport_abort == 0 )
	{
		fprintf(
		 stderr,
		 "\n" );

		if( process_status_initialize(
		     &process_status,
		     _SYSTEM_CHARACTER_T_STRING( "Export" ),
		     _SYSTEM_CHARACTER_T_STRING( "exported" ),
		     _SYSTEM_CHARACTER_T_STRING( "Written" ),
		     stderr,
		     print_status_information,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to initialize process status.\n" );

			notify_error_backtrace(
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
			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
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

			notify_error_backtrace(
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
			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		if( output_raw == 0 )
		{
			export_handle_output_format = EXPORT_HANDLE_OUTPUT_FORMAT_EWF;
		}
		else
		{
			export_handle_output_format = EXPORT_HANDLE_OUTPUT_FORMAT_RAW;
		}
		if( export_handle_open_output(
		     export_handle,
		     export_handle_output_format,
		     target_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to open output.\n" );

			notify_error_backtrace(
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
			memory_free(
			 target_filename );

			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		memory_free(
		 target_filename );

		if( platform_get_operating_system(
		     acquiry_operating_system,
		     32,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to determine operating system.\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			acquiry_operating_system[ 0 ] = 0;
		}
		acquiry_software_version = _SYSTEM_CHARACTER_T_STRING( LIBEWF_VERSION_STRING );

		if( export_handle_set_output_values(
		     export_handle,
		     acquiry_operating_system,
		     program,
		     acquiry_software_version,
		     header_codepage,
		     export_size,
		     compression_level,
		     compression_flags,
		     ewf_format,
		     segment_file_size,
		     sectors_per_chunk,
		     wipe_chunk_on_error,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set output values.\n" );

			notify_error_backtrace(
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
			export_handle_close(
			 export_handle,
			 NULL );
			export_handle_free(
			 &export_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		/* Start exporting data
		 */
		export_count = ewfexport_read_input(
				export_handle,
				media_size,
				export_size,
				export_offset,
				calculated_md5_hash_string,
				DIGEST_HASH_STRING_SIZE_MD5,
				calculated_sha1_hash_string,
				DIGEST_HASH_STRING_SIZE_SHA1,
				swap_byte_pairs,
				(size_t) process_buffer_size,
				process_status,
				&error );

		if( export_count <= -1 )
		{
			notify_error_backtrace(
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
	if( ewfexport_abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     process_status,
	     (size64_t) export_count,
	     status,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to stop process status.\n" );

		notify_error_backtrace(
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
		export_handle_close(
		 export_handle,
		 NULL );
		export_handle_free(
		 &export_handle,
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

		notify_error_backtrace(
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
		export_handle_close(
		 export_handle,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( status == PROCESS_STATUS_COMPLETED )
	{
		if( log_filename != NULL )
		{
			log_file_stream = system_string_fopen(
					   log_filename,
					   _SYSTEM_CHARACTER_T_STRING( "a" ) );

			if( log_file_stream == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to open log file: %s.\n",
				 log_filename );
			}
		}
		if( export_handle_crc_errors_fprint(
		     export_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print export errors.\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
		if( ( log_file_stream != NULL )
		 && ( export_handle_crc_errors_fprint(
		       export_handle,
		       log_file_stream,
		       &error ) != 1 ) )
		{
			fprintf(
			 stderr,
			 "Unable to write export errors in log file.\n" );

			notify_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( export_handle_close(
	     export_handle,
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
		export_handle_free(
		 &export_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( export_handle_free(
	     &export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

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
	if( ewfsignal_detach() != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );
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
		fprintf(
		 stderr,
		 "MD5 hash calculated over data:\t%" PRIs_SYSTEM "\n",
		 calculated_md5_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf(
			 log_file_stream,
			 "MD5 hash calculated over data:\t%" PRIs_SYSTEM "\n",
			 calculated_md5_hash_string );
		}
		memory_free(
		 calculated_md5_hash_string );
	}
	if( calculate_sha1 == 1 )
	{
		fprintf(
		 stderr,
		 "SHA1 hash calculated over data:\t%" PRIs_SYSTEM "\n",
		 calculated_sha1_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf(
			 log_file_stream,
			 "SHA1 hash calculated over data:\t%" PRIs_SYSTEM "\n",
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
			fprintf(
			 stderr,
			 "Unable to close log file: %s.\n",
			 log_filename );
		}
	}
	return( EXIT_SUCCESS );
}

