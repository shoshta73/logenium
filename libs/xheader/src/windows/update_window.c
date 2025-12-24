// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/windows.h>

#ifndef _WIN32

BOOL UpdateWindow(HWND /* hWnd */) { return FALSE; }

#endif
