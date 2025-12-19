#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(GetModuleHandleA, returns_null) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(NULL, GetModuleHandleA(nullptr));
#endif
}
