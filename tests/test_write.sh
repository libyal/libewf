#!/bin/bash
#
# Expert Witness Compression Format (EWF) library write testing script
#
# Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

CUT="cut";

test_write()
{ 
	MEDIA_SIZE=$1;
	MAXIMUM_SEGMENT_SIZE=$2;
	COMPRESSION_LEVEL=$3;

	mkdir ${TMP};

	./${EWF_TEST_WRITE} -B ${MEDIA_SIZE} -c `echo ${COMPRESSION_LEVEL} | ${CUT} -c 1` -S ${MAXIMUM_SEGMENT_SIZE} ${TMP}/write;

	RESULT=$?;

	rm -rf ${TMP};

	echo -n "Testing write with media size: ${MEDIA_SIZE}, maximum segment size: ${MAXIMUM_SEGMENT_SIZE} and compression level: ${COMPRESSION_LEVEL} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_write_chunk()
{ 
	MEDIA_SIZE=$1;
	MAXIMUM_SEGMENT_SIZE=$2;
	COMPRESSION_LEVEL=$3;

	mkdir ${TMP};

	./${EWF_TEST_WRITE_CHUNK} -B ${MEDIA_SIZE} -c `echo ${COMPRESSION_LEVEL} | ${CUT} -c 1` -S ${MAXIMUM_SEGMENT_SIZE} ${TMP}/write;

	RESULT=$?;

	rm -rf ${TMP};

	echo -n "Testing write chunk with media size: ${MEDIA_SIZE}, maximum segment size: ${MAXIMUM_SEGMENT_SIZE} and compression level: ${COMPRESSION_LEVEL} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

EWF_TEST_WRITE="ewf_test_write";

if ! test -x ${EWF_TEST_WRITE};
then
	EWF_TEST_WRITE="ewf_test_write.exe";
fi

if ! test -x ${EWF_TEST_WRITE};
then
	echo "Missing executable: ${EWF_TEST_WRITE}";

	exit ${EXIT_FAILURE};
fi

EWF_TEST_WRITE_CHUNK="ewf_test_write_chunk";

if ! test -x ${EWF_TEST_WRITE_CHUNK};
then
	EWF_TEST_WRITE_CHUNK="ewf_test_write_chunk.exe";
fi

if ! test -x ${EWF_TEST_WRITE_CHUNK};
then
	echo "Missing executable: ${EWF_TEST_WRITE_CHUNK}";

	exit ${EXIT_FAILURE};
fi

for COMPRESSION_LEVEL in none empty-block fast best;
do
	if ! test_write 0 0 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write 0 10000 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write 100000 0 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write 100000 10000 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	echo "";

	if ! test_write_chunk 0 0 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write_chunk 0 10000 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write_chunk 100000 0 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write_chunk 100000 10000 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	echo "";
done

exit ${EXIT_SUCCESS};

