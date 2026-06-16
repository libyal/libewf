# Script that synchronizes the local test data
#
# Version: 20260608

$Repository = "log2timeline/dfvfs"
$TestDataPath = "test_data"
$TestSet = "public"
$TestInputDirectory = "tests\input"
$TestFiles = "ext2.E01 ext2.raw"

If (-Not (Test-Path ${TestInputDirectory}))
{
	New-Item -Name ${TestInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestInputDirectory}\.ewfinfo"))
{
	New-Item -Name "${TestInputDirectory}\.ewfinfo" -ItemType "directory" | Out-Null
	Write-Output "*.[ELels]*01" | Out-File -Encoding ascii -FilePath "${TestInputDirectory}\.ewfinfo\glob"
}
If (-Not (Test-Path "${TestInputDirectory}\.ewfinfo_logical"))
{
	New-Item -Name "${TestInputDirectory}\.ewfinfo_logical" -ItemType "directory" | Out-Null
	Write-Output "*.[Ll]*01" | Out-File -Encoding ascii -FilePath "${TestInputDirectory}\.ewfinfo_logical\glob"
}
If (-Not (Test-Path "${TestInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$UrlTestFile = [System.Uri]::EscapeDataString("${TestFile}")
	$Url = "https://raw.githubusercontent.com/${Repository}/refs/heads/main/${TestDataPath}/${UrlTestFile}"

	$ProgressPreference = 'SilentlyContinue'
	Invoke-WebRequest -Uri ${Url} -OutFile "${TestInputDirectory}\${TestSet}\${TestFile}"
}

