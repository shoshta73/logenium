// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

HICON LoadIconA(HINSTANCE /* hInstance */, LPCSTR /* lpIconName */) { return NULL; }

#endif
