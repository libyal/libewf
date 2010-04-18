/* 
 * Export handle
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (C) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#if defined( HAVE_LOCAL_LIBSMRAW )
#include <libsmraw_types.h>
#elif defined( HAVE_LIBSMRAW_H )
#include <libsmraw.h>
#endif

#include "digest_context.h"
#include "digest_hash.h"
#include "log_handle.h"
#include "md5.h"
#include "sha1.h"
#include "storage_media_buffer.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum EXPORT_HANDLE_OUTPUT_FORMATS
{
	EXPORT_HANDLE_OUTPUT_FORMAT_EWF		= (int) 'e',
	EXPORT_HANDLE_OUTPUT_FORMAT_FILES	= (int) 'f',
	EXPORT_HANDLE_OUTPUT_FORMAT_RAW		= (int) 'r'
};

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* Value to indicate if the MD5 digest hash should be calculated
	 */
	uint8_t calculate_md5;

	/* The MD5 digest context
	 */
	md5_context_t md5_context;

	/* The calculated MD5 digest hash string
	 */
	libcstring_system_character_t *calculated_md5_hash_string;

	/* Value to indicate if the SHA1 digest hash should be calculated
	 */
	uint8_t calculate_sha1;

	/* The SHA1 digest context
	 */
	sha1_context_t sha1_context;

	/* The calculated SHA1 digest hash string
	 */
	libcstring_system_character_t *calculated_sha1_hash_string;

	/* The libewf input handle
	 */
	libewf_handle_t *input_handle;

	/* The output format
	 */
	uint8_t output_format;

	/* The libsmraw output handle
	 */
	libsmraw_handle_t *raw_output_handle;

	/* Value to indicate if stdout should be used
	 */
	uint8_t use_stdout;

	/* The libewf output handle
	 */
	libewf_handle_t *ewf_output_handle;

	/* The input chunk size
	 */
	size32_t input_chunk_size;

	/* The amount of bytes per sector
	 */
	uint32_t bytes_per_sector;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	/* The last offset of the input data
	 */
	off64_t input_offset;
#endif

	/* Value to indicate if the write is compressed
	 */
	int write_compressed;

	/* Value to indicate if the chunk should be wiped on error
	 */
	int wipe_chunk_on_error;
};

int export_handle_initialize(
     export_handle_t **export_handle,
     liberror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     liberror_error_t **error );

int export_handle_signal_abort(
     export_handle_t *export_handle,
     liberror_error_t **error );

int export_handle_make_directory(
     export_handle_t *export_handle,
     libcstring_system_character_t *directory_name,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_sanitize_filename(
     export_handle_t *export_handle,
     libcstring_system_character_t *filename,
     size_t filename_size,
     liberror_error_t **error );

int export_handle_create_target_path(
     export_handle_t *export_handle,
     libcstring_system_character_t *export_path,
     size_t export_path_size,
     uint8_t *utf8_filename,
     size_t utf8_filename_size,
     libcstring_system_character_t **target_path,
     size_t *target_path_size,
     liberror_error_t **error );

int export_handle_open_input(
     export_handle_t *export_handle,
     libcstring_system_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error );

int export_handle_open_output(
     export_handle_t *export_handle,
     uint8_t output_format,
     const libcstring_system_character_t *filename,
     liberror_error_t **error );

int export_handle_close(
     export_handle_t *export_handle,
     liberror_error_t **error );

ssize_t export_handle_prepare_read_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error );

ssize_t export_handle_read_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error );

ssize_t export_handle_prepare_write_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         liberror_error_t **error );

ssize_t export_handle_write_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t write_size,
         liberror_error_t **error );

off64_t export_handle_seek_offset(
         export_handle_t *export_handle,
         off64_t offset,
         liberror_error_t **error );

int export_handle_swap_byte_pairs(
     export_handle_t *export_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error );

int export_handle_update_integrity_hash(
     export_handle_t *export_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     liberror_error_t **error );

int export_handle_get_input_media_size(
     export_handle_t *export_handle,
     size64_t *media_size,
     liberror_error_t **error );

int export_handle_get_input_chunk_size(
     export_handle_t *export_handle,
     size32_t *chunk_size,
     liberror_error_t **error );

int export_handle_get_output_chunk_size(
     export_handle_t *export_handle,
     size32_t *chunk_size,
     liberror_error_t **error );

int export_handle_set_header_codepage(
     export_handle_t *export_handle,
     int header_codepage,
     liberror_error_t **error );

int export_handle_set_processing_values(
     export_handle_t *export_handle,
     uint8_t calculate_md5,
     uint8_t calculate_sha1,
     liberror_error_t **error );

int export_handle_set_output_values(
     export_handle_t *export_handle,
     libcstring_system_character_t *acquiry_operating_system,
     libcstring_system_character_t *acquiry_software,
     libcstring_system_character_t *acquiry_software_version,
     int header_codepage,
     size64_t media_size,
     int8_t compression_level,
     uint8_t compression_flags,
     uint8_t libewf_format,
     size64_t segment_file_size,
     uint32_t sectors_per_chunk,
     uint8_t wipe_chunk_on_error,
     liberror_error_t **error );

int export_handle_set_header_value(
     export_handle_t *export_handle,
     char *header_value_identifier,
     size_t header_value_identifier_length,
     libcstring_system_character_t *header_value,
     size_t header_value_length,
     liberror_error_t **error );

int export_handle_set_hash_value(
     export_handle_t *export_handle,
     char *hash_value_identifier,
     size_t hash_value_identifier_length,
     libcstring_system_character_t *hash_value,
     size_t hash_value_length,
     liberror_error_t **error );

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
int export_handle_add_read_error(
      export_handle_t *export_handle,
      off64_t start_offset,
      size_t amount_of_bytes,
      liberror_error_t **error );
#endif

ssize_t export_handle_finalize(
         export_handle_t *export_handle,
         liberror_error_t **error );

int export_handle_export_single_files(
     export_handle_t *export_handle,
     libcstring_system_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_export_file_entry(
     export_handle_t *export_handle,
     libewf_file_entry_t *file_entry,
     libcstring_system_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_hash_values_fprint(
     export_handle_t *export_handle,
     FILE *stream,
     liberror_error_t **error );

int export_handle_crc_errors_fprint(
     export_handle_t *export_handle,
     FILE *stream,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

