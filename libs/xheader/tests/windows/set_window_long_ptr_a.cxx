#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(SetWindowLongPtrA, returns_zero) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(0, SetWindowLongPtrA(nullptr, 0, 0));
#endif
}
