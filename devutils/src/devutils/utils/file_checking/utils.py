# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib

import typer

from devutils.constants import Directories
from devutils.utils.filesystem import find_files_by_extensions


def collect_files(
    extensions: list[str],
    search_dirs: list[pathlib.Path],
    specific_files: list[pathlib.Path],
) -> list[pathlib.Path]:
    files: list[pathlib.Path] = []

    for search_dir in search_dirs:
        if search_dir.exists():
            files.extend(find_files_by_extensions(search_dir, extensions))

    for specific_file in specific_files:
        if specific_file.exists():
            files.append(specific_file)

    return files


def format_file_path(file_path: pathlib.Path) -> str:
    return str(file_path.relative_to(Directories.root))


def print_status(status_label: str, color: str, file_path: pathlib.Path, message: str = "") -> None:
    formatted_path = format_file_path(file_path)
    if message:
        typer.echo(f"{typer.style(status_label, fg=color)} {formatted_path}: {message}")
    else:
        typer.echo(f"{typer.style(status_label, fg=color)} {formatted_path}")
