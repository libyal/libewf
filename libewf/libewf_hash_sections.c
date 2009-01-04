/*
 * Hash sections functions
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

#include "libewf_hash_sections.h"

/* Allocates memory for a new hash sections struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_hash_sections_t *libewf_hash_sections_alloc(
                         void )
{
	libewf_hash_sections_t *hash_sections = NULL;
	static char *function                 = "libewf_hash_sections_alloc";

	hash_sections = (libewf_hash_sections_t *) memory_allocate(
	                                            sizeof( libewf_hash_sections_t ) );

	if( hash_sections == NULL )
	{
		notify_warning_printf( "%s: unable to allocate hash sections.\n",
		 function );

		return( NULL );
	}
	if( memory_set(
	     hash_sections,
	     0,
	     sizeof( libewf_hash_sections_t ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear hash sections.\n",
		 function );

		memory_free(
		 hash_sections );

		return( NULL );
	}
	return( hash_sections );
}

/* Frees memory of a hash sections struct including elements
 */
void libewf_hash_sections_free(
      libewf_hash_sections_t *hash_sections )
{
        static char *function = "libewf_hash_sections_free";

	if( hash_sections == NULL )
	{
		notify_warning_printf( "%s: invalid hash sections.\n",
		 function );

		return;
	}
	if( hash_sections->xhash != NULL )
	{
		memory_free(
		 hash_sections->xhash );
	}
	memory_free(
	 hash_sections );
}

