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

/* External error type definition hides internal structure
 */
typedef intptr_t libewf_error_t;

/* The error domains
 */
enum LIBEWF_ERROR_DOMAINS
{
	LIBEWF_ERROR_DOMAIN_ARGUMENTS           = (int) 'a',
	LIBEWF_ERROR_DOMAIN_CONVERSION          = (int) 'c',
	LIBEWF_ERROR_DOMAIN_INPUT               = (int) 'i',
	LIBEWF_ERROR_DOMAIN_MEMORY              = (int) 'm',
	LIBEWF_ERROR_DOMAIN_OUTPUT              = (int) 'o',
};

/* The argument error codes
 */
enum LIBEWF_ARGUMENT_ERROR
{
	LIBEWF_ARGUMENT_ERROR_INVALID           = 0,
	LIBEWF_ARGUMENT_ERROR_EXCEEDS_MAXIMUM   = 1,
	LIBEWF_ARGUMENT_ERROR_TOO_SMALL         = 2,
	LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE = 3
};

/* The conversion error codes
 */
enum LIBEWF_CONVERSION_ERROR
{
	LIBEWF_CONVERSION_ERROR_INVALID_INPUT   = 0,
	LIBEWF_CONVERSION_ERROR_INVALID_OUTPUT  = 1
};

/* The memory error codes
 */
enum LIBEWF_MEMORY_ERROR
{
	LIBEWF_MEMORY_ERROR_INSUFFICIENT        = 0,
	LIBEWF_MEMORY_ERROR_SET_UNABLE          = 1
};

#endif

