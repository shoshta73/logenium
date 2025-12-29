// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/dlfcn.h>

#ifndef __linux__

// NOLINTNEXTLINE(bugprone-reserved-identifier)
int dlclose(void *__handle) { return 0; }

#endif  // __linux__
