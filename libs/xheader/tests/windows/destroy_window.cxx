#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(DestroyWindow, returns_false) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(FALSE, DestroyWindow(nullptr));
#endif
}
