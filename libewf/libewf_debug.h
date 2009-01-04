/*
 * libewf debug
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#ifndef _LIBEWF_DEBUG_H
#define _LIBEWF_DEBUG_H

#include "libewf_includes.h"

#include <stdio.h>

#include "libewf_char.h"
#include "libewf_internal_handle.h"

#include "ewf_section.h"
#include "ewf_header.h"
#include "ewf_header2.h"
#include "ewf_ltree.h"

#ifdef __cplusplus
extern "C" {
#endif

void libewf_debug_dump_data( uint8_t *data, size_t size );
void libewf_debug_read_section( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );

void libewf_debug_section_fprint( FILE *stream, EWF_SECTION *section );
void libewf_debug_header_string_fprint( FILE *stream, LIBEWF_CHAR *header_string );
void libewf_debug_header_fprint( FILE *stream, EWF_HEADER *header, size_t size );
void libewf_debug_header2_fprint( FILE *stream, EWF_HEADER2 *header2, size_t size );
void libewf_debug_chunk_fprint( FILE *stream, EWF_CHUNK *chunk );

#ifdef __cplusplus
}
#endif

#endif

