/*
 * Logical Evidence File (LEF) file entry functions
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

#if !defined( _LIBEWF_LEF_FILE_ENTRY_H )
#define _LIBEWF_LEF_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libewf_lef_extended_attribute.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_serialized_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_lef_file_entry libewf_lef_file_entry_t;

struct libewf_lef_file_entry
{
	/* The identifier
	 */
	uint64_t identifier;

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

	/* The GUID string
	 */
	libewf_serialized_string_t *guid;

	/* The name string
	 */
	libewf_serialized_string_t *name;

	/* The short name string
	 */
	libewf_serialized_string_t *short_name;

	/* The (file) size
	 */
	size64_t size;

	/* The logical offset
	 */
	off64_t logical_offset;

	/* The physical offset
	 */
	off64_t physical_offset;

	/* The duplicate data offset
	 */
	off64_t duplicate_data_offset;

	/* The identifier of the source of the file entry
	 */
	int source_identifier;

	/* The identifier of the corresponding subject
	 */
	uint32_t subject_identifier;

	/* The permission group index
	 */
	int permission_group_index;

	/* The record type
	 */
	uint32_t record_type;

	/* The creation date and time
	 * stored as a POSIX timestamp
	 */
	int64_t creation_time;

	/* The last modification date and time
	 * stored as a POSIX timestamp
	 */
	int64_t modification_time;

	/* The last access date and time
	 * stored as a POSIX timestamp
	 */
	int64_t access_time;

	/* The last (file system) entry modification date and time
	 * stored as a POSIX timestamp
	 */
	int64_t entry_modification_time;

	/* The deletion date and time
	 * stored as a POSIX timestamp
	 */
	int64_t deletion_time;

	/* The MD5 digest hash string
	 */
	libewf_serialized_string_t *md5_hash;

	/* The SHA1 digest hash string
	 */
	libewf_serialized_string_t *sha1_hash;

	/* The extended attributes array
	 */
	libcdata_array_t *extended_attributes;
};

int libewf_lef_file_entry_initialize(
     libewf_lef_file_entry_t **lef_file_entry,
     libcerror_error_t **error );

int libewf_lef_file_entry_free(
     libewf_lef_file_entry_t **lef_file_entry,
     libcerror_error_t **error );

int libewf_lef_file_entry_clone(
     libewf_lef_file_entry_t **destination_lef_file_entry,
     libewf_lef_file_entry_t *source_lef_file_entry,
     libcerror_error_t **error );

int libewf_lef_file_entry_read_binary_extents(
     libewf_lef_file_entry_t *lef_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_read_extended_attributes(
     libewf_lef_file_entry_t *lef_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_read_short_name(
     libewf_lef_file_entry_t *lef_file_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_read_data(
     libewf_lef_file_entry_t *lef_file_entry,
     libfvalue_split_utf8_string_t *types,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_identifier(
     libewf_lef_file_entry_t *lef_file_entry,
     uint64_t *identifier,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_type(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *type,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_flags(
     libewf_lef_file_entry_t *lef_file_entry,
     uint32_t *flags,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_data_offset(
     libewf_lef_file_entry_t *lef_file_entry,
     off64_t *data_offset,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_data_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size64_t *data_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_logical_offset(
     libewf_lef_file_entry_t *lef_file_entry,
     off64_t *logical_offset,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_physical_offset(
     libewf_lef_file_entry_t *lef_file_entry,
     off64_t *physical_offset,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_duplicate_data_offset(
     libewf_lef_file_entry_t *lef_file_entry,
     off64_t *duplicate_data_offset,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf8_guid_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf8_guid(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf16_guid_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf16_guid(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf8_name_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf8_name(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf16_name_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf16_name(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf8_short_name_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf8_short_name(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf16_short_name_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf16_short_name(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_size(
     libewf_lef_file_entry_t *lef_file_entry,
     size64_t *size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_source_identifier(
     libewf_lef_file_entry_t *lef_file_entry,
     int *source_identifier,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_permission_group_index(
     libewf_lef_file_entry_t *lef_file_entry,
     int *permission_group_index,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_record_type(
     libewf_lef_file_entry_t *lef_file_entry,
     uint32_t *record_type,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_creation_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_modification_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_access_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_entry_modification_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_deletion_time(
     libewf_lef_file_entry_t *lef_file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf8_hash_value_md5(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf16_hash_value_md5(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf8_hash_value_sha1(
     libewf_lef_file_entry_t *lef_file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_utf16_hash_value_sha1(
     libewf_lef_file_entry_t *lef_file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_number_of_extended_attributes(
     libewf_lef_file_entry_t *lef_file_entry,
     int *number_of_extended_attributes,
     libcerror_error_t **error );

int libewf_lef_file_entry_get_extended_attribute_by_index(
     libewf_lef_file_entry_t *lef_file_entry,
     int extended_attribute_index,
     libewf_lef_extended_attribute_t **lef_extended_attribute,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_LEF_FILE_ENTRY_H ) */

