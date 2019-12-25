/*
 * Logical Evidence File (LEF) file entry functions
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_SINGLE_FILE_ENTRY_H )
#define _LIBEWF_SINGLE_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_single_file_entry libewf_single_file_entry_t;

struct libewf_single_file_entry
{
	/* The type
	 */
	uint8_t type;

	/* The flags
	 */
	uint32_t flags;

	/* The data offset
	 */
	off64_t data_offset;

	/* The data size
	 */
	size64_t data_size;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;

	/* The short name
	 */
	uint8_t *short_name;

	/* The short name size
	 */
	size_t short_name_size;

	/* The (file) size
	 */
	size64_t size;

	/* The duplicate data offset
	 */
	off64_t duplicate_data_offset;

	/* The source identifier
	 */
	uint32_t source_identifier;

	/* The subject identifier
	 */
	uint32_t subject_identifier;

	/* The permissions identifier
	 */
	uint32_t permissions_identifier;

	/* The record type
	 */
	uint32_t record_type;

	/* The creation date and time
	 * stored as a POSIX timestamp
	 */
	int32_t creation_time;

	/* The last modification date and time
	 * stored as a POSIX timestamp
	 */
	int32_t modification_time;

	/* The last access date and time
	 * stored as a POSIX timestamp
	 */
	int32_t access_time;

	/* The last (file system) entry modification date and time
	 * stored as a POSIX timestamp
	 */
	int32_t entry_modification_time;

	/* The deletion date and time
	 * stored as a POSIX timestamp
	 */
	int32_t deletion_time;

	/* The MD5 digest hash
	 */
	uint8_t *md5_hash;

	/* The MD5 digest hash size
	 */
	size_t md5_hash_size;

	/* The SHA1 digest hash
	 */
	uint8_t *sha1_hash;

	/* The SHA1 digest hash size
	 */
	size_t sha1_hash_size;
};

int libewf_single_file_entry_initialize(
     libewf_single_file_entry_t **single_file_entry,
     libcerror_error_t **error );

int libewf_single_file_entry_free(
     libewf_single_file_entry_t **single_file_entry,
     libcerror_error_t **error );

int libewf_single_file_entry_clone(
     libewf_single_file_entry_t **destination_single_file_entry,
     libewf_single_file_entry_t *source_single_file_entry,
     libcerror_error_t **error );

int libewf_single_file_entry_read_binary_extents(
     libewf_single_file_entry_t *single_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_single_file_entry_read_extended_attributes(
     libewf_single_file_entry_t *single_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_single_file_entry_read_hexadecimal_string(
     libewf_single_file_entry_t *single_file_entry,
     const uint8_t *data,
     size_t data_size,
     uint8_t *string,
     size_t string_size,
     int *zero_values_only,
     libcerror_error_t **error );

int libewf_single_file_entry_read_short_name(
     libewf_single_file_entry_t *single_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_single_file_entry_read_data(
     libewf_single_file_entry_t *single_file_entry,
     libfvalue_split_utf8_string_t *types,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_type(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *type,
     libcerror_error_t **error );

int libewf_single_file_entry_get_flags(
     libewf_single_file_entry_t *single_file_entry,
     uint32_t *flags,
     libcerror_error_t **error );

int libewf_single_file_entry_get_data_offset(
     libewf_single_file_entry_t *single_file_entry,
     off64_t *data_offset,
     libcerror_error_t **error );

int libewf_single_file_entry_get_data_size(
     libewf_single_file_entry_t *single_file_entry,
     size64_t *data_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_duplicate_data_offset(
     libewf_single_file_entry_t *single_file_entry,
     off64_t *duplicate_data_offset,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf8_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf8_name(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf16_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf16_name(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf8_short_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf8_short_name(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf16_short_name_size(
     libewf_single_file_entry_t *single_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf16_short_name(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_size(
     libewf_single_file_entry_t *single_file_entry,
     size64_t *size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_record_type(
     libewf_single_file_entry_t *single_file_entry,
     uint32_t *record_type,
     libcerror_error_t **error );

int libewf_single_file_entry_get_creation_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *creation_time,
     libcerror_error_t **error );

int libewf_single_file_entry_get_modification_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *modification_time,
     libcerror_error_t **error );

int libewf_single_file_entry_get_access_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *access_time,
     libcerror_error_t **error );

int libewf_single_file_entry_get_entry_modification_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *entry_modification_time,
     libcerror_error_t **error );

int libewf_single_file_entry_get_deletion_time(
     libewf_single_file_entry_t *single_file_entry,
     int32_t *deletion_time,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf8_hash_value_md5(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf16_hash_value_md5(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf8_hash_value_sha1(
     libewf_single_file_entry_t *single_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_entry_get_utf16_hash_value_sha1(
     libewf_single_file_entry_t *single_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SINGLE_FILE_ENTRY_H ) */

