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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyewf.h"
#include "pyewf_file_entries.h"
#include "pyewf_file_entry.h"
#include "pyewf_libcerror.h"
#include "pyewf_libewf.h"
#include "pyewf_python.h"

PySequenceMethods pyewf_file_entries_sequence_methods = {
	/* sq_length */
	(lenfunc) pyewf_file_entries_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyewf_file_entries_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pyewf_file_entries_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyewf._file_entries",
	/* tp_basicsize */
	sizeof( pyewf_file_entries_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyewf_file_entries_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	&pyewf_file_entries_sequence_methods,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
        /* tp_flags */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"internal pyewf file entries sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyewf_file_entries_iter,
	/* tp_iternext */
	(iternextfunc) pyewf_file_entries_iternext,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyewf_file_entries_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new file entries object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entries_new(
           pyewf_file_entry_t *file_entry_object,
           PyObject* (*get_sub_file_entry_by_index)(
                        pyewf_file_entry_t *file_entry_object,
                        int sub_file_entry_index ),
           int number_of_sub_file_entries )
{
	pyewf_file_entries_t *pyewf_file_entries = NULL;
	static char *function                    = "pyewf_file_entries_new";

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file enry object.",
		 function );

		return( NULL );
	}
	if( get_sub_file_entry_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get sub file entry by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the file entries values are initialized
	 */
	pyewf_file_entries = PyObject_New(
	                      struct pyewf_file_entries,
	                      &pyewf_file_entries_type_object );

	if( pyewf_file_entries == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file entries.",
		 function );

		goto on_error;
	}
	if( pyewf_file_entries_init(
	     pyewf_file_entries ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file entries.",
		 function );

		goto on_error;
	}
	pyewf_file_entries->file_entry_object           = file_entry_object;
	pyewf_file_entries->get_sub_file_entry_by_index = get_sub_file_entry_by_index;
	pyewf_file_entries->number_of_sub_file_entries  = number_of_sub_file_entries;

	Py_IncRef(
	 (PyObject *) pyewf_file_entries->file_entry_object );

	return( (PyObject *) pyewf_file_entries );

on_error:
	if( pyewf_file_entries != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyewf_file_entries );
	}
	return( NULL );
}

/* Intializes a file entries object
 * Returns 0 if successful or -1 on error
 */
int pyewf_file_entries_init(
     pyewf_file_entries_t *pyewf_file_entries )
{
	static char *function = "pyewf_file_entries_init";

	if( pyewf_file_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries.",
		 function );

		return( -1 );
	}
	/* Make sure the file entries values are initialized
	 */
	pyewf_file_entries->file_entry_object           = NULL;
	pyewf_file_entries->get_sub_file_entry_by_index = NULL;
	pyewf_file_entries->sub_file_entry_index        = 0;
	pyewf_file_entries->number_of_sub_file_entries = 0;

	return( 0 );
}

/* Frees a file entries object
 */
void pyewf_file_entries_free(
      pyewf_file_entries_t *pyewf_file_entries )
{
	static char *function = "pyewf_file_entries_free";

	if( pyewf_file_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries.",
		 function );

		return;
	}
	if( pyewf_file_entries->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries - missing ob_type.",
		 function );

		return;
	}
	if( pyewf_file_entries->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyewf_file_entries->file_entry_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyewf_file_entries->file_entry_object );
	}
	pyewf_file_entries->ob_type->tp_free(
	 (PyObject*) pyewf_file_entries );
}

/* The file entries len() function
 */
Py_ssize_t pyewf_file_entries_len(
            pyewf_file_entries_t *pyewf_file_entries )
{
	static char *function = "pyewf_file_entries_len";

	if( pyewf_file_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pyewf_file_entries->number_of_sub_file_entries );
}

/* The file entries getitem() function
 */
PyObject *pyewf_file_entries_getitem(
           pyewf_file_entries_t *pyewf_file_entries,
           Py_ssize_t item_index )
{
	PyObject *file_entry_object = NULL;
	static char *function       = "pyewf_file_entries_getitem";

	if( pyewf_file_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries.",
		 function );

		return( NULL );
	}
	if( pyewf_file_entries->get_sub_file_entry_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries - missing get sub file entry by index function.",
		 function );

		return( NULL );
	}
	if( pyewf_file_entries->number_of_sub_file_entries < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries - invalid number of sub file entries.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pyewf_file_entries->number_of_sub_file_entries ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	file_entry_object = pyewf_file_entries->get_sub_file_entry_by_index(
	                     pyewf_file_entries->file_entry_object,
	                     (int) item_index );

	return( file_entry_object );
}

/* The file entries iter() function
 */
PyObject *pyewf_file_entries_iter(
           pyewf_file_entries_t *pyewf_file_entries )
{
	static char *function = "pyewf_file_entries_iter";

	if( pyewf_file_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pyewf_file_entries );

	return( (PyObject *) pyewf_file_entries );
}

/* The file entries iternext() function
 */
PyObject *pyewf_file_entries_iternext(
           pyewf_file_entries_t *pyewf_file_entries )
{
	PyObject *file_entry_object = NULL;
	static char *function       = "pyewf_file_entries_iternext";

	if( pyewf_file_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries.",
		 function );

		return( NULL );
	}
	if( pyewf_file_entries->get_sub_file_entry_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries - missing get sub file entry by index function.",
		 function );

		return( NULL );
	}
	if( pyewf_file_entries->sub_file_entry_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries - invalid sub file entry index.",
		 function );

		return( NULL );
	}
	if( pyewf_file_entries->number_of_sub_file_entries < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entries - invalid number of sub file entries.",
		 function );

		return( NULL );
	}
	if( pyewf_file_entries->sub_file_entry_index >= pyewf_file_entries->number_of_sub_file_entries )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	file_entry_object = pyewf_file_entries->get_sub_file_entry_by_index(
	                     pyewf_file_entries->file_entry_object,
	                     pyewf_file_entries->sub_file_entry_index );

	if( file_entry_object != NULL )
	{
		pyewf_file_entries->sub_file_entry_index++;
	}
	return( file_entry_object );
}

