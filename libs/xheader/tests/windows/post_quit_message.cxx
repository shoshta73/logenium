// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(PostQuitMessage, completes_successfully) {
#ifdef _WIN32
    SUCCEED();
#else
    PostQuitMessage(0);
    SUCCEED();
#endif
}
