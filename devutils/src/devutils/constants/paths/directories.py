# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass
from pathlib import Path

_ROOT_DIR: Path = Path(__file__).parent.parent.parent.parent.parent.parent.resolve()
_BUILD_DIR: Path = _ROOT_DIR / "build"
_LIBS_DIR: Path = _ROOT_DIR / "libs"
_CAHCE_DIR: Path = _ROOT_DIR / ".cache"
_VSCODE_DIR: Path = _ROOT_DIR / ".vscode"


@dataclass(frozen=True)
class Directories:
    root: Path = _ROOT_DIR
    build: Path = _BUILD_DIR
    libs: Path = _LIBS_DIR
    cache: Path = _CAHCE_DIR
    vscode: Path = _VSCODE_DIR

    logenium_source: Path = _ROOT_DIR / "src"
    logenium_include: Path = _ROOT_DIR / "include"
    logenium_cmake: Path = _ROOT_DIR / "cmake"
    logenium_tests: Path = _ROOT_DIR / "tests"

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

    corelib_root: Path = _LIBS_DIR / "corelib"
    corelib_source: Path = _LIBS_DIR / "corelib" / "src"
    corelib_include: Path = _LIBS_DIR / "corelib" / "include"
    corelib_cmake: Path = _LIBS_DIR / "corelib" / "cmake"
    corelib_tests: Path = _LIBS_DIR / "corelib" / "tests"
    corelib_docs: Path = _LIBS_DIR / "corelib" / "docs"


DOCS_DIRS: list[Path] = [
    Directories.corelib_docs,
]
