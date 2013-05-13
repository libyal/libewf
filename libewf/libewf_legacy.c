/*
 * Legacy functions
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

#include "libewf_definitions.h"
#include "libewf_file_entry.h"
#include "libewf_handle.h"
#include "libewf_legacy.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcstring.h"
#include "libewf_metadata.h"
#include "libewf_notify.h"
#include "libewf_types.h"

#if !defined( HAVE_LOCAL_LIBEWF )

/* Returns the flags for reading
 */
uint8_t libewf_get_flags_read(
         void )
{
	return( (uint8_t) LIBEWF_ACCESS_FLAG_READ );
}

/* Returns the flags for reading and writing
 */
uint8_t libewf_get_flags_read_write(
         void )
{
	return( (uint8_t) ( LIBEWF_ACCESS_FLAG_READ | LIBEWF_ACCESS_FLAG_WRITE ) );
}

/* Returns the flags for writing
 */
uint8_t libewf_get_flags_write(
         void )
{
	return( (uint8_t) LIBEWF_ACCESS_FLAG_WRITE );
}

/* Returns the flags for resume writing
 */
uint8_t libewf_get_flags_write_resume(
         void )
{
	return( (uint8_t) LIBEWF_ACCESS_FLAG_WRITE | LIBEWF_ACCESS_FLAG_RESUME );
}

/* Sets the maximum amount of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_maximum_amount_of_open_handles(
     libewf_handle_t *handle,
     int maximum_amount_of_open_handles,
     libcerror_error_t **error )
{
	return( libewf_handle_set_maximum_number_of_open_handles(
	         handle,
	         maximum_amount_of_open_handles,
	         error ) );
}

/* Retrieves the segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_segment_file_size(
     libewf_handle_t *handle,
     size64_t *segment_file_size,
     libcerror_error_t **error )
{
	return( libewf_handle_get_maximum_segment_size(
	         handle,
	         segment_file_size,
	         error ) );
}

/* Sets the segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_file_size(
     libewf_handle_t *handle,
     size64_t segment_file_size,
     libcerror_error_t **error )
{
	return( libewf_handle_set_maximum_segment_size(
	         handle,
	         segment_file_size,
	         error ) );
}

/* Retrieves the delta segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_delta_segment_file_size(
     libewf_handle_t *handle,
     size64_t *delta_segment_file_size,
     libcerror_error_t **error )
{
	return( libewf_handle_get_maximum_delta_segment_size(
	         handle,
	         delta_segment_file_size,
	         error ) );
}

/* Sets the delta segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_delta_segment_file_size(
     libewf_handle_t *handle,
     size64_t delta_segment_file_size,
     libcerror_error_t **error )
{
	return( libewf_handle_set_maximum_delta_segment_size(
	         handle,
	         delta_segment_file_size,
	         error ) );
}

/* Retrieves the amount of sectors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_amount_of_sectors(
     libewf_handle_t *handle,
     uint64_t *amount_of_sectors,
     libcerror_error_t **error )
{
	return( libewf_handle_get_number_of_sectors(
	         handle,
	         amount_of_sectors,
	         error ) );
}

/* Retrieves the number of chunks written
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_chunks_written(
     libewf_handle_t *handle,
     uint32_t *number_of_chunks,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_number_of_chunks_written";

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
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.",
		 function );

		return( -1 );
	}
	if( number_of_chunks == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of chunks.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_io_handle->number_of_chunks_written > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of chunks written value out of bounds.",
		 function );

		return( -1 );
	}
	*number_of_chunks = (uint32_t) internal_handle->write_io_handle->number_of_chunks_written;

	return( 1 );
}

/* Retrieves the amount of chunks written
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_write_amount_of_chunks(
     libewf_handle_t *handle,
     uint32_t *amount_of_chunks,
     libcerror_error_t **error )
{
	return( libewf_handle_get_number_of_chunks_written(
	         handle,
	         amount_of_chunks,
	         error ) );
}

/* Sets the read wipe chunk on error
 * The chunk is not wiped if read raw is used
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_read_wipe_chunk_on_error(
     libewf_handle_t *handle,
     uint8_t wipe_on_error,
     libcerror_error_t **error )
{
	return( libewf_handle_set_read_zero_chunk_on_error(
	         handle,
	         wipe_on_error,
	         error ) );
}

/* Retrieves the amount of acquiry errors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_amount_of_acquiry_errors(
     libewf_handle_t *handle,
     uint32_t *amount_of_errors,
     libcerror_error_t **error )
{
	return( libewf_handle_get_number_of_acquiry_errors(
	         handle,
	         amount_of_errors,
	         error ) );
}

/* Add an acquiry error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_add_acquiry_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	return( libewf_handle_append_acquiry_error(
	         handle,
	         start_sector,
	         number_of_sectors,
	         error ) );
}

/* Retrieves the number of CRC errors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_number_of_crc_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libcerror_error_t **error )
{
	return( libewf_handle_get_number_of_checksum_errors(
	         handle,
	         number_of_errors,
	         error ) );
}

/* Retrieves the amount of CRC errors
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_amount_of_crc_errors(
     libewf_handle_t *handle,
     uint32_t *amount_of_errors,
     libcerror_error_t **error )
{
	return( libewf_handle_get_number_of_checksum_errors(
	         handle,
	         amount_of_errors,
	         error ) );
}

/* Retrieves a CRC error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_crc_error(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	return( libewf_handle_get_checksum_error(
	         handle,
	         index,
	         start_sector,
	         number_of_sectors,
	         error ) );
}

/* Add a CRC error
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_add_crc_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	return( libewf_handle_append_checksum_error(
	         handle,
	         start_sector,
	         number_of_sectors,
	         error ) );
}

/* Retrieves the amount of sessions
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_amount_of_sessions(
     libewf_handle_t *handle,
     uint32_t *amount_of_sessions,
     libcerror_error_t **error )
{
	return( libewf_handle_get_number_of_sessions(
	         handle,
	         amount_of_sessions,
	         error ) );
}

/* Add a session
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_add_session(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	return( libewf_handle_append_session(
	         handle,
	         start_sector,
	         number_of_sectors,
	         error ) );
}

/* Retrieves the amount of header values
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_handle_get_amount_of_header_values(
     libewf_handle_t *handle,
     uint32_t *amount_of_values,
     libcerror_error_t **error )
{
	return( libewf_handle_get_number_of_header_values(
	         handle,
	         amount_of_values,
	         error ) );
}

/* Retrieves the size of the UTF-8 encoded header value of an identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     libcerror_error_t **error )
{
	return( libewf_handle_get_utf8_header_value_size(
	         handle,
	         identifier,
	         identifier_length,
	         value_size,
	         error ) );
}

/* Retrieves the UTF-8 encoded header value of an identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     libcerror_error_t **error )
{
	return( libewf_handle_get_utf8_header_value(
	         handle,
	         identifier,
	         identifier_length,
	         value,
	         value_size,
	         error ) );
}

/* Sets the UTF-8 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     libcerror_error_t **error )
{
	return( libewf_handle_set_utf8_header_value(
	         handle,
	         identifier,
	         identifier_length,
	         value,
	         value_length,
	         error ) );
}

/* Retrieves the amount of hash values
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 */
int libewf_handle_get_amount_of_hash_values(
     libewf_handle_t *handle,
     uint32_t *amount_of_values,
     libcerror_error_t **error )
{
	return( libewf_handle_get_number_of_hash_values(
	         handle,
	         amount_of_values,
	         error ) );
}

/* Retrieves the size of the UTF-8 encoded hash value of an identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     libcerror_error_t **error )
{
	return( libewf_handle_get_utf8_hash_value_size(
	         handle,
	         identifier,
	         identifier_length,
	         value_size,
	         error ) );
}

/* Retrieves the UTF-8 encoded hash value of an identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     libcerror_error_t **error )
{
	return( libewf_handle_get_utf8_hash_value(
	         handle,
	         identifier,
	         identifier_length,
	         value,
	         value_size,
	         error ) );
}

/* Sets the UTF-8 encoded hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     libcerror_error_t **error )
{
	return( libewf_handle_set_utf8_hash_value(
	         handle,
	         identifier,
	         identifier_length,
	         value,
	         value_length,
	         error ) );
}

/* Retrieves the amount of sub file entries
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_amount_of_sub_file_entries(
     libewf_file_entry_t *file_entry,
     int *amount_of_sub_file_entries,
     libcerror_error_t **error )
{
	return( libewf_file_entry_get_number_of_sub_file_entries(
	         file_entry,
	         amount_of_sub_file_entries,
	         error ) );
}

/* Retrieves the size of the UTF-8 encoded name from the referenced file entry
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_name_size(
     libewf_file_entry_t *file_entry,
     size_t *name_size,
     libcerror_error_t **error )
{
	return( libewf_file_entry_get_utf8_name_size(
	         file_entry,
	         name_size,
	         error ) );
}

/* Retrieves the UTF-8 encoded name value from the referenced file entry
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_name(
     libewf_file_entry_t *file_entry,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	return( libewf_file_entry_get_utf8_name(
	         file_entry,
	         name,
	         name_size,
	         error ) );
}

#endif

#if defined( HAVE_V1_API )

#if !defined( HAVE_LOCAL_LIBEWF )

/* Sets the notify values
 */
void libewf_set_notify_values(
      FILE *stream,
      int verbose )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_notify_values";

	if( libewf_notify_set_stream(
	     stream,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to set notify stream.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	libewf_notify_set_verbose(
	 verbose );
}

#endif

/* Signals the libewf handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libewf_signal_abort(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_signal_abort";

	if( libewf_handle_signal_abort(
	     handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to signal abort.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns a pointer to the new instance of handle, NULL on error
 */
libewf_handle_t *libewf_open(
                  char * const filenames[],
                  int amount_of_files,
                  uint8_t flags )
{
	libcerror_error_t *error = NULL;
	libewf_handle_t *handle = NULL;
	static char *function   = "libewf_open";

	if( libewf_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( libewf_handle_open(
	     handle,
	     filenames,
	     amount_of_files,
	     flags,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		libewf_handle_free(
		 &handle,
		 NULL );

		return( NULL );
	}
	return( handle );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns a pointer to the new instance of handle, NULL on error
 */
libewf_handle_t *libewf_open_wide(
                  wchar_t * const filenames[],
                  int amount_of_files,
                  uint8_t flags )
{
	libcerror_error_t *error = NULL;
	libewf_handle_t *handle = NULL;
	static char *function   = "libewf_open_wide";

	if( libewf_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( libewf_handle_open_wide(
	     handle,
	     filenames,
	     amount_of_files,
	     flags,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( handle );
}
#endif

/* Closes the EWF handle and frees memory used within the handle
 * Returns 0 if successful or -1 on error
 */
int libewf_close(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_close";

	if( libewf_handle_close(
	     handle,
	     &error ) != 0 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to close handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	if( libewf_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Seeks a certain offset of the media data within the EWF file(s)
 * It will set the related file offset to the specific chunk offset
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_seek_offset(
         libewf_handle_t *handle,
         off64_t offset )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_seek_offset";

	offset = libewf_handle_seek_offset(
	          handle,
	          offset,
	          SEEK_SET,
	          &error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( offset );
}

/* Retrieves the current offset of the media data within the EWF file(s)
 * Returns the offset if successful or -1 on error
 */
off64_t libewf_get_offset(
         libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_offset";
	off64_t offset          = 0;

	if( libewf_handle_get_offset(
	     handle,
	     &offset,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( offset );
}

/* Prepares a buffer with chunk data after reading it according to the handle settings
 * intended for raw read
 * The buffer size cannot be larger than the chunk size
 * Returns the resulting chunk size or -1 on error
 */
ssize_t libewf_raw_read_prepare_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         void *uncompressed_buffer,
         size_t *uncompressed_buffer_size,
         int8_t is_compressed,
         uint32_t chunk_checksum,
         int8_t chunk_io_flags )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_raw_read_prepare_buffer";
	ssize_t chunk_data_size = 0;

	chunk_data_size = libewf_handle_prepare_read_chunk(
	                   handle,
	                   buffer,
	                   buffer_size,
	                   uncompressed_buffer,
	                   uncompressed_buffer_size,
	                   is_compressed,
	                   chunk_checksum,
	                   chunk_io_flags,
	                   &error );

	if( chunk_data_size == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to raw read prepare buffer.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( chunk_data_size );
}

/* Reads 'raw' data from the current offset into a buffer
 * size contains the size of the buffer
 * The function sets the chunk checksum, is compressed and read checksum values
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_raw_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         int8_t *is_compressed,
         uint32_t *chunk_checksum,
         int8_t *chunk_io_flags )
{
	uint8_t checksum_buffer[ 4 ];

	libcerror_error_t *error = NULL;
	static char *function   = "libewf_raw_read_buffer";
	ssize_t read_count      = 0;

	read_count = libewf_handle_read_chunk(
	              handle,
	              buffer,
	              buffer_size,
	              is_compressed,
	              checksum_buffer,
	              chunk_checksum,
	              chunk_io_flags,
	              &error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to raw read buffer.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( read_count );
}

/* Reads media data from the last current into a buffer
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_read_buffer";
	ssize_t read_count      = 0;

	read_count = libewf_handle_read_buffer(
	              handle,
	              buffer,
	              buffer_size,
	              &error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( read_count );
}

/* Reads media data at a specific offset
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_read_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_read_random";
	ssize_t read_count      = 0;

	read_count = libewf_handle_read_random(
	              handle,
	              buffer,
	              buffer_size,
	              offset,
	              &error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read random.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( read_count );
}

/* Prepares a buffer with chunk data before writing according to the handle settings
 * intended for raw write
 * The buffer size cannot be larger than the chunk size
 * The function sets the chunk checksum, is compressed and write checksum values
 * Returns the resulting chunk size or -1 on error
 */
ssize_t libewf_raw_write_prepare_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         void *compressed_buffer,
         size_t *compressed_buffer_size,
         int8_t *is_compressed,
         uint32_t *chunk_checksum,
         int8_t *chunk_io_flags )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_raw_write_prepare_buffer";
	ssize_t chunk_data_size = 0;

	chunk_data_size = libewf_handle_prepare_write_chunk(
	                   handle,
	                   buffer,
	                   buffer_size,
	                   compressed_buffer,
	                   compressed_buffer_size,
	                   is_compressed,
	                   chunk_checksum,
	                   chunk_io_flags,
	                   &error );

	if( chunk_data_size == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to raw write prepare buffer.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( chunk_data_size );
}

/* Writes 'raw' data in EWF format the current offset
 * the necessary settings of the write values must have been made
 * size contains the size of the data within the buffer while
 * data size contains the size of the actual input data
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_raw_write_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         size_t data_size,
         int8_t is_compressed,
         uint32_t chunk_checksum,
         int8_t chunk_io_flags )
{
	uint8_t checksum_buffer[ 4 ];

	libcerror_error_t *error = NULL;
	static char *function   = "libewf_raw_write_buffer";
	ssize_t write_count     = 0;

	write_count = libewf_handle_write_chunk(
	               handle,
	               buffer,
	               buffer_size,
	               data_size,
	               is_compressed,
	               checksum_buffer,
	               chunk_checksum,
	               chunk_io_flags,
	               &error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to raw write buffer.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( write_count );
}

/* Writes data in EWF format at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_write_buffer";
	ssize_t write_count     = 0;

	write_count = libewf_handle_write_buffer(
	               handle,
	               buffer,
	               buffer_size,
	               &error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write buffer.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( write_count );
}

/* Writes data in EWF format at a specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_write_random";
	ssize_t write_count     = 0;

	write_count = libewf_handle_write_random(
	               handle,
	               buffer,
	               buffer_size,
	               offset,
	               &error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write random.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( write_count );
}

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after write from stream
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_write_finalize(
         libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_write_finalize";
	ssize_t write_count     = 0;

	write_count = libewf_handle_write_finalize(
	               handle,
	               &error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to finalize write.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( write_count );
}

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_segment_filename";
	int result              = 0;

	result = libewf_handle_get_segment_filename(
	          handle,
	          filename,
	          filename_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment filename.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( result );
}

/* Sets the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_set_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_segment_filename";

	if( libewf_handle_set_segment_filename(
	     handle,
	     filename,
	     filename_length,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set segment filename.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_segment_filename_wide";
	int result              = 0;

	result = libewf_handle_get_segment_filename_wide(
	          handle,
	          filename,
	          filename_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve wide segment filename.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( result );
}

/* Sets the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_set_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_segment_filename_wide";

	if( libewf_handle_set_segment_filename_wide(
	     handle,
	     filename,
	     filename_length,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set wide segment filename.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_get_segment_file_size(
     libewf_handle_t *handle,
     size64_t *segment_file_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_segment_file_size";

	if( libewf_handle_get_segment_file_size(
	     handle,
	     segment_file_size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file size.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_set_segment_file_size(
     libewf_handle_t *handle,
     size64_t segment_file_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_segment_file_size";

	if( libewf_handle_set_segment_file_size(
	     handle,
	     segment_file_size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment file size.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_delta_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_delta_segment_filename";
	int result              = 0;

	result = libewf_handle_get_delta_segment_filename(
	          handle,
	          filename,
	          filename_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve delta segment filename.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( result );
}

/* Sets the delta segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_set_delta_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_delta_segment_filename";

	if( libewf_handle_set_delta_segment_filename(
	     handle,
	     filename,
	     filename_length,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set delta segment filename.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_delta_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_delta_segment_filename_wide";
	int result              = 0;

	result = libewf_handle_get_delta_segment_filename_wide(
	          handle,
	          filename,
	          filename_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve wide delta segment filename.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( result );
}

/* Sets the delta segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_set_delta_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_delta_segment_filename_wide";

	if( libewf_handle_set_delta_segment_filename_wide(
	     handle,
	     filename,
	     filename_length,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set wide delta segment filename.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the delta segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_get_delta_segment_file_size(
     libewf_handle_t *handle,
     size64_t *delta_segment_file_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_delta_segment_file_size";

	if( libewf_handle_get_delta_segment_file_size(
	     handle,
	     delta_segment_file_size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve delta segment file size.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the delta segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_set_delta_segment_file_size(
     libewf_handle_t *handle,
     size64_t delta_segment_file_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_delta_segment_file_size";

	if( libewf_handle_set_delta_segment_file_size(
	     handle,
	     delta_segment_file_size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set delta segment file size.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_get_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t *sectors_per_chunk )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_sectors_per_chunk";

	if( libewf_handle_get_sectors_per_chunk(
	     handle,
	     sectors_per_chunk,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of sectors per chunk.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the amount of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_set_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t sectors_per_chunk )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_sectors_per_chunk";

	if( libewf_handle_set_sectors_per_chunk(
	     handle,
	     sectors_per_chunk,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the amount of sectors per chunk.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of bytes per sector
 * Returns 1 if successful or -1 on error
 */
int libewf_get_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t *bytes_per_sector )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_bytes_per_sector";

	if( libewf_handle_get_bytes_per_sector(
	     handle,
	     bytes_per_sector,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of bytes per sector.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the amount of bytes per sector
 * Returns 1 if successful or -1 on error
 */
int libewf_set_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t bytes_per_sector )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_bytes_per_sector";

	if( libewf_handle_set_bytes_per_sector(
	     handle,
	     bytes_per_sector,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the amount of bytes per sector.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of sectors
 * Returns 1 if successful or -1 on error
 */
int libewf_get_amount_of_sectors(
     libewf_handle_t *handle,
     uint32_t *amount_of_sectors )
{
	libcerror_error_t *error         = NULL;
	static char *function           = "libewf_get_amount_of_sectors";
	uint64_t safe_amount_of_sectors = 0;

	if( amount_of_sectors == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of sectors.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	if( libewf_handle_get_amount_of_sectors(
	     handle,
	     &safe_amount_of_sectors,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of sectors.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	if( safe_amount_of_sectors > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid amount of sectors value exceeds maximum.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	*amount_of_sectors = (uint32_t) safe_amount_of_sectors;

	return( 1 );
}

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 */
int libewf_get_chunk_size(
     libewf_handle_t *handle,
     size32_t *chunk_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_chunk_size";

	if( libewf_handle_get_chunk_size(
	     handle,
	     chunk_size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the chunk size.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the error granularity
 * Returns 1 if successful or -1 on error
 */
int libewf_get_error_granularity(
     libewf_handle_t *handle,
     uint32_t *error_granularity )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_error_granularity";

	if( libewf_handle_get_error_granularity(
	     handle,
	     error_granularity,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the error granularity.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the error granularity
 * Returns 1 if successful or -1 on error
 */
int libewf_set_error_granularity(
     libewf_handle_t *handle,
     uint32_t error_granularity )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_error_granularity";

	if( libewf_handle_set_error_granularity(
	     handle,
	     error_granularity,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the error granularity.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the compression values
 * Returns 1 if successful or -1 on error
 */
int libewf_get_compression_values(
     libewf_handle_t *handle,
     int8_t *compression_level,
     uint8_t *compress_empty_block )
{
	libcerror_error_t *error   = NULL;
	static char *function     = "libewf_get_compression_values";
	uint8_t compression_flags = 0;

	if( libewf_handle_get_compression_values(
	     handle,
	     compression_level,
	     &compression_flags,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the compression values.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	if( ( compression_flags & LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
	{
		*compress_empty_block = 1;
	}
	else
	{
		*compress_empty_block = 0;
	}
	return( 1 );
}

/* Sets the compression values
 * Returns 1 if successful or -1 on error
 */
int libewf_set_compression_values(
     libewf_handle_t *handle,
     int8_t compression_level,
     uint8_t compress_empty_block )
{
	libcerror_error_t *error   = NULL;
	static char *function     = "libewf_set_compression_values";
	uint8_t compression_flags = 0;

	if( compress_empty_block != 0 )
	{
		compression_flags = LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
	}
	if( libewf_handle_set_compression_values(
	     handle,
	     compression_level,
	     compression_flags,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the compression values.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the contained media data
 * Returns 1 if successful or -1 on error
 */
int libewf_get_media_size(
     libewf_handle_t *handle,
     size64_t *media_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_media_size";

	if( libewf_handle_get_media_size(
	     handle,
	     media_size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the media size.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the media size
 * Returns 1 if successful or -1 on error
 */
int libewf_set_media_size(
     libewf_handle_t *handle,
     size64_t media_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_media_size";

	if( libewf_handle_set_media_size(
	     handle,
	     media_size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the media size.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media type value
 * Returns 1 if successful or -1 on error
 */
int libewf_get_media_type(
     libewf_handle_t *handle,
     uint8_t *media_type )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_media_type";

	if( libewf_handle_get_media_type(
	     handle,
	     media_type,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the media type.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the media type
 * Returns 1 if successful or -1 on error
 */
int libewf_set_media_type(
     libewf_handle_t *handle,
     uint8_t media_type )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_media_type";

	if( libewf_handle_set_media_type(
	     handle,
	     media_type,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the media type.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media flags
 * Returns 1 if successful or -1 on error
 */
int libewf_get_media_flags(
     libewf_handle_t *handle,
     uint8_t *media_flags )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_media_flags";

	if( libewf_handle_get_media_flags(
	     handle,
	     media_flags,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the media flags.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the media flags
 * Returns 1 if successful or -1 on error
 */
int libewf_set_media_flags(
     libewf_handle_t *handle,
     uint8_t media_flags )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_media_flags";

	if( libewf_handle_set_media_flags(
	     handle,
	     media_flags,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the media flags.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the volume type value
 * Returns 1 if successful or -1 on error
 */
int libewf_get_volume_type(
     libewf_handle_t *handle,
     uint8_t *volume_type )
{
	libcerror_error_t *error                   = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_volume_type";

	if( handle == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	if( volume_type == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume type.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	if( ( internal_handle->media_values->media_flags & 0x02 ) == 0 )
	{
		*volume_type = (int8_t) LIBEWF_VOLUME_TYPE_LOGICAL;
	}
	else
	{
		*volume_type = (int8_t) LIBEWF_VOLUME_TYPE_PHYSICAL;
	}
	return( 1 );
}

/* Sets the volume type
 * Returns 1 if successful or -1 on error
 */
int libewf_set_volume_type(
     libewf_handle_t *handle,
     uint8_t volume_type )
{
	libcerror_error_t *error                   = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_volume_type";

	if( handle == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: volume type cannot be changed.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	if( volume_type == LIBEWF_VOLUME_TYPE_LOGICAL )
	{
		/* Uses 1-complement of LIBEWF_MEDIA_FLAG_PHYSICAL
		 */
		internal_handle->media_values->media_flags &= ~LIBEWF_MEDIA_FLAG_PHYSICAL;
	}
	else if( volume_type == LIBEWF_VOLUME_TYPE_PHYSICAL )
	{
		internal_handle->media_values->media_flags |= LIBEWF_MEDIA_FLAG_PHYSICAL;
	}
	else
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported volume type.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the format type value
 * Returns 1 if successful or -1 on error
 */
int libewf_get_format(
     libewf_handle_t *handle,
     uint8_t *format )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_format";

	if( libewf_handle_get_format(
	     handle,
	     format,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the format.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the output format
 * Returns 1 if successful or -1 on error
 */
int libewf_set_format(
     libewf_handle_t *handle,
     uint8_t format )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_format";

	if( libewf_handle_set_format(
	     handle,
	     format,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the format.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the GUID
 * Returns 1 if successful or -1 on error
 */
int libewf_get_guid(
     libewf_handle_t *handle,
     uint8_t *guid,
     size_t size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_guid";

	if( libewf_handle_get_guid(
	     handle,
	     guid,
	     size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the GUID.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the GUID
 * Returns 1 if successful or -1 on error
 */
int libewf_set_guid(
     libewf_handle_t *handle,
     uint8_t *guid,
     size_t size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_guid";

	if( libewf_handle_set_guid(
	     handle,
	     guid,
	     size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the GUID.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the GUID
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_guid(
     libewf_handle_t *handle,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_guid";

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
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( guid == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: GUID too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     guid,
	     internal_handle->media_values->set_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set GUID.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the GUID
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_guid(
     libewf_handle_t *handle,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_guid";

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
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( guid == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: GUID too small.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: GUID cannot be changed.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     internal_handle->media_values->set_identifier,
	     guid,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set GUID.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the MD5 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_md5_hash(
     libewf_handle_t *handle,
     uint8_t *md5_hash,
     size_t size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_md5_hash";

	if( libewf_handle_get_md5_hash(
	     handle,
	     md5_hash,
	     size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the MD5 hash.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the MD5 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_set_md5_hash(
     libewf_handle_t *handle,
     uint8_t *md5_hash,
     size_t size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_md5_hash";

	if( libewf_handle_set_md5_hash(
	     handle,
	     md5_hash,
	     size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the MD5 hash.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the SHA1 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_sha1_hash(
     libewf_handle_t *handle,
     uint8_t *sha1_hash,
     size_t size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_sha1_hash";

	if( libewf_handle_get_sha1_hash(
	     handle,
	     sha1_hash,
	     size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the SHA1 hash.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the SHA1 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_set_sha1_hash(
     libewf_handle_t *handle,
     uint8_t *sha1_hash,
     size_t size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_sha1_hash";

	if( libewf_handle_set_sha1_hash(
	     handle,
	     sha1_hash,
	     size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set the SHA1 hash.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of chunks written
 * Returns 1 if successful or -1 on error
 */
int libewf_get_write_amount_of_chunks(
     libewf_handle_t *handle,
     uint32_t *amount_of_chunks )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_write_amount_of_chunks";

	if( libewf_handle_get_write_amount_of_chunks(
	     handle,
	     amount_of_chunks,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of chunks written.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the read wipe chunk on error
 * The chunk is not wiped if read raw is used
 * Returns 1 if successful or -1 on error
 */
int libewf_set_read_wipe_chunk_on_error(
     libewf_handle_t *handle,
     uint8_t wipe_on_error )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_read_wipe_chunk_on_error";

	if( libewf_handle_set_read_wipe_chunk_on_error(
	     handle,
	     wipe_on_error,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set wipe chunk on error during read.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Copies the media values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
int libewf_copy_media_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_copy_media_values";

	if( libewf_handle_copy_media_values(
	     destination_handle,
	     source_handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy media values.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of acquiry errors
 * Returns 1 if successful, 0 if no acquiry errors are present or -1 on error
 */
int libewf_get_amount_of_acquiry_errors(
     libewf_handle_t *handle,
     uint32_t *amount_of_errors )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_amount_of_acquiry_errors";

	if( libewf_handle_get_amount_of_acquiry_errors(
	     handle,
	     amount_of_errors,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of acquiry errors.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an acquiry error
 * Returns 1 if successful or -1 on error
 */
int libewf_get_acquiry_error(
     libewf_handle_t *handle,
     uint32_t index,
     off64_t *start_sector,
     uint32_t *amount_of_sectors )
{
	libcerror_error_t *error         = NULL;
	static char *function           = "libewf_get_acquiry_error";
	uint64_t safe_amount_of_sectors = 0;
	int result                      = 0;

	if( amount_of_sectors == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of sectors.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	result = libewf_handle_get_acquiry_error(
	          handle,
	          index,
	          (uint64_t *) start_sector,
	          &safe_amount_of_sectors,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve acquiry error.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( safe_amount_of_sectors > (uint64_t) UINT32_MAX )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid amount of sectors value exceeds maximum.",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );

			return( -1 );
		}
		*amount_of_sectors = (uint32_t) safe_amount_of_sectors;
	}
	return( result );
}

/* Add an acquiry error
 * Returns 1 if successful or -1 on error
 */
int libewf_add_acquiry_error(
     libewf_handle_t *handle,
     off64_t start_sector,
     uint32_t amount_of_sectors )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_add_acquiry_error";

	if( libewf_handle_add_acquiry_error(
	     handle,
	     (uint64_t) start_sector,
	     (uint64_t) amount_of_sectors,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to add acquiry error.",
		 function );

		libcnotify_print_error_backtrace(
		 error);
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of checksum errors
 * Returns 1 if successful or -1 on error
 */
int libewf_get_amount_of_crc_errors(
     libewf_handle_t *handle,
     uint32_t *amount_of_errors )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_amount_of_crc_errors";

	if( libewf_handle_get_number_of_checksum_errors(
	     handle,
	     amount_of_errors,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of checksum errors.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a checksum error
 * Returns 1 if successful or -1 on error
 */
int libewf_get_crc_error(
     libewf_handle_t *handle,
     uint32_t index,
     off64_t *start_sector,
     uint32_t *amount_of_sectors )
{
	libcerror_error_t *error         = NULL;
	static char *function           = "libewf_get_crc_error";
	uint64_t safe_amount_of_sectors = 0;
	int result                      = 0;

	if( amount_of_sectors == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of sectors.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	result = libewf_handle_get_checksum_error(
	          handle,
	          index,
	          (uint64_t *) start_sector,
	          &safe_amount_of_sectors,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve checksum error.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( safe_amount_of_sectors > (uint64_t) UINT32_MAX )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid amount of sectors value exceeds maximum.",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );

			return( -1 );
		}
		*amount_of_sectors = (uint32_t) safe_amount_of_sectors;
	}
	return( result );
}

/* Add a checksum error
 * Returns 1 if successful or -1 on error
 */
int libewf_add_crc_error(
     libewf_handle_t *handle,
     off64_t start_sector,
     uint32_t amount_of_sectors )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_add_crc_error";

	if( libewf_handle_append_checksum_error(
	     handle,
	     (uint64_t) start_sector,
	     (uint64_t) amount_of_sectors,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to add checksum error.",
		 function );

		libcnotify_print_error_backtrace(
		 error);
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of sessions
 * Returns 1 if successful or -1 on error
 */
int libewf_get_amount_of_sessions(
     libewf_handle_t *handle,
     uint32_t *amount_of_sessions )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_amount_of_sessions";

	if( libewf_handle_get_amount_of_sessions(
	     handle,
	     amount_of_sessions,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of sessions.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a session
 * Returns 1 if successful or -1 on error
 */
int libewf_get_session(
     libewf_handle_t *handle,
     uint32_t index,
     off64_t *start_sector,
     uint32_t *amount_of_sectors )
{
	libcerror_error_t *error         = NULL;
	static char *function           = "libewf_get_session";
	uint64_t safe_amount_of_sectors = 0;
	int result                      = 0;

	if( amount_of_sectors == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of sectors.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	result = libewf_handle_get_session(
	          handle,
	          index,
	          (uint64_t *) start_sector,
	          &safe_amount_of_sectors,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve session.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( safe_amount_of_sectors > (uint64_t) UINT32_MAX )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid amount of sectors value exceeds maximum.",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );

			return( -1 );
		}
		*amount_of_sectors = (uint32_t) safe_amount_of_sectors;
	}
	return( result );
}

/* Add a session
 * Returns 1 if successful or -1 on error
 */
int libewf_add_session(
     libewf_handle_t *handle,
     off64_t start_sector,
     uint32_t amount_of_sectors )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_add_session";

	if( libewf_handle_add_session(
	     handle,
	     (uint64_t) start_sector,
	     (uint64_t) amount_of_sectors,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to add session.",
		 function );

		libcnotify_print_error_backtrace(
		 error);
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the header codepage
 * Returns 1 if successful or -1 on error
 */
int libewf_get_header_codepage(
     libewf_handle_t *handle,
     int *header_codepage )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_header_codepage";

	if( libewf_handle_get_header_codepage(
	     handle,
	     header_codepage,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the header codepage.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int libewf_set_header_codepage(
     libewf_handle_t *handle,
     int header_codepage )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_set_header_codepage";

	if( libewf_handle_set_header_codepage(
	     handle,
	     header_codepage,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set header codepage.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of header values
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_get_amount_of_header_values(
     libewf_handle_t *handle,
     uint32_t *amount_of_values )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_amount_of_header_values";
	int result              = 0;

	result = libewf_handle_get_amount_of_header_values(
	          handle,
	          amount_of_values,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of header values.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( result );
}

/* Retrieves the header value identifier size specified by its index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_header_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_header_value_identifier_size";
	int result              = 0;

	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          index,
	          identifier_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value identifier size.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Retrieves the header value identifier specified by its index
 * The strings are encoded in UTF-8
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_header_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     char *identifier,
     size_t identifier_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_header_value_identifier";
	int result              = 0;

	result = libewf_handle_get_header_value_identifier(
	          handle,
	          index,
	          (uint8_t *) identifier,
	          identifier_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value identifier.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Retrieves the header value size specified by the identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_header_value_size(
     libewf_handle_t *handle,
     const char *identifier,
     size_t *value_size )
{
	libcerror_error_t *error  = NULL;
	static char *function    = "libewf_get_header_value_size";
	size_t identifier_length = 0;
	int result               = 0;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	identifier_length = libcstring_narrow_string_length(
	                     identifier );

	result = libewf_handle_get_header_value_size(
	          handle,
	          (uint8_t *) identifier,
	          identifier_length,
	          value_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value size: %s.",
		 function,
		 identifier );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Retrieves the header value specified by the identifier
 * The strings are encoded in UTF-8
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_header_value(
     libewf_handle_t *handle,
     const char *identifier,
     char *value,
     size_t value_size )
{
	libcerror_error_t *error  = NULL;
	static char *function    = "libewf_get_header_value";
	size_t identifier_length = 0;
	int result               = 0;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	identifier_length = libcstring_narrow_string_length(
	                     identifier );

	result = libewf_handle_get_header_value(
		  handle,
		  (uint8_t *) identifier,
		  identifier_length,
		  (uint8_t *) value,
		  value_size,
		  &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 identifier );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( result );
}

/* Sets the header value specified by the identifier
 * The strings are encoded in UTF-8
 * Returns 1 if successful or -1 on error
 */
int libewf_set_header_value(
     libewf_handle_t *handle,
     const char *identifier,
     const char *value,
     size_t value_length )
{
	libcerror_error_t *error  = NULL;
	static char *function    = "libewf_set_header_value";
	size_t identifier_length = 0;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	identifier_length = libcstring_narrow_string_length(
	                     identifier );

	if( libewf_handle_set_header_value(
	     handle,
	     (uint8_t *) identifier,
	     identifier_length,
	     (uint8_t *) value,
	     value_length,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: %s.",
		 function,
		 identifier );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Copies the header values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
int libewf_copy_header_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_copy_header_values";

	if( libewf_handle_copy_header_values(
	     destination_handle,
	     source_handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy header values.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Parses the header values from the xheader, header2 or header section
 * Will parse the first available header in order mentioned above
 * Returns 1 if successful, 0 if already parsed or -1 on error
 */
int libewf_parse_header_values(
     libewf_handle_t *handle,
     uint8_t date_format )
{
	libcerror_error_t *error = NULL;
	static char *function    = "libewf_parse_header_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date format.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Retrieves the amount of hash values
 * Returns 1 if successful or -1 on error
 */
int libewf_get_amount_of_hash_values(
     libewf_handle_t *handle,
     uint32_t *amount_of_values )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_amount_of_hash_values";

	if( libewf_handle_get_amount_of_hash_values(
	     handle,
	     amount_of_values,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of hash values.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the hash value identifier size specified by its index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_hash_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_hash_value_identifier_size";
	int result              = 0;

	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          index,
	          identifier_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value identifier size.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}


/* Retrieves the hash value identifier specified by its index
 * The strings are encoded in UTF-8
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_hash_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     char *identifier,
     size_t identifier_size )
{
	libcerror_error_t *error = NULL;
	static char *function   = "libewf_get_hash_value_identifier";
	int result              = 0;

	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          index,
	          (uint8_t *) identifier,
	          identifier_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value identifier.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Retrieves the hash value specified by the identifier
 * The strings are encoded in UTF-8
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_hash_value(
     libewf_handle_t *handle,
     const char *identifier,
     char *value,
     size_t value_size )
{
	libcerror_error_t *error  = NULL;
	static char *function    = "libewf_get_hash_value";
	size_t identifier_length = 0;
	int result               = 0;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	identifier_length = libcstring_narrow_string_length(
	                     identifier );

	result = libewf_handle_get_hash_value(
	          handle,
	          (uint8_t *) identifier,
	          identifier_length,
	          (uint8_t *) value,
	          value_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: %s.",
		 function,
		 identifier );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Retrieves the hash value size specified by the identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_hash_value_size(
     libewf_handle_t *handle,
     const char *identifier,
     size_t *value_size )
{
	libcerror_error_t *error  = NULL;
	static char *function    = "libewf_get_hash_value_size";
	size_t identifier_length = 0;
	int result               = 0;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	identifier_length = libcstring_narrow_string_length(
	                     identifier );

	result = libewf_handle_get_hash_value_size(
	          handle,
	          (uint8_t *) identifier,
	          identifier_length,
	          value_size,
	          &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value size: %s.",
		 function,
		 identifier );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Sets the hash value specified by the identifier
 * The strings are encoded in UTF-8
 * Returns 1 if successful or -1 on error
 */
int libewf_set_hash_value(
     libewf_handle_t *handle,
     const char *identifier,
     const char *value,
     size_t value_length )
{
	libcerror_error_t *error  = NULL;
	static char *function    = "libewf_set_hash_value";
	size_t identifier_length = 0;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	identifier_length = libcstring_narrow_string_length(
	                     identifier );

	if( libewf_handle_set_hash_value(
	     handle,
	     (uint8_t *) identifier,
	     identifier_length,
	     (uint8_t *) value,
	     value_length,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set hash value: %s.",
		 function,
		 identifier );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Parses the hash values from the xhash section
 * Returns 1 if successful, 0 if already parsed or -1 on error
 */
int libewf_parse_hash_values(
     libewf_handle_t *handle )
{
	libewf_internal_handle_t *internal_handle = NULL;
	libcerror_error_t *error                   = NULL;
	static char *function                     = "libewf_parse_hash_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_values_parsed != 0 )
	{
		return( 0 );
	}
	if( libewf_handle_parse_hash_values(
	     internal_handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to parse hash values.",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	internal_handle->hash_values_parsed = 1;

	return( 1 );
}

#endif

