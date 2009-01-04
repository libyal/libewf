/*
 * File Object definition for libewf Python bindings
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
 *
 * Integration into libewf package by Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
#include <character_string.h>
#include <memory.h>
#include <string_conversion.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include <Python.h>

#include <libewf.h>

#include "pyewf_file.h"

/* Initialize a pyewf file object
 * Returns 0 if successful or -1 on error
 */
int pyewf_file_initialize(
     pyewf_file_t *pyewf_file,
     PyObject *arguments,
     PyObject *keywords )
{
	system_character_t **filenames = NULL;
	static char *keyword_list[]    = { "files", NULL };
	PyObject *sequence_object      = NULL;
	PyObject *string_object        = NULL;
	int number_of_filenames        = 0;
	int filename_iterator          = 0;

	pyewf_file->read_offset = 0;
	pyewf_file->media_size  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	      keywords,
	      "O",
	      keyword_list,
	      &sequence_object ) == 0 )
	{
		return( -1 );
	}
	if( PySequence_Check(
	     sequence_object ) == 0 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "Argument: files must be a list or tuple" );

		return( -1 );
	}
	number_of_filenames = PySequence_Size(
	                       sequence_object );

	if( ( number_of_filenames <= 0 )
	 || ( number_of_filenames > (int) UINT16_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "Invalid number of files" );

		return( -1 );
	}
	filenames = (system_character_t **) memory_allocate(
	                                     sizeof( system_character_t * ) * number_of_filenames );

	if( filenames == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "Unable to create filenames" );

		return( -1 );
	}
	if( memory_set(
	     filenames,
	     0,
	     sizeof( system_character_t * ) * number_of_filenames ) == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "Unable to clear filenames" );

		memory_free(
		 filenames );

		return( -1 );
	}
	for( filename_iterator = 0; filename_iterator < number_of_filenames; filename_iterator++ )
	{
		string_object = PySequence_GetItem(
		                 sequence_object,
		                 filename_iterator );

		filenames[ filename_iterator ] = system_string_duplicate(
		                                  PyString_AsString(
		                                   string_object ),
		                                   PyString_Size(
		                                    string_object ) );

		Py_DECREF(
		 string_object );
	}
	pyewf_file->handle = libewf_open(
	                      filenames,
	                      number_of_filenames,
	                      LIBEWF_OPEN_READ );

	memory_free(
	 filenames );

	if( pyewf_file->handle == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "libewf_open failed to open file(s)" );

		return( -1 );
	}
	libewf_get_media_size(
	 pyewf_file->handle,
	 &( pyewf_file->media_size ) );

	return( 0 );
}

/* Frees a pyewf file object
 */
void pyewf_file_free(
      pyewf_file_t *pyewf_file )
{
	pyewf_file->ob_type->tp_free(
	 (PyObject*) pyewf_file );
}

/* Closes a pyewf file object
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject* pyewf_file_close(
           pyewf_file_t *pyewf_file )
{
	if( libewf_close(pyewf_file->handle) != 0 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_close was unable to close file(s)" ) );
	}
	Py_RETURN_NONE;
}

/* Read media data from a pyewf file object
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject* pyewf_file_read(
           pyewf_file_t *pyewf_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *result_data       = NULL;
	static char *keyword_list[] = {"size", NULL};
	int read_size               = -1;
	ssize_t read_count          = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|i",
	     keyword_list,
	     &read_size ) == 0 )
	{
		return( NULL );
	}
	/* Adjust the read size if the size not given or is larger than the size of the media data
	 */
	if( ( read_size < 0 )
	 || ( pyewf_file->read_offset + read_size ) > pyewf_file->media_size )
	{
		read_size = pyewf_file->media_size - pyewf_file->read_offset;
	}
	if( read_size < 0 )
	{
		read_size = 0;
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( read_size > INT_MAX )
	{
		read_size = INT_MAX;
	}
	result_data = PyString_FromStringAndSize(
	               NULL,
	               read_size );

	read_count = libewf_read_buffer(
	              pyewf_file->handle,
	              PyString_AsString(
	               result_data),
	              read_size );

	if( read_count != (ssize_t) read_size )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_read failed to read data (requested %" PRId32 ", returned %" PRIzd ")",
		         read_size,
		         read_count ) );
	}
	pyewf_file->read_offset += read_count;

	return( result_data );
}

/* Seeks a certain offset in the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject* pyewf_file_seek_offset(
           pyewf_file_t *pyewf_file,
           PyObject *arguments,
           PyObject *keywords )
{
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments, keywords,
	      "L|i",
	      keyword_list, 
	      &offset,
	      &whence) == 0 )
	{
		return( NULL );
	}
	switch( whence )
	{
		case 0:
			pyewf_file->read_offset = offset;

			break;

		case 1:
			pyewf_file->read_offset += offset;

			break;

		case 2:
			pyewf_file->read_offset = pyewf_file->media_size + offset;

			break;

		default:
			return( PyErr_Format(
			         PyExc_ValueError,
			         "Invalid argument (whence): %d",
			         whence ) );
	}
	if( libewf_seek_offset(
	     pyewf_file->handle,
	     pyewf_file->read_offset ) < 0 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_seek_offset failed (tried to seek to %" PRIu64 " - %" PRIu64 ")",
		         pyewf_file->read_offset,
		         pyewf_file->media_size ) );
	}
	Py_RETURN_NONE;
}

/* Retrieves the current offset in the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject* pyewf_file_get_offset(
           pyewf_file_t *pyewf_file )
{
	off64_t current_offset = 0;

	current_offset = libewf_get_offset(
	                  pyewf_file->handle );

	return( PyLong_FromLongLong(
	         current_offset ) );
}

/* The following (regular) header values exist:
 *
 * case_number
 * description
 * examinier_name
 * evidence_number
 * notes
 * acquiry_date
 * system_date
 * acquiry_operating_system
 * acquiry_software_version
 * password
 * compression_type
 * model
 * serial_number
 *
 * The following hash values exist
 * 
 * MD5
 * SHA1
 *
 * The following media information is present:
 *
 * sectors per chunk
 * bytes per sector
 * amount of sectors
 * chunk size
 * error granularity
 * compression values
 * media size
 * media type
 * media flags
 * volume type
 * format
 * guid
 * md5 hash
 * segment filename
 * delta segment filename
 * amount of acquiry errors
 * acquiry error
 * amount of crc errors
 * crc error
 * amount of sessions
 * session
 * write amount of chunks
 * 
 */

/* Limit the maximum size of a header value to 128 bytes
 * The same limit ewfinfo currenlty uses
 * There is no limit according to the file format however
 * the tooling (read EnCase) uses an unknown limit smaller than 128 bytes
 */
#define PYEWF_FILE_HEADER_VALUE_LENGTH	128

/* Retrieves a header value
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_file_get_header_value(
           pyewf_file_t *pyewf_file,
           PyObject *arguments,
           PyObject *keywords)
{
	character_t header_value[ PYEWF_FILE_HEADER_VALUE_LENGTH ];

	static char *keyword_list[]  = { "identifier", NULL };
	character_t *safe_identifier = NULL;
	char *narrow_identifier      = NULL;
	char *safe_header_value      = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_CHARACTER_TYPE )
	character_t identifier[ PYEWF_FILE_HEADER_VALUE_LENGTH ];
	char narrow_header_value[ PYEWF_FILE_HEADER_VALUE_LENGTH ];

	size_t identifier_length   = 0;
	size_t header_value_length = 0;
#endif

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &narrow_identifier ) == 0 )
	{
		return( NULL );
	}
	/* This function checks if the header values already have been parsed and
	 * returns immediately, so it shouldn't hurt to call it every time.
	 */
	if( libewf_parse_header_values(
	     pyewf_file->handle,
	     LIBEWF_DATE_FORMAT_CTIME ) == -1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_parse_header_values failed to parse header values" ) );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	identifier_length = strlen(
	                     narrow_identifier );

	if( libewf_string_copy_char_to_wchar(
	     identifier,
	     narrow_identifier,
	     identifier_length ) != 1 )
	{
		return( PyErr_Format(
		         PyExc_RuntimeError,
		         "Unable to create identifier string" ) );
	}
	safe_identifier = (character_t *) identifier;
#else
	safe_identifier = (character_t *) narrow_identifier;
#endif
	result = libewf_get_header_value(
	          pyewf_file->handle,
	          safe_identifier,
	          header_value,
	          PYEWF_FILE_HEADER_VALUE_LENGTH );

	if( result == 0 )
	{
		/* Header value not present
		 */
		return( Py_None );
	}
	else if( result == -1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
	                 "libewf_get_header_value unable to retrieve header value" ) );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	header_value_length = string_length(
	                       header_value );

	if( header_value_length >= PYEWF_FILE_HEADER_VALUE_LENGTH )
	{
		return( PyErr_Format(
		         PyExc_RuntimeError,
		         "Unable to create safe header value string" ) );
	}
	if( libewf_string_copy_wchar_to_char(
	     narrow_header_value,
	     header_value,
	     ( header_value_length + 1 ) ) != 1 )
	{
		return( PyErr_Format(
		         PyExc_RuntimeError,
		         "Unable to create safe header value string" ) );
	}
	safe_header_value = narrow_header_value;
#else
	safe_header_value = (char *) header_value;
#endif
	return( PyString_FromString(
	         safe_header_value ) );
}

/* Retrieves the header values
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_file_get_header_values(
           pyewf_file_t *pyewf_file )
{
	character_t header_value[ PYEWF_FILE_HEADER_VALUE_LENGTH ];

	character_t *identifiers[] = { _CHARACTER_T_STRING( "case_number" ),
	                               _CHARACTER_T_STRING( "description" ),
	                               _CHARACTER_T_STRING( "examinier_name" ),
	                               _CHARACTER_T_STRING( "evidence_number" ),
	                               _CHARACTER_T_STRING( "notes" ),
	                               _CHARACTER_T_STRING( "acquiry_date" ),
	                               _CHARACTER_T_STRING( "system_date" ),
	                               _CHARACTER_T_STRING( "acquiry_operating_system" ),
	                               _CHARACTER_T_STRING( "acquiry_software_version" ),
	                               _CHARACTER_T_STRING( "password" ),
	                               _CHARACTER_T_STRING( "compression_type" ),
	                               _CHARACTER_T_STRING( "model" ),
	                               _CHARACTER_T_STRING( "serial_number" ),
	                               NULL  };

	PyObject *dictionary_object = NULL;
	PyObject *string_object     = NULL;
	character_t **identifier    = NULL;
	char *safe_identifier       = NULL;
	char *safe_header_value     = NULL;

#if defined( HAVE_WIDE_CHARACTER_TYPE )
	char narrow_identifier[ PYEWF_FILE_HEADER_VALUE_LENGTH ];
	char narrow_header_value[ PYEWF_FILE_HEADER_VALUE_LENGTH ];

	size_t identifier_length   = 0;
	size_t header_value_length = 0;
#endif

	/* This function checks if the header values already have been parsed and
	 * returns immediately, so it shouldn't hurt to call it every time.
	 */
	if( libewf_parse_header_values(
	     pyewf_file->handle,
	     LIBEWF_DATE_FORMAT_CTIME ) == -1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_parse_header_values failed to parse header values" ) );
	}
	dictionary_object = PyDict_New();

	for( identifier = identifiers; *identifier != NULL; identifier++ )
	{
		if( libewf_get_header_value(
		     pyewf_file->handle,
		     *identifier,
		     header_value,
		     PYEWF_FILE_HEADER_VALUE_LENGTH ) == 1 )
		{
#if defined( HAVE_WIDE_CHARACTER_TYPE )
			identifier_length = string_length(
					     *identifier );

			if( identifier_length >= PYEWF_FILE_HEADER_VALUE_LENGTH )
			{
				return( PyErr_Format(
					 PyExc_RuntimeError,
					 "Unable to create safe identifier string" ) );
			}
			if( libewf_string_copy_wchar_to_char(
			     narrow_identifier,
			     *identifier,
			     ( identifier_length + 1 ) ) != 1 )
			{
				return( PyErr_Format(
					 PyExc_RuntimeError,
					 "Unable to create safe identifier string" ) );
			}
			safe_identifier = narrow_identifier;

			header_value_length = string_length(
					       header_value );

			if( header_value_length >= PYEWF_FILE_HEADER_VALUE_LENGTH )
			{
				return( PyErr_Format(
					 PyExc_RuntimeError,
					 "Unable to create safe header value string" ) );
			}
			if( libewf_string_copy_wchar_to_char(
			     narrow_header_value,
			     header_value,
			     ( header_value_length + 1 ) ) != 1 )
			{
				return( PyErr_Format(
					 PyExc_RuntimeError,
					 "Unable to create safe header value string" ) );
			}
			safe_header_value = narrow_header_value;
#else
			safe_identifier   = (char *) *identifier;
			safe_header_value = (char *) header_value;
#endif
			string_object = PyString_FromFormat(
			                 safe_header_value );

			PyDict_SetItemString(
			 dictionary_object,
			 safe_identifier,
			 string_object );

			Py_DECREF(
			 string_object );
		}
	}
	return( dictionary_object );
}

