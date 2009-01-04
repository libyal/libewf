/*
 * libewf notification
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

#ifndef _LIBEWF_NOTIFY_H
#define _LIBEWF_NOTIFY_H

#include "libewf_includes.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_VERBOSE_OUTPUT
#define LIBEWF_VERBOSE_EXEC( code )	if( libewf_notify_verbose != 0 ) { code };

#else
#define LIBEWF_VERBOSE_EXEC( code )	/* code */

#endif

#define LIBEWF_VERBOSE_PRINT		libewf_verbose_print
#define LIBEWF_WARNING_PRINT		libewf_warning_print
#define LIBEWF_FATAL_PRINT		libewf_fatal_print

extern uint8_t libewf_notify_verbose;

void libewf_notify_set_values( FILE *stream, uint8_t verbose );

void libewf_verbose_print( char *, ... );
void libewf_warning_print( char *, ... );
void libewf_fatal_print( char *, ... );

void libewf_dump_data( uint8_t *data, size_t size );

#ifdef __cplusplus
}
#endif

#endif

