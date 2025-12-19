#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(LoadIconA, returns_null) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(NULL, LoadIconA(nullptr, nullptr));
#endif
}
