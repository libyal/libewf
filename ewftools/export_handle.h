/* 
 * Export handle
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

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <types.h>

#include "digest_hash.h"
#include "ewftools_libcerror.h"
#include "ewftools_libcstring.h"
#include "ewftools_libewf.h"
#include "ewftools_libhmac.h"
#include "ewftools_libsmraw.h"
#include "log_handle.h"
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
	/* The user input buffer
	 */
	libcstring_system_character_t *input_buffer; 

	/* The target path
	 */
	libcstring_system_character_t *target_path;

	/* The target path size
	 */
	size_t target_path_size;

	/* The compression method
	 */
	uint16_t compression_method;

	/* The compression level
	 */
	int8_t compression_level;

	/* The compression flags
	 */
	uint8_t compression_flags;

	/* The output format
	 */
	uint8_t output_format;

	/* The EWF format
	 */
	uint8_t ewf_format;

	/* The number of sectors per chunk
	 */
	uint32_t sectors_per_chunk;

	/* The maximum segment size
	 */
	size64_t maximum_segment_size;

	/* The export offset
	 */
	uint64_t export_offset;

	/* The export size
	 */
	uint64_t export_size;

	/* The header codepage
	 */
	int header_codepage;

	/* Value to indicate if the MD5 digest hash should be calculated
	 */
	uint8_t calculate_md5;

	/* The MD5 digest context
	 */
	libhmac_md5_context_t *md5_context;

	/* Value to indicate the MD5 digest context was initialized
	 */
	uint8_t md5_context_initialized;

	/* The calculated MD5 digest hash string
	 */
	libcstring_system_character_t *calculated_md5_hash_string;

	/* Value to indicate if the SHA1 digest hash should be calculated
	 */
	uint8_t calculate_sha1;

	/* The SHA1 digest context
	 */
	libhmac_sha1_context_t *sha1_context;

	/* Value to indicate the SHA1 digest context was initialized
	 */
	uint8_t sha1_context_initialized;

	/* The calculated SHA1 digest hash string
	 */
	libcstring_system_character_t *calculated_sha1_hash_string;

	/* Value to indicate if the SHA256 digest hash should be calculated
	 */
	uint8_t calculate_sha256;

	/* The SHA256 digest context
	 */
	libhmac_sha256_context_t *sha256_context;

	/* Value to indicate the SHA256 digest context was initialized
	 */
	uint8_t sha256_context_initialized;

	/* The calculated SHA256 digest hash string
	 */
	libcstring_system_character_t *calculated_sha256_hash_string;

	/* The libewf input handle
	 */
	libewf_handle_t *input_handle;

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

	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* The input media size
	 */
	size64_t input_media_size;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	/* The last offset of the input data
	 */
	off64_t input_offset;
#endif

	/* Value to indicate if the write is compressed
	 */
	int write_compressed;

	/* Value to indicate if the chunk should be zeroed on error
	 */
	int zero_chunk_on_error;

	/* The process buffer size
	 */
	size_t process_buffer_size;

	/* The nofication output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int export_handle_initialize(
     export_handle_t **export_handle,
     uint8_t calculate_md5,
     libcerror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     libcerror_error_t **error );

int export_handle_signal_abort(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_set_maximum_number_of_open_handles(
     export_handle_t *export_handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error );

int export_handle_open_input(
     export_handle_t *export_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error );

int export_handle_open_output(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int export_handle_close(
     export_handle_t *export_handle,
     libcerror_error_t **error );

ssize_t export_handle_prepare_read_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         libcerror_error_t **error );

ssize_t export_handle_read_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         libcerror_error_t **error );

ssize_t export_handle_prepare_write_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         libcerror_error_t **error );

ssize_t export_handle_write_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t write_size,
         libcerror_error_t **error );

off64_t export_handle_seek_offset(
         export_handle_t *export_handle,
         off64_t offset,
         libcerror_error_t **error );

int export_handle_swap_byte_pairs(
     export_handle_t *export_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     libcerror_error_t **error );

int export_handle_initialize_integrity_hash(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_update_integrity_hash(
     export_handle_t *export_handle,
     uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error );

int export_handle_finalize_integrity_hash(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_input_is_corrupted(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_get_output_chunk_size(
     export_handle_t *export_handle,
     size32_t *chunk_size,
     libcerror_error_t **error );

int export_handle_prompt_for_string(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcstring_system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error );

int export_handle_prompt_for_compression_method(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error );

int export_handle_prompt_for_compression_level(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error );

int export_handle_prompt_for_output_format(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error );

int export_handle_prompt_for_sectors_per_chunk(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error );

int export_handle_prompt_for_maximum_segment_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error );

int export_handle_prompt_for_export_offset(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error );

int export_handle_prompt_for_export_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error );

int export_handle_set_string(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcstring_system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error );

int export_handle_set_compression_values(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_output_format(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_sectors_per_chunk(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_maximum_segment_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_export_offset(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_export_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_header_codepage(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_process_buffer_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_additional_digest_types(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_output_values(
     export_handle_t *export_handle,
     libcstring_system_character_t *acquiry_operating_system,
     libcstring_system_character_t *acquiry_software,
     libcstring_system_character_t *acquiry_software_version,
     uint8_t zero_chunk_on_error,
     uint8_t copy_input_values,
     libcerror_error_t **error );

int export_handle_set_hash_value(
     export_handle_t *export_handle,
     char *hash_value_identifier,
     size_t hash_value_identifier_length,
     libcstring_system_character_t *hash_value,
     size_t hash_value_length,
     libcerror_error_t **error );

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
int export_handle_append_read_error(
      export_handle_t *export_handle,
      off64_t start_offset,
      size_t number_of_bytes,
      libcerror_error_t **error );
#endif

ssize_t export_handle_finalize(
         export_handle_t *export_handle,
         libcerror_error_t **error );

int export_handle_export_input(
     export_handle_t *export_handle,
     uint8_t swap_byte_pairs,
     uint8_t print_status_information,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_single_files(
     export_handle_t *export_handle,
     const libcstring_system_character_t *export_path,
     uint8_t print_status_information,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_file_entry(
     export_handle_t *export_handle,
     libewf_file_entry_t *file_entry,
     const libcstring_system_character_t *export_path,
     size_t export_path_size,
     size_t file_entry_path_index,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_file_entry_data(
     export_handle_t *export_handle,
     libewf_file_entry_t *file_entry,
     const libcstring_system_character_t *export_path,
     libcerror_error_t **error );

int export_handle_export_file_entry_sub_file_entries(
     export_handle_t *export_handle,
     libewf_file_entry_t *file_entry,
     const libcstring_system_character_t *export_path,
     size_t export_path_size,
     size_t file_entry_path_index,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_hash_values_fprint(
     export_handle_t *export_handle,
     FILE *stream,
     libcerror_error_t **error );

int export_handle_checksum_errors_fprint(
     export_handle_t *export_handle,
     FILE *stream,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

