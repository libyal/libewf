/*
 * Handle class of libewf .net managed wrapper
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
#include "ewf.net_handle.h"

#using <mscorlib.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EWF {

Handle::Handle( System::IntPtr ewf_handle )
{
	this->ewf_handle = ewf_handle;
}

Handle::Handle( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error        = NULL;
	libewf_handle_t *handle      = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::Handle";

	if( libewf_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to create ewf handle." );

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
	this->ewf_handle = Marshal::ReadIntPtr(
	                      (IntPtr) &handle );
}

Handle::~Handle( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error        = NULL;
	libewf_handle_t *handle      = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::~Handle";

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->ewf_handle );

	if( libewf_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to free ewf handle." );

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

int Handle::GetAccessFlagsRead( void )
{
	return( libewf_get_access_flags_read() );
}

int Handle::GetAccessFlagsReadWrite( void )
{
	return( libewf_get_access_flags_read_write() );
}

int Handle::GetAccessFlagsWrite( void )
{
	return( libewf_get_access_flags_write() );
}

array<System::String^>^ Handle::Glob( System::String^ filename )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	array<System::String^>^ filenames   = nullptr;
	libewf_error_t *error               = NULL;
	wchar_t **ewf_filenames             = NULL;
	pin_ptr<const wchar_t> ewf_filename = nullptr;
	System::String^ error_string        = nullptr;
	System::String^ function            = "EWF::Glob";
	size_t ewf_filename_length          = 0;
	int ewf_filename_index              = 0;
	int ewf_number_of_filenames         = 0;

	ewf_filename = PtrToStringChars(
	                  filename );

	ewf_filename_length = wcslen(
	                         ewf_filename );

	if( libewf_glob_wide(
	     ewf_filename,
	     ewf_filename_length,
	     LIBEWF_FORMAT_UNKNOWN,
	     &ewf_filenames,
	     &ewf_number_of_filenames,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to glob filenames." );

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
	filenames = gcnew array<System::String^, 1>(
	                   ewf_number_of_filenames );

	for( ewf_filename_index = 0;
	     ewf_filename_index < ewf_number_of_filenames;
	     ewf_filename_index++ )
	{
		filenames[ ewf_filename_index ] = gcnew System::String(
		                                           ewf_filenames[ ewf_filename_index ] );
	}
	if( libewf_glob_wide_free(
	     ewf_filenames,
	     ewf_number_of_filenames,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to free globbed filenames." );

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
	return( filenames );
}

Handle^ Handle::Clone( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error               = NULL;
	libewf_handle_t *destination_handle = NULL;
	libewf_handle_t *source_handle      = NULL;
	System::String^ error_string        = nullptr;
	System::String^ function            = "Handle::Clone";

	Marshal::WriteIntPtr(
	 (IntPtr) &source_handle,
	 this->ewf_handle );

	if( libewf_handle_clone(
	     &destination_handle,
	     source_handle,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to clone ewf handle." );

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
	return( gcnew Handle( Marshal::ReadIntPtr(
	                       (IntPtr) &destination_handle ) ) );
}

void Handle::Open( array<System::String^>^ filenames,
                   int access_flags )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_handle_t *handle         = NULL;
	libewf_error_t *error           = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "Handle::Open";
	wchar_t **ewf_filenames         = NULL;
	pin_ptr<const wchar_t> filename = nullptr;
	int ewf_filename_index          = 0;
	int ewf_number_of_filenames     = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->ewf_handle );

	ewf_number_of_filenames = filenames->Length;

	if( ewf_number_of_filenames <= 0 )
	{
		throw gcnew System::Exception(
			"ewf.net " + function + ": missing filenames." );
	}
	ewf_filenames = (wchar_t **) memory_allocate(
	                                sizeof( wchar_t* ) * ewf_number_of_filenames );

	if( ewf_filenames == NULL )
	{
		throw gcnew System::Exception(
		             "ewf.net " + function + ": unable to create filenames." );
	}
	for( ewf_filename_index = 0;
	     ewf_filename_index < ewf_number_of_filenames;
	     ewf_filename_index++ )
	{
		filename = PtrToStringChars(
		            filenames[ ewf_filename_index ] );

		ewf_filenames[ ewf_filename_index ] = (wchar_t *) filename;
	}
	if( libewf_handle_open_wide(
	     handle,
	     (wchar_t * const *) ewf_filenames,
	     ewf_number_of_filenames,
	     access_flags,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to open ewf handle." );

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

		memory_free(
		 ewf_filenames );

		throw gcnew System::Exception(
			     error_string );
	}
	memory_free(
	 ewf_filenames );
}

void Handle::Close( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error        = NULL;
	libewf_handle_t *handle      = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::Close";

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->ewf_handle );

	if( libewf_handle_close(
	     handle,
	     &error ) != 0 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to close ewf handle." );

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

int Handle::ReadBuffer( array<System::Byte>^ buffer,
                        int size )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error        = NULL;
	libewf_handle_t *handle      = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::ReadBuffer";
	pin_ptr<uint8_t> ewf_buffer  = nullptr;
	size_t read_count            = 0;

	if( size < 0 )
	{
		throw gcnew System::ArgumentException(
			     "ewf.net " + function + ": invalid size" );
	}
	if( size == 0 )
	{
		return( 0 );
	}
	if( size > buffer->Length )
	{
		throw gcnew System::ArgumentException(
			     "ewf.net " + function + ": buffer too small" );
	}
	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->ewf_handle );

	ewf_buffer = &( buffer[ 0 ] );

	read_count = libewf_handle_read_buffer(
	              handle,
	              ewf_buffer,
	              (size_t) size,
	              &error );

	if( read_count == -1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to read buffer from ewf handle." );

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
	return( (int) read_count );
}

int Handle::WriteBuffer( array<System::Byte>^ buffer,
                         int size )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error             = NULL;
	libewf_handle_t *handle           = NULL;
	System::String^ error_string      = nullptr;
	System::String^ function          = "Handle::WriteBuffer";
	pin_ptr<const uint8_t> ewf_buffer = nullptr;
	size_t write_count                = 0;

	if( size < 0 )
	{
		throw gcnew System::ArgumentException(
			     "ewf.net " + function + ": invalid size" );
	}
	if( size == 0 )
	{
		return( 0 );
	}
	if( size > buffer->Length )
	{
		throw gcnew System::ArgumentException(
			     "ewf.net " + function + ": buffer too small" );
	}
	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->ewf_handle );

	ewf_buffer = &( buffer[ 0 ] );

	write_count = libewf_handle_write_buffer(
	               handle,
	               ewf_buffer,
	               (size_t) size,
	               &error );

	if( write_count == -1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to write buffer to ewf handle." );

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
	return( (int) write_count );
}

System::Int64 Handle::SeekOffset( System::Int64 offset,
                                  System::IO::SeekOrigin origin )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error        = NULL;
	libewf_handle_t *handle      = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::SeekOffset";
	off64_t ewf_offset           = 0;
	int ewf_whence               = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->ewf_handle );

	Marshal::WriteInt64(
	 (IntPtr) &ewf_offset,
	 offset );

	if( origin == System::IO::SeekOrigin::Begin )
	{
		ewf_whence = SEEK_SET;
	}
	else if( origin == System::IO::SeekOrigin::Current )
	{
		ewf_whence = SEEK_CUR;
	}
	else if( origin == System::IO::SeekOrigin::End )
	{
		ewf_whence = SEEK_END;
	}
	else
	{
		throw gcnew System::ArgumentException(
			     "ewf.net " + function + ": unsupported origin" );
	}
	ewf_offset = libewf_handle_seek_offset(
	                handle,
	                ewf_offset,
	                ewf_whence,
	                &error );

	if( ewf_offset == -1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to seek offset in ewf handle." );

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
	offset = Marshal::ReadInt64(
	          (IntPtr) &ewf_offset );

	return( offset );
}

System::Int64 Handle::GetOffset( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error        = NULL;
	libewf_handle_t *handle      = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::GetOffset";
	off64_t ewf_offset           = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->ewf_handle );

	if( libewf_handle_get_offset(
	     handle,
	     &ewf_offset,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve offset from ewf handle." );

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
	return( Marshal::ReadInt64(
	         (IntPtr) &ewf_offset ) );
}

System::UInt64 Handle::GetMediaSize( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error        = NULL;
	libewf_handle_t *handle      = NULL;
	System::String^ error_string = nullptr;
	System::String^ function     = "Handle::GetMediaSize";
	size64_t ewf_media_size      = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &handle,
	 this->ewf_handle );

	if( libewf_handle_get_media_size(
	     handle,
	     &ewf_media_size,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve media size from ewf handle." );

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
	return( Marshal::ReadInt64(
	         (IntPtr) &ewf_media_size ) );
}

} // namespace EWF

