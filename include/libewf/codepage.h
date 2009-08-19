/*
 * Codepage definitions for libewf
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _LIBEWF_CODEPAGE_H )
#define _LIBEWF_CODEPAGE_H

#include <libewf/types.h>

/* The codepages
 */
enum LIBEWF_CODEPAGES
{
	LIBEWF_CODEPAGE_ASCII				= 20127,
	LIBEWF_CODEPAGE_WINDOWS_1250			= 1250,
	LIBEWF_CODEPAGE_WINDOWS_1251			= 1251,
	LIBEWF_CODEPAGE_WINDOWS_1252			= 1252,
	LIBEWF_CODEPAGE_WINDOWS_1253			= 1253,
	LIBEWF_CODEPAGE_WINDOWS_1254			= 1254,
	LIBEWF_CODEPAGE_WINDOWS_1255			= 1255,
	LIBEWF_CODEPAGE_WINDOWS_1256			= 1256,
	LIBEWF_CODEPAGE_WINDOWS_1257			= 1257,
	LIBEWF_CODEPAGE_WINDOWS_1258			= 1258
};

#define LIBEWF_CODEPAGE_WINDOWS_CENTRAL_EUROPEAN	LIBEWF_CODEPAGE_WINDOWS_1250
#define LIBEWF_CODEPAGE_WINDOWS_CYRILLIC		LIBEWF_CODEPAGE_WINDOWS_1251
#define LIBEWF_CODEPAGE_WINDOWS_WESTERN_EUROPEAN	LIBEWF_CODEPAGE_WINDOWS_1252
#define LIBEWF_CODEPAGE_WINDOWS_GREEK			LIBEWF_CODEPAGE_WINDOWS_1253
#define LIBEWF_CODEPAGE_WINDOWS_TURKISH			LIBEWF_CODEPAGE_WINDOWS_1254
#define LIBEWF_CODEPAGE_WINDOWS_HEBREW			LIBEWF_CODEPAGE_WINDOWS_1255
#define LIBEWF_CODEPAGE_WINDOWS_ARABIC			LIBEWF_CODEPAGE_WINDOWS_1256
#define LIBEWF_CODEPAGE_WINDOWS_BALTIC			LIBEWF_CODEPAGE_WINDOWS_1257
#define LIBEWF_CODEPAGE_WINDOWS_VIETNAMESE		LIBEWF_CODEPAGE_WINDOWS_1258

#endif

