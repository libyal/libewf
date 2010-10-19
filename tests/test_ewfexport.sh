#!/bin/bash
#
# ewfexport testing script
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

INPUT="input";
TMP="tmp";

LS="ls";
TR="tr";
WC="wc";

function test_export_raw
{ 
	INPUT_FILE=$1;

	echo "Testing ewfexport to raw of input: ${INPUT_FILE}";

	mkdir ${TMP};

${EWFEXPORT} -q -d sha1 ${INPUT_FILE} <<EOI
raw
tmp/export.raw



EOI

	RESULT=$?;

	rm -rf ${TMP};

	return ${RESULT};
}

function test_export_ewf
{ 
	OUTPUT_FORMAT=$1;
	INPUT_FILE=$2;

	echo "Testing ewfexport to ewf format: ${OUTPUT_FORMAT} of input: ${INPUT_FILE}";

	mkdir ${TMP};

${EWFEXPORT} -q -d sha1 ${INPUT_FILE} <<EOI
${OUTPUT_FORMAT}
tmp/export
none
no




EOI

	RESULT=$?;

	rm -rf ${TMP};

	return ${RESULT};
}

function test_export_unattended
{ 
	OUTPUT_FORMAT=$1;
	INPUT_FILE=$2;

	echo "Testing unattended ewfexport to format: ${OUTPUT_FORMAT} of input: ${INPUT_FILE}";

	mkdir ${TMP};

	${EWFEXPORT} -q -u -d sha1 \
	-t tmp/unattended_export \
	-f ${OUTPUT_FORMAT} \
	${INPUT_FILE}

	RESULT=$?;

	rm -rf ${TMP};

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

RESULT=`${LS} ${INPUT}/*.[esE]01 | ${TR} ' ' '\n' | ${WC} -l`;

if test ${RESULT} -eq 0;
then
	echo "No files found in ${INPUT} directory, to test ewfexport place EWF test files in directory.";

	exit ${EXIT_IGNORE};
fi

for FILENAME in `${LS} ${INPUT}/*.[esE]01 | ${TR} ' ' '\n'`;
do
	if ! test_export_raw "${FILENAME}";
	then
		exit ${EXIT_FAILURE};
	fi

	for FORMAT in ewf encase1 encase2 encase3 encase4 encase5 encase6 linen5 linen6 ftk smart ewfx;
	do
		if ! test_export_ewf "${FORMAT}" "${FILENAME}";
		then
			exit ${EXIT_FAILURE};
		fi
	done
done

for FILENAME in `${LS} ${INPUT}/*.[esE]01 | ${TR} ' ' '\n'`;
do
	for FORMAT in raw ewf encase1 encase2 encase3 encase4 encase5 encase6 linen5 linen6 ftk smart ewfx;
	do
		if ! test_export_unattended "${FORMAT}" "${FILENAME}";
		then
			exit ${EXIT_FAILURE};
		fi
	done
done

exit ${EXIT_SUCCESS};

