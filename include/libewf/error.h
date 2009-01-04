/*
 * The error code definitions for libewf
 *
 * Copyright (c) 2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEWF_ERROR_H )
#define _LIBEWF_ERROR_H

/* libewf uses the same error codes as libuna
 * this allows libewf to pass libuna directly
 */

/* External error type definition hides internal structure
 */
typedef intptr_t libewf_error_t;

/* The error domains
 */
enum LIBEWF_ERROR_DOMAINS
{
	LIBEWF_ERROR_DOMAIN_ARGUMENTS                = (int) 'a',
	LIBEWF_ERROR_DOMAIN_CONVERSION               = (int) 'c',
	LIBEWF_ERROR_DOMAIN_COMPRESSION              = (int) 'C',
	LIBEWF_ERROR_DOMAIN_INPUT                    = (int) 'i',
	LIBEWF_ERROR_DOMAIN_MEMORY                   = (int) 'm',
	LIBEWF_ERROR_DOMAIN_OUTPUT                   = (int) 'o',
	LIBEWF_ERROR_DOMAIN_RUNTIME                  = (int) 'r',
};

/* The argument error codes
 */
enum LIBEWF_ARGUMENT_ERROR
{
	LIBEWF_ARGUMENT_ERROR_GENERIC                = 0,
	LIBEWF_ARGUMENT_ERROR_INVALID                = 1,
	LIBEWF_ARGUMENT_ERROR_LESS_THAN_ZERO         = 2,
	LIBEWF_ARGUMENT_ERROR_ZERO_OR_LESS           = 3,
	LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM        = 4,
	LIBEWF_ARGUMENT_ERROR_TOO_SMALL              = 5,
	LIBEWF_ARGUMENT_ERROR_TOO_LARGE              = 6,
	LIBEWF_ARGUMENT_ERROR_OUT_OF_RANGE           = 7,
	LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE      = 8,
	LIBEWF_ARGUMENT_ERROR_CONFLICTING_VALUE      = 9
};

/* The conversion error codes
 */
enum LIBEWF_CONVERSION_ERROR
{
	LIBEWF_CONVERSION_ERROR_GENERIC              = 0,
	LIBEWF_CONVERSION_ERROR_INPUT_FAILED         = 1,
	LIBEWF_CONVERSION_ERROR_OUTPUT_FAILED        = 2
};

/* The compression error codes
 */
enum LIBEWF_COMPRESSION_ERROR
{
	LIBEWF_COMPRESSION_ERROR_GENERIC             = 0,
	LIBEWF_COMPRESSION_ERROR_COMPRESS_FAILED     = 1,
	LIBEWF_COMPRESSION_ERROR_UNCOMPRESS_FAILED   = 2
};

/* The input error codes
 */
enum LIBEWF_INPUT_ERROR
{
	LIBEWF_INPUT_ERROR_GENERIC                   = 0,
	LIBEWF_INPUT_ERROR_OPEN_FAILED               = 1,
	LIBEWF_INPUT_ERROR_CLOSE_FAILED              = 2,
	LIBEWF_INPUT_ERROR_READ_FAILED               = 3,
	LIBEWF_INPUT_ERROR_SEEK_FAILED               = 4,
	LIBEWF_INPUT_ERROR_INVALID_DATA              = 5,

	LIBEWF_INPUT_ERROR_CRC_MISMATCH              = 1000,
	LIBEWF_INPUT_ERROR_MISSING_LAST_SEGMENT_FILE = 1001,
	LIBEWF_INPUT_ERROR_MISSING_LAST_SECTION      = 1002
};

/* The memory error codes
 */
enum LIBEWF_MEMORY_ERROR
{
	LIBEWF_MEMORY_ERROR_GENERIC                  = 0,
	LIBEWF_MEMORY_ERROR_INSUFFICIENT             = 1,
	LIBEWF_MEMORY_ERROR_COPY_FAILED              = 2,
	LIBEWF_MEMORY_ERROR_SET_FAILED               = 3
};

/* The runtime error codes
 */
enum LIBEWF_RUNTIME_ERROR
{
	LIBEWF_RUNTIME_ERROR_GENERIC                 = 0,
	LIBEWF_RUNTIME_ERROR_VALUE_MISSING           = 1,
	LIBEWF_RUNTIME_ERROR_VALUE_ALREADY_SET       = 2,
	LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED       = 3,
	LIBEWF_RUNTIME_ERROR_RESIZE_FAILED           = 4,
	LIBEWF_RUNTIME_ERROR_FREE_FAILED             = 5,
	LIBEWF_RUNTIME_ERROR_GET_FAILED              = 6,
	LIBEWF_RUNTIME_ERROR_SET_FAILED              = 7,
	LIBEWF_RUNTIME_ERROR_APPEND_FAILED           = 8,
	LIBEWF_RUNTIME_ERROR_REMOVE_FAILED           = 9,
	LIBEWF_RUNTIME_ERROR_VALUE_OUT_OF_RANGE      = 10,
	LIBEWF_RUNTIME_ERROR_EXCEEDS_MAXIMUM         = 11,
	LIBEWF_RUNTIME_ERROR_UNSUPPORTED_VALUE       = 12
};

/* The output error codes
 */
enum LIBEWF_OUTPUT_ERROR
{
	LIBEWF_OUTPUT_ERROR_GENERIC                  = 0,
	LIBEWF_OUTPUT_ERROR_OPEN_FAILED              = 1,
	LIBEWF_OUTPUT_ERROR_CLOSE_FAILED             = 2,
	LIBEWF_OUTPUT_ERROR_WRITE_FAILED             = 3,
	LIBEWF_OUTPUT_ERROR_SEEK_FAILED              = 4
};

#endif

