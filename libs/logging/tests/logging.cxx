// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <logging/logging.hxx>

#include <source_location>
#include <string>

#include <gtest/gtest.h>

// Helper to capture stdout output
class StdoutCapture {
  public:
    StdoutCapture() { testing::internal::CaptureStdout(); }
    ~StdoutCapture() {
        // Ensure we always get the captured output, even if test fails
        try {
            testing::internal::GetCapturedStdout();
        } catch (...) {
        }
    }
    std::string get() { return testing::internal::GetCapturedStdout(); }
};

// Test basic string logging
TEST(Logging, basic_string) {
    StdoutCapture capture;
    logging::log("Hello, world!");
    std::string output = capture.get();
    EXPECT_EQ(output, "Hello, world!\n");
}

// Test logging with single argument
TEST(Logging, single_argument_string) {
    StdoutCapture capture;
    logging::log("Hello, {}!", "world");
    std::string output = capture.get();
    EXPECT_EQ(output, "Hello, world!\n");
}

// Test logging with integer
TEST(Logging, single_argument_int) {
    StdoutCapture capture;
    logging::log("The answer is {}", 42);
    std::string output = capture.get();
    EXPECT_EQ(output, "The answer is 42\n");
}

// Test logging with floating point
TEST(Logging, single_argument_float) {
    StdoutCapture capture;
    logging::log("Pi is approximately {}", 3.14);
    std::string output = capture.get();
    EXPECT_EQ(output, "Pi is approximately 3.14\n");
}

// Test logging with boolean
TEST(Logging, single_argument_bool) {
    StdoutCapture capture;
    logging::log("Is true: {}", true);
    std::string output = capture.get();
    EXPECT_EQ(output, "Is true: true\n");
}

// Test logging with multiple arguments
TEST(Logging, multiple_arguments) {
    StdoutCapture capture;
    logging::log("Values: {}, {}, {}", 1, 2.5, "three");
    std::string output = capture.get();
    EXPECT_EQ(output, "Values: 1, 2.5, three\n");
}

// Test logging with multiple mixed types
TEST(Logging, multiple_mixed_types) {
    StdoutCapture capture;
    logging::log("Mixed: {} {} {} {}", 42, "test", 3.14, true);
    std::string output = capture.get();
    EXPECT_EQ(output, "Mixed: 42 test 3.14 true\n");
}

// Test logging with std::string
TEST(Logging, std_string) {
    StdoutCapture capture;
    std::string msg = "dynamic string";
    logging::log("Message: {}", msg);
    std::string output = capture.get();
    EXPECT_EQ(output, "Message: dynamic string\n");
}

// Test logging with const char*
TEST(Logging, const_char_ptr) {
    StdoutCapture capture;
    const char *msg = "C-style string";
    logging::log("Message: {}", msg);
    std::string output = capture.get();
    EXPECT_EQ(output, "Message: C-style string\n");
}

// Test logging with custom formatting
TEST(Logging, custom_formatting) {
    StdoutCapture capture;
    logging::log("Hex: {:#x}, Oct: {:#o}", 255, 64);
    std::string output = capture.get();
    EXPECT_EQ(output, "Hex: 0xff, Oct: 0100\n");
}

// Test logging with width and alignment
TEST(Logging, width_and_alignment) {
    StdoutCapture capture;
    logging::log("Right: {:>10}, Left: {:<10}, Center: {:^10}", "R", "L", "C");
    std::string output = capture.get();
    EXPECT_EQ(output, "Right:          R, Left: L         , Center:     C     \n");
}

// Test logging with precision
TEST(Logging, precision) {
    StdoutCapture capture;
    logging::log("Pi: {:.2f}", 3.14159);
    std::string output = capture.get();
    EXPECT_EQ(output, "Pi: 3.14\n");
}

// Test that source_location parameter is accepted (even if not used)
TEST(Logging, source_location_parameter) {
    StdoutCapture capture;
    auto loc = std::source_location::current();
    logging::log("With location: {}", "test");  // loc is default parameter
    std::string output = capture.get();
    EXPECT_EQ(output, "With location: test\n");
}

// Test logging with rvalue references (perfect forwarding)
TEST(Logging, rvalue_references) {
    StdoutCapture capture;
    logging::log("Rvalue: {}", std::string("temporary"));
    std::string output = capture.get();
    EXPECT_EQ(output, "Rvalue: temporary\n");
}

// Test logging with pointer
TEST(Logging, pointer) {
    StdoutCapture capture;
    int value = 42;
    int *ptr = &value;
    logging::log("Pointer: {}", static_cast<void *>(ptr));
    std::string output = capture.get();
    // Just verify it contains "Pointer: 0x" (exact address is platform-dependent)
    EXPECT_TRUE(output.find("Pointer: 0x") != std::string::npos);
}

// Test logging with character
TEST(Logging, character) {
    StdoutCapture capture;
    logging::log("Char: {}", 'A');
    std::string output = capture.get();
    EXPECT_EQ(output, "Char: A\n");
}

// Test logging with zero arguments (just format string)
TEST(Logging, no_arguments) {
    StdoutCapture capture;
    logging::log("No placeholders here!");
    std::string output = capture.get();
    EXPECT_EQ(output, "No placeholders here!\n");
}

// Test logging with empty string
TEST(Logging, empty_string) {
    StdoutCapture capture;
    logging::log("");
    std::string output = capture.get();
    EXPECT_EQ(output, "\n");
}

// Test logging compiles with various numeric types
TEST(Logging, numeric_types) {
    StdoutCapture capture;
    short s = 1;
    int i = 2;
    long l = 3;
    long long ll = 4;
    unsigned u = 5;
    float f = 6.0f;
    double d = 7.0;
    logging::log("{} {} {} {} {} {} {}", s, i, l, ll, u, f, d);
    std::string output = capture.get();
    EXPECT_EQ(output, "1 2 3 4 5 6 7\n");
}

// Test that all logging levels work
TEST(Logging, all_levels) {
    {
        StdoutCapture capture;
        logging::trace3("Trace3 message");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::trace2("Trace2 message");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::trace1("Trace1 message");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::trace("Trace message");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::debug("Debug message");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::info("Info message");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::warn("Warn message");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::error("Error message");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::fatal("Fatal message");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
}

#if __LOGENIUM_LOGGING_USE_COLOR_LOGS__

// Test that colored logging produces output (smoke test)
// Note: We can't easily verify the actual color codes, but we can ensure it doesn't crash
TEST(Logging, colored_output_smoke_test) {
    {
        StdoutCapture capture;
        logging::trace("Colored trace");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
        // With colors, output should contain ANSI escape sequences
        // which make it longer than the plain text
    }
    {
        StdoutCapture capture;
        logging::debug("Colored debug");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::info("Colored info");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::warn("Colored warning");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::error("Colored error");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
    {
        StdoutCapture capture;
        logging::fatal("Colored fatal");
        std::string output = capture.get();
        EXPECT_FALSE(output.empty());
    }
}

#endif  // __LOGENIUM_LOGGING_USE_COLOR_LOGS__
