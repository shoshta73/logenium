#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(DefWindowProcW, returns_zero) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(0, DefWindowProcW(nullptr, 0, 0, 0));
#endif
}
