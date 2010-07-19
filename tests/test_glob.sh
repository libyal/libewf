#!/bin/bash
#
# Expert Witness Compression Format (EWF) library glob testing script
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

TMP="tmp";

CMP="cmp";
SEQ="seq";

EWF_TEST_GLOB="ewf_test_glob";

function test_glob
{ 
	BASENAME=$1;
	SCHEMA=$2;
	FILENAMES=$3;

	mkdir ${TMP};
	cd ${TMP};

	echo ${FILENAMES} > input;

	touch ${FILENAMES};

	../${EWF_TEST_GLOB} ${BASENAME} > output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		if ! ${CMP} -s input output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	cd ..;
	rm -rf ${TMP};

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

function test_glob_sequence
{ 
	BASENAME=$1;
	SCHEMA=$2;
	FILENAME=$3;
	LAST=$4;

	SEQUENCE=`${SEQ} 1 ${LAST}`;
	FILENAMES=`for NUMBER in ${SEQUENCE}; do echo -n "${FILENAME}.${NUMBER}of${LAST} "; echo $FILE; done`;

	mkdir ${TMP};
	cd ${TMP};

	echo ${FILENAMES} > input;

	touch ${FILENAMES};

	../${EWF_TEST_GLOB} ${BASENAME} > output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		if ! ${CMP} -s input output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	cd ..;
	rm -rf ${TMP};

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

if ! test -x ${EWF_TEST_GLOB};
then
	echo "Missing executable: ${EWF_TEST_GLOB}";

	exit ${EXIT_FAILURE};
fi

rm -rf ${TMP};

if ! test_glob "PREFIX.e01" ".e01" "PREFIX.e01";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.e01" ".e01" "PREFIX.e01 PREFIX.e02 PREFIX.e03 PREFIX.e04 PREFIX.e05 PREFIX.e06 PREFIX.e07 PREFIX.e08 PREFIX.e09";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.e01" ".e01" "PREFIX.e01 PREFIX.e02 PREFIX.e03 PREFIX.e04 PREFIX.e05 PREFIX.e06 PREFIX.e07 PREFIX.e08 PREFIX.e09 PREFIX.e10 PREFIX.e11";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.E01" ".E01" "PREFIX.E01";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.E01" ".E01" "PREFIX.E01 PREFIX.E02 PREFIX.E03 PREFIX.E04 PREFIX.E05 PREFIX.E06 PREFIX.E07 PREFIX.E08 PREFIX.E09";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.E01" ".E01" "PREFIX.E01 PREFIX.E02 PREFIX.E03 PREFIX.E04 PREFIX.E05 PREFIX.E06 PREFIX.E07 PREFIX.E08 PREFIX.E09 PREFIX.E10 PREFIX.E11";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.s01" ".s01" "PREFIX.s01";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.s01" ".s01" "PREFIX.s01 PREFIX.s02 PREFIX.s03 PREFIX.s04 PREFIX.s05 PREFIX.s06 PREFIX.s07 PREFIX.s08 PREFIX.s09";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.s01" ".s01" "PREFIX.s01 PREFIX.s02 PREFIX.s03 PREFIX.s04 PREFIX.s05 PREFIX.s06 PREFIX.s07 PREFIX.s08 PREFIX.s09 PREFIX.s10 PREFIX.s11";
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

