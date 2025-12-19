#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(GetMessageW, returns_false) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(FALSE, GetMessageW(nullptr, nullptr, 0, 0));
#endif
}
