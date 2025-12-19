#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(LoadCursorW, returns_null) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(NULL, LoadCursorW(nullptr, nullptr));
#endif
}
