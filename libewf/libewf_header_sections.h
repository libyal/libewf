/*
 * libewf header sections
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

#if !defined( _LIBEWF_HEADER_SECTIONS_H )
#define _LIBEWF_HEADER_SECTIONS_H

#include "libewf_includes.h"

#include "libewf_values_table.h"

#include "ewf_char.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_header_sections libewf_header_sections_t;

/* Additional subhandle for media specific parameters
 */
struct libewf_header_sections
{
	/* The stored header
	 */
	ewf_char_t *header;

	/* The size of the stored header
	 */
	size_t header_size;

	/* The stored header2
	 */
	ewf_char_t *header2;

	/* The size of the stored header2
	 */
	size_t header2_size;

	/* The stored xheader
	 */
	ewf_char_t *xheader;

	/* The size of the stored xheader
	 */
	size_t xheader_size;

	/* Value to indicate how much header sections were found
	 */
	uint8_t amount_of_header_sections;
};

libewf_header_sections_t *libewf_header_sections_alloc( void );

void libewf_header_sections_free( libewf_header_sections_t *header_sections );

int libewf_header_sections_create( libewf_header_sections_t *header_sections, libewf_values_table_t *header_values, int8_t compression_level, uint8_t format );

#if defined( __cplusplus )
}
#endif

#endif

