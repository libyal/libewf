/*
 * libewf EWF eXtention delta chunk
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

#if !defined( _EWFX_DELTA_CHUNK_H )
#define _EWFX_DELTA_CHUNK_H

#include "libewf_includes.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define EWFX_DELTA_CHUNK_HEADER ewfx_delta_chunk_header_t
#define EWFX_DELTA_CHUNK_HEADER_SIZE sizeof( EWFX_DELTA_CHUNK_HEADER )

typedef struct ewfx_delta_chunk_header ewfx_delta_chunk_header_t;

struct ewfx_delta_chunk_header
{
	/* The number of the chunk
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t chunk[ 4 ];

	/* Compression value
	 * consists of 1 byte
	 * value is 0x00 if not compressed
	 * or 0x01 if compressed
	 */
	uint8_t compressed;

	/* Padding
	 * some padding reserved for future use
	 * consists of 5 bytes
	 */
	uint8_t padding[ 5 ];

	/* The section crc of all (previous) delta chunk section data
	 * consists of 4 bytes
	 * starts with offset 76
	 */
	uint8_t crc[ 4 ];

	/* The chunk data with CRC
	 */
};

#if defined( __cplusplus )
}
#endif

#endif

