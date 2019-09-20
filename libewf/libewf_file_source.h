/*
 * File source functions
 */

#if !defined( _LIBEWF_INTERNAL_FILE_SOURCE_H )
#define _LIBEWF_INTERNAL_FILE_SOURCE_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_handle.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_single_file_source.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_file_source libewf_internal_file_source_t;

struct libewf_internal_file_source
{
	/* The internal EWF handle
	 */
	libewf_internal_handle_t *internal_handle;

	/* The file source tree node
	 */
	libcdata_tree_node_t *file_source_tree_node;

	/* The file source flags
	 */
	 uint8_t flags;

	/* The offset
	 */
	 off64_t offset;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libewf_file_source_initialize(
     libewf_file_source_t **file_source,
     libewf_internal_handle_t *internal_handle,
     libcdata_tree_node_t *file_source_tree_node,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_source_free(
     libewf_file_source_t **file_source,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_source_get_type(
	 libewf_file_source_t *file_source,
	 uint8_t *type,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_drive_type(
	 libewf_file_source_t *file_source,
	 uint8_t *drive_type,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_physical_offset(
	 libewf_file_source_t *file_source,
	 off64_t *physical_offset,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_logical_offset(
	 libewf_file_source_t *file_source,
	 off64_t *logical_offset,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_total_size(
	 libewf_file_source_t *file_source,
	size64_t *total_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_name_size(
	 libewf_file_source_t *file_source,
	 size_t *utf8_name_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_name(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_name,
	 size_t utf8_name_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_name_size(
	 libewf_file_source_t *file_source,
	 size_t *utf16_name_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_name(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_name,
	 size_t utf16_name_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_serial_number_size(
	 libewf_file_source_t *file_source,
	 size_t *utf8_serial_number_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_serial_number(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_serial_number,
	 size_t utf8_serial_number_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_serial_number_size(
	 libewf_file_source_t *file_source,
	 size_t *utf16_serial_number_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_serial_number(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_serial_number,
	 size_t utf16_serial_number_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_model_size(
	 libewf_file_source_t *file_source,
	 size_t *utf8_model_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_model(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_model,
	 size_t utf8_model_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_model_size(
	 libewf_file_source_t *file_source,
	 size_t *utf16_model_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_model(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_model,
	 size_t utf16_model_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_manufacturer_size(
	 libewf_file_source_t *file_source,
	 size_t *utf8_manufacturer_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_manufacturer(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_manufacturer,
	 size_t utf8_manufacturer_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_manufacturer_size(
	 libewf_file_source_t *file_source,
	 size_t *utf16_manufacturer_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_manufacturer(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_manufacturer,
	 size_t utf16_manufacturer_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_evidence_number_size(
	 libewf_file_source_t *file_source,
	 size_t *utf8_evidence_number_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_evidence_number(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_evidence_number,
	 size_t utf8_evidence_number_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_evidence_number_size(
	 libewf_file_source_t *file_source,
	 size_t *utf16_evidence_number_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_evidence_number(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_evidence_number,
	 size_t utf16_evidence_number_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_acquisition_time(
	 libewf_file_source_t *file_source,
	 uint32_t *acquisition_time,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_hash_value_md5(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_hash_value_md5(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_hash_value_sha1(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_hash_value_sha1(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_guid(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_guid(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_primary_guid(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_primary_guid(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_location_size(
	 libewf_file_source_t *file_source,
	 size_t *utf8_location_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_location(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_location,
	 size_t utf8_location_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_location_size(
	 libewf_file_source_t *file_source,
	 size_t *utf16_location_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_location(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_location,
	 size_t utf16_location_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_domain_size(
	 libewf_file_source_t *file_source,
	 size_t *utf8_domain_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_domain(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_domain,
	 size_t utf8_domain_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_domain_size(
	 libewf_file_source_t *file_source,
	 size_t *utf16_domain_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_domain(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_domain,
	 size_t utf16_domain_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_ip_address_size(
	 libewf_file_source_t *file_source,
	 size_t *utf8_ip_address_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_ip_address(
	 libewf_file_source_t *file_source,
	 uint8_t *utf8_ip_address,
	 size_t utf8_ip_address_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_ip_address_size(
	 libewf_file_source_t *file_source,
	 size_t *utf16_ip_address_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_ip_address(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_ip_address,
	 size_t utf16_ip_address_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_static_ip(
	 libewf_file_source_t *file_source,
	 uint8_t *static_ip,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_mac_address_size(
	 libewf_file_source_t *file_source,
	 size_t *utf8_mac_address_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf8_mac_address(
 	 libewf_file_source_t *file_source,
 	 uint8_t *utf8_mac_address,
 	 size_t utf8_mac_address_size,
 	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_mac_address_size(
	 libewf_file_source_t *file_source,
	 size_t *utf16_mac_address_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_utf16_mac_address(
	 libewf_file_source_t *file_source,
	 uint16_t *utf16_mac_address,
	 size_t utf16_mac_address_size,
	 libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_source_get_number_of_sub_file_sources(
     libewf_file_source_t *file_source,
     int *number_of_sub_file_sources,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_source_get_sub_file_source(
     libewf_file_source_t *file_source,
     int sub_file_source_index,
     libewf_file_source_t **sub_file_source,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_source_get_sub_file_source_by_id(
     libewf_file_source_t *file_source,
     uint32_t id,
     libewf_file_source_t **sub_file_source,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_INTERNAL_FILE_SOURCE_H ) */

