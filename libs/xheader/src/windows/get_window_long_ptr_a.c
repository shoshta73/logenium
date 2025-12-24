// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

LONG_PTR GetWindowLongPtrA(HWND hWnd, int nIndex) { return (LONG_PTR)NULL; }

#endif
