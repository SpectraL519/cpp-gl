#!/bin/bash

# Recursively find .hpp files and format them with clang-format
find . -type f -name "*.hpp" -exec clang-format -i {} \;
