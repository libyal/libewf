/*
 * Single file tree functions
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

#if !defined( _LIBEWF_SINGLE_FILE_TREE_H )
#define _LIBEWF_SINGLE_FILE_TREE_H

#include <common.h>
#include <types.h>

#include "libewf_lef_file_entry.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_single_file_tree_get_sub_node_by_utf8_name(
     libcdata_tree_node_t *single_file_tree_node,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcdata_tree_node_t **single_file_tree_sub_node,
     libewf_lef_file_entry_t **sub_lef_file_entry,
     libcerror_error_t **error );

int libewf_single_file_tree_get_sub_node_by_utf16_name(
     libcdata_tree_node_t *single_file_tree_node,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcdata_tree_node_t **single_file_tree_sub_node,
     libewf_lef_file_entry_t **sub_lef_file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SINGLE_FILE_TREE_H ) */

