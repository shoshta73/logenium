import pathlib
from dataclasses import dataclass

from .file_status import FileStatus as FileStatus

@dataclass
class FileResult:
    path: pathlib.Path
    status: FileStatus
    error: str | None = ...
