import os
import shutil
import stat

import typer

from devutils.constants import Directories

clean = typer.Typer()


def handle_remove_readonly(func, path, exc):
    os.chmod(path, stat.S_IWRITE)
    func(path)


@clean.command()
def run() -> None:
    if not Directories.build.exists():
        typer.echo("Build directory does not exist. Nothing to clean.")
        return

    typer.echo(f"Removing {Directories.build}...")
    shutil.rmtree(Directories.build, onexc=handle_remove_readonly)
    typer.echo("Done!")


@clean.callback(invoke_without_command=True)
def main(ctx: typer.Context) -> None:
    if ctx.invoked_subcommand is None:
        run()
