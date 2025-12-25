# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass
from pathlib import Path

_ROOT_DIR: Path = Path(__file__).parent.parent.parent.parent.parent.resolve()
_BUILD_DIR: Path = _ROOT_DIR / "build"
_LIBS_DIR: Path = _ROOT_DIR / "libs"
_CAHCE_DIR: Path = _ROOT_DIR / ".cache"


@dataclass(frozen=True)
class Directories:
    root: Path = _ROOT_DIR
    build: Path = _BUILD_DIR
    libs: Path = _LIBS_DIR
    cache: Path = _CAHCE_DIR

    logenium_source: Path = _ROOT_DIR / "src"
    logenium_include: Path = _ROOT_DIR / "include"
    logenium_cmake: Path = _ROOT_DIR / "cmake"

    xheader_root: Path = _LIBS_DIR / "xheader"
    xheader_source: Path = _LIBS_DIR / "xheader" / "src"
    xheader_include: Path = _LIBS_DIR / "xheader" / "include"
    xheader_cmake: Path = _LIBS_DIR / "xheader" / "cmake"
    xheader_tests: Path = _LIBS_DIR / "xheader" / "tests"

    debug_root: Path = _LIBS_DIR / "debug"
    debug_source: Path = _LIBS_DIR / "debug" / "src"
    debug_include: Path = _LIBS_DIR / "debug" / "include"
    debug_cmake: Path = _LIBS_DIR / "debug" / "cmake"
    debug_tests: Path = _LIBS_DIR / "debug" / "tests"

    devutils_root: Path = _ROOT_DIR / "devutils"
    devutils_source: Path = _ROOT_DIR / "devutils" / "src"
    devutils_cache: Path = _CAHCE_DIR / "devutils"


@dataclass(frozen=True)
class Files:
    ninja_build_file: Path = Directories.build / "build.ninja"
    devutils_lint_cache_file: Path = Directories.devutils_cache / "lint_cache.yaml"
    devutils_pyproject_toml: Path = Directories.devutils_root / "pyproject.toml"


__all__ = [
    "Directories",
    "Files",
]
