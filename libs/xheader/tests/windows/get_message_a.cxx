#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(GetMessageA, returns_false) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(FALSE, GetMessageA(nullptr, nullptr, 0, 0));
#endif
}
