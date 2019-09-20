/*
 * File extended_attribute functions
 */

#if !defined( _LIBEWF_INTERNAL_FILE_EXTENDED_ATTRIBUTE_H )
#define _LIBEWF_INTERNAL_FILE_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_handle.h"
#include "libewf_file_entry.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_single_file_extended_attribute.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_file_extended_attribute libewf_internal_file_extended_attribute_t;

struct libewf_internal_file_extended_attribute
{
	/* The internal EWF handle
	 */
	libewf_internal_file_entry_t *internal_file_entry;

	/* The file extended_attribute tree node
	 */
	libcdata_tree_node_t *file_extended_attribute_tree_node;

	/* The file extended_attribute flags
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

int libewf_file_extended_attribute_initialize(
     libewf_file_extended_attribute_t **file_extended_attribute,
     libewf_internal_file_entry_t *internal_file_entry,
     libcdata_tree_node_t *file_extended_attribute_tree_node,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_extended_attribute_free(
     libewf_file_extended_attribute_t **file_extended_attribute,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_type(
     libewf_file_extended_attribute_t *file_extended_attribute,
     uint8_t *type,
     libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_utf8_name_size(
     libewf_file_extended_attribute_t *file_extended_attribute,
     size_t *utf8_name_size,
     libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_utf8_name(
     libewf_file_extended_attribute_t *file_extended_attribute,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_utf16_name_size(
     libewf_file_extended_attribute_t *file_extended_attribute,
     size_t *utf16_name_size,
     libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_utf16_name(
     libewf_file_extended_attribute_t *file_extended_attribute,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_utf8_value_size(
     libewf_file_extended_attribute_t *file_extended_attribute,
     size_t *utf8_value_size,
     libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_utf8_value(
     libewf_file_extended_attribute_t *file_extended_attribute,
     uint8_t *utf8_value,
     size_t utf8_value_size,
     libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_utf16_value_size(
     libewf_file_extended_attribute_t *file_extended_attribute,
     size_t *utf16_value_size,
     libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_utf16_value(
     libewf_file_extended_attribute_t *file_extended_attribute,
     uint16_t *utf16_value,
     size_t utf16_value_size,
     libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_number_of_sub_file_extended_attributes(
     libewf_file_extended_attribute_t *file_extended_attribute,
     int *number_of_sub_file_extended_attributes,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_extended_attribute_get_sub_file_extended_attribute(
     libewf_file_extended_attribute_t *file_extended_attribute,
     int sub_file_extended_attribute_index,
     libewf_file_extended_attribute_t **sub_file_extended_attribute,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_INTERNAL_FILE_EXTENDED_ATTRIBUTE_H ) */

