/*
 * ewfglob
 * Globbing functions for the libewf tools
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

#ifndef _EWFGLOB_H
#define _EWFGLOB_H

#include "../libewf/libewf_includes.h"

#include "ewfcommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAVE_GLOB_H

#define EWFGLOB ewfglob_t
#define EWFGLOB_SIZE sizeof( EWFGLOB )
#define EWFGLOB_RESULT_SIZE sizeof( CHAR_T* )

typedef struct ewfglob ewfglob_t;

struct ewfglob
{
	/* The amount of globs resolved
	 */
	uint16_t amount;

	/* The resolved globs
	 */
	CHAR_T **results;
};

EWFGLOB *ewfglob_alloc( void );
EWFGLOB *ewfglob_realloc( EWFGLOB *glob, uint16_t new_amount );
void ewfglob_free( EWFGLOB *glob );
int32_t ewfglob_resolve( EWFGLOB *glob, CHAR_T * const patterns[], uint32_t amount_of_patterns );

#endif

#ifdef __cplusplus
}
#endif

#endif

