/*
 * EWF header2 section
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

#ifndef _EWF_HEADER2_H
#define _EWF_HEADER2_H

#include "libewf_includes.h"

#include "ewf_char.h"
#include "ewf_string.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The header2 section
 * as long as necessary
 * character data compressed by zlib, contains a UTF16 text string in the following format
 * ( spaces added for readability )
 *
 */

/* Header2 found in EnCase4
 *
 * 1                                                                                                                                               \n
 * main                                                                                                                                            \n
 * a                     c           \t n               \t e             \t t     \t av      \t ov       \t m             \t u           \t p      \n
 * unique description \t case number \t evidence number \t examiner name \t notes \t version \t platform \t acquired date \t system date \t pwhash \n
 *                                                                                                                                                 \n
 *
 * unique description, case number, evidence number, examiner name, and notes are free form strings (except for \t and \n)
 *
 * acquired date, and system date are in the form unix time stamp "1142163845", which is March 12 2006, 11:44:05
 *
 * version is the EnCase version used to acquire the image
 *
 * platform is the operating system used to acquire the image
 *
 * pwhash the password hash should be empty for no password
 *
 */

/* Header2 found in EnCase5
 *
 * 3                                                                                                                                                     \n
 * main                                                                                                                                                  \n
 * a                  \t c           \t n               \t e             \t t     \t av      \t ov       \t m             \t u           \t p      \t dc \n
 * unique description \t case number \t evidence number \t examiner name \t notes \t version \t platform \t acquired date \t system date \t pwhash \t ?  \n
 *                                                                                                                                                       \n
 * srce                                                                                                                                                  \n
 * 0       1                                                                                                                                             \n
 * p       n       id      ev      tb      lo      po      ah      gu      aq                                                                            \n
 * 0       0                                                                                                                                             \n
 *                                         -1      -1                                                                                                    \n
 *                                                                                                                                                       \n
 * sub                                                                                                                                                   \n
 * 0       1                                                                                                                                             \n
 * p       n       id      nu      co      gu                                                                                                            \n
 * 0       0                                                                                                                                             \n
 *                                 1                                                                                                                     \n
 *                                                                                                                                                       \n
 * unique description, case number, evidence number, examiner name, and notes are free form strings (except for \t and \n)
 *
 * acquired date, and system date are in the form unix time stamp "1142163845", which is March 12 2006, 11:44:05
 *
 * version is the EnCase version used to acquire the image
 *
 * platform is the operating system used to acquire the image
 *
 * pwhash the password hash should be empty for no password
 *
 * TODO the remaining values are currently unknown
 */

/* Header2 found in EnCase6
 *
 * 3                                                                                                                                                                               \n
 * main                                                                                                                                                                            \n
 * a                  \t c           \t n               \t e             \t t     \t md    \t sn            \t av      \t ov       \t m             \t u           \t p      \t dc \n
 * unique description \t case number \t evidence number \t examiner name \t notes \t model \t serial number \t version \t platform \t acquired date \t system date \t pwhash \t ?  \n
 *                                                                                                                                                                                 \n
 * srce                                                                                                                                                                            \n
 * 0       1                                                                                                                                                                       \n
 * p       n       id      ev      tb      lo      po      ah      gu      aq                                                                                                      \n
 * 0       0                                                                                                                                                                       \n
 *                                         -1      -1                                                                                                                              \n
 *                                                                                                                                                                                 \n
 * sub                                                                                                                                                                             \n
 * 0       1                                                                                                                                                                       \n
 * p       n       id      nu      co      gu                                                                                                                                      \n
 * 0       0                                                                                                                                                                       \n
 *                                 1                                                                                                                                               \n
 *                                                                                                                                                                                 \n
 * unique description, case number, evidence number, examiner name, notes, model, and serial number are free form strings (except for \t and \n)
 *
 * acquired date, and system date are in the form unix time stamp "1142163845", which is March 12 2006, 11:44:05
 *
 * version is the EnCase version used to acquire the image
 *
 * platform is the operating system used to acquire the image
 *
 * pwhash the password hash should be empty for no password
 *
 * TODO the remaining values are currently unknown
 */

#define EWF_HEADER2 EWF_CHAR
#define EWF_HEADER2_SIZE EWF_CHAR_SIZE

#define ewf_header2_uncompress( header2, length ) \
	ewf_string_uncompress( header2, length )
#define ewf_header2_compress( header2, length, compression_level ) \
	ewf_string_compress( header2, length, compression_level )
#define ewf_header2_length( header2 ) \
	ewf_string_length( header2 )
#define ewf_header2_read( file_descriptor, length ) \
	ewf_string_read_compressed( file_descriptor, length )
#define ewf_header2_write( header2, file_descriptor, length ) \
	ewf_string_write_from_buffer( header2, file_descriptor, length )

#ifdef __cplusplus
}
#endif

#endif

