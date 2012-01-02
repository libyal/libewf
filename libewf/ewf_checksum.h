/*
 * EWF Adler-32 checksum handling
 *
 * Copyright (c) 2006-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _EWF_CHECKSUM_H )
#define _EWF_CHECKSUM_H

#include <common.h>
#include <types.h>

#if defined( HAVE_ZLIB_H ) || defined( ZLIB_DLL )
#include <zlib.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#define ewf_checksum_calculate( buffer, size, previous_key ) \
	(uint32_t) adler32( (uLong) previous_key, (const Bytef *) buffer, (uInt) size );

#if defined( __cplusplus )
}
#endif

#endif

