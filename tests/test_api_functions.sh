#!/bin/bash
# Library API functions testing script
#
# Version: 20160328

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\)/\1/'`;

TEST_PROFILE="lib${TEST_PREFIX}";
TEST_FUNCTIONS="get_version";
TEST_FUNCTIONS_WITH_INPUT="open_close seek read read_chunk";
OPTION_SETS="";

TEST_TOOL_DIRECTORY=".";
INPUT_DIRECTORY="input";
INPUT_GLOB="*.[ELels]*01";

test_api_function()
{
	local TEST_PROFILE=$1;
	local TEST_FUNCTION=$2;
	local OPTION_SETS=$3;

	local TEST_TOOL="${TEST_PREFIX}_test_${TEST_FUNCTION}";

	local TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}";

	if ! test -x "${TEST_EXECUTABLE}";
	then
		TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}.exe";
	fi

	if ! test -x "${TEST_EXECUTABLE}";
	then
		echo "Missing test executable: ${TEST_EXECUTABLE}";

		return ${EXIT_FAILURE};
	fi
	echo -n -e "Testing API function: lib${TEST_PREFIX}_${TEST_FUNCTION}\t";

	# TODO: add support for TEST_PROFILE and OPTION_SETS?
	run_test_with_arguments ${TEST_EXECUTABLE};
	local RESULT=$?;

	if test ${RESULT} -ne 0;
	then
		echo "(FAIL)";
	else
		echo "(PASS)";
	fi
	return ${RESULT};
}

test_api_function_with_input()
{
	local TEST_PROFILE=$1;
	local TEST_FUNCTION=$2;
	local OPTION_SETS=$3;
	local INPUT_DIRECTORY=$4;
	local INPUT_GLOB=$5;
	local TEST_TOOL="${TEST_PREFIX}_test_${TEST_FUNCTION}";

	local TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}";

	if ! test -x "${TEST_EXECUTABLE}";
	then
		TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}.exe";
	fi

	if ! test -x "${TEST_EXECUTABLE}";
	then
		echo "Missing test executable: ${TEST_EXECUTABLE}";

		return ${EXIT_FAILURE};
	fi
	run_test_on_input_directory "${TEST_PROFILE}" "${TEST_FUNCTION}" "default" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "${INPUT_DIRECTORY}" "${INPUT_GLOB}";
	local RESULT=$?;

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
	test_api_function "${TEST_PROFILE}" "${TEST_FUNCTION}" "${OPTION_SETS}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${RESULT};
fi

for TEST_FUNCTION in ${TEST_FUNCTIONS_WITH_INTPUT};
do
	test_api_function_with_input "${TEST_PROFILE}" "${TEST_FUNCTION}" "${OPTION_SETS}" "${INPUT_DIRECTORY}" "${INPUT_GLOB}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

exit ${RESULT};

