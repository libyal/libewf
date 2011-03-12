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

#using <mscorlib.dll>

using namespace System;
using namespace System::IO;

namespace EWF {

public ref class Handle sealed
{
	private:
		System::IntPtr ewf_handle;

	private:
		Handle( System::IntPtr ewf_handle );

	public:
		Handle( void );
		~Handle( void );

		int GetAccessFlagsRead( void );
		int GetAccessFlagsReadWrite( void );
		int GetAccessFlagsWrite( void );
		int GetAccessFlagsWriteResume( void );

		bool CheckFileSignature( System::String^ filename );

		array<System::String^>^ Glob( System::String^ filename );

		Handle^ Clone( void );

		void Open( array<System::String^>^ filenames,
		           int access_flags );

		void Close( void );

		/* TODO add read write chunk functions ? */

		int ReadBuffer( array<System::Byte>^ buffer,
		                int size );

		int ReadRandom( array<System::Byte>^ buffer,
		                int size,
		                System::Int64 offset );

		int WriteBuffer( array<System::Byte>^ buffer,
		                 int size );

		int WriteRandom( array<System::Byte>^ buffer,
		                 int size,
		                 System::Int64 offset );

		System::Int64 SeekOffset( System::Int64 offset,
		                          System::IO::SeekOrigin origin );

		System::Int64 GetOffset( void );

		/* TODO add segment file functions ? */

		System::UInt32 GetSectorsPerChunk( void );

		void SetSectorsPerChunk( System::UInt32 sectors_per_chunk );

		System::UInt32 GetBytesPerSector( void );

		void SetBytesPerSector( System::UInt32 bytes_per_sector );

		System::UInt64 GetNumberOfSectors( void );

		System::UInt32 GetChunkSize( void );

		System::UInt32 GetErrorGranularity( void );

		void SetErrorGranularity( System::UInt32 error_granularity );

		/* TODO add compression values functions ? */

		System::UInt64 GetMediaSize( void );

		void SetMediaSize( System::UInt64 media_size );

		System::Byte GetMediaType( void );

		void SetMediaType( System::Byte media_type );

		System::Byte GetMediaFlags( void );

		void SetMediaFlags( System::Byte media_flags );

		System::Byte GetFormat( void );

		void SetFormat( System::Byte format );

		/* TODO add GUID functions ? */

		/* TODO add MD5 hash functions ? why bother when Get/SetHashValue are available ? */

		/* TODO add SHA1 hash functions ? why bother when Get/SetHashValue are available ? */

		/* TODO add other functions ? */

		int GetNumberOfAcquiryErrors( void );

		/* TODO add aquiry error functions ?
 
		bool GetAcquiryError( int index,
		                      System::UInt64 first_sector,
		                      System::UInt32 number_of_sectors );

		void AppendAcquiryError( System::UInt64 first_sector,
		                         System::UInt32 number_of_sectors );

		*/

		int GetNumberOfChecksumErrors( void );

		/* TODO add checksum error functions ?
 
		*/

		int GetNumberOfSessions( void );

		/* TODO add session functions ?
 
		*/

		/* TODO add header codepage functions ?

		int GetHeaderCodepage( void );

		void SetHeaderCodepage( int codepage );

		*/

		/* TODO add header date format functions ?

		int GetHeaderValuesDateFormat( void );

		void SetHeaderValuesDateFormat( int date_format );

		*/

#if _MSC_VER >= 1600

		int GetNumberOfHeaderValues( void );

		System::String^ GetHeaderValueIdentifier( int index );

		System::String^ GetHeaderValue( System::String^ identifier );

		void SetHeaderValue( System::String^ identifier,
		                     System::String^ header_value );

		int GetNumberOfHashValues( void );

		System::String^ GetHashValueIdentifier( int index );

		System::String^ GetHashValue( System::String^ identifier );

		void SetHashValue( System::String^ identifier,
		                   System::String^ hash_value );

#endif /* _MSC_VER >= 1600 */

		/* TODO add single file entry root functions ?
		 * create separate class for single file entry
		 */
};

} // namespace EWF

