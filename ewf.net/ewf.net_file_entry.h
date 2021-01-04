/*
 * FileEntry class of libewf .net managed wrapper
 *
 * Copyright (C) 2006-2021, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EWF_NET_FILE_ENTRY_H )
#define _EWF_NET_FILE_ENTRY_H

#using <mscorlib.dll>

using namespace System;
using namespace System::IO;

namespace EWF {

public ref class FileEntry sealed
{
	private:
		System::IntPtr ewf_file_entry;

	public:
		FileEntry( System::IntPtr ewf_file_entry );
		~FileEntry( void );

		System::Byte GetType( void );
		System::UInt32 GetFlags( void );

		System::Int64 GetMediaDataOffset( void );

		System::UInt64 GetMediaDataSize( void );

		System::Int64 GetDuplicateMediaDataOffset( void );

#if _MSC_VER >= 1600
		System::String^ GetName( void );
#endif /* _MSC_VER >= 1600 */

		System::UInt64 GetSize( void );

		System::DateTime^ GetCreationTime( void );

		System::DateTime^ GetModificationTime( void );

		System::DateTime^ GetAccessTime( void );

		System::DateTime^ GetEntryModificationTime( void );

#if _MSC_VER >= 1600
		System::String^ GetHashValueMD5( void );
#endif /* _MSC_VER >= 1600 */

		int GetNumberOfSubFileEntries( void );

		FileEntry^ GetSubFileEntry( int sub_file_entry_index );

		int ReadBuffer( array<System::Byte>^ buffer,
		                int size );

		int ReadBufferAtOffset( array<System::Byte>^ buffer,
		                        int size,
		                        System::Int64 offset );

		System::Int64 SeekOffset( System::Int64 offset,
		                          System::IO::SeekOrigin origin );

		System::Int64 GetOffset( void );

};

} // namespace EWF

#endif

