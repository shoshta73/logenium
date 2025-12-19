#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(GetWindowLongPtrA, returns_zero) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(0, GetWindowLongPtrA(nullptr, 0));
#endif
}
