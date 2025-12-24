from _typeshed import Incomplete
from dataclasses import dataclass, field

@dataclass(frozen=True)
class _Extensions:
    c_source: list[str] = field(default_factory=Incomplete)
    cpp_source: list[str] = field(default_factory=Incomplete)
    cmake_source: list[str] = field(default_factory=Incomplete)
    python_source: list[str] = field(default_factory=Incomplete)
    powershell_source: list[str] = field(default_factory=Incomplete)
    bat_source: list[str] = field(default_factory=Incomplete)

Extensions: Incomplete
