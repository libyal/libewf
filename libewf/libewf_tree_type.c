/*
 * Tree type functions
 *
 * Copyright (c) 2006-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libewf_tree_type.h"

/* Creates a tree node
 * Make sure the value node is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_initialize(
     libewf_tree_node_t **node,
     liberror_error_t **error )
{
	static char *function = "libewf_tree_node_initialize";

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( *node != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid node value already set.",
		 function );

		return( -1 );
	}
	*node = memory_allocate_structure(
	         libewf_tree_node_t );

	if( *node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *node,
	     0,
	     sizeof( libewf_tree_node_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *node != NULL )
	{
		memory_free(
		 *node );

		*node = NULL;
	}
	return( -1 );
}

/* Frees a tree node, its sub nodes
 * Uses the value_free_function to free the value
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_free(
     libewf_tree_node_t **node,
     int (*value_free_function)(
            intptr_t **value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libewf_tree_node_t *sub_node = NULL;
	static char *function        = "libewf_tree_node_free";
	int number_of_sub_nodes      = 0;
	int sub_node_iterator        = 0;
	int result                   = 1;

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( *node != NULL )
	{
		if( ( ( *node )->parent_node != NULL )
		 || ( ( *node )->previous_node != NULL )
		 || ( ( *node )->next_node != NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid node - connected to other nodes.",
			 function );

			return( -1 );
		}
		number_of_sub_nodes = ( *node )->number_of_sub_nodes;

		for( sub_node_iterator = 0;
		     sub_node_iterator < number_of_sub_nodes;
		     sub_node_iterator++ )
		{
			sub_node = ( *node )->first_sub_node;

			if( sub_node->previous_node != NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
				 "%s: corruption detected in sub node: %d.",
				 function,
				 sub_node_iterator );

				return( -1 );
			}
			( *node )->first_sub_node = sub_node->next_node;

			if( ( *node )->last_sub_node == sub_node )
			{
				( *node )->last_sub_node = sub_node->next_node;
			}
			( *node )->number_of_sub_nodes -= 1;

			if( sub_node->next_node != NULL )
			{
				sub_node->next_node->previous_node = NULL;
			}
			sub_node->parent_node = NULL;
			sub_node->next_node   = NULL;

			if( libewf_tree_node_free(
			     &sub_node,
			     value_free_function,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub node: %d.",
				 function,
				 sub_node_iterator );

				result = -1;
			}
		}
		if( ( *node )->value != NULL )
		{
			if( value_free_function != NULL )
			{
				if( value_free_function(
				     &( ( *node )->value ),
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free value.",
					function );

					result = -1;
				}
				( *node )->value = NULL;
			}
		}
		memory_free(
		 *node );

		*node = NULL;
	}
	return( result );
}

/* Clones the tree node and its sub nodes
 *
 * The values are cloned using the value_clone_function
 * On error the values are freed using the value_free_function
 *
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_clone(
     libewf_tree_node_t **destination_node,
     libewf_tree_node_t *source_node,
     int (*value_free_function)(
            intptr_t **value,
            liberror_error_t **error ),
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libewf_tree_node_t *destination_sub_node = NULL;
	libewf_tree_node_t *source_sub_node      = NULL;
	static char *function                    = "libewf_tree_node_clone";
	int sub_node_iterator                    = 0;

	if( destination_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination tree node.",
		 function );

		return( -1 );
	}
	if( *destination_node != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination tree node already set.",
		 function );

		return( -1 );
	}
	if( value_free_function == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value free function.",
		 function );

		return( -1 );
	}
	if( value_clone_function == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value clone function.",
		 function );

		return( -1 );
	}
	if( source_node == NULL )
	{
		*destination_node = NULL;

		return( 1 );
	}
	if( libewf_tree_node_initialize(
	     destination_node,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination tree node.",
		 function );

		return( -1 );
	}
	if( *destination_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination tree node.",
		 function );

		return( -1 );
	}
	if( value_clone_function(
	     &( ( *destination_node )->value ),
	     source_node->value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone tree node value.",
		 function );

		goto on_error;
	}
	/* Clone the sub nodes
	 */
	source_sub_node = source_node->first_sub_node;

	for( sub_node_iterator = 0;
	     sub_node_iterator < source_node->number_of_sub_nodes;
	     sub_node_iterator++ )
	{
		if( source_sub_node == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected in source sub node: %d.",
			 function,
			 sub_node_iterator );

			goto on_error;
		}
		if( libewf_tree_node_clone(
		     &destination_sub_node,
		     source_sub_node,
		     value_free_function,
		     value_clone_function,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to clone sub node: %d.",
			 function,
			 sub_node_iterator );

			goto on_error;
		}
		if( libewf_tree_node_append_node(
		     *destination_node,
		     destination_sub_node,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sub node: %d to destination tree node.",
			 function,
			 sub_node_iterator );

			goto on_error;
		}
		destination_sub_node = NULL;
		source_sub_node      = source_sub_node->next_node;
	}
	return( 1 );

on_error:
	if( destination_sub_node != NULL )
	{
		libewf_tree_node_free(
		 &destination_sub_node,
		 value_free_function,
		 NULL );
	}
	if( destination_node != NULL )
	{
		libewf_tree_node_free(
		 destination_node,
		 value_free_function,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the value from the tree node
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_get_value(
     libewf_tree_node_t *node,
     intptr_t **value,
     liberror_error_t **error )
{
	static char *function = "libewf_tree_node_get_value";

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	*value = node->value;

	return( 1 );
}

/* Sets the value in the tree node
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_set_value(
     libewf_tree_node_t *node,
     intptr_t *value,
     liberror_error_t **error )
{
	static char *function = "libewf_tree_node_set_value";

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	node->value = value;

	return( 1 );
}

/* Appends a tree node to the parent node
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_append_node(
     libewf_tree_node_t *parent_node,
     libewf_tree_node_t *node,
     liberror_error_t **error )
{
	static char *function = "libewf_tree_node_append_node";

	if( parent_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent node.",
		 function );

		return( -1 );
	}
	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( ( node->parent_node != NULL )
	 || ( node->previous_node != NULL )
	 || ( node->next_node != NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: node already part of another.",
		 function );

		return( -1 );
	}
	node->parent_node = parent_node;

	if( parent_node->number_of_sub_nodes == 0 )
	{
		if( parent_node->first_sub_node != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - first sub node already set.",
			 function );

			return( -1 );
		}
		if( parent_node->last_sub_node != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - last sub node already set.",
			 function );

			return( -1 );
		}
		parent_node->first_sub_node = node;
		parent_node->last_sub_node  = node;
	}
	else
	{
		if( parent_node->first_sub_node == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first sub node.",
			 function );

			return( -1 );
		}
		if( parent_node->last_sub_node == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last sub node.",
			 function );

			return( -1 );
		}
		node->previous_node                   = parent_node->last_sub_node;
		parent_node->last_sub_node->next_node = node;
		parent_node->last_sub_node            = node;
	}
	parent_node->number_of_sub_nodes += 1;

	return( 1 );
}

/* Appends a value to the parent node
 * Creates a new tree node
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_append_value(
     libewf_tree_node_t *parent_node,
     intptr_t *value,
     liberror_error_t **error )
{
	libewf_tree_node_t *tree_node = NULL;
	static char *function         = "libewf_tree_node_append_value";

	if( libewf_tree_node_initialize(
	     &tree_node,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create tree node.",
		 function );

		goto on_error;
	}
	if( libewf_tree_node_append_node(
	     parent_node,
	     tree_node,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append tree node.",
		 function );

		goto on_error;
	}
	if( libewf_tree_node_set_value(
	     tree_node,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set value in tree node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( tree_node != NULL )
	{
		libewf_tree_node_free(
		 &tree_node,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Inserts a tree node in the parent node
 *
 * Uses the entry_compare_function to determine the order of the entries
 * The entry_compare_function should return LIBEWF_TREE_NODE_COMPARE_LESS,
 * LIBEWF_TREE_NODE_COMPARE_EQUAL, LIBEWF_TREE_NODE_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate entry.
 * Only allowing unique entries can be enforced by setting the flag LIBEWF_TREE_NODE_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the node already exists or -1 on error
 */
int libewf_tree_node_insert_node(
     libewf_tree_node_t *parent_node,
     libewf_tree_node_t *node,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     uint8_t insert_flags,
     liberror_error_t **error )
{
	libewf_tree_node_t *sub_node = NULL;
	static char *function        = "libewf_tree_node_insert_node";
	int result                   = -1;
	int sub_node_iterator        = 0;

	if( parent_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent node.",
		 function );

		return( -1 );
	}
	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( ( node->parent_node != NULL )
	 || ( node->previous_node != NULL )
	 || ( node->next_node != NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: node already part of another.",
		 function );

		return( -1 );
	}
	if( value_compare_function == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value compare function.",
		 function );

		return( -1 );
	}
	if( ( insert_flags & ~( LIBEWF_TREE_NODE_INSERT_FLAG_UNIQUE_ENTRIES ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported insert flags: 0x%02" PRIx8 ".",
		 function,
		 insert_flags );

		return( -1 );
	}
	if( parent_node->number_of_sub_nodes == 0 )
	{
		if( parent_node->first_sub_node != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - first sub node already set.",
			 function );

			return( -1 );
		}
		if( parent_node->last_sub_node != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - last sub node already set.",
			 function );

			return( -1 );
		}
		parent_node->first_sub_node = node;
		parent_node->last_sub_node  = node;
	}
	else
	{
		if( parent_node->first_sub_node == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first sub node.",
			 function );

			return( -1 );
		}
		if( parent_node->last_sub_node == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last sub node.",
			 function );

			return( -1 );
		}
		sub_node = parent_node->first_sub_node;

		for( sub_node_iterator = 0;
		     sub_node_iterator < parent_node->number_of_sub_nodes;
		     sub_node_iterator++ )
		{
			result = value_compare_function(
			          node->value,
			          sub_node->value,
			          error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to compare sub node: %d.",
				 function,
				 sub_node_iterator );

				return( -1 );
			}
			else if( result == LIBEWF_TREE_NODE_COMPARE_EQUAL )
			{
				if( ( insert_flags & LIBEWF_TREE_NODE_INSERT_FLAG_UNIQUE_ENTRIES ) != 0 )
				{
					return( 0 );
				}
			}
			else if( result == LIBEWF_TREE_NODE_COMPARE_LESS )
			{
				break;
			}
			else if( result != LIBEWF_TREE_NODE_COMPARE_GREATER )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value compare function return value: %d.",
				 function,
				 result );

				return( -1 );
			}
			sub_node = sub_node->next_node;
		}
		if( result == LIBEWF_TREE_NODE_COMPARE_LESS )
		{
			node->previous_node = sub_node->previous_node;
			node->next_node     = sub_node;

			if( sub_node == parent_node->first_sub_node )
			{
				parent_node->first_sub_node = node;
			}
			else if( sub_node->previous_node == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected - missing previous node in sub node: %d.",
				 function,
				 sub_node_iterator );

				return( -1 );
			}
			else
			{
				sub_node->previous_node->next_node = node;
			}
			sub_node->previous_node = node;
		}
		else
		{
			node->previous_node                   = parent_node->last_sub_node;
			parent_node->last_sub_node->next_node = node;
			parent_node->last_sub_node            = node;
		}
	}
	node->parent_node = parent_node;

	parent_node->number_of_sub_nodes += 1;

	return( 1 );
}

/* Inserts a value in the parent node
 *
 * Creates a new tree node
 *
 * Uses the entry_compare_function to determine the order of the entries
 * The entry_compare_function should return LIBEWF_TREE_NODE_COMPARE_LESS,
 * LIBEWF_TREE_NODE_COMPARE_EQUAL, LIBEWF_TREE_NODE_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate entry.
 * Only allowing unique entries can be enforced by setting the flag LIBEWF_TREE_NODE_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the node already exists or -1 on error
 */
int libewf_tree_node_insert_value(
     libewf_tree_node_t *parent_node,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     uint8_t insert_flags,
     liberror_error_t **error )
{
	libewf_tree_node_t *tree_node = NULL;
	static char *function         = "libewf_tree_node_insert_value";
	int result                    = 0;

	if( libewf_tree_node_initialize(
	     &tree_node,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create tree node.",
		 function );

		goto on_error;
	}
	if( libewf_tree_node_set_value(
	     tree_node,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set value in tree node.",
		 function );

		goto on_error;
	}
	result = libewf_tree_node_insert_node(
	          parent_node,
	          tree_node,
	          value_compare_function,
	          insert_flags,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert node.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libewf_tree_node_free(
		     &tree_node,
		     NULL,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free tree node.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( tree_node != NULL )
	{
		libewf_tree_node_free(
		 &tree_node,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Removes a tree node from the parent node
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_remove_node(
     libewf_tree_node_t *parent_node,
     libewf_tree_node_t *node,
     liberror_error_t **error )
{
	static char *function = "libewf_tree_node_remove_node";

	if( parent_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent node.",
		 function );

		return( -1 );
	}
	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( parent_node != node->parent_node )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch in parent node.",
		 function );

		return( -1 );
	}
	if( parent_node->number_of_sub_nodes == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing number of sub nodes.",
		 function );

		return( -1 );
	}
	if( parent_node->first_sub_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: corruption detected - missing first sub node.",
		 function );

		return( -1 );
	}
	if( parent_node->last_sub_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: corruption detected - missing last sub node.",
		 function );

		return( -1 );
	}
	if( parent_node->first_sub_node == node )
	{
		parent_node->first_sub_node = node->next_node;
	}
	if( parent_node->last_sub_node == node )
	{
		parent_node->last_sub_node = node->previous_node;
	}
	if( node->next_node != NULL )
	{
		node->next_node->previous_node = node->previous_node;
	}
	if( node->previous_node != NULL )
	{
		node->previous_node->next_node = node->next_node;
	}
	node->parent_node   = NULL;
	node->previous_node = NULL;
	node->next_node     = NULL;

	parent_node->number_of_sub_nodes -= 1;

	return( 1 );
}

/* Retrieves the number of sub nodes in the tree node
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_get_number_of_sub_nodes(
     libewf_tree_node_t *node,
     int *number_of_sub_nodes,
     liberror_error_t **error )
{
	static char *function = "libewf_tree_node_get_number_of_sub_nodes";

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( number_of_sub_nodes == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub nodes.",
		 function );

		return( -1 );
	}
	*number_of_sub_nodes = node->number_of_sub_nodes;

	return( 1 );
}

/* Retrieves a specific sub node from the tree node
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_get_sub_node_by_index(
     libewf_tree_node_t *node,
     int sub_node_index,
     libewf_tree_node_t **sub_node,
     liberror_error_t **error )
{
	static char *function = "libewf_tree_node_get_sub_node_by_index";
	int result            = -1;
	int sub_node_iterator = 0;

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( ( sub_node_index < 0 )
	 || ( sub_node_index >= node->number_of_sub_nodes ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sub node index value out of bounds.",
		 function );

		return( -1 );
	}
	if( sub_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node.",
		 function );

		return( -1 );
	}
	/* Check if the sub nodes should be searched front to back
	 * or back to front
	 */
	if( sub_node_index < ( node->number_of_sub_nodes / 2 ) )
	{
		*sub_node = node->first_sub_node;

		for( sub_node_iterator = 0;
		     sub_node_iterator < node->number_of_sub_nodes;
		     sub_node_iterator++ )
		{
			if( *sub_node == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for sub node: %d.",
				 function,
				 sub_node_iterator );

				return( -1 );
			}
			if( sub_node_iterator == sub_node_index )
			{
				result = 1;

				break;
			}	
			*sub_node = ( *sub_node )->next_node;
		}
	}
	else
	{
		*sub_node = node->last_sub_node;

		for( sub_node_iterator = ( node->number_of_sub_nodes - 1 );
		     sub_node_iterator >= 0;
		     sub_node_iterator-- )
		{
			if( *sub_node == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for sub node: %d.",
				 function,
				 sub_node_iterator );

				return( -1 );
			}
			if( sub_node_iterator == sub_node_index )
			{
				result = 1;

				break;
			}	
			*sub_node = ( *sub_node )->previous_node;
		}
	}
	return( result );
}

/* Retrieves a list of all the leaf nodes
 * Returns 1 if successful or -1 on error
 */
int libewf_tree_node_get_leaf_node_list(
     libewf_tree_node_t *node,
     libewf_list_t **leaf_node_list,
     liberror_error_t **error )
{
	libewf_tree_node_t *sub_node = NULL;
	static char *function        = "libewf_tree_node_get_leaf_node_list";
	int sub_node_iterator        = 0;

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( leaf_node_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf node list.",
		 function );

		return( -1 );
	}
	if( *leaf_node_list == NULL )
	{
		if( libewf_list_initialize(
		     leaf_node_list,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create leaf node list.",
			 function );

			return( -1 );
		}
	}
	/* Traverse the sub nodes
	 */
	if( node->number_of_sub_nodes > 0 )
	{
		sub_node = node->first_sub_node;

		for( sub_node_iterator = 0;
		     sub_node_iterator < node->number_of_sub_nodes;
		     sub_node_iterator++ )
		{
			if( sub_node == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for sub node: %d.",
				 function,
				 sub_node_iterator );

				return( -1 );
			}
			if( libewf_tree_node_get_leaf_node_list(
			     sub_node,
			     leaf_node_list,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: unable to traverse sub node: %d.",
				 function,
				 sub_node_iterator );

				return( -1 );
			}
			sub_node = sub_node->next_node;
		}
	}
	else if( node->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid node - missing value.",
		 function );

		return( -1 );
	}
	else if( libewf_list_append_value(
	          *leaf_node_list,
	          node->value,
	          error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append tree node to leaf node list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

