/*
 * Single file source functions
 *
 */

#if !defined( _LIBEWF_SINGLE_FILE_SOURCE_H )
#define _LIBEWF_SINGLE_FILE_SOURCE_H

#include <common.h>
#include <types.h>

#include "libewf_date_time.h"
#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_single_file_source libewf_single_file_source_t;

struct libewf_single_file_source
{
	/* The id
	 */
	uint32_t id;

	/* The type
	 */
	uint8_t type;

	/* The drive type
	 */
	uint8_t drive_type;

	/* The physical offset
	 */
	off64_t physical_offset;

	/* The logical offset
	 */
	off64_t logical_offset;

	/* The acquisition time
	 */
	uint32_t acquisition_time;

	/* The total size
	 */
	size64_t total_size;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;
	
	/* The serial number
	 */
	uint8_t *serial_number;

	/* The serial number size
	 */
	size_t serial_number_size;
	
	/* The model
	 */
	uint8_t *model;

	/* The model size
	 */
	size_t model_size;

	/* The manufacturer
	 */
	uint8_t *manufacturer;

	/* The manufacturer size
	 */
	size_t manufacturer_size;

	/* The evidence number
	 */
	uint8_t *evidence_number;

	/* The evidence number size
	 */
	size_t evidence_number_size;

	/* The location
	 */
	uint8_t *location;

	/* The location size
	 */
	size_t location_size;

	/* The domain
	 */
	uint8_t *domain;

	/* The domain size
	 */
	size_t domain_size;

	/* The ip address
	 */
	uint8_t *ip_address;

	/* The ip address size
	 */
	size_t ip_address_size;

	/* The static ip
	 */
	uint8_t static_ip;
	
	/* The mac address
	 */
	uint8_t *mac_address;

	/* The mac address size
	 */
	size_t mac_address_size;

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

    /* The (file) GUID
    */
    uint8_t *guid;

    /* The (file) GUID size
    */
    size_t guid_size;

    /* The (file) GUID
    */
    uint8_t *primary_guid;

    /* The (file) GUID size
    */
    size_t primary_guid_size;
};

int libewf_single_file_source_initialize(
     libewf_single_file_source_t **single_file_source,
     libcerror_error_t **error );

int libewf_single_file_source_free(
     libewf_single_file_source_t **single_file_source,
     libcerror_error_t **error );

int libewf_single_file_source_clone(
     libewf_single_file_source_t **destination_single_file_source,
     libewf_single_file_source_t *source_single_file_source,
     libcerror_error_t **error );

int libewf_single_file_source_get_type(
     libewf_single_file_source_t *single_file_source,
     uint8_t *type,
     libcerror_error_t **error );

int libewf_single_file_source_get_id(
     libewf_single_file_source_t *single_file_source,
     uint32_t *id,
     libcerror_error_t **error );

int libewf_single_file_source_get_drive_type(
     libewf_single_file_source_t *single_file_source,
     uint8_t *drive_type,
     libcerror_error_t **error );

int libewf_single_file_source_get_physical_offset(
     libewf_single_file_source_t *single_file_source,
     off64_t *physical_offset,
     libcerror_error_t **error );

int libewf_single_file_source_get_logical_offset(
     libewf_single_file_source_t *single_file_source,
     off64_t *logical_offset,
     libcerror_error_t **error );

int libewf_single_file_source_get_total_size(
     libewf_single_file_source_t *single_file_source,
     size64_t *total_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf8_name_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf8_name(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf16_name_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf16_name(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf8_serial_number_size(
	 libewf_single_file_source_t *single_file_source,
	 size_t *utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_source_get_utf8_serial_number(
	 libewf_single_file_source_t *single_file_source,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_source_get_utf16_serial_number_size(
	 libewf_single_file_source_t *single_file_source,
	 size_t *utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_source_get_utf16_serial_number(
	 libewf_single_file_source_t *single_file_source,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_source_get_utf8_manufacturer_size(
	 libewf_single_file_source_t *single_file_source,
	 size_t *utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_source_get_utf8_manufacturer(
	 libewf_single_file_source_t *single_file_source,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_source_get_utf16_manufacturer_size(
	 libewf_single_file_source_t *single_file_source,
	 size_t *utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_source_get_utf16_manufacturer(
	 libewf_single_file_source_t *single_file_source,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_source_get_utf8_model_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf8_model(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf16_model_size(
     libewf_single_file_source_t *single_file_source,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf16_model(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf8_evidence_number_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf8_evidence_number(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_evidence_number_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_evidence_number(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf8_hash_value_md5(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf16_hash_value_md5(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf8_hash_value_sha1(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf16_hash_value_sha1(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_source_get_utf8_guid(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error);

int libewf_single_file_source_get_utf16_guid(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error);

int libewf_single_file_source_get_utf8_primary_guid(
     libewf_single_file_source_t *single_file_source,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error);

int libewf_single_file_source_get_utf16_primary_guid(
     libewf_single_file_source_t *single_file_source,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error);

int libewf_single_file_source_get_acquisition_time(
	libewf_single_file_source_t *single_file_source,
	uint32_t *acquisition_time,
	libcerror_error_t **error);


int libewf_single_file_source_get_utf8_location_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error );

int libewf_single_file_source_get_utf8_location(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_location_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_location(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf8_domain_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf8_domain(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_domain_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_domain(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf8_ip_address_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf8_ip_address(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_ip_address_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_ip_address(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_static_ip(
	libewf_single_file_source_t *single_file_source,
	uint8_t *static_ip,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf8_mac_address_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf8_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf8_mac_address(
	libewf_single_file_source_t *single_file_source,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_mac_address_size(
	libewf_single_file_source_t *single_file_source,
	size_t *utf16_string_size,
	libcerror_error_t **error);

int libewf_single_file_source_get_utf16_mac_address(
	libewf_single_file_source_t *single_file_source,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error);

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SINGLE_FILE_SOURCE_H ) */

