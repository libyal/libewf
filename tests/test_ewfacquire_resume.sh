#!/bin/bash
# Acquire tool testing script
#
# Version: 20201130

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

PROFILES=("ewfacquire_resume" "ewfacquire_resume_chunk" "ewfacquire_resume_multi" "ewfacquire_resume_chunk_multi")
OPTIONS_PER_PROFILE=("-j0" "-j0 -x" "-j4" "-j4 -x")
OPTION_SETS=""

test_write_resume()
{ 
	INPUT_FILE=$1;
	RESUME_OFFSET=$2;
	shift 2;
	local ARGUMENTS=("$@");

	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	run_test_with_input_and_arguments "${ACQUIRE_TOOL}" "${INPUT_FILE}" ${ARGUMENTS[@]} -b 64 -c deflate:none -C Case -D Description -E Evidence -e Examiner -f encase5 -m removable -M logical -N Notes -q -S 650MB -t ${TMPDIR}/acquire_resume -u > /dev/null;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		run_test_with_input_and_arguments "${VERIFY_TOOL}" ${TMPDIR}/acquire_resume.E01 -q > /dev/null;

		RESULT=$?;
	fi

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		run_test_with_input_and_arguments "${TRUNCATE_TOOL}" ${TMPDIR}/acquire_resume.E01 ${RESUME_OFFSET} > /dev/null;

		RESULT=$?;
	fi

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		run_test_with_input_and_arguments "${ACQUIRE_TOOL}" "${INPUT_FILE}" ${ARGUMENTS[@]} -q -R -t ${TMPDIR}/acquire_resume.E01 -u > ${TMPDIR}/output;

		RESULT=$?;
	fi

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		run_test_with_input_and_arguments "${VERIFY_TOOL}" ${TMPDIR}/acquire_resume.E01 -q > /dev/null;

		RESULT=$?;
	fi
	rm -rf ${TMPDIR};

	if test ${RESULT} -ne ${EXIT_IGNORE};
	then
		echo -n "Testing ewfacquire resume of at offset: ${RESUME_OFFSET}";

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			echo " (FAIL)";
		else
			echo " (PASS)";
		fi
	fi
	return ${RESULT};
}

if ! test -z ${SKIP_TOOLS_TESTS};
then
	exit ${EXIT_IGNORE};
fi

ACQUIRE_TOOL="../ewftools/ewfacquire";

if ! test -x "${ACQUIRE_TOOL}";
then
	ACQUIRE_TOOL="../ewftools/ewfacquire.exe";
fi

if ! test -x "${ACQUIRE_TOOL}";
then
	echo "Missing executable: ${ACQUIRE_TOOL}";

	exit ${EXIT_FAILURE};
fi

TRUNCATE_TOOL="./ewf_test_truncate";

if ! test -x "${TRUNCATE_TOOL}";
then
	ACQUIRE_TOOL="ewf_test_truncate.exe";
fi

if ! test -x ${TRUNCATE_TOOL};
then
	echo "Missing executable: ${TRUNCATE_TOOL}";

	exit ${EXIT_FAILURE};
fi

VERIFY_TOOL="../ewftools/ewfverify";

if ! test -x "${VERIFY_TOOL}";
then
	VERIFY_TOOL="../ewftools/ewfverify.exe";
fi

if ! test -x "${VERIFY_TOOL}";
then
	echo "Missing executable: ${VERIFY_TOOL}";

	exit ${EXIT_FAILURE};
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

FILENAME="input/raw/floppy.raw";

if ! test -f ${FILENAME};
then
	exit ${EXIT_IGNORE};
fi

FILESIZE=`ls -l ${FILENAME} | awk '{ print $5 }'`;

if test ${FILESIZE} -ne 1474560;
then
	exit ${EXIT_IGNORE};
fi

for PROFILE_INDEX in ${!PROFILES[*]};
do
	TEST_PROFILE=${PROFILES[${PROFILE_INDEX}]};

	TEST_PROFILE_DIRECTORY=$(get_test_profile_directory "input" "${TEST_PROFILE}");

	IGNORE_LIST=$(read_ignore_list "${TEST_PROFILE_DIRECTORY}");

	IFS=" " read -a OPTIONS <<< ${OPTIONS_PER_PROFILE[${PROFILE_INDEX}]};

	RESULT=${EXIT_SUCCESS};

	test_write_resume "${FILENAME}" 1478560 "${OPTIONS[@]}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${RESULT};
	fi

	test_write_resume "${FILENAME}" 1477351 "${OPTIONS[@]}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${RESULT};
	fi

	test_write_resume "${FILENAME}" 1478432 "${OPTIONS[@]}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${RESULT};
	fi

	test_write_resume "${FILENAME}" 1477296 "${OPTIONS[@]}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${RESULT};
	fi

	test_write_resume "${FILENAME}" 1477008 "${OPTIONS[@]}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${RESULT};
	fi

	test_write_resume "${FILENAME}" 1476736 "${OPTIONS[@]}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${RESULT};
	fi

	test_write_resume "${FILENAME}" 3584 "${OPTIONS[@]}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${RESULT};
	fi

	exit ${RESULT};
done

exit ${EXIT_SUCCESS};

