/*
 * EWF session section (EWF-E01)
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

#if !defined( _EWF_SESSION_H )
#define _EWF_SESSION_H

#include <common.h>

#include <libewf/types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_session ewf_session_t;

struct ewf_session
{
	/* Amount of sessions
	 * consists of 4 bytes
	 */
	uint8_t amount_of_sessions[ 4 ];

	/* Unknown
	 * consists of 28 bytes
	 * contains 0x00
	 */
	uint8_t unknown1[ 28 ];

	/* The section crc of all (previous) session data
	 * consists of 4 bytes (32 bits)
	 * starts with offset 76
	 */
	uint8_t crc[ 4 ];

	/* The session entry array
	 * consists of 32 bytes per sector
	 * as long as necessary
	 */

	/* The last session entry is followed by a 4 byte CRC
	 */
};

typedef struct ewf_session_entry ewf_session_entry_t;

struct ewf_session_entry
{
	/* Unknown
	 * contains 0x00
	 * consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The first sector of the session
	 * consists of 4 bytes
	 */
	uint8_t first_sector[ 4 ];

	/* Unknown
	 * consists of 24 bytes
	 * contains 0x00
	 */
	uint8_t unknown2[ 24 ];
};

#if defined( __cplusplus )
}
#endif

#endif

