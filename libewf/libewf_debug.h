/*
 * Debugging functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBEWF_DEBUG_H )
#define _LIBEWF_DEBUG_H

#include <common.h>
#include <character_string.h>
#include <types.h>

#include <stdio.h>

#include "ewf_section.h"
#include "ewf_ltree.h"

#if defined( __cplusplus )
extern "C" {
#endif

void libewf_debug_dump_data(
      uint8_t *data,
      size_t size );

void libewf_debug_section_print(
      ewf_section_t *section );

void libewf_debug_header_print(
      uint8_t *header,
      size_t header_size );

void libewf_debug_header2_print(
      uint8_t *header2,
      size_t header2_size );

void libewf_debug_xheader_print(
      uint8_t *xheader,
      size_t xheader_size );

#if defined( __cplusplus )
}
#endif

#endif

