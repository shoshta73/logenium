# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import json
import shutil
import subprocess
import sys
from typing import cast

import jsonschema
import typer
import yaml

from devutils.constants.paths import ConfigFiles, Directories, JsonSchemas

# Type aliases for config structure
ConfigEntry = dict[str, str | list[str]]
WaylandConfig = dict[str, list[ConfigEntry]]

wayland: typer.Typer = typer.Typer()


def check_wayland_scanner_available() -> bool:
    return shutil.which("wayland-scanner") is not None


def validate_config_schema(config: WaylandConfig) -> None:
    schema_file = JsonSchemas.codegen
    if not schema_file.exists():
        typer.echo(
            f"{typer.style('[WARNING]', fg='yellow')} Schema file not found: {schema_file}",
            err=True,
        )
        typer.echo("Skipping schema validation...", err=True)
        return

    with schema_file.open("r") as f:
        schema = json.load(f)  # type: ignore[misc]

    try:
        jsonschema.validate(instance=config, schema=schema)  # type: ignore[misc]
        typer.echo(f"{typer.style('[OK]', fg='green')} Configuration validated against schema")
    except jsonschema.ValidationError as e:  # type: ignore[misc]
        typer.echo(
            f"{typer.style('[ERROR]', fg='red')} Configuration validation failed:",
            err=True,
        )
        typer.echo(f"  Path: {' -> '.join(str(p) for p in e.path)}", err=True)
        typer.echo(f"  Message: {e.message}", err=True)
        raise typer.Exit(1) from e
    except jsonschema.SchemaError as e:  # type: ignore[misc]
        typer.echo(
            f"{typer.style('[ERROR]', fg='red')} Invalid schema file:",
            err=True,
        )
        typer.echo(f"  Message: {e.message}", err=True)
        raise typer.Exit(1) from e


def evaluate_condition(condition: str) -> bool:
    allowed_vars: dict[str, object] = {"sys": sys}
    try:
        return bool(eval(condition, {"__builtins__": {}}, allowed_vars))  # type: ignore[misc]
    except Exception:
        return False


def substitute_variables(command: list[str], variables: dict[str, str]) -> list[str]:
    result = []
    for item in command:
        for var_name, var_value in variables.items():
            item = item.replace(f"${{{var_name}}}", var_value)
        result.append(item)
    return result


def generate_file_from_config(file_path: str, config: list[ConfigEntry]) -> bool:
    output_path = Directories.xheader_root / file_path
    output_path.parent.mkdir(parents=True, exist_ok=True)

    for entry in config:
        condition_value = entry.get("if", "")
        condition = condition_value if isinstance(condition_value, str) else ""
        if not evaluate_condition(condition):
            continue

        if "command" in entry:
            protocol_path_value = entry.get("protocol_path", "")
            protocol_path = protocol_path_value if isinstance(protocol_path_value, str) else ""
            command_value = entry["command"]
            command = command_value if isinstance(command_value, list) else [command_value]

            variables: dict[str, str] = {
                "PROTOCOL_PATH": protocol_path,
                "OUTPUT": str(output_path),
            }

            substituted_command = substitute_variables(command, variables)
            substituted_command.append(str(output_path))

            typer.echo(f"  Running: {' '.join(substituted_command)}")
            result = subprocess.run(
                substituted_command,
                capture_output=True,
                text=True,
                check=False,
            )

            if result.returncode != 0:
                typer.echo(f"{typer.style('[FAIL]', fg='red')} Failed to generate {file_path}", err=True)
                if result.stderr:
                    typer.echo(result.stderr, err=True)
                return False

            return True

        elif "cotents" in entry or "contents" in entry:
            contents_value = entry.get("cotents", entry.get("contents", ""))
            contents = contents_value if isinstance(contents_value, str) else "\n".join(contents_value)
            output_path.write_text(contents)
            return True

    typer.echo(
        f"{typer.style('[WARNING]', fg='yellow')} No matching configuration for {file_path} on {sys.platform}",
        err=True,
    )
    return False


@wayland.callback(invoke_without_command=True)  # type: ignore[misc]
def main(ctx: typer.Context) -> None:
    if sys.platform == "linux" and not check_wayland_scanner_available():
        typer.echo(
            f"{typer.style('[ERROR]', fg='red')} wayland-scanner is not available in PATH",
            err=True,
        )
        typer.echo("Install wayland-scanner using your package manager:", err=True)
        typer.echo("  - Arch/Manjaro: sudo pacman -S wayland-protocols", err=True)
        typer.echo("  - Ubuntu/Debian: sudo apt install wayland-protocols libwayland-dev", err=True)
        typer.echo("  - Fedora: sudo dnf install wayland-protocols-devel", err=True)
        raise typer.Exit(1)

    config_file = ConfigFiles.xheader_wayland_codegen
    if not config_file.exists():
        typer.echo(
            f"{typer.style('[ERROR]', fg='red')} Configuration file not found: {config_file}",
            err=True,
        )
        raise typer.Exit(1)

    with config_file.open("r") as f:
        config = cast(WaylandConfig, yaml.safe_load(f))

    validate_config_schema(config)

    typer.echo("\nGenerating Wayland protocol files...")

    for file_path, file_config in config.items():
        if file_path == "revision":
            continue
        typer.echo(f"\nProcessing {file_path}...")
        if not generate_file_from_config(file_path, file_config):
            raise typer.Exit(1)

    typer.echo(f"\n{typer.style('[SUCCESS]', fg='green')} Wayland protocol files generated successfully")
