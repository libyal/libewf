/*
 * libewf offset table
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

#if !defined( _LIBEWF_OFFSET_TABLE_H )
#define _LIBEWF_OFFSET_TABLE_H

#include "libewf_includes.h"

#include "libewf_chunk_offset.h"
#include "libewf_section_list.h"
#include "libewf_segment_file_handle.h"

#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_offset_table libewf_offset_table_t;

struct libewf_offset_table
{
	/* Stores the amount of chunks in the table
	 * There is an offset per chunk in the table
	 */
	uint32_t amount;

	/* The last chunk that was defined
	 */
	uint32_t last;

	/* Dynamic array of chunk offsets
	 */
	libewf_chunk_offset_t *chunk_offset;
};

libewf_offset_table_t *libewf_offset_table_alloc(
                        uint32_t amount );

int libewf_offset_table_realloc(
     libewf_offset_table_t *offset_table,
     uint32_t amount );

void libewf_offset_table_free(
      libewf_offset_table_t *offset_table );

int libewf_offset_table_fill(
     libewf_offset_table_t *offset_table,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t amount_of_chunks,
     libewf_segment_file_handle_t *segment_file_handle,
     uint8_t error_tollerance );

int libewf_offset_table_calculate_last_offset(
     libewf_offset_table_t *offset_table,
     libewf_section_list_t *section_list,
     uint8_t error_tollerance );

int libewf_offset_table_compare(
     libewf_offset_table_t *offset_table1,
     libewf_offset_table_t *offset_table2 );

off64_t libewf_offset_table_seek_chunk_offset(
         libewf_offset_table_t *offset_table,
         uint32_t chunk );

#if defined( __cplusplus )
}
#endif

#endif

