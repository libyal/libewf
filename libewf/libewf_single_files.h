/*
 * Single files functions
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEWF_SINGLE_FILES_H )
#define _LIBEWF_SINGLE_FILES_H

#include <common.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include "libewf_extern.h"
#include "libewf_split_values.h"
#include "libewf_tree_type.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_single_files libewf_single_files_t;

struct libewf_single_files
{
	/* The ltree data
	 */
	uint8_t *ltree_data;

	/* The ltree data size
	 */
	size_t ltree_data_size;

	/* The single file entry tree
	 */
	libewf_tree_node_t *root_file_entry_node;
};

int libewf_single_files_initialize(
     libewf_single_files_t **single_files,
     liberror_error_t **error );

int libewf_single_files_free(
     libewf_single_files_t **single_files,
     liberror_error_t **error );

int libewf_single_files_parse(
     libewf_single_files_t *single_files,
     size64_t *media_size,
     liberror_error_t **error );

int libewf_single_files_parse_file_entries(
     libewf_single_files_t *single_files,
     size64_t *media_size,
     libcstring_character_t *file_entries_string,
     size_t file_entries_string_size,
     liberror_error_t **error );

int libewf_single_files_parse_record_values(
     size64_t *media_size,
     libewf_split_values_t *lines,
     int *line_iterator,
     liberror_error_t **error );

int libewf_single_files_parse_file_entry(
     libewf_tree_node_t *parent_file_entry_node,
     libewf_split_values_t *lines,
     int *line_iterator,
     libewf_split_values_t *types,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

