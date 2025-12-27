# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass
from pathlib import Path

from .directories import Directories as _Directories


@dataclass(frozen=True)
class CodegenFiles:
    xdg_shell_client_header: Path = (
        _Directories.xheader_include / "xheader" / "internal" / "xdg-shell-client-protocol.h"
    )
    xdg_shell_protocol_source: Path = _Directories.xheader_source / "internal" / "xdg-shell-protocol.c"
    xdg_decoration_client_header: Path = (
        _Directories.xheader_include / "xheader" / "internal" / "xdg-decoration-unstable-v1-client-protocol.h"
    )
    xdg_decoration_protocol_source: Path = (
        _Directories.xheader_source / "internal" / "xdg-decoration-unstable-v1-protocol.c"
    )
