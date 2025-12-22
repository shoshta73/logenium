# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from enum import Enum


class FileStatus(Enum):
    OK = "ok"
    ISSUE = "issue"
    ERROR = "error"
    UNKNOWN = "unknown"
