/*
 * File subject functions
 */

#if !defined( _LIBEWF_INTERNAL_FILE_SUBJECT_H )
#define _LIBEWF_INTERNAL_FILE_SUBJECT_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_handle.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_single_file_subject.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_file_subject libewf_internal_file_subject_t;

struct libewf_internal_file_subject
{
	/* The internal EWF handle
	 */
	libewf_internal_handle_t *internal_handle;

	/* The file subject tree node
	 */
	libcdata_tree_node_t *file_subject_tree_node;

	/* The file subject flags
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

int libewf_file_subject_initialize(
     libewf_file_subject_t **file_subject,
     libewf_internal_handle_t *internal_handle,
     libcdata_tree_node_t *file_subject_tree_node,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_subject_free(
     libewf_file_subject_t **file_subject,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_subject_get_type(
	libewf_file_subject_t *file_subject,
	uint8_t *type,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf8_name_size(
	libewf_file_subject_t *file_subject,
	size_t *utf8_name_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf8_name(
	libewf_file_subject_t *file_subject,
	uint8_t *utf8_name,
	size_t utf8_name_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf16_name_size(
	libewf_file_subject_t *file_subject,
	size_t *utf16_name_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf16_name(
	libewf_file_subject_t *file_subject,
	uint16_t *utf16_name,
	size_t utf16_name_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf8_number_size(
	libewf_file_subject_t *file_subject,
	size_t *utf8_number_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf8_number(
	libewf_file_subject_t *file_subject,
	uint8_t *utf8_number,
	size_t utf8_number_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf16_number_size(
	libewf_file_subject_t *file_subject,
	size_t *utf16_number_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf16_number(
	libewf_file_subject_t *file_subject,
	uint16_t *utf16_number,
	size_t utf16_number_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf8_comment_size(
	libewf_file_subject_t *file_subject,
	size_t *utf8_comment_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf8_comment(
	libewf_file_subject_t *file_subject,
	uint8_t *utf8_comment,
	size_t utf8_comment_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf16_comment_size(
	libewf_file_subject_t *file_subject,
	size_t *utf16_comment_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf16_comment(
	libewf_file_subject_t *file_subject,
	uint16_t *utf16_comment,
	size_t utf16_comment_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf8_guid(
	libewf_file_subject_t *file_subject,
	uint8_t *utf8_string,
	size_t utf8_string_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_utf16_guid(
	libewf_file_subject_t *file_subject,
	uint16_t *utf16_string,
	size_t utf16_string_size,
	libcerror_error_t **error);

LIBEWF_EXTERN \
int libewf_file_subject_get_number_of_sub_file_subjects(
     libewf_file_subject_t *file_subject,
     int *number_of_sub_file_subjects,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_subject_get_sub_file_subject(
     libewf_file_subject_t *file_subject,
     int sub_file_subject_index,
     libewf_file_subject_t **sub_file_subject,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_subject_get_sub_file_subject_by_id(
     libewf_file_subject_t *file_subject,
     uint32_t id,
     libewf_file_subject_t **sub_file_subject,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_INTERNAL_FILE_SUBJECT_H ) */

