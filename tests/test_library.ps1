# Tests library functions and types.

$LibraryTests = "access_control_entry analytical_data attribute bit_stream buffer_data_handle case_data case_data_section checksum chunk_data chunk_descriptor chunk_group chunk_table compression data_chunk date_time date_time_values deflate device_information device_information_section digest_section error error2_section file_entry filename hash_sections hash_values header_sections header_values huffman_tree io_handle lef_extended_attribute lef_file_entry lef_permission lef_source lef_subject line_reader ltree_section md5_hash_section media_values notify permission_group read_io_handle restart_data section_data_handle section_descriptor sector_range sector_range_list segment_file segment_table serialized_string session_section sha1_hash_section single_file_tree single_files source table_section value_reader value_table volume_section write_io_handle"
$LibraryTestsWithInput = "handle support"
$OptionSets = "" -split " "

. .\test_functions.ps1

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitSuccess}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$ResultRun = RunTestBinary ${TestExecutablesDirectory} "ewf_test_${TestName}"

	If ((${ResultRun} -ne ${ExitSuccess}) -And (${ResultRun} -ne ${ExitIgnore}))
	{
		$Result = ${ResultRun}
	}
}

$TestInputs = GenerateTestInputs "libewf" ${OptionSets}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	ForEach ($TestInput in ${TestInputs})
	{
		$ResultRun = RunTestBinaryWithInput ${TestExecutablesDirectory} "ewf_test_${TestName}" ${TestInput}

		If ((${ResultRun} -ne ${ExitSuccess}) -And (${ResultRun} -ne ${ExitIgnore}))
		{
			$Result = ${ResultRun}
		}
	}
}

Exit ${Result}
