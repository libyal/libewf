#!/bin/bash
# Script to create the ewfacquire option sets
#
# Version: 20160402

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

INPUT_GLOB="*.[Cc][Uu][Ee]";

list_contains()
{
	LIST=$1;
	SEARCH=$2;

	for LINE in ${LIST};
	do
		if test ${LINE} = ${SEARCH};
		then
			return ${EXIT_SUCCESS};
		fi
	done

	return ${EXIT_FAILURE};
}

TEST_PROFILE_DIR="input/.ewfacquire_optical";
IGNORE_FILE="${TEST_PROFILE_DIR}/ignore";
IGNORE_LIST="";

if test -f "${IGNORE_FILE}";
then
	IGNORE_LIST=`cat ${IGNORE_FILE} | sed '/^#/d'`;
fi

for INPUT_DIR in input/*;
do
	if ! test -d "${INPUT_DIR}";
	then
		continue
	fi
	INPUT_NAME=`basename ${INPUT_DIR}`;

	if list_contains "${IGNORE_LIST}" "${INPUT_NAME}";
	then
		continue
	fi
	TEST_SET_DIR="${TEST_PROFILE_DIR}/${INPUT_NAME}";

	if ! test -d "${TEST_SET_DIR}";
	then
		mkdir "${TEST_SET_DIR}";
	fi
	INPUT_FILES=`ls ${INPUT_DIR}/${INPUT_GLOB}`;

	for INPUT_FILE in ${INPUT_FILES};
	do
		INPUT_NAME=`basename ${INPUT_FILE}`;

		# Test the different supported formats. 
		for FORMAT in encase5 encase6 encase7 encase7-v2;
		do
			OUTPUT_FILE="${TEST_SET_DIR}/${INPUT_NAME}.format:${FORMAT}";

			echo "Creating option set file: ${OUTPUT_FILE}";

			echo "-f${FORMAT} -j4 -x" > "${OUTPUT_FILE}";
		done
	done
done

exit ${EXIT_SUCCESS};

