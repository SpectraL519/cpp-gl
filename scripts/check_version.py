import argparse
import re
import sys
from pathlib import Path
from collections.abc import Iterable


VERSION_REGEX = r"(\d+\.\d+(?:\.\d+)*)"


def get_cmake_version(cmake_path: Path) -> str:
    text = cmake_path.read_text()
    match = re.search(
        rf"project\s*\([^\)]*VERSION\s+{VERSION_REGEX}", text, re.IGNORECASE
    )
    if match:
        return match.group(1)
    raise ValueError(f"[CMake] Could not find a valid project version in {cmake_path}")


def get_doxy_version(doxyfile_path: Path) -> str:
    text = doxyfile_path.read_text()
    match = re.search(
        rf'^\s*PROJECT_NUMBER\s*=\s*("?){VERSION_REGEX}\1', text, re.MULTILINE
    )
    if match:
        return match.group(2)  # group(2) because group(1) is the optional quote
    raise ValueError(
        f"[Doxygen] Could not find a valid PROJECT_NUMBER in {doxyfile_path}"
    )


def all_equal(items: Iterable) -> bool:
    return len(set(items)) == 1


def main(cmake: Path, doxygen: Path):
    try:
        project_versions = {
            "CMake": get_cmake_version(cmake),
            "Doxygen": get_doxy_version(doxygen),
        }
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

    if not all_equal(project_versions.values()):
        version_msg_entries = [
            f"{source}: {version}" for source, version in project_versions.items()
        ]
        print(
            f"Error: Project version mismatch:\n  {'\n  '.join(version_msg_entries)}",
            file=sys.stderr,
        )
        sys.exit(1)

    print(project_versions["CMake"])  # print the version to stdout for shell capture


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-c",
        "--cmake",
        type=Path,
        default="CMakeLists.txt",
        nargs=1,
        help="Path to the root CMake file",
    )
    parser.add_argument(
        "-d",
        "--doxygen",
        type=Path,
        default="Doxyfile",
        nargs=1,
        help="Path to the Doxygen config file",
    )

    main(**vars(parser.parse_args()))
