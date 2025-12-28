// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/unistd.h>

#ifndef __linux__

int ftruncate(int fd, int64_t length) { return -1; }

#endif
