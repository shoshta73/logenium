#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(CreateWindowExW, returns_null) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(NULL, CreateWindowExW(0, nullptr, nullptr, 0, 0, 0, 0, 0, nullptr, nullptr, nullptr, nullptr));
#endif
}
