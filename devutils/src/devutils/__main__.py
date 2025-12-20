import typer

from .commands import build, configure

app = typer.Typer()

app.add_typer(configure, name="configure", help="Configure the project")
app.add_typer(build, name="build", help="Build the project")


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
