import argparse
import subprocess
import sys
from pathlib import Path

from common import find_files


class DefaultParameters:
    modified_files: bool = False
    search_paths: list[str] = ["include", "tests"]
    file_patterns: list[str] = ["*.cpp", "*.hpp", "*.c", "*.h"]
    exclude_paths: list[str] = ["tests/external"]
    check: bool = False


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
        action="extend",
        help="list of search directory paths"
    )
    parser.add_argument(
        "-f", "--file-patterns",
        type=str,
        default=DefaultParameters.file_patterns,
        nargs="*",
        action="extend",
        help="list of file patterns to include"
    )
    parser.add_argument(
        "-e", "--exclude-paths",
        type=str,
        default=DefaultParameters.exclude_paths,
        nargs="*",
        action="extend",
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


def get_modified_files(files: set[Path]) -> set[Path]:
    try:
        result = subprocess.run(
            "git diff --name-only @{u}".split(),
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        modified_files = {Path(file) for file in result.stdout.splitlines() if file}
        return modified_files & files

    except subprocess.CalledProcessError as e:
        print(f"Error executing git command: {e.stderr}")
        raise RuntimeError("Failed to retrieve the modified files.")


def run_clang_format(files: set[Path], check: bool) -> int:
    n_files = len(files)
    if check:
        print(f"Files to check: {n_files}")
    else:
        print(f"Files to format: {n_files}")

    return_code = 0
    for i, file in enumerate(files):
        print(f"[{i + 1}/{n_files}] {file}")

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
