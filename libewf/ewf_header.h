/*
 * EWF header section
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

#ifndef _EWF_HEADER_H
#define _EWF_HEADER_H

#include "libewf_includes.h"

#include <stdio.h>

#include "ewf_char.h"
#include "ewf_string.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The header section
 * as long as necessary
 * character data compressed by zlib, contains a text string in the following format
 * ( spaces added for readability )
 *
 * 1                                                                                                                                \n
 * main                                                                                                                             \n
 * c           \t n               \t a                  \t e             \t t     \t m             \t u           \t p      \t r    \n
 * case number \t evidence number \t unique description \t examiner name \t notes \t acquired date \t system date \t pwhash \t char \n
 *
 * case number, evidence number, unique description, examiner name, and notes are free form strings (except for \t and \n)
 *
 * acquired date, and system date are in the form "2002 3 4 10 19 59", which is March 4, 2002 10:19:59
 *
 * pwhash the password hash should be the character '0' for no password
 *
 * char contains one of the following letters
 * b => best compression
 * f => fastest compression
 * n => no compression
 */

/* Header definition found in FTK Imager 2.3
 * A fifth line is present which is empty
 *
 * 1                                                                                                                                                       \n
 * main                                                                                                                                                    \n
 * c           \t n               \t a                  \t e             \t t     \t av      \t ov       \t m             \t u           \t p      \t r    \n
 * case number \t evidence number \t unique description \t examiner name \t notes \t version \t platform \t acquired date \t system date \t pwhash \t char \n
 *                                                                                                                                                         \n
 *
 * case number, evidence number, unique description, examiner name, and notes are free form strings (except for \t and \n)
 *
 * acquired date, and system date are in the form "2002 3 4 10 19 59", which is March 4, 2002 10:19:59
 *
 * version is the Encase version used to acquire the image
 *
 * platform is the operating system used to acquire the image
 *
 * pwhash the password hash should be the character '0' for no password
 *
 * char contains one of the following letters
 * b => best compression
 * f => fastest compression
 * n => no compression
 */

/* Header definition found in Encase 1
 * A fifth line is present which is empty
 *
 * 1                                                                                                                                \r\n
 * main                                                                                                                             \r\n
 * c           \t n               \t a                  \t e             \t t     \t m             \t u           \t p      \t r    \r\n
 * case number \t evidence number \t unique description \t examiner name \t notes \t acquired date \t system date \t pwhash \t char \r\n
 *                                                                                                                                  \r\n
 *
 * case number, evidence number, unique description, examiner name, and notes are free form strings (except for \t and \n)
 *
 * acquired date, and system date are in the form "2002 3 4 10 19 59", which is March 4, 2002 10:19:59
 *
 * pwhash the password hash should be the character '0' for no password
 *
 * char contains one of the following letters
 * b => best compression
 * f => fastest compression
 * n => no compression
 */

/* Header definition found in Encase 2, 3
 * A fifth line is present which is empty
 *
 * 1                                                                                                                                                       \r\n
 * main                                                                                                                                                    \r\n
 * c           \t n               \t a                  \t e             \t t     \t av      \t ov       \t m             \t u           \t p      \t r    \r\n
 * case number \t evidence number \t unique description \t examiner name \t notes \t version \t platform \t acquired date \t system date \t pwhash \t char \r\n
 *                                                                                                                                                         \r\n
 *
 * case number, evidence number, unique description, examiner name, and notes are free form strings (except for \t and \n)
 *
 * acquired date, and system date are in the form "2002 3 4 10 19 59", which is March 4, 2002 10:19:59
 *
 * version is the Encase version used to acquire the image
 *
 * platform is the operating system used to acquire the image
 *
 * pwhash the password hash should be the character '0' for no password
 *
 * char contains one of the following letters
 * b => best compression
 * f => fastest compression
 * n => no compression
 */

/* Header definition found in Encase 4 and 5
 * A fifth line is present which is empty
 *
 * 1                                                                                                                                               \r\n
 * main                                                                                                                                            \r\n
 * c           \t n               \t a                  \t e             \t t     \t av      \t ov       \t m             \t u           \t p      \r\n
 * case number \t evidence number \t unique description \t examiner name \t notes \t version \t platform \t acquired date \t system date \t pwhash \r\n
 *                                                                                                                                                 \r\n
 *
 * case number, evidence number, unique description, examiner name, and notes are free form strings (except for \t and \n)
 *
 * acquired date, and system date are in the form "2002 3 4 10 19 59", which is March 4, 2002 10:19:59
 *
 * version is the Encase version used to acquire the image
 *
 * platform is the operating system used to acquire the image
 *
 * pwhash the password hash should be the character '0' for no password
 *
 */

/* Header found in linen 5
 *
 * 3                                                                                                                                               \n
 * main                                                                                                                                            \n
 * a                  \t c           \t n               \t e             \t t     \t av      \t ov       \t m             \t u           \t p      \n
 * unique description \t case number \t evidence number \t examiner name \t notes \t version \t platform \t acquired date \t system date \t pwhash \n
 *                                                                                                                                                 \n
 * srce                                                                                                                                            \n
 * 0       1                                                                                                                                       \n
 * p       n       id      ev      tb      lo      po      ah      gu      aq                                                                      \n
 * 0       0                                                                                                                                       \n
 *                                         -1      -1                                                                                              \n
 *                                                                                                                                                 \n
 * sub                                                                                                                                             \n
 * 0       1                                                                                                                                       \n
 * p       n       id      nu      co      gu                                                                                                      \n
 * 0       0                                                                                                                                       \n
 *                                 1                                                                                                               \n
 *                                                                                                                                                 \n
 * unique description, case number, evidence number, examiner name, and notes are free form strings (except for \t and \n)
 *
 * acquired date, and system date are in the form unix time stamp "1142163845", which is March 12 2006, 11:44:05
 *
 * version is the Encase version used to acquire the image
 *
 * platform is the operating system used to acquire the image
 *
 * pwhash the password hash should be empty for no password
 *
 * TODO the remaining values are currently unknown
 */

/* Header found in linen 6
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

#define EWF_HEADER EWF_CHAR
#define EWF_HEADER_SIZE EWF_CHAR_SIZE

#define ewf_header_uncompress( header, length )	\
	ewf_string_uncompress( header, length )
#define ewf_header_compress( header, length, compression_level ) \
	ewf_string_compress( header, length, compression_level )
#define ewf_header_length( header ) \
	ewf_string_length( header )
#define ewf_header_read( file_descriptor, length ) \
	ewf_string_read_compressed( file_descriptor, length )
#define ewf_header_write( header, file_descriptor, length ) \
	ewf_string_write_from_buffer( header, file_descriptor, length )

#ifdef __cplusplus
}
#endif

#endif

