# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from devutils.constants.paths import Directories as Directories
from devutils.constants.paths import Files as Files

build: typer.Typer

def format_elapsed_time(seconds: float) -> str: ...
def run(verbose: bool = ..., jobs: int = ..., no_ninja_override: bool = ...) -> None: ...
def main(ctx: typer.Context, verbose: bool = ..., jobs: int = ..., no_ninja_override: bool = ...) -> None: ...
