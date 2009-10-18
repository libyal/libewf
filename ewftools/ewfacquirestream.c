/*
 * ewfacquirestream
 * Reads data from a stdin and writes it in EWF format
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <liberror.h>

#include <errno.h>

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
#include "ewfcommon.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "imaging_handle.h"
#include "platform.h"
#include "process_status.h"
#include "storage_media_buffer.h"

imaging_handle_t *ewfacquirestream_imaging_handle = NULL;
int ewfacquirestream_abort                        = 0;

/* Prints the executable usage information to the stream
 */
void usage_fprint(
      FILE *stream )
{
	libsystem_character_t default_segment_file_size_string[ 16 ];
	libsystem_character_t minimum_segment_file_size_string[ 16 ];
	libsystem_character_t maximum_32bit_segment_file_size_string[ 16 ];
	libsystem_character_t maximum_64bit_segment_file_size_string[ 16 ];

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
	fprintf( stream, "Use ewfacquirestream to acquire data from a pipe and store it in the EWF format\n"
	                 "(Expert Witness Compression Format).\n\n" );

	fprintf( stream, "Usage: ewfacquirestream [ -A codepage ] [ -b amount_of_sectors ]\n"
	                 "                        [ -B amount_of_bytes ] [ -c compression_type ]\n"
	                 "                        [ -C case_number ] [ -d digest_type ]\n"
	                 "                        [ -D description ] [ -e examiner_name ]\n"
	                 "                        [ -E evidence_number ] [ -f format ] [ -l log_filename ]\n"
	                 "                        [ -m media_type ] [ -M media_flags ] [ -N notes ]\n"
	                 "                        [ -o offset ] [ -p process_buffer_size ]\n"
	                 "                        [ -S segment_file_size ] [ -t target ]\n"
	                 "                        [ -2 secondary_target ] [ -hqsvVw ]\n\n" );

	fprintf( stream, "\tReads data from stdin\n\n" );

	fprintf( stream, "\t-A: codepage of header section, options: ascii (default), windows-874,\n"
	                 "\t    windows-1250, windows-1251, windows-1252, windows-1253, windows-1254,\n"
	                 "\t    windows-1255, windows-1256, windows-1257, windows-1258\n" );
	fprintf( stream, "\t-b: specify the amount of sectors to read at once (per chunk), options:\n"
	                 "\t    64 (default), 128, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768\n" );
	fprintf( stream, "\t-B: specify the amount of bytes to acquire (default is all bytes)\n" );
	fprintf( stream, "\t-c: specify the compression type, options: none (default), empty-block, fast\n"
	                 "\t    or best\n" );
	fprintf( stream, "\t-C: specify the case number (default is case_number).\n" );
	fprintf( stream, "\t-d: calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stream, "\t-D: specify the description (default is description).\n" );
	fprintf( stream, "\t-e: specify the examiner name (default is examiner_name).\n" );
	fprintf( stream, "\t-E: specify the evidence number (default is evidence_number).\n" );
	fprintf( stream, "\t-f: specify the EWF file format to write to, options: ftk, encase2, encase3,\n"
	                 "\t    encase4, encase5, encase6 (default), linen5, linen6, ewfx\n" );
	fprintf( stream, "\t-h: shows this help\n" );
	fprintf( stream, "\t-l: logs acquiry errors and the digest (hash) to the log_filename\n" );
	fprintf( stream, "\t-m: specify the media type, options: fixed (default), removable, optical, memory\n" );
	fprintf( stream, "\t-M: specify the media flags, options: logical, physical (default)\n" );
	fprintf( stream, "\t-N: specify the notes (default is notes).\n" );
	fprintf( stream, "\t-o: specify the offset to start to acquire (default is 0)\n" );
	fprintf( stream, "\t-p: specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-q: quiet shows no status information\n" );
	fprintf( stream, "\t-s: swap byte pairs of the media data (from AB to BA)\n"
	                 "\t    (use this for big to little endian conversion and vice versa)\n" );

	if( result == 1 )
	{
		fprintf( stream, "\t-S: specify the segment file size in bytes (default is %" PRIs_LIBSYSTEM ")\n"
		                 "\t    (minimum is %" PRIs_LIBSYSTEM ", maximum is %" PRIs_LIBSYSTEM " for encase6 format\n"
		                 "\t    and %" PRIs_LIBSYSTEM " for other formats)\n",
		 default_segment_file_size_string,
		 minimum_segment_file_size_string,
		 maximum_64bit_segment_file_size_string,
		 maximum_32bit_segment_file_size_string );
	}
	else
	{
		fprintf( stream, "\t-S: specify the segment file size in bytes (default is %" PRIu32 ")\n"
		                 "\t    (minimum is %" PRIu32 ", maximum is %" PRIu64 " for encase6 format\n"
		                 "\t    and %" PRIu32 " for other formats)\n",
		 (uint32_t) EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
		 (uint32_t) EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
		 (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
		 (uint32_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT );
	}
	fprintf( stream, "\t-t: specify the target file (without extension) to write to (default is image)\n" );
	fprintf( stream, "\t-v: verbose output to stderr\n" );
	fprintf( stream, "\t-V: print version\n" );
	fprintf( stream, "\t-w: wipe sectors on read error (mimic EnCase like behavior)\n" );
	fprintf( stream, "\t-2: specify the secondary target file (without extension) to write to\n" );
}

/* Prints an overview of the aquiry parameters
 * Returns 1 if successful or -1 on error
 */
int ewfacquirestream_acquiry_parameters_fprint(
     FILE *stream,
     libsystem_character_t *filename,
     libsystem_character_t *filename_secondary_copy,
     libsystem_character_t *case_number,
     libsystem_character_t *description,
     libsystem_character_t *evidence_number,
     libsystem_character_t *examiner_name,
     libsystem_character_t *notes,
     uint8_t media_type,
     uint8_t media_flags,
     int8_t compression_level,
     uint8_t compression_flags,
     uint8_t ewf_format,
     off64_t acquiry_offset,
     size64_t acquiry_size,
     size64_t segment_file_size,
     uint32_t bytes_per_sector,
     uint32_t sectors_per_chunk,
     uint32_t sector_error_granularity,
     uint8_t read_error_retry,
     uint8_t wipe_block_on_read_error,
     liberror_error_t **error )
{
	libsystem_character_t acquiry_size_string[ 16 ];
	libsystem_character_t segment_file_size_string[ 16 ];

	static char *function = "ewfacquirestream_acquiry_parameters_fprint";
	int result            = 0;

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
	 "Using the following acquiry parameters:\n" );

	fprintf(
	 stream,
	 "Image path and filename:\t%" PRIs_LIBSYSTEM ".",
	 filename );

	if( ewf_format == LIBEWF_FORMAT_SMART )
	{
		fprintf(
		 stream,
		 "s01\n" );
	}
	else if( ( ewf_format == LIBEWF_FORMAT_EWF )
	      || ( ewf_format == LIBEWF_FORMAT_EWFX ) )
	{
		fprintf(
		 stream,
		 "e01\n" );
	}
	else
	{
		fprintf(
		 stream,
		 "E01\n" );
	}
	if( filename_secondary_copy != NULL )
	{
		fprintf(
		 stream,
		 "Secondary copy:\t\t\t%" PRIs_LIBSYSTEM "",
		 filename_secondary_copy );

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
		 "%" PRIs_LIBSYSTEM "",
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
		 "%" PRIs_LIBSYSTEM "",
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
		 "%" PRIs_LIBSYSTEM "",
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
		 "%" PRIs_LIBSYSTEM "",
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
		 "%" PRIs_LIBSYSTEM "",
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
		 "%" PRIs_LIBSYSTEM " (%" PRIu64 " bytes)",
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
		 stream, "%" PRIs_LIBSYSTEM " (%" PRIu64 " bytes)",
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
	 read_error_retry );

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

	return( 1 );
}

/* Reads a chunk of data from the file descriptor into the buffer
 * Returns the amount of bytes read, 0 if at end of input or -1 on error
 */
ssize_t ewfacquirestream_read_chunk(
         libewf_handle_t *handle,
         int input_file_descriptor,
         uint8_t *buffer,
         size_t buffer_size,
         size32_t chunk_size,
         ssize64_t total_read_count,
         uint8_t read_error_retry,
         liberror_error_t **error )
{
	libsystem_character_t error_string[ 128 ];

	static char *function               = "ewfacquirestream_read_chunk";
	ssize_t read_count                  = 0;
	ssize_t buffer_offset               = 0;
	size_t read_size                    = 0;
	size_t bytes_to_read                = 0;
	int32_t read_amount_of_errors       = 0;
	uint32_t read_error_offset          = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( input_file_descriptor == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input file descriptor.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid chunk size value zero or less.",
		 function );

		return( -1 );
	}
	if( total_read_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid total read count value less than zero.",
		 function );

		return( -1 );
	}
	while( buffer_size > 0 )
	{
		/* Determine the amount of bytes to read from the input
		 * Read as much as possible in chunk sizes
		 */
		if( buffer_size < (size_t) chunk_size )
		{
			read_size = buffer_size;
		}
		else
		{
			read_size = chunk_size;
		}
		bytes_to_read = read_size;

		while( read_amount_of_errors <= read_error_retry )
		{
			read_count = libsystem_file_io_read(
			              input_file_descriptor,
			              &( buffer[ buffer_offset + read_error_offset ] ),
			              bytes_to_read );

#if defined( HAVE_VERBOSE_OUTPUT )
			libsystem_notify_verbose_printf(
			 "%s: read buffer at: %" PRIu64 " of size: %" PRIzd ".\n",
			 function,
			 total_read_count,
			 read_count );
#endif

			if( read_count <= -1 )
			{
				if( ( errno == ESPIPE )
				 || ( errno == EPERM )
				 || ( errno == ENXIO )
				 || ( errno == ENODEV ) )
				{
					if( libsystem_error_copy_to_string(
					     errno,
					     error_string,
					     128,
					     error ) == 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: error reading data: " PRIs_LIBSYSTEM ".",
						 function,
						 error_string );
					}
					else if( errno == ESPIPE )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: error reading data: invalid seek.",
						 function );
					}
					else if( errno == EPERM )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: error reading data: operation not permitted.",
						 function );
					}
					else if( errno == ENXIO )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: error reading data: no such device or address.",
						 function );
					}
					else if( errno == ENODEV )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: error reading data: no such device.",
						 function );
					}
					return( -1 );
				}
			}
			else
			{
				/* The last read is OK, correct read_count
				 */
				if( read_count == (ssize_t) bytes_to_read )
				{
					read_count = read_error_offset + bytes_to_read;
				}
				/* The entire read is OK
				 */
				if( read_count == (ssize_t) read_size )
				{
					break;
				}
				/* If no end of input can be determined
				 */
				/* If some bytes were read it is possible that the end of the input reached
				 */
				if( read_count > 0 )
				{
					return( (ssize32_t) ( buffer_offset + read_count ) );
				}
				/* No bytes were read
				 */
				if( read_count == 0 )
				{
					return( 0 );
				}
#if defined( HAVE_VERBOSE_OUTPUT )
				libsystem_notify_verbose_printf(
				 "%s: read error at offset %" PRIjd " after reading %" PRIzd " bytes.\n",
				 function,
				 total_read_count,
				 read_count );
#endif

				/* There was a read error at a certain offset
				 */
				read_error_offset += read_count;
				bytes_to_read     -= read_count;
			}
			read_amount_of_errors++;

			if( read_amount_of_errors > read_error_retry )
			{
				return( 0 );
			}
		}
		buffer_size   -= read_count;
		buffer_offset += read_count;

		/* At the end of the input
		 */
		if( ewfacquirestream_abort != 0 )
		{
			break;
		}
	}
	return( buffer_offset );
}

/* Reads data from a file descriptor and writes it in EWF format
 * Returns the amount of bytes written or -1 on error
 */
ssize64_t ewfacquirestream_read_input(
           imaging_handle_t *imaging_handle,
           int input_file_descriptor,
           size64_t acquiry_size,
           off64_t acquiry_offset,
           uint32_t bytes_per_sector,
           uint8_t swap_byte_pairs,
           uint8_t read_error_retry,
           uint8_t wipe_block_on_read_error,
           size_t process_buffer_size,
           libsystem_character_t *calculated_md5_hash_string,
           size_t calculated_md5_hash_string_size,
           libsystem_character_t *calculated_sha1_hash_string,
           size_t calculated_sha1_hash_string_size,
           process_status_t *process_status,
           liberror_error_t **error )
{
	storage_media_buffer_t *storage_media_buffer = NULL;
	static char *function                        = "ewfacquirestream_read_input";
	ssize64_t acquiry_count                      = 0;
	size32_t chunk_size                          = 0;
	size_t read_size                             = 0;
	ssize_t read_count                           = 0;
	ssize_t process_count                        = 0;
	ssize_t write_count                          = 0;

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
	if( input_file_descriptor == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file descriptor.",
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

	while( ( acquiry_size == 0 )
	    || ( acquiry_count < (ssize64_t) acquiry_size ) )
	{
		read_size = process_buffer_size;

		/* Align with acquiry offset if necessary
		 */
		if( ( acquiry_offset != 0 )
		 && ( acquiry_offset < (off64_t) read_size ) )
		{
			read_size = (size_t) acquiry_offset;
		}
		else if( ( acquiry_size != 0 )
		      && ( ( (ssize64_t) acquiry_size - acquiry_count ) < (ssize64_t) read_size ) )
		{
			read_size = (size_t) ( (ssize64_t) acquiry_size - acquiry_count );
		}

		/* Read a chunk from the file descriptor
		 */
		read_count = ewfacquirestream_read_chunk(
		              imaging_handle->output_handle,
		              input_file_descriptor,
		              storage_media_buffer->raw_buffer,
		              storage_media_buffer->raw_buffer_size,
		              read_size,
		              acquiry_count,
		              read_error_retry,
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
			break;
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		storage_media_buffer->data_in_compression_buffer = 0;
#endif
		storage_media_buffer->raw_buffer_amount = read_count;

		/* Skip a certain amount of bytes if necessary
		 */
		if( acquiry_offset > (off64_t) acquiry_count )
		{
			acquiry_offset -= read_count;

			continue;
		}
		/* Swap byte pairs
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
		/* Digest hashes are calcultated after swap
		 */
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
		acquiry_count += read_count;

		 if( process_status_update_unknown_total(
		      process_status,
		      (size64_t) acquiry_count,
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
		if( ewfacquirestream_abort != 0 )
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
		 "%s: unable to finalize write.",
		 function );

		return( -1 );
	}
	acquiry_count += write_count;

	return( acquiry_count );
}

/* Signal handler for ewfacquire
 */
void ewfacquirestream_signal_handler(
      libsystem_signal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "ewfacquirestream_signal_handler";

	ewfacquirestream_abort = 1;

	if( ( ewfacquirestream_imaging_handle != NULL )
	 && ( imaging_handle_signal_abort(
	       ewfacquirestream_imaging_handle,
	       &error ) != 1 ) )
	{
		libsystem_notify_printf(
		 "%s: unable to signal imaging handle to abort.\n",
		 function );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return;
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
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libsystem_character_t acquiry_operating_system[ 32 ];

	liberror_error_t *error                            = NULL;

	process_status_t *process_status                   = NULL;

	libsystem_character_t *acquiry_software_version    = NULL;
	libsystem_character_t *calculated_md5_hash_string  = NULL;
	libsystem_character_t *calculated_sha1_hash_string = NULL;
	libsystem_character_t *case_number                 = NULL;
	libsystem_character_t *description                 = NULL;
	libsystem_character_t *evidence_number             = NULL;
	libsystem_character_t *examiner_name               = NULL;
	libsystem_character_t *log_filename                = NULL;
	libsystem_character_t *notes                       = NULL;
	libsystem_character_t *option_case_number          = NULL;
	libsystem_character_t *option_description          = NULL;
	libsystem_character_t *option_examiner_name        = NULL;
	libsystem_character_t *option_evidence_number      = NULL;
	libsystem_character_t *option_notes                = NULL;
	libsystem_character_t *program                     = _LIBSYSTEM_CHARACTER_T_STRING( "ewfacquirestream" );
	libsystem_character_t *secondary_target_filename   = NULL;
	libsystem_character_t *target_filename             = _LIBSYSTEM_CHARACTER_T_STRING( "image" );

	FILE *log_file_stream                              = NULL;

	libsystem_integer_t option                         = 0;
	size_t case_number_length                          = 0;
	size_t description_length                          = 0;
	size_t evidence_number_length                      = 0;
	size_t examiner_name_length                        = 0;
	size_t notes_length                                = 0;
	size_t string_length                               = 0;
	int64_t write_count                                = 0;
	uint64_t acquiry_offset                            = 0;
	uint64_t acquiry_size                              = 0;
	uint64_t process_buffer_size                       = EWFCOMMON_PROCESS_BUFFER_SIZE;
	uint64_t segment_file_size                         = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;
	uint32_t bytes_per_sector                          = 512;
	uint32_t sectors_per_chunk                         = 64;
	uint32_t sector_error_granularity                  = 64;
	int8_t compression_level                           = LIBEWF_COMPRESSION_NONE;
	uint8_t calculate_md5                              = 1;
	uint8_t calculate_sha1                             = 0;
	uint8_t compression_flags                          = 0;
	uint8_t ewf_format                                 = LIBEWF_FORMAT_ENCASE6;
	uint8_t media_flags                                = LIBEWF_MEDIA_FLAG_PHYSICAL;
	uint8_t media_type                                 = LIBEWF_MEDIA_TYPE_FIXED;
	uint8_t print_status_information                   = 1;
	uint8_t read_error_retry                           = 2;
	uint8_t resume_acquiry                             = 0;
	uint8_t swap_byte_pairs                            = 0;
	uint8_t verbose                                    = 0;
	uint8_t wipe_block_on_read_error                   = 0;
	int error_abort                                    = 0;
	int header_codepage                                = LIBEWF_CODEPAGE_ASCII;
	int result                                         = 0;
	int status                                         = 0;

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

#if defined( WINAPI )
#if defined( _MSC_VER )
	if( _setmode(
	     _fileno(
	      stdin ),
	     _O_BINARY ) == -1 )
#else
	if( setmode(
	     _fileno(
	      stdin ),
	     _O_BINARY ) == -1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to set stdin to binary mode.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
#endif

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBSYSTEM_CHARACTER_T_STRING( "A:b:B:c:C:d:D:e:E:f:hl:m:M:N:o:p:qsS:t:vVw2:" ) ) ) != (libsystem_integer_t) -1 )
	{
		switch( option )
		{
			case (libsystem_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBSYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libsystem_integer_t) 'A':
				if( ewfinput_determine_header_codepage(
				     optarg,
				     &header_codepage,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					fprintf(
					 stderr,
					 "Unsuported header codepage defaulting to: ascii.\n" );

					header_codepage = LIBEWF_CODEPAGE_ASCII;
				}
				break;

			case (libsystem_integer_t) 'b':
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
				break;

			case (libsystem_integer_t) 'B':
				string_length = libsystem_string_length(
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
				break;

			case (libsystem_integer_t) 'c':
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
				break;

			case (libsystem_integer_t) 'C':
				option_case_number = optarg;

				break;

			case (libsystem_integer_t) 'd':
				if( libsystem_string_compare(
				     optarg,
				     _LIBSYSTEM_CHARACTER_T_STRING( "sha1" ),
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

			case (libsystem_integer_t) 'D':
				option_description = optarg;

				break;

			case (libsystem_integer_t) 'e':
				option_examiner_name = optarg;

				break;

			case (libsystem_integer_t) 'E':
				option_evidence_number = optarg;

				break;

			case (libsystem_integer_t) 'f':
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
				else if( ( ewf_format == LIBEWF_FORMAT_EWF )
				      || ( ewf_format == LIBEWF_FORMAT_SMART ) )
				{
					ewf_format = LIBEWF_FORMAT_ENCASE6;

					fprintf(
					 stderr,
					 "Unsupported EWF file format type defaulting to: encase6.\n" );
				}
				break;

			case (libsystem_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libsystem_integer_t) 'm':
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
				break;

			case (libsystem_integer_t) 'M':
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
				break;

			case (libsystem_integer_t) 'N':
				option_notes = optarg;

				break;

			case (libsystem_integer_t) 'o':
				string_length = libsystem_string_length(
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
				break;

			case (libsystem_integer_t) 'p':
				string_length = libsystem_string_length(
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

			case (libsystem_integer_t) 'q':
				print_status_information = 0;
				break;

			case (libsystem_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (libsystem_integer_t) 'S':
				string_length = libsystem_string_length(
				                 optarg );

				result = byte_size_string_convert(
				          optarg,
				          string_length,
				          &segment_file_size,
				          &error );

				if( result != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );
				}
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

			case (libsystem_integer_t) 't':
				target_filename = optarg;

				break;

			case (libsystem_integer_t) 'v':
				verbose = 1;

				break;

			case (libsystem_integer_t) 'V':
				ewfoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'w':
				wipe_block_on_read_error = 1;

				break;

			case (libsystem_integer_t) '2':
				secondary_target_filename = optarg;

				break;
		}
	}
	libsystem_notify_set_verbose(
	 verbose );
#if defined( HAVE_V2_API )
	libewf_notify_set_verbose(
	 verbose );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#else
	libewf_set_notify_values(
	 stderr,
	 verbose );
#endif

	if( option_case_number != NULL )
	{
		case_number_length = libsystem_string_length(
		                      option_case_number );

		if( case_number_length > 0 )
		{
			case_number = (libsystem_character_t *) memory_allocate(
			                                         sizeof( libsystem_character_t ) * ( case_number_length + 1 ) );

			if( case_number == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create case number string.\n" );

				error_abort = 1;
			}
			else if( libsystem_string_copy(
			          case_number,
			          option_case_number,
			          case_number_length + 1 ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set case number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( ( error_abort == 0 )
	 && ( option_description != NULL ) )
	{
		description_length = libsystem_string_length(
		                      option_description );

		if( description_length > 0 )
		{
			description = (libsystem_character_t *) memory_allocate(
			                                         sizeof( libsystem_character_t ) * ( description_length + 1 ) );

			if( description == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create description string.\n" );

				error_abort = 1;
			}
			else if( libsystem_string_copy(
			          description,
			          option_description,
			          description_length + 1 ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set description string.\n" );

				error_abort = 1;
			}
		}
	}
	if( ( error_abort == 0 )
	 && ( option_examiner_name != NULL ) )
	{
		examiner_name_length = libsystem_string_length(
		                        option_examiner_name );

		if( examiner_name_length > 0 )
		{
			examiner_name = (libsystem_character_t *) memory_allocate(
			                                           sizeof( libsystem_character_t ) * ( examiner_name_length + 1 ) );

			if( examiner_name == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create examiner name string.\n" );

				error_abort = 1;
			}
			else if( libsystem_string_copy(
			          examiner_name,
			          option_examiner_name,
			          examiner_name_length + 1 ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set examiner name string.\n" );

				error_abort = 1;
			}
		}
	}
	if( ( error_abort == 0 )
	 && ( option_evidence_number != NULL ) )
	{
		evidence_number_length = libsystem_string_length(
		                          option_evidence_number );

		if( evidence_number_length > 0 )
		{
			evidence_number = (libsystem_character_t *) memory_allocate(
			                                             sizeof( libsystem_character_t ) * ( evidence_number_length + 1 ) );

			if( evidence_number == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create evidence number string.\n" );

				error_abort = 1;
			}
			else if( libsystem_string_copy(
			          evidence_number,
			          option_evidence_number,
			          evidence_number_length + 1 ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set evidence number string.\n" );

				error_abort = 1;
			}
		}
	}
	if( ( error_abort == 0 )
	 && ( option_notes != NULL ) )
	{
		notes_length = libsystem_string_length(
		                option_notes );

		if( notes_length > 0 )
		{
			notes = (libsystem_character_t *) memory_allocate(
			                                   sizeof( libsystem_character_t ) * ( notes_length + 1 ) );

			if( notes == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create notes string.\n" );

				error_abort = 1;
			}
			else if( libsystem_string_copy(
			          notes,
			          option_notes,
			          notes_length + 1 ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set notes string.\n" );

				error_abort = 1;
			}
		}
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
		return( EXIT_FAILURE );
	}
	if( libsystem_signal_attach(
	     ewfacquirestream_signal_handler,
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
	if( ewfacquirestream_abort == 0 )
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
		acquiry_software_version = _LIBSYSTEM_CHARACTER_T_STRING( LIBEWF_VERSION_STRING );

		if( ewfacquirestream_acquiry_parameters_fprint(
		     stdout,
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
		     (size64_t) acquiry_size,
		     (size64_t) segment_file_size,
		     bytes_per_sector,
		     sectors_per_chunk,
		     sector_error_granularity,
		     read_error_retry,
		     wipe_block_on_read_error,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print acquiry parameters.\n" );

			error_abort = 1;
		}
	}
	if( ( ewfacquirestream_abort == 0 )
	 && ( error_abort == 0 ) )
	{
		if( imaging_handle_initialize(
		     &ewfacquirestream_imaging_handle,
		     calculate_md5,
		     calculate_sha1,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to create imaging handle.\n" );

			error_abort = 1;
		}
	}
	if( ( ewfacquirestream_abort == 0 )
	 && ( error_abort == 0 ) )
	{
		if( imaging_handle_open_output(
		     ewfacquirestream_imaging_handle,
		     target_filename,
		     resume_acquiry,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to open output file(s).\n" );

			imaging_handle_free(
			 &ewfacquirestream_imaging_handle,
			 NULL );

			error_abort = 1;
		}
	}
	if( ( ewfacquirestream_abort == 0 )
	 && ( error_abort == 0 ) )
	{
		if( secondary_target_filename != NULL )
		{
			if( imaging_handle_open_secondary_output(
			     ewfacquirestream_imaging_handle,
			     secondary_target_filename,
			     resume_acquiry,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to open secondary output file(s).\n" );

				imaging_handle_free(
				 &ewfacquirestream_imaging_handle,
				 NULL );

				error_abort = 1;
			}
		}
	}
	if( ( ewfacquirestream_abort == 0 )
	 && ( error_abort == 0 ) )
	{
		if( imaging_handle_set_output_values(
		     ewfacquirestream_imaging_handle,
		     case_number,
		     case_number_length,
		     description,
		     description_length,
		     evidence_number,
		     evidence_number_length,
		     examiner_name,
		     examiner_name_length,
		     notes,
		     notes_length,
		     acquiry_operating_system,
		     libsystem_string_length(
		      acquiry_operating_system ),
		     program,
		     libsystem_string_length(
		      program ),
		     acquiry_software_version,
		     libsystem_string_length(
		      acquiry_software_version ),
		     NULL,
		     0,
		     NULL,
		     0,
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
			 ewfacquirestream_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &ewfacquirestream_imaging_handle,
			 NULL );

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
	if( error_abort != 0 )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( calculate_md5 == 1 )
	{
		calculated_md5_hash_string = (libsystem_character_t *) memory_allocate(
		                                                        sizeof( libsystem_character_t ) * DIGEST_HASH_STRING_SIZE_MD5 );

		if( calculated_md5_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated MD5 hash string.\n" );

			imaging_handle_close(
			 ewfacquirestream_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &ewfacquirestream_imaging_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( calculate_sha1 == 1 )
	{
		calculated_sha1_hash_string = (libsystem_character_t *) memory_allocate(
		                                                         sizeof( libsystem_character_t ) * DIGEST_HASH_STRING_SIZE_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create calculated SHA1 hash string.\n" );

			memory_free(
			 calculated_md5_hash_string );

			imaging_handle_close(
			 ewfacquirestream_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &ewfacquirestream_imaging_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( ewfacquirestream_abort == 0 )
	{
		if( process_status_initialize(
		     &process_status,
		     _LIBSYSTEM_CHARACTER_T_STRING( "Acquiry" ),
		     _LIBSYSTEM_CHARACTER_T_STRING( "acquired" ),
		     _LIBSYSTEM_CHARACTER_T_STRING( "Written" ),
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
			 ewfacquirestream_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &ewfacquirestream_imaging_handle,
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
			 ewfacquirestream_imaging_handle,
			 NULL );
			imaging_handle_free(
			 &ewfacquirestream_imaging_handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		/* Start acquiring data
		 */
		write_count = ewfacquirestream_read_input(
		               ewfacquirestream_imaging_handle,
		               0,
		               acquiry_size,
		               acquiry_offset,
		               bytes_per_sector,
		               swap_byte_pairs,
		               read_error_retry,
		               wipe_block_on_read_error,
		               (size_t) process_buffer_size,
		               calculated_md5_hash_string,
		               DIGEST_HASH_STRING_SIZE_MD5,
		               calculated_sha1_hash_string,
		               DIGEST_HASH_STRING_SIZE_SHA1,
		               process_status,
		               &error );

		if( write_count <= -1 )
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
	if( ewfacquirestream_abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     process_status,
	     (size64_t) write_count,
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
		 ewfacquirestream_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &ewfacquirestream_imaging_handle,
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
		 ewfacquirestream_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &ewfacquirestream_imaging_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( status == PROCESS_STATUS_COMPLETED )
	{
		if( log_filename != NULL )
		{
			log_file_stream = libsystem_file_stream_open(
					   log_filename,
					   _LIBSYSTEM_CHARACTER_T_STRING( "a" ) );

			if( log_file_stream == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to open log file: %s.\n",
				 log_filename );
			}
		}
		if( imaging_handle_acquiry_errors_fprint(
		     ewfacquirestream_imaging_handle,
		     stdout,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to print acquiry errors.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
		if( ( log_file_stream != NULL )
		 && ( imaging_handle_acquiry_errors_fprint(
		       ewfacquirestream_imaging_handle,
		       log_file_stream,
		       &error ) != 1 ) )
		{
			fprintf(
			 stderr,
			 "Unable to write acquiry errors in log file.\n" );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	if( imaging_handle_close(
	     ewfacquirestream_imaging_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close output file(s).\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		if( log_file_stream != NULL )
		{
			libsystem_file_stream_close(
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
		imaging_handle_free(
		 &ewfacquirestream_imaging_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( imaging_handle_free(
	     &ewfacquirestream_imaging_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free imaging handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		if( log_file_stream != NULL )
		{
			libsystem_file_stream_close(
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
		if( log_file_stream != NULL )
		{
			libsystem_file_stream_close(
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
		 stdout,
		 "MD5 hash calculated over data:\t%" PRIs_LIBSYSTEM "\n",
		 calculated_md5_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf(
			 log_file_stream,
			 "MD5 hash calculated over data:\t%" PRIs_LIBSYSTEM "\n",
			 calculated_md5_hash_string );
		}
		memory_free(
		 calculated_md5_hash_string );
	}
	if( calculate_sha1 == 1 )
	{
		fprintf(
		 stdout,
		 "SHA1 hash calculated over data:\t%" PRIs_LIBSYSTEM "\n",
		 calculated_sha1_hash_string );

		if( log_file_stream != NULL )
		{
			fprintf(
			 log_file_stream,
			 "SHA1 hash calculated over data:\t%" PRIs_LIBSYSTEM "\n",
			 calculated_sha1_hash_string );
		}
		memory_free(
		 calculated_sha1_hash_string );
	}
	if( log_file_stream != NULL )
	{
		if( libsystem_file_stream_close(
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

