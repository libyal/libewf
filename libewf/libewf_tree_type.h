/*
 * Tree type functions
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBEWF_TREE_TYPE_H )
#define _LIBEWF_TREE_TYPE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_list_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The tree node comparison definitions
 */
enum LIBEWF_TREE_NODE_COMPARE_DEFINITIONS
{
	/* The first value is less than the second value
	 */
        LIBEWF_TREE_NODE_COMPARE_LESS,

	/* The first and second values are equal
	 */
        LIBEWF_TREE_NODE_COMPARE_EQUAL,

	/* The first value is greater than the second value
	 */
        LIBEWF_TREE_NODE_COMPARE_GREATER
};

/* The tree node insert flag definitions
 */
enum LIBEWF_TREE_NODE_INSERT_FLAGS
{
	/* Allow duplicate entries
	 */
	LIBEWF_TREE_NODE_INSERT_FLAG_NON_UNIQUE_ENTRIES		= 0x00,

	/* Only allow unique entries, no duplicates
	 */
	LIBEWF_TREE_NODE_INSERT_FLAG_UNIQUE_ENTRIES		= 0x01,
};

typedef struct libewf_tree_node libewf_tree_node_t;

struct libewf_tree_node
{
	/* The parent node
	 */
	libewf_tree_node_t *parent_node;

	/* The previous node list
	 */
	libewf_tree_node_t *previous_node;

	/* The next node list
	 */
	libewf_tree_node_t *next_node;

	/* The first sub node
	 */
	libewf_tree_node_t *first_sub_node;

	/* The last sub node
	 */
	libewf_tree_node_t *last_sub_node;

	/* The number of sub nodes
	 */
	int number_of_sub_nodes;

	/* The node value
	 */
	intptr_t *value;
};

int libewf_tree_node_initialize(
     libewf_tree_node_t **node,
     liberror_error_t **error );

int libewf_tree_node_free(
     libewf_tree_node_t **node,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error );

int libewf_tree_node_clone(
     libewf_tree_node_t **destination_node,
     libewf_tree_node_t *source_node,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error );

int libewf_tree_node_get_value(
     libewf_tree_node_t *node,
     intptr_t **value,
     liberror_error_t **error );

int libewf_tree_node_set_value(
     libewf_tree_node_t *node,
     intptr_t *value,
     liberror_error_t **error );

int libewf_tree_node_append_node(
     libewf_tree_node_t *parent_node,
     libewf_tree_node_t *node,
     liberror_error_t **error );

int libewf_tree_node_append_value(
     libewf_tree_node_t *parent_node,
     intptr_t *value,
     liberror_error_t **error );

int libewf_tree_node_insert_node(
     libewf_tree_node_t *parent_node,
     libewf_tree_node_t *node,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     uint8_t insert_flags,
     liberror_error_t **error );

int libewf_tree_node_insert_value(
     libewf_tree_node_t *parent_node,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     uint8_t insert_flags,
     liberror_error_t **error );

int libewf_tree_node_remove_node(
     libewf_tree_node_t *parent_node,
     libewf_tree_node_t *node,
     liberror_error_t **error );

int libewf_tree_node_get_number_of_sub_nodes(
     libewf_tree_node_t *node,
     int *number_of_sub_nodes,
     liberror_error_t **error );

int libewf_tree_node_get_sub_node_by_index(
     libewf_tree_node_t *node,
     int sub_node_index,
     libewf_tree_node_t **sub_node,
     liberror_error_t **error );

int libewf_tree_node_get_leaf_node_list(
     libewf_tree_node_t *node,
     libewf_list_t **leaf_node_list,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

