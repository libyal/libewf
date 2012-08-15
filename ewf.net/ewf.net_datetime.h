/*
 * Date and time functions of libewf .net managed wrapper
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

#if !defined( _EWF_NET_DATETIME_H )
#define _EWF_NET_DATETIME_H

#include <common.h>
#include <types.h>

#using <mscorlib.dll>

using namespace System;

#if defined( __cplusplus )
extern "C" {
#endif

namespace EWF {

public ref class DateTime sealed
{
        public:
		static System::DateTime^ FromFiletime( System::UInt64 filetime );

		static System::DateTime^ FromPOSIXTime( System::UInt32 posix_time );
};

} // namespace EWF

#if defined( __cplusplus )
}
#endif

#endif

