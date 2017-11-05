# Tests C library functions and types.
#
# Version: 20170911

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$LibraryTests = "analytical_data case_data chunk_data chunk_group chunk_table data_chunk device_information error file_entry hash_sections hash_values header_sections header_values io_handle media_values notify read_io_handle restart_data section sector_range segment_file segment_table single_file_entry single_files write_io_handle"
$LibraryTestsWithInput = "handle support"

Function GetTestToolDirectory
{
	$TestToolDirectory = ""

	ForEach (${VSDirectory} in "msvscpp vs2008 vs2010 vs2012 vs2013 vs2015 vs2017" -split " ")
	{
		ForEach (${VSConfiguration} in "Release VSDebug" -split " ")
		{
			$TestToolDirectory = "..\${VSDirectory}\${VSConfiguration}"

			If (Test-Path ${TestToolDirectory})
			{
				Return ${TestToolDirectory}
			}
			ForEach (${VSPlatform} in "Win32 x64" -split " ")
			{
				$TestToolDirectory = "..\${VSDirectory}\${VSConfiguration}\${VSPlatform}"

				If (Test-Path ${TestToolDirectory})
				{
					Return ${TestToolDirectory}
				}
			}
		}
	}
	Return ${TestToolDirectory}
}

Function RunTest
{
	param( [string]$TestType )

	$TestDescription = "Testing: ${TestName}"
	$TestExecutable = "${TestToolDirectory}\ewf_test_${TestName}.exe"

	$Output = Invoke-Expression ${TestExecutable}
	$Result = ${LastExitCode}

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} " -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host " (FAIL)"
	}
	Else
	{
		Write-Host " (PASS)"
	}
	Return ${Result}
}

$TestToolDirectory = GetTestToolDirectory

If (-Not (Test-Path ${TestToolDirectory}))
{
	Write-Host "Missing test tool directory." -foreground Red

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTest ${TestName}

	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	# TODO: add RunTestWithInput
	$Result = RunTest ${TestName}

	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Exit ${Result}

