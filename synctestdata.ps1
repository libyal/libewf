# Script that synchronizes the local test data

$TestsInputDirectory = "tests\input"
$TestSet = "public"
$TestFiles = "ext2.E01 ext2.raw"

If (-Not (Test-Path "${TestsInputDirectory}\.libewf"))
{
	New-Item -Name "${TestsInputDirectory}\.libewf" -ItemType "directory" | Out-Null
	Write-Output "*.[ELels]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.libewf\glob"
}

If (-Not (Test-Path "${TestsInputDirectory}\.pyewf"))
{
	New-Item -Name "${TestsInputDirectory}\.pyewf" -ItemType "directory" | Out-Null
	Write-Output "*.[ELels]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.pyewf\glob"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfacquire"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfacquire" -ItemType "directory" | Out-Null
	Write-Output "*.[Rr][Aa][Ww]" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfacquire\glob"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfacquire_optical"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfacquire_optical" -ItemType "directory" | Out-Null
	Write-Output "*.[Cc][Uu][Ee]" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfacquire_optical\glob"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfacquirestream"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfacquirestream" -ItemType "directory" | Out-Null
	Write-Output "*.[Rr][Aa][Ww]" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfacquirestream\glob"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfexport"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfexport" -ItemType "directory" | Out-Null
	Write-Output "*.[Ees]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfexport\glob"
	Write-Output "-fraw" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfexport\options"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfexport_logical"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfexport_logical" -ItemType "directory" | Out-Null
	Write-Output "*.[Ll]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfexport_logical\glob"
	Write-Output "-ffiles" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfexport_logical\options"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfexport_write"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfexport_write" -ItemType "directory" | Out-Null
	Write-Output "*.[Ees]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfexport_write\glob"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfinfo"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfinfo" -ItemType "directory" | Out-Null
	Write-Output "*.[ELels]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfinfo\glob"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfinfo_logical"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfinfo_logical" -ItemType "directory" | Out-Null
	Write-Output "*.[Ll]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfinfo_logical\glob"
	Write-Output "-H" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfinfo_logical\options"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfinfo_logical_bodyfile"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfinfo_logical_bodyfile" -ItemType "directory" | Out-Null
	Write-Output "*.[Ll]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfinfo_logical_bodyfile\glob"
	Write-Output "-Bbodyfile -H" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfinfo_logical_bodyfile\options"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfverify"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfverify" -ItemType "directory" | Out-Null
	Write-Output "*.[Ees]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfverify\glob"
	Write-Output "-dmd5" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfverify\options"
}

If (-Not (Test-Path "${TestsInputDirectory}\.ewfverify_logical"))
{
	New-Item -Name "${TestsInputDirectory}\.ewfverify_logical" -ItemType "directory" | Out-Null
	Write-Output "*.[Ll]*01" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfverify_logical\glob"
	Write-Output "-dmd5 -ffiles" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.ewfverify_logical\options"
}

If (-Not (Test-Path ${TestsInputDirectory}))
{
	New-Item -Name ${TestsInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestsInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestsInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$UrlTestFile = [System.Uri]::EscapeDataString("${TestFile}")
	$Url = "https://raw.githubusercontent.com/log2timeline/dfvfs/refs/heads/main/test_data/${UrlTestFile}"

	$ProgressPreference = 'SilentlyContinue'
	Invoke-WebRequest -Uri ${Url} -OutFile "${TestsInputDirectory}\${TestSet}\${TestFile}"
}
