/*
 * FileEntry class of libewf .net managed wrapper
 *
 * Copyright (c) 2010-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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
#pragma managed( push, off )
#include <common.h>
#include <memory.h>

#include <stdlib.h>

#include <libcstring.h>

#include <libewf.h>
#pragma managed( pop )

#include <vcclr.h>

#include "ewf.net.h"
#include "ewf.net_file_entry.h"

#using <mscorlib.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EWF {

FileEntry::FileEntry( System::IntPtr ewf_file_entry )
{
	this->ewf_file_entry = ewf_file_entry;
}

FileEntry::FileEntry( void )
{
	libewf_file_entry *file_entry = NULL;
	System::String^ function      = "FileEntry::FileEntry";

	/* TODO */

	this->ewf_file_entry = Marshal::ReadIntPtr(
	                       (IntPtr) &file_entry );
}

FileEntry::~FileEntry( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::~FileEntry";

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_free(
	     &file_entry,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to free ewf file entry." );

		if( libewf_error_backtrace_sprint(
		     error,
		     &( ewf_error_string[ 1 ] ),
		     EWF_NET_ERROR_STRING_SIZE - 1 ) > 0 )
		{
			ewf_error_string[ 0 ] = '\n';

			error_string = System::String::Concat(
			                error_string,
			                gcnew System::String(
			                       ewf_error_string ) );
		}
		libewf_error_free(
		 &error );

		throw gcnew System::Exception(
			     error_string );
	}
}

} // namespace EWF

