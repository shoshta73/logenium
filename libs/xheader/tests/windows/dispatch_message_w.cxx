#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(DispatchMessageW, returns_zero) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(0, DispatchMessageW(nullptr));
#endif
}
