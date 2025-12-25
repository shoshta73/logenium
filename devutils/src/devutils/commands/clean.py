# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import os
import shutil
import stat
from collections.abc import Callable

import typer

from devutils.constants import Directories

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

    typer.echo("Done!")


@clean.callback(invoke_without_command=True)  # type: ignore[misc]
def main(ctx: typer.Context) -> None:
    if ctx.invoked_subcommand is None:
        run()
