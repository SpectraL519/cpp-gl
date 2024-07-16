#!/bin/bash

format_check=false
if [[ "$1" == "--check" ]]; then
    format_check=true
fi

run_clang_format() {
    local file="$1"
    if [[ "$format_check" == true ]]; then
        clang-format-18 --dry-run --Werror "$file"
    else
        clang-format-18 -i "$file"
    fi
}

search_directory="."
declare -a file_patterns=("*.cpp" "*.hpp" "*.c" "*.h")
declare -a exclude_paths=(
    "*/tests/external/*"
    "*/build/*"
    "*/tmp/*"
)

find_command="find $search_directory -type f \( "
for pattern in "${file_patterns[@]}"; do
    find_command+="-name \"$pattern\" -o "
done
find_command="${find_command::-4} \) ! \( "
for path in "${exclude_paths[@]}"; do
    find_command+="-path \"$path\" -o "
done
find_command="${find_command::-4} \)"

# Count the number of files to format
file_count=$(eval "$find_command | wc -l")
if [[ "$format_check" == true ]]; then
    echo "Files to check: $file_count"
else
    echo "Files to format: $file_count"
fi
echo

# Iterate over the files and run format/check
file_number=0
eval "$find_command -print0" | while IFS= read -r -d '' file; do
    ((file_number++))
    echo "[$file_number/$file_count] $file"
    run_clang_format "$file"
done

echo
echo "Done!"
