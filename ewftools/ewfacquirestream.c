/*
 * Reads data from a stdin and writes it in EWF format
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_FCNTL_H ) || defined( WINAPI )
#include <fcntl.h>
#endif

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#include "byte_size_string.h"
#include "ewfcommon.h"
#include "ewfinput.h"
#include "ewftools_getopt.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcthreads.h"
#include "ewftools_libewf.h"
#include "ewftools_output.h"
#include "ewftools_signal.h"
#include "ewftools_unused.h"
#include "imaging_handle.h"
#include "log_handle.h"
#include "process_status.h"
#include "storage_media_buffer.h"
#include "storage_media_buffer_queue.h"

imaging_handle_t *ewfacquirestream_imaging_handle = NULL;
int ewfacquirestream_abort                        = 0;

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
	fprintf( stream, "Use ewfacquirestream to acquire data from a pipe and store it in the EWF format\n"
	                 "(Expert Witness Compression Format).\n\n" );

	fprintf( stream, "Usage: ewfacquirestream [ -A codepage ] [ -b number_of_sectors ]\n"
	                 "                        [ -B number_of_bytes ] [ -c compression_values ]\n"
	                 "                        [ -C case_number ] [ -d digest_type ]\n"
	                 "                        [ -D description ] [ -e examiner_name ]\n"
	                 "                        [ -E evidence_number ] [ -f format ] [ -j jobs ]\n"
	                 "                        [ -l log_filename ] [ -m media_type ]\n"
	                 "                        [ -M media_flags ] [ -N notes ]\n"
	                 "                        [ -o offset ] [ -p process_buffer_size ]\n"
	                 "                        [ -P bytes_per_sector ] [ -S segment_file_size ]\n"
	                 "                        [ -t target ] [ -2 secondary_target ]\n"
	                 "                        [ -hqsvVx ]\n\n" );

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
#if defined( HAVE_BZIP2_SUPPORT )
	                 "\t    compression method options: deflate (default), bzip2\n"
	                 "\t    (bzip2 is only supported by EWF2 formats)\n"
#else
	                 "\t    compression method options: deflate (default)\n"
#endif
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
	fprintf( stream, "\t-j: the number of concurrent processing jobs (threads), where\n"
	                 "\t    a number of 0 represents single-threaded mode (default is 4\n"
	                 "\t    if multi-threaded mode is supported)\n" );
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
		fprintf( stream, "\t-S: specify the segment file size in bytes (default is %" PRIs_SYSTEM ")\n"
		                 "\t    (minimum is %" PRIs_SYSTEM ", maximum is %" PRIs_SYSTEM " for encase6\n"
		                 "\t    and later formats and %" PRIs_SYSTEM " for other formats)\n",
		 default_segment_file_size_string,
		 minimum_segment_file_size_string,
		 maximum_64bit_segment_file_size_string,
		 maximum_32bit_segment_file_size_string );
	}
	else
	{
		fprintf( stream, "\t-S: specify the segment file size in bytes (default is %" PRIu32 ")\n"
		                 "\t    (minimum is %" PRIu32 ", maximum is %" PRIu64 " for encase6\n"
		                 "\t    and later formats and %" PRIu32 " for other formats)\n",
		 (uint32_t) EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE,
		 (uint32_t) EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE,
		 (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT,
		 (uint32_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT );
	}
	fprintf( stream, "\t-t: specify the target file (without extension) to write to (default\n"
	                 "\t    is image)\n" );
	fprintf( stream, "\t-v: verbose output to stderr\n" );
	fprintf( stream, "\t-V: print version\n" );
	fprintf( stream, "\t-x: use the data chunk functions instead of the buffered read and write\n"
	                 "\t    functions.\n" );
	fprintf( stream, "\t-2: specify the secondary target file (without extension) to write to\n" );
}

/* Signal handler for ewfacquire
 */
void ewfacquirestream_signal_handler(
      ewftools_signal_t signal EWFTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "ewfacquirestream_signal_handler";

	EWFTOOLS_UNREFERENCED_PARAMETER( signal )

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
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
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
         storage_media_buffer_t *storage_media_buffer,
         off64_t storage_media_offset,
         size_t buffer_read_size,
         size32_t chunk_size,
         uint8_t read_error_retries,
         libcerror_error_t **error )
{
	static char *function         = "ewfacquirestream_read_chunk";
	size_t buffer_offset          = 0;
	size_t chunk_read_size        = 0;
	size_t input_read_size        = 0;
	size_t remaining_read_size    = 0;
	ssize_t chunk_read_count      = 0;
	ssize_t input_read_count      = 0;
	int32_t read_number_of_errors = 0;

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
	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	if( buffer_read_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer read size value exceeds maximum.",
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
	remaining_read_size = buffer_read_size;

	while( remaining_read_size > 0 )
	{
		/* Determine the number of bytes to read from the input
		 * Read as much as possible in chunk sizes
		 */
		if( remaining_read_size < (size_t) chunk_size )
		{
			chunk_read_size = remaining_read_size;
		}
		else
		{
			chunk_read_size = chunk_size;
		}
		input_read_size = chunk_read_size;

		chunk_read_count      = 0;
		read_number_of_errors = 0;

		while( input_read_size > 0 )
		{
			if( ewfacquirestream_imaging_handle->abort != 0 )
			{
				break;
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: reading buffer at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd ".\n",
				 function,
				 storage_media_offset + (off64_t) buffer_offset,
				 storage_media_offset + (off64_t) buffer_offset,
				 input_read_size );
			}
#endif
#if defined( WINAPI ) && !defined( __CYGWIN__ )
			input_read_count = _read(
			                    input_file_descriptor,
			                    &( ( storage_media_buffer->raw_buffer )[ buffer_offset ] ),
			                    (unsigned int) input_read_size );
#else
			input_read_count = read(
			                    input_file_descriptor,
			                    &( ( storage_media_buffer->raw_buffer )[ buffer_offset ] ),
			                    input_read_size );
#endif
			if( input_read_count < 0 )
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
				read_number_of_errors++;
			}
			/* No bytes were read
			 */
			else if( input_read_count == 0 )
			{
				break;
			}
			else
			{
				chunk_read_count += input_read_count;
				buffer_offset    += input_read_count;
				input_read_size  -= input_read_count;

				/* The entire read is OK
				 */
				if( chunk_read_count == (ssize_t) chunk_read_size )
				{
					break;
				}
				/* There was a read error at a certain offset
				 */
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: read error at offset: %" PRIi64 " (0x%08" PRIx64 ") when reading %" PRIzd " bytes.\n",
					 function,
					 storage_media_offset + (off64_t) buffer_offset,
					 storage_media_offset + (off64_t) buffer_offset,
					 input_read_count );
				}
#endif /* defined( HAVE_VERBOSE_OUTPUT ) */

				read_number_of_errors++;
			}
			if( read_number_of_errors > read_error_retries )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: error reading data.",
				 function );

				return( -1 );
			}
		}
		if( chunk_read_count == 0 )
		{
			break;
		}
		remaining_read_size -= chunk_read_count;
	}
	storage_media_buffer->storage_media_offset = storage_media_offset;
	storage_media_buffer->requested_size       = buffer_read_size;
	storage_media_buffer->raw_buffer_data_size = buffer_offset;

	return( (ssize_t) buffer_offset );
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
     uint8_t use_data_chunk_functions,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	storage_media_buffer_t *storage_media_buffer = NULL;
	static char *function                        = "ewfacquirestream_read_input";
	off64_t storage_media_offset                 = 0;
	size64_t remaining_aquiry_size               = 0;
	size64_t skip_aquiry_size                    = 0;
	size_t process_buffer_size                   = 0;
	size_t read_size                             = 0;
	ssize_t read_count                           = 0;
	ssize_t write_count                          = 0;
	uint8_t storage_media_buffer_mode            = 0;
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
#if !defined( HAVE_MULTI_THREAD_SUPPORT )
	if( imaging_handle->number_of_threads != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: multi-threading not supported.",
		 function );

		return( -1 );
	}
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

	if( imaging_handle_get_process_buffer_size(
	     imaging_handle,
	     use_data_chunk_functions,
	     &process_buffer_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve process buffer size.",
		 function );

		goto on_error;
	}
	if( use_data_chunk_functions != 0 )
	{
		storage_media_buffer_mode = STORAGE_MEDIA_BUFFER_MODE_CHUNK_DATA;
	}
	else
	{
		storage_media_buffer_mode = STORAGE_MEDIA_BUFFER_MODE_BUFFERED;
	}
	if( imaging_handle->number_of_threads == 0 )
	{
		if( storage_media_buffer_initialize(
		     &storage_media_buffer,
		     imaging_handle->output_handle,
		     storage_media_buffer_mode,
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
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT )
	else
	{
		if( imaging_handle_threads_start(
		     imaging_handle,
		     process_buffer_size,
		     storage_media_buffer_mode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to start threads.",
			 function );

			goto on_error;
		}
	}
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

	if( imaging_handle_start(
	     imaging_handle,
	     print_status_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to start imaging handle.",
		 function );

		goto on_error;
	}
	remaining_aquiry_size = imaging_handle->acquiry_size;
	skip_aquiry_size      = imaging_handle->acquiry_offset;

	while( ( imaging_handle->acquiry_size == 0 )
	    || ( remaining_aquiry_size > 0 ) )
	{
		if( imaging_handle->abort != 0 )
		{
			break;
		}
#if defined( HAVE_MULTI_THREAD_SUPPORT )
		if( imaging_handle->number_of_threads > 0 )
		{
			if( storage_media_buffer_queue_grab_buffer(
			     imaging_handle->storage_media_buffer_queue,
			     &storage_media_buffer,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to grab storage media buffer from queue.",
				 function );

				goto on_error;
			}
			if( storage_media_buffer == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing storage media buffer.",
				 function );

				goto on_error;
			}
		}
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

		read_size = process_buffer_size;

		/* Align with acquiry offset if necessary
		 */
		if( ( skip_aquiry_size > 0 )
		 && ( skip_aquiry_size < (size64_t) read_size ) )
		{
			read_size = (size_t) skip_aquiry_size;
		}
		else if( ( imaging_handle->acquiry_size != 0 )
		      && ( remaining_aquiry_size < (size64_t) read_size ) )
		{
			read_size = (size_t) remaining_aquiry_size;
		}
		/* Read a chunk from the file descriptor
		 */
		read_count = ewfacquirestream_read_chunk(
		              imaging_handle->output_handle,
		              input_file_descriptor,
		              storage_media_buffer,
		              storage_media_offset,
		              process_buffer_size,
		              (size32_t) read_size,
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
		storage_media_offset += read_count;

		/* Skip a certain number of bytes if necessary
		 */
		if( skip_aquiry_size > 0 )
		{
			imaging_handle->last_offset_written += read_count;
			skip_aquiry_size                    -= read_count;

			continue;
		}
		remaining_aquiry_size -= read_count;

		if( imaging_handle_update(
		     imaging_handle,
		     storage_media_buffer,
		     read_count,
		     0,
		     swap_byte_pairs,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to update imaging handle.",
			 function );

			goto on_error;
		}
#if defined( HAVE_MULTI_THREAD_SUPPORT )
		if( imaging_handle->number_of_threads > 0 )
		{
			if( libcthreads_thread_pool_push(
			     imaging_handle->process_thread_pool,
			     (intptr_t *) storage_media_buffer,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to push storage media buffer onto process thread pool queue.",
				 function );

				return( -1 );
			}
			storage_media_buffer = NULL;
		}
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */
	}
	if( imaging_handle->number_of_threads == 0 )
	{
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
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT )
	else
	{
		if( imaging_handle_threads_stop(
		     imaging_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to stop threads.",
			 function );

			return( -1 );
		}
	}
#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

	if( imaging_handle_stop(
	     imaging_handle,
	     0,
	     status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to stop image handle.",
		 function );

		goto on_error;
	}
	if( imaging_handle->abort == 0 )
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
	if( imaging_handle->number_of_threads == 0 )
	{
		if( storage_media_buffer != NULL )
		{
			storage_media_buffer_free(
			 &storage_media_buffer,
			 NULL );
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT )
	else
	{
		imaging_handle_threads_stop(
		 imaging_handle,
		 NULL );
	}
#endif
	if( imaging_handle->process_status != NULL )
	{
		process_status_stop(
		 imaging_handle->process_status,
		 (size64_t) write_count,
		 PROCESS_STATUS_FAILED,
		 NULL );

		process_status_free(
		 &( imaging_handle->process_status ),
		 NULL );
	}
	return( -1 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error                             = NULL;
	log_handle_t *log_handle                             = NULL;
	system_character_t *log_filename                     = NULL;
	system_character_t *option_additional_digest_types   = NULL;
	system_character_t *option_bytes_per_sector          = NULL;
	system_character_t *option_case_number               = NULL;
	system_character_t *option_compression_values        = NULL;
	system_character_t *option_description               = NULL;
	system_character_t *option_evidence_number           = NULL;
	system_character_t *option_examiner_name             = NULL;
	system_character_t *option_format                    = NULL;
	system_character_t *option_header_codepage           = NULL;
	system_character_t *option_maximum_segment_size      = NULL;
	system_character_t *option_media_flags               = NULL;
	system_character_t *option_media_type                = NULL;
	system_character_t *option_notes                     = NULL;
	system_character_t *option_number_of_jobs            = NULL;
	system_character_t *option_offset                    = NULL;
	system_character_t *option_process_buffer_size       = NULL;
        system_character_t *option_secondary_target_filename = NULL;
        system_character_t *option_sectors_per_chunk         = NULL;
	system_character_t *option_size                      = NULL;
        system_character_t *option_target_filename           = NULL;
	system_character_t *program                          = _SYSTEM_STRING( "ewfacquirestream" );
	system_integer_t option                              = 0;
	size_t string_length                                 = 0;
	uint8_t calculate_md5                                = 1;
	uint8_t print_status_information                     = 1;
	uint8_t read_error_retries                           = 2;
	uint8_t resume_acquiry                               = 0;
	uint8_t swap_byte_pairs                              = 0;
	uint8_t use_data_chunk_functions                     = 0;
	uint8_t verbose                                      = 0;
	int result                                           = 0;

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
	if( ewftools_output_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		ewftools_output_version_fprint(
		 stdout,
		 program );

		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

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
		ewftools_output_version_fprint(
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
	while( ( option = ewftools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "A:b:B:c:C:d:D:e:E:f:hj:l:m:M:N:o:p:P:qsS:t:vVx2:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				ewftools_output_version_fprint(
				 stdout,
				 program );

				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				goto on_error;

			case (system_integer_t) 'A':
				option_header_codepage = optarg;

				break;

			case (system_integer_t) 'b':
				option_sectors_per_chunk = optarg;

				break;

			case (system_integer_t) 'B':
				option_size = optarg;

				break;

			case (system_integer_t) 'c':
				option_compression_values = optarg;

				break;

			case (system_integer_t) 'C':
				option_case_number = optarg;

				break;

			case (system_integer_t) 'd':
				option_additional_digest_types = optarg;

				break;

			case (system_integer_t) 'D':
				option_description = optarg;

				break;

			case (system_integer_t) 'e':
				option_examiner_name = optarg;

				break;

			case (system_integer_t) 'E':
				option_evidence_number = optarg;

				break;

			case (system_integer_t) 'f':
				option_format = optarg;

				break;

			case (system_integer_t) 'h':
				ewftools_output_version_fprint(
				 stdout,
				 program );

				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'j':
				option_number_of_jobs = optarg;

				break;

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'm':
				option_media_type = optarg;

				break;

			case (system_integer_t) 'M':
				option_media_flags = optarg;

				break;

			case (system_integer_t) 'N':
				option_notes = optarg;

				break;

			case (system_integer_t) 'o':
				option_offset = optarg;

				break;

			case (system_integer_t) 'p':
				option_process_buffer_size = optarg;

				break;

			case (system_integer_t) 'P':
				option_bytes_per_sector = optarg;

				break;

			case (system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (system_integer_t) 's':
				swap_byte_pairs = 1;

				break;

			case (system_integer_t) 'S':
				option_maximum_segment_size = optarg;

				break;

			case (system_integer_t) 't':
				option_target_filename = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				ewftools_output_version_fprint(
				 stdout,
				 program );

				ewftools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'x':
				use_data_chunk_functions = 1;

				break;

			case (system_integer_t) '2':
				option_secondary_target_filename = optarg;

				break;
		}
	}
	ewftools_output_version_fprint(
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
		string_length = system_string_length(
				 option_secondary_target_filename );

		if( system_string_length(
		     option_target_filename ) == string_length )
		{
			if( system_string_compare(
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
	     use_data_chunk_functions,
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
		     _SYSTEM_STRING( "image" ),
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
	if( imaging_handle_check_write_access(
	     ewfacquirestream_imaging_handle,
	     ewfacquirestream_imaging_handle->target_filename,
	     &error ) != 1 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libcnotify_print_error_backtrace(
		 error );
#endif
		libcerror_error_free(
		 &error );

		fprintf(
		 stdout,
		 "Unable to write target file.\n" );

		goto on_error;
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
		/* Make sure we can write the secondary target file
		 */
		if( imaging_handle_check_write_access(
		     ewfacquirestream_imaging_handle,
		     ewfacquirestream_imaging_handle->secondary_target_filename,
		     &error ) != 1 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libcnotify_print_error_backtrace(
			 error );
#endif
			libcerror_error_free(
			 &error );

			fprintf(
			 stdout,
			 "Unable to write secondary target file.\n" );

			goto on_error;
		}
	}
	if( option_case_number == NULL )
	{
		option_case_number = _SYSTEM_STRING( "case_number" );
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
		option_description = _SYSTEM_STRING( "description" );
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
		option_evidence_number = _SYSTEM_STRING( "evidence_number" );
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
		option_examiner_name = _SYSTEM_STRING( "examiner_name" );
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
		option_notes = _SYSTEM_STRING( "notes" );
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
	if( option_number_of_jobs != NULL )
	{
#if defined( HAVE_MULTI_THREAD_SUPPORT )
		result = imaging_handle_set_number_of_threads(
			  ewfacquirestream_imaging_handle,
			  option_number_of_jobs,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set number of jobs (threads).\n" );

			goto on_error;
		}
		else if( ( result == 0 )
		      || ( ewfacquirestream_imaging_handle->number_of_threads > (int) 32 ) )
		{
			ewfacquirestream_imaging_handle->number_of_threads = 4;

			fprintf(
			 stderr,
			 "Unsupported number of jobs (threads) defaulting to: %d.\n",
			 ewfacquirestream_imaging_handle->number_of_threads );
		}
#else
		ewfacquirestream_imaging_handle->number_of_threads = 0;

		fprintf(
		 stderr,
		 "Unsupported number of jobs (threads) defaulting to: %d.\n",
		 ewfacquirestream_imaging_handle->number_of_threads );

#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */
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
	     _SYSTEM_STRING( LIBEWF_VERSION_STRING ),
	     NULL,
	     NULL,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	if( ewftools_signal_attach(
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
			 "Unable to open log file: %" PRIs_SYSTEM ".\n",
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
	          use_data_chunk_functions,
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
	if( ewftools_signal_detach(
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
		 "%" PRIs_SYSTEM ": ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	if( result != 1 )
	{
		fprintf(
		 stdout,
		 "%" PRIs_SYSTEM ": FAILURE\n",
		 program );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "%" PRIs_SYSTEM ": SUCCESS\n",
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

