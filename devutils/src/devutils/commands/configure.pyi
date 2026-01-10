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

class LibraryConfig(TypedDict):
    enable_testing: bool

class CorelibLibraryConfig(LibraryConfig):
    enable_tracing: bool

class LoggingLibraryConfig(LibraryConfig):
    use_fmtlib: bool

class DebugLibraryConfig(LibraryConfig):
    use_fast_stacktrace: bool
    use_fmtlib: bool
    enable_color_logs: bool

class ConfigurationR1(TypedDict):
    revision: int
    enable_testing: bool
    enable_xheader_testing: bool
    enable_debug_testing: bool
    enable_corelib_testing: bool
    build_mode: str

class ConfigurationR2(TypedDict):
    revision: int
    enable_testing: bool
    build_mode: str
    logenium: LibraryConfig
    xheader: LibraryConfig
    debug: DebugLibraryConfig
    corelib: CorelibLibraryConfig
    logging: LoggingLibraryConfig

def load_schema_r1() -> object: ...
def load_schema_r2() -> object: ...
def load_configuration_r1() -> ConfigurationR1 | None: ...
def load_configuration_r2() -> ConfigurationR2 | None: ...
def save_configuration_r2(config: ConfigurationR2) -> None: ...
def migrate_r1_to_r2(config_r1: ConfigurationR1) -> ConfigurationR2: ...
def migrate_command() -> None: ...
def check_codegen_files() -> None: ...
def run(reconfigure: bool = ...) -> None: ...
def main(ctx: typer.Context, reconfigure: bool = ...) -> None: ...
