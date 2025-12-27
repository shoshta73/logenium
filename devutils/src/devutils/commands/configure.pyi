# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from devutils.constants.paths import CodegenFiles as CodegenFiles
from devutils.constants.paths import Directories as Directories

configure: typer.Typer

def check_codegen_files() -> None: ...
def run() -> None: ...
def main(ctx: typer.Context) -> None: ...
