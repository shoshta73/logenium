// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/sys/mman.h>

#ifndef __linux__

int memfd_create(const char *name, unsigned int flags) { return -1; }

#endif
