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

#include <common.h>
#include <types.h>

#include "libewf_lef_file_entry.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libuna.h"
#include "libewf_single_file_tree.h"

/* Retrieves the file entry sub node for the specific UTF-8 formatted name
 * Returns 1 if successful, 0 in no such value or -1 on error
 */
int libewf_single_file_tree_get_sub_node_by_utf8_name(
     libcdata_tree_node_t *node,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcdata_tree_node_t **sub_node,
     libewf_lef_file_entry_t **sub_lef_file_entry,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *safe_sub_node              = NULL;
	libewf_lef_file_entry_t *safe_sub_lef_file_entry = NULL;
	static char *function                            = "libewf_single_file_tree_get_sub_node_by_utf8_name";
	int compare_result                               = LIBUNA_COMPARE_GREATER;
	int number_of_sub_nodes                          = 0;
	int result                                       = 0;
	int sub_node_index                               = 0;

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node.",
		 function );

		return( -1 );
	}
	if( sub_lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	*sub_node           = NULL;
	*sub_lef_file_entry = NULL;

	if( libcdata_tree_node_get_number_of_sub_nodes(
	     node,
	     &number_of_sub_nodes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_sub_node_by_index(
	     node,
	     0,
	     &safe_sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first sub node.",
		 function );

		return( -1 );
	}
	for( sub_node_index = 0;
	     sub_node_index < number_of_sub_nodes;
	     sub_node_index++ )
	{
		if( libcdata_tree_node_get_value(
		     safe_sub_node,
		     (intptr_t **) &safe_sub_lef_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from sub node: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( safe_sub_lef_file_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub file entry: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		compare_result = libewf_serialized_string_compare_with_utf8_string(
		                  safe_sub_lef_file_entry->name,
		                  utf8_string,
		                  utf8_string_length,
		                  error );

		if( compare_result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare name with UTF-8 string.",
			 function );

			return( -1 );
		}
		else if( compare_result == LIBUNA_COMPARE_EQUAL )
		{
			*sub_node           = safe_sub_node;
			*sub_lef_file_entry = safe_sub_lef_file_entry;

			result = 1;

			break;
		}
		if( libcdata_tree_node_get_next_node(
		     safe_sub_node,
		     &safe_sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next node from sub node: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the file entry sub node for the specific UTF-16 formatted name
 * Returns 1 if successful, 0 in no such value or -1 on error
 */
int libewf_single_file_tree_get_sub_node_by_utf16_name(
     libcdata_tree_node_t *node,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcdata_tree_node_t **sub_node,
     libewf_lef_file_entry_t **sub_lef_file_entry,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *safe_sub_node              = NULL;
	libewf_lef_file_entry_t *safe_sub_lef_file_entry = NULL;
	static char *function                            = "libewf_single_file_tree_get_sub_node_by_utf16_name";
	int compare_result                               = LIBUNA_COMPARE_GREATER;
	int number_of_sub_nodes                          = 0;
	int result                                       = 0;
	int sub_node_index                               = 0;

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node.",
		 function );

		return( -1 );
	}
	if( sub_lef_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	*sub_node           = NULL;
	*sub_lef_file_entry = NULL;

	if( libcdata_tree_node_get_number_of_sub_nodes(
	     node,
	     &number_of_sub_nodes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_sub_node_by_index(
	     node,
	     0,
	     &safe_sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first sub node.",
		 function );

		return( -1 );
	}
	for( sub_node_index = 0;
	     sub_node_index < number_of_sub_nodes;
	     sub_node_index++ )
	{
		if( libcdata_tree_node_get_value(
		     safe_sub_node,
		     (intptr_t **) &safe_sub_lef_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from sub node: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( safe_sub_lef_file_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub file entry: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		compare_result = libewf_serialized_string_compare_with_utf16_string(
		                  safe_sub_lef_file_entry->name,
		                  utf16_string,
		                  utf16_string_length,
		                  error );

		if( compare_result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string.",
			 function );

			return( -1 );
		}
		else if( compare_result == LIBUNA_COMPARE_EQUAL )
		{
			*sub_node           = safe_sub_node;
			*sub_lef_file_entry = safe_sub_lef_file_entry;

			result = 1;

			break;
		}
		if( libcdata_tree_node_get_next_node(
		     safe_sub_node,
		     &safe_sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next node from sub node: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
	}
	return( result );
}

