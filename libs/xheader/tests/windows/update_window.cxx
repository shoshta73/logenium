#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(UpdateWindow, returns_false) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(FALSE, UpdateWindow(nullptr));
#endif
}
