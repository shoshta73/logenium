#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(GetModuleHandleW, returns_null) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(NULL, GetModuleHandleW(nullptr));
#endif
}
