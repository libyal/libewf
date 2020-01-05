/*
 * Logical Evidence File (LEF) source functions
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

#if !defined( _LIBEWF_LEF_SOURCE_H )
#define _LIBEWF_LEF_SOURCE_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_serialized_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_lef_source libewf_lef_source_t;

struct libewf_lef_source
{
	/* The identifier
	 */
	int identifier;

	/* The name string
	 */
	libewf_serialized_string_t *name;

	/* The evidence number string
	 */
	libewf_serialized_string_t *evidence_number;

	/* The location string
	 */
	libewf_serialized_string_t *location;

	/* The device GUID string
	 */
	libewf_serialized_string_t *device_guid;

	/* The primary device GUID string
	 */
	libewf_serialized_string_t *primary_device_guid;

	/* The drive type
	 */
	uint8_t drive_type;

	/* The manufacturer string
	 */
	libewf_serialized_string_t *manufacturer;

	/* The model string
	 */
	libewf_serialized_string_t *model;

	/* The serial number string
	 */
	libewf_serialized_string_t *serial_number;

	/* The domain string
	 */
	libewf_serialized_string_t *domain;

	/* The IP address string
	 */
	libewf_serialized_string_t *ip_address;

	/* The MAC address string
	 */
	libewf_serialized_string_t *mac_address;

	/* The size
	 */
	size64_t size;

	/* The logical offset
	 */
	off64_t logical_offset;

	/* The physical offset
	 */
	off64_t physical_offset;

	/* The acquisition date and time
	 * stored as a POSIX timestamp
	 */
	int64_t acquisition_time;

	/* The MD5 digest hash string
	 */
	libewf_serialized_string_t *md5_hash;

	/* The SHA1 digest hash string
	 */
	libewf_serialized_string_t *sha1_hash;
};

int libewf_lef_source_initialize(
     libewf_lef_source_t **lef_source,
     libcerror_error_t **error );

int libewf_lef_source_free(
     libewf_lef_source_t **lef_source,
     libcerror_error_t **error );

int libewf_lef_source_clone(
     libewf_lef_source_t **destination_lef_source,
     libewf_lef_source_t *source_lef_source,
     libcerror_error_t **error );

int libewf_lef_source_read_data(
     libewf_lef_source_t *lef_source,
     libfvalue_split_utf8_string_t *types,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libewf_lef_source_get_identifier(
     libewf_lef_source_t *lef_source,
     int *identifier,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_name_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_name(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_name_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_name(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_evidence_number_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_evidence_number(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_evidence_number_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_evidence_number(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_location_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_location(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_location_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_location(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_device_guid_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_device_guid(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_device_guid_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_device_guid(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_primary_device_guid_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_primary_device_guid(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_primary_device_guid_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_primary_device_guid(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_drive_type(
     libewf_lef_source_t *lef_source,
     uint8_t *drive_type,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_manufacturer_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_manufacturer(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_manufacturer_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_manufacturer(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_model_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_model(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_model_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_model(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_serial_number_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_serial_number(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_serial_number_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_serial_number(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_domain_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_domain(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_domain_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_domain(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_ip_address_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_ip_address(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_ip_address_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_ip_address(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_mac_address_size(
     libewf_lef_source_t *lef_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_mac_address(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_mac_address_size(
     libewf_lef_source_t *lef_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_mac_address(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_size(
     libewf_lef_source_t *lef_source,
     size64_t *size,
     libcerror_error_t **error );

int libewf_lef_source_get_logical_offset(
     libewf_lef_source_t *lef_source,
     off64_t *logical_offset,
     libcerror_error_t **error );

int libewf_lef_source_get_physical_offset(
     libewf_lef_source_t *lef_source,
     off64_t *physical_offset,
     libcerror_error_t **error );

int libewf_lef_source_get_acquisition_time(
     libewf_lef_source_t *lef_source,
     int64_t *posix_time,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_hash_value_md5(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_hash_value_md5(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf8_hash_value_sha1(
     libewf_lef_source_t *lef_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_lef_source_get_utf16_hash_value_sha1(
     libewf_lef_source_t *lef_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_LEF_SOURCE_H ) */

