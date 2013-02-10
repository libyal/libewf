#!/bin/bash
#
# ewfacquire testing script for optical disc (split) RAW image input
#
# Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

INPUT="input_raw_optical";
TMP="tmp";

LS="ls";
SED="sed";
TR="tr";
WC="wc";

test_acquire_optical_file()
{ 
	TOC_FILE=$1;
	OUTPUT_FORMAT=$2;
	COMPRESSION_METHOD=$3;
	COMPRESSION_LEVEL=$4;
	MAXIMUM_SEGMENT_SIZE=$5;
	CHUNK_SIZE=$6;

	INPUT_FILES=`echo ${TOC_FILE} | ${SED} 's/_*[0-9]*[.][cC][uU][eE]$//'`;

	mkdir ${TMP};

${EWFACQUIRE} -q -d sha1 -T ${TOC_FILE} ${INPUT_FILES}*.[iI][sS][oO] <<EOI
${TMP}/acquire
case_number
description
evidence_number
examiner
notes
optical
logical
${OUTPUT_FORMAT}
${COMPRESSION_METHOD}
${COMPRESSION_LEVEL}


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

	echo -n "Testing ewfacquire of optical disc raw input: ${TOC_FILE} to ewf format: ${OUTPUT_FORMAT} with compression: ${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} and chunk size: ${CHUNK_SIZE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_acquire_unattended_optical_file()
{ 
	TOC_FILE=$1;
	OUTPUT_FORMAT=$2;
	COMPRESSION_METHOD=$3;
	COMPRESSION_LEVEL=$4;
	MAXIMUM_SEGMENT_SIZE=$5;
	CHUNK_SIZE=$6;

	INPUT_FILES=`echo ${TOC_FILE} | ${SED} 's/_*[0-9]*[.][cC][uU][eE]$//'`;

	mkdir ${TMP};

	${EWFACQUIRE} -q -u -d sha1 \
	-T ${TOC_FILE} \
	-t ${TMP}/unattended_acquire \
	-C case_number \
	-D description \
	-E evidence_number \
	-e examiner \
	-N notes \
	-m optical \
	-M logical \
	-c ${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} \
	-f ${OUTPUT_FORMAT} \
	-S ${MAXIMUM_SEGMENT_SIZE} \
	-b ${CHUNK_SIZE} \
	${INPUT_FILES}*.[iI][sS][oO]

	RESULT=$?;

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
		${EWFVERIFY} -q -d sha1 ${TMP}/unattended_acquire.*

		RESULT=$?;
	fi

	rm -rf ${TMP};

	echo -n "Testing unattended ewfacquire of optical disc raw input: ${TOC_FILE} to ewf format: ${OUTPUT_FORMAT} with compression: ${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} and chunk size: ${CHUNK_SIZE} ";

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
	echo "No ${INPUT} directory found, to test ewfacquire create ${INPUT} directory and place optical disc RAW image test files in directory.";
	echo "Make sure the table of contents file has the same basename as the data files.";

	exit ${EXIT_IGNORE};
fi

RESULT=`${LS} ${INPUT}/*.[cC][uU][eE] | ${TR} ' ' '\n' | ${WC} -l`;

if test ${RESULT} -eq 0;
then
	echo "No files found in ${INPUT} directory, to test ewfacquire place optical disc RAW image test files in directory.";
	echo "Make sure the table of contents file has the same basename as the data files.";

	exit ${EXIT_IGNORE};
fi

for FILENAME in `${LS} ${INPUT}/*.[cC][uU][eE] | ${TR} ' ' '\n'`;
do
	for FORMAT in encase5 encase6;
	do
		for COMPRESSION_LEVEL in none empty-block fast best;
		do
			if ! test_acquire_optical_file "${FILENAME}" "${FORMAT}" deflate "${COMPRESSION_LEVEL}" 650MB 16;
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
				if ! test_acquire_optical_file "${FILENAME}" "${FORMAT}" "${COMPRESSION_METHOD}" "${COMPRESSION_LEVEL}" 650MB 16;
				then
					exit ${EXIT_FAILURE};
				fi
			done
		done
	done
done

for FILENAME in `${LS} ${INPUT}/*.[cC][uU][eE] | ${TR} ' ' '\n'`;
do
	for FORMAT in encase5 encase6;
	do
		for COMPRESSION_LEVEL in not_acquire_optical_filee empty-block fast best;
		do
			if ! test_acquire_unattended_optical_file "${FILENAME}" "${FORMAT}" deflate "${COMPRESSION_LEVEL}" 650MB 16;
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
				if ! test_acquire_unattended_optical_file "${FILENAME}" "${FORMAT}" "${COMPRESSION_METHOD}" "${COMPRESSION_LEVEL}" 650MB 16;
				then
					exit ${EXIT_FAILURE};
				fi
			done
		done
	done
done

exit ${EXIT_SUCCESS};

