$formatCheck = $false
if ($args[0] -eq "--check") {
    $formatCheck = $true
}

function Run-Clang-Format {
    param (
        [string]$filePath,
        [bool]$check
    )

    if ($check) {
        clang-format --dry-run --Werror $filePath
    }
    else {
        clang-format -i $filePath
    }
}

# Get the list of files to format
$files = Get-ChildItem -Recurse -Include *.hpp, *.cpp | Where-Object { $_.DirectoryName -notmatch "\\build\\" }

# Count the number of files to format
$fileCount = $files.Count
Write-Host "Total files to format: $fileCount"
Write-Host

# Iterate over the files and run format/check
$fileNumber = 0
foreach ($file in $files) {
    $fileNumber++
    Write-Host "[$fileNumber/$fileCount] $($file.FullName)"
    Run-Clang-Format -filePath $file.FullName -check $formatCheck
}

Write-Host
Write-Host "Done!"
