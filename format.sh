#!/bin/bash

# Check if a regular expression is provided as an argument.
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <regex>"
    exit 1
fi

# Directory where you want to start the search.
start_directory="$1"

# Get the regular expression from the first command-line argument.
file_regex="$2"

# Run clang-format on matching files.
find "$start_directory" -type f -regex "$file_regex" -exec clang-format -i {} \;
