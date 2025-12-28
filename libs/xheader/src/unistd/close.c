// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/unistd.h>

#ifndef __linux__

int close(int fd) { return -1; }

#endif
