/*
 * Library lef_file_entry type test program
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_libfvalue.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_lef_file_entry.h"

uint8_t ewf_test_lef_file_entry_types_data1[ 107 ] = {
	0x6d, 0x69, 0x64, 0x09, 0x6c, 0x73, 0x09, 0x62, 0x65, 0x09, 0x69, 0x64, 0x09, 0x63, 0x72, 0x09,
	0x61, 0x63, 0x09, 0x77, 0x72, 0x09, 0x6d, 0x6f, 0x09, 0x64, 0x6c, 0x09, 0x73, 0x69, 0x67, 0x09,
	0x68, 0x61, 0x09, 0x73, 0x68, 0x61, 0x09, 0x73, 0x6e, 0x68, 0x09, 0x70, 0x09, 0x6e, 0x09, 0x64,
	0x75, 0x09, 0x6c, 0x6f, 0x09, 0x70, 0x6f, 0x09, 0x70, 0x6d, 0x09, 0x6f, 0x65, 0x73, 0x09, 0x6f,
	0x70, 0x72, 0x09, 0x73, 0x72, 0x63, 0x09, 0x73, 0x75, 0x62, 0x09, 0x63, 0x69, 0x64, 0x09, 0x6a,
	0x71, 0x09, 0x61, 0x6c, 0x74, 0x09, 0x65, 0x70, 0x09, 0x61, 0x71, 0x09, 0x63, 0x66, 0x69, 0x09,
	0x73, 0x67, 0x0a, 0x09, 0x65, 0x61, 0x09, 0x6c, 0x70, 0x74, 0x0d };

uint8_t ewf_test_lef_file_entry_values_data1[ 4810 ] = {
	0x44, 0x43, 0x31, 0x38, 0x35, 0x43, 0x36, 0x38, 0x31, 0x31, 0x34, 0x44, 0x34, 0x45, 0x41, 0x45,
	0x42, 0x33, 0x41, 0x37, 0x38, 0x45, 0x43, 0x33, 0x33, 0x36, 0x33, 0x43, 0x36, 0x34, 0x42, 0x36,
	0x09, 0x31, 0x36, 0x31, 0x39, 0x36, 0x09, 0x31, 0x20, 0x31, 0x33, 0x31, 0x33, 0x35, 0x63, 0x31,
	0x20, 0x33, 0x66, 0x34, 0x34, 0x09, 0x32, 0x30, 0x31, 0x09, 0x31, 0x35, 0x33, 0x35, 0x34, 0x37,
	0x30, 0x34, 0x33, 0x32, 0x09, 0x31, 0x35, 0x37, 0x34, 0x31, 0x39, 0x35, 0x37, 0x35, 0x34, 0x09,
	0x31, 0x35, 0x33, 0x35, 0x34, 0x37, 0x30, 0x34, 0x33, 0x32, 0x09, 0x31, 0x35, 0x37, 0x34, 0x31,
	0x39, 0x35, 0x37, 0x35, 0x34, 0x09, 0x09, 0x09, 0x09, 0x09, 0x31, 0x33, 0x20, 0x46, 0x49, 0x4c,
	0x45, 0x31, 0x30, 0x7e, 0x31, 0x2e, 0x54, 0x58, 0x54, 0x09, 0x09, 0x73, 0x61, 0x6d, 0x70, 0x6c,
	0x65, 0x2e, 0x70, 0x6e, 0x67, 0x09, 0x09, 0x09, 0x32, 0x30, 0x30, 0x30, 0x32, 0x32, 0x34, 0x31,
	0x09, 0x30, 0x09, 0x09, 0x34, 0x31, 0x39, 0x34, 0x33, 0x30, 0x34, 0x09, 0x31, 0x09, 0x31, 0x09,
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x31, 0x30, 0x42, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x34, 0x31, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x32, 0x30,
	0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x32, 0x30, 0x30, 0x37, 0x35, 0x30, 0x30, 0x37, 0x34, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x43, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x31, 0x30, 0x30, 0x36,
	0x34, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x32,
	0x30, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x36,
	0x37, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x44, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x31, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x33, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x35, 0x32, 0x30, 0x30, 0x36, 0x31, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x33, 0x34, 0x30, 0x30, 0x33, 0x41, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x45, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x33, 0x30, 0x30, 0x36,
	0x46, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x37,
	0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x39, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36,
	0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x38, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36,
	0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x34, 0x33, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x36,
	0x43, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x34, 0x30, 0x30, 0x35, 0x30, 0x30,
	0x30, 0x34, 0x39, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x35, 0x35, 0x30, 0x30, 0x36, 0x45, 0x30,
	0x30, 0x36, 0x42, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x37, 0x37, 0x30,
	0x30, 0x36, 0x45, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x42, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x34, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x34, 0x34, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x36,
	0x35, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x34, 0x31, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36,
	0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33,
	0x32, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x33, 0x31, 0x30, 0x30, 0x33, 0x39, 0x30, 0x30, 0x32,
	0x44, 0x30, 0x30, 0x33, 0x31, 0x30, 0x30, 0x33, 0x31, 0x30, 0x30, 0x32, 0x44, 0x30, 0x30, 0x33,
	0x31, 0x30, 0x30, 0x33, 0x39, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x33,
	0x30, 0x30, 0x30, 0x33, 0x41, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x35, 0x30, 0x30, 0x33,
	0x41, 0x30, 0x30, 0x33, 0x35, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x36, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x35, 0x30, 0x30, 0x37, 0x38, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x34, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x34, 0x36, 0x30,
	0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30,
	0x30, 0x37, 0x32, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x36, 0x43, 0x30,
	0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36,
	0x35, 0x30, 0x30, 0x35, 0x32, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36,
	0x46, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x34, 0x39, 0x30, 0x30, 0x36,
	0x45, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x43, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x35, 0x32, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x33, 0x30,
	0x30, 0x36, 0x46, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x34, 0x43, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x37, 0x34, 0x30,
	0x30, 0x36, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x33, 0x37, 0x30,
	0x30, 0x33, 0x36, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x31, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36,
	0x35, 0x30, 0x30, 0x35, 0x32, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36,
	0x46, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x34, 0x46, 0x30, 0x30, 0x36,
	0x36, 0x30, 0x30, 0x36, 0x36, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37,
	0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x33,
	0x36, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x46, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x45, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x43, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x35, 0x32, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x33, 0x30,
	0x30, 0x36, 0x46, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x35, 0x30, 0x30,
	0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x36, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x32, 0x46, 0x30, 0x30, 0x32, 0x34, 0x30, 0x30, 0x34, 0x33, 0x30, 0x30, 0x36, 0x31, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x46, 0x30,
	0x30, 0x36, 0x37, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x43, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x44, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36,
	0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x34,
	0x36, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x37,
	0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x36, 0x46, 0x30,
	0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x34, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30, 0x34, 0x45, 0x30, 0x30, 0x34, 0x37, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31,
	0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34,
	0x38, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x34,
	0x35, 0x30, 0x30, 0x37, 0x38, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36,
	0x45, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x32,
	0x30, 0x30, 0x30, 0x34, 0x31, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37,
	0x32, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x32, 0x30, 0x30, 0x37, 0x35, 0x30, 0x30, 0x37,
	0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x37,
	0x34, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x37, 0x35, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x42, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x44, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x30, 0x30,
	0x30, 0x36, 0x39, 0x30, 0x30, 0x37, 0x38, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x43, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x32, 0x45, 0x30,
	0x30, 0x33, 0x30, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x43, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x46, 0x30, 0x30, 0x37, 0x37, 0x30, 0x30, 0x36,
	0x45, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x34,
	0x36, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x37,
	0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x46, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x35, 0x33, 0x30, 0x30, 0x37, 0x34, 0x30,
	0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x31, 0x30,
	0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x34, 0x36, 0x30,
	0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x37, 0x33, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x31, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x34, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32,
	0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x42, 0x39, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x36,
	0x33, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x36,
	0x31, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36,
	0x35, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37,
	0x34, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37,
	0x34, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x33, 0x41, 0x30, 0x30, 0x35, 0x46, 0x30, 0x30, 0x36,
	0x42, 0x30, 0x30, 0x34, 0x44, 0x30, 0x30, 0x34, 0x34, 0x30, 0x30, 0x34, 0x39, 0x30, 0x30, 0x37,
	0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x35, 0x35, 0x30, 0x30, 0x37,
	0x33, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x35, 0x34, 0x30, 0x30, 0x36,
	0x31, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33,
	0x43, 0x30, 0x30, 0x33, 0x46, 0x30, 0x30, 0x37, 0x38, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x36,
	0x43, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x37, 0x36, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37,
	0x32, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x36,
	0x45, 0x30, 0x30, 0x33, 0x44, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x33, 0x31, 0x30, 0x30, 0x32,
	0x45, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x36,
	0x35, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x36,
	0x34, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x33,
	0x44, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x35, 0x35, 0x30, 0x30, 0x35, 0x34, 0x30, 0x30, 0x34,
	0x36, 0x30, 0x30, 0x32, 0x44, 0x30, 0x30, 0x33, 0x38, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x33,
	0x46, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x33, 0x43, 0x30, 0x30, 0x32, 0x31, 0x30, 0x30, 0x34,
	0x34, 0x30, 0x30, 0x34, 0x46, 0x30, 0x30, 0x34, 0x33, 0x30, 0x30, 0x35, 0x34, 0x30, 0x30, 0x35,
	0x39, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30, 0x34, 0x35, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x37,
	0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x37,
	0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30, 0x35, 0x35, 0x30, 0x30, 0x34,
	0x32, 0x30, 0x30, 0x34, 0x43, 0x30, 0x30, 0x34, 0x39, 0x30, 0x30, 0x34, 0x33, 0x30, 0x30, 0x32,
	0x30, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x32, 0x44, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x32,
	0x46, 0x30, 0x30, 0x34, 0x31, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36,
	0x43, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x34,
	0x34, 0x30, 0x30, 0x35, 0x34, 0x30, 0x30, 0x34, 0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x35,
	0x30, 0x30, 0x30, 0x34, 0x43, 0x30, 0x30, 0x34, 0x39, 0x30, 0x30, 0x35, 0x33, 0x30, 0x30, 0x35,
	0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x33, 0x31, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x33,
	0x30, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x34, 0x35, 0x30, 0x30, 0x34,
	0x45, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x36,
	0x38, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x33,
	0x41, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x37, 0x37, 0x30, 0x30, 0x37,
	0x37, 0x30, 0x30, 0x37, 0x37, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37,
	0x30, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x32,
	0x45, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x32,
	0x46, 0x30, 0x30, 0x34, 0x34, 0x30, 0x30, 0x35, 0x34, 0x30, 0x30, 0x34, 0x34, 0x30, 0x30, 0x37,
	0x33, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36,
	0x46, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x37,
	0x34, 0x30, 0x30, 0x37, 0x39, 0x30, 0x30, 0x34, 0x43, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x37,
	0x33, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x32, 0x44, 0x30, 0x30, 0x33, 0x31, 0x30, 0x30, 0x32,
	0x45, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x37,
	0x34, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x33,
	0x43, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x37,
	0x33, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x37, 0x36, 0x30, 0x30, 0x36,
	0x35, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36,
	0x46, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x33, 0x44, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x33,
	0x31, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x33,
	0x45, 0x30, 0x30, 0x33, 0x43, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x37,
	0x32, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x39, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x33,
	0x43, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x37,
	0x32, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x39, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x33,
	0x43, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36,
	0x39, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x35, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x37, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x36, 0x33, 0x30,
	0x30, 0x36, 0x46, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x31, 0x30,
	0x30, 0x37, 0x30, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x35, 0x30,
	0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x34, 0x30,
	0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x34, 0x30,
	0x30, 0x36, 0x31, 0x30, 0x30, 0x33, 0x41, 0x30, 0x30, 0x36, 0x42, 0x30, 0x30, 0x34, 0x44, 0x30,
	0x30, 0x34, 0x34, 0x30, 0x30, 0x34, 0x39, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30,
	0x30, 0x36, 0x44, 0x30, 0x30, 0x35, 0x37, 0x30, 0x30, 0x36, 0x38, 0x30, 0x30, 0x36, 0x35, 0x30,
	0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x34, 0x36, 0x30, 0x30, 0x37, 0x32, 0x30,
	0x30, 0x36, 0x46, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x33, 0x43, 0x30, 0x30, 0x33, 0x46, 0x30, 0x30, 0x37, 0x38, 0x30, 0x30, 0x36, 0x44, 0x30,
	0x30, 0x36, 0x43, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x37, 0x36, 0x30, 0x30, 0x36, 0x35, 0x30,
	0x30, 0x37, 0x32, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x46, 0x30,
	0x30, 0x36, 0x45, 0x30, 0x30, 0x33, 0x44, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x33, 0x31, 0x30,
	0x30, 0x32, 0x45, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x32, 0x30, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36, 0x46, 0x30,
	0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x37, 0x30,
	0x30, 0x33, 0x44, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x35, 0x35, 0x30, 0x30, 0x35, 0x34, 0x30,
	0x30, 0x34, 0x36, 0x30, 0x30, 0x32, 0x44, 0x30, 0x30, 0x33, 0x38, 0x30, 0x30, 0x32, 0x32, 0x30,
	0x30, 0x33, 0x46, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x33, 0x43, 0x30, 0x30, 0x32, 0x31, 0x30,
	0x30, 0x34, 0x34, 0x30, 0x30, 0x34, 0x46, 0x30, 0x30, 0x34, 0x33, 0x30, 0x30, 0x35, 0x34, 0x30,
	0x30, 0x35, 0x39, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30, 0x34, 0x35, 0x30, 0x30, 0x32, 0x30, 0x30,
	0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x37, 0x33, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30, 0x35, 0x35, 0x30,
	0x30, 0x34, 0x32, 0x30, 0x30, 0x34, 0x43, 0x30, 0x30, 0x34, 0x39, 0x30, 0x30, 0x34, 0x33, 0x30,
	0x30, 0x32, 0x30, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x32, 0x44, 0x30, 0x30, 0x32, 0x46, 0x30,
	0x30, 0x32, 0x46, 0x30, 0x30, 0x34, 0x31, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x37, 0x30, 0x30,
	0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x32, 0x46, 0x30,
	0x30, 0x34, 0x34, 0x30, 0x30, 0x35, 0x34, 0x30, 0x30, 0x34, 0x34, 0x30, 0x30, 0x32, 0x30, 0x30,
	0x30, 0x35, 0x30, 0x30, 0x30, 0x34, 0x43, 0x30, 0x30, 0x34, 0x39, 0x30, 0x30, 0x35, 0x33, 0x30,
	0x30, 0x35, 0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x33, 0x31, 0x30, 0x30, 0x32, 0x45, 0x30,
	0x30, 0x33, 0x30, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x34, 0x35, 0x30,
	0x30, 0x34, 0x45, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x32, 0x32, 0x30,
	0x30, 0x36, 0x38, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x30, 0x30,
	0x30, 0x33, 0x41, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x37, 0x37, 0x30,
	0x30, 0x37, 0x37, 0x30, 0x30, 0x37, 0x37, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x31, 0x30,
	0x30, 0x37, 0x30, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x35, 0x30,
	0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x36, 0x44, 0x30,
	0x30, 0x32, 0x46, 0x30, 0x30, 0x34, 0x34, 0x30, 0x30, 0x35, 0x34, 0x30, 0x30, 0x34, 0x34, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x35, 0x30, 0x30, 0x30, 0x37, 0x32, 0x30,
	0x30, 0x36, 0x46, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x32, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x39, 0x30, 0x30, 0x34, 0x43, 0x30, 0x30, 0x36, 0x39, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x32, 0x44, 0x30, 0x30, 0x33, 0x31, 0x30,
	0x30, 0x32, 0x45, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x34, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x32, 0x32, 0x30, 0x30, 0x33, 0x45, 0x30,
	0x30, 0x33, 0x43, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x39, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x32, 0x30, 0x30, 0x30, 0x37, 0x36, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x36, 0x39, 0x30,
	0x30, 0x36, 0x46, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x33, 0x44, 0x30, 0x30, 0x32, 0x32, 0x30,
	0x30, 0x33, 0x31, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x32, 0x32, 0x30,
	0x30, 0x33, 0x45, 0x30, 0x30, 0x33, 0x43, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x32, 0x30,
	0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x39, 0x30, 0x30, 0x33, 0x45, 0x30,
	0x30, 0x33, 0x43, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x32, 0x30,
	0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x33, 0x45, 0x30,
	0x30, 0x36, 0x38, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x30, 0x30,
	0x30, 0x33, 0x41, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x37, 0x34, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36, 0x38, 0x30, 0x30, 0x37, 0x33, 0x30,
	0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36, 0x46, 0x30,
	0x30, 0x36, 0x44, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30,
	0x30, 0x36, 0x44, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x32, 0x46, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x37, 0x30, 0x30,
	0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x32, 0x46, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x37, 0x30, 0x30,
	0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x37, 0x30, 0x30,
	0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x33, 0x43, 0x30, 0x30, 0x32, 0x46, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x39, 0x30,
	0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x33, 0x43, 0x30,
	0x30, 0x37, 0x33, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x39, 0x30,
	0x30, 0x36, 0x45, 0x30, 0x30, 0x36, 0x37, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x36, 0x38, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x33, 0x41, 0x30,
	0x30, 0x32, 0x46, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30,
	0x30, 0x36, 0x33, 0x30, 0x30, 0x36, 0x38, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x36, 0x43, 0x30,
	0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x33, 0x30,
	0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x36, 0x44, 0x30,
	0x30, 0x32, 0x46, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x36, 0x44, 0x30,
	0x30, 0x36, 0x46, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x37, 0x33, 0x30,
	0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x37, 0x33, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x37, 0x33, 0x30,
	0x30, 0x36, 0x31, 0x30, 0x30, 0x36, 0x44, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30,
	0x30, 0x36, 0x35, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x45, 0x30,
	0x30, 0x36, 0x37, 0x30, 0x30, 0x33, 0x43, 0x30, 0x30, 0x32, 0x46, 0x30, 0x30, 0x37, 0x33, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36, 0x45, 0x30,
	0x30, 0x36, 0x37, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x33, 0x43, 0x30, 0x30, 0x32, 0x46, 0x30,
	0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36, 0x31, 0x30,
	0x30, 0x37, 0x39, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x33, 0x43, 0x30, 0x30, 0x32, 0x46, 0x30,
	0x30, 0x37, 0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x37, 0x33, 0x30,
	0x30, 0x37, 0x34, 0x30, 0x30, 0x33, 0x45, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x35, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x37,
	0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x36, 0x46, 0x30, 0x30, 0x36,
	0x44, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x30, 0x30, 0x30, 0x37,
	0x30, 0x30, 0x30, 0x36, 0x43, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x32, 0x45, 0x30, 0x30, 0x37,
	0x31, 0x30, 0x30, 0x37, 0x35, 0x30, 0x30, 0x36, 0x31, 0x30, 0x30, 0x37, 0x32, 0x30, 0x30, 0x36,
	0x31, 0x30, 0x30, 0x36, 0x45, 0x30, 0x30, 0x37, 0x34, 0x30, 0x30, 0x36, 0x39, 0x30, 0x30, 0x36,
	0x45, 0x30, 0x30, 0x36, 0x35, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x30, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x38, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x31, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x36,
	0x32, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x35, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x33,
	0x32, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x38, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x35, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x35, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x33,
	0x34, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x35, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x32, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x36, 0x32, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33,
	0x33, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x33, 0x38, 0x30, 0x30, 0x33, 0x37, 0x30, 0x30, 0x33,
	0x32, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x36, 0x36, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x36,
	0x34, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x33, 0x35, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x36,
	0x32, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33,
	0x32, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x36, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x30, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33,
	0x34, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x37, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x34, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x37, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33, 0x35, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x30, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x36,
	0x34, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x33, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x36, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x36, 0x34, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33,
	0x31, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33,
	0x35, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x39, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x36,
	0x34, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x38, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x32, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x30, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x33, 0x34, 0x30, 0x30, 0x33,
	0x31, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x36, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x32, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x38, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x34, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33, 0x33, 0x30, 0x30, 0x33,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x09, 0x0d, 0x0a };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_lef_file_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_initialize(
     void )
{
	libcerror_error_t *error                = NULL;
	libewf_lef_file_entry_t *lef_file_entry = NULL;
	int result                              = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests         = 7;
	int number_of_memset_fail_tests         = 1;
	int test_number                         = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_initialize(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_file_entry_free(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_initialize(
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	lef_file_entry = (libewf_lef_file_entry_t *) 0x12345678UL;

	result = libewf_lef_file_entry_initialize(
	          &lef_file_entry,
	          &error );

	lef_file_entry = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_lef_file_entry_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_lef_file_entry_initialize(
		          &lef_file_entry,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( lef_file_entry != NULL )
			{
				libewf_lef_file_entry_free(
				 &lef_file_entry,
				 NULL );
			}
		}
		else
		{
			EWF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EWF_TEST_ASSERT_IS_NULL(
			 "lef_file_entry",
			 lef_file_entry );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libewf_lef_file_entry_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_lef_file_entry_initialize(
		          &lef_file_entry,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( lef_file_entry != NULL )
			{
				libewf_lef_file_entry_free(
				 &lef_file_entry,
				 NULL );
			}
		}
		else
		{
			EWF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EWF_TEST_ASSERT_IS_NULL(
			 "lef_file_entry",
			 lef_file_entry );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 &lef_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_lef_file_entry_free(
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_clone(
     void )
{
	libcerror_error_t *error                            = NULL;
	libewf_lef_file_entry_t *destination_lef_file_entry = NULL;
	libewf_lef_file_entry_t *source_lef_file_entry      = NULL;
	libfvalue_split_utf8_string_t *types                = NULL;
	int result                                          = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                     = 9;
	int test_number                                     = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests                     = 1;
#endif
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libewf_lef_file_entry_initialize(
	          &source_lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_lef_file_entry",
	 source_lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_lef_file_entry_types_data1,
	          107,
	          (uint8_t) '\t',
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_file_entry_read_data(
	          source_lef_file_entry,
	          types,
	          ewf_test_lef_file_entry_values_data1,
	          4810,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_clone(
	          &destination_lef_file_entry,
	          source_lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_lef_file_entry",
	 destination_lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_file_entry_free(
	          &destination_lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_lef_file_entry",
	 destination_lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_file_entry_clone(
	          &destination_lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_lef_file_entry",
	 destination_lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_clone(
	          NULL,
	          source_lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	destination_lef_file_entry = (libewf_lef_file_entry_t *) 0x12345678UL;

	result = libewf_lef_file_entry_clone(
	          &destination_lef_file_entry,
	          source_lef_file_entry,
	          &error );

	destination_lef_file_entry = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_lef_file_entry_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_lef_file_entry_clone(
		          &destination_lef_file_entry,
		          source_lef_file_entry,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_lef_file_entry != NULL )
			{
				libewf_lef_file_entry_free(
				 &destination_lef_file_entry,
				 NULL );
			}
		}
		else
		{
			EWF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EWF_TEST_ASSERT_IS_NULL(
			 "destination_lef_file_entry",
			 destination_lef_file_entry );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#if defined( OPTIMIZATION_DISABLED )

	for( test_number = 0;
	     test_number < number_of_memcpy_fail_tests;
	     test_number++ )
	{
		/* Test libewf_lef_file_entry_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_lef_file_entry_clone(
		          &destination_lef_file_entry,
		          source_lef_file_entry,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_lef_file_entry != NULL )
			{
				libewf_lef_file_entry_free(
				 &destination_lef_file_entry,
				 NULL );
			}
		}
		else
		{
			EWF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EWF_TEST_ASSERT_IS_NULL(
			 "destination_lef_file_entry",
			 destination_lef_file_entry );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfvalue_split_utf8_string_free(
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_file_entry_free(
	          &source_lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_lef_file_entry",
	 source_lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( destination_lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 &destination_lef_file_entry,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( source_lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 &source_lef_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_read_binary_extents function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_read_binary_extents(
     void )
{
	libcerror_error_t *error                = NULL;
	libewf_lef_file_entry_t *lef_file_entry = NULL;
	uint8_t *binary_extents_data1           = (uint8_t *) "1 13135c1 3f44";
	uint8_t *binary_extents_error_data1     = (uint8_t *) "1 XXXXXXX 3f44";
	uint8_t *binary_extents_error_data2     = (uint8_t *) "1 13135c1 XXXX";
	int result                              = 0;

	/* Initialize test
	 */
	result = libewf_lef_file_entry_initialize(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_read_binary_extents(
	          lef_file_entry,
	          binary_extents_data1,
	          14,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_read_binary_extents(
	          NULL,
	          binary_extents_data1,
	          14,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_binary_extents(
	          lef_file_entry,
	          NULL,
	          14,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_binary_extents(
	          lef_file_entry,
	          binary_extents_data1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_binary_extents(
	          lef_file_entry,
	          binary_extents_data1,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test unsupported number of offset values
	 */
	result = libewf_lef_file_entry_read_binary_extents(
	          lef_file_entry,
	          binary_extents_data1,
	          9,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test invalid data offset
	 */
	result = libewf_lef_file_entry_read_binary_extents(
	          lef_file_entry,
	          binary_extents_error_data1,
	          14,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test invalid data size
	 */
	result = libewf_lef_file_entry_read_binary_extents(
	          lef_file_entry,
	          binary_extents_error_data2,
	          14,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libewf_lef_file_entry_free(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 &lef_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_read_extended_attributes function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_read_extended_attributes(
     void )
{
	libcerror_error_t *error                = NULL;
	libewf_lef_file_entry_t *lef_file_entry = NULL;
	int result                              = 0;

	/* Initialize test
	 */
	result = libewf_lef_file_entry_initialize(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_read_extended_attributes(
	          lef_file_entry,
	          &( ewf_test_lef_file_entry_values_data1[ 167 ] ),
	          4640,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_read_extended_attributes(
	          NULL,
	          &( ewf_test_lef_file_entry_values_data1[ 167 ] ),
	          4640,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_extended_attributes(
	          lef_file_entry,
	          NULL,
	          4640,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_extended_attributes(
	          lef_file_entry,
	          &( ewf_test_lef_file_entry_values_data1[ 167 ] ),
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_lef_file_entry_read_extended_attributes with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_lef_file_entry_read_extended_attributes(
	          lef_file_entry,
	          &( ewf_test_lef_file_entry_values_data1[ 167 ] ),
	          4640,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libewf_lef_file_entry_read_extended_attributes with malloc failing in libewf_lef_extended_attribute_initialize
	 */
	ewf_test_malloc_attempts_before_fail = 1;

	result = libewf_lef_file_entry_read_extended_attributes(
	          lef_file_entry,
	          &( ewf_test_lef_file_entry_values_data1[ 167 ] ),
	          4640,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libewf_lef_file_entry_free(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 &lef_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_read_short_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_read_short_name(
     void )
{
	libcerror_error_t *error                = NULL;
	libewf_lef_file_entry_t *lef_file_entry = NULL;
	uint8_t *short_name_data1               = (uint8_t *) "13 FILE10~1.TXT";
	uint8_t *short_name_error_data1         = (uint8_t *) "13 FILE10~1.TXT X";
	uint8_t *short_name_error_data2         = (uint8_t *) "XX FILE10~1.TXT";
	int result                              = 0;

	/* Initialize test
	 */
	result = libewf_lef_file_entry_initialize(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_read_short_name(
	          lef_file_entry,
	          short_name_data1,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_read_short_name(
	          NULL,
	          short_name_data1,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_short_name(
	          lef_file_entry,
	          NULL,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_short_name(
	          lef_file_entry,
	          short_name_data1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test unsupported number of offset values
	 */
	result = libewf_lef_file_entry_read_short_name(
	          lef_file_entry,
	          short_name_error_data1,
	          18,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test invalid short name size
	 */
	result = libewf_lef_file_entry_read_short_name(
	          lef_file_entry,
	          short_name_error_data2,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libewf_lef_file_entry_free(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 &lef_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_read_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_read_data(
     void )
{
	libcerror_error_t *error                = NULL;
	libewf_lef_file_entry_t *lef_file_entry = NULL;
	libfvalue_split_utf8_string_t *types    = NULL;
	int result                              = 0;

	/* Initialize test
	 */
	result = libewf_lef_file_entry_initialize(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_lef_file_entry_types_data1,
	          107,
	          (uint8_t) '\t',
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_read_data(
	          lef_file_entry,
	          types,
	          ewf_test_lef_file_entry_values_data1,
	          4810,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_read_data(
	          NULL,
	          types,
	          ewf_test_lef_file_entry_values_data1,
	          4810,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_data(
	          lef_file_entry,
	          NULL,
	          ewf_test_lef_file_entry_values_data1,
	          4810,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_data(
	          lef_file_entry,
	          types,
	          NULL,
	          4810,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_data(
	          lef_file_entry,
	          types,
	          ewf_test_lef_file_entry_values_data1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_read_data(
	          lef_file_entry,
	          types,
	          ewf_test_lef_file_entry_values_data1,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test number_of_types != number_of_values
	 */
	result = libewf_lef_file_entry_read_data(
	          lef_file_entry,
	          types,
	          ewf_test_lef_file_entry_values_data1,
	          166,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* TODO: test with invalid data */

	/* Clean up
	 */
	result = libfvalue_split_utf8_string_free(
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_file_entry_free(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 &lef_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_identifier(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	uint64_t identifier      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_identifier(
	          lef_file_entry,
	          &identifier,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_identifier(
	          NULL,
	          &identifier,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_identifier(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_type(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	uint8_t type             = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_type(
	          lef_file_entry,
	          &type,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_type(
	          NULL,
	          &type,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_type(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_flags function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_flags(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t flags           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_flags(
	          lef_file_entry,
	          &flags,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_flags(
	          NULL,
	          &flags,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_flags(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_data_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_data_offset(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	off64_t data_offset      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_data_offset(
	          lef_file_entry,
	          &data_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_data_offset(
	          NULL,
	          &data_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_data_offset(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_data_size(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	size64_t data_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_data_size(
	          lef_file_entry,
	          &data_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_data_size(
	          NULL,
	          &data_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_data_size(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_logical_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_logical_offset(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	off64_t logical_offset   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_logical_offset(
	          lef_file_entry,
	          &logical_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_logical_offset(
	          NULL,
	          &logical_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_logical_offset(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_physical_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_physical_offset(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	off64_t physical_offset  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_physical_offset(
	          lef_file_entry,
	          &physical_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_physical_offset(
	          NULL,
	          &physical_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_physical_offset(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_duplicate_data_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_duplicate_data_offset(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error      = NULL;
	off64_t duplicate_data_offset = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_duplicate_data_offset(
	          lef_file_entry,
	          &duplicate_data_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_duplicate_data_offset(
	          NULL,
	          &duplicate_data_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_duplicate_data_offset(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf8_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf8_guid_size(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf8_guid_size(
	          lef_file_entry,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf8_guid_size(
	          NULL,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_guid_size(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf8_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf8_guid(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf8_guid(
	          lef_file_entry,
	          utf8_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf8_guid(
	          NULL,
	          utf8_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_guid(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_guid(
	          lef_file_entry,
	          utf8_string,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_guid(
	          lef_file_entry,
	          utf8_string,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf16_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf16_guid_size(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf16_guid_size(
	          lef_file_entry,
	          &utf16_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf16_guid_size(
	          NULL,
	          &utf16_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_guid_size(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf16_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf16_guid(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf16_guid(
	          lef_file_entry,
	          utf16_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf16_guid(
	          NULL,
	          utf16_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_guid(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_guid(
	          lef_file_entry,
	          utf16_string,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_guid(
	          lef_file_entry,
	          utf16_string,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf8_name_size(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf8_name_size(
	          lef_file_entry,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf8_name_size(
	          NULL,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_name_size(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf8_name(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf8_name(
	          lef_file_entry,
	          utf8_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf8_name(
	          NULL,
	          utf8_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_name(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_name(
	          lef_file_entry,
	          utf8_string,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_name(
	          lef_file_entry,
	          utf8_string,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf16_name_size(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf16_name_size(
	          lef_file_entry,
	          &utf16_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf16_name_size(
	          NULL,
	          &utf16_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_name_size(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf16_name(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf16_name(
	          lef_file_entry,
	          utf16_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf16_name(
	          NULL,
	          utf16_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_name(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_name(
	          lef_file_entry,
	          utf16_string,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_name(
	          lef_file_entry,
	          utf16_string,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf8_short_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf8_short_name_size(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf8_short_name_size(
	          lef_file_entry,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf8_short_name_size(
	          NULL,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_short_name_size(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf8_short_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf8_short_name(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf8_short_name(
	          lef_file_entry,
	          utf8_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf8_short_name(
	          NULL,
	          utf8_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_short_name(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_short_name(
	          lef_file_entry,
	          utf8_string,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_short_name(
	          lef_file_entry,
	          utf8_string,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf16_short_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf16_short_name_size(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf16_short_name_size(
	          lef_file_entry,
	          &utf16_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf16_short_name_size(
	          NULL,
	          &utf16_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_short_name_size(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf16_short_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf16_short_name(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf16_short_name(
	          lef_file_entry,
	          utf16_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf16_short_name(
	          NULL,
	          utf16_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_short_name(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_short_name(
	          lef_file_entry,
	          utf16_string,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_short_name(
	          lef_file_entry,
	          utf16_string,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_size(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_size(
	          lef_file_entry,
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_size(
	          NULL,
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_size(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_source_identifier function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_source_identifier(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	int source_identifier    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_source_identifier(
	          lef_file_entry,
	          &source_identifier,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_source_identifier(
	          NULL,
	          &source_identifier,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_source_identifier(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_permission_group_index function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_permission_group_index(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error   = NULL;
	int permission_group_index = 0;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_permission_group_index(
	          lef_file_entry,
	          &permission_group_index,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_permission_group_index(
	          NULL,
	          &permission_group_index,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_permission_group_index(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_record_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_record_type(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t record_type     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_record_type(
	          lef_file_entry,
	          &record_type,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_record_type(
	          NULL,
	          &record_type,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_record_type(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_creation_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_creation_time(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	int64_t creation_time    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_creation_time(
	          lef_file_entry,
	          &creation_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_creation_time(
	          NULL,
	          &creation_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_creation_time(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_modification_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_modification_time(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error  = NULL;
	int64_t modification_time = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_modification_time(
	          lef_file_entry,
	          &modification_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_modification_time(
	          NULL,
	          &modification_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_modification_time(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_access_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_access_time(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	int64_t access_time      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_access_time(
	          lef_file_entry,
	          &access_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_access_time(
	          NULL,
	          &access_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_access_time(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_entry_modification_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_entry_modification_time(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error        = NULL;
	int64_t entry_modification_time = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_entry_modification_time(
	          lef_file_entry,
	          &entry_modification_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_entry_modification_time(
	          NULL,
	          &entry_modification_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_entry_modification_time(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_deletion_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_deletion_time(
     libewf_lef_file_entry_t *lef_file_entry )
{
	libcerror_error_t *error = NULL;
	int64_t deletion_time    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_deletion_time(
	          lef_file_entry,
	          &deletion_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_deletion_time(
	          NULL,
	          &deletion_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_deletion_time(
	          lef_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf8_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf8_hash_value_md5(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint8_t utf8_hash_value_md5[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf8_hash_value_md5(
	          lef_file_entry,
	          utf8_hash_value_md5,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf8_hash_value_md5(
	          NULL,
	          utf8_hash_value_md5,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_hash_value_md5(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_hash_value_md5(
	          lef_file_entry,
	          utf8_hash_value_md5,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_hash_value_md5(
	          lef_file_entry,
	          utf8_hash_value_md5,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf16_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf16_hash_value_md5(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint16_t utf16_hash_value_md5[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf16_hash_value_md5(
	          lef_file_entry,
	          utf16_hash_value_md5,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf16_hash_value_md5(
	          NULL,
	          utf16_hash_value_md5,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_hash_value_md5(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_hash_value_md5(
	          lef_file_entry,
	          utf16_hash_value_md5,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_hash_value_md5(
	          lef_file_entry,
	          utf16_hash_value_md5,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf8_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf8_hash_value_sha1(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint8_t utf8_hash_value_sha1[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf8_hash_value_sha1(
	          lef_file_entry,
	          utf8_hash_value_sha1,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf8_hash_value_sha1(
	          NULL,
	          utf8_hash_value_sha1,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_hash_value_sha1(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_hash_value_sha1(
	          lef_file_entry,
	          utf8_hash_value_sha1,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf8_hash_value_sha1(
	          lef_file_entry,
	          utf8_hash_value_sha1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_file_entry_get_utf16_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_file_entry_get_utf16_hash_value_sha1(
     libewf_lef_file_entry_t *lef_file_entry )
{
	uint16_t utf16_hash_value_sha1[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_file_entry_get_utf16_hash_value_sha1(
	          lef_file_entry,
	          utf16_hash_value_sha1,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_file_entry_get_utf16_hash_value_sha1(
	          NULL,
	          utf16_hash_value_sha1,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_hash_value_sha1(
	          lef_file_entry,
	          NULL,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_hash_value_sha1(
	          lef_file_entry,
	          utf16_hash_value_sha1,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_file_entry_get_utf16_hash_value_sha1(
	          lef_file_entry,
	          utf16_hash_value_sha1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc EWF_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] EWF_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc EWF_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] EWF_TEST_ATTRIBUTE_UNUSED )
#endif
{
#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	libcerror_error_t *error                = NULL;
	libewf_lef_file_entry_t *lef_file_entry = NULL;
	libfvalue_split_utf8_string_t *types    = NULL;
	int result                              = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_lef_file_entry_initialize",
	 ewf_test_lef_file_entry_initialize );

	EWF_TEST_RUN(
	 "libewf_lef_file_entry_free",
	 ewf_test_lef_file_entry_free );

	EWF_TEST_RUN(
	 "libewf_lef_file_entry_clone",
	 ewf_test_lef_file_entry_clone );

	EWF_TEST_RUN(
	 "libewf_lef_file_entry_read_binary_extents",
	 ewf_test_lef_file_entry_read_binary_extents );

	EWF_TEST_RUN(
	 "libewf_lef_file_entry_read_extended_attributes",
	 ewf_test_lef_file_entry_read_extended_attributes );

	EWF_TEST_RUN(
	 "libewf_lef_file_entry_read_short_name",
	 ewf_test_lef_file_entry_read_short_name );

	EWF_TEST_RUN(
	 "libewf_lef_file_entry_read_data",
	 ewf_test_lef_file_entry_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize lef_file_entry for tests
	 */
	result = libewf_lef_file_entry_initialize(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_lef_file_entry_types_data1,
	          107,
	          (uint8_t) '\t',
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_file_entry_read_data(
	          lef_file_entry,
	          types,
	          ewf_test_lef_file_entry_values_data1,
	          4810,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_identifier",
	 ewf_test_lef_file_entry_get_identifier,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_type",
	 ewf_test_lef_file_entry_get_type,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_flags",
	 ewf_test_lef_file_entry_get_flags,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_data_offset",
	 ewf_test_lef_file_entry_get_data_offset,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_data_size",
	 ewf_test_lef_file_entry_get_data_size,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_logical_offset",
	 ewf_test_lef_file_entry_get_logical_offset,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_physical_offset",
	 ewf_test_lef_file_entry_get_physical_offset,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_duplicate_data_offset",
	 ewf_test_lef_file_entry_get_duplicate_data_offset,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf8_guid_size",
	 ewf_test_lef_file_entry_get_utf8_guid_size,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf8_guid",
	 ewf_test_lef_file_entry_get_utf8_guid,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf16_guid_size",
	 ewf_test_lef_file_entry_get_utf16_guid_size,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf16_guid",
	 ewf_test_lef_file_entry_get_utf16_guid,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf8_name_size",
	 ewf_test_lef_file_entry_get_utf8_name_size,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf8_name",
	 ewf_test_lef_file_entry_get_utf8_name,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf16_name_size",
	 ewf_test_lef_file_entry_get_utf16_name_size,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf16_name",
	 ewf_test_lef_file_entry_get_utf16_name,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf8_short_name_size",
	 ewf_test_lef_file_entry_get_utf8_short_name_size,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf8_short_name",
	 ewf_test_lef_file_entry_get_utf8_short_name,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf16_short_name_size",
	 ewf_test_lef_file_entry_get_utf16_short_name_size,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf16_short_name",
	 ewf_test_lef_file_entry_get_utf16_short_name,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_size",
	 ewf_test_lef_file_entry_get_size,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_source_identifier",
	 ewf_test_lef_file_entry_get_source_identifier,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_permission_group_index",
	 ewf_test_lef_file_entry_get_permission_group_index,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_record_type",
	 ewf_test_lef_file_entry_get_record_type,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_creation_time",
	 ewf_test_lef_file_entry_get_creation_time,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_modification_time",
	 ewf_test_lef_file_entry_get_modification_time,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_access_time",
	 ewf_test_lef_file_entry_get_access_time,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_entry_modification_time",
	 ewf_test_lef_file_entry_get_entry_modification_time,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_deletion_time",
	 ewf_test_lef_file_entry_get_deletion_time,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf8_hash_value_md5",
	 ewf_test_lef_file_entry_get_utf8_hash_value_md5,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf16_hash_value_md5",
	 ewf_test_lef_file_entry_get_utf16_hash_value_md5,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf8_hash_value_sha1",
	 ewf_test_lef_file_entry_get_utf8_hash_value_sha1,
	 lef_file_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_file_entry_get_utf16_hash_value_sha1",
	 ewf_test_lef_file_entry_get_utf16_hash_value_sha1,
	 lef_file_entry );

	/* Clean up
	 */
	result = libfvalue_split_utf8_string_free(
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_file_entry_free(
	          &lef_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_file_entry",
	 lef_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )
on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( lef_file_entry != NULL )
	{
		libewf_lef_file_entry_free(
		 &lef_file_entry,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}

