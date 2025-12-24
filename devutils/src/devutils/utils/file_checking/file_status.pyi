from enum import Enum

class FileStatus(Enum):
    OK = "ok"
    WARNING = "warning"
    ISSUE = "issue"
    ERROR = "error"
    UNKNOWN = "unknown"
