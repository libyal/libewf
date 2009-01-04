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

#ifndef _LIBEWF_DEFINIONS_H
#define _LIBEWF_DEFINIONS_H

/* The libewf version
 */
#if 0
#define LIBEWF_VERSION				L"20070512"
#else
#define LIBEWF_VERSION				"20070512"
#endif

/* The libewf file access
 */
#define LIBEWF_OPEN_READ			(uint8_t) 'r'
#define LIBEWF_OPEN_WRITE			(uint8_t) 'w'
#define LIBEWF_OPEN_READ_WRITE			(uint8_t) 'x'

/* The levels are ordered from zero tollerance to full tollerance of non fatal errors
 * fatal error cannot be tollerated
 */
#define LIBEWF_ERROR_TOLLERANCE_NONE		(uint8_t) 0x00
#define LIBEWF_ERROR_TOLLERANCE_DATA_ONLY	(uint8_t) 0x03
#define LIBEWF_ERROR_TOLLERANCE_COMPENSATE	(uint8_t) 0x0C
#define LIBEWF_ERROR_TOLLERANCE_NON_FATAL	(uint8_t) 0x0F

/* Libewf file formats
 */
#define LIBEWF_FORMAT_UNKNOWN			0x00
#define LIBEWF_FORMAT_ENCASE1			0x01
#define LIBEWF_FORMAT_ENCASE2			0x02
#define LIBEWF_FORMAT_ENCASE3			0x03
#define LIBEWF_FORMAT_ENCASE4			0x04
#define LIBEWF_FORMAT_ENCASE5			0x05
#define LIBEWF_FORMAT_ENCASE6			0x06
#define LIBEWF_FORMAT_SMART			0x0E
#define LIBEWF_FORMAT_FTK			0x0F
#define LIBEWF_FORMAT_LVF			0x10
#define LIBEWF_FORMAT_LINEN5			0x25
#define LIBEWF_FORMAT_LINEN6			0x26

/* The format as specified by Andrew Rosen
 */
#define LIBEWF_FORMAT_EWF			0x70

/* Expirimental EWF format with
 * XML based header and hash sections
 */
#define LIBEWF_FORMAT_EWFX			0x71

/* Libewf compression level definitions
 */
#define LIBEWF_COMPRESSION_NONE			0
#define LIBEWF_COMPRESSION_FAST			1
#define LIBEWF_COMPRESSION_BEST			2

/* Libewf media type definitions
 */
#define LIBEWF_MEDIA_TYPE_REMOVABLE		0x00
#define LIBEWF_MEDIA_TYPE_FIXED			0x01

/* Libewf volume type definitions
 */
#define LIBEWF_VOLUME_TYPE_LOGICAL		0x00
#define LIBEWF_VOLUME_TYPE_PHYSICAL		0x01

/* Libwef date representation format
 */
#define LIBEWF_DATE_FORMAT_DAYMONTH		0x01
#define LIBEWF_DATE_FORMAT_MONTHDAY		0x02
#define LIBEWF_DATE_FORMAT_ISO8601		0x03

/* Libewf compression types
 */
#if 0
#define LIBEWF_COMPRESSION_TYPE_NONE		L"n"
#define LIBEWF_COMPRESSION_TYPE_FAST		L"f"
#define LIBEWF_COMPRESSION_TYPE_BEST		L"b"
#else
#define LIBEWF_COMPRESSION_TYPE_NONE		"n"
#define LIBEWF_COMPRESSION_TYPE_FAST		"f"
#define LIBEWF_COMPRESSION_TYPE_BEST		"b"
#endif

/* Libewf default segment file size
 */
#define LIBEWF_DEFAULT_SEGMENT_FILE_SIZE	( 650 * 1024 * 1024 )

/* The length of hash strings
 */
#define LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5	33
#define LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1	41

#endif

