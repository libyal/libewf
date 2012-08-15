/*
 * FileEntry class of libewf .net managed wrapper
 *
 * Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include <libewf.h>
#pragma managed( pop )

#include <vcclr.h>

#include "ewf.net_definitions.h"
#include "ewf.net_datetime.h"
#include "ewf.net_file_entry.h"

#using <mscorlib.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EWF {

FileEntry::FileEntry( System::IntPtr ewf_file_entry )
{
	this->ewf_file_entry = ewf_file_entry;
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

System::UInt32 FileEntry::GetFlags( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::GetFlags";
	uint32_t ewf_flags              = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_flags(
	     file_entry,
	     &ewf_flags,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve flags from ewf file entry." );

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
	return( Marshal::ReadInt32(
	         (IntPtr) &ewf_flags ) );
}

System::Int64 FileEntry::GetMediaDataOffset( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::MediaDataGetOffset";
	off64_t ewf_offset              = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_media_data_offset(
	     file_entry,
	     &ewf_offset,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve media data offset from ewf file entry." );

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

System::UInt64 FileEntry::GetMediaDataSize( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::MediaDataGetSize";
	size64_t ewf_size               = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_media_data_size(
	     file_entry,
	     &ewf_size,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve media data size from ewf file entry." );

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
	         (IntPtr) &ewf_size ) );
}

System::Int64 FileEntry::GetDuplicateMediaDataOffset( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::DuplicateMediaDataGetOffset";
	off64_t ewf_offset              = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_duplicate_media_data_offset(
	     file_entry,
	     &ewf_offset,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve duplicate media data offset from ewf file entry." );

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

#if _MSC_VER >= 1600

System::String^ FileEntry::GetName( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::GetName";
	System::String^ name            = nullptr;
	uint16_t *ewf_name              = NULL;
	size_t ewf_name_size            = 0;
	int result                      = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	result = libewf_file_entry_get_utf16_name_size(
	          file_entry,
	          &ewf_name_size,
	          &error );

	if( result == -1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve name size." );

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
	else if( result == 0 )
	{
		return( nullptr );
	}
	ewf_name = (uint16_t *) memory_allocate(
	                         sizeof( uint16_t ) * ewf_name_size );

	if( ewf_name == NULL )
	{
		throw gcnew System::Exception(
		             "ewf.net " + function + ": unable to create name." );
	}
	if( libewf_file_entry_get_utf16_name(
	     file_entry,
	     ewf_name,
	     ewf_name_size,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve name." );

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
		 ewf_name );

		throw gcnew System::Exception(
			     error_string );
	}
	try
	{
		name = gcnew System::String(
		              (wchar_t *) ewf_name );
	}
	catch( System::Exception^ exception )
	{
		memory_free(
		 ewf_name );

		throw exception;
	}
	memory_free(
	 ewf_name );

	return( name );
}

#endif /* _MSC_VER >= 1600 */

System::UInt64 FileEntry::GetSize( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::GetSize";
	size64_t ewf_size               = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_size(
	     file_entry,
	     &ewf_size,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve size from ewf file entry." );

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
	         (IntPtr) &ewf_size ) );
}

System::DateTime^ FileEntry::GetCreationTime( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::DateTime^ creation_time = nullptr;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::GetCreationTime";
	uint32_t ewf_creation_time      = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_creation_time(
	     file_entry,
	     &ewf_creation_time,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve creation time from ewf file entry." );

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
	creation_time = DateTime::FromPOSIXTime( ewf_creation_time );

	return( creation_time );
}

System::DateTime^ FileEntry::GetModificationTime( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error               = NULL;
	libewf_file_entry_t *file_entry     = NULL;
	System::DateTime^ modification_time = nullptr;
	System::String^ error_string        = nullptr;
	System::String^ function            = "FileEntry::GetModificationTime";
	uint32_t ewf_modification_time      = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_modification_time(
	     file_entry,
	     &ewf_modification_time,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve modification time from ewf file entry." );

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
	modification_time = DateTime::FromPOSIXTime( ewf_modification_time );

	return( modification_time );
}

System::DateTime^ FileEntry::GetAccessTime( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::DateTime^ access_time   = nullptr;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::GetAccessTime";
	uint32_t ewf_access_time        = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_access_time(
	     file_entry,
	     &ewf_access_time,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve access time from ewf file entry." );

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
	access_time = DateTime::FromPOSIXTime( ewf_access_time );

	return( access_time );
}

System::DateTime^ FileEntry::GetEntryModificationTime( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error                     = NULL;
	libewf_file_entry_t *file_entry           = NULL;
	System::DateTime^ entry_modification_time = nullptr;
	System::String^ error_string              = nullptr;
	System::String^ function                  = "FileEntry::GetEntryModificationTime";
	uint32_t ewf_entry_modification_time      = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_entry_modification_time(
	     file_entry,
	     &ewf_entry_modification_time,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve entry modification time from ewf file entry." );

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
	entry_modification_time = DateTime::FromPOSIXTime( ewf_entry_modification_time );

	return( entry_modification_time );
}

#if _MSC_VER >= 1600

System::String^ FileEntry::GetHashValueMD5( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::GetHashValueMD5";
	System::String^ hash_value_md5  = nullptr;
	uint16_t *ewf_hash_value_md5    = NULL;
	size_t ewf_hash_value_md5_size  = 33;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	ewf_hash_value_md5 = (uint16_t *) memory_allocate(
	                                   sizeof( uint16_t ) * ewf_hash_value_md5_size );

	if( ewf_hash_value_md5 == NULL )
	{
		throw gcnew System::Exception(
		             "ewf.net " + function + ": unable to create hash value MD5." );
	}
	if( libewf_file_entry_get_utf16_hash_value_md5(
	     file_entry,
	     ewf_hash_value_md5,
	     ewf_hash_value_md5_size,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve hash value MD5." );

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
		 ewf_hash_value_md5 );

		throw gcnew System::Exception(
			     error_string );
	}
	try
	{
		hash_value_md5 = gcnew System::String(
		                        (wchar_t *) ewf_hash_value_md5 );
	}
	catch( System::Exception^ exception )
	{
		memory_free(
		 ewf_hash_value_md5 );

		throw exception;
	}
	memory_free(
	 ewf_hash_value_md5 );

	return( hash_value_md5 );
}

#endif /* _MSC_VER >= 1600 */

int FileEntry::GetNumberOfSubFileEntries( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::GetNumberOfSubFileEntries";
	int number_of_sub_file_entries  = 0;
	int result                      = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	result = libewf_file_entry_get_number_of_sub_file_entries(
	          file_entry,
	          &number_of_sub_file_entries,
	          &error );

	if( result == -1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve number of sub file entries from ewf file entry." );

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
	return( number_of_sub_file_entries );
}

FileEntry^ FileEntry::GetSubFileEntry( int sub_file_entry_index )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error                   = NULL;
	libewf_file_entry_t *file_entry         = NULL;
	libewf_file_entry_t *ewf_sub_file_entry = NULL;
	FileEntry^ sub_file_entry               = nullptr;
	System::String^ error_string            = nullptr;
	System::String^ function                = "FileEntry::GetSubFileEntry";
	int result                              = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	result = libewf_file_entry_get_sub_file_entry(
	          file_entry,
	          sub_file_entry_index,
	          &ewf_sub_file_entry,
	          &error );

	if( result == -1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve sub file entry: " + sub_file_entry_index + " from ewf file entry." );

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
	else if( result == 0 )
	{
		return( nullptr );
	}
	sub_file_entry = gcnew FileEntry( Marshal::ReadIntPtr(
	                                            (IntPtr) &ewf_sub_file_entry ) );

	return( sub_file_entry );
}

int FileEntry::ReadBuffer( array<System::Byte>^ buffer,
                           int size )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::ReadBuffer";
	pin_ptr<uint8_t> ewf_buffer     = nullptr;
	size_t read_count               = 0;

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
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	ewf_buffer = &( buffer[ 0 ] );

	read_count = libewf_file_entry_read_buffer(
	              file_entry,
	              ewf_buffer,
	              (size_t) size,
	              &error );

	if( read_count == -1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to read buffer from ewf file entry." );

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

int FileEntry::ReadRandom( array<System::Byte>^ buffer,
                           int size,
                           System::Int64 offset )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::ReadRandom";
	pin_ptr<uint8_t> ewf_buffer     = nullptr;
	off64_t ewf_offset              = 0;
	size_t read_count               = 0;

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
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	Marshal::WriteInt64(
	 (IntPtr) &ewf_offset,
	 offset );

	ewf_buffer = &( buffer[ 0 ] );

	read_count = libewf_file_entry_read_random(
	              file_entry,
	              ewf_buffer,
	              (size_t) size,
	              ewf_offset,
	              &error );

	if( read_count == -1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to read random from ewf file entry." );

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

System::Int64 FileEntry::SeekOffset( System::Int64 offset,
                                     System::IO::SeekOrigin origin )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::SeekOffset";
	off64_t ewf_offset              = 0;
	int ewf_whence                  = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

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
	ewf_offset = libewf_file_entry_seek_offset(
	              file_entry,
	              ewf_offset,
	              ewf_whence,
	              &error );

	if( ewf_offset == -1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to seek offset in ewf file entry." );

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

System::Int64 FileEntry::GetOffset( void )
{
	char ewf_error_string[ EWF_NET_ERROR_STRING_SIZE ];

	libewf_error_t *error           = NULL;
	libewf_file_entry_t *file_entry = NULL;
	System::String^ error_string    = nullptr;
	System::String^ function        = "FileEntry::GetOffset";
	off64_t ewf_offset              = 0;

	Marshal::WriteIntPtr(
	 (IntPtr) &file_entry,
	 this->ewf_file_entry );

	if( libewf_file_entry_get_offset(
	     file_entry,
	     &ewf_offset,
	     &error ) != 1 )
	{
		error_string = gcnew System::String(
		                      "ewf.net " + function + ": unable to retrieve offset from ewf file entry." );

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

} // namespace EWF

