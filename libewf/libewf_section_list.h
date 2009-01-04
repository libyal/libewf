/*
 * Section list functions
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

#if !defined( _LIBEWF_SECTION_LIST_H )
#define _LIBEWF_SECTION_LIST_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_section_list_entry libewf_section_list_entry_t;
typedef struct libewf_section_list libewf_section_list_t;

struct libewf_section_list_entry
{
        /* The section type string
         * consists of 16 bytes at the most
         */
        uint8_t type[ 16 ];

	/* The section type string length
	 */
	size_t type_length;

	/* The start offset of the section
	 */
	off64_t start_offset;

	/* The end offset of the section
	 */
	off64_t end_offset;

	/* The next section list entry
	 */
	libewf_section_list_entry_t *next;

	/* The previous section list entry
	 */
	libewf_section_list_entry_t *previous;
};

struct libewf_section_list
{
	/* The first entry in the list
	 */
	libewf_section_list_entry_t *first;

	/* The last entry in the list
	 */
	libewf_section_list_entry_t *last;
};

libewf_section_list_t *libewf_section_list_append(
                        libewf_section_list_t *section_list,
                        uint8_t *type,
                        size_t type_length,
                        off64_t start_offset,
                        off64_t end_offset );

int libewf_section_list_remove_last(
     libewf_section_list_t *section_list );

#if defined( __cplusplus )
}
#endif

#endif

