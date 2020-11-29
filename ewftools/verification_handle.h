/*
 * Verification handle
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

#if !defined( _VERIFICATION_HANDLE_H )
#define _VERIFICATION_HANDLE_H

#include <common.h>
#include <types.h>

#include "digest_hash.h"
#include "ewftools_libcdata.h"
#include "ewftools_libcerror.h"
#include "ewftools_libcthreads.h"
#include "ewftools_libewf.h"
#include "ewftools_libhmac.h"
#include "log_handle.h"
#include "process_status.h"
#include "storage_media_buffer.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum VERIFICATION_HANDLE_INPUT_FORMATS
{
	VERIFICATION_HANDLE_INPUT_FORMAT_FILES	= (int) 'f',
	VERIFICATION_HANDLE_INPUT_FORMAT_RAW	= (int) 'r'
};

typedef struct verification_handle verification_handle_t;

struct verification_handle
{
	/* The input format
	 */
	uint8_t input_format;

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
	system_character_t *calculated_md5_hash_string;

	/* Value to indicate a stored MD5 digest hash is available
	 */
	int stored_md5_hash_available;

	/* The stored MD5 digest hash string
	 */
	system_character_t *stored_md5_hash_string;

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
	system_character_t *calculated_sha1_hash_string;

	/* Value to indicate a stored SHA1 digest hash is available
	 */
	int stored_sha1_hash_available;

	/* The stored SHA1 digest hash string
	 */
	system_character_t *stored_sha1_hash_string;

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
	system_character_t *calculated_sha256_hash_string;

	/* Value to indicate a stored SHA256 digest hash is available
	 */
	int stored_sha256_hash_available;

	/* The stored SHA256 digest hash string
	 */
	system_character_t *stored_sha256_hash_string;

	/* Value to indicate if the data chunk functions instead of the buffered read and write functions should be used
	 */
	uint8_t use_data_chunk_functions;

	/* The process buffer size
	 */
	size_t process_buffer_size;

	/* The number of threads in the process thread pool
	 */
	int number_of_threads;

#if defined( HAVE_MULTI_THREAD_SUPPORT )

	/* The process thread pool
	 */
	libcthreads_thread_pool_t *process_thread_pool;

	/* The output thread pool
	 */
	libcthreads_thread_pool_t *output_thread_pool;

	/* The output list
	 */
	libcdata_list_t *output_list;

	/* The storage media buffer queue
	 */
	libcthreads_queue_t *storage_media_buffer_queue;

#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

	/* The libewf input handle
	 */
	libewf_handle_t *input_handle;

	/* The media size
	 */
	size64_t media_size;

	/* The chunk size
	 */
	size32_t chunk_size;

	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* The last offset hashed
	 */
	off64_t last_offset_hashed;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* The process status information
	 */
	process_status_t *process_status;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int verification_handle_initialize(
     verification_handle_t **verification_handle,
     uint8_t calculate_md5,
     uint8_t use_data_chunk_functions,
     libcerror_error_t **error );

int verification_handle_free(
     verification_handle_t **verification_handle,
     libcerror_error_t **error );

int verification_handle_signal_abort(
     verification_handle_t *verification_handle,
     libcerror_error_t **error );

int verification_handle_set_maximum_number_of_open_handles(
     verification_handle_t *verification_handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error );

int verification_handle_open_input(
     verification_handle_t *verification_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error );

int verification_handle_close(
     verification_handle_t *verification_handle,
     libcerror_error_t **error );

ssize_t verification_handle_process_storage_media_buffer(
         verification_handle_t *verification_handle,
         storage_media_buffer_t *storage_media_buffer,
         libcerror_error_t **error );

int verification_handle_initialize_integrity_hash(
     verification_handle_t *verification_handle,
     libcerror_error_t **error );

int verification_handle_update_integrity_hash(
     verification_handle_t *verification_handle,
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error );

int verification_handle_finalize_integrity_hash(
     verification_handle_t *verification_handle,
     libcerror_error_t **error );

#if defined( HAVE_MULTI_THREAD_SUPPORT )

int verification_handle_process_storage_media_buffer_callback(
     storage_media_buffer_t *storage_media_buffer,
     verification_handle_t *verification_handle );

int verification_handle_output_storage_media_buffer_callback(
     storage_media_buffer_t *storage_media_buffer,
     verification_handle_t *verification_handle );

int verification_handle_empty_output_list(
     verification_handle_t *verification_handle,
     libcerror_error_t **error );

#endif /* defined( HAVE_MULTI_THREAD_SUPPORT ) */

int verification_handle_verify_input(
     verification_handle_t *verification_handle,
     uint8_t print_status_information,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int verification_handle_verify_single_files(
     verification_handle_t *verification_handle,
     uint8_t print_status_information,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int verification_handle_verify_file_entry(
     verification_handle_t *verification_handle,
     libewf_file_entry_t *file_entry,
     const system_character_t *file_entry_path,
     size_t file_entry_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int verification_handle_verify_sub_file_entries(
     verification_handle_t *verification_handle,
     libewf_file_entry_t *file_entry,
     const system_character_t *file_entry_path,
     size_t file_entry_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int verification_handle_get_integrity_hash_from_input(
     verification_handle_t *verification_handle,
     libcerror_error_t **error );

int verification_handle_get_integrity_hash_from_file_entry(
     verification_handle_t *verification_handle,
     libewf_file_entry_t *file_entry,
     libcerror_error_t **error );

int verification_handle_set_header_codepage(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int verification_handle_set_format(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int verification_handle_set_process_buffer_size(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int verification_handle_set_number_of_threads(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int verification_handle_set_additional_digest_types(
     verification_handle_t *verification_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int verification_handle_set_zero_chunk_on_error(
     verification_handle_t *verification_handle,
     uint8_t zero_chunk_on_error,
     libcerror_error_t **error );

int verification_handle_append_read_error(
      verification_handle_t *verification_handle,
      off64_t start_offset,
      size_t number_of_bytes,
      libcerror_error_t **error );

int verification_handle_hash_values_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     libcerror_error_t **error );

int verification_handle_additional_hash_values_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     libcerror_error_t **error );

int verification_handle_checksum_errors_fprint(
     verification_handle_t *verification_handle,
     FILE *stream,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _VERIFICATION_HANDLE_H ) */

