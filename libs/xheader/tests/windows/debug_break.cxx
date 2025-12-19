#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(DebugBreak, returns_void) {
#ifdef _WIN32
    SUCCEED();
#else
    SUCCEED();  // retuns void
#endif
}
