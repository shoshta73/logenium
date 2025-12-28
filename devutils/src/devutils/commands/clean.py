# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import os
import shutil
import stat
from collections.abc import Callable

import typer

from devutils.constants.paths import Directories
from devutils.utils.filesystem import find_directories_by_name

clean: typer.Typer = typer.Typer()


def handle_remove_readonly(func: Callable[[str], object], path: str, exc: BaseException) -> object:
    os.chmod(path, stat.S_IWRITE)
    return func(path)


@clean.command()  # type: ignore[misc]
def run() -> None:
    if not Directories.build.exists() and not Directories.cache.exists():
        typer.echo("Build directory does not exist. Nothing to clean.")
        return

    typer.echo(f"Removing {Directories.build}...")
    shutil.rmtree(Directories.build, onexc=handle_remove_readonly)

    typer.echo(f"Removing {Directories.cache}...")
    shutil.rmtree(Directories.cache, onexc=handle_remove_readonly)

    pycaches = find_directories_by_name(Directories.root, "__pycache__")
    for pycache in pycaches:
        typer.echo(f"Removing {pycache}...")
        shutil.rmtree(pycache, onexc=handle_remove_readonly)

    mypy_caches = find_directories_by_name(Directories.root, ".mypy_cache")
    for mypy_cache in mypy_caches:
        typer.echo(f"Removing {mypy_cache}...")
        shutil.rmtree(mypy_cache, onexc=handle_remove_readonly)

    ruff_caches = find_directories_by_name(Directories.root, ".ruff_cache")
    for ruff_cache in ruff_caches:
        typer.echo(f"Removing {ruff_cache}...")
        shutil.rmtree(ruff_cache, onexc=handle_remove_readonly)

    typer.echo("Done!")


@clean.callback(invoke_without_command=True)  # type: ignore[misc]
def main(ctx: typer.Context) -> None:
    if ctx.invoked_subcommand is None:
        run()
