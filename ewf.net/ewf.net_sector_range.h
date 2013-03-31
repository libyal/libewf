/*
 * SectorRange class of libewf .net managed wrapper
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

#if !defined( _EWF_NET_SECTOR_RANGE_H )
#define _EWF_NET_SECTOR_RANGE_H

#using <mscorlib.dll>

using namespace System;

namespace EWF {

public ref class SectorRange sealed
{
	public:
		System::UInt64 start_sector;
		System::UInt64 number_of_sectors;

	public:
		SectorRange( System::UInt64 start_sector,
		             System::UInt64 number_of_sectors );

		/* Implicit defined:
		 * ~SectorRange( void );
		 */
};

} // namespace EWF

#endif

