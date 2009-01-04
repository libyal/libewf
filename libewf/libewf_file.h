/*
 * libewf file handling
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

#if !defined( _LIBEWF_FILE_H )
#define _LIBEWF_FILE_H

#include <common.h>
#include <character_string.h>
#include <system_string.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_EXPORT
 * before including libewf/extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_EXPORT
#endif

#include <libewf/extern.h>
#include <libewf/handle.h>

#if defined( __cplusplus )
extern "C" {
#endif

LIBEWF_EXTERN int libewf_check_file_signature(
                   const system_character_t *filename );

LIBEWF_EXTERN int libewf_glob(
                   const system_character_t *filename,
                   size_t length,
                   uint8_t format,
                   system_character_t **filenames[] );

LIBEWF_EXTERN libewf_handle_t *libewf_open(
                              system_character_t * const filenames[],
                              uint16_t amount_of_files,
                              uint8_t flags );

LIBEWF_EXTERN int libewf_close(
                   libewf_handle_t *handle );

LIBEWF_EXTERN off64_t libewf_seek_offset(
                       libewf_handle_t *handle,
                        off64_t offset );

LIBEWF_EXTERN off64_t libewf_get_offset(
                       libewf_handle_t *handle );

#if defined( __cplusplus )
}
#endif

#endif

