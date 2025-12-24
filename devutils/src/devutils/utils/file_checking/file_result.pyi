import pathlib
from .file_status import FileStatus as FileStatus
from dataclasses import dataclass

@dataclass
class FileResult:
    path: pathlib.Path
    status: FileStatus
    error: str | None = ...
