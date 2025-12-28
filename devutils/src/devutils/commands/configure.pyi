# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from typing import TypedDict

import typer

from devutils.constants.paths import (
    CodegenFiles as CodegenFiles,
)
from devutils.constants.paths import (
    ConfigFiles as ConfigFiles,
)
from devutils.constants.paths import (
    Directories as Directories,
)
from devutils.constants.paths import (
    JsonSchemas as JsonSchemas,
)

configure: typer.Typer

class Configuration(TypedDict):
    revision: int
    enable_testing: bool
    enable_xheader_testing: bool
    enable_debug_testing: bool
    build_mode: str

def load_schema() -> object: ...
def load_configuration() -> Configuration | None: ...
def save_configuration(config: Configuration) -> None: ...
def check_codegen_files() -> None: ...
def run(reconfigure: bool = ...) -> None: ...
def main(ctx: typer.Context) -> None: ...
