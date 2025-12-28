# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib

def get_earliest_git_year(file_path: pathlib.Path) -> int | None: ...
def get_file_copyright_year(file_path: pathlib.Path) -> int: ...
