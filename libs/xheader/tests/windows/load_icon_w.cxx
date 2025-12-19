#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(LoadIconW, returns_null) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(NULL, LoadIconW(nullptr, nullptr));
#endif
}
