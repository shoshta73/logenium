import pathlib

from devutils.constants import Directories as Directories
from devutils.utils.filesystem import find_files_by_extensions as find_files_by_extensions

def collect_files(
    extensions: list[str], search_dirs: list[pathlib.Path], specific_files: list[pathlib.Path]
) -> list[pathlib.Path]: ...
def format_file_path(file_path: pathlib.Path) -> str: ...
def print_status(status_label: str, color: str, file_path: pathlib.Path, message: str = "") -> None: ...
