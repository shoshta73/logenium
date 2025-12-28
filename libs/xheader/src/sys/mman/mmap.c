// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/sys/mman.h>

#ifndef __linux__

void *mmap(void *addr, size_t length, int prot, int flags, int fd, int64_t offset) { return MAP_FAILED; }

#endif
