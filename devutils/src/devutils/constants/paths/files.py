# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass
from pathlib import Path

from .directories import Directories as _Directories


@dataclass(frozen=True)
class Files:
    ninja_build_file: Path = _Directories.build / "build.ninja"
    devutils_lint_cache_file: Path = _Directories.devutils_cache / "lint_cache.yaml"
    devutils_license_headers_cache_file: Path = _Directories.devutils_cache / "license_headers_cache.yaml"
    devutils_pyproject_toml: Path = _Directories.devutils_root / "pyproject.toml"
    corelib_doxygen_config: Path = _Directories.corelib_root / "Doxyfile"


DOXYGEN_CONFIGS: dict[str, Path] = {
    "corelib": Files.corelib_doxygen_config,
}
