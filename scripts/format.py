import argparse
import pathlib
import subprocess
import sys


class default_parameters:
    search_paths = ["include", "tests"]
    file_patterns = ["*.cpp", "*.hpp", "*.c", "*.h"]
    exclude_paths = ["tests/external"]
    check = False
    help = False


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-p", "--search-paths",
        type=str,
        default=default_parameters.search_paths,
        nargs="*",
        help="list of search directory paths"
    )
    parser.add_argument(
        "-f", "--file-patterns",
        type=str,
        default=default_parameters.file_patterns,
        nargs="*",
        help="list of file patterns to include"
    )
    parser.add_argument(
        "-e", "--exclude-paths",
        type=str,
        default=default_parameters.exclude_paths,
        nargs="*",
        help="list of directory paths to exclude"
    )
    parser.add_argument(
        "-c", "--check",
        type=bool,
        default=default_parameters.check,
        action=argparse.BooleanOptionalAction,
        help="run format check"
    )

    return vars(parser.parse_args())


def find_files(
    search_paths: list[str],
    file_patterns: list[str],
    exclude_paths: list[str]
) -> list[pathlib.Path]:
    matching_files = []
    for search_path in search_paths:
        path = pathlib.Path(search_path)
        for pattern in file_patterns:
            matching_files.extend(path.rglob(pattern))

    filtered_files = [
        file for file in matching_files
        if not any(str(file.parent).startswith(path) for path in exclude_paths)
    ]

    return filtered_files


def run_clang_format(
    files: list[pathlib.Path],
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
    search_paths: list[str],
    file_patterns: list[str],
    exclude_paths: list[str],
    check: bool
):
    matching_files = find_files(search_paths, file_patterns, exclude_paths)
    sys.exit(run_clang_format(matching_files, check))


if __name__ == "__main__":
    main(**parse_args())
