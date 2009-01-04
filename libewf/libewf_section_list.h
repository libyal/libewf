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

#ifndef _LIBEWF_SECTION_LIST_H
#define _LIBEWF_SECTION_LIST_H

#include "libewf_includes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIBEWF_SECTION_LIST_ENTRY libewf_section_list_entry_t
#define LIBEWF_SECTION_LIST_ENTRY_SIZE sizeof( LIBEWF_SECTION_LIST_ENTRY )

#define LIBEWF_SECTION_LIST libewf_section_list_t
#define LIBEWF_SECTION_LIST_SIZE sizeof( LIBEWF_SECTION_LIST )

typedef struct libewf_section_list_entry libewf_section_list_entry_t;
typedef struct libewf_section_list libewf_section_list_t;

struct libewf_section_list_entry
{
        /* The section type string
         * consists of 16 bytes
         */
        uint8_t type[ 16 ];

	/* The start offset of the section
	 */
	off_t start_offset;

	/* The end offset of the section
	 */
	off_t end_offset;

	/* The next section list entry
	 */
	LIBEWF_SECTION_LIST_ENTRY *next;
};

struct libewf_section_list
{
	/* The first entry in the list
	 */
	LIBEWF_SECTION_LIST_ENTRY *first;

	/* The last entry in the list
	 */
	LIBEWF_SECTION_LIST_ENTRY *last;
};

LIBEWF_SECTION_LIST *libewf_section_list_append( LIBEWF_SECTION_LIST *section_list, uint8_t *type, off_t start_offset, off_t end_offset );

#ifdef __cplusplus
}
#endif

#endif

