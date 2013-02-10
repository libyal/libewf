/*
 * Python object definition of the file entries sequence and iterator
 *
 * Copyright (c) 2008-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYEWF_FILE_ENTRIES_H )
#define _PYEWF_FILE_ENTRIES_H

#include <common.h>
#include <types.h>

#include "pyewf_file_entry.h"
#include "pyewf_libewf.h"
#include "pyewf_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyewf_file_entries pyewf_file_entries_t;

struct pyewf_file_entries
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The file entry object
	 */
	pyewf_file_entry_t *file_entry_object;

	/* The get sub file entry by index callback function
	 */
	PyObject* (*get_sub_file_entry_by_index)(
	             pyewf_file_entry_t *file_entry_object,
	             int sub_file_entry_index );

	/* The (current) sub file entry index
	 */
	int sub_file_entry_index;

	/* The number of sub file entries
	 */
	int number_of_sub_file_entries;
};

extern PyTypeObject pyewf_file_entries_type_object;

PyObject *pyewf_file_entries_new(
           pyewf_file_entry_t *file_entry_object,
           PyObject* (*get_sub_file_entry_by_index)(
                        pyewf_file_entry_t *file_entry_object,
                        int sub_file_entry_index ),
           int number_of_sub_file_entries );

int pyewf_file_entries_init(
     pyewf_file_entries_t *pyewf_file_entries );

void pyewf_file_entries_free(
      pyewf_file_entries_t *pyewf_file_entries );

Py_ssize_t pyewf_file_entries_len(
            pyewf_file_entries_t *pyewf_file_entries );

PyObject *pyewf_file_entries_getitem(
           pyewf_file_entries_t *pyewf_file_entries,
           Py_ssize_t item_index );

PyObject *pyewf_file_entries_iter(
           pyewf_file_entries_t *pyewf_file_entries );

PyObject *pyewf_file_entries_iternext(
           pyewf_file_entries_t *pyewf_file_entries );

#if defined( __cplusplus )
}
#endif

#endif

