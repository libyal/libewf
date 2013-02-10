/*
 * Definitions for libewf
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EWF_DEFINIONS_H )
#define _EWF_DEFINIONS_H

#include <common.h>
#include <types.h>

/* The minimum chunk size is 32k or ( 64 sectors * 512 )
 */
#define EWF_MINIMUM_CHUNK_SIZE			32768

/* The maximum number of table entries for EWF
 */
#define EWF_MAXIMUM_TABLE_ENTRIES		16375

/* The maximum number of table entries for EWF as of EnCase 6
 */
#define EWF_MAXIMUM_TABLE_ENTRIES_ENCASE6	65534

#endif

