# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from .vscode import vscode

setup: typer.Typer = typer.Typer()

setup.add_typer(vscode, name="vscode")
