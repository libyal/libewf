/*
 * libewf media values
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

#if !defined( _LIBEWF_MEDIA_VALUES_H )
#define _LIBEWF_MEDIA_VALUES_H

#include "libewf_includes.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_MEDIA_VALUES	 libewf_media_values_t
#define LIBEWF_MEDIA_VALUES_SIZE sizeof( LIBEWF_MEDIA_VALUES )

typedef struct libewf_media_values libewf_media_values_t;

/* Additional subhandle for media specific parameters
 */
struct libewf_media_values
{
	/* The media size
	 */
	size64_t media_size;

	/* The size of an individual chunk
	 */
	size32_t chunk_size;

	/* The amount of sectors per chunk
	 */
	uint32_t sectors_per_chunk;

	/* The amount of bytes per sector
	 */
	uint32_t bytes_per_sector;

        /* The amount of chunks
         */
        uint32_t amount_of_chunks;

        /* The amount of sectors
         */
        uint32_t amount_of_sectors;

        /* The amount of sectors to use as error granularity
         */
        uint32_t error_granularity;

	/* The media type
	 */
	uint8_t media_type;

	/* The media flags
	 */
	uint8_t media_flags;

        /* The GUID of the acquiry system
         */
        uint8_t guid[ 16 ];
};

LIBEWF_MEDIA_VALUES *libewf_media_values_alloc( void );

#if defined( __cplusplus )
}
#endif

#endif

