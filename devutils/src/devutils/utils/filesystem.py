# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib


def find_files_by_extensions(path: pathlib.Path, extensions: list[str]) -> list[pathlib.Path]:
    files: list[pathlib.Path] = []
    for extension in extensions:
        files.extend(path.rglob(f"*{extension}"))
    return sorted(files)


def get_files_recursively(path: pathlib.Path) -> list[pathlib.Path]:
    return sorted(path.rglob("*"))
