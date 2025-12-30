# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from devutils.constants.paths import SettingsFiles

vscode: typer.Typer = typer.Typer()


@vscode.command()  # type: ignore[misc]
def settings(regenerate: bool = typer.Option(False, "--regenerate", "-r", help="Regenerate the settings file")) -> None:
    if SettingsFiles.vscode_settings.exists():
        if regenerate:
            typer.echo(f"Removing settings file: {SettingsFiles.vscode_settings}")
            SettingsFiles.vscode_settings.unlink()
        else:
            typer.echo(f"Settings file already exists: {SettingsFiles.vscode_settings}")
            raise typer.Exit(0)

    if not SettingsFiles.vscode_settings.parent.exists():
        typer.echo(f"Creating directory: {SettingsFiles.vscode_settings.parent}")
        SettingsFiles.vscode_settings.parent.mkdir(parents=True)

    typer.echo(f"Creating settings file: {SettingsFiles.vscode_settings}")
    with open(SettingsFiles.vscode_settings, "w") as f:
        f.write(
            """{
  "yaml.schemas": {
    "devutils/data/schemas/r2/config.schema.json": [
      "config.yaml"
    ],
    "devutils/data/schemas/r1/codegen.schema.json": [
      "**/codegen/*.yaml"
    ]
  }
}"""
        )
