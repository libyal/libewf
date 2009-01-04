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

#ifndef _LIBEWF_SEGMENT_TABLE_H
#define _LIBEWF_SEGMENT_TABLE_H

#include "libewf_includes.h"
#include "libewf_char.h"

#include "libewf_section_list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIBEWF_SEGMENT_TABLE				libewf_segment_table_t
#define LIBEWF_SEGMENT_TABLE_SIZE			sizeof( LIBEWF_SEGMENT_TABLE )

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
#define LIBEWF_SEGMENT_TABLE_FILENAME_SIZE		sizeof( wchar_t* )
#else
#define LIBEWF_SEGMENT_TABLE_FILENAME_SIZE		sizeof( char* )
#endif

#define LIBEWF_SEGMENT_TABLE_FILE_DESCRIPTOR_SIZE	sizeof( int )
#define LIBEWF_SEGMENT_TABLE_FILE_OFFSET_SIZE		sizeof( off_t )
#define LIBEWF_SEGMENT_TABLE_AMOUNT_OF_CHUNKS_SIZE	sizeof( uint32_t )
#define LIBEWF_SEGMENT_TABLE_SECTION_LIST_SIZE		sizeof( LIBEWF_SECTION_LIST )

typedef struct libewf_segment_table libewf_segment_table_t;

struct libewf_segment_table
{
	/* The amount of segments in the table
	 * consists of 2 bytes (16 bits) containing
	 */
	uint16_t amount;

	/* A dynamic array containting the filenames
	 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	wchar_t **filename;
#else
	char **filename;
#endif

	/* A dynamic array containting the file descriptors
	 */
	int *file_descriptor;

	/* A dynamic array containting the file offsets
	 */
	off_t *file_offset;

	/* A dynamic array containting the amount of chunks per segment file
	 */
	uint32_t *amount_of_chunks;

        /* A list of all the sections within a certain segment file
         */
        LIBEWF_SECTION_LIST **section_list;
};

LIBEWF_SEGMENT_TABLE *libewf_segment_table_alloc( uint16_t amount );
LIBEWF_SEGMENT_TABLE *libewf_segment_table_realloc( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t amount );
void libewf_segment_table_free( LIBEWF_SEGMENT_TABLE *segment_table );

uint8_t libewf_segment_table_values_is_set( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
wchar_t *libewf_segment_table_get_wide_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment );
#else
char *libewf_segment_table_get_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment );
#endif

int libewf_segment_table_get_file_descriptor( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment );

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int8_t libewf_segment_table_set_wide_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment, const wchar_t *filename, size_t length_filename );
#else
int8_t libewf_segment_table_set_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment, const char *filename, size_t length_filename );
#endif

int8_t libewf_segment_table_set_file_descriptor( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment, int file_descriptor );

#ifdef __cplusplus
}
#endif

#endif

