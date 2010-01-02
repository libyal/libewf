/*
 * File entry functions
 *
 * Copyright (c) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBEWF_INTERNAL_FILE_ENTRY_H )
#define _LIBEWF_INTERNAL_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_extern.h"
#include "libewf_handle.h"
#include "libewf_single_file_entry.h"
#include "libewf_tree_type.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBEWF_FILE_ENTRY_FLAGS
{
	LIBEWF_FILE_ENTRY_FLAG_MANAGED_FILE_ENTRY_TREE_NODE	= 0x01,
};

typedef struct libewf_internal_file_entry libewf_internal_file_entry_t;

struct libewf_internal_file_entry
{
	/* The internal EWF handle
	 */
	libewf_internal_handle_t *internal_handle;

	/* The file entry tree node
	 */
	libewf_tree_node_t *file_entry_tree_node;

	/* The file entry flags
	 */
	uint8_t flags;

	/* The offset
	 */
	off64_t offset;
};

int libewf_file_entry_initialize(
     libewf_file_entry_t **file_entry,
     liberror_error_t **error );

LIBEWF_EXTERN int libewf_file_entry_free(
                   libewf_file_entry_t **file_entry,
                   liberror_error_t **error );

int libewf_file_entry_attach(
     libewf_internal_file_entry_t *internal_file_entry,
     libewf_internal_handle_t *internal_handle,
     libewf_tree_node_t *file_entry_tree_node,
     uint8_t flags,
     liberror_error_t **error );

int libewf_file_entry_detach(
     libewf_internal_file_entry_t *internal_file_entry,
     liberror_error_t **error );

LIBEWF_EXTERN int libewf_file_entry_get_name_size(
                   libewf_file_entry_t *file_entry,
                   size_t *name_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_file_entry_get_name(
                   libewf_file_entry_t *file_entry,
                   uint8_t *name,
                   size_t name_size,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_file_entry_get_amount_of_sub_file_entries(
                   libewf_file_entry_t *file_entry,
                   int *amount_of_sub_entries,
                   liberror_error_t **error );

LIBEWF_EXTERN int libewf_file_entry_get_sub_file_entry(
                   libewf_file_entry_t *file_entry,
                   int sub_file_entry_index,
                   libewf_file_entry_t **sub_file_entry,
                   liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_file_entry_read_buffer(
                       libewf_file_entry_t *file_entry,
                       void *buffer,
                       size_t buffer_size,
                       liberror_error_t **error );

LIBEWF_EXTERN ssize_t libewf_file_entry_read_random(
                       libewf_file_entry_t *file_entry,
                       void *buffer,
                       size_t buffer_size,
                       off64_t offset,
                       liberror_error_t **error );

LIBEWF_EXTERN off64_t libewf_file_entry_seek_offset(
                       libewf_file_entry_t *file_entry,
                       off64_t offset,
                       int whence,
                       liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

