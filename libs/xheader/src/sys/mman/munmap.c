// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/sys/mman.h>

#ifndef __linux__

int munmap(void *addr, size_t length) { return -1; }

#endif
