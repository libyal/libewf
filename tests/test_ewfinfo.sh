#!/bin/bash
#
# ewfinfo testing script
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

INPUT="input";
INPUT_LOGICAL="input_logical";
INPUT_OPTICAL="input_optical";

LS="ls";
TR="tr";
WC="wc";

test_info()
{ 
	INPUT_FILE=$1;

	${EWFINFO} ${INPUT_FILE};

	RESULT=$?;

	echo -n "Testing ewfinfo of input: ${INPUT_FILE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

EWFINFO="../ewftools/ewfinfo";

if ! test -x ${EWFINFO};
then
	EWFINFO="../ewftools/ewfinfo.exe";
fi

if ! test -x ${EWFINFO};
then
	echo "Missing executable: ${EWFINFO}";

	exit ${EXIT_FAILURE};
fi

if ! test -d ${INPUT};
then
	echo "No ${INPUT} directory found, to test ewfinfo create ${INPUT} directory and place EWF test files in directory.";

	exit ${EXIT_IGNORE};
fi

EXIT_RESULT=${EXIT_IGNORE};

if test -d ${INPUT};
then
	RESULT=`${LS} ${INPUT}/*.[esE]01 | ${TR} ' ' '\n' | ${WC} -l`;

	if test ${RESULT} -eq 0;
	then
		echo "No files found in ${INPUT} directory, to test ewfinfo place EWF test files in directory.";
	else
		for FILENAME in `${LS} ${INPUT}/*.[esE]01 | ${TR} ' ' '\n'`;
		do
			if ! test_info "${FILENAME}";
			then
				exit ${EXIT_FAILURE};
			fi
		done

		EXIT_RESULT=${EXIT_SUCCESS};
	fi
fi

if test -d ${INPUT_LOGICAL};
then
	RESULT=`${LS} ${INPUT_LOGICAL}/*.[esE]01 | ${TR} ' ' '\n' | ${WC} -l`;

	if test ${RESULT} -eq 0;
	then
		echo "No files found in ${INPUT_LOGICAL} directory, to test ewfinfo place EWF test files in directory.";
	else
		for FILENAME in `${LS} ${INPUT_LOGICAL}/*.[esE]01 | ${TR} ' ' '\n'`;
		do
			if ! test_info "${FILENAME}";
			then
				exit ${EXIT_FAILURE};
			fi
		done

		EXIT_RESULT=${EXIT_SUCCESS};
	fi
fi

if test -d ${INPUT_OPTICAL};
then
	RESULT=`${LS} ${INPUT_OPTICAL}/*.[esE]01 | ${TR} ' ' '\n' | ${WC} -l`;

	if test ${RESULT} -eq 0;
	then
		echo "No files found in ${INPUT_OPTICAL} directory, to test ewfinfo place EWF test files in directory.";
	else
		for FILENAME in `${LS} ${INPUT_OPTICAL}/*.[esE]01 | ${TR} ' ' '\n'`;
		do
			if ! test_info "${FILENAME}";
			then
				exit ${EXIT_FAILURE};
			fi
		done

		EXIT_RESULT=${EXIT_SUCCESS};
	fi
fi

exit ${EXIT_RESULT};

