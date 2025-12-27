# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import shlex
import shutil
import subprocess
from typing import cast

import typer

from devutils.constants.paths import CodegenFiles, Directories

configure: typer.Typer = typer.Typer()


def check_codegen_files() -> None:
    required_files = [
        CodegenFiles.xdg_shell_client_header,
        CodegenFiles.xdg_shell_protocol_source,
        CodegenFiles.xdg_decoration_client_header,
        CodegenFiles.xdg_decoration_protocol_source,
    ]

    missing_files = [f for f in required_files if not f.exists()]

    if missing_files:
        typer.echo(
            f"{typer.style('[WARNING]', fg='yellow')} Required Wayland protocol files are missing:",
            err=True,
        )
        for file in missing_files:
            typer.echo(f"  - {file.relative_to(Directories.root)}", err=True)
        typer.echo(
            f"\n{typer.style('[ACTION]', fg='cyan')} Run the following command to generate them:",
            err=True,
        )
        typer.echo("  uv run devutils codegen wayland\n", err=True)
        raise typer.Exit(1)


@configure.command()  # type: ignore[misc]
def run() -> None:
    typer.echo("Configuring the project...")
    check_codegen_files()

    enable_testing = typer.confirm("Do you want to enable testing?")
    enable_xheader_testing = False
    enable_debug_testing = False

    if enable_testing:
        typer.echo("Testing is enabled")

        enable_xheader_testing = typer.confirm("Do you want to enable xheader testing?")
        if enable_xheader_testing:
            typer.echo("Xheader testing is enabled")
        else:
            typer.echo("Xheader testing is disabled")

        enable_debug_testing = typer.confirm("Do you want to enable debug testing?")
        if enable_debug_testing:
            typer.echo("Debug testing is enabled")
        else:
            typer.echo("Debug testing is disabled")

    else:
        typer.echo("Testing is disabled")

    mode: str = cast(str, typer.prompt("In What mode do you want to build?"))
    mode_map = {
        "debug": "Debug",
        "release": "Release",
        "relwithdebinfo": "RelWithDebInfo",
        "minsizerel": "MinSizeRel",
    }

    if mode.lower() not in mode_map:
        typer.echo(typer.style("Invalid mode, defaulting to Release", fg=typer.colors.RED))
        mode = "Release"
    else:
        mode = mode_map[mode.lower()]

    cmake_path = shutil.which("cmake")
    if cmake_path is None:
        typer.echo(typer.style("CMake is not installed", fg=typer.colors.RED))
        raise typer.Exit(1)

    command_line = [cmake_path]
    command_line.extend(["-S", str(Directories.root)])
    command_line.extend(["-B", str(Directories.build)])
    command_line.extend(["-DCMAKE_BUILD_TYPE=" + mode])
    command_line.extend([f"-DLOGENIUM_BUILD_TESTS={'ON' if enable_testing else 'OFF'}"])
    command_line.extend([f"-DLOGENIUM_XHEADER_BUILD_TESTS={'ON' if enable_xheader_testing else 'OFF'}"])
    command_line.extend([f"-DLOGENIUM_DEBUG_BUILD_TESTS={'ON' if enable_debug_testing else 'OFF'}"])
    command_line.extend(["-G", "Ninja"])

    command = shlex.join(command_line)
    typer.echo(f"Running command: {command}")
    subprocess.run(command_line, check=True)
    typer.echo("Done!")


@configure.callback(invoke_without_command=True)  # type: ignore[misc]
def main(ctx: typer.Context) -> None:
    if ctx.invoked_subcommand is None:
        run()
