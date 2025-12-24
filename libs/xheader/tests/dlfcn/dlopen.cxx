// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/dlfcn.h>

TEST(dlopen, returns_nullptr) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(NULL, dlopen(NULL, 0));
#endif
}
