#!/bin/sh
# Script that synchronizes the local test data
#
# Version: 20251217

REPOSITORY="log2timeline/dfvfs";
TEST_DATA_PATH="test_data";
TEST_SET="public";
TEST_INPUT_DIRECTORY="tests/input";
TEST_FILES="ext2.E01 ext2.raw";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfacquire";
echo "*.[Rr][Aa][Ww]" > "${TEST_INPUT_DIRECTORY}/.ewfacquire/glob";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfacquire_optical";
echo "*.[Cc][Uu][Ee]" > "${TEST_INPUT_DIRECTORY}/.ewfacquire_optical/glob";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfacquirestream";
echo "*.[Rr][Aa][Ww]" > "${TEST_INPUT_DIRECTORY}/.ewfacquirestream/glob";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfexport";
echo "*.[Ees]*01" > "${TEST_INPUT_DIRECTORY}/.ewfexport/glob";
echo "-fraw" > "${TEST_INPUT_DIRECTORY}/.ewfexport/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfexport_logical";
echo "*.[Ll]*01" > "${TEST_INPUT_DIRECTORY}/.ewfexport_logical/glob";
echo "-ffiles" > "${TEST_INPUT_DIRECTORY}/.ewfexport_logical/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfexport_write";
echo "*.[Ees]*01" > "${TEST_INPUT_DIRECTORY}/.ewfexport_write/glob";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfinfo";
echo "*.[ELels]*01" > "${TEST_INPUT_DIRECTORY}/.ewfinfo/glob";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfinfo_logical";
echo "*.[Ll]*01" > "${TEST_INPUT_DIRECTORY}/.ewfinfo_logical/glob";
echo "-H" > "${TEST_INPUT_DIRECTORY}/.ewfinfo_logical/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfinfo_logical_bodyfile";
echo "*.[Ll]*01" > "${TEST_INPUT_DIRECTORY}/.ewfinfo_logical_bodyfile/glob";
echo "-Bbodyfile -H" > "${TEST_INPUT_DIRECTORY}/.ewfinfo_logical_bodyfile/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfverify";
echo "*.[Ees]*01" > "${TEST_INPUT_DIRECTORY}/.ewfverify/glob";
echo "-dmd5 -q" > "${TEST_INPUT_DIRECTORY}/.ewfverify/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/.ewfverify_logical";
echo "*.[Ll]*01" > "${TEST_INPUT_DIRECTORY}/.ewfverify_logical/glob";
echo "-dmd5 -ffiles -q" > "${TEST_INPUT_DIRECTORY}/.ewfverify_logical/options";

mkdir -p "${TEST_INPUT_DIRECTORY}/${TEST_SET}";

for TEST_FILE in ${TEST_FILES};
do
	URL="https://raw.githubusercontent.com/${REPOSITORY}/refs/heads/main/${TEST_DATA_PATH}/${TEST_FILE}";

	curl -L -o "${TEST_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL};
done

