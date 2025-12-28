# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib


def find_directories_by_name(path: pathlib.Path, name: str) -> list[pathlib.Path]:
    return sorted(d for d in path.rglob(name) if d.is_dir())


def find_files_by_name(path: pathlib.Path, name: str) -> list[pathlib.Path]:
    return sorted(f for f in path.rglob(name) if f.is_file())


def find_files_by_extensions(path: pathlib.Path, extensions: list[str]) -> list[pathlib.Path]:
    files: list[pathlib.Path] = []
    for extension in extensions:
        files.extend(path.rglob(f"*{extension}"))
    return sorted(files)


def get_files_recursively(path: pathlib.Path) -> list[pathlib.Path]:
    return sorted(path.rglob("*"))
