// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/dlfcn.h>

#ifndef __linux__

#include <stddef.h>

void *dlopen(const char *__file, int __mode) { return NULL; }

#endif  // __linux__
