# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from devutils.constants.paths import ConfigFiles, SettingsFiles

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


@vscode.command()  # type: ignore[misc]
def bookmarks(
    regenerate: bool = typer.Option(False, "--regenerate", "-r", help="Regenerate the bookmarks file"),
) -> None:
    if ConfigFiles.vscode_bookmarks.exists():
        if regenerate:
            typer.echo(f"Removing bookmarks file: {ConfigFiles.vscode_bookmarks}")
            ConfigFiles.vscode_bookmarks.unlink()
        else:
            typer.echo(f"Bookmarks file already exists: {ConfigFiles.vscode_bookmarks}")
            raise typer.Exit(0)

    if not ConfigFiles.vscode_bookmarks.parent.exists():
        typer.echo(f"Creating directory: {ConfigFiles.vscode_bookmarks.parent}")
        ConfigFiles.vscode_bookmarks.parent.mkdir(parents=True)

    typer.echo(f"Creating bookmarks file: {ConfigFiles.vscode_bookmarks}")
    with open(ConfigFiles.vscode_bookmarks, "w") as f:
        f.write("""{
  "files": [
    {
      "path": "libs/debug/CMakeLists.txt",
      "bookmarks": [
        {
          "line": 5,
          "column": 21,
          "label": ""
        }
      ]
    },
    {
      "path": "libs/debug/Doxyfile",
      "bookmarks": [
        {
          "line": 50,
          "column": 2,
          "label": ""
        }
      ]
    },
    {
      "path": "libs/corelib/Doxyfile",
      "bookmarks": [
        {
          "line": 50,
          "column": 1,
          "label": ""
        }
      ]
    },
    {
      "path": "libs/corelib/CMakeLists.txt",
      "bookmarks": [
        {
          "line": 5,
          "column": 21,
          "label": ""
        }
      ]
    },
    {
      "path": "libs/xheader/CMakeLists.txt",
      "bookmarks": [
        {
          "line": 6,
          "column": 12,
          "label": ""
        }
      ]
    },
    {
      "path": "CMakeLists.txt",
      "bookmarks": [
        {
          "line": 23,
          "column": 17,
          "label": ""
        }
      ]
    }
  ]
}
""")
