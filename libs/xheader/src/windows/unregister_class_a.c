// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/windows.h>

#ifndef _WIN32

BOOL UnregisterClassA(LPCSTR /* lpClassName */, HINSTANCE /* hInstance */) { return FALSE; }

#endif
