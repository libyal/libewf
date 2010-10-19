#!/bin/bash
#
# ewfinfo testing script
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

LS="ls";
TR="tr";
WC="wc";

function test_info
{ 
	INPUT_FILE=$1;

	echo "Testing ewfinfo of input: ${INPUT_FILE}";

	./${EWFINFO} ${INPUT_FILE};

	RESULT=$?;

	echo "";

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

exit ${EXIT_RESULT};

