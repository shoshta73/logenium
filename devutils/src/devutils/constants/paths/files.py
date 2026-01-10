# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass
from pathlib import Path

from .directories import Directories as _Directories


@dataclass(frozen=True)
class Files:
    corelib_doxygen_config: Path = _Directories.corelib_root / "Doxyfile"
    devutils_lint_cache_file: Path = _Directories.devutils_cache / "lint_cache.yaml"
    devutils_license_headers_cache_file: Path = _Directories.devutils_cache / "license_headers_cache.yaml"
    devutils_pyproject_toml: Path = _Directories.devutils_root / "pyproject.toml"
    debug_doxygen_config: Path = _Directories.debug_root / "Doxyfile"
    logging_doxygen_config: Path = _Directories.logging_root / "Doxyfile"
    ninja_build_file: Path = _Directories.build / "build.ninja"


DOXYGEN_CONFIGS: dict[str, Path] = {
    "corelib": Files.corelib_doxygen_config,
    "debug": Files.debug_doxygen_config,
    "logging": Files.logging_doxygen_config,
}
