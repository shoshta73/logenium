#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(PostQuitMessage, completes_successfully) {
#ifdef _WIN32
    SUCCEED();
#else
    PostQuitMessage(0);
    SUCCEED();
#endif
}
