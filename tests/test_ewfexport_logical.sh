#!/bin/bash
# Export tool testing script
#
# Version: 20201130

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

PROFILES=("ewfexport_logical" "ewfexport_logical_chunk" "ewfexport_logical_multi" "ewfexport_logical_chunk_multi")
OPTIONS_PER_PROFILE=("-ffiles -j0 -q -texport -u" "-ffiles -j0 -q -texport -u -x" "-ffiles -j4 -q -texport -u" "-ffiles -j4 -q -texport -u -x")
OPTION_SETS="";

INPUT_GLOB="*.[Ll]*01";

test_callback()
{
	local TMPDIR=$1;
	local TEST_SET_DIRECTORY=$2;
	local TEST_OUTPUT=$3;
	local TEST_EXECUTABLE=$4;
	local TEST_INPUT=$5;
	shift 5;
	local ARGUMENTS=("$@");

	TEST_EXECUTABLE=$( readlink_f "${TEST_EXECUTABLE}" );
	INPUT_FILE_FULL_PATH=$( readlink_f "${INPUT_FILE}" );

	(cd ${TMPDIR} && run_test_with_input_and_arguments "${TEST_EXECUTABLE}" "${INPUT_FILE_FULL_PATH}" ${ARGUMENTS[@]});
	local RESULT=$?;

	local TEST_LOG="${TEST_OUTPUT}.log";

	if test "${PLATFORM}" = "Darwin";
	then
		(cd ${TMPDIR} && find export -type f -exec md5 {} \; | sort -k2 > "${TEST_LOG}");
	else
		(cd ${TMPDIR} && find export -type f -exec md5sum {} \; | sort -k2 > "${TEST_LOG}");
	fi

	local TEST_RESULTS="${TMPDIR}/${TEST_LOG}";
	local STORED_TEST_RESULTS="${TEST_SET_DIRECTORY}/${TEST_LOG}.gz";

	if test -f "${STORED_TEST_RESULTS}";
	then
		# Using zcat here since zdiff has issues on Mac OS X.
		# Note that zcat on Mac OS X requires the input from stdin.
		zcat < "${STORED_TEST_RESULTS}" | diff "${TEST_RESULTS}" -;
		RESULT=$?;
	else
		gzip ${TEST_RESULTS};

		mv "${TEST_RESULTS}.gz" ${TEST_SET_DIRECTORY};
	fi
	return ${RESULT};
}

if ! test -z ${SKIP_TOOLS_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_EXECUTABLE="../ewftools/ewfexport";

if ! test -x "${TEST_EXECUTABLE}";
then
	TEST_EXECUTABLE="../ewftools/ewfexport.exe";
fi

if ! test -x "${TEST_EXECUTABLE}";
then
	echo "Missing test executable: ${TEST_EXECUTABLE}";

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

PLATFORM=`uname -s`;

assert_availability_binary find;

if test "${PLATFORM}" = "Darwin";
then
	assert_availability_binary md5;
else
	assert_availability_binary md5sum;
fi

if ! test -d "input";
then
	echo "Test input directory: input not found.";

	exit ${EXIT_IGNORE};
fi
RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

if test ${RESULT} -eq ${EXIT_SUCCESS};
then
	echo "No files or directories found in the test input directory: input";

	exit ${EXIT_IGNORE};
fi

TEST_PROFILE_DIRECTORY=$(get_test_profile_directory "input" "ewfexport_logical");

IGNORE_LIST=$(read_ignore_list "${TEST_PROFILE_DIRECTORY}");

RESULT=${EXIT_SUCCESS};

for PROFILE_INDEX in ${!PROFILES[*]};
do
	TEST_PROFILE=${PROFILES[${PROFILE_INDEX}]};

	TEST_PROFILE_DIRECTORY=$(get_test_profile_directory "input" "${TEST_PROFILE}");

	IGNORE_LIST=$(read_ignore_list "${TEST_PROFILE_DIRECTORY}");

	IFS=" " read -a OPTIONS <<< ${OPTIONS_PER_PROFILE[${PROFILE_INDEX}]};

	RESULT=${EXIT_SUCCESS};

	for TEST_SET_INPUT_DIRECTORY in input/*;
	do
		if ! test -d "${TEST_SET_INPUT_DIRECTORY}";
		then
			continue;
		fi
		if check_for_directory_in_ignore_list "${TEST_SET_INPUT_DIRECTORY}" "${IGNORE_LIST}";
		then
			continue;
		fi

		TEST_SET_DIRECTORY=$(get_test_set_directory "${TEST_PROFILE_DIRECTORY}" "${TEST_SET_INPUT_DIRECTORY}");

		OLDIFS=${IFS};

		# IFS="\n"; is not supported by all platforms.
		IFS="
";

		if test -f "${TEST_SET_DIRECTORY}/files";
		then
			for INPUT_FILE in `cat ${TEST_SET_DIRECTORY}/files | sed "s?^?${TEST_SET_INPUT_DIRECTORY}/?"`;
			do
				run_test_on_input_file_with_options "${TEST_SET_DIRECTORY}" "ewfexport_logical" "with_callback" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "${INPUT_FILE}" "${OPTIONS[@]}";
				RESULT=$?;

				if test ${RESULT} -ne ${EXIT_SUCCESS};
				then
					break;
				fi
			done
		else
			for INPUT_FILE in `ls -1 ${TEST_SET_INPUT_DIRECTORY}/${INPUT_GLOB}`;
			do
				run_test_on_input_file_with_options "${TEST_SET_DIRECTORY}" "ewfexport_logical" "with_callback" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "${INPUT_FILE}" "${OPTIONS[@]}";
				RESULT=$?;

				if test ${RESULT} -ne ${EXIT_SUCCESS};
				then
					break;
				fi
			done
		fi
		IFS=${OLDIFS};

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			break;
		fi
	done
done

exit ${RESULT};

