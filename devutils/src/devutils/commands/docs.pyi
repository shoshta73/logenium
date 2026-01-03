# SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from devutils.constants.paths.files import DOXYGEN_CONFIGS as DOXYGEN_CONFIGS

docs: typer.Typer

def check_doxygen_is_available() -> bool: ...
def build() -> None: ...
def main(ctx: typer.Context) -> None: ...
