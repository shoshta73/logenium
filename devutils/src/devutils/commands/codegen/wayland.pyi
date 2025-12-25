# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from devutils.constants import Directories as Directories

wayland: typer.Typer

def check_wayland_scanner_available() -> bool: ...
def main(ctx: typer.Context) -> None: ...
