from enum import Enum

class FileStatus(Enum):
    OK = 'ok'
    ISSUE = 'issue'
    ERROR = 'error'
    UNKNOWN = 'unknown'
