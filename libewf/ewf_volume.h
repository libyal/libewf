/*
 * EWF volume section (EWF-E01 EWF-L01)
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

#ifndef _EWF_VOLUME_H
#define _EWF_VOLUME_H

#include "libewf_includes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EWF_VOLUME ewf_volume_t
#define EWF_VOLUME_SIZE sizeof( EWF_VOLUME )

typedef struct ewf_volume ewf_volume_t;

struct ewf_volume
{
	/* The media type
	 * consists of 1 byte
	 * EnCase uses 0x00 for remobable media
	 *             0x01 for fixed media
	 *             0x0e for LVF
	 * FTK Imager always uses 0x01
	 */
	uint8_t media_type;

	/* Unknown
	 * consists of 3 bytes
	 * contains 0x00
	 */
	uint8_t unknown1[ 3 ];

	/* The amount of chunks
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t amount_of_chunks[ 4 ];

	/* The amount of sectors per chunks
	 * consists of 4 bytes (32 bits)
	 * value should be 64
	 */
	uint8_t sectors_per_chunk[ 4 ];

	/* The amount of bytes per chunks
	 * consists of 4 bytes (32 bits)
	 * value should be 512
	 */
	uint8_t bytes_per_sector[ 4 ];

	/* The amount of sectors
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t amount_of_sectors[ 4 ];

	/* Unknown
	 * consists of 16 bytes
	 * contains 0x00
	 */
	uint8_t unknown2[ 16 ];

	/* Media flags
	 * consists of 1 byte
	 * byte 2 is physical (1) or non physical (0)
	 */
	uint8_t media_flags;

	/* Unknown
	 * consists of 3 bytes
	 * contains 0x00
	 */
	uint8_t unknown3[ 3 ];

	/* Unknown
	 * consists of 12 bytes
	 * contains 0x00
	 */
	uint8_t unknown4[ 12 ];

	/* Compression level (Encase 5 only)
	 * consists of 1 byte
	 * value is 0x00 for no compression, 0x01 for fast/good compression, 0x02 for best compression
	 */
	uint8_t compression_level;

	/* Unknown
	 * consists of 3 bytes
	 * contains 0x00
	 */
	uint8_t unknown5[ 3 ];

	/* The amount sectors to use for error granularity
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t error_granularity[ 4 ];

	/* Unknown
	 * consists of 4 bytes
	 * contains 0x00
	 */
	uint8_t unknown6[ 4 ];

	/* The GUID (Encase 5 only)
	 * consists of 16 bytes
	 */
	uint8_t guid[ 16 ];

	/* Unknown
	 * consists of 963 bytes
	 * contains 0x00
	 */
	uint8_t unknown7[ 963 ];

	/* Reserved (signature)
	 * consists of 5 bytes
	 */
	uint8_t signature[ 5 ];

	/* The section crc of all (previous) volume data
	 * consits of 4 bytes (32 bits)
	 * starts with offset 76
	 */
	uint8_t crc[ 4 ];
};

ssize_t ewf_volume_read( EWF_VOLUME *volume, int file_descriptor );
ssize_t ewf_volume_write( EWF_VOLUME *volume, int file_descriptor );
int32_t ewf_volume_calculate_chunk_size( EWF_VOLUME *volume );

#ifdef __cplusplus
}
#endif

#endif

