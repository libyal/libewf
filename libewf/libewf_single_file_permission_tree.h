/*
 * Single file permission functions
 */

#if !defined( _LIBEWF_SINGLE_FILE_PERMISSION_TREE_H )
#define _LIBEWF_SINGLE_FILE_PERMISSION_TREE_H

#include <common.h>
#include <types.h>

#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_single_file_permission.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_single_file_permission_tree_get_sub_node_by_id(
	 libcdata_tree_node_t *node,
     uint32_t permission_id,
     libcdata_tree_node_t **sub_node,
     libewf_single_file_permission_t **sub_single_file_permission,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SINGLE_FILE_PERMISSION_TREE_H ) */

