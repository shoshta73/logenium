# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from collections.abc import Callable as Callable

import typer

from devutils.constants import Directories as Directories

clean: typer.Typer

def handle_remove_readonly(func: Callable[[str], object], path: str, exc: BaseException) -> object: ...
def run() -> None: ...
def main(ctx: typer.Context) -> None: ...
