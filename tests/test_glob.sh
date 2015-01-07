#!/bin/bash
#
# Library glob testing script
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

chr()
{
	CHR_VALUE=`expr \( \( $1 / 64 \) \* 100 \) + \( \( \( $1 % 64 \) / 8 \) \* 10 \) + \( $1 % 8 \)`;

	printf \\${CHR_VALUE};
} 

seq()
{
	VALUE=$1;
	SEQUENCE="";

	while [ ${VALUE} -le $2 ];
	do

		if [ ${VALUE} -le 9 ];
		then
			SEQUENCE="${SEQUENCE} 0${VALUE}";
		else
			SEQUENCE="${SEQUENCE} ${VALUE}";
		fi

		VALUE=`expr ${VALUE} + 1`;
	done

	echo ${SEQUENCE};
}

test_glob()
{ 
	BASENAME=$1;
	SCHEMA=$2;
	FILENAMES=$3;

	FILENAMES=`echo ${FILENAMES} | sed 's?^?tmp/?' | sed 's? ? tmp/?g'`;

	rm -rf tmp;
	mkdir tmp;

	echo ${FILENAMES} > tmp/input;

	touch ${FILENAMES};

	${TEST_RUNNER} ./${EWF_TEST_GLOB} tmp/${BASENAME} > tmp/output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		if ! cmp -s tmp/input tmp/output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	rm -rf tmp;

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_glob_sequence3()
{ 
	BASENAME=$1;
	SCHEMA=$2;
	FILENAME=$3;
	LAST=$4;

	RESULT=`echo ${SCHEMA} | egrep "^[.][esEL]01$"`;
	IS_VALID=$?;

	if [ ${IS_VALID} -ne 0 ];
	then
		echo "Unsupported schema: ${SCHEMA}";

		exit ${EXIT_FAILURE};
	fi

	RESULT=`echo ${LAST} | egrep "^[e-zE-Z][0-9a-zA-Z][0-9a-zA-Z]$"`;
	IS_VALID=$?;

	if [ ${IS_VALID} -ne 0 ];
	then
		echo "Unsupported last: ${LAST}";

		exit ${EXIT_FAILURE};
	fi

	FIRST_LETTER=`echo ${SCHEMA} | cut -c 2`;

	RESULT=`echo ${LAST} | egrep "^${FIRST_LETTER}[0-9][0-9]$"`;
	LAST_IS_NUMERIC=$?;

	if [ ${LAST_IS_NUMERIC} -eq 0 ];
	then
		LAST=`echo ${LAST} | cut -c '2 3'`;

		SEQUENCE=`seq 1 ${LAST}`;
	else
		SEQUENCE=`seq 1 99`;
	fi

	FILENAMES=`for NUMBER in ${SEQUENCE}; do echo -n "${FILENAME}.${FIRST_LETTER}${NUMBER} "; echo $FILE; done`;

	if [ ${LAST_IS_NUMERIC} -ne 0 ];
	then
		RESULT=`echo ${LAST} | egrep "^[A-Z][A-Z][A-Z]$"`;
		IS_UPPER_CASE=$?;

		SECOND_ITERATOR=0;
		THIRD_ITERATOR=0;

		if [ ${IS_UPPER_CASE} -eq 0 ];
		then
			if [ ${FIRST_LETTER} = "E" ];
			then
				FIRST_ITERATOR=4;

			elif [ ${FIRST_LETTER} = "L" ];
			then
				FIRST_ITERATOR=11;
			fi

			FIRST_BYTE_VALUE=`expr 65 + ${FIRST_ITERATOR}`;
			SECOND_BYTE_VALUE=`expr 65 + ${SECOND_ITERATOR}`;
			THIRD_BYTE_VALUE=`expr 65 + ${THIRD_ITERATOR}`;
		else
			if [ ${FIRST_LETTER} = "e" ];
			then
				FIRST_ITERATOR=4;

			elif [ ${FIRST_LETTER} = "s" ];
			then
				FIRST_ITERATOR=18;
			fi

			FIRST_BYTE_VALUE=`expr 97 + ${FIRST_ITERATOR}`;
			SECOND_BYTE_VALUE=`expr 97 + ${SECOND_ITERATOR}`;
			THIRD_BYTE_VALUE=`expr 97 + ${THIRD_ITERATOR}`;
		fi

		FIRST_LETTER=`chr ${FIRST_BYTE_VALUE}`;
		SECOND_LETTER=`chr ${SECOND_BYTE_VALUE}`;
		THIRD_LETTER=`chr ${THIRD_BYTE_VALUE}`;

		EXTENSION="${FIRST_LETTER}${SECOND_LETTER}${THIRD_LETTER}";

		until [ ${EXTENSION} = ${LAST} ];
		do
			FILENAMES="${FILENAMES} ${FILENAME}.${EXTENSION}";

			THIRD_ITERATOR=`expr ${THIRD_ITERATOR} + 1`;

			if [ ${THIRD_ITERATOR} -ge 26 ];
			then
				SECOND_ITERATOR=`expr ${SECOND_ITERATOR} + 1`;

				THIRD_ITERATOR=0;
			fi

			if [ ${SECOND_ITERATOR} -ge 26 ];
			then
				FIRST_ITERATOR=`expr ${FIRST_ITERATOR} + 1`;

				SECOND_ITERATOR=0;
			fi

			if [ ${FIRST_ITERATOR} -ge 26 ];
			then
				break;
			fi

			if [ ${IS_UPPER_CASE} -eq 0 ];
			then
				FIRST_BYTE_VALUE=`expr 65 + ${FIRST_ITERATOR}`;
				SECOND_BYTE_VALUE=`expr 65 + ${SECOND_ITERATOR}`;
				THIRD_BYTE_VALUE=`expr 65 + ${THIRD_ITERATOR}`;
			else
				FIRST_BYTE_VALUE=`expr 97 + ${FIRST_ITERATOR}`;
				SECOND_BYTE_VALUE=`expr 97 + ${SECOND_ITERATOR}`;
				THIRD_BYTE_VALUE=`expr 97 + ${THIRD_ITERATOR}`;
			fi

			FIRST_LETTER=`chr ${FIRST_BYTE_VALUE}`;
			SECOND_LETTER=`chr ${SECOND_BYTE_VALUE}`;
			THIRD_LETTER=`chr ${THIRD_BYTE_VALUE}`;

			EXTENSION="${FIRST_LETTER}${SECOND_LETTER}${THIRD_LETTER}";
		done

		FILENAMES="${FILENAMES} ${FILENAME}.${EXTENSION}";
	fi

	FILENAMES=`echo ${FILENAMES} | sed 's?^?tmp/?' | sed 's? ? tmp/?g'`;

	rm -rf tmp;
	mkdir tmp;

	echo ${FILENAMES} > tmp/input;

	touch ${FILENAMES};

	${TEST_RUNNER} ./${EWF_TEST_GLOB} tmp/${BASENAME} > tmp/output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		if ! cmp -s tmp/input tmp/output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	rm -rf tmp;

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_glob_sequence4()
{ 
	BASENAME=$1;
	SCHEMA=$2;
	FILENAME=$3;
	LAST=$4;

	RESULT=`echo ${SCHEMA} | egrep "^[.][EL]x01$"`;
	IS_VALID=$?;

	if [ ${IS_VALID} -ne 0 ];
	then
		echo "Unsupported schema: ${SCHEMA}";

		exit ${EXIT_FAILURE};
	fi

	RESULT=`echo ${LAST} | egrep "^[EL][x-z][0-9A-Z][0-9A-Z]$"`;
	IS_VALID=$?;

	if [ ${IS_VALID} -ne 0 ];
	then
		echo "Unsupported last: ${LAST}";

		exit ${EXIT_FAILURE};
	fi

	FIRST_LETTER=`echo ${SCHEMA} | cut -c 2`;
	SECOND_LETTER=`echo ${SCHEMA} | cut -c 3`;

	RESULT=`echo ${LAST} | egrep "^${FIRST_LETTER}${SECOND_LETTER}[0-9][0-9]$"`;
	LAST_IS_NUMERIC=$?;

	if [ ${LAST_IS_NUMERIC} -eq 0 ];
	then
		LAST=`echo ${LAST} | cut -c '3 4'`;

		SEQUENCE=`seq 1 ${LAST}`;
	else
		SEQUENCE=`seq 1 99`;
	fi

	FILENAMES=`for NUMBER in ${SEQUENCE}; do echo -n "${FILENAME}.${FIRST_LETTER}${SECOND_LETTER}${NUMBER} "; echo $FILE; done`;

	if [ ${LAST_IS_NUMERIC} -ne 0 ];
	then
		SECOND_ITERATOR=23;
		THIRD_ITERATOR=0;
		FOURTH_ITERATOR=0;

		SECOND_BYTE_VALUE=`expr 97 + ${SECOND_ITERATOR}`;
		THIRD_BYTE_VALUE=`expr 65 + ${THIRD_ITERATOR}`;
		FOURTH_BYTE_VALUE=`expr 65 + ${FOURTH_ITERATOR}`;

		SECOND_LETTER=`chr ${SECOND_BYTE_VALUE}`;
		THIRD_LETTER=`chr ${THIRD_BYTE_VALUE}`;
		FOURTH_LETTER=`chr ${FOURTH_BYTE_VALUE}`;

		EXTENSION="${FIRST_LETTER}${SECOND_LETTER}${THIRD_LETTER}${FOURTH_LETTER}";

		until [ ${EXTENSION} = ${LAST} ];
		do
			FILENAMES="${FILENAMES} ${FILENAME}.${EXTENSION}";

			FOURTH_ITERATOR=`expr ${FOURTH_ITERATOR} + 1`;

			if [ ${FOURTH_ITERATOR} -ge 26 ];
			then
				THIRD_ITERATOR=`expr ${THIRD_ITERATOR} + 1`;

				FOURTH_ITERATOR=0;
			fi

			if [ ${THIRD_ITERATOR} -ge 26 ];
			then
				SECOND_ITERATOR=`expr ${SECOND_ITERATOR} + 1`;

				THIRD_ITERATOR=0;
			fi

			if [ ${SECOND_ITERATOR} -ge 26 ];
			then
				break;
			fi

			SECOND_BYTE_VALUE=`expr 97 + ${SECOND_ITERATOR}`;
			THIRD_BYTE_VALUE=`expr 65 + ${THIRD_ITERATOR}`;
			FOURTH_BYTE_VALUE=`expr 65 + ${FOURTH_ITERATOR}`;

			SECOND_LETTER=`chr ${SECOND_BYTE_VALUE}`;
			THIRD_LETTER=`chr ${THIRD_BYTE_VALUE}`;
			FOURTH_LETTER=`chr ${FOURTH_BYTE_VALUE}`;

			EXTENSION="${FIRST_LETTER}${SECOND_LETTER}${THIRD_LETTER}${FOURTH_LETTER}";
		done

		FILENAMES="${FILENAMES} ${FILENAME}.${EXTENSION}";
	fi

	FILENAMES=`echo ${FILENAMES} | sed 's?^?tmp/?' | sed 's? ? tmp/?g'`;

	rm -rf tmp;
	mkdir tmp;

	echo ${FILENAMES} > tmp/input;

	touch ${FILENAMES};

	${TEST_RUNNER} ./${EWF_TEST_GLOB} tmp/${BASENAME} > tmp/output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		if ! cmp -s tmp/input tmp/output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	rm -rf tmp;

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

EWF_TEST_GLOB="ewf_test_glob";

if ! test -x ${EWF_TEST_GLOB};
then
	echo "Missing executable: ${EWF_TEST_GLOB}";

	exit ${EXIT_FAILURE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -x ${TEST_RUNNER};
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -x ${TEST_RUNNER};
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

# .e01

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

if ! test_glob_sequence3 "PREFIX.e01" ".e01" "PREFIX" "eba";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence3 "PREFIX.e01" ".e01" "PREFIX" "faa";
then
	exit ${EXIT_FAILURE};
fi

# .s01

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

if ! test_glob_sequence3 "PREFIX.s01" ".s01" "PREFIX" "sba";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence3 "PREFIX.s01" ".s01" "PREFIX" "taa";
then
	exit ${EXIT_FAILURE};
fi

# .E01

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

if ! test_glob_sequence3 "PREFIX.E01" ".E01" "PREFIX" "EBA";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence3 "PREFIX.E01" ".E01" "PREFIX" "FAA";
then
	exit ${EXIT_FAILURE};
fi

# .L01

if ! test_glob "PREFIX.L01" ".L01" "PREFIX.L01";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.L01" ".L01" "PREFIX.L01 PREFIX.L02 PREFIX.L03 PREFIX.L04 PREFIX.L05 PREFIX.L06 PREFIX.L07 PREFIX.L08 PREFIX.L09";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.L01" ".L01" "PREFIX.L01 PREFIX.L02 PREFIX.L03 PREFIX.L04 PREFIX.L05 PREFIX.L06 PREFIX.L07 PREFIX.L08 PREFIX.L09 PREFIX.L10 PREFIX.L11";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence3 "PREFIX.L01" ".L01" "PREFIX" "LBA";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence3 "PREFIX.L01" ".L01" "PREFIX" "MAA";
then
	exit ${EXIT_FAILURE};
fi

# .Ex01

if ! test_glob "PREFIX.Ex01" ".Ex01" "PREFIX.Ex01";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.Ex01" ".Ex01" "PREFIX.Ex01 PREFIX.Ex02 PREFIX.Ex03 PREFIX.Ex04 PREFIX.Ex05 PREFIX.Ex06 PREFIX.Ex07 PREFIX.Ex08 PREFIX.Ex09";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.Ex01" ".Ex01" "PREFIX.Ex01 PREFIX.Ex02 PREFIX.Ex03 PREFIX.Ex04 PREFIX.Ex05 PREFIX.Ex06 PREFIX.Ex07 PREFIX.Ex08 PREFIX.Ex09 PREFIX.Ex10 PREFIX.Ex11";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence4 "PREFIX.Ex01" ".Ex01" "PREFIX" "ExBA";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence4 "PREFIX.Ex01" ".Ex01" "PREFIX" "EyAA";
then
	exit ${EXIT_FAILURE};
fi

# .Lx01

if ! test_glob "PREFIX.Lx01" ".Lx01" "PREFIX.Lx01";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.Lx01" ".Lx01" "PREFIX.Lx01 PREFIX.Lx02 PREFIX.Lx03 PREFIX.Lx04 PREFIX.Lx05 PREFIX.Lx06 PREFIX.Lx07 PREFIX.Lx08 PREFIX.Lx09";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob "PREFIX.Lx01" ".Lx01" "PREFIX.Lx01 PREFIX.Lx02 PREFIX.Lx03 PREFIX.Lx04 PREFIX.Lx05 PREFIX.Lx06 PREFIX.Lx07 PREFIX.Lx08 PREFIX.Lx09 PREFIX.Lx10 PREFIX.Lx11";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence4 "PREFIX.Lx01" ".Lx01" "PREFIX" "LxBA";
then
	exit ${EXIT_FAILURE};
fi

if ! test_glob_sequence4 "PREFIX.Lx01" ".Lx01" "PREFIX" "LyAA";
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

