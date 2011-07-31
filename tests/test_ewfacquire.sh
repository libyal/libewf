#!/bin/bash
#
# ewfacquire testing script for (split) RAW image input
#
# Copyright (c) 2010-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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
INPUT_MORE="input_raw_more";
TMP="tmp";

LS="ls";
TR="tr";
WC="wc";

test_acquire_file()
{ 
	INPUT_FILE=$1;
	OUTPUT_FORMAT=$2;
	COMPRESSION_LEVEL=$3;
	MAXIMUM_SEGMENT_SIZE=$4;
	CHUNK_SIZE=$5;

	mkdir ${TMP};

${EWFACQUIRE} -q -d sha1 ${INPUT_FILE} <<EOI
${TMP}/acquire
case_number
description
evidence_number
examiner
notes
removable
logical
${COMPRESSION_LEVEL}
${OUTPUT_FORMAT}


${MAXIMUM_SEGMENT_SIZE}

${CHUNK_SIZE}


2
no
yes
EOI

	RESULT=$?;

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
		${EWFVERIFY} -q -d sha1 ${TMP}/acquire.*

		RESULT=$?;
	fi

	echo "";

	rm -rf ${TMP};

	echo -n "Testing ewfacquire of raw input: ${INPUT_FILE} to ewf format: ${OUTPUT_FORMAT} with compression: ${COMPRESSION_LEVEL} and chunk size: ${CHUNK_SIZE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_acquire_unattended_file()
{ 
	INPUT_FILE=$1;
	OUTPUT_FORMAT=$2;
	COMPRESSION_LEVEL=$3;
	MAXIMUM_SEGMENT_SIZE=$4;
	CHUNK_SIZE=$5;

	mkdir ${TMP};

	${EWFACQUIRE} -q -u -d sha1 \
	-t ${TMP}/unattended_acquire \
	-C case_number \
	-D description \
	-E evidence_number \
	-e examiner \
	-N notes \
	-m removable \
	-M logical \
	-c ${COMPRESSION_LEVEL} \
	-f ${OUTPUT_FORMAT} \
	-S ${MAXIMUM_SEGMENT_SIZE} \
	-b ${CHUNK_SIZE} \
	${INPUT_FILE}

	RESULT=$?;

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
		${EWFVERIFY} -q -d sha1 ${TMP}/unattended_acquire.*

		RESULT=$?;
	fi

	rm -rf ${TMP};

	echo -n "Testing unattended ewfacquire of raw input: ${INPUT_FILE} to ewf format: ${OUTPUT_FORMAT} with compression: ${COMPRESSION_LEVEL} and chunk size: ${CHUNK_SIZE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

EWFACQUIRE="../ewftools/ewfacquire";

if ! test -x ${EWFACQUIRE};
then
	EWFACQUIRE="../ewftools/ewfacquire.exe"
fi

if ! test -x ${EWFACQUIRE};
then
	echo "Missing executable: ${EWFACQUIRE}";

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
	echo "No ${INPUT} directory found, to test ewfacquire create ${INPUT} directory and place RAW image test files in directory.";

	exit ${EXIT_IGNORE};
fi

RESULT=`${LS} ${INPUT}/*.[rR][aA][wW] | ${TR} ' ' '\n' | ${WC} -l`;

if test ${RESULT} -eq 0;
then
	echo "No files found in ${INPUT} directory, to test ewfacquire place RAW image test files in directory.";

	exit ${EXIT_IGNORE};
fi

for FILENAME in `${LS} ${INPUT}/*.[rR][aA][wW] | ${TR} ' ' '\n'`;
do
	for FORMAT in ewf encase1 encase2 encase3 encase4 encase5 encase6 linen5 linen6 ftk smart ewfx;
	do
		for SEGMENT_SIZE in 650MB 1MiB;
		do
			if ! test_acquire_file "${FILENAME}" "${FORMAT}" none "${SEGMENT_SIZE}" 64;
			then
				exit ${EXIT_FAILURE};
			fi

			if ! test_acquire_file "${FILENAME}" "${FORMAT}" empty-block "${SEGMENT_SIZE}" 64;
			then
				exit ${EXIT_FAILURE};
			fi

			if ! test_acquire_file "${FILENAME}" "${FORMAT}" fast "${SEGMENT_SIZE}" 64;
			then
				exit ${EXIT_FAILURE};
			fi

			if ! test_acquire_file "${FILENAME}" "${FORMAT}" best "${SEGMENT_SIZE}" 64;
			then
				exit ${EXIT_FAILURE};
			fi
		done
	done

	for CHUNK_SIZE in 16 32 128 256 512 1024 2048 4096 8192 16384 32768;
	do
		for SEGMENT_SIZE in 650MB 1MiB;
		do
			if ! test_acquire_file "${FILENAME}" encase6 none "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
			then
				exit ${EXIT_FAILURE};
			fi

			if ! test_acquire_file "${FILENAME}" encase6 empty-block "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
			then
				exit ${EXIT_FAILURE};
			fi

			if ! test_acquire_file "${FILENAME}" encase6 fast "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
			then
				exit ${EXIT_FAILURE};
			fi

			if ! test_acquire_file "${FILENAME}" encase6 best "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
			then
				exit ${EXIT_FAILURE};
			fi
		done
	done

	for CHUNK_SIZE in 16 32 128 256 512 1024 2048 4096 8192 16384 32768;
	do
		for SEGMENT_SIZE in 650MB 1MiB;
		do
			if ! test_acquire_file "${FILENAME}" smart none "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
			then
				exit ${EXIT_FAILURE};
			fi

			if ! test_acquire_file "${FILENAME}" smart empty-block "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
			then
				exit ${EXIT_FAILURE};
			fi

			if ! test_acquire_file "${FILENAME}" smart fast "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
			then
				exit ${EXIT_FAILURE};
			fi

			if ! test_acquire_file "${FILENAME}" smart best "${SEGMENT_SIZE}" "${CHUNK_SIZE}";
			then
				exit ${EXIT_FAILURE};
			fi
		done
	done
done

for FILENAME in `${LS} ${INPUT}/*.[rR][aA][wW] | ${TR} ' ' '\n'`;
do
	for FORMAT in ewf encase1 encase2 encase3 encase4 encase5 encase6 linen5 linen6 ftk smart ewfx;
	do
		if ! test_acquire_unattended_file "${FILENAME}" "${FORMAT}" none 650MB 64;
		then
			exit ${EXIT_FAILURE};
		fi

		if ! test_acquire_unattended_file "${FILENAME}" "${FORMAT}" empty-block 650MB 64;
		then
			exit ${EXIT_FAILURE};
		fi

		if ! test_acquire_unattended_file "${FILENAME}" "${FORMAT}" fast 650MB 64;
		then
			exit ${EXIT_FAILURE};
		fi

		if ! test_acquire_unattended_file "${FILENAME}" "${FORMAT}" best 650MB 64;
		then
			exit ${EXIT_FAILURE};
		fi
	done

	for CHUNK_SIZE in 16 32 128 256 512 1024 2048 4096 8192 16384 32768;
	do
		if ! test_acquire_unattended_file "${FILENAME}" encase6 none 650MB "${CHUNK_SIZE}";
		then
			exit ${EXIT_FAILURE};
		fi

		if ! test_acquire_unattended_file "${FILENAME}" encase6 empty-block 650MB "${CHUNK_SIZE}";
		then
			exit ${EXIT_FAILURE};
		fi

		if ! test_acquire_unattended_file "${FILENAME}" encase6 fast 650MB "${CHUNK_SIZE}";
		then
			exit ${EXIT_FAILURE};
		fi

		if ! test_acquire_unattended_file "${FILENAME}" encase6 best 650MB "${CHUNK_SIZE}";
		then
			exit ${EXIT_FAILURE};
		fi
	done

	for CHUNK_SIZE in 16 32 128 256 512 1024 2048 4096 8192 16384 32768;
	do
		if ! test_acquire_unattended_file "${FILENAME}" smart none 650MB "${CHUNK_SIZE}";
		then
			exit ${EXIT_FAILURE};
		fi

		if ! test_acquire_unattended_file "${FILENAME}" smart empty-block 650MB "${CHUNK_SIZE}";
		then
			exit ${EXIT_FAILURE};
		fi

		if ! test_acquire_unattended_file "${FILENAME}" smart fast 650MB "${CHUNK_SIZE}";
		then
			exit ${EXIT_FAILURE};
		fi

		if ! test_acquire_unattended_file "${FILENAME}" smart best 650MB "${CHUNK_SIZE}";
		then
			exit ${EXIT_FAILURE};
		fi
	done
done

for FILENAME in `${LS} ${INPUT_MORE}/*.[rR][aA][wW] | ${TR} ' ' '\n'`;
do
	if ! test_acquire_file "${FILENAME}" encase6 none 100GB 64;
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_acquire_file "${FILENAME}" encase6 empty-block 100GB 64;
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_acquire_file "${FILENAME}" encase6 fast 100GB 64;
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_acquire_file "${FILENAME}" encase6 best 100GB 64;
	then
		exit ${EXIT_FAILURE};
	fi
done

exit ${EXIT_SUCCESS};

