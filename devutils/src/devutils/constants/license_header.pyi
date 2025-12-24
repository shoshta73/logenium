from .comments import Comments as Comments
from _typeshed import Incomplete
from dataclasses import dataclass, field

@dataclass(frozen=True)
class _LicenseHeaders:
    c: list[str] = field(default_factory=Incomplete)
    cpp: list[str] = field(default_factory=Incomplete)
    python: list[str] = field(default_factory=Incomplete)
    cmake: list[str] = field(default_factory=Incomplete)
    powershell: list[str] = field(default_factory=Incomplete)
    bat: list[str] = field(default_factory=Incomplete)

LicenseHeaders: Incomplete
