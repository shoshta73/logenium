#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(GetWindowLongPtrW, returns_zero) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(0, GetWindowLongPtrW(nullptr, 0));
#endif
}
