#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(TranslateMessage, returns_false) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(FALSE, TranslateMessage(nullptr));
#endif
}
