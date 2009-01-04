/*
 * Definitions for libewf
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

#ifndef _EWF_DEFINIONS_H
#define _EWF_DEFINIONS_H

#include "libewf_includes.h"

/* The minimum chunk size is 32k (32 * 1024)
 * 64 * 512 (64 sectors)
 */
#define EWF_MINIMUM_CHUNK_SIZE			32768

/* The EWF-E01 and EWF-L01 volume and data section media type definitions
 */
#define EWF_MEDIA_TYPE_REMOVABLE		0x00
#define EWF_MEDIA_TYPE_FIXED			0x01

/* The EWF-E01 media flags definitions
 */
#define EWF_MEDIA_FLAGS_IS_PHYSICAL		0x02

/* The possible EWF file formats
 */
#define EWF_FORMAT_UNKNOWN			(uint8_t) 'u'
#define EWF_FORMAT_E01				(uint8_t) 'e'
#define EWF_FORMAT_L01				(uint8_t) 'l'
#define EWF_FORMAT_S01				(uint8_t) 's'

/* The EWF compression levels
 */
#define EWF_COMPRESSION_UNKNOWN 		-2
#define EWF_COMPRESSION_DEFAULT			-1
#define EWF_COMPRESSION_NONE			 0
#define EWF_COMPRESSION_FAST			 1
#define EWF_COMPRESSION_BEST			 2

/* The EWF offset masks
 */
#define EWF_OFFSET_COMPRESSED_READ_MASK 	0x7fffffff
#define EWF_OFFSET_COMPRESSED_WRITE_MASK 	0x80000000

/* EWF-S01 maximum amount of offsets in a table section
 */
#define EWF_MAXIMUM_OFFSETS_IN_TABLE		16375

#endif

