/*
 * EWF volume section specification for the SMART format and the
 * original EWF format
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * This code is derrived from information and software contributed by
 * - Expert Witness Compression Format specification by Andrew Rosen
 *   (http://www.arsdata.com/SMART/whitepaper.html)
 * - libevf from PyFlag by Michael Cohen
 *   (http://pyflag.sourceforge.net/)
 * - Open SSL for the implementation of the MD5 hash algorithm
 * - Wietse Venema for error handling code
 *
 * Additional credits go to
 * - Robert Jan Mora for testing and other contribution
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
 *   must acknowledge the contribution by people stated above.
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

#ifndef _EWFVOLUME_SMART_H
#define _EWFVOLUME_SMART_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ewf_volume_smart EWF_VOLUME_SMART;

struct ewf_volume_smart
{
	/* Unknown
	 * consists of 4 bytes (32 bits)
	 * contains 0x00
	 */
	uint8_t unknown1[4];

	/* The amount of chunks
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t chunk_count[4];

	/* The amount of sectors per chunks
	 * consists of 4 bytes (32 bits)
	 * value should be 64
	 */
	uint8_t sectors_per_chunk[4];

	/* The amount of bytes per chunks
	 * consists of 4 bytes (32 bits)
	 * value should be 512
	 */
	uint8_t bytes_per_sector[4];

	/* The amount of sectors
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t sector_count[4];

	/* Unknown - reserved
	 * consists of 20 bytes
	 * contains 0x00
	 */
	uint8_t unknown2[20];

	/* Unknown - padding
	 * consists of 45 bytes
	 * contains 0x00
	 */
	uint8_t unknown3[45];

	/* Reserved (signature)
	 * consists of 5 bytes
	 */
	uint8_t signature[5];

	/* The section crc of all (previous) volume data
	 * consits of 4 bytes (32 bits)
	 * starts with offset 76
	 */
	uint8_t crc[4];

} __attribute__((packed));

#define EWF_VOLUME_SMART_SIZE sizeof( EWF_VOLUME_SMART )

EWF_VOLUME_SMART *ewf_volume_smart_alloc( void );
void ewf_volume_smart_free( EWF_VOLUME_SMART *volume );
EWF_VOLUME_SMART *ewf_volume_smart_read( int file_descriptor );
ssize_t ewf_volume_smart_write( EWF_VOLUME_SMART *volume, int file_descriptor );
uint32_t ewf_volume_smart_calculate_chunk_size( EWF_VOLUME_SMART *volume );

#ifdef __cplusplus
}
#endif

#endif

