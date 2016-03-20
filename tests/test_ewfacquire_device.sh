#!/bin/bash
# Acquire tool testing script
#
# Version: 20160320

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`pwd`;
TEST_PREFIX=`dirname ${TEST_PREFIX}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib//'`;

run_test()
{ 
	TEST_SET_DIR=$1;
	TEST_DESCRIPTION=$2;
	TEST_EXECUTABLE=$3;
	IFS=" " read -a OPTIONS <<< $4;
	INPUT_DEVICE=$5;
	INPUT_SIZE=$6;

	TEST_RUNNER="tests/test_runner.sh";

	if ! test -x "${TEST_RUNNER}";
	then
		TEST_RUNNER="./test_runner.sh";
	fi

	if ! test -x "${TEST_RUNNER}";
	then
		echo "Missing test runner: ${TEST_RUNNER}";

		return ${EXIT_FAILURE};
	fi

	INPUT_NAME=`basename ${INPUT_DEVICE}`;

	TEST_OUTPUT="${INPUT_NAME}";

	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	STORED_TEST_RESULTS="${TEST_SET_DIR}/${TEST_OUTPUT}.log.gz";
	TEST_RESULTS="${TMPDIR}/${TEST_OUTPUT}.log";

	${TEST_RUNNER} ${TMPDIR} ${TEST_EXECUTABLE} -B ${INPUT_SIZE} -C Case -D Description -E Evidence -e Examiner -m removable -M logical -N Notes -P 512 -q -t ${TMPDIR}/${INPUT_NAME}.acquire -u ${OPTIONS[*]} ${INPUT_DEVICE} | sed '1,2d' > ${TEST_RESULTS};

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
        then
                ${VERIFY_TOOL} -q ${TMPDIR}/${INPUT_NAME}.acquire.* > /dev/null;

                RESULT=$?;
        fi
	rm -rf ${TMPDIR};

	echo -n "Testing ${TEST_DESCRIPTION} with input: ${INPUT_DEVICE}";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

if ! test -z ${SKIP_TOOLS_TESTS};
then
	exit ${EXIT_IGNORE};
fi

ACQUIRE_TOOL="../${TEST_PREFIX}tools/${TEST_PREFIX}acquire";

if ! test -x "${ACQUIRE_TOOL}";
then
	ACQUIRE_TOOL="../${TEST_PREFIX}tools/${TEST_PREFIX}acquire.exe";
fi

if ! test -x "${ACQUIRE_TOOL}";
then
	echo "Missing executable: ${ACQUIRE_TOOL}";

	exit ${EXIT_FAILURE};
fi

VERIFY_TOOL="../${TEST_PREFIX}tools/${TEST_PREFIX}verify";

if ! test -x "${VERIFY_TOOL}";
then
	VERIFY_TOOL="../${TEST_PREFIX}tools/${TEST_PREFIX}verify.exe";
fi

if ! test -x "${VERIFY_TOOL}";
then
	echo "Missing executable: ${VERIFY_TOOL}";

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

TEST_PROFILE="${TEST_PREFIX}acquire_device";
TEST_SET_DIR="input/.${TEST_PROFILE}";

# Set upper virtual memory limit to 1 GiB
ulimit -Sv 1048576;

# > 2 GiB test
INPUT_SIZE=$(( ( 2 * 1024 * 1024 * 1024 ) + 512 ));

OPTIONS="-cdeflate:none -fencase7";

if ! run_test "${TEST_SET_DIR}" "${TEST_PROFILE}" "${ACQUIRE_TOOL}" "${OPTIONS}" "/dev/urandom" ${INPUT_SIZE};
then
	exit ${EXIT_FAILURE};
fi

OPTIONS="-cdeflate:empty-block -fencase7";

if ! run_test "${TEST_SET_DIR}" "${TEST_PROFILE}" "${ACQUIRE_TOOL}" "${OPTIONS}" "/dev/zero" ${INPUT_SIZE};
then
	exit ${EXIT_FAILURE};
fi

# > 4 GiB test
INPUT_SIZE=$(( ( 4 * 1024 * 1024 * 1024 ) + 512 ));

OPTIONS="-cdeflate:none -fencase7";

if ! run_test "${TEST_SET_DIR}" "${TEST_PROFILE}" "${ACQUIRE_TOOL}" "${OPTIONS}" "/dev/urandom" ${INPUT_SIZE};
then
	exit ${EXIT_FAILURE};
fi

OPTIONS="-cdeflate:empty-block -fencase7";

if ! run_test "${TEST_SET_DIR}" "${TEST_PROFILE}" "${ACQUIRE_TOOL}" "${OPTIONS}" "/dev/zero" ${INPUT_SIZE};
then
	exit ${EXIT_FAILURE};
fi

# > 2 TiB test
INPUT_SIZE=$(( ( 2 * 1024 * 1024 * 1024 * 1024 ) + 512 ));

OPTIONS="-cdeflate:empty-block -fencase7";

if ! run_test "${TEST_SET_DIR}" "${TEST_PROFILE}" "${ACQUIRE_TOOL}" "${OPTIONS}" "/dev/zero" ${INPUT_SIZE};
then
	exit ${EXIT_FAILURE};
fi

# > 8 TiB test
INPUT_SIZE=$(( ( 8 * 1024 * 1024 * 1024 * 1024 ) + 512 ));

OPTIONS="-cdeflate:empty-block -fencase7";

if ! run_test "${TEST_SET_DIR}" "${TEST_PROFILE}" "${ACQUIRE_TOOL}" "${OPTIONS}" "/dev/zero" ${INPUT_SIZE};
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

