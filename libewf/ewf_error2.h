/*
 * EWF error2 section
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

#ifndef _EWF_ERROR2_H
#define _EWF_ERROR2_H

#include "libewf_includes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EWF_ERROR2 ewf_error2_t
#define EWF_ERROR2_SIZE sizeof( EWF_ERROR2 )

#define EWF_ERROR2_SECTOR ewf_error2_sector_t
#define EWF_ERROR2_SECTOR_SIZE sizeof( EWF_ERROR2_SECTOR )

typedef struct ewf_error2 ewf_error2_t;
typedef struct ewf_error2_sector ewf_error2_sector_t;

struct ewf_error2
{
	/* The amount of errors
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t amount_of_errors[ 4 ];

	/* Unknown
	 * consists of 512 bytes
	 * value should be 0x00
	 */
	uint8_t unknown[ 512 ];

	/* The section crc of all (previous) error2 data
	 * consits of 4 bytes
	 * starts with sector 76
	 */
	uint8_t crc[ 4 ];

	/* The sector array
	 * consits of 8 bytes per sector
	 * as long as necessary
	 */

	/* The last sector is followed by a 4 byte CRC
	 */
};

struct ewf_error2_sector
{
	/* A error2 sector
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t sector[ 4 ];

	/* The amount of sectors
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t amount_of_sectors[ 4 ];
};

ssize_t ewf_error2_read( EWF_ERROR2 *error2, int file_descriptor );
ssize_t ewf_error2_sectors_read( EWF_ERROR2_SECTOR *sectors, int file_descriptor, uint32_t amount );
ssize_t ewf_error2_write( EWF_ERROR2 *error2, int file_descriptor );
ssize_t ewf_error2_sectors_write( EWF_ERROR2_SECTOR *sectors, int file_descriptor, uint32_t amount );

#ifdef __cplusplus
}
#endif

#endif

