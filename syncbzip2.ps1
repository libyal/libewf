# Script that synchronizes bzip2.
#
# Version: 20200229

$ExitSuccess = 0
$ExitFailure = 1

Function ExtractTarGz($Filename)
{
	$SevenZip = "C:\Program Files\7-Zip\7z.exe"

	If (-Not (Test-Path ${SevenZip}))
	{
		Write-Host "Missing 7z.exe." -foreground Red

		Exit ${ExitFailure}
	}
	# Run 7z twice first to extract the .gz then the .tar

	# PowerShell will raise NativeCommandError if 7z writes to stdout or stderr
	# therefore 2>&1 is added and the output is stored in a variable.
	# The leading & and single quotes are necessary to compensate for the spaces in the path.
	$Output = Invoke-Expression -Command "& '${SevenZip}' -y x ${Filename} 2>&1"

	$Filename = ${Filename}.TrimEnd(".gz")

	$Output = Invoke-Expression -Command "& '${SevenZip}' -y x ${Filename} 2>&1"

	Remove-Item -Path ${Filename} -Force
}

$Version = "1.0.6"
$Filename = "${pwd}\bzip2-${Version}.tar.gz"
$Url = "http://www.bzip.org/${Version}/bzip2-${Version}.tar.gz"
$ExtractedPath = "bzip2-${Version}"
$DestinationPath = "..\bzip2"

If (Test-Path ${Filename})
{
	Remove-Item -Path ${Filename} -Force
}
Invoke-WebRequest -Uri ${Url} -OutFile ${Filename}

If (Test-Path ${ExtractedPath})
{
	Remove-Item -Path ${ExtractedPath} -Force -Recurse
}
ExtractTarGz -Filename ${Filename}

Remove-Item -Path ${Filename} -Force

If (Test-Path ${DestinationPath})
{
	Remove-Item -Path ${DestinationPath} -Force -Recurse
}
Move-Item ${ExtractedPath} ${DestinationPath}

