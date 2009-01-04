/*
 * The representation of a chunk
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

#ifndef _EWF_CHUNK_H
#define _EWF_CHUNK_H

#include "libewf_includes.h"

#include "ewf_char.h"
#include "ewf_compress.h"
#include "ewf_string.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EWF_CHUNK EWF_CHAR
#define EWF_CHUNK_SIZE EWF_CHAR_SIZE

#define ewf_chunk_read( chunk, file_descriptor, length ) \
	ewf_string_read_to_buffer( chunk, file_descriptor, length )
#define ewf_chunk_write( chunk, file_descriptor, length ) \
	ewf_string_write_from_buffer( chunk, file_descriptor, length )

#define ewf_chunk_compress( compressed_chunk, compressed_size, uncompressed_chunk, uncompressed_size, compression_level ) \
	ewf_compress ( compressed_chunk, compressed_size, uncompressed_chunk, uncompressed_size, compression_level )
#define ewf_chunk_uncompress( uncompressed_chunk, uncompressed_size, compressed_chunk, compressed_size ) \
	ewf_uncompress ( uncompressed_chunk, uncompressed_size, compressed_chunk, compressed_size )

#ifdef __cplusplus
}
#endif

#endif

