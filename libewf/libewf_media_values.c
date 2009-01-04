/*
 * Media values functions
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

#include "libewf_media_values.h"

#include "ewf_definitions.h"

/* Allocates memory for a new handle media struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_media_values_t *libewf_media_values_alloc(
                        void )
{
	libewf_media_values_t *media_values = NULL;
	static char *function               = "libewf_media_values_alloc";

	media_values = (libewf_media_values_t *) memory_allocate(
	                                          sizeof( libewf_media_values_t ) );

	if( media_values == NULL )
	{
		notify_warning_printf( "%s: unable to allocate media values.\n",
		 function );

		return( NULL );
	}
	if( memory_set(
	     media_values,
	     0,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to clear media values.\n",
		 function );

		memory_free(
		 media_values );

		return( NULL );
	}
	media_values->chunk_size        = EWF_MINIMUM_CHUNK_SIZE;
	media_values->sectors_per_chunk = 64;
	media_values->bytes_per_sector  = 512;
	media_values->media_type        = 0;
	media_values->media_flags       = 0x01;

	return( media_values );
}

