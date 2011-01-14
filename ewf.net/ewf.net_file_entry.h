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

#using <mscorlib.dll>

using namespace System;
using namespace System::IO;

namespace EWF {

public ref class FileEntry sealed
{
	private:
		System::IntPtr ewf_file_entry;

	private:
		FileEntry( System::IntPtr ewf_file_entry );

	public:
		FileEntry( void );
		~FileEntry( void );

		/* TODO add functions ? */
};

} // namespace EWF

