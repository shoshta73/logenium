# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
import subprocess
from datetime import datetime


def get_earliest_git_year(file_path: pathlib.Path) -> int | None:
    try:
        result = subprocess.run(
            ["git", "log", "--follow", "--format=%aI", "--reverse", "--", str(file_path)],
            capture_output=True,
            text=True,
            check=True,
        )

        if result.stdout.strip():
            first_commit_date = result.stdout.strip().split("\n")[0]
            return datetime.fromisoformat(first_commit_date).year

        return None

    except (subprocess.CalledProcessError, FileNotFoundError, ValueError):
        return None


def get_file_copyright_year(file_path: pathlib.Path) -> int:
    if file_path.suffix == ".pyi":
        py_file = file_path.with_suffix(".py")
        if py_file.exists():
            year = get_earliest_git_year(py_file)
            if year is not None:
                return year

    year = get_earliest_git_year(file_path)
    if year is not None:
        return year

    return datetime.now().year
