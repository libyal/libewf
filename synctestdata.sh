#!/bin/sh
# Script that synchronizes the local test data

TESTS_INPUT_DIRECTORY="tests/input"
TEST_SET="public"
TEST_FILES="ext2.E01 ext2.raw"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.libewf"
echo "*.[ELels]*01" > "${TESTS_INPUT_DIRECTORY}/.libewf/glob"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.pyewf"
echo "*.[ELels]*01" > "${TESTS_INPUT_DIRECTORY}/.pyewf/glob"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfacquire"
echo "*.[Rr][Aa][Ww]" > "${TESTS_INPUT_DIRECTORY}/.ewfacquire/glob"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfacquire_optical"
echo "*.[Cc][Uu][Ee]" > "${TESTS_INPUT_DIRECTORY}/.ewfacquire_optical/glob"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfacquirestream"
echo "*.[Rr][Aa][Ww]" > "${TESTS_INPUT_DIRECTORY}/.ewfacquirestream/glob"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfexport"
echo "*.[Ees]*01" > "${TESTS_INPUT_DIRECTORY}/.ewfexport/glob"
echo "-fraw" > "${TESTS_INPUT_DIRECTORY}/.ewfexport/options"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfexport_logical"
echo "*.[Ll]*01" > "${TESTS_INPUT_DIRECTORY}/.ewfexport_logical/glob"
echo "-ffiles" > "${TESTS_INPUT_DIRECTORY}/.ewfexport_logical/options"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfexport_write"
echo "*.[Ees]*01" > "${TESTS_INPUT_DIRECTORY}/.ewfexport_write/glob"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfinfo"
echo "*.[ELels]*01" > "${TESTS_INPUT_DIRECTORY}/.ewfinfo/glob"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfinfo_logical"
echo "*.[Ll]*01" > "${TESTS_INPUT_DIRECTORY}/.ewfinfo_logical/glob"
echo "-H" > "${TESTS_INPUT_DIRECTORY}/.ewfinfo_logical/options"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfinfo_logical_bodyfile"
echo "*.[Ll]*01" > "${TESTS_INPUT_DIRECTORY}/.ewfinfo_logical_bodyfile/glob"
echo "-Bbodyfile -H" > "${TESTS_INPUT_DIRECTORY}/.ewfinfo_logical_bodyfile/options"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfverify"
echo "*.[Ees]*01" > "${TESTS_INPUT_DIRECTORY}/.ewfverify/glob"
echo "-dmd5" > "${TESTS_INPUT_DIRECTORY}/.ewfverify/options"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.ewfverify_logical"
echo "*.[Ll]*01" > "${TESTS_INPUT_DIRECTORY}/.ewfverify_logical/glob"
echo "-dmd5 -ffiles" > "${TESTS_INPUT_DIRECTORY}/.ewfverify_logical/options"

mkdir -p "${TESTS_INPUT_DIRECTORY}/${TEST_SET}"

for TEST_FILE in ${TEST_FILES}
do
	URL="https://raw.githubusercontent.com/log2timeline/dfvfs/refs/heads/main/test_data/${TEST_FILE}"

	curl -L -o "${TESTS_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL}
done
