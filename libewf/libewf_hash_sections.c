/*
 * libewf hash sections
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

#include "libewf_includes.h"

#include "libewf_common.h"
#include "libewf_hash_sections.h"
#include "libewf_notify.h"

/* Allocates memory for a new hash sections struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HASH_SECTIONS *libewf_hash_sections_alloc( void )
{
	LIBEWF_HASH_SECTIONS *hash_sections = NULL;
	static char *function                   = "libewf_hash_sections_alloc";

	hash_sections = (LIBEWF_HASH_SECTIONS *) libewf_common_alloc( LIBEWF_HASH_SECTIONS_SIZE );

	if( hash_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate hash sections.\n",
		 function );

		return( NULL );
	}
	if( libewf_common_memset(
	     hash_sections,
	     0,
	     LIBEWF_HASH_SECTIONS_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear hash sections.\n",
		 function );

		libewf_common_free( hash_sections );

		return( NULL );
	}
	return( hash_sections );
}

/* Frees memory of a hash sections struct including elements
 */
void libewf_hash_sections_free( LIBEWF_HASH_SECTIONS *hash_sections )
{
        static char *function = "libewf_hash_sections_free";

	if( hash_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash sections.\n",
		 function );

		return;
	}
	if( hash_sections->xhash != NULL )
	{
		libewf_common_free( hash_sections->xhash );
	}
	libewf_common_free( hash_sections );
}

