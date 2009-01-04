/*
 * libewf hash sections
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

#if !defined( _LIBEWF_HASH_SECTIONS_H )
#define _LIBEWF_HASH_SECTIONS_H

#include "libewf_includes.h"

#include "libewf_values_table.h"

#include "ewf_char.h"
#include "ewf_digest_hash.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_HASH_SECTIONS	 	libewf_hash_sections_t
#define LIBEWF_HASH_SECTIONS_SIZE	sizeof( LIBEWF_HASH_SECTIONS )

typedef struct libewf_hash_sections libewf_hash_sections_t;

/* Additional subhandle for media specific parameters
 */
struct libewf_hash_sections
{
	/* The stored xhash
	 */
	EWF_CHAR *xhash;

	/* The size of the stored xhash
	 */
	size_t xhash_size;

	/* The MD5 hash of the data
	 */
	EWF_DIGEST_HASH md5_hash[ EWF_DIGEST_HASH_SIZE_MD5 ];

	/* Value to indicate if the MD5 hash was set
	 */
	uint8_t md5_hash_set;
};

LIBEWF_HASH_SECTIONS *libewf_hash_sections_alloc( void );

void libewf_hash_sections_free( LIBEWF_HASH_SECTIONS *hash_sections );

#if defined( __cplusplus )
}
#endif

#endif

