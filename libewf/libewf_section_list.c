/*
 * libewf section list
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "libewf_includes.h"
#include "libewf_common.h"
#include "libewf_notify.h"
#include "libewf_section_list.h"

/* Append an entry to the section list
 * Returns a pointer to the instance, NULL on error
 */
LIBEWF_SECTION_LIST *libewf_section_list_append( LIBEWF_SECTION_LIST *section_list, uint8_t *type, off64_t start_offset, off64_t end_offset )
{
	LIBEWF_SECTION_LIST_ENTRY *section_list_entry = NULL;
	static char *function                         = "libewf_section_list_append";

	if( section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid section list.\n",
		 function );

		return( NULL );
	}
	if( type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid type.\n",
		 function );

		return( NULL );
	}
	section_list_entry = (LIBEWF_SECTION_LIST_ENTRY *) libewf_common_alloc( LIBEWF_SECTION_LIST_ENTRY_SIZE );

	if( section_list_entry == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create section list entry.\n",
		 function );

		return( NULL );
	}
	if( libewf_common_memcpy( section_list_entry->type, type, 16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set section list entry type.\n",
		 function );

		libewf_common_free( section_list_entry );

		return( NULL );
	}
	section_list_entry->start_offset = start_offset;
	section_list_entry->end_offset   = end_offset;
	section_list_entry->next         = NULL;

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

