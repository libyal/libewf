/*
 * The internal type definitions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEWF_INTERNAL_TYPES_H )
#define _LIBEWF_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBEWF for local use of libewf
 * The definitions in <libewf/types.h> are copied here
 * for local use of libewf
 */
#if defined( HAVE_LOCAL_LIBEWF )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libewf_access_control_entry {}	libewf_access_control_entry_t;
typedef struct libewf_attribute {}		libewf_attribute_t;
typedef struct libewf_data_chunk {}		libewf_data_chunk_t;
typedef struct libewf_file_entry {}		libewf_file_entry_t;
typedef struct libewf_handle {}			libewf_handle_t;
typedef struct libewf_source {}			libewf_source_t;
typedef struct libewf_subject {}		libewf_subject_t;

#else
typedef intptr_t libewf_access_control_entry_t;
typedef intptr_t libewf_attribute_t;
typedef intptr_t libewf_data_chunk_t;
typedef intptr_t libewf_file_entry_t;
typedef intptr_t libewf_handle_t;
typedef intptr_t libewf_source_t;
typedef intptr_t libewf_subject_t;

#endif /* defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI ) */

#endif /* defined( HAVE_LOCAL_LIBEWF ) */

/* The largest primary (or scalar) available
 * supported by a single load and store instruction
 */
typedef unsigned long int libewf_aligned_t;

#endif /* !defined( _LIBEWF_INTERNAL_TYPES_H ) */

