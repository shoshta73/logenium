from dataclasses import dataclass
from pathlib import Path

_ROOT_DIR: Path = Path(__file__).parent.parent.parent.parent.parent.resolve()
_BUILD_DIR: Path = _ROOT_DIR / "build"
_LIBS_DIR: Path = _ROOT_DIR / "libs"


@dataclass(frozen=True)
class _Directories:
    root: Path = _ROOT_DIR
    build: Path = _BUILD_DIR
    libs: Path = _LIBS_DIR

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

    devutils_root: Path = _ROOT_DIR / "devutils"
    devutils_source: Path = _ROOT_DIR / "devutils" / "src"


@dataclass(frozen=True)
class _Files:
    ninja_build_file: Path = _BUILD_DIR / "build.ninja"


Directories = _Directories()
Files = _Files()
