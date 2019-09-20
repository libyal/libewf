/*
 * Single file source functions
 *
 */

#if !defined( _LIBEWF_SINGLE_FILE_PERMISSION_H )
#define _LIBEWF_SINGLE_FILE_PERMISSION_H

#include <common.h>
#include <types.h>

#include "libewf_date_time.h"
#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_single_file_permission libewf_single_file_permission_t;

struct libewf_single_file_permission
{
	/* The id
	 */
	uint32_t id;

	/* The type
	 */
	uint8_t type;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;

	/* The uid
	 */
	uint8_t *uid;

	/* The uid size
	 */
	size_t uid_size;
	
	/* The props
	 */
	uint32_t props;

	/* The access mask
	 */
	uint32_t access_mask;

	/* The inheritance flags
	 */
	uint32_t inheritance_flags;
};

int libewf_single_file_permission_initialize(
     libewf_single_file_permission_t **single_file_permission,
     libcerror_error_t **error );

int libewf_single_file_permission_free(
     libewf_single_file_permission_t **single_file_permission,
     libcerror_error_t **error );

int libewf_single_file_permission_clone(
     libewf_single_file_permission_t **destination_single_file_permission,
     libewf_single_file_permission_t *source_single_file_permission,
     libcerror_error_t **error );

int libewf_single_file_permission_get_type(
     libewf_single_file_permission_t *single_file_permission,
     uint8_t *type,
     libcerror_error_t **error );

int libewf_single_file_permission_get_id(
	 libewf_single_file_permission_t *single_file_permission,
	 uint32_t *id,
	 libcerror_error_t **error);

int libewf_single_file_permission_get_props(
	 libewf_single_file_permission_t *single_file_permission,
	 uint32_t *props,
	 libcerror_error_t **error);

int libewf_single_file_permission_get_access_mask(
	 libewf_single_file_permission_t *single_file_permission,
	 uint32_t *access_mask,
	 libcerror_error_t **error);

int libewf_single_file_permission_get_inheritance_flags(
	 libewf_single_file_permission_t *single_file_permission,
	 uint32_t *inheritance_flags,
	 libcerror_error_t **error);

int libewf_single_file_permission_get_utf8_name_size(
     libewf_single_file_permission_t *single_file_permission,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_permission_get_utf8_name(
     libewf_single_file_permission_t *single_file_permission,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_permission_get_utf16_name_size(
     libewf_single_file_permission_t *single_file_permission,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_permission_get_utf16_name(
     libewf_single_file_permission_t *single_file_permission,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_permission_get_utf8_uid_size(
	 libewf_single_file_permission_t *single_file_permission,
	 size_t *utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_permission_get_utf8_uid(
	 libewf_single_file_permission_t *single_file_permission,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_permission_get_utf16_uid_size(
	 libewf_single_file_permission_t *single_file_permission,
	 size_t *utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_permission_get_utf16_uid(
	 libewf_single_file_permission_t *single_file_permission,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SINGLE_FILE_PERMISSION_H ) */

