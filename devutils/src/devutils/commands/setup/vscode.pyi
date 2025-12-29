# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import typer

from devutils.constants.paths import SettingsFiles as SettingsFiles

vscode: typer.Typer

def settings(regenerate: bool = ...) -> None: ...
