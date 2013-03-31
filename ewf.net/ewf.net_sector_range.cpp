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
#pragma managed( push, off )
#include <common.h>

#include <stdlib.h>
#pragma managed( pop )

#include <vcclr.h>

#include "ewf.net_sector_range.h"

#using <mscorlib.dll>

using namespace System;

namespace EWF {

SectorRange::SectorRange( System::UInt64 start_sector,
                          System::UInt64 number_of_sectors )
{
	this->start_sector      = start_sector;
	this->number_of_sectors = number_of_sectors;
}

} // namespace EWF

