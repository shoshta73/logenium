# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

configure: typer.Typer

def run() -> None: ...
def main(ctx: typer.Context) -> None: ...
