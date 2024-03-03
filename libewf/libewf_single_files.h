/*
 * Single files functions
 *
 * Copyright (C) 2006-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_SINGLE_FILES_H )
#define _LIBEWF_SINGLE_FILES_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_lef_file_entry.h"
#include "libewf_lef_source.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libfdata.h"
#include "libewf_libfvalue.h"
#include "libewf_line_reader.h"
#include "libewf_permission_group.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_single_files libewf_single_files_t;

struct libewf_single_files
{
	/* The permission groups array
	 */
	libcdata_array_t *permission_groups;

	/* The sources array
	 */
	libcdata_array_t *sources;

	/* The file entry tree root node
	 */
	libcdata_tree_node_t *file_entry_tree_root_node;
};

int libewf_single_files_initialize(
     libewf_single_files_t **single_files,
     libcerror_error_t **error );

int libewf_single_files_free(
     libewf_single_files_t **single_files,
     libcerror_error_t **error );

int libewf_single_files_clone(
     libewf_single_files_t **destination_single_files,
     libewf_single_files_t *source_single_files,
     libcerror_error_t **error );

int libewf_single_files_parse_category_number_of_entries(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     int *number_of_entries,
     libcerror_error_t **error );

int libewf_single_files_parse_category_types(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     libfvalue_split_utf8_string_t **types,
     libcerror_error_t **error );

int libewf_single_files_parse_number_of_entries(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     int *number_of_entries,
     libcerror_error_t **error );

int libewf_single_files_parse_format(
     libfvalue_split_utf8_string_t *types,
     uint8_t *format,
     libcerror_error_t **error );

int libewf_single_files_parse_rec_category(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     size64_t *media_size,
     libcerror_error_t **error );

int libewf_single_files_parse_record_values(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     size64_t *media_size,
     libcerror_error_t **error );

int libewf_single_files_parse_perm_category(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     libcerror_error_t **error );

int libewf_single_files_parse_permission_group(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     libfvalue_split_utf8_string_t *types,
     libewf_permission_group_t *permission_group,
     libcerror_error_t **error );

int libewf_single_files_parse_srce_category(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     libcerror_error_t **error );

int libewf_single_files_parse_sub_category(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     libcerror_error_t **error );

int libewf_single_files_parse_entry_category(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     uint8_t *format,
     libcerror_error_t **error );

int libewf_single_files_parse_file_entry(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     libcdata_tree_node_t *parent_file_entry_node,
     libfvalue_split_utf8_string_t *types,
     libcerror_error_t **error );

int libewf_single_files_parse_file_entry_number_of_sub_entries(
     libewf_single_files_t *single_files,
     libewf_line_reader_t *line_reader,
     int *number_of_sub_entries,
     libcerror_error_t **error );

int libewf_single_files_read_data_stream(
     libewf_single_files_t *single_files,
     libfdata_stream_t *data_stream,
     libbfio_pool_t *file_io_pool,
     size64_t *media_size,
     uint8_t *format,
     libcerror_error_t **error );

int libewf_single_files_get_file_entry_tree_root_node(
     libewf_single_files_t *single_files,
     libcdata_tree_node_t **root_node,
     libcerror_error_t **error );

int libewf_single_files_get_permission_group_by_index(
     libewf_single_files_t *single_files,
     int permission_group_index,
     libewf_permission_group_t **permission_group,
     libcerror_error_t **error );

int libewf_single_files_get_source_by_index(
     libewf_single_files_t *single_files,
     int source_index,
     libewf_lef_source_t **lef_source,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_SINGLE_FILES_H ) */

