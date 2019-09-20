/*
 * Single file source functions
 *
 */

#if !defined( _LIBEWF_SINGLE_FILE_EXTENDED_ATTRIBUTE_H )
#define _LIBEWF_SINGLE_FILE_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libewf_date_time.h"
#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_single_file_extended_attribute libewf_single_file_extended_attribute_t;

struct libewf_single_file_extended_attribute
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
	
	/* The value
	 */
	uint8_t *value;

	/* The value size
	 */
	size_t value_size;

	/* The item level
	 */
	int level;
};

int libewf_single_file_extended_attribute_initialize(
     libewf_single_file_extended_attribute_t **single_file_extended_attribute,
     libcerror_error_t **error );

int libewf_single_file_extended_attribute_free(
     libewf_single_file_extended_attribute_t **single_file_extended_attribute,
     libcerror_error_t **error );

int libewf_single_file_extended_attribute_clone(
     libewf_single_file_extended_attribute_t **destination_single_file_extended_attribute,
     libewf_single_file_extended_attribute_t *source_single_file_extended_attribute,
     libcerror_error_t **error );

int libewf_single_file_extended_attribute_get_type(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint8_t *type,
     libcerror_error_t **error );

int libewf_single_file_extended_attribute_get_id(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint32_t *id,
     libcerror_error_t **error );

int libewf_single_file_extended_attribute_get_utf8_name_size(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_extended_attribute_get_utf8_name(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libewf_single_file_extended_attribute_get_utf16_name_size(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_extended_attribute_get_utf16_name(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libewf_single_file_extended_attribute_get_utf8_value_size(
	 libewf_single_file_extended_attribute_t *single_file_extended_attribute,
	 size_t *utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_extended_attribute_get_utf8_value(
	 libewf_single_file_extended_attribute_t *single_file_extended_attribute,
	 uint8_t *utf8_string,
	 size_t utf8_string_size,
	 libcerror_error_t **error);

int libewf_single_file_extended_attribute_get_utf16_value_size(
	 libewf_single_file_extended_attribute_t *single_file_extended_attribute,
	 size_t *utf16_string_size,
	 libcerror_error_t **error);

int libewf_single_file_extended_attribute_get_utf16_value(
	 libewf_single_file_extended_attribute_t *single_file_extended_attribute,
	 uint16_t *utf16_string,
	 size_t utf16_string_size,
	 libcerror_error_t **error);

#pragma pack(push, 1) 
typedef struct extended_attribute_header extended_attribute_header_t;
struct /*__attribute__((__packed__))*/ extended_attribute_header
{
	uint32_t level;
	uint8_t folder;
	uint32_t name_size;
	uint32_t value_size;
};
#pragma pack(pop)	

int libewf_single_file_decode_extended_attribute_parse_item(
	libewf_single_file_extended_attribute_t *ea_item,
	uint8_t **bufCur,
	libcerror_error_t **error);

int libewf_single_file_decode_extended_attribute_parse_buffer(
	libcdata_tree_node_t *parent_extended_attribute_node,
	libewf_single_file_extended_attribute_t *parent_ea_item,
	uint8_t **bufCur,
	const uint8_t *bufMax,
	libcerror_error_t **error);

int libewf_single_file_decode_extended_attribute(
	libcdata_tree_node_t *parent_file_extended_attribute_node,
	uint8_t *encoded_tree,
	size_t encoded_tree_size,
	libcerror_error_t **error);

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SINGLE_FILE_EXTENDED_ATTRIBUTE_H ) */

