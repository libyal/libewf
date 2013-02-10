/*
 * Reads data from a stdin and writes it in EWF format
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "byte_size_string.h"
#include "ewfcommon.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcstring.h"
#include "ewftools_libcsystem.h"
#include "ewftools_libewf.h"
#include "imaging_handle.h"
#include "log_handle.h"
#include "process_status.h"
#include "storage_media_buffer.h"

imaging_handle_t *ewfacquirestream_imaging_handle = NULL;
int ewfacquirestream_abort                        = 0;

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
	fprintf( stream, "Use ewfacquirestream to acquire data from a pipe and store it in the EWF format\n"
	                 "(Expert Witness Compression Format).\n\n" );

	fprintf( stream, "Usage: ewfacquirestream [ -A codepage ] [ -b number_of_sectors ]\n"
	                 "                        [ -B number_of_bytes ] [ -c compression_values ]\n"
	                 "                        [ -C case_number ] [ -d digest_type ]\n"
	                 "                        [ -D description ] [ -e examiner_name ]\n"
	                 "                        [ -E evidence_number ] [ -f format ]\n"
	                 "                        [ -l log_filename ] [ -m media_type ]\n"
	                 "                        [ -M media_flags ] [ -N notes ]\n"
	                 "                        [ -o offset ] [ -p process_buffer_size ]\n"
	                 "                        [ -P bytes_per_sector ] [ -S segment_file_size ]\n"
	                 "                        [ -t target ] [ -2 secondary_target ]\n"
	                 "                        [ -hqsvV ]\n\n" );

	fprintf( stream, "\tReads data from stdin\n\n" );

	fprintf( stream, "\t-A: codepage of header section, options: ascii (default),\n"
	                 "\t    windows-874, windows-932, windows-936, windows-949,\n"
	                 "\t    windows-950, windows-1250, windows-1251, windows-1252,\n"
	                 "\t    windows-1253, windows-1254, windows-1255, windows-1256,\n"
	                 "\t    windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-b: specify the number of sectors to read at once (per chunk), options:\n"
	                 "\t    16, 32, 64 (default), 128, 256, 512, 1024, 2048, 4096, 8192, 16384\n"
	                 "\t    or 32768\n" );
	fprintf( stream, "\t-B: specify the number of bytes to acquire (default is all bytes)\n" );
	fprintf( stream, "\t-c: specify the compression values as: level or method:level\n"
	                 "\t    compression method options: deflate (default), bzip2\n"
	                 "\t    (bzip2 is only supported by EWF2 formats)\n"
	                 "\t    compression level options: none (default), empty-block,\n"
	                 "\t    fast or best\n" );
	fprintf( stream, "\t-C: specify the case number (default is case_number).\n" );
	fprintf( stream, "\t-d: calculate additional digest (hash) types besides md5, options:\n"
	                 "\t    sha1, sha256\n" );
	fprintf( stream, "\t-D: specify the description (default is description).\n" );
	fprintf( stream, "\t-e: specify the examiner name (default is examiner_name).\n" );
	fprintf( stream, "\t-E: specify the evidence number (default is evidence_number).\n" );
	fprintf( stream, "\t-f: specify the EWF file format to write to, options: ftk, encase2,\n"
	                 "\t    encase3, encase4, encase5, encase6 (default), encase7, linen5,\n"
	                 "\t    linen6, linen7, ewfx\n" );
	fprintf( stream, "\t-h: shows this help\n" );
	fprintf( stream, "\t-l: logs acquiry errors and the digest (hash) to the log_filename\n" );
	fprintf( stream, "\t-m: specify the media type, options: fixed (default), removable,\n"
	                 "\t    optical, memory\n" );
	fprintf( stream, "\t-M: specify the media flags, options: logical, physical (default)\n" );
	fprintf( stream, "\t-N: specify the notes (default is notes).\n" );
	fprintf( stream, "\t-o: specify the offset to start to acquire (default is 0)\n" );
	fprintf( stream, "\t-p: specify the process buffer size (default is the chunk size)\n" );
	fprintf( stream, "\t-P: specify the number of bytes per sector (default is 512)\n" );
	fprintf( stream, "\t-q: quiet shows minimal status information\n" );
	fprintf( stream, "\t-s: swap byte pairs of the media data (from AB to BA)\n"
	                 "\t    (use this for big to little endian conversion and vice versa)\n" );

	if( result == 1 )
	{
		fprintf( stream, "\t-S: specify the segment file size in bytes (default is %" PRIs_LIBCSTRING_SYSTEM ")\n"
		                 "\t    (minimum is %" PRIs_LIBCSTRING_SYSTEM ", maximum is %" PRIs_LIBCSTRING_SYSTEM " for encase6 and\n"
		                 "\t    encase7 format and %" PRIs_LIBCSTRING_SYSTEM " for other formats)\n",
		 default_segment_file_size_string,
		 minimum_segment_file_size_string,
		 maximum_64bit_segment_file_size_string,
		 maximum_32bit_segment_file_size_string );
	}
	else
	{
		fprintf( stream, "\t-S: specify the segment file size in bytes (default is %" PRIu32 ")\n"
		                 "\t    (minimum is %" PRIu32 ", maximum is %" PRIu64 " for encase6 and\n"
		                 "\t    encase7 format and %" PRIu32 " for other formats)\n",
		 (uint32_t) EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
		 (uint32_t) EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
		 (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
		 (uint32_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT );
	}
	fprintf( stream, "\t-t: specify the target file (without extension) to write to (default\n"
	                 "\t    is image)\n" );
	fprintf( stream, "\t-v: verbose output to stderr\n" );
	fprintf( stream, "\t-V: print version\n" );
	fprintf( stream, "\t-2: specify the secondary target file (without extension) to write to\n" );
}

/* Signal handler for ewfacquire
 */
void ewfacquirestream_signal_handler(
      libcsystem_signal_t signal LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "ewfacquirestream_signal_handler";

	LIBCSYSTEM_UNREFERENCED_PARAMETER( signal )

	ewfacquirestream_abort = 1;

	if( ewfacquirestream_imaging_handle != NULL )
	{
		if( imaging_handle_signal_abort(
		     ewfacquirestream_imaging_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal imaging handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libcsystem_file_io_close(
	     0 ) != 0 )
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* Reads a chunk of data from the file descriptor into the buffer
 * Returns the number of bytes read, 0 if at end of input or -1 on error
 */
ssize_t ewfacquirestream_read_chunk(
         libewf_handle_t *handle,
         int input_file_descriptor,
         uint8_t *buffer,
         size_t buffer_size,
         size32_t chunk_size,
         ssize64_t total_read_count,
         uint8_t read_error_retries,
         libcerror_error_t **error )
{
	static char *function         = "ewfacquirestream_read_chunk";
	ssize_t read_count            = 0;
	ssize_t buffer_offset         = 0;
	size_t read_size              = 0;
	size_t bytes_to_read          = 0;
	int32_t read_number_of_errors = 0;
	uint32_t read_error_offset    = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( input_file_descriptor == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input file descriptor.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid chunk size value zero or less.",
		 function );

		return( -1 );
	}
	if( total_read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid total read count value less than zero.",
		 function );

		return( -1 );
	}
	while( buffer_size > 0 )
	{
		/* Determine the number of bytes to read from the input
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

		while( read_number_of_errors <= read_error_retries )
		{
			read_count = libcsystem_file_io_read(
			              input_file_descriptor,
			              &( buffer[ buffer_offset + read_error_offset ] ),
			              bytes_to_read );

#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: read buffer at: %" PRIu64 " of size: %" PRIzd ".\n",
				 function,
				 total_read_count,
				 read_count );
			}
#endif
			if( read_count <= -1 )
			{
				if( ( errno == ESPIPE )
				 || ( errno == EPERM )
				 || ( errno == ENXIO )
				 || ( errno == ENODEV ) )
				{
					if( errno == ESPIPE )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: error reading data: invalid seek.",
						 function );
					}
					else if( errno == EPERM )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: error reading data: operation not permitted.",
						 function );
					}
					else if( errno == ENXIO )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: error reading data: no such device or address.",
						 function );
					}
					else if( errno == ENODEV )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: error reading data: no such device.",
						 function );
					}
					else
					{
						libcerror_system_set_error(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 errno,
						 "%s: error reading data.",
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
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: read error at offset %" PRIjd " after reading %" PRIzd " bytes.\n",
					 function,
					 total_read_count,
					 read_count );
				}
#endif
				/* There was a read error at a certain offset
				 */
				read_error_offset += read_count;
				bytes_to_read     -= read_count;
			}
			read_number_of_errors++;

			if( read_number_of_errors > read_error_retries )
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

/* Reads the input
 * Returns 1 if successful or -1 on error
 */
int ewfacquirestream_read_input(
     imaging_handle_t *imaging_handle,
     int input_file_descriptor,
     uint8_t swap_byte_pairs,
     uint8_t read_error_retries,
     uint8_t print_status_information,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	process_status_t *process_status             = NULL;
	storage_media_buffer_t *storage_media_buffer = NULL;
	uint8_t *data                                = NULL;
	static char *function                        = "ewfacquirestream_read_input";
	size64_t acquiry_count                       = 0;
	size32_t chunk_size                          = 0;
	size_t data_size                             = 0;
	size_t process_buffer_size                   = 0;
	size_t read_size                             = 0;
	ssize_t read_count                           = 0;
	ssize_t process_count                        = 0;
	ssize_t write_count                          = 0;
	int status                                   = PROCESS_STATUS_COMPLETED;

	if( imaging_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid imaging handle.",
		 function );

		return( -1 );
	}
#if !defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( imaging_handle->process_buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid imaging handle - process buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
	if( input_file_descriptor == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file descriptor.",
		 function );

		return( -1 );
	}
	if( imaging_handle_get_chunk_size(
	     imaging_handle,
	     &chunk_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk size.",
		 function );

		goto on_error;
	}
	if( chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	process_buffer_size = (size_t) chunk_size;
#else
	if( imaging_handle->process_buffer_size == 0 )
	{
		process_buffer_size = (size_t) chunk_size;
	}
	else
	{
		process_buffer_size = imaging_handle->process_buffer_size;
	}
#endif
	if( storage_media_buffer_initialize(
	     &storage_media_buffer,
	     process_buffer_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create storage media buffer.",
		 function );

		goto on_error;
	}
	if( imaging_handle_initialize_integrity_hash(
	     imaging_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize integrity hash(es).",
		 function );

		goto on_error;
        }
	if( process_status_initialize(
	     &process_status,
	     _LIBCSTRING_SYSTEM_STRING( "Acquiry" ),
	     _LIBCSTRING_SYSTEM_STRING( "acquired" ),
	     _LIBCSTRING_SYSTEM_STRING( "Written" ),
	     stdout,
	     print_status_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create process status.",
		 function );

		goto on_error;
	}
	if( process_status_start(
	     process_status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to start process status.",
		 function );

		goto on_error;
	}
	while( ( imaging_handle->acquiry_size == 0 )
	    || ( acquiry_count < (size64_t) imaging_handle->acquiry_size ) )
	{
		read_size = process_buffer_size;

		/* Align with acquiry offset if necessary
		 */
		if( ( imaging_handle->acquiry_offset != 0 )
		 && ( imaging_handle->acquiry_offset < (uint64_t) read_size ) )
		{
			read_size = (size_t) imaging_handle->acquiry_offset;
		}
		else if( ( imaging_handle->acquiry_size != 0 )
		      && ( ( (size64_t) imaging_handle->acquiry_size - acquiry_count ) < (size64_t) read_size ) )
		{
			read_size = (size_t) ( (ssize64_t) imaging_handle->acquiry_size - acquiry_count );
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
		              read_error_retries,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: error reading data from input.",
			 function );

			goto on_error;
		}
		if( read_count == 0 )
		{
			break;
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		storage_media_buffer->data_in_compression_buffer = 0;
#endif
		storage_media_buffer->raw_buffer_data_size = (size_t) read_count;

		/* Skip a certain number of bytes if necessary
		 */
		if( (size64_t) imaging_handle->acquiry_offset > acquiry_count )
		{
			imaging_handle->acquiry_offset -= read_count;

			continue;
		}
		/* Swap byte pairs
		 */
		if( swap_byte_pairs == 1 )
		{
			if( imaging_handle_swap_byte_pairs(
			     imaging_handle,
			     storage_media_buffer,
			     read_count,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to swap byte pairs.",
				 function );

				goto on_error;
			}
		}
		/* Digest hashes are calcultated after swap
		 */
		if( storage_media_buffer_get_data(
		     storage_media_buffer,
		     &data,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve storage media buffer data.",
			 function );

			goto on_error;
		}
		if( imaging_handle_update_integrity_hash(
		     imaging_handle,
		     data,
		     read_count,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to update integrity hash(es).",
			 function );

			goto on_error;
		}
		process_count = imaging_handle_prepare_write_buffer(
		                 imaging_handle,
		                 storage_media_buffer,
		                 error );

		if( process_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			"%s: unable to prepare buffer before write.",
			 function );

			goto on_error;
		}
		write_count = imaging_handle_write_buffer(
		               imaging_handle,
		               storage_media_buffer,
		               process_count,
		               error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write data to file.",
			 function );

			goto on_error;
		}
		acquiry_count += read_count;

		 if( process_status_update_unknown_total(
		      process_status,
		      acquiry_count,
		      error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update process status.",
			 function );

			goto on_error;
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free storage media buffer.",
		 function );

		goto on_error;
	}
	if( imaging_handle_finalize_integrity_hash(
	     imaging_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize integrity hash(es).",
		 function );

		goto on_error;
	}
	write_count = imaging_handle_finalize(
	               imaging_handle,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to finalize write.",
		 function );

		goto on_error;
	}
	acquiry_count += write_count;

	if( ewfacquirestream_abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     process_status,
	     acquiry_count,
	     status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to stop process status.",
		 function );

		goto on_error;
	}
	if( process_status_free(
	     &process_status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free process status.",
		 function );

		goto on_error;
	}
	if( ewfacquirestream_abort == 0 )
	{
		if( imaging_handle_print_hashes(
		     imaging_handle,
		     imaging_handle->notify_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print hashes.",
			 function );

			goto on_error;
		}
		if( log_handle != NULL )
		{
			if( imaging_handle_print_hashes(
			     imaging_handle,
			     log_handle->log_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print hashes in log handle.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( process_status != NULL )
	{
		process_status_stop(
		 process_status,
		 (size64_t) write_count,
		 PROCESS_STATUS_FAILED,
		 NULL );
		process_status_free(
		 &process_status,
		 NULL );
	}
	if( storage_media_buffer != NULL )
	{
		storage_media_buffer_free(
		 &storage_media_buffer,
		 NULL );
	}
	return( -1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error                                         = NULL;

	libcstring_system_character_t *log_filename                     = NULL;
	libcstring_system_character_t *option_additional_digest_types   = NULL;
	libcstring_system_character_t *option_bytes_per_sector          = NULL;
	libcstring_system_character_t *option_case_number               = NULL;
	libcstring_system_character_t *option_compression_values        = NULL;
	libcstring_system_character_t *option_description               = NULL;
	libcstring_system_character_t *option_examiner_name             = NULL;
	libcstring_system_character_t *option_evidence_number           = NULL;
	libcstring_system_character_t *option_format                    = NULL;
	libcstring_system_character_t *option_header_codepage           = NULL;
	libcstring_system_character_t *option_maximum_segment_size      = NULL;
	libcstring_system_character_t *option_media_flags               = NULL;
	libcstring_system_character_t *option_media_type                = NULL;
	libcstring_system_character_t *option_notes                     = NULL;
	libcstring_system_character_t *option_offset                    = NULL;
	libcstring_system_character_t *option_process_buffer_size       = NULL;
        libcstring_system_character_t *option_secondary_target_filename = NULL;
        libcstring_system_character_t *option_sectors_per_chunk         = NULL;
	libcstring_system_character_t *option_size                      = NULL;
        libcstring_system_character_t *option_target_filename           = NULL;
	libcstring_system_character_t *program                          = _LIBCSTRING_SYSTEM_STRING( "ewfacquirestream" );

	log_handle_t *log_handle                                        = NULL;

	libcstring_system_integer_t option                              = 0;
	size_t string_length                                            = 0;
	uint8_t calculate_md5                                           = 1;
	uint8_t print_status_information                                = 1;
	uint8_t read_error_retries                                      = 2;
	uint8_t resume_acquiry                                          = 0;
	uint8_t swap_byte_pairs                                         = 0;
	uint8_t verbose                                                 = 0;
	int result                                                      = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "ewftools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( libcsystem_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		ewfoutput_version_fprint(
		 stdout,
		 program );

		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		goto on_error;
	}
#if defined( WINAPI ) && !defined( __CYGWIN__ )
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
		ewfoutput_version_fprint(
		 stdout,
		 program );

		fprintf(
		 stderr,
		 "Unable to set stdin to binary mode.\n" );

		usage_fprint(
		 stdout );

		goto on_error;
	}
#endif
	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "A:b:B:c:C:d:D:e:E:f:hl:m:M:N:o:p:P:qsS:t:vV2:" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				ewfoutput_version_fprint(
				 stdout,
				 program );

				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				goto on_error;

			case (libcstring_system_integer_t) 'A':
				option_header_codepage = optarg;

				break;

			case (libcstring_system_integer_t) 'b':
				option_sectors_per_chunk = optarg;

				break;

			case (libcstring_system_integer_t) 'B':
				option_size = optarg;

				break;

			case (libcstring_system_integer_t) 'c':
				option_compression_values = optarg;

				break;

			case (libcstring_system_integer_t) 'C':
				option_case_number = optarg;

				break;

			case (libcstring_system_integer_t) 'd':
				option_additional_digest_types = optarg;

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
				option_format = optarg;

				break;

			case (libcstring_system_integer_t) 'h':
				ewfoutput_version_fprint(
				 stdout,
				 program );

				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libcstring_system_integer_t) 'm':
				option_media_type = optarg;

				break;

			case (libcstring_system_integer_t) 'M':
				option_media_flags = optarg;

				break;

			case (libcstring_system_integer_t) 'N':
				option_notes = optarg;

				break;

			case (libcstring_system_integer_t) 'o':
				option_offset = optarg;

				break;

			case (libcstring_system_integer_t) 'p':
				option_process_buffer_size = optarg;

				break;

			case (libcstring_system_integer_t) 'P':
				option_bytes_per_sector = optarg;

				break;

			case (libcstring_system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (libcstring_system_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (libcstring_system_integer_t) 'S':
				option_maximum_segment_size = optarg;

				break;

			case (libcstring_system_integer_t) 't':
				option_target_filename = optarg;

				break;

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				ewfoutput_version_fprint(
				 stdout,
				 program );

				ewfoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) '2':
				option_secondary_target_filename = optarg;

				break;
		}
	}
	ewfoutput_version_fprint(
	 stdout,
	 program );

	libcnotify_verbose_set(
	 verbose );

#if !defined( HAVE_LOCAL_LIBEWF )
	libewf_notify_set_verbose(
	 verbose );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif

	if( ( option_target_filename != NULL )
	 && ( option_secondary_target_filename != NULL ) )
	{
		string_length = libcstring_system_string_length(
				 option_secondary_target_filename );

		if( libcstring_system_string_length(
		     option_target_filename ) == string_length )
		{
			if( libcstring_system_string_compare(
			     option_target_filename,
			     option_secondary_target_filename,
			     string_length ) == 0 )
			{
				fprintf(
				 stderr,
				 "Primary and secondary target cannot be the same.\n" );

				goto on_error;
			}
		}
	}
	if( imaging_handle_initialize(
	     &ewfacquirestream_imaging_handle,
	     calculate_md5,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create imaging handle.\n" );

		goto on_error;
	}
	if( option_header_codepage != NULL )
	{
		result = imaging_handle_set_header_codepage(
			  ewfacquirestream_imaging_handle,
			  option_header_codepage,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set header codepage.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported header codepage defaulting to: ascii.\n" );
		}
	}
	if( option_target_filename != NULL )
	{
		if( imaging_handle_set_string(
		     ewfacquirestream_imaging_handle,
		     option_target_filename,
		     &( ewfacquirestream_imaging_handle->target_filename ),
		     &( ewfacquirestream_imaging_handle->target_filename_size ),
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set target filename.\n" );

			goto on_error;
		}
	}
	else
	{
		/* Make sure the target filename is set
		 */
		if( imaging_handle_set_string(
		     ewfacquirestream_imaging_handle,
		     _LIBCSTRING_SYSTEM_STRING( "image" ),
		     &( ewfacquirestream_imaging_handle->target_filename ),
		     &( ewfacquirestream_imaging_handle->target_filename_size ),
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set target filename.\n" );

			goto on_error;
		}
	}
	if( option_secondary_target_filename != NULL )
	{
		if( imaging_handle_set_string(
		     ewfacquirestream_imaging_handle,
		     option_secondary_target_filename,
		     &( ewfacquirestream_imaging_handle->secondary_target_filename ),
		     &( ewfacquirestream_imaging_handle->secondary_target_filename_size ),
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set secondary target filename.\n" );

			goto on_error;
		}
	}
	if( option_case_number == NULL )
	{
		option_case_number = _LIBCSTRING_SYSTEM_STRING( "case_number" );
	}
	if( imaging_handle_set_string(
	     ewfacquirestream_imaging_handle,
	     option_case_number,
	     &( ewfacquirestream_imaging_handle->case_number ),
	     &( ewfacquirestream_imaging_handle->case_number_size ),
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set case number.\n" );

		goto on_error;
	}
	if( option_description == NULL )
	{
		option_description = _LIBCSTRING_SYSTEM_STRING( "description" );
	}
	if( imaging_handle_set_string(
	     ewfacquirestream_imaging_handle,
	     option_description,
	     &( ewfacquirestream_imaging_handle->description ),
	     &( ewfacquirestream_imaging_handle->description_size ),
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set description.\n" );

		goto on_error;
	}
	if( option_evidence_number == NULL )
	{
		option_evidence_number = _LIBCSTRING_SYSTEM_STRING( "evidence_number" );
	}
	if( imaging_handle_set_string(
	     ewfacquirestream_imaging_handle,
	     option_evidence_number,
	     &( ewfacquirestream_imaging_handle->evidence_number ),
	     &( ewfacquirestream_imaging_handle->evidence_number_size ),
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set evidence number.\n" );

		goto on_error;
	}
	if( option_examiner_name == NULL )
	{
		option_examiner_name = _LIBCSTRING_SYSTEM_STRING( "examiner_name" );
	}
	if( imaging_handle_set_string(
	     ewfacquirestream_imaging_handle,
	     option_examiner_name,
	     &( ewfacquirestream_imaging_handle->examiner_name ),
	     &( ewfacquirestream_imaging_handle->examiner_name_size ),
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set examiner name.\n" );

		goto on_error;
	}
	if( option_notes == NULL )
	{
		option_notes = _LIBCSTRING_SYSTEM_STRING( "notes" );
	}
	if( imaging_handle_set_string(
	     ewfacquirestream_imaging_handle,
	     option_notes,
	     &( ewfacquirestream_imaging_handle->notes ),
	     &( ewfacquirestream_imaging_handle->notes_size ),
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set notes.\n" );

		goto on_error;
	}
	if( option_format != NULL )
	{
		result = imaging_handle_set_format(
			  ewfacquirestream_imaging_handle,
			  option_format,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set format.\n" );

			goto on_error;
		}
		else if( ( result == 0 )
		      || ( ewfacquirestream_imaging_handle->ewf_format == LIBEWF_FORMAT_EWF )
		      || ( ewfacquirestream_imaging_handle->ewf_format == LIBEWF_FORMAT_ENCASE1 )
		      || ( ewfacquirestream_imaging_handle->ewf_format == LIBEWF_FORMAT_SMART ) )
		{
			ewfacquirestream_imaging_handle->ewf_format = LIBEWF_FORMAT_ENCASE6;

			fprintf(
			 stderr,
			 "Unsupported EWF format defaulting to: encase6.\n" );
		}
	}
	if( option_compression_values != NULL )
	{
		result = imaging_handle_set_compression_values(
			  ewfacquirestream_imaging_handle,
			  option_compression_values,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set compression values.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported compression values defaulting to method: deflate with level: none.\n" );
		}
	}
	if( option_media_type != NULL )
	{
		result = imaging_handle_set_media_type(
			  ewfacquirestream_imaging_handle,
			  option_media_type,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set media type.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported media type defaulting to: fixed.\n" );
		}
	}
	if( option_media_flags != NULL )
	{
		result = imaging_handle_set_media_flags(
			  ewfacquirestream_imaging_handle,
			  option_media_flags,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set media flags.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported media flags defaulting to: physical.\n" );
		}
	}
	if( option_bytes_per_sector != NULL )
	{
		result = imaging_handle_set_bytes_per_sector(
			  ewfacquirestream_imaging_handle,
			  option_bytes_per_sector,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set bytes per sector.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported bytes per sector defaulting to: %" PRIu32 ".\n",
			 ewfacquirestream_imaging_handle->bytes_per_sector );
		}
	}
	if( option_sectors_per_chunk != NULL )
	{
		result = imaging_handle_set_sectors_per_chunk(
			  ewfacquirestream_imaging_handle,
			  option_sectors_per_chunk,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set sectors per chunk.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported sectors per chunk defaulting to: %" PRIu32 ".\n",
			 ewfacquirestream_imaging_handle->sectors_per_chunk );
		}
	}
	if( option_maximum_segment_size != NULL )
	{
		result = imaging_handle_set_maximum_segment_size(
			  ewfacquirestream_imaging_handle,
			  option_maximum_segment_size,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set maximum segment size.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			ewfacquirestream_imaging_handle->maximum_segment_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

			fprintf(
			 stderr,
			 "Unsupported maximum segment size defaulting to: %" PRIu64 ".\n",
			 ewfacquirestream_imaging_handle->maximum_segment_size );
		}
	}
	if( option_offset != NULL )
	{
		result = imaging_handle_set_acquiry_offset(
			  ewfacquirestream_imaging_handle,
			  option_offset,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set acquiry offset.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			ewfacquirestream_imaging_handle->acquiry_offset = 0;

			fprintf(
			 stderr,
			 "Unsupported acquiry offset defaulting to: 0.\n" );
		}
	}
	if( option_size != NULL )
	{
		result = imaging_handle_set_acquiry_size(
			  ewfacquirestream_imaging_handle,
			  option_size,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set acquiry size.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			ewfacquirestream_imaging_handle->acquiry_size = 0;

			fprintf(
			 stderr,
			 "Unsupported acquiry size defaulting to: all bytes.\n" );
		}
	}
	if( option_process_buffer_size != NULL )
	{
		result = imaging_handle_set_process_buffer_size(
			  ewfacquirestream_imaging_handle,
			  option_process_buffer_size,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set process buffer size.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported process buffer size defaulting to: chunk size.\n" );
		}
	}
	if( option_additional_digest_types != NULL )
	{
		result = imaging_handle_set_additional_digest_types(
			  ewfacquirestream_imaging_handle,
			  option_additional_digest_types,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set additional digest types.\n" );

			goto on_error;
		}
	}
	fprintf(
	 stdout,
	 "Using the following acquiry parameters:\n" );

	if( imaging_handle_print_parameters(
	     ewfacquirestream_imaging_handle,
	     0,
	     read_error_retries,
	     0,
	     0,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print acquiry parameters.\n" );

		goto on_error;
	}
	if( imaging_handle_open_output(
	     ewfacquirestream_imaging_handle,
	     ewfacquirestream_imaging_handle->target_filename,
	     resume_acquiry,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open output.\n" );

		goto on_error;
	}
	if( ewfacquirestream_imaging_handle->secondary_target_filename != NULL )
	{
		if( imaging_handle_open_secondary_output(
		     ewfacquirestream_imaging_handle,
		     ewfacquirestream_imaging_handle->secondary_target_filename,
		     resume_acquiry,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to open secondary output.\n" );

			goto on_error;
		}
	}
	if( imaging_handle_set_output_values(
	     ewfacquirestream_imaging_handle,
	     program,
	     _LIBCSTRING_SYSTEM_STRING( LIBEWF_VERSION_STRING ),
	     NULL,
	     NULL,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	if( libcsystem_signal_attach(
	     ewfacquirestream_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( log_filename != NULL )
	{
		if( log_handle_initialize(
		     &log_handle,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to create log handle.\n" );

			goto on_error;
		}
		if( log_handle_open(
		     log_handle,
		     log_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to open log file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
			 log_filename );

			goto on_error;
		}
	}
	result = ewfacquirestream_read_input(
	          ewfacquirestream_imaging_handle,
	          0,
	          swap_byte_pairs,
	          read_error_retries,
	          print_status_information,
	          log_handle,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to read input.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( log_handle != NULL )
	{
		if( log_handle_close(
		     log_handle,
		     &error ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to close log handle.\n" );

			goto on_error;
		}
		if( log_handle_free(
		     &log_handle,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to free log handle.\n" );

			goto on_error;
		}
	}
	if( libcsystem_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( imaging_handle_close(
	     ewfacquirestream_imaging_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close output.\n" );

		goto on_error;
	}
	if( imaging_handle_free(
	     &ewfacquirestream_imaging_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free imaging handle.\n" );

		goto on_error;
	}
	if( ewfacquirestream_abort != 0 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_LIBCSTRING_SYSTEM ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	if( result != 1 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_LIBCSTRING_SYSTEM ": FAILURE\n",
		 program );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "%" PRIs_LIBCSTRING_SYSTEM ": SUCCESS\n",
	 program );

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( log_handle != NULL )
	{
		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );
	}
	if( ewfacquirestream_imaging_handle != NULL )
	{
		imaging_handle_close(
		 ewfacquirestream_imaging_handle,
		 NULL );
		imaging_handle_free(
		 &ewfacquirestream_imaging_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

