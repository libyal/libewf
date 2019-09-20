/*
 * File permission functions
 */

#if !defined( _LIBEWF_INTERNAL_FILE_PERMISSION_H )
#define _LIBEWF_INTERNAL_FILE_PERMISSION_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_handle.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_single_file_permission.h"
#include "libewf_types.h"

#if defined( _cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_file_permission libewf_internal_file_permission_t;

struct libewf_internal_file_permission
{
	/* The internal EWF handle
	 */
	libewf_internal_handle_t *internal_handle;

	/* The file permission tree node
	 */
	libcdata_tree_node_t *file_permission_tree_node;

	/* The file permission flags
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

int libewf_file_permission_initialize(
     libewf_file_permission_t **file_permission,
     libewf_internal_handle_t *internal_handle,
     libcdata_tree_node_t *file_permission_tree_node,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_permission_free(
     libewf_file_permission_t **file_permission,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_permission_get_type(
	libewf_file_permission_t *file_permission,
	uint8_t *type,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_props(
	libewf_file_permission_t *_file_permission,
	uint32_t *props,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_access_mask(
	libewf_file_permission_t *_file_permission,
	uint32_t *access_mask,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_inheritance_flags(
	libewf_file_permission_t *_file_permission,
	uint32_t *inheritance_flags,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_utf8_name_size(
	libewf_file_permission_t *file_permission,
	size_t *utf8_name_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_utf8_name(
	libewf_file_permission_t *file_permission,
	uint8_t *utf8_name,
	size_t utf8_name_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_utf16_name_size(
	libewf_file_permission_t *file_permission,
	size_t *utf16_name_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_utf16_name(
	libewf_file_permission_t *file_permission,
	uint16_t *utf16_name,
	size_t utf16_name_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_utf8_uid_size(
	libewf_file_permission_t *file_permission,
	size_t *utf8_uid_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_utf8_uid(
	libewf_file_permission_t *file_permission,
	uint8_t *utf8_uid,
	size_t utf8_uid_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_utf16_uid_size(
	libewf_file_permission_t *file_permission,
	size_t *utf16_uid_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_utf16_uid(
	libewf_file_permission_t *file_permission,
	uint16_t *utf16_uid,
	size_t utf16_uid_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_permission_get_number_of_sub_file_permissions(
     libewf_file_permission_t *file_permission,
     int *number_of_sub_file_permissions,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_permission_get_sub_file_permission(
     libewf_file_permission_t *file_permission,
     int sub_file_permission_index,
     libewf_file_permission_t **sub_file_permission,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_permission_get_sub_file_permission_by_id(
     libewf_file_permission_t *file_permission,
     uint32_t id,
     libewf_file_permission_t **sub_file_permission,
     libcerror_error_t **error );

#if defined( _cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_INTERNAL_FILE_PERMISSION_H ) */

