# Recursively find .hpp files and format them with clang-format
Get-ChildItem -Recurse -Include *.hpp | ForEach-Object { clang-format -i $_.FullName }
