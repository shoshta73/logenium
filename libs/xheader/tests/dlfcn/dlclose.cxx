// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/dlfcn.h>

TEST(dlclose, returns_zero) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(0, dlclose(nullptr));
#endif
}
