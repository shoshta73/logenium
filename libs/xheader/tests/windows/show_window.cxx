#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(ShowWindow, returns_false) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(FALSE, ShowWindow(nullptr, 0));
#endif
}
