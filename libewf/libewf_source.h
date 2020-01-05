/*
 * Source functions
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

#if !defined( _LIBEWF_SOURCE_H )
#define _LIBEWF_SOURCE_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_lef_source.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_source libewf_internal_source_t;

struct libewf_internal_source
{
	/* The source
	 */
	libewf_lef_source_t *lef_source;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libewf_source_initialize(
     libewf_source_t **source,
     libewf_lef_source_t *lef_source,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_free(
     libewf_source_t **source,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_name_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_name(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_name_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_name(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_evidence_number_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_evidence_number(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_evidence_number_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_evidence_number(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_location_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_location(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_location_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_location(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_device_guid_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_device_guid(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_device_guid_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_device_guid(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_primary_device_guid_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_primary_device_guid(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_primary_device_guid_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_primary_device_guid(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_manufacturer_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_manufacturer(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_manufacturer_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_manufacturer(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_model_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_model(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_model_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_model(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_serial_number_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_serial_number(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_serial_number_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_serial_number(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_domain_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_domain(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_domain_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_domain(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_ip_address_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_ip_address(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_ip_address_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_ip_address(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_mac_address_size(
     libewf_source_t *source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_mac_address(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_mac_address_size(
     libewf_source_t *source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_mac_address(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_size(
     libewf_source_t *source,
     size64_t *size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_acquisition_time(
     libewf_source_t *source,
     int64_t *posix_time,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_hash_value_md5(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_hash_value_md5(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf8_hash_value_sha1(
     libewf_source_t *source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_source_get_utf16_hash_value_sha1(
     libewf_source_t *source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SOURCE_H ) */

