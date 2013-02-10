#!/bin/bash
#
# ewfexport testing script for logical evidence files
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

INPUT="input_logical";
TMP="tmp";

LS="ls";
TR="tr";
WC="wc";

test_export_logical()
{ 
	INPUT_FILE=$1;

	mkdir ${TMP};

${EWFEXPORT} -q ${INPUT_FILE} <<EOI
files
${TMP}/export
EOI

	RESULT=$?;

	rm -rf ${TMP};

	echo -n "Testing ewfexport to files of input: ${INPUT_FILE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_export_logical_unattended()
{ 
	OUTPUT_FORMAT=$1;
	INPUT_FILE=$2;

	mkdir ${TMP};

	${EWFEXPORT} -q -u \
	-t ${TMP}/unattended_export \
	-f files \
	${INPUT_FILE}

	RESULT=$?;

	rm -rf ${TMP};

	echo -n "Testing unattended ewfexport to files of input: ${INPUT_FILE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

EWFEXPORT="../ewftools/ewfexport";

if ! test -x ${EWFEXPORT};
then
	EWFEXPORT="../ewftools/ewfexport.exe"
fi

if ! test -x ${EWFEXPORT};
then
	echo "Missing executable: ${EWFEXPORT}";

	exit ${EXIT_FAILURE};
fi

if ! test -d ${INPUT};
then
	echo "No ${INPUT} directory found, to test ewfexport create ${INPUT} directory and place EWF test files in directory.";

	exit ${EXIT_IGNORE};
fi

RESULT=`${LS} ${INPUT}/*.L01 | ${TR} ' ' '\n' | ${WC} -l`;

if test ${RESULT} -eq 0;
then
	echo "No files found in ${INPUT} directory, to test ewfexport place EWF test files in directory.";

	exit ${EXIT_IGNORE};
fi

for FILENAME in `${LS} ${INPUT}/*.L01 | ${TR} ' ' '\n'`;
do
	if ! test_export_logical "${FILENAME}";
	then
		exit ${EXIT_FAILURE};
	fi
done

for FILENAME in `${LS} ${INPUT}/*.L01 | ${TR} ' ' '\n'`;
do
	if ! test_export_logical_unattended "${FORMAT}" "${FILENAME}";
	then
		exit ${EXIT_FAILURE};
	fi
done

exit ${EXIT_SUCCESS};

