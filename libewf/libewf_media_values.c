/*
 * libewf media values
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <common.h>
#include <memory.h>

#include "libewf_common.h"
#include "libewf_media_values.h"
#include "libewf_notify.h"

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
		LIBEWF_WARNING_PRINT( "%s: unable to allocate media values.\n",
		 function );

		return( NULL );
	}
	if( memory_set(
	     media_values,
	     0,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear media values.\n",
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

