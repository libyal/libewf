/*
 * Library to access the Expert Witness Compression Format (EWF)
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

#if !defined( _LIBEWF_H )
#define _LIBEWF_H

#include <libewf/codepage.h>
#include <libewf/definitions.h>
#include <libewf/error.h>
#include <libewf/extern.h>
#include <libewf/features.h>
#include <libewf/types.h>

#include <stdio.h>

#if defined( LIBEWF_HAVE_BFIO )
#include <libbfio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBEWF_EXTERN \
const char *libewf_get_version(
             void );

/* Returns the access flags for reading
 */
LIBEWF_EXTERN \
int libewf_get_access_flags_read(
     void );

/* Returns the access flags for reading and writing
 */
LIBEWF_EXTERN \
int libewf_get_access_flags_read_write(
     void );

/* Returns the access flags for writing
 */
LIBEWF_EXTERN \
int libewf_get_access_flags_write(
     void );

/* Returns the access flags for resume writing
 */
LIBEWF_EXTERN \
int libewf_get_access_flags_write_resume(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_get_codepage(
     int *codepage,
     libewf_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_set_codepage(
     int codepage,
     libewf_error_t **error );

/* Determines if a file contains an EWF file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_check_file_signature(
     const char *filename,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains an EWF file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_check_file_signature_wide(
     const wchar_t *filename,
     libewf_error_t **error );

#endif /* defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEWF_HAVE_BFIO )

/* Determines if a file contains an EWF file signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libewf_error_t **error );

#endif /* defined( LIBEWF_HAVE_BFIO ) */

/* Globs the segment files according to the EWF naming schema
 * Make sure the value filenames is referencing, is set to NULL
 *
 * If the format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_glob(
     const char *filename,
     size_t filename_length,
     uint8_t format,
     char **filenames[],
     int *number_of_filenames,
     libewf_error_t **error );

/* Frees globbed filenames
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_glob_free(
     char *filenames[],
     int number_of_filenames,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )

/* Globs the segment files according to the EWF naming schema
 * Make sure the value filenames is referencing, is set to NULL
 *
 * If the format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_glob_wide(
     const wchar_t *filename,
     size_t filename_length,
     uint8_t format,
     wchar_t **filenames[],
     int *number_of_filenames,
     libewf_error_t **error );

/* Frees globbed wide filenames
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_glob_wide_free(
     wchar_t *filenames[],
     int number_of_filenames,
     libewf_error_t **error );

#endif /* defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBEWF_EXTERN \
void libewf_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_notify_set_stream(
     FILE *stream,
     libewf_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_notify_stream_open(
     const char *filename,
     libewf_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_notify_stream_close(
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBEWF_EXTERN \
void libewf_error_free(
      libewf_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_error_fprint(
     libewf_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_error_sprint(
     libewf_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_error_backtrace_fprint(
     libewf_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_error_backtrace_sprint(
     libewf_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Handle functions
 * ------------------------------------------------------------------------- */

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_initialize(
     libewf_handle_t **handle,
     libewf_error_t **error );

/* Frees a handle
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_free(
     libewf_handle_t **handle,
     libewf_error_t **error );

/* Clones the handle including elements
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_clone(
     libewf_handle_t **destination_handle,
     libewf_handle_t *source_handle,
     libewf_error_t **error );

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_signal_abort(
     libewf_handle_t *handle,
     libewf_error_t **error );

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_open(
     libewf_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_open_wide(
     libewf_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     int access_flags,
     libewf_error_t **error );

#endif /* defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEWF_HAVE_BFIO )

/* Opens a set of EWF file(s) using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libewf_error_t **error );

#endif /* defined( LIBEWF_HAVE_BFIO ) */

/* Closes the EWF handle
 * Returns 0 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_close(
     libewf_handle_t *handle,
     libewf_error_t **error );

/* Reads (media) data at the current offset into a buffer
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libewf_error_t **error );

/* Reads (media) data at a specific offset
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_read_buffer_at_offset(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libewf_error_t **error );

/* Writes (media) data at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_write_buffer(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libewf_error_t **error );

/* Writes (media) data at a specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_write_buffer_at_offset(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libewf_error_t **error );

/* Retrieves a (media) data chunk
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_data_chunk(
     libewf_handle_t *handle,
     libewf_data_chunk_t **data_chunk,
     libewf_error_t **error );

/* Reads a (media) data chunk at the current offset
 * Returns the number of bytes in the data chunk, 0 when no longer data can be read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_read_data_chunk(
         libewf_handle_t *handle,
         libewf_data_chunk_t *data_chunk,
         libewf_error_t **error );

/* Writes a (media) data chunk at the current offset
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_write_data_chunk(
         libewf_handle_t *handle,
         libewf_data_chunk_t *data_chunk,
         libewf_error_t **error );

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after writing from stream
 * Returns the number of bytes written or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_write_finalize(
         libewf_handle_t *handle,
         libewf_error_t **error );

/* Seeks a certain offset of the (media) data
 * Returns the offset if seek is successful or -1 on error
 */
LIBEWF_EXTERN \
off64_t libewf_handle_seek_offset(
         libewf_handle_t *handle,
         off64_t offset,
         int whence,
         libewf_error_t **error );

/* Retrieves the current offset of the (media) data
 * Returns the offset if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_offset(
     libewf_handle_t *handle,
     off64_t *offset,
     libewf_error_t **error );

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_maximum_number_of_open_handles(
     libewf_handle_t *handle,
     int maximum_number_of_open_handles,
     libewf_error_t **error );

/* Retrieves the segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libewf_error_t **error );

/* Sets the segment filename
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libewf_error_t **error );

/* Sets the segment filename
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libewf_error_t **error );

#endif /* defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE ) */

/* Retrieves the maximum segment file size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t *maximum_segment_size,
     libewf_error_t **error );

/* Sets the maximum segment file size
 * A maximum segment file size of 0 represents the maximum possible size for the format
 * If the maximum segment file size is smaller than the size needed to store a single chunk
 * the size off the latter is enforced and not the maximum segment file size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t maximum_segment_size,
     libewf_error_t **error );

/* Determine if the segment files are corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_segment_files_corrupted(
     libewf_handle_t *handle,
     libewf_error_t **error );

/* Determine if the segment files are encrypted
 * Returns 1 if encrypted, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_segment_files_encrypted(
     libewf_handle_t *handle,
     libewf_error_t **error );

/* Retrieves the filename size of the segment file of the current chunk
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the filename of the segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the segment file of the current chunk
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the filename of the segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libewf_error_t **error );

#endif /* defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEWF_HAVE_BFIO )

/* Retrieves the file IO handle of the segment file of the current chunk
 * Returns 1 if successful, 0 if no such file IO handle or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_file_io_handle(
     libewf_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libewf_error_t **error );

#endif /* defined( LIBEWF_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Meta data functions
 * ------------------------------------------------------------------------- */

/* Retrieves the number of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t *sectors_per_chunk,
     libewf_error_t **error );

/* Sets the number of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t sectors_per_chunk,
     libewf_error_t **error );

/* Retrieves the number of bytes per sector
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t *bytes_per_sector,
     libewf_error_t **error );

/* Sets the number of bytes per sector
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t bytes_per_sector,
     libewf_error_t **error );

/* Retrieves the number of sectors
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_sectors(
     libewf_handle_t *handle,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_chunk_size(
     libewf_handle_t *handle,
     size32_t *chunk_size,
     libewf_error_t **error );

/* Retrieves the error granularity
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_error_granularity(
     libewf_handle_t *handle,
     uint32_t *error_granularity,
     libewf_error_t **error );

/* Sets the error granularity
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_error_granularity(
     libewf_handle_t *handle,
     uint32_t error_granularity,
     libewf_error_t **error );

/* Retrieves the compression method
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_compression_method(
     libewf_handle_t *handle,
     uint16_t *compression_method,
     libewf_error_t **error );

/* Sets the compression method
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_compression_method(
     libewf_handle_t *handle,
     uint16_t compression_method,
     libewf_error_t **error );

/* Retrieves the compression values
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_compression_values(
     libewf_handle_t *handle,
     int8_t *compression_level,
     uint8_t *compression_flags,
     libewf_error_t **error );

/* Sets the compression values
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_compression_values(
     libewf_handle_t *handle,
     int8_t compression_level,
     uint8_t compression_flags,
     libewf_error_t **error );

/* Retrieves the size of the contained (media) data
 * This function will compensate for a media_size that is not a multitude of bytes_per_sector
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_media_size(
     libewf_handle_t *handle,
     size64_t *media_size,
     libewf_error_t **error );

/* Sets the media size
 * The media_size is stored as number_of_sectors x bytes_per_sector
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_media_size(
     libewf_handle_t *handle,
     size64_t media_size,
     libewf_error_t **error );

/* Retrieves the media type value
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_media_type(
     libewf_handle_t *handle,
     uint8_t *media_type,
     libewf_error_t **error );

/* Sets the media type
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_media_type(
     libewf_handle_t *handle,
     uint8_t media_type,
     libewf_error_t **error );

/* Retrieves the media flags
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_media_flags(
     libewf_handle_t *handle,
     uint8_t *media_flags,
     libewf_error_t **error );

/* Sets the media flags
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_media_flags(
     libewf_handle_t *handle,
     uint8_t media_flags,
     libewf_error_t **error );

/* Retrieves the format type value
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_format(
     libewf_handle_t *handle,
     uint8_t *format,
     libewf_error_t **error );

/* Sets the output format
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_format(
     libewf_handle_t *handle,
     uint8_t format,
     libewf_error_t **error );

/* Retrieves the segment file version
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_file_version(
     libewf_handle_t *handle,
     uint8_t *major_version,
     uint8_t *minor_version,
     libewf_error_t **error );

/* Retrieves the segment file set identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_file_set_identifier(
     libewf_handle_t *handle,
     uint8_t *set_identifier,
     size_t size,
     libewf_error_t **error );

/* Sets the segment file set identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_segment_file_set_identifier(
     libewf_handle_t *handle,
     const uint8_t *set_identifier,
     size_t size,
     libewf_error_t **error );

/* Retrieves the MD5 hash
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_md5_hash(
     libewf_handle_t *handle,
     uint8_t *md5_hash,
     size_t size,
     libewf_error_t **error );

/* Sets the MD5 hash
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_md5_hash(
     libewf_handle_t *handle,
     const uint8_t *md5_hash,
     size_t size,
     libewf_error_t **error );

/* Retrieves the SHA1 hash
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_sha1_hash(
     libewf_handle_t *handle,
     uint8_t *sha1_hash,
     size_t size,
     libewf_error_t **error );

/* Sets the SHA1 hash
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_sha1_hash(
     libewf_handle_t *handle,
     const uint8_t *sha1_hash,
     size_t size,
     libewf_error_t **error );

/* Retrieves the number of chunks written
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_chunks_written(
     libewf_handle_t *handle,
     uint32_t *number_of_chunks,
     libewf_error_t **error );

/* Sets the read zero chunk on error
 * The chunk is not zeroed if read raw is used
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_read_zero_chunk_on_error(
     libewf_handle_t *handle,
     uint8_t zero_on_error,
     libewf_error_t **error );

/* Copies the media values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_copy_media_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libewf_error_t **error );

/* Retrieves the number of acquiry errors
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_acquiry_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libewf_error_t **error );

/* Retrieves an acquiry error
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_acquiry_error(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Appends an acquiry error
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_append_acquiry_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the number of checksum errors
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_checksum_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libewf_error_t **error );

/* Retrieves a checksum error
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_checksum_error(
     libewf_handle_t *handle,
     uint32_t error_index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Appends a checksum error
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_append_checksum_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the number of sessions
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_sessions(
     libewf_handle_t *handle,
     uint32_t *number_of_sessions,
     libewf_error_t **error );

/* Retrieves a session
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_session(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Appends a session
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_append_session(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the number of tracks
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_tracks(
     libewf_handle_t *handle,
     uint32_t *number_of_tracks,
     libewf_error_t **error );

/* Retrieves a track
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_track(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Appends a track
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_append_track(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the header codepage
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_header_codepage(
     libewf_handle_t *handle,
     int *header_codepage,
     libewf_error_t **error );

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_header_codepage(
     libewf_handle_t *handle,
     int header_codepage,
     libewf_error_t **error );

/* Retrieves the header values date format
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_header_values_date_format(
     libewf_handle_t *handle,
     int *date_format,
     libewf_error_t **error );

/* Sets the header values date format
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_header_values_date_format(
     libewf_handle_t *handle,
     int date_format,
     libewf_error_t **error );

/* Retrieves the number of header values
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_header_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libewf_error_t **error );

/* Retrieves the size of the header value identifier of a specific index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_header_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libewf_error_t **error );

/* Retrieves the header value identifier of a specific index
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_header_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf8_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the header value case number
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_case_number( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "case_number", 11, value, value_size, error )

/* Retrieves the header value description
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_description( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "description", 11, value, value_size, error )

/* Retrieves the header value examiner name
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_examiner_name( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "examiner_name", 13, value, value_size, error )

/* Retrieves the header value evidence number
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_evidence_number( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "evidence_number", 15, value, value_size, error )

/* Retrieves the header value notes
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_notes( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "notes", 5, value, value_size, error )

/* Retrieves the header value acquiry date
 * The string is encoded according to the date format
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_acquiry_date( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "acquiry_date", 12, value, value_size, error )

/* Retrieves the header value system date
 * The string is encoded according to the date format
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_system_date( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "system_date", 11, value, value_size, error )

/* Retrieves the header value acquiry operating system
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_acquiry_operating_system( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "acquiry_operating_system", 24, value, value_size, error )

/* Retrieves the header value acquiry software version
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_acquiry_software_version( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "acquiry_software_version", 24, value, value_size, error )

/* Retrieves the header value password
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_password( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "password", 8, value, value_size, error )

/* Retrieves the header value compression level
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_compression_level( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "compression_level", 17, value, value_size, error )

/* Retrieves the header value model
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_model( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "model", 5, value, value_size, error )

/* Retrieves the header value serial number
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_header_value_serial_number( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "serial_number", 13, value, value_size, error )

/* Sets the UTF-8 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_error_t **error );

/* Sets the header value case number
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_case_number( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "case_number", 11, value, value_length, error )

/* Sets the header value description
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_description( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "description", 11, value, value_length, error )

/* Sets the header value examiner name
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_examiner_name( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "examiner_name", 13, value, value_length, error )

/* Sets the header value evidence number
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_evidence_number( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "evidence_number", 15, value, value_length, error )

/* Sets the header value notes
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_notes( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "notes", 5, value, value_length, error )

/* Sets the header value acquiry date
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_acquiry_date( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "acquiry_date", 12, value, value_length, error )

/* Sets the header value system date
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_system_date( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "system_date", 11, value, value_length, error )

/* Sets the header value acquiry operating system
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_acquiry_operating_system( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "acquiry_operating_system", 24, value, value_length, error )

/* Sets the header value acquiry software version
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_acquiry_software_version( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "acquiry_software_version", 24, value, value_length, error )

/* Sets the header value password
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_password( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "password", 8, value, value_length, error )

/* Sets the header value compression level
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_compression_level( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "compression_level", 17, value, value_length, error )

/* Sets the header value model
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_model( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "model", 5, value, value_length, error )

/* Sets the header value serial number
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_serial_number( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "serial_number", 13, value, value_length, error )

/* Retrieves the size of the UTF-16 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf16_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Sets the UTF-16 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_error_t **error );

/* Copies the header values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_copy_header_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libewf_error_t **error );

/* Retrieves the number of hash values
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_hash_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libewf_error_t **error );

/* Retrieves the size of the hash value identifier of a specific index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_hash_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libewf_error_t **error );

/* Retrieves the hash value identifier of a specific index
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_hash_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded hash value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf8_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded hash value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the hash value MD5
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_hash_value_md5( handle, value, value_size, error ) \
        libewf_handle_get_utf8_hash_value( handle, (uint8_t *) "MD5", 3, value, value_size, error )

/* Retrieves the hash value SHA1
 * Returns 1 if successful, 0 if not set or -1 on error
 */
#define libewf_handle_get_utf8_hash_value_sha1( handle, value, value_size, error ) \
        libewf_handle_get_utf8_hash_value( handle, (uint8_t *) "SHA1", 4, value, value_size, error )

/* Sets the UTF-8 hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_error_t **error );

/* Sets the hash value MD5
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_hash_value_md5( handle, value, value_length, error ) \
        libewf_handle_set_utf8_hash_value( handle, (uint8_t *) "MD5", 3, value, value_length, error )

/* Sets the hash value SHA1
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_hash_value_sha1( handle, value, value_length, error ) \
        libewf_handle_set_utf8_hash_value( handle, (uint8_t *) "SHA1", 4, value, value_length, error )

/* Retrieves the size of the UTF-16 encoded hash value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf16_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded hash value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Sets the UTF-16 hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_error_t **error );

/* Retrieves the root (single) file entry
 * Returns 1 if successful, 0 if no file entries are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_root_file_entry(
     libewf_handle_t *handle,
     libewf_file_entry_t **root_file_entry,
     libewf_error_t **error );

/* Retrieves the (single) file entry for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_file_entry_by_utf8_path(
     libewf_handle_t *handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **file_entry,
     libewf_error_t **error );

/* Retrieves the (single) file entry for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_file_entry_by_utf16_path(
     libewf_handle_t *handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **file_entry,
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Data chunk functions
 * ------------------------------------------------------------------------- */

/* Frees a data chunk
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_data_chunk_free(
     libewf_data_chunk_t **data_chunk,
     libewf_error_t **error );

/* Determines if the data chunk is corrupted
 * Returns 1 if the data chunk is corrupted, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_data_chunk_is_corrupted(
     libewf_data_chunk_t *data_chunk,
     libewf_error_t **error );

/* Reads a buffer from the data chunk
 * It applies decompression if necessary and validates the chunk checksum
 * This function should be used after libewf_handle_read_data_chunk
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_data_chunk_read_buffer(
         libewf_data_chunk_t *data_chunk,
         void *buffer,
         size_t buffer_size,
         libewf_error_t **error );

/* Writes a buffer to the data chunk
 * It applies compression if necessary and calculated the chunk checksum
 * This function should be used before libewf_handle_write_data_chunk
 * Returns the number of bytes written, 0 when no longer data can be written or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_data_chunk_write_buffer(
         libewf_data_chunk_t *data_chunk,
         const void *buffer,
         size_t buffer_size,
         libewf_error_t **error );

/* -------------------------------------------------------------------------
 * File entry functions
 * ------------------------------------------------------------------------- */

/* Frees a file entry
 * REturns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_free(
     libewf_file_entry_t **file_entry,
     libewf_error_t **error );

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_identifier(
     libewf_file_entry_t *file_entry,
     uint64_t *identifier,
     libewf_error_t **error );

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_type(
     libewf_file_entry_t *file_entry,
     uint8_t *type,
     libewf_error_t **error );

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_flags(
     libewf_file_entry_t *file_entry,
     uint32_t *flags,
     libewf_error_t **error );

/* Retrieves the media data offset
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_media_data_offset(
     libewf_file_entry_t *file_entry,
     off64_t *media_data_offset,
     libewf_error_t **error );

/* Retrieves the media data size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_media_data_size(
     libewf_file_entry_t *file_entry,
     size64_t *media_data_size,
     libewf_error_t **error );

/* Retrieves the duplicate media data offset
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_duplicate_media_data_offset(
     libewf_file_entry_t *file_entry,
     off64_t *duplicate_media_data_offset,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_name(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_name(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded short name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_short_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded short name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_short_name(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded short name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_short_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded short name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_short_name(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_size(
     libewf_file_entry_t *file_entry,
     size64_t *size,
     libewf_error_t **error );

/* Retrieves the creation date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_creation_time(
     libewf_file_entry_t *file_entry,
     int64_t *posix_time,
     libewf_error_t **error );

/* Retrieves the modification date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_modification_time(
     libewf_file_entry_t *file_entry,
     int64_t *posix_time,
     libewf_error_t **error );

/* Retrieves the access date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_access_time(
     libewf_file_entry_t *file_entry,
     int64_t *posix_time,
     libewf_error_t **error );

/* Retrieves the (file system) entry modification date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_entry_modification_time(
     libewf_file_entry_t *file_entry,
     int64_t *posix_time,
     libewf_error_t **error );

/* Retrieves the deletion date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_deletion_time(
     libewf_file_entry_t *file_entry,
     int64_t *posix_time,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded MD5 hash value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_hash_value_md5(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded MD5 hash value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_hash_value_md5(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded SHA1 hash value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_hash_value_sha1(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded SHA1 hash value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_hash_value_sha1(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_file_entry_read_buffer(
         libewf_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libewf_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_file_entry_read_buffer_at_offset(
         libewf_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libewf_error_t **error );

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
LIBEWF_EXTERN \
off64_t libewf_file_entry_seek_offset(
         libewf_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libewf_error_t **error );

/* Retrieves the current offset of the data
 * Returns the offset if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_offset(
     libewf_file_entry_t *file_entry,
     off64_t *offset,
     libewf_error_t **error );

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_number_of_sub_file_entries(
     libewf_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry(
     libewf_file_entry_t *file_entry,
     int sub_file_entry_index,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf8_name(
     libewf_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf8_path(
     libewf_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf16_name(
     libewf_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf16_path(
     libewf_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* Retrieves the number of attributes
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_number_of_attributes(
     libewf_file_entry_t *file_entry,
     int *number_of_attributes,
     libewf_error_t **error );

/* Retrieves the attribute for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_attribute(
     libewf_file_entry_t *file_entry,
     int attribute_index,
     libewf_attribute_t **attribute,
     libewf_error_t **error );

/* Retrieves the number of access control entries
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_number_of_access_control_entries(
     libewf_file_entry_t *file_entry,
     int *number_of_access_control_entries,
     libewf_error_t **error );

/* Retrieves the access control entry for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_access_control_entry(
     libewf_file_entry_t *file_entry,
     int access_control_entry_index,
     libewf_access_control_entry_t **access_control_entry,
     libewf_error_t **error );

/* Retrieves the (acquisition) source
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_source(
     libewf_file_entry_t *file_entry,
     libewf_source_t **source,
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Attribute functions
 * ------------------------------------------------------------------------- */

/* Frees an attribute
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_attribute_free(
     libewf_attribute_t **attribute,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_attribute_get_utf8_name_size(
     libewf_attribute_t *attribute,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_attribute_get_utf8_name(
     libewf_attribute_t *attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_attribute_get_utf16_name_size(
     libewf_attribute_t *attribute,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_attribute_get_utf16_name(
     libewf_attribute_t *attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded value
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_attribute_get_utf8_value_size(
     libewf_attribute_t *attribute,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_attribute_get_utf8_value(
     libewf_attribute_t *attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded value
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_attribute_get_utf16_value_size(
     libewf_attribute_t *attribute,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_attribute_get_utf16_value(
     libewf_attribute_t *attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Access control entry functions
 * ------------------------------------------------------------------------- */

/* Frees an access control entry
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_free(
     libewf_access_control_entry_t **access_control_entry,
     libewf_error_t **error );

/* Retrieves the (property) type
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_type(
     libewf_access_control_entry_t *access_control_entry,
     uint32_t *type,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf8_identifier_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf8_identifier(
     libewf_access_control_entry_t *access_control_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf16_identifier_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf16_identifier(
     libewf_access_control_entry_t *access_control_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf8_name_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf8_name(
     libewf_access_control_entry_t *access_control_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf16_name_size(
     libewf_access_control_entry_t *access_control_entry,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_utf16_name(
     libewf_access_control_entry_t *access_control_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the access mask
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_access_mask(
     libewf_access_control_entry_t *access_control_entry,
     uint32_t *access_mask,
     libewf_error_t **error );

/* Retrieves the flags
 * Contains Windows NT access control entry (ACE) flags
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_access_control_entry_get_flags(
     libewf_access_control_entry_t *access_control_entry,
     uint32_t *flags,
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Source functions
 * ------------------------------------------------------------------------- */

/* Frees a source
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_free(
     libewf_source_t **source,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_name_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_name(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_name_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_name(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded evidence number
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_evidence_number_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded evidence number
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_evidence_number(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded evidence number
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_evidence_number_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded evidence number
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_evidence_number(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded location
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_location_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded location
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_location(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded location
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_location_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded location
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_location(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded device GUID
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_device_guid_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded device GUID
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_device_guid(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded device GUID
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_device_guid_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded device GUID
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_device_guid(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded primary device GUID
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_primary_device_guid_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded primary device GUID
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_primary_device_guid(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded primary device GUID
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_primary_device_guid_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded primary device GUID
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_primary_device_guid(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded manufacturer
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_manufacturer_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded manufacturer
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_manufacturer(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded manufacturer
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_manufacturer_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded manufacturer
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_manufacturer(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded model
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_model_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded model
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_model(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded model
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_model_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded model
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_model(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded serial number
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_serial_number_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded serial number
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_serial_number(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded serial number
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_serial_number_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded serial number
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_serial_number(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded domain
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_domain_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded domain
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_domain(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded domain
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_domain_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded domain
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_domain(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded IP address
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_ip_address_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded IP address
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_ip_address(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded IP address
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_ip_address_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded IP address
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_ip_address(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded MAC address
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_mac_address_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded MAC address
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_mac_address(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded MAC address
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_mac_address_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded MAC address
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_mac_address(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_size(
     libewf_source_t *source,
     size64_t *size,
     libewf_error_t **error );

/* Retrieves the acquisition date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_acquisition_time(
     libewf_source_t *source,
     int64_t *posix_time,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded MD5 hash value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_hash_value_md5(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded MD5 hash value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_hash_value_md5(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded SHA1 hash value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf8_hash_value_sha1(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded SHA1 hash value
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
LIBEWF_EXTERN \
int libewf_source_get_utf16_hash_value_sha1(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Subject functions
 * ------------------------------------------------------------------------- */

/* Frees a subject
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_subject_free(
     libewf_subject_t **subject,
     libewf_error_t **error );

#ifdef __cplusplus
}
#endif

#endif /* !defined( _LIBEWF_H ) */

