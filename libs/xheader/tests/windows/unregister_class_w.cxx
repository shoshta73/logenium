#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(UnregisterClassW, returns_false) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(FALSE, UnregisterClassW(nullptr, nullptr));
#endif
}
