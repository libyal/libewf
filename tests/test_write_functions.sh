#!/bin/bash
# Library API write functions testing script
#
# Version: 20171107

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_FUNCTIONS="write write_chunk";
OPTION_SETS="";

test_api_write_function()
{ 
	local TEST_FUNCTION=$1;
	shift 1;
	local ARGUMENTS=("$@");

	local TEST_TOOL="ewf_test_${TEST_FUNCTION}";

	local TEST_EXECUTABLE="./${TEST_TOOL}";

	if ! test -x "${TEST_EXECUTABLE}";
	then
		TEST_EXECUTABLE="./${TEST_TOOL}.exe";
	fi

	if ! test -x "${TEST_EXECUTABLE}";
	then
		echo "Missing test executable: ${TEST_EXECUTABLE}";

		return ${EXIT_FAILURE};
	fi
	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	TEST_DESCRIPTION="Testing write function: libewf_${TEST_FUNCTION}";

	if test "${OSTYPE}" = "msys";
	then
		OUTPUT_FILE="${TMPDIR}\\write";
	else
		OUTPUT_FILE="${TMPDIR}/write";
	fi
	run_test_with_arguments "${TEST_DESCRIPTION}" "${TEST_EXECUTABLE}" ${ARGUMENTS[@]} "${OUTPUT_FILE}";

	RESULT=$?;

	rm -rf ${TMPDIR};

	return ${RESULT};
}

test_write()
{
	local TEST_PROFILE=$1;
	local TEST_FUNCTION=$2;
	local OPTION_SETS=$3;

	for COMPRESSION_LEVEL in none empty-block fast best;
	do
		COMPRESSION_LEVEL=`echo ${COMPRESSION_LEVEL} | cut -c 1`;

		test_api_write_function "${TEST_FUNCTION}" -B0 -c${COMPRESSION_LEVEL} -S0;
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			return ${RESULT};
		fi

		test_api_write_function "${TEST_FUNCTION}" -B0 -c${COMPRESSION_LEVEL} -S10000;
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			return ${RESULT};
		fi

		test_api_write_function "${TEST_FUNCTION}" -B100000 -c${COMPRESSION_LEVEL} -S0;
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			return ${RESULT};
		fi

		test_api_write_function "${TEST_FUNCTION}" -B100000 -c${COMPRESSION_LEVEL} -S10000;
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			return ${RESULT};
		fi

		echo "";
	done

	return ${RESULT};
}

if ! test -z ${SKIP_LIBRARY_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

for TEST_FUNCTION in ${TEST_FUNCTIONS};
do
	test_write "libewf" "${TEST_FUNCTION}" "${OPTION_SETS}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

exit ${RESULT};

