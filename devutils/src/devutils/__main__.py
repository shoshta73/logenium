# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from .commands import build, check_license_headers, clean, configure, format, lint

app = typer.Typer()

app.add_typer(build, name="build", help="Build the project")
app.add_typer(check_license_headers, name="check-license-headers", help="Check license headers")
app.add_typer(check_license_headers, name="cls", help="Alias for check-license-headers")
app.add_typer(clean, name="clean", help="Clean the project")
app.add_typer(configure, name="configure", help="Configure the project")
app.add_typer(format, name="format", help="Format the project")
app.add_typer(lint, name="lint", help="Lint the project")


def version_callback(value: bool) -> None:
    if value:
        from . import __version__

        typer.echo(f"Devutils version: {__version__}")
        raise typer.Exit(0)


@app.callback(invoke_without_command=True)
def main(
    ctx: typer.Context,
    version: bool = typer.Option(
        False,
        "--version",
        "-v",
        help="Show the version and exit",
        callback=version_callback,
        is_eager=True,
    ),
) -> None:
    if ctx.invoked_subcommand is None:
        typer.echo(ctx.get_help())
        raise typer.Exit(0)


if __name__ == "__main__":
    app()
