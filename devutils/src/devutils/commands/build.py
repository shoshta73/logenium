# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import os
import shlex
import shutil
import subprocess
import sys
import time

import typer

from devutils.constants import Directories, Files

build = typer.Typer()


def format_elapsed_time(seconds: float) -> str:
    ms = int((seconds % 1) * 1000)
    total_seconds = int(seconds)

    days = total_seconds // 86400
    hours = (total_seconds % 86400) // 3600
    minutes = (total_seconds % 3600) // 60
    secs = total_seconds % 60

    return f"{days}d {hours}h {minutes}m {secs}s {ms}ms"


@build.command()
def run(
    verbose: bool = typer.Option(False, "--verbose", "-v", help="Show verbose output"),
    jobs: int = typer.Option(0, "--jobs", "-j", help="Number of parallel jobs (0 = auto, capped to CPU count)"),
    no_ninja_override: bool = typer.Option(False, "--no-ninja-override", help="Do not override NINJA_STATUS"),
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

    os_env = os.environ.copy()
    start_time = time.time()

    if not no_ninja_override:
        os_env["NINJA_STATUS"] = "[%f/%t : %p - <TIME>] "

    process = subprocess.Popen(
        command_line,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        bufsize=1,
        text=True,
        env=os_env,
    )

    if process.stdout:
        buffer = ""
        while True:
            char = process.stdout.read(1)
            if not char:
                break

            buffer += char

            if char == "\r" or char == "\n":
                output_line = buffer

                # Replace <TIME> placeholder with formatted elapsed time
                if not no_ninja_override and "<TIME>" in output_line:
                    elapsed = time.time() - start_time
                    formatted_time = format_elapsed_time(elapsed)
                    output_line = output_line.replace("<TIME>", formatted_time)

                # Convert standalone \r to \r\n for proper display
                if output_line.endswith("\r") and not output_line.endswith("\r\n"):
                    sys.stdout.write(output_line[:-1] + "\r\n")
                else:
                    sys.stdout.write(output_line)
                sys.stdout.flush()
                buffer = ""

    returncode = process.wait()
    if returncode != 0:
        raise typer.Exit(returncode)

    typer.echo("Done!")


@build.callback(invoke_without_command=True)
def main(
    ctx: typer.Context,
    verbose: bool = typer.Option(False, "--verbose", "-v", help="Show verbose output"),
    jobs: int = typer.Option(0, "--jobs", "-j", help="Number of parallel jobs (0 = auto, capped to CPU count)"),
    no_ninja_override: bool = typer.Option(False, "--no-ninja-override", help="Do not override NINJA_STATUS"),
) -> None:
    if ctx.invoked_subcommand is None:
        run(verbose=verbose, jobs=jobs, no_ninja_override=no_ninja_override)
