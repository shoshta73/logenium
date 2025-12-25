# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from .stubgen import stubgen

python: typer.Typer = typer.Typer()

python.add_typer(stubgen, name="stubgen", help="Generate stubs for Python projects")
