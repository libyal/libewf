/*
 * libewf segment table
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

#if !defined( _LIBEWF_SEGMENT_TABLE_H )
#define _LIBEWF_SEGMENT_TABLE_H

#include "libewf_includes.h"
#include "libewf_char.h"

#include "libewf_filename.h"
#include "libewf_section_list.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_SEGMENT_FILE		libewf_segment_file_t
#define LIBEWF_SEGMENT_FILE_SIZE	sizeof( LIBEWF_SEGMENT_FILE )

typedef struct libewf_segment_file libewf_segment_file_t;

struct libewf_segment_file
{
	/* The filename
	 */
	LIBEWF_FILENAME *filename;

	/* The filename length
	 */
	size_t length_filename;

	/* The file descriptor
	 */
	int file_descriptor;

	/* The file offset
	 */
	off64_t file_offset;

	/* The amount of chunks
	 */
	uint32_t amount_of_chunks;

        /* The list of all the sections
         */
        LIBEWF_SECTION_LIST *section_list;
};

#define LIBEWF_SEGMENT_TABLE		libewf_segment_table_t
#define LIBEWF_SEGMENT_TABLE_SIZE	sizeof( LIBEWF_SEGMENT_TABLE )

typedef struct libewf_segment_table libewf_segment_table_t;

struct libewf_segment_table
{
	/* The amount of segments in the table
	 */
	uint16_t amount;

	/* A dynamic array containting references to segment files
	 */
	LIBEWF_SEGMENT_FILE **segment_file;
};

LIBEWF_SEGMENT_TABLE *libewf_segment_table_alloc( uint16_t amount );
int libewf_segment_table_realloc( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t amount );
void libewf_segment_table_free( LIBEWF_SEGMENT_TABLE *segment_table );

#if defined( __cplusplus )
}
#endif

#endif

