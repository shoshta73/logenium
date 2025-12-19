#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(UnregisterClassA, returns_false) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(FALSE, UnregisterClassA(nullptr, nullptr));
#endif
}
