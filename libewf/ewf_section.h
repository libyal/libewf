/*
 * EWF section start
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

#ifndef _EWF_SECTION_H
#define _EWF_SECTION_H

#include "libewf_includes.h"

#include "ewf_char.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EWF_SECTION ewf_section_t
#define EWF_SECTION_SIZE sizeof( EWF_SECTION )

typedef struct ewf_section ewf_section_t;

struct ewf_section
{
	/* The section type string
	 * consists of 16 bytes
	 */
	uint8_t type[ 16 ];

	/* The section offset to the next section
	 * consists of 8 bytes (64 bits)
	 */
	uint8_t next[ 8 ];

	/* The section size
	 * consists of 8 bytes (64 bits)
	 */
	uint8_t size[ 8 ];

	/* The section padding
	 * consists of 40 bytes
	 */
	uint8_t padding[ 40 ];

	/* The section crc of all (previous) section data
	 * consists of 4 bytes
	 */
	uint8_t crc[ 4 ];
};

ssize_t ewf_section_read( EWF_SECTION *section, int file_descriptor );
ssize_t ewf_section_write( EWF_SECTION *section, int file_descriptor );

uint8_t ewf_section_is_type( EWF_SECTION *section, const EWF_CHAR *type );

#define ewf_section_is_type_header( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "header" )
#define ewf_section_is_type_header2( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "header2" )
#define ewf_section_is_type_volume( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "volume" )
#define ewf_section_is_type_disk( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "disk" )
#define ewf_section_is_type_table( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "table" )
#define ewf_section_is_type_table2( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "table2" )
#define ewf_section_is_type_sectors( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "sectors" )
#define ewf_section_is_type_hash( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "hash" )
#define ewf_section_is_type_done( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "done" )
#define ewf_section_is_type_next( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "next" )
#define ewf_section_is_type_data( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "data" )
#define ewf_section_is_type_error2( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "error2" )

#define ewf_section_is_type_ltree( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "ltree" )

#define ewf_section_is_type_xheader( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "xheader" )
#define ewf_section_is_type_xhash( section ) \
	ewf_section_is_type( section, (EWF_CHAR *) "xhash" )

#ifdef __cplusplus
}
#endif

#endif

