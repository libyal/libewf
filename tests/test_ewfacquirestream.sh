#!/bin/bash
#
# ewfacquirestream testing script
#
# Copyright (C) 2006-2015, Joachim Metz <joachim.metz@gmail.com>
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

INPUT="input_raw";
TMP="tmp";

LS="ls";
TR="tr";
WC="wc";

test_acquire_file()
{ 
	INPUT_FILE=$1;
	OUTPUT_FORMAT=$2;
	COMPRESSION_METHOD=$3;
	COMPRESSION_LEVEL=$4;
	MAXIMUM_SEGMENT_SIZE=$5;
	CHUNK_SIZE=$6;

	mkdir ${TMP};

	${EWFACQUIRESTREAM} -q -d sha1 \
	-t ${TMP}/acquirestream \
	-C case_number \
	-D description \
	-E evidence_number \
	-e examiner \
	-N notes \
	-m removable \
	-M logical \
	-c ${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} \
	-f ${OUTPUT_FORMAT} \
	-S ${MAXIMUM_SEGMENT_SIZE} \
	-b ${CHUNK_SIZE} \
	< ${INPUT_FILE}

	RESULT=$?;

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
		${EWFVERIFY} -q -d sha1 ${TMP}/acquirestream.*

		RESULT=$?;
	fi

	echo "";

	rm -rf ${TMP};

	echo -n "Testing ewfacquirestream of raw input: ${INPUT_FILE} to ewf format: ${OUTPUT_FORMAT} with compression: ${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} and chunk size: ${CHUNK_SIZE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

EWFACQUIRESTREAM="../ewftools/ewfacquirestream";

if ! test -x ${EWFACQUIRESTREAM};
then
	EWFACQUIRESTREAM="../ewftools/ewfacquirestream.exe"
fi

if ! test -x ${EWFACQUIRESTREAM};
then
	echo "Missing executable: ${EWFACQUIRESTREAM}";

	exit ${EXIT_FAILURE};
fi

EWFVERIFY="../ewftools/ewfverify";

if ! test -x ${EWFVERIFY};
then
	EWFVERIFY="../ewftools/ewfverify.exe";
fi

if ! test -x ${EWFVERIFY};
then
	echo "Missing executable: ${EWFVERIFY}";

	exit ${EXIT_FAILURE};
fi

if ! test -d ${INPUT};
then
	echo "No ${INPUT} directory found, to test ewfacquirestream create ${INPUT} directory and place RAW image test files in directory.";

	exit ${EXIT_IGNORE};
fi

RESULT=`${LS} ${INPUT}/*.[rR][aA][wW] | ${TR} ' ' '\n' | ${WC} -l`;

if test ${RESULT} -eq 0;
then
	echo "No files found in ${INPUT} directory, to test ewfacquirestream place RAW image test files in directory.";

	exit ${EXIT_IGNORE};
fi

for FILENAME in `${LS} ${INPUT}/*.[rR][aA][wW] | ${TR} ' ' '\n'`;
do
	for SEGMENT_SIZE in 650MB 1MiB;
	do
		for FORMAT in encase2 encase3 encase4 encase5 encase6 encase7 linen5 linen6 linen7 ftk ewfx;
		do
			for COMPRESSION_LEVEL in none empty-block fast best;
			do
				if ! test_acquire_file "${FILENAME}" "${FORMAT}" deflate "${COMPRESSION_LEVEL}" "${SEGMENT_SIZE}" 64;
				then
					exit ${EXIT_FAILURE};
				fi
			done
		done

		for FORMAT in encase7-v2;
		do
			for COMPRESSION_METHOD in deflate bzip2;
			do
				for COMPRESSION_LEVEL in none empty-block fast best;
				do
					if ! test_acquire_file "${FILENAME}" "${FORMAT}" "${COMPRESSION_METHOD}" "${COMPRESSION_LEVEL}" "${SEGMENT_SIZE}" 64;
					then
						exit ${EXIT_FAILURE};
					fi
				done
			done
		done
	done

	for CHUNK_SIZE in 16 32 128 256 512 1024 2048 4096 8192 16384 32768;
	do
		for SEGMENT_SIZE in 650MB 1MiB;
		do
			for COMPRESSION_LEVEL in none empty-block fast best;
			do
				if ! test_acquire_file "${FILENAME}" encase6 deflate "${COMPRESSION_LEVEL}" "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
				then
					exit ${EXIT_FAILURE};
				fi
			done

			for COMPRESSION_METHOD in deflate bzip2;
			do
				for COMPRESSION_LEVEL in none empty-block fast best;
				do
					if ! test_acquire_file "${FILENAME}" encase7-v2 "${COMPRESSION_METHOD}" "${COMPRESSION_LEVEL}" "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
					then
						exit ${EXIT_FAILURE};
					fi
				done
			done
		done
	done
done

exit ${EXIT_SUCCESS};

