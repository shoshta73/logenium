#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(IsDebuggerPresent, returns_false) {
#ifdef _WIN32
    SUCCEED();
#else
    ASSERT_EQ(FALSE, IsDebuggerPresent());
#endif
}
