#!/bin/bash
#
# ewfexport testing script
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

INPUT="input";
TMP="tmp";

LS="ls";
TR="tr";
WC="wc";

test_export_raw()
{ 
	INPUT_FILE=$1;

	mkdir ${TMP};

${EWFEXPORT} -q -d sha1 ${INPUT_FILE} <<EOI
raw
${TMP}/export.raw



EOI

	RESULT=$?;

	rm -rf ${TMP};

	echo -n "Testing ewfexport to raw of input: ${INPUT_FILE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_export_ewf()
{ 
	INPUT_FILE=$1;
	OUTPUT_FORMAT=$2;
	COMPRESSION_METHOD=$3;
	COMPRESSION_LEVEL=$4;

	mkdir ${TMP};

${EWFEXPORT} -q -d sha1 ${INPUT_FILE} <<EOI
${OUTPUT_FORMAT}
${TMP}/export
${COMPRESSION_METHOD}
${COMPRESSION_LEVEL}




EOI

	RESULT=$?;

	rm -rf ${TMP};

	echo -n "Testing ewfexport of input: ${INPUT_FILE} to ewf format: ${OUTPUT_FORMAT} with compression: ${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_export_unattended()
{ 
	INPUT_FILE=$1;
	OUTPUT_FORMAT=$2;
	COMPRESSION_METHOD=$3;
	COMPRESSION_LEVEL=$4;

	mkdir ${TMP};

	${EWFEXPORT} -q -u -d sha1 \
	-c ${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} \
	-f ${OUTPUT_FORMAT} \
	-t ${TMP}/unattended_export \
	${INPUT_FILE}

	RESULT=$?;

	rm -rf ${TMP};

	echo -n "Testing ewfexport of input: ${INPUT_FILE} to ewf format: ${OUTPUT_FORMAT} with compression: ${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} ";

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

	for FORMAT in ewf encase1 encase2 encase3 encase4 encase5 encase6 encase7 linen5 linen6 linen7 ftk smart ewfx;
	do
		if ! test_export_ewf "${FILENAME}" "${FORMAT}" deflate none;
		then
			exit ${EXIT_FAILURE};
		fi
	done

	for FORMAT in encase7-v2;
	do
		for COMPRESSION_METHOD in deflate bzip2;
		do
			if ! test_export_ewf "${FILENAME}" "${FORMAT}" "${COMPRESSION_METHOD}" none;
			then
				exit ${EXIT_FAILURE};
			fi
		done
	done
done

for FILENAME in `${LS} ${INPUT}/*.[esE]01 | ${TR} ' ' '\n'`;
do
	for FORMAT in raw ewf encase1 encase2 encase3 encase4 encase5 encase6 encase7 linen5 linen6 linen7 ftk smart ewfx;
	do
		if ! test_export_unattended "${FILENAME}" "${FORMAT}" deflate none;
		then
			exit ${EXIT_FAILURE};
		fi
	done

	for FORMAT in encase7-v2;
	do
		for COMPRESSION_METHOD in deflate bzip2;
		do
			if ! test_export_unattended "${FILENAME}" "${FORMAT}" "${COMPRESSION_METHOD}" none;
			then
				exit ${EXIT_FAILURE};
			fi
		done
	done
done

exit ${EXIT_SUCCESS};

