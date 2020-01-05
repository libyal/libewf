/*
 * Python object definition of the libewf media flags
 *
 * Copyright (C) 2008-2020, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYEWF_MEDIA_FLAGS_H )
#define _PYEWF_MEDIA_FLAGS_H

#include <common.h>
#include <types.h>

#include "pyewf_libewf.h"
#include "pyewf_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyewf_media_flags pyewf_media_flags_t;

struct pyewf_media_flags
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pyewf_media_flags_type_object;

int pyewf_media_flags_init_type(
     PyTypeObject *type_object );

PyObject *pyewf_media_flags_new(
           void );

int pyewf_media_flags_init(
     pyewf_media_flags_t *definitions_object );

void pyewf_media_flags_free(
      pyewf_media_flags_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYEWF_MEDIA_FLAGS_H ) */

