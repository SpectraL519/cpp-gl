import argparse
import re
import subprocess
import sys
from pathlib import Path


class DefaultParameters:
    modified_files = False
    search_paths = ["include", "tests"]
    file_patterns = ["*.cpp", "*.hpp", "*.c", "*.h"]
    exclude_paths = ["tests/external"]
    check = False
    help = False


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-m", "--modified-files",
        type=bool,
        default=DefaultParameters.modified_files,
        action=argparse.BooleanOptionalAction,
        help="run clang-format only on the files modified since last pushed commit"
    )
    parser.add_argument(
        "-p", "--search-paths",
        type=str,
        default=DefaultParameters.search_paths,
        nargs="*",
        help="list of search directory paths"
    )
    parser.add_argument(
        "-f", "--file-patterns",
        type=str,
        default=DefaultParameters.file_patterns,
        nargs="*",
        help="list of file patterns to include"
    )
    parser.add_argument(
        "-e", "--exclude-paths",
        type=str,
        default=DefaultParameters.exclude_paths,
        nargs="*",
        help="list of directory paths to exclude"
    )
    parser.add_argument(
        "-c", "--check",
        type=bool,
        default=DefaultParameters.check,
        action=argparse.BooleanOptionalAction,
        help="run format check"
    )

    return vars(parser.parse_args())


def find_files(
    search_paths: list[str],
    file_patterns: list[str],
    exclude_paths: list[str]
) -> set[Path]:
    matching_files = []
    for search_path in search_paths:
        path = Path(search_path)
        for pattern in file_patterns:
            matching_files.extend(path.rglob(pattern))

    filtered_files = {
        file for file in matching_files
        if not any(str(file.parent).startswith(path) for path in exclude_paths)
    }

    return filtered_files


def get_modified_files(found_files: set[Path]) -> set[Path]:
    try:
        result = subprocess.run(
            "git diff --name-only @{u}".split(),
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        modified_files = {Path(file) for file in result.stdout.splitlines() if file}
        return modified_files & found_files

    except subprocess.CalledProcessError as e:
        print(f"Error executing git command: {e.stderr}")
        raise RuntimeError("Failed to retrieve the modified files.")


def run_clang_format(
    files: set[Path],
    check: bool
):
    no_files = len(files)
    if check:
        print(f"Files to check: {no_files}")
    else:
        print(f"Files to format: {no_files}")

    return_code = 0
    for i, file in enumerate(files):
        print(f"[{i + 1}/{no_files}] {file}")

        cmd = ["clang-format-18", str(file)]
        if check:
            cmd.extend(["--dry-run", "--Werror"])
        else:
            cmd.append("-i")
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            return_code = result.returncode
            print(f"[Format error]\n[stdout]\n{result.stdout}\n[stderr]\n{result.stderr}")

    print("Done!")
    return return_code


def main(
    modified_files: bool,
    search_paths: list[str],
    file_patterns: list[str],
    exclude_paths: list[str],
    check: bool
):
    files_to_format = find_files(search_paths, file_patterns, exclude_paths)
    if modified_files:
        files_to_format = get_modified_files(files_to_format)

    sys.exit(run_clang_format(files_to_format, check))


if __name__ == "__main__":
    main(**parse_args())
