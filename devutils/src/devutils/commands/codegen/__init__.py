# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from .wayland import wayland

codegen = typer.Typer()

codegen.add_typer(wayland, name="wayland", help="Generate Wayland protocol files")
