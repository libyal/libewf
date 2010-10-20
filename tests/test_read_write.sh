#!/bin/bash
#
# Expert Witness Compression Format (EWF) library read/write testing script
#
# Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TMP="tmp";

function test_read_write
{ 
	WRITE_OFFSET=$1;
	WRITE_SIZE=$2;

	mkdir ${TMP};

	./${EWF_TEST_READ_WRITE} -B ${WRITE_SIZE} -o ${WRITE_OFFSET} ${TMP}/read_write;

	RESULT=$?;

	rm -rf ${TMP};

	echo -n "Testing read/write with offset: ${WRITE_OFFSET} and size: ${WRITE_SIZE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

EWF_TEST_READ_WRITE="ewf_test_read_write";

if ! test -x ${EWF_TEST_READ_WRITE};
then
	EWF_TEST_READ_WRITE="ewf_test_read_write.exe";
fi

if ! test -x ${EWF_TEST_READ_WRITE};
then
	echo "Missing executable: ${EWF_TEST_READ_WRITE}";

	exit ${EXIT_FAILURE};
fi

if ! test_read_write 0 0
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

