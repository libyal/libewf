#!/usr/bin/env python
#
# Python-bindings seek testing program
#
# Copyright (C) 2008-2016, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

from __future__ import print_function
import argparse
import os
import sys

import pyewf


def get_whence_string(whence):
  """Retrieves a human readable string representation of the whence."""
  if whence == os.SEEK_CUR:
    whence_string = "SEEK_CUR"
  elif whence == os.SEEK_END:
    whence_string = "SEEK_END"
  elif whence == os.SEEK_SET:
    whence_string = "SEEK_SET"
  else:
    whence_string = "UNKNOWN"
  return whence_string


def pyewf_test_seek_offset(
    ewf_handle, input_offset, input_whence, expected_offset):

  print("Testing seek of offset: {0:d} and whence: {1:s}\t".format(
      input_offset, get_whence_string(input_whence)), end="")

  error_string = ""
  result = True
  try:
    ewf_handle.seek(input_offset, input_whence)

    result_offset = ewf_handle.get_offset()
    if expected_offset != result_offset:
      result = False

  except Exception as exception:
    error_string = str(exception)
    if expected_offset != -1:
      result = False

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def pyewf_test_seek(ewf_handle):
  media_size = ewf_handle.media_size

  # Test: SEEK_SET offset: 0
  # Expected result: 0
  if not pyewf_test_seek_offset(ewf_handle, 0, os.SEEK_SET, 0):
    return False

  # Test: SEEK_SET offset: <media_size>
  # Expected result: <media_size>
  if not pyewf_test_seek_offset(
      ewf_handle, media_size, os.SEEK_SET, media_size):
    return False

  # Test: SEEK_SET offset: <media_size / 5>
  # Expected result: <media_size / 5>
  seek_offset, _ = divmod(media_size, 5)
  if not pyewf_test_seek_offset(
      ewf_handle, seek_offset, os.SEEK_SET, seek_offset):
    return False

  # Test: SEEK_SET offset: <media_size + 987>
  # Expected result: <media_size + 987>
  seek_offset = media_size + 987
  if not pyewf_test_seek_offset(
      ewf_handle, seek_offset, os.SEEK_SET, seek_offset):
    return False

  # Test: SEEK_SET offset: -987
  # Expected result: -1
  seek_offset = -987
  if not pyewf_test_seek_offset(ewf_handle, seek_offset, os.SEEK_SET, -1):
    return False

  # Test: SEEK_CUR offset: 0
  # Expected result: <media_size + 987>
  if not pyewf_test_seek_offset(ewf_handle, 0, os.SEEK_CUR, media_size + 987):
    return False

  # Test: SEEK_CUR offset: <-1 * (media_size + 987)>
  # Expected result: 0
  if not pyewf_test_seek_offset(
      ewf_handle, -1 * (media_size + 987), os.SEEK_CUR, 0):
    return False

  # Test: SEEK_CUR offset: <media_size / 3>
  # Expected result: <media_size / 3>
  seek_offset, _ = divmod(media_size, 3)
  if not pyewf_test_seek_offset(
      ewf_handle, seek_offset, os.SEEK_CUR, seek_offset):
    return False

  if media_size == 0:
    # Test: SEEK_CUR offset: <-2 * (media_size / 3)>
    # Expected result: 0
    seek_offset, _ = divmod(media_size, 3)
    if not pyewf_test_seek_offset(ewf_handle, -2 * seek_offset, os.SEEK_CUR, 0):
      return False

  else:
    # Test: SEEK_CUR offset: <-2 * (media_size / 3)>
    # Expected result: -1
    seek_offset, _ = divmod(media_size, 3)
    if not pyewf_test_seek_offset(
        ewf_handle, -2 * seek_offset, os.SEEK_CUR, -1):
      return False

  # Test: SEEK_END offset: 0
  # Expected result: <media_size>
  if not pyewf_test_seek_offset(ewf_handle, 0, os.SEEK_END, media_size):
    return False

  # Test: SEEK_END offset: <-1 * media_size>
  # Expected result: 0
  if not pyewf_test_seek_offset(ewf_handle, -1 * media_size, os.SEEK_END, 0):
    return False

  # Test: SEEK_END offset: <-1 * (media_size / 4)>
  # Expected result: <media_size - (media_size / 4)>
  seek_offset, _ = divmod(media_size, 4)
  if not pyewf_test_seek_offset(
      ewf_handle, -1 * seek_offset, os.SEEK_END, media_size - seek_offset):
    return False

  # Test: SEEK_END offset: 542
  # Expected result: <media_size + 542>
  if not pyewf_test_seek_offset(ewf_handle, 542, os.SEEK_END, media_size + 542):
    return False

  # Test: SEEK_END offset: <-1 * (media_size + 542)>
  # Expected result: -1
  if not pyewf_test_seek_offset(
      ewf_handle, -1 * (media_size + 542), os.SEEK_END, -1):
    return False

  # Test: UNKNOWN (88) offset: 0
  # Expected result: -1
  if not pyewf_test_seek_offset(ewf_handle, 0, 88, -1):
    return False

  return True


def pyewf_test_seek_file(filename):
  filenames = pyewf.glob(filename)
  ewf_handle = pyewf.handle()

  ewf_handle.open(filenames, "r")
  result = pyewf_test_seek(ewf_handle)
  ewf_handle.close()

  return result


def pyewf_test_seek_file_object(filename):
  filenames = pyewf.glob(filename)
  file_objects = []
  for filename in filenames:
    file_object = open(filename, "rb")
    file_objects.append(file_object)

  ewf_handle = pyewf.handle()

  ewf_handle.open_file_objects(file_objects, "r")

  result = pyewf_test_seek(ewf_handle)
  ewf_handle.close()

  return result


def pyewf_test_seek_file_no_open(filename):
  print("Testing seek of offset without open:\t", end="")

  ewf_handle = pyewf.handle()

  error_string = ""
  result = False
  try:
    ewf_handle.seek(0, os.SEEK_SET)
  except Exception as exception:
    error_string = str(exception)
    result = True

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def main():
  args_parser = argparse.ArgumentParser(description=(
      "Tests seek."))

  args_parser.add_argument(
      "source", nargs="?", action="store", metavar="FILENAME",
      default=None, help="The source filename.")

  options = args_parser.parse_args()

  if not options.source:
    print("Source value is missing.")
    print("")
    args_parser.print_help()
    print("")
    return False

  if not pyewf_test_seek_file(options.source):
    return False

  if not pyewf_test_seek_file_object(options.source):
    return False

  if not pyewf_test_seek_file_no_open(options.source):
    return False

  return True


if __name__ == "__main__":
  if not main():
    sys.exit(1)
  else:
    sys.exit(0)

