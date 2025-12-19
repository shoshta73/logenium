#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(LoadCursorA, returns_null) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(NULL, LoadCursorA(nullptr, nullptr));
#endif
}
