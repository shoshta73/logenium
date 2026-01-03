# SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import shutil
import subprocess
import sys

import typer

from devutils.constants.paths.files import DOXYGEN_CONFIGS
from devutils.utils.filesystem import find_files_by_extensions

docs: typer.Typer = typer.Typer()


def check_doxygen_is_available() -> bool:
    if shutil.which("doxygen") is None:
        typer.echo(typer.style("Error: Doxygen is not available. Please ensure it is installed.", fg="red", bold=True))
        return False
    return True


@docs.command()  # type: ignore[misc]
def build(ci: bool = typer.Option(False, "--ci", help="Ci mode, removes unnecessary files")) -> None:
    if not check_doxygen_is_available():
        sys.exit(1)

    for project_name, config_path in DOXYGEN_CONFIGS.items():
        if not config_path.exists():
            typer.echo(typer.style(f"Error: Doxygen config file {config_path} does not exist.", fg="red", bold=True))
            sys.exit(1)

        typer.echo(typer.style(f"Building docs for {project_name}...", fg="cyan", bold=True))

        cwd = config_path.parent

        result = subprocess.run(
            ["doxygen", str(config_path)],
            cwd=cwd,
            capture_output=True,
            text=True,
            check=False,
        )

        if result.returncode != 0:
            typer.echo(typer.style(f"Error: Failed to build docs for {project_name}.", fg="red", bold=True))
            if result.stdout:
                typer.echo(result.stdout)
            if result.stderr:
                typer.echo(result.stderr)
            sys.exit(1)

        if ci:
            docs_dir = config_path.parent / "docs"
            temp_files = find_files_by_extensions(docs_dir, [".map", ".md5"])
            for file in temp_files:
                typer.echo(typer.style(f"Removing {file}...", fg="cyan", bold=True))
                file.unlink()

    typer.echo(typer.style("\nDocumentation built successfully!", fg="green", bold=True))


@docs.callback(invoke_without_command=True)  # type: ignore[misc]
def main(ctx: typer.Context) -> None:
    if ctx.invoked_subcommand is None:
        build()
