import pathlib
from dataclasses import dataclass

@dataclass
class LanguageConfig:
    name: str
    extensions: list[str]
    search_dirs: list[pathlib.Path]
    specific_files: list[pathlib.Path]
    def collect_files(self) -> list[pathlib.Path]: ...
