/*
 * Section list functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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
#include <notify.h>

#include "libewf_section_list.h"

/* Append an entry to the section list
 * Returns a pointer to the instance, NULL on error
 */
libewf_section_list_t *libewf_section_list_append(
                        libewf_section_list_t *section_list,
                        uint8_t *type,
                        size_t type_length,
                        off64_t start_offset,
                        off64_t end_offset )
{
	libewf_section_list_entry_t *section_list_entry = NULL;
	static char *function                           = "libewf_section_list_append";

	if( section_list == NULL )
	{
		notify_warning_printf( "%s: invalid section list.\n",
		 function );

		return( NULL );
	}
	if( type == NULL )
	{
		notify_warning_printf( "%s: invalid type.\n",
		 function );

		return( NULL );
	}
	if( type_length == 0 )
	{
		notify_warning_printf( "%s: invalid type length value cannot be zero.\n",
		 function );

		return( NULL );
	}
	if( type_length >= 16 )
	{
		notify_warning_printf( "%s: invalid type length value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	section_list_entry = (libewf_section_list_entry_t *) memory_allocate(
	                                                      sizeof( libewf_section_list_entry_t ) );

	if( section_list_entry == NULL )
	{
		notify_warning_printf( "%s: unable to create section list entry.\n",
		 function );

		return( NULL );
	}
	if( memory_set(
	     section_list_entry,
	     0,
	     sizeof( libewf_section_list_entry_t ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear section list entry.\n",
		 function );

		memory_free(
		 section_list_entry );

		return( NULL );
	}
	if( memory_copy(
	     section_list_entry->type,
	     type,
	     type_length ) == NULL )
	{
		notify_warning_printf( "%s: unable to set section list entry type.\n",
		 function );

		memory_free(
		 section_list_entry );

		return( NULL );
	}
	section_list_entry->type_length  = type_length;
	section_list_entry->start_offset = start_offset;
	section_list_entry->end_offset   = end_offset;
	section_list_entry->next         = NULL;
	section_list_entry->previous     = section_list->last;

	if( section_list->first == NULL )
	{
		section_list->first = section_list_entry;
	}
	if( section_list->last != NULL )
	{
		section_list->last->next = section_list_entry;
	}
	section_list->last = section_list_entry;

	return( section_list );
}

/* Removes the last entry from the section list
 * Returns 1 if successful or -1 on error
 */
int libewf_section_list_remove_last(
     libewf_section_list_t *section_list )
{
	libewf_section_list_entry_t *section_list_entry = NULL;
	static char *function                           = "libewf_section_list_remove_last";

	if( section_list == NULL )
	{
		notify_warning_printf( "%s: invalid section list.\n",
		 function );

		return( -1 );
	}
	if( section_list->last == NULL )
	{
		if( section_list->first != section_list->last )
		{
			notify_warning_printf( "%s: invalid section list - corruption in list.\n",
			 function );

			return( -1 );
		}
		notify_verbose_printf( "%s: section list is empty.\n",
		 function );
	}
	else if( section_list->last->previous == NULL )
	{
		if( section_list->first != section_list->last )
		{
			notify_warning_printf( "%s: invalid section list - corruption in list.\n",
			 function );

			return( -1 );
		}
		memory_free(
		 section_list->last );

		section_list->first = NULL;
		section_list->last  = NULL;
	}
	else
	{
		section_list_entry       = section_list->last;
		section_list->last       = section_list_entry->previous;
		section_list->last->next = NULL;

		memory_free(
		 section_list_entry );
	}
	return( 1 );
}

