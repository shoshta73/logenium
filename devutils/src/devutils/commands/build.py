import os
import shlex
import shutil
import subprocess
import sys

import typer

from devutils.constants import Directories, Files

build = typer.Typer()


@build.command()
def run(
    verbose: bool = typer.Option(False, "--verbose", "-v", help="Show verbose output"),
    jobs: int = typer.Option(0, "--jobs", "-j", help="Number of parallel jobs (0 = auto, capped to CPU count)"),
) -> None:
    typer.echo("Building the project...")

    if not Files.ninja_build_file.exists():
        typer.echo(typer.style("Ninja build file does not exist", fg=typer.colors.RED))
        typer.echo(typer.style("Run `logenium-devutils configure` first", fg=typer.colors.RED))
        raise typer.Exit(1)

    ninja_path = shutil.which("ninja")
    if not ninja_path:
        typer.echo(typer.style("Ninja is not installed", fg=typer.colors.RED))
        raise typer.Exit(1)

    # Cap jobs to CPU count
    cpu_count = os.cpu_count() or 1
    if jobs <= 0:
        jobs = cpu_count
    else:
        jobs = min(jobs, cpu_count)

    command_line = [ninja_path]
    if verbose:
        command_line.append("-v")
    command_line.extend(["-j", str(jobs)])
    command_line.extend(["-C", str(Directories.build)])

    command = shlex.join(command_line)
    typer.echo(f"Running command: {command}")

    process = subprocess.Popen(
        command_line,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        bufsize=1,
        text=True,
    )

    if process.stdout:
        buffer = ""
        while True:
            char = process.stdout.read(1)
            if not char:
                break

            buffer += char

            if char == "\r" or char == "\n":
                # Convert standalone \r to \r\n for proper display
                if buffer.endswith("\r") and not buffer.endswith("\r\n"):
                    sys.stdout.write(buffer[:-1] + "\r\n")
                else:
                    sys.stdout.write(buffer)
                sys.stdout.flush()
                buffer = ""

    returncode = process.wait()
    if returncode != 0:
        raise typer.Exit(returncode)

    typer.echo("Done!")


@build.callback(invoke_without_command=True)
def main(ctx: typer.Context) -> None:
    if ctx.invoked_subcommand is None:
        run(verbose=False, jobs=0)
