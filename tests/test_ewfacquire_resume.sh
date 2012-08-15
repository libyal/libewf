#!/bin/bash
#
# ewfacquire resume testing script
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

INPUT="input_raw";
TMP="tmp";

AWK="awk";
LS="ls";
TR="tr";
WC="wc";

test_write_resume()
{ 
	INPUT_FILE=$1;
	RESUME_OFFSET=$2;

	mkdir ${TMP};

	${EWFACQUIRE} -q -u \
	-t ${TMP}/resume \
	-C case_number \
	-D description \
	-E evidence_number \
	-e examiner \
	-N notes \
	-m removable \
	-M logical \
	-c none \
	-f encase5 \
	-S 650MB \
	-b 64 \
	${INPUT_FILE}

	RESULT=$?;

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
		${EWFVERIFY} -q ${TMP}/resume.E01

		RESULT=$?;
	fi

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
		${EWFTRUNCATE} ${RESUME_OFFSET} ${TMP}/resume.E01

		RESULT=$?;
	fi

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
${EWFACQUIRE} -q -R ${INPUT_FILE} <<EOI
${TMP}/resume.E01


yes
EOI

		RESULT=$?;
	fi

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
		${EWFVERIFY} -q ${TMP}/resume.E01

		RESULT=$?;
	fi

	rm -rf ${TMP};

	if [ ${RESULT} -ne ${EXIT_IGNORE} ];
	then
		echo -n "Testing ewfacquire resume of at offset: ${RESUME_OFFSET}";

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			echo " (FAIL)";
		else
			echo " (PASS)";
		fi
	fi
	return ${RESULT};
}

EWFTRUNCATE="./ewf_test_truncate";

if ! test -x ${EWFTRUNCATE};
then
	echo "Missing executable: ${EWFTRUNCATE}";

	exit ${EXIT_FAILURE};
fi

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
	echo "No ${INPUT} directory found, to test ewfacquire resume create ${INPUT} directory and place RAW image test files in directory.";

	exit ${EXIT_IGNORE};
fi

RESULT=`${LS} ${INPUT}/*.[rR][aA][wW] | ${TR} ' ' '\n' | ${WC} -l`;

if test ${RESULT} -eq 0;
then
	echo "No files found in ${INPUT} directory, to test ewfacquire resume place RAW image test files in directory.";

	exit ${EXIT_IGNORE};
fi

for FILENAME in `${LS} ${INPUT}/*.[rR][aA][wW] | ${TR} ' ' '\n'`;
do
	FILESIZE=`${LS} -l ${FILENAME} | ${AWK} '{ print $5 }'`;

	if [ ${FILESIZE} -ne 1474560 ];
	then
		echo "ewfacquire resume test currently only supports RAW image of: 1474560 bytes.";

		exit ${EXIT_IGNORE};
	fi

	if ! test_write_resume "${FILENAME}" 1478560
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write_resume "${FILENAME}" 1477351
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write_resume "${FILENAME}" 1478432
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write_resume "${FILENAME}" 1477296
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write_resume "${FILENAME}" 1477008
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write_resume "${FILENAME}" 1476736
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write_resume "${FILENAME}" 3584
	then
		exit ${EXIT_FAILURE};
	fi
done

exit ${EXIT_SUCCESS};

