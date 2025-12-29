# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from .remove_pycache import remove_pycache
from .stubgen import stubgen

python: typer.Typer = typer.Typer()

python.add_typer(stubgen, name="stubgen", help="Generate stubs for Python projects")
python.add_typer(remove_pycache, name="remove-pycache", help="Remove __pycache__ directories")
