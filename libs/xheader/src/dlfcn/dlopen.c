// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/dlfcn.h>

#ifndef __linux__

// NOLINTNEXTLINE(bugprone-reserved-identifier)
void *dlopen(const char *__file, int __mode) { return nullptr; }

#endif  // __linux__
