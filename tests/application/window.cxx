// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/linux/X11/window.hxx"

#include <bit>
#include <cstdint>
#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting.hxx>

#include "logenium/platform/linux/wayland/window.hxx"
#include "logenium/platform/linux/window.hxx"
#include "logenium/platform/windows/window.hxx"
#include "logenium/window.hxx"

using namespace logenium;
using namespace corelib;

namespace {

class MockWindow : public Window {
  public:
    explicit MockWindow(WindowKind kind) : Window(kind) {}

    static bool classof(const Window *win) {
        auto k = win->GetKind();
        return k >= WindowKind::WK_Windows && k <= WindowKind::WK_LinuxX11;
    }
};

class MockWindowsWindow : public Window {
  public:
    MockWindowsWindow() : Window(WindowKind::WK_Windows) {}

    static bool classof(const Window *win) { return win->GetKind() == WindowKind::WK_Windows; }
};

class MockLinuxWindow : public Window {
  public:
    explicit MockLinuxWindow(WindowKind kind) : Window(kind) {}

    static bool classof(const Window *win) {
        auto k = win->GetKind();
        return k >= WindowKind::WK_Linux && k <= WindowKind::WK_LinuxX11;
    }
};

class MockX11Window : public Window {
  public:
    MockX11Window() : Window(WindowKind::WK_LinuxX11) {}

    static bool classof(const Window *win) { return win->GetKind() == WindowKind::WK_LinuxX11; }
};

class MockWaylandWindow : public Window {
  public:
    MockWaylandWindow() : Window(WindowKind::WK_LinuxWayland) {}

    static bool classof(const Window *win) { return win->GetKind() == WindowKind::WK_LinuxWayland; }
};

}  // namespace

TEST(WindowTest, WindowKindEnumValues) {
    EXPECT_EQ(static_cast<std::uint8_t>(Window::WindowKind::WK_Windows), 0);
    EXPECT_EQ(static_cast<std::uint8_t>(Window::WindowKind::WK_Linux), 1);
    EXPECT_EQ(static_cast<std::uint8_t>(Window::WindowKind::WK_LinuxWayland), 2);
    EXPECT_EQ(static_cast<std::uint8_t>(Window::WindowKind::WK_LinuxX11), 3);
}

TEST(WindowTest, GetKindWindowsWindow) {
    MockWindowsWindow win;
    EXPECT_EQ(win.GetKind(), Window::WindowKind::WK_Windows);
}

TEST(WindowTest, GetKindLinuxWindow) {
    MockLinuxWindow win(Window::WindowKind::WK_Linux);
    EXPECT_EQ(win.GetKind(), Window::WindowKind::WK_Linux);
}

TEST(WindowTest, GetKindX11Window) {
    MockX11Window win;
    EXPECT_EQ(win.GetKind(), Window::WindowKind::WK_LinuxX11);
}

TEST(WindowTest, GetKindWaylandWindow) {
    MockWaylandWindow win;
    EXPECT_EQ(win.GetKind(), Window::WindowKind::WK_LinuxWayland);
}

TEST(WindowTest, ClassofWindowsWindow) {
    MockWindowsWindow win_win;
    const Window *win = &win_win;

    EXPECT_TRUE(MockWindowsWindow::classof(win));
    EXPECT_FALSE(MockLinuxWindow::classof(win));
    EXPECT_FALSE(MockX11Window::classof(win));
    EXPECT_FALSE(MockWaylandWindow::classof(win));
}

TEST(WindowTest, ClassofLinuxWindow) {
    MockLinuxWindow linux_win(Window::WindowKind::WK_Linux);
    const Window *win = &linux_win;

    EXPECT_FALSE(MockWindowsWindow::classof(win));
    EXPECT_TRUE(MockLinuxWindow::classof(win));
    EXPECT_FALSE(MockX11Window::classof(win));
    EXPECT_FALSE(MockWaylandWindow::classof(win));
}

TEST(WindowTest, ClassofX11Window) {
    MockX11Window x11_win;
    const Window *win = &x11_win;

    EXPECT_FALSE(MockWindowsWindow::classof(win));
    EXPECT_TRUE(MockLinuxWindow::classof(win));
    EXPECT_TRUE(MockX11Window::classof(win));
    EXPECT_FALSE(MockWaylandWindow::classof(win));
}

TEST(WindowTest, ClassofWaylandWindow) {
    MockWaylandWindow wayland_win;
    const Window *win = &wayland_win;

    EXPECT_FALSE(MockWindowsWindow::classof(win));
    EXPECT_TRUE(MockLinuxWindow::classof(win));
    EXPECT_FALSE(MockX11Window::classof(win));
    EXPECT_TRUE(MockWaylandWindow::classof(win));
}

TEST(WindowTest, IsaWindowsWindow) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    EXPECT_TRUE(isa<MockWindowsWindow>(win));
    EXPECT_FALSE(isa<MockLinuxWindow>(win));
    EXPECT_FALSE(isa<MockX11Window>(win));
    EXPECT_FALSE(isa<MockWaylandWindow>(win));
}

TEST(WindowTest, IsaLinuxWindow) {
    MockLinuxWindow linux_win(Window::WindowKind::WK_Linux);
    Window *win = &linux_win;

    EXPECT_FALSE(isa<MockWindowsWindow>(win));
    EXPECT_TRUE(isa<MockLinuxWindow>(win));
    EXPECT_FALSE(isa<MockX11Window>(win));
    EXPECT_FALSE(isa<MockWaylandWindow>(win));
}

TEST(WindowTest, IsaX11Window) {
    MockX11Window x11_win;
    Window *win = &x11_win;

    EXPECT_FALSE(isa<MockWindowsWindow>(win));
    EXPECT_TRUE(isa<MockLinuxWindow>(win));
    EXPECT_TRUE(isa<MockX11Window>(win));
    EXPECT_FALSE(isa<MockWaylandWindow>(win));
}

TEST(WindowTest, IsaWaylandWindow) {
    MockWaylandWindow wayland_win;
    Window *win = &wayland_win;

    EXPECT_FALSE(isa<MockWindowsWindow>(win));
    EXPECT_TRUE(isa<MockLinuxWindow>(win));
    EXPECT_FALSE(isa<MockX11Window>(win));
    EXPECT_TRUE(isa<MockWaylandWindow>(win));
}

TEST(WindowTest, IsaVariadic) {
    MockX11Window x11_win;
    Window *win = &x11_win;

    EXPECT_TRUE((isa<MockLinuxWindow, MockX11Window>(win)));
    EXPECT_TRUE((isa<MockX11Window, MockWaylandWindow>(win)));
    EXPECT_FALSE((isa<MockWindowsWindow, MockWaylandWindow>(win)));
}

TEST(WindowTest, CastWindowsWindow) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    MockWindowsWindow *casted = cast<MockWindowsWindow>(win);
    EXPECT_EQ(casted, &win_win);
}

TEST(WindowTest, CastX11Window) {
    MockX11Window x11_win;
    Window *win = &x11_win;

    MockX11Window *casted = cast<MockX11Window>(win);
    EXPECT_EQ(casted, &x11_win);

    MockLinuxWindow *linux_casted = cast<MockLinuxWindow>(win);
    EXPECT_EQ(linux_casted, static_cast<Window *>(&x11_win));
}

TEST(WindowTest, CastConstPreservation) {
    const MockWindowsWindow win_win;
    const Window *win = &win_win;

    const MockWindowsWindow *casted = cast<MockWindowsWindow>(win);
    EXPECT_EQ(casted, &win_win);
}

TEST(WindowTest, DynCastSuccessful) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    MockWindowsWindow *casted = dyn_cast<MockWindowsWindow>(win);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_win);
}

TEST(WindowTest, DynCastFailed) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    MockLinuxWindow *casted = dyn_cast<MockLinuxWindow>(win);
    EXPECT_EQ(casted, nullptr);
}

TEST(WindowTest, DynCastX11ToLinux) {
    MockX11Window x11_win;
    Window *win = &x11_win;

    MockLinuxWindow *casted = dyn_cast<MockLinuxWindow>(win);
    EXPECT_NE(casted, nullptr);
}

TEST(WindowTest, DynCastOrNullSuccessful) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    MockWindowsWindow *casted = dyn_cast_or_null<MockWindowsWindow>(win);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_win);
}

TEST(WindowTest, DynCastOrNullFailed) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    MockLinuxWindow *casted = dyn_cast_or_null<MockLinuxWindow>(win);
    EXPECT_EQ(casted, nullptr);
}

TEST(WindowTest, DynCastOrNullWithNullptr) {
    Window *win = nullptr;

    MockWindowsWindow *casted = dyn_cast_or_null<MockWindowsWindow>(win);
    EXPECT_EQ(casted, nullptr);
}

TEST(WindowTest, CastOrNullSuccessful) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    MockWindowsWindow *casted = cast_or_null<MockWindowsWindow>(win);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_win);
}

TEST(WindowTest, CastOrNullWithNullptr) {
    Window *win = nullptr;

    MockWindowsWindow *casted = cast_or_null<MockWindowsWindow>(win);
    EXPECT_EQ(casted, nullptr);
}

TEST(WindowTest, IsaAndPresentTrue) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    EXPECT_TRUE(isa_and_present<MockWindowsWindow>(win));
}

TEST(WindowTest, IsaAndPresentFalse) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    EXPECT_FALSE(isa_and_present<MockLinuxWindow>(win));
}

TEST(WindowTest, IsaAndPresentWithNullptr) {
    Window *win = nullptr;

    EXPECT_FALSE(isa_and_present<MockWindowsWindow>(win));
}

TEST(WindowTest, CastIfPresentSuccessful) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    MockWindowsWindow *casted = cast_if_present<MockWindowsWindow>(win);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_win);
}

TEST(WindowTest, CastIfPresentWithNullptr) {
    Window *win = nullptr;

    MockWindowsWindow *casted = cast_if_present<MockWindowsWindow>(win);
    EXPECT_EQ(casted, nullptr);
}

TEST(WindowTest, DynCastIfPresentSuccessful) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    MockWindowsWindow *casted = dyn_cast_if_present<MockWindowsWindow>(win);
    EXPECT_NE(casted, nullptr);
    EXPECT_EQ(casted, &win_win);
}

TEST(WindowTest, DynCastIfPresentFailed) {
    MockWindowsWindow win_win;
    Window *win = &win_win;

    MockLinuxWindow *casted = dyn_cast_if_present<MockLinuxWindow>(win);
    EXPECT_EQ(casted, nullptr);
}

TEST(WindowTest, DynCastIfPresentWithNullptr) {
    Window *win = nullptr;

    MockWindowsWindow *casted = dyn_cast_if_present<MockWindowsWindow>(win);
    EXPECT_EQ(casted, nullptr);
}

TEST(WindowTest, NativeHandleDefaultConstructor) {
    Window::NativeHandle handle;
    void *ptr = handle;
    EXPECT_EQ(ptr, nullptr);
}

TEST(WindowTest, NativeHandleVoidPtrConstructor) {
    void *test_ptr = reinterpret_cast<void *>(0x12345678);
    Window::NativeHandle handle(test_ptr);
    void *ptr = handle;
    EXPECT_EQ(ptr, test_ptr);
}

TEST(WindowTest, NativeHandleNullptrConstructor) {
    Window::NativeHandle handle(nullptr);
    void *ptr = handle;
    EXPECT_EQ(ptr, nullptr);
}

TEST(WindowTest, NativeHandleXcbWindowConstructor) {
    xcb_window_t xcb_win = 0x87654321;
    Window::NativeHandle handle(xcb_win);
    xcb_window_t converted = handle;
    EXPECT_EQ(converted, xcb_win);
}

TEST(WindowTest, NativeHandleXcbWindowRoundTrip) {
    xcb_window_t original = 0xDEADBEEF;
    Window::NativeHandle handle(original);
    void *intermediate = handle;
    xcb_window_t result = handle;
    EXPECT_EQ(result, original);
    EXPECT_EQ(intermediate, std::bit_cast<void *>(static_cast<std::uintptr_t>(original)));
}

TEST(WindowTest, GetNativeHandleAccess) {
    MockWindowsWindow win;
    Window::NativeHandle &handle = win.GetNativeHandle();
    void *ptr = handle;
    EXPECT_EQ(ptr, nullptr);
}

TEST(WindowTest, PredicateIsaPred) {
    MockWindowsWindow win_win;
    MockLinuxWindow linux_win(Window::WindowKind::WK_Linux);

    auto pred = IsaPred<MockWindowsWindow>;
    EXPECT_TRUE(pred(&win_win));
    EXPECT_FALSE(pred(&linux_win));
}

TEST(WindowTest, PredicateIsaAndPresentPred) {
    MockWindowsWindow win_win;
    Window *win_ptr = &win_win;
    Window *null_ptr = nullptr;

    auto pred = IsaAndPresentPred<MockWindowsWindow>;
    EXPECT_TRUE(pred(win_ptr));
    EXPECT_FALSE(pred(null_ptr));
}

TEST(WindowTest, PredicateCastTo) {
    MockWindowsWindow win_win;
    Window *win_ptr = &win_win;

    auto caster = CastTo<MockWindowsWindow>;
    MockWindowsWindow *result = caster(win_ptr);
    EXPECT_EQ(result, &win_win);
}

TEST(WindowTest, PredicateDynCastTo) {
    MockWindowsWindow win_win;
    Window *win_ptr = &win_win;

    auto caster = DynCastTo<MockWindowsWindow>;
    MockWindowsWindow *result = caster(win_ptr);
    EXPECT_EQ(result, &win_win);

    auto failed_caster = DynCastTo<MockLinuxWindow>;
    MockLinuxWindow *failed_result = failed_caster(win_ptr);
    EXPECT_EQ(failed_result, nullptr);
}
