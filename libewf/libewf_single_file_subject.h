/*
 * Single file source functions
 *
 */

#if !defined( _LIBEWF_SINGLE_FILE_SUBJECT_H )
#define _LIBEWF_SINGLE_FILE_SUBJECT_H

#include <common.h>
#include <types.h>

#include "libewf_date_time.h"
#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_single_file_subject libewf_single_file_subject_t;

struct libewf_single_file_subject
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
	
	/* The comment
	 */
	uint8_t *comment;

	/* The comment size
	 */
	size_t comment_size;

	/* The number
	 */
	uint8_t *number;

	/* The number size
	 */
	size_t number_size;

    /* The GUID
    */
    uint8_t *guid;

    /* The GUID size
    */
    size_t guid_size;
};

int libewf_single_file_subject_initialize(
     libewf_single_file_subject_t **single_file_subject,
     libcerror_error_t **error );

int libewf_single_file_subject_free(
     libewf_single_file_subject_t **single_file_subject,
     libcerror_error_t **error );

int libewf_single_file_subject_clone(
     libewf_single_file_subject_t **destination_single_file_subject,
     libewf_single_file_subject_t *source_single_file_subject,
     libcerror_error_t **error );

int libewf_single_file_subject_get_type(
     libewf_single_file_subject_t *single_file_subject,
     uint8_t *type,
     libcerror_error_t **error );

int libewf_single_file_subject_get_id(
     libewf_single_file_subject_t *single_file_subject,
     uint32_t *id,
     libcerror_error_t **error );

int libewf_single_file_subject_get_utf8_name_size(
     libewf_single_file_subject_t *single_file_subject,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_subject_get_utf8_name(
     libewf_single_file_subject_t *single_file_subject,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_subject_get_utf16_name_size(
     libewf_single_file_subject_t *single_file_subject,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_subject_get_utf16_name(
     libewf_single_file_subject_t *single_file_subject,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_subject_get_utf8_comment_size(
	 libewf_single_file_subject_t *single_file_subject,
	 size_t *utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_subject_get_utf8_comment(
	 libewf_single_file_subject_t *single_file_subject,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_subject_get_utf16_comment_size(
	 libewf_single_file_subject_t *single_file_subject,
	 size_t *utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_subject_get_utf16_comment(
	 libewf_single_file_subject_t *single_file_subject,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_subject_get_utf8_number_size(
	 libewf_single_file_subject_t *single_file_subject,
	 size_t *utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_subject_get_utf8_number(
	 libewf_single_file_subject_t *single_file_subject,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_subject_get_utf16_number_size(
	 libewf_single_file_subject_t *single_file_subject,
	 size_t *utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_subject_get_utf16_number(
	 libewf_single_file_subject_t *single_file_subject,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_subject_get_utf8_guid(
     libewf_single_file_subject_t *single_file_subject,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error);

int libewf_single_file_subject_get_utf16_guid(
     libewf_single_file_subject_t *single_file_subject,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error);

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SINGLE_FILE_SUBJECT_H ) */

