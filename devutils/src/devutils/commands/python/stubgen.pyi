# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from devutils.constants import Directories as Directories

stubgen: typer.Typer

def check_stubgen_available() -> bool: ...
def generate(verbose: bool = ...) -> None: ...
def check(verbose: bool = ...) -> None: ...
