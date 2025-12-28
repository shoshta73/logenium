# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import json
import shlex
import shutil
import subprocess
from typing import TypedDict, cast

import jsonschema
import typer
import yaml

from devutils.constants.paths import CodegenFiles, ConfigFiles, Directories, JsonSchemas

configure: typer.Typer = typer.Typer()


class Configuration(TypedDict):
    revision: int
    enable_testing: bool
    enable_xheader_testing: bool
    enable_debug_testing: bool
    build_mode: str


def load_schema() -> object:
    with JsonSchemas.config.open("r", encoding="utf-8") as f:
        schema: object = json.load(f)
    return schema


def load_configuration() -> Configuration | None:
    if not ConfigFiles.config.exists():
        return None

    with ConfigFiles.config.open("r", encoding="utf-8") as f:
        data: object = yaml.safe_load(f)

    if not isinstance(data, dict):
        typer.echo(
            typer.style("[WARNING]", fg="yellow") + " Config file is not a valid YAML dictionary",
            err=True,
        )
        return None

    schema = load_schema()

    try:
        jsonschema.validate(instance=data, schema=schema)  # type: ignore[arg-type]
    except jsonschema.ValidationError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Config validation failed: {e.message}",
            err=True,
        )
        typer.echo(f"  Path: {'.'.join(str(p) for p in e.path)}", err=True)
        return None
    except jsonschema.SchemaError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Schema error: {e.message}",
            err=True,
        )
        return None

    return cast(Configuration, data)


def save_configuration(config: Configuration) -> None:
    schema = load_schema()

    try:
        jsonschema.validate(instance=config, schema=schema)  # type: ignore[arg-type]
    except jsonschema.ValidationError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Config validation failed: {e.message}",
            err=True,
        )
        raise typer.Exit(1) from e

    with ConfigFiles.config.open("w", encoding="utf-8") as f:
        yaml.safe_dump(dict(config), f, default_flow_style=False, sort_keys=False)


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
def run(
    reconfigure: bool = typer.Option(False, "--reconfigure", help="Remove saved configuration and reconfigure"),
) -> None:
    typer.echo("Configuring the project...")
    check_codegen_files()

    if reconfigure and ConfigFiles.config.exists():
        ConfigFiles.config.unlink()
        typer.echo("Removed saved configuration")

    config = load_configuration()

    if config is not None:
        typer.echo("Using saved configuration from config.yaml")
        enable_testing = config["enable_testing"]
        enable_xheader_testing = config["enable_xheader_testing"]
        enable_debug_testing = config["enable_debug_testing"]
        mode = config["build_mode"]
    else:
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

        mode_input: str = cast(str, typer.prompt("In What mode do you want to build?"))
        mode_map = {
            "debug": "Debug",
            "release": "Release",
            "relwithdebinfo": "RelWithDebInfo",
            "minsizerel": "MinSizeRel",
        }

        if mode_input.lower() not in mode_map:
            typer.echo(typer.style("Invalid mode, defaulting to Release", fg=typer.colors.RED))
            mode = "Release"
        else:
            mode = mode_map[mode_input.lower()]

        new_config: Configuration = {
            "revision": 1,
            "enable_testing": enable_testing,
            "enable_xheader_testing": enable_xheader_testing,
            "enable_debug_testing": enable_debug_testing,
            "build_mode": mode,
        }
        save_configuration(new_config)
        typer.echo(f"Configuration saved to {ConfigFiles.config.relative_to(Directories.root)}")

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
