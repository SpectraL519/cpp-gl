#!/bin/bash

format_check=false
if [[ "$1" == "--check" ]]; then
    format_check=true
fi

run_clang_format() {
    local file="$1"
    if [[ "$format_check" == true ]]; then
        clang-format-17 --dry-run --Werror "$file"
    else
        clang-format-17 -i "$file"
    fi
}

# Count the number of files to format
file_count=$(find . -type f \( -name "*.hpp" -o -name "*.cpp" \) ! -path "*/build/*" | wc -l)
if [[ "$format_check" == true ]]; then
    echo "Files to check: $file_count"
else
    echo "Files to format: $file_count"
fi
echo

# Iterate over the files and run format/check
file_number=0
find . -type f \( -name "*.hpp" -o -name "*.cpp" \) ! -path "*/build/*" -print0 | while IFS= read -r -d '' file; do
    ((file_number++))
    echo "[$file_number/$file_count] $file"
    run_clang_format "$file"
done

echo
echo "Done!"
