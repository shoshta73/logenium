#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(DefWindowProcA, returns_zero) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(0, DefWindowProcA(nullptr, 0, 0, 0));
#endif
}
