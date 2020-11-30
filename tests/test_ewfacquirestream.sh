#!/bin/bash
# Acquirestream tool testing script
#
# Version: 20201130

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

PROFILES=("ewfacquirestream" "ewfacquirestream_chunk" "ewfacquirestream_multi" "ewfacquirestream_chunk_multi")
OPTIONS_PER_PROFILE=("-CCase -DDescription -EEvidence -eExaminer -j0 -mremovable -Mlogical -NNotes -q -tacquirestream" "-CCase -DDescription -EEvidence -eExaminer -j0 -mremovable -Mlogical -NNotes -q -tacquirestream -x" "-CCase -DDescription -EEvidence -eExaminer -j4 -mremovable -Mlogical -NNotes -q -tacquirestream" "-CCase -DDescription -EEvidence -eExaminer -j4 -mremovable -Mlogical -NNotes -q -tacquirestream -x")
OPTION_SETS="format:encase1 format:encase2 format:encase3 format:encase4 format:encase5 format:encase6 format:encase7 format:encase7-v2 format:ewf format:ewfx format:ftk format:linen5 format:linen6 format:linen7 format:raw format:smart deflate:none:encase7 deflate:empty-block:encase7 deflate:fast:encase7 deflate:best:encase7 deflate:none:encase7-v2 deflate:empty-block:encase7-v2 deflate:fast:encase7-v2 deflate:best:encase7-v2 deflate:none:smart deflate:empty-block:smart deflate:fast:smart deflate:best:smart segmentsize:1mib:encase7 segmentsize:1mib:encase7-v2 segmentsize:1mib:smart blocksize:16 blocksize:32 blocksize:128 blocksize:256 blocksize:512 blocksize:1024 blocksize:2048 blocksize:4096 blocksize:8192 blocksize:16384 blocksize:32768 hash:sha1 hash:sha256 hash:all";

INPUT_GLOB="*.[Rr][Aa][Ww]";

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

	local TEST_LOG="${TEST_OUTPUT}.log";

	(cd ${TMPDIR} && run_test_with_input_and_arguments "${TEST_EXECUTABLE}" "" ${ARGUMENTS[@]} < "${INPUT_FILE_FULL_PATH}" | sed '1,2d' > "${TEST_LOG}");
	local RESULT=$?;

	local TEST_RESULTS="${TMPDIR}/${TEST_LOG}";
	local STORED_TEST_RESULTS="${TEST_SET_DIRECTORY}/${TEST_LOG}.gz";

	if test -f "${STORED_TEST_RESULTS}";
	then
		# Using zcat here since zdiff has issues on Mac OS X.
		# Note that zcat on Mac OS X requires the input from stdin.
		zcat < "${STORED_TEST_RESULTS}" | diff "${TEST_RESULTS}" -;
		RESULT=$?;
	else
		gzip "${TEST_RESULTS}";

		mv "${TEST_RESULTS}.gz" ${TEST_SET_DIRECTORY};
	fi
	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		run_test_with_input_and_arguments "${VERIFY_TOOL}" ${TMPDIR}/acquirestream.* -q > /dev/null;
		local RESULT=$?;
	fi
	return ${RESULT};
}

if ! test -z ${SKIP_TOOLS_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_EXECUTABLE="../ewftools/ewfacquirestream";

if ! test -x "${TEST_EXECUTABLE}";
then
	TEST_EXECUTABLE="../ewftools/ewfacquirestream.exe";
fi

if ! test -x "${TEST_EXECUTABLE}";
then
	echo "Missing test executable: ${TEST_EXECUTABLE}";

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
				run_test_on_input_file_with_options "${TEST_SET_DIRECTORY}" "ewfacquirestream" "with_callback" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "${INPUT_FILE}" "${OPTIONS[@]}";
				RESULT=$?;

				if test ${RESULT} -ne ${EXIT_SUCCESS};
				then
					break;
				fi
			done
		else
			for INPUT_FILE in `ls -1 ${TEST_SET_INPUT_DIRECTORY}/${INPUT_GLOB}`;
			do
				run_test_on_input_file_with_options "${TEST_SET_DIRECTORY}" "ewfacquirestream" "with_callback" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "${INPUT_FILE}" "${OPTIONS[@]}";
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

